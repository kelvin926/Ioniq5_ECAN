# Only lateral and acceleration are required when input.use_enable_field=false.
# Arrival time is used for the watchdog. stamp is retained for latency analysis.
time stamp
# Optional. Zero disables checking; wraparound and restart after command timeout are accepted.
uint32 sequence
# Optional deadman, used only when input.use_enable_field=true.
bool enable

# Active profile direct_torque: LFA StrTqReqVal torque request in Panda counts computed by the
# upstream controller (not deg/s, angle or Nm). Rounded to an integer and saturated at +/-1021.
# 0 requests zero torque. Other profiles: steering_rate_deg_s, steering_rate_rad_s, curvature_1pm.
float64 lateral

# Interpreted as m/s^2 after configured scale/offset; this field is the pre-scale value.
# CAN/Panda range is -10.23 .. 10.24 m/s^2, with 0.01 m/s^2 encoding resolution.
# The node does not jerk-limit it; values outside the range saturate at the bound.
float64 acceleration
