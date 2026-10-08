# ECAN 전체 해석 필드 토픽

2026-10-07 ECAN 분석에서 해석한 필드를 ROS로 발행합니다. 토픽은 `/ioniq5/ecan_signals`,
메시지는 [`ioniq5_ecan/EcanSignals`](../msg/EcanSignals.msg)입니다.
2026-10-08 작성했고 Windows에서 정의 생성, 비트 배치 대조, 디코딩 값과 속도만 확인했습니다.
ROS 실행과 차량 수신은 확인하지 않았습니다.

## 구성

| 항목 | 내용 |
| --- | --- |
| 정의 파일 | [`config/ecan_signals.json`](../config/ecan_signals.json), 97개 ID와 604개 필드 |
| 정의 출처 | [해석 사전](evidence/2026-10-07/ecan-interpreted-bit-dictionary-20261007.json)의 표시 필드 |
| 제외 | 차량 식별 정보로 가린 필드 1개 |
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
- 같은 비트에 대안 해석이 여러 개면 모두 발행합니다. 예: `0x125.STEERING_ANGLE#002`와 `#004`는
  부호만 다릅니다. 제어에 쓰는 조향각은 `vehicle_state`의 값을 기준으로 합니다.
- CRC와 counter 같은 프로토콜 필드도 포함합니다.
- 해석 사전이 바뀌면 `scripts/generate_ecan_signals.py`로 정의 파일을 다시 만듭니다.
  생성 스크립트는 모든 필드의 비트 위치가 사전의 `physical_bits`와 같은지 확인합니다.

## 확인 결과

- 604개 필드 모두 계산한 비트 위치가 해석 사전과 일치했습니다.
- 호스트 코덱이 만든 SCC `0x1A0` 기준 프레임을 디코딩해 aReqValue 0.1, aReqRaw 0.5,
  jerk 5.0/5.0, 설정 속도 30, ACCMode 1이 인코딩 값과 같게 나오는 것을 확인했습니다.
- 필드가 가장 많은 `0x1A0`(32개) 3,000프레임 디코딩에 약 35 ms가 걸렸습니다.
  2026-10-06 관측의 ECAN 전체 수신량은 초당 약 2,900프레임입니다.
