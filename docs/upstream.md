# Upstream pins and evidence

2026-10-07 문서 기준입니다. 아래 runtime/build pins는 2026-08-14에 선택한 baseline을
유지합니다. Carrotpilot 및 ECAN 분석용 비교 snapshot은 runtime pins의 업데이트가 아닙니다.

| 프로젝트 | commit | 사용 지점 |
| --- | --- | --- |
| [commaai/opendbc](https://github.com/commaai/opendbc) | `b72c1fd55ae7e84763e40912bbe06b8f533cb66b` | Hyundai CAN-FD DBC, message constructors, parser semantics, safety hooks |
| [commaai/panda](https://github.com/commaai/panda) | `dd8a5b3df77706337a11555377e7180c5adc8726` | USB packet/health ABI, control requests, firmware build flags |
| [ajouatom/openpilot](https://github.com/ajouatom/openpilot/tree/carrot-wip) | `7fae709b39ec060a0bdd8cc141877eefecb72163` | 사용자 조정 가능 파라미터 방식 참고 |

LDA/SET 분리 arm과 forwarding 차단은 upstream 동작이 아니라
[`patches/opendbc-hyundai-canfd-split-arm.patch`](../patches/opendbc-hyundai-canfd-split-arm.patch)의
opt-in 확장입니다. 비-ECAN transceiver 비활성화와 하네스 방향 gate는
[`patches/panda-ecan-only.patch`](../patches/panda-ecan-only.patch)에 있습니다.
I5R1의 NO_OUTPUT/ELM/Hyundai 전환 중 버튼 선택 보존 및 새 CAN 검증은
[`patches/opendbc-command-session.patch`](../patches/opendbc-command-session.patch)의 추가
opt-in 확장입니다. Panda patch의 `0xB7` capability/상태 조회와 `0xB8` 허가 감소 요청도
upstream ABI 확장입니다. marker 문자열만으로 이 확장의 설치를 식별하지 않습니다.

차량 전제는 opendbc의 Ioniq 5 platform entry와 CAN-FD fingerprint logic을 따릅니다.

- Ioniq 5 HDA1: Hyundai K harness
- wheelbase 2.97 m, steering ratio 14.26
- non-LKA HDA1는 LFA steering
- ECAN bus 0의 LFA `0x12A`, SCC `0x1A0` 및 실제 버튼 fingerprint를 사용
- 버튼 `0x1CF`와 alternate `0x1AA` parser를 지원하며 기본 `alternate_buttons=false`
- ECAN bus 0만 사용하고 camera bus 2는 transceiver와 forwarding 모두 비활성화

대조한 주요 파일:

- `opendbc/car/hyundai/values.py`
- `opendbc/car/hyundai/interface.py`
- `opendbc/car/hyundai/hyundaicanfd.py`
- `opendbc/car/hyundai/carcontroller.py`
- `opendbc/dbc/generator/hyundai/hyundai_canfd.dbc`
- `opendbc/safety/modes/hyundai_canfd.h`
- `opendbc/safety/modes/hyundai_common.h`
- `panda/board/can_comms.h`
- `panda/board/health.h`
- `panda/board/main_comms.h`
- `panda/SConscript`

## Carrot Ioniq 5 profile

고정한 Carrotpilot commit에서 CAN-FD 조향 한계는 torque 270 count, 증가/감소
2/3 count per 10 ms, driver allowance 250 및 multiplier 2입니다. Ioniq 5 torque data는
`LAT_ACCEL_FACTOR=3.172929`, `FRICTION=0.096019`이고 공통 torque PID 기본값은
`kp=1.0`, `ki=0.1`, `kf=1.0`입니다. Hyundai 공통 `steerActuatorDelay=0.1 s`와
저속 보상표도 `CommandAdapter` 및 YAML에 옮겼습니다.

2026-10-06 추천값 확인에서는 위 현재 YAML baseline 유지를 권고했습니다. 다음 primary
source를 다시 대조했으며 설정 변경이나 테스트는 수행하지 않았습니다.

- [고정 opendbc Ioniq 5 차량 값](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/car/hyundai/values.py): wheelbase 2.97 m, steerRatio 14.26
- [Hyundai 공통 설정](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/car/hyundai/interface.py): steerActuatorDelay 0.1 s
- [Ioniq 5 torque data](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/car/torque_data/params.toml): LAT_ACCEL_FACTOR 3.172929, FRICTION 0.096019
- [Carrot torque 초기화](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/opendbc_repo/opendbc/car/interfaces.py): kp 1.0, ki 0.1, kf 1.0, deadzone 기본 0
- [Carrot 저속 보상](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/openpilot/selfdrive/controls/lib/latcontrol_torque.py): speed [0,10,20,30] m/s, factor [15,13,10,5]

이 값은 upstream baseline에 근거한 추천이며 이 연구차의 live 추정 결과는 아닙니다.
`direct_torque`에서는 위 ratio/feedback gain/friction/저속 보상으로 토크를 재계산하지
않습니다. 해당 모델/튜닝을 사용하는 상위 제어기와 ROS 내부 feedback 경로를 구분합니다.

Carrot fork의 종방향 범위 `-4.0 .. 2.5 m/s²`는 고정 Panda safety의
`-3.5 .. 2.0 m/s²`보다 넓으므로 적용하지 않고 Panda 범위를 사용합니다.

270 count는 software/CAN 명령 제한이며 MDPS 실제 최대 구동 토크가 검증된 값은 아닙니다.
입력 조향각 추종과 native angle 명령 수용 여부도 구분해야 합니다.

## 2026-10-06 Carrotpilot 비교

사용자가 지정한 `ajouatom/openpilot`의 `carrot-wip`을
`eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d`에 고정해 primary source를 읽었습니다.
`controlsd.py`, `state.py`, Hyundai `carcontroller.py`는 commit tree의 Git blob hash도
대조했습니다. 전체 source 목록과 비교 결과는 [state.json](../state.json)에 있습니다.

모델 target은 곧바로 CAN에 넣지 않습니다. `modelV2.action`의 curvature/acceleration/
velocity/stop target을 planner와 feedback controller가 처리하고, `carControl` → `card`
및 Hyundai CarController/packer → `sendcan` → `pandad` → Panda 순서로 전달합니다.
소프트웨어의 `carOutput.actuatorsOutput`은 ECU ACK나 물리 구동력 측정이 아닙니다.
[controlsd 소스](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/openpilot/selfdrive/controls/controlsd.py),
[Hyundai controller](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/opendbc_repo/opendbc/car/hyundai/carcontroller.py).

일시 fault의 `SOFT_DISABLE`은 3초 안에 해소되면 enabled로 복귀합니다. CAN loss/error는
immediate disable/no-entry이며 신호가 돌아오는 것만으로 자동 engage되지 않습니다.
이 fork의 AlwaysLateral 및 별도 lateral gate는 전체 상태와 구분해야 하며 이번 로컬 구현에
옮기지 않았습니다.
[상태기계](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/openpilot/selfdrive/selfdrived/state.py),
[events](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/openpilot/selfdrive/selfdrived/events.py).

Panda wrapper의 재연결 및 시작 시 ECU disable 재시도는 운행 중 ECU 자체 복구와 다릅니다.
조사한 Hyundai 경로에서는 일반적인 주행 중 ECU reset/stock restore/rearm 상태기계를
찾지 못했습니다. 로컬의 pending camera/radar 복구, valid stock 확인, NO_OUTPUT 확인과
명시적 재arm은 이 저장소에서 구현한 기능입니다. 일시 MDPS 보조 오류의 고정 3초
ACTIVE 복귀는 위 상태기계를 참고했습니다. 이후 추가한 정상 publisher 단절의 stock 복구 및
같은 ON 세션 주행 중 재인계는 I5R1 로컬 확장입니다. 재부팅/USB 장애 후 이전 ON을
무조건 자동 계승하는 동작으로 해석하면 안 됩니다.

비교 snapshot에는 angle-control/LFA_ALT 경로도 있으나 이 연구차 HDA1의 지원 증거는
아닙니다. 해당 fork의 safety 수정도 로컬의 고정 Panda hook에 이식하지 않았습니다.

## MDPS parking interface evidence

On 2026-10-06 the user confirmed that this research vehicle did not have stock RSPA.
The pinned [Hyundai CAN-FD DBC](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc)
defines PA status/feedback in MDPS `0xEA`: `MDPS_PaPlugInSta`, `MDPS_PaModeSta`,
`MDPS_PaCanfltSta` and `MDPS_PaStrAnglVal`. The generic mode enumeration includes
waiting for a PA command and activation steps. These fields do not establish PA
support or a complete command protocol on this 2022 HDA1 MDPS.

`SPAS1` (`0x165`) has unidentified fields in this DBC. The inspected
[Carrotpilot SPAS sender](https://github.com/ajouatom/openpilot/blob/eb5bbf2720286f17fc55d6eab8a2f7aacecf1f3d/opendbc_repo/opendbc/car/hyundai/hyundaicanfd.py#L608)
sends an empty SPAS1 payload and `BLINKER_CONTROL` in SPAS2 (`0x16A`); it is not
a parking target-angle controller. The fork's ADAS ACI angle path is a separate
interface and is not vehicle-specific proof of PA support.

The current codec and selected Panda profile provide LFA torque steering, with
no implemented PA command path. Missing evidence is the exact MDPS hardware/software
capability, PA command/activation/timeout protocol, and CAN routing. Stock RSPA
absence alone does not prove latent PA support or inability to support it. Neither
a separate wiring connector nor higher standstill torque authority is established.
This was source research only, without CAN/USB/diagnostic actions or runtime changes.

## 2026-10-07 ECAN 연구 스냅샷

실차 bag의 필드/비트 비교에서는 ajouatom의 DBC와 공개 E-GMP 정의를 추가로 조사했습니다.
주요 비교는 다음 커밋에 고정했으며 현재 런타임 DBC/Panda를 교체하지 않았습니다.

| 연구 소스 | 고정 commit | 사용 범위 |
| --- | --- | --- |
| [ajouatom/openpilot](https://github.com/ajouatom/openpilot/tree/ab696d049e3d6d0fd3bde51c233ee81f736415be) | `ab696d049e3d6d0fd3bde51c233ee81f736415be` | CAN-FD 신호 및 내비 분류 비교 |
| [dragz/egmpdbc](https://github.com/dragz/egmpdbc/tree/b234d7e0ff5ba881f80087580d6e86cf8af447f1) | `b234d7e0ff5ba881f80087580d6e86cf8af447f1` | E-GMP 동력계 신호 비교 |
| [Sterlingarcher2525/ioniq5-can](https://github.com/Sterlingarcher2525/ioniq5-can/tree/8351cc0317e6cd45a286c2ed9b3d64f65336f666) | `8351cc0317e6cd45a286c2ed9b3d64f65336f666` | Ioniq 5 공개 수신 정의 비교 |

다른 소스와 검색 근거는 [연구 근거 목록](evidence/2026-10-07/README.md)에 있습니다.
소스 명칭만으로 실차 물리량을 확정하지 않았으며 CAN 내부 상관과 동시 기록 GPS/레이더를
대조했습니다. 내비 순환번호 등 일부 배치는 소스와 다르게 관측돼 현재 표에서 수정했습니다.
[현재 분석](ecan_analysis_20261007.md)의 605개 행은 폭/단위 대안을 포함한 배치 수입니다.
새 필드의 ROS 구현, 절대 GPS 좌표와 원시 레이더 객체 목록 CAN 매핑은 확인되지 않았습니다.

## Runtime pin 변경 시

업데이트 절차는 단순히 SHA만 바꾸지 않습니다. 새 opendbc CANPacker로 golden frame을
다시 만들고, Panda packet hashes/health struct/safety flags/TX whitelist/rate limits를
재검토하고 변경에 필요한 host/bench/HIL 확인을 수행합니다. 이번 문서 갱신에서는
runtime pin, dependency, firmware 또는 차량 설정을 변경하지 않았습니다.
