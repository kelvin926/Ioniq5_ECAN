#pragma once

#include <diagnostic_msgs/DiagnosticArray.h>
#include <ros/ros.h>
#include <std_srvs/SetBool.h>

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "ioniq5_ecan/ActuationCommand.h"
#include "ioniq5_ecan/RawCanFrame.h"
#include "ioniq5_ecan/VehicleState.h"
#include "ioniq5_ecan/command_adapter.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
#include "ioniq5_ecan/panda_usb.hpp"
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
  void request_internal_disarm();
  void enter_vehicle_safety_mode();
  void enter_no_output_mode();
  void verify_longitudinal_firmware();

  ros::NodeHandle node_handle_;
  ros::NodeHandle private_node_handle_;
  PandaUsbConfig panda_config_;
  CommandAdapterConfig adapter_config_;
  SafetyConfig safety_config_;
  uint8_t ecan_bus_{0};
  uint8_t camera_bus_{2};
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
  std::atomic<uint64_t> arm_request_generation_{0};
  std::atomic<bool> vehicle_safety_mode_{false};
  std::atomic<uint64_t> raw_can_rx_count_{0};
  std::atomic<uint64_t> raw_can_tx_count_{0};
  std::atomic<uint64_t> raw_can_tx_drop_count_{0};
  std::thread receive_thread_;
  std::thread control_thread_;

  mutable std::mutex command_mutex_;
  CommandSample latest_command_;
  uint32_t last_sequence_{0};
  mutable std::mutex health_mutex_;
  PandaHealth latest_health_;
  mutable std::mutex decision_mutex_;
  SafetyDecision latest_decision_;

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
