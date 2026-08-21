#!/usr/bin/env python3
"""One-shot standstill or guarded low-speed ECAN steering sweep for the pinned Ioniq 5."""

from __future__ import annotations

import argparse
import math
import sys
import time
from typing import Optional

from opendbc.can.packer import CANPacker
from opendbc.can.parser import get_raw_value
from opendbc.can.packer import set_value
from opendbc.car.hyundai.hyundaicanfd import hkg_can_fd_checksum
from panda import Panda


PINNED_VERSION = "IONIQ5ECAN-dd8a5b3d-DEBUG"
SERIAL_LENGTH = 24
HYUNDAI_CANFD = 28
NO_OUTPUT = 19
ELM327 = 3
ELM327_ECAN_PARAM = 1
# HDA1: camera 0x730 owns LFA while radar 0x7D0 owns SCC_CONTROL.
# Both stock transmitters are silenced before bounded steering/longitudinal TX.
ECAN_STEERING_TEST_PARAM = 1 | 4 | 1024 | 2048
IGNORED_NON_ECAN_FAULTS = (1 << 3) | (1 << 4)
TESTER_PRESENT = b"\x02\x3E\x80\x00\x00\x00\x00\x00"
MAX_STEER_TORQUE = 270
MAX_ANGLE_FOR_CONTINUOUS_REQUEST_DEG = 85.0
MAX_ANGLE_REQUEST_FRAMES = 89
MAX_ANGLE_REQUEST_CUT_FRAMES = 2


class SweepError(RuntimeError):
  pass


def clip(value: float, lower: float, upper: float) -> float:
  return min(max(value, lower), upper)


def rate_limit_torque(desired: int, previous: int) -> int:
  if previous > 0:
    return int(clip(desired, previous - 3, previous + 2))
  if previous < 0:
    return int(clip(desired, previous - 2, previous + 3))
  return int(clip(desired, -2, 2))


def make_lfa(packer: CANPacker, torque: int, enabled: bool, steer_request: bool):
  return packer.make_can_msg("LFA", 0, {
    "LKA_OptUsmSta": 2,
    "LKA_SysIndReq": 2 if enabled else 1,
    "StrTqReqVal": torque,
    "LKA_SysWrn": 0,
    "ActToiSta": 1 if steer_request else 0,
    "LKA_UsmMod": 0,
    "LKA_RcgSta": 0,
    "Damping_Gain": 100,
  })


def make_cluster(packer: CANPacker, active: bool):
  return packer.make_can_msg("LFAHDA_CLUSTER", 0, {
    "HDA_ICON": 1 if active else 0,
    "LFA_ICON": 2 if active else 0,
  })


def make_scc_control(packer: CANPacker, stock_template: bytes, counter: int,
                     accel_raw: float, accel_value: float, enabled: bool,
                     set_speed_kph: float, stopping: bool = False):
  values = {
    "COUNTER": counter,
    "ACCMode": 1 if enabled else 0,
    "MainMode_ACC": 1,
    "StopReq": 1 if enabled and stopping else 0,
    "aReqValue": accel_value if enabled else 0.0,
    "aReqRaw": accel_raw if enabled else 0.0,
    "VSetDis": set_speed_kph,
    "JerkLowerLimit": 5.0 if enabled else 1.0,
    "JerkUpperLimit": 3.0,
    "ACC_ObjDist": 1.0,
    "ObjValid": 0,
    "OBJ_STATUS": 2,
    "SET_ME_2": 4,
    "SET_ME_3": 3,
    "SET_ME_TMP_64": 0x64,
    "DISTANCE_SETTING": 4,
  }
  desired = packer.pack(0x1A0, values)
  data = bytearray(stock_template)
  message = packer.dbc.addr_to_msg[0x1A0]
  for name in values:
    signal = message.sigs[name]
    set_value(data, signal, get_raw_value(desired, signal))
  checksum_signal = message.sigs["CHECKSUM"]
  set_value(data, checksum_signal, 0)
  checksum = hkg_can_fd_checksum(0x1A0, checksum_signal, data)
  set_value(data, checksum_signal, checksum)
  return 0x1A0, bytes(data), 0


def make_fca_warning(packer: CANPacker):
  return packer.make_can_msg("ADRV_0x160", 0, {
    "AEB_SETTING": 1,
    "SET_ME_2": 2,
    "SET_ME_FF": 0xFF,
    "SET_ME_FC": 0xFC,
    "SET_ME_9": 0x09,
  })


def require_stationary_before_adas_disable(panda: Panda, require_drive: bool = False,
                                           timeout_s: float = 1.0) -> Optional[bytes]:
  angle_deg: Optional[float] = None
  max_wheel_kph: Optional[float] = None
  gear: Optional[int] = None
  accelerator_pedal: Optional[int] = None
  scc_control_template: Optional[bytes] = None
  deadline = time.monotonic() + timeout_s
  while time.monotonic() < deadline:
    for address, data, bus in panda.can_recv():
      if bus == 0 and address == 0x1A0 and len(data) == 32:
        scc_control_template = bytes(data)
      if bus != 0:
        continue
      if address == 0x125 and len(data) == 16:
        angle_deg = int.from_bytes(data[3:5], "little", signed=True) * 0.1
      elif address == 0xA0 and len(data) == 24:
        max_wheel_kph = max(
          (int.from_bytes(data[index:index + 2], "little") & 0x3FFF) * 0.03125
          for index in (8, 10, 12, 14)
        )
      elif address == 0x35 and len(data) == 32:
        accelerator_pedal = data[5]
        gear = data[24] & 0x7
    if angle_deg is not None and max_wheel_kph is not None and (
      not require_drive or (
        gear is not None and scc_control_template is not None
      )
    ):
      break
    time.sleep(0.005)
  if angle_deg is None or max_wheel_kph is None:
    raise SweepError("standstill preflight did not receive steering angle and wheel speeds")
  if max_wheel_kph > 0.5:
    raise SweepError(f"vehicle is moving before ADAS disable: {max_wheel_kph:.2f} km/h")
  if require_drive and gear != 5:
    raise SweepError(
      f"longitudinal test must start in D before ADAS disable; got gear={gear}. "
      "Hold the brake, select D, then run the command again without shifting afterward"
    )
  if require_drive and scc_control_template is None:
    raise SweepError("longitudinal test did not receive stock ECAN 0x1A0 before ADAS disable")
  print(
    f"PREFLIGHT_STANDSTILL angle={angle_deg:.1f} deg max_wheel={max_wheel_kph:.2f} km/h "
    f"gear={gear} pedal={accelerator_pedal}",
    flush=True,
  )
  return scc_control_template


class EcuSession:
  def __init__(self, panda: Panda, label: str, request_addr: int,
               response_addr: int, quiet_addr: int):
    self.panda = panda
    self.label = label
    self.request_addr = request_addr
    self.response_addr = response_addr
    self.quiet_addr = quiet_addr
    self.disabled = False
    self.last_tester_present = 0.0

  def _send_single_frame(self, payload: bytes) -> None:
    if not 1 <= len(payload) <= 7:
      raise ValueError("only single-frame UDS payloads are supported")
    data = bytes([len(payload)]) + payload + bytes(7 - len(payload))
    self.panda.can_send(self.request_addr, data, 0)

  def _request(self, payload: bytes, expected: bytes, timeout_s: float = 0.5) -> bytes:
    self._send_single_frame(payload)
    deadline = time.monotonic() + timeout_s
    while time.monotonic() < deadline:
      for address, data, bus in self.panda.can_recv():
        if bus != 0 or address != self.response_addr or len(data) != 8:
          continue
        payload_length = data[0] & 0x0F
        if data[0] & 0xF0 or payload_length > 7:
          continue
        response = bytes(data[1:1 + payload_length])
        if response.startswith(b"\x7F"):
          raise SweepError(f"{self.label} UDS negative response: {response.hex(' ')}")
        if response.startswith(expected):
          return response
      time.sleep(0.005)
    raise SweepError(f"{self.label} UDS response timeout for {payload.hex(' ')}")

  def _send_tester_present(self) -> None:
    self.panda.can_send(self.request_addr, TESTER_PRESENT, 0)
    self.last_tester_present = time.monotonic()

  def maybe_keep_alive(self) -> None:
    if self.disabled and time.monotonic() - self.last_tester_present >= 0.8:
      self._send_tester_present()

  def disable(self) -> None:
    self.panda.set_safety_mode(ELM327, ELM327_ECAN_PARAM)
    self._request(b"\x10\x03", b"\x50\x03")
    # 0x80 requests a suppressed positive response. Quieting the owned frame confirms success.
    self._send_single_frame(b"\x28\x83\x01")
    self.disabled = True
    self._send_tester_present()
    time.sleep(0.15)
    self.panda.can_recv()
    observed = 0
    deadline = time.monotonic() + 0.35
    while time.monotonic() < deadline:
      self.maybe_keep_alive()
      observed += sum(
        1 for address, _data, bus in self.panda.can_recv()
        if bus == 0 and address == self.quiet_addr
      )
      time.sleep(0.005)
    if observed:
      raise SweepError(
        f"{self.label} ECU disable failed: observed {observed} stock "
        f"0x{self.quiet_addr:X} frames"
      )
    print(
      f"{self.label}_DISABLED address=0x{self.request_addr:X} "
      f"stock_0x{self.quiet_addr:X}=quiet",
      flush=True,
    )

  def restore(self) -> None:
    if not self.disabled:
      return
    self.panda.set_safety_mode(ELM327, ELM327_ECAN_PARAM)
    response = self._request(b"\x28\x00\x01", b"\x68\x00")
    self._request(b"\x10\x01", b"\x50\x01")
    self.disabled = False
    resumed = 0
    deadline = time.monotonic() + 0.5
    while time.monotonic() < deadline and resumed == 0:
      resumed += sum(
        1 for address, _data, bus in self.panda.can_recv()
        if bus == 0 and address == self.quiet_addr
      )
      time.sleep(0.005)
    if resumed == 0:
      raise SweepError(
        f"{self.label} restore response {response.hex(' ')} received but stock "
        f"0x{self.quiet_addr:X} did not resume"
      )
    print(f"{self.label}_RESTORED stock_0x{self.quiet_addr:X}={resumed}", flush=True)


class SteeringSweep:
  def __init__(self, panda: Panda, ecu_sessions: tuple[EcuSession, ...], offset_deg: float,
               rolling_test: bool, rolling_min_kph: float, rolling_max_kph: float,
               target_speed_kph: float, accel_max_mps2: float,
               scc_control_template: Optional[bytes]):
    self.panda = panda
    self.ecu_sessions = ecu_sessions
    self.offset_deg = offset_deg
    self.rolling_test = rolling_test
    self.rolling_min_kph = rolling_min_kph
    self.rolling_max_kph = rolling_max_kph
    self.target_speed_kph = target_speed_kph
    self.accel_max_mps2 = accel_max_mps2
    self.longitudinal_test = target_speed_kph > 0.0
    self.scc_control_template = scc_control_template
    self.scc_control_counter = (
      ((scc_control_template[2] + 1) & 0xFF) if scc_control_template is not None else 0
    )
    self.packer = CANPacker("hyundai_canfd_generated")
    self.angle_deg: Optional[float] = None
    self.max_wheel_kph: Optional[float] = None
    self.driver_torque: Optional[int] = None
    self.gear: Optional[int] = None
    self.accelerator_pedal: Optional[int] = None
    self.brake_pressed: Optional[bool] = None
    self.cruise_button: Optional[int] = None
    self.angle_time = 0.0
    self.wheel_time = 0.0
    self.frame = 0
    self.applied_torque = 0
    self.torque_polarity = 1
    self.last_heartbeat = 0.0
    self.last_health = 0.0
    self.initial_tx_blocked = 0
    self.accel_last = 0.0
    self.angle_limit_counter = 0
    self.torque_polarity_checked = False
    self.rejected_tx_addresses: set[int] = set()

  def update_state(self) -> None:
    now = time.monotonic()
    for address, data, bus in self.panda.can_recv():
      if bus >= 192:
        self.rejected_tx_addresses.add(address)
        continue
      if bus != 0:
        continue
      if address == 0x125 and len(data) == 16:
        self.angle_deg = int.from_bytes(data[3:5], "little", signed=True) * 0.1
        self.angle_time = now
      elif address == 0xA0 and len(data) == 24:
        self.max_wheel_kph = max(
          (int.from_bytes(data[index:index + 2], "little") & 0x3FFF) * 0.03125
          for index in (8, 10, 12, 14)
        )
        self.wheel_time = now
      elif address == 0xEA and len(data) == 24:
        self.driver_torque = (int.from_bytes(data[10:12], "little") & 0x1FFF) - 4095
      elif address == 0x35 and len(data) == 32:
        self.accelerator_pedal = data[5]
        self.gear = data[24] & 0x7
      elif address == 0x175 and len(data) == 24:
        self.brake_pressed = bool((data[10] >> 1) & 1)
      elif address == 0x1CF and len(data) == 8:
        self.cruise_button = data[2] & 0x7

  def send_control(self, torque: int, active: bool, heartbeat_engaged: bool,
                   longitudinal_enabled: bool = False, accel_raw: float = 0.0,
                   accel_value: float = 0.0, stopping: bool = False) -> None:
    steer_request = active
    if active and self.angle_deg is not None and (
      abs(self.angle_deg) >= MAX_ANGLE_FOR_CONTINUOUS_REQUEST_DEG
    ):
      self.angle_limit_counter += 1
    else:
      self.angle_limit_counter = 0
    if self.angle_limit_counter > MAX_ANGLE_REQUEST_FRAMES:
      steer_request = False
    if self.angle_limit_counter >= (
      MAX_ANGLE_REQUEST_FRAMES + MAX_ANGLE_REQUEST_CUT_FRAMES
    ):
      self.angle_limit_counter = 0

    frames = [make_lfa(self.packer, torque, active, steer_request)]
    if self.frame % 5 == 0:
      frames.append(make_cluster(self.packer, active))
    if self.longitudinal_test and self.frame % 2 == 0:
      assert self.scc_control_template is not None
      frames.append(make_scc_control(
        self.packer, self.scc_control_template, self.scc_control_counter,
        accel_raw, accel_value, longitudinal_enabled, self.target_speed_kph, stopping,
      ))
      self.scc_control_counter = (self.scc_control_counter + 1) & 0xFF
      frames.append(make_fca_warning(self.packer))
    self.panda.can_send_many(frames, fd=True)
    now = time.monotonic()
    if now - self.last_heartbeat >= 0.25:
      self.panda.send_heartbeat(heartbeat_engaged)
      self.last_heartbeat = now
    for session in self.ecu_sessions:
      session.maybe_keep_alive()
    self.frame += 1

  def verify_runtime(self, require_controls: bool, allow_rolling: bool = False,
                     require_rolling: bool = False) -> None:
    now = time.monotonic()
    if self.angle_deg is None or now - self.angle_time > 0.25:
      raise SweepError("STEERING_SENSORS timeout")
    if self.max_wheel_kph is None or now - self.wheel_time > 0.25:
      raise SweepError("WHEEL_SPEEDS timeout")
    if require_rolling and self.max_wheel_kph < self.rolling_min_kph:
      raise SweepError(
        f"rolling speed fell below {self.rolling_min_kph:.1f} km/h: "
        f"{self.max_wheel_kph:.2f} km/h"
      )
    speed_limit = self.rolling_max_kph if allow_rolling or require_rolling else 0.5
    if self.max_wheel_kph > speed_limit:
      raise SweepError(
        f"speed limit {speed_limit:.1f} km/h exceeded: {self.max_wheel_kph:.2f} km/h"
      )
    if now - self.last_health >= 0.2:
      health = self.panda.health()
      effective_faults = health["faults"] & ~IGNORED_NON_ECAN_FAULTS
      if health["car_harness_status"] != 1:
        raise SweepError(f"harness orientation changed to {health['car_harness_status']}")
      if int(health["safety_mode"]) != HYUNDAI_CANFD or health["safety_param"] != ECAN_STEERING_TEST_PARAM:
        raise SweepError(f"safety mode drifted to {health['safety_mode']}/{health['safety_param']}")
      if effective_faults or health["safety_rx_checks_invalid"]:
        raise SweepError(
          f"Panda safety fault: faults={health['faults']} rx_invalid={health['safety_rx_checks_invalid']}"
        )
      if require_controls and not health["controls_allowed"]:
        rejected = ",".join(
          f"0x{address:X}" for address in sorted(self.rejected_tx_addresses)
        ) or "none"
        raise SweepError(
          "Panda controls_allowed became false: "
          f"pedal={self.accelerator_pedal} brake={self.brake_pressed} "
          f"gear={self.gear} button={self.cruise_button} rejected={rejected}"
        )
      if health["safety_tx_blocked"] != self.initial_tx_blocked:
        rejected = ",".join(
          f"0x{address:X}" for address in sorted(self.rejected_tx_addresses)
        ) or "unknown"
        raise SweepError(
          f"Panda blocked CAN address(es): {rejected}; "
          f"controls={health['controls_allowed']} pedal={self.accelerator_pedal} "
          f"brake={self.brake_pressed} gear={self.gear} button={self.cruise_button}"
        )
      self.rejected_tx_addresses.clear()
      self.last_health = now

  def inactive_tick(self, heartbeat_engaged: bool = False) -> None:
    self.update_state()
    self.applied_torque = rate_limit_torque(0, self.applied_torque)
    self.send_control(self.applied_torque, False, heartbeat_engaged)

  def wait_for_arm(self, timeout_s: float, rolling: bool = False) -> None:
    arm_button = "SET" if self.longitudinal_test else "LDA"
    if rolling:
      print(
        f"주행 속도를 유지하면서 {arm_button} 버튼을 지금 한 번 눌렀다가 놓으세요.",
        flush=True,
      )
    else:
      print(f"{arm_button} 버튼을 지금 한 번 눌렀다가 놓으세요.", flush=True)
    deadline = time.monotonic() + timeout_s
    next_tick = time.monotonic()
    while time.monotonic() < deadline:
      self.inactive_tick()
      self.verify_runtime(
        require_controls=False, allow_rolling=rolling, require_rolling=rolling,
      )
      health = self.panda.health()
      if health["controls_allowed"]:
        self.panda.send_heartbeat(True)
        self.last_heartbeat = time.monotonic()
        print(f"{arm_button}_ARMED", flush=True)
        return
      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError("LDA arm timeout")

  def wait_for_rolling(self, timeout_s: float = 30.0) -> None:
    print(
      f"ROLLING_WAIT: release the brake and hold {self.rolling_min_kph:.1f}-"
      f"{self.rolling_max_kph:.1f} km/h; then press "
      f"{'SET' if self.longitudinal_test else 'LDA'} when prompted",
      flush=True,
    )
    deadline = time.monotonic() + timeout_s
    next_tick = time.monotonic()
    while time.monotonic() < deadline:
      self.update_state()
      self.send_control(0, False, False)
      self.verify_runtime(require_controls=False, allow_rolling=True)
      assert self.max_wheel_kph is not None
      if self.rolling_min_kph <= self.max_wheel_kph <= self.rolling_max_kph:
        print(f"ROLLING_READY speed={self.max_wheel_kph:.2f} km/h", flush=True)
        return
      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError("rolling speed entry timeout")

  def move_to(self, label: str, target_deg: float, timeout_s: float = 12.0) -> None:
    if self.angle_deg is None:
      raise SweepError("steering angle is unavailable")
    start_angle = self.angle_deg
    initial_error = target_deg - start_angle
    polarity_checked = False
    stable_since: Optional[float] = None
    last_progress = 0.0
    deadline = time.monotonic() + timeout_s
    next_tick = time.monotonic()
    print(f"{label}: target={target_deg:.1f} deg start={start_angle:.1f} deg", flush=True)

    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True, require_rolling=self.rolling_test)
      assert self.angle_deg is not None
      error = target_deg - self.angle_deg

      if not polarity_checked and abs(self.angle_deg - start_angle) >= 1.0:
        if (self.angle_deg - start_angle) * initial_error < 0.0:
          self.torque_polarity *= -1
          print(f"torque polarity corrected to {self.torque_polarity:+d}", flush=True)
        polarity_checked = True

      desired = int(round(clip(7.0 * error * self.torque_polarity, -260.0, 260.0)))
      self.applied_torque = rate_limit_torque(desired, self.applied_torque)
      self.send_control(self.applied_torque, True, True)

      if time.monotonic() - last_progress >= 0.5:
        print(
          f"{label}: angle={self.angle_deg:.1f} error={error:.1f} torque={self.applied_torque}",
          flush=True,
        )
        last_progress = time.monotonic()

      if abs(error) <= 2.0:
        if stable_since is None:
          stable_since = time.monotonic()
        elif time.monotonic() - stable_since >= 0.3:
          print(f"{label}: reached={self.angle_deg:.1f} deg", flush=True)
          return
      else:
        stable_since = None

      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError(f"{label} target timeout at {self.angle_deg:.1f} deg")

  def drive_toward_for(self, label: str, target_deg: float, duration_s: float) -> None:
    if self.angle_deg is None:
      raise SweepError("steering angle is unavailable")
    start_angle = self.angle_deg
    initial_error = target_deg - start_angle
    polarity_checked = False
    deadline = time.monotonic() + duration_s
    next_tick = time.monotonic()
    last_progress = 0.0
    print(
      f"{label}_PHASE: target={target_deg:.1f} deg duration={duration_s:.1f} s",
      flush=True,
    )
    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True, require_rolling=self.rolling_test)
      assert self.angle_deg is not None
      error = target_deg - self.angle_deg
      if not polarity_checked and abs(self.angle_deg - start_angle) >= 1.0:
        if (self.angle_deg - start_angle) * initial_error < 0.0:
          self.torque_polarity *= -1
          print(f"torque polarity corrected to {self.torque_polarity:+d}", flush=True)
        polarity_checked = True
      desired = int(round(clip(7.0 * error * self.torque_polarity, -260.0, 260.0)))
      self.applied_torque = rate_limit_torque(desired, self.applied_torque)
      self.send_control(self.applied_torque, True, True)
      if time.monotonic() - last_progress >= 0.5:
        print(
          f"{label}_PHASE: angle={self.angle_deg:.1f} torque={self.applied_torque}",
          flush=True,
        )
        last_progress = time.monotonic()
      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    print(f"{label}_PHASE_COMPLETE", flush=True)

  def drive_max_torque_for(self, label: str, direction: int, hold_s: float) -> None:
    if self.angle_deg is None:
      raise SweepError("steering angle is unavailable")
    start_angle = self.angle_deg
    hold_started: Optional[float] = None
    deadline = time.monotonic() + hold_s + 4.0
    next_tick = time.monotonic()
    last_progress = 0.0
    print(
      f"{label}: max_torque={MAX_STEER_TORQUE} hold={hold_s:.1f} s",
      flush=True,
    )
    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True)
      assert self.angle_deg is not None

      if not self.torque_polarity_checked and abs(self.angle_deg - start_angle) >= 1.0:
        if (self.angle_deg - start_angle) * direction < 0.0:
          self.torque_polarity *= -1
          print(f"torque polarity corrected to {self.torque_polarity:+d}", flush=True)
        self.torque_polarity_checked = True

      target_torque = direction * self.torque_polarity * MAX_STEER_TORQUE
      self.applied_torque = rate_limit_torque(target_torque, self.applied_torque)
      self.send_control(self.applied_torque, True, True)

      now = time.monotonic()
      if self.applied_torque == target_torque:
        if hold_started is None:
          hold_started = now
          print(f"{label}: MAX_TORQUE_REACHED angle={self.angle_deg:.1f} deg", flush=True)
        elif now - hold_started >= hold_s:
          print(f"{label}_COMPLETE angle={self.angle_deg:.1f} deg", flush=True)
          return

      if now - last_progress >= 0.5:
        held_s = 0.0 if hold_started is None else now - hold_started
        print(
          f"{label}: angle={self.angle_deg:.1f} torque={self.applied_torque} "
          f"held={held_s:.1f}/{hold_s:.1f} s",
          flush=True,
        )
        last_progress = now

      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError(f"{label} could not hold maximum torque for {hold_s:.1f} s")

  def longitudinal_request(self) -> tuple[float, float]:
    assert self.max_wheel_kph is not None
    speed_error_mps = max(0.0, self.target_speed_kph - self.max_wheel_kph) / 3.6
    accel_raw = 0.0 if self.max_wheel_kph >= self.target_speed_kph - 0.2 else clip(
      0.4 * speed_error_mps, 0.10, self.accel_max_mps2,
    )
    self.accel_last = clip(accel_raw, self.accel_last - 0.01, self.accel_last + 0.01)
    return accel_raw, self.accel_last

  def accelerate_straight(self, center_deg: float, timeout_s: float = 20.0) -> None:
    stable_since: Optional[float] = None
    deadline = time.monotonic() + timeout_s
    next_tick = time.monotonic()
    last_progress = 0.0
    print(
      f"ACCEL_PHASE: target={self.target_speed_kph:.1f} km/h "
      f"max_accel={self.accel_max_mps2:.2f} m/s^2",
      flush=True,
    )
    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True, allow_rolling=True)
      assert self.angle_deg is not None
      assert self.max_wheel_kph is not None

      accel_raw, accel_value = self.longitudinal_request()

      steer_error = center_deg - self.angle_deg
      desired_torque = int(round(clip(3.0 * steer_error, -100.0, 100.0)))
      self.applied_torque = rate_limit_torque(desired_torque, self.applied_torque)
      self.send_control(
        self.applied_torque, True, True, longitudinal_enabled=True,
        accel_raw=accel_raw, accel_value=accel_value,
      )

      if time.monotonic() - last_progress >= 0.5:
        print(
          f"ACCEL_PHASE: speed={self.max_wheel_kph:.2f} km/h "
          f"accel={self.accel_last:.2f} angle={self.angle_deg:.1f} deg",
          flush=True,
        )
        last_progress = time.monotonic()

      if self.max_wheel_kph >= self.target_speed_kph - 0.2 and self.accel_last <= 0.05:
        if stable_since is None:
          stable_since = time.monotonic()
        elif time.monotonic() - stable_since >= 0.3:
          print(f"TARGET_SPEED_REACHED speed={self.max_wheel_kph:.2f} km/h", flush=True)
          return
      else:
        stable_since = None

      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError(
      f"{self.target_speed_kph:.1f} km/h acceleration timeout at "
      f"{self.max_wheel_kph:.2f} km/h"
    )

  def decelerate_to_stop(self, center_deg: float, decel_max_mps2: float,
                         timeout_s: float = 20.0) -> None:
    stopped_since: Optional[float] = None
    deadline = time.monotonic() + timeout_s
    next_tick = time.monotonic()
    last_progress = 0.0
    print(
      f"DECEL_PHASE: target=0.0 km/h max_decel={decel_max_mps2:.2f} m/s^2",
      flush=True,
    )
    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True, allow_rolling=True)
      assert self.angle_deg is not None
      assert self.max_wheel_kph is not None

      speed_mps = self.max_wheel_kph / 3.6
      decel_request = clip(0.35 * speed_mps, 0.15, decel_max_mps2)
      stopping = self.max_wheel_kph <= 2.0
      if self.max_wheel_kph <= 0.3:
        decel_request = min(0.30, decel_max_mps2)
      accel_raw = -decel_request
      self.accel_last = clip(accel_raw, self.accel_last - 0.01, self.accel_last + 0.01)

      steer_error = center_deg - self.angle_deg
      desired_torque = int(round(clip(3.0 * steer_error, -100.0, 100.0)))
      self.applied_torque = rate_limit_torque(desired_torque, self.applied_torque)
      self.send_control(
        self.applied_torque, True, True, longitudinal_enabled=True,
        accel_raw=accel_raw, accel_value=self.accel_last, stopping=stopping,
      )

      now = time.monotonic()
      if now - last_progress >= 0.5:
        print(
          f"DECEL_PHASE: speed={self.max_wheel_kph:.2f} km/h "
          f"accel={self.accel_last:.2f} stop_req={int(stopping)} "
          f"angle={self.angle_deg:.1f} deg",
          flush=True,
        )
        last_progress = now

      if self.max_wheel_kph <= 0.3:
        if stopped_since is None:
          stopped_since = now
        elif now - stopped_since >= 1.0:
          print(
            "VEHICLE_STOPPED: press and hold the brake now; automatic brake release follows",
            flush=True,
          )
          return
      else:
        stopped_since = None

      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    raise SweepError(f"deceleration timeout at {self.max_wheel_kph:.2f} km/h")

  def combined_segment(self, label: str, target_deg: float, duration_s: float) -> None:
    if self.angle_deg is None:
      raise SweepError("steering angle is unavailable")
    start_angle = self.angle_deg
    initial_error = target_deg - start_angle
    polarity_checked = abs(initial_error) < 1.0
    deadline = time.monotonic() + duration_s
    next_tick = time.monotonic()
    last_progress = 0.0
    print(
      f"{label}: target_angle={target_deg:.1f} deg duration={duration_s:.1f} s",
      flush=True,
    )
    while time.monotonic() < deadline:
      self.update_state()
      self.verify_runtime(require_controls=True, require_rolling=True)
      assert self.angle_deg is not None
      assert self.max_wheel_kph is not None

      steer_error = target_deg - self.angle_deg
      if not polarity_checked and abs(self.angle_deg - start_angle) >= 1.0:
        if (self.angle_deg - start_angle) * initial_error < 0.0:
          self.torque_polarity *= -1
          print(f"torque polarity corrected to {self.torque_polarity:+d}", flush=True)
        polarity_checked = True

      desired_torque = int(round(clip(
        7.0 * steer_error * self.torque_polarity, -260.0, 260.0,
      )))
      self.applied_torque = rate_limit_torque(desired_torque, self.applied_torque)
      accel_raw, accel_value = self.longitudinal_request()
      self.send_control(
        self.applied_torque, True, True, longitudinal_enabled=True,
        accel_raw=accel_raw, accel_value=accel_value,
      )

      if time.monotonic() - last_progress >= 0.5:
        print(
          f"{label}: speed={self.max_wheel_kph:.2f} km/h "
          f"accel={accel_value:.2f} angle={self.angle_deg:.1f} deg "
          f"torque={self.applied_torque}",
          flush=True,
        )
        last_progress = time.monotonic()

      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    print(f"{label}_COMPLETE", flush=True)

  def ramp_down(self) -> None:
    next_tick = time.monotonic()
    for _ in range(100):
      self.update_state()
      self.applied_torque = rate_limit_torque(0, self.applied_torque)
      self.send_control(self.applied_torque, self.applied_torque != 0, False)
      if self.applied_torque == 0:
        break
      next_tick += 0.01
      time.sleep(max(0.0, next_tick - time.monotonic()))
    for _ in range(5):
      self.update_state()
      self.send_control(0, False, False)
      time.sleep(0.01)

  def run(self, arm_timeout_s: float, timed_hold_s: float,
          combined_cycles: int, combined_segment_s: float,
          steering_cycles: int, torque_sweep: bool, decel_max_mps2: float) -> None:
    for _ in range(20):
      self.inactive_tick()
      time.sleep(0.01)
    self.verify_runtime(require_controls=False)
    assert self.angle_deg is not None
    center = self.angle_deg
    if not torque_sweep and abs(center) + self.offset_deg > 80.0:
      raise SweepError(f"initial angle {center:.1f} deg is too far from center")
    if torque_sweep and abs(center) > 5.0:
      raise SweepError(f"maximum torque sweep requires steering within +/-5 deg, got {center:.1f}")
    print(
      f"STANDSTILL angle={center:.1f} deg max_wheel={self.max_wheel_kph:.2f} km/h "
      f"driver_torque={self.driver_torque}",
      flush=True,
    )
    if self.longitudinal_test:
      if abs(center) > 5.0:
        raise SweepError(f"straight acceleration requires steering within +/-5 deg, got {center:.1f}")
      self.wait_for_rolling()
      self.wait_for_arm(arm_timeout_s, rolling=True)
      if combined_cycles > 0:
        for cycle in range(1, combined_cycles + 1):
          self.combined_segment(
            f"CYCLE_{cycle}_LEFT", center + self.offset_deg, combined_segment_s,
          )
          self.combined_segment(
            f"CYCLE_{cycle}_STRAIGHT", center, combined_segment_s,
          )
          self.combined_segment(
            f"CYCLE_{cycle}_RIGHT", center - self.offset_deg, combined_segment_s,
          )
          self.combined_segment(
            f"CYCLE_{cycle}_STRAIGHT_AFTER_RIGHT", center, combined_segment_s,
          )
        print("COMBINED_CYCLES_COMPLETE", flush=True)
      else:
        self.accelerate_straight(center)
        self.decelerate_to_stop(center, decel_max_mps2)
        print("STRAIGHT_ACCEL_DECEL_COMPLETE", flush=True)
      return
    if self.rolling_test:
      self.wait_for_rolling()
      self.wait_for_arm(arm_timeout_s, rolling=True)
    else:
      self.wait_for_arm(arm_timeout_s)
    segment_timeout = 6.0 if self.rolling_test else 12.0
    if timed_hold_s > 0.0:
      for cycle in range(1, steering_cycles + 1):
        if torque_sweep:
          self.drive_max_torque_for(f"CYCLE_{cycle}_LEFT", 1, timed_hold_s)
          self.drive_max_torque_for(f"CYCLE_{cycle}_RIGHT", -1, timed_hold_s)
        else:
          self.drive_toward_for(
            f"CYCLE_{cycle}_LEFT", center + self.offset_deg, timed_hold_s,
          )
          self.drive_toward_for(
            f"CYCLE_{cycle}_RIGHT", center - self.offset_deg, timed_hold_s,
          )
      if not torque_sweep:
        self.move_to("CENTER", center, segment_timeout)
      print("TORQUE_SWEEP_COMPLETE" if torque_sweep else "TIMED_SWEEP_COMPLETE", flush=True)
      return
    self.move_to("LEFT", center + self.offset_deg, segment_timeout)
    self.move_to("CENTER", center, segment_timeout)
    self.move_to("RIGHT", center - self.offset_deg, segment_timeout)
    self.move_to("CENTER", center, segment_timeout)
    print("SWEEP_COMPLETE", flush=True)


def main() -> int:
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument("--serial", required=True)
  parser.add_argument("--offset-deg", type=float, default=45.0)
  parser.add_argument("--arm-timeout-s", type=float, default=30.0)
  parser.add_argument("--rolling-test", action="store_true")
  parser.add_argument("--rolling-min-kph", type=float, default=1.0)
  parser.add_argument("--rolling-max-kph", type=float, default=5.0)
  parser.add_argument("--timed-hold-s", type=float, default=0.0)
  parser.add_argument("--target-speed-kph", type=float, default=0.0)
  parser.add_argument("--accel-max-mps2", type=float, default=0.7)
  parser.add_argument("--decel-max-mps2", type=float, default=0.7)
  parser.add_argument("--combined-cycles", type=int, default=0)
  parser.add_argument("--combined-segment-s", type=float, default=2.0)
  parser.add_argument("--steering-cycles", type=int, default=1)
  parser.add_argument("--torque-sweep", action="store_true")
  parser.add_argument("--execute", action="store_true")
  args = parser.parse_args()
  if not args.execute:
    parser.error("--execute is required")
  if len(args.serial) != SERIAL_LENGTH:
    parser.error("--serial must be 24 characters")
  if not math.isfinite(args.offset_deg) or args.offset_deg <= 0.0 or args.offset_deg > 45.0:
    parser.error("--offset-deg must be within (0,45]")
  if not math.isfinite(args.rolling_min_kph) or not math.isfinite(args.rolling_max_kph):
    parser.error("rolling speed limits must be finite")
  rolling_max_limit = args.target_speed_kph + 1.0 if args.target_speed_kph > 0.0 else 10.0
  if args.rolling_min_kph < 0.5 or args.rolling_max_kph > rolling_max_limit:
    parser.error(
      f"rolling speed range must stay within 0.5-{rolling_max_limit:.1f} km/h"
    )
  if args.rolling_min_kph >= args.rolling_max_kph:
    parser.error("--rolling-min-kph must be lower than --rolling-max-kph")
  if not math.isfinite(args.timed_hold_s) or args.timed_hold_s < 0.0 or args.timed_hold_s > 5.0:
    parser.error("--timed-hold-s must be within [0,5]")
  if not math.isfinite(args.target_speed_kph) or args.target_speed_kph < 0.0 or args.target_speed_kph > 15.0:
    parser.error("--target-speed-kph must be within [0,15]")
  if 0.0 < args.target_speed_kph < 5.0:
    parser.error("non-zero --target-speed-kph must be at least 5 km/h")
  if not math.isfinite(args.accel_max_mps2) or not 0.1 <= args.accel_max_mps2 <= 0.7:
    parser.error("--accel-max-mps2 must be within [0.1,0.7]")
  if not math.isfinite(args.decel_max_mps2) or not 0.1 <= args.decel_max_mps2 <= 1.0:
    parser.error("--decel-max-mps2 must be within [0.1,1.0]")
  if not 0 <= args.combined_cycles <= 10:
    parser.error("--combined-cycles must be within [0,10]")
  if not math.isfinite(args.combined_segment_s) or not 0.5 <= args.combined_segment_s <= 5.0:
    parser.error("--combined-segment-s must be within [0.5,5.0]")
  if not 1 <= args.steering_cycles <= 10:
    parser.error("--steering-cycles must be within [1,10]")
  if args.target_speed_kph > 0.0 and not args.rolling_test:
    parser.error("--target-speed-kph requires --rolling-test")
  if args.combined_cycles > 0 and args.target_speed_kph <= 0.0:
    parser.error("--combined-cycles requires a non-zero --target-speed-kph")
  if args.steering_cycles > 1 and args.timed_hold_s <= 0.0:
    parser.error("--steering-cycles above 1 requires --timed-hold-s")
  if args.torque_sweep and args.timed_hold_s <= 0.0:
    parser.error("--torque-sweep requires --timed-hold-s")
  if args.torque_sweep and (args.rolling_test or args.target_speed_kph > 0.0):
    parser.error("--torque-sweep is standstill-only")
  if args.target_speed_kph > 0.0 and args.timed_hold_s > 0.0:
    parser.error("straight acceleration and timed steering modes are mutually exclusive")

  panda = Panda(serial=args.serial, cli=False)
  camera_session = EcuSession(panda, "CAMERA", 0x730, 0x738, 0x12A)
  radar_session = EcuSession(panda, "RADAR", 0x7D0, 0x7D8, 0x1A0)
  ecu_sessions = (camera_session, radar_session) if args.target_speed_kph > 0.0 else (camera_session,)
  sweep: Optional[SteeringSweep] = None
  try:
    if panda.get_version() != PINNED_VERSION:
      raise SweepError(f"unexpected firmware {panda.get_version()!r}")
    health = panda.health()
    if health["car_harness_status"] == Panda.HARNESS_STATUS_FLIPPED:
      raise SweepError(
        "OBD-C orientation is FLIPPED(2); power off the vehicle and rotate the "
        "OBD-C connector 180 degrees so the ECAN-only firmware reports NORMAL(1)"
      )
    if health["car_harness_status"] != 1 or not health["ignition_line"]:
      raise SweepError(
        f"requires NORMAL harness_status=1 and ignition_line=1, got "
        f"{health['car_harness_status']}/{health['ignition_line']}"
      )
    panda.can_reset_communications()
    panda.set_power_save(False)
    panda.set_can_speed_kbps(0, 500)
    panda.set_can_data_speed_kbps(0, 2000)
    panda.set_canfd_auto(0, True)
    panda.can_clear(0xFFFF)
    panda.set_safety_mode(ELM327, ELM327_ECAN_PARAM)
    scc_control_template = require_stationary_before_adas_disable(
      panda, require_drive=args.target_speed_kph > 0.0,
    )
    for session in ecu_sessions:
      session.disable()
    panda.set_safety_mode(HYUNDAI_CANFD, ECAN_STEERING_TEST_PARAM)
    health = panda.health()
    if int(health["safety_mode"]) != HYUNDAI_CANFD or health["safety_param"] != ECAN_STEERING_TEST_PARAM:
      raise SweepError(f"ECAN safety mode rejected: {health['safety_mode']}/{health['safety_param']}")
    runtime_max_kph = (
      args.target_speed_kph + 1.0 if args.target_speed_kph > 0.0 else args.rolling_max_kph
    )
    sweep = SteeringSweep(
      panda, ecu_sessions, args.offset_deg, args.rolling_test,
      args.rolling_min_kph, runtime_max_kph, args.target_speed_kph, args.accel_max_mps2,
      scc_control_template,
    )
    sweep.initial_tx_blocked = health["safety_tx_blocked"]
    sweep.run(
      args.arm_timeout_s, args.timed_hold_s,
      args.combined_cycles, args.combined_segment_s, args.steering_cycles,
      args.torque_sweep, args.decel_max_mps2,
    )
    return 0
  except (SweepError, OSError) as error:
    print(f"SWEEP_ABORTED: {error}", file=sys.stderr, flush=True)
    return 1
  finally:
    if sweep is not None:
      try:
        sweep.ramp_down()
      except Exception as error:  # Best-effort zeroing before relay closure.
        print(f"ramp-down warning: {error}", file=sys.stderr, flush=True)
    try:
      panda.send_heartbeat(False)
    except Exception as error:
      print(f"heartbeat warning: {error}", file=sys.stderr, flush=True)
    for session in reversed(ecu_sessions):
      try:
        session.restore()
      except Exception as error:
        print(
          f"{session.label} restore warning: {error}; ignition cycle is required",
          file=sys.stderr,
          flush=True,
        )
    try:
      panda.set_safety_mode(NO_OUTPUT, 0)
    except Exception as error:
      print(f"NO_OUTPUT warning: {error}", file=sys.stderr, flush=True)
    panda.close()


if __name__ == "__main__":
  raise SystemExit(main())
