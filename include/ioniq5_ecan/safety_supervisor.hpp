#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#include "ioniq5_ecan/types.hpp"

namespace ioniq5_ecan {

struct SafetyConfig {
  bool allow_actuation{false};
  bool allow_longitudinal{false};
  bool set_button_toggle{true};
  bool require_set_resume_button{true};
  bool disengage_on_brake{true};
  bool disengage_on_cancel{true};
  bool longitudinal_override_on_gas{true};
  // Zero disables the optional host-side limit. Panda firmware limits remain active.
  double max_active_speed_mps{0.0};
  double max_abs_steering_angle_deg{0.0};
  uint8_t required_safety_mode{28};
  uint16_t required_safety_param{9};
  std::chrono::milliseconds command_timeout{100};
  std::chrono::milliseconds panda_timeout{250};
};

struct SafetyDecision {
  ControlState state{ControlState::Disconnected};
  bool lateral_allowed{false};
  bool longitudinal_allowed{false};
  bool use_vehicle_safety_mode{false};
  bool heartbeat_engaged{false};
  std::string reason{"not initialized"};
};

class SafetySupervisor {
 public:
  explicit SafetySupervisor(SafetyConfig config = {});

  bool request_arm(bool arm);
  SafetyDecision update(TimePoint now, const VehicleState& vehicle, const PandaHealth& panda,
                        const CommandSample& command);
  ControlState state() const;
  bool arm_requested() const;
  const std::string& reason() const;

 private:
  void transition(ControlState next, std::string reason);
  void disarm(std::string reason);

  SafetyConfig config_;
  ControlState state_{ControlState::Disconnected};
  bool arm_requested_{false};
  bool button_enabled_{false};
  uint64_t last_set_button_events_{0};
  uint64_t last_cancel_button_events_{0};
  uint32_t last_tx_blocked_{0};
  std::string reason_{"waiting for Panda"};
};

}  // namespace ioniq5_ecan
