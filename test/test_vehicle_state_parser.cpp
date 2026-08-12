#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>

#include "ioniq5_ecan/bit_codec.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
#include "ioniq5_ecan/vehicle_state_parser.hpp"

namespace {

template <std::size_t N>
ioniq5_ecan::CanFrame frame_with_crc(uint32_t address, const std::array<uint8_t, N>& input,
                                     ioniq5_ecan::TimePoint received_at) {
  auto data = input;
  const uint16_t crc = ioniq5_ecan::HyundaiCanFdCodec::checksum(address, data.data(), data.size());
  data[0] = static_cast<uint8_t>(crc & 0xFFU);
  data[1] = static_cast<uint8_t>(crc >> 8U);
  ioniq5_ecan::CanFrame frame;
  frame.address = address;
  frame.bus = 0;
  frame.fd = true;
  frame.size = static_cast<uint8_t>(N);
  frame.received_at = received_at;
  std::copy(data.begin(), data.end(), frame.data.begin());
  return frame;
}

TEST(VehicleStateParser, BuildsValidCriticalStateFromEcan) {
  using namespace ioniq5_ecan;
  const TimePoint now = SteadyClock::now();
  VehicleStateParser parser;

  std::array<uint8_t, 16> steering{};
  set_signal(steering, 24, 16, static_cast<uint16_t>(-123), ByteOrder::LittleEndian);
  set_signal(steering, 40, 8, 5, ByteOrder::LittleEndian);
  EXPECT_TRUE(
    parser.update(frame_with_crc(HyundaiCanFdCodec::kSteeringSensorsAddress, steering, now)));

  std::array<uint8_t, 24> mdps{};
  set_signal(mdps, 64, 12, 2148, ByteOrder::LittleEndian);
  set_signal(mdps, 80, 13, 4195, ByteOrder::LittleEndian);
  EXPECT_TRUE(parser.update(frame_with_crc(HyundaiCanFdCodec::kMdpsAddress, mdps, now)));

  std::array<uint8_t, 24> wheels{};
  for (const unsigned start : {64U, 80U, 96U, 112U}) {
    set_signal(wheels, start, 14, 320, ByteOrder::LittleEndian);
  }
  EXPECT_TRUE(parser.update(frame_with_crc(HyundaiCanFdCodec::kWheelSpeedsAddress, wheels, now)));

  std::array<uint8_t, 24> tcs{};
  set_signal(tcs, 81, 1, 0, ByteOrder::BigEndian);
  set_signal(tcs, 67, 2, 0, ByteOrder::BigEndian);
  EXPECT_TRUE(parser.update(frame_with_crc(HyundaiCanFdCodec::kTcsAddress, tcs, now)));

  const VehicleState state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_TRUE(state.valid);
  EXPECT_NEAR(state.steering_angle_deg, -12.3, 1e-9);
  EXPECT_NEAR(state.driver_torque, 100.0, 1e-9);
  EXPECT_NEAR(state.eps_torque_nm, 10.0, 1e-9);
  EXPECT_NEAR(state.speed_mps, 10.0 / 3.6, 1e-9);
}

TEST(VehicleStateParser, RejectsBadChecksum) {
  using namespace ioniq5_ecan;
  const TimePoint now = SteadyClock::now();
  VehicleStateParser parser;
  std::array<uint8_t, 16> steering{};
  CanFrame frame = frame_with_crc(HyundaiCanFdCodec::kSteeringSensorsAddress, steering, now);
  frame.data[5] ^= 0x80U;
  EXPECT_FALSE(parser.update(frame));
  EXPECT_EQ(parser.checksum_failures(), 1U);
}

}  // namespace
