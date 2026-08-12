# Pinned Panda firmware

종방향 Hyundai safety flag는 Panda의 `ALLOW_DEBUG` 빌드에서만 적용됩니다. release
firmware에서 param 13을 설정하면 LONG bit가 무시됩니다. 드라이버는 시작 시 zero-accel
비활성 SCC 프레임으로 이 능력을 검사하고, Panda가 차단하면 `NO_OUTPUT`으로 돌아가
arm을 실패시킵니다.

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

스크립트는 사용자 cache에 두 저장소를 정확한 SHA로 checkout하고 `RELEASE`를 제거한
DEBUG firmware를 빌드합니다. 출력 경로와 SHA-256을 기록해 시험 로그에 보관하십시오.

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

플래시 후 노드를 `allow_actuation: false`로 시작해 protocol hash, hardware type,
harness 상태와 CAN RX만 먼저 확인합니다.
