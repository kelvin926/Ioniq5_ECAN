# ECAN 해석된 비트 및 필드 표

**138개 ID, 1,914,473 CAN 프레임 조사. 현재 표시: 97개 ID, 605개 필드 배치 및 대안.**

[비트 검색표](ecan_bit_viewer_20261007.html) | [현재 JSON](evidence/2026-10-07/ecan-interpreted-bit-dictionary-20261007.json) | [동시 기록 센서 대조](ecan_bag_reference_inference_20261007.md)

CAN 필드의 해석에 같은 bag의 GPS, 외부 레이더, 영상 및 라이다 자료를 참조했다. 표에 표시된 비트와 값은 CAN payload에서 나온다. 센서의 위치/객체값 자체를 CAN 필드로 넣지 않았다.

첫 바이트 B0, 최하위 비트 b0. 실제 비트 번호=8×B+b. 근거가 있는 필드와 추정 후보만 표시한다. 같은 비트의 폭/부호/의미 대안은 독립 신호가 아니다.

## 이번 센서 대조로 달라진 해석

| ID | 실제 비트 | 뜻 | 결과 |
| --- | --- | --- | --- |
| 0x1AA | 48~55 | 보정 표시속도 후보 | raw×0.5 km/h, 속도 의존 보정 패턴 |
| 0x0DA | 64~79 | 차속 | raw×0.01 m/s, GPS 및 레이더 연동 |
| 0x04A | 64~79 | 요레이트 | GPS 좌회전 양수 연동 |
| 0x175 | 32~42 | 기준 종가속도 | GPS 운동가속도 연동 |
| 0x125 | 24~39 | 조향각 | raw×+0.1의 좌회전 양수 근거 |
| 0x4B4 | 18~19 | 내비 순환번호 후보 | 기존19:2 정의 대신18:2 순차 변화 |
| 0x4B8, 0x4B9 | 45~46 | 내비 순환번호 후보 | FF 제외 시0~3 순환, 표본 제한 |
| 0x4BE | 59~60 | 내비 순환번호 후보 | 기존45:2는 유효 형태0 고정 |
| 0x4BF | 56~57 | 내비 순환번호 후보 | FF 제외 시0~3 순환 |

## ID 색인

| ID | 뜻 | 길이 | 평균 Hz | 표시 비트 |
| --- | --- | --- | --- | --- |
| [0x035](#id-035) | 페달 및 구동부하 | 32 B | 100.003 | 63 |
| [0x04A](#id-04a) | 관성센서 | 32 B | 100.004 | 224 |
| [0x060](#id-060) | 차체자세제어 및 오토홀드 | 32 B | 100.003 | 46 |
| [0x065](#id-065) | 제동 및 모터 회전수 후보 | 32 B | 100.003 | 66 |
| [0x06F](#id-06f) | 무결성 및 메시지 생존번호 | 8 B | 100.003 | 24 |
| [0x090](#id-090) | 무결성 및 메시지 생존번호 | 32 B | 10.006 | 8 |
| [0x0A0](#id-0a0) | 네 바퀴 속도 | 24 B | 100.003 | 124 |
| [0x0DA](#id-0da) | 차속 및 구동부하 후보 | 24 B | 100.004 | 72 |
| [0x0EA](#id-0ea) | 전동식 파워스티어링 | 24 B | 100.000 | 168 |
| [0x0F5](#id-0f5) | 구동 및 제동부하 후보 | 32 B | 100.004 | 88 |
| [0x10A](#id-10a) | 후륜 모터 후보 | 32 B | 100.003 | 80 |
| [0x11A](#id-11a) | 전방카메라 기능 상태 | 16 B | 99.989 | 96 |
| [0x120](#id-120) | 전륜 모터 후보 | 32 B | 100.006 | 80 |
| [0x125](#id-125) | 조향센서 | 16 B | 99.482 | 48 |
| [0x12A](#id-12a) | 차로보조 요청 및 표시 | 16 B | 100.000 | 102 |
| [0x130](#id-130) | 변속장치 | 16 B | 100.003 | 32 |
| [0x145](#id-145) | 무결성 및 메시지 생존번호 | 32 B | 50.004 | 8 |
| [0x15A](#id-15a) | 무결성 및 메시지 생존번호 | 8 B | 100.006 | 24 |
| [0x160](#id-160) | 자동긴급제동 설정 | 16 B | 49.999 | 26 |
| [0x175](#id-175) | ESC 및 ACC 상태 | 24 B | 50.002 | 67 |
| [0x180](#id-180) | 무결성 및 메시지 생존번호 | 8 B | 50.048 | 24 |
| [0x185](#id-185) | 무결성 및 메시지 생존번호 | 8 B | 50.048 | 24 |
| [0x19A](#id-19a) | 무결성 및 메시지 생존번호 | 32 B | 100.006 | 24 |
| [0x1A0](#id-1a0) | SCC 요청 및 대상 정보 | 32 B | 49.999 | 160 |
| [0x1AA](#id-1aa) | 계기판 버튼 및 차속 후보 | 16 B | 50.048 | 47 |
| [0x1B0](#id-1b0) | 무결성 및 메시지 생존번호 | 32 B | 50.004 | 8 |
| [0x1B5](#id-1b5) | 카메라 차선 및 앞차 후보 | 32 B | 19.999 | 178 |
| [0x1BA](#id-1ba) | 후측방충돌보조 | 24 B | 20.001 | 95 |
| [0x1CF](#id-1cf) | 핸들 버튼 | 8 B | 50.002 | 20 |
| [0x1DA](#id-1da) | 무결성 및 메시지 생존번호 | 32 B | 1.002 | 24 |
| [0x1E0](#id-1e0) | 고속도로 및 차로추종보조 표시 | 16 B | 20.001 | 56 |
| [0x1E5](#id-1e5) | 후진 상태 후보 | 16 B | 19.999 | 25 |
| [0x1EA](#id-1ea) | 주행보조 화면 표시 후보 | 32 B | 20.001 | 137 |
| [0x1F0](#id-1f0) | 무결성 및 메시지 생존번호 | 16 B | 20.001 | 24 |
| [0x1F5](#id-1f5) | 무결성 및 메시지 생존번호 | 32 B | 9.999 | 24 |
| [0x1FA](#id-1fa) | 제한속도 및 교통표지 | 32 B | 9.999 | 154 |
| [0x200](#id-200) | 차간시간 설정 후보 | 8 B | 20.001 | 30 |
| [0x20A](#id-20a) | 무결성 및 메시지 생존번호 | 16 B | 10.001 | 24 |
| [0x225](#id-225) | 무결성 및 메시지 생존번호 | 16 B | 10.009 | 24 |
| [0x235](#id-235) | 고전압 전압 후보 | 32 B | 9.999 | 40 |
| [0x250](#id-250) | 전력 관련 후보 | 24 B | 10.001 | 34 |
| [0x255](#id-255) | 고전압 전압 후보 | 32 B | 9.999 | 40 |
| [0x25A](#id-25a) | 무결성 및 메시지 생존번호 | 32 B | 9.999 | 24 |
| [0x2AA](#id-2aa) | 무결성 및 메시지 생존번호 | 32 B | 9.998 | 24 |
| [0x2B0](#id-2b0) | 무결성 및 메시지 생존번호 | 32 B | 5.000 | 8 |
| [0x2B5](#id-2b5) | 가속페달 복제 후보 | 32 B | 9.998 | 32 |
| [0x2C0](#id-2c0) | 무결성 및 메시지 생존번호 | 32 B | 5.001 | 24 |
| [0x2D5](#id-2d5) | 무결성 및 메시지 생존번호 | 32 B | 5.001 | 24 |
| [0x2E0](#id-2e0) | 수동 속도제한보조 | 32 B | 9.998 | 43 |
| [0x2E5](#id-2e5) | 무결성 및 메시지 생존번호 | 32 B | 9.999 | 24 |
| [0x2FA](#id-2fa) | 전류 및 전력 관련 후보 | 32 B | 9.998 | 40 |
| [0x2FF](#id-2ff) | 무결성 및 메시지 생존번호 | 8 B | 5.004 | 16 |
| [0x30A](#id-30a) | 무결성 및 메시지 생존번호 | 32 B | 9.999 | 24 |
| [0x315](#id-315) | 무결성 및 메시지 생존번호 | 16 B | 5.004 | 24 |
| [0x31A](#id-31a) | 무결성 및 메시지 생존번호 | 32 B | 1.002 | 8 |
| [0x325](#id-325) | 무결성 및 메시지 생존번호 | 32 B | 9.998 | 24 |
| [0x330](#id-330) | 무결성 및 메시지 생존번호 | 32 B | 9.998 | 24 |
| [0x33A](#id-33a) | 무결성 및 메시지 생존번호 | 32 B | 9.999 | 24 |
| [0x345](#id-345) | 무결성 및 메시지 생존번호 | 8 B | 5.000 | 24 |
| [0x34A](#id-34a) | 무결성 및 메시지 생존번호 | 32 B | 1.002 | 8 |
| [0x360](#id-360) | 무결성 및 메시지 생존번호 | 32 B | 9.998 | 24 |
| [0x36A](#id-36a) | 사각지대 감지 | 16 B | 20.001 | 26 |
| [0x36F](#id-36f) | 무결성 및 메시지 생존번호 | 8 B | 9.989 | 12 |
| [0x37F](#id-37f) | 무결성 및 메시지 생존번호 | 8 B | 9.989 | 12 |
| [0x380](#id-380) | 무결성 및 메시지 생존번호 | 8 B | 5.001 | 16 |
| [0x38A](#id-38a) | 무결성 및 메시지 생존번호 | 16 B | 5.004 | 24 |
| [0x39B](#id-39b) | 무결성 및 메시지 생존번호 | 8 B | 4.943 | 4 |
| [0x3A0](#id-3a0) | 타이어 공기압 | 16 B | 5.004 | 61 |
| [0x3A5](#id-3a5) | 무결성 및 메시지 생존번호 | 8 B | 9.999 | 24 |
| [0x3B5](#id-3b5) | 직류단 전압 후보 | 32 B | 5.000 | 40 |
| [0x3C1](#id-3c1) | 방향지시 및 등화 레버 | 8 B | 5.051 | 18 |
| [0x3CA](#id-3ca) | 무결성 및 메시지 생존번호 | 16 B | 1.002 | 16 |
| [0x3E5](#id-3e5) | 무결성 및 메시지 생존번호 | 8 B | 5.006 | 8 |
| [0x3F0](#id-3f0) | 무결성 및 메시지 생존번호 | 32 B | 1.002 | 24 |
| [0x3F5](#id-3f5) | 무결성 및 메시지 생존번호 | 32 B | 1.002 | 24 |
| [0x411](#id-411) | 문 및 안전벨트 | 8 B | 4.997 | 18 |
| [0x412](#id-412) | 무결성 및 메시지 생존번호 | 8 B | 5.154 | 12 |
| [0x413](#id-413) | 방향지시등 | 8 B | 5.884 | 19 |
| [0x418](#id-418) | 핸들 열선 후보 | 8 B | 5.152 | 20 |
| [0x419](#id-419) | 무결성 및 메시지 생존번호 | 8 B | 16.503 | 12 |
| [0x41C](#id-41c) | 주변광 센서 | 8 B | 19.198 | 23 |
| [0x429](#id-429) | 무결성 및 메시지 생존번호 | 8 B | 5.000 | 16 |
| [0x435](#id-435) | 필터링 차속 후보 | 8 B | 8.162 | 20 |
| [0x448](#id-448) | 핸들 부가버튼 후보 | 8 B | 5.000 | 16 |
| [0x472](#id-472) | 무결성 및 메시지 생존번호 | 8 B | 5.004 | 16 |
| [0x473](#id-473) | 무결성 및 메시지 생존번호 | 8 B | 5.004 | 16 |
| [0x474](#id-474) | 무결성 및 메시지 생존번호 | 8 B | 5.004 | 16 |
| [0x47F](#id-47f) | 공조 터치버튼 | 8 B | 5.000 | 11 |
| [0x4A3](#id-4a3) | 지도 제한속도 및 도로정보 | 8 B | 4.953 | 33 |
| [0x4B4](#id-4b4) | 경로 위치 후보 | 8 B | 9.773 | 24 |
| [0x4B8](#id-4b8) | 내비 경로정보 후보 | 8 B | 0.181 | 21 |
| [0x4B9](#id-4b9) | 내비 도로구간 후보 | 8 B | 0.183 | 26 |
| [0x4BA](#id-4ba) | 내비 짧은 프로파일 후보 | 8 B | 1.329 | 61 |
| [0x4BE](#id-4be) | 내비 긴 프로파일 및 이벤트 후보 | 8 B | 0.101 | 59 |
| [0x4BF](#id-4bf) | 내비 경로정보 후보 | 8 B | 1.325 | 21 |
| [0x4D8](#id-4d8) | 계기판 단위 | 8 B | 4.942 | 1 |
| [0x4EB](#id-4eb) | 차량 시계 | 8 B | 4.942 | 17 |

<a id="id-035"></a>
## 0x035 페달 및 구동부하

가속페달은 raw 0~255 인코딩. 관측 0~112. 기어 5=D는 소스 정의. 구동부하의 Nm 단위 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~65535 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 40~47 | ACCELERATOR_PEDAL | 가속페달 위치 | 부호 없음 / LE | raw × 1 / 원시값 | 0~112 | 정의, 변화값 |
| 96~109 | FRONT_DRIVE_LOAD_COPY_RAW | 전륜 구동부하 복제값 | 부호 있음 / LE | raw × 1 / 원시값 | -548~1126 | 연동 후보 |
| 112~125 | REAR_DRIVE_LOAD_COPY_RAW | 후륜 구동부하 복제값 | 부호 있음 / LE | raw × 1 / 원시값 | -1022~1182 | 연동 후보 |
| 192~194 | GEAR | 기어 위치 | 부호 없음 / LE | raw × 1 / 코드 | 5 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L43), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L44), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L45), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L46).

<a id="id-04a"></a>
## 0x04A 관성센서

요레이트와 횡가속도의 고장/초기화 비트, IMU 재설정 비트를 원본 주석으로 세분화. 관측 0 고정. 신호 상태 0 고정. 온도값 106~107의 섭씨 변환 미확정. 수직가속도는 8비트 정의와 새 16비트 후보 병기. 식별번호 값과 통계 생략.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | IMU_Crc1Val | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 1~65533 | 전수 확인 |
| 16~23 | IMU_AlvCnt1Val | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~27 | IMU_YawSigSta | 요레이트 신호 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 24 | IMU_YAW_FAILURE_FLAG | 요레이트 고장 비트 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 25 | IMU_YAW_INITIALIZATION_FLAG | 요레이트 초기화 비트 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 28~31 | IMU_LatAccelSigSta | 횡가속도 신호 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 28 | IMU_LATERAL_FAILURE_FLAG | 횡가속도 고장 비트 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 29 | IMU_LATERAL_INITIALIZATION_FLAG | 횡가속도 초기화 비트 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 32~35 | IMU_LongAccelSigSta | 종가속도 신호 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36~39 | IMU_VerAccelSigSta | 수직가속도 신호 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 40~43 | IMU_McuVoltSta | IMU 전원 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 44~47 | IMU_AcuRstSta | IMU 재설정 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 44 | IMU_RESET_FLAG | IMU 재설정 비트 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 48~55 | IMU_SnsrTyp | IMU 센서 종류 | 부호 없음 / LE | raw × 1 / 원시값 | 8 고정 | 정의, 고정값 |
| 56~63 | IMU_RollSigSta | 롤레이트 신호 상태 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 64~79 | IMU_YawRtVal | 요레이트 | 부호 없음 / LE | raw × 0.005 + -163.84 / deg/s | -23.45~23.83 | GPS 궤적 연동 |
| 80~95 | IMU_LatAccelVal | 횡가속도 | 부호 없음 / LE | raw × 0.000127465 + -4.17677 / g | -0.238232~0.255567 | 정의, 변화값 |
| 96~111 | IMU_LongAccelVal | 종가속도 | 부호 없음 / LE | raw × 0.000127465 + -4.17677 / g | -0.293679~0.225231 | 정의, 변화값 |
| 112~127 | IMU_SnsrTempVal | IMU 온도 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 106~107 | 정의, 변화값 |
| 128~191 | IMU_SeralNumVal | IMU 식별번호 | 부호 없음 / LE | raw × 1 / 원시값 | 값 생략 | 정의 확인 |
| 192~207 | IMU_RollRtVal | 롤레이트 | 부호 없음 / LE | raw × 0.005 + -163.84 / deg/s | -9.08~11.925 | 정의, 변화값 |
| 208~215 | IMU_VerAccelVal | 수직가속도 하위 바이트 | 부호 없음 / LE | raw × 1 / 원시값 | 1~255 | 정의, 변화값 |
| 208~223 | VERTICAL_ACCEL_16BIT_CANDIDATE | 수직가속도 후보 | 부호 없음 / LE | raw × 0.000127465 + -4.17677 / g 추정 | 0.676712~1.37764 | 단위 추정 |

소스: [근거 1](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L20), [근거 2](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L21), [근거 3](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L22), [근거 4](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L23), [근거 5](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L24), [근거 6](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L25), [근거 7](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L26), [근거 8](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L27), [근거 9](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L28), [근거 10](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L29), [근거 11](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L30), [근거 12](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L31), [근거 13](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L32), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L33), [근거 15](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L34), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L35), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L36), [근거 18](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L756), [근거 19](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L757), [근거 20](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L758).

동시 기록 참조/비트 검증:
- 요레이트:  독립 bag 참조 r=0.992146; 시간 정렬 파라미터는 하드웨어 지연 측정값이 아님.
- 횡가속도: DBC g 환산. GPS 궤적 가속도와 관련되지만 동일 값은 아님. 중력 투영, 장착 자세, 미분/필터 차이 분리 없이 스케일을 임의 변경하지 않음.
- 종가속도: DBC g 환산. GPS 궤적 가속도와 관련되지만 동일 값은 아님. 중력 투영, 장착 자세, 미분/필터 차이 분리 없이 스케일을 임의 변경하지 않음.

<a id="id-060"></a>
## 0x060 차체자세제어 및 오토홀드

제동압력 10/11비트 대안은 관측값 동일. 추가 상위 비트가 고정 0이므로 실제 폭 미확정. 압력 단위 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 19~65514 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 42~44 | TRACTION_AND_STABILITY_CONTROL | 차체자세제어 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128~138 | BRAKE_PRESSURE | 제동압력 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~277 | 정의, 변화값 / 대안 |
| 128~137 | BRAKE_PRESSURE | 제동압력 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~277 | 정의, 변화값 / 대안 |
| 148 | BRAKE_PRESSED | 브레이크 입력 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 192~193 | AVH_Sta | 오토홀드 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~2 | 정의, 변화값 |
| 218~219 | AVH_I_LAMP | 오토홀드 표시등 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 220~222 | AVH_LAMP | 오토홀드 표시 상태 | 부호 없음 / LE | raw × 1 / 코드 | 2~3 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L91), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L92), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L93), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L94), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L95), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L96), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L97), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L98), [근거 9](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L42).

<a id="id-065"></a>
## 0x065 제동 및 모터 회전수 후보

모터 회전수 복제 후보는 양수 구간 위주. 상위 비트와 역회전 부호 인코딩 미검증.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~65534 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 29 | BRAKE_LIGHT | 제동등 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 40~55 | BRAKE_POSITION | 브레이크 위치 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | 11~384 | 정의, 변화값 |
| 57 | BRAKE_PRESSED | 브레이크 입력 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 200~211 | FRONT_MOTOR_SPEED_PROXY_RPM | 전륜 모터 회전수 복제 후보 | 부호 없음 / LE | raw × 8 / rpm | 0~3856 | 연동 후보 |
| 212~223 | REAR_MOTOR_SPEED_PROXY_RPM | 후륜 모터 회전수 복제 후보 | 부호 없음 / LE | raw × 8 / rpm | 0~5096 | 연동 후보 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L101), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L102), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L103), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L104), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L105).

<a id="id-06f"></a>
## 0x06F 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 157~65376 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-090"></a>
## 0x090 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-0a0"></a>
## 0x0A0 네 바퀴 속도

새 회전 누적값 4개는 0~254 순환. 증가량과 바퀴속도 적분이 연동하고, 좌우 차이도 연동. 절대거리 원점과 펄스당 거리의 독립 검증 없음. 14/16비트 속도 대안은 상위 2비트가 0이어서 관측값 동일. 후진 주행과 바퀴 위치의 독립 실험 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 1~65535 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~31 | WHEEL_ROTATION_ACCUMULATOR_24 | 왼쪽 앞바퀴 회전 누적값 후보 | 부호 없음 / LE | raw × 1 / 순환 카운트 | 0~254 | 연동 후보 |
| 32~39 | WHEEL_ROTATION_ACCUMULATOR_32 | 오른쪽 앞바퀴 회전 누적값 후보 | 부호 없음 / LE | raw × 1 / 순환 카운트 | 0~254 | 연동 후보 |
| 40~47 | WHEEL_ROTATION_ACCUMULATOR_40 | 왼쪽 뒷바퀴 회전 누적값 후보 | 부호 없음 / LE | raw × 1 / 순환 카운트 | 0~254 | 연동 후보 |
| 48~55 | WHEEL_ROTATION_ACCUMULATOR_48 | 오른쪽 뒷바퀴 회전 누적값 후보 | 부호 없음 / LE | raw × 1 / 순환 카운트 | 0~254 | 연동 후보 |
| 56 | MOVING_FORWARD | 전진 상태 1 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 57 | MOVING_BACKWARD | 후진 상태 1 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 58 | MOVING_FORWARD2 | 전진 상태 2 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 59 | MOVING_BACKWARD2 | 후진 상태 2 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64~79 | WHEEL_SPEED_1 | 왼쪽 앞바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.375 | 정의, 변화값 |
| 64~77 | WHL_SpdFLVal | 왼쪽 앞바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.375 | 정의, 변화값 |
| 80~95 | WHEEL_SPEED_2 | 오른쪽 앞바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.1875 | 정의, 변화값 |
| 80~93 | WHL_SpdFRVal | 오른쪽 앞바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.1875 | 정의, 변화값 |
| 96~111 | WHEEL_SPEED_3 | 왼쪽 뒷바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65 | 정의, 변화값 |
| 96~109 | WHL_SpdRLVal | 왼쪽 뒷바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65 | 정의, 변화값 |
| 112~127 | WHEEL_SPEED_4 | 오른쪽 뒷바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.0938 | 정의, 변화값 |
| 112~125 | WHL_SpdRRVal | 오른쪽 뒷바퀴 속도 | 부호 없음 / LE | raw × 0.03125 / km/h | 0~65.0938 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L121), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L122), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L123), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L124), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L125), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L126), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L127), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L128), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L129), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L130), [근거 11](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L63), [근거 12](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L64), [근거 13](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L65), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L66).

동시 기록 참조/비트 검증:
- 왼쪽 앞바퀴 속도: 네 바퀴 평균속도를 GPS 위치 차분 속도 및 외부 레이더와 대조. 개별 바퀴 위치/타이어 크기의 직접 검증은 별도.
- 오른쪽 앞바퀴 속도: 네 바퀴 평균속도를 GPS 위치 차분 속도 및 외부 레이더와 대조. 개별 바퀴 위치/타이어 크기의 직접 검증은 별도.
- 왼쪽 뒷바퀴 속도: 네 바퀴 평균속도를 GPS 위치 차분 속도 및 외부 레이더와 대조. 개별 바퀴 위치/타이어 크기의 직접 검증은 별도.
- 오른쪽 뒷바퀴 속도: 네 바퀴 평균속도를 GPS 위치 차분 속도 및 외부 레이더와 대조. 개별 바퀴 위치/타이어 크기의 직접 검증은 별도.

<a id="id-0da"></a>
## 0x0DA 차속 및 구동부하 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65532 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 64~79 | VEHICLE_SPEED_MPS_CANDIDATE | 차속 후보 | 부호 없음 / LE | raw × 0.01 / m/s | 0.01~18.04 | GPS 및 레이더 연동 |
| 80~95 | FRONT_DRIVE_LOAD_RAW | 전륜 구동부하 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -548~1124 | 연동 후보 |
| 96~111 | REAR_DRIVE_LOAD_RAW | 후륜 구동부하 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -1022~1180 | 연동 후보 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc).

동시 기록 참조/비트 검증:
- 차속 후보: 다른 CAN 기준과 수치 연동 독립 bag 참조 r=0.999662; 시간 정렬 파라미터는 하드웨어 지연 측정값이 아님.

<a id="id-0ea"></a>
## 0x0EA 전동식 파워스티어링

MDPS 추정 조향각과 0x125는 같은 부호 변환에서 연동. 양/음 부호 대안 병기. 좌/우 물리 방향은 미검증. 주차보조 필드 존재는 지원 증명이 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~65535 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~26 | MDPS_WrngLmpSta | MDPS 경고등 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 27~29 | MDPS_Typ | MDPS 종류 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 30~31 | MDPS_EngIdleRpmReq | MDPS 전류소모 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 32~33 | MDPS_VsmPlgInSta | 차체안정화 기능 연결 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 34~35 | MDPS_VsmDfctvSta | 차체안정화 MDPS 고장 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36~37 | MDPS_VsmSigErrSta | 차체안정화 신호 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 38~39 | MDPS_PaPlugInSta | 주차보조 기능 연결 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 40~43 | MDPS_PaModeSta | 주차보조 조향 모드 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 44~45 | MDPS_PaCanfltSta | 주차보조 CAN 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 46~47 | MDPS_LkaPlgInSta | 차로보조 기능 연결 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 48 | LKA_ACTIVE | 차로보조 작동 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 48~49 | MDPS_LkaToiActvSta | 차로보조 토크개입 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 50~51 | MDPS_LkaToiUnblSta | 차로보조 토크개입 불가 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 52~53 | MDPS_LkaToiFltSta | 차로보조 토크개입 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 54 | LKA_FAULT | 차로보조 고장 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 54~55 | MDPS_LkaFailSta | 차로보조 고장 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 59~60 | MDPS_ResetOpSta | MDPS 재설정 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 61~63 | MDPS_CurrModVal | 조향감 모드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64~75 | STEERING_OUT_TORQUE | 조향 출력토크 환산값 | 부호 없음 / LE | raw × 0.1 + -204.8 / 원시 환산값 | -22~22.7 | 정의, 변화값 |
| 76~77 | MDPS_VsmActResp | 차체안정화 제어 응답 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 78~79 | Reprogram_State_MDPS | MDPS 재프로그램 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 80~92 | STEERING_COL_TORQUE | 운전자 조향토크 원시값 | 부호 없음 / LE | raw × 1 + -4095 / 원시 환산값 | -490~552 | 정의, 변화값 |
| 96~111 | STEERING_ANGLE | 조향각 | 부호 있음 / LE | raw × -0.1 / deg | 0 고정 | 정의, 고정값 |
| 96~111 | MDPS_PaStrAnglVal | 주차보조 조향각 | 부호 있음 / LE | raw × 0.1 / deg | 0 고정 | 정의, 고정값 |
| 112~113 | MDPS_LoamModSta | MDPS 기능제한 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 114~118 | MDPS_HDPSpprtSWVer | HDP 지원 버전 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 120~123 | MDPS_ADASAciActvSta | ADAS 각도제어 상태 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 124~125 | MDPS_ADASAciPluginSta | ADAS 각도제어 연결 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 126~127 | MDPS_ADAS_AciFltSig | ADAS 각도제어 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128~143 | STEERING_ANGLE_2 | MDPS 추정 조향각 | 부호 있음 / LE | raw × -0.1 / deg | -328.9~364.3 | 정의, 변화값 |
| 128~143 | MDPS_EstStrAnglVal | MDPS 추정 조향각 | 부호 있음 / LE | raw × 0.1 / deg | -364.3~328.9 | 정의, 변화값 |
| 144~145 | LFA2_ACTIVE | 추가 조향보조 작동 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 144~147 | MDPS_ADASAciActvSta_Lv2 | 추가 각도제어 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 148~150 | MDPS_ADAS_AciFltSig_Lv2 | 추가 각도제어 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 149 | LFA2_FAULT | 추가 조향보조 고장 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 152~153 | MDPS_TpaPlugInSta | TPA 기능 연결 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 154~155 | MDPS_TpaCanfltSta | TPA CAN 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 156~159 | MDPS_TpaModeSta | TPA 조향 모드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 160~175 | MDPS_TpaStrAnglVal | TPA 조향각 | 부호 있음 / LE | raw × 0.1 / deg | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L140), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L141), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L145), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L146), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L147), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L148), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L149), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L150), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L151), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L152), [근거 11](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L100), [근거 12](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L101), [근거 13](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L102), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L103), [근거 15](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L104), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L106), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L107), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L109), [근거 19](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L110), [근거 20](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L111), [근거 21](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L112), [근거 22](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L113), [근거 23](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L114), [근거 24](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L115), [근거 25](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L116), [근거 26](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L117), [근거 27](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L118), [근거 28](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L119), [근거 29](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L120), [근거 30](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L121), [근거 31](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L89), [근거 32](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L90), [근거 33](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L91), [근거 34](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L92), [근거 35](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L93), [근거 36](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L94), [근거 37](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L95), [근거 38](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L96), [근거 39](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L97), [근거 40](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L98), [근거 41](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L99).

<a id="id-0f5"></a>
## 0x0F5 구동 및 제동부하 후보

두 24비트 묶음 안에 전륜/후륜 12비트 부하 후보 각각 배치. 단일 24비트 물리량으로 해석 불가. 토크와 제동압력 단위 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 1~65535 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 40~51 | FRONT_LOAD_1_PROXY_COUNTS | 전륜 구동부하 채널 1 | 부호 있음 / LE | raw × 0.25 / 부하 원시값 | -135~277 | 연동 후보 |
| 52~63 | REAR_LOAD_1_PROXY_COUNTS | 후륜 구동부하 채널 1 | 부호 있음 / LE | raw × 0.25 / 부하 원시값 | -187~216 | 연동 후보 |
| 64~75 | FRONT_LOAD_2_PROXY_COUNTS | 전륜 구동부하 채널 2 | 부호 있음 / LE | raw × 0.25 / 부하 원시값 | -135~277 | 연동 후보 |
| 76~87 | REAR_LOAD_2_PROXY_COUNTS | 후륜 구동부하 채널 2 | 부호 있음 / LE | raw × 0.25 / 부하 원시값 | -187~216 | 연동 후보 |
| 96~111 | BRAKING_PRESSURE_OR_DEMAND_RAW | 제동부하 원시값 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 0~1500 | 추정 |

<a id="id-10a"></a>
## 0x10A 후륜 모터 후보

192번 이후의 부하 상세값과 40:10 부하가 연동. 12/14비트 대안의 값 동일. Nm/A 단위 및 요청값/측정값 구분 미확정. 후륜 회전수와 부하 후보. 전압은 공개 코드와 CAN 연동 근거. 모터 부하의 Nm와 A 단위 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65535 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 24~39 | REAR_MOTOR_SPEED_RPM_CANDIDATE | 후륜 모터 회전수 후보 | 부호 있음 / LE | raw × 1 / rpm | -33~5097 | 연동 후보 |
| 40~49 | REAR_MOTOR_LOAD_RAW | 후륜 모터 부하 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -190~216 | 연동 후보 |
| 128~143 | REAR_DC_LINK_VOLTAGE_V_CANDIDATE | 후륜 직류단 전압 후보 | 부호 없음 / LE | raw × 1 / V | 683~700 | 연동 후보 |
| 192~203 | MOTOR_LOAD_DETAIL_12BIT | 후륜 구동부하 상세값 후보 | 부호 있음 / LE | raw × 1 / 원시값 | -1039~1184 | 연동 후보 |
| 192~205 | MOTOR_LOAD_DETAIL_14BIT | 후륜 구동부하 상세값 후보 | 부호 있음 / LE | raw × 1 / 원시값 | -1039~1184 | 연동 후보 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), [근거 2](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp).

<a id="id-11a"></a>
## 0x11A 전방카메라 기능 상태

지도정보 요청 비트 96=분기정보, 97=곡률정보, 98=부가프로파일. 원본 주석 정의이며 이번 캡처 값 0 고정. 기능 의미 미확정. 무결성 및 순환번호는 확인 수준 별도 표시. 고정값은 미사용 증거가 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | FR_CMR_Crc1Val | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 44~65508 | 전수 확인 |
| 16~23 | FR_CMR_AlvCnt1Val | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~25 | HBA_SysOptSta | 상향등보조 장착 여부 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 26~28 | HBA_SysSta | 상향등보조 상태 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 29~30 | HBA_IndLmpReq | 상향등보조 표시등 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 31~32 | iHBAref_VehLftSta | 상향등보조 왼쪽 차량 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~2 | 정의, 변화값 |
| 33~34 | iHBAref_VehCtrSta | 상향등보조 중앙 차량 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~2 | 정의, 변화값 |
| 35~36 | iHBAref_VehRtSta | 상향등보조 오른쪽 차량 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 37~38 | iHBAref_ILLAmbtSta | 상향등보조 주변광 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 39~41 | FCA_Equip_MFC | 전방충돌보조 장착 형태 | 부호 없음 / LE | raw × 1 / 코드 | 4 고정 | 정의, 고정값 |
| 42~43 | HBA_OptUsmSta | 상향등보조 사용자 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 45~47 | FCAref_FusSta | 전방충돌보조 융합 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 48~49 | DAW_LVDA_PUDis | 앞차출발알림 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 50~51 | DAW_LVDA_OptUsmSta | 앞차출발알림 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 52~54 | DAW_OptUsmSta | 운전자주의경고 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 55~58 | DAW_SysSta | 운전자주의 수준 | 부호 없음 / LE | raw × 1 / 코드 | 5 고정 | 정의, 고정값 |
| 59~61 | DAW_WrnMsgSta | 운전자주의경고 메시지 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 62~63 | DAW_TimeRstReq | 운전자주의 타이머 초기화 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64~66 | DAW_SnstvtyModRetVal | 운전자주의경고 민감도 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 85~86 | FR_CMR_SCCEquipSta | 카메라 SCC 장착 여부 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 96~111 | FR_CMR_ReqADASMapMsgVal | ADAS 지도정보 요청 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 96 | NAVI_STUB_REQUEST_FLAG | 내비 분기정보 요청 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 97 | NAVI_CURVATURE_REQUEST_FLAG | 내비 곡률정보 요청 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 98 | NAVI_OTHER_PROFILE_REQUEST_FLAG | 내비 부가프로파일 요청 | 부호 없음 / LE | raw × 1 / 0/1 코드 | 0 고정 | 소스 정의 |
| 112~115 | FR_CMR_SwVer1Val | 카메라 소프트웨어 버전 1 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 120~127 | FR_CMR_SwVer2Val | 카메라 소프트웨어 버전 2 | 부호 없음 / LE | raw × 1 / 원시값 | 5 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L157), [근거 2](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L158), [근거 3](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L159), [근거 4](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L160), [근거 5](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L161), [근거 6](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L162), [근거 7](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L163), [근거 8](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L164), [근거 9](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L165), [근거 10](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L166), [근거 11](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L167), [근거 12](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L168), [근거 13](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L169), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L170), [근거 15](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L171), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L172), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L173), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L174), [근거 19](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L175), [근거 20](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L176), [근거 21](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L177), [근거 22](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L178), [근거 23](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L179), [근거 24](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L834).

<a id="id-120"></a>
## 0x120 전륜 모터 후보

192번 이후의 부하 상세값과 40:10 부하가 연동. 12/14비트 대안의 값 동일. Nm/A 단위 및 요청값/측정값 구분 미확정. 전륜 회전수와 부하 후보. 차량 이동 중 전륜 회전수 0 관측. 이를 클러치나 구동 해제의 확정 증거로 해석 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65530 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 24~39 | FRONT_MOTOR_SPEED_RPM_CANDIDATE | 전륜 모터 회전수 후보 | 부호 있음 / LE | raw × 1 / rpm | -29~3862 | 연동 후보 |
| 40~49 | FRONT_MOTOR_LOAD_RAW | 전륜 모터 부하 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -140~262 | 연동 후보 |
| 128~143 | FRONT_DC_LINK_VOLTAGE_V_CANDIDATE | 전륜 직류단 전압 후보 | 부호 없음 / LE | raw × 1 / V | 683~702 | 연동 후보 |
| 192~203 | MOTOR_LOAD_DETAIL_12BIT | 전륜 구동부하 상세값 후보 | 부호 있음 / LE | raw × 1 / 원시값 | -572~1066 | 연동 후보 |
| 192~205 | MOTOR_LOAD_DETAIL_14BIT | 전륜 구동부하 상세값 후보 | 부호 있음 / LE | raw × 1 / 원시값 | -572~1066 | 연동 후보 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), [근거 2](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp).

<a id="id-125"></a>
## 0x125 조향센서

동일 16비트 조향각에 -0.1과 +0.1 대안 존재. 조향각속도는 크기이며 부호 정보 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 6~65531 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~39 | STEERING_ANGLE | 조향각 | 부호 있음 / LE | raw × -0.1 / deg | -332~364.8 | 반대 부호 정의 |
| 24~39 | STEERING_ANGLE | 조향각 | 부호 있음 / LE | raw × 0.1 / deg | -364.8~332 | 좌회전 양수 근거 |
| 40~47 | STEERING_RATE | 조향각속도 크기 | 부호 없음 / LE | raw × 4 / deg/s | 0~268 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L197), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L198), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L199), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L200), [근거 5](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L184).

동시 기록 참조/비트 검증:
- 조향각: 현재 순방향 주행 기록에서 raw×+0.1 해석이 GPS 좌회전 양수와 일치. 반대 부호 정의는 우회전 양수 관례. 각도 명령/토크 출력 방향의 실차 검증과 구분.
- 조향각: 현재 순방향 주행 기록에서 raw×+0.1 해석이 GPS 좌회전 양수와 일치. 반대 부호 정의는 우회전 양수 관례. 각도 명령/토크 출력 방향의 실차 검증과 구분.

<a id="id-12a"></a>
## 0x12A 차로보조 요청 및 표시

토크와 가속도 요청은 명령값. 실제 조향력이나 ECU 실행 응답이 아님. 동일 영역에 각도제어와 경고 표시 대안 존재.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 1~65532 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~26 | LKA_MODE | 차로보조 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 27~28 | LKA_ACTIVE | 차로보조 작동 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 정의, 변화값 |
| 27~29 | LKA_RcgSta | 차선 인식 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 정의, 변화값 |
| 30~31 | LKA_LHLnWrnSta | 왼쪽 차로이탈 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 32 | LKA_WARNING | 차로이탈 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 32~33 | LKA_RHLnWrnSta | 오른쪽 차로이탈 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 34~35 | LKA_HndsoffSnd | 손놓음 경고음 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36~37 | LKA_StrSnd | 조향 경고음 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 38~39 | LKA_ICON | 차로보조 표시 상태 | 부호 없음 / LE | raw × 1 / 코드 | 1~3 | 정의, 변화값 |
| 38~40 | LKA_SysIndReq | 차로보조 표시 요청 | 부호 없음 / LE | raw × 1 / 코드 | 1~3 | 정의, 변화값 |
| 40 | FCA_SYSWARN | 전방충돌보조 경고 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 41~51 | TORQUE_REQUEST | 조향토크 요청값 | 부호 없음 / LE | raw × 1 + -1024 / 원시 환산값 | -186~94 | 정의, 변화값 |
| 52 | STEER_REQ | 조향개입 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 52~53 | ActToiSta | 조향토크 개입 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 54~55 | ToiFltSta | 조향토크 개입 오류 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 56 | LFA_BUTTON | 차로추종 버튼 신호 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 57~59 | BCA_Rear_WrnSta | 후측방충돌 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 60~63 | LKA_SysWrn | 차로보조 시스템 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64 | FCA_LO_WrnSta | 전방충돌 LO 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 68 | FCA_LS_WrnSta | 전방충돌 LS 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 72~73 | LKA_OnOffEquip2Sta | 차로보조 장착 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 76~77 | LKAS_ANGLE_ACTIVE | 조향각 제어 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 80 | HAS_LANE_SAFETY | 차로안전 기능 상태 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 80~81 | LKA_UsmMod | 차로안전 사용자 모드 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 82~95 | LKAS_ANGLE_CMD | 조향각 요청값 | 부호 있음 / LE | raw × -0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 82~84 | Info_PedtrnDst | 보행자 거리 코드 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 85~89 | ELK_SysFlrSta | 긴급차로유지 고장 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 90~92 | ELK_SymbDisp | 긴급차로유지 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 93 | FCA_ESA_WrnSta | 긴급회피조향 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 96~103 | LKAS_ANGLE_MAX_TORQUE | 각도제어 토크제한 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 101 | FCA_ESA_CtrlSta | 긴급회피조향 제어 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 104~111 | DampingGain | 조향 감쇠이득 | 부호 없음 / LE | raw × 1 / 원시값 | 40~117 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L203), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L204), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L205), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L206), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L207), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L208), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L209), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L210), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L211), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L212), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L216), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L217), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L218), [근거 14](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L219), [근거 15](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L220), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L191), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L192), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L193), [근거 19](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L194), [근거 20](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L195), [근거 21](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L196), [근거 22](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L198), [근거 23](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L199), [근거 24](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L201), [근거 25](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L202), [근거 26](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L203), [근거 27](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L204), [근거 28](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L205), [근거 29](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L206), [근거 30](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L207), [근거 31](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L208), [근거 32](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L209), [근거 33](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L210), [근거 34](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L211).

<a id="id-130"></a>
## 0x130 변속장치

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 232~65373 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 32~33 | PARK_BUTTON | 주차 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 40~42 | KNOB_POSITION | 변속 노브 위치 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 정의, 고정값 |
| 64~66 | GEAR | 기어 위치 | 부호 없음 / LE | raw × 1 / 코드 | 4 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L227), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L228), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L229), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L230), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L231).

<a id="id-145"></a>
## 0x145 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-15a"></a>
## 0x15A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 72~65495 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-160"></a>
## 0x160 자동긴급제동 설정

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 64~65525 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~25 | AEB_SETTING | 자동긴급제동 설정 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L237), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L238), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L239).

<a id="id-175"></a>
## 0x175 ESC 및 ACC 상태

aBasis는 ESC 기준가속도 후보. IMU와 연동하지만 차량 기울기와 필터 차이 가능. 이름만으로 가속도 요청 또는 측정 센서를 단정 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 2~65533 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 32~42 | aBasis | ESC 기준가속도 | 부호 없음 / LE | raw × 0.01 + -10.23 / m/s^2 | -2.09~1.75 | GPS 가속도 연동 |
| 48~58 | ACCEL_REF_ACC | ACC 기준가속도 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 64~65 | SCC_OptTyp | SCC 장착 형태 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 66~67 | ACCEnable | ACC 허용 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 68 | ACC_REQ | ACC 제어 요청 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 70~71 | SCC_ReqLimSta | SCC 요청제한 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 72~73 | ESC_StdStillVal | 정차 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 74~75 | BrakeLight | 제동등 상태 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 76~77 | ESC_DclEnblReq | 감속 허용 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 81 | DriverBraking | 운전자 제동 입력 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 84 | DriverBrakingLowSens | 저민감 제동 입력 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 86~87 | ESC_PrkBrkActvSta | 주차브레이크 작동 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 96~97 | FCA_EquipSta | 전방충돌보조 장착 여부 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 98~99 | FCA_AvlblSta | 전방충돌보조 가용 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L358), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L359), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L362), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L364), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L366), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L367), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L368), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L370), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L371), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L372), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L373), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L376), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L379), [근거 14](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L381), [근거 15](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L383), [근거 16](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L384).

<a id="id-180"></a>
## 0x180 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 234~65397 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-185"></a>
## 0x185 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 128~65311 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L418), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L419).

<a id="id-19a"></a>
## 0x19A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 4~65535 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-1a0"></a>
## 0x1A0 SCC 요청 및 대상 정보

거리/상대속도 기본값 후보 204.6 m와 34.6 m/s 조합 29,921회. ObjValid=1만으로 앞차 유효 판정 불가. 12/9비트 상대속도, 정차 요청 배치 등 대안 병기. ACCMode=0, 가속도 요청값=0 고정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 6~65531 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~34 | ACC_ObjDist | SCC 대상 거리 | 부호 없음 / LE | raw × 0.1 / m | 7.7~204.6 | 정의, 변화값 |
| 35~46 | ACC_ObjRelSpd | SCC 대상 상대속도 | 부호 없음 / LE | raw × 0.1 + -170 / m/s | -12.8~239.4 | 정의, 변화값 / 대안 |
| 35~43 | ACC_ObjRelSpd | SCC 대상 상대속도 | 부호 없음 / LE | raw × 0.1 + -16.4 / m/s | -12.8~34.6 | 정의, 변화값 / 대안 |
| 46 | ObjValid | 대상 유효성 비트 후보 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 47~55 | ACC_ObjLatPos | SCC 대상 횡위치 | 부호 있음 / LE | raw × 0.1 + -20 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 64~65 | SysFailState | SCC 고장 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 66 | MainMode_ACC | ACC 메인 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 68~70 | ACCMode | ACC 작동 모드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 72~73 | TakeOverReq | 운전자 인수 요청 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 74~76 | InfoDisplay | SCC 안내 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 76 | CRUISE_STANDSTILL | 크루즈 정차 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 77~78 | DriverAlert | 운전자 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 80~87 | ObjDistLevel | 대상 거리등급 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 88~91 | DISTANCE_SETTING | 차간거리 단계 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 / 대안 |
| 88~90 | DISTANCE_SETTING | 차간거리 단계 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 / 대안 |
| 96~103 | VSetDis | 크루즈 설정속도 | 부호 없음 / BE | raw × 1 / km/h or mph | 0 고정 | 정의, 고정값 |
| 104~105 | NSCCOper | 내비 연동 SCC 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 106~107 | NSCCOnOff | 내비 연동 SCC 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 108~110 | HUD_LEAD_INFO | 앞차 표시 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 112~114 | DriveMode | SCC 주행 모드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128~138 | aReqValue | 가속도 요청값 | 부호 없음 / LE | raw × 0.01 + -10.23 / m/s^2 | 0 고정 | 정의, 고정값 |
| 140~150 | aReqRaw | 가속도 원요청값 | 부호 없음 / LE | raw × 0.01 + -10.23 / m/s^2 | 0 고정 | 정의, 고정값 |
| 152~158 | JerkUpperLimit | 저크 상한 | 부호 없음 / BE | raw × 0.1 / 원시 환산값 | 0.8~2 | 정의, 변화값 |
| 160~166 | JerkLowerLimit | 저크 하한 | 부호 없음 / BE | raw × 0.1 / m/s^3 | 1 고정 | 정의, 고정값 |
| 168~173 | AccelLimitBandUpper | 가속도 상단 허용폭 | 부호 없음 / BE | raw × 0.02 / 원시 환산값 | 0~0.1 | 정의, 변화값 |
| 176~181 | AccelLimitBandLower | 가속도 하단 허용폭 | 부호 없음 / BE | raw × 0.02 / 원시 환산값 | 0~0.2 | 정의, 변화값 |
| 176~178 | OBJ_STATUS | 대상 상태 코드 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~2 | 정의, 변화값 |
| 184~185 | StopReq | 정차 요청 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 / 대안 |
| 184 | StopReq | 정차 요청 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 / 대안 |
| 192~202 | TARGET_DISTANCE | 목표거리 후보 | 부호 없음 / LE | raw × 0.1 / m | 204.6 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L422), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L423), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L424), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L425), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L426), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L428), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L429), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L430), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L431), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L432), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L433), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L434), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L435), [근거 14](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L436), [근거 15](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L437), [근거 16](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L438), [근거 17](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L439), [근거 18](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L440), [근거 19](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L441), [근거 20](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L442), [근거 21](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L443), [근거 22](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L444), [근거 23](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L445), [근거 24](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L446), [근거 25](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L447), [근거 26](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L449), [근거 27](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L378), [근거 28](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L380), [근거 29](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L387), [근거 30](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L389), [근거 31](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L403), [근거 32](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L405).

동시 기록 참조/비트 검증:
- SCC 대상 거리: 동시 기록된 MFI920 객체와 대응 탐색. 독립 구간의 거리/속도 동시 일치 17.4%, 장착 변환과 객체 선택 미확정. ObjValid=1의 기본값 조합 제외.
- SCC 대상 상대속도: 동시 기록된 MFI920 객체와 대응 탐색. 독립 구간의 거리/속도 동시 일치 17.4%, 장착 변환과 객체 선택 미확정. ObjValid=1의 기본값 조합 제외.
- SCC 대상 상대속도: 동시 기록된 MFI920 객체와 대응 탐색. 독립 구간의 거리/속도 동시 일치 17.4%, 장착 변환과 객체 선택 미확정. ObjValid=1의 기본값 조합 제외.
- 대상 유효성 비트 후보: 동시 기록된 MFI920 객체와 대응 탐색. 독립 구간의 거리/속도 동시 일치 17.4%, 장착 변환과 객체 선택 미확정. ObjValid=1의 기본값 조합 제외.

<a id="id-1aa"></a>
## 0x1AA 계기판 버튼 및 차속 후보

48~55는 raw×0.5 km/h의 보정 표시속도 후보로 갱신. 기존 raw×0.125 m/s 가설은 현재 표시에서 대체. 가속/감속 버튼 값은 소스 코드. 48:8의 차속 환산은 단위 가설. CLU_SPEED는 별도 필드이며 두 차속이 동일 신호라는 증거 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 5~65521 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 30 | DISTANCE_UNIT | 거리 단위 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 34 | ADAPTIVE_CRUISE_MAIN_BTN | SCC 메인 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36~38 | CRUISE_BUTTONS | 크루즈 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 7 고정 | 정의, 고정값 |
| 39 | LFA_BTN | 차로추종 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 41 | NORMAL_CRUISE_MAIN_BTN | 일반 크루즈 메인 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 48~55 | BIASED_DISPLAY_SPEED_KPH_CANDIDATE | 보정 표시속도 후보 | 부호 없음 / LE | raw × 0.5 / km/h 추정 | 0~67.5 | 센서 연동 후보 |
| 64~71 | CLU_SPEED | 계기판 속도 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 0~68 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L454), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L455), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L458), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L460), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L462), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L463), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L465), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L471).

동시 기록 참조/비트 검증:
- 보정 표시속도 후보: GPS 위치 차분과 외부 레이더 속도 대조. 표시값 D≈v+min(0.12v,2.5), v/D는 km/h. 독립 GPS 오차 0.310 km/h, 양쪽 레이더 약 0.28 km/h. 계기판 화면 직접 검증 없음.

<a id="id-1b0"></a>
## 0x1B0 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-1b5"></a>
## 0x1B5 카메라 차선 및 앞차 후보

차선 품질/위치/곡률은 변화. 품질 0~3의 임계값과 포화 코드 해석은 미확정. 앞차 코드와 거리/속도 원시값은 0 고정. 앞차 속도 환산 -100 m/s는 실제 측정으로 사용 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 7~65534 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~26 | LEFT_QUAL | 왼쪽 차선 품질 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 정의, 변화값 |
| 27~28 | LEFT_LDW | 왼쪽 차로이탈 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 29~42 | LEFT_POSITION | 왼쪽 차선 횡위치 | 부호 있음 / LE | raw × 0.0039625 / m | -2.02484~0.594375 | 정의, 변화값 |
| 43~52 | LEFT_HEADING | 왼쪽 차선 방향각 | 부호 있음 / LE | raw × 0.000976563 / rad | -0.0605469~0.0605469 | 정의, 변화값 |
| 64~79 | LEFT_CURVATURE | 왼쪽 차선 곡률 | 부호 있음 / LE | raw × 1e-06 / 1/m | -0.003913~0.006753 | 정의, 변화값 |
| 80~95 | LEFT_CURVATURE_DERIVATIVE | 왼쪽 차선 곡률변화율 | 부호 있음 / LE | raw × 4e-09 / 1/m² | -0.000131068~0.000131068 | 정의, 변화값 |
| 96~98 | RIGHT_QUAL | 오른쪽 차선 품질 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 정의, 변화값 |
| 99~100 | RIGHT_LDW | 오른쪽 차로이탈 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 101~114 | RIGHT_POSITION | 오른쪽 차선 횡위치 | 부호 있음 / LE | raw × 0.0039625 / m | -0.106987~2.02484 | 정의, 변화값 |
| 115~124 | RIGHT_HEADING | 오른쪽 차선 방향각 | 부호 있음 / LE | raw × 0.000976563 / rad | -0.0605469~0.0605469 | 정의, 변화값 |
| 128~143 | RIGHT_CURVATURE | 오른쪽 차선 곡률 | 부호 있음 / LE | raw × 1e-06 / 1/m | -0.004041~0.032767 | 정의, 변화값 |
| 144~159 | RIGHT_CURVATURE_DERIVATIVE | 오른쪽 차선 곡률변화율 | 부호 있음 / LE | raw × 4e-09 / 1/m² | -0.000131068~0.000131068 | 정의, 변화값 |
| 192~198 | LEAD | 카메라 앞차 코드 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 192~199 | ID_CIPV | 주행경로 앞차 식별값 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 200~211 | LEAD_SPEED | 카메라 앞차 속도 후보 | 부호 없음 / LE | raw × 0.05 + -100 / m/s | -100 고정 | 정의, 고정값 |
| 212~223 | LEAD_DISTANCE | 카메라 앞차 거리 후보 | 부호 없음 / LE | raw × 0.05 / m | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L481), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L482), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L483), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L484), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L485), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L486), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L487), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L488), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L489), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L490), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L491), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L492), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L493), [근거 14](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L494), [근거 15](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L495), [근거 16](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L496), [근거 17](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L497), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L453).

동시 기록 참조/비트 검증:
- 왼쪽 차선 곡률: 카메라 차선의 기하량에 대한 소스 정의. 이번 GPS 자차 궤적 곡률과 단순 대응은 검증되지 않음.
- 오른쪽 차선 곡률: 카메라 차선의 기하량에 대한 소스 정의. 이번 GPS 자차 궤적 곡률과 단순 대응은 검증되지 않음.

<a id="id-1ba"></a>
## 0x1BA 후측방충돌보조

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 1~65498 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24 | LEFT_BLOCKED | 왼쪽 후측방 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 28~29 | BCW_OnOffEquipSta | 후측방경고 장착 여부 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 30~31 | INDICATOR_LEFT_TWO | 왼쪽 경고 코드 2 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 32~33 | INDICATOR_RIGHT_TWO | 오른쪽 경고 코드 2 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 34~35 | BCW_LtSndWrngSta | 왼쪽 후측방 경고음 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36~45 | BCW_RtSndWrngSta | 오른쪽 후측방 경고음 | 부호 없음 / LE | raw × 1 / 원시값 | 0~480 | 정의, 변화값 |
| 41~46 | FL_INDICATOR | 왼쪽 앞측방 경고 | 부호 없음 / BE | raw × 1 / 원시값 | 0~45 | 정의, 변화값 |
| 49~54 | FR_INDICATOR | 오른쪽 앞측방 경고 | 부호 없음 / BE | raw × 1 / 원시값 | 0~45 | 정의, 변화값 |
| 56~58 | BCW_SnstvtyModRetVal | 후측방경고 민감도 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 59~61 | BCW_IndSta | 후측방경고 표시 상태 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 62~63 | BCA_OnOffEquip2Sta | 후측방충돌방지 장착 여부 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64 | RIGHT_BLOCKED | 오른쪽 후측방 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 68 | COLLISION_AVOIDANCE_ACTIVE | 후측방충돌방지 작동 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 76~79 | BCA_DRV_WarnSta | 후측방 운전자 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 80~83 | BCA_Plus_Deccel_Req | 후측방 감속 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 84~85 | BCA_Plus_BrkCmdSta | 후측방 제동명령 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 86~87 | BCA_Plus_LtWrngSta | 왼쪽 후측방방지 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 88~92 | BCA_Plus_RtWrngSta | 오른쪽 후측방방지 경고 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 93~95 | BCA_Plus_FuncStat | 후측방방지 기능 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 96 | BCA_Plus_Sta | 후측방방지 작동 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 110~111 | Brake_Control_RL | 왼쪽 후륜 제동제어 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 118~119 | Brake_Control_RR | 오른쪽 후륜 제동제어 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128 | INDICATOR_LEFT_THREE | 왼쪽 경고 코드 3 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128~129 | OSMrrLamp_LtIndSta | 왼쪽 미러 경고등 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 130 | INDICATOR_RIGHT_THREE | 오른쪽 경고 코드 3 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 130~135 | OSMrrLamp_RtIndSta | 오른쪽 미러 경고등 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 138 | INDICATOR_LEFT_FOUR | 왼쪽 경고 코드 4 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 141 | INDICATOR_RIGHT_FOUR | 오른쪽 경고 코드 4 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L516), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L517), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L518), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L519), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L520), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L521), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L522), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L523), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L524), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L525), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L526), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L527), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L528), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L461), [근거 15](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L464), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L465), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L466), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L467), [근거 19](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L468), [근거 20](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L471), [근거 21](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L472), [근거 22](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L473), [근거 23](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L474), [근거 24](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L475), [근거 25](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L476), [근거 26](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L477), [근거 27](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L478), [근거 28](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L479), [근거 29](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L480), [근거 30](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L481).

<a id="id-1cf"></a>
## 0x1CF 핸들 버튼

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | _CHECKSUM | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 32~254 | 전수 확인 |
| 12~15 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 확인 |
| 16~18 | CRUISE_BUTTONS | 크루즈 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 19 | ADAPTIVE_CRUISE_MAIN_BTN | SCC 메인 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 21 | NORMAL_CRUISE_MAIN_BTN | 일반 크루즈 메인 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 23 | LFA_BTN | 차로추종 버튼 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 25 | RIGHT_PADDLE | 오른쪽 패들 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 27 | LEFT_PADDLE | 왼쪽 패들 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L531), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L532), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L533), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L534), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L535), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L536), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L537), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L538).

<a id="id-1da"></a>
## 0x1DA 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 65~65283 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L543), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L544).

<a id="id-1e0"></a>
## 0x1E0 고속도로 및 차로추종보조 표시

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 52~65410 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~26 | HDA_OptUsmSta | 고속도로보조 설정 | 부호 없음 / LE | raw × 1 / 코드 | 2 고정 | 정의, 고정값 |
| 27~29 | LFA_OptUsmSta | 차로추종보조 설정 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 30~31 | HDA_CntrlModSta | 고속도로보조 제어 모드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 31 | HDA_ICON | 고속도로보조 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 32~34 | HDA_InfoPUDis | 고속도로보조 안내 1 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 35~36 | HDA_AutoSetSpdSta | 제한속도 자동설정 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 37~38 | HDA_AutoSetSpdUpdtSta | 제한속도 자동갱신 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 39~46 | HDA_AutoSetSpdVal | 자동설정 속도 | 부호 없음 / LE | raw × 1 / km/h | 0 고정 | 정의, 고정값 |
| 47~48 | HDA_LFA_SymSta | 고속도로 차로추종 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 정의, 변화값 |
| 49~50 | HDA_LFA_WrnSnd | 고속도로 차로추종 경고음 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 51~53 | HDA_InfoPUDis1 | 고속도로보조 안내 2 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 54~55 | HDA_TDMRMDclReq | 고속도로보조 감속 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L549), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L550), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L551), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L552), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L553), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L554), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L555), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L556), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L557), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L558), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L559), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L560), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L561), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L506).

<a id="id-1e5"></a>
## 0x1E5 후진 상태 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~65527 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24 | REVERSING | 후진 상태 후보 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L564), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L565), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L566).

<a id="id-1ea"></a>
## 0x1EA 주행보조 화면 표시 후보

측방 대상 관련 필드는 0 고정, 표시용 차선 위치는 15 고정. 실제 레이더 거리/객체 목록으로 사용 불가. 여러 겹치는 배치 대안 존재.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 193~65411 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 29 | LEFT_BLINK_HOLD | 왼쪽 방향지시 유지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 30 | RIGHT_BLINK_HOLD | 오른쪽 방향지시 유지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 32~34 | HDA_MODE2 | 추가 고속도로보조 모드 | 부호 없음 / LE | raw × 1 / 코드 | 5 고정 | 정의, 고정값 |
| 35~36 | LANE_LEFT | 왼쪽 차선 표시 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 40~41 | LANE_RIGHT | 오른쪽 차선 표시 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 43~45 | LANE_CHANGING | 차선변경 상태 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 46~56 | LF_DETECT_DISTANCE | 왼쪽 앞측방 대상 거리 | 부호 없음 / LE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 64~70 | LF_DETECT_LATERAL | 왼쪽 앞측방 대상 횡위치 | 부호 없음 / BE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 72~74 | LF_DETECT | 왼쪽 앞측방 대상 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 75~85 | RF_DETECT_DISTANCE | 오른쪽 앞측방 대상 거리 | 부호 없음 / LE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 88~94 | RF_DETECT_LATERAL | 오른쪽 앞측방 대상 횡위치 | 부호 없음 / BE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 96~98 | RF_DETECT | 오른쪽 앞측방 대상 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 139~146 | LR_DETECT_DISTANCE | 왼쪽 뒤측방 대상 거리 | 부호 없음 / LE | raw × 0.1 / m | 0 고정 | 정의, 고정값 |
| 152~157 | LR_DETECT_LATERAL | 왼쪽 뒤측방 대상 횡위치 | 부호 없음 / LE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 160~162 | LR_DETECT | 왼쪽 뒤측방 대상 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 163~170 | RR_DETECT_DISTANCE | 오른쪽 뒤측방 대상 거리 | 부호 없음 / LE | raw × 0.1 / m | 0 고정 | 정의, 고정값 |
| 172~177 | RR_DETECT_LATERAL | 오른쪽 뒤측방 대상 횡위치 | 부호 없음 / LE | raw × 0.1 / 원시 환산값 | 0 고정 | 정의, 고정값 |
| 184~186 | RR_DETECT | 오른쪽 뒤측방 대상 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 204~207 | AUTOLANECHANGE_MSG | 자동 차선변경 안내 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 208~211 | LANELINE_CURVATURE | 표시용 차선 곡률 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 212 | LANELINE_CURVATURE_DIRECTION | 표시용 곡률 방향 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 232~239 | LANELINE_LEFT_POSITION | 표시용 왼쪽 차선 위치 | 부호 없음 / BE | raw × 1 / 원시값 | 15 고정 | 정의, 고정값 |
| 240~247 | LANELINE_RIGHT_POSITION | 표시용 오른쪽 차선 위치 | 부호 없음 / BE | raw × 1 / 원시값 | 15 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L577), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L578), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L583), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L584), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L586), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L587), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L589), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L590), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L591), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L592), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L593), [근거 12](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L594), [근거 13](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L595), [근거 14](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L596), [근거 15](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L598), [근거 16](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L599), [근거 17](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L600), [근거 18](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L601), [근거 19](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L602), [근거 20](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L603), [근거 21](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L604), [근거 22](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L605), [근거 23](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L606), [근거 24](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L607), [근거 25](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L608).

<a id="id-1f0"></a>
## 0x1F0 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 236~65369 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-1f5"></a>
## 0x1F5 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 8~65499 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-1fa"></a>
## 0x1FA 제한속도 및 교통표지

제한속도 관련 0/50 변화, 추가 표지와 스쿨존 변화. 0은 인식 없음 소스 코드이며 0 km/h 제한으로 해석 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | FR_CMR_Crc2Val | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 12~65528 | 전수 확인 |
| 16~23 | FR_CMR_AlvCnt2Val | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 24~25 | ISLW_OptUsmSta | 속도제한경고 설정 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 26~27 | ISLW_SysSta | 속도제한경고 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 28~30 | ISLW_NoPassingInfoDis | 추월금지 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 31~32 | ISLW_OvrlpSignDis | 중첩 표지판 표시 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 33~39 | SPEED_LIMIT_1 | 주 제한속도 코드 1 | 부호 없음 / BE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 33~40 | ISLW_SpdCluMainDis | 계기판 주 제한속도 | 부호 없음 / LE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 41~47 | SPEED_LIMIT_2 | 주 제한속도 코드 2 | 부호 없음 / BE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 41~48 | ISLW_SpdNaviMainDis | 내비 주 제한속도 | 부호 없음 / LE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 49~52 | ISLW_SubCondinfoSta1 | 제한속도 부가조건 1 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 53~56 | ISLW_SubCondinfoSta2 | 제한속도 부가조건 2 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 64~71 | ISLW_SpdCluSubMainDis | 계기판 보조 제한속도 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 72~79 | SECONDARY_LIMIT_1 | 보조 제한속도 코드 1 | 부호 없음 / BE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 72~79 | ISLW_SpdCluDisSubCond1 | 계기판 보조속도 조건 1 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 80~87 | ISLW_SpdCluDisSubCond2 | 계기판 보조속도 조건 2 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 88~95 | ISLW_SpdNaviSubMainDis | 내비 보조 제한속도 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 96~103 | SECONDARY_LIMIT_2 | 보조 제한속도 코드 2 | 부호 없음 / BE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 96~103 | ISLW_SpdNaviDisSubCond1 | 내비 보조속도 조건 1 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 104~111 | ISLW_SpdNaviDisSubCond2 | 내비 보조속도 조건 2 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 112~119 | SPEED_LIMIT_3 | 제한속도 코드 3 | 부호 없음 / BE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 112~119 | ISLA_SpdwOffst | 속도보조 관련 속도값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 120 | ARROW_DOWN | 감속 화살표 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 120~121 | ISLA_SwIgnoreReq | 속도설정 버튼 무시 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 121 | ARROW_UP | 가속 화살표 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 122~123 | ISLA_SpdChgReq | 설정속도 변경 요청 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 124~125 | ISLA_SpdWrn | 속도 초과 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 126~127 | ISLA_IcyWrn | 노면결빙 경고 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 128~130 | ISLA_SymFlashMod | 속도보조 표시 깜빡임 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 129 | SPEED_CHANGE_BLINKING | 속도변경 깜빡임 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 131~133 | ISLA_Popup | 속도보조 팝업 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 133 | CHIME_1 | 속도보조 알림음 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 136~138 | ISLA_OptUsmSta | 속도제한보조 설정 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 정의, 고정값 |
| 139~141 | ISLA_OffstUsmSta | 제한속도 보정 설정 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 정의, 고정값 |
| 142~143 | ISLA_AutoUsmSta | 속도제한 자동설정 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 144~147 | ISLA_Cntry | 속도보조 국가 설정 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 149~153 | ISLA_AddtnlSign | 추가 교통표지 종류 | 부호 없음 / LE | raw × 1 / 원시값 | 0~26 | 정의, 변화값 |
| 154~155 | ISLA_SchoolZone | 스쿨존 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 155 | SCHOOL_ZONE | 스쿨존 표시 비트 후보 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 208~215 | SPEED_LIMIT_4 | 제한속도 코드 4 | 부호 없음 / BE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L612), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L613), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L614), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L615), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L616), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L617), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L618), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L620), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L621), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L625), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L626), [근거 12](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L553), [근거 13](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L554), [근거 14](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L555), [근거 15](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L556), [근거 16](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L557), [근거 17](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L558), [근거 18](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L559), [근거 19](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L560), [근거 20](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L561), [근거 21](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L562), [근거 22](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L563), [근거 23](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L564), [근거 24](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L565), [근거 25](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L566), [근거 26](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L567), [근거 27](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L568), [근거 28](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L569), [근거 29](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L570), [근거 30](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L571), [근거 31](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L572), [근거 32](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L573), [근거 33](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L574), [근거 34](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L575), [근거 35](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L576), [근거 36](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L577), [근거 37](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L578), [근거 38](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L579), [근거 39](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L580), [근거 40](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L581).

<a id="id-200"></a>
## 0x200 차간시간 설정 후보

차간시간 단계와 원시 바이트 대안 병기. 관측 고정값만으로 실제 시간 단위 확정 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 17~65422 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 32~34 | TauGapSet | 차간시간 단계 후보 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 정의, 고정값 |
| 40~42 | TauGapSet_ | 차간시간 대안 코드 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L634), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L635), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L637), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L640).

<a id="id-20a"></a>
## 0x20A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 16~65445 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-225"></a>
## 0x225 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 1~65533 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-235"></a>
## 0x235 고전압 전압 후보

전압 환산 /10은 다른 CAN 전압과 연동하는 가설. 이 ID를 레이더 목록으로 해석한 다른 버스 DBC는 적용하지 않음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 13~65521 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 104~119 | HV_VOLTAGE_V_CANDIDATE | 고전압 전압 후보 1 | 부호 없음 / LE | raw × 0.1 / V | 685.5~701.3 | 단위 추정 |

소스: [근거 1](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp).

<a id="id-250"></a>
## 0x250 전력 관련 후보

10비트 전력 관련 원시값과 kW 환산 가설은 같은 비트의 대안. 두 독립 측정값이 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65525 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 168~177 | SIGNED_POWER_LOAD_RAW | 전력 관련 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -56~47 | 연동 후보 |
| 168~177 | POWER_KW_WORKING_HYPOTHESIS | 전력 환산 가설 | 부호 있음 / LE | raw × 1 / kW | -56~47 | 단위 추정 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc).

<a id="id-255"></a>
## 0x255 고전압 전압 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65528 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 136~151 | HV_VOLTAGE_2_V_CANDIDATE | 고전압 전압 후보 2 | 부호 없음 / LE | raw × 0.1 / V | 682.1~698.7 | 단위 추정 |

<a id="id-25a"></a>
## 0x25A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 8~65533 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2aa"></a>
## 0x2AA 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 174~65516 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2b0"></a>
## 0x2B0 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2b5"></a>
## 0x2B5 가속페달 복제 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 5~65525 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 48~55 | ACCELERATOR_PEDAL_COPY_RAW | 가속페달 위치 복제값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~112 | 연동 후보 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc).

<a id="id-2c0"></a>
## 0x2C0 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 46~65482 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2d5"></a>
## 0x2D5 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 0~65425 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2e0"></a>
## 0x2E0 수동 속도제한보조

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 29~65499 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 26~27 | MSLA_STATUS | 수동 속도제한 상태 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 38 | MSLA_ENABLED | 수동 속도제한 허용 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 48~55 | MAX_SPEED | 수동 제한속도 | 부호 없음 / BE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |
| 144~151 | MAX_SPEED_COPY | 수동 제한속도 복제값 | 부호 없음 / LE | raw × 1 / 원시값 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L791), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L792), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L793), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L794), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L795), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L796).

<a id="id-2e5"></a>
## 0x2E5 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 201~65419 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-2fa"></a>
## 0x2FA 전류 및 전력 관련 후보

16비트 전류/전력 관련 원시값과 /10 A 가설은 같은 비트의 대안. 전력 가설과의 관계만으로 절대 단위 확정 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 26~65534 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 64~79 | SIGNED_ELECTRIC_CURRENT_OR_POWER_LOAD_RAW | 전류 또는 전력 원시값 | 부호 있음 / LE | raw × 1 / 원시값 | -759~698 | 연동 후보 |
| 64~79 | CURRENT_A_WORKING_HYPOTHESIS | 전류 환산 가설 | 부호 있음 / LE | raw × 0.1 / A | -75.9~69.8 | 단위 추정 |

소스: [근거 1](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp).

<a id="id-2ff"></a>
## 0x2FF 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 43523 고정 | 전수 확인 |

<a id="id-30a"></a>
## 0x30A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 24~65478 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-315"></a>
## 0x315 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 116~65473 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-31a"></a>
## 0x31A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-325"></a>
## 0x325 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 24~65533 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-330"></a>
## 0x330 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 2~65521 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-33a"></a>
## 0x33A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 200~65418 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-345"></a>
## 0x345 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 51~65452 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L799), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L800).

<a id="id-34a"></a>
## 0x34A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-360"></a>
## 0x360 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 19~65440 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-36a"></a>
## 0x36A 사각지대 감지

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | CHECKSUM | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 원시값 | 117~65472 | 전수 확인 |
| 16~23 | COUNTER | 순환번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~255 | 순차 확인 |
| 27 | RIGHT_BSD | 오른쪽 사각지대 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 28 | LEFT_BSD | 왼쪽 사각지대 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L843), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L844), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L845), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L846).

<a id="id-36f"></a>
## 0x36F 무결성 및 메시지 생존번호

CRC8 등가식과 0~14 생존번호 후보. 초기값/최종 XOR 분리 불가. 동일 payload 반복과 건너뛴 번호 존재, 원래 ECU 갱신 주기 미확정. 기능 의미 미확정. 무결성 및 순환번호는 확인 수준 별도 표시. 고정값은 미사용 증거가 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CRC8_1D_EQUIVALENT_CANDIDATE | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / CRC8 후보 | 32~254 | 구조 후보 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |

<a id="id-37f"></a>
## 0x37F 무결성 및 메시지 생존번호

CRC8 등가식과 0~14 생존번호 후보. 15 미관측. 본문 변화가 카운터에 집중되어 임의 본문에 대한 검증은 없음. 기능 의미 미확정. 무결성 및 순환번호는 확인 수준 별도 표시. 고정값은 미사용 증거가 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CRC8_1D_EQUIVALENT_CANDIDATE | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / CRC8 후보 | 34~252 | 구조 후보 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |

<a id="id-380"></a>
## 0x380 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 61739 고정 | 전수 확인 |

<a id="id-38a"></a>
## 0x38A 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 249~65356 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-39b"></a>
## 0x39B 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 26~29 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~15 | 순차 후보 |

<a id="id-3a0"></a>
## 0x3A0 타이어 공기압

공기압 표시값 36~40, 단위 코드 0. ajouatom 변환과 psi 가설 부합. 독립 압력계 비교 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 6~65465 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 32~39 | PRESSURE_FL | 왼쪽 앞타이어 공기압 | 부호 없음 / BE | raw × 1 / 표시값, psi 후보 | 36~38 | 정의, 변화값 |
| 40~47 | PRESSURE_FR | 오른쪽 앞타이어 공기압 | 부호 없음 / BE | raw × 1 / 표시값, psi 후보 | 36~37 | 정의, 변화값 |
| 48~55 | PRESSURE_RL | 왼쪽 뒷타이어 공기압 | 부호 없음 / BE | raw × 1 / 표시값, psi 후보 | 39~40 | 정의, 변화값 |
| 56~63 | PRESSURE_RR | 오른쪽 뒷타이어 공기압 | 부호 없음 / BE | raw × 1 / 표시값, psi 후보 | 39~40 | 정의, 변화값 |
| 64~66 | STATUS_TPMS | 타이어 공기압 상태 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 67~68 | UNIT | 공기압 단위 코드 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L854), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L855), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L856), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L857), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L858), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L859).

<a id="id-3a5"></a>
## 0x3A5 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 121~65510 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-3b5"></a>
## 0x3B5 직류단 전압 후보

공개 코드의 바이트 15~16 전압 위치는 이 캡처에서 43,903~47,999. 바이트 16~17에서 683~699 및 다른 CAN 전압 연동. 위치 대안을 혼동하지 않음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 8~65490 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |
| 128~143 | DC_LINK_VOLTAGE_ALIGNED_CANDIDATE | 직류단 전압 후보 | 부호 없음 / LE | raw × 1 / V 추정 | 683~699 | 연동 후보 |

<a id="id-3c1"></a>
## 0x3C1 방향지시 및 등화 레버

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CHECKSUM_MAYBE | CRC8 무결성 값 후보 | 부호 없음 / BE | raw × 1 / 원시값 | 6~230 | 전수 확인 |
| 12~15 | COUNTER_ALT | 순환번호 | 부호 없음 / BE | raw × 1 / 코드 | 0~12 | DBC 정의 |
| 18 | HIGHBEAM_FORWARD | 상향등 레버 고정 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 20~21 | LIGHT_KNOB_POSITION | 등화 스위치 위치 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 26 | HIGHBEAM_BACKWARD | 상향등 레버 당김 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 30 | LEFT_BLINKER | 왼쪽 방향지시 레버 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 32 | RIGHT_BLINKER | 오른쪽 방향지시 레버 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L862), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L863), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L864), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L865), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L866), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L867), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L868).

<a id="id-3ca"></a>
## 0x3CA 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 61211 고정 | 전수 확인 |

<a id="id-3e5"></a>
## 0x3E5 무결성 및 메시지 생존번호

CRC8 등가식 후보는 관측 고유 본문 4개에서 일치. 캡처 내부 본문 변화 없음, 일반화 근거 제한. 기능 의미 미확정. 무결성 및 순환번호는 확인 수준 별도 표시. 고정값은 미사용 증거가 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CRC8_1D_EQUIVALENT_CANDIDATE | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / CRC8 후보 | 138~253 | 구조 후보 |

<a id="id-3f0"></a>
## 0x3F0 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 14~65523 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-3f5"></a>
## 0x3F5 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 74~65382 | 전수 확인 |
| 16~23 | OBSERVED_COUNTER | 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 순차 후보 |

<a id="id-411"></a>
## 0x411 문 및 안전벨트

문 닫힘과 운전석 벨트 잠김은 소스 enum 해석. 해당 장치의 반복 조작 검증 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CHECKSUM_MAYBE | CRC8 무결성 값 후보 | 부호 없음 / BE | raw × 1 / 원시값 | 55 고정 | DBC 정의 |
| 12~15 | COUNTER_ALT | 순환번호 | 부호 없음 / BE | raw × 1 / 코드 | 9 고정 | DBC 정의 |
| 24 | DRIVER_DOOR | 운전석 문 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 34 | PASSENGER_DOOR | 조수석 문 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 36 | PASSENGER_SEATBELT | 조수석 안전벨트 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 42 | DRIVER_SEATBELT | 운전석 안전벨트 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 52 | DRIVER_REAR_DOOR | 왼쪽 뒷문 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 56 | PASSENGER_REAR_DOOR | 오른쪽 뒷문 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L881), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L882), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L883), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L884), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L885), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L886), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L887), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L888).

<a id="id-412"></a>
## 0x412 무결성 및 메시지 생존번호

생존번호 후보는 중복 payload를 제외하면 0~14 순환. 다른 소스의 바이트 6 브레이크 해석은 이 캡처와 불일치. 공개 다른 배치의 바이트 6 브레이크 해석은 해당 값 고정과 실제 브레이크 변화로 불일치.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | OBSERVED_observed_CRC8_equivalent | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 코드 | 2~249 | 전수 확인 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |

<a id="id-413"></a>
## 0x413 방향지시등

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | OBSERVED_observed_CRC8_equivalent | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 코드 | 2~253 | 전수 확인 |
| 8 | LEFT_STALK | 왼쪽 방향지시 입력 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 10 | RIGHT_STALK | 오른쪽 방향지시 입력 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 12~15 | COUNTER_ALT | 순환번호 | 부호 없음 / BE | raw × 1 / 0~14 순환 | 0~14 | 순차 확인 |
| 20 | LEFT_LAMP | 왼쪽 방향지시등 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 22 | RIGHT_LAMP | 오른쪽 방향지시등 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 59 | LEFT_LAMP_ALT | 대안 왼쪽 방향지시등 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 61 | RIGHT_LAMP_ALT | 대안 오른쪽 방향지시등 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 62 | USE_ALT_LAMP | 대안 방향지시등 선택 | 부호 없음 / BE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L891), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L892), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L893), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L894), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L895), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L896), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L897), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L898).

<a id="id-418"></a>
## 0x418 핸들 열선 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | OBSERVED_observed_CRC8_equivalent | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 코드 | 1~223 | 전수 확인 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |
| 16~23 | STEERING_HEAT_STATE_SOURCE_CANDIDATE | 핸들 열선 상태 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 고정 후보 |

소스: [근거 1](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc).

<a id="id-419"></a>
## 0x419 무결성 및 메시지 생존번호

CRC8 무결성 값과 0~14 생존번호 후보. 나머지 영역의 기능은 표시 표에서 제외. 기능 의미 미확정. 무결성 및 순환번호는 확인 수준 별도 표시. 고정값은 미사용 증거가 아님.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | OBSERVED_observed_CRC8_equivalent | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 전수 확인 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |

<a id="id-41c"></a>
## 0x41C 주변광 센서

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | CHECKSUM_MAYBE | CRC8 무결성 값 후보 | 부호 없음 / BE | raw × 1 / 원시값 | 0~255 | 전수 확인 |
| 12~15 | COUNTER_ALT | 순환번호 | 부호 없음 / BE | raw × 1 / 0~14 순환 | 0~14 | 순차 확인 |
| 19 | IS_DARK | 어두움 감지 | 부호 없음 / BE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |
| 24~33 | LIGHT_LEVEL | 주변광 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 256~652 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L901), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L902), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L903), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L904).

<a id="id-429"></a>
## 0x429 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 64759 고정 | 전수 확인 |

<a id="id-435"></a>
## 0x435 필터링 차속 후보

차속 후보 외에 0~14 생존번호 후보 추가. 동일 payload 반복은 새 ECU 측정값으로 해석하지 않음. 차속 후보는 저속 주행과 연동하지만 약 0.5초 상대 지연 및 정수 양자화. 빠른 차속 피드백에 사용하기 전 지연 검증 필요.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~7 | OBSERVED_observed_CRC8_equivalent | CRC8 무결성 값 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~255 | 전수 확인 |
| 12~15 | ALIVE_COUNTER_MOD15 | 메시지 생존번호 후보 | 부호 없음 / LE | raw × 1 / 0~14 순환 | 0~14 | 순차 후보 |
| 56~63 | FILTERED_SPEED_KPH_CANDIDATE | 필터링 차속 후보 | 부호 없음 / LE | raw × 1 / km/h | 0~64 | GPS 및 레이더 연동 |

소스: [근거 1](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc).

동시 기록 참조/비트 검증:
- 필터링 차속 후보: 공개 차량 소스와 CAN 연동 독립 bag 참조 r=0.999117; 시간 정렬 파라미터는 하드웨어 지연 측정값이 아님.

<a id="id-448"></a>
## 0x448 핸들 부가버튼 후보

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 16~23 | OTHER_BUTTONS_SOURCE_CANDIDATE | 핸들 부가버튼 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 고정 후보 |
| 40~47 | STAR_BUTTON_SOURCE_CANDIDATE | 즐겨찾기 버튼 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 고정 후보 |

소스: [근거 1](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc).

<a id="id-472"></a>
## 0x472 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 17053 고정 | 전수 확인 |

<a id="id-473"></a>
## 0x473 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 32210 고정 | 전수 확인 |

<a id="id-474"></a>
## 0x474 무결성 및 메시지 생존번호

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~15 | OBSERVED_Hyundai_CANFD_CRC16 | CRC16 무결성 값 | 부호 없음 / LE | raw × 1 / 코드 | 29329 고정 | 전수 확인 |

<a id="id-47f"></a>
## 0x47F 공조 터치버튼

모든 버튼 비트 1 고정. 버튼 눌림/놓임 극성은 실험 미검증. 1을 모든 버튼 동시 눌림으로 해석 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 8 | AUTO_BUTTON | 공조 자동 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 12 | SYNC_BUTTON | 공조 동기화 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 20 | FR_DEFROST_BUTTON | 앞유리 성에제거 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 22 | RR_DEFROST_BUTTON | 뒷유리 열선 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 24 | FAN_SPEED_UP_BUTTON | 풍량 증가 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 26 | FAN_SPEED_DOWN_BUTTON | 풍량 감소 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 28 | AIR_DIRECTION_BUTTON | 송풍 방향 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 40 | AC_BUTTON | 에어컨 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 44 | DRIVER_ONLY_BUTTON | 운전석 전용공조 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 48 | RECIRC_BUTTON | 내기순환 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 52 | HEAT_BUTTON | 난방 버튼 | 부호 없음 / BE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L911), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L912), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L913), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L914), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L915), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L916), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L917), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L918), [근거 9](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L919), [근거 10](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L920), [근거 11](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L921).

<a id="id-4a3"></a>
## 0x4A3 지도 제한속도 및 도로정보

지도 제한속도 0/50, 지도 출처 1/2, 터널 0/1. 국가 코드 410. 단위 코드와 실제 단위는 별도 확인.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~2 | LinkClass | 도로 등급 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 3~5 | Frwinfo | 전방도로 정보 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 6~7 | SpeedUnit | 제한속도 단위 코드 | 부호 없음 / LE | raw × 1 / 코드 | 1 고정 | 정의, 고정값 |
| 8~15 | SPEED_LIMIT | 지도 제한속도 | 부호 없음 / BE | raw × 1 / 원시값 | 0~50 | 정의, 변화값 |
| 16~25 | CountryCode | 지도 국가 코드 | 부호 없음 / LE | raw × 1 / 원시값 | 410 고정 | 정의, 고정값 |
| 27~29 | MapSource | 지도 출처 코드 | 부호 없음 / LE | raw × 1 / 코드 | 1~2 | 정의, 변화값 |
| 30~31 | TollExist | 유료도로 정보 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 38~39 | TunnelExist | 터널 정보 | 부호 없음 / LE | raw × 1 / 코드 | 0~1 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L924), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L925), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L926), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L927), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L928), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L929), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L930), [근거 8](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L931).

<a id="id-4b4"></a>
## 0x4B4 경로 위치 후보

내비 순환번호 위치 18~19 확인. 반복 CAN과 전부 FF 형태를 구분. 위치 후보 0~17, 평균속도 후보 0 고정. 차량 차속이 변하므로 위치 필드 원점과 평균속도 유효성 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~12 | POS_OFFSET | 경로 내 차량위치 후보 | 부호 없음 / LE | raw × 1 / m | 0~17 | 정의, 변화값 |
| 18~19 | NAVI_CYCLIC_COUNTER_OBSERVED_18 | 내비 프레임 순환번호 후보 | 부호 없음 / LE | raw × 1 / 0~3 순환 | 0~3 | 순차 확인 |
| 21~29 | POS_RANGE_AVG_SPEED | 경로구간 평균속도 후보 | 부호 없음 / LE | raw × 1 / km/h | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L935), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L936), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L937).

동시 기록 참조/비트 검증:
- 내비 프레임 순환번호 후보: 전부 FF 형태 제외, 캡처 경계 분리. 값이 바뀐 경우 +1 순환. 독립 캡처 변화 전이 788회. 표본이 적은 ID는 구조 후보.

<a id="id-4b8"></a>
## 0x4B8 내비 경로정보 후보

내비 순환번호 위치 45~46 확인. 반복 CAN과 전부 FF 형태를 구분. 유효 형태 59회, 전부 FF 60회. 경로 번호와 위치는 공통 모양에 따른 구조 후보이며 해당 ID 의미는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~12 | COMMON_PATH_OFFSET_CANDIDATE | 경로 위치 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 3284 고정 | 구조 후보 |
| 13~18 | COMMON_PATH_INDEX_CANDIDATE | 경로 번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 구조 후보 |
| 45~46 | NAVI_CYCLIC_COUNTER_OBSERVED_45 | 내비 프레임 순환번호 후보 | 부호 없음 / LE | raw × 1 / 0~3 순환 | 0~3 | 구조 후보 |

동시 기록 참조/비트 검증:
- 내비 프레임 순환번호 후보: 전부 FF 형태 제외, 캡처 경계 분리. 값이 바뀐 경우 +1 순환. 독립 캡처 변화 전이 4회. 표본이 적은 ID는 구조 후보.

<a id="id-4b9"></a>
## 0x4B9 내비 도로구간 후보

내비 순환번호 위치 45~46 확인. 반복 CAN과 전부 FF 형태를 구분. 유효 형태 60회, 전부 FF 60회. 계산경로 값은 0/2, 소스가 채택하는 값 1 없음. 소스 게이트를 무시한 도로등급 확정 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~12 | SEGMENT_OFFSET | 도로구간 시작위치 후보 | 부호 없음 / LE | raw × 1 / m | 0~1997 | 소스 후보 |
| 13~18 | SEGMENT_PATH_INDEX | 도로구간 경로 번호 | 부호 없음 / LE | raw × 1 / 원시값 | 0~8 | 소스 후보 |
| 22~23 | CALCULATED_ROUTE | 계산경로 상태 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0~2 | 소스 후보 |
| 24~26 | FUNCTIONAL_ROAD_CLASS | 기능별 도로등급 | 부호 없음 / LE | raw × 1 / 코드 | 3~6 | 소스 후보 |
| 45~46 | NAVI_CYCLIC_COUNTER_OBSERVED_45 | 내비 프레임 순환번호 후보 | 부호 없음 / LE | raw × 1 / 0~3 순환 | 0~3 | 구조 후보 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814).

동시 기록 참조/비트 검증:
- 내비 프레임 순환번호 후보: 전부 FF 형태 제외, 캡처 경계 분리. 값이 바뀐 경우 +1 순환. 독립 캡처 변화 전이 1회. 표본이 적은 ID는 구조 후보.

<a id="id-4ba"></a>
## 0x4BA 내비 짧은 프로파일 후보

유효 형태 436회, 전부 FF 437회. 종류 1은 곡률 후보, 종류 16은 시스템별 정의. 19:2는 유효 형태 0 고정, 후단 58:2는 0~3 변화. 후단 코드 3은 공식 SHORT 코드 4와 불일치.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~12 | PROSHORT_OFFSET | 짧은 프로파일 위치 | 부호 없음 / LE | raw × 1 / m | 71~4000 | 정의, 변화값 |
| 13~18 | PROSHORT_PATH_INDEX | 짧은 프로파일 경로 번호 | 부호 없음 / LE | raw × 1 / 원시값 | 8 고정 | 정의, 고정값 |
| 21~22 | PROSHORT_ACCURACY | 짧은 프로파일 정확도 코드 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 23~32 | PROSHORT_DISTANCE | 두 프로파일 값의 간격 | 부호 없음 / LE | raw × 1 / m | 0~800 | 정의, 변화값 |
| 33~42 | PROSHORT_VALUE_0 | 첫 번째 프로파일 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 3~1003 | 정의, 변화값 |
| 43~52 | PROSHORT_VALUE_1 | 두 번째 프로파일 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 0~26 | 정의, 변화값 |
| 53~57 | PROSHORT_PROFILE_TYPE | 짧은 프로파일 종류 | 부호 없음 / LE | raw × 1 / 원시값 | 1~16 | 정의, 변화값 |
| 58~59 | TAIL_COUNTER_CANDIDATE | 후단 순환번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~3 | 구조 후보 |
| 61~63 | TAIL_MESSAGE_CODE_CANDIDATE | 후단 메시지 코드 후보 | 부호 없음 / LE | raw × 1 / 코드 | 3 고정 | 구조 후보 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L950), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L951), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L953), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L954), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L955), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L956), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L957).

<a id="id-4be"></a>
## 0x4BE 내비 긴 프로파일 및 이벤트 후보

내비 순환번호 위치 59~60 확인. 반복 CAN과 전부 FF 형태를 구분. 유효 형태 32회, 전부 FF 34회. 종류 16에서만 이벤트 종류/속도 코드 적용. 값 6=방지턱, 119=30 km/h 구역, 176=50 km/h 카메라는 ajouatom 분류 후보. 위치를 자차 잔여거리로 바로 사용 불가.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~31 | PROLONG_VALUE | 긴 프로파일 원시값 | 부호 없음 / LE | raw × 1 / 원시값 | 6~176 | 정의, 변화값 |
| 0~3 | EVENT_KIND | 내비 이벤트 종류 후보 | 부호 없음 / LE | raw × 1 / 코드 | 0~7 | 소스 후보 |
| 4~31 | SPEED_CODE | 내비 속도 코드 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 0~11 | 소스 후보 |
| 32~44 | PROLONG_OFFSET | 긴 프로파일 위치 | 부호 없음 / LE | raw × 1 / m | 0~1998 | 정의, 변화값 |
| 47 | PROLONG_UPDATE | 긴 프로파일 갱신 비트 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |
| 48~53 | PROLONG_PATH_INDEX | 긴 프로파일 경로 번호 | 부호 없음 / LE | raw × 1 / 원시값 | 8 고정 | 정의, 고정값 |
| 54~58 | PROLONG_PROFILE_TYPE | 긴 프로파일 종류 | 부호 없음 / LE | raw × 1 / 원시값 | 16 고정 | 정의, 고정값 |
| 59~60 | NAVI_CYCLIC_COUNTER_OBSERVED_59 | 내비 프레임 순환번호 후보 | 부호 없음 / LE | raw × 1 / 0~3 순환 | 0~3 | 구조 후보 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L960), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L961), [근거 4](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L962), [근거 5](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L963), [근거 6](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L964), [근거 7](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L965).

동시 기록 참조/비트 검증:
- 내비 프레임 순환번호 후보: 전부 FF 형태 제외, 캡처 경계 분리. 값이 바뀐 경우 +1 순환. 독립 캡처 변화 전이 3회. 표본이 적은 ID는 구조 후보.

<a id="id-4bf"></a>
## 0x4BF 내비 경로정보 후보

내비 순환번호 위치 56~57 확인. 반복 CAN과 전부 FF 형태를 구분. 유효 형태 433회, 전부 FF 437회. 경로 번호 8 공유와 인접 내비 프레임 형태는 구조 근거. 상세 지도 속성은 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0~12 | COMMON_PATH_OFFSET_CANDIDATE | 경로 위치 후보 | 부호 없음 / LE | raw × 1 / 원시값 | 12~2000 | 구조 후보 |
| 13~18 | COMMON_PATH_INDEX_CANDIDATE | 경로 번호 후보 | 부호 없음 / LE | raw × 1 / 코드 | 8 고정 | 구조 후보 |
| 56~57 | NAVI_CYCLIC_COUNTER_OBSERVED_56 | 내비 프레임 순환번호 후보 | 부호 없음 / LE | raw × 1 / 0~3 순환 | 0~3 | 구조 후보 |

동시 기록 참조/비트 검증:
- 내비 프레임 순환번호 후보: 전부 FF 형태 제외, 캡처 경계 분리. 값이 바뀐 경우 +1 순환. 독립 캡처 변화 전이 38회. 표본이 적은 ID는 구조 후보.

<a id="id-4d8"></a>
## 0x4D8 계기판 단위

수신 메시지의 무결성과 순환번호 후보. 전체 메시지 기능과 송신 ECU는 미확정.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | DISTANCE_UNIT | 거리 단위 | 부호 없음 / LE | raw × 1 / 코드 | 0 고정 | 정의, 고정값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L968).

<a id="id-4eb"></a>
## 0x4EB 차량 시계

시각 14시, 분 32~48, 초 0~59. GPS 시간이나 UTC라는 근거 없음.

| 실제 비트 | 필드 | 뜻 | 값 형태 | 변환 및 단위 | 관측값 | 확인 수준 |
| --- | --- | --- | --- | --- | --- | --- |
| 11~15 | HOURS | 시각 | 부호 없음 / BE | raw × 1 / 원시값 | 14 고정 | 정의, 고정값 |
| 16~21 | MINUTES | 분 | 부호 없음 / BE | raw × 1 / 원시값 | 32~48 | 정의, 변화값 |
| 24~29 | SECONDS | 초 | 부호 없음 / LE | raw × 1 / 원시값 | 0~59 | 정의, 변화값 |

소스: [근거 1](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L971), [근거 2](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L972), [근거 3](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L973).

[전체 비트 원본 감사 자료](evidence/2026-10-07/ecan-korean-bit-dictionary-20261007.json)는 보존했다. 이전 수치 archive의0x1AA 단위는 당시 가설이며, 현재 표시속도 후보와 구분한다.

[원본 저작권 및 허가 고지](evidence/2026-10-07/DBC_SOURCE_LICENSE.txt). 런타임 DBC 및 제어기 변경 없음.
