# 2026-10-07 실차 ECAN 데이터 구조

이 문서는 당시 분석 단계의 기록이다. [현재 분석 시작 문서](ecan_analysis_20261007.md)와 [최신 한국어 필드 표](ecan_korean_bit_fields_20261007.md)를 먼저 참고한다.

후속 심층 분석: [138개 ID 전부의 추론과 숫자 값 파일](ecan_inference_20261007.md). 아래는 첫 번째 DBC 비교 결과다.

사용자가 제공한 ROS bag의 읽을 수 있는 범위를 분석했다. 관측된 ID 138개, RAW CAN 1,914,473개, 합산 기록 구간 656.831032초다. 6개 bag의 전체 CAN 인덱스와 마지막 bag의 정상 인덱스가 남은 앞부분을 읽었다.

이 문서는 오프라인 연구 결과다. 차량 제어 소스, 설정, firmware와 runtime DBC pin을 변경하지 않았다. 원본 bag을 수정하거나 reindex하지 않았다.

## 저장된 ROS 메시지

토픽은 `/ioniq5/can_logger/rx`, 타입은 `ioniq5_ecan/RawCanFrame`이다. 각 ROS 메시지는 CAN 프레임 한 개를 담는다.

```text
time stamp
uint32 address
uint8 bus
bool fd
bool extended
bool returned
bool rejected
uint8[] data
```

ROS 1 직렬화 순서는 stamp(sec uint32, nsec uint32) 8 B, address 4 B, bus 1 B, 플래그 4 B, data 길이 uint32 4 B, payload N B다. 총 21+N B이며, CAN wire frame 크기와는 다르다. bag record 시각은 stamp와 별도로 존재한다.

전 프레임에서 bus=0, fd=true, extended=false, returned=false, rejected=false다. ID는 11-bit 범위이며 payload는 8/16/24/32 B였다. RX 토픽의 플래그만으로 전체 차량 송신 또는 Panda의 다른 동작을 검증할 수는 없다.

stamp는 호스트 ROS 시각이며 하드웨어 CAN 도착 시각이 아니다. bag에 포함된 메시지 정의의 주석은 publish time이라고 적혀 있다. 현재 checkout의 logger는 USB read 완료 시각을 stamp로 사용한다. 토픽명, callerid와 메시지 MD5만으로 차량 컴퓨터의 실행 바이너리/버전을 특정할 수 없어 더 좁은 시각 의미는 확정하지 않는다. bag record 시각과 stamp 차이는 관측상 약 0.027~29.114 ms이며 USB 이전 지연을 뜻하지 않는다.

## 해석 범위와 비교 기준

| 구분 | 결과 |
| --- | ---: |
| 기존 pinned CAN-FD DBC와 ID/길이 일치 | 33 ID |
| ajouatom 참조로 추가 일치 | 7 ID |
| CAN-FD DBC 후보가 있는 합계 | 40 ID |
| 해당 차량의 의미가 미확정인 ID | 98 ID |
| 양쪽 DBC 정의와 코드 필드의 고유 배치 | 601개 |
| 전체 관측 프레임이 CRC16 가설과 일치 | 67 ID |
| byte 2의 +1 카운터 후보 조건 충족 | 59 ID |

601개에는 체크섬, 카운터, NEW_SIGNAL/SET_ME/ZEROS/바이트 컨테이너, 서로 충돌하는 정의와 코드로 분해한 하위 필드가 포함된다. 601개 모두의 물리적 의미를 실차에서 확정했다는 뜻이 아니다. 모든 payload bit를 정의 영역과 미정의 영역으로 나눴으며 미정의 구간은 실제 신호 경계라고 주장하지 않는다.

기존 opendbc 비교: `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`. 두 로컬 DBC는 같은 커밋의 upstream 원본과 줄바꿈 정규화 후 동일함을 확인했다.

ajouatom 연구 snapshot: `carrot-wip`의 `ab696d049e3d6d0fd3bde51c233ee81f736415be`. 다운로드한 source blob은 GitHub tree의 Git blob SHA와 일치했다. 연구 snapshot은 runtime pin을 대체하지 않는다.

- [ajouatom CAN-FD DBC](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc)
- [ajouatom Hyundai 상태 파서](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py)
- [기존 opendbc DBC](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc)

## bag별 확인 범위

| 구간 | CAN 프레임 | ID 수 | 읽은 구간 s | 상태 |
| --- | ---: | ---: | ---: | --- |
| 20261007_143228 | 274,790 | 137 | 94.411233 | 전체 CAN 인덱스 확인 |
| 20261007_143440 | 469,529 | 138 | 160.822336 | 전체 CAN 인덱스 확인 |
| 20261007_143727 | 432,462 | 137 | 148.718746 | 전체 CAN 인덱스 확인 |
| 20261007_144153 | 46,550 | 135 | 15.900482 | 전체 CAN 인덱스 확인 |
| 20261007_144215 | 22,789 | 134 | 7.796347 | 전체 CAN 인덱스 확인 |
| 20261007_144327 | 556,455 | 137 | 190.790863 | 전체 CAN 인덱스 확인 |
| 20261007_144810 | 111,898 | 136 | 38.391025 | 정상 인덱스가 남은 앞부분만 |

마지막 `20261007_144810/sensor_suite.bag`은 header가 가리키는 끝 인덱스와 EOF 표본이 0x00이다. 앞부분의 정상 인덱스 1,538 chunk에서 111,898개 CAN 기록을 읽었다. 마지막 인덱스 없는 chunk와 이후 구간은 제외했다. 첫 zero record offset은 3,890,955,933 B이며, 경계 64 KiB와 header index/EOF 각 4 KiB를 확인했다. 제외 구간 전체가 0x00인지 exhaustive scan한 것은 아니며, 원인도 확정하지 않았다.

주기는 기록된 ROS 메시지 수를 해당 CAN 기록 구간 길이로 나눈 평균이다. 누락 없는 수집, ECU 송신 주기 또는 하드웨어 시각 정밀도를 보증하지 않는다. 다른 센서 메시지 payload와 bag 전체 해시는 검증하지 않았다.

## 제어 및 피드백 신호

아래 bit 번호는 payload 첫 byte의 LSB=0 기준이다. LE는 Intel, BE는 DBC Motorola start-bit 표기다. 물리값 = 부호 해석 후 raw x factor + offset이다. 값의 의미는 각 source 정의 기준이며 조향 부호의 차량 방향은 별도 확인 대상이다.

| ID | 신호 | bit 시작/길이 | 변환 | 관측 범위 |
| --- | --- | --- | --- | --- |
| 0x125 | STEERING_ANGLE | 24/16, signed LE | raw x 0.1 deg | -364.8 ~ 332.0 deg |
| 0x125 | STEERING_RATE | 40/8, unsigned LE | raw x 4 deg/s | 크기만 표현, 부호 없음 |
| 0x0A0 | WHL_SpdFL/FR/RL/RRVal | 64/80/96/112, 각 14 LE | raw x 0.03125 km/h | 각 바퀴 최대 약 65 km/h |
| 0x0EA | MDPS_OutTqVal | 64/12 LE | raw x 0.1 - 204.8 Nm | -22.0 ~ 22.7 Nm |
| 0x0EA | MDPS_StrTqSnsrVal | 80/13 LE | raw - 4095 counts | -490 ~ 552 counts |
| 0x12A | StrTqReqVal / TORQUE_REQUEST | 41/11 LE | raw - 1024 counts | -186 ~ 94 counts |
| 0x1A0 | ACCMode | 68/3 LE | enum | 전체 관측 0 |
| 0x1A0 | aReqValue | 128/11 LE | raw x 0.01 - 10.23 m/s^2 | 전체 관측 0 |
| 0x1A0 | aReqRaw | 140/11 LE | raw x 0.01 - 10.23 m/s^2 | 전체 관측 0 |

조향각, 요청 토크, 운전자 토크 및 MDPS 출력 토크는 서로 다른 값이다. SCC의 aReq=0은 실제 차량 가속도가 0이라는 뜻이 아니다. 이 로그만으로 특정 송신 ECU, 조향 최대 능력이나 angle-command 지원을 확정하지 않는다.

## 추가한 해석

| ID | 추가된 구조 | 확인 범위 |
| --- | --- | --- |
| 0x3A0 | TPMS 앞좌/앞우/뒤좌/뒤우 압력, 상태, 단위 코드 | 압력 raw 각각 36~38, 36~37, 39~40, 39~40. UNIT=0. 물리 단위는 별도 확인 필요 |
| 0x41C | IS_DARK, LIGHT_LEVEL | IS_DARK=0/1, LIGHT_LEVEL raw=256~652 |
| 0x4A3 | LinkClass, SPEED_LIMIT, CountryCode, MapSource, TollExist, TunnelExist | SPEED_LIMIT=0~50, CountryCode=410. 도로 정보의 차량별 enum은 source 기준 후보 |
| 0x4B4 | 위치 오프셋, cyclic counter, 범위 평균 속도 | POS_OFFSET=0~17 m, 평균 속도 필드는 0 |
| 0x4B9 | 바이트 컨테이너를 코드에 따라 세그먼트 오프셋, path index, 계산 경로 상태, 도로 등급으로 분해 | 미사용 코드와 sentinel 포함, 차량별 의미 확정 아님 |
| 0x4BA | short profile 오프셋, path index, 정확도, 거리와 값, profile type | 8191/1023/31 등의 sentinel을 실제 거리/상태로 단정하지 않음 |
| 0x4BE | long profile 값, 오프셋, counter, update, path index, profile type; 조건부 이벤트 종류와 속도 코드 | type=16 등 코드 조건 충족 시에만 이벤트로 해석 |

0x4B9의 코드 분해는 offset=bits 0..12, path_index=13..18, calculated_route=22..23, road_class=24..26이다. 0x4BE는 type=16 및 value<=0x1ff 등의 조건에서 event_kind=value&0xf, speed_code=value>>4를 사용한다. source는 조건을 충족하는 camera/zone에 (speed_code-1)x5를 적용하고 value=6을 bump 후보로 취급한다. 조건 밖의 profile 의미는 미확정이다.

## 정의 충돌과 적용하지 않은 후보

- 휠 속도는 기존 DBC 14 bit, ajouatom 16 bit다. 관측 구간에서 상위 두 bit가 0이어서 둘의 값이 같다. 이 로그만으로 폭을 변경할 근거가 없다.
- MDPS 추정각은 기존 +0.1, ajouatom -0.1이다. ajouatom carstate는 선택한 각도에 부호를 다시 곱하는 경로도 있으므로 DBC 값과 애플리케이션 출력 부호를 구분해야 한다.
- MDPS 및 LFA의 상태 필드는 1/2/3 bit로 다르게 나뉜다. 여러 source 정의를 한 메시지의 동시 필드처럼 합쳐 제어에 쓰면 안 된다. 아래 표와 JSON에는 겹치는 대안을 명시했다.
- 0x180은 실차 8 B, 두 CAN-FD DBC의 CAM_0x180은 32 B다. 0x380은 실차 8 B, ajouatom CANFD_NAVI_STATUS_380은 24 B다. ID만으로 카메라/내비 메시지를 붙이지 않았다.
- 같은 저장소의 2015 C-CAN/M-CAN/generic 및 corner-radar DBC도 ID/길이를 비교했다. 구형 WHL_SPD11, EMS17, DI_BOX13, amplifier/network-management 등은 bus/platform 또는 payload가 달라 동일 ID만으로 적용하지 않았다. 후보와 배제 근거는 JSON에 남겼다.

## 의미 미확정 ID의 추가 관측

CRC16가 매 프레임 일치하는 67개 ID는 첫 2 B를 CRC16 구조로 설명할 수 있다. 이 중 일부 ID에는 DBC가 없으므로 그 뒤의 물리 신호 의미는 여전히 미확정이다. byte 2 카운터는 관측 내 +1 비율 >99%, 100개 이상의 서로 다른 값 및 capture 경계 제외 조건을 충족한 59개 ID에서 후보로 표기했다.

변하는 bit와 고정 bit를 모든 ID에서 계산했다. 고정 bit는 이번 로그에서만 고정된 것이며 reserved/unused라는 뜻이 아니다. 100 ms bin의 마지막 기록값으로 다음 후보를 탐색했고, 첫 2개 capture에서 맞춘 관계를 나머지 capture에서 따로 확인했다. 후보는 실제 단위나 ECU를 확정하지 않으며 공통 차량 움직임에 의해 다른 물리량도 함께 변할 수 있다.

| ID | 후보 위치 | 연동 기준 | 후속 capture 상관계수 | 해석 상태 |
| --- | --- | --- | ---: | --- |
| 0x0DA | LE 60:16, unsigned | speed_kph | 0.999920 | 상관 후보, scale/의미 미확정 |
| 0x10A | LE 24:16, signed | speed_kph | 0.999854 | 상관 후보, scale/의미 미확정 |
| 0x2B5 | LE 48:8, unsigned | pedal_raw | 0.999266 | 상관 후보, scale/의미 미확정 |

0x2B5 byte 6은 0x035 byte 5 가속 페달 raw와 20 ms 이내 가장 가까운 stamp로 6,567개를 짝지었다. 완전 일치는 82.78%, 평균 절대 raw 차이는 0.1785였다. 페달 복제 후보로는 유력하지만 같은 시각에 완전히 같은 데이터라고 단정하지 않는다.

## 전체 ID 목록

필드 수에는 source별 충돌 대안과 raw placeholder가 포함된다. CRC 열의 통과 수는 해당 CRC16 가설에 대한 수치이며 checksum이 다른 8 B 메시지를 손상됐다고 분류하지 않는다.

| ID | B | 기록 수 | 평균 Hz | DBC 후보 | 필드 수 | CRC16 통과 |
| --- | ---: | ---: | ---: | --- | ---: | ---: |
| [0x035](ecan_fields_20261007.md#id-035) | 32 | 65,685 | 100.00 | ACCELERATOR | 4 | 65,685/65,685 |
| [0x04A](ecan_fields_20261007.md#id-04a) | 32 | 65,686 | 100.00 | IMU_01_10ms | 17 | 65,686/65,686 |
| [0x060](ecan_fields_20261007.md#id-060) | 32 | 65,685 | 100.00 | ESP_STATUS | 9 | 65,685/65,685 |
| [0x065](ecan_fields_20261007.md#id-065) | 32 | 65,685 | 100.00 | BRAKE | 5 | 65,685/65,685 |
| [0x06F](ecan_fields_20261007.md#id-06f) | 8 | 65,685 | 100.00 | 의미 미확정 | 0 | 65,685/65,685 |
| [0x08B](ecan_fields_20261007.md#id-08b) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 0/3,284 |
| [0x090](ecan_fields_20261007.md#id-090) | 32 | 6,572 | 10.01 | 의미 미확정 | 0 | 6,561/6,572 |
| [0x0A0](ecan_fields_20261007.md#id-0a0) | 24 | 65,685 | 100.00 | WHEEL_SPEEDS | 14 | 65,685/65,685 |
| [0x0DA](ecan_fields_20261007.md#id-0da) | 24 | 65,686 | 100.00 | 의미 미확정 | 0 | 65,686/65,686 |
| [0x0EA](ecan_fields_20261007.md#id-0ea) | 24 | 65,683 | 100.00 | MDPS | 47 | 65,683/65,683 |
| [0x0F5](ecan_fields_20261007.md#id-0f5) | 32 | 65,686 | 100.00 | 의미 미확정 | 0 | 65,686/65,686 |
| [0x0FF](ecan_fields_20261007.md#id-0ff) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x10A](ecan_fields_20261007.md#id-10a) | 32 | 65,685 | 100.00 | 의미 미확정 | 0 | 65,685/65,685 |
| [0x11A](ecan_fields_20261007.md#id-11a) | 16 | 65,676 | 99.99 | FR_CMR_01_10ms | 23 | 65,676/65,676 |
| [0x120](ecan_fields_20261007.md#id-120) | 32 | 65,687 | 100.01 | 의미 미확정 | 0 | 65,687/65,687 |
| [0x125](ecan_fields_20261007.md#id-125) | 16 | 65,343 | 99.48 | STEERING_SENSORS | 5 | 65,343/65,343 |
| [0x12A](ecan_fields_20261007.md#id-12a) | 16 | 65,683 | 100.00 | LFA | 37 | 65,683/65,683 |
| [0x130](ecan_fields_20261007.md#id-130) | 16 | 65,685 | 100.00 | GEAR_SHIFTER | 5 | 65,685/65,685 |
| [0x145](ecan_fields_20261007.md#id-145) | 32 | 32,844 | 50.00 | 의미 미확정 | 0 | 32,778/32,844 |
| [0x15A](ecan_fields_20261007.md#id-15a) | 8 | 65,687 | 100.01 | 의미 미확정 | 0 | 65,687/65,687 |
| [0x160](ecan_fields_20261007.md#id-160) | 16 | 32,841 | 50.00 | ADRV_0x160 | 14 | 32,841/32,841 |
| [0x175](ecan_fields_20261007.md#id-175) | 24 | 32,843 | 50.00 | TCS | 43 | 32,843/32,843 |
| [0x180](ecan_fields_20261007.md#id-180) | 8 | 32,873 | 50.05 | 의미 미확정 | 0 | 32,873/32,873 |
| [0x185](ecan_fields_20261007.md#id-185) | 8 | 32,873 | 50.05 | CAM_0x185 | 2 | 32,873/32,873 |
| [0x19A](ecan_fields_20261007.md#id-19a) | 32 | 65,687 | 100.01 | 의미 미확정 | 0 | 65,687/65,687 |
| [0x1A0](ecan_fields_20261007.md#id-1a0) | 32 | 32,841 | 50.00 | SCC_CONTROL | 49 | 32,841/32,841 |
| [0x1AA](ecan_fields_20261007.md#id-1aa) | 16 | 32,873 | 50.05 | CRUISE_BUTTONS_ALT | 25 | 32,873/32,873 |
| [0x1B0](ecan_fields_20261007.md#id-1b0) | 32 | 32,844 | 50.00 | 의미 미확정 | 0 | 32,778/32,844 |
| [0x1B5](ecan_fields_20261007.md#id-1b5) | 32 | 13,136 | 20.00 | CCNC_0x1B5 / FR_CMR_03_50ms | 18 | 13,136/13,136 |
| [0x1BA](ecan_fields_20261007.md#id-1ba) | 24 | 13,137 | 20.00 | BLINDSPOTS_REAR_CORNERS / ADAS_CMD_50_50ms | 30 | 13,137/13,137 |
| [0x1CF](ecan_fields_20261007.md#id-1cf) | 8 | 32,843 | 50.00 | CRUISE_BUTTONS | 10 | 0/32,843 |
| [0x1DA](ecan_fields_20261007.md#id-1da) | 32 | 658 | 1.00 | ADRV_0x1da | 4 | 658/658 |
| [0x1E0](ecan_fields_20261007.md#id-1e0) | 16 | 13,137 | 20.00 | LFAHDA_CLUSTER | 18 | 13,137/13,137 |
| [0x1E5](ecan_fields_20261007.md#id-1e5) | 16 | 13,136 | 20.00 | BLINDSPOTS_FRONT_CORNER_1 | 11 | 13,136/13,136 |
| [0x1EA](ecan_fields_20261007.md#id-1ea) | 32 | 13,137 | 20.00 | ADRV_0x1ea | 55 | 13,137/13,137 |
| [0x1F0](ecan_fields_20261007.md#id-1f0) | 16 | 13,137 | 20.00 | 의미 미확정 | 0 | 13,137/13,137 |
| [0x1F5](ecan_fields_20261007.md#id-1f5) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x1FA](ecan_fields_20261007.md#id-1fa) | 32 | 6,568 | 10.00 | CLUSTER_SPEED_LIMIT / FR_CMR_02_100ms | 45 | 6,568/6,568 |
| [0x200](ecan_fields_20261007.md#id-200) | 8 | 13,137 | 20.00 | ADRV_0x200 | 9 | 13,137/13,137 |
| [0x20A](ecan_fields_20261007.md#id-20a) | 16 | 6,569 | 10.00 | 의미 미확정 | 0 | 6,569/6,569 |
| [0x225](ecan_fields_20261007.md#id-225) | 16 | 6,574 | 10.01 | 의미 미확정 | 0 | 6,574/6,574 |
| [0x235](ecan_fields_20261007.md#id-235) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x250](ecan_fields_20261007.md#id-250) | 24 | 6,569 | 10.00 | 의미 미확정 | 0 | 6,569/6,569 |
| [0x255](ecan_fields_20261007.md#id-255) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x25A](ecan_fields_20261007.md#id-25a) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x2AA](ecan_fields_20261007.md#id-2aa) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x2B0](ecan_fields_20261007.md#id-2b0) | 32 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,278/3,284 |
| [0x2B5](ecan_fields_20261007.md#id-2b5) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x2C0](ecan_fields_20261007.md#id-2c0) | 32 | 3,285 | 5.00 | 의미 미확정 | 0 | 3,285/3,285 |
| [0x2D5](ecan_fields_20261007.md#id-2d5) | 32 | 3,285 | 5.00 | 의미 미확정 | 0 | 3,285/3,285 |
| [0x2E0](ecan_fields_20261007.md#id-2e0) | 32 | 6,567 | 10.00 | MANUAL_SPEED_LIMIT_ASSIST | 6 | 6,567/6,567 |
| [0x2E5](ecan_fields_20261007.md#id-2e5) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x2FA](ecan_fields_20261007.md#id-2fa) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x2FF](ecan_fields_20261007.md#id-2ff) | 8 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x30A](ecan_fields_20261007.md#id-30a) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x315](ecan_fields_20261007.md#id-315) | 16 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x31A](ecan_fields_20261007.md#id-31a) | 32 | 658 | 1.00 | 의미 미확정 | 0 | 657/658 |
| [0x325](ecan_fields_20261007.md#id-325) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x330](ecan_fields_20261007.md#id-330) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x33A](ecan_fields_20261007.md#id-33a) | 32 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x345](ecan_fields_20261007.md#id-345) | 8 | 3,284 | 5.00 | ADRV_0x345 | 3 | 3,284/3,284 |
| [0x34A](ecan_fields_20261007.md#id-34a) | 32 | 658 | 1.00 | 의미 미확정 | 0 | 657/658 |
| [0x360](ecan_fields_20261007.md#id-360) | 32 | 6,567 | 10.00 | 의미 미확정 | 0 | 6,567/6,567 |
| [0x36A](ecan_fields_20261007.md#id-36a) | 16 | 13,137 | 20.00 | BLINDSPOTS_FRONT_CORNER_2 | 4 | 13,137/13,137 |
| [0x36F](ecan_fields_20261007.md#id-36f) | 8 | 6,561 | 9.99 | 의미 미확정 | 0 | 0/6,561 |
| [0x37F](ecan_fields_20261007.md#id-37f) | 8 | 6,561 | 9.99 | 의미 미확정 | 0 | 0/6,561 |
| [0x380](ecan_fields_20261007.md#id-380) | 8 | 3,285 | 5.00 | 의미 미확정 | 0 | 3,285/3,285 |
| [0x382](ecan_fields_20261007.md#id-382) | 8 | 3,290 | 5.01 | 의미 미확정 | 0 | 3,288/3,290 |
| [0x383](ecan_fields_20261007.md#id-383) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,281/3,284 |
| [0x384](ecan_fields_20261007.md#id-384) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 3,283/3,288 |
| [0x386](ecan_fields_20261007.md#id-386) | 8 | 3,285 | 5.00 | 의미 미확정 | 0 | 0/3,285 |
| [0x38A](ecan_fields_20261007.md#id-38a) | 16 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x39B](ecan_fields_20261007.md#id-39b) | 8 | 3,247 | 4.94 | 의미 미확정 | 0 | 0/3,247 |
| [0x3A0](ecan_fields_20261007.md#id-3a0) | 16 | 3,287 | 5.00 | TPMS | 6 | 3,287/3,287 |
| [0x3A5](ecan_fields_20261007.md#id-3a5) | 8 | 6,568 | 10.00 | 의미 미확정 | 0 | 6,568/6,568 |
| [0x3A6](ecan_fields_20261007.md#id-3a6) | 8 | 3,285 | 5.00 | 의미 미확정 | 0 | 0/3,285 |
| [0x3AA](ecan_fields_20261007.md#id-3aa) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 0/3,284 |
| [0x3B0](ecan_fields_20261007.md#id-3b0) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,280/3,284 |
| [0x3B1](ecan_fields_20261007.md#id-3b1) | 8 | 3,286 | 5.00 | 의미 미확정 | 0 | 3,282/3,286 |
| [0x3B2](ecan_fields_20261007.md#id-3b2) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,282/3,284 |
| [0x3B5](ecan_fields_20261007.md#id-3b5) | 32 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,284/3,284 |
| [0x3C1](ecan_fields_20261007.md#id-3c1) | 8 | 3,318 | 5.05 | BLINKER_STALKS | 7 | 0/3,318 |
| [0x3C2](ecan_fields_20261007.md#id-3c2) | 8 | 3,266 | 4.97 | 의미 미확정 | 0 | 0/3,266 |
| [0x3CA](ecan_fields_20261007.md#id-3ca) | 16 | 658 | 1.00 | 의미 미확정 | 0 | 658/658 |
| [0x3E0](ecan_fields_20261007.md#id-3e0) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x3E1](ecan_fields_20261007.md#id-3e1) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x3E2](ecan_fields_20261007.md#id-3e2) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x3E5](ecan_fields_20261007.md#id-3e5) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x3E6](ecan_fields_20261007.md#id-3e6) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x3EA](ecan_fields_20261007.md#id-3ea) | 8 | 3,282 | 5.00 | 의미 미확정 | 0 | 0/3,282 |
| [0x3F0](ecan_fields_20261007.md#id-3f0) | 32 | 658 | 1.00 | 의미 미확정 | 0 | 658/658 |
| [0x3F5](ecan_fields_20261007.md#id-3f5) | 32 | 658 | 1.00 | 의미 미확정 | 0 | 658/658 |
| [0x401](ecan_fields_20261007.md#id-401) | 8 | 3,282 | 5.00 | 의미 미확정 | 0 | 0/3,282 |
| [0x405](ecan_fields_20261007.md#id-405) | 8 | 3,282 | 5.00 | 의미 미확정 | 0 | 0/3,282 |
| [0x410](ecan_fields_20261007.md#id-410) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 0/3,284 |
| [0x411](ecan_fields_20261007.md#id-411) | 8 | 3,282 | 5.00 | DOORS_SEATBELTS | 8 | 0/3,282 |
| [0x412](ecan_fields_20261007.md#id-412) | 8 | 3,385 | 5.15 | 의미 미확정 | 0 | 0/3,385 |
| [0x413](ecan_fields_20261007.md#id-413) | 8 | 3,865 | 5.88 | BLINKERS | 8 | 0/3,865 |
| [0x414](ecan_fields_20261007.md#id-414) | 8 | 3,281 | 5.00 | 의미 미확정 | 0 | 0/3,281 |
| [0x416](ecan_fields_20261007.md#id-416) | 8 | 3,281 | 5.00 | 의미 미확정 | 0 | 0/3,281 |
| [0x417](ecan_fields_20261007.md#id-417) | 8 | 3,281 | 5.00 | 의미 미확정 | 0 | 0/3,281 |
| [0x418](ecan_fields_20261007.md#id-418) | 8 | 3,384 | 5.15 | 의미 미확정 | 0 | 0/3,384 |
| [0x419](ecan_fields_20261007.md#id-419) | 8 | 10,840 | 16.50 | 의미 미확정 | 0 | 0/10,840 |
| [0x41A](ecan_fields_20261007.md#id-41a) | 8 | 3,293 | 5.01 | 의미 미확정 | 0 | 0/3,293 |
| [0x41B](ecan_fields_20261007.md#id-41b) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x41C](ecan_fields_20261007.md#id-41c) | 8 | 12,610 | 19.20 | LIGHT_SENSOR | 4 | 0/12,610 |
| [0x41F](ecan_fields_20261007.md#id-41f) | 8 | 1,081 | 1.65 | 의미 미확정 | 0 | 0/1,081 |
| [0x422](ecan_fields_20261007.md#id-422) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x425](ecan_fields_20261007.md#id-425) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x427](ecan_fields_20261007.md#id-427) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,283/3,284 |
| [0x429](ecan_fields_20261007.md#id-429) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,284/3,284 |
| [0x42A](ecan_fields_20261007.md#id-42a) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 3,281/3,284 |
| [0x432](ecan_fields_20261007.md#id-432) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x435](ecan_fields_20261007.md#id-435) | 8 | 5,361 | 8.16 | 의미 미확정 | 0 | 0/5,361 |
| [0x442](ecan_fields_20261007.md#id-442) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x444](ecan_fields_20261007.md#id-444) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x445](ecan_fields_20261007.md#id-445) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x448](ecan_fields_20261007.md#id-448) | 8 | 3,284 | 5.00 | 의미 미확정 | 0 | 0/3,284 |
| [0x454](ecan_fields_20261007.md#id-454) | 8 | 3,288 | 5.01 | 의미 미확정 | 0 | 0/3,288 |
| [0x472](ecan_fields_20261007.md#id-472) | 8 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x473](ecan_fields_20261007.md#id-473) | 8 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x474](ecan_fields_20261007.md#id-474) | 8 | 3,287 | 5.00 | 의미 미확정 | 0 | 3,287/3,287 |
| [0x47F](ecan_fields_20261007.md#id-47f) | 8 | 3,284 | 5.00 | HVAC_TOUCH_BUTTONS | 11 | 0/3,284 |
| [0x48F](ecan_fields_20261007.md#id-48f) | 8 | 657 | 1.00 | 의미 미확정 | 0 | 0/657 |
| [0x4A3](ecan_fields_20261007.md#id-4a3) | 8 | 3,253 | 4.95 | HDA_INFO_4A3 | 9 | 0/3,253 |
| [0x4B4](ecan_fields_20261007.md#id-4b4) | 8 | 6,419 | 9.77 | NEW_MSG_4B4 | 3 | 0/6,419 |
| [0x4B8](ecan_fields_20261007.md#id-4b8) | 8 | 119 | 0.18 | 의미 미확정 | 0 | 0/119 |
| [0x4B9](ecan_fields_20261007.md#id-4b9) | 8 | 120 | 0.18 | NEW_MSG_4B9 | 12 | 0/120 |
| [0x4BA](ecan_fields_20261007.md#id-4ba) | 8 | 873 | 1.33 | NEW_MSG_4BA | 8 | 0/873 |
| [0x4BE](ecan_fields_20261007.md#id-4be) | 8 | 66 | 0.10 | NEW_MSG_4BE | 8 | 0/66 |
| [0x4BF](ecan_fields_20261007.md#id-4bf) | 8 | 870 | 1.32 | 의미 미확정 | 0 | 0/870 |
| [0x4D8](ecan_fields_20261007.md#id-4d8) | 8 | 3,246 | 4.94 | CLUSTER_INFO | 1 | 0/3,246 |
| [0x4DD](ecan_fields_20261007.md#id-4dd) | 8 | 3,246 | 4.94 | 의미 미확정 | 0 | 0/3,246 |
| [0x4DF](ecan_fields_20261007.md#id-4df) | 8 | 3,251 | 4.95 | 의미 미확정 | 0 | 0/3,251 |
| [0x4EB](ecan_fields_20261007.md#id-4eb) | 8 | 3,246 | 4.94 | LOCAL_TIME2 | 4 | 0/3,246 |
| [0x4F2](ecan_fields_20261007.md#id-4f2) | 8 | 4,702 | 7.16 | 의미 미확정 | 0 | 0/4,702 |
| [0x4F3](ecan_fields_20261007.md#id-4f3) | 8 | 650 | 0.99 | 의미 미확정 | 0 | 0/650 |
| [0x4FE](ecan_fields_20261007.md#id-4fe) | 8 | 325 | 0.49 | 의미 미확정 | 0 | 0/325 |

전체 필드의 bit 배치, 부호, factor/offset, 관측 범위, enum, 미정의 bit 구간 및 source별 대안은 [필드 상세](ecan_fields_20261007.md)와 [JSON](evidence/2026-10-07/ecan-fields-20261007.json)에 있다.
