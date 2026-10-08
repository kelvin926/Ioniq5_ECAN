# Architecture

2026-10-07 현재 ROS C++ 노드의 구조입니다. 실행 타깃은 Ubuntu 20.04 / ROS 1 Noetic입니다.

```text
Upstream controller
  └─ /ioniq5/actuation_command (TCPROS, queue 1, TCP_NODELAY)
       └─ latest-command snapshot
            └─ steady-clock control thread, nominal 100 Hz
                 ├─ supervisor(command + vehicle + Panda health)
                 ├─ ECU ownership / restoration lifecycle
                 └─ command adapter, independent lateral/longitudinal permission
                      ├─ rate/curvature → target angle → feedback torque controller
                      └─ acceleration scale/offset and encoding range check
                           └─ Hyundai CAN-FD codec
                                ├─ LFA 0x12A: 100 Hz
                                ├─ LFAHDA_CLUSTER 0x1E0: 20 Hz
                                └─ SCC 0x1A0 / FCA 0x160: 50 Hz, optional
                                     └─ Red Panda libusb → Hyundai K / ECAN

Red Panda CAN RX, separate thread
  ├─ raw topics: /ioniq5/can_rx + /ioniq5/can{0,1,2}/rx
  ├─ UDS response / stock-frame ownership observations
  └─ vehicle parser → supervisor + adapter + state/diagnostics

/ioniq5/can_tx, ROS callback
  └─ serialized actuation gate + connected Hyundai mode + longitudinal permission
       └─ LFA additionally requires lateral permission
            └─ Panda whitelist/content checks → USB write
```

상태는 `/ioniq5/vehicle_state`로 20 Hz 발행합니다. `stamp`는 ROS 발행 시각이며 `valid`는
steering/MDPS/wheel/TCS의 수신 갱신만 검사합니다. IMU/페달/기어/SCC/버튼의 개별 유효
시각은 제공하지 않습니다. [현재 피드백 필드](vehicle_state.md)에 parser 근거를 정리했습니다.

별도 [수신 전용 기록기](can_logger.md)는 제어/ECU 소유권 경로를 사용하지 않고 SILENT에서
USB IN을 CSV/RAW 토픽으로 기록합니다. [ECAN 추론 자료](ecan_analysis_20261007.md)는
오프라인 결과이며 새 신호 정의를 런타임 parser에 적용한 상태는 아닙니다.

## CAN 소유권과 활성화

Hyundai K의 정상 `harness_status=1`에서 physical CAN1은 Panda logical bus 0 ECAN입니다.
ECAN-only firmware는 나머지 transceiver와 forwarding을 비활성화합니다.
`camera_bus=2` 설정은 남아 있지만 이 차량의 parser와 제어에 사용하지 않습니다.

입력 구독은 대기/OFF/USB 재연결 중에도 유지합니다.
USB 시작 시 plain NO_OUTPUT에서 이전 수신 데이터를 성공한 short transfer 경계까지
비운 뒤 새 데이터를 해석합니다. runtime 체크섬 오류는 재동기화하지 않고 계속 차단합니다.
미arm 초기 연결 실패는 빈 버튼 감시 profile을 복원하지만 hard fault/OFF/종료는 재승인을 요구합니다.
I5R1 기본 프로파일은 firmware가
NO_OUTPUT에서도 물리 버튼 선택을 검증하며, 최신 입력과 ON이 모두 있어야 첫 takeover를
준비합니다. 초기 takeover는 유효한 CAN, EPS 정상 상태와 정차를 요구합니다. 종방향을 켠 프로파일은
D와 최근 순정 SCC template도 요구합니다. camera `0x730/0x738`의 stock LFA `0x12A`를
UDS로 멈추고 quiet를 확인합니다. 종방향이면 radar `0x7D0/0x7D8`의 stock SCC `0x1A0`도
멈추고 확인한 뒤 Hyundai safety mode로 전환합니다. SCC는 마지막 순정 payload에서
소유 신호와 counter/CRC만 덮어써 미확인 비트를 보존합니다.

물리 LDA 상승 에지는 조향 전용, SET release는 조향과 종방향 통합 모드를 선택/토글합니다.
I5R1에서는 firmware의 volatile 버튼 선택을 host가 사용합니다. brake는 종방향만 래치 해제하고
횡방향을 유지합니다. firmware 전역 `controls_allowed`와 채널 허가는 같은 개념이 아닙니다.

## 세 복귀/복구 경로

정상 ACTIVE에서 publisher 입력만 250 ms 끊기면 actuator 출력과 적분을 중단하고
radar→camera 순정 통신 복구/NO_OUTPUT으로 돌아갑니다. 같은 I5R1 프로파일의 진단 및
무출력 모드 전환 동안 ON 선택은 보존됩니다. 이전 ACTIVE 자격, 정상 CAN/ignition,
완료된 stock 복구와 새 command를 확인하면 같은 세션은 주행 중에도 재인계합니다.
첫 takeover의 정차 조건을 전역으로 없애는 기능이 아닙니다. OFF/CANCEL/hard fault 또는
프로세스/USB/Panda 재시작은 이 예외를 취소합니다. 이전 command/dt와 조향 목표는 재사용하지 않습니다.

활성 중 일시 MDPS LKA 보조 오류는 `SOFT_DISABLING`으로 전환합니다. 최초 오류부터
고정 3초 동안 LFA를 0/비활성으로 내리고 arm, heartbeat, ECU 소유권을 유지합니다.
허가된 정상 종방향은 현재 명령을 계속 처리합니다. deadline 전에 오류가 사라지고
최신 명령/CAN 및 Panda 허가가 정상이면 `ACTIVE`로 복귀합니다. 횡방향 적분과 목표각은
실측각으로 초기화하며 raw LFA도 일시 정지를 우회할 수 없습니다.

3초 만료나 CAN/Panda hard fault, EPS pause 중 command 단절은 전체 disarm과 순정 ECU 복구로 전환합니다.
radar→camera 순서로 두 통신 복구를 먼저 요청한 뒤 유효한 stock frame 재개와 Panda `NO_OUTPUT`을 확인합니다.
실패한 복구는 즉시 첫 시도 후 1, 2, 4, 8, 16, 최대 30초 간격으로 재시도합니다.
USB 연결은 별도 재연결 루프를 사용합니다. 복구 중 재arm과 actuator 출력은 차단하며,
완료 후 운전자 acknowledge/rearm 및 정차/물리 버튼 조건을 다시 요구합니다.

정상 제어와 일시 복귀 대기에서는 비활성화한 각 ECU에 0.8초 이내 tester-present를
보냅니다. 일반 종료도 stock ECU 복구를 시도하지만 종료 후에는 재시도 루프가 존재하지
않습니다. 이 로직은 ECU 자체 reset이나 실제 고장 수리 기능이 아닙니다.

## 스레드와 입력 경계

ROS command callback은 mutex로 최신 값 하나를 교환합니다. CAN RX와 제어 루프는 별도
스레드이며 raw RX는 receive thread에서 publish합니다. safety 전환 epoch로 오래된 health
snapshot이 mode 전환 후 상태를 덮어쓰지 못하게 합니다. USB timeout에 부분 수신 bytes가
있으면 packet carry에 보존합니다. raw TX는 actuation mutex로
제어/복구 전환과 직렬화하고 Panda write mutex로 전송합니다. 별도 100 Hz 송신 queue는
추가하지 않습니다.

UDS takeover/restore는 control thread에서 동기 실행하므로 제어 주기와 tester-present
간격이 지연될 수 있습니다. USB, scheduler와 동적 할당도 남아 있어 hard real-time 보장은
없습니다. 복구 재시도 후에는 최신 command/state/health를 다시 읽습니다.

기본 입력(`direct_torque`)은 상위 제어기가 계산한 LFA 토크 count를 반올림하고 ±1021에서
끝값으로 맞춰 그대로 보냅니다. rate/curvature 모드를 고르면 목표각을 적분한 뒤 Carrot Ioniq 5
토크 제어기로 변환합니다. native angle 제어는 구현하지 않았습니다. 입력 변경은 메시지,
callback, adapter 경계에서 수용합니다. 현재 상위 계약은 조향 토크 count와 종방향 가속도
m/s²이며, 실제 토크 부호/발행 주기와 차량 추종 검증은 남아 있습니다.

상세 동작은 [입력 계약](input_contract.md), [상태 및 제한](safety.md),
[raw CAN](raw_can.md), [주기와 측정](latency.md)을 참고하십시오.
