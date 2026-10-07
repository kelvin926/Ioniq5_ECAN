#!/usr/bin/env bash
set -eo pipefail

ECAN_PACKAGE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
if [[ ! -r "$ECAN_PACKAGE/package.xml" && -r "$ECAN_PACKAGE/../share/ioniq5_ecan/package.xml" ]]; then
  ECAN_PACKAGE="$(cd -- "$ECAN_PACKAGE/../share/ioniq5_ecan" && pwd)"
fi
ECAN_WORKSPACE="$(cd -- "$ECAN_PACKAGE/../.." && pwd)"
ECAN_SETUP="$ECAN_WORKSPACE/devel/setup.bash"
if [[ ! -r "$ECAN_SETUP" && -r "$ECAN_WORKSPACE/setup.bash" ]]; then
  ECAN_SETUP="$ECAN_WORKSPACE/setup.bash"
fi
if [[ "${1:-}" == --help || "${1:-}" == -h ]]; then
  echo "Usage: $0 [roslaunch arguments]"
  echo "Examples: publish:=false duration_s:=60 output_dir:=/project/log/can_capture"
  echo "Receive-only Panda SILENT logger; no CAN/UDS/heartbeat/settings writes."
  exit 0
fi
for ECAN_REQUIRED in /opt/ros/noetic/setup.bash "$ECAN_SETUP" "$ECAN_PACKAGE/package.xml"; do
  if [[ ! -r "$ECAN_REQUIRED" ]]; then
    echo "Missing prerequisite: $ECAN_REQUIRED" >&2
    exit 1
  fi
done

# Set these before roslaunch itself starts, not only inside its child node.
export ROS_HOME="$ECAN_PACKAGE/log/ros"
export ROS_LOG_DIR="$ROS_HOME/log"
export PYTHONDONTWRITEBYTECODE=1
export TMPDIR="$ECAN_PACKAGE/log/tmp"
mkdir -p -- "$TMPDIR"
source /opt/ros/noetic/setup.bash
source "$ECAN_SETUP"
set -u
exec roslaunch ioniq5_ecan can_logger.launch "$@"
