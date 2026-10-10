#!/usr/bin/env python3
"""Execute an exact owner-image registration slice; never boots the game."""
import importlib.util
import json
from pathlib import Path
import re
import sys
ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("aot", ROOT / "scripts/run-recovery-aot.py")
aot = importlib.util.module_from_spec(spec)
spec.loader.exec_module(aot)


def main():
    if len(sys.argv) != 3:
        raise ValueError("usage: run-recovery-registration.py BOOT.BIN DECODED_SBOARD.BIN")
    boot, board = [Path(x).resolve(strict=True) for x in sys.argv[1:]]
    identities = {str(boot): aot.sha256(boot), str(board): aot.sha256(board)}
    if identities[str(boot)] != "68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29" or identities[str(board)] != "428dce0260d89a2e524691456cfb228bdc8cc4247631cd5e850eff69a28ff3ac":
        raise ValueError("Routine boundaries require exact declared owner-input identities")
    allocator = aot.LOGS / "allocator.bin"
    allocator.write_bytes(boot.read_bytes()[0x9A720-0x10000:0x9A804-0x10000])
    sys.argv = [sys.argv[0], "--coverage", str(allocator), "--base", "0x8C09A720"]
    aot.main()  # Pin/patch/toolchain verification and bounded allocator analysis.
    aot.REPORT.update(passed=False, scope="Native isolated SBOARD registration; declared heap/task state, no scheduler/game boot",
                      r3_complete=False, r4_complete=False, inputs=identities)
    generator = aot.LOGS / "registration-generator"
    generated, build = aot.LOGS / "registration-generated", aot.LOGS / "registration-build"
    aot.run("build-registration-generator", ["/usr/bin/clang++", "-std=c++20", "-O2", "-arch", "arm64",
        "-mmacosx-version-min=13.0", "-I", aot.SOURCE / "include",
        ROOT / "tests/recovery/registration-generator.cpp", aot.BUILD / "libdcrecomp_core.a", "-o", generator])
    aot.REPORT["slice"] = json.loads(aot.run("generate-registration", [generator, boot, board, generated]))
    aot.configure("configure-registration", ROOT / "tests/recovery", build,
        [f"-DFIXTURE_DIR={generated}", "-DFIXTURE=7", f"-DORACLE_SOURCE={ROOT / 'tests/recovery/registration-execution.cpp'}"])
    aot.run("build-registration", ["cmake", "--build", build, "--target", "recovery_oracle", "--parallel", "4"])
    binary = build / "recovery_oracle"
    if "Mach-O 64-bit executable arm64" not in aot.run("identity-registration", ["file", binary]):
        raise ValueError("Oracle is not native ARM64")
    imports = aot.run("imports-registration", ["/usr/bin/nm", "-u", binary])
    if re.search(r"\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b", imports):
        raise ValueError("Review executable-memory/process imports")
    result = aot.run("execute-registration", [binary], timeout=30)
    if result.count("PASS size=") != 4:
        raise ValueError("Missing registration case")
    if any(aot.sha256(Path(p)) != h for p, h in identities.items()):
        raise ValueError("Original input changed")
    aot.REPORT.update(passed=True, result=result, binary_sha256=aot.sha256(binary),
        generated_cpp_sha256=aot.sha256(generated / "generated_program.cpp"),
        harness_sha256=aot.sha256(ROOT / "tests/recovery/registration-execution.cpp"),
        checks_sha256=aot.sha256(ROOT / "tests/recovery/registration-check.hpp"),
        generator_sha256=aot.sha256(ROOT / "tests/recovery/registration-generator.cpp"))
    print(result.strip(), flush=True)


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
