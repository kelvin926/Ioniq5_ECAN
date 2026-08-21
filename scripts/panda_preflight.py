#!/usr/bin/env python3
"""Read-only Red Panda preflight check.

This script intentionally uses vendor control reads only. It does not reset CAN,
change bitrates, change the safety model, send a heartbeat, or transmit CAN.
"""

from __future__ import annotations

import argparse
import ctypes
import json
import struct
import sys
import time
from typing import Any, Dict, List, Optional


PANDA_VIDS = (0xBBAA, 0x3801)
PANDA_PIDS = (0xDDCC, 0xDDEE)
RED_PANDA_TYPE = 0x07
APPLICATION_PID = 0xDDCC
EXPECTED_FIRMWARE = "IONIQ5ECAN-dd8a5b3d-DEBUG"
EXPECTED_HEALTH_HASH = 0x290DAE03
EXPECTED_CAN_HASH = 0x75ABF276
ECAN_ONLY_IGNORED_FAULTS = (1 << 3) | (1 << 4)

HEALTH_STRUCT = struct.Struct("<IIIIIIIIBBBBBHBBBHfBBHHHBHf")
HEALTH_FIELDS = (
    "uptime_s",
    "voltage_mV",
    "current_mA",
    "safety_tx_blocked",
    "safety_rx_invalid",
    "tx_buffer_overflow",
    "rx_buffer_overflow",
    "faults",
    "ignition_line",
    "ignition_can",
    "controls_allowed",
    "harness_status",
    "safety_mode",
    "safety_param",
    "fault_status",
    "power_save_enabled",
    "heartbeat_lost",
    "alternative_experience",
    "interrupt_load",
    "fan_power",
    "safety_rx_checks_invalid",
    "spi_error_count",
    "sbu1_voltage_mV",
    "sbu2_voltage_mV",
    "som_reset_triggered",
    "sound_output_level",
    "temperature_C",
)

CAN_HEALTH_STRUCT = struct.Struct("<BIBBBBBBBBIIIIIIIHHBBBIIII")
CAN_HEALTH_FIELDS = (
    "bus_off",
    "bus_off_count",
    "error_warning",
    "error_passive",
    "last_error",
    "last_stored_error",
    "last_data_error",
    "last_data_stored_error",
    "receive_error_count",
    "transmit_error_count",
    "total_error_count",
    "total_tx_lost_count",
    "total_rx_lost_count",
    "total_tx_count",
    "total_rx_count",
    "total_forwarded_count",
    "total_tx_checksum_error_count",
    "can_speed_kbps",
    "can_data_speed_kbps",
    "canfd_enabled",
    "brs_enabled",
    "canfd_non_iso",
    "irq0_call_rate",
    "irq1_call_rate",
    "irq2_call_rate",
    "can_core_reset_count",
)


class PreflightError(RuntimeError):
    pass


def parse_packet_versions(data: bytes) -> Dict[str, Any]:
    if len(data) == 8:
        health_hash, can_hash = struct.unpack("<II", data)
        return {
            "format": "hash",
            "health": f"0x{health_hash:08X}",
            "can": f"0x{can_hash:08X}",
            "matches_pinned": health_hash == EXPECTED_HEALTH_HASH
            and can_hash == EXPECTED_CAN_HASH,
        }
    if len(data) == 3:
        return {
            "format": "legacy",
            "health": data[0],
            "can": data[1],
            "can_health": data[2],
            "matches_pinned": False,
        }
    return {"format": "unknown", "length": len(data), "raw": data.hex(), "matches_pinned": False}


def parse_health(data: bytes) -> Dict[str, Any]:
    if len(data) != HEALTH_STRUCT.size:
        raise PreflightError(
            f"health packet is {len(data)} bytes; pinned ABI requires {HEALTH_STRUCT.size}"
        )
    return dict(zip(HEALTH_FIELDS, HEALTH_STRUCT.unpack(data)))


def parse_can_health(data: bytes) -> Dict[str, Any]:
    if len(data) != CAN_HEALTH_STRUCT.size:
        raise PreflightError(
            f"CAN health packet is {len(data)} bytes; expected {CAN_HEALTH_STRUCT.size}"
        )
    return dict(zip(CAN_HEALTH_FIELDS, CAN_HEALTH_STRUCT.unpack(data)))


def load_usb1():
    try:
        import usb1  # type: ignore
    except ImportError as error:
        raise PreflightError(
            "python usb1 is missing; run `python3 -m pip install --user libusb1` on Ubuntu or use "
            "`uv run --with libusb1 --with libusb-package`"
        ) from error

    if sys.platform == "win32":
        try:
            import libusb_package  # type: ignore

            usb1._libusb1.loadLibrary(ctypes.CDLL(str(libusb_package.get_library_path())))
        except ImportError as error:
            raise PreflightError("libusb-package is required for WinUSB diagnostics") from error
    return usb1


def probe(serial: Optional[str], sample_seconds: float) -> Dict[str, Any]:
    usb1 = load_usb1()
    request_in = usb1.ENDPOINT_IN | usb1.TYPE_VENDOR | usb1.RECIPIENT_DEVICE

    def control_read(handle, request: int, length: int, value: int = 0) -> bytes:
        return bytes(handle.controlRead(request_in, request, value, 0, length, timeout=2000))

    with usb1.USBContext() as context:
        matches = []
        for device in context.getDeviceList(skip_on_error=True):
            if device.getVendorID() not in PANDA_VIDS or device.getProductID() not in PANDA_PIDS:
                continue
            try:
                usb_serial = device.getSerialNumber()
            except usb1.USBError as error:
                raise PreflightError(f"cannot read Panda USB serial: {error}") from error
            if serial is None or usb_serial == serial:
                matches.append((device, usb_serial))

        if not matches:
            wanted = f" serial {serial}" if serial else ""
            raise PreflightError(f"no Panda{wanted} found")
        if len(matches) != 1:
            found = ", ".join(item[1] for item in matches)
            raise PreflightError(f"multiple Pandas found ({found}); pass --serial")

        device, usb_serial = matches[0]
        result: Dict[str, Any] = {
            "serial": usb_serial,
            "vid": f"0x{device.getVendorID():04X}",
            "pid": f"0x{device.getProductID():04X}",
            "application_mode": device.getProductID() == APPLICATION_PID,
            "read_only": True,
        }
        handle = device.open()
        try:
            handle.claimInterface(0)
            result["firmware"] = control_read(handle, 0xD6, 64).rstrip(b"\x00").decode(
                "utf-8", errors="replace"
            )
            hardware = control_read(handle, 0xC1, 64)
            result["hardware_type"] = f"0x{hardware[0]:02X}" if hardware else "missing"
            result["is_red_panda"] = bool(hardware and hardware[0] == RED_PANDA_TYPE)
            result["packet_versions"] = parse_packet_versions(control_read(handle, 0xDD, 8))

            health_before_raw = control_read(handle, 0xD2, HEALTH_STRUCT.size)
            result["health_packet_length"] = len(health_before_raw)
            if len(health_before_raw) == HEALTH_STRUCT.size:
                health_before = parse_health(health_before_raw)
                can_before = [
                    parse_can_health(control_read(handle, 0xC2, CAN_HEALTH_STRUCT.size, bus))
                    for bus in range(3)
                ]
                time.sleep(sample_seconds)
                health_after = parse_health(control_read(handle, 0xD2, HEALTH_STRUCT.size))
                can_after = [
                    parse_can_health(control_read(handle, 0xC2, CAN_HEALTH_STRUCT.size, bus))
                    for bus in range(3)
                ]
                result["health"] = health_after
                result["uptime_delta"] = health_after["uptime_s"] - health_before["uptime_s"]
                for bus, (before, after) in enumerate(zip(can_before, can_after)):
                    after["bus"] = bus
                    after["rx_delta"] = after["total_rx_count"] - before["total_rx_count"]
                    after["tx_delta"] = after["total_tx_count"] - before["total_tx_count"]
                result["can"] = can_after
        finally:
            try:
                handle.releaseInterface(0)
            except usb1.USBError:
                pass
            handle.close()
    return result


def evaluate(
    result: Dict[str, Any],
    allow_unpinned: bool,
    require_harness: bool,
    ecan_only: bool = False,
) -> List[str]:
    failures = []
    if not result.get("application_mode", True):
        failures.append("Panda is in bootstub mode, not application mode")
    if not result.get("is_red_panda"):
        failures.append("connected hardware is not a Red Panda")
    if not allow_unpinned:
        if not result.get("packet_versions", {}).get("matches_pinned"):
            failures.append("Panda packet ABI does not match the pinned firmware")
        if result.get("firmware") != EXPECTED_FIRMWARE:
            failures.append(
                f"Panda firmware is {result.get('firmware')!r}, expected {EXPECTED_FIRMWARE!r}"
            )
    health = result.get("health")
    if health is None:
        failures.append(
            f"health packet length {result.get('health_packet_length')} does not match pinned ABI"
        )
        return failures
    effective_faults = health["faults"]
    if ecan_only:
        effective_faults &= ~ECAN_ONLY_IGNORED_FAULTS
    ignored_fault_is_only_fault = (
        ecan_only and health["faults"] != 0 and effective_faults == 0
    )
    if effective_faults:
        failures.append(f"health.faults={health['faults']} effective={effective_faults}")
    if health["fault_status"] and not ignored_fault_is_only_fault:
        failures.append(f"health.fault_status={health['fault_status']}")
    for field in (
        "heartbeat_lost",
        "safety_rx_checks_invalid",
        "tx_buffer_overflow",
        "rx_buffer_overflow",
    ):
        if health[field]:
            failures.append(f"health.{field}={health[field]}")
    if require_harness:
        if ecan_only and health["harness_status"] != 1:
            failures.append("ECAN-only mode requires harness_status=1")
        elif not ecan_only and health["harness_status"] == 0:
            failures.append("Hyundai harness is not detected")
    for bus in result.get("can", []):
        if ecan_only and bus["bus"] != 0:
            continue
        if bus["bus_off"] or bus["error_warning"] or bus["error_passive"]:
            failures.append(f"physical CAN controller {bus['bus']} reports an error state")
    if require_harness and ecan_only:
        ecan = next((bus for bus in result.get("can", []) if bus["bus"] == 0), None)
        if ecan is None or ecan.get("rx_delta", 0) <= 0:
            failures.append("ECAN physical controller 0 received no frames during the sample")
    return failures


def print_human(result: Dict[str, Any], failures: List[str]) -> None:
    print(
        f"[{'ok' if result.get('application_mode') else 'fail'}] USB Panda "
        f"{result['serial']} ({result['vid']}:{result['pid']}) application_mode="
        f"{result.get('application_mode')}"
    )
    print(
        f"[{'ok' if result.get('is_red_panda') else 'fail'}] hardware "
        f"{result.get('hardware_type')} firmware={result.get('firmware')}"
    )
    packet_versions = result.get("packet_versions", {})
    print(
        f"[{'ok' if packet_versions.get('matches_pinned') else 'fail'}] "
        f"packet_versions={packet_versions}"
    )
    if "health" in result:
        health = result["health"]
        print(
            "[ok] health "
            f"uptime={health['uptime_s']}s voltage={health['voltage_mV']}mV "
            f"harness={health['harness_status']} ignition="
            f"{int(bool(health['ignition_line'] or health['ignition_can']))} "
            f"safety={health['safety_mode']}/{health['safety_param']}"
        )
        for bus in result.get("can", []):
            print(
                f"[ok] physical CAN controller {bus['bus']} "
                f"rx_delta={bus['rx_delta']} tx_delta={bus['tx_delta']} "
                f"bus_off={bus['bus_off']} errors={bus['total_error_count']}"
            )
    for failure in failures:
        print(f"[fail] {failure}")
    print("PREFLIGHT PASS" if not failures else "PREFLIGHT FAIL")


def main() -> int:
    parser = argparse.ArgumentParser(description="Read-only Red Panda preflight check")
    parser.add_argument("--serial", help="required when more than one Panda is attached")
    parser.add_argument("--sample-seconds", type=float, default=1.0)
    parser.add_argument(
        "--allow-unpinned",
        action="store_true",
        help="skip firmware/hash checks; ABI and health faults still fail",
    )
    parser.add_argument(
        "--require-harness",
        action="store_true",
        help="fail when a Hyundai harness is not detected",
    )
    parser.add_argument(
        "--ecan-only",
        action="store_true",
        help="validate the ECAN-only firmware profile and physical controller 0 only",
    )
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    if args.sample_seconds < 0.0 or args.sample_seconds > 30.0:
        parser.error("--sample-seconds must be within [0,30]")

    try:
        result = probe(args.serial, args.sample_seconds)
        failures = evaluate(result, args.allow_unpinned, args.require_harness, args.ecan_only)
    except (PreflightError, OSError) as error:
        if args.json:
            print(json.dumps({"read_only": True, "failures": [str(error)]}, indent=2))
        else:
            print(f"[fail] {error}", file=sys.stderr)
            print("PREFLIGHT FAIL", file=sys.stderr)
        return 1

    if args.json:
        result["failures"] = failures
        result["passed"] = not failures
        print(json.dumps(result, indent=2))
    else:
        print_human(result, failures)
    return 0 if not failures else 1


if __name__ == "__main__":
    raise SystemExit(main())
