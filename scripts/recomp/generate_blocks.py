#!/usr/bin/env python3
"""Generate SonicPad ahead-of-time C++ for traced SH-4 blocks.

Input: one or more trace files written by the SonicPad executor
(SONICPAD_TRACE=<file>), each line "PC NOPS CYCLES OP...". Output: C++ sources
in generated/recomp/ that the root CMake build compiles into the app.

Each instruction calls the same Flycast handler the interpreter would use;
delay-slot branches are emitted inline so the delay slot runs without a
memory refetch. The runtime checks every referenced handler against
Flycast's live opcode table before enabling the generated code.
"""
import argparse
import hashlib
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
FLYCAST = ROOT / 'ref/flycast/core/hw/sh4'
BLOCKS_PER_FILE = 1500


def evaluate(expr, names):
    tokens = [t.strip() for t in expr.split('|')]
    value = 0
    for token in tokens:
        value |= int(token, 0) if token[0].isdigit() else names[token]
    return value


def reios_opcode():
    header = (FLYCAST.parents[1] / 'reios/reios.h').read_text()
    return int(re.search(r'#define\s+REIOS_OPCODE\s+(0x[0-9A-Fa-f]+)', header).group(1), 16)


def opcode_table():
    header = (FLYCAST / 'sh4_opcode_list.h').read_text()
    body = re.search(r'enum OpcodeType\s*\{(.*?)\};', header, re.S).group(1)
    types = {}
    for line in body.splitlines():
        line = line.split('//')[0].strip().rstrip(',')
        if '=' in line:
            name, expr = (part.strip() for part in line.split('=', 1))
            types[name] = evaluate(expr, types)
    source = (FLYCAST / 'sh4_opcode_list.cpp').read_text()
    masks = {m.group(1): int(m.group(2), 16)
             for m in re.finditer(r'#define\s+(Mask_\w+)\s+0x([0-9A-Fa-f]+)', source)}
    masks['Mask_none'] = 0xFFFF
    declared = set(re.findall(r'sh4op\((\w+)\);', (FLYCAST / 'interpr/sh4_opcodes.h').read_text()))
    declared.add('reios_trap')
    table = [None] * 0x10000
    entries = re.finditer(r'^\s*\{\s*\w+\s*,\s*(\w+)\s*,\s*(\w+)\s*,\s*(\w+)\s*,\s*([\w|\s]+?)\s*,\s*"', source, re.M)
    for entry in entries:
        handler, mask_name, rez, type_expr = entry.groups()
        mask = masks[mask_name]
        base = reios_opcode() if rez == 'REIOS_OPCODE' else int(rez, 0)
        kind = evaluate(type_expr, types)
        free = [bit for bit in range(16) if not (mask >> bit) & 1]
        for combo in range(1 << len(free)):
            op = base
            for index, bit in enumerate(free):
                if (combo >> index) & 1:
                    op |= 1 << bit
            table[op] = (handler, kind)  # later rows win, as in BuildOpcodeTables
    return table, types, declared


def sext(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


class Emitter:
    def __init__(self, table, types, declared):
        self.table, self.types, self.declared = table, types, declared
        self.checks = {}

    def call(self, op, pc_after, epc, indent):
        handler, kind = self.table[op]
        lines = [f'{indent}ctx->pc = 0x{pc_after:08X}u;']
        if kind & self.types['UsesFPU']:
            lines.append(f'{indent}if (ctx->sr.FD) throw SH4ThrownException(0x{epc:08X}u, Sh4Ex_FpuDisabled);')
        if handler in self.declared:
            self.checks[op] = handler
            lines.append(f'{indent}{handler}(ctx, 0x{op:04X}u);')
        else:
            lines.append(f'{indent}OpPtr[0x{op:04X}](ctx, 0x{op:04X}u);')
        return lines

    def slot(self, op, addr, indent):
        inner = self.call(op, addr + 4, addr + 2, indent + '\t')
        return [f'{indent}try {{', *inner,
                f'{indent}}} catch (SH4ThrownException& ex) {{ AdjustDelaySlotException(ex); throw; }}']

    def branch(self, op, ds, addr):
        n = (op >> 8) & 0xF
        t = '\t'
        if (op & 0xF000) in (0xA000, 0xB000):  # bra / bsr
            target = (addr + 4 + sext(op & 0xFFF, 12) * 2) & 0xFFFFFFFF
            out = self.slot(ds, addr, t)
            if op & 0xF000 == 0xB000:
                out.append(f'\tctx->pr = 0x{addr + 4:08X}u;')
            return out + [f'\tctx->pc = 0x{target:08X}u;']
        if (op & 0xFF00) in (0x8D00, 0x8F00):  # bt/s, bf/s
            target = (addr + 4 + sext(op & 0xFF, 8) * 2) & 0xFFFFFFFF
            cond = 'ctx->sr.T != 0' if op & 0xFF00 == 0x8D00 else 'ctx->sr.T == 0'
            return ([f'\tif ({cond}) {{', *self.slot(ds, addr, '\t\t'), f'\t\tctx->pc = 0x{target:08X}u;',
                     '\t} else {', *self.call(ds, addr + 4, addr + 2, '\t\t'), '\t}'])
        forms = {
            0x0023: (f'ctx->r[{n}] + 0x{addr + 4:08X}u', False),  # braf
            0x0003: (f'ctx->r[{n}] + 0x{addr + 4:08X}u', True),   # bsrf
            0x402B: (f'ctx->r[{n}]', False),                      # jmp
            0x400B: (f'ctx->r[{n}]', True),                       # jsr
        }
        key = op & 0xF0FF
        if op == 0x000B:  # rts
            expr, link = 'ctx->pr', False
        elif key in forms:
            expr, link = forms[key]
        else:
            return None
        out = [f'\t{{ const u32 newpc = {expr};', *self.slot(ds, addr, '\t')]
        if link:
            out.append(f'\tctx->pr = 0x{addr + 4:08X}u;')
        return out + ['\tctx->pc = newpc; }']

    def block(self, name, pc, ops):
        body = []
        i = 0
        while i < len(ops):
            addr = (pc + i * 2) & 0xFFFFFFFF
            op = ops[i]
            _, kind = self.table[op]
            delayed = (kind & self.types['Delayslot']) and (kind & self.types['WritesPC'])
            if delayed and i + 1 < len(ops):
                inline = self.branch(op, ops[i + 1], addr)
                if inline is not None:
                    body += inline
                    i += 2
                    continue
                # rte and any other delayed branch: the handler runs its own slot
                body += self.call(op, addr + 2, addr, '\t')
                i += 2
                continue
            body += self.call(op, addr + 2, addr, '\t')
            i += 1
        return [f'static void {name}(Sh4Context *ctx)', '{', *body, '}']


def read_traces(paths):
    blocks = {}
    for path in paths:
        for line in Path(path).read_text().splitlines():
            if not line or line.startswith('#'):
                continue
            fields = line.split()
            pc, nops, cycles = int(fields[0], 16), int(fields[1]), int(fields[2])
            ops = tuple(int(x, 16) for x in fields[3:])
            if len(ops) != nops:
                raise ValueError(f'{path}: malformed line {line!r}')
            blocks[(pc, ops)] = cycles
    return blocks


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('traces', nargs='+', type=Path)
    parser.add_argument('--out', type=Path, default=ROOT / 'generated/recomp')
    parser.add_argument('--manifest', type=Path, default=ROOT / 'generated/recomp-manifest.txt')
    args = parser.parse_args()

    inputs = list(args.traces)
    if args.manifest.exists():
        inputs.append(args.manifest)
    blocks = read_traces(inputs)
    ordered = sorted(blocks.items())
    manifest = ['# sonicpad-trace v1: pc nops cycles ops...']
    manifest += [f'{pc:08X} {len(ops)} {cycles} ' + ' '.join(f'{op:04X}' for op in ops)
                 for (pc, ops), cycles in ordered]
    manifest_text = '\n'.join(manifest) + '\n'
    manifest_id = hashlib.sha256(manifest_text.encode()).hexdigest()[:16]

    table, types, declared = opcode_table()
    emitter = Emitter(table, types, declared)
    args.out.mkdir(parents=True, exist_ok=True)
    for stale in args.out.glob('sp_aot_*.cpp'):
        stale.unlink()

    chunks = [ordered[i:i + BLOCKS_PER_FILE] for i in range(0, len(ordered), BLOCKS_PER_FILE)]
    variants = {}
    for number, chunk in enumerate(chunks):
        lines = ['// Generated by scripts/recomp/generate_blocks.py; do not edit.',
                 '#include "sp_block_api.h"', '', 'namespace {']
        rows = []
        for (pc, ops), cycles in chunk:
            variant = variants.get(pc, 0)
            variants[pc] = variant + 1
            name = f'b_{pc:08X}_{variant}'
            lines += emitter.block(name, pc, ops)
            lines.append(f'const u16 c_{pc:08X}_{variant}[] = {{' + ', '.join(f'0x{op:04X}' for op in ops) + '};')
            lines.append('')
            rows.append(f'\t{{0x{pc:08X}u, {len(ops)}, {min(cycles, 0xFFFF)}, c_{pc:08X}_{variant}, {name}}},')
        lines += ['} // namespace', '',
                  f'extern const SpAotBlock sp_blocks_{number}[];',
                  f'extern const size_t sp_blocks_{number}_count;',
                  f'const SpAotBlock sp_blocks_{number}[] = {{', *rows, '};',
                  f'const size_t sp_blocks_{number}_count = {len(chunk)};', '']
        (args.out / f'sp_aot_blocks_{number:03d}.cpp').write_text('\n'.join(lines))

    index = ['// Generated by scripts/recomp/generate_blocks.py; do not edit.', '#include "sp_block_api.h"', '']
    for number in range(len(chunks)):
        index += [f'extern const SpAotBlock sp_blocks_{number}[];', f'extern const size_t sp_blocks_{number}_count;']
    index += ['', 'const SpAotTable sp_aot_tables[] = {']
    index += [f'\t{{sp_blocks_{n}, sp_blocks_{n}_count}},' for n in range(len(chunks))] or ['\t{nullptr, 0},']
    index += ['};', f'const size_t sp_aot_table_count = {len(chunks)};', '', 'const SpOpCheck sp_aot_opchecks[] = {']
    index += [f'\t{{0x{op:04X}, {handler}}},' for op, handler in sorted(emitter.checks.items())] or ['\t{0, nullptr},']
    index += ['};', f'const size_t sp_aot_opcheck_count = {len(emitter.checks)};',
              f'const char sp_aot_manifest_id[] = "{manifest_id}";', '']
    (args.out / 'sp_aot_index.cpp').write_text('\n'.join(index))
    args.manifest.write_text(manifest_text)
    ops_total = sum(len(ops) for (_, ops), _ in ordered)
    print(f'manifest={manifest_id} blocks={len(ordered)} ops={ops_total} files={len(chunks) + 1} '
          f'handlers={len(emitter.checks)}')


if __name__ == '__main__':
    main()
