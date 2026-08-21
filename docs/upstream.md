# Upstream pins and evidence

2026-08-14 기준 다음 commit을 고정해 구현 및 golden frame을 대조했습니다.

| 프로젝트 | commit | 사용 지점 |
| --- | --- | --- |
| [commaai/opendbc](https://github.com/commaai/opendbc) | `b72c1fd55ae7e84763e40912bbe06b8f533cb66b` | Hyundai CAN-FD DBC, message constructors, parser semantics, safety hooks |
| [commaai/panda](https://github.com/commaai/panda) | `dd8a5b3df77706337a11555377e7180c5adc8726` | USB packet/health ABI, control requests, firmware build flags |
| [ajouatom/openpilot](https://github.com/ajouatom/openpilot/tree/carrot-wip) | `7fae709b39ec060a0bdd8cc141877eefecb72163` | 사용자 조정 가능 파라미터 방식 참고 |

LDA/SET 분리 arm은 upstream 동작이 아니라 [`patches/opendbc-hyundai-canfd-split-arm.patch`](../patches/opendbc-hyundai-canfd-split-arm.patch)의 opt-in 확장입니다.

차량 전제는 opendbc의 Ioniq 5 platform entry와 CAN-FD fingerprint logic을 따릅니다.

- Ioniq 5 HDA1: Hyundai K harness
- wheelbase 2.97 m, steering ratio 14.26
- non-LKA HDA1는 LFA steering
- radar-SCC flag가 없는 non-LKA 구성은 camera-SCC
- ECAN bus 0, camera bus 2

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

Carrot fork의 종방향 범위 `-4.0 .. 2.5 m/s²`는 고정 Panda safety의
`-3.5 .. 2.0 m/s²`보다 넓으므로 적용하지 않고 Panda 범위를 사용합니다.

업데이트 절차는 단순히 SHA만 바꾸지 않습니다. 새 opendbc CANPacker로 golden frame을
다시 만들고, Panda packet hashes/health struct/safety flags/TX whitelist/rate limits를
재검토한 뒤 HIL 단계부터 반복해야 합니다.
