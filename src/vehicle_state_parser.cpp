#include "ioniq5_ecan/vehicle_state_parser.hpp"

#include <algorithm>
#include <array>
#include <cmath>

#include "ioniq5_ecan/bit_codec.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"

namespace ioniq5_ecan {
namespace {

template <std::size_t N>
std::array<uint8_t, N> payload_as(const CanFrame& frame) {
  std::array<uint8_t, N> result{};
  std::copy_n(frame.data.begin(), N, result.begin());
  return result;
}

bool recent(TimePoint timestamp, TimePoint now, std::chrono::milliseconds timeout) {
  return timestamp.time_since_epoch().count() != 0 && now >= timestamp &&
         now - timestamp <= timeout;
}

}  // namespace

VehicleStateParser::VehicleStateParser(uint8_t ecan_bus, uint8_t camera_bus, bool alternate_buttons)
    : ecan_bus_(ecan_bus), camera_bus_(camera_bus), alternate_buttons_(alternate_buttons) {}

bool VehicleStateParser::require_crc(const CanFrame& frame) {
  if (!HyundaiCanFdCodec::checksum_valid(frame)) {
    ++checksum_failures_;
    return false;
  }
  return true;
}

bool VehicleStateParser::update(const CanFrame& frame) {
  if (frame.rejected || frame.returned) {
    return false;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  const bool on_ecan = frame.bus == ecan_bus_;
  const bool on_camera = frame.bus == camera_bus_;
  if (!on_ecan && !on_camera) {
    return false;
  }

  switch (frame.address) {
    case HyundaiCanFdCodec::kSteeringSensorsAddress:
      if (!on_ecan || frame.size != 16U || !require_crc(frame)) break;
      parse_steering(frame);
      return true;
    case HyundaiCanFdCodec::kMdpsAddress:
      if (!on_ecan || frame.size != 24U || !require_crc(frame)) break;
      parse_mdps(frame);
      return true;
    case HyundaiCanFdCodec::kWheelSpeedsAddress:
      if (!on_ecan || frame.size != 24U || !require_crc(frame)) break;
      parse_wheel_speeds(frame);
      return true;
    case HyundaiCanFdCodec::kTcsAddress:
      if (!on_ecan || frame.size != 24U || !require_crc(frame)) break;
      parse_tcs(frame);
      return true;
    case HyundaiCanFdCodec::kAcceleratorAddress:
      if (!on_ecan || frame.size != 32U || !require_crc(frame)) break;
      parse_accelerator(frame);
      return true;
    case HyundaiCanFdCodec::kCruiseButtonsAddress:
      if (alternate_buttons_) return false;
      if (!on_ecan || frame.size != 8U) break;
      parse_buttons(frame, false);
      return true;
    case HyundaiCanFdCodec::kCruiseButtonsAltAddress:
      if (!alternate_buttons_) return false;
      if (!on_ecan || frame.size != 16U || !require_crc(frame)) break;
      parse_buttons(frame, true);
      return true;
    case HyundaiCanFdCodec::kSccControlAddress:
      if ((!on_ecan && !on_camera) || frame.size != 32U || !require_crc(frame)) break;
      parse_scc(frame);
      return true;
    default:
      return false;
  }

  ++malformed_frames_;
  return false;
}

void VehicleStateParser::parse_steering(const CanFrame& frame) {
  const auto data = payload_as<16>(frame);
  const int64_t angle_raw = sign_extend(get_signal(data, 24, 16, ByteOrder::LittleEndian), 16);
  const double angle_deg = static_cast<double>(angle_raw) * 0.1;
  const double reported_rate =
    static_cast<double>(get_signal(data, 40, 8, ByteOrder::LittleEndian)) * 4.0;

  double signed_rate = 0.0;
  if (have_previous_angle_ && frame.received_at > previous_angle_time_) {
    const double dt =
      std::chrono::duration<double>(frame.received_at - previous_angle_time_).count();
    const double finite_difference = (angle_deg - previous_angle_deg_) / dt;
    if (std::abs(finite_difference) > 0.2) {
      signed_rate = std::copysign(reported_rate, finite_difference);
    }
  }

  state_.steering_angle_deg = angle_deg;
  state_.steering_rate_deg_s = signed_rate;
  state_.updated_at = frame.received_at;
  steering_time_ = frame.received_at;
  previous_angle_deg_ = angle_deg;
  previous_angle_time_ = frame.received_at;
  have_previous_angle_ = true;
}

void VehicleStateParser::parse_mdps(const CanFrame& frame) {
  const auto data = payload_as<24>(frame);
  state_.driver_torque =
    static_cast<double>(get_signal(data, 80, 13, ByteOrder::LittleEndian)) - 4095.0;
  state_.eps_torque_nm =
    static_cast<double>(get_signal(data, 64, 12, ByteOrder::LittleEndian)) * 0.1 - 204.8;
  state_.eps_fault = get_signal(data, 54, 2, ByteOrder::LittleEndian) != 0U;
  mdps_time_ = frame.received_at;
}

void VehicleStateParser::parse_wheel_speeds(const CanFrame& frame) {
  const auto data = payload_as<24>(frame);
  double total_kph = 0.0;
  for (const unsigned start : {64U, 80U, 96U, 112U}) {
    total_kph +=
      static_cast<double>(get_signal(data, start, 14, ByteOrder::LittleEndian)) * 0.03125;
  }
  state_.speed_mps = (total_kph / 4.0) / 3.6;
  state_.standstill = state_.speed_mps < (0.375 / 3.6);
  wheel_time_ = frame.received_at;
}

void VehicleStateParser::parse_tcs(const CanFrame& frame) {
  const auto data = payload_as<24>(frame);
  state_.brake_pressed = get_signal(data, 81, 1, ByteOrder::BigEndian) != 0U;
  state_.acc_fault = get_signal(data, 67, 2, ByteOrder::BigEndian) != 0U;
  tcs_time_ = frame.received_at;
}

void VehicleStateParser::parse_accelerator(const CanFrame& frame) {
  const auto data = payload_as<32>(frame);
  state_.accelerator_pedal = static_cast<double>(get_signal(data, 40, 8, ByteOrder::LittleEndian));
  state_.gas_pressed = state_.accelerator_pedal > 0.0;
  state_.gear = static_cast<uint8_t>(get_signal(data, 192, 3, ByteOrder::LittleEndian));
}

void VehicleStateParser::parse_buttons(const CanFrame& frame, bool alternate) {
  uint8_t current = 0;
  if (alternate) {
    const auto data = payload_as<16>(frame);
    current = static_cast<uint8_t>(get_signal(data, 36, 3, ByteOrder::LittleEndian));
  } else {
    const auto data = payload_as<8>(frame);
    current = static_cast<uint8_t>(get_signal(data, 16, 3, ByteOrder::LittleEndian));
  }

  // Hyundai button values: RES=1, SET=2, CANCEL=4. Only a SET release controls
  // this project's host-side actuation toggle.
  if (previous_button_ == 2U && current == 0U) {
    ++state_.set_button_events;
  }
  if (current == 4U && previous_button_ != 4U) {
    ++state_.cancel_button_events;
  }
  previous_button_ = current;
  state_.cruise_button = current;
}

void VehicleStateParser::parse_scc(const CanFrame& frame) {
  const auto data = payload_as<32>(frame);
  const uint64_t mode = get_signal(data, 68, 3, ByteOrder::LittleEndian);
  state_.cruise_engaged = mode == 1U || mode == 2U;
}

VehicleState VehicleStateParser::snapshot(TimePoint now,
                                          std::chrono::milliseconds critical_timeout) const {
  std::lock_guard<std::mutex> lock(mutex_);
  VehicleState copy = state_;
  copy.valid = recent(steering_time_, now, critical_timeout) &&
               recent(mdps_time_, now, critical_timeout) &&
               recent(wheel_time_, now, critical_timeout) &&
               recent(tcs_time_, now, critical_timeout) && !copy.eps_fault;
  return copy;
}

uint64_t VehicleStateParser::checksum_failures() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return checksum_failures_;
}

uint64_t VehicleStateParser::malformed_frames() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return malformed_frames_;
}

}  // namespace ioniq5_ecan
