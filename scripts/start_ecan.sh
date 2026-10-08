#!/usr/bin/env bash
set -eo pipefail

ECAN_PACKAGE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
ECAN_WORKSPACE="$(cd -- "$ECAN_PACKAGE/../.." && pwd)"
ECAN_CONFIG="$ECAN_PACKAGE/config/ioniq5_ecan.yaml"

case "${1:-}" in
  --help|-h)
    echo "Usage: $0 [--check]"
    echo "Starts continuous ROS subscription; LDA toggles steering, SET toggles combined control."
    echo "--check validates host paths only: no ROS master, Panda access or CAN transmission."
    exit 0
    ;;
  --check|"") ;;
  *) echo "Unknown argument: $1" >&2; exit 2 ;;
esac
if (( $# > 1 )); then
  echo "Only --check or --help is supported." >&2
  exit 2
fi

for ECAN_REQUIRED in /opt/ros/noetic/setup.bash "$ECAN_WORKSPACE/devel/setup.bash" "$ECAN_CONFIG"; do
  if [[ ! -r "$ECAN_REQUIRED" ]]; then
    echo "Missing prerequisite: $ECAN_REQUIRED" >&2
    exit 1
  fi
done
if [[ ! -x "$ECAN_WORKSPACE/devel/lib/ioniq5_ecan/ioniq5_ecan_node" ]]; then
  echo "Build this workspace with catkin_make before starting ECAN." >&2
  exit 1
fi

# Do not edit shell/system settings or let ROS write its default ~/.ros files.
export ROS_HOME="$ECAN_WORKSPACE/logs/ros"
export ROS_LOG_DIR="$ROS_HOME/log"
export ROS_TEST_RESULTS_DIR="$ECAN_WORKSPACE/build/test_results"
export PYTHONDONTWRITEBYTECODE=1
export TMPDIR="$ECAN_WORKSPACE/logs/tmp"
mkdir -p -- "$TMPDIR"
source /opt/ros/noetic/setup.bash
source "$ECAN_WORKSPACE/devel/setup.bash"
set -u

if [[ "${1:-}" == --check ]]; then
  echo "ECAN host paths OK; topic /ioniq5/actuation_command, lateral torque count, acceleration m/s^2."
  echo "ROS_HOME=$ROS_HOME"
  exit 0
fi
if pgrep -x _cabana >/dev/null || pgrep -x pandad >/dev/null ||
   pgrep -f '^/[^ ]*/ioniq5_ecan_node([[:space:]]|$)' >/dev/null; then
  echo "Another Panda client is running (Cabana, pandad or ECAN). Close it before starting." >&2
  exit 1
fi

mkdir -p -- "$ROS_LOG_DIR"
echo "Subscribing to /ioniq5/actuation_command continuously; Ctrl+C stops the node."
echo "LDA: steering-only ON/OFF. SET press/release: combined ON/OFF."
echo "First takeover requires fresh commands, physical ON and a stationary vehicle."
echo "Healthy command gaps restore stock communication; input return may resume the same selected session."
exec roslaunch ioniq5_ecan ioniq5_ecan.launch config:="$ECAN_CONFIG"
