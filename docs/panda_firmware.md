# Pinned Panda firmware

종방향 Hyundai safety flag는 Panda의 `ALLOW_DEBUG` 빌드에서만 적용됩니다. release
firmware에서 param `3077`을 설정하면 LONG bit가 무시됩니다. 드라이버는 시작 시 zero-accel
비활성 SCC 프레임으로 이 능력을 검사하고, Panda가 차단하면 `NO_OUTPUT`으로 돌아가
arm을 실패시킵니다.

이 저장소는 고정 opendbc에 opt-in safety param bit `1024`와 ECAN-only bit `2048`을
추가합니다. `1024`가 있을 때
차선유지(LDA) 버튼의 상승 에지도 Panda의 전역 `controls_allowed`를 허용하며, host가 LDA의
조향 전용 모드와 SET의 조향+종방향 모드를 게이트합니다.
`2048`은 Panda forwarding을 양방향 차단하고 firmware가 physical CAN1(논리 ECAN 0)을
제외한 transceiver를 끕니다. 활성 safety mode는 정확히 `harness_status=1`일 때만 허용됩니다.
따라서 upstream stock image가 아니라 아래 스크립트가 만든
`IONIQ5ECAN-dd8a5b3d-DEBUG` image가 필요합니다.

## 고정 버전

- panda: `dd8a5b3df77706337a11555377e7180c5adc8726`
- opendbc: `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`
- expected health packet hash: `0x290DAE03`
- expected CAN packet hash: `0x75ABF276`

다른 firmware는 USB 연결 단계에서 거부됩니다. 이 고정은 ABI와 safety semantics가
조용히 바뀌는 것을 막기 위한 의도적인 제한입니다.

## 빌드

Ubuntu 20.04 기본 Python은 너무 오래되므로 `uv`가 관리하는 Python 3.11 환경을
사용합니다. `uv` 설치 후:

```bash
./scripts/build_panda_debug_firmware.sh
```

스크립트는 사용자 cache에 두 저장소를 정확한 SHA로 checkout하고 저장소의 opendbc
split-button/forwarding patch, Panda ECAN-only transceiver patch와 builder-marker patch를
idempotent하게 적용합니다. 그 뒤 `RELEASE`와 ambient `DEBUG` 환경 변수를 제거하고
`PANDA_BUILDER=IONIQ5ECAN`으로 `ALLOW_DEBUG` bootstub과 firmware를 빌드합니다. 두 출력 및
세 patch의 SHA-256을 시험 로그에 보관하십시오.

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
  --bootstub ~/.cache/ioniq5_ecan/upstream/panda/board/obj/bootstub.panda_h7.bin
```

## 플래시

플래시는 Panda firmware를 덮어쓰는 명시적 위험 작업입니다. 차량에서 분리하고 안정된
USB 전원에서, serial을 두 번 입력해야만 helper가 실행됩니다.

```bash
source ~/.cache/ioniq5_ecan/upstream/venv/bin/activate
python3 scripts/flash_panda.py \
  --serial RED_PANDA_SERIAL \
  --confirm RED_PANDA_SERIAL \
  --firmware ~/.cache/ioniq5_ecan/upstream/panda/board/obj/panda_h7.bin.signed
```

helper는 앱에서 bootstub으로 전환한 뒤 새로 USB interface를 claim하므로 Windows WinUSB에서도
bulk flash가 가능합니다. 기존 고정 `DEV-dd8a5b3d-DEBUG` 또는
`IONIQ5-dd8a5b3d-DEBUG`, 그리고 새 `IONIQ5ECAN-dd8a5b3d-DEBUG`
bootstub만 진입점으로 허용하고, 앱 image에는 반드시 `IONIQ5ECAN-dd8a5b3d-DEBUG` marker가
있어야 합니다. 플래시 후 version과 signature를 다시
검증하며 RELEASE 또는 다른 commit의 bootstub이면 앱 영역을 지우기 전에 중단합니다.

플래시 후 노드를 `allow_actuation: false`로 시작해 protocol hash, hardware type,
harness 상태와 CAN RX만 먼저 확인합니다.

차량과 분리된 상태에서 다음 read-only 검사 결과가 `PREFLIGHT PASS`인지 먼저 확인합니다.

```bash
python3 scripts/panda_preflight.py --serial RED_PANDA_SERIAL --ecan-only
```

이 검사는 application PID와 정확한 `IONIQ5ECAN-dd8a5b3d-DEBUG` 문자열, 두 packet hash,
Red Panda hardware type, health ABI, fault/overflow 및 ECAN physical controller 0 상태를
확인합니다. Panda에 control write나 CAN frame을 보내지 않습니다. 차량 하네스 연결 후에는
`--require-harness`를 추가해 `harness_status=1`과 ECAN RX 증가까지 확인합니다.

2026-08-21 Windows ARM GCC 13.2 재현 빌드 및 실제 Red Panda 앱 플래시에서 확인한 signed app
SHA-256은 `cc5a34e8149327d571ad03db9169a584759e8e908d98416efd6f8ab7e26a3e6e`입니다.
부트스텁은 플래시하지 않았습니다.
