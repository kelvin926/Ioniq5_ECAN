#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "ioniq5_ecan/command_adapter.hpp"

namespace {

TEST(CommandAdapter, DirectTorqueUsesSoftwareSlewAndLimits) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::DirectTorque;
  config.max_torque = 5;
  config.torque_rate_up = 2;
  config.torque_rate_down = 3;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = 100.0;
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 2);
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 4);
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 5);
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, false, false).steering_torque, 0);
}

TEST(CommandAdapter, ConvertsRadiansAndClampsAcceleration) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::SteeringRateRadPerSec;
  config.accel_min_mps2 = -0.5;
  config.accel_max_mps2 = 0.5;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = 1.0;
  command.acceleration_mps2 = 2.0;
  const ControlOutput output = adapter.update(command, vehicle, 0.01, true, true);
  EXPECT_TRUE(output.lateral_active);
  EXPECT_TRUE(output.longitudinal_active);
  EXPECT_DOUBLE_EQ(output.acceleration_mps2, 0.5);
}

TEST(CommandAdapter, AppliesRateLimitBeforeIntegratingTargetAngle) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::SteeringRateDegPerSec;
  config.max_target_rate_deg_s = 10.0;
  config.steer_actuator_delay_s = 0.0;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = 1000.0;

  (void)adapter.update(command, vehicle, 0.05, true, false);
  EXPECT_DOUBLE_EQ(adapter.target_rate_deg_s(), 10.0);
  EXPECT_DOUBLE_EQ(adapter.target_angle_deg(), 0.5);
}

TEST(CommandAdapter, SlewsCurvatureTargetInsteadOfJumpingToIt) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::Curvature;
  config.max_target_rate_deg_s = 10.0;
  config.steer_actuator_delay_s = 0.0;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = 1.0;

  (void)adapter.update(command, vehicle, 0.01, true, false);
  EXPECT_DOUBLE_EQ(adapter.target_rate_deg_s(), 10.0);
  EXPECT_NEAR(adapter.target_angle_deg(), 0.1, 1e-12);
}

TEST(CommandAdapter, UsesCarrotIoniq5Defaults) {
  using namespace ioniq5_ecan;
  const CommandAdapterConfig config;
  EXPECT_DOUBLE_EQ(config.steer_actuator_delay_s, 0.1);
  EXPECT_DOUBLE_EQ(config.torque_kp, 1.0);
  EXPECT_DOUBLE_EQ(config.torque_ki, 0.1);
  EXPECT_DOUBLE_EQ(config.torque_kf, 1.0);
  EXPECT_DOUBLE_EQ(config.lat_accel_factor, 3.172929);
  EXPECT_DOUBLE_EQ(config.friction, 0.096019);
  EXPECT_EQ(config.max_torque, 270);
  EXPECT_EQ(config.torque_rate_up, 2);
  EXPECT_EQ(config.torque_rate_down, 3);
  EXPECT_DOUBLE_EQ(config.driver_torque_allowance, 250.0);
}

TEST(CommandAdapter, AppliesCarrotDirectionalDriverTorqueLimit) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::DirectTorque;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  vehicle.driver_torque = -400.0;
  CommandSample command;
  command.lateral = 270.0;

  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 0);
  command.lateral = -270.0;
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, -2);
}

TEST(CommandAdapter, UsesCarrotHighAngleRequestPattern) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::DirectTorque;
  CommandAdapter adapter(config);
  VehicleStateData vehicle;
  vehicle.steering_angle_deg = 90.0;
  CommandSample command;

  for (int frame = 0; frame < 89; ++frame) {
    EXPECT_TRUE(adapter.update(command, vehicle, 0.01, true, false).steering_request) << frame;
  }
  EXPECT_FALSE(adapter.update(command, vehicle, 0.01, true, false).steering_request);
  EXPECT_FALSE(adapter.update(command, vehicle, 0.01, true, false).steering_request);
  EXPECT_TRUE(adapter.update(command, vehicle, 0.01, true, false).steering_request);
}

TEST(CommandAdapter, RejectsParametersAbovePandaLimits) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.max_torque = 271;
  EXPECT_THROW((void)CommandAdapter{config}, std::invalid_argument);
  config.max_torque = 100;
  config.accel_min_mps2 = -3.6;
  EXPECT_THROW((void)CommandAdapter{config}, std::invalid_argument);
}

TEST(CommandAdapter, RejectsNonFiniteInput) {
  using namespace ioniq5_ecan;
  CommandAdapter adapter;
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(adapter.update(command, vehicle, 0.01, true, false), std::invalid_argument);
}

TEST(CommandSequence, AcceptsWraparoundAndRejectsDuplicates) {
  using ioniq5_ecan::sequence_is_newer;
  EXPECT_TRUE(sequence_is_newer(1U, 0U));
  EXPECT_TRUE(sequence_is_newer(0U, 100U));
  EXPECT_TRUE(sequence_is_newer(1U, 0xFFFFFFFFU));
  EXPECT_FALSE(sequence_is_newer(10U, 10U));
  EXPECT_FALSE(sequence_is_newer(9U, 10U));
}

}  // namespace
