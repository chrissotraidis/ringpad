#!/usr/bin/env python3
"""Bounded original BIOS-wrapper execution through public GD-ROM HLE."""
import importlib.util
import json
from pathlib import Path
import re
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)

def main():
    if len(sys.argv)!=3:raise ValueError('usage: run-recovery-disc-aot.py BOOT.BIN DISC_MAP')
    boot,discmap=[Path(p).resolve(strict=True) for p in sys.argv[1:]]
    fields=dict(line.split('=',1) for line in discmap.read_text().splitlines() if '=' in line)
    disc=Path(fields['image']).resolve(strict=True)
    if aot.sha256(boot)!='68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29':raise ValueError('Wrong owner boot')
    if aot.sha256(disc)!='105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533':raise ValueError('Wrong owner disc')
    identities={str(p):aot.sha256(p) for p in (boot,discmap,disc)}
    raw=aot.LOGS/'bios-wrappers.bin';raw.write_bytes(boot.read_bytes()[0xFF558:0xFF5A8])
    seeds=aot.LOGS/'seeds.txt';seeds.write_text('0x8C10F56C\n0x8C10F580\n0x8C10F594\n')
    sys.argv=[sys.argv[0],'--coverage',str(raw),'--base','0x8C10F558','--seed-file',str(seeds)]
    aot.main()
    aot.REPORT.update(passed=False,scope='Original BIOS wrappers using public GD-ROM HLE; file provider/game unexecuted',r3_complete=False,r4_complete=False,inputs=identities)
    metrics=aot.REPORT['closure_metrics']
    if metrics['Reachable functions']!=4 or metrics['Unknown SH-4'] or metrics['RAW_SH4']:
        raise ValueError('Unexpected BIOS-wrapper closure')
    aot.REPORT['execution']={'guest_cpu':'native ARM64 from four original SH4 wrappers',
        'disc_service':'public candidate GD-ROM HLE', 'title_provider_initialized':False}
    # Independently read the recorded absolute-LBA Mini Cart extent, including sector padding.
    extent,size=187264,331320;count=(size+2047)//2048;data=bytearray()
    prefix='track.1.'
    with disc.open('rb') as f:
        for n in range(count):
            offset=int(fields[prefix+'file_base'])+(extent+150-int(fields[prefix+'start_fad'])+n)*int(fields[prefix+'sector_size'])+int(fields[prefix+'user_offset'])
            f.seek(offset);part=f.read(2048)
            if len(part)!=2048:raise ValueError('Short reference read')
            data+=part
    import hashlib
    if hashlib.sha256(data[:size]).hexdigest()!='8f8d410eafb2315b79d52588ce7a5d249018277009f036091caf4651d7adc17b':raise ValueError('Mini Cart reference identity mismatch')
    expected=aot.LOGS/'expected-sectors.bin';expected.write_bytes(data)
    generated,build=aot.LOGS/'disc-generated',aot.LOGS/'disc-build'
    aot.run('generate-disc',[aot.BUILD/'dc_raw_recomp',raw,'--base=0x8C10F558','--entry=0x8C10F558',f'--seed-file={seeds}',f'--output={generated}','--max-functions=16','--closure-passes=2'])
    aot.configure('configure-disc',ROOT/'tests/recovery',build,[f'-DFIXTURE_DIR={generated}','-DFIXTURE=8',f'-DORACLE_SOURCE={ROOT / "tests/recovery/disc-execution.cpp"}'])
    aot.run('build-disc',[ 'cmake','--build',build,'--target','recovery_oracle','--parallel','4'])
    binary=build/'recovery_oracle'
    if 'Mach-O 64-bit executable arm64' not in aot.run('identity-disc',['file',binary]):raise ValueError('Not ARM64')
    imports=aot.run('imports-disc',['/usr/bin/nm','-u',binary])
    if re.search(r'\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b',imports):raise ValueError('Review executable-memory/process imports')
    result=aot.run('execute-disc',[binary,discmap,expected,str(extent+150)],timeout=30)
    if result.count('PASS ')!=3:raise ValueError('Missing disc case')
    if any(aot.sha256(Path(p))!=h for p,h in identities.items()):raise ValueError('Original input changed')
    aot.REPORT.update(passed=True,result=result,binary_sha256=aot.sha256(binary),expected_sha256=aot.sha256(expected),generated_sha256=aot.sha256(generated/'generated_program.cpp'),harness_sha256=aot.sha256(ROOT/'tests/recovery/disc-execution.cpp'))
    print(result.strip(),flush=True)

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:
        (aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n');print(f'Evidence: {aot.LOGS}',flush=True)
    sys.exit(0 if aot.REPORT['passed'] else 1)
