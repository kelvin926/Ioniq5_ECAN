# Unknown ECAN IDs: deeper offline analysis (2026-10-08)

Scope: the 41 IDs in `build/deep-ecan-20261007/arrays.npz` that are absent from `config/ecan_signals.json`
(the computed set equals the requested list). Data: the 2026-10-07 recording (7 captures, 656.8 s) and the
2026-10-08 10 s standstill capture. Timestamps are host/bag receive times. Bit numbers use the DBC little-endian
convention (bit k = byte k//8, bit k%8). Full per-ID detail: `unknown_ids_results.json`.

## 1. Protocol structure (CAN-only evidence, confirmed)

**E2E CRC8 family, 24 IDs**: 0x08B 0x0FF 0x3A6 0x3C2 0x3E0 0x3E1 0x3E2 0x3E6 0x401 0x405 0x410 0x414 0x416
0x417 0x41A 0x41B 0x422 0x425 0x432 0x442 0x444 0x445 0x454 0x48F.
- byte0 = CRC8 with poly 0x1D (SAE J1850), init 0xFF, xorout 0xEA (equivalently init 0x00, xorout 0x69). It covers
  [CAN ID low byte, CAN ID high byte, bytes 1..7]. A brute force over 255 polynomials, with a residual per ID, gave
  exactly one hit on 16 multi-payload IDs. A single global constant then matched all 49 distinct payloads.
- bits 12..15 = change counter, modulo 15 (0..14). It steps once per content change, not per frame. Every change is
  sent as a burst of about 4 frames at 40 ms, then the 200 ms cycle resumes. Confirmed on 0x445 (sequence 14, 0, 1,
  2, 3, 4, 5 on 2026-10-08), 0x41A (9, 10, 11) and 0x410 (5, 6).
- 0x3A6 has a verified CRC8, but its counter position is not established: byte1 stayed 0xD1 although bit 40
  changed between the two days.

**Hyundai CAN-FD CRC16 family, 8 IDs**: 0x382 0x383 0x384 0x3B0 0x3B1 0x3B2 0x427 0x42A.
- bytes 0..1 = opendbc `hkg_can_fd_checksum` (XMODEM over bytes 2..7 plus the address, xor 0x5F29). It matched 25 of
  25 distinct payloads. 24 isolated frames (0.09 %) carry 0xFFFF in the CRC field with the data unchanged. What 0xFFFF
  means is unresolved.
- byte2 = 8-bit change sequence counter. On 0x382 it ran 0x0E, 0x0F, 0x10, 0x11, then 0x12 after one change inferred
  in a capture gap. On 0x384 it ran 0, 1, 2. The first frame of a change still carries the old value.

**No checksum identified, 9 IDs**: 0x386 0x3AA 0x3EA 0x41F 0x4DD 0x4DF 0x4F2 0x4F3 0x4FE. Tested: XOR, sum, CRC8 with
polys 0x1D and 0x2F (with and without the ID prefix), and HKG CRC16, at every byte position.

## 2. Evidence-backed field candidates

| ID | Field (bits) | Finding | Confidence |
|---|---|---|---|
| 0x4F2 | 54..63 LE, 10 bit | Follows 0x41C.LIGHT_LEVEL, r = 0.807 with light leading by 1 s. Best of 158 numeric fields (next 0.50). Saturates at 1000 in daylight and has a floor of 420 in the dark (tunnel, and the whole 2026-10-08 capture). | strong candidate |
| 0x4F2 | 44..45, 2 bit | 2 only while 0x41C.IS_DARK = 1 (181/181 frames), 1 otherwise. 2 on 2026-10-08. | strong candidate |
| 0x4DF | 18..19 and 26 (24..29?) | Tunnel window from 1791351822.17 to 1854.64. Starts 5.15 s (about 88 m) before 0x4A3.TunnelExist and ends 0.08 s before TunnelExist clears. | strong candidate |
| 0x382 | 50..51, 2 bit | 2 changes to 1 during the same window, edges within 0.01 s of 0x4DF. | strong candidate |
| 0x384 | 60..61, 2 bit | Same as 0x382, edges within 0.03 s. | strong candidate |
| 0x41A | 46, 1 bit | Sets in the same frame time as IS_DARK 0 to 1. Clears 5.0 s before IS_DARK clears. 1 on 2026-10-08 (dark). | strong candidate |
| 0x445 | 42, 1 bit | 2026-10-08 only: 3 pulses about 190 ms long with a 3.20 s period. No co-changing bit in 133 IDs. Never set on 2026-10-07. | weak candidate |
| 0x410 | 16..17 | Changed 1 to 2 once while moving at 4.9 m/s, with no known correlate. Odometer hypothesis rejected (no increment over at least 3.6 km). | weak candidate |
| 0x382 | 24..25 | Changed 2 to 1 once, 0.34 s after brake release; back to 2 by capture 6. | weak candidate |
| 0x4DF | 0..9 | 0 on 2026-10-07, 1023 on 2026-10-08 (destination set). The 2026-10-07 ADASIS CalculatedRoute never equals 1. | weak candidate |

Labels such as display brightness, navigation tunnel approach, climate intake recirculation, and auto-lamp state are
inference. The evidence is only the timing and correlation listed above. Every event-based row rests on a single
tunnel passage.

Single bits that differ between the two days (cause not separable, weak): 0x3A6 bit 40, 0x3E1 bit 20,
0x414 bit 37, 0x417 bit 40, 0x41A bits 22, 28 and 40, 0x422 bit 50, 0x410 bits 19, 32, 35, 36, 37 and 40.
P gear, standstill, EPB, two open doors, darkness and the navigation destination all changed together.

## 3. Name-only source matches (field layouts do not fit)

Pinned commaai/opendbc b72c1fd (old-platform DBCs `hyundai_2015_ccan`, `hyundai_2015_mcan`, `hyundai_i30_2014`):
- 0x382 EMS9, 0x383 FATC11, 0x384 EMS17, 0x386 WHL_SPD11, 0x410 CGW_USM1, 0x42A _4WD13: rejected. Their bytes 0..1
  are the verified CRC16 or CRC8, the EV has no EMS, and 0x386 would decode to a constant 264 km/h.
- 0x08B AMP_HU_E_12, 0x442/0x444/0x445/0x454 NM_*, 0x48F/0x4F2/0x4F3 TP_*: rejected. These frames carry E2E CRC8
  protection or continuous data, not network-management or transport structure.
- tylerharvey/Ioniq5_CAN 2c08cf3 lists 0x405 as expected to change on Ioniq 5 remote lock/unlock (an association,
  no layout). This cannot be tested here.
- dragz/egmpdbc b234d7e, Sterlingarcher2525, openvehicles, Battery-Emulator and wicant: no entries for any of the 41 IDs.
- ADASIS v2.0.4: this car's ADASIS stream is 0x4B4..0x4BF. 0x4DD, 0x4DF and 0x4FE have no cyclic counter and are
  constant or state-like. 0x4FE fails the META-DATA test: its type bits read 5 (META-DATA is 6) and the Korea code 410
  is absent.

## 4. Still uninterpretable

- Data constant on both days (only the protocol bits move): 0x08B 0x0FF 0x3C2 0x3E0 0x3E2 0x3E6 0x401 0x405 0x416 0x41B
  0x425 0x432 0x442 0x444 0x454 0x48F 0x383 0x3B0 0x3B1 0x3B2 0x427 0x42A. Fixed bits cannot be correlated.
- No checksum and constant: 0x386 0x3AA 0x3EA 0x41F 0x4DD 0x4F3 0x4FE.
- Remaining fixed bits in every ID are listed per ID in the JSON.

## 5. Limits

- 0x035/0x130 GEAR stayed D throughout 2026-10-07, so gear-change timing could not be tested.
- Correlations use host timestamps, with lag searched over -3 to +3 s.
- None of this is a runtime DBC. No tracked file was changed.
