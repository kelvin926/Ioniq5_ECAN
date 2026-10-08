"""Subscriber-only ECAN display data. No ROS, USB, publishing or control APIs.

Signal layouts/units match vehicle_state_parser.cpp and hyundai_canfd_codec.cpp.
Freshness/rate use local monotonic callback time, including during bag replay.
This is a display decoder, not a vehicle-readiness or engagement decision.
"""

from binascii import crc_hqx
from collections import deque


def checksum_valid(address, data):
    xor_out = {8: 0x5F29, 16: 0x041D, 24: 0x819D, 32: 0x9F5B}.get(len(data))
    if xor_out is None:
        return False
    suffix = bytes((address & 0xFF, (address >> 8) & 0xFF))
    crc = crc_hqx(data[2:] + suffix, 0) ^ xor_out
    return int.from_bytes(data[:2], "little") == crc


class CanDashboardData:
    SIZES = {0x04A: 32, 0x0E5: 32, 0x125: 16, 0x0EA: 24,
             0x0A0: 24, 0x175: 24, 0x035: 32}

    def __init__(self, bus=0):
        self.bus = bus
        self.times = deque(maxlen=20000)
        self.ids = set()
        self.total = 0
        self.crc_errors = 0
        self.size_errors = 0
        self.quality_errors = 0
        self.last_frame = None
        self.values = {}

    def feed(self, message, now):
        if message.bus != self.bus or message.returned or message.rejected:
            return
        data = bytes(message.data)
        self.total += 1
        self.times.append(now)
        if len(self.ids) < 4096:
            self.ids.add((message.address, message.extended))
        self.last_frame = (message.address, len(data), data[:16].hex(" "))
        if message.extended or message.address not in self.SIZES:
            return
        if len(data) != self.SIZES[message.address]:
            self.size_errors += 1
            return
        # ESC_02_10ms has no checksum field in the pinned DBC.
        if message.address != 0x0E5 and not checksum_valid(message.address, data):
            self.crc_errors += 1
            return

        raw = int.from_bytes(data, "little")

        def bits(start, length):
            return (raw >> start) & ((1 << length) - 1)

        address = message.address
        if address in (0x04A, 0x0E5):
            if any(bits(start, 4) for start in (24, 28, 32)):
                self.quality_errors += 1
                return
            decoded = {
                "yaw_rate_deg_s": bits(64, 16) * 0.005 - 163.84,
                "lateral_accel_mps2": (bits(80, 16) * 0.000127465 - 4.17677312) * 9.80665,
                "longitudinal_accel_mps2": (bits(96, 16) * 0.000127465 - 4.17677312) * 9.80665,
            }
        elif address == 0x125:
            angle = bits(24, 16)
            decoded = {"steering_angle_deg": (angle - 65536 if angle & 0x8000 else angle) * 0.1}
        elif address == 0x0EA:
            decoded = {"driver_torque_counts": bits(80, 13) - 4095,
                       "eps_torque_nm": bits(64, 12) * 0.1 - 204.8,
                       "eps_fault": bool(bits(54, 2))}
        elif address == 0x0A0:
            wheels = tuple(bits(start, 14) * 0.03125 for start in (64, 80, 96, 112))
            decoded = {"wheel_speeds_kph": wheels, "speed_kph": sum(wheels) / 4.0}
        elif address == 0x175:
            # Motorola 81|1 and 67|2 map to little-endian 81|1 and 66|2.
            decoded = {"brake_pressed": bool(bits(81, 1)), "acc_fault": bool(bits(66, 2))}
        else:  # ACCELERATOR (EV): preserve raw pedal and gear, do not invent units/mappings.
            decoded = {"accelerator_raw": bits(40, 8), "gear_raw": bits(192, 3)}
        self.values.update({name: (value, now) for name, value in decoded.items()})

    def snapshot(self, now, timeout=1.0):
        recent = [stamp for stamp in self.times if now - stamp <= 3.0]
        duration = recent[-1] - recent[0] if len(recent) > 1 else 0.0
        last = self.times[-1] if self.times else None
        return {
            "status": "WAITING" if last is None else "RECEIVING" if now - last < timeout else "STALE",
            "fps": (len(recent) - 1) / duration if duration > 0 else 0.0,
            "total": self.total, "id_count": len(self.ids), "last_frame": self.last_frame,
            "crc_errors": self.crc_errors, "size_errors": self.size_errors,
            "quality_errors": self.quality_errors,
            "values": {name: value for name, (value, stamp) in self.values.items()
                       if 0 <= now - stamp < timeout},
        }
