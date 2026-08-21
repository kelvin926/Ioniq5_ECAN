#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "ioniq5_ecan/bit_codec.hpp"
#include "ioniq5_ecan/command_adapter.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
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

}  // namespace

int main() {
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
    std::array<uint8_t, 32>{0x83, 0x45, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x14, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0x09, 0x14, 0x43, 0x1E, 0x32, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

  std::array<uint8_t, 8> bits{};
  set_signal(bits, 11, 12, 0xA5B, ByteOrder::BigEndian);
  require(get_signal(bits, 11, 12, ByteOrder::BigEndian) == 0xA5B,
          "Motorola signal round trip failed");

  CommandAdapterConfig adapter_config;
  adapter_config.lateral_mode = LateralInputMode::DirectTorque;
  adapter_config.max_torque = 10;
  adapter_config.torque_rate_up = 1;
  CommandAdapter adapter(adapter_config);
  VehicleState vehicle;
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
  rate_config.max_target_rate_deg_s = 10.0;
  rate_config.steer_actuator_delay_s = 0.0;
  CommandAdapter rate_adapter(rate_config);
  command.lateral = 1000.0;
  (void)rate_adapter.update(command, vehicle, 0.05, true, false);
  require(std::abs(rate_adapter.target_angle_deg() - 0.5) < 1e-12,
          "target rate was not limited before integration");

  SafetyConfig safety_config;
  safety_config.allow_actuation = true;
  safety_config.allow_longitudinal = true;
  safety_config.required_safety_param = 1037;
  SafetySupervisor safety(safety_config);
  const TimePoint now = SteadyClock::now();
  vehicle.valid = true;
  PandaHealth panda;
  panda.connected = true;
  panda.controls_allowed = true;
  panda.harness_status = 1;
  panda.safety_mode = 28;
  panda.safety_param = 1037;
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
          "SET did not independently arm longitudinal control");
  ++vehicle.lane_keep_button_events;
  split = safety.update(now, vehicle, panda, command);
  require(!split.lateral_allowed && split.longitudinal_allowed,
          "LDA did not toggle only lateral control off");
  ++vehicle.set_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "SET did not toggle only longitudinal control off");
  panda.controls_allowed = false;
  ++vehicle.lane_keep_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "channel became active while Panda controls were disabled");
  panda.controls_allowed = true;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Active,
          "armed lateral channel did not activate when Panda controls returned");
  panda.faults = 1U;
  const SafetyDecision fault = safety.update(now, vehicle, panda, command);
  require(
    fault.state == ControlState::Fault && !fault.lateral_allowed && !fault.longitudinal_allowed,
    "Panda hardware fault did not stop control");

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
  const VehicleState button_state = button_parser.snapshot(now, std::chrono::milliseconds(100));
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
  const VehicleState dynamics = dynamics_parser.snapshot(now, std::chrono::milliseconds(100));
  require(std::abs(dynamics.wheel_speed_fl - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_fr - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_rl - 10.0 / 3.6) < 1e-9 &&
            std::abs(dynamics.wheel_speed_rr - 10.0 / 3.6) < 1e-9,
          "individual wheel-speed conversion failed");

  std::cout << "core smoke tests passed\n";
  return 0;
}
