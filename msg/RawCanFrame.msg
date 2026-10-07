# Control-node RX stamp is ROS publish time. Receive-only logger uses host USB read time.
# Neither stamp is a hardware CAN arrival timestamp.
# TX uses address/bus/flags/data; returned and rejected must be false.
time stamp
uint32 address
uint8 bus
bool fd
bool extended
bool returned
bool rejected
uint8[] data
