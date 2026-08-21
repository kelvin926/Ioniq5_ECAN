#pragma once

#include <chrono>
#include <mutex>

#include "ioniq5_ecan/types.hpp"

namespace ioniq5_ecan {

class VehicleStateParser {
 public:
  explicit VehicleStateParser(uint8_t ecan_bus = 0, uint8_t camera_bus = 2,
                              bool alternate_buttons = false);

  bool update(const CanFrame& frame);
  VehicleStateData snapshot(TimePoint now, std::chrono::milliseconds critical_timeout) const;
  uint64_t checksum_failures() const;
  uint64_t malformed_frames() const;

 private:
  bool require_crc(const CanFrame& frame);
  void parse_dynamics(const CanFrame& frame);
  void parse_steering(const CanFrame& frame);
  void parse_mdps(const CanFrame& frame);
  void parse_wheel_speeds(const CanFrame& frame);
  void parse_tcs(const CanFrame& frame);
  void parse_accelerator(const CanFrame& frame);
  void parse_buttons(const CanFrame& frame, bool alternate);
  void parse_scc(const CanFrame& frame);

  uint8_t ecan_bus_;
  uint8_t camera_bus_;
  bool alternate_buttons_;
  mutable std::mutex mutex_;
  VehicleStateData state_;
  uint8_t previous_button_{0};
  bool previous_lane_keep_button_{false};
  bool have_previous_angle_{false};
  double previous_angle_deg_{0.0};
  TimePoint previous_angle_time_{};
  TimePoint steering_time_{};
  TimePoint mdps_time_{};
  TimePoint wheel_time_{};
  TimePoint tcs_time_{};
  uint64_t checksum_failures_{0};
  uint64_t malformed_frames_{0};
};

}  // namespace ioniq5_ecan
