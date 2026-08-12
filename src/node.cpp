#include "ioniq5_ecan/node.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#endif

#include <diagnostic_msgs/msg/diagnostic_status.hpp>
#include <diagnostic_msgs/msg/key_value.hpp>

namespace ioniq5_ecan {
namespace {

template <typename T>
diagnostic_msgs::msg::KeyValue key_value(const std::string& key, T value) {
  diagnostic_msgs::msg::KeyValue item;
  item.key = key;
  item.value = std::to_string(value);
  return item;
}

diagnostic_msgs::msg::KeyValue key_value(const std::string& key, const std::string& value) {
  diagnostic_msgs::msg::KeyValue item;
  item.key = key;
  item.value = value;
  return item;
}

}  // namespace

Ioniq5EcanNode::Ioniq5EcanNode() : Node("ioniq5_ecan_node") {
  load_configuration();
  panda_ = std::make_unique<PandaUsb>(panda_config_);
  parser_ = std::make_unique<VehicleStateParser>(ecan_bus_, camera_bus_, alternate_buttons_);
  adapter_ = std::make_unique<CommandAdapter>(adapter_config_);
  supervisor_ = std::make_unique<SafetySupervisor>(safety_config_);

#ifdef __linux__
  if (realtime_priority_ > 0 && mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
    RCLCPP_WARN(get_logger(), "mlockall failed; continuing without locked memory");
  }
#endif

  auto command_qos = rclcpp::QoS(rclcpp::KeepLast(1));
  command_qos.best_effort();
  command_subscription_ = create_subscription<msg::ActuationCommand>(
    command_topic_, command_qos,
    std::bind(&Ioniq5EcanNode::command_callback, this, std::placeholders::_1));
  state_publisher_ = create_publisher<msg::VehicleState>(state_topic_, rclcpp::QoS(10));
  diagnostics_publisher_ =
    create_publisher<diagnostic_msgs::msg::DiagnosticArray>("/diagnostics", rclcpp::QoS(10));
  arm_service_ = create_service<std_srvs::srv::SetBool>(
    "/ioniq5_ecan/set_armed",
    std::bind(&Ioniq5EcanNode::arm_callback, this, std::placeholders::_1, std::placeholders::_2));
  status_timer_ = create_wall_timer(std::chrono::milliseconds(50),
                                    std::bind(&Ioniq5EcanNode::publish_status, this));

  receive_thread_ = std::thread(&Ioniq5EcanNode::receive_loop, this);
  control_thread_ = std::thread(&Ioniq5EcanNode::control_loop, this);
  RCLCPP_WARN(
    get_logger(),
    "Started in NO_OUTPUT. allow_actuation=%s allow_longitudinal=%s; explicit arm is required.",
    safety_config_.allow_actuation ? "true" : "false",
    safety_config_.allow_longitudinal ? "true" : "false");
}

Ioniq5EcanNode::~Ioniq5EcanNode() {
  running_ = false;
  if (receive_thread_.joinable()) receive_thread_.join();
  if (control_thread_.joinable()) control_thread_.join();
  if (panda_ && panda_->connected()) {
    try {
      panda_->send_heartbeat(false);
      panda_->set_safety_mode(PandaUsb::kSafetyNoOutput, 0);
    } catch (const std::exception&) {
    }
    panda_->disconnect();
  }
}

void Ioniq5EcanNode::load_configuration() {
  panda_config_.serial = declare_parameter<std::string>("hardware.panda_serial", "");
  panda_config_.nominal_bitrate_kbps = declare_parameter<int>("hardware.nominal_bitrate_kbps", 500);
  panda_config_.data_bitrate_kbps = declare_parameter<int>("hardware.data_bitrate_kbps", 2000);
  panda_config_.read_timeout_ms = declare_parameter<int>("hardware.usb_read_timeout_ms", 20);
  panda_config_.write_timeout_ms = declare_parameter<int>("hardware.usb_write_timeout_ms", 10);
  ecan_bus_ = static_cast<uint8_t>(declare_parameter<int>("hardware.ecan_bus", 0));
  camera_bus_ = static_cast<uint8_t>(declare_parameter<int>("hardware.camera_bus", 2));
  alternate_buttons_ = declare_parameter<bool>("hardware.alternate_buttons", false);

  command_topic_ = declare_parameter<std::string>("topics.command", command_topic_);
  state_topic_ = declare_parameter<std::string>("topics.vehicle_state", state_topic_);
  adapter_config_.lateral_mode = lateral_mode_from_string(
    declare_parameter<std::string>("input.lateral_mode", "steering_rate_deg_s"));
  adapter_config_.lateral_scale = declare_parameter<double>("input.lateral_scale", 1.0);
  adapter_config_.lateral_offset = declare_parameter<double>("input.lateral_offset", 0.0);
  adapter_config_.acceleration_scale = declare_parameter<double>("input.acceleration_scale", 1.0);
  adapter_config_.acceleration_offset = declare_parameter<double>("input.acceleration_offset", 0.0);

  adapter_config_.wheelbase_m = declare_parameter<double>("vehicle.wheelbase_m", 2.97);
  adapter_config_.steering_ratio = declare_parameter<double>("vehicle.steering_ratio", 14.26);
  adapter_config_.kp_angle = declare_parameter<double>("lateral.kp_angle", 2.0);
  adapter_config_.ki_angle = declare_parameter<double>("lateral.ki_angle", 0.0);
  adapter_config_.kd_rate = declare_parameter<double>("lateral.kd_rate", 0.2);
  adapter_config_.feedforward_rate = declare_parameter<double>("lateral.feedforward_rate", 0.0);
  adapter_config_.integral_limit = declare_parameter<double>("lateral.integral_limit", 50.0);
  adapter_config_.max_target_angle_deg =
    declare_parameter<double>("lateral.max_target_angle_deg", 80.0);
  adapter_config_.max_target_rate_deg_s =
    declare_parameter<double>("lateral.max_target_rate_deg_s", 120.0);
  adapter_config_.max_torque = declare_parameter<int>("lateral.max_torque", 100);
  adapter_config_.torque_rate_up = declare_parameter<int>("lateral.torque_rate_up", 1);
  adapter_config_.torque_rate_down = declare_parameter<int>("lateral.torque_rate_down", 2);
  adapter_config_.driver_torque_start =
    declare_parameter<double>("lateral.driver_torque_start", 100.0);
  adapter_config_.driver_torque_zero =
    declare_parameter<double>("lateral.driver_torque_zero", 250.0);
  adapter_config_.accel_min_mps2 = declare_parameter<double>("longitudinal.accel_min_mps2", -1.0);
  adapter_config_.accel_max_mps2 = declare_parameter<double>("longitudinal.accel_max_mps2", 1.0);
  adapter_config_.jerk_limit_mps3 = declare_parameter<double>("longitudinal.jerk_limit_mps3", 2.0);
  set_speed_kph_ = declare_parameter<double>("longitudinal.set_speed_kph", 30.0);

  safety_config_.allow_actuation = declare_parameter<bool>("safety.allow_actuation", false);
  safety_config_.allow_longitudinal = declare_parameter<bool>("safety.allow_longitudinal", false);
  safety_config_.require_set_resume_button =
    declare_parameter<bool>("safety.require_set_resume_button", true);
  safety_config_.disengage_on_brake = declare_parameter<bool>("safety.disengage_on_brake", true);
  safety_config_.disengage_on_cancel = declare_parameter<bool>("safety.disengage_on_cancel", true);
  safety_config_.longitudinal_override_on_gas =
    declare_parameter<bool>("safety.longitudinal_override_on_gas", true);
  safety_config_.max_active_speed_mps =
    declare_parameter<double>("safety.max_active_speed_mps", 8.33);
  safety_config_.max_abs_steering_angle_deg =
    declare_parameter<double>("safety.max_abs_steering_angle_deg", 85.0);
  safety_config_.required_safety_mode = static_cast<uint8_t>(PandaUsb::kSafetyHyundaiCanFd);
  safety_config_.required_safety_param = safety_config_.allow_longitudinal
                                           ? PandaUsb::kIoniq5Hda1LongParam
                                           : PandaUsb::kIoniq5Hda1PassiveParam;
  if (alternate_buttons_) {
    safety_config_.required_safety_param |= PandaUsb::kHyundaiAlternateButtons;
  }
  safety_config_.command_timeout =
    std::chrono::milliseconds(declare_parameter<int>("safety.command_timeout_ms", 100));
  safety_config_.panda_timeout =
    std::chrono::milliseconds(declare_parameter<int>("safety.panda_timeout_ms", 250));
  vehicle_state_timeout_ =
    std::chrono::milliseconds(declare_parameter<int>("safety.vehicle_state_timeout_ms", 100));

  control_rate_hz_ = declare_parameter<int>("runtime.control_rate_hz", 100);
  health_rate_hz_ = declare_parameter<int>("runtime.health_rate_hz", 10);
  realtime_priority_ = declare_parameter<int>("runtime.realtime_priority", 0);
  control_cpu_ = declare_parameter<int>("runtime.control_cpu", -1);
  receive_cpu_ = declare_parameter<int>("runtime.receive_cpu", -1);

  if (ecan_bus_ > 2U || camera_bus_ > 2U || ecan_bus_ == camera_bus_) {
    throw std::invalid_argument("Hyundai K bus mapping must use two distinct Panda buses in [0,2]");
  }
  if (panda_config_.nominal_bitrate_kbps != 500 || panda_config_.data_bitrate_kbps != 2000) {
    throw std::invalid_argument("Ioniq 5 CAN-FD rates must be 500/2000 kbps");
  }
  if (panda_config_.read_timeout_ms < 1 || panda_config_.read_timeout_ms > 100 ||
      panda_config_.write_timeout_ms < 1 || panda_config_.write_timeout_ms > 100) {
    throw std::invalid_argument("USB timeouts must be within [1,100] ms");
  }
  if (control_rate_hz_ != 100) {
    throw std::invalid_argument("LFA control loop must run at 100 Hz");
  }
  if (health_rate_hz_ < 5 || health_rate_hz_ > 20) {
    throw std::invalid_argument("health_rate_hz must be within [5,20]");
  }
  if (realtime_priority_ < 0 || realtime_priority_ > 99) {
    throw std::invalid_argument("realtime_priority must be within [0,99]");
  }
  if (control_cpu_ < -1 || receive_cpu_ < -1) {
    throw std::invalid_argument("CPU affinity values must be -1 or non-negative");
  }
  if (vehicle_state_timeout_.count() <= 0) {
    throw std::invalid_argument("vehicle_state_timeout_ms must be positive");
  }
  if (!std::isfinite(set_speed_kph_) || set_speed_kph_ < 0.0 || set_speed_kph_ > 255.0) {
    throw std::invalid_argument("set_speed_kph must be finite and within [0,255]");
  }
  if (adapter_config_.max_target_angle_deg > 80.0) {
    throw std::invalid_argument("software steering angle limit cannot exceed 80 degrees");
  }
  if (safety_config_.max_abs_steering_angle_deg < 80.0 ||
      safety_config_.max_abs_steering_angle_deg > 85.0) {
    throw std::invalid_argument("steering safety limit must be within [80,85] degrees");
  }
}

void Ioniq5EcanNode::command_callback(const msg::ActuationCommand::SharedPtr message) {
  std::lock_guard<std::mutex> lock(command_mutex_);
  if (message->sequence != 0U && last_sequence_ != 0U && message->sequence <= last_sequence_) {
    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                         "discarding non-monotonic command sequence");
    return;
  }
  if (!std::isfinite(message->lateral) || !std::isfinite(message->acceleration)) {
    latest_command_.valid = false;
    latest_command_.enable = false;
    latest_command_.received_at = SteadyClock::now();
    RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000,
                          "rejecting non-finite actuation command");
    return;
  }
  if (message->sequence != 0U) last_sequence_ = message->sequence;
  latest_command_.lateral = message->lateral;
  latest_command_.acceleration_mps2 = message->acceleration;
  latest_command_.enable = message->enable;
  latest_command_.valid = true;
  latest_command_.sequence = message->sequence;
  latest_command_.received_at = SteadyClock::now();
}

void Ioniq5EcanNode::arm_callback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                  std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  if (request->data && !safety_config_.allow_actuation) {
    response->success = false;
    response->message = "safety.allow_actuation is false in YAML";
    return;
  }
  requested_arm_ = request->data;
  arm_request_generation_.fetch_add(1U);
  response->success = true;
  response->message =
    request->data ? "arm requested; SET/RES and deadman are still required" : "disarm requested";
}

void Ioniq5EcanNode::receive_loop() {
  apply_realtime_settings("ecan_rx", std::max(0, realtime_priority_ - 1), receive_cpu_);
  auto next_connect_attempt = SteadyClock::now();
  auto next_health = SteadyClock::now();
  const auto health_period = std::chrono::milliseconds(1000 / health_rate_hz_);

  while (running_) {
    try {
      if (!panda_->connected()) {
        if (SteadyClock::now() < next_connect_attempt) {
          std::this_thread::sleep_for(std::chrono::milliseconds(20));
          continue;
        }
        panda_->connect();
        RCLCPP_INFO(get_logger(), "Connected to Red Panda serial=%s", panda_->serial().c_str());
        next_health = SteadyClock::now();
      }

      for (const CanFrame& frame : panda_->receive()) parser_->update(frame);
      const auto now = SteadyClock::now();
      if (now >= next_health) {
        PandaHealth health = panda_->health();
        std::lock_guard<std::mutex> lock(health_mutex_);
        latest_health_ = health;
        next_health = now + health_period;
      }
    } catch (const std::exception& error) {
      RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 2000, "Panda receive/health error: %s",
                            error.what());
      panda_->disconnect();
      request_internal_disarm();
      applied_arm_ = false;
      vehicle_safety_mode_ = false;
      {
        std::lock_guard<std::mutex> lock(health_mutex_);
        latest_health_ = PandaHealth{};
      }
      next_connect_attempt = SteadyClock::now() + std::chrono::seconds(1);
    }
  }
}

void Ioniq5EcanNode::control_loop() {
  apply_realtime_settings("ecan_ctrl", realtime_priority_, control_cpu_);
  const auto period = std::chrono::nanoseconds(1000000000LL / control_rate_hz_);
  auto next = SteadyClock::now() + period;
  auto previous = SteadyClock::now();
  uint64_t frame_index = 0;
  uint64_t applied_arm_generation = 0;
  double accel_last = 0.0;
  ControlState previous_state = ControlState::Disconnected;
  std::vector<CanFrame> frames;
  frames.reserve(4);

  while (running_) {
    std::this_thread::sleep_until(next);
    const auto now = SteadyClock::now();
    const double dt = std::chrono::duration<double>(now - previous).count();
    previous = now;
    next += period;
    if (now > next + period) next = now + period;

    CommandSample command;
    PandaHealth panda_health;
    {
      std::lock_guard<std::mutex> lock(command_mutex_);
      command = latest_command_;
    }
    {
      std::lock_guard<std::mutex> lock(health_mutex_);
      panda_health = latest_health_;
    }
    const VehicleState vehicle = parser_->snapshot(now, vehicle_state_timeout_);

    const bool requested = requested_arm_.load();
    const uint64_t request_generation = arm_request_generation_.load();
    if (request_generation != applied_arm_generation && (!requested || panda_->connected())) {
      applied_arm_generation = request_generation;
      if (supervisor_->request_arm(requested)) {
        applied_arm_ = requested;
        try {
          if (requested && panda_->connected()) enter_vehicle_safety_mode();
          if (!requested && panda_->connected()) enter_no_output_mode();
        } catch (const std::exception& error) {
          RCLCPP_ERROR(get_logger(), "failed to change Panda safety mode: %s", error.what());
          request_internal_disarm();
          supervisor_->request_arm(false);
          applied_arm_ = false;
          try {
            if (panda_->connected()) enter_no_output_mode();
          } catch (const std::exception& no_output_error) {
            RCLCPP_ERROR(get_logger(), "failed to recover NO_OUTPUT: %s", no_output_error.what());
          }
        }
      } else {
        request_internal_disarm();
        applied_arm_ = false;
      }
    }

    SafetyDecision decision = supervisor_->update(now, vehicle, panda_health, command);
    {
      std::lock_guard<std::mutex> lock(decision_mutex_);
      latest_decision_ = decision;
    }

    if (!supervisor_->arm_requested() && applied_arm_.load()) {
      requested_arm_ = false;
      applied_arm_ = false;
      try {
        if (panda_->connected()) enter_no_output_mode();
      } catch (const std::exception& error) {
        RCLCPP_ERROR(get_logger(), "failed to enter NO_OUTPUT: %s", error.what());
      }
    }

    const ControlOutput output = adapter_->update(command, vehicle, dt, decision.lateral_allowed,
                                                  decision.longitudinal_allowed);
    if (vehicle_safety_mode_.load() && panda_->connected()) {
      frames.clear();
      const bool steer_request = output.lateral_active;
      frames.push_back(codec_.make_lfa(
        output.steering_torque, decision.state == ControlState::Active, steer_request, ecan_bus_));
      if (frame_index % 5U == 0U) {
        frames.push_back(
          codec_.make_lfa_cluster(decision.state == ControlState::Active, ecan_bus_));
      }
      if (safety_config_.allow_longitudinal && frame_index % 2U == 0U) {
        const double target = output.longitudinal_active ? output.acceleration_mps2 : 0.0;
        const double max_delta = adapter_config_.jerk_limit_mps3 * 0.02;
        accel_last = std::clamp(target, accel_last - max_delta, accel_last + max_delta);
        const bool acc_enabled = decision.state == ControlState::Active;
        frames.push_back(codec_.make_scc_control(target, accel_last, acc_enabled, output.stopping,
                                                 vehicle.gas_pressed, set_speed_kph_,
                                                 adapter_config_.jerk_limit_mps3, ecan_bus_));
        frames.push_back(codec_.make_fca_warning(ecan_bus_));
      }
      try {
        panda_->send(frames);
      } catch (const std::exception& error) {
        RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000, "Panda control send failed: %s",
                              error.what());
        request_internal_disarm();
      }
    } else {
      accel_last = 0.0;
    }

    const bool just_activated =
      decision.state == ControlState::Active && previous_state != ControlState::Active;
    if (panda_->connected() && (just_activated || frame_index % 50U == 0U)) {
      try {
        panda_->send_heartbeat(decision.heartbeat_engaged);
      } catch (const std::exception& error) {
        RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000, "Panda heartbeat failed: %s",
                              error.what());
      }
    }
    previous_state = decision.state;
    ++frame_index;
  }
}

void Ioniq5EcanNode::request_internal_disarm() {
  requested_arm_ = false;
  arm_request_generation_.fetch_add(1U);
}

void Ioniq5EcanNode::enter_vehicle_safety_mode() {
  const uint16_t param = safety_config_.allow_longitudinal ? PandaUsb::kIoniq5Hda1LongParam
                                                           : PandaUsb::kIoniq5Hda1PassiveParam;
  const uint16_t configured_param =
    alternate_buttons_ ? static_cast<uint16_t>(param | PandaUsb::kHyundaiAlternateButtons) : param;
  panda_->set_safety_mode(PandaUsb::kSafetyHyundaiCanFd, configured_param);
  vehicle_safety_mode_ = true;
  codec_.reset_counters();
  if (safety_config_.allow_longitudinal) verify_longitudinal_firmware();
  RCLCPP_WARN(get_logger(), "Panda HYUNDAI_CANFD safety enabled with param=%u", configured_param);
}

void Ioniq5EcanNode::enter_no_output_mode() {
  panda_->send_heartbeat(false);
  panda_->set_safety_mode(PandaUsb::kSafetyNoOutput, 0);
  vehicle_safety_mode_ = false;
  RCLCPP_INFO(get_logger(), "Panda returned to NO_OUTPUT");
}

void Ioniq5EcanNode::verify_longitudinal_firmware() {
  const PandaHealth before = panda_->health();
  panda_->send(
    codec_.make_scc_control(0.0, 0.0, false, false, false, set_speed_kph_, 1.0, ecan_bus_));
  const PandaHealth after = panda_->health();
  codec_.reset_counters();
  if (after.safety_tx_blocked != before.safety_tx_blocked) {
    enter_no_output_mode();
    throw std::runtime_error(
      "Panda firmware does not allow HYUNDAI longitudinal; flash pinned DEBUG firmware");
  }
}

void Ioniq5EcanNode::publish_status() {
  const auto steady_now = SteadyClock::now();
  const VehicleState vehicle = parser_->snapshot(steady_now, vehicle_state_timeout_);
  PandaHealth panda;
  SafetyDecision decision;
  {
    std::lock_guard<std::mutex> lock(health_mutex_);
    panda = latest_health_;
  }
  {
    std::lock_guard<std::mutex> lock(decision_mutex_);
    decision = latest_decision_;
  }

  msg::VehicleState message;
  message.stamp = now();
  message.valid = vehicle.valid;
  message.speed_mps = vehicle.speed_mps;
  message.steering_angle_deg = vehicle.steering_angle_deg;
  message.steering_rate_deg_s = vehicle.steering_rate_deg_s;
  message.driver_torque = vehicle.driver_torque;
  message.eps_torque_nm = vehicle.eps_torque_nm;
  message.accelerator_pedal = vehicle.accelerator_pedal;
  message.brake_pressed = vehicle.brake_pressed;
  message.gas_pressed = vehicle.gas_pressed;
  message.eps_fault = vehicle.eps_fault;
  message.acc_fault = vehicle.acc_fault;
  message.cruise_engaged = vehicle.cruise_engaged;
  message.standstill = vehicle.standstill;
  message.gear = vehicle.gear;
  message.cruise_button = vehicle.cruise_button;
  message.control_state = static_cast<uint8_t>(decision.state);
  message.control_state_name = to_string(decision.state);
  message.control_reason = decision.reason;
  message.panda_connected = panda.connected;
  message.panda_controls_allowed = panda.controls_allowed;
  message.panda_safety_tx_blocked = panda.safety_tx_blocked;
  message.can_checksum_failures = parser_->checksum_failures();
  message.can_malformed_frames = parser_->malformed_frames();
  state_publisher_->publish(message);
  publish_diagnostics(vehicle, panda, decision);
}

void Ioniq5EcanNode::publish_diagnostics(const VehicleState& vehicle, const PandaHealth& panda,
                                         const SafetyDecision& decision) {
  diagnostic_msgs::msg::DiagnosticArray array;
  array.header.stamp = now();
  diagnostic_msgs::msg::DiagnosticStatus status;
  status.name = "ioniq5_ecan";
  status.hardware_id = panda_->serial();
  status.level =
    decision.state == ControlState::Fault || decision.state == ControlState::Disconnected
      ? diagnostic_msgs::msg::DiagnosticStatus::ERROR
      : (decision.state == ControlState::Passive ? diagnostic_msgs::msg::DiagnosticStatus::WARN
                                                 : diagnostic_msgs::msg::DiagnosticStatus::OK);
  status.message = decision.reason;
  status.values.push_back(key_value("control_state", std::string(to_string(decision.state))));
  status.values.push_back(key_value("vehicle_valid", vehicle.valid ? 1 : 0));
  status.values.push_back(key_value("panda_connected", panda.connected ? 1 : 0));
  status.values.push_back(key_value("panda_controls_allowed", panda.controls_allowed ? 1 : 0));
  status.values.push_back(key_value("panda_safety_mode", panda.safety_mode));
  status.values.push_back(key_value("panda_safety_param", panda.safety_param));
  status.values.push_back(key_value("safety_tx_blocked", panda.safety_tx_blocked));
  status.values.push_back(key_value("can_checksum_failures", parser_->checksum_failures()));
  array.status.push_back(status);
  diagnostics_publisher_->publish(array);
}

void Ioniq5EcanNode::apply_realtime_settings(const char* name, int priority, int cpu) {
#ifdef __linux__
  pthread_setname_np(pthread_self(), name);
  if (cpu >= 0) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    if (pthread_setaffinity_np(pthread_self(), sizeof(set), &set) != 0) {
      RCLCPP_WARN(get_logger(), "failed to set CPU affinity for %s", name);
    }
  }
  if (priority > 0) {
    sched_param parameters{};
    parameters.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &parameters) != 0) {
      RCLCPP_WARN(get_logger(), "failed to set SCHED_FIFO for %s", name);
    }
  }
#else
  (void)name;
  (void)priority;
  (void)cpu;
#endif
}

}  // namespace ioniq5_ecan
