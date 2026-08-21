#!/usr/bin/env python3
import argparse
import hashlib
import re
import time
from pathlib import Path

from panda import Panda


PINNED_VERSION = "IONIQ5-dd8a5b3d-DEBUG"
ACCEPTED_BOOTSTUB_VERSIONS = {PINNED_VERSION, "DEV-dd8a5b3d-DEBUG"}


def _open_in_state(serial: str, bootstub: bool, timeout_s: float = 15.0) -> Panda:
    deadline = time.monotonic() + timeout_s
    last_error = "device did not appear"
    while time.monotonic() < deadline:
        panda = None
        try:
            panda = Panda(serial=serial, cli=False)
            if panda.bootstub == bootstub:
                return panda
            last_error = (
                "device stayed in bootstub" if panda.bootstub else "device stayed in application mode"
            )
        except Exception as exc:  # USB disappears briefly during reset.
            last_error = str(exc)
        finally:
            if panda is not None and panda.bootstub != bootstub:
                panda.close()
        time.sleep(0.1)
    expected = "bootstub" if bootstub else "application"
    raise RuntimeError(f"Red Panda {serial} did not enter {expected} mode: {last_error}")


def _read_pinned_firmware(path: Path) -> bytes:
    code = path.read_bytes()
    if PINNED_VERSION.encode("ascii") not in code:
        raise RuntimeError(
            f"firmware does not contain pinned version marker {PINNED_VERSION!r}"
        )
    if len(code) <= 128:
        raise RuntimeError("firmware is too small to contain a Panda signature")
    return code


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Flash an explicitly selected Red Panda with a pinned firmware image."
    )
    parser.add_argument("--serial", required=True)
    parser.add_argument("--firmware", required=True, type=Path)
    parser.add_argument(
        "--confirm",
        required=True,
        help="Must match --serial to authorize the destructive flash operation.",
    )
    args = parser.parse_args()

    if not re.fullmatch(r"[0-9A-Fa-f]{24}", args.serial):
        parser.error("--serial must be a 24-character hexadecimal Panda serial")
    if args.confirm.lower() != args.serial.lower():
        parser.error("--confirm must exactly match --serial")
    args.serial = args.serial.lower()
    firmware = args.firmware.resolve(strict=True)
    if firmware.name != "panda_h7.bin.signed":
        parser.error("firmware must be the signed Red Panda image panda_h7.bin.signed")
    code = _read_pinned_firmware(firmware)
    digest = hashlib.sha256(code).hexdigest()

    panda = Panda(serial=args.serial, cli=False)
    try:
        if panda.get_type() != Panda.HW_TYPE_RED_PANDA:
            raise RuntimeError("selected device is not a Red Panda")
        if not panda.bootstub:
            # Panda.reconnect() intentionally does not claim USB interface 0. That is fine on
            # Linux, but Windows WinUSB needs a fresh claimed handle before EP2 bulk writes.
            panda.reset(enter_bootstub=True, reconnect=False)
        else:
            panda.close()

        panda = _open_in_state(args.serial, bootstub=True)
        if panda.get_type() != Panda.HW_TYPE_RED_PANDA:
            raise RuntimeError("device changed identity while entering bootstub")
        bootstub_version = panda.get_version()
        if bootstub_version not in ACCEPTED_BOOTSTUB_VERSIONS:
            raise RuntimeError(
                f"bootstub is {bootstub_version!r}, expected one of "
                f"{sorted(ACCEPTED_BOOTSTUB_VERSIONS)!r}; "
                "install the pinned ALLOW_DEBUG bootstub with recover_panda.py before "
                "flashing the application"
            )

        panda.flash(fn=str(firmware), code=code, reconnect=False)
        panda.close()
        panda = _open_in_state(args.serial, bootstub=False)
        if panda.get_type() != Panda.HW_TYPE_RED_PANDA:
            raise RuntimeError("flashed device is not a Red Panda")
        firmware_version = panda.get_version()
        if firmware_version != PINNED_VERSION or not panda.up_to_date(fn=str(firmware)):
            raise RuntimeError(
                f"post-flash verification failed: device version is {firmware_version!r}"
            )
    finally:
        panda.close()

    print(f"Flashed and verified Red Panda {args.serial}")
    print(f"Firmware: {firmware}")
    print(f"SHA-256: {digest}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
