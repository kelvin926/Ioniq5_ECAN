# 수신 전용 ROS CAN 기록기

Ubuntu 20.04 / ROS 1 Noetic용 독립 노드입니다. 제어 노드를 실행하지 않고 Panda에서 받은
RAW CAN 프레임을 CSV로 저장하며, 선택적으로 `/ioniq5/can_logger/rx`에 발행합니다.
CAN ID별 선택/DBC 해석 없이 최대 64-byte payload와 CAN-FD/IDE/returned/rejected 플래그를
보존합니다. 기본 수집 대상은 ECAN logical bus 0입니다.

## 송신과 설정 변경 없이 연결

`scripts/can_logger.py`는 raw libusb의 vendor IN/status read와 CAN bulk IN만 사용합니다.
`Panda()` 생성자나 기존 C++ `PandaUsb::connect()`를 사용하지 않습니다. CAN TX, ECU UDS,
heartbeat, bitrate/safety/power-save 변경, CAN/USB reset, firmware flash API가 없습니다.

Panda가 **SILENT mode 0 / controls_allowed=0**인 상태에서만 시작하고 주기적으로 상태를
확인합니다. 앱 부팅 기본 상태가 SILENT이며 이 노드는 그 모드를 강제로 만들지 않습니다.
다른 프로그램이 NO_OUTPUT/Hyundai mode를 유지하고 있으면 닫힌 뒤 firmware의 heartbeat
fallback을 기다리거나 별도 상태 확인이 필요합니다. 기다리는 동안에도 설정을 바꾸지
않습니다. Cabana, pandad와 제어 노드가 같은 USB interface를 동시에 점유할 수 없습니다.

초기 USB stream은 성공한 short transfer까지 **IN read만으로** 비웁니다. 그 전의 backlog는
기록하지 않고 버린 byte 수를 metadata에 남깁니다. Timeout의 partial bytes는 유효한 수신
데이터로 보존합니다. 기록 중 USB packet checksum 오류는 reset/resync로 덮지 않고 종료합니다.
이 검사는 USB packet 경계/무결성이며, 개별 차량 메시지의 application CRC/신호 값 필터가 아닙니다.

## 빌드와 실행

기존 Noetic catkin workspace에서 `catkin_make -DCMAKE_BUILD_TYPE=Release`로 메시지와
Python 실행 래퍼를 생성합니다. `rospy`, `python3-rospkg`, `libusb1` Python 모듈이 필요합니다.
기존 preflight 환경에 `usb1`이 있다면 같은 모듈을 사용하며, Panda/opendbc Python checkout은
필요하지 않습니다. 이번 구현에서 시스템 패키지를 설치하거나 firmware를 변경하지 않았습니다.

아래 실행기는 `/opt/ros/noetic`과 이 패키지가 있는 catkin workspace의 환경을 자기 프로세스에
적용하고, **roslaunch 부모와 자식의 ROS 로그를 모두 패키지 `log/` 안에** 둡니다.
전역 alias/`.bashrc`를 수정하지 않습니다. 소스 checkout에서 실행합니다.

```bash
./scripts/start_can_logger.sh
# 저장만 수행하고 ROS 토픽 발행 생략
./scripts/start_can_logger.sh publish:=false
# 60초 기록 후 종료
./scripts/start_can_logger.sh duration_s:=60
```

Panda가 없거나 읽기 전용 시작 조건을 충족하지 못하면 vendor IN 확인만 재시도합니다.
한 번 기록을 시작한 뒤 USB/CAN stream 오류나 mode 변화가 생기면 해당 기록을 종료합니다.
그런 종료 뒤에는 원인을 확인하고 다시 실행합니다. Ctrl+C는 flush/close 후 종료합니다.

| 옵션 | 기본값 | 의미 |
| --- | --- | --- |
| `output_dir` | 패키지 `log/can_logger` | CSV 및 metadata 저장 위치 |
| `serial` | 빈 문자열 | Panda 한 대일 때 자동 선택. 여러 대면 실제 값을 로컬에서 지정 |
| `bus` | 0 | logical bus. `-1`이면 들어온 모든 bus를 저장; 새 transceiver를 켜지는 않음 |
| `publish` | true | ROS RX 토픽 발행 |
| `topic` | `/ioniq5/can_logger/rx` | `ioniq5_ecan/RawCanFrame` 토픽 |
| `duration_s` | 0 | 0은 계속 기록, 양수는 시작 후 지정 초 동안 기록 |
| `fsync` | false | true면 주기적 flush에 disk sync 추가 |
| `logger_name` | `ecan_can_logger` | 노드 이름. 센서 실행기는 `sensor_suite_ecan_logger`로 소유권 구분 |

기존 `sensors_all`, `sensors_bag`, `sensors_check` 연결은 [센서 통합](sensor_suite.md)을 참고합니다.
대시보드는 RAW ROS 토픽만 구독하며 제어 노드나 별도 Panda 연결을 만들지 않습니다.

실제 장치 일련번호와 개인 경로는 Git에 넣지 않습니다. 기본 출력 폴더는 Git에서 제외됩니다.
원본 CAN payload에 차량 식별정보 등이 있을 수 있으므로 저장 파일을 공개하기 전에는 따로
검토합니다. 실제 수신 payload 자체는 이 기록기가 익명화하거나 바꾸지 않습니다.

## 파일과 시각의 의미

파일 이름은 `can_YYYYMMDDTHHMMSS_microsecondsZ.csv`, sidecar는 같은 이름의 `.meta.json`입니다.
기존 파일을 덮어쓰지 않습니다. CSV 한 행은 재조립된 RAW 프레임 한 개입니다.

| 열 | 의미 |
| --- | --- |
| `sequence` | 파일 안의 1부터 시작하는 순번 |
| `timestamp_utc` / `unix_time_ns` | host USB read 완료 시각, UTC 문자열 / Unix epoch ns |
| `monotonic_time_ns` | 같은 host 수신 시점의 monotonic ns, 시스템 시각 변경과 구분 |
| `ros_time_ns` | 같은 시점의 ROS clock. `/use_sim_time` 환경에서는 Unix clock과 다를 수 있음 |
| `bus`, `address_hex` | logical bus / CAN ID |
| `fd`, `extended`, `returned`, `rejected` | CAN-FD/IDE 및 Panda 송신 결과 metadata 플래그 |
| `length`, `data_hex` | payload byte 수 / 원본 bytes의 hex 표현 |

**Panda의 고정 USB CAN packet에는 하드웨어 도착 timestamp가 없습니다.** 시각은 ECU가
측정/송신한 시각이 아니라 PC가 USB 데이터를 읽은 시각입니다. 한 USB read에서 완성된
프레임은 같은 시각을 공유하고, 분할 프레임은 완성된 read 시각을 씁니다. USB queue 및 read
timeout의 지연이 포함될 수 있으며 ns 표기가 ns 정확도를 보장하지 않습니다.

토픽 `stamp`도 이 host 수신 시점의 ROS clock입니다. 기존 제어 노드의 RAW 토픽 `stamp`는
발행 시각이므로 발행 주체와 의미를 구분합니다. 차량 ECU에서 온 프레임은
`returned=false`, `rejected=false`이고, true인 프레임은 Panda metadata입니다.

metadata에는 packet ABI, 시작/마지막 관측 health/CAN counter, 시작 backlog 폐기량,
프레임 수와 종료 상태를 남기며 실제 장치 일련번호는 넣지 않습니다. 누적 RX overflow와
관측 중 증가는 다릅니다. 기록 중 증가하면 경고하며 무손실 수집으로 해석하지 않습니다.

기본 CSV flush 간격은 1초입니다. 차량 전원과 PC가 함께 꺼지면 마지막 버퍼/OS cache가
유실될 수 있습니다. `fsync:=true`는 주기적으로 disk sync하지만 전원 손실 전체를 보장하는
기능은 아닙니다. 토픽 queue의 손실 여부도 disk 기록과 별개입니다.
Metadata의 최종 프레임 수/종료 상태는 정상 close 때 갱신합니다. 강제 전원 차단 후
`status=recording`이 남았다면 CSV 행 자체를 확인하며 정상 종료 기록으로 취급하지 않습니다.

## 확인 범위

모의 USB를 사용한 수신 전용/모드 거부, 부분 timeout, 분할 packet, checksum 실패,
64-byte CAN-FD/extended/flags와 CSV ns round-trip 테스트 6개가 통과했습니다.
Windows에서 실제 ROS/차량/Panda 실행은 하지 않았으며 Noetic launch와 실차 기록은
차량 컴퓨터에서 확인해야 합니다. 이 노드의 추가로 기존 제어 노드 동작은 바꾸지 않았습니다.
