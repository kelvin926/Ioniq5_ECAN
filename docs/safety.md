# Safety model

이 프로젝트는 소프트웨어 supervisor와 Panda firmware safety hook을 겹쳐 사용합니다.
어느 한쪽만으로 충분하다고 가정하지 않습니다.

| 상태 | Panda relay/safety | 명령 송신 |
| --- | --- | --- |
| `DISCONNECTED` | 연결/health 없음 | 없음 |
| `PASSIVE` | `NO_OUTPUT`, 순정 camera 회로 물리 연결 | 없음 |
| `ARMED` | `HYUNDAI_CANFD`, camera 프레임 선택 차단/forward | 비활성 LFA와 선택적 비활성 SCC를 정주기로 대체 |
| `ACTIVE` | 동일 safety hook, `controls_allowed` 필요 | arm된 채널의 bounded LFA 및/또는 SCC |
| `FAULT` | 즉시 arm 해제 후 `NO_OUTPUT` 복귀 | 없음 |

ARMED에서 비활성 프레임을 계속 보내는 이유는 relay가 순정 LFA를 차단하기 때문입니다.
프레임이 끊기면 MDPS가 timeout/fault를 낼 수 있습니다.

## 고정된 Panda 제한

- safety model `HYUNDAI_CANFD = 28`
- Ioniq 5 HDA1 EV + camera SCC lateral param `1 | 8 | 1024 = 1033`
- longitudinal param `1 | 4 | 8 | 1024 = 1037` (DEBUG firmware만 LONG 적용)
- alt-buttons 차량은 위 param에 `32` 추가
- steering max 270 count
- steering rate up/down 2/3 count per 10 ms
- acceleration `-3.5 .. 2.0 m/s²`
- `SAFETY_ALLOUTPUT` 금지

YAML은 Panda 경계 안에서 최대 토크/변화율/가속도와 선택적인 속도/조향각 상한을
조정합니다. 속도/조향각 상한은 `0`으로 비활성화할 수 있지만 생성 함수와 Panda
firmware의 hard limit는 항상 남습니다.

85도 이상에서 EPS fault를 피하는 Carrot 방식도 적용했습니다. 기본값은 조향 request를
89 frame 유지한 뒤 2 frame 동안 토크 값은 유지하고 `STEER_REQ`만 내립니다. cutoff
angle을 `0`으로 설정하면 이 host 동작도 비활성화할 수 있습니다.

## 해제 및 fault 조건

- `disengage_on_brake`/`disengage_on_cancel`이 true일 때 brake 또는 CANCEL
- command timeout
- Panda health timeout/USB disconnect
- critical vehicle CAN timeout 또는 checksum/counter에 따른 Panda RX invalid
- bus-off/error-passive
- EPS/ACC fault
- Panda TX rejection 증가
- safety mode/param drift
- 하네스 미검출
- 활성화한 경우에만 설정 속도/조향각 상한 초과

`1024`는 이 저장소의 opt-in split-button 확장입니다. 차선유지(LDA) 버튼의 상승 에지가
Panda의 전역 `controls_allowed`를 허용하게 하고, host가 LDA 조향 전용 모드와 SET
조향+종방향 모드를 게이트합니다. upstream 그대로의 firmware는 이 비트를 모르므로 LDA 단독 조향 arm이
동작하지 않습니다.

정상 상태에서는 첫 최신 명령이 Panda를 HYUNDAI_CANFD 대기 상태로 만듭니다. 물리 LDA
버튼은 조향 전용 모드를, `SET` release는 조향+종방향과 raw TX를 ON/OFF 토글합니다.
LDA를 누르면 종방향은 항상 꺼지고, SET을 누르면 조향이 항상 함께 켜집니다.
`/ioniq5/vehicle_state`의 채널별 arm/active 필드로 구분합니다.
OFF에서도 순정 차단 구간의 timeout을 막기 위한 비활성 LFA/SCC 프레임은 유지됩니다.

FAULT 후 재arm하려면 먼저 `set_armed=false`를 호출해 fault를 명시적으로 acknowledge한
뒤 `true`와 필요한 물리 LDA/SET 절차를 다시 수행합니다. 기본 자동 arm은 arm 요청을 줄여 주지만,
latched FAULT를 자동으로 지우지는 않습니다. 연구장 기본 YAML에서는 host의 brake/CANCEL
자동 해제를 끄며, Panda firmware가 자체적으로 강제하는 controls 허용 조건은 그대로입니다.
서비스로 `set_armed=false`를 요청하면 자동 arm도 억제되고, 명시적인 `true` 요청으로
다시 허용됩니다.

## 종방향 경고

HDA1 camera-SCC longitudinal 모드는 camera의 `SCC_CONTROL` 및 관련 FCA 메시지를
차단하고 이 노드가 대체합니다. 순정 AEB 기능이 유지된다고 가정할 수 없습니다.
기본 연구장 프로파일은 요청에 따라 종방향이 켜져 있으므로 고정 DEBUG firmware가
필수입니다. 수동 관찰이나 lateral-only 시험에서는 YAML에서 명시적으로 끄십시오.
