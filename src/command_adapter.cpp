#include "ioniq5_ecan/command_adapter.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace ioniq5_ecan {
namespace {
constexpr double kRadiansToDegrees = 57.2957795130823208768;
}

LateralInputMode lateral_mode_from_string(const std::string& value) {
  if (value == "steering_rate_deg_s") return LateralInputMode::SteeringRateDegPerSec;
  if (value == "steering_rate_rad_s") return LateralInputMode::SteeringRateRadPerSec;
  if (value == "curvature_1pm") return LateralInputMode::Curvature;
  if (value == "direct_torque") return LateralInputMode::DirectTorque;
  throw std::invalid_argument("unsupported lateral input mode: " + value);
}

CommandAdapter::CommandAdapter(CommandAdapterConfig config) : config_(config) {
  const bool finite =
    std::isfinite(config_.lateral_scale) && std::isfinite(config_.lateral_offset) &&
    std::isfinite(config_.acceleration_scale) && std::isfinite(config_.acceleration_offset) &&
    std::isfinite(config_.wheelbase_m) && std::isfinite(config_.steering_ratio) &&
    std::isfinite(config_.kp_angle) && std::isfinite(config_.ki_angle) &&
    std::isfinite(config_.kd_rate) && std::isfinite(config_.feedforward_rate) &&
    std::isfinite(config_.integral_limit) && std::isfinite(config_.max_target_angle_deg) &&
    std::isfinite(config_.max_target_rate_deg_s) && std::isfinite(config_.driver_torque_start) &&
    std::isfinite(config_.driver_torque_zero) && std::isfinite(config_.accel_min_mps2) &&
    std::isfinite(config_.accel_max_mps2) && std::isfinite(config_.jerk_limit_mps3);
  if (!finite || config_.wheelbase_m <= 0.0 || config_.steering_ratio <= 0.0 ||
      config_.integral_limit < 0.0 || config_.max_target_angle_deg <= 0.0 ||
      config_.max_target_rate_deg_s <= 0.0 || config_.driver_torque_start < 0.0 ||
      config_.driver_torque_zero <= config_.driver_torque_start || config_.jerk_limit_mps3 <= 0.0) {
    throw std::invalid_argument("invalid or non-finite command adapter configuration");
  }
  if (config_.max_torque < 0 || config_.max_torque > 270) {
    throw std::invalid_argument("max_torque must be within Panda limit [0, 270]");
  }
  if (config_.torque_rate_up < 1 || config_.torque_rate_up > 2 || config_.torque_rate_down < 1 ||
      config_.torque_rate_down > 3) {
    throw std::invalid_argument("software torque rates cannot exceed Panda limits");
  }
  if (config_.accel_min_mps2 < -3.5 || config_.accel_max_mps2 > 2.0 ||
      config_.accel_min_mps2 > config_.accel_max_mps2) {
    throw std::invalid_argument("software acceleration bounds exceed Panda limits");
  }
}

void CommandAdapter::reset(const VehicleState& vehicle) {
  target_angle_deg_ = vehicle.steering_angle_deg;
  target_rate_deg_s_ = 0.0;
  angle_integral_ = 0.0;
  last_torque_ = 0;
  initialized_ = true;
}

ControlOutput CommandAdapter::update(const CommandSample& command, const VehicleState& vehicle,
                                     double dt_seconds, bool active, bool longitudinal_allowed) {
  dt_seconds = std::clamp(dt_seconds, 0.001, 0.05);
  if (!initialized_) reset(vehicle);

  if (!std::isfinite(command.lateral) || !std::isfinite(command.acceleration_mps2)) {
    throw std::invalid_argument("control command contains a non-finite value");
  }

  ControlOutput output;
  if (!active) {
    angle_integral_ = 0.0;
    target_angle_deg_ = vehicle.steering_angle_deg;
    target_rate_deg_s_ = 0.0;
    last_torque_ = apply_torque_slew(0);
    output.steering_torque = last_torque_;
    return output;
  }

  const double lateral = command.lateral * config_.lateral_scale + config_.lateral_offset;
  int desired_torque = 0;
  if (config_.lateral_mode == LateralInputMode::DirectTorque) {
    desired_torque = static_cast<int>(std::lround(lateral));
  } else {
    if (config_.lateral_mode == LateralInputMode::SteeringRateDegPerSec) {
      target_rate_deg_s_ = lateral;
      target_angle_deg_ += target_rate_deg_s_ * dt_seconds;
    } else if (config_.lateral_mode == LateralInputMode::SteeringRateRadPerSec) {
      target_rate_deg_s_ = lateral * kRadiansToDegrees;
      target_angle_deg_ += target_rate_deg_s_ * dt_seconds;
    } else {
      const double new_target =
        std::atan(config_.wheelbase_m * lateral) * config_.steering_ratio * kRadiansToDegrees;
      target_rate_deg_s_ = (new_target - target_angle_deg_) / dt_seconds;
      target_angle_deg_ = new_target;
    }

    target_rate_deg_s_ =
      std::clamp(target_rate_deg_s_, -config_.max_target_rate_deg_s, config_.max_target_rate_deg_s);
    target_angle_deg_ =
      std::clamp(target_angle_deg_, -config_.max_target_angle_deg, config_.max_target_angle_deg);
    const double angle_error = target_angle_deg_ - vehicle.steering_angle_deg;
    angle_integral_ = std::clamp(angle_integral_ + angle_error * dt_seconds,
                                 -config_.integral_limit, config_.integral_limit);
    const double torque = config_.kp_angle * angle_error + config_.ki_angle * angle_integral_ +
                          config_.kd_rate * (target_rate_deg_s_ - vehicle.steering_rate_deg_s) +
                          config_.feedforward_rate * target_rate_deg_s_;
    desired_torque = static_cast<int>(std::lround(torque));
  }

  desired_torque = std::clamp(desired_torque, -config_.max_torque, config_.max_torque);
  desired_torque = apply_driver_override(desired_torque, vehicle.driver_torque);
  last_torque_ = apply_torque_slew(desired_torque);

  output.steering_torque = last_torque_;
  output.lateral_active = true;
  output.longitudinal_active = longitudinal_allowed;
  if (longitudinal_allowed) {
    const double requested =
      command.acceleration_mps2 * config_.acceleration_scale + config_.acceleration_offset;
    output.acceleration_mps2 =
      std::clamp(requested, config_.accel_min_mps2, config_.accel_max_mps2);
    output.stopping = output.acceleration_mps2 < -0.1 && vehicle.speed_mps < 0.3;
  }
  return output;
}

int CommandAdapter::apply_torque_slew(int desired) const {
  int lower = 0;
  int upper = 0;
  if (last_torque_ > 0) {
    lower = last_torque_ - config_.torque_rate_down;
    upper = last_torque_ + config_.torque_rate_up;
  } else if (last_torque_ < 0) {
    lower = last_torque_ - config_.torque_rate_up;
    upper = last_torque_ + config_.torque_rate_down;
  } else {
    lower = -config_.torque_rate_up;
    upper = config_.torque_rate_up;
  }
  return std::clamp(desired, lower, upper);
}

int CommandAdapter::apply_driver_override(int desired, double driver_torque) const {
  const double magnitude = std::abs(driver_torque);
  if (magnitude <= config_.driver_torque_start) return desired;
  if (magnitude >= config_.driver_torque_zero) return 0;
  const double scale = (config_.driver_torque_zero - magnitude) /
                       (config_.driver_torque_zero - config_.driver_torque_start);
  return static_cast<int>(std::lround(static_cast<double>(desired) * scale));
}

const CommandAdapterConfig& CommandAdapter::config() const { return config_; }

}  // namespace ioniq5_ecan
