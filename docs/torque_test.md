# 조향 토크 시험 발행기

손으로 토크를 바꿔 보려면 아래 [슬라이더](#슬라이더)를, 정해진 파형을
반복하려면 `torque_test.py`를 사용합니다. 둘 다 같은 ROS 경로와 CSV 형식을 씁니다.

`scripts/torque_test.py`는 상위 제어기 대신 `/ioniq5/actuation_command`에 `direct_torque`
토크 count를 발행합니다. 실행 중인 ECAN 노드를 그대로 거치므로 상위 제어기와 같은 경로를
시험합니다. 스크립트는 진폭, 변화율, 속도 조건을 두지 않으며 노드의 ±1021 count 끝값 처리만
적용됩니다. 2026-10-08 작성했고 Windows에서 구문, 프로파일 계산, 슬라이더 발행 스레드와
CSV 기록만 가짜 ROS 객체로 확인했습니다. ROS 실행, GUI 표시와 차량 시험은 수행하지 않았습니다.

## 전제

- 이 저장소의 로컬 safety patch가 들어간 Panda 펌웨어가 빌드되고 flash되어 있어야 합니다.
- ECAN 노드가 `lateral_mode=direct_torque`로 실행 중이어야 합니다.
- 처음 인계는 정차, D단, 최근 순정 SCC 수신, EPS 정상 조건이 필요합니다.
  브레이크를 밟고 있으면 종방향은 꺼진 채 조향만 동작합니다.

## 실행

```bash
source /opt/ros/noetic/setup.bash
source <작업공간>/devel/setup.bash
rosrun ioniq5_ecan torque_test.py --profile sine --amplitude 100 --period-s 2 --duration-s 6
```

1. 스크립트가 토크 0을 100 Hz로 발행하며 대기합니다.
2. LDA를 누르면 노드가 인계하고, `lateral_control_active`가 true가 되는 순간 프로파일을 시작합니다.
3. 프로파일이 끝나거나 Ctrl-C를 누르면 토크 0을 `--tail-zero-s` 동안 보낸 뒤 발행을 멈춥니다.
   노드는 command timeout 뒤 순정 통신을 복구합니다. 즉시 멈추려면 LDA를 다시 누릅니다.

## 옵션

| 옵션 | 기본값 | 의미 |
| --- | --- | --- |
| `--profile` | `hold` | `hold` 즉시 진폭으로 계단 후 유지, `square` ±진폭 교대, `ramp` 주기 동안 0→진폭 후 유지, `sine` |
| `--amplitude` | 필수 | 토크 count, 부호 포함 |
| `--offset` | 0 | 모든 프로파일에 더하는 count |
| `--period-s` | 2.0 | square/sine 주기, ramp 상승 시간 |
| `--duration-s` | 5.0 | 프로파일 길이, 0이면 Ctrl-C까지 |
| `--rate-hz` | 100 | 발행 주기 |
| `--acceleration` | 0.0 | 함께 보내는 가속도. SET 조향+종방향 인계 때만 사용 |
| `--no-wait-active` | 꺼짐 | 인계를 기다리지 않고 바로 프로파일 시작 |
| `--tail-zero-s` | 0.5 | 종료 전 토크 0 발행 시간 |
| `--output-dir` | 패키지 `log/torque_test` | CSV 저장 위치 |

## 기록

매 발행 주기마다 CSV 한 줄을 남깁니다. 명령 토크와 가장 최근 `vehicle_state`의 조향각,
조향각속도, 운전자 토크, EPS 토크/오류, 브레이크, 정차, 제어 상태와 Panda 허가/거부 횟수를
함께 기록합니다. `vehicle_state`는 100 Hz이고 50 Hz 신호(브레이크, 버튼)는 두 줄씩 같은 값입니다.

## 슬라이더

`scripts/torque_slider.py`입니다.

```bash
rosrun ioniq5_ecan torque_slider.py            # 화면이 있으면 창, 없으면 터미널 게이지
rosrun ioniq5_ecan torque_slider.py --ui terminal
```

- 슬라이더 값을 100 Hz로 `lateral`에 발행합니다. 시작 값은 0이고 범위는 기본 ±1021입니다.
  `--range`로 슬라이더 폭만 바꿀 수 있으며 변화율이나 속도 제한은 없습니다.
- 창 모드는 Python 표준 `tkinter`를 사용합니다. 화면(`DISPLAY`)이 없거나 `tkinter`가 없으면
  터미널 게이지로 자동 전환하며 추가 패키지를 설치하지 않습니다.
- 조작: 슬라이더 드래그, ±1/±10/±100 버튼, 0 버튼. 키보드는 좌우 1, 상하 10, PgUp/PgDn 100,
  Space(터미널은 0도 가능) 0입니다. 터미널에서는 q로 종료합니다.
- 창 아래(터미널은 게이지 아래)에 제어 상태, 조향 활성 여부, 조향각, 조향각속도, 운전자 토크,
  EPS 토크/오류, 브레이크, 속도, Panda 허가와 거부 횟수를 0.1초마다 표시합니다.
- 인계 전에 슬라이더를 움직여 두면 LDA로 인계되는 순간 그 토크가 바로 나갑니다.
- 창을 닫거나 q, Ctrl-C로 끝내면 토크 0을 `--tail-zero-s` 동안 보낸 뒤 발행을 멈춥니다.
  CSV는 같은 `log/torque_test` 폴더에 `torque_slider_*.csv`로 저장됩니다.

## 확인할 점

- 작은 진폭의 `hold`로 토크 부호와 `steering_angle_deg` 증가 방향의 관계를 먼저 확인합니다.
- `panda_safety_tx_blocked`가 증가하면 설치된 펌웨어가 프레임을 거부한 것입니다.
- `eps_fault`가 켜지면 노드가 조향 출력을 멈추고 3초 복귀 창을 적용합니다.
