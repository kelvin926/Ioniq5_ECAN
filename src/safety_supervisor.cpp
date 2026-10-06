#include "ioniq5_ecan/safety_supervisor.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace ioniq5_ecan {
namespace {
// Match Carrotpilot's soft-disable recovery window, without extending it on each update.
constexpr auto kSoftDisableTime = std::chrono::seconds(3);

bool is_engaged(ControlState state) {
  return state == ControlState::Active || state == ControlState::SoftDisabling;
}
}  // namespace

SafetySupervisor::SafetySupervisor(SafetyConfig config) : config_(config) {
  if (!std::isfinite(config_.max_active_speed_mps) ||
      !std::isfinite(config_.max_abs_steering_angle_deg) || config_.max_active_speed_mps < 0.0 ||
      config_.max_abs_steering_angle_deg < 0.0 || config_.command_timeout.count() <= 0 ||
      config_.panda_timeout.count() <= 0) {
    throw std::invalid_argument("invalid safety supervisor configuration");
  }
}

bool SafetySupervisor::request_arm(bool arm) {
  if (arm && !config_.allow_actuation) {
    transition(ControlState::Passive, "actuation disabled by YAML");
    return false;
  }
  if (arm && !arm_requested_) {
    lateral_enabled_ = false;
    longitudinal_enabled_ = false;
    longitudinal_latched_off_ = false;
  }
  arm_requested_ = arm;
  if (!arm) {
    lateral_enabled_ = false;
    longitudinal_enabled_ = false;
    if (state_ != ControlState::Disconnected) {
      transition(ControlState::Passive, "operator disarmed");
    }
  }
  return true;
}

SafetyDecision SafetySupervisor::update(TimePoint now, const VehicleStateData& vehicle,
                                        const PandaHealth& panda, const CommandSample& command) {
  waiting_for_command_ = false;
  const auto decision = [&](bool lateral_allowed, bool longitudinal_allowed,
                            bool use_vehicle_safety_mode, bool heartbeat_engaged) {
    const auto remaining = state_ == ControlState::SoftDisabling && now < soft_disable_deadline_
                             ? std::chrono::duration_cast<std::chrono::milliseconds>(
                                 soft_disable_deadline_ - now).count()
                             : 0;
    return SafetyDecision{state_,
                          lateral_allowed,
                          longitudinal_allowed,
                          lateral_enabled_,
                          config_.allow_longitudinal && longitudinal_enabled_,
                          use_vehicle_safety_mode,
                          heartbeat_engaged,
                          reason_,
                          static_cast<uint32_t>(remaining),
                          waiting_for_command_};
  };

  const bool panda_fresh = panda.connected && panda.updated_at.time_since_epoch().count() != 0 &&
                           now >= panda.updated_at &&
                           now - panda.updated_at <= config_.panda_timeout;
  if (!panda_fresh) {
    arm_requested_ = false;
    lateral_enabled_ = false;
    longitudinal_enabled_ = false;
    transition(ControlState::Disconnected, "Panda disconnected or health timeout");
    return decision(false, false, false, false);
  }

  if (!config_.allow_actuation) {
    arm_requested_ = false;
    lateral_enabled_ = false;
    longitudinal_enabled_ = false;
    transition(ControlState::Passive, "actuation disabled by YAML");
    return decision(false, false, false, false);
  }

  const bool lane_keep_button_event =
    vehicle.lane_keep_button_events != last_lane_keep_button_events_;
  if (lane_keep_button_event) {
    last_lane_keep_button_events_ = vehicle.lane_keep_button_events;
  }
  const bool set_button_event = vehicle.set_button_events != last_set_button_events_;
  if (set_button_event) {
    last_set_button_events_ = vehicle.set_button_events;
  }

  bool selected_mode_turned_off = false;
  if (config_.resume_on_command_return && arm_requested_) {
    if (set_button_event && !vehicle.brake_pressed && !vehicle.acc_fault) {
      longitudinal_latched_off_ = false;
    }
    selected_mode_turned_off = (lateral_enabled_ || longitudinal_enabled_) &&
                              !panda.lateral_selected && !panda.longitudinal_selected;
    lateral_enabled_ = panda.lateral_selected;
    longitudinal_enabled_ = panda.longitudinal_selected && !longitudinal_latched_off_;
  } else if (arm_requested_ && lane_keep_button_event) {
    const bool lateral_only_selected = lateral_enabled_ && !longitudinal_enabled_;
    const bool turn_off =
      config_.lateral_button_toggle && lateral_only_selected;
    selected_mode_turned_off = turn_off;
    lateral_enabled_ = !turn_off;
    longitudinal_enabled_ = false;
  }
  if (!config_.resume_on_command_return && arm_requested_ && set_button_event) {
    const bool combined_selected = lateral_enabled_ && longitudinal_enabled_;
    const bool turn_off =
      config_.longitudinal_button_toggle && combined_selected;
    selected_mode_turned_off = turn_off;
    lateral_enabled_ = !turn_off;
    longitudinal_enabled_ = !turn_off;
  }
  const bool cancel_event = vehicle.cancel_button_events != last_cancel_button_events_;
  last_cancel_button_events_ = vehicle.cancel_button_events;

  bool longitudinal_disengaged_by_brake = false;
  bool longitudinal_disengaged_by_acc_fault = false;
  if (vehicle.brake_pressed) {
    if (config_.longitudinal_disengage_on_brake) longitudinal_latched_off_ = true;
    if (config_.lateral_disengage_on_brake) {
      lateral_enabled_ = false;
    }
    if (config_.longitudinal_disengage_on_brake && longitudinal_enabled_) {
      longitudinal_enabled_ = false;
      longitudinal_disengaged_by_brake = true;
    }
  }
  if (vehicle.acc_fault && config_.allow_longitudinal && longitudinal_enabled_) {
    longitudinal_latched_off_ = true;
    longitudinal_enabled_ = false;
    longitudinal_disengaged_by_acc_fault = true;
  }

  const uint32_t effective_faults = panda.faults & ~panda.ignored_faults;
  const bool ignored_fault_is_only_fault =
    panda.faults != 0U && effective_faults == 0U && panda.ignored_faults != 0U;
  bool longitudinal_tx_rejected = false;
  if (panda.command_session_blocked || panda.heartbeat_lost || panda.safety_rx_checks_invalid || panda.bus_off ||
      effective_faults != 0U || (panda.fault_status != 0U && !ignored_fault_is_only_fault)) {
    fault("Panda safety or CAN health fault");
  } else if (is_engaged(state_) && panda.safety_tx_blocked > last_tx_blocked_) {
    // Firmware may process OFF before the host sees the same button edge and reject an
    // in-flight actuator frame. This is still OFF, not a fault acknowledgement or TX bypass.
    const bool known_actuator_address = panda.last_rejected_address == 0x12AU ||
                                       panda.last_rejected_address == 0x1A0U ||
                                       panda.last_rejected_address == 0x160U;
    const bool button_off_rejection = selected_mode_turned_off && !lateral_enabled_ &&
                                     !longitudinal_enabled_ && known_actuator_address;
    const bool longitudinal_address =
      panda.last_rejected_address == 0x1A0U || panda.last_rejected_address == 0x160U;
    // A returned rejected frame can arrive one USB read after the health counter. When brake and
    // the counter rise in the same update, the one in-flight SCC/FCA rejection belongs to the
    // longitudinal channel even if its address has not been observed yet.
    const bool pending_brake_rejection = longitudinal_disengaged_by_brake;
    if (button_off_rejection) {
      // Both channels remain disabled; a later physical button edge is required to select ON.
    } else if ((longitudinal_address || pending_brake_rejection) && lateral_enabled_) {
      longitudinal_enabled_ = false;
      longitudinal_tx_rejected = true;
      longitudinal_latched_off_ = true;
    } else {
      fault("Panda rejected an active lateral or unknown control frame");
    }
  }
  last_tx_blocked_ = panda.safety_tx_blocked;

  if (!arm_requested_) {
    if (state_ != ControlState::Fault) {
      transition(ControlState::Passive, "waiting for arm request");
    }
    return decision(false, false, false, false);
  }

  if (panda.harness_status != config_.required_harness_status) {
    fault("Panda harness orientation does not expose ECAN on logical bus 0");
    return decision(false, false, false, false);
  }
  if (config_.resume_on_command_return && !panda.ignition_on) {
    fault("vehicle ignition lost during command session");
    return decision(false, false, false, false);
  }

  if (state_ == ControlState::Fault) {
    return decision(false, false, false, false);
  }

  if (!vehicle.valid) {
    fault("critical vehicle state timeout");
    return decision(false, false, false, false);
  }
  if (config_.max_active_speed_mps > 0.0 && vehicle.speed_mps > config_.max_active_speed_mps) {
    fault("configured active speed limit exceeded");
    return decision(false, false, false, false);
  }
  if (config_.max_abs_steering_angle_deg > 0.0 &&
      std::abs(vehicle.steering_angle_deg) >= config_.max_abs_steering_angle_deg) {
    fault("steering angle safety limit exceeded");
    return decision(false, false, false, false);
  }
  if (config_.disengage_on_cancel && cancel_event) {
    disarm("cancel button pressed");
    return decision(false, false, false, false);
  }

  const bool configured_mode = panda.safety_mode == config_.required_safety_mode &&
                               panda.safety_param == config_.required_safety_param;
  const bool standby_mode = config_.resume_on_command_return &&
                            panda.command_session_active && panda.safety_mode == 19U &&
                            panda.safety_param == config_.required_safety_param;
  if (!configured_mode && (!standby_mode || is_engaged(state_))) {
    if (is_engaged(state_) || config_.resume_on_command_return) {
      fault("Panda safety mode changed while active");
    } else {
      transition(ControlState::Armed, "waiting for configured Panda safety mode");
    }
    return decision(false, false, false, false);
  }

  const bool command_fresh = command.received_at.time_since_epoch().count() != 0 &&
                             now >= command.received_at &&
                             now - command.received_at <= config_.command_timeout;
  if (!command_fresh) {
    if (config_.resume_on_command_return && state_ != ControlState::SoftDisabling &&
        !vehicle.eps_fault) {
      waiting_for_command_ = true;
      transition(ControlState::Passive, "waiting for fresh command; preserve buttons and restore stock");
    } else if (is_engaged(state_)) {
      fault("control command timeout");
    } else {
      // No active control was in progress. Stop owning stock ADAS traffic while waiting
      // for the publisher; the ROS subscription stays alive in the node.
      disarm("waiting for fresh command; restore stock communication");
    }
    return decision(false, false, false, false);
  }
  if (!command.valid) {
    fault("control command contains a non-finite value");
    return decision(false, false, false, false);
  }
  if (!command.enable) {
    transition(ControlState::Armed, "command deadman is false");
    return decision(false, false, true, false);
  }

  if (standby_mode) {
    transition(ControlState::Armed, "fresh command; waiting for verified ECU takeover");
    return decision(false, false, false, false);
  }
  if (config_.resume_on_command_return && !panda.command_session_ready) {
    if (is_engaged(state_)) {
      fault("Panda command-session CAN freshness lost");
      return decision(false, false, false, false);
    }
    transition(ControlState::Armed, "waiting for validated command-session CAN");
    return decision(false, false, true, false);
  }

  const bool lateral_requested = lateral_enabled_;
  const bool longitudinal_requested = config_.allow_longitudinal && longitudinal_enabled_;
  if (!lateral_requested && !longitudinal_requested) {
    transition(ControlState::Armed, "waiting for LDA lateral-only or SET combined arm");
    return decision(false, false, true, false);
  }

  if (vehicle.eps_fault && state_ != ControlState::SoftDisabling) {
    if (state_ != ControlState::Active) {
      transition(ControlState::Armed, "waiting for temporary EPS fault to clear");
      return decision(false, false, true, false);
    }
    soft_disable_deadline_ = now + kSoftDisableTime;
    transition(ControlState::SoftDisabling, "temporary EPS fault; lateral paused");
  }
  if (state_ == ControlState::SoftDisabling) {
    // Even a late fault-clear sample cannot resume a pause whose window has expired.
    if (now >= soft_disable_deadline_) {
      fault("temporary EPS fault recovery timed out");
      return decision(false, false, false, false);
    }
    if (vehicle.eps_fault || !panda.controls_allowed) {
      const bool longitudinal = longitudinal_requested && panda.controls_allowed &&
                                !(config_.longitudinal_override_on_gas && vehicle.gas_pressed);
      transition(ControlState::SoftDisabling,
                 vehicle.eps_fault ? "temporary EPS fault; lateral paused"
                                   : "EPS fault cleared; waiting for Panda controls_allowed");
      // Retain channel selection, CAN ownership and heartbeat. Never force Panda permission.
      return decision(false, longitudinal, true, true);
    }
  }
  if (!panda.controls_allowed) {
    transition(ControlState::Armed, "Panda controls_allowed is false");
    return decision(false, false, true, false);
  }

  const bool lateral = lateral_requested;
  const bool longitudinal =
    longitudinal_requested && !(config_.longitudinal_override_on_gas && vehicle.gas_pressed);
  if (!lateral && !longitudinal) {
    transition(ControlState::Armed, "longitudinal arm paused by gas override");
    return decision(false, false, true, false);
  }

  transition(
    ControlState::Active,
    lateral && longitudinal
      ? "active: lateral + longitudinal"
      : (longitudinal_disengaged_by_brake
           ? "active: lateral; longitudinal disengaged by brake"
           : (longitudinal_disengaged_by_acc_fault
                ? "active: lateral; longitudinal disengaged by ACC fault"
                : (longitudinal_tx_rejected ? "active: lateral; longitudinal frame rejected"
                                            : "active: lateral"))));
  return decision(lateral, longitudinal, true, true);
}

void SafetySupervisor::transition(ControlState next, std::string reason) {
  if (next != ControlState::SoftDisabling) soft_disable_deadline_ = TimePoint{};
  state_ = next;
  reason_ = std::move(reason);
}

void SafetySupervisor::disarm(std::string reason) {
  arm_requested_ = false;
  lateral_enabled_ = false;
  longitudinal_enabled_ = false;
  transition(ControlState::Passive, std::move(reason));
}

void SafetySupervisor::fault(std::string reason) {
  arm_requested_ = false;
  lateral_enabled_ = false;
  longitudinal_enabled_ = false;
  transition(ControlState::Fault, std::move(reason));
}

ControlState SafetySupervisor::state() const { return state_; }
bool SafetySupervisor::arm_requested() const { return arm_requested_; }
const std::string& SafetySupervisor::reason() const { return reason_; }

}  // namespace ioniq5_ecan
