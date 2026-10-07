# 2026-10-07 ECAN 분석 근거

[분석 시작 문서](../../ecan_analysis_20261007.md)의 현재 표와 함께 읽는다.
동일 날짜 안의 이전 분석 단계와 현재 결과를 구분한다.

| 파일 | 역할 | 상태 |
| --- | --- | --- |
| `ecan-interpreted-bit-dictionary-20261007.json` | 현재 97개 ID, 605개 필드와 3,892개 표시 비트 | 현재 결과 |
| `ecan-bag-reference-inference-20261007.json` | 동시 기록 센서와의 대응, 시간 기준, 단위 및 순환번호 수정 | 현재 근거 |
| `ecan-bag-reference-receipt-20261007.json` | 현재 표의 검증 범위와 파일 SHA-256 | 현재 검증 기록 |
| `ecan-korean-bit-dictionary-20261007.json` | 138개 ID의 전체 17,920개 비트 감사 | 원본 감사 스냅샷 |
| `ecan-bit-audit-receipt-20261007.json` | 전체 비트 감사의 계산/검증 범위 | 당시 스냅샷 |
| `ecan-exa-followup-sources-20261007.json` | 추가 검색 140건, 고유 URL 76개, source 판단 | 검색 이력 |
| `ecan-exa-followup-inference-20261007.json` | 회전 누적값, 부하, 생존번호 및 CRC8 후보 검증 | 당시 계산 |
| `ecan-exa-followup-receipt-20261007.json` | 이전 602행 표시와 제외 항목의 기록 | 이전 표시 스냅샷 |
| `ecan-structure-20261007.json` | 캡처/ID/길이/기록 수와 최초 DBC 일치 | 초기 인벤토리 |
| `ecan-fields-20261007.json` | 소스 정의, alias, 폭/부호 대안과 관측 범위 | 초기 DBC 사전 |
| `ecan-deep-inference-20261007.json` | 모든 ID의 가설과 수치 관계 | 초기 추론 스냅샷 |
| `ecan-internet-source-audit-20261007.json` | 초기 검색과 고정 레포 source 근거 | 검색 이력 |
| `ecan-inference-evidence.png`, `.svg` | 초기 추론의 관계 그림 | 초기 그림 |
| `ecan-value-export-receipt-20261007.json` | 로컬 수치 ZIP의 버전, 해시와 열 구조 | 로컬 자료 이력 |
| `can-only-4ba-layout.json` | `0x4BA` 개별 배치, raw 예시와 표준 비교 | 당시 개별 분석 |
| `DBC_SOURCE_LICENSE.txt` | 재사용 DBC 정의의 원본 허가 고지 | 라이선스 |

이전 receipt의 해시는 그 분석 단계의 파일 상태를 뜻한다.
수정된 표시 문서의 현재 해시와 비교할 때는 최신 bag-reference receipt를 사용한다.
숫자 수치, placeholder 제거, 익명화와 센서 참조 범위는 각 JSON의 검증/한계 필드에 기록했다.

원본 bag과 센서 영상/점군, 로컬 수치 ZIP은 저장소에 넣지 않았다.
IMU 식별번호의 배치만 남기고 값과 비트 통계는 생략했다.
절대 GPS 좌표와 개인 실행 환경 경로도 이 배포의 근거 자료에 포함하지 않는다.
