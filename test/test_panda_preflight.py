import struct
import unittest

from scripts.panda_preflight import (
    CAN_HEALTH_FIELDS,
    CAN_HEALTH_STRUCT,
    EXPECTED_CAN_HASH,
    EXPECTED_HEALTH_HASH,
    HEALTH_FIELDS,
    HEALTH_STRUCT,
    PreflightError,
    evaluate,
    parse_can_health,
    parse_health,
    parse_packet_versions,
)


class PandaPreflightTest(unittest.TestCase):
    def test_pinned_hash_packet(self):
        parsed = parse_packet_versions(
            struct.pack("<II", EXPECTED_HEALTH_HASH, EXPECTED_CAN_HASH)
        )
        self.assertEqual(parsed["format"], "hash")
        self.assertTrue(parsed["matches_pinned"])

    def test_legacy_packet_is_not_pinned(self):
        parsed = parse_packet_versions(bytes((15, 4, 5)))
        self.assertEqual(parsed["format"], "legacy")
        self.assertFalse(parsed["matches_pinned"])

    def test_health_requires_exact_abi(self):
        with self.assertRaises(PreflightError):
            parse_health(bytes(HEALTH_STRUCT.size - 1))
        parsed = parse_health(bytes(HEALTH_STRUCT.size))
        self.assertEqual(set(parsed), set(HEALTH_FIELDS))

    def test_can_health_requires_exact_abi(self):
        parsed = parse_can_health(bytes(CAN_HEALTH_STRUCT.size))
        self.assertEqual(set(parsed), set(CAN_HEALTH_FIELDS))

    def test_evaluate_clean_disconnected_bench(self):
        result = {
            "is_red_panda": True,
            "application_mode": True,
            "firmware": "IONIQ5-dd8a5b3d-DEBUG",
            "packet_versions": {"matches_pinned": True},
            "health": {
                "faults": 0,
                "fault_status": 0,
                "heartbeat_lost": 0,
                "safety_rx_checks_invalid": 0,
                "tx_buffer_overflow": 0,
                "rx_buffer_overflow": 0,
                "harness_status": 0,
            },
            "can": [
                {"bus": bus, "bus_off": 0, "error_warning": 0, "error_passive": 0}
                for bus in range(3)
            ],
        }
        self.assertEqual(evaluate(result, allow_unpinned=False, require_harness=False), [])
        self.assertIn(
            "Hyundai harness is not detected",
            evaluate(result, allow_unpinned=False, require_harness=True),
        )

    def test_bootstub_and_other_debug_build_fail_strict_check(self):
        result = {
            "is_red_panda": True,
            "application_mode": False,
            "firmware": "DEV-12345678-DEBUG",
            "packet_versions": {"matches_pinned": True},
            "health": {
                "faults": 0,
                "fault_status": 0,
                "heartbeat_lost": 0,
                "safety_rx_checks_invalid": 0,
                "tx_buffer_overflow": 0,
                "rx_buffer_overflow": 0,
                "harness_status": 0,
            },
            "can": [],
        }
        failures = evaluate(result, allow_unpinned=False, require_harness=False)
        self.assertIn("Panda is in bootstub mode, not application mode", failures)
        self.assertTrue(any("expected 'IONIQ5-dd8a5b3d-DEBUG'" in item for item in failures))


if __name__ == "__main__":
    unittest.main()
