#!/usr/bin/env python3
"""Bounded offline PRS inspection. Writes only to a new private output directory."""
import argparse
import hashlib
import json
from pathlib import Path


def decode_prs(data, limit=16 * 1024 * 1024):
    position = 0
    control = 0
    remaining = 0
    output = bytearray()

    def byte():
        nonlocal position
        if position >= len(data):
            raise ValueError("Truncated PRS stream")
        value = data[position]
        position += 1
        return value

    def bit():
        nonlocal control, remaining
        if remaining == 0:
            control, remaining = byte(), 8
        value = control & 1
        control >>= 1
        remaining -= 1
        return value

    while True:
        if bit():
            if len(output) >= limit:
                raise ValueError("Decoded size limit exceeded")
            output.append(byte())
            continue
        if bit():
            token = byte() | (byte() << 8)
            if token == 0:
                return bytes(output), position
            distance = 8192 - (token >> 3)
            length = (token & 7) + 2 if token & 7 else byte() + 1
        else:
            length = (bit() << 1) + bit() + 2
            distance = 256 - byte()
        if distance > len(output):
            raise ValueError("PRS back-reference precedes output")
        if len(output) + length > limit:
            raise ValueError("Decoded size limit exceeded")
        for _ in range(length):
            output.append(output[-distance])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output_directory", type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    destination = args.output_directory.resolve()
    if not any(destination.is_relative_to(root / p) for p in ("work", "docs/artifacts", "generated")):
        parser.error("Output must be inside an ignored private work/artifact directory")
    if args.input.stat().st_size > 16 * 1024 * 1024:
        parser.error("Encoded input exceeds inspection limit")
    encoded = args.input.read_bytes()
    decoded, consumed = decode_prs(encoded)
    destination.mkdir(parents=True, exist_ok=False)
    (destination / "decoded.bin").write_bytes(decoded)
    report = {"input": str(args.input.resolve()), "encoded_size": len(encoded),
              "encoded_sha256": hashlib.sha256(encoded).hexdigest(),
              "decoded_size": len(decoded), "decoded_sha256": hashlib.sha256(decoded).hexdigest(),
              "consumed_bytes": consumed, "trailing_bytes": len(encoded) - consumed,
              "trailing_nonzero_bytes": sum(x != 0 for x in encoded[consumed:]),
              "scope": "Format decoding only; no load address, entry or execution claim"}
    (destination / "identity.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
