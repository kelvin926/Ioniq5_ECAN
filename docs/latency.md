# Latency and scheduling

2026-10-07 현재 기본 YAML의 목표 주기입니다. 실제 bus 도착 주기나 최대 지연을 보장하는
측정 결과는 아직 없습니다.

| 경로 | 기본 동작 |
| --- | --- |
| command subscription | TCPROS, queue 1, TCP_NODELAY, 최신 값 교환 |
| control loop / LFA | steady clock, 목표 100 Hz |
| SCC / FCA | control loop 두 주기마다, 목표 50 Hz |
| cluster | control loop 다섯 주기마다, 목표 20 Hz |
| CAN RX | 별도 libusb thread, USB read timeout 20 ms |
| raw TX | ROS callback에서 gate 확인 후 직접 USB write, subscriber queue 256 |
| vehicle state / diagnostics | ROS timer, 20 Hz |
| Panda health | 10 Hz |
| heartbeat | 2 Hz, `ACTIVE` 진입과 일시 오류 복귀 시 즉시 전송 |
| disabled ECU tester-present | 정상 제어/복귀 대기에서 각 ECU 0.8초 이내 목표 |

기본 watchdog은 command 250 ms, Panda health 500 ms, critical CAN 250 ms입니다.
일시 EPS 복귀 deadline 3초와 ECU 복구 backoff 1~30초는 별도 상태기계 시간이며
YAML watchdog과 같은 제한이 아닙니다.

UDS takeover/restore는 control thread에서 동기 실행하여 제어 cadence를 지연시킬 수 있습니다.
USB 대기, scheduler, CAN 묶음/packet의 동적 할당도 남아 있습니다. 복구 재시도 후에는
최신 command/state/health를 다시 읽지만 hard real-time 보장은 하지 않습니다.

`runtime.realtime_priority`, `control_cpu`, `receive_cpu`로 `SCHED_FIFO` 및 CPU affinity를
선택할 수 있습니다. 기본 `0/-1/-1`은 추가 권한 없이 일반 scheduler에서 동작합니다.
RT 사용 시 systemd 또는 limits의 RT 권한이 필요하며, 설정 실패는 warning 후 일반
scheduler로 계속 동작합니다.

지연 측정에는 source timestamp, callback 수신 시각, codec 출력 시각, Panda returned frame,
외부 CAN logger 시각을 함께 사용하고 clock domain을 맞춥니다. 제어 노드의
`RawCanFrame.stamp`와 `VehicleState.stamp`는 ROS publish 시각입니다.
[독립 수신 기록기](can_logger.md)의 RAW/CSV 시각은 host USB read 완료 시각이며 한 read의
프레임들이 시각을 공유할 수 있습니다. 모두 하드웨어 CAN 도착 timestamp와 다릅니다.
software 출력 또는 returned frame만으로 ECU 적용 시각과 실제 actuator 응답을 단정하지 않습니다.

동시 기록 센서를 대조할 때는 헤더 시계 차이를 먼저 확인하고 같은 토픽의 발행 주체를
분리합니다. 10월 7일 영상은 큰 헤더 시계 차이 때문에 bag 기록 시각을 사용했습니다.
[센서 대조](ecan_bag_reference_inference_20261007.md)의 정렬/필터 시차도 실제 CAN 지연
측정값으로 사용하지 않습니다.
