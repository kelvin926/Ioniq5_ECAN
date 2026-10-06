# 차량 컴퓨터 전달 문서

2026-10-06 작성. 대상은 **Ubuntu 20.04 / ROS 1 Noetic / C++17**입니다.
이 문서는 차량 컴퓨터에서 현재 작업을 이어가기 위한 시작 절차입니다.

## 가져올 브랜치와 읽을 파일

- 저장소: `https://github.com/kelvin926/Ioniq5_ECAN`
- 전달 브랜치: **`feat/initial-implementation`**. 이번 전달에서 `main`은 변경하지 않습니다.
- 제어 코드 기준 commit: `7d9797bb6a0c3d93fa2794b902fa882ff50dc6d6`.
  전달 문서와 evidence는 이 commit 다음의 문서 commit에 포함됩니다. 위 코드 commit만
  checkout하지 말고 전달 브랜치의 최신 내용을 가져옵니다.
- 먼저 [AGENTS.md](../AGENTS.md), [state.json](../state.json), 이 문서를 읽습니다.
- 상세 실차 이력: [vehicle_handoff.md](vehicle_handoff.md).
  입력 단위: [input_contract.md](input_contract.md). 검증 범위: [validation.md](validation.md).

이번 전달에는 기존 로컬의 ROS 제어/ECU 복구 변경, 관련 test와 firmware patch source,
문서, 프로젝트 상태 및 날짜별 JSON 관측 기록이 포함됩니다. Windows 빌드 산출물,
firmware binary와 임시 진단/고장코드 삭제 스크립트는 전달하지 않습니다.

## 현재 차량과 최신 확인 결과

2022 Ioniq 5 HDA1 EV, radar-SCC, Hyundai K camera harness, Red Panda 구성입니다.
Panda serial은 `PANDA_SERIAL`입니다. 물리 CAN1 = firmware controller index 0
= Panda logical bus 0(ECAN), nominal/data bitrate는 500/2000 kbps입니다.
현재 firmware는 비-ECAN transceiver 및 forwarding을 비활성화합니다.

사용자가 퓨즈 문제를 발견하여 교체 후 해결됐다고 알려줬습니다. 퓨즈 위치/규격은
제공되지 않았습니다. 그 뒤 Windows 노트북에서 송신 없이 확인한 3초 관측은 다음과
같습니다. 차량 컴퓨터에 연결한 뒤에는 새로 확인합니다.

| 항목 | 2026-10-06 퓨즈 교체 후 관측 |
| --- | --- |
| USB application / marker | Red Panda 앱 / `IONIQ5ECAN-dd8a5b3d-DEBUG` |
| packet hash: health / CAN | `0x290DAE03` / `0x75ABF276` |
| 하네스 방향 | `NORMAL`, `harness_status=1`, 정방향 |
| ignition line / CAN | `1 / 0` |
| 전압 / Panda uptime | 14.102 V / 242~245초 |
| ECAN 새 수신 | 3초에 8,636개 |
| 순정 LFA `0x12A` | CRC 정상 300개, 100 Hz |
| 순정 SCC `0x1A0` | CRC 정상 150개, 50 Hz |
| MDPS / steering sensor / wheel speed / TCS | CRC 정상 300 / 299 / 300 / 150개, counter 진행 |
| 선택 프레임 CRC 오류 / 관측 중 RX overflow 증가 | 0 / 0 |
| 최신 조향각 / 4륜 속도 | 0.3 deg / 모두 0 km/h |
| MDPS warning / LKA fault / LKA fail / ACCMode | 모두 0 |
| safety mode / controls allowed / 모든 controller TX 증가 | `0 / 0 / [0,0,0]` |

원본은 [퓨즈 교체 후 관측](evidence/2026-10-06/panda-post-fuse-20261006.json)입니다.
`faults=24`는 기존 ECAN-only 제외 mask의 비-ECAN 두 비트이며 해당 관측의 ECAN
bus-off/error-warning/error-passive는 모두 0입니다. 원시 fault 값과 vehicle DTC는 다릅니다.
점화 미감지와 LFA/SCC 미수신은 퓨즈 교체 **전**의 관측입니다.

퓨즈 교체 전 HVAC DTC raw `923413`, status `09`를 읽었습니다. 한 번 삭제 요청이
수락됐지만 즉시 같은 코드가 다시 확인됐습니다. ADAS 후보 주소들은 당시 timeout으로
삭제 성공이 미확인이었습니다. 퓨즈 교체 후에는 DTC/ECU 식별을 재조회하지 않았습니다.
원본과 순서는 [evidence 목록](evidence/2026-10-06/README.md)에 있습니다.

## 차량 컴퓨터에서 저장소 준비

새 clone의 예시입니다. ROS 1 Noetic이 설치된 Ubuntu 20.04를 전제로 합니다.

```bash
mkdir -p "$HOME/catkin_ws/src"
git clone --branch feat/initial-implementation \
  https://github.com/kelvin926/Ioniq5_ECAN.git "$HOME/catkin_ws/src/ioniq5_ecan"
export ECAN_REPO="$HOME/catkin_ws/src/ioniq5_ecan"
cd "$ECAN_REPO"
git log -2 --oneline
git status --short
```

이미 clone이 있으면 해당 checkout에서 local diff를 먼저 확인하고 보존합니다. 깨끗한
checkout에서는 아래처럼 갱신합니다. 다른 경로라면 `ECAN_REPO`를 그 경로로 지정하고
catkin workspace의 `src/ioniq5_ecan`이 그 checkout을 가리키게 합니다.

```bash
git status --short
git fetch origin
git checkout feat/initial-implementation
git pull --ff-only origin feat/initial-implementation
git rev-parse HEAD
```

## 의존성과 USB 권한

아래 apt 명령은 ROS Noetic apt 저장소가 설정된 환경에서 사용합니다. ROS가 아직 없다면
그 환경 설치를 먼저 완료해야 합니다. ROS C++ 실행과 read-only preflight에는 Panda/opendbc
Python checkout이나 firmware 재빌드가 필요하지 않습니다.

```bash
sudo apt update
sudo apt install -y build-essential cmake git usbutils libusb-1.0-0-dev pkg-config \
  python3-nose python3-pip ros-noetic-ros-base ros-noetic-diagnostic-msgs \
  ros-noetic-message-generation ros-noetic-roscpp ros-noetic-std-srvs
python3 -m pip install --user libusb1
sudo install -m 0644 "$ECAN_REPO/config/99-red-panda.rules" \
  /etc/udev/rules.d/99-red-panda.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo usermod -aG plugdev "$USER"
```

그룹 변경 후 다시 로그인하고 Panda USB를 재연결합니다. 새 터미널에서 `ECAN_REPO`를
다시 지정합니다. 현재 연결된 Panda의 marker/ABI는 확인됐지만 설치 binary의 정확한 hash와
split-brake patch revision은 미확인입니다. 빌드와 flash는 별도이며, firmware 작업이 필요하면
[panda_firmware.md](panda_firmware.md)의 provenance 및 해당 절차를 따릅니다.

## 차량 컴퓨터에서 빌드

```bash
export ECAN_REPO="$HOME/catkin_ws/src/ioniq5_ecan"
source /opt/ros/noetic/setup.bash
cd "$ECAN_REPO"
./scripts/check_environment.sh
cd "$HOME/catkin_ws"
catkin_make -DCMAKE_BUILD_TYPE=Release
source devel/setup.bash
rospack find ioniq5_ecan
```

이 checkout의 전체 ROS Noetic 빌드는 Windows에서 수행하지 못했습니다. 차량 컴퓨터의
첫 native 빌드 결과를 `state.json`에 기록합니다. 기존 core smoke는 통과했으므로 문서
확인만을 위해 반복할 필요는 없습니다. 변경/실패 원인에 관련된 package 검증이 필요하면
`catkin_make run_tests_ioniq5_ecan`과 `catkin_test_results --verbose`를 한 번 수행합니다.

## 첫 연결: read-only 확인과 passive ROS

Panda를 점유하는 ROS node/Cabana/helper를 종료한 뒤 아래 명령을 사용합니다.

```bash
cd "$ECAN_REPO"
mkdir -p log/vehicle_computer
python3 scripts/panda_preflight.py \
  --serial PANDA_SERIAL --ecan-only --require-harness --json \
  > log/vehicle_computer/preflight.json
cat log/vehicle_computer/preflight.json
```

이 스크립트는 USB control read만 수행합니다. `harness_status=1`, ignition, ECAN RX 증가,
marker/ABI와 failure 목록을 확인합니다. 반대 방향 `2`이면 현재 ECAN-only 구성과 맞지
않습니다. latest `rx_buffer_overflow`는 누적 687,229였지만 passive 3초 동안 증가는 0입니다.
preflight는 누적 overflow도 failure로 처리하므로 해당 기록과 현재 수신 상태를 구분하고,
실패 결과를 숨기거나 `PREFLIGHT PASS`로 기록하지 않습니다.

이후 actuation이 비활성화된 프로파일을 **명시해서** 실행합니다. passive ROS node는 Panda
bitrate/NO_OUTPUT 설정과 heartbeat를 수행하므로 위 read-only preflight와 동작이 다릅니다.

```bash
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"
roslaunch ioniq5_ecan ioniq5_ecan.launch \
  config:="$ECAN_REPO/config/ioniq5_ecan_passive.yaml"
```

같은 ROS 환경을 source한 별도 터미널에서 확인합니다.

```bash
rostopic echo -n 1 /ioniq5/vehicle_state
rostopic hz /ioniq5/vehicle_state
rostopic echo -n 5 /ioniq5/can0/rx
rostopic echo -n 1 /diagnostics
```

`vehicle_state`는 20 Hz 상태 게시이며 LFA/SCC 각각의 CAN 주기와 다릅니다. `valid=true`,
EPS/ACC 상태, 물리 입력에 맞는 조향/속도/페달/브레이크/기어 값을 확인합니다.
버튼 프레임 `0x1CF`/`0x1AA`와 실제 버튼 변화에 맞춰 `hardware.alternate_buttons`를 정합니다.
raw `0x12A`/`0x1A0`의 크기/CRC/counter를 확인하고 source commit, YAML, preflight와 trace를
기록합니다. 외부 컴퓨터의 상위 ROS 제어기를 연결할 경우 양쪽의 `ROS_MASTER_URI`와
각 host의 `ROS_IP`/`ROS_HOSTNAME`을 실제 네트워크에 맞춥니다. 주소는 이 문서에서 추정하지
않습니다.

## 제어 설정과 아직 남은 선행 확인

현재 `src/node.cpp`는 camera 소유권 대상으로 `0x730/0x738`, radar로 `0x7D0/0x7D8`를
사용합니다. pinned upstream Ioniq 5 firmware 기록의 camera 주소는 `0x7C4`이고 `0x730`은
LKA steering 플랫폼의 ADAS Driving 후보입니다. 실차 ECU 식별은 아직 미확인입니다.
퓨즈 교체 전 timeout을 근거로 주소를 바꾸지 않았습니다. **첫 takeover 전에 실제 응답 ECU와
LFA/SCC 송신 소유권을 확인하는 작업이 남아 있습니다.** stock quiet 확인과 positive UDS
응답은 각각 기록합니다. 송신 성공만으로 ECU의 실제 제어 수용을 판단하지 않습니다.

상위 제어기의 입력 계약도 최종 확정 전입니다. 현재 계약은 다음과 같습니다.

| 항목 | 현재 값/동작 |
| --- | --- |
| 토픽 / 메시지 | `/ioniq5/actuation_command`, `ioniq5_ecan/ActuationCommand` |
| 필수 값 | `lateral`, `acceleration`; 기본 `use_enable_field=false` |
| 기본 lateral | `steering_rate_deg_s`, deg/s를 목표각으로 적분한 뒤 토크로 변환 |
| 상위가 토크를 보내는 경우 | `input.lateral_mode=direct_torque`; 단위는 Panda count, Nm가 아님 |
| acceleration | m/s², 0.01 m/s² CAN 양자화, 허용 범위 -3.5~2.0 |
| 입력 shaping | 기본 `unfiltered_input=true`; host smoothing/clamp 생략, 토크/전송/채널 경계는 유지 |
| command watchdog / CAN freshness | 250 / 250 ms |
| 차량 출력 | LFA 토크 100 Hz, SCC 가속도 50 Hz; native angle 입력은 미구현 |

상위 토크를 기본 rate 모드로 보내지 않습니다. 단위/부호/주기를 확정한 뒤 실제 제어용
설정을 사용합니다. 제어 프로파일 예시는 아래이며, launch 기본값도 이 활성 프로파일입니다.

```bash
roslaunch ioniq5_ecan ioniq5_ecan.launch \
  config:="$ECAN_REPO/config/ioniq5_ecan.yaml"
```

최초 최신 command 수신으로 auto-arm/ECU takeover가 시작될 수 있습니다. 시작 command를
보내기 전에 정차, EPS 정상, 종방향 구성의 D/recent stock SCC와 위 endpoint 확인을
완료합니다. LDA는 lateral-only, SET을 눌렀다 놓으면 combined 선택/토글입니다.
브레이크는 lateral을 유지하고 longitudinal을 래치 해제하며, release만으로 종방향이
재개되지 않습니다. 기본 연구장 YAML의 host CANCEL/gas override는 false입니다.
명시적 해제 서비스는 다음과 같습니다.

```bash
rosservice call /ioniq5_ecan/set_armed "data: false"
```

활성 일시 EPS 오류는 고정 3초 `SOFT_DISABLING` 창 안에서 최신 command/CAN/Panda 허가로
복귀하며 기존 brake/ACC 종방향 래치를 보존합니다. hard fault/창 만료는 disarm과 순정 ECU
복구로 전환합니다. stock 복구는 radar→camera, 실패 시 1/2/4/8/16/30초 retry와 USB 재연결을
사용합니다. 복구 완료 후에도 명시적 재arm과 물리 활성화가 필요합니다.
`/diagnostics`의 `soft_disable_remaining_ms`, `ecu_recovery_pending`,
`ecu_recovery_attempts`, `recovery_rearm_required`를 기록합니다. 최신 ROS 경로의 실제 차량
제어, 채널 동작과 복구 성공은 아직 기록되지 않았습니다.

## 다음 작업자에게 전달할 시작 문구

> Ubuntu 20.04 차량 컴퓨터에서 Ioniq5_ECAN 작업을 이어간다. `feat/initial-implementation`의
> 최신 checkout에서 AGENTS.md, state.json, docs/vehicle_computer_handoff.md를 먼저 읽고
> 현재 commit을 기록해라. 퓨즈 교체 뒤 Panda 정방향/ignition=1과 순정 LFA 100 Hz/SCC 50 Hz
> 수신은 확인했지만 차량 컴퓨터 catkin 빌드, 설치 firmware patch revision과 camera 소유권
> endpoint는 미확인이다. 환경/빌드와 read-only 및 passive 수신 확인부터 진행하고 결과를
> state.json에 기록해라. 상위 토크와 가속도 계약을 확인하여 rate 모드와 혼동하지 말아라.
> 차량 컴퓨터로 옮기는 요청 자체는 실제 actuator/ECU disable/flash 실행 요청이 아니다.
