# 연구차량 인수인계 — 2026-08-21

이 문서는 차량 탑재 Ubuntu 20.04 컴퓨터의 Codex가 현재 상태에서 바로 작업을 이어가기
위한 기준 문서입니다. 먼저 이 문서와 `git status`, `git log -1`, 실제 Panda health를
확인하십시오. 아래의 “확인됨”과 “미확인”을 구분해서 사용해야 합니다.

## 한눈에 보는 현재 상태

| 항목 | 현재 값 |
| --- | --- |
| 차량 | Hyundai Ioniq 5 2022, HDA1, EV, radar-SCC |
| 호스트 타깃 | Ubuntu 20.04, ROS 1 Noetic, C++17 |
| 하네스 | Hyundai K camera harness, 정상 방향 `harness_status=1` 필수 |
| 제어 CAN | physical CAN1 / Panda logical bus 0 / ECAN 500/2000 kbps |
| 사용하지 않는 CAN | physical CAN2/CAN3/CAN4, forwarding 모두 firmware에서 비활성화 |
| Red Panda serial | `05001a000151333031393436` |
| firmware marker | `IONIQ5ECAN-dd8a5b3d-DEBUG` |
| 현재 signed app SHA-256 | `1ef6d981457e8b2018fcbf72b8a43f88ba85f95a6b94800b177e8302df212bae` |
| 현재 Panda 상태 | 새 image 플래시 및 검증 완료, 차량 미연결 `NO_OUTPUT`, faults/overflow 0 |

확인된 실차 결과:

- 저속 주행 중 LFA `0x12A` 조향 송신과 좌우 조향 동작
- 약 1 km/h에서 시작하여 직선 15 km/h까지 `SCC_CONTROL (0x1A0)` 가속 1회 성공
- yaw rate, 횡·종가속도, 4륜 속도를 포함한 ECAN 수신 경로 구현
- LDA 버튼은 조향 arm, SET 버튼은 조향+종방향 arm으로 사용

아직 확인되지 않은 결과:

- 이 커밋의 camera `0x730` + radar `0x7D0` 이중 ECU 비활성화 수정 후의 실차 가속
- 자동 감속 요청으로 15 km/h에서 완전 정지하는 최신 경로
- ROS C++ 노드의 전체 종방향 HIL/실차 동작

## 하드웨어에서 확인된 사실

차량 연결은 `차량 카메라 커넥터 ↔ Hyundai K Y 하네스 ↔ harness board ↔ Red Panda ↔ USB
호스트` 구성입니다. 하네스보드와 K 하네스만 연결하면 HVAC가 정상인데, 문제가 있던
OBD-C 케이블로 Red Panda를 연결하면 USB와 Comma Power가 없어도 HVAC UI/LED/냉방이
먹통이 됐습니다. 다른 OBD-C 케이블로 교체하자 HVAC가 정상으로 돌아왔습니다. 따라서
HVAC 문제를 firmware나 Comma Power 문제로 단정하지 말고, 현재 정상 확인된 OBD-C
케이블과 `harness_status=1` 방향만 사용하십시오.

이 차량은 과거 제조사가 자율주행 개조용 gateway를 제거했고 일부 케이블 상태가
불명확합니다. ADAS fuse도 과거에는 제거됐다가 현재 다시 장착됐습니다. Panda 연결과
무관하게 전방 안전 시스템/차로 변경 보조 경고가 들어오는 기존 상태가 있었습니다.
별도 PCAN도 다른 컴퓨터에 연결된 적이 있으므로, 재현 시험에서는 PCAN이 송신하지 않는지
확인하거나 분리하여 송신 주체를 하나로 유지하십시오.

CAN2 쪽 ADAS camera 신호는 이 차량에서 신뢰할 수 없었고 현재 설계에는 필요하지 않습니다.
ECAN bus 0에서 조향, 종방향, 버튼 및 차량 상태를 모두 처리합니다.

## 간헐적 가속 실패의 원인과 수정

기존 시험 helper는 camera ECU `0x730`만 UDS communication-control로 비활성화하고 순정
LFA `0x12A`가 멈춘 것만 확인했습니다. 그러나 HDA1 radar-SCC 차량의 메시지 소유권은
다음과 같습니다.

| ECU | UDS request/response | 소유 메시지 |
| --- | --- | --- |
| ADAS camera | `0x730` / `0x738` | LFA `0x12A` |
| radar | `0x7D0` / `0x7D8` | SCC_CONTROL `0x1A0` |

따라서 radar가 보내는 순정 `0x1A0`과 helper가 보내는 제어 `0x1A0`이 동시에 존재했습니다.
15 km/h까지 성공한 경우도 있었지만 다른 실행에서는 약 6.9 km/h에서 더 가속되지 않거나
Panda가 `0x1A0`을 차단했습니다. 현재 수정은 다음 순서로 동작합니다.

1. D 상태와 정지 상태에서 순정 `0x1A0` template을 캡처합니다.
2. camera `0x730`을 비활성화하고 순정 `0x12A`가 quiet인지 확인합니다.
3. 종방향 시험이면 radar `0x7D0`도 비활성화하고 순정 `0x1A0`이 quiet인지 확인합니다.
4. Panda를 Hyundai CAN-FD safety param `3077`로 전환합니다.
5. HDA1에 필요한 `0x12A`, `0x1E0`, `0x1A0`, `0x160`과 두 ECU tester-present만 보냅니다.
6. `0x1A0`은 캡처한 순정 payload 위에 제어 신호만 덮어써 차량별 미확인 비트를 보존하고
   counter/checksum을 다시 생성합니다.
7. 종료 또는 예외 시 radar, camera 순서로 통신을 복구하고 Panda를 `NO_OUTPUT`으로 돌립니다.

HDA2용 `0x51`, `0x1EA`, `0x200`, `0x345`, `0x1DA`는 ECAN-only HDA1 송신 목록에서
제거했습니다. 구현은 `scripts/steering_sweep.py`와
`patches/opendbc-hyundai-canfd-split-arm.patch`에 있습니다.

## Panda firmware 재현과 플래시

고정 upstream:

- panda `dd8a5b3df77706337a11555377e7180c5adc8726`
- opendbc `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`

Ubuntu 차량 컴퓨터에서 재현 빌드:

```bash
./scripts/build_panda_debug_firmware.sh
source ~/.cache/ioniq5_ecan/upstream/venv/bin/activate
sha256sum ~/.cache/ioniq5_ecan/upstream/panda/board/obj/panda_h7.bin.signed
```

Panda를 차량에서 분리하고 USB로만 연결한 상태에서 플래시합니다.

```bash
python3 scripts/flash_panda.py \
  --serial 05001a000151333031393436 \
  --firmware ~/.cache/ioniq5_ecan/upstream/panda/board/obj/panda_h7.bin.signed \
  --confirm 05001a000151333031393436
```

2026-08-21 Windows 개발 컴퓨터에서는 다음 명령으로 동일 소스를 빌드했습니다.

```powershell
& 'C:\Program Files\Git\bin\bash.exe' `
  .\scripts\build_panda_debug_firmware.sh `
  '/c/Users/hyunseo/.cache/ioniq5_ecan/upstream-adas730'
```

부트스텁은 이번 변경에서 다시 플래시하지 않았습니다. USB 단독에서는
`harness_status=0`이므로 custom Hyundai safety mode가 SILENT로 거부되는 것이 정상입니다.
실제 TX safety hook 검사는 정상 방향의 차량 하네스에서만 가능합니다.

## 다음 실차 시험: 가장 먼저 실행할 명령

시험 전 Cabana, ROS node, PCAN 송신 프로그램처럼 Panda나 같은 CAN을 점유하는 프로그램을
모두 종료합니다. 시동 후 브레이크를 밟은 채 먼저 D에 넣고, 그 이후 P/R/N으로 바꾸지 않은
상태에서 helper를 시작합니다.

Ubuntu:

```bash
source ~/.cache/ioniq5_ecan/upstream/venv/bin/activate
python3 scripts/steering_sweep.py \
  --serial 05001a000151333031393436 \
  --target-speed-kph 15 \
  --accel-max-mps2 0.7 \
  --decel-max-mps2 0.7 \
  --rolling-test \
  --rolling-min-kph 1.0 \
  --rolling-max-kph 16.0 \
  --arm-timeout-s 60 \
  --execute
```

현재 Windows 개발 컴퓨터에서는 Python 경로만 다음과 같이 바꿉니다.

```powershell
& 'C:\Users\hyunseo\.cache\ioniq5_ecan\upstream-adas730\venv\Scripts\python.exe' `
  .\scripts\steering_sweep.py `
  --serial 05001a000151333031393436 `
  --target-speed-kph 15 `
  --accel-max-mps2 0.7 `
  --decel-max-mps2 0.7 `
  --rolling-test `
  --rolling-min-kph 1.0 `
  --rolling-max-kph 16.0 `
  --arm-timeout-s 60 `
  --execute
```

제어 전에 반드시 아래 두 줄이 모두 출력되어야 합니다.

```text
CAMERA_DISABLED address=0x730 stock_0x12A=quiet
RADAR_DISABLED address=0x7D0 stock_0x1A0=quiet
```

그다음 `ROLLING_WAIT`에서 브레이크를 놓아 1 km/h 이상으로 천천히 구르고,
`ROLLING_READY` 후 안내가 나오면 SET을 한 번 눌렀다 놓습니다. `VEHICLE_STOPPED`가 나오면
브레이크를 밟아 유지합니다. ECU 응답이나 quiet 확인이 실패하면 helper가 제어 전에
중단합니다. 종료 로그에 restore warning이 있으면 시동을 완전히 껐다 켜 ECU 통신을
복구하십시오.

## 이전에 사용한 시험 예제

저속 좌 30도 3초, 우 30도 3초 1회. LDA 버튼으로 arm합니다.

```bash
python3 scripts/steering_sweep.py \
  --serial 05001a000151333031393436 \
  --offset-deg 30 --timed-hold-s 3 --steering-cycles 1 \
  --rolling-test --rolling-min-kph 1 --rolling-max-kph 10 \
  --arm-timeout-s 60 --execute
```

가속하면서 좌 15도 2초 → 직진 2초 → 우 15도 2초 → 직진 2초를 반복합니다. SET으로
arm하며, 이 combined mode는 반복 종료 후 자동 정지하지 않고 제어를 해제하므로 운전자가
속도를 관리해야 합니다.

```bash
python3 scripts/steering_sweep.py \
  --serial 05001a000151333031393436 \
  --target-speed-kph 15 --accel-max-mps2 0.7 \
  --offset-deg 15 --combined-cycles 3 --combined-segment-s 2 \
  --rolling-test --rolling-min-kph 1 --rolling-max-kph 16 \
  --arm-timeout-s 60 --execute
```

정차 상태 최대 허용 토크 좌 3초, 우 3초 반복. 핸들을 ±5도 안에 두고 LDA로 arm합니다.

```bash
python3 scripts/steering_sweep.py \
  --serial 05001a000151333031393436 \
  --torque-sweep --timed-hold-s 3 --steering-cycles 3 \
  --arm-timeout-s 60 --execute
```

## 검증 결과와 남은 작업

이번 Windows 작업에서 완료한 검증:

- `steering_sweep.py` Python 문법 검사
- 양/음 가속과 stop request를 포함한 `SCC_CONTROL` 길이 및 Hyundai CAN-FD checksum 검사
- opendbc/Panda patch 적용 후 ARM firmware 전체 빌드
- 실제 Red Panda flash 후 marker, application mode, faults 0, RX/TX overflow 0 확인
- `git diff --check`

Windows에는 native `cc`가 없고 WSL 호출은 `Wsl/CallMsi/REGDB_E_CLASSNOTREG`로 실패하여
opendbc host safety unittest를 실행하지 못했습니다. 차량 Ubuntu 컴퓨터에서는 firmware를
사용하기 전에 최소한 다음을 수행하십시오.

```bash
cd ~/.cache/ioniq5_ecan/upstream/opendbc
python3 -m unittest opendbc.safety.tests.test_hyundai_canfd.TestHyundaiCanfdSplitButtonArm

cd /path/to/catkin_ws
catkin_make -DCMAKE_BUILD_TYPE=Release
catkin_make run_tests_ioniq5_ecan
catkin_test_results --verbose
```

가장 중요한 후속 구현은 `steering_sweep.py`에서 검증할 camera/radar 이중 UDS lifecycle을
ROS C++ 노드에도 통합하는 것입니다. 현재 C++ 노드는 CAN 송수신과 LFA/SCC codec을 갖고
있지만, 최신 radar `0x7D0` 비활성화/quiet 확인/복구 상태기계는 아직 helper에만 있습니다.
따라서 최신 helper의 실차 가속·감속을 먼저 확인한 뒤 같은 절차를 C++ 노드에 옮기고 HIL을
통과시키십시오.

문제 재현 시 다음을 한 로그에 남깁니다.

- software commit과 Panda firmware SHA-256
- Panda health 전체와 `harness_status`, orientation, ignition
- `CAMERA_DISABLED`/`RADAR_DISABLED` 및 restore 결과
- gear, brake, pedal, button, `controls_allowed`, `safety_tx_blocked`
- 순정/제어 `0x12A`, `0x1A0`의 address, bus, counter, timestamp
- 정상 OBD-C 케이블 사용 여부와 PCAN/다른 송신기 연결 여부
