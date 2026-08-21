#!/usr/bin/env python3
import argparse
import hashlib
import re
import time
from pathlib import Path

from panda import McuType, Panda, PandaDFU


PINNED_VERSION = "IONIQ5ECAN-dd8a5b3d-DEBUG"


def _wait_for_bootstub(serial: str, timeout_s: float = 15.0) -> Panda:
    deadline = time.monotonic() + timeout_s
    last_error = "device did not appear"
    while time.monotonic() < deadline:
        panda = None
        try:
            panda = Panda(serial=serial, cli=False)
            if panda.bootstub:
                return panda
            last_error = "device entered application mode instead of bootstub"
        except Exception as exc:  # USB disappears briefly during reset.
            last_error = str(exc)
        finally:
            if panda is not None and not panda.bootstub:
                panda.close()
        time.sleep(0.1)
    raise RuntimeError(f"Red Panda {serial} did not return in bootstub mode: {last_error}")


def _read_pinned_bootstub(path: Path) -> bytes:
    code = path.read_bytes()
    if PINNED_VERSION.encode("ascii") not in code:
        raise RuntimeError(
            f"bootstub does not contain pinned version marker {PINNED_VERSION!r}"
        )
    if not code or len(code) >= 128 * 1024:
        raise RuntimeError("bootstub size is outside the Red Panda H7 boot sector")
    return code


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Recover an explicitly selected Red Panda with a pinned DEBUG bootstub."
    )
    parser.add_argument("--serial", required=True, help="24-character Panda application serial")
    parser.add_argument("--dfu-serial", required=True, help="12-character STM32 DFU serial")
    parser.add_argument("--bootstub", required=True, type=Path)
    parser.add_argument(
        "--confirm",
        required=True,
        help="Must match --dfu-serial to authorize erasing boot sectors 0 and 1.",
    )
    args = parser.parse_args()

    if not re.fullmatch(r"[0-9A-Fa-f]{24}", args.serial):
        parser.error("--serial must be a 24-character hexadecimal Panda serial")
    if not re.fullmatch(r"[0-9A-Fa-f]{12}", args.dfu_serial):
        parser.error("--dfu-serial must be a 12-character hexadecimal STM32 serial")
    if args.confirm.upper() != args.dfu_serial.upper():
        parser.error("--confirm must exactly match --dfu-serial")
    args.serial = args.serial.lower()
    args.dfu_serial = args.dfu_serial.upper()
    expected_dfu_serial = PandaDFU.st_serial_to_dfu_serial(args.serial)
    if expected_dfu_serial != args.dfu_serial:
        parser.error(
            f"--serial maps to DFU serial {expected_dfu_serial}, not {args.dfu_serial}"
        )
    bootstub = args.bootstub.resolve(strict=True)
    if bootstub.name != "bootstub.panda_h7.bin":
        parser.error("bootstub must be the Red Panda image bootstub.panda_h7.bin")
    code = _read_pinned_bootstub(bootstub)
    digest = hashlib.sha256(code).hexdigest()

    attached = PandaDFU.list()
    if args.dfu_serial not in attached:
        raise RuntimeError(
            f"selected DFU device {args.dfu_serial} is not attached; found {attached}"
        )

    with PandaDFU(args.dfu_serial) as dfu:
        if dfu.get_mcu_type() != McuType.H7:
            raise RuntimeError("selected DFU device is not an STM32H7 Red Panda")
        dfu.program_bootstub(code)
        dfu.reset()

    panda = _wait_for_bootstub(args.serial)
    try:
        if panda.get_type() != Panda.HW_TYPE_RED_PANDA:
            raise RuntimeError("recovered device is not a Red Panda")
        version = panda.get_version()
        if version != PINNED_VERSION:
            raise RuntimeError(
                f"bootstub verification failed: got {version!r}, expected {PINNED_VERSION!r}"
            )
    finally:
        panda.close()

    print(f"Recovered and verified Red Panda bootstub {args.serial}")
    print(f"Bootstub: {bootstub}")
    print(f"SHA-256: {digest}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
