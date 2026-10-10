#!/usr/bin/env python3
"""Compare static module calls with resident discovery; never infer execution."""
import argparse
from collections import Counter
import csv
import hashlib
import json
from pathlib import Path


def rows(path):
    with path.open() as source:
        return list(csv.DictReader(source))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("resident", type=Path)
    p.add_argument("module", type=Path)
    p.add_argument("output", type=Path)
    p.add_argument("--resident-base", type=lambda x: int(x, 0), required=True)
    p.add_argument("--resident-size", type=int, required=True)
    a = p.parse_args()
    root = Path(__file__).resolve().parents[1]
    if not any(a.output.resolve().is_relative_to(root / d) for d in ("work", "docs/artifacts", "generated")):
        p.error("Output must be in an ignored private directory")
    files = [a.resident / "functions.csv", a.resident / "instructions.csv", a.module / "external-calls.csv"]
    entries = {int(r["address"], 16) for r in rows(files[0])}
    instructions = {int(r["address"], 16) for r in rows(files[1])}
    result = []
    for row in rows(files[2]):
        target = int(row["target"], 16)
        physical = target & 0x1FFFFFFF
        canonical = physical | 0x80000000 if target >> 29 in (0, 4, 5) and 0x0C000000 <= physical < 0x0D000000 else target
        if row["resolved"] != "1":
            kind = "unresolved_dynamic"
        elif canonical in entries:
            kind = "resident_entry"
        elif canonical in instructions:
            kind = "resident_instruction_not_entry"
        elif a.resident_base <= canonical < a.resident_base + a.resident_size:
            kind = "resident_image_not_discovered_code"
        else:
            kind = "outside_resident_discovery"
        result.append(dict(row, canonical_target=f"0x{canonical:08X}", classification=kind))
    report = {"scope": "Static address comparison; no execution/relocation proof",
              "input_sha256": {str(f.resolve()): hashlib.sha256(f.read_bytes()).hexdigest() for f in files},
              "records": len(result), "unique_sites": len({r["site"] for r in result}),
              "classifications": dict(Counter(r["classification"] for r in result)),
              "unique_targets_by_class": {kind: len({r["canonical_target"] for r in result if r["classification"] == kind})
                                          for kind in sorted({r["classification"] for r in result})}}
    a.output.mkdir(parents=True, exist_ok=False)
    with (a.output / "linkage.csv").open("w") as output:
        writer = csv.DictWriter(output, fieldnames=["caller", "site", "target", "resolved", "reason", "canonical_target", "classification"])
        writer.writeheader()
        writer.writerows(result)
    seeds = sorted({r["canonical_target"] for r in result if r["classification"] in
                    ("resident_instruction_not_entry", "resident_image_not_discovered_code")})
    (a.output / "resident-seeds.txt").write_text("# Static call targets: hypotheses, not confirmed execution\n" + "\n".join(seeds) + "\n")
    (a.output / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
