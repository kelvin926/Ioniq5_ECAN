#!/usr/bin/env python3
"""Build config/ecan_signals.json from the 2026-10-07 interpreted ECAN bit dictionary.

Each displayed field keeps its layout, scaling, unit, Korean label, confidence and enums.
Redacted identity fields are excluded. The computed bit positions of every field are checked
against the dictionary's physical_bits before writing; a mismatch aborts generation.
The 2026-10-08 validation overlay then removes contradicted fields, applies corrections and
adds validated fields; added fields may not overlap existing bits of the same ID.
"""
from __future__ import annotations

import argparse
import collections
import json
from pathlib import Path
import sys

REPO = Path(__file__).resolve().parent.parent
DEFAULT_INPUT = REPO / "docs/evidence/2026-10-07/ecan-interpreted-bit-dictionary-20261007.json"
DEFAULT_OUTPUT = REPO / "config/ecan_signals.json"
DEFAULT_OVERLAY = REPO / "docs/evidence/2026-10-08/ecan-validation-overlay-20261008.json"
SIGNAL_KEYS = ("name", "address", "start_bit", "bits", "endian", "signed", "factor", "offset", "unit",
               "label_ko", "confidence_ko", "role", "enums")


def bit_positions(start_bit, bits, endian):
    """Payload bit positions from MSB to LSB, using the DBC convention of the host codec."""
    if endian == "little":
        return list(range(start_bit + bits - 1, start_bit - 1, -1))
    positions, position = [], start_bit
    for _ in range(bits):
        positions.append(position)
        position = position + 15 if position % 8 == 0 else position - 1
    return positions


def build(dictionary):
    signals, skipped = [], []
    for row in dictionary["rows"]:
        names = collections.Counter(field["name"] for field in row["fields"])
        for field in row["fields"]:
            if field.get("redacted") or field.get("placeholder"):
                skipped.append(field["field_key"])
                continue
            positions = bit_positions(field["start_bit"], field["bits"], field["endian"])
            if sorted(positions) != sorted(field["physical_bits"]):
                raise ValueError("bit layout mismatch for %s" % field["field_key"])
            name = "%s.%s" % (row["can_id"], field["name"])
            if names[field["name"]] > 1:
                # Alternative layouts may share start/bits; the dictionary row index is unique.
                name += "#" + field["field_key"].split("/")[1]
            signals.append({
                "name": name,
                "address": row["address"],
                "start_bit": field["start_bit"],
                "bits": field["bits"],
                "endian": field["endian"],
                "signed": bool(field["signed"]),
                "factor": field["factor"],
                "offset": field["offset"],
                "unit": field.get("unit", ""),
                "label_ko": field.get("label_ko", ""),
                "confidence_ko": field.get("confidence_ko", ""),
                "role": field.get("role", ""),
                "enums": field.get("enums", {}),
            })
    if len({signal["name"] for signal in signals}) != len(signals):
        raise ValueError("signal names are not unique")
    return signals, skipped


def apply_overlay(signals, overlay, payload_bytes):
    """Remove, correct and add fields from the dated validation overlay."""
    by_name = {signal["name"]: signal for signal in signals}
    for name in overlay["remove"]:
        if name not in by_name:
            raise ValueError("overlay removes unknown field %s" % name)
    signals = [signal for signal in signals if signal["name"] not in overlay["remove"]]
    by_name = {signal["name"]: signal for signal in signals}
    renamed = []
    for name, changes in overlay["modify"].items():
        if name not in by_name:
            raise ValueError("overlay modifies unknown field %s" % name)
        for key, value in changes.items():
            if key == "name":
                renamed.append((by_name[name], value))
            else:
                by_name[name][key] = value
    # Drop the #index suffix where removals left a single layout for that field name.
    bases = collections.Counter((s["address"], s["name"].split("#")[0]) for s in signals)
    for signal in signals:
        base = signal["name"].split("#")[0]
        if "#" in signal["name"] and bases[(signal["address"], base)] == 1:
            signal["name"] = base
    for signal, new_field_name in renamed:
        signal["name"] = "%s.%s" % (signal["name"].split(".")[0], new_field_name)
    occupied = collections.defaultdict(set)
    for signal in signals:
        occupied[signal["address"]].update(bit_positions(signal["start_bit"], signal["bits"], signal["endian"]))
    for spec in overlay["add"]:
        positions = set(bit_positions(spec["start_bit"], spec["bits"], spec["endian"]))
        length = payload_bytes.get(spec["address"], 8)
        if max(positions) >= length * 8 or positions & occupied[spec["address"]]:
            raise ValueError("overlay field 0x%03X.%s is out of range or overlaps" % (spec["address"], spec["name"]))
        occupied[spec["address"]].update(positions)
        signal = {key: spec[key] for key in SIGNAL_KEYS if key != "name"}
        signal["name"] = "0x%03X.%s" % (spec["address"], spec["name"])
        signals.append({key: signal[key] for key in SIGNAL_KEYS})
    if len({signal["name"] for signal in signals}) != len(signals):
        raise ValueError("signal names are not unique after the overlay")
    return sorted(signals, key=lambda s: (s["address"], s["start_bit"]))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--overlay", type=Path, default=DEFAULT_OVERLAY)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    dictionary = json.loads(args.input.read_text(encoding="utf-8"))
    signals, skipped = build(dictionary)
    overlay = json.loads(args.overlay.read_text(encoding="utf-8"))
    payload_bytes = {row["address"]: row["bytes"] for row in dictionary["rows"]}
    signals = apply_overlay(signals, overlay, payload_bytes)
    document = {
        "source": "docs/evidence/2026-10-07/%s plus docs/evidence/2026-10-08/%s"
                  % (args.input.name, args.overlay.name),
        "date": overlay["date"],
        "scope": "Interpretation candidates from offline CAN analysis, validated on 2026-10-08; not a DBC.",
        "excluded_redacted_fields": len(skipped),
        "signals": signals,
    }
    with open(args.output, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(json.dumps(document, ensure_ascii=False, indent=1) + "\n")
    print("wrote %d signals for %d IDs to %s (excluded %d)"
          % (len(signals), len({s["address"] for s in signals}), args.output, len(skipped)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
