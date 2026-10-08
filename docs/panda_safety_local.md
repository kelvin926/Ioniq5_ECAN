# Panda safety 로컬 편집 방법

차량에 연결하지 않아도 Windows에서 고정 버전의 Panda와 opendbc 전체 소스를 준비할 수 있습니다.
준비 과정에서 이 프로젝트의 기존 패치 5개를 적용하고, 패치가 적용된 상태를 기준점으로 저장합니다.
전체 소스 파일을 편집할 수 있으며, 로컬 safety patch가 있으면 준비 단계에서 함께 적용됩니다.

## 소스 준비

저장소 루트에서 PowerShell을 열고 아래 명령을 실행합니다.
Windows의 `bash` 명령은 WSL 실행기로 연결될 수 있으므로 Git Bash의 경로를 직접 지정합니다.

```powershell
& 'C:\Program Files\Git\bin\bash.exe' --noprofile --norc `
  scripts/build_panda_debug_firmware.sh --prepare-only
```

소스 준비에는 Git이 필요합니다. `--prepare-only`는 소스를 준비하는 단계이므로
Python/패키지 설치, 컴파일러 실행, Panda 장치 접근을 수행하지 않습니다.
받은 소스와 기준점 정보는 Git에서 제외된 `.firmware-build/` 폴더에 저장합니다.
다른 빌드 폴더를 사용하려면 `--prepare-only` 뒤에 해당 경로를 지정합니다.
기존 소스 수정은 보존하며, 패치 적용이 충돌하면 준비를 중단합니다.

## 항목별 수정 위치

아래 경로는 `.firmware-build/` 폴더를 기준으로 합니다.
안전 검사 소스는 opendbc에 있으며 Panda 펌웨어에 함께 컴파일됩니다.
MCU의 동작과 모드 전환을 처리하는 소스는 Panda에 있습니다.

| 조정 항목 | 파일 | 확인할 정의 |
| --- | --- | --- |
| 조향 제한 | `opendbc/opendbc/safety/modes/hyundai_canfd.h` | `HYUNDAI_CANFD_STEERING_LIMITS`: 최대 토크, 증가/감소율, 시간 기준 토크 변화, 운전자 토크 허용치/배율, 요청 프레임 수와 간격 |
| 가속도 제한 | `opendbc/opendbc/safety/modes/hyundai_canfd.h` | `HYUNDAI_CANFD_LONG_LIMITS`: 최소/최대 가속도, 단위 0.01 m/s² |
| 송신 메시지 허용 목록 | `opendbc/opendbc/safety/modes/hyundai_canfd.h` | ECAN 전용 TX 배열, ID/bus/길이 조합, 메시지별 검사 |
| 수신 메시지 검사 조건 | 같은 `hyundai_canfd.h` | RX 배열, 수신 주기와 순환번호 정의, 메시지별 체크섬 처리 |
| LDA/SET/브레이크 채널 처리 | 같은 `hyundai_canfd.h` | `hyundai_canfd_rx_hook`: 조향/종방향 채널의 상태 전환 |
| I5R1 명령 세션 | `opendbc/opendbc/safety/modes/ioniq5_command_session.h` | `ioniq5_session_ready`, `ioniq5_session_enforce`, `ioniq5_session_abort` |
| 초기 수신 대기 | 같은 `ioniq5_command_session.h` | `ioniq5_session_rx_initializing`: 현재 100000 µs, 100 ms |
| 수신 데이터 갱신 시간 | 같은 `ioniq5_command_session.h` | `ioniq5_session_ready`: 현재 100000 µs, 100 ms 이내 갱신 조건 |
| 공통 안전 검사 | `opendbc/opendbc/safety/safety.h` | 수신 유효성, 순환번호 오류 임계값, 송신 허용 목록, 릴레이 검사, 주기적 안전 검사 |
| 토크/각도 검사 알고리즘 | `opendbc/opendbc/safety/lateral.h` | 조향 명령 검사, 운전자 개입 제한, 시간 기준 토크 변화 제한 |
| 종방향 검사 알고리즘 | `opendbc/opendbc/safety/longitudinal.h` | 종방향 제어 허가와 요청값 검사 |
| 공통 상수와 자료형 | `opendbc/opendbc/safety/declarations.h` | `MAX_RT_INTERVAL`, safety 모드, 제한값 구조체 |
| Heartbeat와 MCU 모드 전환 | `panda/board/main.c` | 시동 ON/OFF별 heartbeat 임계값, 제어 활성 상태 불일치, 하네스 모드 변경 |
| 호스트에서 Panda로 보내는 요청 | `panda/board/main_comms.h` | 모드/heartbeat 요청, 프로젝트 전용 B7/B8 세션 요청 |
| ECAN 트랜시버 | `panda/board/main.c`, `panda/board/sys/power_saving.h` | 하네스 방향과 ECAN 전용 트랜시버 동작 |

`controls_allowed`, 고장 플래그, 세션 준비 상태는 검사 결과에 따라 갱신되는 실행 상태입니다.
일반적인 튜닝 상수와 구분하고, 관련 검사와 상태 전환 로직을 함께 확인해야 합니다.
공통 헤더를 수정하면 다른 safety 프로파일에도 영향을 줄 수 있습니다.
새 명령을 추가할 때는 TX 배열 등록과 함께 허용값과 제어 허가 조건도 정의해야 합니다.

## 현재값과 검사 범위

2026-10-08 로컬 준비 소스에서 확인한 값입니다. Panda는
`dd8a5b3df77706337a11555377e7180c5adc8726`, opendbc는
`b72c1fd55ae7e84763e40912bbe06b8f533cb66b`이며 기존 프로젝트 패치 5개가 적용된 상태입니다.
표의 범위는 현재 코드가 검사하는 조건입니다. 다른 값으로 변경했을 때의 실차 허용 범위는
확인되지 않았습니다. 장치의 현재 실행 상태를 조회한 기록과도 구분합니다.

### 조향과 가속도

조향 상수는 `hyundai_canfd.h`의 `HYUNDAI_CANFD_STEERING_LIMITS`, 가속도 상수는
같은 파일의 `HYUNDAI_CANFD_LONG_LIMITS`에 있습니다. 토크의 count는 송신 인터페이스 단위이며
MDPS의 실제 최대 출력 토크를 뜻하지 않습니다.

| 항목 | 변수 | 현재값 | 현재 검사 범위 / 의미 |
| --- | --- | --- | --- |
| 조향 토크 상한 | `max_torque` | 1023 count | 절대값 검사상 −1023~+1023 count, 11-bit 표현 최대값 |
| 토크 증가율 | `max_rate_up` | 1023 count | ±1023 안의 어떤 요청도 한 프레임에 허용 |
| 토크 감소율 | `max_rate_down` | 1023 count | 운전자 배율 0이므로 적용되지 않음 |
| 시간 기준 토크 변화 | `max_rt_delta` | 1023 count | RT 검사가 ±1023 안의 요청을 제한하지 않음 |
| RT 검사 시간창 | `MAX_RT_INTERVAL` | 250000 µs | 250 ms, `declarations.h`의 공통 상수 |
| 운전자 토크 허용치 | `driver_torque_allowance` | 0 count | 운전자 배율 0이므로 적용되지 않음 |
| 운전자 토크 배율 | `driver_torque_multiplier` | 0 | 운전자 토크 한도가 항상 `max_torque` |
| 조향 검사 방식 | `type` | `TorqueDriverLimited` | 위 값으로 운전자 토크 제한 효과 없음 |
| 정상 요청 누적 | `min_valid_request_frames` | 89 프레임 | 비영점 토크를 유지한 요청 비트 해제 예외의 사전 조건 |
| 요청 비트 해제 허용 | `max_invalid_request_frames` | 2 프레임 | 해당 예외에서 허용하는 연속 불일치 프레임 수 |
| 요청 비트 예외 간격 | `min_valid_request_rt_interval` | 810000 µs | 810 ms, 예외 반복에 적용되는 시간 조건 |
| 요청 비트 예외 사용 | `has_steer_req_tolerance` | true | 프레임 수와 시간 조건을 충족한 예외 허용 |
| 최소 가속도 | `HYUNDAI_CANFD_LONG_LIMITS.min_accel` | −1023 | 0.01 m/s² 단위, −10.23 m/s², 11-bit 최소 |
| 최대 가속도 | `HYUNDAI_CANFD_LONG_LIMITS.max_accel` | 1024 | 0.01 m/s² 단위, +10.24 m/s², 11-bit 최대 |
| SCC 가속도 검사 | `aReqRaw`, `aReqValue` | 각각 −10.23~+10.24 m/s² | 표현 가능한 값은 모두 통과 |

사용자 결정에 따라 상위 제어기가 모든 shaping을 맡으며, 위 값은
`patches/opendbc-local-safety.patch`에 있습니다. Panda 조향 검사에는 ±1023 범위, 채널 허가,
허가 없음 시 토크 0, 요청 비트 조건만 있고, 가속도는 11-bit 표현 범위 전체를 허용합니다.
`test_hyundai_canfd.py`의 범용 조향/가속도 테스트 중 표현 범위 밖 값을 보내거나 범위 제한을
전제하는 항목은 이 동작에 맞게 재정의했습니다.
이 patch의 테스트 실행, ARM 빌드, flash, 실차 수용은 확인되지 않았습니다.
Panda는 검사를 통과하지 못한 프레임을 거부합니다. 종방향을 지원하는 프로파일에서 종방향 채널
허가가 꺼져 있으면 가속도 두 필드와 `ACCMode`가 모두 0이어야 합니다.

#### `StrTqReqVal` 표현 범위와 현재 ROS 제한 위치

`StrTqReqVal`은 부호 없는 11-bit 원시값에 −1024 offset을 적용합니다.
따라서 `raw=0..2047`, 요청값은 계산상 `−1024..+1023 count`입니다.
DBC의 원시값 표기 `2046=Reserved`, `2047=Invalid`를 적용해 제외하면
정상 코드로 해석할 수 있는 표현 범위는 `−1024..+1021 count`입니다.
Panda의 `max_torque=1023`은 +1022/+1023(raw 2046/2047)도 통과시키지만, host는 EPS 오류를
피하기 위해 이 원시값을 보내지 않습니다. MDPS가 이 원시값을 어떻게 처리하는지는 확인되지 않았습니다.
이 값들도 해당 MDPS가 실제 수용하는 최대 토크를 증명하지는 않습니다.

DBC 주석의 환산은 `요청 추가 토크 = count / 128 Nm`입니다.
−1024는 −8 Nm, +1023은 약 +7.99 Nm 요청에 해당합니다.
+1024는 11-bit 필드에 표현할 수 없으며, 비트폭을 넘어선 값을 단순히 잘라 넣으면
반대 부호로 해석될 수 있습니다. host는 Reserved/Invalid 원시값을 피하도록 ±1021로 clamp합니다.

| ROS 측 위치 | 검사 / 처리 | 현재값 |
| --- | --- | --- |
| [`CommandAdapter` 생성자](../src/command_adapter.cpp) | `max_torque` 0~1021, `torque_output_scale` 1~1021, rate 1~2042, 가속도 −10.23~10.24 밖이면 시작 거부 | 설정값 검사 |
| [`CommandAdapter::update`](../src/command_adapter.cpp) | 토크와 가속도를 설정 범위로 clamp | 범위 밖 명령을 끊지 않고 끝값으로 전달 |
| [`HyundaiCanFdCodec::make_lfa`](../src/hyundai_canfd_codec.cpp) | `torque = std::clamp(torque, -1021, 1021)` | Reserved/Invalid 원시값 제외 |
| [`HyundaiCanFdCodec::make_scc_control`](../src/hyundai_canfd_codec.cpp) | 가속도 −10.23~10.24, jerk 0~12.7, 설정 속도 0~255로 clamp | 예외 없이 CAN 표현 범위로 맞춤 |
| [active YAML](../config/ioniq5_ecan.yaml) | `torque_output_scale`, `max_torque`, `torque_rate_up/down`, `driver_torque_multiplier`, 가속도, jerk | 270, 1021, 2042/2042, 0, −10.23/10.24, 12.7 |
| [노드의 설정 읽기](../src/node.cpp) | YAML에 값이 없을 때의 코드 기본값 | YAML과 같은 값 |

토크 제어기 출력 1.0은 `torque_output_scale` count에 대응하며 Carrot 튜닝 게인을 유지합니다.
`max_torque` 1021은 clamp로만 쓰입니다. rate 2042는 한 프레임에 전체 범위를 이동할 수 있어
host slew가 없고, 배율 0은 host driver torque 제한이 없음을 뜻합니다.
RAW 송신 경로의 별도 허가 조건은 [raw CAN 안내](raw_can.md)를 참고합니다.

### 송신 허용 목록

아래는 현재 설정의 combined I5R1 `7173` 프로파일이며, 종방향 기능이 빌드된 경우의
`HYUNDAI_CANFD_LFA_STEERING_LONG_ECAN_ONLY_TX_MSGS`입니다.
ID, logical bus, byte 수가 함께 일치해야 합니다. 표에 들어 있어도 메시지별 값 검사와
제어 허가 조건은 계속 적용됩니다.

| CAN ID | logical bus | payload 길이 | 역할 | 순정 송신 충돌 검사 `check_relay` |
| --- | --- | --- | --- | --- |
| `0x12A` | 0 | 16 B | 조향 토크 요청 | true |
| `0x1E0` | 0 | 16 B | LFA/HDA 표시 | true |
| `0x1A0` | 0 | 32 B | SCC 요청 | true |
| `0x160` | 0 | 16 B | FCA 관련 프레임 | false |
| `0x730` | 0 | 8 B | ADAS 소유권 대상의 Tester Present | false |
| `0x7D0` | 0 | 8 B | 레이더의 Tester Present | false |
| `0x1CF` | 2 | 8 B | 기존 크루즈 버튼 송신 항목 | false |

bus 2 버튼 항목은 배열에 남아 있지만 현재 ECAN-only 구성은 해당 물리 트랜시버를
비활성화하므로 사용하지 않습니다. bus 0 버튼 송신이 허용된다는 의미가 아닙니다.
Hyundai 제어 모드에서 `0x730`/`0x7D0`의 허용 진단 payload는
`02 3E 80 00 00 00 00 00`입니다. 다른 진단 서비스는 별도 진단 모드의 검사를 따릅니다.
종방향 기능을 끈 lateral-only 프로파일에서는 `0x160`과 `0x7D0`이 위 목록에서 빠집니다.

### 수신 검사와 시간 조건

현재 EV, 기본 버튼 설정의 필수 수신 그룹은 `0x035`, `0x175`, `0x0A0`, `0x0EA`,
`0x1CF`입니다. 페달 그룹에는 다른 파워트레인용 `0x100`/`0x105` 대체 정의도 있습니다.

| CAN ID | logical bus | payload 길이 | 기대 수신 주기 | 순환번호 범위 | CRC 검사 |
| --- | --- | --- | --- | --- | --- |
| `0x035` | 0 | 32 B | 100 Hz, 10 ms | 0~255 | 사용 |
| `0x175` | 0 | 24 B | 50 Hz, 20 ms | 0~255 | 사용 |
| `0x0A0` | 0 | 24 B | 100 Hz, 10 ms | 0~255 | 사용 |
| `0x0EA` | 0 | 24 B | 100 Hz, 10 ms | 0~255 | 사용 |
| `0x1CF` | 0 | 8 B | 50 Hz, 20 ms | 0~15 | 생략 |
| `0x1AA` | 0 | 16 B | 50 Hz, 20 ms | 0~255 | 생략, alternate 버튼 설정 시 `0x1CF` 대체 |
| `0x1A0` | 0 | 32 B | 50 Hz, 20 ms | 0~255 | 사용, 종방향 기능이 꺼진 radar-SCC 프로파일의 추가 검사 |

위 수신 정의는 `ignore_quality_flag=true`이므로 메시지의 품질 비트를 별도로 검사하지
않습니다. I5R1은 각 정의에 따른 유효성 결과와 수신 시각을 확인합니다.

| 항목 | 정의 위치 | 현재값 / 범위 | 의미 |
| --- | --- | --- | --- |
| I5R1 수신 freshness | `ioniq5_session_ready` | 100000 µs, 100 ms 이내 | 검사 대상 모두 새 정상 프레임을 수신해야 ready/TX 가능 |
| 초기 미수신 대기 | `ioniq5_session_rx_initializing` | 최대 100000 µs, 100 ms | 아직 받지 않은 메시지에만 적용, 출력 허가나 불량 프레임 면제 없음 |
| 순환번호 오류 점수 | `MAX_WRONG_COUNTERS` | 5, 점수 0~5 | 점수 5에서 불량 판정, 잘못된 번호는 +1, 정상 번호는 −1 |
| 공통 누락 감시 | `MAX_MISSED_MSGS` | 10 | 임계 시간은 `max(10 × 기대 주기, 1초)`, 현재 표의 50/100 Hz에서는 1초 |
| 운전자 토크 샘플 수 | `MAX_SAMPLE_VALS` | 6 | 최근 샘플의 최소/최대값을 토크 제한 계산에 사용 |

공통 누락 감시와 I5R1 ready 검사는 별개입니다. 현재 프로파일은 공통 감시의 1초보다
엄격한 100 ms 수신 조건을 적용하므로 공통 임계값만 보고 송신 가능 여부를 판단하지 않습니다.

### Heartbeat, 모드, 제어 허가

| 항목 | 변수 / 정의 | 현재값 / 조건 |
| --- | --- | --- |
| 시동 ON heartbeat 임계값 | `HEARTBEAT_IGNITION_CNT_ON` | 5, 1 Hz tick에서 판단하는 약 5초 기준 |
| 시동 OFF heartbeat 임계값 | `HEARTBEAT_IGNITION_CNT_OFF` | 2, 약 2초 기준 |
| 제어 활성 상태 불일치 | `heartbeat_engaged_mismatches` 검사 | 불일치 점수 3에서 허가 해제와 I5R1 세션 취소 |
| MCU heartbeat-loop watchdog | `simple_watchdog_init` | 375000 µs, 375 ms, 8 Hz loop에서 갱신 |
| SILENT 모드 | `SAFETY_SILENT` | 0 |
| 진단 모드 | `SAFETY_ELM327` | 3 |
| 무출력 모드 | `SAFETY_NOOUTPUT` | 19 |
| Hyundai CAN-FD 모드 | `SAFETY_HYUNDAI_CANFD` | 28 |
| I5R1 조향 전용 파라미터 | EV + split + ECAN-only + I5R1 | 7169 = 1 + 1024 + 2048 + 4096 |
| I5R1 combined 파라미터 | 위 구성 + longitudinal | 7173 = 7169 + 4, 현재 active YAML 구성 |
| alternate 버튼 옵션 | `HYUNDAI_PARAM_CANFD_ALT_BUTTONS` | 32 추가, 현재 false |
| 종방향 빌드 조건 | `ALLOW_DEBUG` | 정의된 빌드에서 longitudinal bit 4 적용 |
| 하네스 방향 | `HARNESS_STATUS_NORMAL` | 1일 때 ECAN-only Hyundai 모드 허용 |
| 차량 CAN 경로 | ECAN-only 트랜시버 구성 | physical CAN1 / logical bus 0, 다른 트랜시버 비활성 |
| CAN forwarding | `disable_forwarding` | ECAN-only에서 true, 양방향 forwarding 차단 |
| 채널 선택 | LDA / SET | LDA는 조향 전용, SET release는 조향+종방향 토글 |
| 브레이크 개입 | split-channel 처리 | 종방향 해제 래치, 조향 유지, 다음 SET 조작 필요 |
| CANCEL 개입 | `ioniq5_session_abort` | I5R1 세션과 두 채널 허가 취소 |
| 세션 조회 요청 | USB `0xB7` | magic `0x49355231`, 세션 상태 읽기 |
| 종방향 해제 요청 | USB `0xB8` | `param1=1`이면 종방향 허가 해제, 허가 생성 기능 없음 |

`controls_allowed`의 현재 장치값은 이 문서에서 조회하지 않았습니다. 코드상으로는
Hyundai 모드, 정상 수신, 차단되지 않은 세션과 선택된 채널을 함께 만족해야 true가 됩니다.
Heartbeat가 없으면 임계값에 따라 SILENT로 전환합니다. 이 시간 조건은 ROS 명령의
250 ms timeout이나 Panda health의 500 ms timeout과 구분합니다.

## 추가 변경분을 패치로 추출

로컬 소스 파일을 수정한 뒤 아래 명령을 실행합니다.

```powershell
& 'C:\Program Files\Git\bin\bash.exe' --noprofile --norc `
  scripts/export_panda_safety_edits.sh
```

추출 스크립트는 고정 버전 소스에 기존 패치 5개를 적용한 상태와 현재 소스를 비교합니다.
임시 Git 인덱스를 사용하므로 두 소스 저장소의 실제 인덱스와 staging 상태를 유지합니다.
내부 Git tree 참조로 기준점을 보존해 Git 객체 정리 후에도 비교할 수 있습니다.
새로 추가한 소스 파일도 Git에서 제외되지 않았다면 패치에 포함됩니다.

| 수정한 소스 저장소 | 생성되는 프로젝트 패치 |
| --- | --- |
| opendbc | `patches/opendbc-local-safety.patch` |
| Panda | `patches/panda-local-safety.patch` |

위 두 패치 파일은 추출 스크립트가 관리합니다. 다시 추출하면 현재 추가 변경분으로 갱신합니다.
소스의 추가 변경을 모두 되돌리면 해당 패치 파일도 삭제합니다.
빌드 스크립트는 기존 프로젝트 패치 뒤에 내용이 있는 로컬 패치를 적용합니다.
커밋 전에 생성된 패치의 diff를 확인합니다. 기존 프로젝트 패치 5개는 그대로 보존합니다.

## 빌드와 확인 범위

소스 준비와 패치 추출은 이 컴퓨터에서 수행할 수 있습니다.
Windows에서 ARM 펌웨어를 실제로 빌드하려면 Python/SCons와 도구 모음이 추가로 필요합니다.
자세한 요구사항은 [펌웨어 안내](panda_firmware.md)를 참고합니다.
로컬 편집 환경 준비만으로 빌드 성공이나 차량 호환성이 확인되는 것은 아닙니다.
펌웨어 빌드와 장치 flash는 별도 단계입니다.

ROS adapter와 CAN codec에도 명령 제한이 있습니다. 펌웨어 제한을 변경하면 호스트 제한도
함께 대조해야 Panda가 거부하는 명령을 반복해서 보내는 상황을 방지할 수 있습니다.
차량 ECU의 명령 수용과 실제 출력은 소프트웨어의 제한값과 별도로 확인해야 합니다.

2026-10-08 확인 기록: 고정 버전 소스와 기존 패치 준비, Bash 구문 검사를 완료했습니다.
추가 변경이 없을 때 튜닝 패치가 생성되지 않는 것을 확인했습니다. 임시 주석을 추가했을 때는
그 변경만 추출됐고, 패치를 역방향으로 적용할 수 있는지도 확인했습니다.
검사 후 소스를 복원했으며 파일과 실제 Git 인덱스의 hash가 원래 값과 일치했습니다.
이 준비 과정에서 ARM 빌드나 하드웨어 작업은 수행하지 않았습니다.
