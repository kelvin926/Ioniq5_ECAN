# 검증 상태와 남은 확인

2026-10-06 기준입니다. 완료한 호스트 검증, 과거 실차 결과와 향후 확인 항목을 구분합니다.
상세 날짜와 provenance는 [인수인계](vehicle_handoff.md) 및 [state.json](../state.json)에 있습니다.
문서 갱신 자체에는 빌드나 하드웨어 시험을 반복하지 않습니다.

## 완료한 검증

| 시점 | 확인한 범위 | 한계 |
| --- | --- | --- |
| 2026-10-06 수정 후 재연결/진단 | 오류 없이 startup/CAN valid/session ready/NO_OUTPUT 대기, stock LFA100/SCC50 Hz 및 TX0 관측. 0x730/7D0 DTC 조회 성공, 승인된 0x730 한 번 삭제 54 수락 | 즉시 기록 없음 이후 2초 내 동일 세 status89 기록 재발, ACCEnable3 지속. 추가 삭제/진단/제어 중단. ECU identity/고장 원인과 실제 추종 미확인. [기록](evidence/2026-10-06/post-fix-ecu-dtcs-20261006.json) |
| 2026-10-06 정차 실패 후 수정 | 실제 libsafety로 tick-before-first-RX 경쟁 재현, 수정 뒤 custom 14개와 host 관련 18개 통과. 수정 node/ARM 앱 빌드, 새 앱 USB-only flash/서명/capability, preflight PASS/3개 차단/물리 TX 0 | 초기 USB 동기화, 빈 listener 복귀와 통신 복구 순서 수정. 수정 후 실차 ACTIVE/추종/주행 중 복귀 및 ACC 이상 해소 미확인. [기록](evidence/2026-10-06/ecan-recovery-fix-20261006.json) |
| 2026-10-06 22:09~22:17 정차 ROS/ECU 시험 | 임시 20 Hz 입력과 700 ms idle 단절/복귀, physical OFF 무출력 확인. D/brake/LDA 조건에서 0x730/0x7D0 응답, stock LFA/SCC quiet 및 복구 확인 | 인계 후 Panda CAN 준비 검증 실패로 ACTIVE 미진입, 비영점 실조향 단계 미실행. 복구 뒤 ACCEnable=3 통신 이상 신호 관측. [기록](evidence/2026-10-06/stationary-ros-test-20261006.json) |
| 2026-10-06 I5R1 자동 재인계 | Release 빌드, core 50개 + protocol 5개 + USB read 4개 + preflight 8개 모두 통과. 입력 단절/주행 중 복귀, OFF, stale health/brake 래치, CAN/ignition 고장, 대기 중 mode drift 및 USB partial timeout 회귀 | 호스트 67개 unique test. 실제 ROS publisher와 차량 ECU 수명주기 통합 시험은 미실시 |
| 2026-10-06 I5R1 firmware | 고정 Hyundai CAN-FD 및 custom safety 995개 실행, 117개 skip, 878개 통과. 이 중 command-session custom 9개 전부 통과. Red Panda DEBUG ARM 앱/bootstub 빌드 통과 | bootstub은 후보 빌드만 수행, flash하지 않음. CAN trace를 재현한 safety test는 실제 ECU 수용을 증명하지 않음 |
| 2026-10-06 USB-only flash/bench | 사용자 확인 후 하네스 분리 상태에서 앱 flash, 서명/기능 응답 재검증. read-only preflight PASS. NO_OUTPUT 7173에서 조향/가속/진단 3개 차단, 물리 TX 증가 [0,0,0], controls_allowed=0 | [기록](evidence/2026-10-06/command-session-usb-20261006.json). 차량 CAN 수신, stock 복구 및 주행 중 재인계 실차 결과는 아님 |
| 2026-10-06 ROS 입력/버튼 시작 경로 | catkin Release node 빌드, core 42개 + protocol 5개 + preflight unit 7개 통과. 무명령/idle timeout 대기, Panda health 도착 순서와 무관한 OFF, 알려진 OFF in-flight rejection, fault 유지, OFF 중 조향 목표 초기화 회귀 포함 | 호스트 54개 unique test. 실제 CAN 송신, ECU takeover/복구, firmware flash 미실시 |
| 2026-10-06 실행 단축어 | workspace `ecan` 및 `start_ecan.sh` Bash 구문, `--help`/`--check`, 모의 busy-client 시작 거부, ROS 메시지 조회와 `roslaunch --nodes` 통과 | 노드/ROS master를 실행하지 않음. 실제 Cabana 프로세스는 종료하지 않음 |
| 2026-10-06 | Ubuntu 20.04/GCC 9.4/ROS Noetic에서 전체 catkin Release 빌드 및 core 36개, Panda protocol 5개, preflight unit test 7개 통과 | 새 작업공간의 호스트 검증, 차량 수신/송신과 firmware flash 미실시 |
| 2026-10-06 빌드 이후 | Cabana live 로그의 ECAN 8,620개/약 3초, LFA 100 Hz/SCC 50 Hz, 주요 7종 CRC 오류 0, 별도 health 관측 중 TX/overflow 증가 0 확인 | 누적 overflow로 엄격한 preflight는 FAIL; 비-ECAN 오류 유지, actuator/ECU 검증 아님; 이후 Cabana 종료 |
| 2026-10-06 | native Zig 0.16 C++17 core 및 `core_smoke`, `-Wall -Wextra -Wpedantic -Werror` 통과 | ROS node/USB/ECU 통합 빌드가 아님 |
| 2026-10-07 | 수신 전용 CAN logger 모의 USB/분할/timeout/64-byte/CSV timestamp 테스트 6개 통과 | Windows host 테스트, 실제 Noetic launch/차량 기록 미실시 |
| 2026-10-06 | ECU retry deadline/backoff cap, fault latch 및 명시적 재arm core smoke 통과 | 실제 UDS 복구 및 USB fault injection 미실시 |
| 2026-10-06 | EPS 일시 복귀, 2999/3000 ms 경계, 반복 오류 deadline 유지, 새 오류 창, 정상 종방향 유지, 목표각 초기화, Panda 허가, brake/CANCEL/disarm, hard fault 우선순위, CRC/freshness core smoke 통과 | 실차 MDPS 복귀와 ROS raw callback 미시험 |
| 2026-08-21 기록 | codec/checksum, SCC template 보존, 무평활 입력, split-brake, custom Panda host safety 5개, ARM firmware 빌드 | 당시 소스/후보 기준, 현재 설치 image와 동일하다고 단정 불가 |
| 2026-08-21 기록 | 임시 ROS API/message 선언으로 node object compile | 실제 ROS Noetic/catkin 빌드가 아님 |
| 2026-08-21 기록 | helper 저속 좌우 조향, 직선 약 15 km/h 가속 1회 | 이후 가속 실패 존재, 최신 dual-ECU/ROS 경로 성공 미기록 |

이전 Windows 전달 작업에서는 ROS Noetic 빌드를 수행하지 못했으나, 같은 날 차량 컴퓨터의
`/home/ave/catkin_ws_ioniq5`에서 native 빌드와 package test를 확인했습니다. 최신 I5R1 앱
설치 서명과 capability는 USB-only로 확인했습니다. 이후 정차 시험에서 dual-ECU 통신
disable/복구는 관측했지만 Panda 준비 검증 실패와 SCC 통신 이상 신호가 남았습니다.
관련 경쟁 조건과 host 복구 경로는 이후 수정/빌드하고 새 앱을 USB-only로 검증했습니다.
수정 후 차량 수신/대기는 정상이나, 승인된 0x730 한 번 삭제 후 세 기록이 재발하고 ACC 이상이 남아 제어 시험은 중단했습니다.
ACTIVE 추종, 주행 중 입력 복귀, 실제 일시 EPS 복귀 및 차량 고장 해소는 여전히 미확인입니다.

## 변경에 맞는 host 확인

core 변경 시 ROS 없이 다음 경로로 확인할 수 있습니다. `core_smoke`에는 최신 일시 복귀
회귀 검사가 포함됩니다. 이 명령은 재현 방법이며 이번 문서 작업에서 실행한 결과가 아닙니다.

```bash
cmake -S . -B build/core -DIONIQ5_ECAN_CORE_ONLY=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build/core --target core_smoke
cmake -E chdir build/core ctest -R '^core_smoke$' --output-on-failure
```

위 명령은 Ubuntu 20.04의 도구와 호환되도록 `ctest --test-dir` 대신 `cmake -E chdir`을
사용합니다. `--test-dir`는 CMake 3.20부터 추가된 옵션입니다.
[CMake 3.20 release notes](https://cmake.org/cmake/help/v3.20/release/3.20.html#ctest),
[CMake 3.16 command tools](https://cmake.org/cmake/help/v3.16/manual/cmake.1.html#run-a-command-line-tool).

ROS integration 변경은 Ubuntu 20.04에서 catkin 빌드와 관련 package test를 확인합니다.
Panda patch 변경은 고정 opendbc의 custom safety test와 firmware 빌드가 필요합니다.
단순 문서/주석 변경은 링크, 값/스키마 불변, JSON, `git diff --check`로 확인합니다.

## Panda bench 및 passive 수신

- USB-only `panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only --require-command-session` PASS 기록 완료
- firmware build/flash hash와 네 patch hash는 위 기록에 분리 보관, marker/packet hash만으로 설치 revision 단정 금지
- `NO_OUTPUT` TX 차단, loopback counter/CRC/목표 주기 및 raw metadata 보존 확인
- 차량 연결 시 `--require-harness`로 `harness_status=1`, ignition과 ECAN RX 증가 확인
- passive YAML로 ECAN 주소 및 실제 버튼 `0x1CF`/`0x1AA` 확인 후 `alternate_buttons` 선택
- parser의 조향/속도/페달/브레이크/기어 부호와 배율을 물리 입력에 대조
- ROS 노드, Cabana 및 PCAN 등 송신/USB 점유 주체 기록

passive ROS 노드는 Panda 설정을 수행합니다. control write 없는 preflight와 구분합니다.

## ROS/ECU 수명주기와 일시 복귀

- 초기 takeover의 정차/EPS 정상 조건, 종방향 프로파일의 D/recent stock SCC 요구 확인
- camera/radar 각각 disable response와 순정 `0x12A`/`0x1A0` quiet 확인, 실패 시 무출력
- LDA lateral-only 및 SET combined 토글, brake 중 lateral 유지/longitudinal latch-off와 SET 재개
- ACTIVE 중 MDPS 보조 오류에서 LFA 0/비활성, heartbeat/tester-present/소유권 유지와 정상 SCC 지속
- 유효한 clear 샘플이 3초 전에 들어오면 현재 명령으로 복귀, CRC 불량 clear 샘플은 무시
- 3초 만료/늦은 clear/CAN stale/Panda 장애 또는 EPS pause 중 명령 단절은 hard fault와 자동 재arm 억제
- 정상 ACTIVE의 입력 단절은 출력 해제/stock 복구, 물리 ON 유지, 복구 완료 후 새 입력으로 주행 중 재인계
- 입력 대기 중 물리 OFF/CANCEL, hard fault, 프로세스/USB/Panda 재시작은 이전 ON을 자동 계승하지 않음
- EPS 복귀가 brake/ACC 종방향 래치를 해제하지 않는지 확인
- raw TX는 종방향 허가 gate를 따르고, 일시 EPS 대기 중 raw LFA는 차단되는지 확인
- hard fault 복구의 radar→camera 통신 요청 후 두 stock 재개, 유실 ACK와 valid stock 구분, Panda NO_OUTPUT 확인
- 실패 복구의 1/2/4/8/16/30초 backoff, USB 재연결, pending 중 재arm 거부 및 완료 후 명시적 재arm 확인

## 실차 결과를 남길 항목

폐쇄 시험장 및 기존 차량 시험계획에서 lateral-only와 combined 동작, 가속/감속/정지,
운전자 개입, timeout과 복구를 확인합니다. 실제로 켠 YAML 옵션을 함께 기록합니다.
기본 연구장 YAML은 host CANCEL/gas override가 false입니다. I5R1 firmware CANCEL은 이와
독립적으로 세션을 취소하며, gas override는 자동 추가하지 않았습니다. 순정 AEB 유지 여부도 확인되지 않았습니다.

각 결과에는 software commit과 local diff, YAML, 실제 설치 firmware hash/provenance,
harness/ignition, CAN trace, Panda health, 채널 상태 및 diagnostics를 보관합니다.
가속 실패 뒤 운전자 brake 개입을 실패 원인으로 취급하지 않습니다.
