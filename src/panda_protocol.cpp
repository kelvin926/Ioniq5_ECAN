#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ioniq5_ecan/panda_usb.hpp"

namespace ioniq5_ecan {
namespace {

constexpr std::size_t kCanHeaderSize = 6;
constexpr std::array<uint8_t, 16> kDlcToLength{0, 1,  2,  3,  4,  5,  6,  7,
                                               8, 12, 16, 20, 24, 32, 48, 64};

uint8_t length_to_dlc(uint8_t length) {
  const auto it = std::find(kDlcToLength.begin(), kDlcToLength.end(), length);
  if (it == kDlcToLength.end()) {
    throw std::invalid_argument("CAN-FD payload length is not representable by a DLC");
  }
  return static_cast<uint8_t>(std::distance(kDlcToLength.begin(), it));
}

uint8_t xor_checksum(const uint8_t* data, std::size_t size) {
  uint8_t result = 0;
  for (std::size_t i = 0; i < size; ++i) result ^= data[i];
  return result;
}

}  // namespace

std::vector<uint8_t> PandaUsb::pack_frames(const std::vector<CanFrame>& frames) {
  std::vector<uint8_t> output;
  pack_frames_into(frames, output);
  return output;
}

void PandaUsb::pack_frames_into(const std::vector<CanFrame>& frames, std::vector<uint8_t>& output) {
  output.clear();
  std::size_t required = 0;
  for (const CanFrame& frame : frames) required += kCanHeaderSize + frame.size;
  output.reserve(required);
  for (const CanFrame& frame : frames) {
    if (frame.bus > 7U || frame.address >= (1U << 29U)) {
      throw std::invalid_argument("invalid Panda CAN address or bus");
    }
    const uint8_t dlc = length_to_dlc(frame.size);
    const std::size_t start = output.size();
    output.resize(start + kCanHeaderSize + frame.size, 0);
    output[start] = static_cast<uint8_t>((dlc << 4U) | (frame.bus << 1U) | (frame.fd ? 1U : 0U));
    const uint32_t word = (frame.address << 3U) | (frame.address >= 0x800U ? 4U : 0U);
    output[start + 1] = static_cast<uint8_t>(word & 0xFFU);
    output[start + 2] = static_cast<uint8_t>((word >> 8U) & 0xFFU);
    output[start + 3] = static_cast<uint8_t>((word >> 16U) & 0xFFU);
    output[start + 4] = static_cast<uint8_t>((word >> 24U) & 0xFFU);
    std::copy_n(frame.data.begin(), frame.size, output.begin() + start + kCanHeaderSize);
    output[start + 5] = xor_checksum(output.data() + start, kCanHeaderSize + frame.size);
  }
}

std::vector<CanFrame> PandaUsb::unpack_frames(std::vector<uint8_t>& carry, const uint8_t* bytes,
                                              std::size_t size, TimePoint received_at) {
  if (bytes != nullptr && size != 0U) carry.insert(carry.end(), bytes, bytes + size);
  std::vector<CanFrame> frames;
  std::size_t position = 0;
  while (carry.size() - position >= kCanHeaderSize) {
    const uint8_t dlc = carry[position] >> 4U;
    if (dlc >= kDlcToLength.size()) {
      carry.clear();
      throw std::runtime_error("invalid DLC in Panda CAN packet");
    }
    const uint8_t length = kDlcToLength[dlc];
    const std::size_t packet_size = kCanHeaderSize + length;
    if (carry.size() - position < packet_size) break;
    if (xor_checksum(carry.data() + position, packet_size) != 0U) {
      carry.clear();
      throw std::runtime_error("Panda USB CAN packet checksum failure");
    }

    const uint32_t word = static_cast<uint32_t>(carry[position + 1]) |
                          (static_cast<uint32_t>(carry[position + 2]) << 8U) |
                          (static_cast<uint32_t>(carry[position + 3]) << 16U) |
                          (static_cast<uint32_t>(carry[position + 4]) << 24U);
    CanFrame frame;
    frame.fd = (carry[position] & 1U) != 0;
    frame.bus = static_cast<uint8_t>((carry[position] >> 1U) & 7U);
    frame.rejected = (carry[position + 1] & 1U) != 0;
    frame.returned = (carry[position + 1] & 2U) != 0;
    frame.address = word >> 3U;
    frame.size = length;
    frame.received_at = received_at;
    std::copy_n(carry.begin() + position + kCanHeaderSize, length, frame.data.begin());
    frames.push_back(frame);
    position += packet_size;
  }
  carry.erase(carry.begin(), carry.begin() + static_cast<std::ptrdiff_t>(position));
  return frames;
}

}  // namespace ioniq5_ecan
