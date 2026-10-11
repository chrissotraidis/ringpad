#!/usr/bin/env python3
"""R0-R2 synthetic AOT checks, or R3 disc/coverage audits. Never executes a game."""
import argparse
from collections import Counter
import csv
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import signal
import struct
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
WORK = ROOT / "work/dreamcast-recovery"
SOURCE = WORK / "source"
BUILD = WORK / "build"
PIN = "ba7df34a50b5a7c676a590bb8b50bd784328c9b9"
LOGS = ROOT / "docs/artifacts/recovery-aot" / time.strftime("%Y%m%d-%H%M%S")
ENV = dict(os.environ, DEVELOPER_DIR="/Library/Developer/CommandLineTools",
           TMPDIR=str(WORK / "tmp"), DCR_GPR_CACHE="0", DCR_EMIT_HOT_TRACE="0")
REPORT = {"source": "https://github.com/Duw0ng/DreamcastRecompiled.git", "pin": PIN,
          "scope": "synthetic SH-4 AOT on macOS ARM64 only", "steps": [], "passed": False}


def run(name, args, timeout=180):
    args = [str(arg) for arg in args]
    print(name, flush=True)
    log = LOGS / (name + ".log")
    step = {"name": name, "argv": args, "log": str(log), "timeout_seconds": timeout,
            "codegen_environment": {key: ENV[key] for key in ("DCR_GPR_CACHE", "DCR_EMIT_HOT_TRACE")}}
    REPORT["steps"].append(step)
    with log.open("w") as output:
        process = subprocess.Popen(args, cwd=ROOT, env=ENV, stdout=output,
                                   stderr=subprocess.STDOUT, start_new_session=True)
        try:
            step["exit"] = process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            # Ninja gives compiler jobs separate process groups. Snapshot this
            # command's descendants before killing the parent so none escape.
            parents = {}
            for row in subprocess.check_output(['ps', '-axo', 'pid=,ppid='], text=True).splitlines():
                pid, parent = map(int, row.split())
                parents.setdefault(parent, []).append(pid)
            descendants = []
            pending = [process.pid]
            while pending:
                children = parents.get(pending.pop(), [])
                descendants.extend(children)
                pending.extend(children)
            for pid in reversed(descendants):
                try:
                    os.kill(pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            process.wait()
            step['terminated_descendants'] = descendants
            step["timed_out"] = True
            raise RuntimeError(f"{name} timed out; see {log}")
    text = log.read_text()
    if step["exit"]:
        raise RuntimeError(f"{name} failed ({step['exit']}):\n{text[-3000:]}")
    return text


def configure(name, source, build, extra=()):
    run(name, ["cmake", "-S", source, "-B", build, "-G", "Ninja",
               "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_CXX_COMPILER=/usr/bin/clang++",
               "-DCMAKE_OSX_ARCHITECTURES=arm64", "-DCMAKE_OSX_DEPLOYMENT_TARGET=13.0", *extra])


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def probe_disc(disc):
    disc = disc.resolve(strict=True)
    REPORT.update(scope="R3 partial static disc inventory; no game execution", r3_complete=False)
    REPORT["disc"] = {"path": str(disc), "size": disc.stat().st_size, "sha256_before": sha256(disc)}
    try:
        run("build-disc-tools", ["cmake", "--build", BUILD, "--target", "dc_disc_probe",
                                 "dc_boot_prepare", "dc_raw_boot_probe", "--parallel", "4"])
        run("disc-inventory", [BUILD / "dc_disc_probe", disc,
                              f"--extract-ip={LOGS / 'IP.BIN'}",
                              f"--extract-boot={LOGS / 'BOOT.DISC.BIN'}",
                              f"--extract-disc-map={LOGS / 'disc-map.txt'}"])
        inventory_disc_root(disc)
        run("prepare-boot", [BUILD / "dc_boot_prepare", LOGS / "IP.BIN", LOGS / "BOOT.DISC.BIN",
                             "--mode=auto", f"--boot-out={LOGS / 'BOOT.BIN'}",
                             f"--combined-out={LOGS / 'BOOTSTRAP.BIN'}"])
        for label, file, base, entry in [
            ("bootstrap", "BOOTSTRAP.BIN", "0x8C008000", "0x8C008300"),
            ("game-entry", "BOOT.BIN", "0x8C010000", "0x8C010000")]:
            output = run(f"discover-{label}", [BUILD / "dc_raw_boot_probe", LOGS / file,
                         f"--base={base}", f"--entry={entry}", "--max-blocks=8192", "--max-instructions=300000"])
            REPORT[label] = {name.strip(): int(value) for name, value in
                            re.findall(r"^([A-Za-z][A-Za-z0-9 -]+):\s+(\d+)\s*$", output, re.M)}
            REPORT[label]["limit_reached"] = "[LIMIT]" in output
            print(output.strip(), flush=True)
        REPORT["extracted_sha256"] = {name: sha256(LOGS / name)
                                      for name in ["IP.BIN", "BOOT.DISC.BIN", "BOOT.BIN", "BOOTSTRAP.BIN"]}
        REPORT["remaining"] = ["Validate bootstrap preparation and actual entry semantics",
                               "Resolve indirect and out-of-image targets", "Inventory loaded/decompressed/auxiliary code",
                               "Generate and execute Sonic-specific C++; graphics and audio remain unverified"]
    finally:
        REPORT["disc"]["sha256_after"] = sha256(disc)
        if REPORT["disc"]["sha256_before"] != REPORT["disc"]["sha256_after"]:
            raise RuntimeError("Original disc identity changed during probe")
    REPORT["passed"] = True  # Probe completed; r3_complete remains false.


def inventory_disc_root(disc):
    """Read the observed absolute-LBA ISO layout; this is not a generic importer."""
    fields = dict(line.split("=", 1) for line in (LOGS / "disc-map.txt").read_text().splitlines() if "=" in line)
    prefix = f"track.{fields['boot_track']}."
    start = int(fields[prefix + "start_fad"]) - 150
    end = int(fields[prefix + "end_fad"]) - 150
    raw_size = int(fields[prefix + "sector_size"])
    user_offset = int(fields[prefix + "user_offset"])
    file_base = int(fields[prefix + "file_base"])
    with disc.open("rb") as image:
        def read(extent, size):
            if size < 0 or size > 16 * 1024 * 1024 or extent < start or extent + (size + 2047) // 2048 > end + 1:
                raise RuntimeError("Directory/file extent outside bounded absolute-LBA layout")
            result = bytearray()
            while len(result) < size:
                image.seek(file_base + (extent - start + len(result) // 2048) * raw_size + user_offset)
                part = image.read(min(2048, size - len(result)))
                if len(part) != min(2048, size - len(result)):
                    raise RuntimeError("Short ISO sector read")
                result.extend(part)
            return result
        pvd = read(start + 16, 2048)
        if pvd[:7] != b"\x01CD001\x01" or int.from_bytes(pvd[128:130], "little") != 2048:
            raise RuntimeError("Unsupported PVD placement or block size; inspect manually")
        root_extent = int.from_bytes(pvd[158:162], "little")
        pending = [("", root_extent, int.from_bytes(pvd[166:170], "little"))]
        rows, seen = [], {root_extent}
        while pending:
            parent, extent, length = pending.pop()
            directory, offset = read(extent, length), 0
            while offset < len(directory):
                size = directory[offset]
                if size == 0:
                    offset = (offset // 2048 + 1) * 2048
                    continue
                record = directory[offset:offset + size]
                if len(record) != size or size < 34 or 33 + record[32] > size:
                    raise RuntimeError("Invalid ISO directory record")
                offset += size
                name = bytes(record[33:33 + record[32]])
                if name in (b"\0", b"\1"):
                    continue
                extent, length = int.from_bytes(record[2:6], "little"), int.from_bytes(record[10:14], "little")
                if extent != int.from_bytes(record[6:10], "big") or length != int.from_bytes(record[14:18], "big"):
                    raise RuntimeError("ISO endian copies disagree")
                row = {"name": name.decode("ascii").split(";")[0], "extent": extent,
                       "size": length, "directory": bool(record[25] & 2), "multi_extent": bool(record[25] & 128)}
                row["path"] = parent + row["name"]
                rows.append(row)
                if len(rows) > 16384:
                    raise RuntimeError("Directory inventory limit reached")
                if row["directory"]:
                    if row["multi_extent"] or extent in seen or parent.count("/") >= 16:
                        raise RuntimeError("Unsupported directory graph")
                    seen.add(extent)
                    pending.append((row["path"] + "/", extent, length))
                if row["name"] in ("1ST_READ.BIN", "MINICART.PRS"):
                    if row["directory"] or row["multi_extent"]:
                        raise RuntimeError("Unsupported code-candidate entry type")
                    data = read(extent, length)
                    row["sha256"] = hashlib.sha256(data).hexdigest()
                    if row["name"] == "1ST_READ.BIN" and row["sha256"] != sha256(LOGS / "BOOT.DISC.BIN"):
                        raise RuntimeError("Independent ISO reader disagrees with candidate boot extraction")
                    if row["name"] == "MINICART.PRS":
                        (LOGS / "MINICART.PRS").write_bytes(data)
        (LOGS / "disc-files.json").write_text(json.dumps(rows, indent=2) + "\n")
        REPORT["disc_inventory"] = {"entries": len(rows), "directories": len(seen),
            "code_or_compressed_candidates": [r for r in rows if r["name"].endswith((".BIN", ".PRS"))],
            "scope": "Directory tree traversed; names/extensions do not prove executable content"}



def gap_fixture():
    # Renesas MOV.L R0,@(disp,GBR): 11000010dddddddd; offset is disp * 4.
    # These two legal instructions occur in the inspected Sonic image.
    # Record both the old decoder gap and the repaired state without conflating
    # successful static lowering with the separate native execution oracle.
    raw = LOGS / "gbr-store.bin"
    raw.write_bytes(struct.pack("<4H", 0xC216, 0xC20E, 0x000B, 0x0009))
    output = run("gbr-store-gap", [BUILD / "dc_raw_recomp", raw, "--base=0x8C010000",
                 "--entry=0x8C010000", "--max-functions=16", "--closure-passes=2",
                 f"--output={LOGS / 'gbr-gap'}", "--no-emit"])
    metrics = {name.strip(): int(value) for name, value in
               re.findall(r"^([A-Za-z][A-Za-z0-9 /_-]+):\s+(\d+)\s*$", output, re.M)}
    REPORT["gbr_gap_fixture"] = metrics
    REPORT["gbr_gap_present"] = metrics.get("Unknown SH-4") == 2 and metrics.get("RAW_SH4") == 2


def coverage(boot, inspect, base, seed_files):
    boot = boot.resolve(strict=True)
    REPORT.update(scope="R3 static closure audit; no game source emission or execution", r3_complete=False)
    REPORT["boot"] = {"path": str(boot), "sha256": sha256(boot)}
    REPORT["placement_hypothesis"] = base
    REPORT["seed_files"] = [{"path": str(path.resolve(strict=True)), "sha256": sha256(path)} for path in seed_files]
    run("build-raw-recompiler", ["cmake", "--build", BUILD, "--target", "dc_raw_recomp", "--parallel", "4"])
    gap_fixture()
    output = run("raw-closure", [BUILD / "dc_raw_recomp", boot, f"--base={base}", f"--entry={base}",
                 *[f"--seed-file={path.resolve()}" for path in seed_files],
                 "--max-functions=16384", "--closure-passes=32", f"--output={LOGS}",
                 f"--map={LOGS / 'functions.csv'}", "--no-emit"], timeout=120)
    REPORT["closure_metrics"] = {name.strip(): int(value) for name, value in
                                re.findall(r"^([A-Za-z][A-Za-z0-9 /_-]+):\s+(\d+)\s*$", output, re.M)}
    REPORT["closure_limit_warning"] = "[WARN]" in output
    REPORT["pass_budget_exhausted"] = REPORT["closure_metrics"].get("Closure passes") == 32
    binary = WORK / "coverage-audit"
    run("build-coverage-audit", ["/usr/bin/clang++", "-std=c++20", "-O2", "-arch", "arm64",
        "-mmacosx-version-min=13.0", "-I", SOURCE / "include", ROOT / "tests/recovery/coverage.cpp",
        BUILD / "libdcrecomp_core.a", "-o", binary])
    # An unknown opcode inside the root must remain visible in our audit report.
    fixture_map = LOGS / "fixture-functions.csv"
    fixture_map.write_text("address,name\n0x8C010000,synthetic_gbr_store\n")
    run("audit-gap-control", [binary, LOGS / "gbr-store.bin", "0x8C010000", fixture_map,
        LOGS / "gap-control", "0x8C010000", "0x8C010004"])
    with (LOGS / "gap-control/inspected-sites.csv").open() as source:
        control = list(csv.DictReader(source))
    if not (control[0]["in_discovered_closure"] == "1" and
            control[1]["known"] == "1" and control[1]["in_discovered_closure"] == "1"):
        raise RuntimeError("Coverage control lost the root instruction or its return")
    REPORT["coverage_control_passed"] = True
    REPORT["coverage"] = json.loads(run("audit-coverage", [binary, boot, base,
                                    LOGS / "functions.csv", LOGS, *inspect], timeout=60))
    with (LOGS / "external-calls.csv").open() as source:
        rows = list(csv.DictReader(source))
    REPORT["external_reasons"] = dict(Counter(row["reason"] for row in rows))
    REPORT["unique_external_sites"] = len({row["site"] for row in rows})
    with (LOGS / "inspected-sites.csv").open() as source:
        REPORT["inspected_sites"] = list(csv.DictReader(source))
    if sha256(boot) != REPORT["boot"]["sha256"]:
        raise RuntimeError("Boot input changed during static audit")
    REPORT["passed"] = True
    print(json.dumps(REPORT["coverage"]), flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--disc-probe", type=Path, help="Inspect an owner-held disc instead of running synthetic tests")
    mode.add_argument("--coverage", type=Path, help="Audit static code discovery from an extracted BOOT.BIN")
    parser.add_argument("--inspect-address", action="append", default=[], help="Check a specific guest address in --coverage mode")
    parser.add_argument("--base", default="0x8C010000", help="Placement/root hypothesis for --coverage")
    parser.add_argument("--seed-file", type=Path, action="append", default=[], help="Additional entry hypotheses for --coverage")
    args = parser.parse_args()
    if platform.system() != "Darwin" or platform.machine() != "arm64":
        raise RuntimeError("This experiment requires native macOS ARM64")
    if not SOURCE.exists():
        run("clone", ["git", "clone", "--filter=blob:none", "--no-checkout", REPORT["source"], SOURCE])
        run("disable-push", ["git", "-C", SOURCE, "remote", "set-url", "--push", "origin", "DISABLED"])
        run("sparse", ["git", "-C", SOURCE, "sparse-checkout", "set", "--cone", "src", "include", "tests", "samples"])
        run("checkout", ["git", "-C", SOURCE, "checkout", "--detach", PIN])
    if run("pin", ["git", "-C", SOURCE, "rev-parse", "HEAD"]).strip() != PIN:
        raise RuntimeError("Candidate pin mismatch; preserve checkout and inspect it")
    run("source-status", ["git", "-C", SOURCE, "status", "--porcelain"])
    patches = [ROOT / "patches/recovery" / name for name in
               ("0001-gbr-long-store.patch", "0002-cfg-call-analysis.patch", "0003-cfg-call-candidates.patch", "0004-bulk-literal-invalidation.patch", "0005-explicit-code-modules.patch", "0006-explicit-data-ranges.patch", "0007-gbr-long-load.patch", "0008-gbr-load-analysis-clobber.patch", "0009-explicit-system-flash.patch", "0010-read-only-fifo-status.patch", "0011-post-bios-aica-idle.patch", "0012-in-memory-flash-program-erase.patch")]
    actual = run("source-diff", ["git", "-C", SOURCE, "diff", "HEAD", "--binary"])
    if run("source-untracked", ["git", "-C", SOURCE, "ls-files", "--others", "--exclude-standard"]).strip():
        raise RuntimeError("Candidate has unrecorded files; preserve and inspect")
    # Compute canonical diffs in an isolated index. Never alter the user's index.
    expected = [""]
    with tempfile.TemporaryDirectory(dir=WORK / "tmp") as temp:
        previous_index = ENV.get("GIT_INDEX_FILE")
        ENV["GIT_INDEX_FILE"] = str(Path(temp) / "index")
        try:
            run("patch-index", ["git", "-C", SOURCE, "read-tree", "HEAD"])
            for n, patch in enumerate(patches, 1):
                run(f"index-patch-{n}", ["git", "-C", SOURCE, "apply", "--cached", patch])
                expected.append(run(f"expected-diff-{n}", ["git", "-C", SOURCE,
                    "diff", "--cached", "HEAD", "--binary"]))
        finally:
            if previous_index is None:
                ENV.pop("GIT_INDEX_FILE", None)
            else:
                ENV["GIT_INDEX_FILE"] = previous_index
    if actual not in expected:
        raise RuntimeError("Candidate changes differ from the recorded patch; preserve and inspect")
    for n in range(expected.index(actual), len(patches)):
        run(f"check-patch-{n+1}", ["git", "-C", SOURCE, "apply", "--check", patches[n]])
        run(f"apply-patch-{n+1}", ["git", "-C", SOURCE, "apply", patches[n]])
    if run("applied-diff", ["git", "-C", SOURCE, "diff", "HEAD", "--binary"]) != expected[-1]:
        raise RuntimeError("Applied patch sequence mismatch")
    REPORT["patches"] = [{"path": str(p), "sha256": sha256(p)} for p in patches]
    if run("push-url", ["git", "-C", SOURCE, "remote", "get-url", "--push", "origin"]).strip() != "DISABLED":
        raise RuntimeError("Candidate push URL must be DISABLED")
    REPORT["compiler"] = run("compiler", ["/usr/bin/clang++", "--version"]).strip()
    REPORT["sdk"] = run("sdk", ["xcrun", "--sdk", "macosx", "--show-sdk-version"]).strip()
    configure("configure-generator", SOURCE, BUILD)
    if args.disc_probe:
        probe_disc(args.disc_probe)
        return
    if args.coverage:
        coverage(args.coverage, args.inspect_address, args.base, args.seed_file)
        return
    targets = ["sh4_decoder_tests", "branch_control_tests", "memory_ops_tests", "fpu_arith_tests", "cross_branch_tests",
               "function_analysis_tests", "cfg_dcir_tests", "program_analysis_tests", "noreturn_flow_tests"]
    run("build-generator", ["cmake", "--build", BUILD, "--target", "dc_recomp", "dc_raw_recomp", *targets, "--parallel", "4"])
    identity = run("generator-identity", ["file", BUILD / "dc_recomp"])
    if "Mach-O 64-bit executable arm64" not in identity:
        raise RuntimeError("Generator is not native ARM64")
    cases = ["branches", "memory", "fpu_arith"]
    expression = "^(" + "|".join(targets + [f"dc_recomp_{c}_codegen" for c in cases]) + ")$"
    run("focused-tests", ["ctest", "--test-dir", BUILD, "-R", expression, "--output-on-failure", "--timeout", "60"])
    # 256 encodings cover zero/nonzero/max unsigned displacement. Self-moves
    # preserve R1 while making the optional GPR cache eligible for a later check.
    raw = LOGS / "gbr-stores.bin"
    words = [0xC200 + d for d in range(256)] + [0x6113] * 16 + [0x000B, 0x0009]
    raw.write_bytes(struct.pack("<" + "H" * len(words), *words))
    run("generate-gbr-stores", [BUILD / "dc_raw_recomp", raw,
        f"--output={BUILD / 'dc_recomp_gbr_stores_output'}", "--max-functions=16", "--closure-passes=2"])
    ENV["DCR_GPR_CACHE"] = "1"
    try:
        run("generate-gbr-cached", [BUILD / "dc_raw_recomp", raw,
            f"--output={BUILD / 'dc_recomp_gbr_cached_output'}", "--max-functions=16", "--closure-passes=2"])
    finally:
        ENV["DCR_GPR_CACHE"] = "0"
    if "gpc_gbr" not in (BUILD / "dc_recomp_gbr_cached_output/generated_program.cpp").read_text():
        raise RuntimeError("Register-cache test did not activate GBR caching")
    # Renesas SH-4A MOV.L @(disp,GBR),R0: all unsigned disp8 values,
    # copied to a separate output buffer so every intermediate load is observed.
    raw = LOGS / "gbr-loads.bin"
    words = [word for d in range(256) for word in (0xC600 + d, 0x2102, 0x7104)] + [0x000B, 0x0009]
    raw.write_bytes(struct.pack("<" + "H" * len(words), *words))
    for case, cache in (("gbr_loads", "0"), ("gbr_loads_cached", "1")):
        ENV["DCR_GPR_CACHE"] = cache
        try:
            run(f"generate-{case}", [BUILD / "dc_raw_recomp", raw,
                f"--output={BUILD / f'dc_recomp_{case}_output'}", "--max-functions=16", "--closure-passes=2"])
        finally:
            ENV["DCR_GPR_CACHE"] = "0"
    if "gpc_gbr" not in (BUILD / "dc_recomp_gbr_loads_cached_output/generated_program.cpp").read_text():
        raise RuntimeError("Load test did not activate GBR caching")
    diagnostic = WORK / "indirect-analysis"
    run("build-indirect-analysis", ["/usr/bin/clang++", "-std=c++20", "-O2", "-arch", "arm64",
        "-mmacosx-version-min=13.0", "-I", SOURCE / "include",
        ROOT / "tests/recovery/indirect-analysis.cpp", BUILD / "libdcrecomp_core.a", "-o", diagnostic])
    REPORT["indirect_discovery_diagnostic"] = json.loads(run("generate-indirect", [diagnostic,
        BUILD / "dc_recomp_indirect_output"]))
    for number, case in [*enumerate(cases, 1), (4, "gbr_stores"), (4, "gbr_cached"), (5, "indirect"), (6, "gbr_loads"), (6, "gbr_loads_cached")]:
        generated = BUILD / f"dc_recomp_{case}_output"
        build = WORK / f"oracle-{case}"
        configure(f"configure-{case}", ROOT / "tests/recovery", build,
                  [f"-DFIXTURE_DIR={generated}", f"-DFIXTURE={number}"])
        run(f"build-{case}", ["cmake", "--build", build, "--target", "recovery_oracle", "--parallel", "4"])
        binary = build / "recovery_oracle"
        identity = run(f"identity-{case}", ["file", binary])
        if "Mach-O 64-bit executable arm64" not in identity:
            raise RuntimeError(f"{case} is not native ARM64")
        imports = run(f"imports-{case}", ["/usr/bin/nm", "-u", binary])
        # Bounded linked-import screen, not a proof about every future game path.
        if re.search(r"\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b", imports):
            raise RuntimeError(f"Review executable-memory/process import in {case}")
        print(run(f"execute-{case}", [binary], timeout=15).strip(), flush=True)
        REPORT[case] = {"binary_sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
                        "generated_cpp_sha256": hashlib.sha256((generated / "generated_program.cpp").read_bytes()).hexdigest()}
    REPORT["passed"] = True


if __name__ == "__main__":
    LOGS.mkdir(parents=True)
    (WORK / "tmp").mkdir(parents=True, exist_ok=True)
    try:
        main()
    except Exception as error:
        REPORT["error"] = str(error)
        print(error, file=sys.stderr)
    finally:
        (LOGS / "report.json").write_text(json.dumps(REPORT, indent=2) + "\n")
        print(f"Evidence: {LOGS}", flush=True)
    sys.exit(0 if REPORT["passed"] else 1)
