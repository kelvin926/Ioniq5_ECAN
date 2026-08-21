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
  SafetySupervisor safety(safety_config);
  const TimePoint now = SteadyClock::now();
  vehicle.valid = true;
  vehicle.set_button_events = 1;
  PandaHealth panda;
  panda.connected = true;
  panda.controls_allowed = true;
  panda.harness_status = 1;
  panda.safety_mode = 28;
  panda.safety_param = 9;
  panda.updated_at = now;
  command.received_at = now;
  require(safety.request_arm(true), "arm request rejected");
  require(safety.update(now, vehicle, panda, command).state == ControlState::Active,
          "safety supervisor did not activate");
  ++vehicle.set_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "second SET release did not toggle control off");
  ++vehicle.set_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Active,
          "third SET release did not toggle control on");
  panda.controls_allowed = false;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Armed,
          "Panda disengagement did not pause control");
  panda.controls_allowed = true;
  ++vehicle.set_button_events;
  require(safety.update(now, vehicle, panda, command).state == ControlState::Active,
          "one SET release did not re-enable after Panda disengagement");
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

  std::cout << "core smoke tests passed\n";
  return 0;
}
