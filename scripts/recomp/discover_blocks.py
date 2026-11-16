#!/usr/bin/env python3
"""Find RingPad code blocks statically in the game's disc files.

Traces only cover code that ran on a recorded route. The rest of the game's
code lives in the same disc files: 1ST_READ.BIN, the movie player, and the
PRS-compressed stage, field, boss, mini-game and event modules. This tool
walks that code from known entry points (traced blocks, call targets loaded
from constant pools, and code pointers in data that land on a function
prologue), following branches and fall-throughs, and decodes each block
exactly as the executor's decodeBlock does. The result is a manifest in the
trace format that generate_blocks.py compiles alongside the traced one.

Blocks found here are only ever run when the game jumps to their address and
guest memory holds exactly their bytes, so a block decoded from data costs
code size but cannot change behavior.
"""
import argparse
import hashlib
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from disc_blocks import Disc, read_manifest              # noqa: E402
from generate_blocks import opcode_table, sext            # noqa: E402

MAX_OPS = 64
HEADER = '# ringpad-trace v1: pc nops cycles ops...'


class Module:
    def __init__(self, name, base, data, resident=False):
        self.name, self.base, self.data, self.resident = name, base, data, resident
        self.end = base + len(data)

    def contains(self, pc):
        return self.base <= pc < self.end - 1

    def half(self, pc):
        o = pc - self.base
        return self.data[o] | self.data[o + 1] << 8

    def word(self, addr):
        o = addr - self.base
        if 0 <= o <= len(self.data) - 4:
            return struct.unpack_from('<I', self.data, o)[0]
        return None


class Decoder:
    def __init__(self):
        self.table, types, _ = opcode_table()
        self.writes_pc, self.delay = types['WritesPC'], types['Delayslot']
        self.cycles = None

    def kind(self, op):
        entry = self.table[op]
        return None if entry is None else entry[1]

    def block(self, mod, pc):
        """Mirror of decodeBlock: ops, or None if the executor would interpret."""
        ops = []
        for i in range(MAX_OPS - 1):
            addr = pc + i * 2
            if not mod.contains(addr):
                return None
            op = mod.half(addr)
            kind = self.kind(op)
            if kind is None:          # Flycast's "missing" opcode: leave it to the interpreter
                return None
            ops.append(op)
            if kind & self.writes_pc:
                if kind & self.delay:
                    if not mod.contains(addr + 2):
                        return None
                    ds = mod.half(addr + 2)
                    dk = self.kind(ds)
                    if dk is None or dk & self.writes_pc:
                        return ops[:-1] or None
                    ops.append(ds)
                return ops
        return ops

    def successors(self, mod, pc, ops):
        """Addresses the game can continue at after this block, plus call targets."""
        out, calls, known = [], [], {}
        end = pc + len(ops) * 2
        for i, op in enumerate(ops):
            kind = self.kind(op)
            if kind & self.writes_pc:
                break
            if op & 0xF000 == 0xD000:              # mov.l @(disp,PC),Rn
                known[(op >> 8) & 0xF] = mod.word(((pc + i * 2 + 4) & ~3) + (op & 0xFF) * 4)
            else:
                known.pop((op >> 8) & 0xF, None)
                known.pop((op >> 4) & 0xF, None)
        else:
            return [end], calls                    # 63 ops without a branch: the next block follows
        addr, hi, n = pc + i * 2, op & 0xF000, (op >> 8) & 0xF
        if hi in (0xA000, 0xB000):                 # bra, bsr
            out.append(addr + 4 + sext(op & 0xFFF, 12) * 2)
            if hi == 0xB000:
                out.append(addr + 4)
        elif op & 0xFF00 in (0x8900, 0x8B00):      # bt, bf
            out += [addr + 4 + sext(op & 0xFF, 8) * 2, addr + 2]
        elif op & 0xFF00 in (0x8D00, 0x8F00):      # bt/s, bf/s
            out += [addr + 4 + sext(op & 0xFF, 8) * 2, addr + 4]
        elif op & 0xF0FF in (0x400B, 0x402B):      # jsr, jmp through a constant-pool address
            if known.get(n) is not None:
                calls.append(known[n])
            if op & 0xF0FF == 0x400B:
                out.append(addr + 4)
        elif op & 0xF0FF == 0x0003:                # bsrf returns after its slot
            out.append(addr + 4)
        elif op in (0x000B, 0x002B) or op & 0xF0FF == 0x0023:   # rts, rte, braf
            pass
        elif not (kind & self.delay):              # trapa, ldc SR, sleep resume after
            out.append(addr + 2)
        return out, calls


PROLOGUE = lambda op: ((op & 0xFF0F) == 0x2F06 or op == 0x4F22 or (op & 0xFF00) == 0x7F00
                       or (op & 0xF0FF) == 0xF0FB or op in (0x4F12, 0x4F02, 0x4F52, 0x4F62))


RESIDENT = (('1ST_READ.BIN', 0x8C010000), ('SONICAD/MOVIE.BIN', 0x8CEB0000))
MODULE_LIST = Path(__file__).resolve().parents[2] / 'config/code-modules.txt'
SHARED_BASES = (0x0C900000, 0x8C900000, 0x0CB80000, 0x8CB80000)


def discover(modules, seeds, dec):
    """Walk code from seeds [(pc, module or None)] and from code pointers in
    every module. Returns {(pc, ops)}."""
    resident = [m for m in modules if m.resident]

    def owner(pc, hint):
        if hint is not None and hint.contains(pc):
            return hint
        return next((m for m in resident if m.contains(pc)), None)

    work = list(seeds)
    for m in modules:
        for w in struct.unpack_from('<%dI' % (len(m.data) // 4), m.data):
            if w & 1:
                continue
            target = owner(w, m)
            if target is not None and PROLOGUE(target.half(w)):
                work.append((w, target))
    seen, blocks = set(), set()
    while work:
        pc, hint = work.pop()
        mod = owner(pc, hint)
        if mod is None or pc & 1 or (mod.name, pc) in seen:
            continue
        seen.add((mod.name, pc))
        ops = dec.block(mod, pc)
        if ops is None:
            continue
        blocks.add((pc, tuple(ops)))
        nxt, calls = dec.successors(mod, pc, ops)
        work += [(t & 0xFFFFFFFF, mod) for t in nxt]
        work += [(t, mod) for t in calls if t is not None]
    return blocks


def load_modules(disc, listed=True):
    mods = [Module(name, base, disc.file(name), resident=True) for name, base in RESIDENT]
    if listed:
        for line in MODULE_LIST.read_text().splitlines():
            if line and not line.startswith('#'):
                name, base, sha = line.split()
                data = disc.file(name)
                if hashlib.sha256(data).hexdigest()[:16] != sha:
                    raise SystemExit(f'{name} on this disc differs from the supported release')
                mods.append(Module(name, int(base, 16), data))
    return mods


def survey(disc, dec):
    """Code modules: PRS files whose own pointers land on function prologues
    at one of the shared load addresses."""
    out = []
    for name in sorted(disc.files):
        if not name.upper().endswith('.PRS'):
            continue
        data = disc.file(name)
        words = set(struct.unpack_from('<%dI' % (len(data) // 4), data))
        best = (0, 0)
        for base in SHARED_BASES:
            m = Module(name, base, data)
            hits = sum(1 for w in words if w & 1 == 0 and m.contains(w) and PROLOGUE(m.half(w)))
            best = max(best, (hits, base))
        if best[0] >= 4:
            out.append((name, best[1], hashlib.sha256(data).hexdigest()[:16]))
    return out


def seeds_from(manifest, modules):
    """Traced blocks seed every module holding the same bytes at that address."""
    seeds = []
    for pc, _, code in read_manifest(manifest):
        for m in modules:
            if m.contains(pc) and m.contains(pc + len(code) - 2) and \
                    m.data[pc - m.base:pc - m.base + len(code)] == code:
                seeds.append((pc, m))
    return seeds


def static_manifest(disc, traced=None):
    """The static block manifest for this disc, as text, and its id."""
    modules = load_modules(disc)
    seeds = seeds_from(traced, modules) if traced else []
    blocks = discover(modules, seeds, Decoder())
    lines = [HEADER] + ['%08X %d 0 %s' % (pc, len(ops), ' '.join('%04X' % op for op in ops))
                        for pc, ops in sorted(blocks)]
    text = '\n'.join(lines) + '\n'
    return text, hashlib.sha256(text.encode()).hexdigest()[:16], blocks, len(modules)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='command', required=True)
    p = sub.add_parser('modules', help='find the code modules on the disc (slow) and write config/code-modules.txt')
    p.add_argument('gdi', type=Path)
    p = sub.add_parser('run', help='write the static block manifest')
    p.add_argument('gdi', type=Path)
    p.add_argument('--traced', type=Path, help='trace manifest whose blocks seed the walk')
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--expect', help='stop unless the result has this id')
    args = parser.parse_args()
    disc = Disc(args.gdi)
    if args.command == 'modules':
        rows = survey(disc, Decoder())
        MODULE_LIST.write_text('# Code modules: disc file, load address, SHA-256 of the decompressed file.\n'
                               '# Written by scripts/recomp/discover_blocks.py modules.\n'
                               + ''.join(f'{n} {b:08X} {s}\n' for n, b, s in rows))
        print(f'modules={len(rows)} -> {MODULE_LIST}')
        return
    text, static_id, blocks, modules = static_manifest(disc, args.traced)
    if args.expect and static_id != args.expect:
        raise SystemExit(f'static blocks {static_id}, expected {args.expect}')
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text)
    print('static=%s blocks=%d ops=%d modules=%d -> %s' % (
        static_id, len(blocks), sum(len(o) for _, o in blocks), modules, args.out))


if __name__ == '__main__':
    main()

