# Pinned Panda firmware

2026-10-07 문서 기준의 build/ABI 요구사항입니다. 빌드 결과, 플래시 이력과 날짜별 USB 관측은
별도 사실이며 설치 image의 정확한 provenance는 [인수인계](vehicle_handoff.md)에 있습니다.

종방향 Hyundai safety flag는 Panda의 `ALLOW_DEBUG` 빌드에서만 적용됩니다. release
firmware에서 param `3077` 또는 I5R1 `7173`을 설정하면 LONG bit가 무시됩니다. 드라이버는 시작 시 zero-accel
비활성 SCC 프레임으로 이 능력을 검사하고, Panda가 차단하면 `NO_OUTPUT`으로 돌아가
arm을 실패시킵니다.

HDA1 종방향 실차 시험은 카메라 ECU `0x730`의 통신을 끊어 순정 LFA `0x12A`를
정지시키는 것만으로는 충분하지 않습니다. 레이더 ECU `0x7D0`도 별도로 통신 비활성화하여
순정 `SCC_CONTROL (0x1A0)`이 멈춘 것을 확인한 뒤 제어를 시작합니다. 두 ECU에는 시험 중
각각 tester-present를 보내며, 종료 시 레이더와 카메라 순서로 통신을 복구합니다.
두 ECU의 통신을 모두 복구한 다음 순정 SCC/LFA의 새 유효 프레임을 확인합니다.

이 저장소는 고정 opendbc에 opt-in safety param bit `1024`와 ECAN-only bit `2048`을
추가합니다. `1024`가 있을 때 firmware가 횡/종방향 허가를 별도로 추적합니다. LDA는
조향 전용 모드, SET release는 조향+종방향 모드를 토글하며 brake는 종방향 허가만
래치 해제합니다. 따라서 brake 중에도 LFA용 전역 `controls_allowed`가 유지됩니다.
`2048`은 Panda forwarding을 양방향 차단하고 firmware가 physical CAN1(논리 ECAN 0)을
제외한 transceiver를 끕니다. 활성 safety mode는 정확히 `harness_status=1`일 때만 허용됩니다.
따라서 upstream stock image가 아니라 아래 스크립트가 만든
`IONIQ5ECAN-dd8a5b3d-DEBUG` image가 필요합니다.

## 고정 버전

- panda: `dd8a5b3df77706337a11555377e7180c5adc8726`
- opendbc: `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`
- expected health packet hash: `0x290DAE03`
- expected CAN packet hash: `0x75ABF276`

USB 연결 단계에서 firmware marker와 packet ABI를 검사합니다. marker가 같더라도 패치
revision이나 설치 binary가 같다는 보장은 없습니다. 실제 설치 hash는 플래시 및 signature
기록으로 별도 확인해야 합니다. 로컬 host의 3초 일시 EPS 복귀는 firmware 변경 없이
구현했지만 채널 분리 동작에는 대응하는 split-brake firmware가 필요합니다.

## 빌드

### I5R1 command-session 확장

`opendbc-command-session.patch`는 opt-in param `4096`을 추가합니다. 현재 active YAML의
combined param은 `7173`입니다. 같은 param을 NO_OUTPUT와 ELM327에도 사용하여 정상
입력 단절 동안 물리 버튼 선택과 brake latch를 RAM에만 유지합니다. 두 모드에서는
actuator 프레임이 금지되고 stock 통신을 복구합니다. Hyundai 모드로 돌아갈 때는 모든
필수 CAN의 새 checksum/counter/freshness 검증 전까지 actuator TX를 금지합니다.
SILENT/일반 NO_OUTPUT/프로세스 시작, CAN 고장 및 heartbeat 불일치는 선택을 지웁니다.
후속 `opendbc-command-session-startup.patch`는 모드 전환 직후 첫 RX보다 안전 tick이
먼저 실행되는 경쟁 조건을 수정합니다. 아직 수신하지 않은 메시지에만 최대 100 ms 초기
대기를 적용하며, ready/TX는 여전히 모든 필수 CAN의 새 정상 수신을 요구합니다.
수신된 불량 CAN이나 100 ms 이후 미수신은 이 예외의 대상이 아닙니다.
CANCEL도 세션을 취소합니다. USB `0xB7`은 magic `0x49355231`과 세션 상태를 읽기만 하며,
`0xB8`은 종방향 허가를 지울 수만 있고 허가를 새로 만들 수 없습니다.

firmware marker와 기존 health/CAN packet ABI는 그대로 유지되므로 marker만으로 I5R1
설치를 판단하면 안 됩니다. host는 capability를 확인하고, flash helper는 signature와
capability를 모두 재검증합니다. 실차 stock 복구/주행 중 재인계 결과는 오프라인 검사와 별개입니다.

Ubuntu 20.04 기본 Python은 너무 오래되므로 `uv`가 관리하는 Python 3.11 환경을
사용합니다. `uv` 설치 후:

```bash
./scripts/build_panda_debug_firmware.sh
```

스크립트는 기본적으로 프로젝트 내부 `.firmware-build`에 두 저장소를 정확한 SHA로 checkout하고 저장소의 opendbc
split-button/forwarding 및 I5R1 command-session patch, Panda ECAN-only patch와 builder-marker patch를
idempotent하게 적용합니다. 그 뒤 `RELEASE`와 ambient `DEBUG` 환경 변수를 제거하고
`PANDA_BUILDER=IONIQ5ECAN`으로 `ALLOW_DEBUG` bootstub과 firmware를 빌드합니다. 두 출력 및
다섯 patch의 SHA-256을 시험 로그에 보관하십시오. UV/Python/임시 cache도 빌드 폴더 안에 둡니다.
이 컴퓨터의 빌드 경로는 `/home/ave/catkin_ws_ioniq5/third_party/ecan_firmware`이며 첫 인자로
명시해 재사용할 수 있습니다. 시스템 Python/ROS 또는 다른 작업공간은 변경하지 않습니다.

기존 RELEASE bootstub은 debug key로 서명된 앱을 거부할 수 있습니다. 플래시 후 Panda가
`PID_DDEE` bootstub에 남으면 앱을 반복해서 쓰지 말고, 차량과 분리된 상태에서 공식 Panda
DFU recovery 절차로 같은 빌드의 `bootstub.panda_h7.bin`을 먼저 설치해야 합니다.

Windows에서 STM32 DFU 장치가 Code 28로 표시되면 [공식 Zadig](https://zadig.akeo.ie/)로
USB ID가 정확히 `0483:DF11`인 `DFU in FS Mode`에만 WinUSB를 설치합니다. 다른 USB 장치의
드라이버를 교체하지 마십시오. Ubuntu에서는 udev rule 적용 후 libusb가 DFU를 직접 엽니다.

DFU 복구는 boot sector 0과 1을 지우므로 정상 앱 모드에서는 실행하지 않습니다. 일반 Panda
serial과 그 serial에서 계산되는 DFU serial이 일치해야만 helper가 진행합니다.

```bash
python3 scripts/recover_panda.py \
  --serial RED_PANDA_SERIAL \
  --dfu-serial STM32_DFU_SERIAL \
  --confirm STM32_DFU_SERIAL \
  --bootstub /home/ave/catkin_ws_ioniq5/third_party/ecan_firmware/panda/board/obj/bootstub.panda_h7.bin
```

## 플래시

플래시는 Panda firmware를 덮어쓰는 명시적 위험 작업입니다. 차량에서 분리하고 안정된
USB 전원에서, serial을 두 번 입력해야만 helper가 실행됩니다.

```bash
source /home/ave/catkin_ws_ioniq5/third_party/ecan_firmware/venv/bin/activate
python3 scripts/flash_panda.py \
  --serial RED_PANDA_SERIAL \
  --confirm RED_PANDA_SERIAL \
  --firmware /home/ave/catkin_ws_ioniq5/third_party/ecan_firmware/panda/board/obj/panda_h7.bin.signed
```

helper는 앱에서 bootstub으로 전환한 뒤 새로 USB interface를 claim하므로 Windows WinUSB에서도
bulk flash가 가능합니다. 기존 고정 `DEV-dd8a5b3d-DEBUG` 또는
`IONIQ5-dd8a5b3d-DEBUG`, 그리고 새 `IONIQ5ECAN-dd8a5b3d-DEBUG`
bootstub만 진입점으로 허용하고, 앱 image에는 반드시 `IONIQ5ECAN-dd8a5b3d-DEBUG` marker가
있어야 합니다. 플래시 후 version과 signature를 다시
검증하며 RELEASE 또는 다른 commit의 bootstub이면 앱 영역을 지우기 전에 중단합니다.

플래시 후 read-only preflight로 ABI를 확인하고, 차량 연결 시
`config/ioniq5_ecan_passive.yaml`로 시작해 hardware/harness와 RX를 확인합니다.
launch 기본값은 actuation 연구장 YAML이므로 passive 설정을 명시해야 합니다.

차량과 분리된 상태에서 다음 read-only 검사 결과가 `PREFLIGHT PASS`인지 먼저 확인합니다.

```bash
python3 scripts/panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only --require-command-session
```

이 검사는 application PID와 정확한 `IONIQ5ECAN-dd8a5b3d-DEBUG` 문자열, 두 packet hash,
Red Panda hardware type, health ABI, fault/overflow 및 ECAN controller index 0 상태를
확인합니다. Panda에 control write나 CAN frame을 보내지 않습니다. 차량 하네스 연결 후에는
`--require-harness`를 추가해 `harness_status=1`과 ECAN RX 증가까지 확인합니다.
여기서 controller index 0은 0부터 세는 firmware index이며 차량 physical CAN1 및
Panda logical bus 0에 대응합니다.

## 날짜별 firmware 기록

- 2026-08-21 마지막 기록된 앱 flash SHA-256:
  `1ef6d981457e8b2018fcbf72b8a43f88ba85f95a6b94800b177e8302df212bae`.
  당시 bootstub은 플래시하지 않았습니다.
- 같은 날짜의 split-brake signed 후보:
  `ffe0428536394ee7743a37f10338b4a19668f343f742e1b5de25c43df43f6986`.
  ARM GCC `-Werror` 빌드는 통과했지만 후보 flash는 기록되지 않았습니다.
- 당시 bootstub 후보:
  `b420ab8d7d10d85a4b6883e37f6e4cbf38b724776afde823e7a9f4d5e1463a08`.
- split patch SHA-256:
  `e69f5a71f2a53ada43a38c1aefa759855b9915659d5ecb28682ad2311c910d54`.
- 2026-10-06 USB read snapshot: Red Panda application mode,
  `IONIQ5ECAN-dd8a5b3d-DEBUG`, 위 packet hashes, harness/ignition 0,
  safety mode/param 0, controls/faults 0. 정확한 설치 binary hash는 읽지 않았고
  전체 preflight PASS 판정도 실행하지 않았습니다.

위 초기 USB snapshot만으로 당시 split-brake 후보의 설치 여부를 확정할 수 없습니다.
이후 사용자가 I5R1 firmware 수정/flash를 승인하고 차량 하네스를 분리했습니다.

- 2026-10-06 I5R1 ARM DEBUG 빌드 signed 앱 SHA-256:
  `fbfcae2ee11daa9bdd38e407183aeb5de755f61e2d0c79b2373372b42c52cc7b`.
- 별도 앱 flash 완료: 장치 일련번호는 공개 기록에서 제외했으며, 하네스/ignition 0 확인 후 앱 영역만
  교체했고 `Panda.up_to_date` 서명 비교와 `0xB7` capability를 재검증했습니다.
  이전 앱 binary의 정확한 hash는 확보하지 않았습니다.
- bootstub 후보 SHA-256:
  `644ef25f217ffd899ef008d7d876ba67a4666d1a311b411ef326b57c77dfc3e2`.
  기존 호환 DEBUG bootstub을 그대로 사용했으며 이 후보는 flash하지 않았습니다.
- 고정 Hyundai safety/custom test: 995개 실행, 117개 skip, 878개 통과. custom 9개 전부 통과.
- USB-only preflight PASS, NO_OUTPUT 7173에서 3개 송신 시도 차단, 실제 CAN TX 증가 0.

네 patch hash 및 관측 범위는 [USB-only 기록](evidence/2026-10-06/command-session-usb-20261006.json)에
있습니다. 이는 초기 설치 이력이며, 이후 시험과 현재 수정 앱은 아래 기록으로 구분합니다.

2026-10-06 정차 인계 실패 뒤 초기 RX 경쟁 조건 수정 앱을 USB-only로 별도 플래시했습니다.
당시 설치에서 검증한 signed 앱 SHA-256은 `6c4e4b388642911a6329690d7d917feceda618fa84255f5fe13e1d288d092d52`입니다.
추가 patch hash, 설치 서명/capability, 관련 14개 safety test, preflight PASS와 standby 송신
3개 차단/물리 TX 0은 [복구 수정 기록](evidence/2026-10-06/ecan-recovery-fix-20261006.json)에 있습니다.
bootstub은 변경하지 않았으며 수정 후 실차 ACTIVE/주행 재인계는 아직 확인하지 않았습니다.
10월 7일 CAN 분석과 문서 갱신에서는 장치 연결/설치 image를 재조회하거나 firmware를 변경하지 않았습니다.
