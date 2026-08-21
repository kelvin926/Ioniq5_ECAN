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
    button_enabled_ = false;
  }
  arm_requested_ = arm;
  if (!arm) {
    button_enabled_ = false;
    if (state_ != ControlState::Disconnected) {
      transition(ControlState::Passive, "operator disarmed");
    }
  }
  return true;
}

SafetyDecision SafetySupervisor::update(TimePoint now, const VehicleState& vehicle,
                                        const PandaHealth& panda, const CommandSample& command) {
  const bool panda_fresh = panda.connected && panda.updated_at.time_since_epoch().count() != 0 &&
                           now >= panda.updated_at &&
                           now - panda.updated_at <= config_.panda_timeout;
  if (!panda_fresh) {
    arm_requested_ = false;
    button_enabled_ = false;
    transition(ControlState::Disconnected, "Panda disconnected or health timeout");
    return {state_, false, false, false, false, reason_};
  }

  if (!config_.allow_actuation) {
    arm_requested_ = false;
    button_enabled_ = false;
    transition(ControlState::Passive, "actuation disabled by YAML");
    return {state_, false, false, false, false, reason_};
  }

  if (vehicle.set_button_events != last_set_button_events_) {
    last_set_button_events_ = vehicle.set_button_events;
    // SET turns control off only when it was genuinely active. If Panda disabled controls after
    // a brake intervention, one SET release re-enables instead of requiring an off/on double tap.
    button_enabled_ =
      config_.set_button_toggle
        ? !(button_enabled_ && state_ == ControlState::Active && panda.controls_allowed)
        : true;
  }
  const bool cancel_event = vehicle.cancel_button_events != last_cancel_button_events_;
  last_cancel_button_events_ = vehicle.cancel_button_events;

  if (panda.heartbeat_lost || panda.safety_rx_checks_invalid || panda.bus_off ||
      panda.faults != 0U || panda.fault_status != 0U) {
    arm_requested_ = false;
    transition(ControlState::Fault, "Panda safety or CAN health fault");
  } else if (state_ == ControlState::Active && panda.safety_tx_blocked > last_tx_blocked_) {
    arm_requested_ = false;
    transition(ControlState::Fault, "Panda rejected an active control frame");
  }
  last_tx_blocked_ = panda.safety_tx_blocked;

  if (!arm_requested_) {
    if (state_ != ControlState::Fault) {
      transition(ControlState::Passive, "waiting for arm request");
    }
    return {state_, false, false, false, false, reason_};
  }

  if (panda.harness_status == 0U) {
    arm_requested_ = false;
    transition(ControlState::Fault, "Panda harness is not detected");
    return {state_, false, false, false, false, reason_};
  }

  if (state_ == ControlState::Fault) {
    return {state_, false, false, false, false, reason_};
  }

  if (panda.safety_mode != config_.required_safety_mode ||
      panda.safety_param != config_.required_safety_param) {
    if (state_ == ControlState::Active) {
      transition(ControlState::Fault, "Panda safety mode changed while active");
      arm_requested_ = false;
    } else {
      transition(ControlState::Armed, "waiting for configured Panda safety mode");
    }
    return {state_, false, false, false, false, reason_};
  }

  if (!vehicle.valid) {
    transition(ControlState::Fault, "critical vehicle state timeout or EPS fault");
    arm_requested_ = false;
    return {state_, false, false, false, false, reason_};
  }
  if (vehicle.acc_fault && config_.allow_longitudinal) {
    transition(ControlState::Fault, "ACC fault");
    arm_requested_ = false;
    return {state_, false, false, false, false, reason_};
  }
  if (config_.max_active_speed_mps > 0.0 && vehicle.speed_mps > config_.max_active_speed_mps) {
    transition(ControlState::Fault, "configured active speed limit exceeded");
    arm_requested_ = false;
    return {state_, false, false, false, false, reason_};
  }
  if (config_.max_abs_steering_angle_deg > 0.0 &&
      std::abs(vehicle.steering_angle_deg) >= config_.max_abs_steering_angle_deg) {
    transition(ControlState::Fault, "steering angle safety limit exceeded");
    arm_requested_ = false;
    return {state_, false, false, false, false, reason_};
  }
  if (config_.disengage_on_brake && vehicle.brake_pressed) {
    disarm("brake pressed");
    return {state_, false, false, false, false, reason_};
  }
  if (config_.disengage_on_cancel && cancel_event) {
    disarm("cancel button pressed");
    return {state_, false, false, false, false, reason_};
  }

  const bool command_fresh = command.received_at.time_since_epoch().count() != 0 &&
                             now >= command.received_at &&
                             now - command.received_at <= config_.command_timeout;
  if (!command_fresh) {
    if (state_ == ControlState::Active) {
      transition(ControlState::Fault, "control command timeout");
      arm_requested_ = false;
    } else {
      transition(ControlState::Armed, "waiting for fresh command");
    }
    return {state_, false, false, true, false, reason_};
  }
  if (!command.valid) {
    transition(ControlState::Fault, "control command contains a non-finite value");
    arm_requested_ = false;
    return {state_, false, false, false, false, reason_};
  }
  if (!command.enable) {
    transition(ControlState::Armed, "command deadman is false");
    return {state_, false, false, true, false, reason_};
  }
  if (config_.set_button_toggle && !button_enabled_) {
    transition(ControlState::Armed, "SET control toggle is off");
    return {state_, false, false, true, false, reason_};
  }
  if (!config_.set_button_toggle && config_.require_set_resume_button && !button_enabled_) {
    transition(ControlState::Armed, "waiting for SET release");
    return {state_, false, false, true, false, reason_};
  }
  if (!panda.controls_allowed) {
    transition(ControlState::Armed, "Panda controls_allowed is false");
    return {state_, false, false, true, false, reason_};
  }

  transition(ControlState::Active, "active");
  const bool longitudinal =
    config_.allow_longitudinal && !(config_.longitudinal_override_on_gas && vehicle.gas_pressed);
  return {state_, true, longitudinal, true, true, reason_};
}

void SafetySupervisor::transition(ControlState next, std::string reason) {
  state_ = next;
  reason_ = std::move(reason);
}

void SafetySupervisor::disarm(std::string reason) {
  arm_requested_ = false;
  button_enabled_ = false;
  transition(ControlState::Passive, std::move(reason));
}

ControlState SafetySupervisor::state() const { return state_; }
bool SafetySupervisor::arm_requested() const { return arm_requested_; }
const std::string& SafetySupervisor::reason() const { return reason_; }

}  // namespace ioniq5_ecan
