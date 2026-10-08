#include "ioniq5_ecan/node.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#endif

#include <diagnostic_msgs/DiagnosticStatus.h>
#include <diagnostic_msgs/KeyValue.h>

namespace ioniq5_ecan {
namespace {

constexpr std::array<uint8_t, 16> kCanPayloadSizes{0, 1,  2,  3,  4,  5,  6,  7,
                                                   8, 12, 16, 20, 24, 32, 48, 64};
constexpr uint32_t kCameraRequestAddress = 0x730U;
constexpr uint32_t kCameraResponseAddress = 0x738U;
constexpr uint32_t kRadarRequestAddress = 0x7D0U;
constexpr uint32_t kRadarResponseAddress = 0x7D8U;
constexpr auto kUdsResponseTimeout = std::chrono::milliseconds(500);
constexpr auto kEcuQuietSettle = std::chrono::milliseconds(150);
constexpr auto kEcuQuietObservation = std::chrono::milliseconds(350);
constexpr auto kEcuRestoreObservation = std::chrono::milliseconds(500);
constexpr auto kTesterPresentPeriod = std::chrono::milliseconds(800);
constexpr auto kStockSccTimeout = std::chrono::milliseconds(500);
constexpr double kDisableMaximumSpeedMps = 0.5 / 3.6;

struct SafetyTransition {
  explicit SafetyTransition(std::atomic<uint64_t>& value) : epoch(value) { epoch.fetch_add(1U); }
  ~SafetyTransition() { epoch.fetch_add(1U); }
  std::atomic<uint64_t>& epoch;
};

bool valid_can_payload_size(uint8_t size) {
  return std::find(kCanPayloadSizes.begin(), kCanPayloadSizes.end(), size) !=
         kCanPayloadSizes.end();
}

bool starts_with(const std::vector<uint8_t>& value, const std::vector<uint8_t>& prefix) {
  return value.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), value.begin());
}

std::string bytes_as_hex(const std::vector<uint8_t>& bytes) {
  std::ostringstream stream;
  stream << std::hex;
  for (std::size_t index = 0; index < bytes.size(); ++index) {
    if (index != 0U) stream << ' ';
    stream.width(2);
    stream.fill('0');
    stream << static_cast<unsigned>(bytes[index]);
  }
  return stream.str();
}

template <typename T>
diagnostic_msgs::KeyValue key_value(const std::string& key, T value) {
  diagnostic_msgs::KeyValue item;
  item.key = key;
  item.value = std::to_string(value);
  return item;
}

diagnostic_msgs::KeyValue key_value(const std::string& key, const std::string& value) {
  diagnostic_msgs::KeyValue item;
  item.key = key;
  item.value = value;
  return item;
}

}  // namespace

Ioniq5EcanNode::Ioniq5EcanNode(ros::NodeHandle node_handle, ros::NodeHandle private_node_handle)
    : node_handle_(std::move(node_handle)), private_node_handle_(std::move(private_node_handle)) {
  load_configuration();
  panda_ = std::make_unique<PandaUsb>(panda_config_);
  parser_ =
    std::make_unique<VehicleStateParser>(ecan_bus_, camera_bus_, alternate_buttons_, !ecan_only_);
  adapter_ = std::make_unique<CommandAdapter>(adapter_config_);
  supervisor_ = std::make_unique<SafetySupervisor>(safety_config_);

#ifdef __linux__
  if (realtime_priority_ > 0 && mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
    ROS_WARN("mlockall failed; continuing without locked memory");
  }
#endif

  command_subscription_ = node_handle_.subscribe(
    command_topic_, 1, &Ioniq5EcanNode::command_callback, this, ros::TransportHints().tcpNoDelay());
  state_publisher_ = node_handle_.advertise<VehicleState>(state_topic_, 10);
  if (publish_raw_can_rx_) {
    raw_can_rx_publisher_ = node_handle_.advertise<RawCanFrame>(raw_can_rx_topic_, 256);
    if (publish_raw_can_bus_topics_) {
      for (std::size_t bus = 0; bus < raw_can_bus_publishers_.size(); ++bus) {
        raw_can_bus_publishers_[bus] = node_handle_.advertise<RawCanFrame>(
          raw_can_bus_prefix_ + std::to_string(bus) + "/rx", 256);
      }
    }
  }
  if (allow_raw_can_tx_) {
    raw_can_tx_subscription_ =
      node_handle_.subscribe(raw_can_tx_topic_, 256, &Ioniq5EcanNode::raw_can_tx_callback, this,
                             ros::TransportHints().tcpNoDelay());
  }
  diagnostics_publisher_ =
    node_handle_.advertise<diagnostic_msgs::DiagnosticArray>("/diagnostics", 10);
  arm_service_ =
    node_handle_.advertiseService("/ioniq5_ecan/set_armed", &Ioniq5EcanNode::arm_callback, this);
  status_timer_ =
    node_handle_.createTimer(ros::Duration(0.05), &Ioniq5EcanNode::publish_status, this);

  receive_thread_ = std::thread(&Ioniq5EcanNode::receive_loop, this);
  control_thread_ = std::thread(&Ioniq5EcanNode::control_loop, this);
  ROS_WARN("Started in NO_OUTPUT. allow_actuation=%s allow_longitudinal=%s auto_arm=%s.",
           safety_config_.allow_actuation ? "true" : "false",
           safety_config_.allow_longitudinal ? "true" : "false",
           auto_arm_on_command_ ? "true" : "false");
}

Ioniq5EcanNode::~Ioniq5EcanNode() {
  auto_arm_inhibited_ = true;
  operator_disarmed_ = true;
  button_rearm_takeover_ = false;
  preserve_command_session_ = false;
  command_gap_resume_qualified_ = false;
  requested_arm_ = false;
  arm_request_generation_.fetch_add(1U);
  if (panda_ && panda_->connected()) {
    try {
      // Keep the receive thread alive until both ADAS ECUs acknowledge communication restore.
      enter_no_output_mode();
    } catch (const std::exception& error) {
      ROS_ERROR("shutdown ECU restore warning: %s; ignition cycle may be required", error.what());
    }
  }
  running_ = false;
  uds_condition_.notify_all();
  if (receive_thread_.joinable()) receive_thread_.join();
  if (control_thread_.joinable()) control_thread_.join();
  if (panda_ && panda_->connected()) {
    panda_->disconnect();
  }
}

void Ioniq5EcanNode::load_configuration() {
  panda_config_.serial = parameter<std::string>("hardware/panda_serial", "");
  panda_config_.nominal_bitrate_kbps = parameter<int>("hardware/nominal_bitrate_kbps", 500);
  panda_config_.data_bitrate_kbps = parameter<int>("hardware/data_bitrate_kbps", 2000);
  panda_config_.read_timeout_ms = parameter<int>("hardware/usb_read_timeout_ms", 20);
  panda_config_.write_timeout_ms = parameter<int>("hardware/usb_write_timeout_ms", 10);
  ecan_bus_ = static_cast<uint8_t>(parameter<int>("hardware/ecan_bus", 0));
  camera_bus_ = static_cast<uint8_t>(parameter<int>("hardware/camera_bus", 2));
  ecan_only_ = parameter<bool>("hardware/ecan_only", true);
  panda_config_.ecan_only = ecan_only_;
  alternate_buttons_ = parameter<bool>("hardware/alternate_buttons", false);

  command_topic_ = parameter<std::string>("topics/command", command_topic_);
  state_topic_ = parameter<std::string>("topics/vehicle_state", state_topic_);
  raw_can_rx_topic_ = parameter<std::string>("topics/raw_can_rx", raw_can_rx_topic_);
  raw_can_tx_topic_ = parameter<std::string>("topics/raw_can_tx", raw_can_tx_topic_);
  raw_can_bus_prefix_ = parameter<std::string>("topics/raw_can_bus_prefix", raw_can_bus_prefix_);
  publish_raw_can_rx_ = parameter<bool>("raw_can/publish_rx", true);
  publish_raw_can_bus_topics_ = parameter<bool>("raw_can/publish_bus_topics", true);
  allow_raw_can_tx_ = parameter<bool>("raw_can/allow_tx", false);
  adapter_config_.lateral_mode =
    lateral_mode_from_string(parameter<std::string>("input/lateral_mode", "direct_torque"));
  adapter_config_.unfiltered_input = parameter<bool>("input/unfiltered_input", true);
  adapter_config_.lateral_scale = parameter<double>("input/lateral_scale", 1.0);
  adapter_config_.lateral_offset = parameter<double>("input/lateral_offset", 0.0);
  adapter_config_.acceleration_scale = parameter<double>("input/acceleration_scale", 1.0);
  adapter_config_.acceleration_offset = parameter<double>("input/acceleration_offset", 0.0);
  use_enable_field_ = parameter<bool>("input/use_enable_field", false);

  adapter_config_.wheelbase_m = parameter<double>("vehicle/wheelbase_m", 2.97);
  adapter_config_.steering_ratio = parameter<double>("vehicle/steering_ratio", 14.26);
  adapter_config_.steer_actuator_delay_s = parameter<double>("lateral/steer_actuator_delay_s", 0.1);
  adapter_config_.torque_kp = parameter<double>("lateral/torque_kp", 1.0);
  adapter_config_.torque_ki = parameter<double>("lateral/torque_ki", 0.1);
  adapter_config_.torque_kd = parameter<double>("lateral/torque_kd", 0.0);
  adapter_config_.torque_kf = parameter<double>("lateral/torque_kf", 1.0);
  adapter_config_.lat_accel_factor = parameter<double>("lateral/lat_accel_factor", 3.172929);
  adapter_config_.friction = parameter<double>("lateral/friction", 0.096019);
  adapter_config_.friction_threshold = parameter<double>("lateral/friction_threshold", 0.3);
  adapter_config_.steering_angle_deadzone_deg =
    parameter<double>("lateral/steering_angle_deadzone_deg", 0.0);
  adapter_config_.integral_output_limit = parameter<double>("lateral/integral_output_limit", 3.78);
  adapter_config_.integrator_freeze_speed_mps =
    parameter<double>("lateral/integrator_freeze_speed_mps", 0.0);
  const auto low_speed_bp =
    parameter<std::vector<double>>("lateral/low_speed_factor_bp_mps", {0.0, 10.0, 20.0, 30.0});
  const auto low_speed_v =
    parameter<std::vector<double>>("lateral/low_speed_factor_v", {15.0, 13.0, 10.0, 5.0});
  if (low_speed_bp.size() != adapter_config_.low_speed_factor_bp_mps.size() ||
      low_speed_v.size() != adapter_config_.low_speed_factor_v.size()) {
    throw std::invalid_argument("Carrot low-speed factor tables must contain exactly four values");
  }
  std::copy(low_speed_bp.begin(), low_speed_bp.end(),
            adapter_config_.low_speed_factor_bp_mps.begin());
  std::copy(low_speed_v.begin(), low_speed_v.end(), adapter_config_.low_speed_factor_v.begin());
  adapter_config_.max_target_angle_deg = parameter<double>("lateral/max_target_angle_deg", 175.0);
  adapter_config_.max_target_rate_deg_s = parameter<double>("lateral/max_target_rate_deg_s", 500.0);
  adapter_config_.torque_output_scale = parameter<int>("lateral/torque_output_scale", 270);
  adapter_config_.max_torque = parameter<int>("lateral/max_torque", 1021);
  adapter_config_.torque_rate_up = parameter<int>("lateral/torque_rate_up", 2042);
  adapter_config_.torque_rate_down = parameter<int>("lateral/torque_rate_down", 2042);
  adapter_config_.driver_torque_allowance =
    parameter<double>("lateral/driver_torque_allowance", 250.0);
  adapter_config_.driver_torque_multiplier =
    parameter<double>("lateral/driver_torque_multiplier", 0.0);
  adapter_config_.driver_torque_factor = parameter<double>("lateral/driver_torque_factor", 1.0);
  adapter_config_.steer_request_cutoff_angle_deg =
    parameter<double>("lateral/steer_request_cutoff_angle_deg", 85.0);
  const int steer_request_valid_frames = parameter<int>("lateral/steer_request_valid_frames", 89);
  const int steer_request_cut_frames = parameter<int>("lateral/steer_request_cut_frames", 2);
  if (steer_request_valid_frames < 0 || steer_request_cut_frames < 0) {
    throw std::invalid_argument("steering request frame counts cannot be negative");
  }
  adapter_config_.steer_request_valid_frames = static_cast<uint32_t>(steer_request_valid_frames);
  adapter_config_.steer_request_cut_frames = static_cast<uint32_t>(steer_request_cut_frames);
  adapter_config_.accel_min_mps2 = parameter<double>("longitudinal/accel_min_mps2", -10.23);
  adapter_config_.accel_max_mps2 = parameter<double>("longitudinal/accel_max_mps2", 10.24);
  adapter_config_.jerk_limit_mps3 = parameter<double>("longitudinal/jerk_limit_mps3", 12.7);
  set_speed_kph_ = parameter<double>("longitudinal/set_speed_kph", 30.0);

  safety_config_.allow_actuation = parameter<bool>("safety/allow_actuation", false);
  safety_config_.allow_longitudinal = parameter<bool>("safety/allow_longitudinal", false);
  safety_config_.resume_on_command_return =
    parameter<bool>("safety/resume_on_command_return", false);
  safety_config_.lateral_button_toggle = parameter<bool>("safety/lateral_button_toggle", true);
  safety_config_.longitudinal_button_toggle =
    parameter<bool>("safety/longitudinal_button_toggle", true);
  auto_arm_on_command_ = parameter<bool>("safety/auto_arm_on_command", true);
  safety_config_.lateral_disengage_on_brake =
    parameter<bool>("safety/lateral_disengage_on_brake", false);
  safety_config_.longitudinal_disengage_on_brake =
    parameter<bool>("safety/longitudinal_disengage_on_brake", true);
  safety_config_.disengage_on_cancel = parameter<bool>("safety/disengage_on_cancel", true);
  safety_config_.longitudinal_override_on_gas =
    parameter<bool>("safety/longitudinal_override_on_gas", true);
  safety_config_.max_active_speed_mps = parameter<double>("safety/max_active_speed_mps", 0.0);
  safety_config_.max_abs_steering_angle_deg =
    parameter<double>("safety/max_abs_steering_angle_deg", 0.0);
  safety_config_.required_safety_mode = static_cast<uint8_t>(PandaUsb::kSafetyHyundaiCanFd);
  safety_config_.required_safety_param = safety_config_.allow_longitudinal
                                           ? PandaUsb::kIoniq5Hda1LongParam
                                           : PandaUsb::kIoniq5Hda1PassiveParam;
  if (alternate_buttons_) {
    safety_config_.required_safety_param |= PandaUsb::kHyundaiAlternateButtons;
  }
  if (safety_config_.allow_actuation && safety_config_.resume_on_command_return) {
    safety_config_.required_safety_param |= PandaUsb::kHyundaiCommandSession;
    panda_config_.command_session_param = safety_config_.required_safety_param;
  }
  safety_config_.required_harness_status = 1U;
  safety_config_.command_timeout =
    std::chrono::milliseconds(parameter<int>("safety/command_timeout_ms", 100));
  safety_config_.panda_timeout =
    std::chrono::milliseconds(parameter<int>("safety/panda_timeout_ms", 250));
  vehicle_state_timeout_ =
    std::chrono::milliseconds(parameter<int>("safety/vehicle_state_timeout_ms", 100));

  control_rate_hz_ = parameter<int>("runtime/control_rate_hz", 100);
  health_rate_hz_ = parameter<int>("runtime/health_rate_hz", 10);
  realtime_priority_ = parameter<int>("runtime/realtime_priority", 0);
  control_cpu_ = parameter<int>("runtime/control_cpu", -1);
  receive_cpu_ = parameter<int>("runtime/receive_cpu", -1);

  if (ecan_bus_ > 2U || camera_bus_ > 2U || ecan_bus_ == camera_bus_) {
    throw std::invalid_argument("Hyundai K bus mapping must use two distinct Panda buses in [0,2]");
  }
  if (!ecan_only_ || ecan_bus_ != 0U) {
    throw std::invalid_argument("this firmware requires hardware/ecan_only=true and ecan_bus=0");
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
}

void Ioniq5EcanNode::command_callback(const ActuationCommand::ConstPtr& message) {
  std::lock_guard<std::mutex> lock(command_mutex_);
  const auto received_at = SteadyClock::now();
  const bool previous_command_fresh =
    latest_command_.received_at.time_since_epoch().count() != 0 &&
    received_at >= latest_command_.received_at &&
    received_at - latest_command_.received_at <= safety_config_.command_timeout;
  if (message->sequence != 0U && last_sequence_ != 0U && previous_command_fresh &&
      !sequence_is_newer(message->sequence, last_sequence_)) {
    ROS_WARN_THROTTLE(1.0, "discarding non-monotonic command sequence");
    return;
  }
  if (!std::isfinite(message->lateral) || !std::isfinite(message->acceleration)) {
    latest_command_.valid = false;
    latest_command_.enable = false;
    latest_command_.received_at = received_at;
    ROS_ERROR_THROTTLE(1.0, "rejecting non-finite actuation command");
    return;
  }
  if (message->sequence != 0U) last_sequence_ = message->sequence;
  latest_command_.lateral = message->lateral;
  latest_command_.acceleration_mps2 = message->acceleration;
  latest_command_.enable = !use_enable_field_ || message->enable;
  latest_command_.valid = true;
  latest_command_.sequence = message->sequence;
  latest_command_.received_at = received_at;
}

void Ioniq5EcanNode::raw_can_tx_callback(const RawCanFrame::ConstPtr& message) {
  if (!allow_raw_can_tx_) return;
  const std::size_t incoming_size = message->data.size();
  const auto size = static_cast<uint8_t>(incoming_size <= 64U ? incoming_size : 0U);
  const bool valid = incoming_size <= 64U && message->bus < 3U && message->address < (1U << 29U) &&
                     (message->extended || message->address < 0x800U) &&
                     valid_can_payload_size(size) && (message->fd || size <= 8U) &&
                     !message->returned && !message->rejected;
  if (!valid) {
    ++raw_can_tx_drop_count_;
    ROS_WARN_THROTTLE(1.0, "dropping malformed raw CAN TX frame");
    return;
  }
  CanFrame frame;
  frame.address = message->address;
  frame.bus = message->bus;
  frame.fd = message->fd;
  frame.extended = message->extended;
  frame.size = size;
  std::copy_n(message->data.begin(), frame.size, frame.data.begin());
  std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
  bool longitudinal_allowed = false;
  bool lateral_allowed = false;
  {
    std::lock_guard<std::mutex> lock(decision_mutex_);
    longitudinal_allowed = latest_decision_.longitudinal_allowed;
    lateral_allowed = latest_decision_.lateral_allowed;
  }
  if (frame.address == HyundaiCanFdCodec::kLfaAddress && !lateral_allowed) {
    ++raw_can_tx_drop_count_;
    ROS_WARN_THROTTLE(1.0, "raw LFA TX requires active lateral control");
    return;
  }
  if (!vehicle_safety_mode_.load() || !panda_->connected() || !longitudinal_allowed) {
    ++raw_can_tx_drop_count_;
    ROS_WARN_THROTTLE(1.0, "raw CAN TX requires SET combined mode and active Panda controls");
    return;
  }
  try {
    panda_->send(frame);
    ++raw_can_tx_count_;
  } catch (const std::exception& error) {
    ++raw_can_tx_drop_count_;
    ROS_ERROR_THROTTLE(1.0, "raw CAN TX failed: %s", error.what());
  }
}

bool Ioniq5EcanNode::arm_callback(std_srvs::SetBool::Request& request,
                                  std_srvs::SetBool::Response& response) {
  if (request.data && !safety_config_.allow_actuation) {
    response.success = false;
    response.message = "safety/allow_actuation is false in YAML";
    return true;
  }
  if (request.data && recovery_pending_.load()) {
    response.success = false;
    response.message = "ECU restoration is pending; wait for recovery before requesting arm";
    return true;
  }
  auto_arm_inhibited_ = !request.data;
  operator_disarmed_ = !request.data;
  button_rearm_takeover_ = false;
  command_gap_resume_qualified_ = false;
  preserve_command_session_ = false;
  if (request.data) recovery_rearm_required_ = false;
  requested_arm_ = request.data;
  arm_request_generation_.fetch_add(1U);
  response.success = true;
  response.message =
    request.data ? "arm requested; Panda controls_allowed is still required" : "disarm requested";
  return true;
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
        bool restore_pending = false;
        {
          std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
          panda_->connect();
          vehicle_safety_mode_ = false;
          restore_pending = camera_disabled_ || radar_disabled_;
        }
        ROS_INFO("Connected to Red Panda serial=%s", panda_->serial().c_str());
        if (restore_pending) {
          ROS_WARN("Panda reconnected while an ADAS ECU restore is pending");
          request_internal_disarm();
        }
        next_health = SteadyClock::now();
      }

      for (const CanFrame& frame : panda_->receive()) {
        observe_control_frame(frame);
        publish_raw_can(frame);
        parser_->update(frame);
      }
      const auto now = SteadyClock::now();
      if (now >= next_health) {
        const uint64_t health_epoch = safety_transition_epoch_.load();
        PandaHealth health = panda_->health();
        health.last_rejected_address = last_rejected_address_.load();
        std::lock_guard<std::mutex> lock(health_mutex_);
        if (health_epoch % 2U == 0U && health_epoch == safety_transition_epoch_.load()) {
          latest_health_ = health;
        }
        next_health = now + health_period;
      }
    } catch (const std::exception& error) {
      ROS_ERROR_THROTTLE(2.0, "Panda receive/health error: %s", error.what());
      {
        std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
        panda_->disconnect();
        vehicle_safety_mode_ = false;
      }
      uds_condition_.notify_all();
      request_internal_disarm();
      applied_arm_ = false;
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
  ControlState previous_state = ControlState::Disconnected;
  std::vector<CanFrame> frames;
  frames.reserve(4);

  while (running_) {
    std::this_thread::sleep_until(next);
    // Restoration can wait for UDS replies; sample command/health/state only afterwards.
    retry_no_output_recovery(SteadyClock::now());
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
    const VehicleStateData vehicle = parser_->snapshot(now, vehicle_state_timeout_);

    const bool command_fresh = command.received_at.time_since_epoch().count() != 0 &&
                               now >= command.received_at &&
                               now - command.received_at <= safety_config_.command_timeout;
    CanFrame unused_stock_scc;
    const bool longitudinal_preflight_ready =
      !safety_config_.allow_longitudinal ||
      (vehicle.standstill && vehicle.speed_mps <= kDisableMaximumSpeedMps && vehicle.gear == 5U &&
       copy_recent_scc_template(now, unused_stock_scc));
    const bool listener_ready = safety_config_.resume_on_command_return &&
                                panda_health.ignition_on &&
                                panda_health.safety_mode == PandaUsb::kSafetyNoOutput &&
                                panda_health.safety_param == safety_config_.required_safety_param &&
                                panda_health.command_session_active &&
                                panda_health.command_session_ready &&
                                !panda_health.command_session_blocked;
    // User decision: after any fault, a new physical LDA press or SET release re-engages.
    // The press acknowledges the latched fault; the reinstalled Panda listener records the same
    // edge as channel intent, and the next ECU takeover may happen while moving.
    const bool engage_press = vehicle.lane_keep_button_events > rearm_lane_keep_events_ ||
                              vehicle.set_button_events > rearm_set_events_;
    rearm_lane_keep_events_ = vehicle.lane_keep_button_events;
    rearm_set_events_ = vehicle.set_button_events;
    if (engage_press && safety_config_.resume_on_command_return && !operator_disarmed_.load() &&
        !requested_arm_.load() && (auto_arm_inhibited_.load() || recovery_rearm_required_.load())) {
      auto_arm_inhibited_ = false;
      recovery_rearm_required_ = false;
      button_rearm_takeover_ = true;
      ROS_WARN("Physical LDA/SET press after fault: re-engaging when the listener is ready");
    }

    const bool auto_arm_ready = auto_arm_on_command_ && !auto_arm_inhibited_.load() &&
                                !recovery_pending_.load() &&
                                safety_config_.allow_actuation && vehicle.valid && !vehicle.eps_fault &&
                                panda_health.connected &&
                                (listener_ready || (command_fresh && command.valid && command.enable &&
                                                     longitudinal_preflight_ready &&
                                                     !safety_config_.resume_on_command_return));
    if (auto_arm_ready && !requested_arm_.load()) {
      requested_arm_ = true;
      arm_request_generation_.fetch_add(1U);
    }

    const bool requested = requested_arm_.load();
    const uint64_t request_generation = arm_request_generation_.load();
    if (request_generation != applied_arm_generation && (!requested || panda_->connected())) {
      applied_arm_generation = request_generation;
      if (supervisor_->request_arm(requested)) {
        applied_arm_ = requested;
        try {
          if (requested && safety_config_.resume_on_command_return &&
              !panda_health.command_session_active) {
            std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
            SafetyTransition transition(safety_transition_epoch_);
            panda_->set_safety_mode(PandaUsb::kSafetyNoOutput,
                                    safety_config_.required_safety_param);
            {
              std::lock_guard<std::mutex> lock(health_mutex_);
              latest_health_ = panda_->health();
            }
            previous = SteadyClock::now();
            next = previous + period;
            continue;
          }
          if (requested && panda_->connected() && !safety_config_.resume_on_command_return) {
            enter_vehicle_safety_mode(vehicle);
            previous = SteadyClock::now();
            next = previous + period;
            continue;  // Never use a command sampled before a blocking ECU transition.
          }
          if (!requested && panda_->connected()) {
            if (recovery_pending_.load() && !vehicle_safety_mode_.load()) {
              retry_no_output_recovery(SteadyClock::now());
            } else {
              enter_no_output_mode();
            }
          }
        } catch (const std::exception& error) {
          ROS_ERROR("failed to change Panda safety mode: %s", error.what());
          request_internal_disarm();
          supervisor_->request_arm(false);
          applied_arm_ = false;
          if (panda_->connected()) retry_no_output_recovery(SteadyClock::now());
        }
      } else {
        request_internal_disarm();
        applied_arm_ = false;
      }
    }

    SafetyDecision decision = supervisor_->update(now, vehicle, panda_health, command);

    if (safety_config_.resume_on_command_return) {
      if (!decision.longitudinal_armed && panda_health.longitudinal_selected) {
        try {
          std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
          panda_->drop_longitudinal_permission();  // Can only remove, never grant, permission.
        } catch (const std::exception& error) {
          ROS_ERROR("failed to latch Panda longitudinal OFF: %s", error.what());
          request_internal_disarm();
          continue;
        }
      }
      if (!decision.lateral_armed && !decision.longitudinal_armed) {
        command_gap_resume_qualified_ = false;  // Physical OFF revokes moving-resume eligibility.
      }
      if (decision.waiting_for_command && vehicle_safety_mode_.load()) {
        command_gap_resume_qualified_ = previous_state == ControlState::Active &&
                                       (decision.lateral_armed || decision.longitudinal_armed);
        preserve_command_session_ = true;
        adapter_->reset(vehicle);
        try {
          enter_no_output_mode();
        } catch (const std::exception& error) {
          ROS_ERROR("command-gap stock restoration failed: %s", error.what());
          request_internal_disarm();
        }
        {
          std::lock_guard<std::mutex> lock(decision_mutex_);
          latest_decision_ = decision;
        }
        previous_state = decision.state;
        previous = SteadyClock::now();
        next = previous + period;
        continue;
      }

      const bool moving_resume = (command_gap_resume_qualified_.load() ||
                                  button_rearm_takeover_.load()) &&
                                 panda_health.command_session_active &&
                                 panda_health.command_session_ready &&
                                 !panda_health.command_session_blocked;
      const bool takeover_ready = applied_arm_.load() && requested_arm_.load() &&
                                  panda_health.safety_mode == PandaUsb::kSafetyNoOutput &&
                                  panda_health.safety_param == safety_config_.required_safety_param &&
                                  !vehicle_safety_mode_.load() && !recovery_pending_.load() &&
                                  decision.state == ControlState::Armed &&
                                  (decision.lateral_armed || decision.longitudinal_armed) &&
                                  command_fresh && command.valid && command.enable &&
                                  panda_health.command_session_ready &&
                                  (vehicle.standstill || moving_resume);
      if (takeover_ready) {
        try {
          enter_vehicle_safety_mode(vehicle, moving_resume);
          adapter_->reset(parser_->snapshot(SteadyClock::now(), vehicle_state_timeout_));
          {
            std::lock_guard<std::mutex> lock(health_mutex_);
            latest_health_ = panda_->health();
          }
        } catch (const std::exception& error) {
          ROS_ERROR("verified ECU takeover failed: %s", error.what());
          request_internal_disarm();
        }
        previous = SteadyClock::now();
        next = previous + period;
        continue;  // Resample latest commands and CAN after diagnostic/quiet-period waits.
      }
    }

    if (!supervisor_->arm_requested() && applied_arm_.load()) {
      // The cleanup generation is handled here; do not implicitly acknowledge a latched fault.
      // Preserve any newer operator request that arrives during cleanup.
      const bool inactive_command_wait = decision.state == ControlState::Passive && !command_fresh;
      applied_arm_generation = request_internal_disarm(!inactive_command_wait);
      applied_arm_ = false;
      try {
        if (panda_->connected()) enter_no_output_mode();
      } catch (const std::exception& error) {
        ROS_ERROR("failed to enter NO_OUTPUT: %s", error.what());
      }
      previous = SteadyClock::now();
      next = previous + period;
      previous_state = decision.state;
      {
        std::lock_guard<std::mutex> lock(decision_mutex_);
        latest_decision_ = decision;
      }
      continue;  // No stale output or long rate-integration step after restoration.
    }

    ControlOutput output;
    try {
      output = adapter_->update(command, vehicle, dt, decision.lateral_allowed,
                                decision.longitudinal_allowed);
    } catch (const std::exception& error) {
      ROS_ERROR_THROTTLE(1.0, "actuation command cannot be represented: %s", error.what());
      request_internal_disarm();
      decision.lateral_allowed = false;
      decision.longitudinal_allowed = false;
      decision.heartbeat_engaged = false;
      output = adapter_->update(CommandSample{}, vehicle, dt, false, false);
    }
    {
      std::lock_guard<std::mutex> lock(decision_mutex_);
      latest_decision_ = decision;
    }
    try {
      std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
      if (vehicle_safety_mode_.load() && panda_->connected() &&
          (!safety_config_.resume_on_command_return || panda_health.command_session_ready)) {
        frames.clear();
        const bool steer_request = output.steering_request;
        frames.push_back(codec_.make_lfa(output.steering_torque, decision.lateral_allowed,
                                         steer_request, ecan_bus_));
        if (frame_index % 5U == 0U) {
          frames.push_back(codec_.make_lfa_cluster(decision.lateral_allowed, ecan_bus_));
        }
        if (safety_config_.allow_longitudinal && frame_index % 2U == 0U) {
          if (!codec_.has_scc_control_template()) {
            throw std::runtime_error("stock SCC_CONTROL template was lost");
          }
          const double target = output.longitudinal_active ? output.acceleration_mps2 : 0.0;
          const bool acc_enabled = decision.longitudinal_allowed;
          frames.push_back(codec_.make_scc_control(
            acc_enabled ? target : 0.0, acc_enabled ? target : 0.0, acc_enabled, output.stopping,
            safety_config_.longitudinal_override_on_gas && vehicle.gas_pressed, set_speed_kph_,
            adapter_config_.jerk_limit_mps3, ecan_bus_));
          frames.push_back(codec_.make_fca_warning(ecan_bus_));
        }
        panda_->send(frames);
        maintain_disabled_ecus(now);
      }
    } catch (const std::exception& error) {
      ROS_ERROR_THROTTLE(1.0, "Panda control send failed: %s", error.what());
      request_internal_disarm();
    }

    const bool just_activated =
      decision.state == ControlState::Active && previous_state != ControlState::Active;
    if (panda_->connected() && (just_activated || frame_index % 50U == 0U)) {
      try {
        std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
        panda_->send_heartbeat(decision.heartbeat_engaged);
      } catch (const std::exception& error) {
        ROS_ERROR_THROTTLE(1.0, "Panda heartbeat failed: %s", error.what());
      }
    }
    previous_state = decision.state;
    ++frame_index;
  }
}

uint64_t Ioniq5EcanNode::request_internal_disarm(bool require_operator_rearm) {
  preserve_command_session_ = false;
  command_gap_resume_qualified_ = false;
  button_rearm_takeover_ = false;  // A failed re-engagement needs a new physical press.
  const bool was_armed = requested_arm_.exchange(false) || applied_arm_.load() ||
                         vehicle_safety_mode_.load();
  if (was_armed && require_operator_rearm) {
    auto_arm_inhibited_ = true;
    recovery_rearm_required_ = true;
  }
  recovery_pending_ = true;
  return arm_request_generation_.fetch_add(1U) + 1U;
}

void Ioniq5EcanNode::enter_vehicle_safety_mode(const VehicleStateData& vehicle, bool command_gap_resume) {
  std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
  SafetyTransition transition(safety_transition_epoch_);
  if (vehicle_safety_mode_.load()) return;
  if (recovery_pending_.load()) {
    throw std::runtime_error("ECU restoration must finish before rearming");
  }
  if (!vehicle.valid || vehicle.eps_fault) {
    throw std::runtime_error("vehicle state or EPS is not ready for ADAS ECU disable");
  }
  if ((!vehicle.standstill || vehicle.speed_mps > kDisableMaximumSpeedMps) &&
      !(command_gap_resume &&
        (command_gap_resume_qualified_.load() || button_rearm_takeover_.load()) &&
        safety_config_.resume_on_command_return)) {
    throw std::runtime_error("ADAS ECUs may only be disabled while the vehicle is stationary");
  }

  CanFrame stock_scc;
  if (safety_config_.allow_longitudinal) {
    if (vehicle.gear != 5U) {
      throw std::runtime_error("longitudinal control must be armed in D (gear=5)");
    }
    if (!copy_recent_scc_template(SteadyClock::now(), stock_scc)) {
      throw std::runtime_error("no recent stock ECAN SCC_CONTROL 0x1A0 template");
    }
  }

  panda_->send_heartbeat(false);
  panda_->set_safety_mode(PandaUsb::kSafetyElm327,
    safety_config_.resume_on_command_return ? safety_config_.required_safety_param
                                          : PandaUsb::kElm327EcanParam);
  disable_ecu("CAMERA", kCameraRequestAddress, kCameraResponseAddress,
              HyundaiCanFdCodec::kLfaAddress, stock_lfa_count_, camera_disabled_,
              last_camera_tester_present_);
  panda_->send_heartbeat(false);
  if (safety_config_.allow_longitudinal) {
    disable_ecu("RADAR", kRadarRequestAddress, kRadarResponseAddress,
                HyundaiCanFdCodec::kSccControlAddress, stock_scc_count_, radar_disabled_,
                last_radar_tester_present_);
    panda_->send_heartbeat(false);
  }

  codec_.reset_counters();
  if (safety_config_.allow_longitudinal) codec_.set_scc_control_template(stock_scc);
  const uint16_t configured_param = safety_config_.required_safety_param;
  panda_->set_safety_mode(PandaUsb::kSafetyHyundaiCanFd, configured_param);
  if (safety_config_.resume_on_command_return) {
    const auto deadline = SteadyClock::now() + std::chrono::milliseconds(500);
    PandaHealth readiness = panda_->health();
    while (!readiness.command_session_ready) {
      if (SteadyClock::now() >= deadline) {
        throw std::runtime_error("Panda command-session CAN did not become ready after takeover: "
          "active=" + std::to_string(readiness.command_session_active) +
          " blocked=" + std::to_string(readiness.command_session_blocked) +
          " rx_invalid=" + std::to_string(readiness.safety_rx_invalid) +
          " rx_checks_invalid=" + std::to_string(readiness.safety_rx_checks_invalid) +
          " faults=" + std::to_string(readiness.faults));
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      readiness = panda_->health();
    }
  }
  if (safety_config_.allow_longitudinal) verify_longitudinal_firmware();
  const PandaHealth configured_health = panda_->health();
  if (configured_health.safety_mode != PandaUsb::kSafetyHyundaiCanFd ||
      configured_health.safety_param != configured_param) {
    throw std::runtime_error("Panda rejected the requested Hyundai CAN-FD safety mode");
  }
  maintain_disabled_ecus(SteadyClock::now());
  vehicle_safety_mode_ = true;
  button_rearm_takeover_ = false;
  ROS_WARN("Panda HYUNDAI_CANFD safety enabled with param=%u", configured_param);
}

void Ioniq5EcanNode::enter_no_output_mode() {
  std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
  SafetyTransition transition(safety_transition_epoch_);
  vehicle_safety_mode_ = false;
  recovery_pending_ = true;
  recovery_retry_.request(SteadyClock::now());
  recovery_attempts_ = recovery_retry_.failures() + 1U;
  std::string errors;
  const auto record_error = [&errors](const char* stage, const std::exception& error) {
    if (!errors.empty()) errors += "; ";
    errors += std::string(stage) + ": " + error.what();
  };

  try {
    panda_->send_heartbeat(false);
  } catch (const std::exception& error) {
    record_error("heartbeat", error);
  }

  bool elm_ready = true;
  if (camera_disabled_ || radar_disabled_) {
    try {
      panda_->set_safety_mode(PandaUsb::kSafetyElm327,
        preserve_command_session_.load() ? safety_config_.required_safety_param
                                        : PandaUsb::kElm327EcanParam);
    } catch (const std::exception& error) {
      elm_ready = false;
      record_error("ELM327 restore mode", error);
    }
  }
  if (elm_ready && radar_disabled_) {
    try {
      restore_ecu("RADAR", kRadarRequestAddress, kRadarResponseAddress,
                  HyundaiCanFdCodec::kSccControlAddress, stock_scc_count_, radar_disabled_);
      panda_->send_heartbeat(false);
    } catch (const std::exception& error) {
      record_error("radar restore", error);
    }
  }
  if (elm_ready && camera_disabled_) {
    try {
      restore_ecu("CAMERA", kCameraRequestAddress, kCameraResponseAddress,
                  HyundaiCanFdCodec::kLfaAddress, stock_lfa_count_, camera_disabled_);
      panda_->send_heartbeat(false);
    } catch (const std::exception& error) {
      record_error("camera restore", error);
    }
  }
  // Camera communication control also silences SCC on this vehicle. Restore
  // both communications first; a radar ACK alone cannot make stock SCC resume.
  // Keep each disabled flag until new checksum-valid stock frames are observed.
  if (elm_ready && radar_disabled_) {
    try {
      confirm_ecu_restored("RADAR", HyundaiCanFdCodec::kSccControlAddress,
                           stock_scc_count_, radar_disabled_);
    } catch (const std::exception& error) {
      record_error("radar stock confirmation", error);
    }
  }
  if (elm_ready && camera_disabled_) {
    try {
      confirm_ecu_restored("CAMERA", HyundaiCanFdCodec::kLfaAddress,
                           stock_lfa_count_, camera_disabled_);
    } catch (const std::exception& error) {
      record_error("camera stock confirmation", error);
    }
  }

  try {
    const uint16_t standby_param = recovery_standby_param(
      safety_config_.required_safety_param, preserve_command_session_.load(),
      safety_config_.resume_on_command_return, operator_disarmed_.load());
    if (standby_param != 0U && !preserve_command_session_.load()) {
      // Unless a healthy command gap preserves intent, pass through profile 0 so Panda clears a
      // blocked session and the reinstalled listener records the next physical press as intent.
      panda_->set_safety_mode(PandaUsb::kSafetyNoOutput, 0U);
    }
    panda_->set_safety_mode(PandaUsb::kSafetyNoOutput, standby_param);
    const PandaHealth health = panda_->health();
    if (health.safety_mode != PandaUsb::kSafetyNoOutput || health.safety_param != standby_param ||
        health.controls_allowed) {
      throw std::runtime_error("Panda NO_OUTPUT confirmation failed");
    }
    {
      std::lock_guard<std::mutex> lock(health_mutex_);
      latest_health_ = health;
    }
  } catch (const std::exception& error) {
    record_error("NO_OUTPUT", error);
  }
  codec_.clear_scc_control_template();
  codec_.reset_counters();
  if (!errors.empty()) {
    auto_arm_inhibited_ = true;
    recovery_rearm_required_ = true;
    recovery_retry_.failed(SteadyClock::now());
    throw std::runtime_error(errors);
  }
  recovery_retry_.restored();
  recovery_pending_ = false;
  ROS_INFO("ECU restoration confirmed; Panda returned to NO_OUTPUT%s",
           recovery_rearm_required_.load() ? "; press LDA or SET to re-engage" : "");
}

void Ioniq5EcanNode::retry_no_output_recovery(TimePoint now) {
  {
    std::lock_guard<std::mutex> actuation_lock(actuation_mutex_);
    if (!recovery_pending_.load() || requested_arm_.load() || applied_arm_.load() ||
        vehicle_safety_mode_.load() || !panda_->connected()) {
      return;
    }
    recovery_retry_.request(now);
    if (!recovery_retry_.due(now)) return;
  }
  try {
    enter_no_output_mode();
  } catch (const std::exception& error) {
    ROS_ERROR("ECU restoration attempt %llu failed; will retry with capped backoff: %s",
              static_cast<unsigned long long>(recovery_attempts_.load()), error.what());
  }
}

void Ioniq5EcanNode::verify_longitudinal_firmware() {
  const PandaHealth before = panda_->health();
  panda_->send(
    codec_.make_scc_control(0.0, 0.0, false, false, false, set_speed_kph_, 1.0, ecan_bus_));
  PandaHealth after = before;
  for (int attempt = 0; attempt < 5; ++attempt) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    after = panda_->health();
    if (after.safety_tx_blocked != before.safety_tx_blocked) break;
  }
  if (after.safety_tx_blocked != before.safety_tx_blocked) {
    throw std::runtime_error(
      "Panda firmware does not allow HYUNDAI longitudinal; flash pinned DEBUG firmware");
  }
}

void Ioniq5EcanNode::observe_control_frame(const CanFrame& frame) {
  if (frame.rejected) {
    last_rejected_address_ = frame.address;
    return;
  }
  if (frame.returned || frame.bus != ecan_bus_) return;

  if (frame.address == HyundaiCanFdCodec::kLfaAddress) {
    if (frame.size != 16U || !HyundaiCanFdCodec::checksum_valid(frame)) return;
    ++stock_lfa_count_;
  } else if (frame.address == HyundaiCanFdCodec::kSccControlAddress) {
    if (frame.size != 32U || !HyundaiCanFdCodec::checksum_valid(frame)) return;
    ++stock_scc_count_;
    std::lock_guard<std::mutex> lock(stock_scc_mutex_);
    latest_stock_scc_ = frame;
    have_stock_scc_ = true;
  }

  if (frame.size != 8U ||
      (frame.address != kCameraResponseAddress && frame.address != kRadarResponseAddress)) {
    return;
  }
  const uint8_t payload_size = frame.data[0] & 0x0FU;
  if ((frame.data[0] & 0xF0U) != 0U || payload_size == 0U || payload_size > 7U) return;

  std::lock_guard<std::mutex> lock(uds_mutex_);
  if (!uds_waiting_ || frame.address != uds_response_address_) return;
  uds_response_.assign(frame.data.begin() + 1, frame.data.begin() + 1 + payload_size);
  uds_waiting_ = false;
  uds_condition_.notify_all();
}

std::vector<uint8_t> Ioniq5EcanNode::uds_request(uint32_t request_address,
                                                 uint32_t response_address,
                                                 const std::vector<uint8_t>& payload,
                                                 const std::vector<uint8_t>& expected_prefix) {
  {
    std::lock_guard<std::mutex> lock(uds_mutex_);
    if (uds_waiting_) throw std::logic_error("another UDS request is already pending");
    uds_waiting_ = true;
    uds_response_address_ = response_address;
    uds_response_.clear();
  }

  try {
    send_uds(request_address, payload);
  } catch (...) {
    std::lock_guard<std::mutex> lock(uds_mutex_);
    uds_waiting_ = false;
    uds_response_address_ = 0U;
    throw;
  }

  std::unique_lock<std::mutex> lock(uds_mutex_);
  const bool completed = uds_condition_.wait_for(lock, kUdsResponseTimeout, [this] {
    return !uds_waiting_ || !running_.load() || !panda_->connected();
  });
  if (!completed || uds_waiting_ || uds_response_.empty()) {
    uds_waiting_ = false;
    uds_response_address_ = 0U;
    throw std::runtime_error("UDS response timeout for " + bytes_as_hex(payload));
  }
  std::vector<uint8_t> response = uds_response_;
  uds_response_.clear();
  uds_response_address_ = 0U;
  lock.unlock();

  if (response.front() == 0x7FU) {
    throw std::runtime_error("UDS negative response: " + bytes_as_hex(response));
  }
  if (!starts_with(response, expected_prefix)) {
    throw std::runtime_error("unexpected UDS response: " + bytes_as_hex(response));
  }
  return response;
}

void Ioniq5EcanNode::send_uds(uint32_t request_address, const std::vector<uint8_t>& payload) {
  if (payload.empty() || payload.size() > 7U) {
    throw std::invalid_argument("only single-frame UDS payloads are supported");
  }
  CanFrame frame;
  frame.address = request_address;
  frame.bus = ecan_bus_;
  frame.size = 8U;
  frame.data[0] = static_cast<uint8_t>(payload.size());
  std::copy(payload.begin(), payload.end(), frame.data.begin() + 1);
  panda_->send(frame);
}

void Ioniq5EcanNode::send_tester_present(uint32_t request_address, TimePoint& last_sent) {
  send_uds(request_address, {0x3EU, 0x80U});
  last_sent = SteadyClock::now();
}

void Ioniq5EcanNode::disable_ecu(const char* label, uint32_t request_address,
                                 uint32_t response_address, uint32_t owned_address,
                                 std::atomic<uint64_t>& owned_count, bool& disabled,
                                 TimePoint& last_tester_present) {
  if (disabled) return;
  uds_request(request_address, response_address, {0x10U, 0x03U}, {0x50U, 0x03U});
  // Sub-function 0x83 suppresses the positive response. Quieting the owned frame confirms it.
  send_uds(request_address, {0x28U, 0x83U, 0x01U});
  disabled = true;
  send_tester_present(request_address, last_tester_present);
  std::this_thread::sleep_for(kEcuQuietSettle);
  const uint64_t before = owned_count.load();
  const auto deadline = SteadyClock::now() + kEcuQuietObservation;
  while (running_.load() && SteadyClock::now() < deadline) {
    maintain_disabled_ecus(SteadyClock::now());
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  const uint64_t observed = owned_count.load() - before;
  if (observed != 0U) {
    throw std::runtime_error(std::string(label) + " disable failed: observed " +
                             std::to_string(observed) + " stock 0x" +
                             [&] {
                               std::ostringstream stream;
                               stream << std::hex << owned_address;
                               return stream.str();
                             }() +
                             " frames");
  }
  ROS_WARN("%s_DISABLED request=0x%X stock_0x%X=quiet", label, request_address, owned_address);
}

void Ioniq5EcanNode::restore_ecu(const char* label, uint32_t request_address,
                                 uint32_t response_address, uint32_t owned_address,
                                 std::atomic<uint64_t>& owned_count, bool& disabled) {
  if (!disabled) return;
  // A previous attempt may have restored communication but lost its acknowledgement.
  // Confirm live stock traffic before repeating diagnostics against an already restored ECU.
  if (recovery_retry_.failures() != 0U) {
    const uint64_t before = owned_count.load();
    const auto deadline = SteadyClock::now() + kEcuRestoreObservation;
    while (running_.load() && SteadyClock::now() < deadline && owned_count.load() == before) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (owned_count.load() != before) {
      disabled = false;
      ROS_INFO("%s_RESTORED confirmed by resumed stock_0x%X", label, owned_address);
      return;
    }
  }
  // Re-enter the diagnostic session because a timed-out previous restore may have
  // returned the ECU to its default session already.
  uds_request(request_address, response_address, {0x10U, 0x03U}, {0x50U, 0x03U});
  uds_request(request_address, response_address, {0x28U, 0x00U, 0x01U}, {0x68U, 0x00U});
  uds_request(request_address, response_address, {0x10U, 0x01U}, {0x50U, 0x01U});
  ROS_INFO("%s communication restore acknowledged; stock confirmation pending", label);
}

void Ioniq5EcanNode::confirm_ecu_restored(const char* label, uint32_t owned_address,
                                        std::atomic<uint64_t>& owned_count, bool& disabled) {
  if (!disabled) return;
  const uint64_t before = owned_count.load();
  const auto deadline = SteadyClock::now() + kEcuRestoreObservation;
  while (running_.load() && SteadyClock::now() < deadline && owned_count.load() == before) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  const uint64_t resumed = owned_count.load() - before;
  if (resumed == 0U) {
    throw std::runtime_error(std::string(label) + " stock 0x" +
                             [&] {
                               std::ostringstream stream;
                               stream << std::hex << owned_address;
                               return stream.str();
                             }() +
                             " did not resume");
  }
  disabled = false;
  ROS_INFO("%s_RESTORED stock_0x%X=%llu", label, owned_address,
           static_cast<unsigned long long>(resumed));
}

void Ioniq5EcanNode::maintain_disabled_ecus(TimePoint now) {
  if (camera_disabled_ && (last_camera_tester_present_.time_since_epoch().count() == 0 ||
                           now - last_camera_tester_present_ >= kTesterPresentPeriod)) {
    send_tester_present(kCameraRequestAddress, last_camera_tester_present_);
  }
  if (radar_disabled_ && (last_radar_tester_present_.time_since_epoch().count() == 0 ||
                          now - last_radar_tester_present_ >= kTesterPresentPeriod)) {
    send_tester_present(kRadarRequestAddress, last_radar_tester_present_);
  }
}

bool Ioniq5EcanNode::copy_recent_scc_template(TimePoint now, CanFrame& frame) const {
  std::lock_guard<std::mutex> lock(stock_scc_mutex_);
  if (!have_stock_scc_ || latest_stock_scc_.received_at.time_since_epoch().count() == 0 ||
      now < latest_stock_scc_.received_at ||
      now - latest_stock_scc_.received_at > kStockSccTimeout) {
    return false;
  }
  frame = latest_stock_scc_;
  return true;
}

void Ioniq5EcanNode::publish_status(const ros::TimerEvent&) {
  const auto steady_now = SteadyClock::now();
  const VehicleStateData vehicle = parser_->snapshot(steady_now, vehicle_state_timeout_);
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

  VehicleState message;
  message.stamp = ros::Time::now();
  message.valid = vehicle.valid;
  message.speed_mps = vehicle.speed_mps;
  message.yaw_rate_deg_s = vehicle.yaw_rate_deg_s;
  message.lateral_accel_mps2 = vehicle.lateral_accel_mps2;
  message.longitudinal_accel_mps2 = vehicle.longitudinal_accel_mps2;
  message.wheel_speed_fl = vehicle.wheel_speed_fl;
  message.wheel_speed_fr = vehicle.wheel_speed_fr;
  message.wheel_speed_rl = vehicle.wheel_speed_rl;
  message.wheel_speed_rr = vehicle.wheel_speed_rr;
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
  message.lane_keep_button_pressed = vehicle.lane_keep_button_pressed;
  message.control_state = static_cast<uint8_t>(decision.state);
  message.control_state_name = to_string(decision.state);
  message.control_reason = decision.reason;
  message.lateral_armed = decision.lateral_armed;
  message.longitudinal_armed = decision.longitudinal_armed;
  message.lateral_control_active = decision.lateral_allowed;
  message.longitudinal_control_active = decision.longitudinal_allowed;
  message.panda_connected = panda.connected;
  message.panda_controls_allowed = panda.controls_allowed;
  message.panda_safety_tx_blocked = panda.safety_tx_blocked;
  message.can_checksum_failures = parser_->checksum_failures();
  message.can_malformed_frames = parser_->malformed_frames();
  state_publisher_.publish(message);
  publish_diagnostics(vehicle, panda, decision);
}

void Ioniq5EcanNode::publish_raw_can(const CanFrame& frame) {
  if (!raw_can_rx_publisher_) return;
  RawCanFrame message;
  message.stamp = ros::Time::now();
  message.address = frame.address;
  message.bus = frame.bus;
  message.fd = frame.fd;
  message.extended = frame.extended;
  message.returned = frame.returned;
  message.rejected = frame.rejected;
  message.data.assign(frame.data.begin(), frame.data.begin() + frame.size);
  raw_can_rx_publisher_.publish(message);
  if (frame.bus < raw_can_bus_publishers_.size() && raw_can_bus_publishers_[frame.bus]) {
    raw_can_bus_publishers_[frame.bus].publish(message);
  }
  ++raw_can_rx_count_;
}

void Ioniq5EcanNode::publish_diagnostics(const VehicleStateData& vehicle, const PandaHealth& panda,
                                         const SafetyDecision& decision) {
  diagnostic_msgs::DiagnosticArray array;
  array.header.stamp = ros::Time::now();
  diagnostic_msgs::DiagnosticStatus status;
  status.name = "ioniq5_ecan";
  status.hardware_id = panda_->serial();
  status.level =
    decision.state == ControlState::Fault || decision.state == ControlState::Disconnected
      ? diagnostic_msgs::DiagnosticStatus::ERROR
      : (decision.state == ControlState::Passive || decision.state == ControlState::SoftDisabling
           ? diagnostic_msgs::DiagnosticStatus::WARN
           : diagnostic_msgs::DiagnosticStatus::OK);
  status.message = decision.reason;
  status.values.push_back(key_value("control_state", std::string(to_string(decision.state))));
  status.values.push_back(key_value("vehicle_valid", vehicle.valid ? 1 : 0));
  status.values.push_back(key_value("eps_fault", vehicle.eps_fault ? 1 : 0));
  status.values.push_back(
    key_value("soft_disable_remaining_ms", decision.soft_disable_remaining_ms));
  status.values.push_back(key_value("panda_connected", panda.connected ? 1 : 0));
  status.values.push_back(key_value("panda_controls_allowed", panda.controls_allowed ? 1 : 0));
  status.values.push_back(key_value("waiting_for_command", decision.waiting_for_command ? 1 : 0));
  status.values.push_back(key_value("command_session_ready", panda.command_session_ready ? 1 : 0));
  status.values.push_back(key_value("command_gap_resume_qualified", command_gap_resume_qualified_.load() ? 1 : 0));
  status.values.push_back(key_value("panda_safety_mode", panda.safety_mode));
  status.values.push_back(key_value("panda_safety_param", panda.safety_param));
  status.values.push_back(key_value("panda_faults", panda.faults));
  status.values.push_back(key_value("panda_ignored_faults", panda.ignored_faults));
  status.values.push_back(key_value("panda_fault_status", panda.fault_status));
  status.values.push_back(key_value("panda_bus_off", panda.bus_off ? 1 : 0));
  status.values.push_back(key_value("safety_tx_blocked", panda.safety_tx_blocked));
  status.values.push_back(key_value("last_rejected_can_address", panda.last_rejected_address));
  status.values.push_back(key_value("ecu_recovery_pending", recovery_pending_.load() ? 1 : 0));
  status.values.push_back(key_value("ecu_recovery_attempts", recovery_attempts_.load()));
  status.values.push_back(
    key_value("recovery_rearm_required", recovery_rearm_required_.load() ? 1 : 0));
  status.values.push_back(key_value("raw_can_rx_count", raw_can_rx_count_.load()));
  status.values.push_back(key_value("raw_can_tx_count", raw_can_tx_count_.load()));
  status.values.push_back(key_value("raw_can_tx_drop_count", raw_can_tx_drop_count_.load()));
  status.values.push_back(key_value("can_checksum_failures", parser_->checksum_failures()));
  array.status.push_back(status);
  diagnostics_publisher_.publish(array);
}

void Ioniq5EcanNode::apply_realtime_settings(const char* name, int priority, int cpu) {
#ifdef __linux__
  pthread_setname_np(pthread_self(), name);
  if (cpu >= 0) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    if (pthread_setaffinity_np(pthread_self(), sizeof(set), &set) != 0) {
      ROS_WARN("failed to set CPU affinity for %s", name);
    }
  }
  if (priority > 0) {
    sched_param parameters{};
    parameters.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &parameters) != 0) {
      ROS_WARN("failed to set SCHED_FIFO for %s", name);
    }
  }
#else
  (void)name;
  (void)priority;
  (void)cpu;
#endif
}

}  // namespace ioniq5_ecan
