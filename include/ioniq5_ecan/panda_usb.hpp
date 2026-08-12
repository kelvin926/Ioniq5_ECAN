#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

#include "ioniq5_ecan/types.hpp"

struct libusb_context;
struct libusb_device_handle;

namespace ioniq5_ecan {

struct PandaUsbConfig {
  std::string serial;
  int nominal_bitrate_kbps{500};
  int data_bitrate_kbps{2000};
  int read_timeout_ms{20};
  int write_timeout_ms{10};
};

class PandaUsb {
 public:
  static constexpr uint16_t kSafetySilent = 0;
  static constexpr uint16_t kSafetyNoOutput = 19;
  static constexpr uint16_t kSafetyHyundaiCanFd = 28;
  static constexpr uint16_t kHyundaiEv = 1;
  static constexpr uint16_t kHyundaiLongitudinal = 4;
  static constexpr uint16_t kHyundaiCameraScc = 8;
  static constexpr uint16_t kHyundaiAlternateButtons = 32;
  static constexpr uint16_t kIoniq5Hda1PassiveParam = kHyundaiEv | kHyundaiCameraScc;
  static constexpr uint16_t kIoniq5Hda1LongParam =
    kHyundaiEv | kHyundaiLongitudinal | kHyundaiCameraScc;
  static constexpr uint32_t kPinnedHealthPacketVersion = 0x290DAE03U;
  static constexpr uint32_t kPinnedCanPacketVersion = 0x75ABF276U;
  static constexpr uint8_t kRedPandaHardwareType = 0x07U;

  explicit PandaUsb(PandaUsbConfig config = {});
  ~PandaUsb();
  PandaUsb(const PandaUsb&) = delete;
  PandaUsb& operator=(const PandaUsb&) = delete;

  void connect();
  void disconnect();
  bool connected() const;
  const std::string& serial() const;

  void configure_can();
  void set_safety_mode(uint16_t mode, uint16_t param = 0);
  void send_heartbeat(bool engaged);
  PandaHealth health();
  std::vector<CanFrame> receive();
  void send(const std::vector<CanFrame>& frames);
  void send(const CanFrame& frame);

  static std::vector<uint8_t> pack_frames(const std::vector<CanFrame>& frames);
  static std::vector<CanFrame> unpack_frames(std::vector<uint8_t>& carry, const uint8_t* bytes,
                                             std::size_t size, TimePoint received_at);

 private:
  static void pack_frames_into(const std::vector<CanFrame>& frames, std::vector<uint8_t>& output);
  void control_write(uint8_t request, uint16_t value, uint16_t index);
  int control_read(uint8_t request, uint16_t value, uint16_t index, void* data, uint16_t size);
  void ensure_connected() const;

  PandaUsbConfig config_;
  libusb_context* context_{nullptr};
  libusb_device_handle* handle_{nullptr};
  std::atomic<bool> handle_open_{false};
  std::atomic<bool> ready_{false};
  std::string serial_;
  std::vector<uint8_t> receive_carry_;
  std::vector<uint8_t> transmit_buffer_;
  mutable std::mutex control_mutex_;
  mutable std::mutex write_mutex_;
};

}  // namespace ioniq5_ecan
