# Receive-only ECAN in the existing sensor suite

Implemented on the vehicle computer on 2026-10-07 with explicit user approval
for the relevant `multi_camera_recorder` hooks. No bashrc/global ROS, firmware,
actuation behavior, packages or unrelated projects were changed.

## Commands

Use the existing commands in separate terminals. Do not `source` the launchers.

| Command | ECAN addition |
| --- | --- |
| `sensors_all` | Starts one USB-IN-only logger alongside the existing sensors |
| `sensors_check` | Adds an RX status/vehicle-data panel to the existing dashboard |
| `sensors_bag` | Subscribes to RAW CAN as well as all 13 existing sensor topics |
| `sensors_stop` | Also stops the suite-owned logger, not standalone CAN clients |

`sensors_all` starts sensors and a CAN CSV recorder, **not a sensor rosbag**.
Run `sensors_bag` separately to record the combined bag. Its existing delay and
output-directory options are unchanged. To leave Panda USB available for
Cabana or the ECAN controller instead, use `sensors_all start_ecan:=false`.

The suite uses its existing master `http://192.168.2.102:11311` and
`ROS_IP=192.168.2.102`. The CAN launcher inherits them. Its workspace sourcing
affects only its child process. The dashboard adds only the existing generated
ECAN message path and a pure Python decoder to its own `PYTHONPATH`, retaining
the Ouster/Duro/radar paths and the caller's shell environment.

## Data and storage

- RAW topic: `/ioniq5/can_logger/rx`, `ioniq5_ecan/RawCanFrame`, non-latched.
- CAN source: logical bus 0. Unknown IDs and original bytes remain in the CSV/bag.
- CAN CSV/metadata: package `log/can_logger/` (starts once reception is ready).
- Combined bag: existing `~/sensor_bags/<session>/sensor_suite.bag` by default.

The dashboard shows RX freshness, frames/s, frame/ID counts, latest payload,
wheel-mean speed in km/h, steering-wheel angle in deg, yaw in deg/s,
longitudinal/lateral acceleration in m/s2, brake, raw gear/pedal, EPS torque
and EPS/ACC fault bits. These are decoded observations, **not control targets**.
No decoded ROS feedback publisher or actuation node is started.

The display helper `scripts/can_dashboard_data.py` follows the existing C++
parser/codec layouts. Known messages require the correct length and Hyundai
CRC, except `0x0E5 ESC_02_10ms`, which has no CRC field in the pinned DBC.
IMU/ESC quality flags must be clear. Extended-ID aliases, other buses and
Panda returned/rejected metadata are not decoded as vehicle signals.
Bad known messages increment display counters and do not refresh values.

After 1 second without a valid sample, that value becomes `--`; after 1 second
without any vehicle RX, status becomes `STALE`. Fresh messages restore it
without another dashboard launch. Zero, brake OFF and fault OFF remain valid
values. Rates/freshness describe local callback delivery, also during bag
replay, not vehicle readiness. ID display is capped at 4096 unique IDs.

CAN `stamp` is host USB-read completion ROS time, not ECU/hardware acquisition
time. A common master/bag does not prove physical synchronization or loss-free
recording; USB, ROS queues and disk load can still lose/delay data.

## Optional CAN and lifecycle

Missing Panda or a non-SILENT mode leaves the logger waiting without changing
device settings. Missing ECAN modules leave the dashboard's other sensors
working. Missing CAN never blocks `sensors_bag`; the recorder subscribes even
if RAW CAN appears later. `sensors_bag_check` inspects RAW CAN when present,
and reports it as optional/not recorded otherwise, including older bags.

The logger remains SILENT/controls_allowed=0, USB IN only. **Do not concurrently
run Cabana, pandad, the ECAN controller or another Panda reader.** Logger runtime
USB/checksum/mode/file errors end its capture rather than resetting/resyncing
the hardware. Other sensors remain running. Check the cause, then restart the
suite or run the standalone logger on the same master; no automatic runtime
logger restart was added. Stop a standalone logger separately before starting
the suite again. It is intentionally not a `sensors_stop` target.

Suite cleanup owns the logger child; orphan cleanup requires its exact script
and `__name:=sensor_suite_ecan_logger`, or its launch pair and `logger_name`
argument. Bash 5.0 `wait -n` does not select only the supplied jobs, so the
supervisor now monitors only the required suite/radar processes every 0.5 s.
A required-driver exit still ends the suite; an optional CAN exit does not.

## Deployment and verification

Seven existing files under the Ouster workspace's
`src/multi_camera_recorder` were changed: `scripts/start_sensor_suite.sh`,
`start_sensor_check.sh`, `record_sensor_suite.sh`, `stop_sensor_suite.py`,
`sensor_dashboard.py`, `check_sensor_bag.py`, and `launch/sensor_suite.launch`.
They are outside this ECAN Git checkout. Their original copies are preserved
under the Ioniq5 workspace's `backups/sensor-ecan-integration-20261007.q5NuXr/`.
The scoped deployment diff is `patches/sensor-suite-ecan.patch`; on another
computer, inspect/check it against that package before applying. Do not
overwrite newer local sensor changes.

Offline verification on Ubuntu 20.04/Python 3.8: six decoder unit cases,
CRC against an existing C++ golden vector, mocked startup with missing/failed/
disabled CAN and required-driver failure, mocked bag-topic selection, scoped
stop matching, mocked bag checking with absent/present/wrong-type CAN, and
the offscreen Qt dashboard with mocked ROS subscriptions
including missing-module fallback. Shell/Python syntax and launch XML were
checked. Existing dashboard sensors retained all 12 subscriptions, plus CAN.

No real sensors/ROS master, Panda, firmware or vehicle test was started for
this implementation. Live multi-sensor throughput and co-recording remain
unverified. This integration does not change or validate vehicle control.
