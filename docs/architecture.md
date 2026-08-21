# Architecture

```text
Alpamayo-side controller
  └─ /ioniq5/actuation_command (TCPROS, queue 1, TCP_NODELAY)
       └─ command adapter
            ├─ rate/curvature → target angle → Carrot lateral-accel torque PID
            └─ acceleration scaling and bounds
                 └─ safety supervisor
                      └─ 100 Hz control thread
                           ├─ LFA 100 Hz
                           ├─ LFAHDA_CLUSTER 20 Hz
                           ├─ SCC_CONTROL 50 Hz (optional)
                           └─ ADRV_0x160 50 Hz (optional)
                                └─ Red Panda libusb
                                     └─ Hyundai K harness / ECAN

Red Panda CAN RX
  └─ /ioniq5/can_rx + /ioniq5/can{0,1,2}/rx
       ├─ raw CAN-FD logging / rosbag / Cabana 변환
       └─ vehicle state parser

/ioniq5/can_tx
  └─ SET longitudinal arm + active gate
       └─ Panda HYUNDAI_CANFD whitelist
            └─ Red Panda CAN TX
```

차선유지(LDA) 버튼의 상승 에지는 조향 채널만, 크루즈 `SET` release는 종방향 채널만
토글합니다. Panda의 `controls_allowed`는 전역 값이므로 저장소의 opt-in firmware 패치가
LDA 입력도 허용 상태로 만들고, host supervisor가 실제 LFA/SCC 출력을 채널별로 분리합니다.

ROS callback은 최신 command 하나만 mutex로 교환합니다. CAN RX와 제어 루프는 별도
스레드이고, 제어 루프는 ROS callback spinner와 독립된 steady clock 100 Hz 주기를 사용합니다.
raw RX는 receive thread에서 즉시 publish하고 raw TX는 ROS callback에서 Panda write mutex로
직접 전달하므로 별도 100 Hz queue 지연을 추가하지 않습니다.
동적 할당은 CAN 프레임 묶음과 USB packet에 남아 있으므로 hard real-time 보장은 하지
않지만, Python/IPC 경계를 제어 hot path에서 제거했습니다.

Carrot 제어 경로는 `latAccelFactor=3.172929`, `friction=0.096019`, PID
`1.0/0.1/0.0/1.0`과 저속 보간표를 기본으로 사용합니다. 목표 조향각과 actuator delay를
적용한 뒤 자전거 모델로 곡률을 계산하므로 입력이 steering rate여도 동일한 토크
제어기를 사용합니다. 모든 값은 YAML에서 변경할 수 있습니다.

입력 메시지는 임시 계약입니다. 최종 계약 변경 시 다음 경계만 수정합니다.

1. `msg/ActuationCommand.msg`
2. `Ioniq5EcanNode::command_callback`
3. 필요하면 `CommandAdapter`

Hyundai codec, Panda USB protocol, parser, safety supervisor는 물리 입력 계약과 분리되어
있습니다.
