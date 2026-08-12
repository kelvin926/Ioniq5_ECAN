#include "ioniq5_ecan/hyundai_canfd_codec.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include "ioniq5_ecan/bit_codec.hpp"

namespace ioniq5_ecan {
namespace {

uint16_t crc_step(uint16_t crc, uint8_t byte) {
  crc ^= static_cast<uint16_t>(byte) << 8U;
  for (int i = 0; i < 8; ++i) {
    crc = static_cast<uint16_t>((crc & 0x8000U) ? (crc << 1U) ^ 0x1021U : crc << 1U);
  }
  return crc;
}

template <std::size_t N>
void finish_frame(CanFrame& frame, const std::array<uint8_t, N>& payload) {
  frame.size = static_cast<uint8_t>(N);
  std::copy(payload.begin(), payload.end(), frame.data.begin());
}

template <std::size_t N>
void write_checksum(uint32_t address, std::array<uint8_t, N>& data) {
  const uint16_t crc = HyundaiCanFdCodec::checksum(address, data.data(), N);
  data[0] = static_cast<uint8_t>(crc & 0xFFU);
  data[1] = static_cast<uint8_t>(crc >> 8U);
}

uint64_t physical_to_raw(double value, double factor, double offset, unsigned bits) {
  const double rounded = std::floor(((value - offset) / factor) + 0.5);
  const double maximum = static_cast<double>((uint64_t{1} << bits) - 1U);
  return static_cast<uint64_t>(std::clamp(rounded, 0.0, maximum));
}

}  // namespace

uint16_t HyundaiCanFdCodec::checksum(uint32_t address, const uint8_t* data, std::size_t size) {
  if (data == nullptr || size < 3U || size > 64U) {
    throw std::invalid_argument("invalid Hyundai CAN-FD payload");
  }
  uint16_t crc = 0;
  for (std::size_t i = 2; i < size; ++i) {
    crc = crc_step(crc, data[i]);
  }
  crc = crc_step(crc, static_cast<uint8_t>(address & 0xFFU));
  crc = crc_step(crc, static_cast<uint8_t>((address >> 8U) & 0xFFU));
  switch (size) {
    case 8:
      crc ^= 0x5F29U;
      break;
    case 16:
      crc ^= 0x041DU;
      break;
    case 24:
      crc ^= 0x819DU;
      break;
    case 32:
      crc ^= 0x9F5BU;
      break;
    default:
      throw std::invalid_argument("unsupported Hyundai CAN-FD payload size");
  }
  return crc;
}

bool HyundaiCanFdCodec::checksum_valid(const CanFrame& frame) {
  if (frame.size != 8U && frame.size != 16U && frame.size != 24U && frame.size != 32U) {
    return false;
  }
  const uint16_t received =
    static_cast<uint16_t>(frame.data[0]) | (static_cast<uint16_t>(frame.data[1]) << 8U);
  return received == checksum(frame.address, frame.data.data(), frame.size);
}

CanFrame HyundaiCanFdCodec::make_lfa(int torque, bool enabled, bool torque_request, uint8_t bus) {
  std::array<uint8_t, 16> data{};
  torque = std::clamp(torque, -270, 270);
  set_signal(data, 16, 8, lfa_counter_++, ByteOrder::LittleEndian);
  set_signal(data, 24, 3, 2, ByteOrder::LittleEndian);  // LKA_OptUsmSta
  set_signal(data, 27, 3, 0, ByteOrder::LittleEndian);  // LKA_RcgSta
  set_signal(data, 38, 3, enabled ? 2U : 1U, ByteOrder::LittleEndian);
  set_signal(data, 41, 11, static_cast<uint64_t>(torque + 1024), ByteOrder::LittleEndian);
  set_signal(data, 52, 2, torque_request ? 1U : 0U, ByteOrder::LittleEndian);
  set_signal(data, 60, 4, 0, ByteOrder::LittleEndian);
  set_signal(data, 104, 8, 100, ByteOrder::LittleEndian);  // Damping_Gain
  set_signal(data, 80, 2, 0, ByteOrder::LittleEndian);     // hide LKA settings
  write_checksum(kLfaAddress, data);

  CanFrame frame;
  frame.address = kLfaAddress;
  frame.bus = bus;
  frame.fd = true;
  finish_frame(frame, data);
  return frame;
}

CanFrame HyundaiCanFdCodec::make_scc_control(double accel_raw_mps2, double accel_value_mps2,
                                             bool enabled, bool stopping, bool gas_override,
                                             double set_speed_kph, double jerk_mps3, uint8_t bus) {
  std::array<uint8_t, 32> data{};
  accel_raw_mps2 = std::clamp(accel_raw_mps2, -3.5, 2.0);
  accel_value_mps2 = std::clamp(accel_value_mps2, -3.5, 2.0);
  jerk_mps3 = std::clamp(jerk_mps3, 0.0, 12.7);
  set_speed_kph = std::clamp(set_speed_kph, 0.0, 255.0);

  set_signal(data, 16, 8, scc_counter_++, ByteOrder::LittleEndian);
  set_signal(data, 24, 11, 10, ByteOrder::LittleEndian);  // 1.0 m object distance
  set_signal(data, 45, 2, 3, ByteOrder::BigEndian);
  set_signal(data, 46, 1, 0, ByteOrder::BigEndian);
  set_signal(data, 55, 8, 0x64, ByteOrder::BigEndian);
  set_signal(data, 66, 1, 1, ByteOrder::LittleEndian);
  set_signal(data, 68, 3, enabled ? (gas_override ? 2U : 1U) : 0U, ByteOrder::LittleEndian);
  set_signal(data, 76, 1, stopping ? 1U : 0U, ByteOrder::LittleEndian);
  set_signal(data, 88, 3, 4, ByteOrder::LittleEndian);
  set_signal(data, 103, 8, physical_to_raw(set_speed_kph, 1.0, 0.0, 8), ByteOrder::BigEndian);
  set_signal(data, 105, 3, 4, ByteOrder::LittleEndian);
  set_signal(data, 128, 11, physical_to_raw(accel_value_mps2, 0.01, -10.23, 11),
             ByteOrder::LittleEndian);
  set_signal(data, 140, 11, physical_to_raw(accel_raw_mps2, 0.01, -10.23, 11),
             ByteOrder::LittleEndian);
  set_signal(data, 158, 7, 30, ByteOrder::BigEndian);  // 3.0 m/s^3
  set_signal(data, 166, 7, physical_to_raw(enabled ? jerk_mps3 : 1.0, 0.1, 0.0, 7),
             ByteOrder::BigEndian);
  set_signal(data, 176, 3, 2, ByteOrder::LittleEndian);
  set_signal(data, 184, 1, stopping ? 1U : 0U, ByteOrder::BigEndian);
  write_checksum(kSccControlAddress, data);

  CanFrame frame;
  frame.address = kSccControlAddress;
  frame.bus = bus;
  frame.fd = true;
  finish_frame(frame, data);
  return frame;
}

CanFrame HyundaiCanFdCodec::make_lfa_cluster(bool enabled, uint8_t bus) {
  std::array<uint8_t, 16> data{};
  set_signal(data, 16, 8, cluster_counter_++, ByteOrder::LittleEndian);
  set_signal(data, 31, 1, enabled ? 1U : 0U, ByteOrder::LittleEndian);
  set_signal(data, 47, 2, enabled ? 2U : 0U, ByteOrder::LittleEndian);
  write_checksum(kLfaClusterAddress, data);

  CanFrame frame;
  frame.address = kLfaClusterAddress;
  frame.bus = bus;
  frame.fd = true;
  finish_frame(frame, data);
  return frame;
}

CanFrame HyundaiCanFdCodec::make_fca_warning(uint8_t bus) {
  std::array<uint8_t, 16> data{};
  set_signal(data, 16, 8, fca_warning_counter_++, ByteOrder::LittleEndian);
  set_signal(data, 24, 2, 1, ByteOrder::LittleEndian);  // AEB_SETTING: disabled
  set_signal(data, 56, 8, 0x02, ByteOrder::LittleEndian);
  set_signal(data, 64, 8, 0xFF, ByteOrder::LittleEndian);
  set_signal(data, 72, 8, 0xFC, ByteOrder::LittleEndian);
  set_signal(data, 80, 8, 0x09, ByteOrder::LittleEndian);
  write_checksum(kFcaWarningAddress, data);

  CanFrame frame;
  frame.address = kFcaWarningAddress;
  frame.bus = bus;
  frame.fd = true;
  finish_frame(frame, data);
  return frame;
}

void HyundaiCanFdCodec::reset_counters(uint8_t lfa, uint8_t scc, uint8_t cluster,
                                       uint8_t fca_warning) {
  lfa_counter_ = lfa;
  scc_counter_ = scc;
  cluster_counter_ = cluster;
  fca_warning_counter_ = fca_warning;
}

}  // namespace ioniq5_ecan
