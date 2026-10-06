# Only lateral and acceleration are required when input.use_enable_field=false.
# Arrival time is used for the watchdog. stamp is retained for latency analysis.
time stamp
# Optional. Zero disables checking; wraparound and restart after command timeout are accepted.
uint32 sequence
# Optional deadman, used only when input.use_enable_field=true.
bool enable

# Agreed upstream input: steering-wheel angle rate in deg/s (not yaw rate or target angle).
# The active profile uses steering_rate_deg_s. Other profiles can select:
# steering_rate_deg_s, steering_rate_rad_s, curvature_1pm, or direct_torque.
# With input.unfiltered_input=true, steering-rate/curvature targets are not host-clamped or smoothed.
float64 lateral

# Interpreted as m/s^2 after configured scale/offset; this field is the pre-scale value.
# CAN/Panda range is -3.5 .. 2.0 m/s^2, with 0.01 m/s^2 encoding resolution.
# With input.unfiltered_input=true, the node does not jerk-limit or silently clamp this value.
float64 acceleration
