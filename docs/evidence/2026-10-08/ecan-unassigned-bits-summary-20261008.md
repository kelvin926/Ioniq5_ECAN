# Unassigned bits in the 97 interpreted IDs (2026-10-08)

Compiled from unassigned_bits_results.json (the analysis run wrote the JSON, then stopped on a network error before writing this summary). Offline, ECAN bus 0, 2026-10-07 drive plus 2026-10-08 10 s standstill.

| Metric | Value |
| --- | --- |
| uncovered_varying_bits_before | 1197 |
| explained_confirmed_or_strong | 548 |
| explained_including_weak | 862 |
| still_unexplained_after_all | 335 |

Spot checks repeated by the orchestrator: 0x065 bit56 is the exact inverse of BRAKE_PRESSED (65,685/65,685); 0x035 52:2 is 2 whenever 0x0F5 bit92 reports the front axle connected (19,433/19,433); 0x225 72:9 increments summed within captures (4.9 km) match wheel-speed-integrated distance (4,986 m).

## Confirmed and strong candidates

| ID | Field | Bits | Confidence | Meaning |
| --- | --- | --- | --- | --- |
| 0x035 | FRONT_AXLE_CONNECTION_STATE_2BIT | 52:2 | strong candidate/semantic | Front-axle (front motor) connection state: 0 while the front motor is decoupled/stationary, 2 while connected and co-rotating; 1 and 3 only briefly during trans |
| 0x035 | DRIVE_DEMAND_SIGNED_RAW | 72:16 | strong candidate/semantic | Signed drive-demand-like quantity (bits 85..87 are sign extension of bit 84; 72:13 signed decodes identically). Tracks the sum of the front and rear drive-load  |
| 0x035 | FRONT_AXLE_DISCONNECTED_STATUS | 88:1 | strong candidate/semantic | Inverse copy of the front-axle connection flag (1 = front motor decoupled). |
| 0x035 | FRONT_AXLE_CONNECTION_LEAD_FLAG | 126:1 | strong candidate/semantic | AWD connection-related inverse flag that changes about 0.34..0.44 s before the front axle connects and 0.11..0.26 s after it disconnects (request-like timing; t |
| 0x065 | BRAKE_PRESSED_INVERTED | 56:1 | confirmed/semantic | Exact inverse of 0x065.BRAKE_PRESSED. |
| 0x065 | FRONT_REGEN_BRAKE_RAW | 72:10 | strong candidate/semantic | Front-motor regenerative braking amount (raw). Non-zero only while the brake pedal is pressed; follows the negative part of the front drive load. |
| 0x065 | TOTAL_BRAKE_DEMAND_RAW | 88:12 | strong candidate/semantic | Total brake demand (friction + regen) in a common raw unit: equals about 8.76 x friction pressure + 1.0 x rear regen + 0.8 x front regen. |
| 0x065 | BRAKE_PEDAL_TRAVEL_COARSE | 104:7 | strong candidate/semantic | Coarse brake pedal travel, about BRAKE_POSITION / 5 (2 at rest, 75 at maximum observed). |
| 0x065 | BRAKE_PEDAL_TRAVEL_COARSE_COPY | 112:7 | confirmed/structural | Bit-exact duplicate of 0x065 104:7. |
| 0x065 | REAR_REGEN_BRAKE_RAW | 128:10 | strong candidate/semantic | Rear-motor regenerative braking amount (raw). Non-zero only while braking; follows the negative part of the rear drive load; coefficient 1.0 in the total-brake  |
| 0x06F | AVH_HOLDING_COPY | 36:1 | confirmed/semantic | Copy of 0x060.AVH_Sta == 1 (vehicle held by service brake). |
| 0x090 | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x090 | UNKNOWN_64_5_COPY | 100:5 | confirmed/structural | Bit-exact duplicate of 0x090 64:5. |
| 0x0F5 | FRONT_AXLE_CONNECTED | 92:1 | strong candidate/semantic | Front axle connected (front motor coupled). With 0, the front motor speed is near zero while the rear motor turns; with 1 both motors co-rotate. Connects when s |
| 0x0F5 | FRONT_REGEN_BRAKE_RAW_COPY | 160:10 | strong candidate/semantic | Copy of 0x065 72:10 (front regen). |
| 0x0F5 | REAR_REGEN_BRAKE_RAW_COPY | 176:10 | strong candidate/semantic | Copy of 0x065 128:10 (rear regen). |
| 0x0F5 | FRONT_MOTOR_TORQUE_LIMIT_A | 216:7 | strong candidate/semantic | Speed-dependent front-motor limit: constant 122 below about 2260 rpm, then decreasing roughly as 1/rpm (constant-power region). Units unknown. |
| 0x0F5 | FRONT_MOTOR_TORQUE_LIMIT_A_COPY | 224:7 | confirmed/structural | Bit-exact duplicate of 0x0F5 216:7. |
| 0x0F5 | REAR_MOTOR_TORQUE_LIMIT_A | 232:5 | strong candidate/semantic | Speed-dependent rear-motor limit: constant 31 below about 4410 rpm, then decreasing with rpm. |
| 0x0F5 | REAR_MOTOR_TORQUE_LIMIT_A_COPY | 240:5 | confirmed/structural | Bit-exact duplicate of 0x0F5 232:5. |
| 0x10A | REAR_MOTOR_TORQUE_LIMIT_B | 74:6 | strong candidate/semantic | Speed-dependent rear-motor limit: 62 below 4605 rpm, then about proportional to 1/rpm. |
| 0x10A | REAR_MOTOR_TORQUE_LIMIT_B_COPY | 84:6 | confirmed/structural | Bit-exact duplicate of 0x10A 74:6. |
| 0x10A | REAR_LOAD_COPY_10BIT | 144:10 | strong candidate/semantic | Second copy of 0x10A.REAR_MOTOR_LOAD_RAW (40:10 signed), same scale. |
| 0x10A | AVH_NOT_HOLDING | 164:1 | strong candidate/semantic | Inverse of AVH holding (0 while 0x060.AVH_Sta == 1). |
| 0x10A | REAR_LOAD_13BIT | 225:13 | strong candidate/semantic | Rear drive load at half the 0x0DA 16-bit scale (raw = 0.50 x 0x0DA.REAR_DRIVE_LOAD_RAW). |
| 0x10A | REAR_MOTOR_TORQUE_LIMIT_C | 238:6 | strong candidate/semantic | Second speed-dependent rear limit: 47 below 4613 rpm, then about proportional to 1/rpm. |
| 0x10A | REAR_MOTOR_TORQUE_LIMIT_C_COPY | 247:6 | confirmed/structural | Bit-exact duplicate of 0x10A 238:6. |
| 0x120 | FRONT_MOTOR_TORQUE_LIMIT_B | 74:8 | strong candidate/semantic | Speed-dependent front-motor limit: 246 below about 2600..2820 rpm, then about proportional to 1/rpm. |
| 0x120 | FRONT_MOTOR_TORQUE_LIMIT_B_COPY1 | 84:8 | confirmed/structural | Bit-exact duplicate of 0x120 74:8. |
| 0x120 | FRONT_LOAD_COPY_10BIT | 144:10 | strong candidate/semantic | Second copy of 0x120.FRONT_MOTOR_LOAD_RAW (40:10 signed). |
| 0x120 | FRONT_LOAD_13BIT | 225:13 | strong candidate/semantic | Front drive load at half the 0x0DA 16-bit scale. |
| 0x120 | FRONT_MOTOR_TORQUE_LIMIT_C | 238:8 | strong candidate/semantic | Second speed-dependent front limit: 128 at low rpm, reduced about as 1/rpm at high rpm. |
| 0x120 | FRONT_MOTOR_TORQUE_LIMIT_B_COPY2 | 247:8 | confirmed/structural | Bit-exact duplicate of 0x120 74:8. |
| 0x145 | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x145 | LFA_ENGAGED_INVERTED_2BIT | 222:2 | strong candidate/semantic | Lane-following state: 0 only while 0x12A.LKA_SysIndReq == 2 (green), otherwise 3. |
| 0x1AA | NOT_IS_DARK | 28:1 | confirmed/semantic | Inverse copy of 0x41C.IS_DARK. |
| 0x1AA | ROLLING_COUNTER_2BIT | 32:2 | strong candidate/structural | Second rolling counter, +1 mod 4 about every second frame. |
| 0x1B0 | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x1B0 | UNKNOWN_96_7_COPY | 128:7 | strong candidate/structural | Near-duplicate of 0x1B0 96:7 (differs by 1 in rare frames). |
| 0x1E5 | STANDSTILL_COPY | 88:1 | strong candidate/semantic | Copy of 0x175.ESC_StdStillVal (standstill). |
| 0x1E5 | RIGHT_BSD_COPY | 107:1 | confirmed/semantic | Copy of 0x36A.RIGHT_BSD. |
| 0x1E5 | LEFT_BSD_COPY | 108:1 | confirmed/semantic | Copy of 0x36A.LEFT_BSD. |
| 0x1F5 | SHARED_SLOW_192_COPY | 240:4 | confirmed/structural | Bit-exact duplicate of 0x1F5 192:4. |
| 0x225 | TRIP_DISTANCE_0P1KM | 72:9 | strong candidate/semantic | Distance counter in 0.1 km: +1 per 100.1 m of CAN-speed-integrated travel; 251 -> 314 on 2026-10-07, 2 on 2026-10-08 (reset). |
| 0x250 | LFA_ENGAGED_INVERTED_80 | 80:2 | strong candidate/semantic | Same state as 0x145 222:2: 0 only while LKA_SysIndReq == 2, else 3. |
| 0x250 | LFA_ENGAGED_INVERTED_80_COPY | 88:2 | confirmed/structural | Bit-exact duplicate of 0x250 80:2. |
| 0x250 | LFA_STATE_4LEVEL_83_COPY | 91:2 | confirmed/structural | Bit-exact duplicate of 0x250 83:2. |
| 0x250 | AVH_NOT_HOLDING | 133:1 | strong candidate/semantic | Inverse of AVH holding. |
| 0x25A | NAV_MAP_EVENT_241_COPY | 243:1 | confirmed/structural | Bit-exact duplicate of 0x25A bit 241. |
| 0x2B0 | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x2B5 | SLOW_DECREASING_64_COPY | 64:5 | strong candidate/structural | Copy of 0x225 32:5 (minor transition timing differences). |
| 0x2E0 | FRONT_AXLE_CONNECTION_RAMP | 64:7 | strong candidate/semantic | Front-axle connection progress-like value: 18 while decoupled, ramps by about 6.5 per 0.1 s to 125 when the axle connects. |
| 0x2FA | SHARED_32_4 | 32:4 | confirmed/structural | Bit-exact copy of 0x235 144:4. |
| 0x2FA | SHARED_120_2 | 120:2 | strong candidate/structural | Near copy of 0x235 80:2. |
| 0x30A | ROTATING_PAGE_INDEX | 250:6 | strong candidate/structural | Rotating index cycling 2,7,0,63,0,0 (each value held 3 frames); bits 250/252 and 253..255 move together. |
| 0x315 | SLOW_DECREASING_28_COPY | 28:5 | strong candidate/structural | Copy of 0x225 32:5. |
| 0x31A | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x325 | SHARED_SLOW_129 | 129:6 | confirmed/structural | Bit-exact copy of 0x1F5 81:6. |
| 0x325 | SHARED_SLOW_169 | 169:6 | confirmed/structural | Bit-exact copy of 0x1F5 137:6. |
| 0x325 | SHARED_SLOW_201 | 201:8 | confirmed/structural | Bit-exact copy of 0x1F5 153:8. |
| 0x325 | SHARED_SLOW_144 | 144:4 | confirmed/structural | Bit-exact copy of 0x1F5 192:4. |
| 0x325 | COUNTER_LSB_COPY | 98:1 | confirmed/structural | Bit-exact copy of the counter LSB (bit 16). |
| 0x325 | SHARED_160_4 | 160:4 | confirmed/structural | Bit-exact copy shared with 0x3B5 116:4 (meaning unknown). |
| 0x330 | MINUTE_UPDATED_224_COPY | 240:7 | confirmed/structural | Bit-exact duplicate of 0x330 224:7. |
| 0x34A | CRC16_HYUNDAI_CANFD | 0:16 | confirmed/structural | Hyundai CAN-FD CRC16 (XMODEM table over bytes 2..N-1, then address bytes, then length XOR). Value 0xFFFF appears in a few frames as a non-CRC placeholder. |
| 0x360 | SHARED_232_4 | 232:4 | confirmed/structural | Bit-exact copy of 0x2FA 40:4. |
| 0x360 | SHARED_224_1 | 224:1 | confirmed/structural | Bit-exact copy of 0x25A 112:1. |
| 0x3B5 | SHARED_116_4 | 116:4 | confirmed/structural | Bit-exact copy of 0x325 160:4. |
| 0x3F0 | SHARED_40_7 | 40:7 | strong candidate/structural | Near copy of 0x31A 96:7. |
| 0x3F0 | SHARED_64_7 | 64:7 | strong candidate/structural | Copy of 0x31A 112:7. |
| 0x3F5 | ROTATING_INDEX_3 | 24:3 | strong candidate/structural | Rotating index 2 -> 1 -> 7 every frame. |
| 0x3F5 | ROTATING_INDEX_LOW2_COPY | 28:2 | confirmed/structural | Bit-exact copy of bits 24..25. |
| 0x412 | BRAKE_PRESSED_COPY | 50:1 | confirmed/semantic | Copy of 0x065.BRAKE_PRESSED. |
| 0x418 | BRAKE_PRESSED_COPY | 46:1 | confirmed/semantic | Copy of 0x065.BRAKE_PRESSED. |
| 0x435 | BRAKE_PRESSED_COPY | 28:1 | confirmed/semantic | Copy of 0x065.BRAKE_PRESSED. |
| 0x4B8 | INVALID_FILLER_FRAME_ALL_FF | 0:64 | confirmed/structural | Frames with all eight bytes 0xFF alternate with real navigation frames; these bits vary only between filler and real frames. |
| 0x4B9 | INVALID_FILLER_FRAME_ALL_FF | 0:64 | confirmed/structural | Frames with all eight bytes 0xFF alternate with real navigation frames; these bits vary only between filler and real frames. |
| 0x4BA | INVALID_FILLER_FRAME_ALL_FF | 0:64 | confirmed/structural | Frames with all eight bytes 0xFF alternate with real navigation frames; these bits vary only between filler and real frames. |
| 0x4BE | INVALID_FILLER_FRAME_ALL_FF | 0:64 | confirmed/structural | Frames with all eight bytes 0xFF alternate with real navigation frames; these bits vary only between filler and real frames. |
| 0x4BF | INVALID_FILLER_FRAME_ALL_FF | 0:64 | confirmed/structural | Frames with all eight bytes 0xFF alternate with real navigation frames; these bits vary only between filler and real frames. |

Weak candidates (66) and remaining unexplained bits are listed per ID in the JSON; they are associations only and are not proposed for display.
