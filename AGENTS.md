# Ioniq5_ECAN Agent Instructions

## Communication and scope
- Speak to the user in Korean. Repository documentation and state may be English.
- Never use the middle-dot character (U+00B7).
- Follow the user's current request and preserve earlier accepted decisions.
- Infer intent and complete tasks with the smallest root-cause fix.
- Preserve existing behavior and scope; avoid unrelated changes.
- Do not modify this computer's unrelated files, directory structure, installed packages,
  OS/ROS configuration, or personal settings. The allowed project boundary is in state.json.
- Inside that boundary, change only files directly required by the current request.
- Account for applications' incidental outside-boundary writes. If an outside change is
  required, stop and request explicit new authorization; never infer it from a build/run request.
- For research, prioritize peer-reviewed papers and primary sources.
- Verify key research decisions and distinguish evidence from inference.
- Pin upstream comparisons to a commit; keep research snapshots separate from runtime pins.
- Distinguish model targets, controller outputs, transmitted CAN values, and ECU feedback.
- Distinguish transport reconnection, ECU communication restoration, and control re-engagement.
- Keep implementation QA minimal; avoid slow, broad, or repeated tests/builds
  unless failures or significant risk justify them.
- Existing implementation limits are recorded facts, not additional work to perform.
- Receive-only logging uses the standalone can_logger launch/helper, not the actuation node.
- The logger uses USB IN only in SILENT; never add hidden mode/bitrate/reset/heartbeat/UDS writes.
- Logger timestamps are host USB completion times, not hardware CAN arrival times.
- For confirmation-only requests, inspect and report; do not implement changes.
- Preserve existing uncommitted work. Do not discard it or overwrite unrelated files.

## Persistent context
- Read this file and `state.json` at the start of work in this repository.
- `AGENTS.md` holds durable instructions and project conventions.
- `state.json` holds the current project state, decisions, observations, and open issues.
- Create these repo-root files if absent.
- Update them only on material changes to goals, constraints, decisions, blockers,
  active work, environment, or verified results; do not update them for routine status or cosmetic work.
- Update `state.json` in the same session as a material change.
- Update `AGENTS.md` when such a change affects durable guidance.
- Keep `AGENTS.md` at 200 lines or fewer; replace obsolete guidance instead of appending it.
- Keep `state.json` valid JSON, use ISO dates, and record the observation date and source.
- Separate implemented behavior, proposals, historical results, and unknowns.
- Record firmware build and flash as separate events.
- A firmware version marker does not establish the exact installed binary hash.
- Treat hardware observations as dated snapshots; do not present them as live readings.
- Keep a concise recent event log; retain important older decisions in structured fields.
- Do not copy complete transcripts or repetitive tool output into either file.
- Open issues describe unfinished work; they do not authorize work outside the current request.
- Keep README, topic contracts, configuration comments, and handoff aligned with material changes.
- Documentation must label current source behavior, dated observations, and unverified paths separately.
- Keep README short: ROS input/message/units, control and button flow, startup, verification limits;
  route detailed setup, safety and firmware procedures to docs/.
- On shared computers, use user-requested one-time Git authentication only. Do not save account
  tokens, change global Git credentials, or reuse another user's login; verify the requested account.
- Keep personal Windows paths, Codex session/environment references and personal Codex settings
  out of tracked files. Redact real device serials in published evidence and use local configuration.
- After a privacy rewrite of main, preserve local work and use a fresh clone on other machines;
  do not merge or push pre-redaction history back into the sanitized repository.

## Project baseline
- Vehicle: research Hyundai Ioniq 5, model year 2022, HDA1, EV, radar-SCC.
- Stock RSPA is absent (user-confirmed); this MDPS's PA/parking-command support is unresolved.
- Runtime target: Ubuntu 20.04, ROS 1 Noetic, catkin, roscpp, C++17.
- Keep build/test instructions compatible with target tools; newer tools need explicit prerequisites.
- Windows host-only checks do not establish Ubuntu/ROS runtime compatibility.
- Hardware: Hyundai K camera harness and Red Panda over USB.
- Control path: physical CAN1 / Panda logical bus 0 / ECAN, 500/2000 kbps.
- Current design is ECAN-only; camera bus 2 and non-ECAN forwarding are unused.
- Distinguish logical CAN bus numbers from physical controller numbers.
- Distinguish Panda MCU boot, host USB enumeration and mode-change CAN initialization.
- Independent Panda power does not establish ECU fault recovery; preserve stock passthrough before takeover.
- Select the Panda serial locally; public examples use the `PANDA_SERIAL` placeholder.
- Repository: `https://github.com/kelvin926/Ioniq5_ECAN`.
- Current branch and commit belong in `state.json`, not this durable baseline.

## Input and engagement behavior
- Command topic: `/ioniq5/actuation_command`.
- Existing feedback: `/ioniq5/vehicle_state` at 20 Hz; `stamp` is publication time.
- Its `valid` checks steering/MDPS/wheel/TCS freshness, not every optional signal.
- Required input values: `lateral` and `acceleration`.
- Agreed upstream contract: `lateral` is steering-wheel angle rate in deg/s (not yaw rate
  or target angle), and `acceleration` is longitudinal acceleration in m/s^2.
- Keep the existing ActuationCommand message and `steering_rate_deg_s` active profile.
- Upstream owns command generation/shaping. Vehicle sign validation and publisher cadence
  verification remain pending; recommend non-latched publication at 20 Hz.
- Supported modes: `steering_rate_deg_s`, `steering_rate_rad_s`,
  `curvature_1pm`, and `direct_torque`.
- `unfiltered_input: true` skips host target/acceleration clamps and input smoothing.
- Acceleration has no host jerk slew; `jerk_limit_mps3` is SCC metadata.
- Unfiltered input is not unrestricted value pass-through: lateral conversion,
  torque limits/slew, CAN quantization, engagement, and watchdogs still apply.
- The user wants the upstream controller to own command shaping; avoid redundant
  downstream smoothing or discretionary reshaping and disclose remaining command constraints.
- Default `use_enable_field: false` makes the enable field optional.
- LDA selects/toggles lateral-only control.
- SET selects/toggles combined lateral and longitudinal control.
- The user confirmed press-again ON/OFF toggles, not hold-to-run buttons.
- Keep ROS subscription alive while waiting, OFF, disconnected, and recovering. No received
  command means stock communication/NO_OUTPUT; numeric zero is a valid command, not absence.
- With resume_on_command_return/I5R1 enabled, fresh commands plus physical ON may prepare
  the first stationary ECU takeover. Panda is authoritative for volatile button selection.
- Healthy publisher loss restores stock communication/NO_OUTPUT while retaining button intent.
  Fresh input may retake ownership while moving only after prior verified ACTIVE control in the
  same uninterrupted session and completed stock restoration. OFF revokes this eligibility.
- CAN/USB/harness/ignition faults, restoration failures, CANCEL and expired EPS recovery do not
  auto-resume. Process/USB/Panda restart clears selection. Never replay old targets.
- The user authorized scoped firmware modification/flash and moving command-gap re-takeover.
  The earlier USB-only confirmation applied to that flash, not later sessions. Recheck physical
  connection before any firmware bench/flash; the user has since reported reconnecting Panda.
- Use the workspace-local `ecan` entry point; do not install a global alias or edit shell config.
- Keep ROS runtime files under this workspace by setting ROS_HOME and ROS_LOG_DIR.
- Brake preserves lateral control and latches longitudinal control off.
- Releasing the brake does not resume longitudinal control; SET must be operated again.

## Steering and longitudinal protocol
- Current steering output is torque: `0x12A LFA / StrTqReqVal`, at 100 Hz.
- The host and pinned firmware torque cap of 270 counts is a command-path limit,
  not a verified maximum MDPS motor/output torque; encoding range does not prove ECU acceptance.
- Default path: steering rate -> target angle -> angle feedback -> torque output.
- The ROS node has no direct steering-angle input mode yet.
- `scripts/steering_sweep.py` already tracks target angles using torque feedback.
- Software angle tracking and native MDPS angle-command control are different interfaces.
- Native angle signals exist in the pinned DBC, but support on this vehicle is unresolved.
- Changing the input to angle alone does not increase available steering force.
- Longitudinal output uses `0x1A0 SCC_CONTROL` and `0x160`, at 50 Hz.
- The implementation uses `0x730/0x738` for camera ownership; ECU identity is unverified.
- Pinned upstream Ioniq 5 records use camera `0x7C4`; `0x730` is an ADAS Driving candidate.
- Radar UDS `0x7D0/0x7D8` owns stock `0x1A0`.
- The local ROS node includes dual-ECU communication control, tester-present,
  and radar-then-camera restoration.
- Failed ECU restoration remains pending and retries with 1-30 second capped backoff;
  USB reconnection is automatic, but no actuator output resumes during restoration.
- Confirm valid stock traffic and Panda NO_OUTPUT before finishing restoration.
- Restore both radar and camera communications before confirming stock SCC/LFA;
  camera communication control has also silenced SCC on this vehicle.
- After restoration, check vehicle broadcast fault status separately. Resumed stock traffic
  does not establish restored assistance functionality; never bypass RX readiness to force ACTIVE.
- The I5R1 initial-RX grace applies only to unseen messages for 100 ms after mode change.
  It never grants ready/TX, forgives bad received CAN, or replaces freshness checks.
- USB startup draining is limited to plain NO_OUTPUT before ready. Only a successful short
  transfer is a stream boundary; never resynchronize runtime checksum failures in place.
- For an active temporary MDPS LKA assistance fault, retain CAN ownership and arm,
  pause lateral output, and allow ACTIVE return only within a fixed 3-second window
  with fresh valid commands/CAN and Panda permission. Healthy longitudinal may continue.
- Temporary EPS return must not re-enable brake/ACC-latched longitudinal control,
  ignore operator cancellation, replay old steering targets, or allow raw LFA bypass.
- After a hard fault or an expired temporary-fault window, inhibit automatic rearm;
  require operator acknowledgement,
  explicit rearm, and the existing stationary/physical LDA or SET activation conditions.
- Preserve unknown SCC payload bits by starting from the last stock `0x1A0` template.
- DTC reading transmits diagnostic requests; distinguish it from passive CAN reception.
- Retain raw DTC/status bytes, restore prior Panda settings, and never treat a timeout as no faults.
- Distinguish vehicle DTCs, MDPS broadcast fault flags, and Panda hardware-health faults.
- On a user-authorized DTC clear, preserve before/after records and report acknowledgements.
- No immediate DTC recurrence does not establish that the underlying problem is resolved.
- HDA2 auxiliary frames and the temporary ACAN `0x51` approach were removed
  from the final HDA1 ECAN-only path.

## Important historical context
- Low-speed steering and one straight acceleration to 15 km/h succeeded in earlier work.
- Intermittent acceleration failure remained; later dual-ECU success is not recorded.
- User correction: `brake=True` followed failed acceleration because the user intervened.
  It must not be treated as the established cause of that failure.
- HVAC UI/LED/cooling recovered after replacing the OBD-C cable.
- On 2026-10-06 the user reported resolution after fuse replacement; passive capture
  confirmed ignition detection and stock LFA/SCC reception returned. DTCs were not re-read.
- The manufacturer removed an earlier control gateway; its interface remains unknown.
- P-CAN/gateway work was deferred in the final discussion; ECAN `0x1A0` was the priority.
- Strong MDPS position holding remains a separate unresolved requirement.
- Current worktree status, firmware build/flash status, USB observations,
  and exact hashes belong in `state.json`.

## Key files and references
- `config/ioniq5_ecan.yaml`: current input, tuning, and engagement configuration.
- `msg/ActuationCommand.msg`: temporary ROS command contract.
- `src/command_adapter.cpp`: lateral conversion and torque controller.
- `src/hyundai_canfd_codec.cpp`: LFA and SCC frame construction.
- `src/node.cpp`: ROS, USB, engagement, and ECU lifecycle.
- `src/safety_supervisor.cpp`: existing channel engagement behavior.
- `scripts/steering_sweep.py`: earlier vehicle control examples.
- `scripts/can_logger.py` and `docs/can_logger.md`: standalone USB-IN-only ROS RAW recorder.
- `patches/opendbc-hyundai-canfd-split-arm.patch`: custom channel semantics.
- `patches/panda-ecan-only.patch`: ECAN-only firmware behavior.
- `docs/vehicle_handoff.md`: current handoff and dated historical evidence; consult verification scope.
- `docs/vehicle_computer_handoff.md`: Ubuntu 20.04/Noetic transfer and startup procedure.
- `docs/evidence/2026-10-06/`: preserved observations; distinguish pre/post-fuse records.
- `docs/validation.md`: completed verification versus remaining ROS, bench, and vehicle checks.
- Pinned Panda: `dd8a5b3df77706337a11555377e7180c5adc8726`.
- Pinned opendbc: `b72c1fd55ae7e84763e40912bbe06b8f533cb66b`.
- Pinned Carrotpilot: `7fae709b39ec060a0bdd8cc141877eefecb72163`.
