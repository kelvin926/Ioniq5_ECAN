# 2026-10-07 ECAN 138개 ID 개별 추론

이 문서는 당시 분석 단계의 기록이다. [현재 분석 시작 문서](ecan_analysis_20261007.md)와 [최신 한국어 필드 표](ecan_korean_bit_fields_20261007.md)를 먼저 참고한다.

[분석 방법과 해석 한계](ecan_inference_20261007.md). 각 ID에는 값 파일이 있으며 morphology_only는 특정 기능을 알아냈다는 뜻이 아니다. 기존 DBC 전체 필드는 [기존 상세 사전](ecan_fields_20261007.md)에 있다.

| ID | B / 기록 수 | 현재 가장 구체적인 가설 | 근거 수준 |
| --- | --- | --- | --- |
| [0x035](#id-035) | 32 /65,685 | 페달과 앞/뒤 구동 부하 복제 채널 | specific_candidate |
| [0x04A](#id-04a) | 32 /65,686 | IMU_01_10ms | specific_candidate |
| [0x060](#id-060) | 32 /65,685 | ESP_STATUS | specific_candidate |
| [0x065](#id-065) | 32 /65,685 | 브레이크와 압축 모터 회전수 후보 | specific_candidate |
| [0x06F](#id-06f) | 8 /65,685 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x08B](#id-08b) | 8 /3,284 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x090](#id-090) | 32 /6,572 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x0A0](#id-0a0) | 24 /65,685 | WHEEL_SPEEDS | specific_candidate |
| [0x0DA](#id-0da) | 24 /65,686 | 차속과 앞/뒤 구동 부하 후보 | specific_candidate |
| [0x0EA](#id-0ea) | 24 /65,683 | MDPS | specific_candidate |
| [0x0F5](#id-0f5) | 32 /65,686 | 앞/뒤 부하의 두 중복 packed word와 제동 채널 후보 | specific_candidate |
| [0x0FF](#id-0ff) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x10A](#id-10a) | 32 /65,685 | 뒤 모터 회전수, 부하, DC 전압 후보 | specific_candidate |
| [0x11A](#id-11a) | 16 /65,676 | FR_CMR_01_10ms | specific_candidate |
| [0x120](#id-120) | 32 /65,687 | 앞 모터 회전수, 부하, DC 전압 후보 | specific_candidate |
| [0x125](#id-125) | 16 /65,343 | STEERING_SENSORS | specific_candidate |
| [0x12A](#id-12a) | 16 /65,683 | LFA | specific_candidate |
| [0x130](#id-130) | 16 /65,685 | GEAR_SHIFTER | specific_candidate |
| [0x145](#id-145) | 32 /32,844 | 빠른 센서, 제어 피드백 또는 주행 텔레메트리 후보 | morphology_only |
| [0x15A](#id-15a) | 8 /65,687 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x160](#id-160) | 16 /32,841 | ADRV_0x160 | specific_candidate |
| [0x175](#id-175) | 24 /32,843 | TCS | specific_candidate |
| [0x180](#id-180) | 8 /32,873 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x185](#id-185) | 8 /32,873 | CAM_0x185 | specific_candidate |
| [0x19A](#id-19a) | 32 /65,687 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x1A0](#id-1a0) | 32 /32,841 | SCC_CONTROL | specific_candidate |
| [0x1AA](#id-1aa) | 16 /32,873 | 버튼과 속도 복제 후보 | specific_candidate |
| [0x1B0](#id-1b0) | 32 /32,844 | 빠른 센서, 제어 피드백 또는 주행 텔레메트리 후보 | morphology_only |
| [0x1B5](#id-1b5) | 32 /13,136 | CCNC_0x1B5 / FR_CMR_03_50ms | specific_candidate |
| [0x1BA](#id-1ba) | 24 /13,137 | BLINDSPOTS_REAR_CORNERS / ADAS_CMD_50_50ms | specific_candidate |
| [0x1CF](#id-1cf) | 8 /32,843 | CRUISE_BUTTONS | specific_candidate |
| [0x1DA](#id-1da) | 32 /658 | ADRV_0x1da | specific_candidate |
| [0x1E0](#id-1e0) | 16 /13,137 | LFAHDA_CLUSTER | specific_candidate |
| [0x1E5](#id-1e5) | 16 /13,136 | BLINDSPOTS_FRONT_CORNER_1 | specific_candidate |
| [0x1EA](#id-1ea) | 32 /13,137 | ADRV_0x1ea | specific_candidate |
| [0x1F0](#id-1f0) | 16 /13,137 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x1F5](#id-1f5) | 32 /6,568 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x1FA](#id-1fa) | 32 /6,568 | CLUSTER_SPEED_LIMIT / FR_CMR_02_100ms | specific_candidate |
| [0x200](#id-200) | 8 /13,137 | ADRV_0x200 | specific_candidate |
| [0x20A](#id-20a) | 16 /6,569 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x225](#id-225) | 16 /6,574 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x235](#id-235) | 32 /6,568 | 고전압/배터리 측 텔레메트리 후보 | specific_candidate |
| [0x250](#id-250) | 24 /6,569 | 전기 구동 전력/부하 후보 | specific_candidate |
| [0x255](#id-255) | 32 /6,568 | 두 번째 고전압 채널 후보 | specific_candidate |
| [0x25A](#id-25a) | 32 /6,568 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x2AA](#id-2aa) | 32 /6,567 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x2B0](#id-2b0) | 32 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x2B5](#id-2b5) | 32 /6,567 | 가속 페달 복제와 전장 상태 후보 | specific_candidate |
| [0x2C0](#id-2c0) | 32 /3,285 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x2D5](#id-2d5) | 32 /3,285 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x2E0](#id-2e0) | 32 /6,567 | MANUAL_SPEED_LIMIT_ASSIST | specific_candidate |
| [0x2E5](#id-2e5) | 32 /6,568 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x2FA](#id-2fa) | 32 /6,567 | 배터리 측 전류/전력성 부하 후보 | specific_candidate |
| [0x2FF](#id-2ff) | 8 /3,287 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x30A](#id-30a) | 32 /6,568 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x315](#id-315) | 16 /3,287 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x31A](#id-31a) | 32 /658 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x325](#id-325) | 32 /6,567 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x330](#id-330) | 32 /6,567 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x33A](#id-33a) | 32 /6,568 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x345](#id-345) | 8 /3,284 | ADRV_0x345 | specific_candidate |
| [0x34A](#id-34a) | 32 /658 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x360](#id-360) | 32 /6,567 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x36A](#id-36a) | 16 /13,137 | BLINDSPOTS_FRONT_CORNER_2 | specific_candidate |
| [0x36F](#id-36f) | 8 /6,561 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x37F](#id-37f) | 8 /6,561 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x380](#id-380) | 8 /3,285 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x382](#id-382) | 8 /3,290 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x383](#id-383) | 8 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x384](#id-384) | 8 /3,288 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x386](#id-386) | 8 /3,285 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x38A](#id-38a) | 16 /3,287 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x39B](#id-39b) | 8 /3,247 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3A0](#id-3a0) | 16 /3,287 | TPMS | specific_candidate |
| [0x3A5](#id-3a5) | 8 /6,568 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3A6](#id-3a6) | 8 /3,285 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3AA](#id-3aa) | 8 /3,284 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3B0](#id-3b0) | 8 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x3B1](#id-3b1) | 8 /3,286 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x3B2](#id-3b2) | 8 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x3B5](#id-3b5) | 32 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x3C1](#id-3c1) | 8 /3,318 | BLINKER_STALKS | specific_candidate |
| [0x3C2](#id-3c2) | 8 /3,266 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3CA](#id-3ca) | 16 /658 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3E0](#id-3e0) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3E1](#id-3e1) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3E2](#id-3e2) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3E5](#id-3e5) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3E6](#id-3e6) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3EA](#id-3ea) | 8 /3,282 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x3F0](#id-3f0) | 32 /658 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x3F5](#id-3f5) | 32 /658 | 배터리 측 통신에 참여하는 텔레메트리/상태 후보 | source_domain_only |
| [0x401](#id-401) | 8 /3,282 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x405](#id-405) | 8 /3,282 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x410](#id-410) | 8 /3,284 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x411](#id-411) | 8 /3,282 | DOORS_SEATBELTS | specific_candidate |
| [0x412](#id-412) | 8 /3,385 | 게이트웨이 상태 후보; 공개 B6 브레이크 정의는 관측과 불일치 | morphology_only |
| [0x413](#id-413) | 8 /3,865 | BLINKERS | specific_candidate |
| [0x414](#id-414) | 8 /3,281 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x416](#id-416) | 8 /3,281 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x417](#id-417) | 8 /3,281 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x418](#id-418) | 8 /3,384 | 조향 열선 상태 source 후보, 자극 없음 | specific_candidate |
| [0x419](#id-419) | 8 /10,840 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x41A](#id-41a) | 8 /3,293 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x41B](#id-41b) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x41C](#id-41c) | 8 /12,610 | LIGHT_SENSOR | specific_candidate |
| [0x41F](#id-41f) | 8 /1,081 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x422](#id-422) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x425](#id-425) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x427](#id-427) | 8 /3,284 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x429](#id-429) | 8 /3,284 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x42A](#id-42a) | 8 /3,284 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x432](#id-432) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x435](#id-435) | 8 /5,361 | 필터된 차속 후보 | specific_candidate |
| [0x442](#id-442) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x444](#id-444) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x445](#id-445) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x448](#id-448) | 8 /3,284 | 조향 버튼 source 후보, 자극 없음 | specific_candidate |
| [0x454](#id-454) | 8 /3,288 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x472](#id-472) | 8 /3,287 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x473](#id-473) | 8 /3,287 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x474](#id-474) | 8 /3,287 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x47F](#id-47f) | 8 /3,284 | HVAC_TOUCH_BUTTONS | specific_candidate |
| [0x48F](#id-48f) | 8 /657 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x4A3](#id-4a3) | 8 /3,253 | HDA_INFO_4A3 | specific_candidate |
| [0x4B4](#id-4b4) | 8 /6,419 | NEW_MSG_4B4 | specific_candidate |
| [0x4B8](#id-4b8) | 8 /119 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x4B9](#id-4b9) | 8 /120 | NEW_MSG_4B9 | specific_candidate |
| [0x4BA](#id-4ba) | 8 /873 | NEW_MSG_4BA | specific_candidate |
| [0x4BE](#id-4be) | 8 /66 | NEW_MSG_4BE | specific_candidate |
| [0x4BF](#id-4bf) | 8 /870 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x4D8](#id-4d8) | 8 /3,246 | CLUSTER_INFO | specific_candidate |
| [0x4DD](#id-4dd) | 8 /3,246 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x4DF](#id-4df) | 8 /3,251 | 이산 상태, 이벤트 또는 설정 코드 후보 | morphology_only |
| [0x4EB](#id-4eb) | 8 /3,246 | LOCAL_TIME2 | specific_candidate |
| [0x4F2](#id-4f2) | 8 /4,702 | 느린 수치 텔레메트리 또는 복합 상태 후보 | morphology_only |
| [0x4F3](#id-4f3) | 8 /650 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |
| [0x4FE](#id-4fe) | 8 /325 | 고정 설정, 비활성 기능 또는 상태 프레임 후보 | morphology_only |

<a id="id-035"></a>
## 0x035 페달과 앞/뒤 구동 부하 복제 채널

32 B, 65,685 records, 평균 100.0029 Hz. 38개 숫자 column과 raw byte 값이 `0x035.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FRONT_DRIVE_LOAD_COPY_RAW | 96:14 S LE | raw x1, raw_counts | -548 ~ 1126 | cross_frame_supported_candidate |
| REAR_DRIVE_LOAD_COPY_RAW | 112:14 S LE | raw x1, raw_counts | -1022 ~ 1182 | cross_frame_supported_candidate |

- `FRONT_DRIVE_LOAD_COPY_RAW`: 14-bit interpretation separates upper word flags; nearly identical to 0x0DA front-load channel. Exact sign-extension boundary remains a candidate.
- `REAR_DRIVE_LOAD_COPY_RAW`: 14-bit interpretation separates upper word flags; nearly identical to 0x0DA rear-load channel. Exact sign-extension boundary remains a candidate.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_load_counts | 113:11 little S | 0.999429 | 0.00661 | 0.00 | 0.6792 |
| front_load_counts | 97:10 little S | 0.996872 | 0.00648 | 0.00 | 0.8458 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [FRONT_MOTOR_RPM_MSG](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 0 fields: 길이 불일치, 적용 제외.
- [ACCELERATOR](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 4 fields: 길이 일치, 의미는 별도 검증.

<a id="id-04a"></a>
## 0x04A IMU_01_10ms

32 B, 65,686 records, 평균 100.0044 Hz. 40개 숫자 column과 raw byte 값이 `0x04A.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| gnss_yaw_deg_s | 65:15 little U | 0.995257 | 0.01923 | 0.30 | 0.4124 |
| steering_deg | 64:16 little U | -0.958030 | 0.04258 | 0.20 | -0.6233 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [methinks_lateral_and_longitudinal_force](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 5 fields: 길이 일치, 의미는 별도 검증.
- [HU_AMP_E_11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 4 fields: 길이 불일치, 적용 제외.
- [IMU_01_10ms](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 17 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-060"></a>
## 0x060 ESP_STATUS

32 B, 65,685 records, 평균 100.0029 Hz. 41개 숫자 column과 raw byte 값이 `0x060.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ESP_STATUS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 5 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-065"></a>
## 0x065 브레이크와 압축 모터 회전수 후보

32 B, 65,685 records, 평균 100.0029 Hz. 39개 숫자 column과 raw byte 값이 `0x065.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FRONT_MOTOR_SPEED_PROXY_RPM | 200:12 U LE | raw x8, rpm | 0 ~ 3856 | cross_frame_supported_candidate |
| REAR_MOTOR_SPEED_PROXY_RPM | 212:12 U LE | raw x8, rpm | 0 ~ 5096 | cross_frame_supported_candidate |

- `FRONT_MOTOR_SPEED_PROXY_RPM`: Compressed positive speed-like fields track respective source RPM channels at about raw=RPM/8; upper unused width/sign not fully excited.
- `REAR_MOTOR_SPEED_PROXY_RPM`: Compressed positive speed-like fields track respective source RPM channels at about raw=RPM/8; upper unused width/sign not fully excited.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_motor_rpm_source | 212:10 little U | 0.999983 | 0.00168 | 0.00 | 0.8557 |
| speed_kph | 212:10 little U | 0.999870 | 0.00658 | 0.00 | 0.7012 |
| front_motor_rpm_source | 200:10 little S | 0.999245 | 0.00553 | 0.00 | 0.9301 |
| gnss_speed_kph | 212:10 little U | 0.999111 | 0.01157 | 0.00 | 0.3725 |
| brake_pressed | 44:14 little U | 0.996194 | 0.03859 | 0.00 | 0.7648 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [Brake_Pedal](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 1 fields: 길이 불일치, 적용 제외.
- [BRAKE](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 4 fields: 길이 일치, 의미는 별도 검증.

<a id="id-06f"></a>
## 0x06F 이산 상태, 이벤트 또는 설정 코드 후보

8 B, 65,685 records, 평균 100.0029 Hz. 8개 숫자 column과 raw byte 값이 `0x06F.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-08b"></a>
## 0x08B 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x08B.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [AMP_HU_E_12](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 6 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-090"></a>
## 0x090 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 6,572 records, 평균 10.0056 Hz. 32개 숫자 column과 raw byte 값이 `0x090.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-0a0"></a>
## 0x0A0 WHEEL_SPEEDS

24 B, 65,685 records, 평균 100.0029 Hz. 38개 숫자 column과 raw byte 값이 `0x0A0.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_motor_rpm_source | 112:12 little U | 0.999924 | 0.00756 | 0.00 | 0.7823 |
| gnss_speed_kph | 112:11 little U | 0.999049 | 0.01175 | 0.00 | 0.3336 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_A0](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 24 B, 4 fields: 길이 일치, 의미는 별도 검증.
- [EngFrzFrm1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_i30_2014.dbc), 8 B, 6 fields: 길이 불일치, 적용 제외.
- [WHEEL_SPEEDS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 24 B, 10 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-0da"></a>
## 0x0DA 차속과 앞/뒤 구동 부하 후보

24 B, 65,686 records, 평균 100.0044 Hz. 27개 숫자 column과 raw byte 값이 `0x0DA.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| VEHICLE_SPEED_MPS_CANDIDATE | 64:16 U LE | raw x0.01, m/s | 0.01 ~ 18.04 | calibrated_candidate |
| FRONT_DRIVE_LOAD_RAW | 80:16 S LE | raw x1, raw_counts | -548 ~ 1124 | cross_frame_supported_candidate |
| REAR_DRIVE_LOAD_RAW | 96:16 S LE | raw x1, raw_counts | -1022 ~ 1180 | cross_frame_supported_candidate |

- `VEHICLE_SPEED_MPS_CANDIDATE`: Raw*0.036 kph agrees with wheel speed: overall RMSE 0.294 kph. Width beyond observed 11 active bits relies on source raw layout. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)
- `FRONT_DRIVE_LOAD_RAW`: Tracks front motor load and 0x035 bits96..109. Nm/current/command versus feedback remains unresolved.
- `REAR_DRIVE_LOAD_RAW`: Tracks rear motor load and 0x035 bits112..125; unequal front/rear gains indicate distinct channel calibrations. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_motor_rpm_source | 64:11 little U | 0.999970 | 0.00248 | 0.10 | 0.8811 |
| speed_kph | 64:11 little U | 0.999920 | 0.00521 | 0.00 | 0.7867 |
| gnss_speed_kph | 64:11 little U | 0.999118 | 0.01185 | 0.00 | 0.3949 |
| rear_load_counts | 97:11 little S | 0.999470 | 0.00640 | 0.00 | 0.6951 |
| front_load_counts | 80:11 little S | 0.997703 | 0.00565 | 0.00 | 0.9034 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_DA](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 24 B, 2 fields: 길이 일치, 의미는 별도 검증.

<a id="id-0ea"></a>
## 0x0EA MDPS

24 B, 65,683 records, 평균 99.9998 Hz. 71개 숫자 column과 raw byte 값이 `0x0EA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| steering_deg | 128:13 little S | -0.999900 | 0.00190 | 0.00 | -0.9981 |
| gnss_yaw_deg_s | 129:12 little S | 0.978961 | 0.04650 | 0.20 | 0.5789 |
| yaw_deg_s | 128:13 little S | 0.958269 | 0.03983 | -0.20 | 0.6206 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [STEERING](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 24 B, 2 fields: 길이 일치, 의미는 별도 검증.
- [MDPS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 24 B, 35 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-0f5"></a>
## 0x0F5 앞/뒤 부하의 두 중복 packed word와 제동 채널 후보

32 B, 65,686 records, 평균 100.0044 Hz. 37개 숫자 column과 raw byte 값이 `0x0F5.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FRONT_LOAD_1_PROXY_COUNTS | 40:12 S LE | raw x0.25, motor_load_proxy_counts | -135 ~ 277 | cross_frame_supported_candidate |
| REAR_LOAD_1_PROXY_COUNTS | 52:12 S LE | raw x0.25, motor_load_proxy_counts | -187 ~ 216 | cross_frame_supported_candidate |
| FRONT_LOAD_2_PROXY_COUNTS | 64:12 S LE | raw x0.25, motor_load_proxy_counts | -135 ~ 277 | cross_frame_supported_candidate |
| REAR_LOAD_2_PROXY_COUNTS | 76:12 S LE | raw x0.25, motor_load_proxy_counts | -187 ~ 216 | cross_frame_supported_candidate |
| BRAKING_PRESSURE_OR_DEMAND_RAW | 96:16 U LE | raw x1, raw_counts | 0 ~ 1500 | candidate |

- `FRONT_LOAD_1_PROXY_COUNTS`: Two identical 24-bit packed words each contain candidate front/rear signed 12-bit load fields, not one 24-bit physical scalar. Divide by 4 to compare with MCU load raw; not verified Nm.
- `REAR_LOAD_1_PROXY_COUNTS`: Two identical 24-bit packed words each contain candidate front/rear signed 12-bit load fields, not one 24-bit physical scalar. Divide by 4 to compare with MCU load raw; not verified Nm.
- `FRONT_LOAD_2_PROXY_COUNTS`: Two identical 24-bit packed words each contain candidate front/rear signed 12-bit load fields, not one 24-bit physical scalar. Divide by 4 to compare with MCU load raw; not verified Nm.
- `REAR_LOAD_2_PROXY_COUNTS`: Two identical 24-bit packed words each contain candidate front/rear signed 12-bit load fields, not one 24-bit physical scalar. Divide by 4 to compare with MCU load raw; not verified Nm.
- `BRAKING_PRESSURE_OR_DEMAND_RAW`: Correlates with known brake-pressure raw; physical unit and pressure versus requested braking remain unresolved.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_load_counts | 52:11 little S | 0.999432 | 0.00659 | 0.00 | 0.6814 |
| front_load_counts | 63:12 little S | 0.996878 | 0.00648 | 0.00 | 0.8459 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_F5](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 4 fields: 길이 일치, 의미는 별도 검증.

<a id="id-0ff"></a>
## 0x0FF 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x0FF.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-10a"></a>
## 0x10A 뒤 모터 회전수, 부하, DC 전압 후보

32 B, 65,685 records, 평균 100.0029 Hz. 35개 숫자 column과 raw byte 값이 `0x10A.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| REAR_MOTOR_SPEED_RPM_CANDIDATE | 24:16 S LE | raw x1, rpm | -33 ~ 5097 | source_and_data_supported_candidate |
| REAR_MOTOR_LOAD_RAW | 40:10 S LE | raw x1, raw_counts | -190 ~ 216 | cross_frame_supported_candidate |
| REAR_DC_LINK_VOLTAGE_V_CANDIDATE | 128:16 U LE | raw x1, V | 683 ~ 700 | source_and_data_supported_candidate |

- `REAR_MOTOR_SPEED_RPM_CANDIDATE`: Exact vehicle-source layout; rear channel tracks wheel/GNSS speed; front channel can remain zero while rear moves. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)
- `REAR_MOTOR_LOAD_RAW`: Signed 10-bit load-like channel; related copies in 0x035, 0x0DA and packed 0x0F5. Nm/A scaling not established.
- `REAR_DC_LINK_VOLTAGE_V_CANDIDATE`: Emulator source maps bytes 16..17 to live pack V. Observed 683..702, consistent with OEM nominal 697 V and other voltage-like channels. [source](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp)

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| speed_kph | 24:16 little S | 0.999854 | 0.00669 | 0.00 | 0.7079 |
| gnss_speed_kph | 25:13 little S | 0.999094 | 0.01165 | 0.00 | 0.3782 |
| pack_voltage_proxy | 132:32 big U | 0.994097 | 0.02232 | -0.20 | 0.1383 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [REAR_MOTOR](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 1 fields: 길이 일치, 의미는 별도 검증.

<a id="id-11a"></a>
## 0x11A FR_CMR_01_10ms

16 B, 65,676 records, 평균 99.9892 Hz. 39개 숫자 column과 raw byte 값이 `0x11A.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [maybe_warning](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 16 B, 0 fields: 길이 일치, 의미는 별도 검증.
- [FR_CMR_01_10ms](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 23 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-120"></a>
## 0x120 앞 모터 회전수, 부하, DC 전압 후보

32 B, 65,687 records, 평균 100.0059 Hz. 35개 숫자 column과 raw byte 값이 `0x120.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FRONT_MOTOR_SPEED_RPM_CANDIDATE | 24:16 S LE | raw x1, rpm | -29 ~ 3862 | source_and_data_supported_candidate |
| FRONT_MOTOR_LOAD_RAW | 40:10 S LE | raw x1, raw_counts | -140 ~ 262 | cross_frame_supported_candidate |
| FRONT_DC_LINK_VOLTAGE_V_CANDIDATE | 128:16 U LE | raw x1, V | 683 ~ 702 | source_and_data_supported_candidate |

- `FRONT_MOTOR_SPEED_RPM_CANDIDATE`: Exact vehicle-source layout; rear channel tracks wheel/GNSS speed; front channel can remain zero while rear moves. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)
- `FRONT_MOTOR_LOAD_RAW`: Signed 10-bit load-like channel; related copies in 0x035, 0x0DA and packed 0x0F5. Nm/A scaling not established.
- `FRONT_DC_LINK_VOLTAGE_V_CANDIDATE`: Emulator source maps bytes 16..17 to live pack V. Observed 683..702, consistent with OEM nominal 697 V and other voltage-like channels. [source](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp)

공개 자료의 같은 ID 후보:
- [FRONT_MOTOR](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 3 fields: 길이 일치, 의미는 별도 검증.

<a id="id-125"></a>
## 0x125 STEERING_SENSORS

16 B, 65,343 records, 평균 99.4822 Hz. 21개 숫자 column과 raw byte 값이 `0x125.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| gnss_yaw_deg_s | 24:16 little S | 0.979175 | 0.04632 | 0.20 | 0.5796 |
| yaw_deg_s | 25:12 little S | 0.958038 | 0.04016 | -0.20 | 0.6228 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [Qaccel_dependent](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 16 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [STEERING_SENSORS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 4 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-12a"></a>
## 0x12A LFA

16 B, 65,683 records, 평균 99.9998 Hz. 53개 숫자 column과 raw byte 값이 `0x12A.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `fast_dynamic`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [LFA](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 25 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-130"></a>
## 0x130 GEAR_SHIFTER

16 B, 65,685 records, 평균 100.0029 Hz. 21개 숫자 column과 raw byte 값이 `0x130.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [DATC_PE_01](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.
- [YRS1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_i30_2014.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.
- [YRS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.
- [GEAR_SHIFTER](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 5 fields: 길이 일치, 의미는 별도 검증.
- [YRS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-145"></a>
## 0x145 빠른 센서, 제어 피드백 또는 주행 텔레메트리 후보

32 B, 32,844 records, 평균 50.0037 Hz. 32개 숫자 column과 raw byte 값이 `0x145.npz`에 있다.

근거: Rate and nonprotocol bit activity support a dynamic numeric class only; no unique named physical signal.

본문 클래스: `fast_dynamic`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TMU_HU_PE_01](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 10 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-15a"></a>
## 0x15A 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 65,687 records, 평균 100.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x15A.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-160"></a>
## 0x160 ADRV_0x160

16 B, 32,841 records, 평균 49.9992 Hz. 30개 숫자 column과 raw byte 값이 `0x160.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [AHB1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 10 fields: 길이 불일치, 적용 제외.
- [ADRV_0x160](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 7 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-175"></a>
## 0x175 TCS

24 B, 32,843 records, 평균 50.0022 Hz. 67개 숫자 column과 raw byte 값이 `0x175.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_175](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 24 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [TCS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 24 B, 18 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-180"></a>
## 0x180 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 32,873 records, 평균 50.0479 Hz. 8개 숫자 column과 raw byte 값이 `0x180.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [AMP_HU_PE_01](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 14 fields: 길이 일치, 의미는 별도 검증.
- [CAM_0x180](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/_hyundai_canfd_common.dbc), 32 B, 2 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-185"></a>
## 0x185 CAM_0x185

8 B, 32,873 records, 평균 50.0479 Hz. 10개 숫자 column과 raw byte 값이 `0x185.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [AMP_HU_PE_05](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.
- [CAM_0x185](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/_hyundai_canfd_common.dbc), 8 B, 2 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-19a"></a>
## 0x19A 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 65,687 records, 평균 100.0059 Hz. 32개 숫자 column과 raw byte 값이 `0x19A.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [HU_CLU_PE_08](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 4 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1a0"></a>
## 0x1A0 SCC_CONTROL

32 B, 32,841 records, 평균 49.9992 Hz. 81개 숫자 column과 raw byte 값이 `0x1A0.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [radar_cruiz](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 11 fields: 길이 일치, 의미는 별도 검증.
- [SCC_CONTROL](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 35 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1aa"></a>
## 0x1AA 버튼과 속도 복제 후보

16 B, 32,873 records, 평균 50.0479 Hz. 42개 숫자 column과 raw byte 값이 `0x1AA.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FILTERED_SPEED_MPS_HYPOTHESIS | 48:8 U LE | raw x0.125, m/s | 0 ~ 16.875 | working_unit_hypothesis |

- `FILTERED_SPEED_MPS_HYPOTHESIS`: Strong lagged speed relationship. Raw*0.45 kph is a compact encoding hypothesis, RMSE about 1.52 kph; no direct source unit definition.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| gnss_speed_kph | 48:8 little U | 0.998512 | 0.02182 | 0.30 | 0.2825 |
| rear_motor_rpm_source | 48:8 little U | 0.999604 | 0.02635 | 0.30 | 0.6937 |
| speed_kph | 48:8 little U | 0.999507 | 0.03176 | 0.30 | 0.6036 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_1AA](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 16 B, 5 fields: 길이 일치, 의미는 별도 검증.
- [CRUISE_BUTTONS_ALT](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 25 fields: 길이 일치, 의미는 별도 검증.

<a id="id-1b0"></a>
## 0x1B0 빠른 센서, 제어 피드백 또는 주행 텔레메트리 후보

32 B, 32,844 records, 평균 50.0037 Hz. 32개 숫자 column과 raw byte 값이 `0x1B0.npz`에 있다.

근거: Rate and nonprotocol bit activity support a dynamic numeric class only; no unique named physical signal.

본문 클래스: `fast_dynamic`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TMU_GW_PE_01](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 5 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1b5"></a>
## 0x1B5 CCNC_0x1B5 / FR_CMR_03_50ms

32 B, 13,136 records, 평균 19.9991 Hz. 50개 숫자 column과 raw byte 값이 `0x1B5.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_1B5](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 7 fields: 길이 일치, 의미는 별도 검증.
- [FR_CMR_03_50ms](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 17 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1ba"></a>
## 0x1BA BLINDSPOTS_REAR_CORNERS / ADAS_CMD_50_50ms

24 B, 13,137 records, 평균 20.0006 Hz. 54개 숫자 column과 raw byte 값이 `0x1BA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ADAS_CMD_50_50ms](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 24 B, 24 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1cf"></a>
## 0x1CF CRUISE_BUTTONS

8 B, 32,843 records, 평균 50.0022 Hz. 18개 숫자 column과 raw byte 값이 `0x1CF.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_1CF](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [CRUISE_BUTTONS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 9 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1da"></a>
## 0x1DA ADRV_0x1da

32 B, 658 records, 평균 1.0018 Hz. 36개 숫자 column과 raw byte 값이 `0x1DA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [CLU_HU_PE_02](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 6 fields: 길이 불일치, 적용 제외.
- [ADRV_0x1da](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 4 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1e0"></a>
## 0x1E0 LFAHDA_CLUSTER

16 B, 13,137 records, 평균 20.0006 Hz. 34개 숫자 column과 raw byte 값이 `0x1E0.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [LFAHDA_CLUSTER](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 9 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1e5"></a>
## 0x1E5 BLINDSPOTS_FRONT_CORNER_1

16 B, 13,136 records, 평균 19.9991 Hz. 27개 숫자 column과 raw byte 값이 `0x1E5.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [HU_CLU_PE_11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 13 fields: 길이 불일치, 적용 제외.
- [BLINDSPOTS_FRONT_CORNER_1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 11 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1ea"></a>
## 0x1EA ADRV_0x1ea

32 B, 13,137 records, 평균 20.0006 Hz. 87개 숫자 column과 raw byte 값이 `0x1EA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ADRV_0x1ea](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 26 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1f0"></a>
## 0x1F0 고정 설정, 비활성 기능 또는 상태 프레임 후보

16 B, 13,137 records, 평균 20.0006 Hz. 16개 숫자 column과 raw byte 값이 `0x1F0.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1f5"></a>
## 0x1F5 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 6,568 records, 평균 9.9995 Hz. 32개 숫자 column과 raw byte 값이 `0x1F5.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-1fa"></a>
## 0x1FA CLUSTER_SPEED_LIMIT / FR_CMR_02_100ms

32 B, 6,568 records, 평균 9.9995 Hz. 77개 숫자 column과 raw byte 값이 `0x1FA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [FR_CMR_02_100ms](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 29 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-200"></a>
## 0x200 ADRV_0x200

8 B, 13,137 records, 평균 20.0006 Hz. 17개 숫자 column과 raw byte 값이 `0x200.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_200](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 0 fields: 길이 일치, 의미는 별도 검증.
- [EMS20](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 6 B, 3 fields: 길이 불일치, 적용 제외.
- [EMS20](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 6 B, 3 fields: 길이 불일치, 적용 제외.
- [ADRV_0x200](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 4 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-20a"></a>
## 0x20A 고정 설정, 비활성 기능 또는 상태 프레임 후보

16 B, 6,569 records, 평균 10.0011 Hz. 16개 숫자 column과 raw byte 값이 `0x20A.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-225"></a>
## 0x225 이산 상태, 이벤트 또는 설정 코드 후보

16 B, 6,574 records, 평균 10.0087 Hz. 16개 숫자 column과 raw byte 값이 `0x225.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-235"></a>
## 0x235 고전압/배터리 측 텔레메트리 후보

32 B, 6,568 records, 평균 9.9995 Hz. 33개 숫자 column과 raw byte 값이 `0x235.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| HV_VOLTAGE_V_CANDIDATE | 104:16 U LE | raw x0.1, V | 685.5 ~ 701.3 | working_unit_hypothesis |

- `HV_VOLTAGE_V_CANDIDATE`: 16-bit channel 6855..7013 tracks inverter-voltage references; scaling /10 produces plausible 685.5..701.3 V. Battery-side source identifies ID but not this field. [source](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp)

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_inverter_voltage_V_source | 104:8 little S | 0.994093 | 0.03195 | 0.20 | 0.1383 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

<a id="id-250"></a>
## 0x250 전기 구동 전력/부하 후보

24 B, 6,569 records, 평균 10.0011 Hz. 26개 숫자 column과 raw byte 값이 `0x250.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| SIGNED_POWER_LOAD_RAW | 168:10 S LE | raw x1, raw_counts | -56 ~ 47 | electrical_load_candidate |
| POWER_KW_WORKING_HYPOTHESIS | 168:10 S LE | raw x1, kW | -56 ~ 47 | working_unit_hypothesis |

- `SIGNED_POWER_LOAD_RAW`: 10-bit signed interpretation removes upper status flag; raw -56..47 tracks 0x2FA current/power-like channel. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)
- `POWER_KW_WORKING_HYPOTHESIS`: Joint unit hypothesis with 0x2FA raw/10 A and inverter V. Overall current raw relationship gain about0.068, compatible with ~690V/10000. No calibrated external power sensor.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| power_current_raw | 168:8 little S | 0.994609 | 0.02133 | 0.00 | 0.5522 |
| mechanical_power_proxy | 168:8 little S | 0.991255 | 0.03143 | 0.10 | 0.3823 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_250](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 24 B, 3 fields: 길이 일치, 의미는 별도 검증.

<a id="id-255"></a>
## 0x255 두 번째 고전압 채널 후보

32 B, 6,568 records, 평균 9.9995 Hz. 33개 숫자 column과 raw byte 값이 `0x255.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| HV_VOLTAGE_2_V_CANDIDATE | 136:16 U LE | raw x0.1, V | 682.1 ~ 698.7 | working_unit_hypothesis |

- `HV_VOLTAGE_2_V_CANDIDATE`: Tracks 0x235 voltage channel with systematic offset, raw6821..6987. Sensor location and absolute calibration unknown.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| pack_voltage_proxy | 136:8 little S | 0.994222 | 0.02275 | -0.10 | 0.2741 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

<a id="id-25a"></a>
## 0x25A 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 6,568 records, 평균 9.9995 Hz. 32개 숫자 column과 raw byte 값이 `0x25A.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2aa"></a>
## 0x2AA 고정 설정, 비활성 기능 또는 상태 프레임 후보

32 B, 6,567 records, 평균 9.9980 Hz. 32개 숫자 column과 raw byte 값이 `0x2AA.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2b0"></a>
## 0x2B0 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 3,284 records, 평균 4.9998 Hz. 32개 숫자 column과 raw byte 값이 `0x2B0.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [SAS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 5 B, 5 fields: 길이 불일치, 적용 제외.
- [SAS1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_i30_2014.dbc), 8 B, 5 fields: 길이 불일치, 적용 제외.
- [SAS_Data](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_santafe_2007.dbc), 5 B, 5 fields: 길이 불일치, 적용 제외.
- [SAS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 5 B, 5 fields: 길이 불일치, 적용 제외.
- [SAS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 5 B, 5 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2b5"></a>
## 0x2B5 가속 페달 복제와 전장 상태 후보

32 B, 6,567 records, 평균 9.9980 Hz. 33개 숫자 column과 raw byte 값이 `0x2B5.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| ACCELERATOR_PEDAL_COPY_RAW | 48:8 U LE | raw x1, raw_counts | 0 ~ 112 | calibrated_candidate |

- `ACCELERATOR_PEDAL_COPY_RAW`: Matches 0x035 pedal raw with held-out r>0.999. 6567 nearest-stamp pairs within20ms, 82.78% exactly equal; async/filtered copy not same-instant identity. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| pedal_raw | 48:8 little S | 0.999348 | 0.00912 | 0.10 | 0.7552 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [Throttle_Pos](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 1 fields: 길이 불일치, 적용 제외.

<a id="id-2c0"></a>
## 0x2C0 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 3,285 records, 평균 5.0013 Hz. 32개 숫자 column과 raw byte 값이 `0x2C0.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_2C0](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 1 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2d5"></a>
## 0x2D5 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 3,285 records, 평균 5.0013 Hz. 32개 숫자 column과 raw byte 값이 `0x2D5.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2e0"></a>
## 0x2E0 MANUAL_SPEED_LIMIT_ASSIST

32 B, 6,567 records, 평균 9.9980 Hz. 38개 숫자 column과 raw byte 값이 `0x2E0.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [MANUAL_SPEED_LIMIT_ASSIST](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 32 B, 6 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2e5"></a>
## 0x2E5 이산 상태, 이벤트 또는 설정 코드 후보

32 B, 6,568 records, 평균 9.9995 Hz. 32개 숫자 column과 raw byte 값이 `0x2E5.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-2fa"></a>
## 0x2FA 배터리 측 전류/전력성 부하 후보

32 B, 6,567 records, 평균 9.9980 Hz. 34개 숫자 column과 raw byte 값이 `0x2FA.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| SIGNED_ELECTRIC_CURRENT_OR_POWER_LOAD_RAW | 64:16 S LE | raw x1, raw_counts | -759 ~ 698 | electrical_load_candidate |
| CURRENT_A_WORKING_HYPOTHESIS | 64:16 S LE | raw x0.1, A | -75.9 ~ 69.8 | working_unit_hypothesis |

- `SIGNED_ELECTRIC_CURRENT_OR_POWER_LOAD_RAW`: Signed raw -759..698 strongly tracks mechanical power proxy and related 0x250 channel. Battery-side ID source does not define A scale. [source](https://github.com/dalathegreat/Battery-Emulator/blob/ef6a1b2d716c8b46e0c134d7a236afe96dd620a2/Software/src/battery/KIA-E-GMP-BATTERY.cpp)
- `CURRENT_A_WORKING_HYPOTHESIS`: Joint hypothesis with 0x250 integer-kW field: current=raw/10 A makes V*I track its raw power scale. No independent current measurement; torque=1Nm/count energy model fails and is not certified.

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| mechanical_power_proxy | 64:11 little S | 0.998138 | 0.01455 | 0.20 | 0.5057 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [NEW_MSG_2FA](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 1 fields: 길이 일치, 의미는 별도 검증.

<a id="id-2ff"></a>
## 0x2FF 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,287 records, 평균 5.0043 Hz. 8개 숫자 column과 raw byte 값이 `0x2FF.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-30a"></a>
## 0x30A 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 6,568 records, 평균 9.9995 Hz. 32개 숫자 column과 raw byte 값이 `0x30A.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-315"></a>
## 0x315 이산 상태, 이벤트 또는 설정 코드 후보

16 B, 3,287 records, 평균 5.0043 Hz. 16개 숫자 column과 raw byte 값이 `0x315.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-31a"></a>
## 0x31A 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 658 records, 평균 1.0018 Hz. 32개 숫자 column과 raw byte 값이 `0x31A.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-325"></a>
## 0x325 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 6,567 records, 평균 9.9980 Hz. 32개 숫자 column과 raw byte 값이 `0x325.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-330"></a>
## 0x330 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 6,567 records, 평균 9.9980 Hz. 32개 숫자 column과 raw byte 값이 `0x330.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-33a"></a>
## 0x33A 고정 설정, 비활성 기능 또는 상태 프레임 후보

32 B, 6,568 records, 평균 9.9995 Hz. 32개 숫자 column과 raw byte 값이 `0x33A.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-345"></a>
## 0x345 ADRV_0x345

8 B, 3,284 records, 평균 4.9998 Hz. 11개 숫자 column과 raw byte 값이 `0x345.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ADRV_0x345](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-34a"></a>
## 0x34A 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 658 records, 평균 1.0018 Hz. 32개 숫자 column과 raw byte 값이 `0x34A.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-360"></a>
## 0x360 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 6,567 records, 평균 9.9980 Hz. 32개 숫자 column과 raw byte 값이 `0x360.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_360](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 32 B, 0 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-36a"></a>
## 0x36A BLINDSPOTS_FRONT_CORNER_2

16 B, 13,137 records, 평균 20.0006 Hz. 20개 숫자 column과 raw byte 값이 `0x36A.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [BLINDSPOTS_FRONT_CORNER_2](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 16 B, 2 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-36f"></a>
## 0x36F 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 6,561 records, 평균 9.9889 Hz. 8개 숫자 column과 raw byte 값이 `0x36F.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-37f"></a>
## 0x37F 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 6,561 records, 평균 9.9889 Hz. 8개 숫자 column과 raw byte 값이 `0x37F.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-380"></a>
## 0x380 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,285 records, 평균 5.0013 Hz. 8개 숫자 column과 raw byte 값이 `0x380.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [DI_BOX13](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 12 fields: 길이 일치, 의미는 별도 검증.
- [DI_BOX13](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 12 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-382"></a>
## 0x382 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,290 records, 평균 5.0089 Hz. 8개 숫자 column과 raw byte 값이 `0x382.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [EMS9](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_i30_2014.dbc), 8 B, 13 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-383"></a>
## 0x383 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x383.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [FATC11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 19 fields: 길이 일치, 의미는 별도 검증.
- [FATC11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 19 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-384"></a>
## 0x384 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x384.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [EMS17](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 10 fields: 길이 일치, 의미는 별도 검증.
- [EMS17](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 10 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-386"></a>
## 0x386 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,285 records, 평균 5.0013 Hz. 8개 숫자 column과 raw byte 값이 `0x386.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [WHL_SPD11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.
- [WHL_SPD11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.
- [WHL_SPD11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-38a"></a>
## 0x38A 고정 설정, 비활성 기능 또는 상태 프레임 후보

16 B, 3,287 records, 평균 5.0043 Hz. 16개 숫자 column과 raw byte 값이 `0x38A.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ABS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.
- [ABS11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 7 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-39b"></a>
## 0x39B 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,247 records, 평균 4.9434 Hz. 8개 숫자 column과 raw byte 값이 `0x39B.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [RADAR_0x39b](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 8 B, 9 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3a0"></a>
## 0x3A0 TPMS

16 B, 3,287 records, 평균 5.0043 Hz. 22개 숫자 column과 raw byte 값이 `0x3A0.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TPMS_BROADCAST](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 16 B, 5 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3a5"></a>
## 0x3A5 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 6,568 records, 평균 9.9995 Hz. 8개 숫자 column과 raw byte 값이 `0x3A5.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3a6"></a>
## 0x3A6 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,285 records, 평균 5.0013 Hz. 8개 숫자 column과 raw byte 값이 `0x3A6.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3aa"></a>
## 0x3AA 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x3AA.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3b0"></a>
## 0x3B0 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x3B0.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3b1"></a>
## 0x3B1 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,286 records, 평균 5.0028 Hz. 8개 숫자 column과 raw byte 값이 `0x3B1.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3b2"></a>
## 0x3B2 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x3B2.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3b5"></a>
## 0x3B5 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 3,284 records, 평균 4.9998 Hz. 32개 숫자 column과 raw byte 값이 `0x3B5.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3c1"></a>
## 0x3C1 BLINKER_STALKS

8 B, 3,318 records, 평균 5.0515 Hz. 15개 숫자 column과 raw byte 값이 `0x3C1.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [BLINKER_STALKS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 7 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3c2"></a>
## 0x3C2 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,266 records, 평균 4.9724 Hz. 8개 숫자 column과 raw byte 값이 `0x3C2.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3ca"></a>
## 0x3CA 고정 설정, 비활성 기능 또는 상태 프레임 후보

16 B, 658 records, 평균 1.0018 Hz. 16개 숫자 column과 raw byte 값이 `0x3CA.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3e0"></a>
## 0x3E0 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x3E0.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3e1"></a>
## 0x3E1 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x3E1.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3e2"></a>
## 0x3E2 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x3E2.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3e5"></a>
## 0x3E5 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x3E5.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3e6"></a>
## 0x3E6 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x3E6.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3ea"></a>
## 0x3EA 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,282 records, 평균 4.9967 Hz. 8개 숫자 column과 raw byte 값이 `0x3EA.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3f0"></a>
## 0x3F0 느린 수치 텔레메트리 또는 복합 상태 후보

32 B, 658 records, 평균 1.0018 Hz. 32개 숫자 column과 raw byte 값이 `0x3F0.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-3f5"></a>
## 0x3F5 배터리 측 통신에 참여하는 텔레메트리/상태 후보

32 B, 658 records, 평균 1.0018 Hz. 32개 숫자 column과 raw byte 값이 `0x3F5.npz`에 있다.

근거: ID appears in isolated E-GMP battery receive code; this does not establish its sender or field semantics on ECAN.

본문 클래스: `slow_numeric_or_status`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-401"></a>
## 0x401 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,282 records, 평균 4.9967 Hz. 8개 숫자 column과 raw byte 값이 `0x401.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-405"></a>
## 0x405 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,282 records, 평균 4.9967 Hz. 8개 숫자 column과 raw byte 값이 `0x405.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-410"></a>
## 0x410 이산 상태, 이벤트 또는 설정 코드 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x410.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [CGW_USM1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 17 fields: 길이 일치, 의미는 별도 검증.
- [CGW_USM1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 17 fields: 길이 일치, 의미는 별도 검증.
- [CGW_USM1](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 8 B, 17 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-411"></a>
## 0x411 DOORS_SEATBELTS

8 B, 3,282 records, 평균 4.9967 Hz. 16개 숫자 column과 raw byte 값이 `0x411.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [Door_Seatbelt_Status](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.
- [UNKNOWN_411](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 0 fields: 길이 일치, 의미는 별도 검증.
- [DOORS_SEATBELTS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-412"></a>
## 0x412 게이트웨이 상태 후보; 공개 B6 브레이크 정의는 관측과 불일치

8 B, 3,385 records, 평균 5.1535 Hz. 8개 숫자 column과 raw byte 값이 `0x412.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `sparse_status_or_event`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [Brake_Pedal_Status](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [ICM_412h](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 16 fields: 길이 일치, 의미는 별도 검증.
- [ICM_412h](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 8 B, 16 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-413"></a>
## 0x413 BLINKERS

8 B, 3,865 records, 평균 5.8843 Hz. 16개 숫자 column과 raw byte 값이 `0x413.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [BLINKERS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-414"></a>
## 0x414 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,281 records, 평균 4.9952 Hz. 8개 숫자 column과 raw byte 값이 `0x414.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-416"></a>
## 0x416 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,281 records, 평균 4.9952 Hz. 8개 숫자 column과 raw byte 값이 `0x416.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-417"></a>
## 0x417 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,281 records, 평균 4.9952 Hz. 8개 숫자 column과 raw byte 값이 `0x417.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-418"></a>
## 0x418 조향 열선 상태 source 후보, 자극 없음

8 B, 3,384 records, 평균 5.1520 Hz. 9개 숫자 column과 raw byte 값이 `0x418.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `sparse_status_or_event`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| STEERING_HEAT_STATE_SOURCE_CANDIDATE | 16:8 U LE | raw x1, enum_code | 0 ~ 0 | source_only_unexcited |

- `STEERING_HEAT_STATE_SOURCE_CANDIDATE`: E-GMP source names this field. All observed values0; no deliberate heating event establishes this vehicle meaning. [source](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc)

공개 자료의 같은 ID 후보:
- [Steering_Wheel_Heating](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [Steering_Wheel_Heating](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.

<a id="id-419"></a>
## 0x419 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 10,840 records, 평균 16.5035 Hz. 8개 숫자 column과 raw byte 값이 `0x419.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NEW_MSG_419](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 2 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-41a"></a>
## 0x41A 이산 상태, 이벤트 또는 설정 코드 후보

8 B, 3,293 records, 평균 5.0135 Hz. 8개 숫자 column과 raw byte 값이 `0x41A.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-41b"></a>
## 0x41B 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x41B.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-41c"></a>
## 0x41C LIGHT_SENSOR

8 B, 12,610 records, 평균 19.1982 Hz. 12개 숫자 column과 raw byte 값이 `0x41C.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-41f"></a>
## 0x41F 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 1,081 records, 평균 1.6458 Hz. 8개 숫자 column과 raw byte 값이 `0x41F.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-422"></a>
## 0x422 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x422.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-425"></a>
## 0x425 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x425.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-427"></a>
## 0x427 이산 상태, 이벤트 또는 설정 코드 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x427.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-429"></a>
## 0x429 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x429.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [_4WD12](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 8 B, 6 fields: 길이 일치, 의미는 별도 검증.
- [AWD_Data2](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_santafe_2007.dbc), 8 B, 5 fields: 길이 일치, 의미는 별도 검증.
- [_4WD12](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 8 B, 6 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-42a"></a>
## 0x42A 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 3,284 records, 평균 4.9998 Hz. 8개 숫자 column과 raw byte 값이 `0x42A.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [_4WD13](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 6 B, 4 fields: 길이 불일치, 적용 제외.
- [_4WD13](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 6 B, 4 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-432"></a>
## 0x432 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x432.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-435"></a>
## 0x435 필터된 차속 후보

8 B, 5,361 records, 평균 8.1619 Hz. 9개 숫자 column과 raw byte 값이 `0x435.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `slow_numeric_or_status`. checksum: `observed_CRC8_equivalent`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| FILTERED_SPEED_KPH_CANDIDATE | 56:8 U LE | raw x1, km/h | 0 ~ 64 | source_and_data_supported_candidate |

- `FILTERED_SPEED_KPH_CANDIDATE`: Vehicle-source definition plus speed/GNSS agreement. Best relative alignment about 0.5 s; do not interpret host stamps as hardware timing. [source](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc)

| 비교 reference | 후보 bit | holdout r | NRMSE | lag s | 차분 r |
| --- | --- | ---: | ---: | ---: | ---: |
| rear_motor_rpm_source | 56:8 little S | 0.999686 | 0.00883 | 0.50 | 0.5066 |
| speed_kph | 56:8 little S | 0.999528 | 0.01177 | 0.50 | 0.4666 |
| gnss_speed_kph | 56:8 little S | 0.998822 | 0.01263 | 0.50 | 0.3130 |
| brake_pressed | 13:16 little S | -0.992179 | 0.05580 | 0.00 | -0.7131 |

최고 상관의 최소 bit 폭은 수치적으로 동등한 alias일 수 있다. 위 추가 필드의 nominal 폭과 source 대안을 함께 확인한다.

공개 자료의 같은 ID 후보:
- [Power_CAN_Status](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.
- [VEH_SPEED](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.

<a id="id-442"></a>
## 0x442 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x442.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NM_KBD](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-444"></a>
## 0x444 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x444.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NM_CLOCK](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-445"></a>
## 0x445 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x445.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NM_RRC](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-448"></a>
## 0x448 조향 버튼 source 후보, 자극 없음

8 B, 3,284 records, 평균 4.9998 Hz. 10개 숫자 column과 raw byte 값이 `0x448.npz`에 있다.

근거: Published target-platform source and/or held-out relationships; individual fields retain distinct confidence.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.

| 추가 필드 | 배치 | 변환과 단위 | 관측 범위 | confidence |
| --- | --- | --- | --- | --- |
| OTHER_BUTTONS_SOURCE_CANDIDATE | 16:8 U LE | raw x1, enum_code | 0 ~ 0 | source_only_unexcited |
| STAR_BUTTON_SOURCE_CANDIDATE | 40:8 U LE | raw x1, enum_code | 0 ~ 0 | source_only_unexcited |

- `OTHER_BUTTONS_SOURCE_CANDIDATE`: E-GMP source names this field; payload constant in these captures. Alternative older network-management definition exists. [source](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc)
- `STAR_BUTTON_SOURCE_CANDIDATE`: E-GMP source names this field; payload constant in these captures. Alternative older network-management definition exists. [source](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc)

공개 자료의 같은 ID 후보:
- [Steering_Wheel_Buttons](https://github.com/dragz/egmpdbc/blob/b234d7e0ff5ba881f80087580d6e86cf8af447f1/ioniq5-2022.dbc), 8 B, 9 fields: 길이 일치, 의미는 별도 검증.
- [Steering_Wheel_Buttons](https://github.com/Sterlingarcher2525/ioniq5-can/blob/8351cc0317e6cd45a286c2ed9b3d64f65336f666/dbc/I5_with_torque_pids.dbc), 8 B, 2 fields: 길이 일치, 의미는 별도 검증.
- [NM_AMP](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

<a id="id-454"></a>
## 0x454 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,288 records, 평균 5.0059 Hz. 8개 숫자 column과 raw byte 값이 `0x454.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [NM_HUD](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 3 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-472"></a>
## 0x472 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,287 records, 평균 5.0043 Hz. 8개 숫자 column과 raw byte 값이 `0x472.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-473"></a>
## 0x473 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,287 records, 평균 5.0043 Hz. 8개 숫자 column과 raw byte 값이 `0x473.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-474"></a>
## 0x474 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,287 records, 평균 5.0043 Hz. 8개 숫자 column과 raw byte 값이 `0x474.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `Hyundai_CANFD_CRC16`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-47f"></a>
## 0x47F HVAC_TOUCH_BUTTONS

8 B, 3,284 records, 평균 4.9998 Hz. 19개 숫자 column과 raw byte 값이 `0x47F.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [ESP11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_ccan.dbc), 6 B, 11 fields: 길이 불일치, 적용 제외.
- [ESP11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_can.dbc), 6 B, 11 fields: 길이 불일치, 적용 제외.
- [HVAC_TOUCH_BUTTONS](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 11 fields: 길이 일치, 의미는 별도 검증.
- [ESP11](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_palisade_2023.dbc), 6 B, 11 fields: 길이 불일치, 적용 제외.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-48f"></a>
## 0x48F 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 657 records, 평균 1.0003 Hz. 8개 숫자 column과 raw byte 값이 `0x48F.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TP_HU_Ipod_CLU](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4a3"></a>
## 0x4A3 HDA_INFO_4A3

8 B, 3,253 records, 평균 4.9526 Hz. 17개 숫자 column과 raw byte 값이 `0x4A3.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `sparse_status_or_event`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4b4"></a>
## 0x4B4 NEW_MSG_4B4

8 B, 6,419 records, 평균 9.7727 Hz. 11개 숫자 column과 raw byte 값이 `0x4B4.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TP_HU_DMB_CLU](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4b8"></a>
## 0x4B8 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 119 records, 평균 0.1812 Hz. 8개 숫자 column과 raw byte 값이 `0x4B8.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4b9"></a>
## 0x4B9 NEW_MSG_4B9

8 B, 120 records, 평균 0.1827 Hz. 20개 숫자 column과 raw byte 값이 `0x4B9.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4ba"></a>
## 0x4BA NEW_MSG_4BA

8 B, 873 records, 평균 1.3291 Hz. 16개 숫자 column과 raw byte 값이 `0x4BA.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4be"></a>
## 0x4BE NEW_MSG_4BE

8 B, 66 records, 평균 0.1005 Hz. 16개 숫자 column과 raw byte 값이 `0x4BE.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4bf"></a>
## 0x4BF 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 870 records, 평균 1.3245 Hz. 8개 숫자 column과 raw byte 값이 `0x4BF.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4d8"></a>
## 0x4D8 CLUSTER_INFO

8 B, 3,246 records, 평균 4.9419 Hz. 9개 숫자 column과 raw byte 값이 `0x4D8.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [CLUSTER_INFO](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 1 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4dd"></a>
## 0x4DD 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 3,246 records, 평균 4.9419 Hz. 8개 숫자 column과 raw byte 값이 `0x4DD.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4df"></a>
## 0x4DF 이산 상태, 이벤트 또는 설정 코드 후보

8 B, 3,251 records, 평균 4.9495 Hz. 8개 숫자 column과 raw byte 값이 `0x4DF.npz`에 있다.

근거: Sparse nonprotocol transitions; absent event labels prevent unique function assignment.

본문 클래스: `sparse_status_or_event`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4eb"></a>
## 0x4EB LOCAL_TIME2

8 B, 3,246 records, 평균 4.9419 Hz. 12개 숫자 column과 raw byte 값이 `0x4EB.npz`에 있다.

근거: Existing source definitions; source variants and unused fields are not all vehicle-verified.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TP_CLU_MP_HU](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.
- [LOCAL_TIME2](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/generator/hyundai/hyundai_canfd.dbc), 8 B, 4 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4f2"></a>
## 0x4F2 느린 수치 텔레메트리 또는 복합 상태 후보

8 B, 4,702 records, 평균 7.1586 Hz. 8개 숫자 column과 raw byte 값이 `0x4F2.npz`에 있다.

근거: Observed numeric/state morphology; temperature/voltage/counter alternatives may remain indistinguishable.

본문 클래스: `slow_numeric_or_status`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TP_HU_CARPLAY_CLU](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4f3"></a>
## 0x4F3 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 650 records, 평균 0.9896 Hz. 8개 숫자 column과 raw byte 값이 `0x4F3.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


공개 자료의 같은 ID 후보:
- [TP_CLU_CARPLAY_HU](https://github.com/sunnypilot/opendbc/blob/b2acec1dc7f15cfb258aa48415168485e29cc4ff/opendbc/dbc/hyundai_2015_mcan.dbc), 8 B, 8 fields: 길이 일치, 의미는 별도 검증.

새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.

<a id="id-4fe"></a>
## 0x4FE 고정 설정, 비활성 기능 또는 상태 프레임 후보

8 B, 325 records, 평균 0.4948 Hz. 8개 숫자 column과 raw byte 값이 `0x4FE.npz`에 있다.

근거: No changing nonprotocol body observed. Exact physical function/scale is not identifiable from this capture.

본문 클래스: `constant_payload_after_protocol`. checksum: `unresolved`. 검증된 CRC 관계와 의미 검증은 별개다.


새로운 특정 물리 필드 배치/단위는 확인되지 않았다. source 정의가 있으면 후보 숫자 해석을 유지하고, 나머지는 raw byte 값으로 제공한다.
