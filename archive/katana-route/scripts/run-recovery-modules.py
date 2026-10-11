#!/usr/bin/env python3
"""Synthetic independently compiled module activation; not a game loader integration."""
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
    if len(sys.argv)!=1:raise ValueError('usage: run-recovery-modules.py')
    control=aot.LOGS/'control.bin';control.write_bytes(struct.pack('<3H',0xE02A,0x000B,0x0009))
    sys.argv=[sys.argv[0],'--coverage',str(control)];aot.main()
    aot.REPORT.update(passed=False,scope='Synthetic code-module activation only',r3_complete=False,r4_complete=False)
    generator=aot.LOGS/'module-analysis';generated=aot.LOGS/'module-generated';build=aot.LOGS/'module-build'
    aot.run('build-module-analysis',['/usr/bin/clang++','-std=c++20','-O2','-arch','arm64','-mmacosx-version-min=13.0','-I',aot.SOURCE/'include',ROOT/'tests/recovery/module-analysis.cpp',aot.BUILD/'libdcrecomp_core.a','-o',generator])
    aot.run('generate-module',[generator,generated])
    aot.configure('configure-module',ROOT/'tests/recovery/modules',build,[f'-DFIXTURE_DIR={generated}'])
    aot.run('build-module',['cmake','--build',build,'--target','recovery_oracle','--parallel','4'])
    binary=build/'recovery_oracle'
    if 'Mach-O 64-bit executable arm64' not in aot.run('identity-module',['file',binary]):raise ValueError('Not ARM64')
    imports=aot.run('imports-module',['/usr/bin/nm','-u',binary])
    if re.search(r'\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b',imports):raise ValueError('Review executable-memory/process imports')
    result=aot.run('execute-module',[binary],timeout=15).strip()
    aot.REPORT.update(passed=True,activation_result=result,binary_sha256=aot.sha256(binary),source_sha256={str(p):aot.sha256(p) for p in (Path(__file__).resolve(),ROOT/'tests/recovery/module-analysis.cpp',ROOT/'tests/recovery/module-execution.cpp',ROOT/'tests/recovery/modules/CMakeLists.txt')})
    aot.REPORT['generated_sha256']={str(p):aot.sha256(p) for p in generated.rglob('*') if p.is_file()}
    print(json.dumps(result),flush=True)

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:
        (aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n');print(f'Evidence: {aot.LOGS}',flush=True)
    sys.exit(0 if aot.REPORT['passed'] else 1)
