#!/usr/bin/env python3
"""Explicit USB-only negative-TX bench check; never run with a vehicle attached."""
import argparse
import json
import struct

from panda import Panda
from opendbc.can.packer import CANPacker


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--serial", required=True)
    args = parser.parse_args()
    panda = Panda(serial=args.serial, cli=False)
    original = panda.health()
    if original["car_harness_status"] or original["ignition_line"] or original["ignition_can"]:
        panda.close()
        raise RuntimeError("disconnect the vehicle harness; USB-only test required")
    report = {"serial": args.serial, "vehicle_connected": False, "test": "standby_negative_tx"}
    try:
        panda.set_safety_mode(19, 7173)  # CarParams.SafetyModel.noOutput in the pinned ABI.
        panda.send_heartbeat(False)
        magic, flags = struct.unpack("<II", panda._handle.controlRead(Panda.REQUEST_IN, 0xB7, 0, 0, 8))
        if magic != 0x49355231 or not flags & 16 or flags & 3:
            raise RuntimeError("standby capability/initial physical-button state mismatch")
        before = panda.health()
        tx_before = [panda.can_health(i)["total_tx_cnt"] for i in range(3)]
        packer = CANPacker("hyundai_canfd_generated")
        panda.can_send_many([
            packer.make_can_msg("LFA", 0, {"StrTqReqVal": 1, "ActToiSta": 1}),
            packer.make_can_msg("SCC_CONTROL", 0, {"ACCMode": 1, "aReqRaw": 0.1, "aReqValue": 0.1}),
            (0x730, b"\x02\x3e\x80" + bytes(5), 0),
        ])
        panda.can_recv()  # Includes firmware-rejected frames; no vehicle or loopback used.
        after = panda.health()
        tx_after = [panda.can_health(i)["total_tx_cnt"] for i in range(3)]
        report.update({"capability_magic": hex(magic), "session_flags": flags,
                       "blocked_delta": after["safety_tx_blocked"] - before["safety_tx_blocked"],
                       "physical_tx_delta": [a - b for a, b in zip(tx_after, tx_before)],
                       "controls_allowed": after["controls_allowed"]})
        if report["blocked_delta"] != 3 or any(report["physical_tx_delta"]) or after["controls_allowed"]:
            raise RuntimeError("standby negative-TX validation failed: " + str(report))
        report["passed"] = True
    finally:
        panda.set_safety_mode(original["safety_mode"], original["safety_param"])
        panda.close()
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
