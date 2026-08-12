# 임시 입력 계약

다른 팀의 출력 정의가 확정되기 전까지 `/ioniq5/actuation_command`는 다음 의미를
사용합니다.

| 필드 | 의미 |
| --- | --- |
| `stamp` | end-to-end latency 분석용 source timestamp. watchdog에는 사용하지 않음 |
| `sequence` | 0이면 검사 생략, 그 외에는 단조 증가해야 함 |
| `enable` | 매 메시지 deadman. false이면 ARMED로 내려감 |
| `lateral` | YAML의 `input.lateral_mode`로 의미 선택 |
| `acceleration` | scale/offset 적용 전 가속도, 기본 단위 m/s² |

`steering_rate_*` 입력은 목표 조향각을 적분한 뒤 현재 조향각/조향속도 피드백으로
토크를 만듭니다. `curvature_1pm`은 wheelbase와 steering ratio를 이용해 조향휠 각도로
변환합니다. `direct_torque`는 벤치/식별 시험용이며 Panda 단위의 토크 count입니다.

최종 계약에서 확인해야 할 항목:

- lateral 값의 정확한 물리량, 부호, 단위, 좌표계
- acceleration의 부호, 단위, 중력/경사 보상 여부
- publisher 주기와 최대 jitter
- source timestamp clock domain
- sequence rollover와 restart 규칙
- enable/deadman 소유 주체

정의가 확정되기 전에는 기본 `allow_actuation: false`를 유지합니다.
