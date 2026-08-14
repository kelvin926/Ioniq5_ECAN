# 임시 입력 계약

다른 팀의 출력 정의가 확정되기 전까지 `/ioniq5/actuation_command`는 다음 의미를
사용합니다.

| 필드 | 의미 |
| --- | --- |
| `stamp` | end-to-end latency 분석용 source timestamp. watchdog에는 사용하지 않음 |
| `sequence` | 선택 사항. 0이면 검사 생략, wraparound와 timeout 후 publisher restart 허용 |
| `enable` | `input.use_enable_field=true`일 때만 매 메시지 deadman |
| `lateral` | YAML의 `input.lateral_mode`로 의미 선택 |
| `acceleration` | scale/offset 적용 전 가속도, 기본 단위 m/s² |

기본 설정에서는 `lateral`과 `acceleration` 두 값만 필요합니다. `steering_rate_*` 입력은
rate limit을 적용한 뒤 목표 조향각으로 적분하고, Carrotpilot 방식으로 목표/실제
곡률을 횡가속도와 저속 보상값으로 변환해 마찰 보상 토크를 만듭니다.
`curvature_1pm`은 wheelbase와 steering ratio를 이용해 같은 제어기에 들어가며,
`direct_torque`는 벤치/식별 시험용 Panda 토크 count입니다.

최종 계약에서 확인해야 할 항목:

- lateral 값의 정확한 물리량, 부호, 단위, 좌표계
- acceleration의 부호, 단위, 중력/경사 보상 여부
- publisher 주기와 최대 jitter
- source timestamp clock domain
- sequence rollover와 restart 규칙
- enable/deadman 소유 주체

단위가 확정되면 YAML의 scale/offset과 `lateral_mode`만 맞추고, 필요하면
`use_enable_field`를 활성화합니다.
