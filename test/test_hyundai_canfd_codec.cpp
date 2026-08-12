#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>

#include "ioniq5_ecan/bit_codec.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"

namespace {

template <std::size_t N>
void expect_payload(const ioniq5_ecan::CanFrame& frame, const std::array<uint8_t, N>& expected) {
  ASSERT_EQ(frame.size, N);
  for (std::size_t i = 0; i < N; ++i) EXPECT_EQ(frame.data[i], expected[i]) << i;
}

TEST(BitCodec, RoundTripsIntelAndMotorolaSignals) {
  using namespace ioniq5_ecan;
  std::array<uint8_t, 16> data{};
  set_signal(data, 19, 13, 0x13A5, ByteOrder::LittleEndian);
  set_signal(data, 55, 8, 0x64, ByteOrder::BigEndian);
  EXPECT_EQ(get_signal(data, 19, 13, ByteOrder::LittleEndian), 0x13A5U);
  EXPECT_EQ(get_signal(data, 55, 8, ByteOrder::BigEndian), 0x64U);
  EXPECT_EQ(sign_extend(0x7FF, 12), 2047);
  EXPECT_EQ(sign_extend(0x800, 12), -2048);
}

TEST(HyundaiCanFdCodec, MatchesPinnedOpenDbcGoldenFrames) {
  using namespace ioniq5_ecan;
  HyundaiCanFdCodec codec;
  expect_payload(codec.make_lfa(100, true, true),
                 std::array<uint8_t, 16>{0xBE, 0xBF, 0x00, 0x02, 0x80, 0xC8, 0x18, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x64, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(codec.make_lfa(0, false, false),
                 std::array<uint8_t, 16>{0x05, 0x10, 0x00, 0x02, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x64, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(codec.make_lfa_cluster(true),
                 std::array<uint8_t, 16>{0xE9, 0x53, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(codec.make_fca_warning(),
                 std::array<uint8_t, 16>{0x2D, 0x84, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0xFF, 0xFC,
                                         0x09, 0x00, 0x00, 0x00, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(
    codec.make_scc_control(0.5, 0.1, true, false, false, 30.0, 5.0),
    std::array<uint8_t, 32>{0x83, 0x45, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x14, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0x09, 0x14, 0x43, 0x1E, 0x32, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(
    codec.make_scc_control(0.0, 0.0, false, false, false, 30.0, 1.0),
    std::array<uint8_t, 32>{0x0F, 0x37, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x04, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0xFF, 0xF3, 0x3F, 0x1E, 0x0A, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
}

TEST(HyundaiCanFdCodec, ClampsToPandaHardBoundsAndMaintainsChecksum) {
  using namespace ioniq5_ecan;
  HyundaiCanFdCodec codec;
  const CanFrame lfa = codec.make_lfa(999, true, true);
  EXPECT_TRUE(HyundaiCanFdCodec::checksum_valid(lfa));
  std::array<uint8_t, 16> data{};
  std::copy_n(lfa.data.begin(), 16, data.begin());
  EXPECT_EQ(static_cast<int>(get_signal(data, 41, 11, ByteOrder::LittleEndian)) - 1024, 270);

  const CanFrame scc = codec.make_scc_control(9.0, -9.0, true, false, false, 30.0, 20.0);
  EXPECT_TRUE(HyundaiCanFdCodec::checksum_valid(scc));
}

}  // namespace
