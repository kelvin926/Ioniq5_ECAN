# Ioniq5_ECAN

Ubuntu 20.04 / ROS 1 Noetic에서 Red Panda와 Hyundai K 하네스를 통해 2022년식
아이오닉 5 HDA1의 조향 및 가속도 명령을 전달하는 C++17 연구용 드라이버입니다.

> 2026-10-06 현재 코드와 설정 기준입니다. 2026-08-21 helper 시험에서 저속 조향과 직선
> 15 km/h 가속 1회를 확인했지만, 간헐적 가속 실패는 아직 해결이 검증되지 않았습니다.
> 최신 ROS 노드의 이중 ECU 수명주기, 일시 EPS 복귀와 통신 복구는 실차 재시험 전입니다.
> 현재 상태와 과거 시험 기록은 [`docs/vehicle_handoff.md`](docs/vehicle_handoff.md)에 있습니다.
>
> 저장소의 기본 YAML은 요청한 폐쇄
> 연구장 프로파일로 `allow_actuation`과 종방향 제어가 활성화되어 있습니다. 노드는
> 시작할 때 `NO_OUTPUT`이고 첫 최신 명령으로 Panda를 대기 상태로 전환합니다. 이후 물리
> 차선유지(LDA) 버튼은 조향 전용 모드를, 크루즈 `SET` 버튼은 조향+종방향 모드와 raw CAN
> TX를 ON/OFF 토글합니다.
> ROS C++ 노드도 종방향 제어 전에 순정 camera/radar 메시지 소유권을 차단하고 종료 시
> 복구합니다. 이 수명주기의 실차 확인은 아직 남아 있습니다.

## 구현 범위

- 단일 C++ 프로세스: ROS 콜백, 100 Hz 제어, Hyundai CAN-FD codec, libusb Panda 통신
- `LFA (0x12A)` 100 Hz 조향 토크 명령
- 선택적 `SCC_CONTROL (0x1A0)` 및 `ADRV_0x160` 50 Hz 가속도 명령
- 현재 코드의 camera 소유권 대상 `0x730`/radar `0x7D0` UDS 비활성화, quiet 확인, tester-present, 역순 복구
- 마지막 순정 `0x1A0` payload를 보존하고 제어 신호만 덮어쓰는 HDA1 SCC 생성
- CRC16, rolling counter, Panda USB CAN packet protocol
- 차량 속도/4륜 속도/요레이트/횡가속도/종가속도/조향/토크/페달/브레이크/버튼 상태 파싱
- `PASSIVE → ARMED → ACTIVE`, 일시 EPS 오류의 `SOFT_DISABLING` 복귀, 지속/통신 장애의 `FAULT` 상태기계
- ECAN 전용 Panda firmware: logical bus 0만 사용, 비-ECAN transceiver/forwarding 비활성화
- carrotpilot의 Ioniq 5 횡가속도/마찰 기반 토크 제어와 ROS 1 YAML 파라미터 조정
- Panda 프로토콜 버전, Red Panda 기종, 하네스, heartbeat, RX/TX 오류 확인
- CAN-FD 64-byte 보존 raw RX/TX와 bus별 ROS 1 토픽
- Panda hard limit, 명령 watchdog, 브레이크 종방향 래치 해제 및 채널별 TX fault 처리

입력 단위가 아직 확정되지 않았으므로 CAN 계층 앞에 어댑터를 두었습니다.
`input.lateral_mode`는 `steering_rate_deg_s`, `steering_rate_rad_s`,
`curvature_1pm`, `direct_torque`를 지원합니다. 직접 조향각 입력 모드는 없으며 현재 차량
출력은 `0x12A` 토크입니다. native MDPS angle 제어 지원은 미확인입니다. 최종 상위 계약은
메시지와 adapter 경계에서 반영합니다. 단위와 남는 변환은
[`docs/input_contract.md`](docs/input_contract.md)를 참고하십시오.

## 대상 구성

- Hyundai Ioniq 5 2022, HDA1, EV, radar-SCC
- Hyundai K camera harness
- Red Panda USB
- Ubuntu 20.04 + ROS 1 Noetic + C++17
- ECAN Panda bus 0, `harness_status=1`
- camera bus 2는 이 차량에서 사용하지 않으며 firmware가 transceiver와 forwarding을 차단

HDA2/LKA steering, camera-SCC와 다른 하네스는 이 차량 프로파일의 대상이 아닙니다.
버튼 parser는 `0x1CF`와 alternate `0x1AA`를 지원하며 passive fingerprint에 맞춰
`hardware.alternate_buttons`를 선택합니다. 기본값은 false입니다.

## 설치 및 빌드

```bash
sudo apt update
sudo apt install -y build-essential libusb-1.0-0-dev pkg-config \
  python3-nose python3-pip ros-noetic-ros-base ros-noetic-diagnostic-msgs \
  ros-noetic-message-generation ros-noetic-roscpp ros-noetic-std-srvs
python3 -m pip install --user libusb1

sudo install -m 0644 config/99-red-panda.rules /etc/udev/rules.d/99-red-panda.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo usermod -aG plugdev "$USER"
# group 변경 적용을 위해 다시 로그인

source /opt/ros/noetic/setup.bash
./scripts/check_environment.sh
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
ln -s /path/to/Ioniq5_ECAN ioniq5_ecan
cd ~/catkin_ws
catkin_make -DCMAKE_BUILD_TYPE=Release
source devel/setup.bash
catkin_make run_tests_ioniq5_ecan
catkin_test_results --verbose
```

기준 타깃은 연구차량과 동일한 Ubuntu 20.04/ROS 1 Noetic입니다. ROS 2 빌드와 실행은
더 이상 지원하지 않습니다.

## 실행

차량에 처음 연결할 때는 actuation이 구조적으로 비활성화된 passive profile을 사용합니다.
먼저 Panda만 USB에 연결한 상태에서 쓰기 요청을 전혀 보내지 않는 preflight를 통과시킵니다.

```bash
python3 scripts/panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only
```

하네스 연결 후에는 다음 명령이 `harness_status`, ignition, CAN RX 증가까지 함께 검사합니다.

```bash
python3 scripts/panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only --require-harness
roslaunch ioniq5_ecan ioniq5_ecan.launch \
  config:=/path/to/Ioniq5_ECAN/config/ioniq5_ecan_passive.yaml
```

`panda_preflight.py`는 USB vendor control read만 사용하며 CAN 통신 리셋, bitrate/safety 변경,
heartbeat 또는 CAN TX를 수행하지 않습니다. application PID, 정확한 고정 firmware 문자열과
packet hash도 함께 확인합니다. Cabana `--panda`와 ROS 노드를 포함해 Panda를 점유하는 다른
프로그램은 preflight 전에 종료합니다.

launch의 기본 `ioniq5_ecan.yaml`은 연구장 actuation 프로파일입니다. passive fingerprint와
parser 검증을 끝낸 뒤에 사용합니다. passive ROS 노드도 Panda 설정 및 `NO_OUTPUT` 전환을
수행하므로 USB control read만 사용하는 preflight와는 구분해야 합니다.

| 설정 | 기본 연구장 YAML | passive YAML |
| --- | --- | --- |
| actuation / longitudinal / auto arm / raw TX | true | false |
| LDA / SET 버튼 토글 | true | true, 출력 비활성 |
| brake의 횡방향 해제 / 종방향 해제 | false / true | false / true |
| host CANCEL 해제 / gas 종방향 override | false / false | true / true |
| command / Panda / critical CAN watchdog | 250 / 500 / 250 ms | 250 / 500 / 250 ms |

기본 연구장 프로파일은 host CANCEL 및 gas override로 해제하지 않습니다. 이를 사용하려면
해당 YAML 항목을 켭니다. Panda firmware의 별도 검사는 유지됩니다.

```bash
roslaunch ioniq5_ecan ioniq5_ecan.launch \
  config:=/path/to/Ioniq5_ECAN/config/ioniq5_ecan.yaml
rostopic echo /ioniq5/vehicle_state
rostopic echo /ioniq5/can0/rx
rostopic echo /diagnostics
```

기본 `input.use_enable_field: false`에서는 다음처럼 두 값만 보내면 됩니다. `sequence=0`과
`enable=false`의 기본값은 무시되고 watchdog은 수신 시각을 사용합니다.

```bash
rostopic pub -r 20 /ioniq5/actuation_command ioniq5_ecan/ActuationCommand \
  "{lateral: 0.0, acceleration: 0.0}"
```

기본 `input.unfiltered_input: true`에서는 표현 가능한 `acceleration`을 호스트에서 평활화하거나
저크 제한하지 않고 다음 50 Hz SCC 프레임에 반영하며 CAN에서 0.01 m/s²로 양자화합니다.
steering-rate도 호스트 상한으로
잘라내지 않고 적분합니다. 다만 Ioniq 5의 조향 CAN 입력은 rate가 아니라 torque이므로
steering-rate→목표각→토크 변환과 Panda의 270 count 상한, 100 Hz에서 2/3 count 변화율은
남습니다. 이는 명령 상한이며 실제 MDPS 최대 구동력이 확인된 값은 아닙니다.
가속도 `-3.5 .. 2.0 m/s²`처럼 CAN/Panda가 표현할 수 없는 값은 조용히 잘라내지 않고 arm을
해제하며 진단 오류를 냅니다.

기본 자동 arm 모드에서 출력 조건은 다음과 같습니다.

1. 저장소의 split-button/ECAN-only 패치가 적용된 고정 `IONIQ5ECAN` DEBUG Panda firmware
2. Red Panda/Hyundai K 연결과 정확한 `harness_status=1`
3. 유효하고 최신인 필수 ECAN 차량 상태와 command
4. 초기 takeover는 정차와 EPS 정상 상태 요구, 종방향 프로파일은 D와 최근 순정 `0x1A0`도 요구
   (camera quiet 확인, 종방향이면 radar quiet도 확인)
5. 물리적인 차선유지(LDA) 버튼을 누르면 조향 전용 ON, 같은 모드에서 다시 누르면 OFF
6. 물리적인 `SET` 버튼을 눌렀다 놓으면 조향+종방향 및 raw TX ON, 같은 모드에서 다시 누르면 OFF

LDA와 SET은 각각 조향 전용/통합 모드를 선택합니다. `/ioniq5/vehicle_state`의 `lateral_armed`,
`longitudinal_armed`, `lateral_control_active`, `longitudinal_control_active`로 현재 상태를
확인할 수 있습니다.

브레이크는 조향 상태를 바꾸지 않습니다. 통합 모드에서 브레이크를 밟으면 종방향만 즉시
비활성화되어 0 가속/`ACCMode=0` 프레임으로 전환되고, 브레이크를 놓아도 자동 재개하지
않습니다. 다시 종방향을 사용하려면 `SET`을 한 번 눌렀다 놓습니다. LDA 조향은 그동안
계속 활성 상태를 유지하며 LDA 버튼으로만 ON/OFF합니다.

```bash
# auto_arm_on_command=false일 때 수동 arm
rosservice call /ioniq5_ecan/set_armed "data: true"
# 즉시 해제
rosservice call /ioniq5_ecan/set_armed "data: false"
```

수동 `false` 요청은 자동 arm을 함께 억제하며, 다시 `true`를 요청할 때까지 유지됩니다.
활성 제어 중 일시적인 MDPS LKA 보조 오류가 발생하면 `SOFT_DISABLING`으로 들어갑니다.
조향 토크와 request는 즉시 0/비활성으로 내리고, 기존 arm과 ECU 통신 소유권은 유지합니다.
최신 정상 CAN 데이터, 유효한 최신 명령, Panda 허가가 있고 오류가 3초 안에 해소되면
현재 명령으로 `ACTIVE`에 자동 복귀합니다. 정상적인 종방향은 계속 동작하지만 brake/ACC
오류로 해제된 종방향은 복귀시키지 않습니다. 대기 중 조향 제어기의 적분과 목표각을
초기화하여 장애 전 조향 목표를 누적하지 않으며, raw LFA 송신으로 정지를 우회할 수 없습니다.
3초 만료, CAN/Panda 장애 또는 명령 단절은 기존의 전체 해제와 순정 ECU 복구로 넘어갑니다.
`/diagnostics`의 `soft_disable_remaining_ms`로 남은 복귀 시간을 확인합니다.
ECU 응답이나 Panda 연결이 끊기면 노드는 계속 실행되며, USB 재연결 후 실패한 순정 ECU
통신 복구와 `NO_OUTPUT` 확인을 1, 2, 4, 8, 16, 최대 30초 간격으로 재시도합니다.
복구 중에는 제어 재arm을 받지 않고, 장애 전 명령이나 채널 선택을 자동 재개하지 않습니다.
복구 완료 후 최신 정상 차량 상태와 명령, 정차 조건을 확인하고 `set_armed=false` → `true`,
물리 LDA/SET 절차로 재승인합니다. `/diagnostics`의 `ecu_recovery_pending`,
`ecu_recovery_attempts`, `recovery_rearm_required`에서 복구 진행 상태를 확인합니다.
`input.use_enable_field: true`로 바꾸면 `enable`이 다시 매 메시지 deadman으로 동작합니다.
속도/조향각 호스트 상한은 `0`이면 비활성이고, 토크/토크 변화율/가속도 범위는 Panda
firmware 경계를 넘길 수 없습니다.

raw CAN 토픽은 `/ioniq5/can_rx`와 `/ioniq5/can0/rx`~`can2/rx`가 생성되지만 ECAN-only
firmware에서는 실제 차량 프레임이 `/ioniq5/can0/rx`에만 들어옵니다. raw CAN은
`/ioniq5/can_tx`로 송신합니다. TX는 `raw_can.allow_tx: true`, 연결된 Hyundai mode와 종방향
출력 허가, Panda whitelist를 요구합니다. 일시 EPS 대기 중 정상 종방향의 raw gate는
유지될 수 있지만 `0x12A` LFA는 횡방향 허가를 추가로 요구합니다. 상세 형식과 Panda/Cabana
동시 사용 제약은 [`docs/raw_can.md`](docs/raw_can.md)를 참고하십시오.

안전 상태 전이와 시험 순서는 [`docs/safety.md`](docs/safety.md)와
[`docs/validation.md`](docs/validation.md)를 따르십시오. 종방향 제어는
[`docs/panda_firmware.md`](docs/panda_firmware.md)의 고정 DEBUG 펌웨어가 필요합니다.

## 저장소 구조

```text
include/ioniq5_ecan/   C++ core, Panda driver, ROS node
src/                   implementation
msg/                   provisional command, vehicle state and raw CAN interfaces
config/                ROS YAML and udev rule
launch/                ROS 1 roslaunch file
test/                  golden-frame, parser, safety, protocol tests
scripts/               environment and pinned firmware helpers
patches/               고정 opendbc/Panda split-button, ECAN-only firmware 패치
docs/                  safety, latency, validation, upstream evidence
AGENTS.md              durable project and agent guidance, 200 lines or fewer
state.json             current decisions, observations, verification and open issues
```

## 문서와 확인 상태

| 문서 | 내용 |
| --- | --- |
| [입력 계약](docs/input_contract.md) | 단위, 변환, watchdog, 상태 필드 |
| [구조](docs/architecture.md) | 제어/RX 흐름과 ECU 소유권 |
| [상태와 제한](docs/safety.md) | 채널 해제, 일시 복귀, hard fault 복구 |
| [raw CAN](docs/raw_can.md) | RX/TX 형식과 채널 gate |
| [주기와 지연](docs/latency.md) | 목표 주기, 스레드, 측정 한계 |
| [Panda firmware](docs/panda_firmware.md) | 고정 ABI, 패치, 빌드/플래시 구분 |
| [upstream 근거](docs/upstream.md) | runtime pins와 별도 Carrot 비교 snapshot |
| [검증](docs/validation.md) | 완료한 검증과 남은 ROS/bench/실차 확인 |
| [인수인계](docs/vehicle_handoff.md) | 날짜별 하드웨어와 실차 기록, 다음 작업 |

2026-10-06 native C++17 core smoke에서 3초 복귀 경계, 채널 유지, hard fault 우선순위와
CRC/freshness를 확인했습니다. 전체 ROS Noetic 빌드, raw TX callback bench와 최신 경로의
실차 fault injection은 미확인입니다. 당시 USB read snapshot에서 Panda는 application mode,
하네스/ignition은 0이었으며 설치된 정확한 binary hash는 확인하지 않았습니다.
marker만으로 현재 split-brake revision의 설치를 단정할 수 없습니다.

작업을 이어갈 때 [AGENTS.md](AGENTS.md)와 [state.json](state.json)을 먼저 읽습니다.
중요한 변경에는 해당 문서와 state를 함께 갱신하고 과거 결과의 날짜를 보존합니다.

## 참고 upstream

- [commaai/opendbc](https://github.com/commaai/opendbc)
- [commaai/panda](https://github.com/commaai/panda)
- [commaai/openpilot](https://github.com/commaai/openpilot)
- [ajouatom/openpilot carrot-wip](https://github.com/ajouatom/openpilot/tree/carrot-wip)

정확한 commit SHA와 대조 지점은 [`docs/upstream.md`](docs/upstream.md)에 기록했습니다.

## 라이선스

MIT. 차량 사용 위험과 검증 책임은 사용자에게 있습니다.
