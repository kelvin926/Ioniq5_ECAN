# ROS 입력 계약

2026-10-08 사용자 결정에 따라 입력은 **LFA 조향 토크 요청(Panda count)**과
**목표 종방향 가속도(m/s²)**입니다. 상위 제어기(알파마요 기반)가 조향 토크를 직접 계산합니다.
모델 action, 목표 조향각, 조향각속도, 차량 yaw rate 또는 페달 비율을 보내지 않습니다.
기본 연구장 YAML은 `input.lateral_mode=direct_torque`, scale=1, offset=0입니다.

명령 토픽은 `/ioniq5/actuation_command`이며 메시지는
[`ActuationCommand.msg`](../msg/ActuationCommand.msg)입니다.

| 필드 | 현재 의미 |
| --- | --- |
| `stamp` | source timestamp, 지연 분석용. watchdog에는 사용하지 않음 |
| `sequence` | 선택적 순서 검사. 0은 검사 생략, wraparound 및 timeout 후 publisher 재시작 허용 |
| `enable` | `input.use_enable_field=true`일 때 매 메시지 deadman |
| `lateral` | 활성 프로파일에서 LFA `StrTqReqVal` 토크 요청, Panda count |
| `acceleration` | 목표 종방향 가속도, m/s², 양수 가속 / 음수 감속 |

기본 `use_enable_field=false`에서는 `lateral`과 `acceleration` 두 값만 필요합니다.
유한한 최신 값을 계속 보내야 하며, 기본 command watchdog은 호스트 수신 시각 기준
250 ms입니다. source timestamp와 호스트 steady clock은 별도 clock domain입니다.

송신 권장은 non-latched publisher입니다. 같은 메시지에 두 값을 함께 넣습니다. 노드는 100 Hz마다
가장 최근 값을 보내므로(그 사이에는 같은 값 유지), 토크 제어에는 50~100 Hz 발행을 권장합니다.
`stamp`는 현재 수신 유효성 검사에 쓰이지 않으므로 상위 출력 노드가 오래된 모델 계획이나
명령을 새 값처럼 반복 발행하지 않아야 합니다. `sequence=0`도 허용하지만 순서 번호를
증가시켜 보내는 것을 권장합니다. 조향 부호는 차량의 실제 `steering_angle_deg` 증가 방향과
대조하여 검증해야 합니다. 가속도는 CAN 표현 범위 -10.23~10.24 m/s²로 전달합니다.
2026-10-07 bag 대조에서는 조향각 피드백의 좌회전 양수 해석을 지지했습니다.
DBC 주석은 음수 토크를 핸들 시계방향(우회전) 가속 토크로 설명하지만, 이 차량에서 토크 부호와
조향각 증가 방향의 관계는 실차 통합 확인이 필요합니다.

## direct_torque 입력

- `lateral`은 정수로 반올림해 `StrTqReqVal`에 그대로 넣습니다. ±1021 밖의 값은 끝값으로 맞춥니다.
  1021은 DBC Reserved/Invalid 원시값(+1022/+1023)을 피하는 대칭 최대값입니다.
- 1 count는 DBC 주석상 1/128 Nm(최대 약 ±8 Nm) 추가 토크 요청입니다. 이 차량 MDPS의 실제
  출력과 수용 범위는 확인되지 않았습니다.
- `lateral=0`은 토크 0 요청이며 현재 각도 유지가 아닙니다. 위치 유지는 상위 제어기가
  `/ioniq5/vehicle_state`의 `steering_angle_deg`, `steering_rate_deg_s`, `driver_torque` 등으로
  폐루프를 구성해야 합니다.
- 호스트 토크 제어기(목표각 적분, PID, 피드포워드, 마찰, 저속 보정)는 거치지 않습니다.
  호스트 slew와 운전자 토크 제한도 없습니다.
- 85도 이상에서는 EPS 오류 예방을 위해 89프레임마다 2프레임 동안 토크 값은 유지하고
  요청 비트(`ActToiSta`)만 내립니다.
- 채널 OFF, EPS 일시 오류, 고장 중에는 상위 값과 관계없이 토크 0을 보냅니다.

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

버튼 OFF 중에도 구독은 유지하며 토크는 0을 보냅니다. rate/curvature 모드에서는 조향 목표각을
실측각으로 초기화하고, OFF 중 값을 미리 적분하거나 다음 ON에 과거 목표를 이어 붙이지 않습니다.
최신 값이 계속 있으면 OFF 중에도 ECU 소유권 및 비활성 CAN 프레임은 유지되며,
순정 ADAS 통신 복귀와는 구분됩니다. 기본 I5R1 프로파일에서 값이 250 ms 끊기면
소유권을 해제하고 순정 ECU 통신 복구 후 대기로 돌아가되 버튼 ON/OFF 선택을 유지합니다.
입력만 끊긴 이전 ACTIVE 세션은 새 값과 정상 CAN, 복구 완료를 확인하여 주행 중 재인계할
수 있습니다. 대기 중 OFF, 브레이크의 종방향 latch-off, CANCEL과 CAN/USB 고장을 무시하지 않습니다.
이전 목표 조향각과 적분 상태는 버립니다. 첫 takeover는 정차가 필요합니다. 고장 후에는 순정 복구 뒤
버튼 대기로 돌아가며, 새 LDA 또는 SET 조작이 고장을 확인하고 주행 중에도 다시 인계합니다.
복구 실패 시 구독은 계속하지만 제어는 금지합니다. 정차 시험에서 ECU 통신 복구는 관측했으나
ACTIVE 추종/주행 중 재인계와 차량 보조 기능 정상 복구는 확인되지 않았습니다. [검증 기록](validation.md)을 참고하십시오.

`direct_torque`에서 `lateral=0`은 토크 0 요청입니다. rate 모드라면 누적 목표 조향각 유지입니다.
두 필드가 모두 0이어도 유효한 명령입니다. 값이 없다는 뜻은 토픽의 최신 수신이 없다는 뜻입니다.

## 횡방향 모드

| `input.lateral_mode` | 입력 단위 | 차량 출력까지의 변환 |
| --- | --- | --- |
| `direct_torque` (기본) | Panda torque count | 반올림, ±1021 끝값 처리 후 그대로 LFA 토크 |
| `steering_rate_deg_s` | deg/s | rate 적분 → 목표 조향각 → 실측각 피드백 → 토크 |
| `steering_rate_rad_s` | rad/s | deg/s로 변환한 뒤 동일 경로 |
| `curvature_1pm` | 1/m | wheelbase/steering ratio로 목표각 변환 → 동일 토크 제어기 |

ROS 노드에는 직접 조향각 입력 모드가 없습니다. 목표각을 토크로 추종하는 방식과
MDPS에 native angle 명령을 보내는 방식은 다릅니다. 이 HDA1 차량의 native angle 수용 여부는
확인되지 않았습니다. `direct_torque` 단위는 Nm가 아니며, 1021 count는 명령 경로의 상한입니다.

rate/curvature 경로는 Carrotpilot Ioniq 5 횡가속도 및 마찰 보상 토크 제어기를 사용합니다.
정확한 부호와 scale은 실제 차량 상태 및 입력 정의와 대조해야 합니다.

## 값 전달과 남는 변환

scale/offset은 `scaled = input * scale + offset`입니다. 기본 scale은 1, offset은 0입니다.
`input.unfiltered_input=true`에서는 host의 rate/목표각 clamp와 입력 평활화를 생략합니다.
가속도는 host clamp 또는 jerk slew 없이 다음 50 Hz SCC에 반영하며, CAN에서 0.01 m/s²
단위로 양자화됩니다. 2026-10-08 사용자 결정에 따라 입력 전달을 끊지 않습니다.
scale/offset 적용 후 YAML 범위(`-10.23 .. 10.24 m/s²`, CAN 11-bit 표현 범위)를 벗어나면
오류나 disarm 없이 끝값으로 맞춥니다. 토크도 ±1021 count에서 끝값으로 맞춥니다.

`unfiltered_input=false`에서는 설정한 rate/목표각 범위로도 clamp합니다.
`longitudinal.jerk_limit_mps3`(12.7, CAN 최대값)는 활성 SCC의 `JerkUpperLimit`/`JerkLowerLimit`
metadata이며 host 가속도 slew 기능이 아닙니다. 차량이 이 값을 jerk 제한으로 쓰는지는
확인되지 않았습니다. 모든 모드에서 ±1021 count 상한, CAN 양자화, 채널 허가와 watchdog은
남습니다. active YAML은 host torque slew(`torque_rate_up/down: 2042`)와
driver torque 제한(`driver_torque_multiplier: 0`)을 끕니다.
1021은 DBC Reserved/Invalid 원시값(+1022/+1023)을 피하는 대칭 최대값입니다.

아래 호스트 토크 제어기 설정은 rate/curvature 모드에서만 쓰이며 `direct_torque`에는 영향이 없습니다.
토크 제어기 출력 1.0은 `torque_output_scale` 270 count에 대응하고, `max_torque` 1021은 clamp로만
쓰입니다. 적분항은 정규화 3.78(약 1021 count)까지 쓸 수 있고 정차와 저속에서도 누적합니다.
출력이 상한에 닿으면 적분 누적을 멈추며, 버튼 OFF와 재인계 때 적분과 목표각을 초기화합니다.
NaN/Inf 입력은 맞출 범위가 없어 기존처럼 무효 명령으로 처리합니다.

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
