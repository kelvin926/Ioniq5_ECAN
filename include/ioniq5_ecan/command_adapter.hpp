#pragma once

#include <string>

#include "ioniq5_ecan/types.hpp"

namespace ioniq5_ecan {

enum class LateralInputMode {
  SteeringRateDegPerSec,
  SteeringRateRadPerSec,
  Curvature,
  DirectTorque,
};

LateralInputMode lateral_mode_from_string(const std::string& value);

struct CommandAdapterConfig {
  LateralInputMode lateral_mode{LateralInputMode::SteeringRateDegPerSec};
  double lateral_scale{1.0};
  double lateral_offset{0.0};
  double acceleration_scale{1.0};
  double acceleration_offset{0.0};
  double wheelbase_m{2.97};
  double steering_ratio{14.26};
  double kp_angle{2.0};
  double ki_angle{0.0};
  double kd_rate{0.2};
  double feedforward_rate{0.0};
  double integral_limit{50.0};
  double max_target_angle_deg{80.0};
  double max_target_rate_deg_s{120.0};
  int max_torque{100};
  int torque_rate_up{1};
  int torque_rate_down{2};
  double driver_torque_start{100.0};
  double driver_torque_zero{250.0};
  double accel_min_mps2{-1.0};
  double accel_max_mps2{1.0};
  double jerk_limit_mps3{2.0};
};

class CommandAdapter {
 public:
  explicit CommandAdapter(CommandAdapterConfig config = {});

  ControlOutput update(const CommandSample& command, const VehicleState& vehicle, double dt_seconds,
                       bool active, bool longitudinal_allowed);
  void reset(const VehicleState& vehicle);
  const CommandAdapterConfig& config() const;

 private:
  int apply_torque_slew(int desired) const;
  int apply_driver_override(int desired, double driver_torque) const;

  CommandAdapterConfig config_;
  double target_angle_deg_{0.0};
  double target_rate_deg_s_{0.0};
  double angle_integral_{0.0};
  int last_torque_{0};
  bool initialized_{false};
};

}  // namespace ioniq5_ecan
