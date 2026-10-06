# Safety model

2026-10-06 현재 구현된 supervisor와 고정 Panda hook의 동작입니다. 기본 입력은 host
평활화를 생략하지만 아래의 채널 허가, 프로토콜 제한과 watchdog은 유지됩니다.

| 상태 | Panda relay/safety | 명령 송신 |
| --- | --- | --- |
| `DISCONNECTED` | 연결/health 없음 | 없음 |
| `PASSIVE` | `NO_OUTPUT`, 순정 camera 회로 물리 연결 | 없음 |
| `ARMED` | takeover 전에는 `NO_OUTPUT`, 완료 후 `HYUNDAI_CANFD`/순정 송신 차단 | takeover 전 없음, 완료 후 비활성 LFA/SCC를 정주기로 대체 |
| `ACTIVE` | 동일 safety hook, `controls_allowed` 필요 | arm된 채널의 bounded LFA 및/또는 SCC |
| `SOFT_DISABLING` | safety mode와 ECU 소유권 유지, 3초 복귀 대기 | 비활성 LFA, 허가된 정상 종방향 SCC 유지 |
| `FAULT` | 즉시 arm 해제, 순정 ECU 복구와 `NO_OUTPUT` 확인 | actuator 명령 없음, 복구 진단 통신 가능 |

ARMED에서 비활성 프레임을 계속 보내는 이유는 UDS로 순정 송신기를 정지시킨 뒤 호스트가
메시지 소유권을 인계받기 때문입니다. 프레임이 끊기면 MDPS/ACC가 timeout을 낼 수 있습니다.

## 고정된 Panda 제한

- safety model `HYUNDAI_CANFD = 28`
- Ioniq 5 HDA1 EV + split-button + ECAN-only lateral param `1 | 1024 | 2048 = 3073`
- longitudinal param `1 | 4 | 1024 | 2048 = 3077` (DEBUG firmware만 LONG 적용)
- I5R1 command-session opt-in은 `4096` 추가: lateral `7169`, combined `7173`
- alt-buttons 차량은 위 param에 `32` 추가
- steering max 270 count
- steering rate up/down 2/3 count per 10 ms
- acceleration `-3.5 .. 2.0 m/s²`
- `SAFETY_ALLOUTPUT` 금지

YAML은 Panda 경계 안에서 최대 토크/변화율/가속도와 선택적인 속도/조향각 상한을
조정합니다. 속도/조향각 상한은 `0`으로 비활성화할 수 있지만 생성 함수와 Panda
firmware의 hard limit는 항상 남습니다.
270 count는 명령 경로의 상한이며 실제 MDPS 최대 구동력이나 native angle 수용 여부를
증명하지 않습니다.

85도 이상에서 EPS fault를 피하는 Carrot 방식도 적용했습니다. 기본값은 조향 request를
89 frame 유지한 뒤 2 frame 동안 토크 값은 유지하고 `STEER_REQ`만 내립니다. cutoff
angle을 `0`으로 설정하면 이 host 동작도 비활성화할 수 있습니다.
이는 고각 request 반복으로 fault를 예방하는 로직이며, 이미 발생한 MDPS 보조 오류의
3초 복귀 창과는 별도입니다.

## 채널 해제 및 fault 조건

- brake: 횡방향 유지, 종방향만 래치 해제; release만으로 재개하지 않고 다음 SET 필요
- ACC fault 또는 SCC/FCA TX rejection: 종방향만 래치 해제
- LFA/알 수 없는 TX rejection: 전체 FAULT. 단, 같은 버튼 OFF 처리 주기의 알려진
  actuator frame(0x12A/0x1A0/0x160) rejection은 양 채널 OFF를 유지하는 정상 해제로 처리
- `disengage_on_cancel`이 true일 때 host CANCEL 전체 해제; I5R1 firmware CANCEL은 host 설정과 독립적으로 세션 취소
- I5R1 프로파일의 정상 command timeout: 무출력/순정 복구 대기, 버튼 선택 유지
- I5R1 비활성 프로파일 또는 EPS soft-disable 중 command timeout: 기존 FAULT/재승인
- Panda health timeout/USB disconnect
- critical vehicle CAN timeout 또는 checksum/counter에 따른 Panda RX invalid
- bus-off/error-passive
- 일시 EPS 보조 오류의 3초 복귀 창 만료
- safety mode/param drift
- 하네스 방향이 정확히 `harness_status=1`이 아님
- 활성화한 경우에만 설정 속도/조향각 상한 초과

MDPS LKA 보조 오류는 Carrotpilot의 soft-disable 방식으로 처리합니다. CAN 수신 유효성과
EPS 보조 오류를 별도로 추적합니다. `ACTIVE` 중 오류가 발생하면 `SOFT_DISABLING`으로
전환해 LFA 토크와 request를 비활성화하고, 채널 선택과 heartbeat 및 ECU 세션을 유지합니다.
정상 종방향은 기존 명령을 계속 처리합니다. 최초 오류 시점부터 3초 안에 오류가 해소되고
최신 명령, 정상 CAN, Panda `controls_allowed`가 모두 유효하면 자동으로 `ACTIVE`에 복귀합니다.
Panda 허가를 강제로 설정하지 않으며, 첫 활성화는 EPS 오류가 있는 동안 차단합니다.
3초가 만료된 시점에 뒤늦게 정상 샘플이 들어와도 `FAULT`가 유지되며 재승인이 필요합니다.
브레이크, ACC 오류, CANCEL, 수동 disarm, 명령 단절 및 Panda/CAN 고장 처리가 우선합니다.
특히 브레이크로 래치 해제된 종방향은 EPS 복귀로 재활성화하지 않습니다. 일시 정지 중
조향 제어기 적분과 목표각을 현재 실측값으로 초기화하고 raw LFA 송신도 차단합니다.
동일 오류가 반복되어도 최초 deadline을 연장하지 않습니다. `VehicleState.valid`는 필수
CAN의 크기/CRC/freshness이며 `eps_fault`와 독립입니다. CRC 불량 clear frame은 오류를
해소하지 않고 critical CAN freshness 상실은 hard fault입니다. host parser의 이 검사는
Panda RX counter 검사와 구분합니다.
상태 이름은 `SOFT_DISABLING`, 수치 값은 `5`이며 기존 `0..4` 값은 유지됩니다.
`/diagnostics`에 EPS 오류와 `soft_disable_remaining_ms`를 게시합니다.

`1024`는 이 저장소의 opt-in split-button 확장입니다. 기존 프로파일은 firmware와 host가
각각 버튼 edge를 추적합니다. I5R1은 firmware의 물리 선택이 authoritative하며 host는
그 상태와 종방향 brake/ACC latch를 사용합니다. LDA 상승 에지는 조향 전용 모드를, SET release는
조향+종방향 모드를 토글합니다. brake는 Panda의 횡방향 `controls_allowed`를 유지한 채
종방향 허가만 지웁니다. upstream 그대로의 firmware는 이 채널 분리를 모르므로 요구한
브레이크 동작을 보장하지 않습니다.

`2048`은 ECAN-only 확장입니다. 물리 CAN1만 켜 두고 논리 bus 0으로 사용하며, CAN0↔CAN2
forwarding을 모두 차단합니다. host는 의도적으로 꺼진 physical CAN2/CAN3의 interrupt-rate
fault bit만 무시하고, ECAN physical CAN1과 나머지 Panda fault는 계속 FAULT로 처리합니다.

초기 takeover는 정차와 EPS 정상/유효 CAN을 요구합니다. 종방향을 켠 프로파일은 D와 최근
순정 SCC template도 요구합니다. 기본 I5R1 프로파일에서는 이 조건과 ECU quiet 확인,
최신 명령 및 물리 ON 선택이 함께 있어야 첫 takeover를 수행합니다. 물리 LDA
버튼은 조향 전용 모드를, `SET` release는 조향+종방향과 raw TX를 ON/OFF 토글합니다.
LDA를 누르면 종방향은 항상 꺼지고, SET을 누르면 조향이 항상 함께 켜집니다.
선택 ON/OFF와 Panda 전역 `controls_allowed`는 구분되며 health 도착 순서로 허가를 새로 만들지 않습니다.
`/ioniq5/vehicle_state`의 채널별 arm/active 필드로 구분합니다.
OFF에서도 순정 차단 구간의 timeout을 막기 위한 비활성 LFA/SCC 프레임은 유지됩니다.
최신 command가 없는 초기 상태는 순정 통신/NO_OUTPUT 대기이며, 준비/OFF 중 command
단절은 순정 ECU 통신 복구 후 대기로 돌아갑니다. I5R1은 정상 ACTIVE의 입력만 끊겼을 때
버튼 선택을 보존하고 복구 완료 후 새 입력으로 주행 중 재인계하는 예외를 제공합니다.
NO_OUTPUT/ELM327에서 물리 버튼과 brake를 계속 검증하지만 actuator TX는 허용하지 않습니다.
모드 전환 직후에는 새 CRC/counter/freshness-valid CAN이 모두 들어오기 전까지 출력 금지입니다.
초기 100 ms의 미수신 메시지는 안전 tick이 세션을 조기에 취소하지 않도록 대기하지만
출력 허가를 만들지 않습니다. 이미 수신된 불량 CAN과 이후 미수신은 계속 차단합니다.
OFF, CANCEL, CAN/USB/시동/하네스 고장, 복구 실패는 자동 재인계 대상이 아닙니다.
이 예외는 프로세스 재부팅 후 무승인 자동 제어가 아니며 실차 재인계 성능은 별도 검증 대상입니다.

통합 모드에서 brake가 들어오면 host와 Panda가 각각 종방향 arm을 false로 래치합니다.
host는 바로 `aReqRaw=0`, `aReqValue=0`, `ACCMode=0`을 송신하며 조향 LFA는 계속 보냅니다.
브레이크 입력과 한 주기 겹친 active SCC가 Panda에서 거부되더라도 마지막 rejected address가
`0x1A0`/`0x160`이면 종방향 채널만 해제하고 횡방향은 유지합니다.

FAULT 후 재arm하려면 먼저 `set_armed=false`를 호출해 fault를 명시적으로 acknowledge한
뒤 `true`와 필요한 물리 LDA/SET 절차를 다시 수행합니다. 기본 자동 arm은 arm 요청을 줄여 주지만,
latched FAULT를 자동으로 지우지는 않습니다. 연구장 기본 YAML은
`disengage_on_cancel=false`, `longitudinal_override_on_gas=false`이며 brake의 종방향 전용
해제는 켜져 있습니다. passive YAML은 CANCEL/gas 옵션이 true지만 actuation 자체가 꺼져
있습니다. Panda hook의 별도 검사는 host 옵션과 구분합니다.
서비스로 `set_armed=false`를 요청하면 자동 arm도 억제되고, 명시적인 `true` 요청으로
다시 허용됩니다.

## 종방향 경고

HDA1 ECAN-only longitudinal 모드는 ECAN의 `SCC_CONTROL` 및 관련 FCA 메시지를 이 노드가
대체하며 camera bus는 전달하지 않습니다. 순정 AEB 기능이 유지된다고 가정할 수 없습니다.
실차 시험 helper와 ROS C++ 노드는 종방향 시작 전에 camera `0x730`/순정 LFA `0x12A`와 radar `0x7D0`/순정
SCC `0x1A0`을 각각 비활성화하고, 두 순정 메시지가 모두 멈추지 않으면 명령 송신 전에
중단합니다. 제어 중 두 ECU에 tester-present를 0.8초 이내 주기로 보내고, 해제/예외/종료 시
radar→camera 순서로 두 통신을 모두 복구한 뒤 순정 SCC/LFA 재개를 확인하고 `NO_OUTPUT`으로 돌아갑니다. 복구 응답 뒤 순정 메시지가
재개되지 않으면 ROS 노드는 실행 중 복구 대기를 유지하고 1, 2, 4, 8, 16, 최대 30초
간격으로 재시도합니다. 이전 Python helper에는 이 지속 재시도 상태기계가 없습니다.
재시도에서는 이미 순정 메시지가 돌아왔는지 먼저 확인하여 응답 유실을 복구 실패와
구분하고, 필요할 때 진단 세션을 다시 연 뒤 통신을 복구합니다. 두 ECU의 순정 메시지
재개와 Panda `NO_OUTPUT` 확인 전에는 재arm하지 않습니다. USB 단절 중에는 재연결을
기다리고, 복구 후에도 운전자 재승인과 기존 정차 조건을 거쳐야 제어를 재개합니다.
이는 ECU reset이나 장애 중 명령 유지가 아닙니다. ECU 자체 고장은 복구를 보장하지 않으며,
`/diagnostics`에 복구 대기, 시도 횟수, 재승인 필요 상태를 게시합니다. HDA2 ADRV 메시지는 HDA1
ECAN-only 송신 목록에 포함하지 않습니다.
기본 연구장 프로파일은 요청에 따라 종방향이 켜져 있으므로 고정 DEBUG firmware가
필수입니다. 수동 관찰이나 lateral-only 시험에서는 YAML에서 명시적으로 끄십시오.
