#pragma once

#include <array>
#include <cstdint>
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
  double steer_actuator_delay_s{0.1};
  double torque_kp{1.0};
  double torque_ki{0.1};
  double torque_kd{0.0};
  double torque_kf{1.0};
  double lat_accel_factor{3.172929};
  double friction{0.096019};
  double friction_threshold{0.3};
  double steering_angle_deadzone_deg{0.0};
  double integral_output_limit{1.0};
  double integrator_freeze_speed_mps{5.0};
  std::array<double, 4> low_speed_factor_bp_mps{0.0, 10.0, 20.0, 30.0};
  std::array<double, 4> low_speed_factor_v{15.0, 13.0, 10.0, 5.0};
  double max_target_angle_deg{175.0};
  double max_target_rate_deg_s{500.0};
  int max_torque{270};
  int torque_rate_up{2};
  int torque_rate_down{3};
  double driver_torque_allowance{250.0};
  double driver_torque_multiplier{2.0};
  double driver_torque_factor{1.0};
  double steer_request_cutoff_angle_deg{85.0};
  uint32_t steer_request_valid_frames{89};
  uint32_t steer_request_cut_frames{2};
  double accel_min_mps2{-3.5};
  double accel_max_mps2{2.0};
  double jerk_limit_mps3{5.0};
};

class CommandAdapter {
 public:
  explicit CommandAdapter(CommandAdapterConfig config = {});

  ControlOutput update(const CommandSample& command, const VehicleState& vehicle, double dt_seconds,
                       bool active, bool longitudinal_allowed);
  void reset(const VehicleState& vehicle);
  const CommandAdapterConfig& config() const;
  double target_angle_deg() const;
  double target_rate_deg_s() const;

 private:
  int apply_torque_slew(int desired) const;
  int apply_driver_torque_limits(int desired, double driver_torque) const;
  bool update_steering_request(bool active, double steering_angle_deg);
  double low_speed_factor(double speed_mps) const;

  CommandAdapterConfig config_;
  double target_angle_deg_{0.0};
  double target_rate_deg_s_{0.0};
  double torque_integral_{0.0};
  double previous_torque_error_{0.0};
  int last_torque_{0};
  uint32_t high_angle_frames_{0};
  bool initialized_{false};
};

}  // namespace ioniq5_ecan
