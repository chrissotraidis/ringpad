#!/usr/bin/env python3
"""Privately compile and check the isolated owner-image PRS routine on ARM64."""
import importlib.util
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]


def module(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT / "scripts" / file)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


aot = module("aot", "run-recovery-aot.py")
prs = module("prs", "decode-recovery-prs.py")


def main():
    if len(sys.argv) != 2:
        raise ValueError("usage: run-recovery-prs-aot.py EXTRACTED_BOOT.BIN")
    boot = Path(sys.argv[1]).resolve(strict=True)
    boot_sha = aot.sha256(boot)
    if boot_sha != "68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29":
        raise ValueError("This routine boundary is verified only for the declared V1.004 boot")
    base, entry, end = 0x8C010000, 0x8C04E260, 0x8C04E2F8
    routine = aot.LOGS / "owner-prs-routine.bin"
    routine.write_bytes(boot.read_bytes()[entry - base:end - base])
    # Reuse exact pin/ordered-patch/toolchain checks and bounded static controls.
    sys.argv = [sys.argv[0], "--coverage", str(routine), "--base", hex(entry)]
    aot.main()
    aot.REPORT.update(passed=False, scope="Native isolated owner-image PRS routine; loader, app and gameplay untested",
                      r3_complete=False, r4_complete=False,
                      owner_boot={"path": str(boot), "sha256": boot_sha},
                      routine={"entry": hex(entry), "end": hex(end), "bytes": routine.stat().st_size,
                               "sha256": aot.sha256(routine)})
    metrics = aot.REPORT["closure_metrics"]
    if metrics["Reachable functions"] != 1 or metrics["Unknown SH-4"] or metrics["RAW_SH4"] or metrics["External/unresolved"]:
        raise ValueError("Routine is not a complete isolated decoded function")
    generated, build = aot.LOGS / "prs-generated", aot.LOGS / "prs-build"
    aot.run("generate-owner-prs", [aot.BUILD / "dc_raw_recomp", routine,
        f"--base={hex(entry)}", f"--entry={hex(entry)}", f"--output={generated}",
        "--max-functions=8", "--closure-passes=2"])
    aot.configure("configure-owner-prs", ROOT / "tests/recovery", build,
        [f"-DFIXTURE_DIR={generated}", "-DFIXTURE=6", f"-DORACLE_SOURCE={ROOT / 'tests/recovery/prs-execution.cpp'}"])
    aot.run("build-owner-prs", ["cmake", "--build", build, "--target", "recovery_oracle", "--parallel", "4"])
    binary = build / "recovery_oracle"
    if "Mach-O 64-bit executable arm64" not in aot.run("identity-owner-prs", ["file", binary]):
        raise ValueError("Oracle is not native ARM64")
    imports = aot.run("imports-owner-prs", ["/usr/bin/nm", "-u", binary])
    if re.search(r"\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b", imports):
        raise ValueError("Review executable-memory/process imports")
    synthetic = [b"\x17ABC\0\0", bytes([0x59, 65, 255, 0, 0]),
                 bytes([0x15, 65, 0xFF, 0xFF, 0, 0]), bytes([0x15, 65, 0xF8, 0xFF, 19, 0, 0]),
                 b"\xFF12345678\x02\0\0"]
    cases = []
    for n, data in enumerate(synthetic):
        encoded = aot.LOGS / f"synthetic-{n}.prs"
        expected = aot.LOGS / f"synthetic-{n}.bin"
        decoded, used = prs.decode_prs(data)
        assert used == len(data)
        encoded.write_bytes(data); expected.write_bytes(decoded)
        cases.append((f"synthetic-{n}", encoded, expected, 0x0C900000))
    candidates = aot.WORK / "prs-candidates"
    reference_file = candidates / "reference-checks.json"
    reference = json.loads(reference_file.read_text())
    reference_by_path = {row["path"]: row for row in reference["checks"]}
    aot.REPORT["independent_reference"] = {
        "record": str(reference_file), "record_sha256": aot.sha256(reference_file),
        "reference_binary_sha256": reference["reference_binary_sha256"],
        "scope": "Previously recorded independent Rust outputs, matched by decoded SHA-256"}
    cases += [("minicart", ROOT / "docs/artifacts/recovery-aot/20261005-103149/MINICART.PRS",
               aot.WORK / "minicart-decoded/decoded.bin", 0x0C900000)]
    for name, number, destination in [("al-main", "0037", 0x0C900000),
                                      ("al-race", "0038", 0x0CB80000), ("sboard", "0698", 0x0C900000)]:
        path = candidates / ("candidate-" + number)
        cases.append((name, path / "encoded.prs", path / "decoded.bin", destination))
    aot.REPORT["cases"] = []
    for name, encoded, expected, destination in cases:
        data = encoded.read_bytes()
        decoded, consumed = prs.decode_prs(data, limit=4 * 1024 * 1024)
        if consumed != len(data) or decoded != expected.read_bytes():
            raise ValueError(f"Independent validation failed for {name}")
        identities = {str(p): aot.sha256(p) for p in (encoded, expected)}
        if not name.startswith("synthetic-"):
            key = "SONICADV/" + name.upper().replace("-", "_") + ".PRS"
            old = reference_by_path[key]
            if not old["matches"] or old["decoded_sha256"] != identities[str(expected)]:
                raise ValueError(f"Recorded independent Rust result differs for {name}")
        result = aot.run("execute-owner-prs-" + name, [binary, encoded, expected, hex(destination)], timeout=30)
        if any(aot.sha256(Path(p)) != digest for p, digest in identities.items()):
            raise ValueError("Input file changed")
        aot.REPORT["cases"].append(dict(name=name, input_sha256=identities, destination=hex(destination),
                                         decoded_bytes=len(decoded), result=result.strip()))
        print(result.strip(), flush=True)
    if aot.sha256(boot) != boot_sha:
        raise ValueError("Original boot changed")
    aot.REPORT.update(passed=True, binary_sha256=aot.sha256(binary),
                      generated_cpp_sha256=aot.sha256(generated / "generated_program.cpp"))


if __name__ == "__main__":
    aot.LOGS.mkdir(parents=True)
    (aot.WORK / "tmp").mkdir(parents=True, exist_ok=True)
    try:
        main()
    except Exception as error:
        aot.REPORT.update(passed=False, error=str(error))
        print(error, file=sys.stderr)
    finally:
        (aot.LOGS / "report.json").write_text(json.dumps(aot.REPORT, indent=2) + "\n")
        print(f"Evidence: {aot.LOGS}", flush=True)
    sys.exit(0 if aot.REPORT["passed"] else 1)
