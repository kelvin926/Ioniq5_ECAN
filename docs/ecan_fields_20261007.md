# 2026-10-07 ECAN 전체 필드 사전

이 문서는 당시 분석 단계의 기록이다. [현재 분석 시작 문서](ecan_analysis_20261007.md)와 [최신 한국어 필드 표](ecan_korean_bit_fields_20261007.md)를 먼저 참고한다.

[요약과 검증 범위](ecan_capture_structure_20261007.md). 아래 source 정의는 연구 후보이며 runtime/control DBC가 아니다. opendbc 원본 정의와 enum의 라이선스는 [원본 저작권 및 허가 고지](evidence/2026-10-07/DBC_SOURCE_LICENSE.txt)를 따른다.

각 표의 start:bits는 DBC 표기다. LE와 BE는 다르게 bit를 이동한다. source 링크와 JSON의 physical_bits로 실제 payload 위치를 확인할 수 있다. enum은 source 정의를 보여주며 차량별 의미를 따로 검증해야 한다.

<a id="id-035"></a>
## 0x035 ACCELERATOR

bus 0, 32 B, 65,685 records, 평균 100.0029 Hz. 가속 페달 원시값과 기어

변화 bit 103개. 정의/헤더의 구조 coverage 35/256 bit. 의미 source가 있는 영역 11 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 0 ~ 65535 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L43) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L44) |
| ACCELERATOR_PEDAL | 40:8 | U / LE | raw x 1 + 0 |  | 0 ~ 112 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L45) |
| GEAR | 192:3 | U / LE | raw x 1 + 0 |  | 5 ~ 5 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L46) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `ACCELERATOR_PEDAL`와 같은 배치/변환의 alias: ACCELERATOR_PEDAL (기존)
- `GEAR` source enum: 0=P; 5=D; 6=N; 7=R
- `GEAR`와 같은 배치/변환의 alias: GEAR (기존)

- 미정의 bit 구간: 24:16, 48:144, 195:61
- 미정의 구간 중 변화한 bit: 52:2, 64:5, 72:17, 97:13, 113:14, 152:21
- 전체 변화 bit 구간: 0:24, 40:7, 52:2, 64:5, 72:17, 97:13, 113:14, 152:21
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-04a"></a>
## 0x04A IMU_01_10ms

bus 0, 32 B, 65,686 records, 평균 100.0044 Hz. IMU yaw rate와 가속도, 상태 플래그

변화 bit 103개. 정의/헤더의 구조 coverage 216/256 bit. 의미 source가 있는 영역 192 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| IMU_Crc1Val | 0:16 | U / LE | raw x 1 + 0 |  | 1 ~ 65533 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L20) |
| IMU_AlvCnt1Val | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L21) |
| IMU_YawSigSta | 24:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L22) |
| IMU_LatAccelSigSta | 28:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L23) |
| IMU_LongAccelSigSta | 32:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L24) |
| IMU_VerAccelSigSta | 36:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L25) |
| IMU_McuVoltSta | 40:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L26) |
| IMU_AcuRstSta | 44:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L27) |
| IMU_SnsrTyp | 48:8 | U / LE | raw x 1 + 0 |  | 8 ~ 8 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L28) |
| IMU_RollSigSta | 56:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L29) |
| IMU_YawRtVal | 64:16 | U / LE | raw x 0.005 + -163.84 | Deg/s | -23.45 ~ 23.83 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L30) |
| IMU_LatAccelVal | 80:16 | U / LE | raw x 0.000127465 + -4.176773 | g | -0.2382321 ~ 0.2555673 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L31) |
| IMU_LongAccelVal | 96:16 | U / LE | raw x 0.000127465 + -4.176773 | g | -0.2936794 ~ 0.2252307 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L32) |
| IMU_SnsrTempVal | 112:16 | U / LE | raw x 1 + 0 |  | 106 ~ 107 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L33) |
| IMU_SeralNumVal | 128:64 | U / LE | raw x 1 + 0 |  | 식별정보 값 생략 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L34) |
| IMU_RollRtVal | 192:16 | U / LE | raw x 0.005 + -163.84 | º/s | -9.08 ~ 11.925 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L35) |
| IMU_VerAccelVal | 208:8 | U / LE | raw x 1 + 0 |  | 1 ~ 255 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L36) |


- 미정의 bit 구간: 216:40
- 미정의 구간 중 변화한 bit: 216:6
- 전체 변화 bit 구간: 0:24, 64:49, 192:30
- CRC16 probe: 65,686 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-060"></a>
## 0x060 ESP_STATUS

bus 0, 32 B, 65,685 records, 평균 100.0029 Hz. ESC 상태와 AutoHold 후보

변화 bit 37개. 정의/헤더의 구조 coverage 46/256 bit. 의미 source가 있는 영역 22 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 19 ~ 65514 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L91) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L92) |
| TRACTION_AND_STABILITY_CONTROL | 42:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L93) |
| BRAKE_PRESSURE | 128:11 | U / LE | raw x 1 + 0 |  | 0 ~ 277 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L94) |
| BRAKE_PRESSED | 148:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L95) |
| AVH_Sta | 192:2 | U / LE | raw x 1 + 0 |  | 0 ~ 2 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L96) |
| AVH_I_LAMP | 218:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L97) |
| AVH_LAMP | 220:3 | U / LE | raw x 1 + 0 |  | 2 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L98) |
| BRAKE_PRESSURE | 128:10 | U / LE | raw x 1 + 0 |  | 0 ~ 277 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L42) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `TRACTION_AND_STABILITY_CONTROL` source enum: 0=On; 5=Limited; 1=Off
- `TRACTION_AND_STABILITY_CONTROL`와 같은 배치/변환의 alias: TRACTION_AND_STABILITY_CONTROL (기존)
- `BRAKE_PRESSED`와 같은 배치/변환의 alias: BRAKE_PRESSED (기존)
- `AVH_Sta` source enum: 0=No apply; 1=Vehicle is held by the service brake; 2=Being released; 3=Error Indicator
- `AVH_I_LAMP` source enum: 0=Lamp on; 1=Lamp off; 2=Reserved; 3=Reserved
- `AVH_LAMP` source enum: 0=AVH off; 1=AVH failure; 2=AVH active; 3=AVH ready; 4=Reserved; 5=Reserved; 6=Reserved; 7=Reserved

- 미정의 bit 구간: 24:18, 45:83, 139:9, 149:43, 194:24, 223:33
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 128:9, 148:1, 192:2, 220:1
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-065"></a>
## 0x065 BRAKE

bus 0, 32 B, 65,685 records, 평균 100.0029 Hz. 브레이크 위치와 작동, 램프

변화 bit 101개. 정의/헤더의 구조 coverage 42/256 bit. 의미 source가 있는 영역 18 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 0 ~ 65534 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L101) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L102) |
| BRAKE_LIGHT | 29:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L103) |
| BRAKE_POSITION | 40:16 | S / LE | raw x 1 + 0 |  | 11 ~ 384 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L104) |
| BRAKE_PRESSED | 57:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L105) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `BRAKE_POSITION`와 같은 배치/변환의 alias: BRAKE_POSITION (기존)
- `BRAKE_PRESSED`와 같은 배치/변환의 alias: BRAKE_PRESSED (기존)

- 미정의 bit 구간: 24:5, 30:10, 56:1, 58:198
- 미정의 구간 중 변화한 bit: 56:1, 72:10, 88:12, 104:7, 112:7, 128:10, 200:9, 212:10
- 전체 변화 bit 구간: 0:24, 29:1, 40:9, 56:2, 72:10, 88:12, 104:7, 112:7, 128:10, 200:9, 212:10
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-06f"></a>
## 0x06F 의미 미확정

bus 0, 8 B, 65,685 records, 평균 100.0029 Hz. 물리 신호 의미 미확정

변화 bit 25개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:40
- 미정의 구간 중 변화한 bit: 36:1
- 전체 변화 bit 구간: 0:24, 36:1
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-08b"></a>
## 0x08B 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,284 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: AMP_HU_E_12 (hyundai_2015_mcan.dbc)

<a id="id-090"></a>
## 0x090 의미 미확정

bus 0, 32 B, 6,572 records, 평균 10.0056 Hz. 물리 신호 의미 미확정

변화 bit 40개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:30, 64:5, 100:5
- 전체 변화 bit 구간: 0:30, 64:5, 100:5
- CRC16 probe: 6,561 통과, 11 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-0a0"></a>
## 0x0A0 WHEEL_SPEEDS

bus 0, 24 B, 65,685 records, 평균 100.0029 Hz. 네 바퀴 속도

변화 bit 104개. 정의/헤더의 구조 coverage 92/192 bit. 의미 source가 있는 영역 68 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 1 ~ 65535 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L121) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L122) |
| MOVING_FORWARD | 56:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L123) |
| MOVING_BACKWARD | 57:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L124) |
| MOVING_FORWARD2 | 58:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L125) |
| MOVING_BACKWARD2 | 59:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L126) |
| WHEEL_SPEED_1 | 64:16 | U / LE | raw x 0.03125 + 0 | kph | 0 ~ 65.375 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L127) |
| WHEEL_SPEED_2 | 80:16 | U / LE | raw x 0.03125 + 0 | kph | 0 ~ 65.1875 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L128) |
| WHEEL_SPEED_3 | 96:16 | U / LE | raw x 0.03125 + 0 | kph | 0 ~ 65 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L129) |
| WHEEL_SPEED_4 | 112:16 | U / LE | raw x 0.03125 + 0 | kph | 0 ~ 65.09375 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L130) |
| WHL_SpdFLVal | 64:14 | U / LE | raw x 0.03125 + 0 | km^h | 0 ~ 65.375 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L63) |
| WHL_SpdFRVal | 80:14 | U / LE | raw x 0.03125 + 0 | km^h | 0 ~ 65.1875 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L64) |
| WHL_SpdRLVal | 96:14 | U / LE | raw x 0.03125 + 0 | km^h | 0 ~ 65 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L65) |
| WHL_SpdRRVal | 112:14 | U / LE | raw x 0.03125 + 0 | km^h | 0 ~ 65.09375 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L66) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `MOVING_FORWARD`와 같은 배치/변환의 alias: MOVING_FORWARD (기존)
- `MOVING_BACKWARD`와 같은 배치/변환의 alias: MOVING_BACKWARD (기존)
- `MOVING_FORWARD2`와 같은 배치/변환의 alias: MOVING_FORWARD2 (기존)
- `MOVING_BACKWARD2`와 같은 배치/변환의 alias: MOVING_BACKWARD2 (기존)

- 미정의 bit 구간: 24:32, 60:4, 128:64
- 미정의 구간 중 변화한 bit: 24:32
- 전체 변화 bit 구간: 0:56, 64:12, 80:12, 96:12, 112:12
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-0da"></a>
## 0x0DA 의미 미확정

bus 0, 24 B, 65,686 records, 평균 100.0044 Hz. 물리 신호 의미 미확정

변화 bit 109개. 정의/헤더의 구조 coverage 24/192 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:168
- 미정의 구간 중 변화한 bit: 24:6, 40:8, 56:6, 64:11, 80:32, 116:1, 128:5, 144:16
- 전체 변화 bit 구간: 0:30, 40:8, 56:6, 64:11, 80:32, 116:1, 128:5, 144:16
- CRC16 probe: 65,686 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 연동 후보 LE 60:16 -> speed_kph, holdout r=0.999920. 물리 의미 미확정.

<a id="id-0ea"></a>
## 0x0EA MDPS

bus 0, 24 B, 65,683 records, 평균 99.9998 Hz. MDPS 토크, 추정 각도, LKA/PA/ACI/TPA 상태

변화 bit 66개. 정의/헤더의 구조 coverage 168/192 bit. 의미 source가 있는 영역 144 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 0 ~ 65535 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L140) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L141) |
| NEW_SIGNAL_1 | 31:8 | U / BE | raw x 1 + 0 |  | 16 ~ 16 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L142) |
| NEW_SIGNAL_2 | 39:8 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L143) |
| NEW_SIGNAL_3 | 47:8 | U / BE | raw x 1 + 0 |  | 65 ~ 65 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L144) |
| LKA_ACTIVE | 48:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L145) |
| LKA_FAULT | 54:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L146) |
| STEERING_OUT_TORQUE | 64:12 | U / LE | raw x 0.1 + -204.8 |  | -22 ~ 22.7 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L147) |
| STEERING_COL_TORQUE | 80:13 | U / LE | raw x 1 + -4095 |  | -490 ~ 552 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L148) |
| STEERING_ANGLE | 96:16 | S / LE | raw x -0.1 + 0 | deg | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L149) |
| STEERING_ANGLE_2 | 128:16 | S / LE | raw x -0.1 + 0 | deg | -328.9 ~ 364.3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L150) |
| LFA2_ACTIVE | 145:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L151) |
| LFA2_FAULT | 149:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L152) |
| NEW_SIGNAL_4 | 159:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L153) |
| NEW_SIGNAL_5 | 167:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L154) |
| NEW_SIGNAL_6 | 175:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L155) |
| MDPS_WrngLmpSta | 24:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L89) |
| MDPS_Typ | 27:3 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L90) |
| MDPS_EngIdleRpmReq | 30:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L91) |
| MDPS_VsmPlgInSta | 32:2 | U / LE | raw x 1 + 0 | flag | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L92) |
| MDPS_VsmDfctvSta | 34:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L93) |
| MDPS_VsmSigErrSta | 36:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L94) |
| MDPS_PaPlugInSta | 38:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L95) |
| MDPS_PaModeSta | 40:4 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L96) |
| MDPS_PaCanfltSta | 44:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L97) |
| MDPS_LkaPlgInSta | 46:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L98) |
| MDPS_LkaToiActvSta | 48:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L99) |
| MDPS_LkaToiUnblSta | 50:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L100) |
| MDPS_LkaToiFltSta | 52:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L101) |
| MDPS_LkaFailSta | 54:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L102) |
| MDPS_ResetOpSta | 59:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L103) |
| MDPS_CurrModVal | 61:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L104) |
| MDPS_VsmActResp | 76:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L106) |
| Reprogram_State_MDPS | 78:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L107) |
| MDPS_PaStrAnglVal | 96:16 | S / LE | raw x 0.1 + 0 | Deg | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L109) |
| MDPS_LoamModSta | 112:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L110) |
| MDPS_HDPSpprtSWVer | 114:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L111) |
| MDPS_ADASAciActvSta | 120:4 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L112) |
| MDPS_ADASAciPluginSta | 124:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L113) |
| MDPS_ADAS_AciFltSig | 126:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L114) |
| MDPS_EstStrAnglVal | 128:16 | S / LE | raw x 0.1 + 0 | Deg | -364.3 ~ 328.9 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L115) |
| MDPS_ADASAciActvSta_Lv2 | 144:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L116) |
| MDPS_ADAS_AciFltSig_Lv2 | 148:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L117) |
| MDPS_TpaPlugInSta | 152:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L118) |
| MDPS_TpaCanfltSta | 154:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L119) |
| MDPS_TpaModeSta | 156:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L120) |
| MDPS_TpaStrAnglVal | 160:16 | S / LE | raw x 0.1 + 0 | Deg | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L121) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `LKA_FAULT` source enum: 0=ok; 1=lka fault
- `STEERING_OUT_TORQUE`와 같은 배치/변환의 alias: MDPS_OutTqVal (기존)
- `STEERING_COL_TORQUE`와 같은 배치/변환의 alias: MDPS_StrTqSnsrVal (기존)
- `MDPS_WrngLmpSta` source enum: 0=MDPS Malfunction Warning lamp OFF; 1=MDPS Malfunciton Warning lamp BLANKING(2Hz); 2=MDPS Malfunction Warning lamp ON; 3=MDPS Malfunction Warning lamp BLANKING(1Hz); 4=RESERVED; 5=RESERVED; 6=RESERVED; 7=Error Indicator
- `MDPS_Typ` source enum: 0=C-MDPS; 1=R-MDPS(Dual Pinion); 2=R-MDPS(Belt); 3=SbW; 4=RESERVED; 5=RESERVED; 6=RESERVED; 7=Error Indicator
- `MDPS_EngIdleRpmReq` source enum: 0=Current Consumption is Low; 1=Current Consumption is High; 2=:RESERVED; 3=Error Indicator
- `MDPS_VsmPlgInSta` source enum: 0=The VSM function of MDPS is not plugged in (unplugged).; 1=The VSM function of MDPS is plugged in.; 2=RESERVED; 3=Error Indicator
- `MDPS_VsmDfctvSta` source enum: 0=MDPS is not defective; 1=MDPS is defective; 2=RESERVED; 3=Error Indicator
- `MDPS_VsmSigErrSta` source enum: 0=VSM1 signal is not error; 1=VSM signal is error; 2=RESERVED; 3=Error Indicator
- `MDPS_PaPlugInSta` source enum: 0=The PA function of MDPS is not plugged in (unplugged).; 1=The PA function of MDPS is plugged in.; 2=RESERVED; 3=Error Indicator
- `MDPS_PaModeSta` source enum: 0=RESERVED; 1=Steering still in initialization phase; 2=Steering ready, waits for PA command; 3=Steering set in standby by PA; 4=Steering requested to go to first activation step; 5=Steering requested to go to final activation step; 6=Steering went to error internally; 7=Steering aborted the automatic function; 8=RESERVED; 9=RESERVED; 10=RESERVED; 11=RESERVED; 12=RESERVED; 13=RESERVED; 14=RESERVED; 15=Error Indicator
- `MDPS_PaCanfltSta` source enum: 0=No Failure; 1=Failure about Pa can message; 2=RESERVED; 3=Error Indicator
- `MDPS_LkaPlgInSta` source enum: 0=The LKA function of MDPS is not plugged in (unplugged).; 1=The LKA function of MDPS is plugged in.; 2=RESERVED; 3=Error Indicator
- `MDPS_LkaToiActvSta` source enum: 0=Deactivated; 1=Activated; 2=RESERVED; 3=Error Indicator
- `MDPS_LkaToiUnblSta` source enum: 0=Available; 1=Unavailable; 2=RESERVED; 3=Error Indicator
- `MDPS_LkaToiFltSta` source enum: 0=No fault; 1=Faulty; 2=RESERVED; 3=Error Indicator
- `MDPS_LkaFailSta` source enum: 0=Not in fail state; 1=In fail state; 2=RESERVED; 3=Error Indicator
- `MDPS_ResetOpSta` source enum: 0=Normal; 1=Reset Complete; 2=Reset Fail; 3=RESERVED
- `MDPS_CurrModVal` source enum: 0=Comfort Mode; 1=Sport Mode; 2=Sport+ Mode; 3=Chauffeur Mode; 4=Mild Mode; 5=Drift Mode; 6=RESERVED; 7=Error Indicator
- `MDPS_VsmActResp` source enum: 0=VSM Control inactivated; 1=VSM Control Activated; 2=RESERVED; 3=Invalid
- `Reprogram_State_MDPS` source enum: 0=normal operation; 1=reprogramming; 2=RESERVED; 3=Error Indicator
- `MDPS_PaStrAnglVal` source enum: 0=0 (0x000~0x7FFD valid range); 32765=32765 (0x000~0x7FFD valid range); 32766=Not Initialized; 32767=Error Indicator; 32768=32768 (0x8000~0xFFFF valid range); 65535=65535 (0x8000~0xFFFF valid range)
- `MDPS_LoamModSta` source enum: 0=Normal; 1=Loam Flag Warning Display; 2=RESERVED; 3=Error Indicator
- `MDPS_HDPSpprtSWVer` source enum: 0=HDP-MDPS Control Requirement Version; 1=HDP-MDPS Control Requirement Version; 2=HDP-MDPS Control Requirement Version; 3=HDP-MDPS Control Requirement Version; 4=HDP-MDPS Control Requirement Version; 5=HDP-MDPS Control Requirement Version; 6=HDP-MDPS Control Requirement Version; 7=HDP-MDPS Control Requirement Version; 8=HDP-MDPS Control Requirement Version; 9=HDP-MDPS Control Requirement Version; 10=HDP-MDPS Control Requirement Version; 11=HDP-MDPS Control Requirement Version; 12=HDP-MDPS Control Requirement Version; 13=HDP-MDPS Control Requirement Version; 14=HDP-MDPS Control Requirement Version; 15=HDP-MDPS Control Requirement Version; 16=HDP-MDPS Control Requirement Version; 17=HDP-MDPS Control Requirement Version; 18=HDP-MDPS Control Requirement Version; 19=HDP-MDPS Control Requirement Version; 20=HDP-MDPS Control Requirement Version; 21=HDP-MDPS Control Requirement Version; 22=HDP-MDPS Control Requirement Version; 23=HDP-MDPS Control Requirement Version; 24=HDP-MDPS Control Requirement Version; 25=HDP-MDPS Control Requirement Version; 26=HDP-MDPS Control Requirement Version; 27=HDP-MDPS Control Requirement Version; 28=HDP-MDPS Control Requirement Version; 29=HDP-MDPS Control Requirement Version; 30=HDP-MDPS Control Requirement Version; 31=HDP-MDPS Control Requirement Version
- `MDPS_ADASAciActvSta` source enum: 0=INIT; 1=INACTIVE; 2=ACTIVE35(ACTIVE); 3=ACTIVE33(Redundancy); 4=RESERVED; 5=RESERVED; 6=RESERVED; 7=RESERVED; 8=RESERVED; 9=RESERVED; 10=RESERVED; 11=RESERVED; 12=RESERVED; 13=RESERVED; 14=RESERVED; 15=RESERVED
- `MDPS_ADASAciPluginSta` source enum: 0=ADASAciNotPlugged-in; 1=ADASAciPlugged-in; 2=RESERVED; 3=ErrorIndicator
- `MDPS_ADAS_AciFltSig` source enum: 0=No Fault; 1=MDPS Fault; 2=Fault Except MDPS; 3=Both Fault
- `MDPS_EstStrAnglVal` source enum: 0=0 (0x000~0x7FFD valid range); 32765=32765 (0x000~0x7FFD valid range); 32766=Not Initialized; 32767=Error Indicator; 32768=32768 (0x8000~0xFFFF valid range); 65535=65535 (0x8000~0xFFFF valid range)
- `MDPS_ADASAciActvSta_Lv2` source enum: 0=Not USED; 1=INACTIVE; 2=ACTIVE35(Active); 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Reserved; 8=Reserved; 9=Reserved; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=Reserved; 15=Reserved
- `MDPS_ADAS_AciFltSig_Lv2` source enum: 0=No Fault; 1=MDPS Fault; 2=Fault Except MDPS; 3=Both Fault; 4=Temporarily Unavailable; 5=Temporarily Unavailable and MDPS Fault; 6=Temporarily Unavailable and Fault Except MDPS; 7=Temporarily Unavailable and Both Fault
- `MDPS_TpaPlugInSta` source enum: 0=The TPA function of MDPS is not plugged in (unplugged); 1=The TPA function of MDPS is plugged in; 2=RESERVED; 3=Error Indicator
- `MDPS_TpaCanfltSta` source enum: 0=No Failure; 1=Failure about Tpa can message; 2=RESERVED; 3=Error Indicator
- `MDPS_TpaModeSta` source enum: 0=RESERVED; 1=Steering still in initialization phase; 2=Steering ready, waits for TPA command; 3=Steering set in standby by TPA; 4=Steering requested to go to first activation step activation step; 5=Steering requested to go to final activation step; 6=Steering went to error internally; 7=Steering aborted the automatic function; 14=RESERVED; 15=Error Indicator
- `MDPS_TpaStrAnglVal` source enum: 0=0 (0x000~0x7FFD valid range); 32765=32765 (0x000~0x7FFD valid range); 32766=Not Initialized; 32767=Error Indicator; 32768=32768 (0x8000~0xFFFF valid range); 65535=65535 (0x8000~0xFFFF valid range)

- 미정의 bit 구간: 56:3, 93:3, 119:1, 151:1, 176:16
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 48:1, 64:12, 80:13, 128:16
- CRC16 probe: 65,683 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-0f5"></a>
## 0x0F5 의미 미확정

bus 0, 32 B, 65,686 records, 평균 100.0044 Hz. 물리 신호 의미 미확정

변화 bit 169개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 40:48, 92:1, 96:11, 112:9, 128:42, 176:10, 216:7, 224:7, 232:5, 240:5
- 전체 변화 bit 구간: 0:24, 40:48, 92:1, 96:11, 112:9, 128:42, 176:10, 216:7, 224:7, 232:5, 240:5
- CRC16 probe: 65,686 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-0ff"></a>
## 0x0FF 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-10a"></a>
## 0x10A 의미 미확정

bus 0, 32 B, 65,685 records, 평균 100.0029 Hz. 물리 신호 의미 미확정

변화 bit 133개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 24:26, 74:6, 84:6, 96:4, 104:4, 112:5, 120:2, 128:5, 144:10, 164:1, 176:1, 192:14, 225:19, 247:6
- 전체 변화 bit 구간: 0:50, 74:6, 84:6, 96:4, 104:4, 112:5, 120:2, 128:5, 144:10, 164:1, 176:1, 192:14, 225:19, 247:6
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 연동 후보 LE 24:16 -> speed_kph, holdout r=0.999854. 물리 의미 미확정.

<a id="id-11a"></a>
## 0x11A FR_CMR_01_10ms

bus 0, 16 B, 65,676 records, 평균 99.9892 Hz. 전방 카메라 상태 후보

변화 bit 27개. 정의/헤더의 구조 coverage 96/128 bit. 의미 source가 있는 영역 72 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| FR_CMR_Crc1Val | 0:16 | U / LE | raw x 1 + 0 |  | 44 ~ 65508 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L157) |
| FR_CMR_AlvCnt1Val | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L158) |
| HBA_SysOptSta | 24:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L159) |
| HBA_SysSta | 26:3 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L160) |
| HBA_IndLmpReq | 29:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L161) |
| iHBAref_VehLftSta | 31:2 | U / LE | raw x 1 + 0 |  | 0 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L162) |
| iHBAref_VehCtrSta | 33:2 | U / LE | raw x 1 + 0 |  | 0 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L163) |
| iHBAref_VehRtSta | 35:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L164) |
| iHBAref_ILLAmbtSta | 37:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L165) |
| FCA_Equip_MFC | 39:3 | U / LE | raw x 1 + 0 |  | 4 ~ 4 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L166) |
| HBA_OptUsmSta | 42:2 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L167) |
| FCAref_FusSta | 45:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L168) |
| DAW_LVDA_PUDis | 48:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L169) |
| DAW_LVDA_OptUsmSta | 50:2 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L170) |
| DAW_OptUsmSta | 52:3 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L171) |
| DAW_SysSta | 55:4 | U / LE | raw x 1 + 0 |  | 5 ~ 5 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L172) |
| DAW_WrnMsgSta | 59:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L173) |
| DAW_TimeRstReq | 62:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L174) |
| DAW_SnstvtyModRetVal | 64:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L175) |
| FR_CMR_SCCEquipSta | 85:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L176) |
| FR_CMR_ReqADASMapMsgVal | 96:16 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L177) |
| FR_CMR_SwVer1Val | 112:4 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L178) |
| FR_CMR_SwVer2Val | 120:8 | U / LE | raw x 1 + 0 |  | 5 ~ 5 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L179) |

- `HBA_SysOptSta` source enum: 0=None HBA Option (Default); 1=HBA Option; 2=Reserved; 3=Error indicator
- `HBA_SysSta` source enum: 0=HBA Disable; 1=HBA Enable & High Beam Off; 2=HBA Enable & High Beam On; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=System Fail
- `HBA_IndLmpReq` source enum: 0=HBA Indicator Lamp Off; 1=HBA Indicator Lamp On; 2=Reserved; 3=Error indicator
- `FCA_Equip_MFC` source enum: 0=No Coding; 1=Sensor Fusion FCA; 2=Camera only FCA; 3=No FCA Option; 4=ADAS_DRV Option; 5=Reserved; 6=Not used; 7=Error indicator
- `HBA_OptUsmSta` source enum: 0=None HBA Option (Default); 1=HBA Function Off; 2=HBA Function On; 3=Invalid (Fail)
- `DAW_LVDA_PUDis` source enum: 0=Default; 1=Display “Leading vehicle departure alert”; 2=Reserved; 3=Error indicator
- `DAW_LVDA_OptUsmSta` source enum: 0=No Option (default); 1=Off; 2=On; 3=Error Indicator
- `DAW_OptUsmSta` source enum: 0=None DAW Option (Default); 1=System Off; 2=System On; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid (Gray)
- `DAW_SysSta` source enum: 0=System Off; 1=Attention Level  1; 2=Attention Level  2; 3=Attention Level  3; 4=Attention Level  4; 5=Attention Level  5; 6=Reserved; 7=Reserved; 8=Reserved; 9=Reserved; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=System Standby; 15=System Fail
- `DAW_WrnMsgSta` source enum: 0=No Warning; 1=Rest Recommend Warning; 2=Hands-Off TMS call request; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Error indicator
- `DAW_TimeRstReq` source enum: 0=No signal; 1=Time reset; 2=Reserved; 3=Error indicator
- `DAW_SnstvtyModRetVal` source enum: 0=Default; 1=Late; 2=Normal; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid
- `FR_CMR_SCCEquipSta` source enum: 0=Not Applied; 1=Applied; 2=Not used; 3=Error Indicator

- 미정의 bit 구간: 44:1, 67:18, 87:9, 116:4
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 32:3
- CRC16 probe: 65,676 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-120"></a>
## 0x120 의미 미확정

bus 0, 32 B, 65,687 records, 평균 100.0059 Hz. 물리 신호 의미 미확정

변화 bit 146개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 24:26, 74:8, 84:8, 96:5, 104:3, 112:5, 120:7, 128:5, 144:10, 164:1, 176:1, 192:14, 225:21, 247:8
- 전체 변화 bit 구간: 0:50, 74:8, 84:8, 96:5, 104:3, 112:5, 120:7, 128:5, 144:10, 164:1, 176:1, 192:14, 225:21, 247:8
- CRC16 probe: 65,687 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-125"></a>
## 0x125 STEERING_SENSORS

bus 0, 16 B, 65,343 records, 평균 99.4822 Hz. 핸들 각도와 회전속도 크기

변화 bit 47개. 정의/헤더의 구조 coverage 48/128 bit. 의미 source가 있는 영역 24 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 6 ~ 65531 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L197) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L198) |
| STEERING_ANGLE | 24:16 | S / LE | raw x -0.1 + 0 | deg | -332 ~ 364.8 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L199) |
| STEERING_RATE | 40:8 | U / LE | raw x 4 + 0 | deg/s | 0 ~ 268 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L200) |
| STEERING_ANGLE | 24:16 | S / LE | raw x 0.1 + 0 | deg | -364.8 ~ 332 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L184) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `STEERING_RATE`와 같은 배치/변환의 alias: STEERING_RATE (기존)

- 미정의 bit 구간: 48:80
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:47
- CRC16 probe: 65,343 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-12a"></a>
## 0x12A LFA

bus 0, 16 B, 65,683 records, 평균 99.9998 Hz. 조향 토크 요청과 LFA 상태

변화 bit 47개. 정의/헤더의 구조 coverage 110/128 bit. 의미 source가 있는 영역 78 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 1 ~ 65532 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L203) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L204) |
| LKA_MODE | 24:3 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L205) |
| LKA_ACTIVE | 27:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L206) |
| LKA_WARNING | 32:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L207) |
| LKA_ICON | 38:2 | U / LE | raw x 1 + 0 |  | 1 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L208) |
| FCA_SYSWARN | 40:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L209) |
| TORQUE_REQUEST | 41:11 | U / LE | raw x 1 + -1024 |  | -186 ~ 94 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L210) |
| STEER_REQ | 52:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L211) |
| LFA_BUTTON | 56:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L212) |
| VALUE63 | 63:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L213) |
| VALUE64 | 64:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L214) |
| NEW_SIGNAL_1 | 75:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L215) |
| LKAS_ANGLE_ACTIVE | 77:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L216) |
| HAS_LANE_SAFETY | 80:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L217) |
| LKAS_ANGLE_CMD | 82:14 | S / LE | raw x -0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L218) |
| LKAS_ANGLE_MAX_TORQUE | 96:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L219) |
| DampingGain | 104:8 | U / LE | raw x 1 + 0 |  | 40 ~ 117 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L220) |
| LKA_RcgSta | 27:3 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L191) |
| LKA_LHLnWrnSta | 30:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L192) |
| LKA_RHLnWrnSta | 32:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L193) |
| LKA_HndsoffSnd | 34:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L194) |
| LKA_StrSnd | 36:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L195) |
| LKA_SysIndReq | 38:3 | U / LE | raw x 1 + 0 |  | 1 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L196) |
| ActToiSta | 52:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L198) |
| ToiFltSta | 54:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L199) |
| BCA_Rear_WrnSta | 57:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L201) |
| LKA_SysWrn | 60:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L202) |
| FCA_LO_WrnSta | 64:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L203) |
| FCA_LS_WrnSta | 68:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L204) |
| LKA_OnOffEquip2Sta | 72:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L205) |
| LKA_UsmMod | 80:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L206) |
| Info_PedtrnDst | 84:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L207) |
| ELK_SysFlrSta | 85:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L208) |
| ELK_SymbDisp | 90:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L209) |
| FCA_ESA_WrnSta | 93:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L210) |
| FCA_ESA_CtrlSta | 101:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L211) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `LKA_MODE` source enum: 1=warning only; 2=assist; 6=off
- `LKA_MODE`와 같은 배치/변환의 alias: LKA_OptUsmSta (기존)
- `LKA_ICON` source enum: 0=hidden; 1=grey; 2=green; 3=flashing green
- `TORQUE_REQUEST`와 같은 배치/변환의 alias: StrTqReqVal (기존)
- `LFA_BUTTON`와 같은 배치/변환의 alias: LFA_BUTTON (기존)
- `DampingGain`와 같은 배치/변환의 alias: Damping_Gain (기존)
- `LKA_RcgSta` source enum: 0=Not Recognized; 1=Left Lane Recognition; 2=Right Lane Recognition; 3=Full Lane Recognition; 4=Reserved; 5=Reserved; 6=Reserved; 7=Reserved
- `LKA_LHLnWrnSta` source enum: 0=No Warning; 1=Lane Departure Warning; 2=Reserved; 3=Reserved
- `LKA_RHLnWrnSta` source enum: 0=No Warning; 1=Lane Departure Warning; 2=Reserved; 3=Reserved
- `LKA_HndsoffSnd` source enum: 0=Off; 1=Hands-off Sound Warning; 2=Reserved; 3=Reserved
- `LKA_StrSnd` source enum: 0=Off; 1=Warning; 2=Reserved; 3=Reserved
- `LKA_SysIndReq` source enum: 0=Off; 1=Unavailable_(Grey On); 2=Lane Recognized_(Green On); 3=Lane Departure_(Green Blink); 4=System Fail_(Orange On); 5=Not Calibrated_(Orange Blink); 6=Regulation_(Orange On); 7=Reserved
- `ActToiSta` source enum: 0=De-activate TOI; 1=Activate TOI; 2=Reserved; 3=Error indicator
- `ToiFltSta` source enum: 0=No Fault; 1=Fault; 2=Reserved; 3=Error indicatorr
- `LKA_SysWrn` source enum: 0=No Info; 1=Reserved; 2=Reserved; 3=Reserved; 4=Hands-Off Warning 1; 5=Hands-Off Warning 2; 6=Hands-Off Warning 3; 7=Reserved; 8=Reserved; 9=System Automatic off; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=Reserved; 15=System Fail
- `FCA_LS_WrnSta` source enum: 3=ELK Steering to the right
- `LKA_UsmMod` source enum: 0=NONE; 1=LKA 1 mode; 2=Reserved; 3=LDW
- `ELK_SymbDisp` source enum: 3=Evasive Action Taking Place

- 미정의 bit 구간: 78:2, 112:16
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 27:2, 38:2, 41:12, 104:7
- CRC16 probe: 65,683 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-130"></a>
## 0x130 GEAR_SHIFTER

bus 0, 16 B, 65,685 records, 평균 100.0029 Hz. 기어 셀렉터

변화 bit 24개. 정의/헤더의 구조 coverage 32/128 bit. 의미 source가 있는 영역 8 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 232 ~ 65373 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L227) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L228) |
| PARK_BUTTON | 32:2 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L229) |
| KNOB_POSITION | 40:3 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L230) |
| GEAR | 64:3 | U / LE | raw x 1 + 0 |  | 4 ~ 4 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L231) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `PARK_BUTTON` source enum: 1=Pressed; 2=Not Pressed
- `PARK_BUTTON`와 같은 배치/변환의 alias: PARK_BUTTON (기존)
- `KNOB_POSITION` source enum: 1=R; 2=N (on R side); 3=Centered; 4=N (on D side); 5=D
- `KNOB_POSITION`와 같은 배치/변환의 alias: KNOB_POSITION (기존)
- `GEAR` source enum: 1=P; 2=R; 3=N; 4=D
- `GEAR`와 같은 배치/변환의 alias: GEAR (기존)

- 미정의 bit 구간: 24:8, 34:6, 43:21, 67:61
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 65,685 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-145"></a>
## 0x145 의미 미확정

bus 0, 32 B, 32,844 records, 평균 50.0037 Hz. 물리 신호 의미 미확정

변화 bit 45개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:24, 64:1, 104:10, 168:8, 222:2
- 전체 변화 bit 구간: 0:24, 64:1, 104:10, 168:8, 222:2
- CRC16 probe: 32,778 통과, 66 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-15a"></a>
## 0x15A 의미 미확정

bus 0, 8 B, 65,687 records, 평균 100.0059 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:40
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 65,687 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-160"></a>
## 0x160 ADRV_0x160

bus 0, 16 B, 32,841 records, 평균 49.9992 Hz. AEB 설정과 경고 후보

변화 bit 24개. 정의/헤더의 구조 coverage 93/128 bit. 의미 source가 있는 영역 2 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 64 ~ 65525 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L237) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L238) |
| AEB_SETTING | 24:2 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L239) |
| NEW_SIGNAL_1 | 30:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L240) |
| NEW_SIGNAL_4 | 31:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L241) |
| NEW_SIGNAL_5 | 39:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L242) |
| NEW_SIGNAL_6 | 47:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L243) |
| NEW_SIGNAL_2 | 55:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L244) |
| SET_ME_2 | 56:8 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L245) |
| SET_ME_FF | 64:8 | U / LE | raw x 1 + 0 |  | 255 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L246) |
| SET_ME_FC | 72:8 | U / LE | raw x 1 + 0 |  | 252 ~ 252 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L247) |
| SET_ME_9 | 80:8 | U / LE | raw x 1 + 0 |  | 25 ~ 25 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L248) |
| NEW_SIGNAL_3 | 95:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L249) |
| NEW_SIGNAL_7 | 102:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L250) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `AEB_SETTING` source enum: 1=off; 2=warning only; 3=active assist
- `AEB_SETTING`와 같은 배치/변환의 alias: AEB_SETTING (기존)
- `SET_ME_2`와 같은 배치/변환의 alias: SET_ME_2 (기존)
- `SET_ME_FF`와 같은 배치/변환의 alias: SET_ME_FF (기존)
- `SET_ME_FC`와 같은 배치/변환의 alias: SET_ME_FC (기존)
- `SET_ME_9`와 같은 배치/변환의 alias: SET_ME_9 (기존)

- 미정의 bit 구간: 26:4, 96:6, 103:25
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 32,841 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-175"></a>
## 0x175 TCS

bus 0, 24 B, 32,843 records, 평균 50.0022 Hz. 브레이크, SCC 요청/허용, ESC/FCA 상태

변화 bit 38개. 정의/헤더의 구조 coverage 168/192 bit. 의미 source가 있는 영역 43 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 2 ~ 65533 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L358) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L359) |
| NEW_SIGNAL_4 | 24:7 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L360) |
| NEW_SIGNAL_9 | 31:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L361) |
| aBasis | 32:11 | U / LE | raw x 0.01 + -10.23 | m/s^2 | -2.09 ~ 1.75 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L362) |
| NEW_SIGNAL_1 | 47:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L363) |
| ACCEL_REF_ACC | 48:11 | S / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L364) |
| NEW_SIGNAL_2 | 63:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L365) |
| SCC_OptTyp | 64:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L366) |
| ACCEnable | 67:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L367) |
| ACC_REQ | 68:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L368) |
| NEW_SIGNAL_3 | 69:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L369) |
| SCC_ReqLimSta | 70:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L370) |
| ESC_StdStillVal | 72:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L371) |
| BrakeLight | 74:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L372) |
| ESC_DclEnblReq | 76:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L373) |
| NEW_SIGNAL_5 | 79:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L374) |
| NEW_SIGNAL_21 | 80:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L375) |
| DriverBraking | 81:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L376) |
| NEW_SIGNAL_20 | 82:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L377) |
| NEW_SIGNAL_11 | 83:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L378) |
| DriverBrakingLowSens | 84:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L379) |
| NEW_SIGNAL_10 | 85:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L380) |
| ESC_PrkBrkActvSta | 86:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L381) |
| NEW_SIGNAL_12 | 95:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L382) |
| FCA_EquipSta | 96:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L383) |
| FCA_AvlblSta | 98:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L384) |
| NEW_SIGNAL_13 | 103:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L385) |
| NEW_SIGNAL_14 | 111:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L386) |
| NEW_SIGNAL_15 | 119:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L387) |
| NEW_SIGNAL_16 | 127:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L388) |
| NEW_SIGNAL_6 | 128:4 | U / LE | raw x 1 + 0 |  | 15 ~ 15 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L389) |
| NEW_SIGNAL_17 | 133:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L390) |
| NEW_SIGNAL_7 | 135:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L391) |
| PROBABLY_EQUIP | 136:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L392) |
| NEW_SIGNAL_18 | 143:6 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L393) |
| NEW_SIGNAL_19 | 151:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L394) |
| NEW_SIGNAL_8 | 183:16 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L395) |
| EQUIP_MAYBE | 64:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L360) |
| NEW_SIGNAL_5 | 72:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L363) |
| NEW_SIGNAL_2 | 74:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L364) |
| NEW_SIGNAL_3 | 76:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L365) |
| AEB_EQUIP_MAYBE | 96:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L369) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `NEW_SIGNAL_4`와 같은 배치/변환의 alias: NEW_SIGNAL_4 (기존)
- `aBasis`와 같은 배치/변환의 alias: aBasis (기존)
- `ACCEL_REF_ACC`와 같은 배치/변환의 alias: ACCEL_REF_ACC (기존)
- `SCC_OptTyp` source enum: 0=SCC is not equipped; 1=SCC is equipped; 2=Not used; 3=Error Indicator
- `ACCEnable` source enum: 0=SCC ready; 1=SCC temp fault; 2=SCC permanent fault; 3=SCC permanent fault, communication issue
- `ACCEnable`와 같은 배치/변환의 alias: ACCEnable (기존)
- `ACC_REQ`와 같은 배치/변환의 alias: ACC_REQ (기존)
- `SCC_ReqLimSta` source enum: 0=No request; 1=Limited No Limitation; 2=Acceleration Limited; 3=Deceleration Limited
- `ESC_StdStillVal` source enum: 0=No stand still detected; 1=stand still detected; 2=Not used; 3=Error Indicator
- `ESC_DclEnblReq` source enum: 0=Disable deceleration control; 1=Enable deceleration control; 2=Not used; 3=Error Indicator
- `NEW_SIGNAL_21`와 같은 배치/변환의 alias: NEW_SIGNAL_1 (기존)
- `DriverBraking`와 같은 배치/변환의 alias: DriverBraking (기존)
- `DriverBrakingLowSens`와 같은 배치/변환의 alias: DriverBrakingLowSens (기존)
- `ESC_PrkBrkActvSta` source enum: 0=Parking brake is not activated; 1=Parking brake is activated; 2=Not used; 3=Error Indicator
- `FCA_EquipSta` source enum: 0=FCA is not equipped; 1=FCA is equipped; 2=Not used; 3=Error Indicator
- `FCA_AvlblSta` source enum: 0=Available; 1=Temporarily not available; 2=Permanently not available; 3=FCA Communication Error
- `NEW_SIGNAL_6`와 같은 배치/변환의 alias: NEW_SIGNAL_6 (기존)
- `NEW_SIGNAL_7`와 같은 배치/변환의 alias: NEW_SIGNAL_7 (기존)
- `PROBABLY_EQUIP`와 같은 배치/변환의 alias: PROBABLY_EQUIP (기존)

- 미정의 bit 구간: 152:24
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 32:11, 72:1, 81:1, 84:1
- CRC16 probe: 32,843 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-180"></a>
## 0x180 의미 미확정

bus 0, 8 B, 32,873 records, 평균 50.0479 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:40
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 32,873 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 적용 제외: CAM_0x180 32 B, 관측 8 B.
- 적용 제외: CAM_0x180 32 B, 관측 8 B.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: AMP_HU_PE_01 (hyundai_2015_mcan.dbc)

<a id="id-185"></a>
## 0x185 CAM_0x185

bus 0, 8 B, 32,873 records, 평균 50.0479 Hz. 카메라 계열 헤더만 정의

변화 bit 24개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 128 ~ 65311 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L418) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L419) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)

- 미정의 bit 구간: 24:40
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 32,873 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: AMP_HU_PE_05 (hyundai_2015_mcan.dbc)

<a id="id-19a"></a>
## 0x19A 의미 미확정

bus 0, 32 B, 65,687 records, 평균 100.0059 Hz. 물리 신호 의미 미확정

변화 bit 32개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 80:8
- 전체 변화 bit 구간: 0:24, 80:8
- CRC16 probe: 65,687 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1a0"></a>
## 0x1A0 SCC_CONTROL

bus 0, 32 B, 32,841 records, 평균 49.9992 Hz. SCC 가속도 요청, 설정 속도, 선행차 및 상태

변화 bit 55개. 정의/헤더의 구조 coverage 249/256 bit. 의미 source가 있는 영역 138 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 6 ~ 65531 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L422) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L423) |
| ACC_ObjDist | 24:11 | U / LE | raw x 0.1 + 0 | m | 7.7 ~ 204.6 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L424) |
| ACC_ObjRelSpd | 35:12 | U / LE | raw x 0.1 + -170 | m/s | -12.8 ~ 239.4 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L425) |
| ACC_ObjLatPos | 47:9 | S / LE | raw x 0.1 + -20 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L426) |
| ZEROS_7 | 63:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L427) |
| SysFailState | 64:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L428) |
| MainMode_ACC | 66:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L429) |
| ACCMode | 68:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L430) |
| TakeOverReq | 73:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L431) |
| InfoDisplay | 74:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L432) |
| DriverAlert | 77:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L433) |
| ObjDistLevel | 80:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L434) |
| DISTANCE_SETTING | 88:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L435) |
| VSetDis | 103:8 | U / BE | raw x 1 + 0 | km/h or mph | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L436) |
| NSCCOper | 104:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L437) |
| NSCCOnOff | 106:2 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L438) |
| HUD_LEAD_INFO | 108:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L439) |
| DriveMode | 112:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L440) |
| aReqValue | 128:11 | U / LE | raw x 0.01 + -10.23 | m/s^2 | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L441) |
| aReqRaw | 140:11 | U / LE | raw x 0.01 + -10.23 | m/s^2 | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L442) |
| JerkUpperLimit | 158:7 | U / BE | raw x 0.1 + 0 |  | 0.8 ~ 2 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L443) |
| JerkLowerLimit | 166:7 | U / BE | raw x 0.1 + 0 | m/s^3 | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L444) |
| AccelLimitBandUpper | 173:6 | U / BE | raw x 0.02 + 0 |  | 0 ~ 0.1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L445) |
| AccelLimitBandLower | 181:6 | U / BE | raw x 0.02 + 0 |  | 0 ~ 0.2 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L446) |
| StopReq | 185:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L447) |
| CRUSE_INFO_SET_2 | 189:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L448) |
| TARGET_DISTANCE | 192:11 | U / LE | raw x 0.1 + 0 | m | 204.6 ~ 204.6 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L449) |
| ZEROS_2 | 207:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L450) |
| ZEROS | 215:48 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L451) |
| ACC_ObjRelSpd | 35:9 | U / LE | raw x 0.1 + -16.4 | m/s | -12.8 ~ 34.6 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L378) |
| SET_ME_3 | 45:2 | U / BE | raw x 1 + 0 |  | 3 ~ 3 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L379) |
| ObjValid | 46:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L380) |
| SET_ME_TMP_64 | 55:8 | U / BE | raw x 1 + 0 |  | 100 ~ 100 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L381) |
| ZEROS_9 | 71:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L386) |
| CRUISE_STANDSTILL | 76:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L387) |
| ZEROS_5 | 77:11 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L388) |
| DISTANCE_SETTING | 88:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L389) |
| ZEROS_8 | 95:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L390) |
| NEW_SIGNAL_6 | 104:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L392) |
| SET_ME_2 | 105:3 | U / LE | raw x 1 + 0 |  | 4 ~ 4 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L393) |
| ZEROS_10 | 111:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L395) |
| ZEROS_6 | 119:16 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L396) |
| NEW_SIGNAL_2 | 168:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L401) |
| NEW_SIGNAL_8 | 170:4 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L402) |
| OBJ_STATUS | 176:3 | U / LE | raw x 1 + 0 |  | 0 ~ 2 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L403) |
| ZEROS_4 | 183:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L404) |
| StopReq | 184:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L405) |
| ZEROS_3 | 191:7 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L406) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `ACC_ObjDist`와 같은 배치/변환의 alias: ACC_ObjDist (기존)
- `ZEROS_7`와 같은 배치/변환의 alias: ZEROS_7 (기존)
- `SysFailState`와 같은 배치/변환의 alias: NEW_SIGNAL_1 (기존)
- `MainMode_ACC`와 같은 배치/변환의 alias: MainMode_ACC (기존)
- `ACCMode` source enum: 0=off; 1=enabled; 2=driver_override; 3=off_maybe_fault; 4=cancelled
- `ACCMode`와 같은 배치/변환의 alias: ACCMode (기존)
- `VSetDis`와 같은 배치/변환의 alias: VSetDis (기존)
- `HUD_LEAD_INFO`와 같은 배치/변환의 alias: SCC_ObjSta (기존)
- `aReqValue`와 같은 배치/변환의 alias: aReqValue (기존)
- `aReqRaw`와 같은 배치/변환의 alias: aReqRaw (기존)
- `JerkUpperLimit`와 같은 배치/변환의 alias: JerkUpperLimit (기존)
- `JerkLowerLimit`와 같은 배치/변환의 alias: JerkLowerLimit (기존)
- `TARGET_DISTANCE`와 같은 배치/변환의 alias: NEW_SIGNAL_15 (기존)
- `ZEROS_2`와 같은 배치/변환의 alias: ZEROS_2 (기존)
- `ZEROS`와 같은 배치/변환의 alias: ZEROS (기존)

- 미정의 bit 구간: 67:1, 139:1, 151:1, 159:1, 167:1, 174:2
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:44, 46:1, 152:5, 168:3, 177:1, 179:1
- CRC16 probe: 32,841 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1aa"></a>
## 0x1AA CRUISE_BUTTONS_ALT

bus 0, 16 B, 32,873 records, 평균 50.0479 Hz. 대체 크루즈/차선유지 버튼

변화 bit 45개. 정의/헤더의 구조 coverage 128/128 bit. 의미 source가 있는 영역 87 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 5 ~ 65521 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L454) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L455) |
| NEW_SIGNAL_1 | 24:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L456) |
| SET_ME_1 | 28:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L457) |
| DISTANCE_UNIT | 30:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L458) |
| NEW_SIGNAL_2 | 31:3 | U / LE | raw x 1 + 0 |  | 0 ~ 6 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L459) |
| ADAPTIVE_CRUISE_MAIN_BTN | 34:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L460) |
| NEW_SIGNAL_3 | 35:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L461) |
| CRUISE_BUTTONS | 36:3 | U / LE | raw x 1 + 0 |  | 7 ~ 7 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L462) |
| LFA_BTN | 39:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L463) |
| NEW_SIGNAL_4 | 40:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L464) |
| NORMAL_CRUISE_MAIN_BTN | 41:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L465) |
| NEW_SIGNAL_5 | 42:2 | U / LE | raw x 1 + 0 |  | 0 ~ 2 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L466) |
| SET_ME_2 | 44:3 | U / LE | raw x 1 + 0 |  | 1 ~ 2 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L467) |
| NEW_SIGNAL_6 | 47:1 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L468) |
| BYTE6 | 48:8 | U / LE | raw x 1 + 0 |  | 0 ~ 135 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L469) |
| BYTE7 | 56:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L470) |
| CLU_SPEED | 64:8 | U / LE | raw x 1 + 0 |  | 0 ~ 68 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L471) |
| BYTE9 | 72:8 | U / LE | raw x 1 + 0 |  | 40 ~ 40 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L472) |
| BYTE10 | 80:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L473) |
| BYTE11 | 88:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L474) |
| BYTE12 | 96:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L475) |
| BYTE13 | 104:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L476) |
| BYTE14 | 112:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L477) |
| BYTE15 | 120:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L478) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `NEW_SIGNAL_1`와 같은 배치/변환의 alias: NEW_SIGNAL_1 (기존)
- `SET_ME_1`와 같은 배치/변환의 alias: SET_ME_1 (기존)
- `DISTANCE_UNIT`와 같은 배치/변환의 alias: DISTANCE_UNIT (기존)
- `NEW_SIGNAL_2`와 같은 배치/변환의 alias: NEW_SIGNAL_2 (기존)
- `ADAPTIVE_CRUISE_MAIN_BTN`와 같은 배치/변환의 alias: ADAPTIVE_CRUISE_MAIN_BTN (기존)
- `NEW_SIGNAL_3`와 같은 배치/변환의 alias: NEW_SIGNAL_3 (기존)
- `CRUISE_BUTTONS` source enum: 0=none; 1=res_accel; 2=set_decel; 3=gap_distance; 4=pause_resume
- `CRUISE_BUTTONS`와 같은 배치/변환의 alias: CRUISE_BUTTONS (기존)
- `LFA_BTN`와 같은 배치/변환의 alias: LDA_BTN (기존)
- `NEW_SIGNAL_4`와 같은 배치/변환의 alias: NEW_SIGNAL_4 (기존)
- `NORMAL_CRUISE_MAIN_BTN`와 같은 배치/변환의 alias: NORMAL_CRUISE_MAIN_BTN (기존)
- `NEW_SIGNAL_5`와 같은 배치/변환의 alias: NEW_SIGNAL_5 (기존)
- `SET_ME_2`와 같은 배치/변환의 alias: SET_ME_2 (기존)
- `NEW_SIGNAL_6`와 같은 배치/변환의 alias: NEW_SIGNAL_6 (기존)
- `BYTE6`와 같은 배치/변환의 alias: BYTE6 (기존)
- `BYTE7`와 같은 배치/변환의 alias: BYTE7 (기존)
- `CLU_SPEED`와 같은 배치/변환의 alias: BYTE8 (기존)
- `BYTE9`와 같은 배치/변환의 alias: BYTE9 (기존)
- `BYTE10`와 같은 배치/변환의 alias: BYTE10 (기존)
- `BYTE11`와 같은 배치/변환의 alias: BYTE11 (기존)
- `BYTE12`와 같은 배치/변환의 alias: BYTE12 (기존)
- `BYTE13`와 같은 배치/변환의 alias: BYTE13 (기존)
- `BYTE14`와 같은 배치/변환의 alias: BYTE14 (기존)
- `BYTE15`와 같은 배치/변환의 alias: BYTE15 (기존)

- 미정의 bit 구간: 없음
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 28:1, 32:2, 43:3, 48:8, 64:7
- CRC16 probe: 32,873 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1b0"></a>
## 0x1B0 의미 미확정

bus 0, 32 B, 32,844 records, 평균 50.0037 Hz. 물리 신호 의미 미확정

변화 bit 89개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:24, 88:15, 104:7, 120:7, 128:7, 136:3, 144:4, 160:6, 176:3, 184:4, 192:5, 200:4
- 전체 변화 bit 구간: 0:24, 88:15, 104:7, 120:7, 128:7, 136:3, 144:4, 160:6, 176:3, 184:4, 192:5, 200:4
- CRC16 probe: 32,778 통과, 66 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-1b5"></a>
## 0x1B5 CCNC_0x1B5 / FR_CMR_03_50ms

bus 0, 32 B, 13,136 records, 평균 19.9991 Hz. 카메라 상태와 계기판 후보 정의 충돌

변화 bit 142개. 정의/헤더의 구조 coverage 178/256 bit. 의미 source가 있는 영역 154 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 7 ~ 65534 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L481) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L482) |
| LEFT_QUAL | 24:3 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L483) |
| LEFT_LDW | 27:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L484) |
| LEFT_POSITION | 29:14 | S / LE | raw x 0.0039625 + 0 | m | -2.024837 ~ 0.594375 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L485) |
| LEFT_HEADING | 43:10 | S / LE | raw x 0.000976563 + 0 | rad | -0.06054691 ~ 0.06054691 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L486) |
| LEFT_CURVATURE | 64:16 | S / LE | raw x 1e-06 + 0 | 1/m | -0.003913 ~ 0.006753 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L487) |
| LEFT_CURVATURE_DERIVATIVE | 80:16 | S / LE | raw x 4e-09 + 0 | 1/m2 | -0.000131068 ~ 0.000131068 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L488) |
| RIGHT_QUAL | 96:3 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L489) |
| RIGHT_LDW | 99:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L490) |
| RIGHT_POSITION | 101:14 | S / LE | raw x 0.0039625 + 0 | m | -0.1069875 ~ 2.024837 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L491) |
| RIGHT_HEADING | 115:10 | S / LE | raw x 0.000976563 + 0 | rad | -0.06054691 ~ 0.06054691 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L492) |
| RIGHT_CURVATURE | 128:16 | S / LE | raw x 1e-06 + 0 | 1/m | -0.004041 ~ 0.032767 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L493) |
| RIGHT_CURVATURE_DERIVATIVE | 144:16 | S / LE | raw x 4e-09 + 0 | 1/m2 | -0.000131068 ~ 0.000131068 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L494) |
| LEAD | 192:7 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L495) |
| LEAD_SPEED | 200:12 | U / LE | raw x 0.05 + -100 | m/s | -100 ~ -100 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L496) |
| LEAD_DISTANCE | 212:12 | U / LE | raw x 0.05 + 0 | m | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L497) |
| ID_CIPV | 192:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L453) |

- `CHECKSUM`와 같은 배치/변환의 alias: FR_CMR_Crc3Val (기존)
- `COUNTER`와 같은 배치/변환의 alias: FR_CMR_AlvCnt3Val (기존)
- `LEFT_QUAL`와 같은 배치/변환의 alias: Info_LftLnQualSta (기존)
- `LEFT_LDW`와 같은 배치/변환의 alias: Info_LftLnDptSta (기존)
- `LEFT_POSITION`와 같은 배치/변환의 alias: Info_LftLnPosVal (기존)
- `LEFT_HEADING`와 같은 배치/변환의 alias: Info_LftLnHdingAnglVal (기존)
- `LEFT_CURVATURE`와 같은 배치/변환의 alias: Info_LftLnCvtrVal (기존)
- `LEFT_CURVATURE_DERIVATIVE`와 같은 배치/변환의 alias: Info_LftLnCrvtrDrvtvVal (기존)
- `RIGHT_QUAL`와 같은 배치/변환의 alias: Info_RtLnQualSta (기존)
- `RIGHT_LDW`와 같은 배치/변환의 alias: Info_RtLnDptSta (기존)
- `RIGHT_POSITION`와 같은 배치/변환의 alias: Info_RtLnPosVal (기존)
- `RIGHT_HEADING`와 같은 배치/변환의 alias: Info_RtLnHdingAnglVal (기존)
- `RIGHT_CURVATURE`와 같은 배치/변환의 alias: Info_RtLnCvtrVal (기존)
- `RIGHT_CURVATURE_DERIVATIVE`와 같은 배치/변환의 alias: Info_RtLnCrvtrDrvtvVal (기존)
- `LEAD_SPEED`와 같은 배치/변환의 alias: Relative_Velocity (기존)
- `LEAD_DISTANCE`와 같은 배치/변환의 alias: Longitudinal_Distance (기존)

- 미정의 bit 구간: 53:11, 125:3, 160:32, 224:32
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:26, 27:1, 29:24, 64:34, 99:1, 101:24, 128:32
- CRC16 probe: 13,136 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1ba"></a>
## 0x1BA BLINDSPOTS_REAR_CORNERS / ADAS_CMD_50_50ms

bus 0, 24 B, 13,137 records, 평균 20.0006 Hz. ADAS 명령과 후측방 감지 후보 정의 충돌

변화 bit 37개. 정의/헤더의 구조 coverage 95/192 bit. 의미 source가 있는 영역 71 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 1 ~ 65498 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L516) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L517) |
| LEFT_BLOCKED | 24:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L518) |
| INDICATOR_LEFT_TWO | 31:2 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L519) |
| INDICATOR_RIGHT_TWO | 32:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L520) |
| FL_INDICATOR | 46:6 | U / BE | raw x 1 + 0 |  | 0 ~ 45 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L521) |
| FR_INDICATOR | 54:6 | U / BE | raw x 1 + 0 |  | 0 ~ 45 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L522) |
| RIGHT_BLOCKED | 64:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L523) |
| COLLISION_AVOIDANCE_ACTIVE | 68:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L524) |
| INDICATOR_LEFT_THREE | 128:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L525) |
| INDICATOR_RIGHT_THREE | 130:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L526) |
| INDICATOR_LEFT_FOUR | 138:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L527) |
| INDICATOR_RIGHT_FOUR | 141:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L528) |
| BCW_OnOffEquipSta | 28:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L461) |
| BCW_LtSndWrngSta | 34:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L464) |
| BCW_RtSndWrngSta | 36:10 | U / LE | raw x 1 + 0 |  | 0 ~ 480 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L465) |
| BCW_SnstvtyModRetVal | 56:3 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L466) |
| BCW_IndSta | 59:3 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L467) |
| BCA_OnOffEquip2Sta | 62:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L468) |
| BCA_DRV_WarnSta | 76:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L471) |
| BCA_Plus_Deccel_Req | 80:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L472) |
| BCA_Plus_BrkCmdSta | 84:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L473) |
| BCA_Plus_LtWrngSta | 86:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L474) |
| BCA_Plus_RtWrngSta | 88:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L475) |
| BCA_Plus_FuncStat | 93:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L476) |
| BCA_Plus_Sta | 96:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L477) |
| Brake_Control_RL | 110:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L478) |
| Brake_Control_RR | 118:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L479) |
| OSMrrLamp_LtIndSta | 128:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L480) |
| OSMrrLamp_RtIndSta | 130:6 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L481) |

- `CHECKSUM`와 같은 배치/변환의 alias: ADAS_CMD_Crc50Val (기존)
- `COUNTER`와 같은 배치/변환의 alias: ADAS_CMD_AlvCnt50Val (기존)
- `LEFT_BLOCKED`와 같은 배치/변환의 alias: BCW_Sta (기존)
- `INDICATOR_LEFT_TWO`와 같은 배치/변환의 alias: BCW_LtIndSta (기존)
- `INDICATOR_RIGHT_TWO`와 같은 배치/변환의 alias: BCW_RtIndSta (기존)
- `RIGHT_BLOCKED`와 같은 배치/변환의 alias: BCA_Sta (기존)
- `COLLISION_AVOIDANCE_ACTIVE`와 같은 배치/변환의 alias: BCA_OnOffEquipSta (기존)

- 미정의 bit 구간: 25:3, 47:2, 55:1, 65:3, 69:7, 97:13, 112:6, 120:8, 136:2, 139:2, 142:50
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:25, 30:1, 32:1, 41:4, 46:1, 49:1, 51:2, 54:1, 64:1
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1cf"></a>
## 0x1CF CRUISE_BUTTONS

bus 0, 8 B, 32,843 records, 평균 50.0022 Hz. 크루즈/차선유지 버튼과 패들

변화 bit 13개. 정의/헤더의 구조 coverage 22/64 bit. 의미 source가 있는 영역 8 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| _CHECKSUM | 0:8 | U / LE | raw x 1 + 0 |  | 32 ~ 254 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L531) |
| COUNTER | 12:4 | U / LE | raw x 1 + 0 |  | 0 ~ 14 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L532) |
| CRUISE_BUTTONS | 16:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L533) |
| ADAPTIVE_CRUISE_MAIN_BTN | 19:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L534) |
| NORMAL_CRUISE_MAIN_BTN | 21:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L535) |
| LFA_BTN | 23:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L536) |
| RIGHT_PADDLE | 25:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L537) |
| LEFT_PADDLE | 27:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L538) |
| SET_ME_1 | 29:1 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L539) |
| SET_ME_1_ | 40:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L540) |

- `_CHECKSUM`와 같은 배치/변환의 alias: _CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `CRUISE_BUTTONS` source enum: 0=none; 1=res_accel; 2=set_decel; 3=gap_distance; 4=pause_resume
- `CRUISE_BUTTONS`와 같은 배치/변환의 alias: CRUISE_BUTTONS (기존)
- `ADAPTIVE_CRUISE_MAIN_BTN`와 같은 배치/변환의 alias: ADAPTIVE_CRUISE_MAIN_BTN (기존)
- `NORMAL_CRUISE_MAIN_BTN`와 같은 배치/변환의 alias: NORMAL_CRUISE_MAIN_BTN (기존)
- `LFA_BTN`와 같은 배치/변환의 alias: LDA_BTN (기존)
- `RIGHT_PADDLE` source enum: 0=Not Pulled; 1=Pulled
- `RIGHT_PADDLE`와 같은 배치/변환의 alias: RIGHT_PADDLE (기존)
- `LEFT_PADDLE` source enum: 0=Not Pulled; 1=Pulled
- `LEFT_PADDLE`와 같은 배치/변환의 alias: LEFT_PADDLE (기존)
- `SET_ME_1`와 같은 배치/변환의 alias: SET_ME_1 (기존)

- 미정의 bit 구간: 8:4, 20:1, 22:1, 24:1, 26:1, 28:1, 30:10, 41:23
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:8, 12:4, 23:1
- CRC16 probe: 0 통과, 32,843 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-1da"></a>
## 0x1DA ADRV_0x1da

bus 0, 32 B, 658 records, 평균 1.0018 Hz. ADAS 경고 후보

변화 bit 24개. 정의/헤더의 구조 coverage 40/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 65 ~ 65283 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L543) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L544) |
| SET_ME_22 | 31:8 | U / BE | raw x 1 + 0 |  | 103 ~ 103 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L545) |
| SET_ME_41 | 47:8 | U / BE | raw x 1 + 0 |  | 113 ~ 113 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L546) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `SET_ME_22`와 같은 배치/변환의 alias: SET_ME_22 (기존)
- `SET_ME_41`와 같은 배치/변환의 alias: SET_ME_41 (기존)

- 미정의 bit 구간: 32:8, 48:208
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 658 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1e0"></a>
## 0x1E0 LFAHDA_CLUSTER

bus 0, 16 B, 13,137 records, 평균 20.0006 Hz. LFA/HDA 계기판 표시

변화 bit 26개. 정의/헤더의 구조 coverage 56/128 bit. 의미 source가 있는 영역 32 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 52 ~ 65410 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L549) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L550) |
| HDA_OptUsmSta | 24:3 | U / LE | raw x 1 + 0 |  | 2 ~ 2 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L551) |
| LFA_OptUsmSta | 27:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L552) |
| HDA_CntrlModSta | 30:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L553) |
| HDA_InfoPUDis | 32:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L554) |
| HDA_AutoSetSpdSta | 35:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L555) |
| HDA_AutoSetSpdUpdtSta | 37:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L556) |
| HDA_AutoSetSpdVal | 39:8 | U / LE | raw x 1 + 0 | km/h | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L557) |
| HDA_LFA_SymSta | 47:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L558) |
| HDA_LFA_WrnSnd | 49:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L559) |
| HDA_InfoPUDis1 | 51:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L560) |
| HDA_TDMRMDclReq | 54:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L561) |
| NEW_SIGNAL_4 | 24:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L503) |
| NEW_SIGNAL_5 | 25:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L504) |
| NEW_SIGNAL_2 | 30:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L505) |
| HDA_ICON | 31:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L506) |
| NEW_SIGNAL_3 | 49:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L509) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `HDA_OptUsmSta` source enum: 0=Not Applied; 1=Function Off; 2=Function On; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid (Fail)
- `LFA_OptUsmSta` source enum: 0=Not Applied; 1=Function Off; 2=Function On; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid (Fail)
- `HDA_CntrlModSta` source enum: 0=System Deactive (Default); 1=System Ready; 2=System Active; 3=Reserved
- `HDA_InfoPUDis` source enum: 0=No pop-up; 1=system start pop-up; 2=system auto disengaged pop-up by highway off; 3=system auto disengaged pop-up; 4=system Fail pop-up; 5=Hands-off pop up; 6=Hands-off pop up w/ sound; 7=System Automatic off
- `HDA_InfoPUDis`와 같은 배치/변환의 alias: NEW_SIGNAL_1 (기존)
- `HDA_AutoSetSpdSta` source enum: 0=Auto Set Speed Off; 1=Auto Set Speed On; 2=Reserved; 3=Error indicator
- `HDA_AutoSetSpdUpdtSta` source enum: 0=Auto Set Speed Update Off; 1=Auto Set Speed Update On; 2=Reserved; 3=Error indicator
- `HDA_LFA_SymSta` source enum: 0=Off; 1=Gray; 2=Green; 3=Green blink
- `HDA_LFA_SymSta`와 같은 배치/변환의 alias: LFA_ICON (기존)
- `HDA_LFA_WrnSnd` source enum: 0=Off; 1=Additional Warning Sound; 2=Reserved; 3=Error indicator
- `HDA_InfoPUDis1` source enum: 0=No Pop-up; 1=System Automatic off (LFA); 2=System Automatic off (HDA); 3=Reserved; 4=Reserved; 5=Reserved; 6=Not Used; 7=Error Indicator
- `HDA_TDMRMDclReq` source enum: 0=Not automatically deactivated state of LFA (default); 1=Automatically deactivated state of LFA; 2=Reserved; 3=Reserved

- 미정의 bit 구간: 56:72
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 47:2
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1e5"></a>
## 0x1E5 BLINDSPOTS_FRONT_CORNER_1

bus 0, 16 B, 13,136 records, 평균 19.9991 Hz. 전측방 감지 후보

변화 bit 29개. 정의/헤더의 구조 coverage 50/128 bit. 의미 source가 있는 영역 1 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 0 ~ 65527 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L564) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L565) |
| REVERSING | 24:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L566) |
| NEW_SIGNAL_5 | 31:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L567) |
| NEW_SIGNAL_7 | 32:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L568) |
| NEW_SIGNAL_8 | 47:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L569) |
| NEW_SIGNAL_9 | 55:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L570) |
| NEW_SIGNAL_4 | 80:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L571) |
| NEW_SIGNAL_3 | 88:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L572) |
| NEW_SIGNAL_2 | 96:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L573) |
| NEW_SIGNAL_1 | 108:2 | U / BE | raw x 1 + 0 |  | 0 ~ 3 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L574) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `REVERSING`와 같은 배치/변환의 alias: REVERSING (기존)
- `NEW_SIGNAL_5`와 같은 배치/변환의 alias: NEW_SIGNAL_5 (기존)
- `NEW_SIGNAL_7`와 같은 배치/변환의 alias: NEW_SIGNAL_7 (기존)
- `NEW_SIGNAL_8`와 같은 배치/변환의 alias: NEW_SIGNAL_8 (기존)
- `NEW_SIGNAL_9`와 같은 배치/변환의 alias: NEW_SIGNAL_9 (기존)
- `NEW_SIGNAL_4`와 같은 배치/변환의 alias: NEW_SIGNAL_4 (기존)
- `NEW_SIGNAL_3`와 같은 배치/변환의 alias: NEW_SIGNAL_3 (기존)
- `NEW_SIGNAL_2`와 같은 배치/변환의 alias: NEW_SIGNAL_2 (기존)
- `NEW_SIGNAL_1`와 같은 배치/변환의 alias: NEW_SIGNAL_1 (기존)

- 미정의 bit 구간: 25:5, 34:6, 56:24, 81:7, 89:7, 97:10, 109:19
- 미정의 구간 중 변화한 bit: 94:1
- 전체 변화 bit 구간: 0:24, 88:1, 94:1, 96:1, 107:2
- CRC16 probe: 13,136 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1ea"></a>
## 0x1EA ADRV_0x1ea

bus 0, 32 B, 13,137 records, 평균 20.0006 Hz. HDA/차선변경/계기판 표시 후보

변화 bit 24개. 정의/헤더의 구조 coverage 155/256 bit. 의미 source가 있는 영역 113 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 193 ~ 65411 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L577) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L578) |
| NEW_SIGNAL_4 | 25:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L579) |
| NEW_SIGNAL_3 | 26:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L580) |
| NEW_SIGNAL_2 | 27:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L581) |
| NEW_SIGNAL_1 | 28:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L582) |
| LEFT_BLINK_HOLD | 29:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L583) |
| RIGHT_BLINK_HOLD | 30:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L584) |
| NEW_SIGNAL_5 | 31:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L585) |
| HDA_MODE2 | 32:3 | U / LE | raw x 1 + 0 |  | 5 ~ 5 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L586) |
| LANE_LEFT | 36:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L587) |
| NEW_SIGNAL_6 | 39:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L588) |
| LANE_RIGHT | 41:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L589) |
| LANE_CHANGING | 45:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L590) |
| LF_DETECT_DISTANCE | 46:11 | U / LE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L591) |
| LF_DETECT_LATERAL | 70:7 | U / BE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L592) |
| LF_DETECT | 74:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L593) |
| RF_DETECT_DISTANCE | 75:11 | U / LE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L594) |
| RF_DETECT_LATERAL | 94:7 | U / BE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L595) |
| RF_DETECT | 98:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L596) |
| SET_ME_FF | 120:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L597) |
| LR_DETECT_DISTANCE | 139:8 | U / LE | raw x 0.1 + 0 | m | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L598) |
| LR_DETECT_LATERAL | 152:6 | U / LE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L599) |
| LR_DETECT | 162:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L600) |
| RR_DETECT_DISTANCE | 163:8 | U / LE | raw x 0.1 + 0 | m | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L601) |
| RR_DETECT_LATERAL | 172:6 | U / LE | raw x 0.1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L602) |
| RR_DETECT | 186:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L603) |
| AUTOLANECHANGE_MSG | 207:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L604) |
| LANELINE_CURVATURE | 208:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L605) |
| LANELINE_CURVATURE_DIRECTION | 212:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L606) |
| LANELINE_LEFT_POSITION | 239:8 | U / BE | raw x 1 + 0 |  | 15 ~ 15 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L607) |
| LANELINE_RIGHT_POSITION | 247:8 | U / BE | raw x 1 + 0 |  | 15 ~ 15 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L608) |
| NEW_SIGNAL_7 | 248:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L609) |
| SET_ME_1C | 31:8 | U / BE | raw x 1 + 0 |  | 28 ~ 28 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L527) |
| NEW_SIGNAL_1 | 32:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L528) |
| NEW_SIGNAL_2 | 47:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L529) |
| NEW_SIGNAL_3 | 55:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L530) |
| NEW_SIGNAL_4 | 64:6 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L531) |
| NEW_SIGNAL_5 | 72:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L532) |
| NEW_SIGNAL_6 | 75:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L533) |
| NEW_SIGNAL_7 | 80:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L534) |
| NEW_SIGNAL_8 | 88:7 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L535) |
| NEW_SIGNAL_9 | 96:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L536) |
| NEW_SIGNAL_10 | 143:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L538) |
| NEW_SIGNAL_11 | 144:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L539) |
| NEW_SIGNAL_12 | 152:6 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L540) |
| NEW_SIGNAL_13 | 160:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L541) |
| NEW_SIGNAL_14 | 163:5 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L542) |
| NEW_SIGNAL_16 | 168:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L543) |
| NEW_SIGNAL_15 | 175:4 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L544) |
| NEW_SIGNAL_17 | 176:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L545) |
| NEW_SIGNAL_18 | 184:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L546) |
| NEW_SIGNAL_19 | 208:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L547) |
| SET_ME_TMP_F | 232:5 | U / LE | raw x 1 + 0 |  | 15 ~ 15 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L549) |
| SET_ME_TMP_F_2 | 240:5 | U / LE | raw x 1 + 0 |  | 15 ~ 15 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L550) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `HDA_MODE2` source enum: 1=Lane change icon gray; 2=lane green + lane change icon green; 3=lane green + lane change icon green (both blinking); 4=lane white + lane change icon white (both blinking rapidly); 5=Lane Change Assist system check warning
- `LANE_LEFT` source enum: 0=IDLE; 1=ON_SOURCE; 2=ON_TARGET
- `LANE_RIGHT` source enum: 0=IDLE; 1=ON_SOURCE; 2=ON_TARGET
- `LANE_CHANGING` source enum: 0=IDLE; 1=LEFT_CHECK; 3=LEFT_CHANGING; 2=RIGHT_CHECK; 4=RIGHT_CHANGING
- `LF_DETECT` source enum: 0=none; 1=gray; 2=white; 3=white; 4=hide
- `RF_DETECT` source enum: 0=none; 1=gray; 2=white; 3=white; 4=hide
- `SET_ME_FF`와 같은 배치/변환의 alias: SET_ME_FF (기존)
- `LR_DETECT` source enum: 0=none; 1=gray; 2=white; 3=white; 4=hide
- `RR_DETECT` source enum: 0=none; 1=gray; 2=white; 3=white; 4=hide
- `AUTOLANECHANGE_MSG` source enum: 1=Check surrounding conditions; 2=Operating conditions not met; 3=Analyzing driving lane; 4=Sharp curve ahead; 5=Current lane is too narrow; 6=Not an operational section; 7=Hazard lights are on; 8=Vehicle speed is too low; 9=Please hold the steering wheel; 10=Not an operable lane; 11=Steering input detected; 12=Press the OK button to activate lane change assist
- `LANELINE_CURVATURE_DIRECTION` source enum: 0=LEFT; 1=RIGHT
- `LANELINE_CURVATURE_DIRECTION`와 같은 배치/변환의 alias: NEW_SIGNAL_20 (기존)

- 미정의 bit 구간: 42:1, 57:7, 71:1, 86:2, 95:1, 99:21, 128:11, 147:5, 158:2, 171:1, 178:6, 187:17, 213:19, 249:7
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1f0"></a>
## 0x1F0 의미 미확정

bus 0, 16 B, 13,137 records, 평균 20.0006 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:104
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1f5"></a>
## 0x1F5 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 52개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 81:6, 137:6, 153:8, 192:4, 240:4
- 전체 변화 bit 구간: 0:24, 81:6, 137:6, 153:8, 192:4, 240:4
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-1fa"></a>
## 0x1FA CLUSTER_SPEED_LIMIT / FR_CMR_02_100ms

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 카메라/제한속도 표시 후보 정의 충돌

변화 bit 46개. 정의/헤더의 구조 coverage 163/256 bit. 의미 source가 있는 영역 130 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| SPEED_LIMIT_1 | 39:7 | U / BE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L612) |
| SPEED_LIMIT_2 | 47:7 | U / BE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L613) |
| SECONDARY_LIMIT_1 | 79:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L614) |
| SECONDARY_LIMIT_2 | 103:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L615) |
| SPEED_LIMIT_3 | 119:8 | U / BE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L616) |
| ARROW_DOWN | 120:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L617) |
| ARROW_UP | 121:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L618) |
| NEW_SIGNAL_2 | 122:3 | U / LE | raw x 1 + 0 |  | 0 ~ 4 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L619) |
| SPEED_CHANGE_BLINKING | 129:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L620) |
| CHIME_1 | 133:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L621) |
| NEW_SIGNAL_4 | 143:8 | U / BE | raw x 1 + 0 |  | 27 ~ 27 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L622) |
| NEW_SIGNAL_3 | 146:3 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L623) |
| NEW_SIGNAL_1 | 147:7 | U / LE | raw x 1 + 0 |  | 0 ~ 104 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L624) |
| SCHOOL_ZONE | 155:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L625) |
| SPEED_LIMIT_4 | 215:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L626) |
| NEW_SIGNAL_5 | 223:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L627) |
| FR_CMR_Crc2Val | 0:16 | U / LE | raw x 1 + 0 |  | 12 ~ 65528 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L553) |
| FR_CMR_AlvCnt2Val | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L554) |
| ISLW_OptUsmSta | 24:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L555) |
| ISLW_SysSta | 26:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L556) |
| ISLW_NoPassingInfoDis | 28:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L557) |
| ISLW_OvrlpSignDis | 31:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L558) |
| ISLW_SpdCluMainDis | 33:8 | U / LE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L559) |
| ISLW_SpdNaviMainDis | 41:8 | U / LE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L560) |
| ISLW_SubCondinfoSta1 | 49:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L561) |
| ISLW_SubCondinfoSta2 | 53:4 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L562) |
| ISLW_SpdCluSubMainDis | 64:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L563) |
| ISLW_SpdCluDisSubCond1 | 72:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L564) |
| ISLW_SpdCluDisSubCond2 | 80:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L565) |
| ISLW_SpdNaviSubMainDis | 88:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L566) |
| ISLW_SpdNaviDisSubCond1 | 96:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L567) |
| ISLW_SpdNaviDisSubCond2 | 104:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L568) |
| ISLA_SpdwOffst | 112:8 | U / LE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L569) |
| ISLA_SwIgnoreReq | 120:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L570) |
| ISLA_SpdChgReq | 122:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L571) |
| ISLA_SpdWrn | 124:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L572) |
| ISLA_IcyWrn | 126:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L573) |
| ISLA_SymFlashMod | 128:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L574) |
| ISLA_Popup | 131:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L575) |
| ISLA_OptUsmSta | 136:3 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L576) |
| ISLA_OffstUsmSta | 139:3 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L577) |
| ISLA_AutoUsmSta | 142:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L578) |
| ISLA_Cntry | 144:4 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L579) |
| ISLA_AddtnlSign | 149:5 | U / LE | raw x 1 + 0 |  | 0 ~ 26 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L580) |
| ISLA_SchoolZone | 154:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L581) |

- `ISLW_OptUsmSta` source enum: 0=None ISLW Option (Default); 1=System Disabled by USM; 2=System Enable by USM; 3=Invalid
- `ISLW_SysSta` source enum: 0=Normal (Default); 1=System Fail; 2=ISLW Temporary Unavailable; 3=Reserved
- `ISLW_NoPassingInfoDis` source enum: 0=None Display (Default); 1=LHD No Passing Zone Display; 2=RHD No Passing Zone Display; 3=Reserved; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid
- `ISLW_OvrlpSignDis` source enum: 0=None (Default); 1=Overlap Sign; 2=Reserved; 3=Error indicator
- `ISLW_SpdCluMainDis` source enum: 0=No Recognition (Default); 253=Unlimited Speed; 254=Reserved; 255=Invalid
- `ISLW_SpdNaviMainDis` source enum: 0=No Recognition (Default); 253=Unlimited Speed; 254=Reserved; 255=Invalid
- `ISLW_SubCondinfoSta1` source enum: 0=None (Default); 1=Rain; 2=Snow; 3=Snow&Rain; 4=Trailer; 5=Reserved; 6=Reserved; 7=Reserved; 8=Reserved; 9=Reserved; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=Generic; 15=Invalid
- `ISLW_SubCondinfoSta2` source enum: 0=None (Default); 1=Rain; 2=Snow; 3=Snow&Rain; 4=Trailer; 5=Reserved; 6=Reserved; 7=Reserved; 8=Reserved; 9=Reserved; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=Generic; 15=Invalid
- `ISLW_SpdCluSubMainDis` source enum: 0=No Recognition (Default); 253=Unlimited Speed; 254=Reserved; 255=Invalid
- `ISLW_SpdCluDisSubCond1` source enum: 0=No Recognition (Default); 253=LHD Conditional No Passing ZONE; 254=RHD Conditional No Passing ZONE; 255=Invalid
- `ISLW_SpdCluDisSubCond2` source enum: 0=No Recognition (Default); 253=LHD Conditional No Passing ZONE; 254=RHD Conditional No Passing ZONE; 255=Invalid
- `ISLW_SpdNaviSubMainDis` source enum: 0=No Recognition (Default); 253=Unlimited Speed; 254=Reserved; 255=Invalid
- `ISLW_SpdNaviDisSubCond1` source enum: 0=No Recognition (Default); 253=LHD Conditional No Passing ZONE; 254=RHD Conditional No Passing ZONE; 255=Invalid
- `ISLW_SpdNaviDisSubCond2` source enum: 0=No Recognition (Default); 253=LHD Conditional No Passing ZONE; 254=RHD Conditional No Passing ZONE; 255=Invalid
- `ISLA_SpdwOffst` source enum: 0=No Recognition; 253=Unlimited Speed; 254=Reserved; 255=Invalid
- `ISLA_SwIgnoreReq` source enum: 0=Allow All Switch Inputs (default); 1=-(SET) Switch Input Ignore; 2=+(SET) Switch Input Ignore; 3=-(SET) & +(SET) Switch Inputs Ignore
- `ISLA_SpdChgReq` source enum: 0=Default; 1=Speed Change Request; 2=Reserved; 3=Reserved
- `ISLA_SpdWrn` source enum: 0=No Warning; 1=Warning; 2=Reserved; 3=Reserved
- `ISLA_IcyWrn` source enum: 0=No Warning; 1=Warning; 2=Reserved; 3=Reserved
- `ISLA_SymFlashMod` source enum: 0=No Flasing; 1=Flashing Sign; 2=Flashing - Arrow Symbol; 3=Flashing + Arrow Symbol; 4=Flashing Auto Symbol; 5=Reserved; 6=Reserved; 7=Reserved
- `ISLA_Popup` source enum: 0=No Popup; 1=MSLA Speed will Change; 2=MSLA Speed has Changed; 3=CC_SCC Speed will Change; 4=CC_SCC Speed has Changed; 5=Reserved; 6=Reserved; 7=Reserved
- `ISLA_OptUsmSta` source enum: 0=None ISLA Option (속도 제한 메뉴 삭제); 1=Off; 2=Warning; 3=Assist; 4=Reserved; 5=Reserved; 6=Reserved; 7=Invalid (GRAY)
- `ISLA_OffstUsmSta` source enum: 0=None Offset Function; 1=-10kph or -5mph; 2=-5kph or -3mph; 3=0kph or 0mph; 4=+5kph or +3mph; 5=+10kph or +5mph; 6=Reserved; 7=Invalid (GRAY)
- `ISLA_AutoUsmSta` source enum: 0=None Auto Function (Delete Menu); 1=Auto Off; 2=Auto On; 3=Invalid (GRAY)
- `ISLA_Cntry` source enum: 0=Europe/Russia/Australia; 1=Domestic; 2=China; 3=USA; 4=Canada; 5=Australia; 6=Reserved; 7=Reserved; 8=Reserved; 9=Reserved; 10=Reserved; 11=Reserved; 12=Reserved; 13=Reserved; 14=Reserved; 15=Initial Value (default)
- `ISLA_AddtnlSign` source enum: 0=No Recognition (default); 1=School Crossing; 16=Do Not Pass; 17=Reserved; 18=Reserved; 19=Reserved; 20=Reserved; 21=Reserved; 22=Reserved; 23=Reserved; 24=Exit; 25=Roundabout; 26=Right Curve; 27=Left Curve; 28=Winding Road; 29=Reserved; 30=Reserved; 31=Reserved; 2=Pedestrian Crossing; 3=Bicycle Crossing; 4=Reserved; 5=Reserved; 6=Reserved; 7=Reserved; 8=Stop; 9=Yield; 10=Stop Ahead; 11=Yield Ahead; 12=Road Construction Ahead; 13=Lane Reduction; 14=Reserved; 15=Reserved
- `ISLA_SchoolZone` source enum: 0=No School Zone; 1=School Zone; 2=Reserved; 3=Reserved

- 미정의 bit 구간: 57:7, 134:2, 156:52, 224:32
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 34:5, 42:5, 113:5, 124:1, 149:6
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-200"></a>
## 0x200 ADRV_0x200

bus 0, 8 B, 13,137 records, 평균 20.0006 Hz. ADAS 표시/경고 후보

변화 bit 24개. 정의/헤더의 구조 coverage 48/64 bit. 의미 source가 있는 영역 6 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 17 ~ 65422 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L634) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L635) |
| SET_ME_E1 | 24:8 | U / LE | raw x 1 + 0 |  | 225 ~ 225 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L636) |
| TauGapSet | 32:3 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 소스 의미 정의, 차량 적용 후보; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L637) |
| NEW_SIGNAL_2 | 35:2 | U / LE | raw x 1 + 0 |  | 3 ~ 3 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L638) |
| NEW_SIGNAL_1 | 39:3 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; 겹치는 대안 있음; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L639) |
| TauGapSet_ | 42:3 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L640) |
| NEW_SIGNAL_3 | 47:5 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L641) |
| SET_ME_3A | 32:8 | U / LE | raw x 1 + 0 |  | 59 ~ 59 | 원시값 자리만 정의; 겹치는 대안 있음; [기존 opendbc](https://github.com/commaai/opendbc/blob/b72c1fd55ae7e84763e40912bbe06b8f533cb66b/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc#L591) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `SET_ME_E1`와 같은 배치/변환의 alias: SET_ME_E1 (기존)

- 미정의 bit 구간: 48:16
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-20a"></a>
## 0x20A 의미 미확정

bus 0, 16 B, 6,569 records, 평균 10.0011 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:104
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 6,569 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-225"></a>
## 0x225 의미 미확정

bus 0, 16 B, 6,574 records, 평균 10.0087 Hz. 물리 신호 의미 미확정

변화 bit 45개. 정의/헤더의 구조 coverage 24/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:104
- 미정의 구간 중 변화한 bit: 24:7, 32:5, 72:9
- 전체 변화 bit 구간: 0:31, 32:5, 72:9
- CRC16 probe: 6,574 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-235"></a>
## 0x235 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 53개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 48:11, 76:3, 80:2, 104:9, 144:4
- 전체 변화 bit 구간: 0:24, 48:11, 76:3, 80:2, 104:9, 144:4
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: CORNER_RADAR_235_OBJECTS_235 (hyundai_canfd_corner_radar_235_generated.dbc)

<a id="id-250"></a>
## 0x250 의미 미확정

bus 0, 24 B, 6,569 records, 평균 10.0011 Hz. 물리 신호 의미 미확정

변화 bit 62개. 정의/헤더의 구조 coverage 24/192 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:168
- 미정의 구간 중 변화한 bit: 80:2, 83:2, 88:2, 91:2, 120:6, 128:4, 133:1, 160:2, 168:10, 184:7
- 전체 변화 bit 구간: 0:24, 80:2, 83:2, 88:2, 91:2, 120:6, 128:4, 133:1, 160:2, 168:10, 184:7
- CRC16 probe: 6,569 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-255"></a>
## 0x255 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 72개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 52:6, 72:1, 88:6, 96:3, 120:1, 128:6, 136:9, 192:10, 208:5, 226:1
- 전체 변화 bit 구간: 0:24, 52:6, 72:1, 88:6, 96:3, 120:1, 128:6, 136:9, 192:10, 208:5, 226:1
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-25a"></a>
## 0x25A 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 57개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 96:13, 112:1, 192:8, 201:1, 204:1, 208:1, 210:2, 216:4, 241:1, 243:1
- 전체 변화 bit 구간: 0:24, 96:13, 112:1, 192:8, 201:1, 204:1, 208:1, 210:2, 216:4, 241:1, 243:1
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2aa"></a>
## 0x2AA 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2b0"></a>
## 0x2B0 의미 미확정

bus 0, 32 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 25개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:24, 55:1
- 전체 변화 bit 구간: 0:24, 55:1
- CRC16 probe: 3,278 통과, 6 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-2b5"></a>
## 0x2B5 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 66개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 48:7, 64:5, 80:4, 98:12, 216:7, 224:7
- 전체 변화 bit 구간: 0:24, 48:7, 64:5, 80:4, 98:12, 216:7, 224:7
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.
- 연동 후보 LE 48:8 -> pedal_raw, holdout r=0.999266. 물리 의미 미확정.

<a id="id-2c0"></a>
## 0x2C0 의미 미확정

bus 0, 32 B, 3,285 records, 평균 5.0013 Hz. 물리 신호 의미 미확정

변화 bit 40개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 80:16
- 전체 변화 bit 구간: 0:24, 80:16
- CRC16 probe: 3,285 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2d5"></a>
## 0x2D5 의미 미확정

bus 0, 32 B, 3,285 records, 평균 5.0013 Hz. 물리 신호 의미 미확정

변화 bit 43개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 120:5, 128:2, 132:1, 168:10, 184:1
- 전체 변화 bit 구간: 0:24, 120:5, 128:2, 132:1, 168:10, 184:1
- CRC16 probe: 3,285 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2e0"></a>
## 0x2E0 MANUAL_SPEED_LIMIT_ASSIST

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 수동 제한속도 보조

변화 bit 31개. 정의/헤더의 구조 coverage 43/256 bit. 의미 source가 있는 영역 19 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 29 ~ 65499 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L791) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L792) |
| MSLA_STATUS | 26:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L793) |
| MSLA_ENABLED | 38:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L794) |
| MAX_SPEED | 55:8 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L795) |
| MAX_SPEED_COPY | 144:8 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L796) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `MSLA_STATUS` source enum: 0=disabled; 1=active; 2=paused
- `MSLA_STATUS`와 같은 배치/변환의 alias: MSLA_STATUS (기존)
- `MSLA_ENABLED`와 같은 배치/변환의 alias: MSLA_ENABLED (기존)
- `MAX_SPEED`와 같은 배치/변환의 alias: MAX_SPEED (기존)
- `MAX_SPEED_COPY`와 같은 배치/변환의 alias: MAX_SPEED_COPY (기존)

- 미정의 bit 구간: 24:2, 28:10, 39:9, 56:88, 152:104
- 미정의 구간 중 변화한 bit: 64:7
- 전체 변화 bit 구간: 0:24, 64:7
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2e5"></a>
## 0x2E5 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 29개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 72:4, 96:1
- 전체 변화 bit 구간: 0:24, 72:4, 96:1
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2fa"></a>
## 0x2FA 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 70개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 32:4, 40:4, 48:3, 56:3, 64:16, 88:1, 112:2, 120:2, 176:5, 240:6
- 전체 변화 bit 구간: 0:24, 32:4, 40:4, 48:3, 56:3, 64:16, 88:1, 112:2, 120:2, 176:5, 240:6
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-2ff"></a>
## 0x2FF 의미 미확정

bus 0, 8 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-30a"></a>
## 0x30A 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 39개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 160:8, 249:7
- 전체 변화 bit 구간: 0:24, 160:8, 249:7
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-315"></a>
## 0x315 의미 미확정

bus 0, 16 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 29개. 정의/헤더의 구조 coverage 24/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:104
- 미정의 구간 중 변화한 bit: 28:5
- 전체 변화 bit 구간: 0:24, 28:5
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-31a"></a>
## 0x31A 의미 미확정

bus 0, 32 B, 658 records, 평균 1.0018 Hz. 물리 신호 의미 미확정

변화 bit 52개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:24, 88:7, 96:7, 104:6, 112:7, 120:1
- 전체 변화 bit 구간: 0:24, 88:7, 96:7, 104:6, 112:7, 120:1
- CRC16 probe: 657 통과, 1 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-325"></a>
## 0x325 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 53개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 98:1, 129:6, 144:4, 160:4, 169:6, 201:8
- 전체 변화 bit 구간: 0:24, 98:1, 129:6, 144:4, 160:4, 169:6, 201:8
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-330"></a>
## 0x330 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 42개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 82:4, 224:7, 240:7
- 전체 변화 bit 구간: 0:24, 82:4, 224:7, 240:7
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-33a"></a>
## 0x33A 의미 미확정

bus 0, 32 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-345"></a>
## 0x345 ADRV_0x345

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. ADAS 헤더와 미확정 필드

변화 bit 24개. 정의/헤더의 구조 coverage 32/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 51 ~ 65452 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L799) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L800) |
| SET_ME_15 | 24:8 | U / LE | raw x 1 + 0 |  | 21 ~ 21 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L801) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)
- `SET_ME_15`와 같은 배치/변환의 alias: SET_ME_15 (기존)

- 미정의 bit 구간: 32:32
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 3,284 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-34a"></a>
## 0x34A 의미 미확정

bus 0, 32 B, 658 records, 평균 1.0018 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 0/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:256
- 미정의 구간 중 변화한 bit: 0:24
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 657 통과, 1 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-360"></a>
## 0x360 의미 미확정

bus 0, 32 B, 6,567 records, 평균 9.9980 Hz. 물리 신호 의미 미확정

변화 bit 30개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 224:1, 232:4, 248:1
- 전체 변화 bit 구간: 0:24, 224:1, 232:4, 248:1
- CRC16 probe: 6,567 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-36a"></a>
## 0x36A BLINDSPOTS_FRONT_CORNER_2

bus 0, 16 B, 13,137 records, 평균 20.0006 Hz. 전측방 감지 후보

변화 bit 26개. 정의/헤더의 구조 coverage 26/128 bit. 의미 source가 있는 영역 2 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM | 0:16 | U / LE | raw x 1 + 0 |  | 117 ~ 65472 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L843) |
| COUNTER | 16:8 | U / LE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L844) |
| RIGHT_BSD | 27:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L845) |
| LEFT_BSD | 28:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L846) |

- `CHECKSUM`와 같은 배치/변환의 alias: CHECKSUM (기존)
- `COUNTER`와 같은 배치/변환의 alias: COUNTER (기존)

- 미정의 bit 구간: 24:3, 29:99
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 27:2
- CRC16 probe: 13,137 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-36f"></a>
## 0x36F 의미 미확정

bus 0, 8 B, 6,561 records, 평균 9.9889 Hz. 물리 신호 의미 미확정

변화 bit 12개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:4
- 전체 변화 bit 구간: 0:8, 12:4
- CRC16 probe: 0 통과, 6,561 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-37f"></a>
## 0x37F 의미 미확정

bus 0, 8 B, 6,561 records, 평균 9.9889 Hz. 물리 신호 의미 미확정

변화 bit 12개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:4
- 전체 변화 bit 구간: 0:8, 12:4
- CRC16 probe: 0 통과, 6,561 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-380"></a>
## 0x380 의미 미확정

bus 0, 8 B, 3,285 records, 평균 5.0013 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,285 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 적용 제외: CANFD_NAVI_STATUS_380 24 B, 관측 8 B.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: DI_BOX13 (hyundai_2015_ccan.dbc); DI_BOX13 (hyundai_kia_generic.dbc)

<a id="id-382"></a>
## 0x382 의미 미확정

bus 0, 8 B, 3,290 records, 평균 5.0089 Hz. 물리 신호 의미 미확정

변화 bit 25개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:21, 24:2, 50:2
- 전체 변화 bit 구간: 0:21, 24:2, 50:2
- CRC16 probe: 3,288 통과, 2 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-383"></a>
## 0x383 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 8개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 1:2, 5:3, 9:2, 14:1
- 전체 변화 bit 구간: 1:2, 5:3, 9:2, 14:1
- CRC16 probe: 3,281 통과, 3 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: FATC11 (hyundai_2015_ccan.dbc); FATC11 (hyundai_kia_generic.dbc)

<a id="id-384"></a>
## 0x384 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 18개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:3, 4:8, 13:5, 60:2
- 전체 변화 bit 구간: 0:3, 4:8, 13:5, 60:2
- CRC16 probe: 3,283 통과, 5 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: EMS17 (hyundai_2015_ccan.dbc); EMS17 (hyundai_kia_generic.dbc)

<a id="id-386"></a>
## 0x386 의미 미확정

bus 0, 8 B, 3,285 records, 평균 5.0013 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,285 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: WHL_SPD11 (hyundai_2015_ccan.dbc); WHL_SPD11 (hyundai_kia_generic.dbc)

<a id="id-38a"></a>
## 0x38A 의미 미확정

bus 0, 16 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:104
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-39b"></a>
## 0x39B 의미 미확정

bus 0, 8 B, 3,247 records, 평균 4.9434 Hz. 물리 신호 의미 미확정

변화 bit 4개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 26:4
- 전체 변화 bit 구간: 26:4
- CRC16 probe: 0 통과, 3,247 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3a0"></a>
## 0x3A0 TPMS

bus 0, 16 B, 3,287 records, 평균 5.0043 Hz. TPMS 네 바퀴 압력 원시값과 상태/단위 코드

변화 bit 35개. 정의/헤더의 구조 coverage 61/128 bit. 의미 source가 있는 영역 37 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| PRESSURE_FL | 39:8 | U / BE | raw x 1 + 0 |  | 36 ~ 38 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L854) |
| PRESSURE_FR | 47:8 | U / BE | raw x 1 + 0 |  | 36 ~ 37 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L855) |
| PRESSURE_RL | 55:8 | U / BE | raw x 1 + 0 |  | 39 ~ 40 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L856) |
| PRESSURE_RR | 63:8 | U / BE | raw x 1 + 0 |  | 39 ~ 40 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L857) |
| STATUS_TPMS | 66:3 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L858) |
| UNIT | 68:2 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L859) |


- 미정의 bit 구간: 24:8, 69:59
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24, 32:2, 40:1, 48:4, 56:4
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-3a5"></a>
## 0x3A5 의미 미확정

bus 0, 8 B, 6,568 records, 평균 9.9995 Hz. 물리 신호 의미 미확정

변화 bit 24개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:40
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:24
- CRC16 probe: 6,568 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-3a6"></a>
## 0x3A6 의미 미확정

bus 0, 8 B, 3,285 records, 평균 5.0013 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,285 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3aa"></a>
## 0x3AA 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,284 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3b0"></a>
## 0x3B0 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 7개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:1, 2:2, 7:1, 11:1, 14:2
- 전체 변화 bit 구간: 0:1, 2:2, 7:1, 11:1, 14:2
- CRC16 probe: 3,280 통과, 4 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3b1"></a>
## 0x3B1 의미 미확정

bus 0, 8 B, 3,286 records, 평균 5.0028 Hz. 물리 신호 의미 미확정

변화 bit 9개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 4:5, 10:4
- 전체 변화 bit 구간: 4:5, 10:4
- CRC16 probe: 3,282 통과, 4 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3b2"></a>
## 0x3B2 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 6개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 6:2, 9:3, 15:1
- 전체 변화 bit 구간: 6:2, 9:3, 15:1
- CRC16 probe: 3,282 통과, 2 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3b5"></a>
## 0x3B5 의미 미확정

bus 0, 32 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 48개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 56:4, 64:4, 88:3, 96:3, 107:1, 116:4, 128:5
- 전체 변화 bit 구간: 0:24, 56:4, 64:4, 88:3, 96:3, 107:1, 116:4, 128:5
- CRC16 probe: 3,284 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-3c1"></a>
## 0x3C1 BLINKER_STALKS

bus 0, 8 B, 3,318 records, 평균 5.0515 Hz. 방향지시등 및 조명 레버

변화 bit 16개. 정의/헤더의 구조 coverage 18/64 bit. 의미 source가 있는 영역 6 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM_MAYBE | 7:8 | U / BE | raw x 1 + 0 |  | 6 ~ 230 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L862) |
| COUNTER_ALT | 15:4 | U / BE | raw x 1 + 0 |  | 0 ~ 12 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L863) |
| HIGHBEAM_FORWARD | 18:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L864) |
| LIGHT_KNOB_POSITION | 21:2 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L865) |
| HIGHBEAM_BACKWARD | 26:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L866) |
| LEFT_BLINKER | 30:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L867) |
| RIGHT_BLINKER | 32:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L868) |

- `CHECKSUM_MAYBE`와 같은 배치/변환의 alias: CHECKSUM_MAYBE (기존)
- `COUNTER_ALT`와 같은 배치/변환의 alias: COUNTER_ALT (기존)
- `HIGHBEAM_FORWARD`와 같은 배치/변환의 alias: HIGHBEAM_FORWARD (기존)
- `LIGHT_KNOB_POSITION`와 같은 배치/변환의 alias: LIGHT_KNOB_POSITION (기존)
- `HIGHBEAM_BACKWARD`와 같은 배치/변환의 alias: HIGHBEAM_BACKWARD (기존)
- `LEFT_BLINKER`와 같은 배치/변환의 alias: LEFT_BLINKER (기존)
- `RIGHT_BLINKER`와 같은 배치/변환의 alias: RIGHT_BLINKER (기존)

- 미정의 bit 구간: 8:4, 16:2, 19:1, 22:4, 27:3, 31:1, 33:31
- 미정의 구간 중 변화한 bit: 36:1, 38:1
- 전체 변화 bit 구간: 0:8, 12:4, 30:1, 32:1, 36:1, 38:1
- CRC16 probe: 0 통과, 3,318 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3c2"></a>
## 0x3C2 의미 미확정

bus 0, 8 B, 3,266 records, 평균 4.9724 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,266 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3ca"></a>
## 0x3CA 의미 미확정

bus 0, 16 B, 658 records, 평균 1.0018 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/128 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:112
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 658 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-3e0"></a>
## 0x3E0 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3e1"></a>
## 0x3E1 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3e2"></a>
## 0x3E2 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3e5"></a>
## 0x3E5 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 11개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:7, 12:4
- 전체 변화 bit 구간: 0:7, 12:4
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3e6"></a>
## 0x3E6 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3ea"></a>
## 0x3EA 의미 미확정

bus 0, 8 B, 3,282 records, 평균 4.9967 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,282 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-3f0"></a>
## 0x3F0 의미 미확정

bus 0, 32 B, 658 records, 평균 1.0018 Hz. 물리 신호 의미 미확정

변화 bit 52개. 정의/헤더의 구조 coverage 24/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 24:232
- 미정의 구간 중 변화한 bit: 24:8, 40:7, 48:1, 64:7, 72:5
- 전체 변화 bit 구간: 0:32, 40:7, 48:1, 64:7, 72:5
- CRC16 probe: 658 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 관측 헤더 `BYTE2_COUNTER_CANDIDATE` 16:8: Byte 2 modulo-256 step +1 exceeds 99%, with >=100 distinct values; boundaries excluded.

<a id="id-3f5"></a>
## 0x3F5 의미 미확정

bus 0, 32 B, 658 records, 평균 1.0018 Hz. 물리 신호 의미 미확정

변화 bit 38개. 정의/헤더의 구조 coverage 16/256 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:240
- 미정의 구간 중 변화한 bit: 16:11, 28:2, 80:2, 200:3, 224:1, 232:3
- 전체 변화 bit 구간: 0:27, 28:2, 80:2, 200:3, 224:1, 232:3
- CRC16 probe: 658 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-401"></a>
## 0x401 의미 미확정

bus 0, 8 B, 3,282 records, 평균 4.9967 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,282 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-405"></a>
## 0x405 의미 미확정

bus 0, 8 B, 3,282 records, 평균 4.9967 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,282 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-410"></a>
## 0x410 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 7개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 2:3, 12:2, 16:2
- 전체 변화 bit 구간: 2:3, 12:2, 16:2
- CRC16 probe: 0 통과, 3,284 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: CGW_USM1 (hyundai_2015_ccan.dbc); CGW_USM1 (hyundai_kia_generic.dbc)

<a id="id-411"></a>
## 0x411 DOORS_SEATBELTS

bus 0, 8 B, 3,282 records, 평균 4.9967 Hz. 도어 및 안전벨트

변화 bit 0개. 정의/헤더의 구조 coverage 18/64 bit. 의미 source가 있는 영역 6 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM_MAYBE | 7:8 | U / BE | raw x 1 + 0 |  | 55 ~ 55 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L881) |
| COUNTER_ALT | 15:4 | U / BE | raw x 1 + 0 |  | 9 ~ 9 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L882) |
| DRIVER_DOOR | 24:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L883) |
| PASSENGER_DOOR | 34:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L884) |
| PASSENGER_SEATBELT | 36:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L885) |
| DRIVER_SEATBELT | 42:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L886) |
| DRIVER_REAR_DOOR | 52:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L887) |
| PASSENGER_REAR_DOOR | 56:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L888) |

- `CHECKSUM_MAYBE`와 같은 배치/변환의 alias: CHECKSUM_MAYBE (기존)
- `COUNTER_ALT`와 같은 배치/변환의 alias: COUNTER_ALT (기존)
- `DRIVER_DOOR` source enum: 0=Closed; 1=Opened
- `DRIVER_DOOR`와 같은 배치/변환의 alias: DRIVER_DOOR (기존)
- `PASSENGER_DOOR` source enum: 0=Closed; 1=Opened
- `PASSENGER_DOOR`와 같은 배치/변환의 alias: PASSENGER_DOOR (기존)
- `PASSENGER_SEATBELT` source enum: 0=Unlatched; 1=Latched
- `PASSENGER_SEATBELT`와 같은 배치/변환의 alias: PASSENGER_SEATBELT (기존)
- `DRIVER_SEATBELT` source enum: 0=Unlatched; 1=Latched
- `DRIVER_SEATBELT`와 같은 배치/변환의 alias: DRIVER_SEATBELT (기존)
- `DRIVER_REAR_DOOR` source enum: 0=Closed; 1=Opened
- `DRIVER_REAR_DOOR`와 같은 배치/변환의 alias: DRIVER_REAR_DOOR (기존)
- `PASSENGER_REAR_DOOR` source enum: 0=Closed; 1=Opened
- `PASSENGER_REAR_DOOR`와 같은 배치/변환의 alias: PASSENGER_REAR_DOOR (기존)

- 미정의 bit 구간: 8:4, 16:8, 25:9, 35:1, 37:5, 43:9, 53:3, 57:7
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,282 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-412"></a>
## 0x412 의미 미확정

bus 0, 8 B, 3,385 records, 평균 5.1535 Hz. 물리 신호 의미 미확정

변화 bit 13개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:4, 50:1
- 전체 변화 bit 구간: 0:8, 12:4, 50:1
- CRC16 probe: 0 통과, 3,385 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: ICM_412h (hyundai_kia_generic.dbc)

<a id="id-413"></a>
## 0x413 BLINKERS

bus 0, 8 B, 3,865 records, 평균 5.8843 Hz. 방향지시등 레버/램프

변화 bit 16개. 정의/헤더의 구조 coverage 11/64 bit. 의미 source가 있는 영역 7 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| LEFT_STALK | 8:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L891) |
| RIGHT_STALK | 10:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L892) |
| COUNTER_ALT | 15:4 | U / BE | raw x 1 + 0 |  | 0 ~ 14 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L893) |
| LEFT_LAMP | 20:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L894) |
| RIGHT_LAMP | 22:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L895) |
| LEFT_LAMP_ALT | 59:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L896) |
| RIGHT_LAMP_ALT | 61:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L897) |
| USE_ALT_LAMP | 62:1 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L898) |

- `LEFT_STALK`와 같은 배치/변환의 alias: LEFT_STALK (기존)
- `RIGHT_STALK`와 같은 배치/변환의 alias: RIGHT_STALK (기존)
- `COUNTER_ALT`와 같은 배치/변환의 alias: COUNTER_ALT (기존)
- `LEFT_LAMP`와 같은 배치/변환의 alias: LEFT_LAMP (기존)
- `RIGHT_LAMP`와 같은 배치/변환의 alias: RIGHT_LAMP (기존)
- `LEFT_LAMP_ALT`와 같은 배치/변환의 alias: LEFT_LAMP_ALT (기존)
- `RIGHT_LAMP_ALT`와 같은 배치/변환의 alias: RIGHT_LAMP_ALT (기존)
- `USE_ALT_LAMP`와 같은 배치/변환의 alias: USE_ALT_LAMP (기존)

- 미정의 bit 구간: 0:8, 9:1, 11:1, 16:4, 21:1, 23:36, 60:1, 63:1
- 미정의 구간 중 변화한 bit: 0:8
- 전체 변화 bit 구간: 0:9, 10:1, 12:4, 20:1, 22:1
- CRC16 probe: 0 통과, 3,865 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-414"></a>
## 0x414 의미 미확정

bus 0, 8 B, 3,281 records, 평균 4.9952 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,281 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-416"></a>
## 0x416 의미 미확정

bus 0, 8 B, 3,281 records, 평균 4.9952 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,281 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: CRUISE_BUTTON_LFA (hyundai_kia_generic.dbc)

<a id="id-417"></a>
## 0x417 의미 미확정

bus 0, 8 B, 3,281 records, 평균 4.9952 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,281 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-418"></a>
## 0x418 의미 미확정

bus 0, 8 B, 3,384 records, 평균 5.1520 Hz. 물리 신호 의미 미확정

변화 bit 13개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:4, 46:1
- 전체 변화 bit 구간: 0:8, 12:4, 46:1
- CRC16 probe: 0 통과, 3,384 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-419"></a>
## 0x419 의미 미확정

bus 0, 8 B, 10,840 records, 평균 16.5035 Hz. 물리 신호 의미 미확정

변화 bit 32개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:11, 34:10, 48:3
- 전체 변화 bit 구간: 0:8, 12:11, 34:10, 48:3
- CRC16 probe: 0 통과, 10,840 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-41a"></a>
## 0x41A 의미 미확정

bus 0, 8 B, 3,293 records, 평균 5.0135 Hz. 물리 신호 의미 미확정

변화 bit 11개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:2, 46:1
- 전체 변화 bit 구간: 0:8, 12:2, 46:1
- CRC16 probe: 0 통과, 3,293 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-41b"></a>
## 0x41B 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-41c"></a>
## 0x41C LIGHT_SENSOR

bus 0, 8 B, 12,610 records, 평균 19.1982 Hz. 조도 센서 후보

변화 bit 23개. 정의/헤더의 구조 coverage 23/64 bit. 의미 source가 있는 영역 11 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| CHECKSUM_MAYBE | 7:8 | U / BE | raw x 1 + 0 |  | 0 ~ 255 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L901) |
| COUNTER_ALT | 15:4 | U / BE | raw x 1 + 0 |  | 0 ~ 14 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L902) |
| IS_DARK | 19:1 | U / BE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L903) |
| LIGHT_LEVEL | 24:10 | U / LE | raw x 1 + 0 |  | 256 ~ 652 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L904) |


- 미정의 bit 구간: 8:4, 16:3, 20:4, 34:30
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:8, 12:4, 19:1, 24:10
- CRC16 probe: 0 통과, 12,610 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-41f"></a>
## 0x41F 의미 미확정

bus 0, 8 B, 1,081 records, 평균 1.6458 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 1,081 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-422"></a>
## 0x422 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-425"></a>
## 0x425 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-427"></a>
## 0x427 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 10개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:2, 6:1, 8:7
- 전체 변화 bit 구간: 0:2, 6:1, 8:7
- CRC16 probe: 3,283 통과, 1 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-429"></a>
## 0x429 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,284 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: _4WD12 (hyundai_2015_ccan.dbc); _4WD12 (hyundai_kia_generic.dbc)

<a id="id-42a"></a>
## 0x42A 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 9개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 1:2, 6:3, 10:2, 13:2
- 전체 변화 bit 구간: 1:2, 6:3, 10:2, 13:2
- CRC16 probe: 3,281 통과, 3 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-432"></a>
## 0x432 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-435"></a>
## 0x435 의미 미확정

bus 0, 8 B, 5,361 records, 평균 8.1619 Hz. 물리 신호 의미 미확정

변화 bit 20개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:8, 12:4, 28:1, 56:7
- 전체 변화 bit 구간: 0:8, 12:4, 28:1, 56:7
- CRC16 probe: 0 통과, 5,361 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-442"></a>
## 0x442 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: NM_KBD (hyundai_2015_mcan.dbc)

<a id="id-444"></a>
## 0x444 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: NM_CLOCK (hyundai_2015_mcan.dbc)

<a id="id-445"></a>
## 0x445 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: NM_RRC (hyundai_2015_mcan.dbc)

<a id="id-448"></a>
## 0x448 의미 미확정

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,284 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: NM_AMP (hyundai_2015_mcan.dbc)

<a id="id-454"></a>
## 0x454 의미 미확정

bus 0, 8 B, 3,288 records, 평균 5.0059 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,288 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: NM_HUD (hyundai_2015_mcan.dbc)

<a id="id-472"></a>
## 0x472 의미 미확정

bus 0, 8 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-473"></a>
## 0x473 의미 미확정

bus 0, 8 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-474"></a>
## 0x474 의미 미확정

bus 0, 8 B, 3,287 records, 평균 5.0043 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 16/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 16:48
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 3,287 통과, 0 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 관측 헤더 `CRC16_HEADER` 0:16: CRC matches every checked record. Algorithm is the pinned Hyundai CAN-FD CRC16.

<a id="id-47f"></a>
## 0x47F HVAC_TOUCH_BUTTONS

bus 0, 8 B, 3,284 records, 평균 4.9998 Hz. 공조 터치 버튼

변화 bit 0개. 정의/헤더의 구조 coverage 11/64 bit. 의미 source가 있는 영역 11 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| AUTO_BUTTON | 8:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L911) |
| SYNC_BUTTON | 12:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L912) |
| FR_DEFROST_BUTTON | 20:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L913) |
| RR_DEFROST_BUTTON | 22:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L914) |
| FAN_SPEED_UP_BUTTON | 24:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L915) |
| FAN_SPEED_DOWN_BUTTON | 26:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L916) |
| AIR_DIRECTION_BUTTON | 28:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L917) |
| AC_BUTTON | 40:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L918) |
| DRIVER_ONLY_BUTTON | 44:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L919) |
| RECIRC_BUTTON | 48:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L920) |
| HEAT_BUTTON | 52:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L921) |

- `AUTO_BUTTON`와 같은 배치/변환의 alias: AUTO_BUTTON (기존)
- `SYNC_BUTTON`와 같은 배치/변환의 alias: SYNC_BUTTON (기존)
- `FR_DEFROST_BUTTON`와 같은 배치/변환의 alias: FR_DEFROST_BUTTON (기존)
- `RR_DEFROST_BUTTON`와 같은 배치/변환의 alias: RR_DEFROST_BUTTON (기존)
- `FAN_SPEED_UP_BUTTON`와 같은 배치/변환의 alias: FAN_SPEED_UP_BUTTON (기존)
- `FAN_SPEED_DOWN_BUTTON`와 같은 배치/변환의 alias: FAN_SPEED_DOWN_BUTTON (기존)
- `AIR_DIRECTION_BUTTON`와 같은 배치/변환의 alias: AIR_DIRECTION_BUTTON (기존)
- `AC_BUTTON`와 같은 배치/변환의 alias: AC_BUTTON (기존)
- `DRIVER_ONLY_BUTTON`와 같은 배치/변환의 alias: DRIVER_ONLY_BUTTON (기존)
- `RECIRC_BUTTON`와 같은 배치/변환의 alias: RECIRC_BUTTON (기존)
- `HEAT_BUTTON`와 같은 배치/변환의 alias: HEAT_BUTTON (기존)

- 미정의 bit 구간: 0:8, 9:3, 13:7, 21:1, 23:1, 25:1, 27:1, 29:11, 41:3, 45:3, 49:3, 53:11
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,284 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-48f"></a>
## 0x48F 의미 미확정

bus 0, 8 B, 657 records, 평균 1.0003 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 657 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: TP_HU_Ipod_CLU (hyundai_2015_mcan.dbc)

<a id="id-4a3"></a>
## 0x4A3 HDA_INFO_4A3

bus 0, 8 B, 3,253 records, 평균 4.9526 Hz. HDA 도로 분류, 제한속도, 지도 정보

변화 bit 6개. 정의/헤더의 구조 coverage 57/64 bit. 의미 source가 있는 영역 33 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| LinkClass | 0:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L924) |
| Frwinfo | 3:3 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L925) |
| SpeedUnit | 6:2 | U / LE | raw x 1 + 0 |  | 1 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L926) |
| SPEED_LIMIT | 15:8 | U / BE | raw x 1 + 0 |  | 0 ~ 50 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L927) |
| CountryCode | 16:10 | U / LE | raw x 1 + 0 |  | 410 ~ 410 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L928) |
| MapSource | 27:3 | U / LE | raw x 1 + 0 |  | 1 ~ 2 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L929) |
| TollExist | 30:2 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L930) |
| TunnelExist | 38:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L931) |
| NEW_SIGNAL_6 | 47:24 | U / BE | raw x 1 + 0 |  | 0 ~ 0 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L932) |


- 미정의 bit 구간: 26:1, 32:6
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 9:1, 12:2, 27:2, 38:1
- CRC16 probe: 0 통과, 3,253 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4b4"></a>
## 0x4B4 NEW_MSG_4B4

bus 0, 8 B, 6,419 records, 평균 9.7727 Hz. 내비 위치 오프셋/카운터

변화 bit 29개. 정의/헤더의 구조 coverage 24/64 bit. 의미 source가 있는 영역 22 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| POS_OFFSET | 0:13 | U / LE | raw x 1 + 0 | m | 0 ~ 17 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L935) |
| POS_CYCLIC_COUNTER | 19:2 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L936) |
| POS_RANGE_AVG_SPEED | 21:9 | U / LE | raw x 1 + 0 | km/h | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L937) |


- 미정의 bit 구간: 13:6, 30:34
- 미정의 구간 중 변화한 bit: 18:1, 30:2, 36:9, 46:2, 50:2, 54:2, 57:1, 59:4
- 전체 변화 bit 구간: 0:5, 18:2, 30:2, 36:9, 46:2, 50:2, 54:2, 57:1, 59:4
- CRC16 probe: 0 통과, 6,419 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: TP_HU_DMB_CLU (hyundai_2015_mcan.dbc)

<a id="id-4b8"></a>
## 0x4B8 의미 미확정

bus 0, 8 B, 119 records, 평균 0.1812 Hz. 물리 신호 의미 미확정

변화 bit 54개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:2, 3:1, 5:1, 8:2, 12:18, 31:1, 33:3, 37:3, 41:23
- 전체 변화 bit 구간: 0:2, 3:1, 5:1, 8:2, 12:18, 31:1, 33:3, 37:3, 41:23
- CRC16 probe: 0 통과, 119 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4b9"></a>
## 0x4B9 NEW_MSG_4B9

bus 0, 8 B, 120 records, 평균 0.1827 Hz. 내비 세그먼트 경로/도로 분류

변화 bit 63개. 정의/헤더의 구조 coverage 64/64 bit. 의미 source가 있는 영역 24 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| BYTE_1 | 7:8 | U / BE | raw x 1 + 0 |  | 0 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L940) |
| BYTE_2 | 15:8 | U / BE | raw x 1 + 0 |  | 0 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L941) |
| BYTE_3 | 23:8 | U / BE | raw x 1 + 0 |  | 1 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L942) |
| BYTE_4 | 31:8 | U / BE | raw x 1 + 0 |  | 6 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L943) |
| BYTE_5 | 39:8 | U / BE | raw x 1 + 0 |  | 17 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L944) |
| BYTE_6 | 47:8 | U / BE | raw x 1 + 0 |  | 16 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L945) |
| BYTE_7 | 55:8 | U / BE | raw x 1 + 0 |  | 0 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L946) |
| BYTE_8 | 63:8 | U / BE | raw x 1 + 0 |  | 0 ~ 255 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L947) |
| SEGMENT_OFFSET | 0:13 | U / LE | raw x 1 + 0 | m | 0 ~ 8191 | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |
| SEGMENT_PATH_INDEX | 13:6 | U / LE | raw x 1 + 0 |  | 0 ~ 63 | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |
| CALCULATED_ROUTE | 22:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |
| FUNCTIONAL_ROAD_CLASS | 24:3 | U / LE | raw x 1 + 0 |  | 3 ~ 7 | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |


- 미정의 bit 구간: 없음
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 0:44, 45:19
- CRC16 probe: 0 통과, 120 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4ba"></a>
## 0x4BA NEW_MSG_4BA

bus 0, 8 B, 873 records, 평균 1.3291 Hz. 내비 짧은 프로파일

변화 bit 61개. 정의/헤더의 구조 coverage 58/64 bit. 의미 source가 있는 영역 56 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| PROSHORT_OFFSET | 0:13 | U / LE | raw x 1 + 0 | m | 71 ~ 8191 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L950) |
| PROSHORT_PATH_INDEX | 13:6 | U / LE | raw x 1 + 0 |  | 8 ~ 63 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L951) |
| PROSHORT_CYCLIC_COUNTER | 19:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L952) |
| PROSHORT_ACCURACY | 21:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L953) |
| PROSHORT_DISTANCE | 23:10 | U / LE | raw x 1 + 0 | m | 0 ~ 1023 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L954) |
| PROSHORT_VALUE_0 | 33:10 | U / LE | raw x 1 + 0 |  | 3 ~ 1023 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L955) |
| PROSHORT_VALUE_1 | 43:10 | U / LE | raw x 1 + 0 |  | 0 ~ 1023 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L956) |
| PROSHORT_PROFILE_TYPE | 53:5 | U / LE | raw x 1 + 0 |  | 1 ~ 31 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L957) |


- 미정의 bit 구간: 58:6
- 미정의 구간 중 변화한 bit: 58:3, 63:1
- 전체 변화 bit 구간: 0:16, 17:44, 63:1
- CRC16 probe: 0 통과, 873 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4be"></a>
## 0x4BE NEW_MSG_4BE

bus 0, 8 B, 66 records, 평균 0.1005 Hz. 내비 긴 프로파일 및 조건부 이벤트

변화 bit 62개. 정의/헤더의 구조 coverage 59/64 bit. 의미 source가 있는 영역 57 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| PROLONG_VALUE | 0:32 | U / LE | raw x 1 + 0 |  | 6 ~ 4.294967e+09 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L960) |
| PROLONG_OFFSET | 32:13 | U / LE | raw x 1 + 0 | m | 0 ~ 8191 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L961) |
| PROLONG_CYCLIC_COUNTER | 45:2 | U / LE | raw x 1 + 0 |  | 0 ~ 3 | 프로토콜 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L962) |
| PROLONG_UPDATE | 47:1 | U / LE | raw x 1 + 0 |  | 0 ~ 1 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L963) |
| PROLONG_PATH_INDEX | 48:6 | U / LE | raw x 1 + 0 |  | 8 ~ 63 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L964) |
| PROLONG_PROFILE_TYPE | 54:5 | U / LE | raw x 1 + 0 |  | 16 ~ 31 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L965) |
| EVENT_KIND | 0:4 | U / LE | raw x 1 + 0 |  | 0 ~ 7 (조건 충족 32건) | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |
| SPEED_CODE | 4:28 | U / LE | raw x 1 + 0 |  | 0 ~ 11 (조건 충족 32건) | 코드에서 분해한 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/car/hyundai/carstate.py#L814) |

- `EVENT_KIND` 조건: Only profile_type==16, 0<PROLONG_VALUE<=0x1ff; event-specific checks in source apply. Other profile types remain uninterpreted.
- `SPEED_CODE` 조건: Only profile_type==16, 0<PROLONG_VALUE<=0x1ff; event-specific checks in source apply. Other profile types remain uninterpreted.

- 미정의 bit 구간: 59:5
- 미정의 구간 중 변화한 bit: 59:5
- 전체 변화 bit 구간: 0:51, 52:6, 59:5
- CRC16 probe: 0 통과, 66 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4bf"></a>
## 0x4BF 의미 미확정

bus 0, 8 B, 870 records, 평균 1.3245 Hz. 물리 신호 의미 미확정

변화 bit 58개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 0:16, 17:7, 25:2, 28:1, 30:1, 32:7, 40:24
- 전체 변화 bit 구간: 0:16, 17:7, 25:2, 28:1, 30:1, 32:7, 40:24
- CRC16 probe: 0 통과, 870 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4d8"></a>
## 0x4D8 CLUSTER_INFO

bus 0, 8 B, 3,246 records, 평균 4.9419 Hz. 표시 거리 단위

변화 bit 0개. 정의/헤더의 구조 coverage 1/64 bit. 의미 source가 있는 영역 1 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| DISTANCE_UNIT | 0:1 | U / LE | raw x 1 + 0 |  | 0 ~ 0 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L968) |

- `DISTANCE_UNIT` source enum: 1=Miles; 0=Kilometers
- `DISTANCE_UNIT`와 같은 배치/변환의 alias: DISTANCE_UNIT (기존)

- 미정의 bit 구간: 1:63
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,246 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4dd"></a>
## 0x4DD 의미 미확정

bus 0, 8 B, 3,246 records, 평균 4.9419 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 3,246 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4df"></a>
## 0x4DF 의미 미확정

bus 0, 8 B, 3,251 records, 평균 4.9495 Hz. 물리 신호 의미 미확정

변화 bit 3개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 18:2, 26:1
- 전체 변화 bit 구간: 18:2, 26:1
- CRC16 probe: 0 통과, 3,251 불일치. 불일치는 다른 checksum 형식일 수 있다.

<a id="id-4eb"></a>
## 0x4EB LOCAL_TIME2

bus 0, 8 B, 3,246 records, 평균 4.9419 Hz. 차량 시각

변화 bit 11개. 정의/헤더의 구조 coverage 18/64 bit. 의미 source가 있는 영역 17 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| HOURS | 15:5 | U / BE | raw x 1 + 0 |  | 14 ~ 14 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L971) |
| MINUTES | 21:6 | U / BE | raw x 1 + 0 |  | 32 ~ 48 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L972) |
| SECONDS | 24:6 | U / LE | raw x 1 + 0 |  | 0 ~ 59 | 소스 의미 정의, 차량 적용 후보; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L973) |
| NEW_SIGNAL_3 | 39:1 | U / BE | raw x 1 + 0 |  | 1 ~ 1 | 원시값 자리만 정의; [ajouatom](https://github.com/ajouatom/openpilot/blob/ab696d049e3d6d0fd3bde51c233ee81f736415be/opendbc_repo/opendbc/dbc/hyundai_canfd_generated.dbc#L974) |

- `HOURS`와 같은 배치/변환의 alias: HOURS (기존)
- `MINUTES`와 같은 배치/변환의 alias: MINUTES (기존)
- `SECONDS`와 같은 배치/변환의 alias: SECONDS (기존)
- `NEW_SIGNAL_3`와 같은 배치/변환의 alias: NEW_SIGNAL_3 (기존)

- 미정의 bit 구간: 0:11, 22:2, 30:9, 40:24
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 16:5, 24:6
- CRC16 probe: 0 통과, 3,246 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: TP_CLU_MP_HU (hyundai_2015_mcan.dbc)

<a id="id-4f2"></a>
## 0x4F2 의미 미확정

bus 0, 8 B, 4,702 records, 평균 7.1586 Hz. 물리 신호 의미 미확정

변화 bit 12개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 44:2, 54:10
- 전체 변화 bit 구간: 44:2, 54:10
- CRC16 probe: 0 통과, 4,702 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: TP_HU_CARPLAY_CLU (hyundai_2015_mcan.dbc)

<a id="id-4f3"></a>
## 0x4F3 의미 미확정

bus 0, 8 B, 650 records, 평균 0.9896 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 650 불일치. 불일치는 다른 checksum 형식일 수 있다.
- 다른 bus/platform의 ID/길이 후보, 적용 안 함: TP_CLU_CARPLAY_HU (hyundai_2015_mcan.dbc)

<a id="id-4fe"></a>
## 0x4FE 의미 미확정

bus 0, 8 B, 325 records, 평균 0.4948 Hz. 물리 신호 의미 미확정

변화 bit 0개. 정의/헤더의 구조 coverage 0/64 bit. 의미 source가 있는 영역 0 bit 역시 차량 적용 후보이며 확정률이 아니다.

| 필드 | start:bits | 부호/순서 | raw 변환 | 단위 | 관측 범위 | 해석/source |
| --- | --- | --- | --- | --- | --- | --- |
| 의미 필드 없음 | | | | | | 아래 헤더와 미정의 구간 참조 |


- 미정의 bit 구간: 0:64
- 미정의 구간 중 변화한 bit: 없음
- 전체 변화 bit 구간: 없음
- CRC16 probe: 0 통과, 325 불일치. 불일치는 다른 checksum 형식일 수 있다.
