# Ioniq5_ECAN

Ubuntu 20.04 / ROS 2 Foxy에서 Red Panda와 Hyundai K 하네스를 통해 2022년식
아이오닉 5 HDA1의 조향 및 가속도 명령을 전달하는 C++17 연구용 드라이버입니다.

> 이 코드는 실제 차량에서 검증되지 않았습니다. 기본 설정은 CAN 송신이 불가능한
> `NO_OUTPUT`이며, `allow_actuation: false`입니다. 리프트/다이나모와 폐쇄 시험장 검증
> 전에는 공공도로에서 사용하지 마십시오. 종방향 제어 중에는 순정 SCC/AEB 메시지
> 소유권이 차단되므로 특히 위험합니다.

## 구현 범위

- 단일 C++ 프로세스: ROS 콜백, 100 Hz 제어, Hyundai CAN-FD codec, libusb Panda 통신
- `LFA (0x12A)` 100 Hz 조향 토크 명령
- 선택적 `SCC_CONTROL (0x1A0)` 및 `ADRV_0x160` 50 Hz 가속도 명령
- CRC16, rolling counter, Panda USB CAN packet protocol
- 차량 속도/조향/운전자 토크/페달/브레이크/크루즈 버튼 상태 파싱
- `PASSIVE → ARMED → ACTIVE → FAULT` 안전 상태기계
- carrotpilot 방식의 ROS 2 YAML 파라미터 조정
- Panda 프로토콜 버전, Red Panda 기종, 하네스, heartbeat, RX/TX 오류 확인
- Panda hard limit보다 강한 소프트웨어 제한과 명령 watchdog

입력 단위가 아직 확정되지 않았으므로 CAN 계층 앞에 어댑터를 두었습니다.
`input.lateral_mode`는 `steering_rate_deg_s`, `steering_rate_rad_s`,
`curvature_1pm`, `direct_torque`를 지원합니다. 최종 Alpamayo 메시지가 정해지면
[`ActuationCommand.msg`](msg/ActuationCommand.msg)와 command adapter만 교체하면 됩니다.

## 대상 구성

- Hyundai Ioniq 5 2022, HDA1, EV
- Hyundai K camera harness
- Red Panda USB
- Ubuntu 20.04 + ROS 2 Foxy + C++17
- ECAN Panda bus 0, camera bus 2

HDA2/LKA steering, 다른 하네스, alt buttons 또는 radar-SCC 구성은 지원하지 않습니다.

## 설치 및 빌드

```bash
sudo apt update
sudo apt install -y build-essential libusb-1.0-0-dev pkg-config \
  python3-colcon-common-extensions ros-foxy-diagnostic-msgs ros-foxy-std-srvs

sudo install -m 0644 config/99-red-panda.rules /etc/udev/rules.d/99-red-panda.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo usermod -aG plugdev "$USER"
# group 변경 적용을 위해 다시 로그인

source /opt/ros/foxy/setup.bash
./scripts/check_environment.sh
cd ~/ros2_ws
ln -s /path/to/Ioniq5_ECAN src/ioniq5_ecan
colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
colcon test --packages-select ioniq5_ecan
colcon test-result --verbose
```

Foxy는 공식 EOL이므로 연구 컴퓨터는 격리하고 OS 보안 업데이트 정책을 별도로
운영해야 합니다. 코드는 Humble에서도 빌드 가능한 API만 사용했지만 기준 타깃은
요청대로 20.04/Foxy입니다.

## 실행

먼저 기본 YAML 그대로 수동 CAN 관찰만 확인합니다.

```bash
ros2 launch ioniq5_ecan ioniq5_ecan.launch.py \
  config:=/path/to/Ioniq5_ECAN/config/ioniq5_ecan.yaml
ros2 topic echo /ioniq5/vehicle_state
ros2 topic echo /diagnostics
```

명령 예시는 다음과 같습니다. `stamp`는 지연 측정용이고 hard watchdog은 안전하게
수신 시각을 사용합니다.

```bash
ros2 topic pub -r 20 /ioniq5/actuation_command ioniq5_ecan/msg/ActuationCommand \
  "{stamp: {sec: 0, nanosec: 0}, sequence: 1, enable: true, lateral: 0.0, acceleration: 0.0}"
```

실제 actuation을 허용하려면 먼저 검증 복사본 YAML에서
`safety.allow_actuation: true`로 변경하고 노드를 재시작합니다. 그 뒤에도 다음 조건이
모두 필요합니다.

1. `/ioniq5_ecan/set_armed` 서비스로 arm 요청
2. 유효하고 최신인 필수 차량 CAN 상태
3. 최신 command와 `enable: true`
4. 물리적인 SET 또는 RES 버튼을 눌렀다 놓기
5. Panda `controls_allowed: true`
6. 브레이크/CANCEL/timeout/fault 없음

```bash
ros2 service call /ioniq5_ecan/set_armed std_srvs/srv/SetBool "{data: true}"
# 즉시 해제
ros2 service call /ioniq5_ecan/set_armed std_srvs/srv/SetBool "{data: false}"
```

안전 상태 전이와 시험 순서는 [`docs/safety.md`](docs/safety.md)와
[`docs/validation.md`](docs/validation.md)를 따르십시오. 종방향 제어는
[`docs/panda_firmware.md`](docs/panda_firmware.md)의 고정 DEBUG 펌웨어가 필요합니다.

## 저장소 구조

```text
include/ioniq5_ecan/   C++ core, Panda driver, ROS node
src/                   implementation
msg/                   placeholder command and status interfaces
config/                ROS YAML and udev rule
launch/                ROS 2 launch file
test/                  golden-frame, parser, safety, protocol tests
scripts/               environment and pinned firmware helpers
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
