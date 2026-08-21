# Raw CAN ROS 1 interface

이 패키지는 Red Panda USB를 C++ 노드 하나가 단독 소유합니다. `pandad`, Cabana `--panda`,
다른 Panda Python 프로세스를 동시에 실행하면 USB interface claim이 충돌합니다. Cabana가
필요하면 ROS raw topic을 rosbag으로 기록한 뒤 변환하거나, 별도 실행에서 Cabana를 사용합니다.

## RX

- `/ioniq5/can_rx`: bus 0/1/2 통합 stream
- `/ioniq5/can0/rx`, `/ioniq5/can1/rx`, `/ioniq5/can2/rx`: bus별 stream

`RawCanFrame`은 29-bit address, Panda bus, classic/CAN-FD 구분, returned/rejected 표시와
0~64 byte payload를 보존합니다. RX publisher queue는 256입니다.

```bash
rostopic echo /ioniq5/can0/rx
rosbag record /ioniq5/can_rx /ioniq5/vehicle_state /ioniq5/actuation_command
```

## TX

`/ioniq5/can_tx`도 같은 `RawCanFrame`을 사용합니다. `returned`와 `rejected`는 TX에서 항상
false여야 하며, classic CAN은 최대 8 byte, CAN-FD는 표준 DLC 길이
`0..8, 12, 16, 20, 24, 32, 48, 64`만 받습니다.

메시지 구조는 `rosmsg show ioniq5_ecan/RawCanFrame`으로 확인하십시오. ROS 1 메시지 배열은
길이 제한을 표현하지 못하므로 노드가 수신 시 64 byte 상한을 검사합니다. 실제 TX에는
수동 분석으로 검증한 address, bus, FD 여부, payload를 입력해야 합니다. 고수준 제어 노드가 이미
소유하는 LFA/SCC/FCA ID를 raw TX로 동시에 보내면 counter와 주기가 충돌하므로 한 경로만
사용합니다. `extended`는 address 숫자와 별개인 CAN IDE 비트이며 그대로 Panda에 전달됩니다.

실제 송신 조건은 다음 세 가지입니다.

1. `raw_can.allow_tx: true`
2. 최신 명령과 차량 상태가 유효하고 `SET` 조향+종방향 통합 모드가 active
3. Panda `HYUNDAI_CANFD` safety hook의 address/bus/content 검사 통과

임의 CAN ID를 무제한 송신하도록 `SAFETY_ALLOUTPUT`을 사용하지 않습니다. 허용되지 않은
프레임은 Panda가 차단하고 returned stream에 `rejected: true`로 나타납니다.
