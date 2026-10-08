#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>

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
    std::array<uint8_t, 32>{0x60, 0x64, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x14, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0x09, 0x14, 0x43, 0x32, 0x32, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

  codec.reset_counters();
  expect_payload(
    codec.make_scc_control(0.0, 0.0, false, false, false, 30.0, 1.0),
    std::array<uint8_t, 32>{0x0F, 0x37, 0x00, 0x0A, 0x00, 0x30, 0x64, 0x00, 0x04, 0x00, 0x00,
                            0x04, 0x1E, 0x08, 0x00, 0x00, 0xFF, 0xF3, 0x3F, 0x1E, 0x0A, 0x00,
                            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
}

TEST(HyundaiCanFdCodec, SaturatesAtCanRepresentationAndMaintainsChecksum) {
  using namespace ioniq5_ecan;
  HyundaiCanFdCodec codec;
  const CanFrame lfa = codec.make_lfa(2000, true, true);
  EXPECT_TRUE(HyundaiCanFdCodec::checksum_valid(lfa));
  std::array<uint8_t, 16> data{};
  std::copy_n(lfa.data.begin(), 16, data.begin());
  EXPECT_EQ(static_cast<int>(get_signal(data, 41, 11, ByteOrder::LittleEndian)) - 1024, 1021);

  const CanFrame scc = codec.make_scc_control(20.0, -20.0, true, false, false, 300.0, 20.0);
  EXPECT_TRUE(HyundaiCanFdCodec::checksum_valid(scc));
  std::array<uint8_t, 32> scc_data{};
  std::copy_n(scc.data.begin(), 32, scc_data.begin());
  EXPECT_EQ(get_signal(scc_data, 140, 11, ByteOrder::LittleEndian), 2047U);  // aReqRaw +10.24
  EXPECT_EQ(get_signal(scc_data, 128, 11, ByteOrder::LittleEndian), 0U);     // aReqValue -10.23
  EXPECT_EQ(get_signal(scc_data, 158, 7, ByteOrder::BigEndian), 127U);       // jerk 12.7
  EXPECT_EQ(get_signal(scc_data, 166, 7, ByteOrder::BigEndian), 127U);
  EXPECT_EQ(get_signal(scc_data, 103, 8, ByteOrder::BigEndian), 255U);       // set speed
}

TEST(HyundaiCanFdCodec, PreservesUnownedStockSccFields) {
  using namespace ioniq5_ecan;
  HyundaiCanFdCodec codec;
  CanFrame stock;
  stock.address = HyundaiCanFdCodec::kSccControlAddress;
  stock.bus = 0;
  stock.fd = true;
  stock.size = 32;
  stock.data[2] = 41U;
  stock.data[25] = 0x5AU;
  stock.data[31] = 0xA5U;

  codec.set_scc_control_template(stock);
  ASSERT_TRUE(codec.has_scc_control_template());
  const CanFrame scc = codec.make_scc_control(0.7, 0.7, true, false, false, 30.0, 5.0);
  EXPECT_EQ(scc.data[2], 42U);
  EXPECT_EQ(scc.data[25], 0x5AU);
  EXPECT_EQ(scc.data[31], 0xA5U);
  EXPECT_TRUE(HyundaiCanFdCodec::checksum_valid(scc));

  codec.clear_scc_control_template();
  EXPECT_FALSE(codec.has_scc_control_template());
}

}  // namespace
