#include <gtest/gtest.h>

#include <chrono>

#include "ioniq5_ecan/safety_supervisor.hpp"
#include "ioniq5_ecan/recovery_retry.hpp"

namespace {

TEST(RecoveryStandby, UnarmedStartupRestoresEmptyListener) {
  using namespace ioniq5_ecan;
  EXPECT_EQ(recovery_standby_param(7173U, false, true, false, false), 7173U);
  EXPECT_EQ(recovery_standby_param(7173U, false, false, false, false), 0U);
}

TEST(RecoveryStandby, FaultExplicitOffAndShutdownDoNotReinstallListener) {
  using namespace ioniq5_ecan;
  EXPECT_EQ(recovery_standby_param(7173U, false, true, true, false), 0U);
  EXPECT_EQ(recovery_standby_param(7173U, false, true, false, true), 0U);
  EXPECT_EQ(recovery_standby_param(7173U, false, true, true, true), 0U);
}

TEST(RecoveryStandby, HealthyCommandGapRetainsProfile) {
  using namespace ioniq5_ecan;
  EXPECT_EQ(recovery_standby_param(7173U, true, true, false, false), 7173U);
}

struct FixtureData {
  ioniq5_ecan::TimePoint now{ioniq5_ecan::SteadyClock::now()};
  ioniq5_ecan::VehicleStateData vehicle;
  ioniq5_ecan::PandaHealth panda;
  ioniq5_ecan::CommandSample command;

  FixtureData() {
    vehicle.valid = true;
    panda.connected = true;
    panda.controls_allowed = true;
    panda.harness_status = 1;
    panda.safety_mode = 28;
    panda.safety_param = 3073;
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

TEST(SafetySupervisor, BrakeDoesNotDisengageLateral) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);

  data.vehicle.brake_pressed = true;
  const SafetyDecision decision =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(decision.state, ControlState::Active);
  EXPECT_TRUE(decision.lateral_allowed);
  EXPECT_FALSE(decision.longitudinal_allowed);
  EXPECT_TRUE(supervisor.arm_requested());
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

TEST(SafetySupervisor, NoCommandWaitsWithoutStockTrafficOwnership) {
  using namespace ioniq5_ecan;
  FixtureData data;
  data.command.received_at = TimePoint{};
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  const auto waiting = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(waiting.state, ControlState::Passive);
  EXPECT_FALSE(waiting.use_vehicle_safety_mode);
  EXPECT_FALSE(waiting.lateral_allowed);
  EXPECT_FALSE(waiting.longitudinal_allowed);
  EXPECT_FALSE(supervisor.arm_requested());

  data.command.received_at = data.now;
  const auto fresh = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(fresh.state, ControlState::Passive);
  EXPECT_FALSE(fresh.lateral_allowed);  // A received value alone never selects a channel.
}

TEST(SafetySupervisor, IdleCommandTimeoutReturnsStockButActiveTimeoutRemainsLatched) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Armed);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  const auto waiting = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(waiting.state, ControlState::Passive);
  EXPECT_FALSE(waiting.use_vehicle_safety_mode);

  data.command.received_at = data.now;
  arm_lateral(supervisor, data);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
  data.command.received_at = data.now;
  ++data.vehicle.lane_keep_button_events;
  const auto still_faulted = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(still_faulted.state, ControlState::Fault);
  EXPECT_FALSE(still_faulted.lateral_allowed);
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

TEST(SafetySupervisor, IgnoresOnlyFaultsFromDisabledNonEcanControllers) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  constexpr uint32_t ignored_non_ecan_faults = (1U << 3U) | (1U << 4U);
  data.panda.faults = ignored_non_ecan_faults;
  data.panda.ignored_faults = ignored_non_ecan_faults;
  data.panda.fault_status = 1U;
  arm_lateral(supervisor, data);

  data.panda.faults |= 1U << 2U;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, RequiresVerifiedEcanHarnessOrientation) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  data.panda.harness_status = 2U;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, LaneAndSetButtonsSelectLateralOnlyOrCombinedControl) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
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
  EXPECT_TRUE(both.lateral_armed);
  EXPECT_TRUE(both.longitudinal_armed);

  ++data.vehicle.lane_keep_button_events;
  SafetyDecision lateral_again =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(lateral_again.lateral_allowed);
  EXPECT_FALSE(lateral_again.longitudinal_allowed);
  EXPECT_TRUE(lateral_again.lateral_armed);
  EXPECT_FALSE(lateral_again.longitudinal_armed);

  ++data.vehicle.lane_keep_button_events;
  const SafetyDecision off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(off.state, ControlState::Armed);
  EXPECT_FALSE(off.lateral_armed);
  EXPECT_FALSE(off.longitudinal_armed);

  ++data.vehicle.set_button_events;
  both = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(both.lateral_allowed);
  EXPECT_TRUE(both.longitudinal_allowed);

  ++data.vehicle.set_button_events;
  const SafetyDecision combined_off =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(combined_off.state, ControlState::Armed);
  EXPECT_FALSE(combined_off.lateral_armed);
  EXPECT_FALSE(combined_off.longitudinal_armed);
}

TEST(SafetySupervisor, SetToggleDoesNotDependOnPandaHealthArrivalOrder) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.set_button_events;
  ASSERT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Active);

  data.panda.controls_allowed = false;
  SafetyDecision paused = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(paused.state, ControlState::Armed);
  EXPECT_TRUE(paused.lateral_armed);
  EXPECT_TRUE(paused.longitudinal_armed);

  ++data.vehicle.set_button_events;
  SafetyDecision button_edge = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(button_edge.state, ControlState::Armed);
  EXPECT_FALSE(button_edge.lateral_armed);
  EXPECT_FALSE(button_edge.longitudinal_armed);

  data.panda.controls_allowed = true;
  SafetyDecision still_off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_FALSE(still_off.lateral_allowed);
  EXPECT_FALSE(still_off.longitudinal_allowed);

  ++data.vehicle.set_button_events;
  SafetyDecision resumed = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(resumed.lateral_allowed);
  EXPECT_TRUE(resumed.longitudinal_allowed);
}

TEST(SafetySupervisor, LdaToggleDoesNotDependOnPandaHealthArrivalOrder) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);

  data.panda.controls_allowed = false;
  ++data.vehicle.lane_keep_button_events;
  const auto off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(off.state, ControlState::Armed);
  EXPECT_FALSE(off.lateral_armed);
  EXPECT_FALSE(off.lateral_allowed);
  data.panda.controls_allowed = true;
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);

  ++data.vehicle.lane_keep_button_events;
  EXPECT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
}

TEST(SafetySupervisor, ButtonOffWithKnownInFlightRejectionStaysOff) {
  using namespace ioniq5_ecan;
  for (const uint32_t address : {0x12AU, 0x1A0U, 0x160U}) {
    FixtureData data;
    SafetyConfig config;
    config.allow_actuation = true;
    SafetySupervisor supervisor(config);
    arm_lateral(supervisor, data);
    ++data.vehicle.lane_keep_button_events;
    data.panda.controls_allowed = false;
    ++data.panda.safety_tx_blocked;
    data.panda.last_rejected_address = address;
    const auto off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
    EXPECT_EQ(off.state, ControlState::Armed) << address;
    EXPECT_FALSE(off.lateral_allowed);
    EXPECT_FALSE(off.longitudinal_allowed);
    EXPECT_TRUE(supervisor.arm_requested());
  }
}

TEST(SafetySupervisor, ButtonOffDoesNotMaskUnknownRejectionOrHardwareFault) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  SafetySupervisor supervisor(config);
  arm_lateral(supervisor, data);
  ++data.vehicle.lane_keep_button_events;
  data.panda.controls_allowed = false;
  ++data.panda.safety_tx_blocked;
  data.panda.last_rejected_address = 0x999U;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state,
            ControlState::Fault);

  SafetySupervisor other(config);
  FixtureData hardware_data;
  arm_lateral(other, hardware_data);
  ++hardware_data.vehicle.lane_keep_button_events;
  hardware_data.panda.faults = 1U;
  hardware_data.panda.last_rejected_address = 0x12AU;
  EXPECT_EQ(other.update(hardware_data.now, hardware_data.vehicle, hardware_data.panda,
                         hardware_data.command).state,
            ControlState::Fault);
}

TEST(SafetySupervisor, BrakeLatchesOffLongitudinalAndKeepsLateral) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));

  ++data.vehicle.set_button_events;
  SafetyDecision both = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  ASSERT_TRUE(both.lateral_allowed);
  ASSERT_TRUE(both.longitudinal_allowed);

  data.vehicle.brake_pressed = true;
  SafetyDecision braking = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(braking.state, ControlState::Active);
  EXPECT_TRUE(braking.lateral_allowed);
  EXPECT_FALSE(braking.longitudinal_allowed);
  EXPECT_TRUE(braking.lateral_armed);
  EXPECT_FALSE(braking.longitudinal_armed);
  EXPECT_TRUE(supervisor.arm_requested());

  data.vehicle.brake_pressed = false;
  SafetyDecision released = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(released.lateral_allowed);
  EXPECT_FALSE(released.longitudinal_allowed);

  ++data.vehicle.set_button_events;
  SafetyDecision reenabled = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(reenabled.lateral_allowed);
  EXPECT_TRUE(reenabled.longitudinal_allowed);
}

TEST(SafetySupervisor, AccFaultOnlyBlocksLongitudinalChannel) {
  using namespace ioniq5_ecan;
  FixtureData data;
  data.vehicle.acc_fault = true;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.lane_keep_button_events;
  const SafetyDecision lateral =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(lateral.state, ControlState::Active);
  EXPECT_TRUE(lateral.lateral_allowed);

  ++data.vehicle.set_button_events;
  const SafetyDecision combined_attempt =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(combined_attempt.state, ControlState::Active);
  EXPECT_TRUE(combined_attempt.lateral_allowed);
  EXPECT_FALSE(combined_attempt.longitudinal_allowed);
  EXPECT_TRUE(supervisor.arm_requested());
}

TEST(SafetySupervisor, LongitudinalTxRejectionKeepsLateralActive) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.set_button_events;
  ASSERT_TRUE(
    supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);

  data.panda.last_rejected_address = 0x1A0U;
  ++data.panda.safety_tx_blocked;
  const SafetyDecision rejected =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(rejected.state, ControlState::Active);
  EXPECT_TRUE(rejected.lateral_allowed);
  EXPECT_FALSE(rejected.longitudinal_allowed);
  EXPECT_TRUE(supervisor.arm_requested());
}

TEST(SafetySupervisor, BrakeAndDelayedRejectedAddressKeepLateralActive) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.required_safety_param = 3077;
  data.panda.safety_param = 3077;
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  ++data.vehicle.set_button_events;
  ASSERT_TRUE(
    supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);

  data.vehicle.brake_pressed = true;
  // The last observed address can still be stale when the health counter arrives first.
  data.panda.last_rejected_address = 0x12AU;
  ++data.panda.safety_tx_blocked;
  const SafetyDecision braking =
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(braking.state, ControlState::Active);
  EXPECT_TRUE(braking.lateral_allowed);
  EXPECT_FALSE(braking.longitudinal_allowed);
  EXPECT_TRUE(supervisor.arm_requested());
}

ioniq5_ecan::SafetyConfig session_config(FixtureData& data) {
  using namespace ioniq5_ecan;
  SafetyConfig config;
  config.allow_actuation = true;
  config.allow_longitudinal = true;
  config.resume_on_command_return = true;
  config.required_safety_param = 7173;
  data.panda.safety_param = 7173;
  data.panda.command_session_active = true;
  data.panda.ignition_on = true;
  data.panda.command_session_ready = true;
  data.panda.lateral_selected = true;
  return config;
}

TEST(SafetySupervisor, CommandGapKeepsButtonIntentAndResumesWithFreshCommandWhileMoving) {
  using namespace ioniq5_ecan;
  FixtureData data;
  auto config = session_config(data);
  SafetySupervisor supervisor(config);
  ASSERT_TRUE(supervisor.request_arm(true));
  data.vehicle.speed_mps = 15.0;
  ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  auto waiting = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(waiting.state, ControlState::Passive);
  EXPECT_TRUE(waiting.waiting_for_command);
  EXPECT_TRUE(waiting.lateral_armed);
  EXPECT_TRUE(supervisor.arm_requested());
  EXPECT_FALSE(waiting.lateral_allowed);
  EXPECT_FALSE(waiting.heartbeat_engaged);
  EXPECT_FALSE(waiting.use_vehicle_safety_mode);
  data.panda.safety_mode = 19;
  data.panda.controls_allowed = false;
  data.command.received_at = data.now;
  auto takeover = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(takeover.state, ControlState::Armed);
  EXPECT_FALSE(takeover.lateral_allowed);
  data.panda.safety_mode = 28;
  data.panda.controls_allowed = true;
  auto resumed = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_EQ(resumed.state, ControlState::Active);
  EXPECT_TRUE(resumed.lateral_allowed);
}

TEST(SafetySupervisor, PhysicalOffDuringCommandGapPreventsAutomaticActuation) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetySupervisor supervisor(session_config(data));
  ASSERT_TRUE(supervisor.request_arm(true));
  ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  supervisor.update(data.now, data.vehicle, data.panda, data.command);
  data.panda.safety_mode = 19;
  data.panda.controls_allowed = false;
  data.panda.lateral_selected = false;
  auto off = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_FALSE(off.lateral_armed);
  data.command.received_at = data.now;
  data.panda.safety_mode = 28;
  auto fresh = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_FALSE(fresh.lateral_allowed);
  EXPECT_FALSE(fresh.longitudinal_allowed);
}

TEST(SafetySupervisor, CommandSessionKeepsBrakeLatchDespiteStaleFirmwareSelection) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetySupervisor supervisor(session_config(data));
  data.panda.longitudinal_selected = true;
  ASSERT_TRUE(supervisor.request_arm(true));
  ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);
  data.vehicle.brake_pressed = true;
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);
  data.vehicle.brake_pressed = false;
  // A delayed health sample still says combined. Brake release alone must not resume.
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);
  data.now += std::chrono::milliseconds(101);
  data.panda.updated_at = data.now;
  supervisor.update(data.now, data.vehicle, data.panda, data.command);
  data.command.received_at = data.now;
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);
  ++data.vehicle.set_button_events;
  EXPECT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).longitudinal_allowed);
}

TEST(SafetySupervisor, HardwareOrCriticalCanFaultDuringGapCannotAutoResume) {
  using namespace ioniq5_ecan;
  for (bool firmware_fault : {false, true}) {
    FixtureData data;
    SafetySupervisor supervisor(session_config(data));
    ASSERT_TRUE(supervisor.request_arm(true));
    ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
    data.now += std::chrono::milliseconds(101);
    data.panda.updated_at = data.now;
    supervisor.update(data.now, data.vehicle, data.panda, data.command);
    data.vehicle.valid = firmware_fault;
    data.panda.command_session_blocked = firmware_fault;
    EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state, ControlState::Fault);
    data.vehicle.valid = true;
    data.panda.command_session_blocked = false;
    data.command.received_at = data.now;
    EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state, ControlState::Fault);
    EXPECT_FALSE(supervisor.arm_requested());
  }
}

TEST(SafetySupervisor, CommandSessionCannotForcePandaPermissionOrSkipFreshCan) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetySupervisor supervisor(session_config(data));
  ASSERT_TRUE(supervisor.request_arm(true));
  data.panda.command_session_ready = false;
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
  data.panda.command_session_ready = true;
  data.panda.controls_allowed = false;
  EXPECT_FALSE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
}

TEST(SafetySupervisor, InitiallyAbsentCommandKeepsStockAndTracksPhysicalSelection) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetySupervisor supervisor(session_config(data));
  ASSERT_TRUE(supervisor.request_arm(true));
  data.command.received_at = TimePoint{};
  data.panda.safety_mode = 19;
  data.panda.controls_allowed = false;
  auto waiting = supervisor.update(data.now, data.vehicle, data.panda, data.command);
  EXPECT_TRUE(waiting.waiting_for_command);
  EXPECT_TRUE(waiting.lateral_armed);
  EXPECT_FALSE(waiting.use_vehicle_safety_mode);
}

TEST(SafetySupervisor, CommandSessionDoesNotRecoverModeDriftWhileWaiting) {
  using namespace ioniq5_ecan;
  FixtureData data;
  SafetySupervisor supervisor(session_config(data));
  ASSERT_TRUE(supervisor.request_arm(true));
  data.command.received_at = TimePoint{};
  data.panda.safety_mode = 19;
  ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).waiting_for_command);
  data.panda.safety_mode = 3;
  EXPECT_EQ(supervisor.update(data.now, data.vehicle, data.panda, data.command).state, ControlState::Fault);
}

TEST(SafetySupervisor, IgnitionOrFirmwareFreshnessLossCancelsMovingResume) {
  using namespace ioniq5_ecan;
  for (bool ignition_loss : {false, true}) {
    FixtureData data;
    SafetySupervisor supervisor(session_config(data));
    ASSERT_TRUE(supervisor.request_arm(true));
    ASSERT_TRUE(supervisor.update(data.now, data.vehicle, data.panda, data.command).lateral_allowed);
    data.panda.ignition_on = !ignition_loss;
    data.panda.command_session_ready = ignition_loss;
    auto failed = supervisor.update(data.now, data.vehicle, data.panda, data.command);
    EXPECT_EQ(failed.state, ControlState::Fault);
    EXPECT_FALSE(failed.lateral_armed);
    EXPECT_FALSE(supervisor.arm_requested());
  }
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
