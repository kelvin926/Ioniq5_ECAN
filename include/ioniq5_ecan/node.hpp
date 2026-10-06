#pragma once

#include <diagnostic_msgs/DiagnosticArray.h>
#include <ros/ros.h>
#include <std_srvs/SetBool.h>

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "ioniq5_ecan/ActuationCommand.h"
#include "ioniq5_ecan/RawCanFrame.h"
#include "ioniq5_ecan/VehicleState.h"
#include "ioniq5_ecan/command_adapter.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
#include "ioniq5_ecan/panda_usb.hpp"
#include "ioniq5_ecan/recovery_retry.hpp"
#include "ioniq5_ecan/safety_supervisor.hpp"
#include "ioniq5_ecan/vehicle_state_parser.hpp"

namespace ioniq5_ecan {

class Ioniq5EcanNode {
 public:
  Ioniq5EcanNode(ros::NodeHandle node_handle, ros::NodeHandle private_node_handle);
  ~Ioniq5EcanNode();

 private:
  template <typename T>
  T parameter(const std::string& name, const T& default_value) const {
    T value{};
    private_node_handle_.param(name, value, default_value);
    return value;
  }

  void command_callback(const ActuationCommand::ConstPtr& message);
  void raw_can_tx_callback(const RawCanFrame::ConstPtr& message);
  bool arm_callback(std_srvs::SetBool::Request& request, std_srvs::SetBool::Response& response);
  void receive_loop();
  void control_loop();
  void publish_status(const ros::TimerEvent& event);
  void publish_raw_can(const CanFrame& frame);
  void publish_diagnostics(const VehicleStateData& vehicle, const PandaHealth& panda,
                           const SafetyDecision& decision);
  void apply_realtime_settings(const char* name, int priority, int cpu);
  void load_configuration();
  uint64_t request_internal_disarm();
  void enter_vehicle_safety_mode(const VehicleStateData& vehicle);
  void enter_no_output_mode();
  void retry_no_output_recovery(TimePoint now);
  void verify_longitudinal_firmware();
  void observe_control_frame(const CanFrame& frame);
  std::vector<uint8_t> uds_request(uint32_t request_address, uint32_t response_address,
                                   const std::vector<uint8_t>& payload,
                                   const std::vector<uint8_t>& expected_prefix);
  void send_uds(uint32_t request_address, const std::vector<uint8_t>& payload);
  void send_tester_present(uint32_t request_address, TimePoint& last_sent);
  void disable_ecu(const char* label, uint32_t request_address, uint32_t response_address,
                   uint32_t owned_address, std::atomic<uint64_t>& owned_count, bool& disabled,
                   TimePoint& last_tester_present);
  void restore_ecu(const char* label, uint32_t request_address, uint32_t response_address,
                   uint32_t owned_address, std::atomic<uint64_t>& owned_count, bool& disabled);
  void maintain_disabled_ecus(TimePoint now);
  bool copy_recent_scc_template(TimePoint now, CanFrame& frame) const;

  ros::NodeHandle node_handle_;
  ros::NodeHandle private_node_handle_;
  PandaUsbConfig panda_config_;
  CommandAdapterConfig adapter_config_;
  SafetyConfig safety_config_;
  uint8_t ecan_bus_{0};
  uint8_t camera_bus_{2};
  bool ecan_only_{true};
  bool alternate_buttons_{false};
  bool use_enable_field_{false};
  bool auto_arm_on_command_{true};
  bool publish_raw_can_rx_{true};
  bool publish_raw_can_bus_topics_{true};
  bool allow_raw_can_tx_{false};
  int control_rate_hz_{100};
  int health_rate_hz_{10};
  int realtime_priority_{0};
  int control_cpu_{-1};
  int receive_cpu_{-1};
  double set_speed_kph_{30.0};
  std::chrono::milliseconds vehicle_state_timeout_{100};
  std::string command_topic_{"/ioniq5/actuation_command"};
  std::string state_topic_{"/ioniq5/vehicle_state"};
  std::string raw_can_rx_topic_{"/ioniq5/can_rx"};
  std::string raw_can_tx_topic_{"/ioniq5/can_tx"};
  std::string raw_can_bus_prefix_{"/ioniq5/can"};

  std::unique_ptr<PandaUsb> panda_;
  std::unique_ptr<VehicleStateParser> parser_;
  std::unique_ptr<CommandAdapter> adapter_;
  std::unique_ptr<SafetySupervisor> supervisor_;
  HyundaiCanFdCodec codec_;

  std::atomic<bool> running_{true};
  std::atomic<bool> requested_arm_{false};
  std::atomic<bool> applied_arm_{false};
  std::atomic<bool> auto_arm_inhibited_{false};
  std::atomic<bool> recovery_pending_{false};
  std::atomic<bool> recovery_rearm_required_{false};
  std::atomic<uint64_t> recovery_attempts_{0};
  std::atomic<uint64_t> arm_request_generation_{0};
  std::atomic<bool> vehicle_safety_mode_{false};
  std::atomic<uint64_t> raw_can_rx_count_{0};
  std::atomic<uint64_t> raw_can_tx_count_{0};
  std::atomic<uint64_t> raw_can_tx_drop_count_{0};
  std::atomic<uint64_t> stock_lfa_count_{0};
  std::atomic<uint64_t> stock_scc_count_{0};
  std::atomic<uint32_t> last_rejected_address_{0};
  std::thread receive_thread_;
  std::thread control_thread_;

  // Serializes safety-mode transitions and all host CAN transmission. The receive thread remains
  // free to deliver UDS replies while a transition is waiting for an ECU response.
  mutable std::mutex actuation_mutex_;
  mutable std::mutex command_mutex_;
  CommandSample latest_command_;
  uint32_t last_sequence_{0};
  mutable std::mutex health_mutex_;
  PandaHealth latest_health_;
  mutable std::mutex decision_mutex_;
  SafetyDecision latest_decision_;
  mutable std::mutex stock_scc_mutex_;
  CanFrame latest_stock_scc_;
  bool have_stock_scc_{false};
  mutable std::mutex uds_mutex_;
  std::condition_variable uds_condition_;
  bool uds_waiting_{false};
  uint32_t uds_response_address_{0};
  std::vector<uint8_t> uds_response_;
  bool camera_disabled_{false};
  bool radar_disabled_{false};
  // Protected by actuation_mutex_; receive and service callbacks only set the atomic request.
  RecoveryRetry recovery_retry_;
  TimePoint last_camera_tester_present_{};
  TimePoint last_radar_tester_present_{};

  ros::Subscriber command_subscription_;
  ros::Subscriber raw_can_tx_subscription_;
  ros::Publisher state_publisher_;
  ros::Publisher raw_can_rx_publisher_;
  std::array<ros::Publisher, 3> raw_can_bus_publishers_;
  ros::Publisher diagnostics_publisher_;
  ros::ServiceServer arm_service_;
  ros::Timer status_timer_;
};

}  // namespace ioniq5_ecan
