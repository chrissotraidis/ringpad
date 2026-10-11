#!/usr/bin/env python3
"""Recover hash-matching title source from public Git history, never game assets.

Exit 2 means some files remain unavailable. This does not establish G1 closure.
"""
import hashlib
import json
from pathlib import Path, PurePosixPath
import subprocess

ROOT = Path(__file__).resolve().parents[1]
REFERENCE = ROOT / 'ref/sarecomp'
R354_TITLE_REVISION = '40cb8ca05dd849390cbcde31f4b00da0a4137511'


def blob(revision, path):
    result = subprocess.run(['git', '-C', str(REFERENCE), 'show',
                             f'{revision}:{path}'], capture_output=True)
    return result.stdout if result.returncode == 0 else None


def main():
    pins = json.loads((ROOT / 'config/pins.json').read_text())
    current = next(p['pin'] for p in pins['references'] if p['name'] == 'SARecomp')
    manifest = json.loads(blob(current, 'baseline/r354.json'))
    output = ROOT / '.local/source-only/r354/title-source'
    records = []
    for entry in manifest['files']:
        if '/title-source/' not in entry['path']:
            continue
        relative = PurePosixPath(entry['path'].split('/title-source/', 1)[1])
        if relative.is_absolute() or '..' in relative.parts:
            raise SystemExit('Unsafe source manifest path')
        record = {'path': str(relative), 'sha256': entry['sha256'], 'status': 'missing'}
        for revision in (current, R354_TITLE_REVISION):
            data = blob(revision, str(relative))
            if data is None or len(data) != entry['bytes']:
                continue
            if hashlib.sha256(data).hexdigest() != entry['sha256']:
                continue
            target = output.joinpath(*relative.parts)
            target.parent.mkdir(parents=True, exist_ok=True)
            if target.exists():
                if target.read_bytes() != data:
                    raise SystemExit(f'Refusing to replace unknown work: {target}')
            else:
                with target.open('xb') as stream:
                    stream.write(data)
            record.update(status='present-verified', revision=revision, bytes=len(data))
            break
        records.append(record)
    output.mkdir(parents=True, exist_ok=True)
    (output / 'source-provenance.json').write_text(json.dumps(records, indent=2) + '\n')
    missing = [r['path'] for r in records if r['status'] == 'missing']
    print(f'R354_TITLE_SOURCE verified={len(records)-len(missing)}/{len(records)}')
    print(f'Source: {output}')
    for path in missing:
        print(f'MISSING: {path}')
    print('G1 runtime/generated guest/media interfaces/disc: still require verification')
    return 2 if missing else 0


if __name__ == '__main__':
    raise SystemExit(main())
