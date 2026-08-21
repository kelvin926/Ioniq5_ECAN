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
    panda.connected = true;
    panda.controls_allowed = true;
    panda.harness_status = 1;
    panda.safety_mode = 28;
    panda.safety_param = 1033;
    panda.updated_at = now;
    command.enable = true;
    command.valid = true;
    command.received_at = now;
  }
};

void arm_lateral(ioniq5_ecan::SafetySupervisor& supervisor, FixtureData& data) {
  ASSERT_TRUE(supervisor.request_arm(true));
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ioniq5_ecan::ControlState::Armed);
  ++data.vehicle.lane_keep_button_events;
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ioniq5_ecan::ControlState::Active);
}

TEST(SafetySupervisor, RequiresArmCommandAndLaneButtonForLateral) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);

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
  arm_lateral(supervisor, data);

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
  arm_lateral(supervisor, data);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, FaultsOnPandaHardwareFault) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);

  data.panda.faults = 1U;
  const SafetyDecision decision =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(decision.state, ControlState::Fault);
  EXPECT_FALSE(decision.lateral_allowed);
  EXPECT_FALSE(decision.longitudinal_allowed);
}

TEST(SafetySupervisor, LaneAndSetButtonsToggleIndependentChannels) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 1037;
  data.panda.safety_param = 1037;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Armed);

  ++data.vehicle.lane_keep_button_events;
  SafetyDecision lateral = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(lateral.lateral_allowed);
  EXPECT_FALSE(lateral.longitudinal_allowed);
  EXPECT_TRUE(lateral.lateral_armed);
  EXPECT_FALSE(lateral.longitudinal_armed);

  ++data.vehicle.set_button_events;
  SafetyDecision both = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(both.lateral_allowed);
  EXPECT_TRUE(both.longitudinal_allowed);

  ++data.vehicle.lane_keep_button_events;
  SafetyDecision longitudinal = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_FALSE(longitudinal.lateral_allowed);
  EXPECT_TRUE(longitudinal.longitudinal_allowed);

  ++data.vehicle.set_button_events;
  const SafetyDecision off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(off.state, ControlState::Armed);
  EXPECT_FALSE(off.lateral_armed);
  EXPECT_FALSE(off.longitudinal_armed);
}

TEST(SafetySupervisor, OneButtonPressReenablesItsChannelAfterPandaDisengagement) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 1037;
  data.panda.safety_param = 1037;
  config.disengage_on_brake = false;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.lane_keep_button_events;
  ++data.vehicle.set_button_events;
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);

  data.panda.controls_allowed = false;
  SafetyDecision paused = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(paused.state, ControlState::Armed);
  EXPECT_TRUE(paused.lateral_armed);
  EXPECT_TRUE(paused.longitudinal_armed);

  ++data.vehicle.lane_keep_button_events;
  SafetyDecision button_edge = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(button_edge.state, ControlState::Armed);
  EXPECT_TRUE(button_edge.lateral_armed);

  data.panda.controls_allowed = true;
  SafetyDecision resumed = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(resumed.lateral_allowed);
  EXPECT_TRUE(resumed.longitudinal_allowed);
}

TEST(SafetySupervisor, AccFaultOnlyBlocksLongitudinalChannel) {
  using namespace ioniq5_ecan;
  FixtureData data;
  data.vehicle.acc_fault = true;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 1037;
  data.panda.safety_param = 1037;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.lane_keep_button_events;
  const SafetyDecision lateral =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(lateral.state, ControlState::Active);
  EXPECT_TRUE(lateral.lateral_allowed);

  ++data.vehicle.set_button_events;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, ZeroDisablesOptionalHostSpeedAndAngleLimits) {
  using namespace ioniq5_ecan;
  FixtureData data;
  data.vehicle.speed_mps = 60.0;
  data.vehicle.steering_angle_deg = 180.0;
  SafetyConfig config;
  config.allow_actuation = true;
  config.max_active_speed_mps = 0.0;
  config.max_abs_steering_angle_deg = 0.0;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);
}

}  // namespace
