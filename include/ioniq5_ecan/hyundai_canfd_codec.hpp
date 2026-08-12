#pragma once

#include <array>
#include <cstdint>

#include "ioniq5_ecan/types.hpp"

namespace ioniq5_ecan {

class HyundaiCanFdCodec {
 public:
  static constexpr uint32_t kWheelSpeedsAddress = 0x0A0;
  static constexpr uint32_t kSteeringSensorsAddress = 0x125;
  static constexpr uint32_t kLfaAddress = 0x12A;
  static constexpr uint32_t kSccControlAddress = 0x1A0;
  static constexpr uint32_t kCruiseButtonsAltAddress = 0x1AA;
  static constexpr uint32_t kMdpsAddress = 0x0EA;
  static constexpr uint32_t kTcsAddress = 0x175;
  static constexpr uint32_t kCruiseButtonsAddress = 0x1CF;
  static constexpr uint32_t kLfaClusterAddress = 0x1E0;
  static constexpr uint32_t kAcceleratorAddress = 0x035;
  static constexpr uint32_t kFcaWarningAddress = 0x160;

  static uint16_t checksum(uint32_t address, const uint8_t* data, std::size_t size);
  static bool checksum_valid(const CanFrame& frame);

  CanFrame make_lfa(int torque, bool enabled, bool torque_request, uint8_t bus = 0);
  CanFrame make_scc_control(double accel_raw_mps2, double accel_value_mps2, bool enabled,
                            bool stopping, bool gas_override, double set_speed_kph,
                            double jerk_mps3, uint8_t bus = 0);
  CanFrame make_lfa_cluster(bool enabled, uint8_t bus = 0);
  CanFrame make_fca_warning(uint8_t bus = 0);

  void reset_counters(uint8_t lfa = 0, uint8_t scc = 0, uint8_t cluster = 0,
                      uint8_t fca_warning = 0);

 private:
  uint8_t lfa_counter_{0};
  uint8_t scc_counter_{0};
  uint8_t cluster_counter_{0};
  uint8_t fca_warning_counter_{0};
};

}  // namespace ioniq5_ecan
