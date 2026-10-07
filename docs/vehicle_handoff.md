# 연구차량 인수인계 — 2026-10-06

현재 코드/설정, 날짜가 있는 하드웨어 관측과 과거 실차 시험을 구분한 문서입니다.
작업 시작 시 [AGENTS.md](../AGENTS.md), [state.json](../state.json), `git status`, `git log -1`을
확인합니다. 아래 USB 기록은 관측 당시의 값이며 현재 연결 상태를 대신하지 않습니다.

## 프로젝트와 현재 구현

| 항목 | 기준 |
| --- | --- |
| 차량 | 2022 Hyundai Ioniq 5, HDA1, EV, radar-SCC |
| 실행 타깃 | Ubuntu 20.04, ROS 1 Noetic, catkin, C++17 |
| 하네스 | Hyundai K camera harness, `harness_status=1` |
| 제어 CAN | physical CAN1 / Panda logical bus 0 / ECAN 500/2000 kbps |
| 비-ECAN | transceiver 및 forwarding 비활성화, camera bus 2 미사용 |
| Panda | Red Panda, 일련번호는 로컬 설정 |
| 명령 | `/ioniq5/actuation_command`, 기본 `lateral` deg/s + `acceleration` m/s² |
| 차량 조향 출력 | LFA `0x12A` 토크 100 Hz, native angle 제어 미구현/차량 지원 미확인 |
| 종방향 출력 | SCC `0x1A0` 및 FCA `0x160` 50 Hz |
| launch 기본값 | actuation/longitudinal/auto arm/raw TX가 켜진 연구장 YAML |
| 관찰용 설정 | `config/ioniq5_ecan_passive.yaml`, 네 기능 모두 false |

기본 `unfiltered_input=true`, `use_enable_field=false`입니다. 입력 scale/offset, rate→angle→torque
변환, torque 제한/변화율, CAN 양자화, 채널 허가와 watchdog은 유지됩니다.
270 count는 명령 상한이며 MDPS 최대 구동력이 확인된 값은 아닙니다.
host CANCEL/gas override는 연구장 기본 YAML에서 false입니다. I5R1 firmware CANCEL은
host 설정과 독립적으로 세션을 취소합니다. 자세한 단위와 상태 필드는
[입력 계약](input_contract.md), 활성화 조건은 [상태와 제한](safety.md)에 있습니다.

초기 takeover는 정차, EPS 정상과 유효한 CAN을 요구합니다. 종방향 프로파일에서는 D와
최근 순정 SCC template도 필요합니다. camera LFA quiet, 종방향이면 radar SCC quiet도
확인한 뒤 Hyundai mode로 전환합니다. LDA는 lateral-only, SET release는 combined를
선택/토글합니다. brake는 lateral을 유지하고 longitudinal만 SET 재조작 전까지 래치 해제합니다.

## 최신 복귀/복구 구현

- 활성 중 일시 MDPS LKA 보조 오류: `SOFT_DISABLING`, 고정 3초 창, LFA 0/비활성,
  기존 arm/소유권/heartbeat/tester-present 유지, 정상 허가된 종방향은 현재 명령 지속
- deadline 전에 EPS clear + 최신 유효 command/CAN + Panda 허가: 현재 명령으로 ACTIVE 복귀,
  steering 적분/목표각 초기화, brake/ACC 종방향 래치 보존, raw LFA 우회 차단
- 3초 만료 또는 CAN/Panda/command hard fault: 전체 disarm, auto rearm 억제, stock ECU 복구
- I5R1 정상 ACTIVE 입력 단절: 출력 중단/순정 통신 복구, 물리 ON 보존. 복구 완료와
  새 유효 command/CAN 확인 후 같은 세션에서는 주행 중에도 현재 입력으로 재인계
- 최초 takeover는 정차. 물리 OFF/CANCEL, CAN/USB/harness/ignition 고장 및 프로세스/장치
  재시작은 이전 ON 또는 주행 중 재인계 자격을 자동 계승하지 않음
- ROS stock 복구: radar→camera 통신 요청을 먼저 완료한 뒤 두 valid stock 재개와 Panda NO_OUTPUT 확인, 실패 시 실행 중
  1/2/4/8/16/30초 capped retry, USB 자동 재연결, pending 동안 actuator 출력/rearm 차단
- hard fault 복구 완료 후: 정상 상태와 정차 조건을 확인하고 `set_armed=false` → `true`,
  물리 LDA/SET으로 재활성화. 장애 전 명령이나 채널을 자동 재개하지 않음

이 구현은 ECU 자체 reset/수리를 보장하지 않습니다. UDS는 control thread에서 동기 실행하며
hard real-time 보장도 없습니다. [구조](architecture.md)와 [지연](latency.md)을 참고하십시오.

## 2026-10-06 USB 관측과 확인 범위

read-only `panda_preflight.probe`의 vendor control read와 USB bulk CAN 수신 결과입니다.
사용자가 실차 연결, READY 상태와 하네스 방향 변경을 알려준 뒤 확인했습니다.
전체 preflight validation 루틴은 실행하지 않았으므로 `PREFLIGHT PASS` 기록으로 취급하지 않습니다.

| 관측 항목 | 당시 값 |
| --- | --- |
| hardware / application | `0x07` Red Panda / true |
| marker | `IONIQ5ECAN-dd8a5b3d-DEBUG` |
| health / CAN packet hash | `0x290DAE03` / `0x75ABF276` |
| harness / ignition line / ignition CAN | 1 / 0 / 0, READY는 사용자 확인 |
| safety mode / param / controls allowed | 0 / 0 / 0 |
| faults / heartbeat lost | 16 / 0, 기존 ECAN-only mask 적용 시 effective faults 0 |
| ECAN 새 RX / TX | 2초 동안 4,854 / 모든 controller 0 |
| MDPS `0xEA` | bus 0, 24 bytes, CRC 정상 368개 / 실패 0개 |
| PA plugin / mode / CAN fault | 0 / 1 / 0: 미연결, 초기화 상태, fault 없음 |
| ACI plugin / active / fault | 0 / 1 / 0: 미연결, 비활성, fault 없음 |
| 정확한 설치 binary hash | 읽지 않음 |

같은 날짜의 앞선 USB-only 관측은 harness/ignition/faults 모두 0이었습니다. 실차 연결 후
처음에는 `FLIPPED(2)`와 새 RX 0이었고, 사용자 방향 변경 후 `NORMAL(1)`과 지속 RX를
확인했습니다. READY와 별개로 Panda ignition 값은 계속 0이므로 초기 제어 허가를
확인한 결과로 취급하지 않습니다. PA/ACI 상태는 현재 협조 제어가 연결되지 않았다는
뜻이며 MDPS firmware의 잠재 지원 부재를 증명하지 않습니다. 순정 RSPA는 사용자 확인상
미장착이며, native 명령 수용과 더 큰 정차 조향력은 여전히 미확인입니다.

USB 수신에는 이전 queue가 포함되므로 읽은 총 8,947개와 새 hardware RX 수를 구분합니다.
RX overflow는 누적 220,434, 마지막 2초 증가 2로 loss-free capture는 아닙니다.
원본 요약은 [vehicle-observation-20261006-mdps.json](evidence/2026-10-06/vehicle-observation-20261006-mdps.json)과 [state.json](../state.json)에
있습니다. marker만으로 아래 split-brake 후보의 설치 여부를 단정할 수 없습니다.
위 passive 관측에서는 CAN 송신, ECU 진단 요청, Panda control write 또는 firmware flash를
수행하지 않았습니다.

같은 날 이후 제한을 유지한 우측 조향 요청을 받고 2초 read-only 수신을 다시 수행했습니다.
최신 CRC 정상 프레임은 바퀴 속도 모두 0, brake=true, D(raw 5), pedal=0,
조향각 -1.3 deg, EPS fault=false였습니다. Panda uptime 511초의 health는 harness=1,
ignition line/CAN=0/0, safety mode=0, controls_allowed=0입니다.
기존 `steering_sweep.py`의 시작 조건 `ignition_line=1`을 충족하지 못해 조향을
실행하지 않았습니다. 이 조건을 바꾸거나 우회하지 않았으며, 점화 미감지 원인은
미확인입니다. 원본은 [steering-readiness-20261006.json](evidence/2026-10-06/steering-readiness-20261006.json)에 있습니다.

같은 날 사용자의 DTC 조회 요청으로 logical bus 0에서 진단을 수행했습니다.
HVAC `0x7B3/0x7BB`가 `19 02 FF`에 정상 응답했고, 원시 DTC `923413` 한 건과
status `0x09`(TEST_FAILED + CONFIRMED_DTC)를 읽었습니다. 통상 SAE 표시 후보는
`B1234-13`이나, ECU의 DTC format identifier 조회 `19 01 FF`는 NRC `0x12`로
거절됐습니다. 아이오닉5 전용 제조사 코드 설명과 계기판 경고의 원인은 미확인입니다.
다른 9개 후보 진단 주소는 timeout이며, 코드 없음이나 ECU 부재로 해석하지 않습니다.

조회 동안 Panda ELM327 `3/1`, controls_allowed=0을 유지했고 diagnostic 끝의
safety_tx_blocked=0, physical CAN controller TX 증가 [11,0,0]을 확인했습니다.
점화 미감지에 따른 2초 SILENT 복귀를 방지하려고 비활성 heartbeat를 사용했습니다.
USB packet-tail reset과 timeout의 부분 수신 데이터 보존은 통신 처리이며 ECU reset이
아닙니다. 종료 후 기존 safety `0/0`과 power_save=1 복원을 확인했습니다.
DTC 삭제, ECU session 변경/reset, 통신 disable, 조향/가속 명령 또는 flash는 하지
않았습니다. 원본은 [vehicle-dtcs-20261006.json](evidence/2026-10-06/vehicle-dtcs-20261006.json), 요약은 [state.json](../state.json)에
있으며 현재 ECAN 경로의 부분 조회 결과입니다.

같은 날 ADAS 집중 조회에서는 전방 radar `0x7D0`, 전방 camera `0x7C4`, ADAS 후보
`0x730`, parking ADAS `0x7B1`, corner radar `0x7B7`, MDPS `0x7D4`, ABS/ESC `0x7D1`에
`22 F1 00`, `22 F1 10`, `19 02 FF`를 보냈지만 모두 timeout이었습니다. HVAC 참조 조회는
앞서 기록한 코드에 다시 정상 응답했습니다. 조회 구간의 ECAN RX 증가는 41,450,
진단 TX 증가는 22였으며, `0x12A` LFA와 `0x1A0` SCC는 수신 기록에 없었습니다.
최신 CRC 정상 MDPS `0xEA`는 warning lamp/LKA fault/fail raw 값이 모두 0입니다.
RX overflow 증가 26 때문에 무손실 관측은 아니며, 이 결과로 ADAS ECU의 고장이나
전원 단절을 확정하지 않습니다. 전원, harness/network 연결과 진단 접근 경로의 구분이
필요합니다. 원본은 [adas-diagnostics-20261006.json](evidence/2026-10-06/adas-diagnostics-20261006.json)에 있습니다.

이후 사용자가 고장코드 삭제를 명시적으로 요청하여 위 7개 후보와 HVAC에 각각
`14 FF FF FF` 삭제 요청을 한 번 보냈습니다. HVAC는 positive response `54`로 수락했지만
즉시 `19 02 FF` 재조회에서 동일한 `923413`, status `09`가 다시 확인됐습니다.
삭제 전 snapshot/extended data 조회는 모두 NRC `12`로 거절되어 추가 기록을 얻지
못했습니다. ADAS 관련 7개 후보는 삭제 및 전후 조회 모두 timeout이므로 삭제 성공은
미확인입니다. ECU reset, 통신 disable 또는 actuator 명령은 보내지 않았고 Panda의
기존 `0/0`, power_save=1을 복원했습니다. 삭제 전 원본을 보존했고 새 실행 기록은
[dtc-clear-20261006.json](evidence/2026-10-06/dtc-clear-20261006.json)에 있습니다. 즉시 코드가 없더라도 관련 감시 조건이
충족되기 전에는 문제 해결로 판단하지 않습니다.

그 뒤 사용자가 퓨즈 교체로 문제가 해결됐다고 알렸습니다. 교체한 퓨즈의 위치/규격은
제공되지 않았습니다. 송신 없이 USB/CAN 수신만 확인한 최신 3초 관측(Panda uptime
242~245초)은 harness=1(NORMAL, 정방향), ignition_line=1, 전압 14.102 V였습니다.
ECAN RX가 8,636 증가했고 LFA `0x12A` 300개, SCC `0x1A0` 150개를 수신했습니다.
MDPS 300개, 조향각 299개, wheel speed 300개, TCS 150개도 CRC가 모두 정상이며
counter가 진행했습니다. 선택 프레임 CRC 오류와 관측 중 RX overflow 증가는 0,
모든 controller TX 증가는 0입니다. 최신 조향각은 0.3 deg, wheel speed는 모두 0,
MDPS warning/LKA fault/fail은 0, ACCMode는 0입니다. Panda raw faults=24는 기존
ECAN-only 제외 mask의 non-ECAN 두 비트이며 ECAN bus-off/error-warning/error-passive는
모두 0입니다. 이 관측은 stock CAN 수신 복귀를 확인한 결과이며 ECU 진단 접근 경로나
고장코드 소거를 재확인한 결과는 아닙니다. 원본은 [panda-post-fuse-20261006.json](evidence/2026-10-06/panda-post-fuse-20261006.json)에
있습니다. 이전 점화 미감지와 LFA/SCC 미수신 결과는 퓨즈 교체 전의 기록입니다.

이후 Ubuntu 차량 컴퓨터에서 ROS Noetic Release 빌드와 최신 host 67개 test를 통과했습니다.
사용자가 firmware 수정/flash를 승인하고 하네스를 분리한 뒤 I5R1 앱을 flash했습니다.
설치 서명/capability, USB-only preflight PASS 및 standby 3개 송신 시도 차단/물리 TX 증가 0을
확인했습니다. bootstub은 교체하지 않았습니다. [USB-only 기록](evidence/2026-10-06/command-session-usb-20261006.json)에
앱/후보/patch hash가 있습니다. 이후 정차 ROS 시험의 제한된 결과는 아래와 같습니다.
[검증 상태와 재현 방법](validation.md)에 정확한 범위가 있습니다.

## 2026-10-06 22:09~22:17 정차 ROS 시험

사용자 승인 임시 publisher로 20 Hz 조향각속도 입력과 700 ms idle 단절/복귀를 확인했습니다.
물리 OFF에서는 `ARMED → PASSIVE → ARMED`이며 actuator 출력은 없었습니다.
실행 직후 USB packet checksum 오류는 두 번의 시작에서 재현됐고, 자동 재연결 후
버튼 감시 profile이 0으로 남았습니다. 기존 재arm service로 NO_OUTPUT/7173 대기만 복원했습니다.

CAN으로 D/brake/정차/LDA lateral-only를 확인한 후 0 값으로 최초 인계를 시도했습니다.
`0x730/0x738`, `0x7D0/0x7D8`이 session 요청에 응답했고 순정 LFA/SCC quiet도 확인됐지만,
Hyundai mode 전환 뒤 Panda CAN 준비 검증이 500 ms 안에 통과하지 못했습니다.
ACTIVE, 비영점 실조향 및 ACTIVE 단절/복귀 단계는 실행되지 않았습니다. 어느 필수 RX 조건이
실패했는지는 추가 trace가 필요하며, RX 검사를 우회하거나 firmware를 수정하지 않았습니다.

camera는 stock 복구를 확인했고 radar는 첫 관측 timeout 뒤 자동 retry에서 stock 재개를
확인했습니다. NO_OUTPUT/복구 pending 해제 뒤 LFA 100 Hz/SCC 50 Hz와 TX 증가 0을
관측하고 시험 프로세스를 종료했습니다. 이후 읽기 전용 capture에서도 ACCEnable=3
통신 이상 신호가 남았습니다. **통신 복구가 차량 보조 기능 정상 복구를 의미하지 않습니다.**
DTC 조회/삭제, ECU reset, 차량 고장 해소와 ECU firmware identity 확인은 수행하지 않았습니다.
원본은 [정차 시험 기록](evidence/2026-10-06/stationary-ros-test-20261006.json)입니다.

## 2026-10-06 정차 실패 후 복구 수정

모드 변경 직후 첫 RX보다 1 Hz safety tick이 먼저 실행되면 I5R1 선택이 영구 취소되는
경쟁 조건을 실제 libsafety로 재현했습니다. 아직 수신하지 않은 메시지에만 최대 100 ms
초기 대기를 적용해 수정했고 필수 새 CAN/CRC/counter/freshness 및 TX gate는 유지했습니다.
원래 실차 실패 시 tick phase는 기록되지 않아 그 발생 건의 원인으로 확정하지는 않습니다.
관련 safety 14개와 host 18개 테스트, node/ARM 앱 빌드를 통과했습니다.

host USB 시작은 NO_OUTPUT에서 short-transfer 경계까지 이전 데이터를 비우며 실행 중
checksum 오류는 계속 차단합니다. 미arm 초기 오류의 빈 버튼 감시 복구와, radar/camera
두 통신을 모두 복구한 다음 순정 프레임을 확인하는 2단계 복구도 적용했습니다.
카메라 disable만으로 LFA와 SCC가 함께 quiet해진 독립 진단 trace가 근거입니다.

사용자가 다시 USB-only 상태를 확인하고 장치 harness/ignition0도 확인한 뒤 수정 앱을
플래시하고 설치 서명을 비교했습니다. 현재 앱 SHA-256은
`6c4e4b388642911a6329690d7d917feceda618fa84255f5fe13e1d288d092d52`입니다.
bootstub 변경 없이 preflight PASS, standby 3개 차단/물리 TX 0을 확인했습니다.
수정 후 차량 재연결에서 USB 오류 없는 수신/NO_OUTPUT 대기와 Panda ready를 확인했습니다.
그러나 ACCEnable3은 남았고, 별도 승인된 0x730 1회 DTC 삭제는 54 수락 후 약 2초 만에
동일 raw `588186`, `56b881`, `563881`이 status89로 재발했습니다. 추가 삭제/진단/제어를
중단했으며 제조사 고장 의미/ECU identity와 실제 추종은 미확인입니다.
[DTC 재발 기록](evidence/2026-10-06/post-fix-ecu-dtcs-20261006.json)을 함께 참고하십시오.
[복구 수정 기록](evidence/2026-10-06/ecan-recovery-fix-20261006.json)을 참고하십시오.

## 2026-08-21~24 실차 이력

2026-08-21 helper 시험에서 저속 LFA 송신과 좌우 목표각 추종, 약 1 km/h부터 직선 약
15 km/h까지 가속 1회를 확인했습니다. 이후 실행에서는 creep 부근에서 가속이 멈추거나
Panda SCC 거부가 발생했습니다. 최신 dual-ECU 수정 후 성공이나 자동 감속/완전 정지
성공 기록은 없습니다.

**2026-08-24 사용자 정정:** 실패 로그의 `brake=True`는 가속이 실패한 뒤 운전자가 개입한
결과입니다. 가속 실패의 확정 원인으로 해석하면 안 됩니다. P-CAN/gateway 작업은 미루고
ECAN `0x1A0` 문제를 우선하기로 했습니다.

당시 연결은 차량 camera connector → Hyundai K Y harness → harness board → Red Panda →
USB host였습니다. 문제 OBD-C 케이블 사용 시 USB/Comma Power 없이도 HVAC UI/LED/냉방이
먹통이 되었고 케이블 교체 후 회복했습니다. 이 결과는 firmware 원인으로 단정할 근거가
아닙니다. 정상 확인한 케이블과 harness 방향을 기록해야 합니다.

제조사가 이전 개조 gateway를 제거한 이력이 있고 그 인터페이스/일부 배선 상태는
불명확합니다. 과거 ADAS fuse 제거 및 재장착, Panda와 무관한 전방 안전/차로 변경 보조
경고도 기록되었습니다. 별도 PCAN을 사용한 이력은 동시 송신 재현 조건에 포함해야 합니다.
강한 MDPS 위치 유지와 native angle 인터페이스는 별도 미해결 요구사항입니다.

## SCC 소유권 수정의 근거와 한계

| ECU | UDS request/response | 순정 메시지 |
| --- | --- | --- |
| 현재 코드의 camera 소유권 대상, ECU 식별 미확인 | `0x730` / `0x738` | LFA `0x12A` |
| radar | `0x7D0` / `0x7D8` | SCC `0x1A0` |

Pinned opendbc의 Ioniq 5 firmware 기록은 전방 camera를 `0x7C4`, radar를 `0x7D0`으로
분류하며, `0x730`은 LKA steering 플랫폼의 ADAS Driving 후보입니다. 현재 코드의
`0x730` camera 소유권 가정과 이 분류는 일치하지 않습니다. 퓨즈 교체 전에는 `0x730`과
`0x7C4` 모두 진단 응답이 없었습니다. 이후 정차 시험에서 `0x730` 응답과 LFA quiet/복구를
관측했지만 ECU firmware identity는 확인하지 않았고 runtime 주소는 변경하지 않았습니다.
[Pinned firmware 기록](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/car/hyundai/fingerprints.py)과
[query 분류](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/car/hyundai/values.py)를 근거로 실차 식별을 먼저 확인해야 합니다.

이전 helper는 camera만 비활성화했습니다. radar stock SCC가 계속 송신될 수 있어 제어
SCC와 소유권 경합 위험이 있었습니다. 이를 차단하기 위해 helper와 ROS 노드 모두
radar disable/quiet/tester-present/restore를 추가하고 stock SCC의 미소유 비트를 보존합니다.
이 수정이 간헐적 가속 실패의 모든 원인을 해결했는지는 실차에서 검증되지 않았습니다.
HDA2 auxiliary `0x51/0x1EA/0x200/0x345/0x1DA`는 최종 HDA1 송신 경로에서 제외했습니다.

## Firmware provenance

runtime/build pins는 Panda `dd8a5b3df77706337a11555377e7180c5adc8726`,
opendbc `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`입니다.
별도 Carrot 비교 commit은 [upstream 근거](upstream.md)에 있으며 runtime pin 변경이 아닙니다.

2026-08-21 기록:

```text
last recorded flashed signed app:
1ef6d981457e8b2018fcbf72b8a43f88ba85f95a6b94800b177e8302df212bae
split-brake signed candidate (built, flash not recorded):
ffe0428536394ee7743a37f10338b4a19668f343f742e1b5de25c43df43f6986
bootstub candidate (built, not flashed in that record):
b420ab8d7d10d85a4b6883e37f6e4cbf38b724776afde823e7a9f4d5e1463a08
split patch:
e69f5a71f2a53ada43a38c1aefa759855b9915659d5ecb28682ad2311c910d54
```

이 hash를 현재 설치 image의 hash로 취급하지 않습니다. 재현 build, 앱 flash, DFU recovery와
ABI 검사 명령은 [Panda firmware](panda_firmware.md)에 있습니다.

## 다음 확인과 이전 helper 예제

ROS Release 빌드와 I5R1 설치 서명/capability 검증은 완료했습니다. 다음 실제 확인은
차량 재연결 후 read-only/passive fingerprint, ECU 소유권 endpoint, 순정 통신 복구와
주행 중 입력 복귀, 채널 동작 및 일시 EPS/hard fault 복구입니다.
한 기록에 commit/local diff/YAML/설치 firmware provenance, CAN address/bus/counter/time,
gear/brake/pedal/button, Panda health와 diagnostics, disable/restore 로그 및 다른 송신기
연결 상태를 남깁니다. 정차와 low-speed 조건, 버튼 조작은 현재 코드 및 시험계획을 따릅니다.

아래는 이전 `scripts/steering_sweep.py`의 재현 예제이며 ROS 노드 명령이 아닙니다.
helper에는 최신 ROS의 3초 EPS 복귀 및 지속 ECU retry 상태기계가 없습니다. `--execute`는
실제 CAN 송신을 수행합니다. helper 종료 시 restore warning은 별도로 처리해야 합니다.
Panda를 점유하는 다른 프로그램과 동시에 실행하지 않습니다.

```bash
# PANDA_SERIAL에는 실제 연결한 장치의 일련번호를 로컬에서 입력합니다.
read -r -p 'Panda serial: ' PANDA_SERIAL
# 이전 직선 가속/감속 helper
python3 scripts/steering_sweep.py \
  --serial "$PANDA_SERIAL" \
  --target-speed-kph 15 --accel-max-mps2 0.7 --decel-max-mps2 0.7 \
  --rolling-test --rolling-min-kph 1 --rolling-max-kph 16 \
  --arm-timeout-s 60 --execute

# 이전 저속 좌우 목표각 추종, LDA
python3 scripts/steering_sweep.py \
  --serial "$PANDA_SERIAL" \
  --offset-deg 30 --timed-hold-s 3 --steering-cycles 1 \
  --rolling-test --rolling-min-kph 1 --rolling-max-kph 10 \
  --arm-timeout-s 60 --execute

# 이전 combined 반복, SET. 반복 종료는 제어 해제이며 자동 정지 아님
python3 scripts/steering_sweep.py \
  --serial "$PANDA_SERIAL" \
  --target-speed-kph 15 --accel-max-mps2 0.7 \
  --offset-deg 15 --combined-cycles 3 --combined-segment-s 2 \
  --rolling-test --rolling-min-kph 1 --rolling-max-kph 16 \
  --arm-timeout-s 60 --execute

# 이전 정차 torque count 시험, 실제 MDPS 최대 구동력의 증명 아님
python3 scripts/steering_sweep.py \
  --serial "$PANDA_SERIAL" \
  --torque-sweep --timed-hold-s 3 --steering-cycles 3 \
  --arm-timeout-s 60 --execute
```

종방향 helper 제어 전에는 `CAMERA_DISABLED ... stock_0x12A=quiet`와
`RADAR_DISABLED ... stock_0x1A0=quiet` 둘 다 확인해야 합니다. helper 자체 출력 안내와
시험계획을 따르며, 이 예제를 최신 ROS 복귀 검증 완료 기록으로 취급하지 않습니다.
