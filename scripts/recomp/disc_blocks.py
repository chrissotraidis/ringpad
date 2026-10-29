#!/usr/bin/env python3
"""Publishable block list for SonicPad's recompiled code.

The trace manifest (generated/recomp-manifest.txt) holds the game's own SH-4
instructions, so it stays private. Every traced block is also a byte range of
a file on the disc: 1ST_READ.BIN, or a PRS-compressed stage, event or movie
module. This tool stores each block as an address, a source file and an
offset, with no game bytes, and rebuilds the manifest from a player's own disc.

  export: private manifest + disc -> config/recomp-blocks.txt (committed)
  import: config/recomp-blocks.txt + disc -> generated/recomp-manifest.txt

The import checks a SHA-256 of every source file it reads and of the rebuilt
manifest, whose id is the one generate_blocks.py reports. Hashes are one-way,
so the list cannot be turned back into game code.
"""
import argparse
import hashlib
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BLOCKS = ROOT / 'config/recomp-blocks.txt'
MANIFEST = ROOT / 'generated/recomp-manifest.txt'
HEADER = '# sonicpad-trace v1: pc nops cycles ops...'
REIOS = 'reios'   # Flycast's high-level BIOS writes its trap opcode here, not the disc
BOOT = 'IP.BIN'   # the boot sector: the first 16 sectors of the high-density area


def reios_opcode():
    header = (ROOT / 'ref/flycast/core/reios/reios.h').read_text()
    return int(re.search(r'#define\s+REIOS_OPCODE\s+(0x[0-9A-Fa-f]+)', header).group(1), 16)


class Disc:
    """Read-only ISO 9660 access to a GDI's tracks."""

    def __init__(self, gdi):
        gdi = Path(gdi)
        lines = gdi.read_text().split('\n')
        self.tracks = []
        for line in lines[1:int(lines[0])+1]:
            fields = line.split()
            number, lba, kind, size = (int(x) for x in fields[:4])
            name = ' '.join(fields[4:-1]).strip('"')
            self.tracks.append((lba, kind, size, gdi.parent / name))
        self.files = {}
        data = [t for t in self.tracks if t[1] == 4]
        session = max(data, key=lambda t: t[0])
        self.boot = self.read(session[0], 16 * 2048)
        pvd = self.read(session[0] + 16, 2048)
        if pvd[1:6] != b'CD001':
            raise SystemExit('no ISO 9660 volume in the high-density area')
        root = pvd[156:190]
        self.walk(struct.unpack('<I', root[2:6])[0], struct.unpack('<I', root[10:14])[0], '')

    def read(self, lba, size):
        track = max((t for t in self.tracks if t[0] <= lba), key=lambda t: t[0])
        start, _, sector, path = track
        skip = 16 if sector == 2352 else 0
        out = bytearray()
        with open(path, 'rb') as f:
            for i in range((size + 2047) // 2048):
                f.seek((lba - start + i) * sector + skip)
                out += f.read(2048)
        return bytes(out[:size])

    def walk(self, lba, size, prefix):
        data, i = self.read(lba, size), 0
        while i < len(data):
            length = data[i]
            if length == 0:
                i = (i // 2048 + 1) * 2048
                continue
            rec = data[i:i + length]
            extent, bytes_ = struct.unpack('<I', rec[2:6])[0], struct.unpack('<I', rec[10:14])[0]
            name = rec[33:33 + rec[32]]
            if name not in (b'\x00', b'\x01'):
                name = name.decode('latin1').split(';')[0]
                if rec[25] & 2:
                    self.walk(extent, bytes_, prefix + name + '/')
                else:
                    self.files[prefix + name] = (extent, bytes_)
            i += length

    def file(self, name):
        if name == BOOT:
            return self.boot
        data = self.read(*self.files[name])
        return prs(data) if name.upper().endswith('.PRS') else data


def prs(src):
    """Sega's PRS (LZ77) decompression."""
    out, i, bits, left = bytearray(), 0, 0, 0

    def bit():
        nonlocal i, bits, left
        if left == 0:
            bits, left, i = src[i], 8, i + 1
        b, bits, left = bits & 1, bits >> 1, left - 1
        return b

    while True:
        if bit():
            out.append(src[i])
            i += 1
            continue
        if bit():
            value = src[i] | src[i + 1] << 8
            i += 2
            if value == 0:
                return bytes(out)
            offset, count = (value >> 3) - 0x2000, value & 7
            if count == 0:
                count, i = src[i] + 1, i + 1
            else:
                count += 2
        else:
            count = (bit() << 1 | bit()) + 2
            offset, i = src[i] - 0x100, i + 1
        for _ in range(count):
            out.append(out[offset])


def physical(pc):
    return (pc & 0x1FFFFFFF) | 0x0C000000


def read_manifest(path):
    blocks = []
    for line in Path(path).read_text().splitlines():
        if line and not line.startswith('#'):
            f = line.split()
            blocks.append((int(f[0], 16), int(f[2]), bytes(struct.pack('<%dH' % (len(f) - 3), *(int(x, 16) for x in f[3:])))))
    return blocks


def manifest_text(blocks):
    lines = [HEADER] + ['%08X %d %d %s' % (pc, len(code) // 2, cycles,
             ' '.join('%04X' % op for op in struct.unpack('<%dH' % (len(code) // 2), code)))
             for pc, cycles, code in blocks]
    return '\n'.join(lines) + '\n'


def digest(data):
    return hashlib.sha256(data).hexdigest()[:16]


def export(args):
    disc = Disc(args.gdi)
    blocks = read_manifest(args.manifest)
    reios = struct.pack('<H', reios_opcode())
    cache, pairs = {}, [('1ST_READ.BIN', 0x0C010000), (BOOT, 0x0C008000)]

    def data(name):
        if name not in cache:
            cache[name] = disc.file(name)
        return cache[name]

    def matches(name, base, pc, code):
        offset = physical(pc) - base
        d = data(name)
        return 0 <= offset <= len(d) - len(code) and d[offset:offset + len(code)] == code

    skip = ('.SFD', '.ADX', '.AFS', '.GIF', '.JPG', '.PVR', '.MLT')
    searchable = sorted(n for n in disc.files if not n.upper().endswith(skip))
    rows = []
    for pc, cycles, code in blocks:
        if code == reios * (len(code) // 2):
            rows.append((pc, len(code) // 2, cycles, REIOS, 0, code))
            continue
        hit = next(((n, b) for n, b in pairs if matches(n, b, pc, code)), None)
        if hit is None:
            for name in searchable:
                at = data(name).find(code)
                if at >= 0:
                    hit = (name, physical(pc) - at)
                    pairs.append(hit)
                    break
        if hit is None:
            raise SystemExit('block %08X is not on the disc' % pc)
        rows.append((pc, len(code) // 2, cycles, hit[0], physical(pc) - hit[1], code))

    sources = sorted({r[3] for r in rows})
    index = {s: i for i, s in enumerate(sources)}
    # Blocks found statically from the disc, seeded by these traced ones.
    from discover_blocks import static_manifest
    _, static_id, found, _ = static_manifest(disc, args.manifest)
    out = ['# sonicpad-blocks v2: pc nops cycles @source offset; sources are @index name sha256',
           '# No game data: each block is a byte range of a file on the player\'s disc,',
           '# rebuilt by scripts/recomp/disc_blocks.py import.',
           '# manifest %s' % digest(manifest_text(blocks).encode()),
           '# static %s (scripts/recomp/discover_blocks.py run, seeded by this manifest)' % static_id]
    out += ['@%d %s %s' % (i, s, '-' if s == REIOS else digest(data(s))) for i, s in enumerate(sources)]
    out += ['%08X %d %d @%d %X' % (pc, n, c, index[s], o) for pc, n, c, s, o, code in rows]
    args.blocks.write_text('\n'.join(out) + '\n')
    print('blocks=%d sources=%d from_disc=%d reios=%d static=%s (%d blocks) -> %s' % (
        len(rows), len(sources), sum(r[3] != REIOS for r in rows), sum(r[3] == REIOS for r in rows),
        static_id, len(found), args.blocks))


def import_(args):
    disc = Disc(args.gdi)
    sources, hashes, rows, expected = {}, {}, [], None
    for line in args.blocks.read_text().splitlines():
        if line.startswith('# manifest '):
            expected = line.split()[2]
        elif line.startswith('@'):
            i, name, sha = line[1:].rsplit(' ', 2)
            sources[int(i)] = name
            hashes[name] = sha
        elif line and not line.startswith('#'):
            pc, n, c, s, o = line.split()
            rows.append((int(pc, 16), int(n), int(c), sources[int(s.lstrip('@'))], int(o, 16)))
    reios = struct.pack('<H', reios_opcode())
    cache, blocks = {}, []
    for pc, n, cycles, source, offset in rows:
        if source == REIOS:
            code = reios * n
        else:
            if source not in cache:
                if source != BOOT and source not in disc.files:
                    raise SystemExit('%s is not on this disc' % source)
                cache[source] = disc.file(source)
                if digest(cache[source]) != hashes[source]:
                    raise SystemExit('%s on this disc differs from the supported release' % source)
            code = cache[source][offset:offset + 2 * n]
        blocks.append((pc, cycles, code))
    text = manifest_text(blocks)
    got = digest(text.encode())
    if got != expected:
        raise SystemExit('rebuilt manifest %s, expected %s' % (got, expected))
    args.manifest.parent.mkdir(parents=True, exist_ok=True)
    args.manifest.write_text(text)
    print('manifest=%s blocks=%d files_read=%d -> %s' % (got, len(blocks), len(cache), args.manifest))


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='command', required=True)
    for name, fn in (('export', export), ('import', import_)):
        p = sub.add_parser(name)
        p.add_argument('gdi', type=Path)
        p.add_argument('--blocks', type=Path, default=BLOCKS)
        p.add_argument('--manifest', type=Path, default=MANIFEST)
        p.set_defaults(fn=fn)
    args = parser.parse_args()
    args.fn(args)


if __name__ == '__main__':
    sys.exit(main())

