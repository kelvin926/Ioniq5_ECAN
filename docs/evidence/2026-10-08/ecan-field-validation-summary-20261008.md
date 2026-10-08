# ECAN field validation (2026-10-08, offline)

Scope: all 604 fields in `config/ecan_signals.json` (97 IDs) against the 2026-10-07 recording
(1,914,473 frames, 7 captures, co-recorded Piksi GNSS) and the 2026-10-08 10 s parked capture
(gear P, ignition on, driver and driver-rear doors reported open, 22:07 KST).
Per-field numbers: `validation_results.json`. Scripts: `protocol.py`, `proto2.py`, `proto3.py`,
`stats.py`, `phys1.py`..`phys10.py`, `assemble.py` (this folder; NumPy only, no device access).
Lag convention: `lag_s` = CAN time minus reference time at best correlation (positive = CAN later).

## Verdict counts

| | confirmed | consistent | unverifiable | contradicted | total |
|---|---:|---:|---:|---:|---:|
| protocol (CRC/counter) | 149 | 7 | 0 | 7 | 163 |
| non-protocol | 56 | 117 | 257 | 11 | 441 |
| all | 205 | 124 | 257 | 18 | 604 |

"confirmed" needs an independent reference (GNSS, gravity, user report, host clock, ISO code)
or an exact rule (CRC, counter, P = V*I, cross-ECU handshake, derivative of a confirmed field).
Most "unverifiable" fields are constant on both days (status/option/warning enums never exercised).

## Protocol results

- Hyundai CAN-FD CRC16: all 67 defined CRC16 fields pass 100% (both days). Undefined IDs
  0x090/0x145/0x1B0/0x2B0/0x31A/0x34A/0x382..0x384/0x3B0..0x3B2/0x427/0x42A also carry CRC16;
  their 0.03..0.2% failures are all checksum = 0xFFFF with an in-sequence counter (sentinel, not corruption).
- CRC8 (poly 0x1D over bytes 1..7, constant XOR per ID): 12 defined fields pass 100%, including
  0x1CF._CHECKSUM (30 bodies, XOR 0x97; Hyundai CRC16 fails 100%) and the CHECKSUM_MAYBE fields of
  0x3C1 (21 bodies), 0x41C (3369) and 0x411 (2 bodies, one per day; weakest case). Init/xorout/data ID
  are not separately identifiable.
- 8-bit counters at 16:8: +1 in 100% of consecutive frames for 66 fields (wrap 255->0), except
  0x3F5 (98.2%, twelve +2 steps at the normal 1 s period). 0x1CF.COUNTER is mod 15 (0..14; 100%).
  0x36F/0x37F mod-15 alive counters: 97.2% / 99.9%.
- Contradicted (7): 0x3C1.COUNTER_ALT, 0x413.COUNTER_ALT, 0x41C.COUNTER_ALT,
  0x412/0x418/0x419/0x435.ALIVE_COUNTER_MOD15 are message-change counters: they change in exactly
  the frames where the payload changes (100%), always by +1, and repeat otherwise. A per-frame
  alive check would reject 47..99% of frames. Navigation 2-bit counters (0x4B4..0x4BF) behave the
  same way (only repeat or +1) and are rated consistent.

## Most important confirmations (non-protocol)

| field | reference | n | r | fit | lag |
|---|---|---:|---:|---|---:|
| 0x0A0 wheel speeds (4, both widths) | GNSS speed | 4369 | 0.9987..0.9992 | slope 0.987..0.999 | -0.1..-0.05 s |
| 0x0A0 left/right labels | GNSS yaw | 4309 | 0.981 | right-left = 1.54 m * yaw | 0.2 s |
| 0x0A0 rotation accumulators (4) | GNSS distance | 4129 | 0.9988..0.9994 | 46 (front) / 40.7 (rear) pulses/m, wrap mod 255 | |
| 0x0DA.VEHICLE_SPEED_MPS_CANDIDATE | GNSS speed | 4369 | 0.9996 | slope 0.993 | 0 |
| 0x1AA.CLU_SPEED (km/h display) | GNSS speed | 4369 | 0.9984 | slope 1.033, +1.7 km/h | 0.2 s |
| 0x435.FILTERED_SPEED_KPH | GNSS speed | 4354 | 0.9987 | slope 0.991 | 0.5 s |
| 0x10A / 0x065 rear motor rpm | GNSS speed | 4369 | 0.9995 | 280.7 rpm per m/s | -0.1 s |
| 0x120 / 0x065 front motor rpm | rear rpm | 19717 | | ratio median 1.005 when connected | |
| 0x04A.IMU_YawRtVal (left +) | GNSS yaw | 4309 | 0.982 | slope 0.964 | 0.2 s |
| 0x04A lat / long accel (g) | GNSS accel | 4309 | 0.876 / 0.846 | slope 0.98 / 1.02 | |
| 0x04A vertical accel 16-bit | gravity | 1000 | | 0.9985 g at standstill | |
| 0x175.aBasis | GNSS long accel | 4314 | 0.944 | slope 0.87 | 0.3 s |
| 0x125.STEERING_ANGLE#004, 0x0EA.MDPS_EstStrAnglVal | 14.26*atan(2.97*kappa) | 4149 | 0.987 | slope 1.06 (ratio 15.1) | 0.15 s |
| 0x125.STEERING_RATE | abs d(angle)/dt | 65611 | 0.994 | slope 0.994 | 0 |
| 0x175.DriverBraking, 0x065.BRAKE_PRESSED/LIGHT | GNSS accel | 4314 | | -0.74 vs +0.10 m/s^2 mean | 0.3 s |
| 0x235 V, 0x2FA I, 0x250 P | P = V*I/1000 | 6567 | 0.993 | slope 0.98; P vs GNSS traction power r 0.83 | |
| 0x0EA LKA active, 0x12A.STEER_REQ | cross-ECU handshake | 65683 | | 99.98% equal | 0.02 s |
| 0x3C1 / 0x413 blinkers | GNSS-confirmed yaw | 8 episodes | | left -> +92/+92/+24 deg, right -> -93/-90/-91 deg | |
| 0x035.GEAR, 0x130.GEAR | motion, user report | | | D while driving, P on 10-08 | |
| 0x411 driver / driver-rear door | user report | | | 1 on 10-08, 0 while driving | |
| 0x4EB hour/minute/second | host clock KST | 3246 + 50 | | host - CAN = 0.43 s / 1.71 s; weekday byte 100% | |
| 0x41C.IS_DARK, 0x4A3.TunnelExist | tunnel/night, GNSS covariance | | | 92.8% dark in tunnels; night = 1 | |
| 0x4A3.CountryCode | ISO 3166, GNSS | | | 410 = Korea, 100% fixes in Korea | |

## Contradictions and proposed corrections (non-protocol, 11)

| field | evidence | proposed correction |
|---|---|---|
| 0x125.STEERING_ANGLE#002 | r -0.987 vs left-positive curvature | factor +0.1 (= #004) |
| 0x0EA.STEERING_ANGLE_2 | r -0.987 (same bits as MDPS_EstStrAnglVal) | factor +0.1 or drop duplicate |
| 0x04A.IMU_VerAccelVal (208:8) | low byte only: 153.7 +- 19.7 counts at rest | 208:16, x0.000127465, -4.17677 g |
| 0x060.BRAKE_PRESSED | set only at <= 0.04 m/s; 98.3% = AVH_Sta 1; 84.9% = DriverBraking | relabel standstill brake-hold |
| 0x0A0.MOVING_FORWARD, MOVING_FORWARD2 | constant 0 while driving forward (byte 7 always 0) | do not use as direction |
| 0x1A0.ObjValid | 1 on all 29921 no-object frames, 0 on all 2920 object frames | invert (1 = no object) |
| 0x1A0.ACC_ObjRelSpd#003 | span includes bit 46; 239.4 m/s on no-object frames | use #030 (35:9, 0.1, -16.4) |
| 0x1AA.CRUISE_BUTTONS | constant 7 (outside enum 0..4) both days; 0x1CF reads 0 | unknown constant; use 0x1CF |
| 0x1AA.LFA_BTN | constant 0 during 2 presses seen on 0x1CF that toggled LFA | use 0x1CF.LFA_BTN |
| 0x1BA.BCW_RtSndWrngSta (36:10) | non-zero only via bits 41..44 shared with FL_INDICATOR (100%) | width 2 (36:2) |

## Alternative-layout decisions

- 0x125 steering angle: #004 (+0.1, left positive) selected; #002 is the same bits negated.
- 0x1A0 relative speed: #030 (35:9) selected; identical to #003 on object frames
  (d(ObjDist)/dt r 0.995, slope 0.98), #003 overlaps ObjValid.
- 0x060 BRAKE_PRESSURE #003 (11 bit) vs #008 (10 bit): identical in every frame (raw max 277,
  bit 138 never set); undecidable from data.
- 0x1A0 DISTANCE_SETTING #013/#037 and StopReq #025/#047: constant 0 on both days (SCC never
  engaged); undecidable.
- Width duplicates also undecidable: 0x0A0 16-bit vs 14-bit wheel speeds, 0x10A/0x120 load
  detail 12 vs 14 bit; 0x0F5 LOAD_1 = LOAD_2; six 0x1FA speed-limit fields carry identical values.

## Other notes (inference unless stated)

- Lane fields (0x1B5): positions use a right-positive axis (lane width median 3.11 m); heading
  is consistent with lateral drift (r 0.74); curvature sign is opposite to left-positive path
  curvature (r -0.37..-0.53) and its scale could not be identified.
- 0x0EA driver/MDPS torques and 0x12A torque request are plausible (left positive), no torque reference.
- Motor/drive load raw fields correlate with GNSS traction force (rear r 0.815) but have no unit.
- 0x0DA speed reads 0.01 m/s at standstill; IMU roll rate has a -0.4 deg/s bias on both days.
- 0x0E5 is not present in the recording.

## Limits

- One driving day, mostly 2..7.4 m/s (max 15.6 m/s, GNSS-valid 4369 samples at 10 Hz); no reverse,
  N, SCC engagement, high speed or high lateral acceleration.
- GNSS yaw, curvature and acceleration are derived from positions with 0.45..0.8 s windows;
  they smooth and delay the reference (lags of -0.1..0.5 s). Host/bag timestamps are USB
  completion times, not CAN arrival times.
- The 10-08 capture is 10 s at standstill; door and gear states rely on the user's report.
- Mass 2100 kg and Crr 0.012 are assumptions for the traction-power check; rpm-to-ratio uses an
  assumed 0.36 m dynamic radius.
