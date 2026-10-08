# Latest values of every ECAN field in config/ecan_signals.json (2026-10-07 offline analysis).
# These are interpretation candidates, not a validated DBC; alternative layouts of the same bits
# appear as separate names with a #index suffix. Arrays are index-aligned and keep the
# definition order. stamp is the ROS publish time.
time stamp
# "<CAN ID>.<field>", for example "0x125.STEERING_ANGLE#004".
string[] name
string[] unit
# raw * factor + offset of the last received frame; NaN until the ID is received.
float64[] value
# Seconds since that ID was last received by this decoder; -1 until received.
float64[] age_s
