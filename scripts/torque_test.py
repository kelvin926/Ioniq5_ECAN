#!/usr/bin/env python3
"""Steering torque test publisher for the direct_torque ROS path.

Publishes ActuationCommand.lateral as LFA torque counts through the running ECAN node, so the
test uses the same path as the upstream controller. The script adds no amplitude, rate or speed
limits; the node only saturates at the CAN range. Each tick is logged with the latest
/ioniq5/vehicle_state to CSV.
"""
from __future__ import annotations

import argparse
import csv
from datetime import datetime
import math
import os
from pathlib import Path
import sys
import time

PROFILES = ("hold", "square", "ramp", "sine")
STATE_FIELDS = ("valid", "speed_mps", "steering_angle_deg", "steering_rate_deg_s",
                "driver_torque", "eps_torque_nm", "eps_fault", "brake_pressed", "standstill",
                "control_state_name", "lateral_armed", "lateral_control_active",
                "panda_controls_allowed", "panda_safety_tx_blocked")


def profile_value(profile, amplitude, offset, period_s, t):
    """Torque count at time t (s) after the profile starts."""
    if profile == "hold":
        value = amplitude
    elif profile == "square":
        value = amplitude if (t % period_s) < period_s / 2.0 else -amplitude
    elif profile == "ramp":
        value = amplitude * min(t / period_s, 1.0)
    else:
        value = amplitude * math.sin(2.0 * math.pi * t / period_s)
    return offset + value


def parse_args(argv):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--profile", choices=PROFILES, default="hold",
                        help="hold: step to amplitude and hold; square: +/-amplitude; "
                             "ramp: 0 to amplitude over period then hold; sine")
    parser.add_argument("--amplitude", type=float, required=True, help="torque counts")
    parser.add_argument("--offset", type=float, default=0.0, help="torque counts added")
    parser.add_argument("--period-s", type=float, default=2.0)
    parser.add_argument("--duration-s", type=float, default=5.0,
                        help="profile length; 0 runs until Ctrl-C")
    parser.add_argument("--rate-hz", type=float, default=100.0)
    parser.add_argument("--acceleration", type=float, default=0.0,
                        help="published acceleration (m/s^2); used only when SET combined is active")
    parser.add_argument("--no-wait-active", action="store_true",
                        help="start the profile immediately instead of after lateral_control_active")
    parser.add_argument("--tail-zero-s", type=float, default=0.5,
                        help="publish zero torque this long before stopping")
    parser.add_argument("--command-topic", default="/ioniq5/actuation_command")
    parser.add_argument("--state-topic", default="/ioniq5/vehicle_state")
    parser.add_argument("--output-dir", default="", help="CSV directory (default: package log/torque_test)")
    args = parser.parse_args(argv)
    if args.period_s <= 0 or args.rate_hz <= 0 or args.duration_s < 0 or args.tail_zero_s < 0:
        parser.error("period/rate must be positive and duration/tail-zero non-negative")
    return args


def main(argv=None):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    # Resolve the package before importing rospy/init_node so default ROS logs stay local.
    import rospkg
    package = Path(rospkg.RosPack().get_path("ioniq5_ecan"))
    os.environ["ROS_HOME"] = str(package / "log/ros")
    os.environ["ROS_LOG_DIR"] = str(package / "log/ros/log")
    import rospy
    from ioniq5_ecan.msg import ActuationCommand, VehicleState

    rospy.init_node("ecan_torque_test", anonymous=True, disable_signals=True)
    publisher = rospy.Publisher(args.command_topic, ActuationCommand, queue_size=1, latch=False)
    latest = {"state": None}
    rospy.Subscriber(args.state_topic, VehicleState,
                     lambda message: latest.__setitem__("state", message), queue_size=1)

    output_dir = Path(args.output_dir) if args.output_dir else package / "log/torque_test"
    output_dir.mkdir(parents=True, exist_ok=True)
    csv_path = output_dir / ("torque_test_" + datetime.now().strftime("%Y%m%d_%H%M%S") + ".csv")
    sequence = [0]

    def publish(lateral):
        sequence[0] = sequence[0] % 0xFFFFFFFF + 1
        message = ActuationCommand()
        message.stamp = rospy.Time.now()
        message.sequence = sequence[0]
        message.enable = True
        message.lateral = float(lateral)
        message.acceleration = args.acceleration
        publisher.publish(message)

    def state_row():
        state = latest["state"]
        if state is None:
            return [""] * len(STATE_FIELDS)
        return [getattr(state, name) for name in STATE_FIELDS]

    period = 1.0 / args.rate_hz
    print("CSV: %s" % csv_path)
    print("profile=%s amplitude=%g offset=%g period=%gs duration=%gs rate=%gHz"
          % (args.profile, args.amplitude, args.offset, args.period_s, args.duration_s, args.rate_hz))
    with open(csv_path, "w", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(("host_time_s", "phase", "profile_time_s", "commanded_lateral") + STATE_FIELDS)
        start = None
        phase = "run" if args.no_wait_active else "wait"
        if phase == "wait":
            print("Publishing zero torque. Press LDA (or SET) to engage; the profile starts when "
                  "lateral_control_active is true. Ctrl-C stops.")
        next_tick = time.monotonic()
        try:
            while not rospy.is_shutdown():
                now = time.monotonic()
                if phase == "wait":
                    state = latest["state"]
                    if state is not None and state.lateral_control_active:
                        phase = "run"
                        print("lateral_control_active: profile started")
                if phase == "run" and start is None:
                    start = now
                profile_t = now - start if start is not None else 0.0
                if phase == "run" and args.duration_s > 0 and profile_t >= args.duration_s:
                    break
                lateral = (profile_value(args.profile, args.amplitude, args.offset, args.period_s,
                                         profile_t) if phase == "run" else 0.0)
                publish(lateral)
                writer.writerow([time.time(), phase, "%.4f" % profile_t, lateral] + state_row())
                next_tick += period
                time.sleep(max(0.0, next_tick - time.monotonic()))
        except KeyboardInterrupt:
            print("Ctrl-C: sending zero torque")
        tail_end = time.monotonic() + args.tail_zero_s
        while time.monotonic() < tail_end:
            publish(0.0)
            writer.writerow([time.time(), "tail", "", 0.0] + state_row())
            time.sleep(period)
    print("Stopped publishing; the node restores stock control after its command timeout.")
    rospy.signal_shutdown("torque test finished")
    return 0


if __name__ == "__main__":
    sys.exit(main())
