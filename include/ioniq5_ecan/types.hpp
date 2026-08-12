#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

namespace ioniq5_ecan {

using SteadyClock = std::chrono::steady_clock;
using TimePoint = SteadyClock::time_point;

struct CanFrame {
  uint32_t address{0};
  uint8_t bus{0};
  bool fd{false};
  bool returned{false};
  bool rejected{false};
  uint8_t size{0};
  std::array<uint8_t, 64> data{};
  TimePoint received_at{};
};

struct CommandSample {
  double lateral{0.0};
  double acceleration_mps2{0.0};
  bool enable{false};
  bool valid{false};
  uint32_t sequence{0};
  TimePoint received_at{};
};

struct VehicleState {
  double speed_mps{0.0};
  double steering_angle_deg{0.0};
  double steering_rate_deg_s{0.0};
  double driver_torque{0.0};
  double eps_torque_nm{0.0};
  double accelerator_pedal{0.0};
  bool brake_pressed{false};
  bool gas_pressed{false};
  bool eps_fault{false};
  bool acc_fault{false};
  bool cruise_engaged{false};
  bool standstill{true};
  uint8_t gear{0};
  uint8_t cruise_button{0};
  uint64_t enable_button_events{0};
  uint64_t cancel_button_events{0};
  bool valid{false};
  TimePoint updated_at{};
};

struct PandaHealth {
  bool connected{false};
  bool controls_allowed{false};
  bool heartbeat_lost{false};
  bool safety_rx_checks_invalid{false};
  bool bus_off{false};
  uint8_t harness_status{0};
  uint8_t safety_mode{0};
  uint16_t safety_param{0};
  uint32_t safety_tx_blocked{0};
  uint32_t safety_rx_invalid{0};
  uint32_t tx_buffer_overflow{0};
  uint32_t rx_buffer_overflow{0};
  TimePoint updated_at{};
};

enum class ControlState : uint8_t {
  Disconnected = 0,
  Passive = 1,
  Armed = 2,
  Active = 3,
  Fault = 4,
};

inline const char* to_string(ControlState state) {
  switch (state) {
    case ControlState::Disconnected:
      return "DISCONNECTED";
    case ControlState::Passive:
      return "PASSIVE";
    case ControlState::Armed:
      return "ARMED";
    case ControlState::Active:
      return "ACTIVE";
    case ControlState::Fault:
      return "FAULT";
  }
  return "UNKNOWN";
}

struct ControlOutput {
  int steering_torque{0};
  double acceleration_mps2{0.0};
  bool lateral_active{false};
  bool longitudinal_active{false};
  bool stopping{false};
};

}  // namespace ioniq5_ecan
