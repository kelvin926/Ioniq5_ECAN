#!/usr/bin/env python3
"""Interactive slider for direct_torque steering tests through the running ECAN node.

The slider value is published as ActuationCommand.lateral (LFA torque counts) at 100 Hz, the same
path the upstream controller uses. The script adds no rate or speed limits; the slider spans the
CAN range by default and the node saturates at +/-1021. A tkinter window is used when available;
otherwise a terminal gauge is shown. Every tick is logged with the latest vehicle_state to CSV.
"""
from __future__ import annotations

import argparse
import csv
from datetime import datetime
import os
from pathlib import Path
import signal
import sys
import threading
import time

# Works from source and catkin's Python relay; torque_test.py lives beside this script.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from torque_test import STATE_FIELDS

KEY_STEPS = {"small": 1, "medium": 10, "large": 100}


class TorqueSender:
    """Publishes the current target at a fixed rate on a background thread."""

    def __init__(self, args, rospy, command_type, csv_path):
        self.args = args
        self.rospy = rospy
        self.command_type = command_type
        self.csv_path = csv_path
        self.publisher = rospy.Publisher(args.command_topic, command_type, queue_size=1, latch=False)
        self.target = 0.0
        self.state = None
        self.sequence = 0
        self.stop = threading.Event()
        self.thread = threading.Thread(target=self.run, daemon=True)

    def set_target(self, value):
        self.target = max(-self.args.range, min(self.args.range, float(value)))

    def publish(self, lateral):
        self.sequence = self.sequence % 0xFFFFFFFF + 1
        message = self.command_type()
        message.stamp = self.rospy.Time.now()
        message.sequence = self.sequence
        message.enable = True
        message.lateral = float(lateral)
        message.acceleration = self.args.acceleration
        self.publisher.publish(message)

    def state_row(self):
        state = self.state
        if state is None:
            return [""] * len(STATE_FIELDS)
        return [getattr(state, name) for name in STATE_FIELDS]

    def run(self):
        period = 1.0 / self.args.rate_hz
        with open(self.csv_path, "w", newline="") as handle:
            writer = csv.writer(handle)
            writer.writerow(("host_time_s", "phase", "commanded_lateral") + STATE_FIELDS)
            next_tick = time.monotonic()
            while not self.stop.is_set():
                value = self.target
                self.publish(value)
                writer.writerow([time.time(), "slider", value] + self.state_row())
                next_tick += period
                time.sleep(max(0.0, next_tick - time.monotonic()))
            tail_end = time.monotonic() + self.args.tail_zero_s
            while time.monotonic() < tail_end:
                self.publish(0.0)
                writer.writerow([time.time(), "tail", 0.0] + self.state_row())
                time.sleep(period)

    def status_text(self):
        state = self.state
        if state is None:
            return "vehicle_state: not received"
        return ("state %s | lateral active %s | angle %+.1f deg | rate %+.0f deg/s | "
                "driver %+.0f | EPS %+.2f Nm | eps_fault %s | brake %s | speed %.2f m/s | "
                "Panda allowed %s | tx_blocked %d"
                % (state.control_state_name, state.lateral_control_active,
                   state.steering_angle_deg, state.steering_rate_deg_s, state.driver_torque,
                   state.eps_torque_nm, state.eps_fault, state.brake_pressed, state.speed_mps,
                   state.panda_controls_allowed, state.panda_safety_tx_blocked))


def run_tk(sender):
    import tkinter as tk

    root = tk.Tk()
    root.title("ECAN steering torque slider")
    span = sender.args.range
    value = tk.DoubleVar(value=0.0)

    def apply(new_value):
        sender.set_target(new_value)
        value.set(sender.target)

    tk.Label(root, text="StrTqReqVal torque count (direct_torque). Press LDA to engage.",
             font=("TkDefaultFont", 11)).pack(padx=12, pady=(10, 0))
    command_label = tk.Label(root, text="+0", font=("TkFixedFont", 28, "bold"))
    command_label.pack()
    tk.Scale(root, from_=-span, to=span, orient=tk.HORIZONTAL, resolution=1, length=900,
             showvalue=False, variable=value, command=apply).pack(padx=12)
    buttons = tk.Frame(root)
    buttons.pack(pady=6)
    for step in (-100, -10, -1):
        tk.Button(buttons, text="%+d" % step, width=6,
                  command=lambda s=step: apply(sender.target + s)).pack(side=tk.LEFT, padx=2)
    tk.Button(buttons, text="0", width=8, command=lambda: apply(0.0)).pack(side=tk.LEFT, padx=8)
    for step in (1, 10, 100):
        tk.Button(buttons, text="%+d" % step, width=6,
                  command=lambda s=step: apply(sender.target + s)).pack(side=tk.LEFT, padx=2)
    status_label = tk.Label(root, text="", font=("TkFixedFont", 10), justify=tk.LEFT)
    status_label.pack(padx=12, pady=(4, 10))
    tk.Label(root, text="Keys: Left/Right 1, Up/Down 10, PgUp/PgDn 100, Space 0. "
                        "Closing the window sends 0 and stops publishing.").pack(pady=(0, 10))

    root.bind("<Left>", lambda _event: apply(sender.target - KEY_STEPS["small"]))
    root.bind("<Right>", lambda _event: apply(sender.target + KEY_STEPS["small"]))
    root.bind("<Down>", lambda _event: apply(sender.target - KEY_STEPS["medium"]))
    root.bind("<Up>", lambda _event: apply(sender.target + KEY_STEPS["medium"]))
    root.bind("<Next>", lambda _event: apply(sender.target - KEY_STEPS["large"]))
    root.bind("<Prior>", lambda _event: apply(sender.target + KEY_STEPS["large"]))
    root.bind("<space>", lambda _event: apply(0.0))

    def close():
        sender.stop.set()
        root.destroy()

    def refresh():
        if sender.stop.is_set():
            root.destroy()
            return
        command_label.config(text="%+d" % round(sender.target))
        status_label.config(text=sender.status_text())
        root.after(100, refresh)

    root.protocol("WM_DELETE_WINDOW", close)
    refresh()
    root.mainloop()


def run_terminal(sender):
    import curses

    span = sender.args.range
    keys = {curses.KEY_LEFT: -KEY_STEPS["small"], curses.KEY_RIGHT: KEY_STEPS["small"],
            curses.KEY_DOWN: -KEY_STEPS["medium"], curses.KEY_UP: KEY_STEPS["medium"],
            curses.KEY_NPAGE: -KEY_STEPS["large"], curses.KEY_PPAGE: KEY_STEPS["large"]}

    def loop(screen):
        screen.nodelay(True)
        curses.curs_set(0)
        while not sender.stop.is_set():
            key = screen.getch()
            if key in keys:
                sender.set_target(sender.target + keys[key])
            elif key in (ord(" "), ord("0")):
                sender.set_target(0.0)
            elif key in (ord("q"), ord("Q"), 27):
                sender.stop.set()
                break
            rows, cols = screen.getmaxyx()
            width = max(21, min(cols - 4, 101))
            position = int(round((sender.target + span) / (2.0 * span) * (width - 1)))
            bar = ["-"] * width
            bar[width // 2] = "|"
            bar[position] = "#"
            lines = ["ECAN steering torque (direct_torque). Press LDA to engage.",
                     "",
                     "command %+d count   [-%d .. +%d]" % (round(sender.target), span, span),
                     "[" + "".join(bar) + "]",
                     "",
                     sender.status_text()[:max(0, cols - 1)],
                     "",
                     "Left/Right 1, Up/Down 10, PgUp/PgDn 100, Space/0 zero, q quit (sends 0)"]
            screen.erase()
            for row, text in enumerate(lines[:rows]):
                screen.addstr(row, 0, text[:max(0, cols - 1)])
            screen.refresh()
            time.sleep(0.03)

    curses.wrapper(loop)


def parse_args(argv):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--ui", choices=("auto", "gui", "terminal"), default="auto")
    parser.add_argument("--range", type=float, default=1021.0, help="slider span in counts")
    parser.add_argument("--rate-hz", type=float, default=100.0)
    parser.add_argument("--acceleration", type=float, default=0.0,
                        help="published acceleration (m/s^2); used only when SET combined is active")
    parser.add_argument("--tail-zero-s", type=float, default=0.5,
                        help="publish zero torque this long before stopping")
    parser.add_argument("--command-topic", default="/ioniq5/actuation_command")
    parser.add_argument("--state-topic", default="/ioniq5/vehicle_state")
    parser.add_argument("--output-dir", default="", help="CSV directory (default: package log/torque_test)")
    args = parser.parse_args(argv)
    if args.range <= 0 or args.rate_hz <= 0 or args.tail_zero_s < 0:
        parser.error("range/rate must be positive and tail-zero non-negative")
    return args


def gui_available():
    if not os.environ.get("DISPLAY") and not os.environ.get("WAYLAND_DISPLAY"):
        return False
    try:
        import tkinter  # noqa: F401
    except ImportError:
        return False
    return True


def main(argv=None):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    # Resolve the package before importing rospy/init_node so default ROS logs stay local.
    import rospkg
    package = Path(rospkg.RosPack().get_path("ioniq5_ecan"))
    os.environ["ROS_HOME"] = str(package / "log/ros")
    os.environ["ROS_LOG_DIR"] = str(package / "log/ros/log")
    import rospy
    from ioniq5_ecan.msg import ActuationCommand, VehicleState

    use_gui = args.ui == "gui" or (args.ui == "auto" and gui_available())
    rospy.init_node("ecan_torque_slider", anonymous=True, disable_signals=True)
    output_dir = Path(args.output_dir) if args.output_dir else package / "log/torque_test"
    output_dir.mkdir(parents=True, exist_ok=True)
    csv_path = output_dir / ("torque_slider_" + datetime.now().strftime("%Y%m%d_%H%M%S") + ".csv")
    sender = TorqueSender(args, rospy, ActuationCommand, csv_path)
    rospy.Subscriber(args.state_topic, VehicleState,
                     lambda message: setattr(sender, "state", message), queue_size=1)
    signal.signal(signal.SIGINT, lambda *_unused: sender.stop.set())
    sender.thread.start()
    try:
        run_tk(sender) if use_gui else run_terminal(sender)
    finally:
        sender.stop.set()
        sender.thread.join(timeout=args.tail_zero_s + 2.0)
    print("CSV: %s" % csv_path)
    print("Stopped publishing; the node restores stock control after its command timeout.")
    rospy.signal_shutdown("torque slider finished")
    return 0


if __name__ == "__main__":
    sys.exit(main())
