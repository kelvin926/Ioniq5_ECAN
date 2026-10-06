# 검증 상태와 남은 확인

2026-10-06 기준입니다. 완료한 호스트 검증, 과거 실차 결과와 향후 확인 항목을 구분합니다.
상세 날짜와 provenance는 [인수인계](vehicle_handoff.md) 및 [state.json](../state.json)에 있습니다.
문서 갱신 자체에는 빌드나 하드웨어 시험을 반복하지 않습니다.

## 완료한 검증

| 시점 | 확인한 범위 | 한계 |
| --- | --- | --- |
| 2026-10-06 | native Zig 0.16 C++17 core 및 `core_smoke`, `-Wall -Wextra -Wpedantic -Werror` 통과 | ROS node/USB/ECU 통합 빌드가 아님 |
| 2026-10-06 | ECU retry deadline/backoff cap, fault latch 및 명시적 재arm core smoke 통과 | 실제 UDS 복구 및 USB fault injection 미실시 |
| 2026-10-06 | EPS 일시 복귀, 2999/3000 ms 경계, 반복 오류 deadline 유지, 새 오류 창, 정상 종방향 유지, 목표각 초기화, Panda 허가, brake/CANCEL/disarm, hard fault 우선순위, CRC/freshness core smoke 통과 | 실차 MDPS 복귀와 ROS raw callback 미시험 |
| 2026-08-21 기록 | codec/checksum, SCC template 보존, 무평활 입력, split-brake, custom Panda host safety 5개, ARM firmware 빌드 | 당시 소스/후보 기준, 현재 설치 image와 동일하다고 단정 불가 |
| 2026-08-21 기록 | 임시 ROS API/message 선언으로 node object compile | 실제 ROS Noetic/catkin 빌드가 아님 |
| 2026-08-21 기록 | helper 저속 좌우 조향, 직선 약 15 km/h 가속 1회 | 이후 가속 실패 존재, 최신 dual-ECU/ROS 경로 성공 미기록 |

현재 Windows shell에는 `catkin_make`/`rosversion`이 없습니다. 전체 ROS Noetic 빌드와
최신 firmware 설치 revision, dual-ECU HIL/실차, 실제 일시 EPS 및 hard fault 복구는 미확인입니다.

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

- USB-only `panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only`의 실제 PASS 결과 기록
- firmware build/flash hash와 세 patch hash를 구분하여 기록, marker/packet hash만으로 설치 revision 단정 금지
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
- 3초 만료/늦은 clear/CAN stale/Panda 장애/명령 단절에서 hard fault와 자동 재arm 억제
- EPS 복귀가 brake/ACC 종방향 래치를 해제하지 않는지 확인
- raw TX는 종방향 허가 gate를 따르고, 일시 EPS 대기 중 raw LFA는 차단되는지 확인
- hard fault 복구의 radar→camera stock 재개, 유실 ACK와 valid stock 구분, Panda NO_OUTPUT 확인
- 실패 복구의 1/2/4/8/16/30초 backoff, USB 재연결, pending 중 재arm 거부 및 완료 후 명시적 재arm 확인

## 실차 결과를 남길 항목

폐쇄 시험장 및 기존 차량 시험계획에서 lateral-only와 combined 동작, 가속/감속/정지,
운전자 개입, timeout과 복구를 확인합니다. 실제로 켠 YAML 옵션을 함께 기록합니다.
기본 연구장 YAML은 host CANCEL/gas override가 false이므로 해당 옵션을 켜지 않은 시험에서
그 동작을 기대하면 안 됩니다. 순정 AEB 유지 여부도 확인되지 않았습니다.

각 결과에는 software commit과 local diff, YAML, 실제 설치 firmware hash/provenance,
harness/ignition, CAN trace, Panda health, 채널 상태 및 diagnostics를 보관합니다.
가속 실패 뒤 운전자 brake 개입을 실패 원인으로 취급하지 않습니다.
