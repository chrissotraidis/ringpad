#!/usr/bin/env python3
"""Build a personal SonicPad app from the player's own Sonic Adventure disc.

  scripts/build-personal.py --gdi <disc>.gdi --output SonicPad.ipa [--jobs N]

Needs an Apple Silicon Mac with full Xcode and CMake 3.24+. The disc must be
the PAL v1.003 GDI (MK-5100050) with its three tracks next to it. Steps:

1. Check the disc's tracks against config/game-identity.json.
2. Fetch the pinned Flycast and MoltenVK into ref/ if they are not there.
3. Rebuild the recompiled-code manifest from the disc (config/recomp-blocks.txt),
   find the rest of the game's code in the disc's modules
   (scripts/recomp/discover_blocks.py) and generate C++ for both. All of it
   stays in build/personal/ on this Mac.
4. Build the iOS app without signing and package an unsigned IPA.

The IPA contains code translated from the player's disc: it is for the
player's own devices and must not be shared.
"""
import argparse
import hashlib
import json
import os
import platform
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WORK = ROOT / 'build/personal'
FLYCAST_URL = 'https://github.com/flyinghead/flycast.git'
MOLTENVK_URL = 'https://github.com/KhronosGroup/MoltenVK/releases/download/v1.4.2/MoltenVK-all.tar'


def run(*command, cwd=ROOT):
    print('+ ' + ' '.join(str(part) for part in command), flush=True)
    subprocess.run([str(part) for part in command], cwd=cwd, check=True)


def output(*command):
    return subprocess.check_output([str(part) for part in command], cwd=ROOT, text=True).strip()


def sha256(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b''):
            digest.update(chunk)
    return digest.hexdigest()


def flycast(lock):
    pin = lock['commit']
    path = ROOT / lock['path']
    if not (path / '.git').exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        run('git', 'clone', '--filter=blob:none', '--no-checkout', FLYCAST_URL, path)
        run('git', '-C', path, 'checkout', '--quiet', pin)
    head = output('git', '-C', path, 'rev-parse', 'HEAD')
    if head != pin:
        raise SystemExit(f'{path} is at {head}, not the pinned Flycast {pin}. Move it aside and run again.')
    if output('git', '-C', path, 'status', '--porcelain', '--untracked-files=no'):
        raise SystemExit(f'{path} has local changes; SonicPad builds against an unmodified Flycast.')
    # Partial clones keep Flycast's dependency history to about 255 MB instead of 1.4 GB.
    run('git', '-C', path, 'submodule', 'update', '--init', '--recursive', '--filter=blob:none')
    run('git', '-C', path, 'remote', 'set-url', '--push', 'origin', 'DISABLED')


def moltenvk(lock):
    framework = ROOT / lock['path']
    if framework.exists():
        return
    folder = ROOT / 'ref/moltenvk-all'
    folder.mkdir(parents=True, exist_ok=True)
    archive = folder / lock['asset']
    if not archive.exists() or sha256(archive) != lock['sha256']:
        run('curl', '--fail', '--location', '--silent', '--show-error', '--output', archive, MOLTENVK_URL)
    if sha256(archive) != lock['sha256']:
        raise SystemExit(f'{archive.name} does not match the pinned SHA-256 {lock["sha256"]}')
    run('tar', 'xf', archive.name, cwd=folder)
    if not framework.exists():
        raise SystemExit(f'MoltenVK xcframework missing after unpacking: {framework}')


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--gdi', required=True, type=Path, help='the .gdi with its three tracks next to it')
    parser.add_argument('--output', required=True, type=Path, help='new unsigned IPA path')
    parser.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    args = parser.parse_args()
    if platform.system() != 'Darwin' or platform.machine() != 'arm64':
        parser.error('An Apple Silicon Mac with full Xcode is required.')
    result = args.output.expanduser().resolve()
    if result.exists():
        parser.error(f'Keeping the existing file: {result}')
    gdi = args.gdi.expanduser().resolve(strict=True)

    # The disc and the toolchain first, before anything is downloaded.
    run(sys.executable, ROOT / 'scripts/verify-game.py', gdi)
    run('xcodebuild', '-version')
    run('xcrun', '--sdk', 'iphoneos', '--show-sdk-version')
    run('cmake', '--version')

    lock = json.loads((ROOT / 'dependencies.lock.json').read_text())['references']
    flycast(lock['Flycast'])
    moltenvk(lock['MoltenVK'])

    manifest, static, recomp = WORK / 'recomp-manifest.txt', WORK / 'static-manifest.txt', WORK / 'recomp'
    WORK.mkdir(parents=True, exist_ok=True)
    header = (ROOT / 'config/recomp-blocks.txt').read_text().split('\n', 6)
    expected = next(line.split()[2] for line in header if line.startswith('# manifest '))
    expected_static = next(line.split()[2] for line in header if line.startswith('# static '))
    run(sys.executable, ROOT / 'scripts/recomp/disc_blocks.py', 'import', gdi, '--manifest', manifest)
    run(sys.executable, ROOT / 'scripts/recomp/discover_blocks.py', 'run', gdi, '--traced', manifest,
        '--out', static, '--expect', expected_static)
    run(sys.executable, ROOT / 'scripts/recomp/generate_blocks.py', manifest, '--manifest', manifest,
        '--out', recomp, '--static', static)

    build = WORK / 'ios'
    run('cmake', '-S', ROOT, '-B', build, '-G', 'Xcode', '-DCMAKE_SYSTEM_NAME=iOS', '-DCMAKE_OSX_SYSROOT=iphoneos',
        '-DCMAKE_BUILD_TYPE=Release', f'-DSONICPAD_RECOMP_DIR={recomp}', '-Wno-deprecated')
    run('xcodebuild', 'build', '-project', build / 'sonicpad.xcodeproj', '-target', 'flycast',
        '-configuration', 'Release', '-sdk', 'iphoneos', '-jobs', args.jobs, '-quiet',
        'CODE_SIGNING_ALLOWED=NO', 'CODE_SIGNING_REQUIRED=NO')
    app = build / 'flycast/Release-iphoneos/SonicPad.app'
    expected = f'{expected}+{expected_static}'
    if expected.encode() not in (app / 'SonicPad').read_bytes():
        raise SystemExit(f'{app.name} does not contain the recompiled code for manifest {expected}')

    result.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='sonicpad-', dir=result.parent) as temporary:
        staging = Path(temporary)
        shutil.copytree(app, staging / 'Payload/SonicPad.app', symlinks=True)
        archive = staging / 'personal.ipa'
        run('ditto', '-c', '-k', '--norsrc', '--keepParent', staging / 'Payload', archive)
        os.link(archive, result)   # never replaces a file that appeared meanwhile
    print(f'Personal unsigned IPA (manifest {expected}): {result}')


if __name__ == '__main__':
    try:
        main()
    except (OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))

