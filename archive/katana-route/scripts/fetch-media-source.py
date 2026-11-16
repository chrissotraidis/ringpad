#!/usr/bin/env python3
"""Acquire the pinned public FFmpeg source only, without changing references."""
import hashlib
import json
from pathlib import Path
import shutil
import tarfile
import tempfile
import urllib.request

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    pin = json.loads((ROOT / 'config/media-source.json').read_text())
    cache = ROOT / '.local/media-source'
    cache.mkdir(parents=True, exist_ok=True)
    archive = cache / f"ffmpeg-{pin['revision']}.tar.gz"
    if not archive.exists():
        # A failed transfer never replaces a verified input.
        with tempfile.TemporaryDirectory(prefix='download-', dir=cache) as stage:
            temporary = Path(stage) / 'source.tar.gz'
            with urllib.request.urlopen(pin['url'], timeout=60) as source:
                with temporary.open('wb') as output:
                    shutil.copyfileobj(source, output)
            if digest(temporary) != pin['archive_sha256']:
                raise SystemExit('FFmpeg source SHA-256 mismatch')
            temporary.rename(archive)
    if digest(archive) != pin['archive_sha256']:
        raise SystemExit('FFmpeg source SHA-256 mismatch; preserving existing input')
    source_dir = cache / f"FFmpeg-{pin['revision']}"
    stamp = source_dir / '.ringpad-source-archive-sha256'
    if source_dir.exists():
        if not stamp.is_file() or stamp.read_text().strip() != pin['archive_sha256']:
            raise SystemExit('Refusing to replace an unrecognized source directory')
    else:
        with tempfile.TemporaryDirectory(prefix='extract-', dir=cache) as stage:
            with tarfile.open(archive) as source:
                expected_root = source_dir.name
                if any(Path(m.name).parts[0] != expected_root for m in source.getmembers()):
                    raise SystemExit('Unexpected FFmpeg archive root')
                source.extractall(stage, filter='data')
            extracted = Path(stage) / expected_root
            (extracted / stamp.name).write_text(pin['archive_sha256'] + '\n')
            extracted.rename(source_dir)
    print(f"FFMPEG_SOURCE_READY revision={pin['revision']} sha256={digest(archive)}")
    print(f'Source: {source_dir}')
    print('Apple build and decoder tests: not run')


if __name__ == '__main__':
    main()
