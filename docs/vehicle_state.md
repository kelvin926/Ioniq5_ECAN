# ROS 차량 상태와 피드백 필드

2026-10-07 현재 소스 기준입니다. 토픽은 `/ioniq5/vehicle_state`, 메시지는
[`ioniq5_ecan/VehicleState`](../msg/VehicleState.msg)이며 ROS timer로 100 Hz 발행합니다. 조향각(`0x125`), MDPS(`0x0EA`), 휠 속도(`0x0A0`), 페달/기어(`0x035`), IMU(`0x04A`)는 CAN에서 약 100 Hz, 브레이크(`0x175`), SCC(`0x1A0`), 버튼(`0x1CF`)은 약 50 Hz로 들어옵니다(2026-10-07 캡처 기준).
CAN 수신을 해석한 최신 상태와 호스트/Panda 제어 상태를 함께 담습니다.
코드 근거는 [parser](../src/vehicle_state_parser.cpp)와 [발행 노드](../src/node.cpp)입니다.

## 차량 측정 및 상태

| 필드 | 의미 | 단위 / 값 | 현재 해석 근거 |
| --- | --- | --- | --- |
| `stamp` | 상태 발행 시각 | ROS time | `ros::Time::now()` |
| `valid` | 필수 CAN 갱신 상태 | bool | 조향 센서/MDPS/휠 속도/TCS, 기본 250 ms 이내 |
| `speed_mps` | 4륜 평균 차속 | m/s | `0x0A0`의 휠 속도 평균 |
| `wheel_speed_fl`, `wheel_speed_fr` | 앞 왼쪽/오른쪽 휠 속도 | m/s | `0x0A0`, DBC km/h를 3.6으로 나눔 |
| `wheel_speed_rl`, `wheel_speed_rr` | 뒤 왼쪽/오른쪽 휠 속도 | m/s | `0x0A0`, 같은 환산 |
| `standstill` | 휠 속도 기준 정지 | bool | 4륜 모두 0.375 km/h 이하 |
| `steering_angle_deg` | 핸들 조향각 | deg | `0x125`, signed16 bit24부터, raw × 0.1 |
| `steering_rate_deg_s` | 핸들 조향각속도 | deg/s | `0x125` 속도 크기 × 4, 각도 차분으로 방향 부여 |
| `yaw_rate_deg_s` | 차량 요레이트 | deg/s | `0x04A` 또는 `0x0E5`, raw × 0.005 − 163.84 |
| `lateral_accel_mps2` | CAN IMU 횡가속도 | m/s² | `0x04A` 또는 `0x0E5`, DBC 환산 후 g × 9.80665 |
| `longitudinal_accel_mps2` | CAN IMU 종가속도 | m/s² | 같은 IMU 환산, `0x175` 기준 가속도와 별개 |
| `driver_torque` | 운전자 조향 토크 신호 | count | `0x0EA`, raw − 4095 |
| `eps_torque_nm` | MDPS 출력 토크 신호 | Nm, DBC 단위 | `0x0EA`, raw × 0.1 − 204.8 |
| `accelerator_pedal` | 가속 페달 원시값 | 0~255 | `0x035`, 8-bit 값, 백분율 환산 없음 |
| `gas_pressed` | 가속 페달 입력 | bool | 페달 원시값 > 0 |
| `brake_pressed` | 브레이크 입력 | bool | `0x175` 브레이크 비트 |
| `gear` | 기어 코드 | uint8 | `0x035`의 3-bit 값, 정의상 0=P, 5=D, 6=N, 7=R |
| `eps_fault` | MDPS LKA 보조 오류 | bool | `0x0EA`의 2-bit 상태가 0 이외 |
| `acc_fault` | ACC 비정상 상태 | bool | `0x175` ACCEnable 2-bit 값이 0 이외 |
| `cruise_engaged` | SCC 모드 활성 상태 | bool | `0x1A0` ACCMode가 1 또는 2 |
| `cruise_button` | 크루즈 버튼 코드 | uint8 | 기본 `0x1CF`, alternate 설정에서는 `0x1AA` |
| `lane_keep_button_pressed` | LDA 버튼 입력 | bool | 같은 버튼 메시지의 LDA 비트 |

휠 속도는 크기이며 후진 부호를 붙이지 않습니다. 조향각속도는 CAN이 직접 제공하는
부호 있는 속도가 아닙니다. 첫 샘플이나 각도 차분이 작을 때는 0이 될 수 있습니다.
IMU 가속도 환산은 `(raw × 0.000127465 − 4.17677312) × 9.80665`입니다.
중력, 장착 자세와 필터가 남아 있어 요청 가속도나 GPS 운동가속도와 동일하지 않습니다.
`eps_torque_nm`은 DBC 환산값이며 검증된 최대 조향력이나 송신 토크 count를 뜻하지 않습니다.

## 제어 및 통신 상태

| 필드 | 의미 | 읽는 기준 |
| --- | --- | --- |
| `control_state`, `control_state_name` | 호스트 제어 상태 | 0 DISCONNECTED, 1 PASSIVE, 2 ARMED, 3 ACTIVE, 4 FAULT, 5 SOFT_DISABLING |
| `control_reason` | 현재 상태 사유 | supervisor 문자열 |
| `lateral_armed`, `longitudinal_armed` | 채널 선택 | 운전자 선택 및 래치 반영 |
| `lateral_control_active`, `longitudinal_control_active` | 채널 출력 허가 | 호스트의 현재 허가, ECU 실행 확인과 구분 |
| `panda_connected` | Panda 연결 상태 | USB 연결 관측 |
| `panda_controls_allowed` | Panda 전역 허가 | 채널별 active와 함께 확인 |
| `panda_safety_tx_blocked` | Panda 송신 차단 누적 수 | health 값, 최근 증가와 누적값 구분 |
| `can_checksum_failures` | parser CRC 실패 누적 수 | parser가 CRC를 검사하는 프레임 |
| `can_malformed_frames` | parser 형식 오류 누적 수 | 대상 메시지 크기/CRC 등 거부 횟수, CRC 실패와 중복 가능 |

`valid`는 필수 4종의 정상 크기/CRC 수신 시각을 확인합니다. 호스트 parser는 counter를
별도 검사하지 않으며 Panda의 RX 검증과 구분합니다. `valid=true`여도 IMU/페달/기어/SCC/
버튼이 최근에 갱신됐다는 보장은 없습니다. 미수신 값은 초기값, 이후 미갱신 값은 마지막
값이 남을 수 있습니다. 필드별 수신 시각과 개별 valid는 현재 메시지에 없습니다.
CAN 시각과 ROS 발행 시각, 요청값과 실제 ECU 응답은 각각 구분해야 합니다.

## 최신 분석과 구현 범위

현재 [ECAN 비트 분석](ecan_analysis_20261007.md)의 필드 사전은 별도 오프라인 결과입니다.
`0x0DA` 차속, `0x175` 기준 종가속도, `0x1AA` 보정 표시속도 후보와 내비 프로필은
현재 `VehicleState`에 추가되지 않았습니다. 기존 `speed_mps`는 계속 4륜 평균입니다.
CAN에서 확인된 GPS 절대 좌표나 원시 레이더 객체 목록의 매핑도 없습니다.

동시 기록 GPS는 조향각 피드백의 좌회전 양수 해석을 지지했지만 상위 조향 명령과
송신 토크 방향의 실차 검증은 남아 있습니다. yaw의 GPS 대조와 IMU 가속도의 장착/중력
미해결 사항은 [센서 대조 결과](ecan_bag_reference_inference_20261007.md)에 있습니다.
요청값/적용값/포화/거부를 묶은 전용 피드백 메시지도 아직 구현하지 않았습니다.
