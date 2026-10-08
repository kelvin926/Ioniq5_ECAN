# ECAN 전체 해석 필드 토픽

2026-10-07 ECAN 분석에서 해석한 필드를 ROS로 발행합니다. 토픽은 `/ioniq5/ecan_signals`,
메시지는 [`ioniq5_ecan/EcanSignals`](../msg/EcanSignals.msg)입니다.
2026-10-08 작성했고 Windows에서 정의 생성, 비트 배치 대조, 디코딩 값과 속도만 확인했습니다.
ROS 실행과 차량 수신은 확인하지 않았습니다.

## 구성

| 항목 | 내용 |
| --- | --- |
| 정의 파일 | [`config/ecan_signals.json`](../config/ecan_signals.json), 131개 ID와 726개 필드 |
| 정의 출처 | [해석 사전](evidence/2026-10-07/ecan-interpreted-bit-dictionary-20261007.json)의 표시 필드에 [2026-10-08 검증 overlay](evidence/2026-10-08/ecan-validation-overlay-20261008.json) 적용 |
| 제외 | 차량 식별 정보로 가린 필드 1개, 2026-10-08 검증에서 틀린 것으로 판정한 7개 |
| 디코더 | [`scripts/ecan_signal_decoder.py`](../scripts/ecan_signal_decoder.py), 제어 노드와 별도 프로세스 |
| 입력 | 기본 `/ioniq5/can0/rx`. Panda가 돌려준 송신 확인과 거부 프레임은 제외 |
| 발행 | 기본 20 Hz, 각 필드의 가장 최근 값 |

`launch/ioniq5_ecan.launch`가 제어 노드와 함께 디코더를 실행합니다. 끄거나 주기를 바꾸려면
`decode_signals:=false`, `signals_rate_hz:=50`처럼 launch 인수를 줍니다.
수신 전용 기록기와 함께 쓰려면 디코더의 `~input_topic`을 `/ioniq5/can_logger/rx`로 지정합니다.

## 메시지

| 필드 | 의미 |
| --- | --- |
| `name` | `"<CAN ID>.<필드>"`. 같은 비트의 대안 해석은 `#번호`를 붙여 따로 둠 |
| `unit` | 분석 표의 단위. 빈 문자열은 무단위나 코드값 |
| `value` | `raw × factor + offset`. 해당 ID를 아직 받지 못했으면 NaN |
| `age_s` | 해당 ID를 마지막으로 받은 뒤 지난 시간. 받기 전에는 -1 |

배열 순서는 정의 파일 순서와 같고 실행 중 바뀌지 않습니다. 한국어 이름, 확신도, 역할과
enum 값 설명은 정의 파일에 있습니다.

## 해석 한계

- 오프라인 캡처 분석에서 나온 **해석 후보**이며 검증된 DBC가 아닙니다. 확신도는 정의 파일의
  `confidence_ko`와 [분석 문서](ecan_analysis_20261007.md)를 참고합니다.
- 데이터로 가를 수 없는 대안 해석은 모두 발행합니다(예: `0x060.BRAKE_PRESSURE#003`/`#008`,
  SCC가 작동하지 않아 값이 고정이었던 `DISTANCE_SETTING`/`StopReq` 대안). 제어에 쓰는 값은
  `vehicle_state`를 기준으로 합니다.
- CRC와 counter 같은 프로토콜 필드도 포함합니다. `CRC8_CHECKSUM`은 다항식 0x1D, init 0xFF,
  xorout 0xEA를 [CAN ID 하위, 상위 바이트, byte1..7]에 적용한 값입니다.
- 확신도 `2026-10-08 검증 확정`은 독립 기준이나 정확한 규칙과 일치, `2026-10-08 강한 후보`는
  정량 근거가 있으나 의미 일부가 추론인 필드입니다. 터널 연동 비트처럼 한 번의 사건에만 근거한
  후보도 있습니다.
- 해석 사전이나 overlay가 바뀌면 `scripts/generate_ecan_signals.py`로 정의 파일을 다시 만듭니다.
  생성 스크립트는 사전 필드의 비트 위치를 `physical_bits`와 대조하고, overlay 필드가 payload
  범위를 벗어나거나 기존 비트와 겹치면 생성을 중단합니다.

## 2026-10-08 검증

2026-10-07 주행 기록(GNSS/레이더 참조 포함)과 2026-10-08 정차 캡처로 오프라인 검증했습니다.
요약은 [필드 검증](evidence/2026-10-08/ecan-field-validation-summary-20261008.md),
[미해석 ID](evidence/2026-10-08/ecan-unknown-ids-summary-20261008.md),
[미할당 비트](evidence/2026-10-08/ecan-unassigned-bits-summary-20261008.md)에 있습니다.

| 구분 | 결과 |
| --- | --- |
| 기존 604개 필드 | 확정 205, 일관 124, 검증 불가 257, 틀림 18 |
| 틀린 필드 처리 | 7개 제거(조향각 `#002`, `ACC_ObjRelSpd#003`, 8비트 수직가속도, `MOVING_FORWARD` 2개, `0x1AA` 버튼 2개), 12개 수정(`0x0EA.STEERING_ANGLE_2` 부호, `ObjValid`→`ObjInvalid`, `0x060.BRAKE_PRESSED`→`AVH_HOLD_INDICATION`, `BCW_RtSndWrngSta` 2비트, 메시지 변경 카운터 7개 설명 등) |
| 미해석 41개 ID | 32개 CRC8/CRC16와 변경 카운터 확정, 조도/터널 연동 후보 일부. 29개는 값이 고정이라 의미 미확정 |
| 미할당 변화 비트 1,197개 | 548개를 확정 또는 강한 후보로 추가(전륜 연결, 회생제동량, 모터 토크 한계, 0.1 km 주행거리 카운터, 각종 복사 비트 등) |

조향각 부호, `ObjInvalid`, CRC 규칙, 브레이크 반전 비트, 전륜 연결 상태, 주행거리 카운터는
원본 데이터로 다시 계산해 확인했습니다. 저속(최대 약 15.6 m/s) 하루 주행과 10초 정차 캡처
기준이며, 후진, N단, SCC 작동 구간은 없었습니다.

## 확인 결과

- 사전에서 가져온 필드는 모두 계산한 비트 위치가 해석 사전과 일치했습니다. 2026-10-08 overlay
  적용 후 정의로 정차 캡처를 디코딩하면 시계(목요일 22:07), 조향각 +0.7°, 앞차 없음 등이
  관측 상태와 맞습니다.
- 호스트 코덱이 만든 SCC `0x1A0` 기준 프레임을 디코딩해 aReqValue 0.1, aReqRaw 0.5,
  jerk 5.0/5.0, 설정 속도 30, ACCMode 1이 인코딩 값과 같게 나오는 것을 확인했습니다.
- 필드가 가장 많은 `0x1A0`(32개) 3,000프레임 디코딩에 약 35 ms가 걸렸습니다.
  2026-10-06 관측의 ECAN 전체 수신량은 초당 약 2,900프레임입니다.
