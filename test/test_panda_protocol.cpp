#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ioniq5_ecan/panda_usb.hpp"

namespace {

TEST(PandaProtocol, PackAndPartialUnpackRoundTrip) {
  using namespace ioniq5_ecan;
  CanFrame source;
  source.address = 0x12A;
  source.bus = 0;
  source.fd = true;
  source.size = 16;
  for (uint8_t i = 0; i < source.size; ++i) source.data[i] = i;

  const std::vector<uint8_t> bytes = PandaUsb::pack_frames({source});
  ASSERT_EQ(bytes.size(), 22U);
  std::vector<uint8_t> carry;
  EXPECT_TRUE(PandaUsb::unpack_frames(carry, bytes.data(), 5, SteadyClock::now()).empty());
  const auto frames =
    PandaUsb::unpack_frames(carry, bytes.data() + 5, bytes.size() - 5, SteadyClock::now());
  ASSERT_EQ(frames.size(), 1U);
  EXPECT_EQ(frames[0].address, source.address);
  EXPECT_EQ(frames[0].bus, source.bus);
  EXPECT_TRUE(frames[0].fd);
  EXPECT_FALSE(frames[0].extended);
  EXPECT_EQ(frames[0].size, source.size);
  EXPECT_TRUE(carry.empty());
}

TEST(PandaProtocol, PreservesExtendedIdBitIndependentlyOfNumericAddress) {
  using namespace ioniq5_ecan;
  CanFrame source;
  source.address = 0x123;
  source.extended = true;
  source.size = 8;

  const std::vector<uint8_t> bytes = PandaUsb::pack_frames({source});
  std::vector<uint8_t> carry;
  const auto frames =
    PandaUsb::unpack_frames(carry, bytes.data(), bytes.size(), SteadyClock::now());
  ASSERT_EQ(frames.size(), 1U);
  EXPECT_EQ(frames[0].address, source.address);
  EXPECT_TRUE(frames[0].extended);
}

TEST(PandaProtocol, RejectsOutOfRangeStandardId) {
  using namespace ioniq5_ecan;
  CanFrame frame;
  frame.address = 0x800;
  frame.size = 8;
  EXPECT_THROW(PandaUsb::pack_frames({frame}), std::invalid_argument);
}

TEST(PandaProtocol, RejectsCorruptUsbChecksum) {
  using namespace ioniq5_ecan;
  CanFrame frame;
  frame.address = 0x1A0;
  frame.fd = true;
  frame.size = 32;
  std::vector<uint8_t> bytes = PandaUsb::pack_frames({frame});
  bytes.back() ^= 1U;
  std::vector<uint8_t> carry;
  EXPECT_THROW(PandaUsb::unpack_frames(carry, bytes.data(), bytes.size(), SteadyClock::now()),
               std::runtime_error);
}

TEST(PandaProtocol, ForbidsUnsupportedPayloadLength) {
  using namespace ioniq5_ecan;
  CanFrame frame;
  frame.address = 1;
  frame.size = 9;
  EXPECT_THROW(PandaUsb::pack_frames({frame}), std::invalid_argument);
}

}  // namespace
