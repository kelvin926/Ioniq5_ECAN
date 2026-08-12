#!/usr/bin/env python3
import argparse
from pathlib import Path

from panda import Panda


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Flash an explicitly selected Red Panda with a pinned firmware image."
    )
    parser.add_argument("--serial", required=True)
    parser.add_argument("--firmware", required=True, type=Path)
    parser.add_argument(
        "--confirm",
        required=True,
        help="Must exactly match --serial to authorize the destructive flash operation.",
    )
    args = parser.parse_args()

    if args.confirm != args.serial:
        parser.error("--confirm must exactly match --serial")
    firmware = args.firmware.resolve(strict=True)
    if firmware.name != "panda_h7.bin.signed":
        parser.error("firmware must be the signed Red Panda image panda_h7.bin.signed")

    panda = Panda(serial=args.serial, cli=False)
    if panda.get_type() != Panda.HW_TYPE_RED_PANDA:
        raise RuntimeError("selected device is not a Red Panda")
    panda.flash(fn=str(firmware))
    print(f"Flashed Red Panda {args.serial} with {firmware}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
