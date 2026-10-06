time stamp
# Fresh, checksum-valid critical CAN data; EPS assistance status is separate.
bool valid
float64 speed_mps
float64 yaw_rate_deg_s
float64 lateral_accel_mps2
float64 longitudinal_accel_mps2
# Individual wheel speeds are converted from the DBC's km/h to m/s.
float64 wheel_speed_fl
float64 wheel_speed_fr
float64 wheel_speed_rl
float64 wheel_speed_rr
float64 steering_angle_deg
float64 steering_rate_deg_s
float64 driver_torque
float64 eps_torque_nm
float64 accelerator_pedal
bool brake_pressed
bool gas_pressed
# Decoded MDPS LKA assistance fault, independent of critical-CAN validity above.
bool eps_fault
bool acc_fault
bool cruise_engaged
bool standstill
uint8 gear
uint8 cruise_button
bool lane_keep_button_pressed
# Stable values: 0 DISCONNECTED, 1 PASSIVE, 2 ARMED, 3 ACTIVE, 4 FAULT,
# 5 SOFT_DISABLING (bounded temporary EPS pause).
uint8 control_state
string control_state_name
string control_reason
# Armed fields select channels; active fields report the current output permission.
bool lateral_armed
bool longitudinal_armed
bool lateral_control_active
bool longitudinal_control_active
bool panda_connected
# Firmware global permission; inspect per-channel active fields as well.
bool panda_controls_allowed
uint32 panda_safety_tx_blocked
uint64 can_checksum_failures
uint64 can_malformed_frames
