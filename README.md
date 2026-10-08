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
가속도는 CAN 표현 범위 -10.23~10.24 m/s²로 전달하고 범위 밖 값은 끝값으로 맞춥니다.
차량의 가속도 수용 범위와 조향 좌우 부호는 실차 통합 검증이 필요합니다.
`lateral=0`은 누적 목표각 유지이지 핸들 중앙 복귀가 아닙니다. 두 값이 0이어도 유효한 입력입니다.

발행 형식 예시입니다. 실제 차량에서는 아래 0 입력도 제어 명령이라는 점에 주의하십시오.

```bash
rostopic pub -r 20 /ioniq5/actuation_command ioniq5_ecan/ActuationCommand \
  "{lateral: 0.0, acceleration: 0.0}"
```

상세 계약은 [입력 계약](docs/input_contract.md)과 [메시지 정의](msg/ActuationCommand.msg)를 참고합니다.

## ROS 피드백

`/ioniq5/vehicle_state`는 `ioniq5_ecan/VehicleState`를 20 Hz로 발행합니다.
차속과 4륜 속도(m/s), 조향각(deg), 조향각속도와 요레이트(deg/s), 가속도(m/s²),
페달/브레이크/기어, 채널 선택과 출력 허가 상태를 제공합니다.
`stamp`는 발행 시각이며 `valid`는 조향 센서/MDPS/휠 속도/TCS의 갱신 조건만 확인합니다.
IMU, 페달, 기어와 SCC의 개별 유효 시각은 제공하지 않습니다.
[피드백 필드 표](docs/vehicle_state.md)에서 단위와 해석 한계를 확인하십시오.

## 제어 흐름

```text
상위 ROS 제어기 → /ioniq5/actuation_command → ECAN 제어기 → Red Panda → 차량 ECU
  lateral      → 목표 조향각 적분 → 실측각 피드백 → LFA 0x12A 토크, 100 Hz
  acceleration → SCC 0x1A0 / FCA 0x160 가속도 명령, 50 Hz
```

- 실행 중에는 입력 토픽을 계속 구독합니다. 초기 입력 또는 버튼 ON이 없으면 순정 통신/무출력 대기입니다.
- LDA 버튼은 조향 전용, 크루즈 SET을 누르고 놓으면 조향+종방향 ON/OFF 토글입니다. 첫 제어 인계는 정차와 정상 CAN/ECU 조건을 요구합니다.
- 정상 제어 중 입력만 끊기면 출력을 해제하고 순정 통신을 복구합니다. ON 선택을 유지한 같은 세션에서는 복구 완료와 새 입력/정상 CAN 확인 후 주행 중에도 다시 추종합니다. 버튼 OFF는 이 자동 복귀를 취소합니다.
- 브레이크는 종방향만 해제하며 SET 재조작 전까지 재개하지 않습니다.
- CANCEL, CAN/USB 고장, EPS 복귀 창 만료 등 고장 뒤에는 순정 통신을 복구하고 버튼 대기로 돌아갑니다. 운전자가 LDA를 누르거나 SET을 눌렀다 놓으면 주행 중에도 다시 제어를 인계합니다. 서비스로 명시 OFF한 경우와 프로세스 재시작은 예외입니다.

기본 입력 평활화는 생략하지만 조향 rate→torque 변환, 토크 표현 범위, Panda 허가와 CAN 양자화는 유지합니다.
I5R1 확장 펌웨어가 필요하며, 실제 출력은 채널 허가와 ECU 소유권 확인 후에만 수행합니다.
버튼 OFF의 ECAN 비활성화와 입력 단절 때의 순정 ECU 통신 복구는 다른 동작입니다.

## 실행

CAN을 송신하지 않고 RAW 데이터만 저장하려면 제어 실행기와 별도로 다음을 사용합니다.
CSV와 timestamp를 저장하고 선택적으로 `/ioniq5/can_logger/rx`에 발행합니다.

```bash
./scripts/start_can_logger.sh
```

[수신 전용 기록기](docs/can_logger.md)의 시작 조건과 시각 의미를 참고하십시오.

2026-10-06 차량 컴퓨터에 설치한 작업공간 실행 명령입니다. `source`하지 말고 실행합니다.

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

2026-10-07 문서 기준입니다. 10월 6일 퓨즈 교체 후 ignition과 순정 LFA/SCC 수신 복귀를
확인했지만, 이후 정차 시험과 수정 후 재연결에서 ACC 이상 및 DTC 재발을 관측했습니다.
USB 초기 수신과 Panda 첫 RX 경쟁 조건의 수정, 관련 호스트 18개/펌웨어 14개 테스트,
새 앱의 USB-only 설치/서명 및 무출력 송신 차단 검증은 완료했습니다.
**수정 후 실제 추종, 주행 중 재인계와 순정 보조 기능 정상 복구는 미검증**입니다.
순정 AEB 유지도 보장하지 않습니다. [검증 기록](docs/validation.md)에 범위를 구분했습니다.

10월 7일 오프라인 ECAN 분석은 138개 ID와 1,914,473프레임을 조사했습니다.
현재 한국어 표에는 97개 ID, 605개 필드 배치/대안, 3,892개 해석 비트를 표시합니다.
동시 기록 GPS/레이더는 CAN 해석의 참조이며, 새 추론 필드는 현재 ROS 피드백에 추가되지 않았습니다.

- [설치와 차량 컴퓨터 시작 절차](docs/vehicle_computer_handoff.md)
- [현재 ROS 피드백 필드](docs/vehicle_state.md), [상위 입력 계약](docs/input_contract.md)
- [상태와 제어 제한](docs/safety.md), [구조](docs/architecture.md), [raw CAN](docs/raw_can.md)
- [Panda 펌웨어 빌드/flash](docs/panda_firmware.md), [upstream 근거](docs/upstream.md)
- [차량 없이 Panda safety 로컬 편집](docs/panda_safety_local.md): 전체 소스 준비와 추가 변경 패치 추출
- [실차 이력](docs/vehicle_handoff.md), [날짜별 관측](docs/evidence/2026-10-06/README.md)
- [실차 ECAN 필드/비트 분석](docs/ecan_analysis_20261007.md): 한국어 표, 비트 검색표, 동시 기록 센서 검증
- 작업 인수인계: [AGENTS.md](AGENTS.md), [state.json](state.json)
