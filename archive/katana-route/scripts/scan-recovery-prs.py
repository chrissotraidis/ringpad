#!/usr/bin/env python3
"""Bounded private PRS/BIN inventory using the observed module-registration header."""
import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import signal
import struct
import time

spec = importlib.util.spec_from_file_location("prs", Path(__file__).with_name("decode-recovery-prs.py"))
prs = importlib.util.module_from_spec(spec)
spec.loader.exec_module(prs)


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def module_header(data, resident_entries):
    if len(data) < 32:
        return None
    first, second, branch, delay = struct.unpack_from("<4H", data)
    # MOV.L @(disp,PC),R3; MOV.L @(disp,PC),R5; JMP @R3; MOV #imm,R6.
    if first & 0xFF00 != 0xD300 or second & 0xFF00 != 0xD500 or branch != 0x432B or delay & 0xFF00 != 0xE600:
        return None
    def literal(word, pc):
        offset = ((pc + 4) & ~3) + 4 * (word & 255)
        return struct.unpack_from("<I", data, offset)[0] if offset + 4 <= len(data) else None
    target, metadata = literal(first, 0), literal(second, 2)
    if target is None or metadata is None:
        return {"error": "Header literal outside image"}
    placements = []
    # Enumerate aligned hypotheses; do not silently assume Mini Cart's base.
    for base in range(0x0C000000, 0x0D000000, 0x10000):
        offset = (metadata & 0x1FFFFFFF) - base
        if not 0 <= offset <= len(data) - 24:
            continue
        size, kind, *callbacks = struct.unpack_from("<6I", data, offset)
        active = [p for p in callbacks if p]
        if size > 0x10000 or kind > 0x10000 or not active:
            continue
        if any(p >> 29 not in (0, 4, 5) or (p & 1) or not base <= (p & 0x1FFFFFFF) < base + len(data) for p in active):
            continue
        placements.append({"base": f"0x{base | 0x80000000:08X}", "metadata_offset": offset,
                           "size_field": size, "kind_field": kind,
                           "callbacks": [f"0x{(p & 0x1FFFFFFF) | 0x80000000:08X}" for p in active]})
    return {"resident_target": f"0x{target:08X}", "resident_entry_known": target in resident_entries,
            "metadata_pointer": f"0x{metadata:08X}", "placement_hypotheses": placements}


def prefix_kind(data, header):
    if header is not None:
        return "registration_header"
    if data[:4] == b"PVMH":
        return "PVMH_prefix"
    if len(data) >= 2:
        word = int.from_bytes(data[:2], "little")
        if word == 0x4F22 or word & 0xFF0F == 0x2F06:
            return "stack_prologue_prefix"
    return "other_prefix"


def main():
    import csv
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("disc_audit", type=Path)
    parser.add_argument("resident_audit", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--retain-non-pvm", action="store_true", help="Keep non-PVMH candidates privately for follow-up analysis")
    parser.add_argument("--kind", choices=("prs", "bin"), default="prs", help="Decode PRS or inspect raw BIN files")
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    if not args.output.resolve().is_relative_to(root / "work") and not args.output.resolve().is_relative_to(root / "docs/artifacts"):
        parser.error("Output must be in an ignored private directory")
    audit = json.loads((args.disc_audit / "report.json").read_text())
    disc = Path(audit["disc"]["path"])
    original_hash = sha256(disc)
    if original_hash != audit["disc"]["sha256_after"]:
        parser.error("Disc does not match inventory identity")
    entries_file = args.resident_audit / "functions.csv"
    with entries_file.open() as source:
        resident_entries = {int(row["address"], 16) for row in csv.DictReader(source)}
    candidates = [r for r in json.loads((args.disc_audit / "disc-files.json").read_text())
                  if r["name"].endswith("." + args.kind.upper()) and not r["directory"]]
    fields = dict(line.split("=", 1) for line in (args.disc_audit / "disc-map.txt").read_text().splitlines() if "=" in line)
    prefix = f"track.{fields['boot_track']}."
    start, end = int(fields[prefix + "start_fad"]) - 150, int(fields[prefix + "end_fad"]) - 150
    raw_size, user_offset, file_base = (int(fields[prefix + key]) for key in ("sector_size", "user_offset", "file_base"))
    args.output.mkdir(parents=True, exist_ok=False)
    report = {"scope": "Header classification; unknown headers are not proven data-only", "input_kind": args.kind,
              "disc_sha256": original_hash, "resident_entries_sha256": sha256(entries_file),
              "inventory_sha256": sha256(args.disc_audit / "disc-files.json"),
              "decoder_sha256": sha256(Path(__file__).with_name("decode-recovery-prs.py")),
              "expected_files": len(candidates), "completed": False, "rows": []}
    def timeout(*_):
        raise TimeoutError("15-second per-file limit")
    signal.signal(signal.SIGALRM, timeout)
    deadline = time.monotonic() + 600
    try:
        with disc.open("rb") as image, (args.output / "files.jsonl").open("w") as progress:
            for index, item in enumerate(candidates):
                if time.monotonic() > deadline:
                    raise TimeoutError("10-minute scan limit")
                row = {"index": index, "path": item["path"], "encoded_size": item["size"]}
                try:
                    signal.alarm(15)
                    if item["multi_extent"] or not 0 < item["size"] <= 16 * 1024 * 1024:
                        raise ValueError("Unsupported extent/encoded size")
                    if item["extent"] < start or item["extent"] + (item["size"] + 2047) // 2048 > end + 1:
                        raise ValueError("Extent outside track")
                    encoded = bytearray()
                    for sector in range((item["size"] + 2047) // 2048):
                        image.seek(file_base + (item["extent"] - start + sector) * raw_size + user_offset)
                        part = image.read(min(2048, item["size"] - len(encoded)))
                        if len(part) != min(2048, item["size"] - len(encoded)):
                            raise ValueError("Short sector read")
                        encoded.extend(part)
                    decoded, consumed = prs.decode_prs(encoded) if args.kind == "prs" else (bytes(encoded), len(encoded))
                    header = module_header(decoded, resident_entries)
                    kind = prefix_kind(decoded, header)
                    row.update(status="decoded" if args.kind == "prs" else "raw_read", prefix_kind=kind, encoded_sha256=hashlib.sha256(encoded).hexdigest(),
                               decoded_size=len(decoded), decoded_sha256=hashlib.sha256(decoded).hexdigest(),
                               consumed_bytes=consumed, trailing_bytes=len(encoded) - consumed,
                               prefix_hex=decoded[:16].hex(), module_header=header)
                    if header is not None or args.retain_non_pvm and kind != "PVMH_prefix":
                        directory = args.output / f"candidate-{index:04d}"
                        directory.mkdir()
                        (directory / ("encoded." + args.kind)).write_bytes(encoded)
                        (directory / "decoded.bin").write_bytes(decoded)
                        row["private_directory"] = str(directory.resolve())
                except (ValueError, TimeoutError, struct.error) as error:
                    row.update(status="unresolved", error=str(error))
                finally:
                    signal.alarm(0)
                report["rows"].append(row)
                progress.write(json.dumps(row) + "\n")
                progress.flush()
                if (index + 1) % 50 == 0:
                    print(f"inspected {index + 1}/{len(candidates)}", flush=True)
        report["completed"] = True
    finally:
        report["disc_sha256_after"] = sha256(disc)
        report["counts"] = dict(Counter(r["status"] for r in report["rows"]))
        report["matching_headers"] = sum(r.get("module_header") is not None for r in report["rows"])
        report["prefix_counts"] = dict(Counter(r.get("prefix_kind", "unresolved") for r in report["rows"]))
        (args.output / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    if report["disc_sha256_after"] != original_hash:
        raise RuntimeError("Original disc identity changed")
    print(json.dumps({k: v for k, v in report.items() if k != "rows"}, indent=2))


if __name__ == "__main__":
    main()
