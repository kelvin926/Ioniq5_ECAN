# Ioniq5_ECAN

2022 Ioniq 5 HDA1 연구차용 ROS 1 Noetic ECAN 제어기입니다.
상위 제어기의 핸들 조향각속도와 종방향 가속도를 받아 Red Panda로 차량 CAN 명령을 보냅니다.
대상 환경은 Ubuntu 20.04, Hyundai K 하네스, Red Panda, ECAN logical bus 0입니다.

## ROS 입력

토픽: `/ioniq5/actuation_command`

메시지: `ioniq5_ecan/ActuationCommand`

| 필수 필드 | 의미 | 단위 |
| --- | --- | --- |
| `lateral` | 목표 핸들 조향각속도. 목표 조향각이나 차량 요레이트가 아님 | deg/s |
| `acceleration` | 목표 종방향 가속도. 양수 가속, 음수 감속 | m/s² |

전체 메시지 구조는 다음과 같습니다.

```text
time stamp             # 선택: source 시각, 지연 분석용
uint32 sequence        # 선택: 순서 번호, 0이면 검사 생략
bool enable            # 기본 use_enable_field=false에서는 무시
float64 lateral
float64 acceleration
```

두 필수 값을 같은 메시지에 넣어 non-latched 20 Hz로 발행하는 것을 권장합니다.
수신 시각 기준 250 ms 동안 새 명령이 없으면 입력 단절로 처리합니다.
가속도 허용 범위는 -3.5~2.0 m/s²이며 조향 좌우 부호는 실차 통합 검증이 필요합니다.
`lateral=0`은 누적 목표각 유지이지 핸들 중앙 복귀가 아닙니다. 두 값이 0이어도 유효한 입력입니다.

발행 형식 예시입니다. 실제 차량에서는 아래 0 입력도 제어 명령이라는 점에 주의하십시오.

```bash
rostopic pub -r 20 /ioniq5/actuation_command ioniq5_ecan/ActuationCommand \
  "{lateral: 0.0, acceleration: 0.0}"
```

상세 계약은 [입력 계약](docs/input_contract.md)과 [메시지 정의](msg/ActuationCommand.msg)를 참고합니다.

## 제어 흐름

```text
상위 ROS 제어기 → /ioniq5/actuation_command → ECAN 제어기 → Red Panda → 차량 ECU
  lateral      → 목표 조향각 적분 → 실측각 피드백 → LFA 0x12A 토크, 100 Hz
  acceleration → SCC 0x1A0 / FCA 0x160 가속도 명령, 50 Hz
```

- 실행 중에는 입력 토픽을 계속 구독합니다. 초기 입력 또는 버튼 ON이 없으면 순정 통신/무출력 대기입니다.
- LDA 버튼은 조향 전용, 크루즈 SET을 누르고 놓으면 조향+종방향 ON/OFF 토글입니다. 첫 제어 인계는 정차와 정상 CAN/ECU 조건을 요구합니다.
- 정상 제어 중 입력만 끊기면 출력을 해제하고 순정 통신을 복구합니다. ON 선택을 유지한 같은 세션에서는 복구 완료와 새 입력/정상 CAN 확인 후 주행 중에도 다시 추종합니다. 버튼 OFF는 이 자동 복귀를 취소합니다.
- 브레이크는 종방향만 해제하며 SET 재조작 전까지 재개하지 않습니다. CANCEL, CAN/USB 고장, 시동/하네스 이상 또는 재부팅은 무조건 자동 복귀하지 않습니다.

기본 입력 평활화는 생략하지만 조향 rate→torque 변환, 토크 제한, Panda 허가와 CAN 양자화는 유지합니다.
I5R1 확장 펌웨어가 필요하며, 실제 출력은 채널 허가와 ECU 소유권 확인 후에만 수행합니다.
버튼 OFF의 ECAN 비활성화와 입력 단절 때의 순정 ECU 통신 복구는 다른 동작입니다.

## 실행

CAN을 송신하지 않고 RAW 데이터만 저장하려면 제어 실행기와 별도로 다음을 사용합니다.
CSV와 timestamp를 저장하고 선택적으로 `/ioniq5/can_logger/rx`에 발행합니다.

```bash
./scripts/start_can_logger.sh
```

[수신 전용 기록기](docs/can_logger.md)의 시작 조건과 시각 의미를 참고하십시오.

이 차량 컴퓨터에 설치된 한 줄 명령입니다. `source`하지 말고 실행합니다.

```bash
/home/ave/catkin_ws_ioniq5/ecan
```

저장소에서 직접 실행할 수도 있습니다.

```bash
cd /home/ave/catkin_ws_ioniq5/src/ioniq5_ecan
./scripts/start_ecan.sh
# 경로만 확인: ROS/Panda/CAN 접근 없음
./scripts/start_ecan.sh --check
```

Ctrl+C로 종료합니다. 실행기는 자기 프로세스에만 Noetic/이 작업공간 환경을 적용하고 로그는 작업공간 안에 둡니다.
전역 alias나 `.bashrc`는 수정하지 않습니다. 같은 ROS master/토픽은 공유하며 Panda USB는 다른 프로그램과 동시에 사용하지 않습니다.
기본 설정은 **제어 활성 프로파일**입니다. 첫 차량 연결/수신 확인은 [passive 시작 절차](docs/vehicle_computer_handoff.md#첫-연결-read-only-확인과-passive-ros)를 따릅니다.

## 확인 상태와 상세 문서

2026-10-06 정차 시험 이후 USB 초기 수신 동기화와 순정 복구 순서를 수정했고, Panda 첫 수신/안전검사 경쟁 조건도 수정해 USB-only로 새 앱을 설치했습니다.
관련 호스트 18개와 펌웨어 14개 테스트, 빌드, 설치 서명 검증, preflight PASS 및 무출력 송신 차단을 확인했습니다.
**수정 후 실제 추종과 주행 중 재인계는 미검증**입니다. 재연결 수신/대기는 정상이지만 ACC 이상과 ECU 고장이 재발해 제어 시험을 중단했습니다.
순정 AEB 유지도 보장하지 않습니다. [검증 기록](docs/validation.md)에 완료와 미확인 범위를 구분했습니다.

- [설치와 차량 컴퓨터 시작 절차](docs/vehicle_computer_handoff.md)
- [상태와 제어 제한](docs/safety.md), [구조](docs/architecture.md), [raw CAN](docs/raw_can.md)
- [Panda 펌웨어 빌드/flash](docs/panda_firmware.md), [upstream 근거](docs/upstream.md)
- [실차 이력](docs/vehicle_handoff.md), [날짜별 관측](docs/evidence/2026-10-06/README.md)
- 작업 인수인계: [AGENTS.md](AGENTS.md), [state.json](state.json)
