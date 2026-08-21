#!/usr/bin/env bash
set -euo pipefail

failures=0

check_command() {
  if command -v "$1" >/dev/null 2>&1; then
    printf '[ok] %s\n' "$1"
  else
    printf '[missing] %s\n' "$1" >&2
    failures=$((failures + 1))
  fi
}

printf 'Ioniq5_ECAN environment check\n'
check_command catkin_make
check_command cmake
check_command pkg-config
check_command python3
check_command roscore
check_command lsusb

if [[ "${ROS_DISTRO:-}" == "noetic" ]]; then
  printf '[ok] ROS_DISTRO=noetic\n'
else
  printf '[warning] expected ROS_DISTRO=noetic, got %s\n' "${ROS_DISTRO:-unset}" >&2
fi

if pkg-config --exists libusb-1.0 2>/dev/null; then
  printf '[ok] libusb-1.0 development files\n'
else
  printf '[missing] libusb-1.0 development files\n' >&2
  failures=$((failures + 1))
fi

if python3 -c 'import usb1' >/dev/null 2>&1; then
  printf '[ok] python3 usb1 module\n'
else
  printf '[missing] python3 usb1 module (run: python3 -m pip install --user libusb1)\n' >&2
  failures=$((failures + 1))
fi

if lsusb 2>/dev/null | grep -Eiq '(bbaa:|3801:)'; then
  printf '[ok] Panda USB device detected\n'
else
  printf '[warning] Panda USB device not detected\n' >&2
fi

if [[ -r /etc/udev/rules.d/99-red-panda.rules ]]; then
  printf '[ok] Red Panda udev rule installed\n'
else
  printf '[warning] /etc/udev/rules.d/99-red-panda.rules is not installed\n' >&2
fi

if (( failures > 0 )); then
  printf '%d required check(s) failed.\n' "$failures" >&2
  exit 1
fi
