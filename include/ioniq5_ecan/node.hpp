#pragma once

#include <atomic>
#include <diagnostic_msgs/msg/diagnostic_array.hpp>
#include <memory>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <string>
#include <thread>

#include "ioniq5_ecan/command_adapter.hpp"
#include "ioniq5_ecan/hyundai_canfd_codec.hpp"
#include "ioniq5_ecan/msg/actuation_command.hpp"
#include "ioniq5_ecan/msg/vehicle_state.hpp"
#include "ioniq5_ecan/panda_usb.hpp"
#include "ioniq5_ecan/safety_supervisor.hpp"
#include "ioniq5_ecan/vehicle_state_parser.hpp"

namespace ioniq5_ecan {

class Ioniq5EcanNode : public rclcpp::Node {
 public:
  Ioniq5EcanNode();
  ~Ioniq5EcanNode() override;

 private:
  void command_callback(const msg::ActuationCommand::SharedPtr message);
  void arm_callback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                    std::shared_ptr<std_srvs::srv::SetBool::Response> response);
  void receive_loop();
  void control_loop();
  void publish_status();
  void publish_diagnostics(const VehicleState& vehicle, const PandaHealth& panda,
                           const SafetyDecision& decision);
  void apply_realtime_settings(const char* name, int priority, int cpu);
  void load_configuration();
  void request_internal_disarm();
  void enter_vehicle_safety_mode();
  void enter_no_output_mode();
  void verify_longitudinal_firmware();

  PandaUsbConfig panda_config_;
  CommandAdapterConfig adapter_config_;
  SafetyConfig safety_config_;
  uint8_t ecan_bus_{0};
  uint8_t camera_bus_{2};
  bool alternate_buttons_{false};
  int control_rate_hz_{100};
  int health_rate_hz_{10};
  int realtime_priority_{0};
  int control_cpu_{-1};
  int receive_cpu_{-1};
  double set_speed_kph_{30.0};
  std::chrono::milliseconds vehicle_state_timeout_{100};
  std::string command_topic_{"/ioniq5/actuation_command"};
  std::string state_topic_{"/ioniq5/vehicle_state"};

  std::unique_ptr<PandaUsb> panda_;
  std::unique_ptr<VehicleStateParser> parser_;
  std::unique_ptr<CommandAdapter> adapter_;
  std::unique_ptr<SafetySupervisor> supervisor_;
  HyundaiCanFdCodec codec_;

  std::atomic<bool> running_{true};
  std::atomic<bool> requested_arm_{false};
  std::atomic<bool> applied_arm_{false};
  std::atomic<uint64_t> arm_request_generation_{0};
  std::atomic<bool> vehicle_safety_mode_{false};
  std::thread receive_thread_;
  std::thread control_thread_;

  mutable std::mutex command_mutex_;
  CommandSample latest_command_;
  uint32_t last_sequence_{0};
  mutable std::mutex health_mutex_;
  PandaHealth latest_health_;
  mutable std::mutex decision_mutex_;
  SafetyDecision latest_decision_;

  rclcpp::Subscription<msg::ActuationCommand>::SharedPtr command_subscription_;
  rclcpp::Publisher<msg::VehicleState>::SharedPtr state_publisher_;
  rclcpp::Publisher<diagnostic_msgs::msg::DiagnosticArray>::SharedPtr diagnostics_publisher_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr arm_service_;
  rclcpp::TimerBase::SharedPtr status_timer_;
};

}  // namespace ioniq5_ecan
