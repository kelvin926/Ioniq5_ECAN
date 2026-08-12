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
  VehicleState vehicle;
  CommandSample command;
  command.lateral = 100.0;
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 2);
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 4);
  EXPECT_EQ(adapter.update(command, vehicle, 0.01, true, false).steering_torque, 5);
}

TEST(CommandAdapter, ConvertsRadiansAndClampsAcceleration) {
  using namespace ioniq5_ecan;
  CommandAdapterConfig config;
  config.lateral_mode = LateralInputMode::SteeringRateRadPerSec;
  config.accel_min_mps2 = -0.5;
  config.accel_max_mps2 = 0.5;
  CommandAdapter adapter(config);
  VehicleState vehicle;
  CommandSample command;
  command.lateral = 1.0;
  command.acceleration_mps2 = 2.0;
  const ControlOutput output = adapter.update(command, vehicle, 0.01, true, true);
  EXPECT_TRUE(output.lateral_active);
  EXPECT_TRUE(output.longitudinal_active);
  EXPECT_DOUBLE_EQ(output.acceleration_mps2, 0.5);
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
  VehicleState vehicle;
  CommandSample command;
  command.lateral = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(adapter.update(command, vehicle, 0.01, true, false), std::invalid_argument);
}

}  // namespace
