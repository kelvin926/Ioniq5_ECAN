#include "ioniq5_ecan/safety_supervisor.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace ioniq5_ecan {

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
  const auto decision = [&](bool lateral_allowed, bool longitudinal_allowed,
                            bool use_vehicle_safety_mode, bool heartbeat_engaged) {
    return SafetyDecision{state_,
                          lateral_allowed,
                          longitudinal_allowed,
                          lateral_enabled_,
                          config_.allow_longitudinal && longitudinal_enabled_,
                          use_vehicle_safety_mode,
                          heartbeat_engaged,
                          reason_};
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

  if (arm_requested_ && lane_keep_button_event) {
    const bool lateral_only_selected = lateral_enabled_ && !longitudinal_enabled_;
    const bool turn_off =
      config_.lateral_button_toggle && lateral_only_selected && panda.controls_allowed;
    lateral_enabled_ = !turn_off;
    longitudinal_enabled_ = false;
  }
  if (arm_requested_ && set_button_event) {
    const bool combined_selected = lateral_enabled_ && longitudinal_enabled_;
    const bool turn_off =
      config_.longitudinal_button_toggle && combined_selected && panda.controls_allowed;
    lateral_enabled_ = !turn_off;
    longitudinal_enabled_ = !turn_off;
  }
  const bool cancel_event = vehicle.cancel_button_events != last_cancel_button_events_;
  last_cancel_button_events_ = vehicle.cancel_button_events;

  const uint32_t effective_faults = panda.faults & ~panda.ignored_faults;
  const bool ignored_fault_is_only_fault =
    panda.faults != 0U && effective_faults == 0U && panda.ignored_faults != 0U;
  if (panda.heartbeat_lost || panda.safety_rx_checks_invalid || panda.bus_off ||
      effective_faults != 0U || (panda.fault_status != 0U && !ignored_fault_is_only_fault)) {
    fault("Panda safety or CAN health fault");
  } else if (state_ == ControlState::Active && panda.safety_tx_blocked > last_tx_blocked_) {
    fault("Panda rejected an active control frame");
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

  if (state_ == ControlState::Fault) {
    return decision(false, false, false, false);
  }

  if (panda.safety_mode != config_.required_safety_mode ||
      panda.safety_param != config_.required_safety_param) {
    if (state_ == ControlState::Active) {
      fault("Panda safety mode changed while active");
    } else {
      transition(ControlState::Armed, "waiting for configured Panda safety mode");
    }
    return decision(false, false, false, false);
  }

  if (!vehicle.valid) {
    fault("critical vehicle state timeout or EPS fault");
    return decision(false, false, false, false);
  }
  if (vehicle.acc_fault && config_.allow_longitudinal && longitudinal_enabled_) {
    fault("ACC fault");
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
  if (config_.disengage_on_brake && vehicle.brake_pressed) {
    disarm("brake pressed");
    return decision(false, false, false, false);
  }
  if (config_.disengage_on_cancel && cancel_event) {
    disarm("cancel button pressed");
    return decision(false, false, false, false);
  }

  const bool command_fresh = command.received_at.time_since_epoch().count() != 0 &&
                             now >= command.received_at &&
                             now - command.received_at <= config_.command_timeout;
  if (!command_fresh) {
    if (state_ == ControlState::Active) {
      fault("control command timeout");
    } else {
      transition(ControlState::Armed, "waiting for fresh command");
    }
    return decision(false, false, true, false);
  }
  if (!command.valid) {
    fault("control command contains a non-finite value");
    return decision(false, false, false, false);
  }
  if (!command.enable) {
    transition(ControlState::Armed, "command deadman is false");
    return decision(false, false, true, false);
  }

  const bool lateral_requested = lateral_enabled_;
  const bool longitudinal_requested = config_.allow_longitudinal && longitudinal_enabled_;
  if (!lateral_requested && !longitudinal_requested) {
    transition(ControlState::Armed, "waiting for LDA lateral-only or SET combined arm");
    return decision(false, false, true, false);
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

  transition(ControlState::Active,
             lateral && longitudinal ? "active: lateral + longitudinal" : "active: lateral");
  return decision(lateral, longitudinal, true, true);
}

void SafetySupervisor::transition(ControlState next, std::string reason) {
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
