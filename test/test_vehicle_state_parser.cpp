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

  const VehicleStateData state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_TRUE(state.valid);
  EXPECT_NEAR(state.steering_angle_deg, -12.3, 1e-9);
  EXPECT_NEAR(state.driver_torque, 100.0, 1e-9);
  EXPECT_NEAR(state.eps_torque_nm, 10.0, 1e-9);
  EXPECT_NEAR(state.speed_mps, 10.0 / 3.6, 1e-9);
  EXPECT_NEAR(state.wheel_speed_fl, 10.0 / 3.6, 1e-9);
  EXPECT_NEAR(state.wheel_speed_fr, 10.0 / 3.6, 1e-9);
  EXPECT_NEAR(state.wheel_speed_rl, 10.0 / 3.6, 1e-9);
  EXPECT_NEAR(state.wheel_speed_rr, 10.0 / 3.6, 1e-9);
}

TEST(VehicleStateParser, ParsesImuDynamicsInRequestedUnits) {
  using namespace ioniq5_ecan;
  const TimePoint now = SteadyClock::now();
  VehicleStateParser parser;
  std::array<uint8_t, 32> imu{};
  constexpr uint64_t yaw_raw = 32968U;
  constexpr uint64_t lateral_raw = 33568U;
  constexpr uint64_t longitudinal_raw = 31198U;
  set_signal(imu, 64, 16, yaw_raw, ByteOrder::LittleEndian);
  set_signal(imu, 80, 16, lateral_raw, ByteOrder::LittleEndian);
  set_signal(imu, 96, 16, longitudinal_raw, ByteOrder::LittleEndian);
  EXPECT_TRUE(parser.update(frame_with_crc(HyundaiCanFdCodec::kImuAddress, imu, now)));

  const VehicleStateData state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_NEAR(state.yaw_rate_deg_s, static_cast<double>(yaw_raw) * 0.005 - 163.84, 1e-9);
  EXPECT_NEAR(state.lateral_accel_mps2,
              (static_cast<double>(lateral_raw) * 0.000127465 - 4.17677312) * 9.80665, 1e-9);
  EXPECT_NEAR(state.longitudinal_accel_mps2,
              (static_cast<double>(longitudinal_raw) * 0.000127465 - 4.17677312) * 9.80665, 1e-9);

  set_signal(imu, 24, 4, 1U, ByteOrder::LittleEndian);
  set_signal(imu, 64, 16, 0U, ByteOrder::LittleEndian);
  EXPECT_TRUE(parser.update(frame_with_crc(HyundaiCanFdCodec::kImuAddress, imu, now)));
  EXPECT_NEAR(parser.snapshot(now, std::chrono::milliseconds(100)).yaw_rate_deg_s,
              state.yaw_rate_deg_s, 1e-9);
}

TEST(VehicleStateParser, ParsesEscDynamicsFallback) {
  using namespace ioniq5_ecan;
  const TimePoint now = SteadyClock::now();
  VehicleStateParser parser;
  CanFrame esc;
  esc.address = HyundaiCanFdCodec::kEscDynamicsAddress;
  esc.bus = 0;
  esc.fd = true;
  esc.size = 32;
  esc.received_at = now;
  set_signal(esc.data, 64, 16, 32768U, ByteOrder::LittleEndian);
  set_signal(esc.data, 80, 16, 32768U, ByteOrder::LittleEndian);
  set_signal(esc.data, 96, 16, 32768U, ByteOrder::LittleEndian);
  EXPECT_TRUE(parser.update(esc));

  const VehicleStateData state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_NEAR(state.yaw_rate_deg_s, 0.0, 1e-12);
  EXPECT_NEAR(state.lateral_accel_mps2, 0.0, 1e-12);
  EXPECT_NEAR(state.longitudinal_accel_mps2, 0.0, 1e-12);
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

TEST(VehicleStateParser, SeparatesLaneKeepPressFromSetRelease) {
  using namespace ioniq5_ecan;
  VehicleStateParser parser;
  const TimePoint now = SteadyClock::now();

  auto send_button = [&](uint8_t value, bool lane_keep = false) {
    CanFrame frame;
    frame.address = HyundaiCanFdCodec::kCruiseButtonsAddress;
    frame.bus = 0;
    frame.size = 8;
    frame.received_at = now;
    set_signal(frame.data, 16, 3, value, ByteOrder::LittleEndian);
    set_signal(frame.data, 23, 1, lane_keep ? 1U : 0U, ByteOrder::LittleEndian);
    EXPECT_TRUE(parser.update(frame));
  };

  send_button(1U);  // RES press
  send_button(0U);  // RES release
  EXPECT_EQ(parser.snapshot(now, std::chrono::milliseconds(100)).set_button_events, 0U);

  send_button(2U);  // SET press
  send_button(0U);  // SET release
  VehicleStateData state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_EQ(state.set_button_events, 1U);
  EXPECT_EQ(state.lane_keep_button_events, 0U);

  send_button(0U, true);  // LDA press
  send_button(0U, true);  // repeated frame must not create another event
  state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_EQ(state.lane_keep_button_events, 1U);
  EXPECT_TRUE(state.lane_keep_button_pressed);
  send_button(0U, false);
  EXPECT_FALSE(parser.snapshot(now, std::chrono::milliseconds(100)).lane_keep_button_pressed);
}

TEST(VehicleStateParser, ParsesLaneKeepFromAlternateButtons) {
  using namespace ioniq5_ecan;
  VehicleStateParser parser(0, 2, true);
  const TimePoint now = SteadyClock::now();
  std::array<uint8_t, 16> buttons{};
  set_signal(buttons, 39, 1, 1U, ByteOrder::LittleEndian);
  EXPECT_TRUE(
    parser.update(frame_with_crc(HyundaiCanFdCodec::kCruiseButtonsAltAddress, buttons, now)));
  const VehicleStateData state = parser.snapshot(now, std::chrono::milliseconds(100));
  EXPECT_EQ(state.lane_keep_button_events, 1U);
  EXPECT_TRUE(state.lane_keep_button_pressed);
}

TEST(VehicleStateParser, EcanOnlyProfileRejectsCameraBusScc) {
  using namespace ioniq5_ecan;
  const TimePoint now = SteadyClock::now();
  std::array<uint8_t, 32> scc{};
  CanFrame camera_frame = frame_with_crc(HyundaiCanFdCodec::kSccControlAddress, scc, now);
  camera_frame.bus = 2U;

  VehicleStateParser ecan_only;
  EXPECT_FALSE(ecan_only.update(camera_frame));

  VehicleStateParser legacy_camera_profile(0U, 2U, false, true);
  EXPECT_TRUE(legacy_camera_profile.update(camera_frame));
}

}  // namespace
