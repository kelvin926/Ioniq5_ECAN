# ROS 입력 계약

2026-10-06 사용자와 합의한 입력은 **목표 핸들 조향각속도(deg/s)**와
**목표 종방향 가속도(m/s²)**입니다. 동료가 별도 상위 제어기를 만들어 이 형태로 출력합니다.
모델 action, 목표 조향각, 앞바퀴 각속도, 차량 yaw rate 또는 페달 비율을 직접 보내지 않습니다.
기본 연구장 YAML의 `input.lateral_mode=steering_rate_deg_s`, scale=1, offset=0을 유지합니다.

명령 토픽은 `/ioniq5/actuation_command`이며 메시지는
[`ActuationCommand.msg`](../msg/ActuationCommand.msg)입니다.

| 필드 | 현재 의미 |
| --- | --- |
| `stamp` | source timestamp, 지연 분석용. watchdog에는 사용하지 않음 |
| `sequence` | 선택적 순서 검사. 0은 검사 생략, wraparound 및 timeout 후 publisher 재시작 허용 |
| `enable` | `input.use_enable_field=true`일 때 매 메시지 deadman |
| `lateral` | 합의된 활성 프로파일에서 목표 핸들 조향각속도, deg/s |
| `acceleration` | 목표 종방향 가속도, m/s², 양수 가속 / 음수 감속 |

기본 `use_enable_field=false`에서는 `lateral`과 `acceleration` 두 값만 필요합니다.
유한한 최신 값을 계속 보내야 하며, 기본 command watchdog은 호스트 수신 시각 기준
250 ms입니다. source timestamp와 호스트 steady clock은 별도 clock domain입니다.

송신 권장은 20 Hz, non-latched publisher입니다. 같은 메시지에 두 값을 함께 넣습니다.
`stamp`는 현재 수신 유효성 검사에 쓰이지 않으므로 상위 출력 노드가 오래된 모델 계획이나
명령을 새 값처럼 반복 발행하지 않아야 합니다. `sequence=0`도 허용하지만 순서 번호를
증가시켜 보내는 것을 권장합니다. 조향 부호는 차량의 실제 `steering_angle_deg` 증가 방향과
대조하여 검증해야 합니다. 가속도 허용 범위는 -3.5~2.0 m/s²입니다.
2026-10-07 bag 대조에서는 조향각 피드백의 좌회전 양수 해석을 지지했습니다.
상위 `lateral` 명령과 LFA 토크 요청의 실제 좌우 방향은 별도 실차 통합 확인이 필요합니다.

## 상시 구독과 차량 버튼

차량 컴퓨터에서는 `/home/ave/catkin_ws_ioniq5/ecan` 한 줄로 시작합니다.
노드는 publisher가 없거나 Panda가 연결되지 않아도 명령 토픽 구독을 유지합니다.
버튼은 hold-to-run이 아닌 ON/OFF 토글입니다.

| 입력 / 버튼 | 동작 |
| --- | --- |
| 최신 명령 없음 | 순정 ECU 통신을 유지하는 무출력 대기 |
| 최신 명령 있음, 버튼 선택 없음 | 초기 순정 통신 유지, actuator 출력 없음 |
| LDA 한 번 누름 | 조향 전용 ON, 다시 누르면 OFF |
| SET 누르고 놓음 | 조향+종방향 ON, 다시 누르고 놓으면 OFF |

버튼 OFF 중에도 구독은 유지하고 조향 목표각을 실측각으로 초기화합니다.
OFF 중 수신된 조향각속도를 미리 적분하거나 다음 ON에 과거 목표를 이어 붙이지 않습니다.
최신 값이 계속 있으면 OFF 중에도 ECU 소유권 및 비활성 CAN 프레임은 유지되며,
순정 ADAS 통신 복귀와는 구분됩니다. 기본 I5R1 프로파일에서 값이 250 ms 끊기면
소유권을 해제하고 순정 ECU 통신 복구 후 대기로 돌아가되 버튼 ON/OFF 선택을 유지합니다.
입력만 끊긴 이전 ACTIVE 세션은 새 값과 정상 CAN, 복구 완료를 확인하여 주행 중 재인계할
수 있습니다. 대기 중 OFF, 브레이크의 종방향 latch-off, CANCEL과 CAN/USB 고장을 무시하지 않습니다.
이전 목표 조향각과 적분 상태는 버립니다. 첫 takeover 및 고장 후 재arm은 여전히 정차가 필요합니다.
복구 실패 시 구독은 계속하지만 제어는 금지합니다. 정차 시험에서 ECU 통신 복구는 관측했으나
ACTIVE 추종/주행 중 재인계와 차량 보조 기능 정상 복구는 확인되지 않았습니다. [검증 기록](validation.md)을 참고하십시오.

`lateral=0`은 누적 목표 조향각 유지이며 중앙 복귀가 아닙니다.
두 필드가 모두 0이어도 유효한 명령입니다. 값이 없다는 뜻은 토픽의 최신 수신이 없다는 뜻입니다.

## 횡방향 모드

| `input.lateral_mode` | 입력 단위 | 차량 출력까지의 변환 |
| --- | --- | --- |
| `steering_rate_deg_s` (기본) | deg/s | rate 적분 → 목표 조향각 → 실측각 피드백 → 토크 |
| `steering_rate_rad_s` | rad/s | deg/s로 변환한 뒤 동일 경로 |
| `curvature_1pm` | 1/m | wheelbase/steering ratio로 목표각 변환 → 동일 토크 제어기 |
| `direct_torque` | Panda torque count | torque count 제한 및 변화율 적용 |

ROS 노드에는 직접 조향각 입력 모드가 없습니다. 목표각을 토크로 추종하는 방식과
MDPS에 native angle 명령을 보내는 방식은 다릅니다. 이 HDA1 차량의 native angle 수용 여부는
확인되지 않았습니다. `direct_torque` 단위는 Nm가 아니며, 270 count는 명령 경로의 상한입니다.

rate/curvature 경로는 Carrotpilot Ioniq 5 횡가속도 및 마찰 보상 토크 제어기를 사용합니다.
정확한 부호와 scale은 실제 차량 상태 및 입력 정의와 대조해야 합니다.

## 값 전달과 남는 변환

scale/offset은 `scaled = input * scale + offset`입니다. 기본 scale은 1, offset은 0입니다.
`input.unfiltered_input=true`에서는 host의 rate/목표각 clamp와 입력 평활화를 생략합니다.
가속도는 host clamp 또는 jerk slew 없이 다음 50 Hz SCC에 반영하며, CAN에서 0.01 m/s²
단위로 양자화됩니다. scale/offset 적용 후 `-3.5 .. 2.0 m/s²` 범위를 벗어나면 조용히
잘라내지 않고 오류와 disarm으로 처리합니다.

`unfiltered_input=false`에서는 설정한 rate/목표각 및 가속도 범위로 clamp합니다.
`longitudinal.jerk_limit_mps3`는 SCC `JerkLowerLimit` metadata이며 host 가속도 slew 기능이
아닙니다. 두 모드 모두 토크 변환, driver torque 제한, 270 count 상한, 100 Hz에서
2/3 count 증가/감소, CAN 양자화, 채널 허가와 watchdog은 남습니다.

## 상태와 피드백

`/ioniq5/vehicle_state`는 20 Hz로 발행하며 `stamp`는 ROS 발행 시각입니다.
현재 필드별 의미/단위와 parser 동작은 [피드백 필드 표](vehicle_state.md)를 참고합니다.
[`VehicleState.msg`](../msg/VehicleState.msg)의 `valid`는 필수 steering/MDPS/wheel/TCS
CAN의 크기, CRC와 freshness를 뜻합니다. `eps_fault`는 별도 MDPS LKA 보조 상태입니다.
IMU/페달/기어/SCC/버튼의 개별 freshness는 포함하지 않습니다.
`lateral_armed`/`longitudinal_armed`는 채널 선택이며 `*_control_active`는 현재 출력 허가입니다.
Panda `controls_allowed`는 firmware 전역 허가이므로 각 채널의 active 필드와 함께 봅니다.

| `control_state` | 이름 |
| --- | --- |
| 0 | `DISCONNECTED` |
| 1 | `PASSIVE` |
| 2 | `ARMED` |
| 3 | `ACTIVE` |
| 4 | `FAULT` |
| 5 | `SOFT_DISABLING` |

일시 EPS 오류의 복귀 창과 ECU 복구 진행은 `/diagnostics`에 게시합니다.
요청값, 적용값, 포화 및 거부를 묶은 상위 제어기 전용 피드백 메시지는 아직 구현하지
않았습니다. CAN 송신이나 software active 상태 자체가 ECU 실행 응답 또는 실제 구동력
확인을 의미하지는 않습니다.

[오프라인 ECAN 분석](ecan_analysis_20261007.md)의 새 필드와 동시 기록 GPS/레이더 참조값은
현재 `VehicleState`에 추가되지 않았습니다. 상위 제어기는 현재 메시지 계약을 사용합니다.

물리량/단위/메시지는 위 계약을 사용합니다. 좌우 부호와 차량 추종 결과, 가속도 경사 보상,
실제 송신 주기와 jitter, source clock domain, 상위 deadman 소유 주체는 통합 시 검증해야 합니다.
설정으로 수용할 수 없는 변경은 메시지와 adapter 경계에서 반영합니다.
