# Validation plan

실차 actuation 전 단계를 건너뛰지 않습니다. 현재 저장소에서 자동 검증한 것은 codec,
parser, adapter, supervisor, Panda wire protocol의 호스트 측 동작까지이며 HIL/실차 검증은
아직 수행되지 않았습니다.

## 1. Host-only

- `colcon build` 및 모든 unit test 통과
- opendbc 고정 SHA의 CANPacker와 LFA/SCC/FCA byte-for-byte golden frame 일치
- malformed DLC, USB checksum, CAN CRC fault injection 통과
- command/Panda timeout과 safety mode drift가 FAULT를 만드는지 확인

## 2. Panda bench, 차량 미연결

- `python3 scripts/panda_preflight.py --serial RED_PANDA_SERIAL` 통과
- 고정 `IONIQ5` ALLOW_DEBUG bootstub/firmware SHA-256, split-button patch hash, firmware 문자열, packet hash, Red Panda type과 health ABI 확인
- `NO_OUTPUT`에서 모든 TX가 거부되는지 확인
- CAN loopback fixture에서 100/50 Hz 주기와 counter/CRC 확인
- `/ioniq5/can_rx`와 bus별 raw RX가 address/bus/FD/payload를 손실 없이 보존하는지 확인
- `/ioniq5/can_tx`가 SET 종방향 OFF에서 drop되고 SET ON/active에서 Panda whitelist를 통과하는지 확인
- USB 제거 시 설정한 `panda_timeout_ms` 이내 DISCONNECTED 및 heartbeat false 확인

## 3. 차량 연결, 바퀴 지면 이탈

- `panda_preflight.py --require-harness` 통과
- `config/ioniq5_ecan_passive.yaml`로 ECAN/CAM bus와 주소 fingerprint 기록
- 0x12A가 camera bus에서 보이고 ECAN bus mapping이 0인지 확인
- 0x1A0이 camera bus 2에서 보이는 HDA1 camera-SCC 구성인지 확인
- 0x1CF/0x1AA 중 실제 버튼 메시지를 확인하고 `hardware.alternate_buttons` 설정
- parser 값과 계기판/물리 입력의 부호와 배율 비교
- `yaw_rate_deg_s`, 횡·종가속도와 4륜 속도의 정지 영점, 방향 및 IMU/ESC source 확인
- arm 전후 relay forwarding 및 정주기 비활성 프레임 확인
- LDA가 조향만, SET이 종방향만 ON/OFF하는지 먼저 확인하고 조향 토크 ±1 count부터 방향과 운전자 override 확인

fingerprint가 위 전제와 다르면 actuation하지 말고 codec/safety param을 수정합니다.

## 4. 폐쇄 시험장 저속

- 별도 safety driver와 물리 비상정지
- `max_active_speed_mps`를 초기에는 2.0 이하로 설정
- lateral-only부터 실시
- zero command, step, ramp, timeout, brake, CANCEL, USB disconnect 시험
- Panda `safety_tx_blocked`, `safety_rx_invalid`, bus-off가 모두 0인지 확인

## 5. 종방향

- lateral-only 전체 완료 후 별도 승인
- 고정 DEBUG firmware capability probe 통과
- 구동륜 지면 이탈 또는 다이나모에서 ±0.1 m/s²부터 시작
- brake/gas override, SCC/FCA 경고, 정차/재출발 동작 확인
- 순정 AEB가 차단될 수 있다는 위험을 시험계획에 명시

각 단계에서 software commit, YAML, Panda firmware SHA-256, 차량 VIN/트림,
harness 방향, CAN trace, 시험 결과를 함께 보관하십시오.
