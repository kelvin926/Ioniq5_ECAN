# Ioniq5_ECAN

Ubuntu 20.04 / ROS 1 Noetic에서 Red Panda와 Hyundai K 하네스를 통해 2022년식
아이오닉 5 HDA1의 조향 및 가속도 명령을 전달하는 C++17 연구용 드라이버입니다.

> 이 코드는 실제 차량에서 검증되지 않았습니다. 저장소의 기본 YAML은 요청한 폐쇄
> 연구장 프로파일로 `allow_actuation`과 종방향 제어가 활성화되어 있습니다. 노드는
> 시작할 때 `NO_OUTPUT`이고 첫 최신 명령으로 Panda를 대기 상태로 전환합니다. 이후 물리
> 차선유지(LDA) 버튼은 조향 전용 모드를, 크루즈 `SET` 버튼은 조향+종방향 모드와 raw CAN
> TX를 ON/OFF 토글합니다.
> 종방향 제어 중에는 순정 SCC/AEB 메시지 소유권이 차단됩니다.

## 구현 범위

- 단일 C++ 프로세스: ROS 콜백, 100 Hz 제어, Hyundai CAN-FD codec, libusb Panda 통신
- `LFA (0x12A)` 100 Hz 조향 토크 명령
- 선택적 `SCC_CONTROL (0x1A0)` 및 `ADRV_0x160` 50 Hz 가속도 명령
- CRC16, rolling counter, Panda USB CAN packet protocol
- 차량 속도·4륜 속도/요레이트/횡·종가속도/조향/토크/페달/브레이크/버튼 상태 파싱
- `PASSIVE → ARMED → ACTIVE → FAULT` 안전 상태기계
- ECAN 전용 Panda firmware: logical bus 0만 사용, 비-ECAN transceiver/forwarding 비활성화
- carrotpilot의 Ioniq 5 횡가속도/마찰 기반 토크 제어와 ROS 1 YAML 파라미터 조정
- Panda 프로토콜 버전, Red Panda 기종, 하네스, heartbeat, RX/TX 오류 확인
- CAN-FD 64-byte 보존 raw RX/TX와 bus별 ROS 1 토픽
- Panda hard limit, 명령 watchdog, 선택 가능한 브레이크/CANCEL 반응 및 통신 fault 해제

입력 단위가 아직 확정되지 않았으므로 CAN 계층 앞에 어댑터를 두었습니다.
`input.lateral_mode`는 `steering_rate_deg_s`, `steering_rate_rad_s`,
`curvature_1pm`, `direct_torque`를 지원합니다. 최종 Alpamayo 메시지가 정해지면
[`ActuationCommand.msg`](msg/ActuationCommand.msg)와 command adapter만 교체하면 됩니다.

## 대상 구성

- Hyundai Ioniq 5 2022, HDA1, EV
- Hyundai K camera harness
- Red Panda USB
- Ubuntu 20.04 + ROS 1 Noetic + C++17
- ECAN Panda bus 0, `harness_status=1`
- camera bus 2는 이 차량에서 사용하지 않으며 firmware가 transceiver와 forwarding을 차단

HDA2/LKA steering, 다른 하네스, alt buttons 또는 radar-SCC 구성은 지원하지 않습니다.

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

기본 `ioniq5_ecan.yaml`은 연구장 actuation 프로파일입니다. passive fingerprint와 parser
검증을 끝낸 뒤에만 사용합니다.

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

기본 자동 arm 모드에서 출력 조건은 다음과 같습니다.

1. 저장소의 split-button/ECAN-only 패치가 적용된 고정 `IONIQ5ECAN` DEBUG Panda firmware
2. Red Panda/Hyundai K 연결과 정확한 `harness_status=1`
3. 유효하고 최신인 필수 ECAN 차량 상태와 command
4. 물리적인 차선유지(LDA) 버튼을 누르면 조향 전용 ON, 같은 모드에서 다시 누르면 OFF
5. 물리적인 `SET` 버튼을 눌렀다 놓으면 조향+종방향·raw TX ON, 같은 모드에서 다시 누르면 OFF

LDA와 SET은 각각 조향 전용/통합 모드를 선택합니다. `/ioniq5/vehicle_state`의 `lateral_armed`,
`longitudinal_armed`, `lateral_control_active`, `longitudinal_control_active`로 현재 상태를
확인할 수 있습니다.

```bash
# auto_arm_on_command=false일 때 수동 arm
rosservice call /ioniq5_ecan/set_armed "data: true"
# 즉시 해제
rosservice call /ioniq5_ecan/set_armed "data: false"
```

수동 `false` 요청은 자동 arm을 함께 억제하며, 다시 `true`를 요청할 때까지 유지됩니다.
`input.use_enable_field: true`로 바꾸면 `enable`이 다시 매 메시지 deadman으로 동작합니다.
속도/조향각 호스트 상한은 `0`이면 비활성이고, 토크·토크 변화율·가속도 범위는 Panda
firmware 경계를 넘길 수 없습니다.

raw CAN 토픽은 `/ioniq5/can_rx`와 `/ioniq5/can0/rx`~`can2/rx`가 생성되지만 ECAN-only
firmware에서는 실제 차량 프레임이 `/ioniq5/can0/rx`에만 들어옵니다. raw CAN은
`/ioniq5/can_tx`로 송신합니다. TX는 `raw_can.allow_tx: true`, SET 통합 모드 arm/active 상태, Panda
Hyundai safety whitelist를 모두 통과해야 실제 bus로 나갑니다. 상세 형식과 Panda/Cabana
동시 사용 제약은 [`docs/raw_can.md`](docs/raw_can.md)를 참고하십시오.

안전 상태 전이와 시험 순서는 [`docs/safety.md`](docs/safety.md)와
[`docs/validation.md`](docs/validation.md)를 따르십시오. 종방향 제어는
[`docs/panda_firmware.md`](docs/panda_firmware.md)의 고정 DEBUG 펌웨어가 필요합니다.

## 저장소 구조

```text
include/ioniq5_ecan/   C++ core, Panda driver, ROS node
src/                   implementation
msg/                   placeholder command and status interfaces
config/                ROS YAML and udev rule
launch/                ROS 1 roslaunch file
test/                  golden-frame, parser, safety, protocol tests
scripts/               environment and pinned firmware helpers
patches/               고정 opendbc/Panda split-button·ECAN-only firmware 패치
docs/                  safety, latency, validation, upstream evidence
```

## 참고 upstream

- [commaai/opendbc](https://github.com/commaai/opendbc)
- [commaai/panda](https://github.com/commaai/panda)
- [commaai/openpilot](https://github.com/commaai/openpilot)
- [ajouatom/openpilot carrot-wip](https://github.com/ajouatom/openpilot/tree/carrot-wip)

정확한 commit SHA와 대조 지점은 [`docs/upstream.md`](docs/upstream.md)에 기록했습니다.

## 라이선스

MIT. 차량 사용 위험과 검증 책임은 사용자에게 있습니다.
