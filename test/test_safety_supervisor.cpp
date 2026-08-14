#include <gtest/gtest.h>

#include <chrono>

#include "ioniq5_ecan/safety_supervisor.hpp"

namespace {

struct FixtureData {
  ioniq5_ecan::TimePoint now{ioniq5_ecan::SteadyClock::now()};
  ioniq5_ecan::VehicleState vehicle;
  ioniq5_ecan::PandaHealth panda;
  ioniq5_ecan::CommandSample command;

  FixtureData() {
    vehicle.valid = true;
    vehicle.enable_button_events = 1;
    panda.connected = true;
    panda.controls_allowed = true;
    panda.harness_status = 1;
    panda.safety_mode = 28;
    panda.safety_param = 9;
    panda.updated_at = now;
    command.enable = true;
    command.valid = true;
    command.received_at = now;
  }
};

TEST(SafetySupervisor, RequiresAllIndependentEnableConditions) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);

  data.vehicle.brake_pressed = true;
  const SafetyDecision decision =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(decision.state, ControlState::Passive);
  EXPECT_FALSE(decision.lateral_allowed);
  EXPECT_FALSE(supervisor.arm_requested());
}

TEST(SafetySupervisor, FaultsOnModeDriftAndCanBeExplicitlyCleared) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);

  data.panda.safety_mode = 19;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
  EXPECT_TRUE(supervisor.request_arm(false));
  EXPECT_EQ(supervisor.state(), ControlState::Passive);
}

TEST(SafetySupervisor, FailsClosedOnStaleCommand) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, RequiresAnEnableButtonEventAfterArming) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);

  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Passive);
  ASSERT_TRUE(supervisor.request_arm(true));
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Armed);

  ++data.vehicle.enable_button_events;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);
}

TEST(SafetySupervisor, ZeroDisablesOptionalHostSpeedAndAngleLimits) {
  using namespace ioniq5_ecan;
  FixtureData data;
  data.vehicle.speed_mps = 60.0;
  data.vehicle.steering_angle_deg = 180.0;
  SafetyConfig config;
  config.allow_actuation = true;
  config.require_set_resume_button = false;
  config.max_active_speed_mps = 0.0;
  config.max_abs_steering_angle_deg = 0.0;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);
}

}  // namespace
