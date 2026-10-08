"""Offline display-decoder checks; no ROS master or Panda connection."""

from pathlib import Path
import sys
from types import SimpleNamespace
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from can_dashboard_data import CanDashboardData, checksum_valid


def frame(address, size, signals=(), crc=True, **flags):
    data = bytearray(size)
    for start, length, value in signals:
        for bit in range(length):
            index = start + bit
            data[index // 8] |= ((value >> bit) & 1) << (index % 8)
    if crc:
        # Independent bitwise reference from the C++ codec, not the decoder.
        checksum = 0
        for byte in data[2:] + bytes((address & 255, (address >> 8) & 255)):
            checksum ^= byte << 8
            for _ in range(8):
                checksum = ((checksum << 1) ^ (0x1021 if checksum & 0x8000 else 0)) & 65535
        checksum ^= {8: 0x5F29, 16: 0x041D, 24: 0x819D, 32: 0x9F5B}[size]
        data[:2] = checksum.to_bytes(2, "little")
    result = SimpleNamespace(address=address, data=bytes(data), bus=0,
                             extended=False, returned=False, rejected=False)
    for key, value in flags.items():
        setattr(result, key, value)
    return result


class CanDashboardDataTest(unittest.TestCase):
    def test_crc_matches_existing_cpp_golden_lfa(self):
        data = bytes.fromhex("be bf 00 02 80 c8 18 00 00 00 00 00 00 64 00 00")
        self.assertTrue(checksum_valid(0x12A, data))
        self.assertFalse(checksum_valid(0x125, data))

    def test_vehicle_fields_match_cpp_parser_units_and_signed_angle(self):
        decoder = CanDashboardData()
        decoder.feed(frame(0x125, 16, [(24, 16, -123)]), 10)
        decoder.feed(frame(0x0EA, 24, [(64, 12, 2148), (80, 13, 4195)]), 10)
        decoder.feed(frame(0x0A0, 24, [(start, 14, 320) for start in (64, 80, 96, 112)]), 10)
        decoder.feed(frame(0x175, 24, [(81, 1, 1), (66, 2, 2)]), 10)
        decoder.feed(frame(0x035, 32, [(40, 8, 21), (192, 3, 5)]), 10)
        values = decoder.snapshot(10)["values"]
        self.assertAlmostEqual(values["steering_angle_deg"], -12.3)
        self.assertEqual(values["speed_kph"], 10)
        self.assertEqual(values["wheel_speeds_kph"], (10, 10, 10, 10))
        self.assertEqual(values["driver_torque_counts"], 100)
        self.assertAlmostEqual(values["eps_torque_nm"], 10)
        self.assertFalse(values["eps_fault"])
        self.assertTrue(values["brake_pressed"])
        self.assertTrue(values["acc_fault"])
        self.assertEqual(values["gear_raw"], 5)
        self.assertEqual(values["accelerator_raw"], 21)

    def test_dynamics_units_quality_and_no_crc_esc_fallback(self):
        decoder = CanDashboardData()
        signals = [(64, 16, 32968), (80, 16, 33568), (96, 16, 31198)]
        decoder.feed(frame(0x04A, 32, signals), 10)
        values = decoder.snapshot(10)["values"]
        self.assertAlmostEqual(values["yaw_rate_deg_s"], 1)
        self.assertAlmostEqual(values["lateral_accel_mps2"], (33568 * .000127465 - 4.17677312) * 9.80665)
        self.assertAlmostEqual(values["longitudinal_accel_mps2"], (31198 * .000127465 - 4.17677312) * 9.80665)
        decoder.feed(frame(0x04A, 32, [(24, 4, 1)]), 10.5)
        self.assertEqual(decoder.quality_errors, 1)
        self.assertNotIn("yaw_rate_deg_s", decoder.snapshot(11.1)["values"])
        decoder.feed(frame(0x0E5, 32, [(start, 16, 32768) for start in (64, 80, 96)], crc=False), 12)
        self.assertAlmostEqual(decoder.snapshot(12)["values"]["yaw_rate_deg_s"], 0)

    def test_bad_crc_size_and_extended_alias_never_refresh_values(self):
        decoder = CanDashboardData()
        good = frame(0x125, 16, [(24, 16, 200)])
        decoder.feed(good, 1)
        bad = frame(0x125, 16)
        bad.data = bad.data[:4] + bytes((bad.data[4] ^ 1,)) + bad.data[5:]
        decoder.feed(bad, 1.5)
        decoder.feed(frame(0x125, 8), 1.5)
        decoder.feed(frame(0x125, 16, extended=True), 1.5)
        self.assertEqual(decoder.crc_errors, 1)
        self.assertEqual(decoder.size_errors, 1)
        self.assertEqual(decoder.snapshot(2.1)["values"], {})

    def test_raw_count_rate_wait_stale_and_return_to_live(self):
        decoder = CanDashboardData()
        self.assertEqual(decoder.snapshot(0)["status"], "WAITING")
        unknown = frame(0x12345, 64, crc=False, extended=True)
        decoder.feed(unknown, 0)
        decoder.feed(unknown, .1)
        sample = decoder.snapshot(.1)
        self.assertEqual(sample["status"], "RECEIVING")
        self.assertAlmostEqual(sample["fps"], 10)
        self.assertEqual(sample["total"], 2)
        self.assertEqual(sample["id_count"], 1)
        self.assertEqual(sample["last_frame"][1], 64)
        self.assertEqual(decoder.snapshot(1.2)["status"], "STALE")
        decoder.feed(unknown, 2)
        self.assertEqual(decoder.snapshot(2)["status"], "RECEIVING")

    def test_other_bus_and_panda_tx_metadata_are_not_vehicle_rx(self):
        decoder = CanDashboardData()
        for flags in ({"bus": 2}, {"returned": True}, {"rejected": True}):
            decoder.feed(frame(0x125, 16, **flags), 10)
        self.assertEqual(decoder.total, 0)
        self.assertEqual(decoder.snapshot(10)["values"], {})


if __name__ == "__main__":
    unittest.main()
