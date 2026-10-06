# 임시 ROS 입력 계약

2026-10-06 현재 코드와 기본 연구장 YAML 기준입니다. 상위 제어기 팀의 최종 물리량,
단위, 부호, 메시지 구조와 송신 주기는 아직 확정되지 않았습니다.

명령 토픽은 `/ioniq5/actuation_command`이며 메시지는
[`ActuationCommand.msg`](../msg/ActuationCommand.msg)입니다.

| 필드 | 현재 의미 |
| --- | --- |
| `stamp` | source timestamp, 지연 분석용. watchdog에는 사용하지 않음 |
| `sequence` | 선택적 순서 검사. 0은 검사 생략, wraparound 및 timeout 후 publisher 재시작 허용 |
| `enable` | `input.use_enable_field=true`일 때 매 메시지 deadman |
| `lateral` | `input.lateral_mode`가 정하는 값, scale/offset 적용 전 |
| `acceleration` | scale/offset 적용 전 가속도, 기본 단위 m/s² |

기본 `use_enable_field=false`에서는 `lateral`과 `acceleration` 두 값만 필요합니다.
유한한 최신 값을 계속 보내야 하며, 기본 command watchdog은 호스트 수신 시각 기준
250 ms입니다. source timestamp와 호스트 steady clock은 별도 clock domain입니다.

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

[`VehicleState.msg`](../msg/VehicleState.msg)의 `valid`는 필수 steering/MDPS/wheel/TCS
CAN의 크기, CRC와 freshness를 뜻합니다. `eps_fault`는 별도 MDPS LKA 보조 상태입니다.
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

최종 계약에서는 물리량/좌표계/부호, 가속도 경사 보상, 송신 주기와 jitter,
source clock domain, sequence 재시작 규칙, deadman 소유 주체를 확정해야 합니다.
설정으로 수용할 수 없는 변경은 메시지와 adapter 경계에서 반영합니다.
