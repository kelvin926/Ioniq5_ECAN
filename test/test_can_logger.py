import csv
import functools
import json
import operator
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
from can_logger import (CsvRecorder, PacketDecoder, RawFrame, ReceiveOnlyError,
                        ReceiveOnlyPanda, utc_timestamp)
from panda_preflight import (EXPECTED_CAN_HASH, EXPECTED_HEALTH_HASH, HEALTH_FIELDS,
                             HEALTH_STRUCT)
import struct


def packet(address, payload, bus=0, fd=False, extended=False, returned=False, rejected=False):
    lengths = (0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64)
    word = (address << 3) | (int(extended) << 2) | (int(returned) << 1) | int(rejected)
    data = bytearray(((lengths.index(len(payload)) << 4) | (bus << 1) | int(fd),))
    data.extend(struct.pack("<I", word))
    data.append(0)
    data.extend(payload)
    data[5] = functools.reduce(operator.xor, data, 0)
    return bytes(data)


class Timeout(Exception):
    def __init__(self, received):
        self.received = received


class InOnlyHandle:
    def __init__(self, mode=0, controls=0, reads=()):
        self.mode, self.controls, self.reads = mode, controls, list(reads)
        self.operations = []

    def claimInterface(self, interface):
        self.operations.append(("claim", interface))

    def controlRead(self, direction, request, value, index, length, timeout):
        assert direction == 0xC0
        self.operations.append(("control_in", request))
        if request == 0xDD:
            return struct.pack("<II", EXPECTED_HEALTH_HASH, EXPECTED_CAN_HASH)
        if request == 0xC1:
            return b"\x07"
        if request == 0xD2:
            fields = dict.fromkeys(HEALTH_FIELDS, 0)
            fields.update(safety_mode=self.mode, controls_allowed=self.controls, harness_status=1)
            return HEALTH_STRUCT.pack(*(fields[name] for name in HEALTH_FIELDS))
        raise AssertionError("Unexpected status read")

    def bulkRead(self, endpoint, capacity, timeout):
        assert endpoint == 0x81
        self.operations.append(("bulk_in", endpoint))
        result = self.reads.pop(0)
        if isinstance(result, Exception):
            raise result
        return result

    def controlWrite(self, *args, **kwargs):
        raise AssertionError("USB OUT forbidden")

    def bulkWrite(self, *args, **kwargs):
        raise AssertionError("CAN TX forbidden")

    def releaseInterface(self, interface):
        self.operations.append(("release", interface))

    def close(self):
        self.operations.append(("close",))


def fake_usb(handle):
    class Device:
        getVendorID = lambda self: 0x3801
        getProductID = lambda self: 0xDDCC
        getSerialNumber = lambda self: "LOCAL_DEVICE"
        open = lambda self: handle
    class Context:
        __enter__ = lambda self: self
        getDeviceList = lambda self, **kwargs: [Device()]
        close = lambda self: None
    class Usb:
        USBErrorTimeout = Timeout
        USBContext = Context
    return Usb


class CanLoggerTest(unittest.TestCase):
    def test_fragmented_classic_frame_timestamp_is_completion_batch(self):
        raw = packet(0x123, bytes(range(8)))
        decoder = PacketDecoder()
        self.assertEqual(decoder.feed(raw[:7], 100, 200, 300), [])
        frames = decoder.feed(raw[7:], 101, 201, 301)
        self.assertEqual(len(frames), 1)
        self.assertEqual(frames[0].data, bytes(range(8)))
        self.assertEqual((frames[0].unix_time_ns, frames[0].monotonic_time_ns, frames[0].ros_time_ns), (101, 201, 301))

    def test_unknown_extended_canfd_64_bytes_and_metadata_preserved(self):
        payload = bytes(range(64))
        frame = PacketDecoder().feed(packet(0x18DAF110, payload, 2, True, True, True, True), 1, 2, 3)[0]
        self.assertEqual((frame.address, frame.bus, frame.data), (0x18DAF110, 2, payload))
        self.assertTrue(frame.fd and frame.extended and frame.returned and frame.rejected)

    def test_checksum_error_stops_without_resynchronizing(self):
        raw = bytearray(packet(0x456, b"abc"))
        raw[-1] ^= 1
        with self.assertRaises(ReceiveOnlyError):
            PacketDecoder().feed(raw + packet(0x123, b"valid"), 1, 2, 3)

    def test_open_partial_timeout_and_startup_use_in_only(self):
        frame = packet(0x555, b"raw")
        handle = InOnlyHandle(reads=[Timeout(b"old partial"), b"short boundary", Timeout(frame[:5]), frame[5:]])
        panda = ReceiveOnlyPanda(usb1=fake_usb(handle))
        panda.open()
        self.assertEqual(panda.synchronize(), len(b"old partialshort boundary"))
        decoder = PacketDecoder()
        partial, timed_out = panda.read_chunk()
        self.assertTrue(timed_out)
        self.assertEqual(decoder.feed(partial, 1, 2, 3), [])
        tail, _ = panda.read_chunk()
        self.assertEqual(decoder.feed(tail, 4, 5, 6)[0].data, b"raw")
        panda.close()
        self.assertTrue(all(operation[0] in {"claim", "control_in", "bulk_in", "release", "close"}
                            for operation in handle.operations))

    def test_non_silent_or_allowed_control_is_refused_without_writes(self):
        for mode, controls in ((19, 0), (28, 1), (0, 1)):
            handle = InOnlyHandle(mode, controls)
            with self.assertRaises(ReceiveOnlyError):
                ReceiveOnlyPanda(usb1=fake_usb(handle)).open()
            self.assertIn(("close",), handle.operations)

    def test_csv_round_trip_and_nanosecond_precision_without_serial(self):
        parent = ROOT / "build/can-logger-unit"
        assert ROOT.resolve() in parent.resolve().parents
        parent.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=str(parent)) as directory:
            assert ROOT.resolve() in Path(directory).resolve().parents
            recorder = CsvRecorder(directory, {"bus_filter": 0})
            frame = RawFrame(0x123, 0, True, False, False, False, bytes((0, 255)),
                             1_700_000_000_123_456_789, 9_000_000_001, 7_000_000_002)
            recorder.write(frame)
            recorder.close("shutdown")
            with recorder.path.open(newline="", encoding="utf-8") as handle:
                row = next(csv.DictReader(handle))
            self.assertEqual(row["data_hex"], "00ff")
            self.assertEqual(int(row["unix_time_ns"]), frame.unix_time_ns)
            self.assertEqual(int(row["monotonic_time_ns"]), frame.monotonic_time_ns)
            self.assertEqual(row["timestamp_utc"], "2023-11-14T22:13:20.123456789Z")
            metadata = json.loads(recorder.meta_path.read_text(encoding="utf-8"))
            self.assertEqual(metadata["frames"], 1)
            self.assertFalse(metadata["device_serial_recorded"])
            self.assertNotIn("serial", metadata)


if __name__ == "__main__":
    unittest.main()
