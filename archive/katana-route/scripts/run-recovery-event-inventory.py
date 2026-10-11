#!/usr/bin/env python3
"""Bind owner event modules to the original loader/header; optionally audit all."""
import argparse
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('aot', ROOT / 'scripts/run-recovery-aot.py')
aot = importlib.util.module_from_spec(spec)
spec.loader.exec_module(aot)
BOOT_SHA = '68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29'
DISC_SHA = '105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533'


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('boot', type=Path)
    p.add_argument('scan', type=Path)
    p.add_argument('--audit', action='store_true')
    a = p.parse_args()
    if aot.sha256(a.boot) != BOOT_SHA:
        raise ValueError('Wrong owner boot identity')
    data = a.boot.read_bytes()
    u32 = lambda address: struct.unpack_from('<I', data, address - 0x8C010000)[0]
    # Original 8C04B502 formats ev%04X.prs and uses combined loader 8C09F664.
    # On success it reads header[0], stores it at 8C74D320, then JSRs it with R4=0.
    if u32(0x8C04B5F8) != 0x0CB80000 or u32(0x8C04B610) != 0x8C09F664:
        raise ValueError('Event loader destination/provider changed')
    fmt = u32(0x8C04B600) - 0x8C010000
    if data[fmt:fmt+11] != b'ev%04X.prs\0':
        raise ValueError('Event filename contract changed')
    if struct.unpack_from('<2H', data, 0x4B56C - 0x10000) != (0x420B, 0xE400):
        raise ValueError('Event entry invocation changed')
    scan = json.loads(a.scan.read_text())
    if not scan['completed'] or scan['disc_sha256'] != DISC_SHA or scan['disc_sha256_after'] != DISC_SHA:
        raise ValueError('Incomplete or different disc scan')
    modules = []
    for row in scan['rows']:
        if not re.fullmatch(r'SONICADV/EV[0-9A-F]{4}\.PRS', row['path']):
            continue
        raw = Path(row['private_directory']) / 'decoded.bin'
        if aot.sha256(raw) != row['decoded_sha256'] or aot.sha256(raw.with_name('encoded.prs')) != row['encoded_sha256']:
            raise ValueError('Event input changed')
        entry, secondary = struct.unpack_from('<II', raw.read_bytes())
        if entry & 1 or not 0x0CB80000 <= entry < 0x0CB80000 + raw.stat().st_size:
            raise ValueError('Invalid event code header: ' + row['path'])
        modules.append({'file': row['path'], 'directory': str(raw.parent), 'base': '0x8CB80000',
                        'entries': [hex(entry | 0x80000000)], 'secondary_header': hex(secondary),
                        'root_basis': 'Original event loader reads header[0] then JSR with R4=0',
                        'decoded_size': row['decoded_size'], 'decoded_sha256': row['decoded_sha256'],
                        'encoded_sha256': row['encoded_sha256'], 'prefix_kind': row['prefix_kind']})
    if not modules:
        raise ValueError('No event modules found')
    root = aot.LOGS
    report = {'scope': 'Static event loader/header binding; runtime loading and playback unproved',
              'passed': False, 'r3_complete': False, 'boot_sha256': BOOT_SHA,
              'scan_sha256': aot.sha256(a.scan), 'script_sha256': aot.sha256(Path(__file__)),
              'loader': '0x8C04B502', 'entry_call': '0x8C04B56C', 'modules': modules, 'remaining': []}
    try:
        if a.audit:
            control = root / 'control.bin'
            control.write_bytes(struct.pack('<3H', 0xE02A, 0x000B, 0x0009))
            sys.argv = [sys.argv[0], '--coverage', str(control)]
            aot.main()
            report['candidate_pin'] = aot.PIN
            report['candidate_patches'] = aot.REPORT['patches']
            reference = aot.WORK / 'prs-reference/prs-reference'
            if aot.sha256(reference) != 'abff04206838dbe559c62153d8405c654c897ce6a6824643aaccd6ae8994c82d':
                raise ValueError('Reference decoder changed')
            for index, module in enumerate(modules):
                raw = Path(module['directory']) / 'decoded.bin'
                out = root / Path(module['file']).stem
                out.mkdir()
                aot.LOGS = out
                try:
                    aot.run('reference-decode', [reference, raw.with_name('encoded.prs'), out/'reference.bin', str(module['decoded_size'])], timeout=30)
                    if aot.sha256(out/'reference.bin') != module['decoded_sha256']:
                        raise ValueError('Reference decoder disagrees')
                    log = aot.run('raw-closure', [aot.BUILD/'dc_raw_recomp', raw, '--base=0x8CB80000',
                        '--entry='+module['entries'][0], '--max-functions=16384', '--closure-passes=32',
                        f'--output={out}', f'--map={out/"functions.csv"}', '--no-emit'], timeout=120)
                    metrics = {k.strip(): int(v) for k, v in re.findall(r'^([A-Za-z][A-Za-z0-9 /_-]+):\s+(\d+)\s*$', log, re.M)}
                    module['closure_metrics'] = metrics
                    if '[WARN]' in log or metrics['Closure passes'] >= 32:
                        raise ValueError('Closure bound exhausted')
                    module['coverage'] = json.loads(aot.run('audit-coverage', [aot.WORK/'coverage-audit', raw,
                        module['base'], out/'functions.csv', out, '--entry='+module['entries'][0]], timeout=60))
                    module['evidence'] = str(out)
                    module['audit_completed'] = True
                    # Keep decoder gaps visible; a completed audit is not coverage acceptance.
                    module['has_unknown'] = bool(metrics['Unknown SH-4'] or metrics['RAW_SH4'])
                except Exception as error:
                    module['audit_completed'] = False
                    module['error'] = str(error)
                    report['remaining'].append(module['file'])
                if aot.sha256(raw) != module['decoded_sha256']:
                    raise ValueError('Original input changed')
                (root/'event-inventory.json').write_text(json.dumps(report, indent=2)+'\n')
                print(f'{index+1}/{len(modules)} {module["file"]} audited={module["audit_completed"]}', flush=True)
        report['passed'] = not report['remaining']
        aot.REPORT['passed'] = report['passed']
    finally:
        aot.LOGS = root
        report['original_boot_unchanged'] = aot.sha256(a.boot) == BOOT_SHA
        (root/'event-inventory.json').write_text(json.dumps(report, indent=2)+'\n')
        print('Evidence:', root, flush=True)


if __name__ == '__main__':
    aot.LOGS.mkdir(parents=True)
    try:
        main()
    except Exception as error:
        aot.REPORT.update(passed=False, error=str(error))
        print(error, file=sys.stderr)
    finally:
        (aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT, indent=2)+'\n')
    sys.exit(0 if aot.REPORT['passed'] else 1)
