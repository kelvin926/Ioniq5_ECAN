#include "ioniq5_ecan/panda_usb.hpp"

#include <libusb.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace ioniq5_ecan {
namespace {

constexpr uint8_t kVendorOut = 0x40;
constexpr uint8_t kVendorIn = 0xC0;
constexpr uint8_t kCanWriteEndpoint = 0x03;
constexpr uint8_t kCanReadEndpoint = 0x81;

#pragma pack(push, 1)
struct PandaHealthPacket {
  uint32_t uptime;
  uint32_t voltage;
  uint32_t current;
  uint32_t safety_tx_blocked;
  uint32_t safety_rx_invalid;
  uint32_t tx_buffer_overflow;
  uint32_t rx_buffer_overflow;
  uint32_t faults;
  uint8_t ignition_line;
  uint8_t ignition_can;
  uint8_t controls_allowed;
  uint8_t harness_status;
  uint8_t safety_mode;
  uint16_t safety_param;
  uint8_t fault_status;
  uint8_t power_save_enabled;
  uint8_t heartbeat_lost;
  uint16_t alternative_experience;
  float interrupt_load;
  uint8_t fan_power;
  uint8_t safety_rx_checks_invalid;
  uint16_t spi_error_count;
  uint16_t sbu1_voltage_mv;
  uint16_t sbu2_voltage_mv;
  uint8_t som_reset_triggered;
  uint16_t sound_output_level;
  float temperature;
};

struct PandaCanHealthPacket {
  uint8_t bus_off;
  uint32_t bus_off_count;
  uint8_t error_warning;
  uint8_t error_passive;
  uint8_t last_error;
  uint8_t last_stored_error;
  uint8_t last_data_error;
  uint8_t last_data_stored_error;
  uint8_t receive_error_count;
  uint8_t transmit_error_count;
  uint32_t total_error_count;
  uint32_t total_tx_lost_count;
  uint32_t total_rx_lost_count;
  uint32_t total_tx_count;
  uint32_t total_rx_count;
  uint32_t total_forwarded_count;
  uint32_t total_tx_checksum_error_count;
  uint16_t can_speed;
  uint16_t can_data_speed;
  uint8_t canfd_enabled;
  uint8_t brs_enabled;
  uint8_t canfd_non_iso;
  uint32_t irq0_call_rate;
  uint32_t irq1_call_rate;
  uint32_t irq2_call_rate;
  uint32_t can_core_reset_count;
};
#pragma pack(pop)

static_assert(sizeof(PandaHealthPacket) == 63, "Panda health ABI drift");
static_assert(sizeof(PandaCanHealthPacket) == 64, "Panda CAN health ABI drift");

bool panda_usb_id(uint16_t vendor, uint16_t product) {
  const bool vendor_ok = vendor == 0xBBAA || vendor == 0x3801;
  const bool product_ok = product == 0xDDEE || product == 0xDDCC;
  return vendor_ok && product_ok;
}

std::string usb_serial(libusb_device_handle* handle, uint8_t index) {
  if (index == 0) return {};
  std::array<unsigned char, 128> bytes{};
  const int count =
    libusb_get_string_descriptor_ascii(handle, index, bytes.data(), static_cast<int>(bytes.size()));
  return count > 0 ? std::string(reinterpret_cast<char*>(bytes.data()), count) : std::string{};
}

}  // namespace

PandaUsb::PandaUsb(PandaUsbConfig config) : config_(std::move(config)) {}

PandaUsb::~PandaUsb() { disconnect(); }

void PandaUsb::connect() {
  if (connected()) return;
  // A failed transfer clears ready_ but deliberately leaves cleanup to the reconnect owner.
  // Always close the stale handle/context before starting a new libusb session.
  disconnect();
  if (libusb_init(&context_) != 0) {
    throw std::runtime_error("libusb initialization failed");
  }

  libusb_device** devices = nullptr;
  const ssize_t count = libusb_get_device_list(context_, &devices);
  if (count < 0) {
    disconnect();
    throw std::runtime_error("failed to enumerate USB devices");
  }

  for (ssize_t i = 0; i < count && handle_ == nullptr; ++i) {
    libusb_device_descriptor descriptor{};
    if (libusb_get_device_descriptor(devices[i], &descriptor) != 0 ||
        !panda_usb_id(descriptor.idVendor, descriptor.idProduct)) {
      continue;
    }
    libusb_device_handle* candidate = nullptr;
    if (libusb_open(devices[i], &candidate) != 0 || candidate == nullptr) continue;
    const std::string candidate_serial = usb_serial(candidate, descriptor.iSerialNumber);
    if (!config_.serial.empty() && candidate_serial != config_.serial) {
      libusb_close(candidate);
      continue;
    }
    handle_ = candidate;
    serial_ = candidate_serial;
  }
  libusb_free_device_list(devices, 1);

  if (handle_ == nullptr) {
    disconnect();
    throw std::runtime_error("requested Red Panda was not found");
  }
  libusb_set_auto_detach_kernel_driver(handle_, 1);
  if (libusb_claim_interface(handle_, 0) != 0) {
    disconnect();
    throw std::runtime_error("failed to claim Red Panda USB interface; check udev rules");
  }
  handle_open_ = true;
  receive_carry_.clear();

  try {
    control_write(0xC0, 0, 0);  // reset CAN communications
    std::array<uint32_t, 2> versions{};
    if (control_read(0xDD, 0, 0, versions.data(), sizeof(versions)) !=
          static_cast<int>(sizeof(versions)) ||
        versions[0] != kPinnedHealthPacketVersion || versions[1] != kPinnedCanPacketVersion) {
      throw std::runtime_error("Panda protocol version mismatch; flash the pinned firmware");
    }
    uint8_t hardware_type = 0;
    if (control_read(0xC1, 0, 0, &hardware_type, sizeof(hardware_type)) != 1 ||
        hardware_type != kRedPandaHardwareType) {
      throw std::runtime_error("connected Panda is not a Red Panda");
    }
    control_write(0xE7, 0, 0);  // disable power saving
    configure_can();
    set_safety_mode(kSafetyNoOutput, 0);
    send_heartbeat(false);
    ready_ = true;
  } catch (...) {
    disconnect();
    throw;
  }
}

void PandaUsb::disconnect() {
  ready_ = false;
  std::scoped_lock<std::mutex, std::mutex> io_lock(write_mutex_, control_mutex_);
  const bool interface_claimed = handle_open_.exchange(false);
  if (handle_ != nullptr) {
    if (interface_claimed) libusb_release_interface(handle_, 0);
    libusb_close(handle_);
    handle_ = nullptr;
  }
  if (context_ != nullptr) {
    libusb_exit(context_);
    context_ = nullptr;
  }
  receive_carry_.clear();
}

bool PandaUsb::connected() const { return ready_.load(); }
const std::string& PandaUsb::serial() const { return serial_; }

void PandaUsb::configure_can() {
  for (uint16_t bus = 0; bus < 3; ++bus) {
    control_write(0xDE, bus, static_cast<uint16_t>(config_.nominal_bitrate_kbps * 10));
    control_write(0xF9, bus, static_cast<uint16_t>(config_.data_bitrate_kbps * 10));
    control_write(0xE8, bus, 1);  // automatic classic CAN/CAN-FD switching
  }
}

void PandaUsb::set_safety_mode(uint16_t mode, uint16_t param) {
  if (mode == 17U) {
    throw std::invalid_argument("SAFETY_ALLOUTPUT is forbidden by this driver");
  }
  control_write(0xDC, mode, param);
}

void PandaUsb::send_heartbeat(bool engaged) { control_write(0xF3, engaged ? 1U : 0U, 0); }

PandaHealth PandaUsb::health() {
  PandaHealthPacket packet{};
  const int count = control_read(0xD2, 0, 0, &packet, sizeof(packet));
  if (count != static_cast<int>(sizeof(packet))) {
    throw std::runtime_error("Panda health packet ABI mismatch; reflash pinned firmware");
  }

  PandaHealth result;
  result.connected = true;
  result.controls_allowed = packet.controls_allowed != 0;
  result.heartbeat_lost = packet.heartbeat_lost != 0;
  result.safety_rx_checks_invalid = packet.safety_rx_checks_invalid != 0;
  result.faults = packet.faults;
  result.fault_status = packet.fault_status;
  result.harness_status = packet.harness_status;
  result.safety_mode = packet.safety_mode;
  result.safety_param = packet.safety_param;
  result.safety_tx_blocked = packet.safety_tx_blocked;
  result.safety_rx_invalid = packet.safety_rx_invalid;
  result.tx_buffer_overflow = packet.tx_buffer_overflow;
  result.rx_buffer_overflow = packet.rx_buffer_overflow;

  for (uint16_t bus = 0; bus < 3; ++bus) {
    PandaCanHealthPacket can{};
    if (control_read(0xC2, bus, 0, &can, sizeof(can)) == static_cast<int>(sizeof(can))) {
      result.bus_off = result.bus_off || can.bus_off != 0 || can.error_passive != 0;
    }
  }
  result.updated_at = SteadyClock::now();
  return result;
}

std::vector<CanFrame> PandaUsb::receive() {
  ensure_connected();
  std::array<uint8_t, 16384> buffer{};
  int transferred = 0;
  const int status =
    libusb_bulk_transfer(handle_, kCanReadEndpoint, buffer.data(), static_cast<int>(buffer.size()),
                         &transferred, config_.read_timeout_ms);
  if (status == LIBUSB_ERROR_TIMEOUT) return {};
  if (status != 0) {
    ready_ = false;
    throw std::runtime_error(std::string("Red Panda CAN read failed: ") +
                             libusb_error_name(status));
  }
  return unpack_frames(receive_carry_, buffer.data(), static_cast<std::size_t>(transferred),
                       SteadyClock::now());
}

void PandaUsb::send(const std::vector<CanFrame>& frames) {
  ensure_connected();
  if (frames.empty()) return;
  std::lock_guard<std::mutex> lock(write_mutex_);
  ensure_connected();
  pack_frames_into(frames, transmit_buffer_);
  std::size_t position = 0;
  while (position < transmit_buffer_.size()) {
    int transferred = 0;
    const int status = libusb_bulk_transfer(
      handle_, kCanWriteEndpoint, transmit_buffer_.data() + position,
      static_cast<int>(transmit_buffer_.size() - position), &transferred, config_.write_timeout_ms);
    if (status != 0 || transferred <= 0) {
      if (status != LIBUSB_ERROR_TIMEOUT) ready_ = false;
      throw std::runtime_error(std::string("Red Panda CAN write failed: ") +
                               libusb_error_name(status));
    }
    position += static_cast<std::size_t>(transferred);
  }
}

void PandaUsb::send(const CanFrame& frame) { send(std::vector<CanFrame>{frame}); }

void PandaUsb::control_write(uint8_t request, uint16_t value, uint16_t index) {
  std::lock_guard<std::mutex> lock(control_mutex_);
  ensure_connected();
  const int status =
    libusb_control_transfer(handle_, kVendorOut, request, value, index, nullptr, 0, 1000);
  if (status < 0) {
    ready_ = false;
    throw std::runtime_error(std::string("Red Panda control write failed: ") +
                             libusb_error_name(status));
  }
}

int PandaUsb::control_read(uint8_t request, uint16_t value, uint16_t index, void* data,
                           uint16_t size) {
  std::lock_guard<std::mutex> lock(control_mutex_);
  ensure_connected();
  const int status = libusb_control_transfer(handle_, kVendorIn, request, value, index,
                                             static_cast<unsigned char*>(data), size, 1000);
  if (status < 0) {
    ready_ = false;
    throw std::runtime_error(std::string("Red Panda control read failed: ") +
                             libusb_error_name(status));
  }
  return status;
}

void PandaUsb::ensure_connected() const {
  if (!handle_open_.load() || handle_ == nullptr) {
    throw std::runtime_error("Red Panda is not connected");
  }
}

}  // namespace ioniq5_ecan
