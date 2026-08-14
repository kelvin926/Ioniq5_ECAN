#include "ioniq5_ecan/command_adapter.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace ioniq5_ecan {
namespace {
constexpr double kRadiansToDegrees = 57.2957795130823208768;
constexpr double kDegreesToRadians = 1.0 / kRadiansToDegrees;

double interpolate(double value, const std::array<double, 4>& x, const std::array<double, 4>& y) {
  if (value <= x.front()) return y.front();
  if (value >= x.back()) return y.back();
  for (std::size_t index = 1; index < x.size(); ++index) {
    if (value <= x[index]) {
      const double ratio = (value - x[index - 1]) / (x[index] - x[index - 1]);
      return y[index - 1] + ratio * (y[index] - y[index - 1]);
    }
  }
  return y.back();
}
}  // namespace

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
    std::isfinite(config_.steer_actuator_delay_s) && std::isfinite(config_.torque_kp) &&
    std::isfinite(config_.torque_ki) && std::isfinite(config_.torque_kd) &&
    std::isfinite(config_.torque_kf) && std::isfinite(config_.lat_accel_factor) &&
    std::isfinite(config_.friction) && std::isfinite(config_.friction_threshold) &&
    std::isfinite(config_.steering_angle_deadzone_deg) &&
    std::isfinite(config_.integral_output_limit) &&
    std::isfinite(config_.integrator_freeze_speed_mps) &&
    std::isfinite(config_.max_target_angle_deg) && std::isfinite(config_.max_target_rate_deg_s) &&
    std::isfinite(config_.driver_torque_allowance) &&
    std::isfinite(config_.driver_torque_multiplier) &&
    std::isfinite(config_.driver_torque_factor) &&
    std::isfinite(config_.steer_request_cutoff_angle_deg) &&
    std::isfinite(config_.accel_min_mps2) && std::isfinite(config_.accel_max_mps2) &&
    std::isfinite(config_.jerk_limit_mps3);
  if (!finite || config_.wheelbase_m <= 0.0 || config_.steering_ratio <= 0.0 ||
      config_.steer_actuator_delay_s < 0.0 || config_.torque_kp < 0.0 || config_.torque_ki < 0.0 ||
      config_.torque_kd < 0.0 || config_.torque_kf < 0.0 || config_.lat_accel_factor <= 0.0 ||
      config_.friction < 0.0 || config_.friction_threshold <= 0.0 ||
      config_.steering_angle_deadzone_deg < 0.0 || config_.integral_output_limit < 0.0 ||
      config_.integrator_freeze_speed_mps < 0.0 || config_.max_target_angle_deg <= 0.0 ||
      config_.max_target_rate_deg_s <= 0.0 || config_.driver_torque_allowance < 0.0 ||
      config_.driver_torque_multiplier < 0.0 || config_.driver_torque_factor < 0.0 ||
      config_.steer_request_cutoff_angle_deg < 0.0 || config_.steer_request_valid_frames < 89U ||
      config_.steer_request_cut_frames < 1U || config_.steer_request_cut_frames > 2U ||
      config_.jerk_limit_mps3 <= 0.0) {
    throw std::invalid_argument("invalid or non-finite command adapter configuration");
  }
  for (std::size_t index = 0; index < config_.low_speed_factor_bp_mps.size(); ++index) {
    if (!std::isfinite(config_.low_speed_factor_bp_mps[index]) ||
        !std::isfinite(config_.low_speed_factor_v[index]) ||
        config_.low_speed_factor_v[index] < 0.0 ||
        (index > 0 &&
         config_.low_speed_factor_bp_mps[index] <= config_.low_speed_factor_bp_mps[index - 1])) {
      throw std::invalid_argument("invalid Carrot low-speed factor table");
    }
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
  torque_integral_ = 0.0;
  previous_torque_error_ = 0.0;
  last_torque_ = 0;
  high_angle_frames_ = 0;
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
    torque_integral_ = 0.0;
    previous_torque_error_ = 0.0;
    target_angle_deg_ = vehicle.steering_angle_deg;
    target_rate_deg_s_ = 0.0;
    // Panda requires zero torque immediately whenever controls_allowed is false.
    last_torque_ = 0;
    output.steering_torque = last_torque_;
    output.steering_request = update_steering_request(false, vehicle.steering_angle_deg);
    return output;
  }

  const double lateral = command.lateral * config_.lateral_scale + config_.lateral_offset;
  int desired_torque = 0;
  if (config_.lateral_mode == LateralInputMode::DirectTorque) {
    desired_torque = static_cast<int>(std::lround(lateral));
  } else {
    if (config_.lateral_mode == LateralInputMode::SteeringRateDegPerSec) {
      target_rate_deg_s_ =
        std::clamp(lateral, -config_.max_target_rate_deg_s, config_.max_target_rate_deg_s);
      target_angle_deg_ += target_rate_deg_s_ * dt_seconds;
    } else if (config_.lateral_mode == LateralInputMode::SteeringRateRadPerSec) {
      target_rate_deg_s_ = std::clamp(lateral * kRadiansToDegrees, -config_.max_target_rate_deg_s,
                                      config_.max_target_rate_deg_s);
      target_angle_deg_ += target_rate_deg_s_ * dt_seconds;
    } else {
      const double new_target =
        std::atan(config_.wheelbase_m * lateral) * config_.steering_ratio * kRadiansToDegrees;
      const double requested_rate = (new_target - target_angle_deg_) / dt_seconds;
      target_rate_deg_s_ =
        std::clamp(requested_rate, -config_.max_target_rate_deg_s, config_.max_target_rate_deg_s);
      target_angle_deg_ += target_rate_deg_s_ * dt_seconds;
    }

    target_angle_deg_ =
      std::clamp(target_angle_deg_, -config_.max_target_angle_deg, config_.max_target_angle_deg);

    const double control_target_angle =
      std::clamp(target_angle_deg_ + target_rate_deg_s_ * config_.steer_actuator_delay_s,
                 -config_.max_target_angle_deg, config_.max_target_angle_deg);
    const double desired_curvature =
      std::tan(control_target_angle * kDegreesToRadians / config_.steering_ratio) /
      config_.wheelbase_m;
    const double actual_curvature =
      std::tan(vehicle.steering_angle_deg * kDegreesToRadians / config_.steering_ratio) /
      config_.wheelbase_m;
    const double speed = std::max(vehicle.speed_mps, 0.0);
    const double speed_squared = speed * speed;
    const double low_speed = low_speed_factor(speed);
    const double setpoint = desired_curvature * (speed_squared + low_speed * low_speed);
    const double measurement = actual_curvature * (speed_squared + low_speed * low_speed);
    const double torque_error = (setpoint - measurement) / config_.lat_accel_factor;

    const double desired_lateral_accel = desired_curvature * speed_squared;
    const double actual_lateral_accel = actual_curvature * speed_squared;
    const double curvature_deadzone = std::abs(
      std::tan(config_.steering_angle_deadzone_deg * kDegreesToRadians / config_.steering_ratio) /
      config_.wheelbase_m);
    const double lateral_accel_deadzone = curvature_deadzone * speed_squared;
    const double lateral_accel_error = desired_lateral_accel - actual_lateral_accel;
    const double friction_error =
      std::abs(lateral_accel_error) < lateral_accel_deadzone ? 0.0 : lateral_accel_error;
    const double friction =
      std::clamp(friction_error / config_.friction_threshold, -1.0, 1.0) * config_.friction;
    const double feedforward = desired_lateral_accel / config_.lat_accel_factor + friction;

    const bool freeze_integrator =
      speed < config_.integrator_freeze_speed_mps || std::abs(last_torque_) >= config_.max_torque;
    if (!freeze_integrator && config_.torque_ki > 0.0) {
      const double integral_limit = config_.integral_output_limit / config_.torque_ki;
      torque_integral_ =
        std::clamp(torque_integral_ + torque_error * dt_seconds, -integral_limit, integral_limit);
    }
    const double derivative = (torque_error - previous_torque_error_) / dt_seconds;
    previous_torque_error_ = torque_error;
    const double normalized_torque =
      config_.torque_kp * torque_error + config_.torque_ki * torque_integral_ +
      config_.torque_kd * derivative + config_.torque_kf * feedforward;
    desired_torque =
      static_cast<int>(std::lround(normalized_torque * static_cast<double>(config_.max_torque)));
  }

  desired_torque = std::clamp(desired_torque, -config_.max_torque, config_.max_torque);
  desired_torque = apply_driver_torque_limits(desired_torque, vehicle.driver_torque);
  last_torque_ = apply_torque_slew(desired_torque);

  output.steering_torque = last_torque_;
  output.lateral_active = true;
  output.steering_request = update_steering_request(true, vehicle.steering_angle_deg);
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

int CommandAdapter::apply_driver_torque_limits(int desired, double driver_torque) const {
  const double maximum = static_cast<double>(config_.max_torque);
  const double driver_maximum =
    maximum + (config_.driver_torque_allowance + driver_torque * config_.driver_torque_factor) *
                config_.driver_torque_multiplier;
  const double driver_minimum =
    -maximum + (-config_.driver_torque_allowance + driver_torque * config_.driver_torque_factor) *
                 config_.driver_torque_multiplier;
  const double maximum_allowed = std::max(std::min(maximum, driver_maximum), 0.0);
  const double minimum_allowed = std::min(std::max(-maximum, driver_minimum), 0.0);
  return static_cast<int>(
    std::lround(std::clamp(static_cast<double>(desired), minimum_allowed, maximum_allowed)));
}

bool CommandAdapter::update_steering_request(bool active, double steering_angle_deg) {
  if (!active || config_.steer_request_cutoff_angle_deg == 0.0 ||
      std::abs(steering_angle_deg) < config_.steer_request_cutoff_angle_deg) {
    high_angle_frames_ = 0;
    return active;
  }

  const uint32_t cycle = config_.steer_request_valid_frames + config_.steer_request_cut_frames;
  const uint32_t position = high_angle_frames_ % cycle;
  high_angle_frames_ = (high_angle_frames_ + 1U) % cycle;
  return position < config_.steer_request_valid_frames;
}

double CommandAdapter::low_speed_factor(double speed_mps) const {
  return interpolate(speed_mps, config_.low_speed_factor_bp_mps, config_.low_speed_factor_v);
}

const CommandAdapterConfig& CommandAdapter::config() const { return config_; }
double CommandAdapter::target_angle_deg() const { return target_angle_deg_; }
double CommandAdapter::target_rate_deg_s() const { return target_rate_deg_s_; }

}  // namespace ioniq5_ecan
