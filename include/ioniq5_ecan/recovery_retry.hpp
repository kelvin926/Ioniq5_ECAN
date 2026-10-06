#pragma once

#include <chrono>
#include <cstdint>

#include "ioniq5_ecan/types.hpp"

namespace ioniq5_ecan {

// Reinstall only the empty physical-button listener after an unarmed startup
// failure. Explicit OFF, shutdown and faults requiring rearm stay plain NO_OUTPUT.
inline uint16_t recovery_standby_param(uint16_t profile, bool preserve_session,
                                      bool listener_enabled, bool inhibited, bool rearm_required) {
  return preserve_session || (listener_enabled && !inhibited && !rearm_required) ? profile : 0U;
}

// Schedules restoration only. It never authorizes actuator output or clears a fault.
class RecoveryRetry {
 public:
  void request(TimePoint now) {
    if (pending_) return;
    pending_ = true;
    failures_ = 0;
    delay_ = std::chrono::seconds(1);
    next_attempt_ = now;
  }

  bool due(TimePoint now) const { return pending_ && now >= next_attempt_; }
  bool pending() const { return pending_; }
  uint64_t failures() const { return failures_; }

  void failed(TimePoint now) {
    if (!pending_) return;
    ++failures_;
    next_attempt_ = now + delay_;
    const auto maximum = std::chrono::seconds(30);
    delay_ = delay_ >= maximum - delay_ ? maximum : delay_ + delay_;
  }

  void restored() { pending_ = false; }

 private:
  bool pending_{false};
  uint64_t failures_{0};
  TimePoint next_attempt_{};
  std::chrono::seconds delay_{1};
};

}  // namespace ioniq5_ecan
