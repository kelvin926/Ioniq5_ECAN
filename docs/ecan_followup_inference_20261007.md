# ECAN 추가 검색 및 추론 근거

이 문서는 당시 분석 단계의 기록이다. [현재 분석 시작 문서](ecan_analysis_20261007.md)와 [최신 한국어 필드 표](ecan_korean_bit_fields_20261007.md)를 먼저 참고한다.

2026-10-07, CAN 전용. Exa 6개 분야, 14개 질의, 140개 결과, 76개 고유 URL.

바퀴 회전 누적값과 모터 부하 상세값, 생존번호 및 CRC8 후보를 추가했다. 끝내 의미를 붙이지 못한 비트와 임시 원시 컨테이너는 사용자 표시 표에서 제외했다.

## 바퀴 회전 누적값

| 실제 비트 | 뜻 | 순환 | 독립 캡처 상관계수 | 증가량 오차 |
| --- | --- | --- | --- | --- |
| 24~31 | 왼쪽 앞바퀴 회전 누적값 후보 | 0~254 | 0.9999918 | 0.531 카운트 |
| 32~39 | 오른쪽 앞바퀴 회전 누적값 후보 | 0~254 | 0.9999917 | 0.535 카운트 |
| 40~47 | 왼쪽 뒷바퀴 회전 누적값 후보 | 0~254 | 0.9999893 | 0.522 카운트 |
| 48~55 | 오른쪽 뒷바퀴 회전 누적값 후보 | 0~254 | 0.9999888 | 0.535 카운트 |

50프레임 증가량과 대응 바퀴속도 적분 비교. 10 ms 명목 주기를 사용했으며 물리 타이어 치수나 하드웨어 시각으로 검증하지 않았다. 앞/뒤 좌우 차이의 독립 캡처 상관계수는 각각 0.997818, 0.997124다. 절대 주행거리의 원점은 없다.

## 모터 부하 상세값

| ID | 실제 비트 | 뜻 | 원시 범위 | 독립 캡처 상관계수 |
| --- | --- | --- | --- | --- |
| 0x10A | 192~203 | 후륜 구동부하 상세값 후보 | -1039~1184 | 0.9999882 |
| 0x10A | 192~205 | 후륜 구동부하 상세값 후보 | -1039~1184 | 0.9999882 |
| 0x120 | 192~203 | 전륜 구동부하 상세값 후보 | -572~1066 | 0.9999799 |
| 0x120 | 192~205 | 전륜 구동부하 상세값 후보 | -572~1066 | 0.9999799 |

192:12와 192:14의 부호 있는 해석은 이번 자료에서 값이 동일하다. 192:16 부호 해석은 관련성을 잃어 제외했다. 기존 40:10 부하와의 이득은 후륜 약 5.51, 전륜 약 4.10이다. Nm/A 단위와 요청/실제 측정 구분은 이 관계로 확정할 수 없다.

## 메시지 생존번호

| ID | 실제 비트 | 순환 | 독립 캡처 +1 비율 |
| --- | --- | --- | --- |
| 0x1CF | 12~15 | 0~14 | 100.000% |
| 0x36F | 12~15 | 0~14 | 99.950% |
| 0x37F | 12~15 | 0~14 | 99.875% |
| 0x412 | 12~15 | 0~14 | 100.000% |
| 0x413 | 12~15 | 0~14 | 100.000% |
| 0x418 | 12~15 | 0~14 | 100.000% |
| 0x419 | 12~15 | 0~14 | 100.000% |
| 0x41C | 12~15 | 0~14 | 100.000% |
| 0x435 | 12~15 | 0~14 | 99.619% |

동일 payload 반복을 별도로 세었다. +2는 중간 원래 갱신을 관측하지 못했거나 중계 갱신이 건너뛴 경우와 구분되지 않는다. 하드웨어 수신 누락률이나 원 ECU 갱신율로 해석하지 않는다.

## CRC8 후보

| ID | 실제 비트 | 다항식 | 초기값 0 등가 XOR | 일치 기록 | 고유 본문 |
| --- | --- | --- | --- | --- | --- |
| 0x36F | 0~7 | 0x1D | 0xe5 | 6561 | 15 |
| 0x37F | 0~7 | 0x1D | 0xfb | 6561 | 15 |
| 0x3E5 | 0~7 | 0x1D | 0xd0 | 3288 | 4 |

CRC 계산은 바이트 1~7, 비반사, 초기값 0 등가식이다. 고정 길이에서 초기값과 최종 XOR, Data-ID를 각각 확정할 수 없다. 특히 0x3E5는 캡처 내부 본문 변화가 없어 일반화 근거가 제한적이다.

## 원본 주석의 비트 의미

| ID | 실제 비트 | 뜻 | 소스 |
| --- | --- | --- | --- |
| 0x04A | 24 | 요레이트 고장 비트 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L756) |
| 0x04A | 25 | 요레이트 초기화 비트 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L756) |
| 0x04A | 28 | 횡가속도 고장 비트 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L757) |
| 0x04A | 29 | 횡가속도 초기화 비트 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L757) |
| 0x04A | 44 | IMU 재설정 비트 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L758) |
| 0x11A | 96 | 내비 분기정보 요청 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L834) |
| 0x11A | 97 | 내비 곡률정보 요청 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L834) |
| 0x11A | 98 | 내비 부가프로파일 요청 | [고정 커밋 원본 주석](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L834) |

이 상태/요청 비트들은 캡처에서 0 고정이며 실제 고장, 재설정, 지도요청 조작으로 검증하지 않았다.

## 추가 탐색의 한계

남은 수치 영역 2,691개에 전/후륜 부하 공동 모델을 적용하고, 미정의 변화 비트 1,096개에 부하 부호 관계를 탐색했다. 모터 부하 영역 외에 기준을 통과한 새 부하 부호 비트는 없었다. 결과에 의미 없는 이름을 붙이지 않고 표에서 제외했다.

[Exa 검색 근거](evidence/2026-10-07/ecan-exa-followup-sources-20261007.json), [계산 및 후보 검증 수치](evidence/2026-10-07/ecan-exa-followup-inference-20261007.json), [해석된 필드 표](ecan_korean_bit_fields_20261007.md).
