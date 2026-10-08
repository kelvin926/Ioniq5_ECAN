# 차량 컴퓨터 전달 문서

2026-10-07 갱신. 대상은 **Ubuntu 20.04 / ROS 1 Noetic / C++17**입니다.
이 문서는 차량 컴퓨터에서 현재 작업을 이어가기 위한 시작 절차입니다.
설치와 USB/차량 값은 10월 6일의 관측이며 이번 문서 갱신에서 원격 설치 상태를 재조회하지 않았습니다.

## 차량 컴퓨터의 설치 결과

2026-10-06 사용자의 요청으로 Ouster 작업공간에서 아래 별도 작업공간으로 이전했습니다.
당시 Ioniq5 저장소는 `main`의 `48f9d210c08de1fba4508fee7879d278880a32c5`로
fast-forward 갱신했습니다. Cabana의 기존 openpilot revision과 로컬 CAN-FD 설정은 보존했습니다.

| 용도 | 실제 경로 |
| --- | --- |
| ROS 작업공간 | `/home/ave/catkin_ws_ioniq5` |
| Ioniq5 패키지 | `/home/ave/catkin_ws_ioniq5/src/ioniq5_ecan` |
| Cabana/openpilot | `/home/ave/catkin_ws_ioniq5/third_party/openpilot` |
| Cabana 로그 | `/home/ave/catkin_ws_ioniq5/logs/cabana_live_stream` |
| 이전 venv, 설정 및 Ouster의 Ioniq5 빌드 산출물 백업 | `/home/ave/catkin_ws_ioniq5/backups` |

ROS 전체 Release 빌드와 package test 48개가 통과했습니다. Cabana는 새 Python 3.12 venv에서
재빌드했고 `--help` 실행과 동적 라이브러리 경로를 확인했습니다. ROS의 시스템 Python 3.8은
변경하지 않았습니다. 이번 이전 작업에서 ROS node, 실시간 CAN 수신/송신, firmware flash는
실행하지 않았습니다.

Cabana 실행과 재빌드 도우미는 다음과 같습니다. 인수 없는 실행은 저장된 Panda serial과
Hyundai CAN-FD DBC를 사용합니다. ROS node 등 다른 Panda 점유 프로그램과 동시에 실행하지 않습니다.

```bash
~/catkin_ws_ioniq5/scripts/cabana.sh
# 필요할 때만 재빌드
~/catkin_ws_ioniq5/scripts/build_cabana.sh
```

이전 완료 후 같은 날 사용자 요청으로 Cabana를 켜 CAN 수신을 확인했습니다. 약 3초의
live 로그에서 ECAN 8,620개, LFA 100 Hz/SCC 50 Hz와 주요 7종 CRC 오류 0을 확인했습니다.
별도 health 관측 중 CAN TX와 RX overflow 증가도 0이었습니다. 누적 overflow는 남아 있어
엄격한 preflight는 FAIL이며 PASS로 취급하지 않습니다. 결과는
[차량 컴퓨터 수신 기록](evidence/2026-10-06/vehicle-computer-can-reception-20261006.json)에 있습니다.

그 뒤 사용자가 종료를 요청하여 Cabana가 꺼진 상태를 확인했습니다. 앱 목록 또는
`/home/ave/Desktop/ioniq5-cabana.desktop`의 **Cabana (Ioniq5 CAN)**으로 다시 실행할 수 있습니다.
바로가기는 실행 권한과 desktop 신뢰 metadata를 설정했으며, 종료 상태 유지를 위해
검증 과정에서 다시 실행하지 않았습니다.

## 가져올 브랜치와 읽을 파일

2026-10-07 사용자의 요청으로 `main` 이력에서 선택한 개인정보와 장치 식별정보를
제거했습니다. 기존 clone의 `git pull`로 옛 이력을 병합하지 않습니다. 로컬 변경과
관측 자료를 별도로 보존한 뒤 새 clone으로 이어갑니다. 옛 이력을 다시 merge/push하면
제거한 내용이 재유입될 수 있습니다. 아래 새 clone 절차는 기존 폴더를 덮어쓰지 않는
별도 위치에서 사용하고, 기존 workspace 변경은 사용자 승인 범위에서 진행합니다.

- 저장소: `https://github.com/kelvin926/Ioniq5_ECAN`
- 전달 브랜치: **`main`**. 사용자의 요청으로 최신 제어 코드와 전달 문서를 main에 반영합니다.
- 10월 6일 제어/ECU 복구 수정 기준은 [복구 수정 기록](evidence/2026-10-06/ecan-recovery-fix-20261006.json)입니다.
  개인정보 정리 전 전달 SHA를 checkout 기준으로 사용하지 않습니다. 현재 공개 `main`과
  `git log -1`을 사용하며 이번 문서 갱신은 제어 코드나 firmware를 변경하지 않았습니다.
- 먼저 [AGENTS.md](../AGENTS.md), [state.json](../state.json), 이 문서를 읽습니다.
- 상세 실차 이력: [vehicle_handoff.md](vehicle_handoff.md).
  입력 단위: [input_contract.md](input_contract.md). 검증 범위: [validation.md](validation.md).
- 현재 피드백: [vehicle_state.md](vehicle_state.md). 최신 CAN 추론: [분석 안내](ecan_analysis_20261007.md).

현재 공개 저장소에는 ROS 제어/ECU 복구 변경, 관련 test와 firmware patch source,
문서, 프로젝트 상태 및 날짜별 JSON 관측 기록이 포함됩니다. Windows 빌드 산출물,
firmware binary와 임시 진단/고장코드 삭제 스크립트는 전달하지 않습니다.
10월 7일 추가한 CAN 필드 표와 비트 검색표는 오프라인 자료이며 ROS parser의 신규 구현은 아닙니다.

## 차량 구성과 2026-10-06 관측 순서

2022 Ioniq 5 HDA1 EV, radar-SCC, Hyundai K camera harness, Red Panda 구성입니다.
Panda 일련번호는 로컬에서 확인하고 `PANDA_SERIAL`에 입력합니다. 저장소에는 기록하지 않습니다.
물리 CAN1 = firmware controller index 0
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
삭제 성공이 미확인이었습니다. 위 퓨즈 교체 직후 passive 관측에서는 DTC/ECU 식별을 재조회하지 않았습니다.
원본과 순서는 [evidence 목록](evidence/2026-10-06/README.md)에 있습니다.

그 뒤 22:09~22:17 정차 시험에서 Panda 준비 검증 실패로 ACTIVE에 진입하지 못했습니다.
초기 USB 수신/첫 RX 경쟁 조건과 stock 복구 순서를 수정하고 새 앱을 USB-only로 설치했습니다.
22:48 수정 후 재연결은 CAN valid/ready/무출력으로 정상 대기했지만 `ACCEnable=3`이 남았습니다.
승인된 `0x730` 한 번 삭제는 수락됐으나 22:52 재확인에서 같은 세 DTC/status89가 약 2초 뒤
재발하여 추가 진단/삭제/제어를 중단했습니다. 정확한 ECU identity와 제조사 고장 원인은
미확인입니다. [수정 후 기록](evidence/2026-10-06/post-fix-ecu-dtcs-20261006.json)과
[검증 범위](validation.md)를 먼저 확인합니다. 최신 ACTIVE 추종/주행 중 재인계 성공 기록은 없습니다.

## 차량 컴퓨터에서 저장소 준비

새 clone의 예시입니다. ROS 1 Noetic이 설치된 Ubuntu 20.04를 전제로 합니다.

```bash
mkdir -p "$HOME/catkin_ws_ioniq5/src"
git clone --branch main \
  https://github.com/kelvin926/Ioniq5_ECAN.git "$HOME/catkin_ws_ioniq5/src/ioniq5_ecan"
export ECAN_REPO="$HOME/catkin_ws_ioniq5/src/ioniq5_ecan"
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
git checkout main
git pull --ff-only origin main
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
다시 지정합니다. 이후 사용자 승인으로 USB-only I5R1 앱 flash와 서명/capability 검증을
완료했습니다. [기록](evidence/2026-10-06/command-session-usb-20261006.json)을 참고합니다.
차량 endpoint와 주행 중 재인계 결과는 미확인입니다. 빌드와 flash는 별도이며, firmware 작업이 필요하면
[panda_firmware.md](panda_firmware.md)의 provenance 및 해당 절차를 따릅니다.

## 차량 컴퓨터에서 빌드

```bash
export ECAN_REPO="$HOME/catkin_ws_ioniq5/src/ioniq5_ecan"
source /opt/ros/noetic/setup.bash
cd "$ECAN_REPO"
./scripts/check_environment.sh
cd "$HOME/catkin_ws_ioniq5"
catkin_make -DCMAKE_BUILD_TYPE=Release
source devel/setup.bash
rospack find ioniq5_ecan
```

2026-10-06 위 별도 작업공간에서 시스템 Python 3.8을 명시한 Release 빌드와
`catkin_make -j4 -l4 run_tests_ioniq5_ecan`을 각각 한 번 수행해 통과했습니다.
`rospack find`와 launch node 목록 해석도 새 경로에서
확인했습니다. 상세 결과는 `state.json`의 `vehicle_computer_workspace`에 있습니다.
문서 확인만을 위해 빌드와 test를 반복하지 않습니다.

## 첫 연결: read-only 확인과 passive ROS

Panda를 점유하는 ROS node/Cabana/helper를 종료한 뒤 아래 명령을 사용합니다.

```bash
cd "$ECAN_REPO"
mkdir -p log/vehicle_computer
read -r -p 'Panda serial: ' PANDA_SERIAL
python3 scripts/panda_preflight.py \
  --serial "$PANDA_SERIAL" --ecan-only --require-harness --json \
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
source "$HOME/catkin_ws_ioniq5/devel/setup.bash"
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
`valid`는 필수 4종 갱신만 검사하며 IMU/페달/기어/SCC/버튼의 개별 freshness를 보장하지 않습니다.
필드별 단위와 현재 미구현 추론 값은 [피드백 필드 표](vehicle_state.md)를 확인합니다.
버튼 프레임 `0x1CF`/`0x1AA`와 실제 버튼 변화에 맞춰 `hardware.alternate_buttons`를 정합니다.
raw `0x12A`/`0x1A0`의 크기/CRC/counter를 확인하고 source commit, YAML, preflight와 trace를
기록합니다. 외부 컴퓨터의 상위 ROS 제어기를 연결할 경우 양쪽의 `ROS_MASTER_URI`와
각 host의 `ROS_IP`/`ROS_HOSTNAME`을 실제 네트워크에 맞춥니다. 주소는 이 문서에서 추정하지
않습니다.

CAN 저장만 필요하면 [독립 수신 기록기](can_logger.md)의 `./scripts/start_can_logger.sh`를
사용합니다. SILENT 상태를 요구하고 USB IN만 수행하며 위 passive 제어 노드처럼 Panda
설정을 쓰지 않습니다. 이 기록기의 Noetic launch/실차 수집은 아직 확인되지 않았습니다.

## 제어 설정과 아직 남은 선행 확인

현재 `src/node.cpp`는 camera 소유권 대상으로 `0x730/0x738`, radar로 `0x7D0/0x7D8`를
사용합니다. pinned upstream Ioniq 5 firmware 기록의 camera 주소는 `0x7C4`이고 `0x730`은
LKA steering 플랫폼의 ADAS Driving 후보입니다. 실차 ECU 식별은 아직 미확인입니다.
퓨즈 교체 전 timeout을 근거로 주소를 바꾸지 않았습니다. **첫 takeover 전에 실제 응답 ECU와
LFA/SCC 송신 소유권을 확인하는 작업이 남아 있습니다.** stock quiet 확인과 positive UDS
응답은 각각 기록합니다. 송신 성공만으로 ECU의 실제 제어 수용을 판단하지 않습니다.

2026-10-08 사용자 결정에 따라 상위 제어기(알파마요 기반)가 조향 토크(Panda count)와 목표
종방향 가속도(m/s²)를 기존 ROS 메시지로 출력합니다. 현재 계약은 다음과 같습니다.

| 항목 | 현재 값/동작 |
| --- | --- |
| 토픽 / 메시지 | `/ioniq5/actuation_command`, `ioniq5_ecan/ActuationCommand` |
| 필수 값 | `lateral`, `acceleration`; 기본 `use_enable_field=false` |
| lateral | `input.lateral_mode=direct_torque`; LFA 토크 Panda count, 반올림 후 ±1021 끝값, Nm가 아님 |
| lateral=0 | 토크 0 요청, 각도 유지가 아님. 위치 유지는 상위 제어기의 폐루프 책임 |
| 발행 주기 | 노드는 최신 값을 100 Hz로 송신, 토크 제어에는 50~100 Hz 발행 권장 |
| acceleration | m/s², 0.01 m/s² CAN 양자화, 표현 범위 -10.23~10.24, 범위 밖은 끝값 |
| 입력 shaping | 기본 `unfiltered_input=true`; host smoothing/clamp 생략, 토크/전송/채널 경계는 유지 |
| command watchdog / CAN freshness | 250 / 250 ms |
| 차량 출력 | LFA 토크 100 Hz, SCC 가속도 50 Hz; native angle 입력은 미구현 |

상위 토크를 기본 rate 모드로 보내지 않습니다. 단위/부호/주기를 확정한 뒤 실제 제어용
설정을 사용합니다. 제어 프로파일 예시는 아래이며, launch 기본값도 이 활성 프로파일입니다.

이 컴퓨터의 시작 단축 명령은 `/home/ave/catkin_ws_ioniq5/ecan`이며 작업공간에서는 `./ecan`입니다.
`--check`는 경로 점검만 하고 ROS/Panda를 시작하지 않습니다. 전역 alias나 shell 설정은
변경하지 않으며 ROS runtime 파일은 이 작업공간 `logs/ros`에 둡니다. Cabana 등 다른 Panda
점유 프로그램을 임의로 종료하지 않고 시작을 거부합니다. 토픽 구독은 항상 유지하고,
명령 없는 초기 상태는 순정 통신/NO_OUTPUT 대기입니다. I5R1 기본 프로파일은 정상
ACTIVE 입력 단절에서도 버튼 선택을 유지한 순정 통신 복구 대기로 전환합니다.
새 입력과 정상 CAN, 완료된 stock 복구를 확인해 같은 세션은 주행 중 재인계합니다.
첫 takeover, 프로세스 재시작 또는 CAN/USB 고장 이후에는 이 예외를 적용하지 않습니다.

```bash
roslaunch ioniq5_ecan ioniq5_ecan.launch \
  config:="$ECAN_REPO/config/ioniq5_ecan.yaml"
```

최초 takeover는 최신 command와 물리 ON이 모두 있을 때 정차 상태에서 시작할 수 있습니다. 시작 command를
보내기 전에 정차, EPS 정상, 종방향 구성의 D/recent stock SCC와 위 endpoint 확인을
완료합니다. LDA는 lateral-only, SET을 눌렀다 놓으면 combined 선택/토글입니다.
브레이크는 lateral을 유지하고 longitudinal을 래치 해제하며, release만으로 종방향이
재개되지 않습니다. 기본 연구장 YAML의 host CANCEL/gas override는 false입니다.
I5R1 firmware CANCEL은 host 설정과 독립적으로 세션을 취소합니다.
명시적 해제 서비스는 다음과 같습니다.

```bash
rosservice call /ioniq5_ecan/set_armed "data: false"
```

활성 일시 EPS 오류는 고정 3초 `SOFT_DISABLING` 창 안에서 최신 command/CAN/Panda 허가로
복귀하며 기존 brake/ACC 종방향 래치를 보존합니다. hard fault/창 만료는 disarm과 순정 ECU
복구로 전환합니다. stock 복구는 radar→camera, 실패 시 1/2/4/8/16/30초 retry와 USB 재연결을
사용합니다. 복구 완료 후 새 LDA 또는 SET 조작으로 주행 중에도 다시 인계합니다.
`set_armed=false`로 명시 OFF한 뒤에는 버튼 재인계가 없고 `true` 요청이 필요합니다.
`/diagnostics`의 `soft_disable_remaining_ms`, `ecu_recovery_pending`,
`ecu_recovery_attempts`, `recovery_rearm_required`를 기록합니다. 최신 ROS 경로의 실제 차량
제어, 채널 동작과 복구 성공은 아직 기록되지 않았습니다.

## 다음 작업자에게 전달할 시작 문구

> Ubuntu 20.04 차량 컴퓨터에서 Ioniq5_ECAN 작업을 이어간다. `main`의
> 최신 checkout에서 AGENTS.md, state.json, docs/vehicle_computer_handoff.md를 먼저 읽고
> 현재 commit을 기록해라. 퓨즈 교체 뒤 Panda 정방향/ignition=1과 순정 LFA 100 Hz/SCC 50 Hz
> 수신은 확인했다. 차량 컴퓨터 Release 빌드/67개 호스트 test와 USB-only I5R1 앱 flash/서명
> 검증도 완료했다. 이후 정차 시험 실패를 수정하고 새 앱을 설치했지만 수정 후 DTC/ACC 이상이
> 재발했다. ACTIVE 추종/주행 중 입력 복귀와 보조 기능 정상 복구는 미확인이다.
> camera endpoint의 정확한 ECU identity도 미확인이다. 10월 7일 CAN 분석은 오프라인 근거이며
> 새 필드는 현재 ROS 피드백에 추가되지 않았다.
> 차량 연결 후 read-only 및 passive 수신 확인부터 진행하고 결과를 state.json에 기록해라.
> lateral은 direct_torque의 토크 count, acceleration은 m/s²이다. 조향각속도/목표각과 혼동하지 말아라.
> 차량 컴퓨터로 옮기는 요청 자체는 실제 actuator/ECU disable/flash 실행 요청이 아니다.
