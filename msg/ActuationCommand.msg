# Only lateral and acceleration are required when input.use_enable_field=false.
# Arrival time is used for the watchdog. stamp is retained for latency analysis.
time stamp
# Optional. Zero disables sequence checking; wraparound and publisher restart are accepted.
uint32 sequence
# Optional deadman, used only when input.use_enable_field=true.
bool enable

# Meaning is selected by input.lateral_mode in YAML:
# steering_rate_deg_s, steering_rate_rad_s, curvature_1pm, or direct_torque.
float64 lateral

# Physical m/s^2 after applying configured scale and offset.
float64 acceleration
