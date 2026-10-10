#!/usr/bin/env python3
"""Audit retained source input identities. Does not assert G1 or buildability.

Reads source/metadata only; never downloads, extracts, compiles, or restores assets.
Exit 2: missing/mismatched inputs. Exit 0: audited input hashes match, but the
transitive build graph, auxiliary generation route and disc still need review.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import subprocess
import tarfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
BASE_PREFIX = '.local/baseline/r354/'
GROUPS = {
    'guest_source_metadata': 'product/generated/',
    'sdk_headers': 'sdk/include/',
    'sdk_contracts': 'sdk/generated/include/',
    'title_providers': 'title-source/',
}
# Old Windows build outputs/commands are not portable source prerequisites.
EXCLUDED = {'product/generated/build.ninja', 'product/generated/compile_commands.json'}


def sha(stream):
    return hashlib.file_digest(stream, 'sha256').hexdigest()


def safe_relative(name):
    path = PurePosixPath(name)
    if path.is_absolute() or '..' in path.parts or '\\' in name or ':' in name:
        raise ValueError(f'Unsafe source path: {name}')
    return path


def check_file(root, relative, size, expected):
    relative = safe_relative(relative)
    path = root.joinpath(*relative.parts)
    record = {'path': str(relative), 'expected_bytes': size, 'expected_sha256': expected}
    if not path.exists():
        return dict(record, status='missing')
    if not path.resolve().is_relative_to(root.resolve()) or not path.is_file():
        return dict(record, status='incompatible', reason='Not a file within the source root')
    record['bytes'] = path.stat().st_size
    if record['bytes'] != size:
        return dict(record, status='incompatible', reason='Size mismatch')
    with path.open('rb') as stream:
        record['sha256'] = sha(stream)
    return dict(record, status='present-verified' if record['sha256'] == expected else 'incompatible')


def summary(records):
    return {'expected_files': len(records),
            'verified': sum(r['status'] == 'present-verified' for r in records),
            'missing': sum(r['status'] == 'missing' for r in records),
            'incompatible': sum(r['status'] == 'incompatible' for r in records),
            'files': records}


def validate_lock(pins, lock, media):
    for reference in pins['references']:
        if reference['pin'] is not None:
            frozen = lock['references'].get(reference['name'])
            if not frozen or frozen['commit'] != reference['pin']:
                raise ValueError(f'Dependency lock disagrees with {reference["name"]} pin')
    if (media['revision'], media['archive_sha256']) != (
            lock['media_source']['commit'], lock['media_source']['archive_sha256']):
        raise ValueError('Dependency lock disagrees with FFmpeg source identity')


def runtime_archive(source_root, runtime_root, record):
    name = record['path'][len(BASE_PREFIX):]
    archive = check_file(source_root, name, record['bytes'], record['sha256'])
    if archive['status'] != 'present-verified':
        return summary([archive])
    records = [archive]
    with zipfile.ZipFile(source_root / name) as source:
        seen = set()
        for member in source.infolist():
            relative = safe_relative(member.filename)
            if str(relative) in seen:
                raise ValueError('Duplicate runtime source archive path')
            seen.add(str(relative))
            if member.is_dir():
                continue
            with source.open(member) as stream:
                expected = sha(stream)
            records.append(check_file(runtime_root, str(relative), member.file_size, expected))
    return summary(records)


def media_source(pin):
    cache = ROOT / '.local/media-source'
    name = f'ffmpeg-{pin["revision"]}.tar.gz'
    archive_path = cache / name
    if not archive_path.is_file():
        return summary([{'path': name, 'status': 'missing'}])
    archive = check_file(cache, name, archive_path.stat().st_size, pin['archive_sha256'])
    records = [archive]
    if archive['status'] == 'present-verified':
        # Check the materialized source, not merely a cache stamp.
        with tarfile.open(archive_path) as source:
            for member in source:
                safe_relative(member.name)
                if member.isfile():
                    with source.extractfile(member) as stream:
                        expected = sha(stream)
                    records.append(check_file(cache, member.name, member.size, expected))
    return summary(records)


def audit(source_root, runtime_root):
    pins = json.loads((ROOT / 'config/pins.json').read_text())
    lock = json.loads((ROOT / 'dependencies.lock.json').read_text())
    media = json.loads((ROOT / 'config/media-source.json').read_text())
    validate_lock(pins, lock, media)
    for reference in pins['references']:
        if reference['pin'] is not None:
            head = subprocess.check_output(['git', '-C', str(ROOT / reference['ref_path']),
                                            'rev-parse', 'HEAD'], text=True).strip()
            if head != reference['pin']:
                raise ValueError(f'{reference["name"]} checkout differs from its pin')
    pin = next(p['pin'] for p in pins['references'] if p['name'] == 'SARecomp')
    manifest = json.loads(subprocess.check_output(['git', '-C', str(ROOT / 'ref/sarecomp'),
                            'show', f'{pin}:baseline/r354.json']))
    groups = {}
    for label, prefix in GROUPS.items():
        files = [row for row in manifest['files']
                 if row['path'].startswith(BASE_PREFIX + prefix)
                 and row['path'][len(BASE_PREFIX):] not in EXCLUDED]
        if not files:
            raise ValueError(f'Pinned source manifest has no entries for {label}')
        groups[label] = summary([check_file(source_root, row['path'][len(BASE_PREFIX):],
                                            row['bytes'], row['sha256']) for row in files])
    runtime = next(row for row in manifest['files']
                   if row['path'] == BASE_PREFIX + 'katana-source-178448be.zip')
    frozen = lock['katana_source_input']
    if (runtime['bytes'], runtime['sha256']) != (frozen['bytes'], frozen['archive_sha256']):
        raise ValueError('Katana archive lock differs from the pinned baseline identity')
    groups['katana_runtime_source'] = runtime_archive(source_root, runtime_root, runtime)
    groups['ffmpeg_source'] = media_source(media)
    return {'scope': 'retained-source-input-identity-audit', 'g1_complete': False,
            'dependency_lock_sha256': hashlib.sha256((ROOT / 'dependencies.lock.json').read_bytes()).hexdigest(),
            'sarecomp_pin': pin, 'source_root': str(source_root.resolve()),
            'runtime_root': str(runtime_root.resolve()), 'groups': groups,
            'excluded_windows_build_metadata': sorted(EXCLUDED),
            'remaining_review': ['transitive build/include graph and matching ABI/providers',
                                 'auxiliary Mini Cart generation/retained-source route',
                                 'source provenance/permission declaration',
                                 'supported disc and extraction pipeline']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, default=ROOT / '.local/source-only/r354')
    parser.add_argument('--runtime-root', type=Path, help='materialized runtime matching source ZIP paths')
    parser.add_argument('--report', type=Path, help='save detailed JSON inventory')
    args = parser.parse_args()
    try:
        report = audit(args.source_root, args.runtime_root or args.source_root / 'katana')
        if args.report:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(report, indent=2) + '\n')
    except (OSError, ValueError, subprocess.CalledProcessError, zipfile.BadZipFile,
            tarfile.TarError) as error:
        print(f'SOURCE_INPUT_AUDIT_BLOCKED: {error}')
        return 2
    missing = False
    for label, group in report['groups'].items():
        print(f'{label}: verified={group["verified"]}/{group["expected_files"]} '
              f'missing={group["missing"]} incompatible={group["incompatible"]}')
        missing |= group['missing'] > 0 or group['incompatible'] > 0
    print('G1 remains unproven; input hashes do not establish the complete build/generation/disc route.')
    return 2 if missing else 0


if __name__ == '__main__':
    raise SystemExit(main())
