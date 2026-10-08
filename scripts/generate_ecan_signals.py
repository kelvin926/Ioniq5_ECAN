#!/usr/bin/env python3
"""Build config/ecan_signals.json from the 2026-10-07 interpreted ECAN bit dictionary.

Each displayed field keeps its layout, scaling, unit, Korean label, confidence and enums.
Redacted identity fields are excluded. The computed bit positions of every field are checked
against the dictionary's physical_bits before writing; a mismatch aborts generation.
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


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    dictionary = json.loads(args.input.read_text(encoding="utf-8"))
    signals, skipped = build(dictionary)
    document = {
        "source": "docs/evidence/2026-10-07/" + args.input.name,
        "date": dictionary["summary"]["date"],
        "scope": "Interpretation candidates from offline CAN analysis, not a validated DBC.",
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
