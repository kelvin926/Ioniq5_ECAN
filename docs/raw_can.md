# Raw CAN ROS 1 interface

2026-10-06 현재 노드와 YAML 기준입니다. Red Panda USB는 C++ 노드 하나가 단독 소유합니다.
`pandad`, Cabana `--panda`, 다른 Panda 프로세스와 동시에 열면 interface claim이 충돌합니다.
Cabana 분석에는 ROS 기록을 변환하거나 별도 실행에서 Panda를 사용합니다.

## RX

- `/ioniq5/can_rx`: bus 0/1/2 통합 stream
- `/ioniq5/can0/rx`, `/ioniq5/can1/rx`, `/ioniq5/can2/rx`: bus별 stream

[`RawCanFrame.msg`](../msg/RawCanFrame.msg)는 address, bus, CAN-FD/IDE,
returned/rejected와 최대 64 byte payload를 보존합니다. publisher queue는 256입니다.
ECAN-only firmware에서는 실제 차량 RX가 bus 0에만 들어옵니다. returned/rejected 프레임은
Panda 송신 결과 metadata이며 순정 ECU 메시지 재개 증거로 사용하지 않습니다.
`stamp`는 ROS publish 시각으로, 하드웨어 CAN 수신 timestamp가 아닙니다.

독립 수신 전용 기록기는 [can_logger.md](can_logger.md)에 있습니다. USB IN만 사용하여
CSV로 기록하고 `/ioniq5/can_logger/rx`에 발행할 수 있으며, 그 `stamp`는 host USB read 시점입니다.
기존 제어 노드를 실행하지 않아도 되며 동일 Panda를 동시에 점유하지 않습니다.

```bash
rostopic echo /ioniq5/can0/rx
rosbag record /ioniq5/can_rx /ioniq5/vehicle_state /ioniq5/actuation_command /diagnostics
```

## TX

`/ioniq5/can_tx`는 같은 메시지와 subscriber queue 256을 사용합니다.
`returned`와 `rejected`는 false여야 합니다. classic CAN은 최대 8 byte, CAN-FD는
표준 DLC 길이 `0..8, 12, 16, 20, 24, 32, 48, 64`만 받습니다. `extended`는 address와
별개인 CAN IDE 비트입니다. ROS 배열은 크기 제한이 없으므로 callback이 크기와 형식을
검사합니다. 형식은 `rosmsg show ioniq5_ecan/RawCanFrame`으로 확인할 수 있습니다.

실제 송신은 다음 조건을 모두 요구합니다.

1. `raw_can.allow_tx=true`
2. 연결된 Panda가 노드의 Hyundai safety mode에 있고 최신 supervisor 결정이 종방향을 허가
3. `0x12A` LFA는 추가로 횡방향 출력 허가
4. Panda `HYUNDAI_CANFD`의 address/bus/content 검사 통과

정상 상태에서는 SET 통합 모드가 이 gate를 엽니다. brake/ACC 오류로 종방향이 래치
해제되면 raw TX도 차단됩니다. `SOFT_DISABLING`에서 정상 종방향 허가가 유지되면
그 gate는 남지만 raw LFA는 차단합니다. PASSIVE, ECU 복구 중, 연결 단절 시에는 송신하지
않습니다. 모든 raw ID의 개별 허용 여부는 Panda hook이 결정합니다.

고수준 경로가 소유하는 LFA/SCC/FCA를 raw로 동시에 송신하면 counter와 주기가 충돌합니다.
해당 ID는 송신 주체를 하나로 유지해야 합니다. Panda가 거부한 프레임은 returned stream의
`rejected=true`로 관찰하며 호스트의 채널 해제/fault 처리에도 반영합니다.

기본 연구장 YAML은 raw TX를 켜며 passive YAML은 끕니다. passive ROS 노드는 Panda 설정과
`NO_OUTPUT` 전환을 수행합니다. USB control read만 수행하는 별도
[`panda_preflight.py`](../scripts/panda_preflight.py)와 동작이 다릅니다.
