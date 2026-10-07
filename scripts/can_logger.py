#!/usr/bin/env python3
"""ROS 1 RAW CAN recorder using USB IN only, independent of the actuation node."""
from __future__ import annotations

import csv
from dataclasses import dataclass
from datetime import datetime, timezone
import functools
import json
import operator
import os
from pathlib import Path
import sys
import time

# Works from source, catkin's Python relay, and an installed package beside preflight.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from panda_preflight import (APPLICATION_PID, CAN_HEALTH_STRUCT, HEALTH_STRUCT,
                             PANDA_VIDS, PANDA_PIDS, RED_PANDA_TYPE, load_usb1,
                             parse_can_health, parse_health, parse_packet_versions)

DLC_LENGTHS = (0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64)
CSV_FIELDS = ("sequence", "timestamp_utc", "unix_time_ns", "monotonic_time_ns",
              "ros_time_ns", "bus", "address_hex", "fd", "extended", "returned",
              "rejected", "length", "data_hex")


class ReceiveOnlyError(RuntimeError):
    pass


@dataclass(frozen=True)
class RawFrame:
    address: int
    bus: int
    fd: bool
    extended: bool
    returned: bool
    rejected: bool
    data: bytes
    unix_time_ns: int
    monotonic_time_ns: int
    ros_time_ns: int


class PacketDecoder:
    def __init__(self):
        self.carry = bytearray()

    def feed(self, data, unix_ns, monotonic_ns, ros_ns):
        self.carry.extend(data)
        frames, position = [], 0
        while len(self.carry) - position >= 6:
            size = 6 + DLC_LENGTHS[self.carry[position] >> 4]
            if len(self.carry) - position < size:
                break
            packet = bytes(self.carry[position:position+size])
            if functools.reduce(operator.xor, packet, 0):
                self.carry.clear()
                raise ReceiveOnlyError("Panda USB CAN checksum failure; capture stopped without reset/resync")
            word = int.from_bytes(packet[1:5], "little")
            frames.append(RawFrame(word >> 3, (packet[0] >> 1) & 7,
                                   bool(packet[0] & 1), bool(word & 4),
                                   bool(word & 2), bool(word & 1), packet[6:],
                                   unix_ns, monotonic_ns, ros_ns))
            position += size
        del self.carry[:position]
        return frames


class ReceiveOnlyPanda:
    """No CAN send API, vendor OUT requests, heartbeat or firmware/settings writes."""
    def __init__(self, serial="", read_timeout_ms=20, usb1=None):
        self.serial = serial
        self.read_timeout_ms = read_timeout_ms
        self.usb1 = usb1
        self.context = self.handle = None
        self.claimed = False

    def control_read(self, request, length, value=0):
        if request not in (0xC1, 0xC2, 0xD2, 0xDD):
            raise ReceiveOnlyError("Read outside the logger's status/ABI scope")
        return bytes(self.handle.controlRead(0xC0, request, value, 0, length, timeout=2000))

    @staticmethod
    def check_silent(health):
        if health["safety_mode"] != 0 or health["controls_allowed"]:
            raise ReceiveOnlyError("Receive-only logger requires Panda SILENT (mode 0). "
                                   "Stop other Panda clients; the logger never changes the mode.")

    def health(self):
        return parse_health(self.control_read(0xD2, HEALTH_STRUCT.size))

    def can_health(self):
        return parse_can_health(self.control_read(0xC2, CAN_HEALTH_STRUCT.size, value=0))

    def open(self):
        self.usb1 = self.usb1 or load_usb1()
        self.context = self.usb1.USBContext()
        self.context.__enter__()
        try:
            devices = []
            for device in self.context.getDeviceList(skip_on_error=True):
                if device.getVendorID() not in PANDA_VIDS or device.getProductID() not in PANDA_PIDS:
                    continue
                if self.serial and device.getSerialNumber() != self.serial:
                    continue
                devices.append(device)
            if len(devices) != 1:
                raise ReceiveOnlyError("Expected one Panda; connect it or select ~serial locally")
            if devices[0].getProductID() != APPLICATION_PID:
                raise ReceiveOnlyError("Panda is not in application mode; logger does not reset/flash it")
            self.handle = devices[0].open()
            self.handle.claimInterface(0)
            self.claimed = True
            versions = parse_packet_versions(self.control_read(0xDD, 8))
            if not versions.get("matches_pinned"):
                raise ReceiveOnlyError("Panda packet ABI does not match this recorder")
            hardware = self.control_read(0xC1, 1)
            if hardware != bytes((RED_PANDA_TYPE,)):
                raise ReceiveOnlyError("This recorder targets Red Panda")
            health = self.health()
            self.check_silent(health)
            return {"packet_versions": versions, "health_before_sync": health}
        except Exception:
            self.close()
            raise

    def read_chunk(self, capacity=1024, timeout_ms=None):
        try:
            data = self.handle.bulkRead(0x81, capacity, timeout=timeout_ms or self.read_timeout_ms)
            return bytes(data), False
        except self.usb1.USBErrorTimeout as error:
            # Partial successful USB packets remain data even on a timed-out read.
            return bytes(getattr(error, "received", b"") or b""), True

    def synchronize(self, stopped=lambda: False, timeout_s=2.0):
        """Startup-only IN drain; a successful short read is the stream boundary."""
        deadline, discarded = time.monotonic()+timeout_s, 0
        while not stopped() and time.monotonic() < deadline:
            data, timed_out = self.read_chunk(capacity=16384, timeout_ms=100)
            discarded += len(data)
            if not timed_out and len(data) < 16384:
                self.check_silent(self.health())
                return discarded
        raise ReceiveOnlyError("No initial USB packet boundary; no reset/control write was attempted")

    def close(self):
        try:
            if self.handle is not None:
                try:
                    if self.claimed:
                        self.handle.releaseInterface(0)
                finally:
                    self.handle.close()
        finally:
            if self.context is not None:
                self.context.close()
            self.context = self.handle = None
            self.claimed = False


def utc_timestamp(nanoseconds):
    seconds, fraction = divmod(nanoseconds, 1_000_000_000)
    return datetime.fromtimestamp(seconds, timezone.utc).strftime("%Y-%m-%dT%H:%M:%S") + f".{fraction:09d}Z"


class CsvRecorder:
    def __init__(self, output_dir, metadata, flush_period_s=1.0, fsync=False):
        output_dir = Path(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
        name = "can_" + datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S_%fZ")
        self.path = output_dir / (name + ".csv")
        self.meta_path = output_dir / (name + ".meta.json")
        self.file = self.path.open("x", newline="", encoding="utf-8")
        self.writer = csv.writer(self.file)
        self.writer.writerow(CSV_FIELDS)
        self.file.flush()
        self.frames, self.flush_period_s, self.fsync = 0, flush_period_s, fsync
        self.next_flush = time.monotonic()+flush_period_s
        self.metadata = dict(metadata, schema_version=1, csv_file=self.path.name,
                             started_unix_ns=time.time_ns(), frames=0, status="recording",
                             timestamp_source="Host USB read completion; not ECU/hardware CAN arrival time",
                             raw_payload="Hex bytes, no DBC decoding or ID filter",
                             device_serial_recorded=False)
        self.save_metadata()

    def save_metadata(self):
        with self.meta_path.open("w", encoding="utf-8", newline="\n") as handle:
            json.dump(self.metadata, handle, indent=2)
            handle.write("\n")

    def write(self, frame):
        sequence = self.frames + 1
        self.writer.writerow((sequence, utc_timestamp(frame.unix_time_ns), frame.unix_time_ns,
                              frame.monotonic_time_ns, frame.ros_time_ns, frame.bus,
                              f"0x{frame.address:X}", int(frame.fd), int(frame.extended),
                              int(frame.returned), int(frame.rejected), len(frame.data), frame.data.hex()))
        self.frames = sequence

    def flush_if_due(self):
        if time.monotonic() >= self.next_flush:
            self.file.flush()
            if self.fsync:
                os.fsync(self.file.fileno())
            self.next_flush = time.monotonic()+self.flush_period_s

    def close(self, status, health=None, can_health=None):
        try:
            self.file.flush()
            if self.fsync:
                os.fsync(self.file.fileno())
        finally:
            self.file.close()
            self.metadata.update(frames=self.frames, ended_unix_ns=time.time_ns(), status=status)
            if health is not None:
                self.metadata["last_observed_health"] = health
            if can_health is not None:
                self.metadata["last_observed_can_health"] = can_health
            self.save_metadata()


def run_ros():
    # Resolve the package before importing rospy/init_node so default ROS logs stay local.
    import rospkg
    package = Path(rospkg.RosPack().get_path("ioniq5_ecan"))
    os.environ["ROS_HOME"] = str(package / "log/ros")
    os.environ["ROS_LOG_DIR"] = str(package / "log/ros/log")
    import rospy
    from ioniq5_ecan.msg import RawCanFrame
    rospy.init_node("ecan_can_logger")
    output_dir = rospy.get_param("~output_dir", str(package / "log/can_logger"))
    bus = int(rospy.get_param("~bus", 0))
    duration_s = float(rospy.get_param("~duration_s", 0.0))
    flush_s = float(rospy.get_param("~flush_period_s", 1.0))
    read_ms = int(rospy.get_param("~read_timeout_ms", 20))
    if bus not in range(-1, 8) or duration_s < 0 or flush_s <= 0 or not 1 <= read_ms <= 1000:
        raise ValueError("Invalid bus/duration/flush/read-timeout parameter")
    publisher = None
    if rospy.get_param("~publish", True):
        publisher = rospy.Publisher(rospy.get_param("~topic", "/ioniq5/can_logger/rx"),
                                    RawCanFrame, queue_size=256, latch=False)
    panda = ReceiveOnlyPanda(rospy.get_param("~serial", ""), read_ms)
    recorder = None
    final_health = final_can = None
    status = "shutdown"
    try:
        while not rospy.is_shutdown():
            try:
                initial = panda.open()
                break
            except Exception as error:
                rospy.logwarn_throttle(5.0, "CAN recorder waiting: %s", error)
                time.sleep(1.0)  # Wall clock: retry also works without a /clock publisher.
        if rospy.is_shutdown():
            return
        discarded = panda.synchronize(rospy.is_shutdown)
        final_health, final_can = panda.health(), panda.can_health()
        ReceiveOnlyPanda.check_silent(final_health)
        initial.update(health_at_capture_start=final_health, can_health_at_capture_start=final_can,
                       discarded_startup_usb_bytes=discarded, bus_filter=bus,
                       usb_control_writes=0, can_transmission_calls=0)
        recorder = CsvRecorder(output_dir, initial, flush_s, bool(rospy.get_param("~fsync", False)))
        decoder = PacketDecoder()
        started, health_due = time.monotonic(), time.monotonic()
        rospy.loginfo("Receive-only CAN capture: %s", recorder.path)
        while not rospy.is_shutdown():
            if duration_s and time.monotonic()-started >= duration_s:
                status = "duration_complete"
                break
            data, _ = panda.read_chunk()
            unix_ns, monotonic_ns, ros_ns = time.time_ns(), time.monotonic_ns(), rospy.Time.now().to_nsec()
            for frame in decoder.feed(data, unix_ns, monotonic_ns, ros_ns):
                if bus >= 0 and frame.bus != bus:
                    continue
                # Disk capture precedes optional ROS publication; ROS queue loss is separate.
                recorder.write(frame)
                if publisher is not None:
                    message = RawCanFrame()
                    message.stamp = rospy.Time(frame.ros_time_ns // 1_000_000_000,
                                              frame.ros_time_ns % 1_000_000_000)
                    for key in ("address", "bus", "fd", "extended", "returned", "rejected"):
                        setattr(message, key, getattr(frame, key))
                    message.data = list(frame.data)
                    publisher.publish(message)
            recorder.flush_if_due()
            if time.monotonic() >= health_due:
                final_health, final_can = panda.health(), panda.can_health()
                ReceiveOnlyPanda.check_silent(final_health)
                if final_health["rx_buffer_overflow"] > initial["health_at_capture_start"]["rx_buffer_overflow"]:
                    rospy.logwarn_throttle(5.0, "Panda RX overflow increased; capture is not loss-free")
                health_due = time.monotonic()+1.0
    except (rospy.ROSInterruptException, KeyboardInterrupt):
        status = "shutdown"
    except Exception as error:
        status = "error: " + str(error)
        rospy.logerr("CAN capture stopped: %s", error)
        raise
    finally:
        try:
            if recorder is not None:
                recorder.close(status, final_health, final_can)
                rospy.loginfo("Saved %d RAW frames", recorder.frames)
        finally:
            panda.close()


if __name__ == "__main__":
    run_ros()
