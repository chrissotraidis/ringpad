#!/usr/bin/env python3
"""Read-only PAL v1.003 GDI/track verifier; never extracts or acquires game data."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def expected_tracks():
    identity = json.loads((ROOT / 'config/game-identity.json').read_text())
    pins = json.loads((ROOT / 'config/pins.json').read_text())
    pin = next(p['pin'] for p in pins['references'] if p['name'] == 'SARecomp')
    header = subprocess.check_output(['git', '-C', str(ROOT / 'ref/sarecomp'),
        'show', f'{pin}:src/setup/supported_disc.hpp'], text=True)
    matches = re.findall(r'\{(\d+),(\d+),(\d+),(\d+),(\d+),"([0-9a-f]{64})"\}', header)
    authoritative = [dict(zip(('n', 'lba', 'type', 'sector_size', 'bytes'),
                               map(int, row[:5])), sha256=row[5]) for row in matches]
    if len(authoritative) != 3 or identity['supported']['tracks'] != authoritative:
        raise ValueError('Game identity config differs from the pinned supported-disc contract')
    return authoritative


def verify_gdi(path, expected):
    if path.suffix.lower() != '.gdi':
        raise ValueError('Select the supported PAL v1.003 .gdi, not CDI/CHD/ISO/archive')
    if path.stat().st_size > 65536:
        raise ValueError('GDI descriptor exceeds 64 KiB')
    lines = path.read_text(encoding='utf-8-sig').splitlines()
    lines = [line.strip() for line in lines if line.strip()]
    if not lines or lines[0] != str(len(expected)) or len(lines) != len(expected) + 1:
        raise ValueError('Expected exactly three GDI track records')
    root = path.parent.resolve()
    records = []
    for line, track in zip(lines[1:], expected):
        fields = shlex.split(line)
        if len(fields) != 6:
            raise ValueError('GDI track requires number, LBA, type, sector size, filename, offset')
        number, lba, kind, sector = map(int, fields[:4])
        offset = int(fields[5])
        layout = (number, lba, kind, sector, offset)
        wanted = (track['n'], track['lba'], track['type'], track['sector_size'], 0)
        if layout != wanted:
            raise ValueError(f'Unsupported layout for track {track["n"]}: {layout}')
        name = Path(fields[4])
        if name.is_absolute() or '..' in name.parts or '\\' in fields[4] or ':' in fields[4]:
            raise ValueError('GDI track filename must stay within the selected directory')
        source = (root / name).resolve()
        if not source.is_relative_to(root) or not source.is_file():
            raise ValueError(f'Track missing or outside selected directory: {fields[4]}')
        if source.stat().st_size != track['bytes']:
            raise ValueError(f'Track {number} size mismatch; expected {track["bytes"]} bytes')
        sha = digest(source)
        if sha != track['sha256']:
            raise ValueError(f'Track {number} SHA-256 mismatch; expected {track["sha256"]}')
        records.append({'number': number, 'filename': fields[4],
                        'bytes': track['bytes'], 'sha256': sha})
    return {'descriptor_sha256': digest(path), 'tracks': records}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('gdi', type=Path, help='owner-supplied GDI and adjacent tracks')
    args = parser.parse_args()
    try:
        report = verify_gdi(args.gdi, expected_tracks())
    except (OSError, UnicodeError, ValueError, subprocess.CalledProcessError) as error:
        print(f'GAME_INPUT_BLOCKED: {error}')
        return 2
    print('GAME_TRACK_IDENTITIES_VERIFIED: Sonic Adventure PAL v1.003 (1999)')
    print(json.dumps(report, indent=2))
    print('Extraction/runtime acceptance: not tested by this verifier')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
