# Safety model

이 프로젝트는 소프트웨어 supervisor와 Panda firmware safety hook을 겹쳐 사용합니다.
어느 한쪽만으로 충분하다고 가정하지 않습니다.

| 상태 | Panda relay/safety | 명령 송신 |
| --- | --- | --- |
| `DISCONNECTED` | 연결/health 없음 | 없음 |
| `PASSIVE` | `NO_OUTPUT`, 순정 camera 회로 물리 연결 | 없음 |
| `ARMED` | `HYUNDAI_CANFD`, camera 프레임 선택 차단/forward | 비활성 LFA와 선택적 비활성 SCC를 정주기로 대체 |
| `ACTIVE` | 동일 safety hook, `controls_allowed` 필요 | bounded LFA/SCC |
| `FAULT` | 즉시 arm 해제 후 `NO_OUTPUT` 복귀 | 없음 |

ARMED에서 비활성 프레임을 계속 보내는 이유는 relay가 순정 LFA를 차단하기 때문입니다.
프레임이 끊기면 MDPS가 timeout/fault를 낼 수 있습니다.

## 고정된 Panda 제한

- safety model `HYUNDAI_CANFD = 28`
- Ioniq 5 HDA1 EV + camera SCC lateral param `1 | 8 = 9`
- longitudinal param `1 | 4 | 8 = 13` (DEBUG firmware만 LONG 적용)
- alt-buttons 차량은 위 param에 `32` 추가
- steering max 270 count
- steering rate up/down 2/3 count per 10 ms
- acceleration `-3.5 .. 2.0 m/s²`
- `SAFETY_ALLOUTPUT` 금지

YAML은 이보다 강한 소프트웨어 제한만 허용합니다. 소스의 생성 함수도 Panda hard
limit로 다시 clamp합니다.

## 해제 및 fault 조건

- brake 또는 CANCEL
- command timeout
- Panda health timeout/USB disconnect
- critical vehicle CAN timeout 또는 checksum/counter에 따른 Panda RX invalid
- bus-off/error-passive
- EPS/ACC fault
- Panda TX rejection 증가
- safety mode/param drift
- 하네스 미검출
- 설정 속도/조향각 한계 초과

FAULT 후 재arm하려면 먼저 `set_armed=false`를 호출해 fault를 명시적으로 acknowledge한
뒤 `true`와 물리 SET/RES 절차를 다시 수행합니다.

## 종방향 경고

HDA1 camera-SCC longitudinal 모드는 camera의 `SCC_CONTROL` 및 관련 FCA 메시지를
차단하고 이 노드가 대체합니다. 순정 AEB 기능이 유지된다고 가정할 수 없습니다.
종방향 기본값이 false인 이유입니다. 안전 운전자, 외부 비상정지, 저속 제한, 충분한
run-off 공간 없이 활성화하지 마십시오.
