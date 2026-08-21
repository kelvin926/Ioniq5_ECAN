# Latency and scheduling

기본 설정은 일반 Linux scheduler에서 동작합니다. 먼저 이 상태로 안정성을 확보하고,
측정 결과가 필요할 때만 RT 권한을 추가합니다.

- command subscription: TCPROS, queue 1, TCP_NODELAY
- control loop: steady-clock 100 Hz
- CAN RX: 별도 libusb thread
- state publish: 20 Hz
- Panda health: 10 Hz
- heartbeat: 2 Hz, ACTIVE 진입 시 즉시 1회

YAML의 `runtime.realtime_priority`, `control_cpu`, `receive_cpu`로 `SCHED_FIFO`와 CPU
affinity를 선택할 수 있습니다. RT priority를 쓰려면 systemd unit 또는 limits 설정에서
`LimitRTPRIO`, `LimitMEMLOCK` 권한을 부여해야 합니다. 권한 설정 실패는 warning으로
보고하고 일반 scheduler로 계속 동작합니다.

측정 시에는 command source timestamp와 `/ioniq5/vehicle_state.stamp`, Panda returned
frame 또는 외부 CAN logger를 함께 기록하십시오. ROS timestamp만으로 실제 bus 도착
지연을 단정하지 마십시오.
