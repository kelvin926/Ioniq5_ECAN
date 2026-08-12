# Arrival time is used for the hard watchdog. stamp is retained for end-to-end latency analysis.
builtin_interfaces/Time stamp
uint32 sequence
bool enable

# Meaning is selected by input.lateral_mode in YAML:
# steering_rate_deg_s, steering_rate_rad_s, curvature_1pm, or direct_torque.
float64 lateral

# Physical m/s^2 after applying configured scale and offset.
float64 acceleration
