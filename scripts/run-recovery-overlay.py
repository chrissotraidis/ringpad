#!/usr/bin/env python3
"""Synthetic overlay/literal diagnostics; a diagnostic pass is not overlay support."""
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)

def main():
    if len(sys.argv)!=1:raise ValueError('usage: run-recovery-overlay.py')
    control=aot.LOGS/'control.bin';control.write_bytes(struct.pack('<3H',0xE02A,0x000B,0x0009))
    sys.argv=[sys.argv[0],'--coverage',str(control)];aot.main()
    aot.REPORT.update(passed=False,scope='Synthetic overlay diagnostic only',r3_complete=False,r4_complete=False)
    generator=aot.LOGS/'overlay-analysis';generated=aot.LOGS/'overlay-generated';build=aot.LOGS/'overlay-build'
    aot.run('build-overlay-analysis',['/usr/bin/clang++','-std=c++20','-O2','-arch','arm64','-mmacosx-version-min=13.0','-I',aot.SOURCE/'include',ROOT/'tests/recovery/overlay-analysis.cpp',aot.BUILD/'libdcrecomp_core.a','-o',generator])
    aot.run('generate-overlay',[generator,generated])
    aot.configure('configure-overlay',ROOT/'tests/recovery',build,[f'-DFIXTURE_DIR={generated}','-DFIXTURE=10',f'-DORACLE_SOURCE={ROOT / "tests/recovery/overlay-execution.cpp"}'])
    aot.run('build-overlay',['cmake','--build',build,'--target','recovery_oracle','--parallel','4'])
    binary=build/'recovery_oracle'
    if 'Mach-O 64-bit executable arm64' not in aot.run('identity-overlay',['file',binary]):raise ValueError('Not ARM64')
    imports=aot.run('imports-overlay',['/usr/bin/nm','-u',binary])
    if re.search(r'\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b',imports):raise ValueError('Review executable-memory/process imports')
    result=json.loads(aot.run('execute-overlay',[binary],timeout=15))
    aot.REPORT.update(passed=True,diagnostic=result,binary_sha256=aot.sha256(binary),source_sha256={str(p):aot.sha256(p) for p in (Path(__file__).resolve(),ROOT/'tests/recovery/overlay-analysis.cpp',ROOT/'tests/recovery/overlay-execution.cpp')})
    print(json.dumps(result),flush=True)

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:
        (aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n');print(f'Evidence: {aot.LOGS}',flush=True)
    sys.exit(0 if aot.REPORT['passed'] else 1)
