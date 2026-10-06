#include <gtest/gtest.h>
#include <libusb.h>

#include "ioniq5_ecan/panda_usb.hpp"

namespace {
using namespace ioniq5_ecan;

TEST(PandaUsbRead, PartialTimeoutPreservesPacketBoundary) {
  CanFrame frame;
  frame.address = 0xEA;
  frame.bus = 0;
  frame.fd = true;
  frame.size = 24;
  const auto bytes = PandaUsb::pack_frames({frame});
  std::vector<uint8_t> carry;
  EXPECT_TRUE(PandaUsb::decode_bulk_read(carry, bytes.data(), bytes.size(),
    LIBUSB_ERROR_TIMEOUT, 9, SteadyClock::now()).empty());
  EXPECT_EQ(carry.size(), 9U);
  const auto decoded = PandaUsb::decode_bulk_read(carry, bytes.data() + 9,
    bytes.size() - 9, 0, static_cast<int>(bytes.size() - 9), SteadyClock::now());
  ASSERT_EQ(decoded.size(), 1U);
  EXPECT_EQ(decoded[0].address, 0xEAU);
  EXPECT_TRUE(carry.empty());
}

TEST(PandaUsbRead, EmptyTimeoutDoesNotDiscardExistingCarry) {
  std::vector<uint8_t> carry{0xAA, 0xBB};
  uint8_t bytes{};
  EXPECT_TRUE(PandaUsb::decode_bulk_read(carry, &bytes, 1, LIBUSB_ERROR_TIMEOUT,
    0, SteadyClock::now()).empty());
  EXPECT_EQ(carry, (std::vector<uint8_t>{0xAA, 0xBB}));
}

TEST(PandaUsbRead, HardTransferErrorDoesNotDecodePartialData) {
  std::vector<uint8_t> carry;
  uint8_t bytes{};
  EXPECT_THROW(PandaUsb::decode_bulk_read(carry, &bytes, 1, LIBUSB_ERROR_NO_DEVICE,
    1, SteadyClock::now()), std::runtime_error);
}

TEST(PandaUsbRead, InvalidTransferLengthIsRejected) {
  std::vector<uint8_t> carry;
  uint8_t bytes{};
  for (int count : {-1, 2}) {
    EXPECT_THROW(PandaUsb::decode_bulk_read(carry, &bytes, 1, 0, count,
      SteadyClock::now()), std::runtime_error);
  }
}

TEST(PandaUsbRead, StartupDrainRequiresSuccessfulShortTransfer) {
  EXPECT_TRUE(PandaUsb::initial_receive_boundary(16384, 0, 0));
  EXPECT_TRUE(PandaUsb::initial_receive_boundary(16384, 0, 63));
  EXPECT_TRUE(PandaUsb::initial_receive_boundary(16384, 0, 128));
  EXPECT_FALSE(PandaUsb::initial_receive_boundary(16384, 0, 16384));
  for (int count : {0, 63, 128, 16384}) {
    EXPECT_FALSE(PandaUsb::initial_receive_boundary(16384, LIBUSB_ERROR_TIMEOUT, count));
  }
}

TEST(PandaUsbRead, StartupDrainRejectsHardErrorsAndInvalidCounts) {
  EXPECT_THROW(PandaUsb::initial_receive_boundary(16384, LIBUSB_ERROR_NO_DEVICE, 0),
               std::runtime_error);
  EXPECT_THROW(PandaUsb::initial_receive_boundary(16384, 0, -1), std::runtime_error);
  EXPECT_THROW(PandaUsb::initial_receive_boundary(16384, 0, 16385), std::runtime_error);
  EXPECT_THROW(PandaUsb::initial_receive_boundary(0, 0, 0), std::runtime_error);
  EXPECT_THROW(PandaUsb::initial_receive_boundary(63, 0, 0), std::runtime_error);
}
}  // namespace
