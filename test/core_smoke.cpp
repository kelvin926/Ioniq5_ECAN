#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "ioniq5_ecan/bit_codec.hpp"
#include "ioniq5_ecan/command_adapter.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
#include "ioniq5_ecan/recovery_retry.hpp"
#include "ioniq5_ecan/safety_supervisor.hpp"
#include "ioniq5_ecan/vehicle_state_parser.hpp"

namespace {

void require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

template <std::size_t N>
void expect_payload(const ioniq5_ecan::CanFrame& frame, const std::array<uint8_t, N>& expected) {
  require(frame.size == N, "payload length mismatch");
  for (std::size_t i = 0; i < N; ++i) {
    require(frame.data[i] == expected[i], "payload byte mismatch");
  }
}

struct SteeringRecoveryFixture {
  ioniq5_ecan::TimePoint now{ioniq5_ecan::SteadyClock::now()};
  ioniq5_ecan::VehicleStateData vehicle;
  ioniq5_ecan::PandaHealth panda;
  ioniq5_ecan::CommandSample command;
  ioniq5_ecan::SafetySupervisor supervisor{[] {
    ioniq5_ecan::SafetyConfig config;
    config.allow_actuation = true;
    config.allow_longitudinal = true;
    config.required_safety_param = 3077;
    return config;
  }()};

  SteeringRecoveryFixture() {
    vehicle.valid = true;
    panda.connected = panda.controls_allowed = true;
    panda.harness_status = 1;
    panda.safety_mode = 28;
    panda.safety_param = 3077;
    command.enable = command.valid = true;
    require(supervisor.request_arm(true), "could not arm recovery fixture");
    (void)update();
    ++vehicle.set_button_events;
    require(update().state == ioniq5_ecan::ControlState::Active,
            "could not activate recovery fixture");
  }

  ioniq5_ecan::SafetyDecision update(std::chrono::milliseconds elapsed = {}) {
    now += elapsed;
    panda.updated_at = command.received_at = now;
    return supervisor.update(now, vehicle, panda, command);
  }
};

void test_temporary_steering_recovery() {
  using namespace ioniq5_ecan;
  using namespace std::chrono_literals;
  {
    SteeringRecoveryFixture data;
    CommandAdapter adapter;
    data.command.lateral = 100.0;
    data.command.acceleration_mps2 = 0.7;
    (void)adapter.update(data.command, data.vehicle, 0.01, true, true);
    data.vehicle.steering_angle_deg = 12.0;
    data.vehicle.eps_fault = true;
    SafetyDecision paused = data.update();
    require(paused.state == ControlState::SoftDisabling && !paused.lateral_allowed &&
              paused.longitudinal_allowed && paused.lateral_armed && paused.longitudinal_armed &&
              paused.use_vehicle_safety_mode && paused.heartbeat_engaged &&
              paused.soft_disable_remaining_ms == 3000U && data.supervisor.arm_requested(),
            "temporary EPS fault lost engagement or did not pause lateral output");
    const auto output = adapter.update(data.command, data.vehicle, 0.01,
                                       paused.lateral_allowed, paused.longitudinal_allowed);
    require(output.steering_torque == 0 && !output.lateral_active && !output.steering_request &&
              output.longitudinal_active && std::abs(output.acceleration_mps2 - 0.7) < 1e-12,
            "temporary lateral pause altered the healthy longitudinal command");
    paused = data.update(2999ms);
    require(paused.state == ControlState::SoftDisabling && paused.soft_disable_remaining_ms == 1U,
            "repeated fault samples extended or shortened the recovery deadline");
    data.vehicle.eps_fault = false;
    data.command.lateral = -10.0;
    const auto resumed = data.update();
    require(resumed.state == ControlState::Active && resumed.lateral_allowed &&
              resumed.longitudinal_allowed && resumed.soft_disable_remaining_ms == 0U,
            "healthy temporary fault did not resume before its deadline");
    (void)adapter.update(data.command, data.vehicle, 0.01,
                         resumed.lateral_allowed, resumed.longitudinal_allowed);
    require(std::abs(adapter.target_angle_deg() - 11.9) < 1e-12,
            "steering recovery reused a stale target instead of the current command and angle");
    data.vehicle.eps_fault = true;
    require(data.update().soft_disable_remaining_ms == 3000U,
            "a new temporary fault reused the previous recovery deadline");
  }
  for (const bool clear_at_deadline : {false, true}) {
    SteeringRecoveryFixture data;
    data.vehicle.eps_fault = true;
    (void)data.update();
    data.vehicle.eps_fault = !clear_at_deadline;
    const auto expired = data.update(3000ms);
    require(expired.state == ControlState::Fault && !expired.lateral_allowed &&
              !expired.longitudinal_allowed && !expired.use_vehicle_safety_mode &&
              !expired.heartbeat_engaged && !data.supervisor.arm_requested(),
            "expired temporary fault window resumed actuator output");
    data.vehicle.eps_fault = false;
    require(data.update().state == ControlState::Fault,
            "fault-clear sample automatically acknowledged a recovery timeout");
  }
  {
    SteeringRecoveryFixture data;
    data.vehicle.eps_fault = true;
    (void)data.update();
    data.panda.controls_allowed = false;
    data.vehicle.eps_fault = false;
    const auto blocked = data.update(1000ms);
    require(blocked.state == ControlState::SoftDisabling && !blocked.lateral_allowed &&
              !blocked.longitudinal_allowed && blocked.soft_disable_remaining_ms == 2000U,
            "fault recovery forced Panda permission or lost its bounded deadline");
    data.panda.controls_allowed = true;
    require(data.update(1000ms).state == ControlState::Active,
            "healthy Panda permission did not complete temporary recovery");
  }
  {
    SteeringRecoveryFixture data;
    data.vehicle.eps_fault = true;
    (void)data.update();
    data.vehicle.brake_pressed = true;
    require(!data.update(100ms).longitudinal_allowed, "brake did not interrupt temporary recovery");
    data.vehicle.brake_pressed = data.vehicle.eps_fault = false;
    const auto resumed = data.update();
    require(resumed.state == ControlState::Active && resumed.lateral_allowed &&
              !resumed.longitudinal_allowed && !resumed.longitudinal_armed,
            "temporary EPS recovery reactivated the brake-latched longitudinal channel");
  }
  for (const bool service_disarm : {false, true}) {
    SteeringRecoveryFixture data;
    data.vehicle.eps_fault = true;
    (void)data.update();
    if (service_disarm) {
      require(data.supervisor.request_arm(false), "operator could not cancel temporary recovery");
    } else {
      ++data.vehicle.cancel_button_events;
    }
    require(data.update().state == ControlState::Passive, "operator cancellation lost priority");
    data.vehicle.eps_fault = false;
    require(data.update().state == ControlState::Passive && !data.supervisor.arm_requested(),
            "temporary recovery resumed after operator cancellation");
  }
  for (unsigned failure = 0; failure < 7U; ++failure) {
    SteeringRecoveryFixture data;
    data.vehicle.eps_fault = true;
    (void)data.update();
    data.now += 101ms;
    data.panda.updated_at = data.command.received_at = data.now;
    switch (failure) {
      case 0: data.vehicle.valid = false; break;
      case 1: data.panda.faults = 1U; break;
      case 2: data.panda.safety_mode = 19U; break;
      case 3: data.command.valid = false; break;
      case 4: data.command.received_at -= 101ms; break;
      case 5:
        ++data.panda.safety_tx_blocked;
        data.panda.last_rejected_address = 0x12AU;
        break;
      case 6: data.panda.connected = false; break;
    }
    const auto stopped = data.supervisor.update(data.now, data.vehicle, data.panda, data.command);
    require(stopped.state == (failure == 6U ? ControlState::Disconnected : ControlState::Fault) &&
              !stopped.lateral_allowed && !stopped.longitudinal_allowed &&
              !data.supervisor.arm_requested(),
            "temporary EPS recovery masked an immediate-disable condition");
  }
  {
    SteeringRecoveryFixture data;
    require(data.supervisor.request_arm(false) && data.supervisor.request_arm(true),
            "could not reset fixture to armed");
    data.vehicle.eps_fault = true;
    ++data.vehicle.set_button_events;
    const auto waiting = data.update();
    require(waiting.state == ControlState::Armed && !waiting.lateral_allowed &&
              !waiting.longitudinal_allowed && !waiting.heartbeat_engaged,
            "temporary fault allowed initial engagement");
  }
}

void test_eps_fault_reception_validity() {
  using namespace ioniq5_ecan;
  const auto now = SteadyClock::now();
  VehicleStateParser parser;
  for (const auto address : {HyundaiCanFdCodec::kSteeringSensorsAddress,
                             HyundaiCanFdCodec::kMdpsAddress,
                             HyundaiCanFdCodec::kWheelSpeedsAddress,
                             HyundaiCanFdCodec::kTcsAddress}) {
    CanFrame frame;
    frame.address = address;
    frame.bus = 0;
    frame.fd = true;
    frame.size = address == HyundaiCanFdCodec::kSteeringSensorsAddress ? 16U : 24U;
    frame.received_at = now;
    if (address == HyundaiCanFdCodec::kMdpsAddress) {
      set_signal(frame.data, 54, 2, 1U, ByteOrder::LittleEndian);
    }
    const auto crc = HyundaiCanFdCodec::checksum(address, frame.data.data(), frame.size);
    frame.data[0] = static_cast<uint8_t>(crc);
    frame.data[1] = static_cast<uint8_t>(crc >> 8U);
    require(parser.update(frame), "could not parse recovery critical CAN fixture");
  }
  const auto fault = parser.snapshot(now, std::chrono::milliseconds(100));
  require(fault.valid && fault.eps_fault,
          "fresh MDPS assistance fault was confused with CAN reception loss");
  CanFrame cleared;
  cleared.address = HyundaiCanFdCodec::kMdpsAddress;
  cleared.size = 24U;
  cleared.received_at = now;
  const auto crc = HyundaiCanFdCodec::checksum(cleared.address, cleared.data.data(), cleared.size);
  cleared.data[0] = static_cast<uint8_t>(crc);
  cleared.data[1] = static_cast<uint8_t>(crc >> 8U);
  cleared.data[0] ^= 1U;
  require(!parser.update(cleared) && parser.snapshot(now, std::chrono::milliseconds(100)).eps_fault,
          "bad-checksum MDPS frame cleared the temporary assistance fault");
  cleared.data[0] ^= 1U;
  require(parser.update(cleared) && !parser.snapshot(now, std::chrono::milliseconds(100)).eps_fault,
          "valid MDPS recovery frame did not clear the temporary assistance fault");
  require(!parser.snapshot(now + std::chrono::milliseconds(101),
                           std::chrono::milliseconds(100)).valid,
          "EPS recovery allowed stale critical CAN data");
}

}  // namespace

int main() {
  test_temporary_steering_recovery();
  test_eps_fault_reception_validity();
  {
    using namespace ioniq5_ecan;
    const TimePoint now = SteadyClock::now();
    RecoveryRetry retry;
    require(!retry.due(now), "idle recovery unexpectedly requested an attempt");
    retry.request(now);
    require(retry.due(now), "first restoration was not immediate");
    TimePoint attempt = now;
    for (const int delay : {1, 2, 4, 8, 16, 30, 30}) {
      retry.failed(attempt);
      retry.request(attempt);  // Repeated loss notifications must not reset the backoff.
      require(!retry.due(attempt + std::chrono::seconds(delay) - std::chrono::milliseconds(1)),
              "ECU restoration retried before its backoff elapsed");
      attempt += std::chrono::seconds(delay);
      require(retry.due(attempt), "ECU restoration did not retry after its backoff");
    }
    require(retry.pending() && retry.failures() == 7U,
            "failed recovery lost its pending state or attempt count");
    retry.restored();
    require(!retry.pending() && !retry.due(attempt),
            "successful recovery kept retrying diagnostics");
    retry.request(attempt);
    require(retry.failures() == 0U && retry.due(attempt),
            "a new outage reused the previous failure count");
  }
  using namespace ioniq5_ecan;

  HyundaiCanFdCodec codec;
  expect_payload(codec.make_lfa(100, true, true),
                 std::array<uint8_t, 16>{0xBE, 0xBF, 0x00, 0x02, 0x80, 0xC8, 0x18, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x64, 0x00, 0x00});
  require(HyundaiCanFdCodec::checksum_valid(codec.make_lfa(0, false, false)),
          "invalid LFA checksum");

  codec.reset_counters();
  expect_payload(
    codec.make_scc_control(0.5, 0.1, true, false, false, 30.0, 5.0),
    std::array<uint8_t, 32>{0x60, 0x64, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x14, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0x09, 0x14, 0x43, 0x32, 0x32, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

  HyundaiCanFdCodec templated_codec;
  CanFrame stock_scc;
  stock_scc.address = HyundaiCanFdCodec::kSccControlAddress;
  stock_scc.size = 32U;
  stock_scc.data[2] = 41U;
  stock_scc.data[31] = 0xA5U;
  templated_codec.set_scc_control_template(stock_scc);
  const CanFrame templated_scc =
    templated_codec.make_scc_control(0.7, 0.7, true, false, false, 30.0, 5.0);
  require(templated_scc.data[2] == 42U && templated_scc.data[31] == 0xA5U,
          "stock SCC_CONTROL fields were not preserved");
  require(HyundaiCanFdCodec::checksum_valid(templated_scc),
          "templated SCC_CONTROL checksum is invalid");

  std::array<uint8_t, 8> bits{};
  set_signal(bits, 11, 12, 0xA5B, ByteOrder::BigEndian);
  require(get_signal(bits, 11, 12, ByteOrder::BigEndian) == 0xA5B,
          "Motorola signal round trip failed");

  CommandAdapterConfig adapter_config;
  adapter_config.lateral_mode = LateralInputMode::DirectTorque;
  adapter_config.max_torque = 10;
  adapter_config.torque_rate_up = 1;
  CommandAdapter adapter(adapter_config);
  VehicleStateData vehicle;
  CommandSample command;
  command.lateral = 10.0;
  command.enable = true;
  command.valid = true;
  require(adapter.update(command, vehicle, 0.01, true, false).steering_torque == 1,
          "first torque slew failed");
  require(adapter.update(command, vehicle, 0.01, true, false).steering_torque == 2,
          "second torque slew failed");
  require(sequence_is_newer(1U, 0xFFFFFFFFU), "sequence wraparound was rejected");

  CommandAdapterConfig rate_config;
  rate_config.unfiltered_input = false;
  rate_config.max_target_rate_deg_s = 10.0;
  rate_config.steer_actuator_delay_s = 0.0;
  CommandAdapter rate_adapter(rate_config);
  command.lateral = 1000.0;
  (void)rate_adapter.update(command, vehicle, 0.05, true, false);
  require(std::abs(rate_adapter.target_angle_deg() - 0.5) < 1e-12,
          "target rate was not limited before integration");

  CommandAdapterConfig exact_config;
  exact_config.max_target_rate_deg_s = 10.0;
  exact_config.steer_actuator_delay_s = 0.0;
  CommandAdapter exact_adapter(exact_config);
  command.lateral = 20.0;
  command.acceleration_mps2 = 0.7;
  const ControlOutput exact_output = exact_adapter.update(command, vehicle, 0.01, true, true);
  require(std::abs(exact_adapter.target_rate_deg_s() - 20.0) < 1e-12 &&
            std::abs(exact_adapter.target_angle_deg() - 0.2) < 1e-12 &&
            std::abs(exact_output.acceleration_mps2 - 0.7) < 1e-12,
          "unfiltered command path changed a representable ROS input");

  SafetyConfig safety_config;
  safety_config.allow_actuation = true;
  safety_config.allow_longitudinal = true;
  safety_config.required_safety_param = 3077;
  SafetySupervisor safety(safety_config);
  const TimePoint now = SteadyClock::now();
  vehicle.valid = true;
  PandaHealth panda;
  panda.connected = true;
  panda.controls_allowed = true;
  panda.harness_status = 1;
  panda.safety_mode = 28;
  panda.safety_param = 3077;
  panda.updated_at = now;
  command.received_at = now;
  require(safety.request_arm(true), "arm request rejected");
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "safety supervisor activated before a channel arm button");
  ++vehicle.lane_keep_button_events;
  SafetyDecision split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && !split.longitudinal_allowed,
          "LDA did not arm only lateral control");
  ++vehicle.set_button_events;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && split.longitudinal_allowed,
          "SET did not select combined control");
  vehicle.brake_pressed = true;
  panda.last_rejected_address = 0x12AU;
  ++panda.safety_tx_blocked;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && !split.longitudinal_allowed,
          "brake did not disable only longitudinal control");
  vehicle.brake_pressed = false;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && !split.longitudinal_allowed,
          "brake release unexpectedly resumed longitudinal control");
  ++vehicle.set_button_events;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && split.longitudinal_allowed,
          "SET did not re-arm longitudinal control after brake release");
  ++vehicle.lane_keep_button_events;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && !split.longitudinal_allowed,
          "LDA did not switch combined control to lateral-only control");
  ++vehicle.lane_keep_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "LDA did not toggle lateral-only control off");
  panda.controls_allowed = false;
  ++vehicle.set_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "combined mode became active while Panda controls were disabled");
  panda.controls_allowed = true;
  split = safety.update(now, vehicle, panda, command);
  require(
    split.state == ControlState::Active && split.lateral_allowed && split.longitudinal_allowed,
    "armed combined mode did not activate when Panda controls returned");
  panda.faults = 1U;
  const SafetyDecision fault = safety.update(now, vehicle, panda, command);
  require(
    fault.state == ControlState::Fault && !fault.lateral_allowed && !fault.longitudinal_allowed,
    "Panda hardware fault did not stop control");
  panda.faults = 0U;
  split = safety.update(now, vehicle, panda, command);
  require(split.state == ControlState::Fault && !split.lateral_allowed && !split.longitudinal_allowed,
          "recovered health resumed output without operator acknowledgement");
  require(safety.request_arm(false) && safety.request_arm(true),
          "operator could not acknowledge and rearm after recovery");
  split = safety.update(now, vehicle, panda, command);
  require(split.state == ControlState::Armed && !split.lateral_allowed && !split.longitudinal_allowed,
          "rearming reused the channel selection from before the outage");
  ++vehicle.set_button_events;
  split = safety.update(now, vehicle, panda, command);
  require(split.lateral_allowed && split.longitudinal_allowed,
          "explicit SET selection did not resume healthy recovered control");

  VehicleStateParser button_parser;
  CanFrame button;
  button.address = HyundaiCanFdCodec::kCruiseButtonsAddress;
  button.bus = 0;
  button.size = 8;
  button.received_at = now;
  set_signal(button.data, 16, 3, 1U, ByteOrder::LittleEndian);
  require(button_parser.update(button), "RES press was not parsed");
  set_signal(button.data, 16, 3, 0U, ByteOrder::LittleEndian);
  require(button_parser.update(button), "RES release was not parsed");
  require(button_parser.snapshot(now, std::chrono::milliseconds(100)).set_button_events == 0U,
          "RES release incorrectly toggled control");
  set_signal(button.data, 16, 3, 2U, ByteOrder::LittleEndian);
  require(button_parser.update(button), "SET press was not parsed");
  set_signal(button.data, 16, 3, 0U, ByteOrder::LittleEndian);
  require(button_parser.update(button), "SET release was not parsed");
  require(button_parser.snapshot(now, std::chrono::milliseconds(100)).set_button_events == 1U,
          "SET release did not create control event");
  set_signal(button.data, 23, 1, 1U, ByteOrder::LittleEndian);
  require(button_parser.update(button), "LDA press was not parsed");
  const VehicleStateData button_state = button_parser.snapshot(now, std::chrono::milliseconds(100));
  require(button_state.lane_keep_button_events == 1U && button_state.lane_keep_button_pressed,
          "LDA press did not create lateral arm event");

  VehicleStateParser dynamics_parser;
  CanFrame imu;
  imu.address = HyundaiCanFdCodec::kImuAddress;
  imu.bus = 0;
  imu.fd = true;
  imu.size = 32;
  imu.received_at = now;
  set_signal(imu.data, 64, 16, 32968U, ByteOrder::LittleEndian);
  set_signal(imu.data, 80, 16, 32768U, ByteOrder::LittleEndian);
  set_signal(imu.data, 96, 16, 32768U, ByteOrder::LittleEndian);
  const uint16_t imu_crc = HyundaiCanFdCodec::checksum(imu.address, imu.data.data(), imu.size);
  imu.data[0] = static_cast<uint8_t>(imu_crc & 0xFFU);
  imu.data[1] = static_cast<uint8_t>(imu_crc >> 8U);
  require(dynamics_parser.update(imu), "IMU frame was not parsed");
  require(std::abs(dynamics_parser.snapshot(now, std::chrono::milliseconds(100)).yaw_rate_deg_s -
                   1.0) < 1e-9,
          "IMU yaw-rate conversion failed");

  CanFrame wheels;
  wheels.address = HyundaiCanFdCodec::kWheelSpeedsAddress;
  wheels.bus = 0;
  wheels.fd = true;
  wheels.size = 24;
  wheels.received_at = now;
  for (const unsigned start : {64U, 80U, 96U, 112U}) {
    set_signal(wheels.data, start, 14, 320U, ByteOrder::LittleEndian);
  }
  const uint16_t wheels_crc =
    HyundaiCanFdCodec::checksum(wheels.address, wheels.data.data(), wheels.size);
  wheels.data[0] = static_cast<uint8_t>(wheels_crc & 0xFFU);
  wheels.data[1] = static_cast<uint8_t>(wheels_crc >> 8U);
  require(dynamics_parser.update(wheels), "wheel-speed frame was not parsed");
  const VehicleStateData dynamics = dynamics_parser.snapshot(now, std::chrono::milliseconds(100));
  require(std::abs(dynamics.wheel_speed_fl - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_fr - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_rl - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_rr - 10.0 / 3.6) < 1e-9,
          "individual wheel-speed conversion failed");

  std::cout << "core smoke tests passed\n";
  return 0;
}
