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
STATIC_PER_FILE = 4000


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


MEM, R0INDEX = 1, 2


def inline_forms():
    """Inline C++ for common opcodes, transcribed from Flycast's sh4_opcodes.cpp.

    Each entry maps a handler name to fn(op, addr) -> (lines, flags). 'addr' is
    the instruction address; the handler would see ctx->pc == addr + 2.
    """
    def n(op): return (op >> 8) & 0xF
    def m(op): return (op >> 4) & 0xF
    def i4(op): return op & 0xF
    def i8(op): return op & 0xFF
    def s8(op): return sext(op & 0xFF, 8)
    def t(cond): return [f'ctx->sr.T = ({cond}) ? 1 : 0;']
    def h(v): return f'0x{v & 0xFFFFFFFF:08X}u'
    r = lambda k: f'ctx->r[{k}]'
    f = {}
    alu = lambda fn: (lambda op, a: (fn(op, a), 0))
    mem = lambda fn, flags=MEM: (lambda op, a: (fn(op, a), flags))
    f['i0000_0000_0000_1001'] = alu(lambda op, a: [])                                       # nop
    f['i1110_nnnn_iiii_iiii'] = alu(lambda op, a: [f'{r(n(op))} = {h(s8(op))};'])            # mov #imm,Rn
    f['i0110_nnnn_mmmm_0011'] = alu(lambda op, a: [f'{r(n(op))} = {r(m(op))};'])             # mov Rm,Rn
    f['i0111_nnnn_iiii_iiii'] = alu(lambda op, a: [f'{r(n(op))} += {h(s8(op))};'])           # add #imm,Rn
    f['i0011_nnnn_mmmm_1100'] = alu(lambda op, a: [f'{r(n(op))} += {r(m(op))};'])            # add
    f['i0011_nnnn_mmmm_1000'] = alu(lambda op, a: [f'{r(n(op))} -= {r(m(op))};'])            # sub
    f['i0010_nnnn_mmmm_1001'] = alu(lambda op, a: [f'{r(n(op))} &= {r(m(op))};'])            # and
    f['i0010_nnnn_mmmm_1010'] = alu(lambda op, a: [f'{r(n(op))} ^= {r(m(op))};'])            # xor
    f['i0010_nnnn_mmmm_1011'] = alu(lambda op, a: [f'{r(n(op))} |= {r(m(op))};'])            # or
    f['i1100_1001_iiii_iiii'] = alu(lambda op, a: [f'ctx->r[0] &= {h(i8(op))};'])             # and #imm,R0
    f['i1100_1011_iiii_iiii'] = alu(lambda op, a: [f'ctx->r[0] |= {h(i8(op))};'])             # or #imm,R0
    f['i0110_nnnn_mmmm_0111'] = alu(lambda op, a: [f'{r(n(op))} = ~{r(m(op))};'])            # not
    f['i0110_nnnn_mmmm_1011'] = alu(lambda op, a: [f'{r(n(op))} = 0u - {r(m(op))};'])        # neg
    f['i0110_nnnn_mmmm_1100'] = alu(lambda op, a: [f'{r(n(op))} = (u32)(u8){r(m(op))};'])    # extu.b
    f['i0110_nnnn_mmmm_1101'] = alu(lambda op, a: [f'{r(n(op))} = (u32)(u16){r(m(op))};'])   # extu.w
    f['i0110_nnnn_mmmm_1110'] = alu(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s8)(u8){r(m(op))};'])    # exts.b
    f['i0110_nnnn_mmmm_1111'] = alu(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s16)(u16){r(m(op))};'])  # exts.w
    for name, amount in (('i0100_nnnn_0000_1000', 2), ('i0100_nnnn_0001_1000', 8), ('i0100_nnnn_0010_1000', 16)):
        f[name] = alu(lambda op, a, s=amount: [f'{r(n(op))} <<= {s};'])                      # shll2/8/16
    for name, amount in (('i0100_nnnn_0000_1001', 2), ('i0100_nnnn_0001_1001', 8), ('i0100_nnnn_0010_1001', 16)):
        f[name] = alu(lambda op, a, s=amount: [f'{r(n(op))} >>= {s};'])                      # shlr2/8/16
    f['i0100_nnnn_0000_0000'] = alu(lambda op, a: [f'ctx->sr.T = {r(n(op))} >> 31;', f'{r(n(op))} <<= 1;'])   # shll
    f['i0100_nnnn_0000_0001'] = alu(lambda op, a: [f'ctx->sr.T = {r(n(op))} & 1;', f'{r(n(op))} >>= 1;'])     # shlr
    f['i0000_nnnn_0010_1001'] = alu(lambda op, a: [f'{r(n(op))} = ctx->sr.T;'])               # movt
    f['i0010_nnnn_mmmm_1000'] = alu(lambda op, a: t(f'({r(n(op))} & {r(m(op))}) == 0'))       # tst
    f['i1100_1000_iiii_iiii'] = alu(lambda op, a: t(f'(ctx->r[0] & {h(i8(op))}) == 0'))       # tst #imm,R0
    f['i1000_1000_iiii_iiii'] = alu(lambda op, a: t(f'ctx->r[0] == {h(s8(op))}'))             # cmp/eq #imm
    f['i0011_nnnn_mmmm_0000'] = alu(lambda op, a: t(f'{r(m(op))} == {r(n(op))}'))            # cmp/eq
    f['i0011_nnnn_mmmm_0010'] = alu(lambda op, a: t(f'{r(n(op))} >= {r(m(op))}'))            # cmp/hs
    f['i0011_nnnn_mmmm_0011'] = alu(lambda op, a: t(f'(s32){r(n(op))} >= (s32){r(m(op))}'))  # cmp/ge
    f['i0011_nnnn_mmmm_0110'] = alu(lambda op, a: t(f'{r(n(op))} > {r(m(op))}'))             # cmp/hi
    f['i0011_nnnn_mmmm_0111'] = alu(lambda op, a: t(f'(s32){r(n(op))} > (s32){r(m(op))}'))   # cmp/gt
    f['i0100_nnnn_0001_0001'] = alu(lambda op, a: t(f'(s32){r(n(op))} >= 0'))                 # cmp/pz
    f['i0100_nnnn_0001_0101'] = alu(lambda op, a: t(f'(s32){r(n(op))} > 0'))                  # cmp/pl
    f['i0100_nnnn_0001_0000'] = alu(lambda op, a: [f'{r(n(op))} -= 1;', *t(f'{r(n(op))} == 0')])   # dt
    f['i1100_0111_iiii_iiii'] = alu(lambda op, a: [f'ctx->r[0] = {h(((a + 4) & ~3) + (i8(op) << 2))};'])  # mova
    # loads
    f['i1101_nnnn_iiii_iiii'] = mem(lambda op, a: [f'{r(n(op))} = ReadMem32({h((i8(op) << 2) + ((a + 4) & ~3))});'])
    f['i1001_nnnn_iiii_iiii'] = mem(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s16)ReadMem16({h((i8(op) << 1) + a + 4)});'])
    f['i0110_nnnn_mmmm_0010'] = mem(lambda op, a: [f'{r(n(op))} = ReadMem32({r(m(op))});'])
    f['i0110_nnnn_mmmm_0001'] = mem(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s16)ReadMem16({r(m(op))});'])
    f['i0110_nnnn_mmmm_0000'] = mem(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s8)ReadMem8({r(m(op))});'])
    def postinc(read, size):
        return lambda op, a: [f'{r(n(op))} = {read}({r(m(op))});'] + ([f'{r(m(op))} += {size};'] if n(op) != m(op) else [])
    f['i0110_nnnn_mmmm_0110'] = mem(postinc('ReadMem32', 4))
    f['i0110_nnnn_mmmm_0101'] = mem(postinc('(u32)(s32)(s16)ReadMem16', 2))
    f['i0110_nnnn_mmmm_0100'] = mem(postinc('(u32)(s32)(s8)ReadMem8', 1))
    f['i0101_nnnn_mmmm_iiii'] = mem(lambda op, a: [f'{r(n(op))} = ReadMem32({r(m(op))} + {h(i4(op) << 2)});'])
    f['i1000_0100_mmmm_iiii'] = mem(lambda op, a: [f'ctx->r[0] = (u32)(s32)(s8)ReadMem8({r(m(op))} + {h(i4(op))});'])
    f['i1000_0101_mmmm_iiii'] = mem(lambda op, a: [f'ctx->r[0] = (u32)(s32)(s16)ReadMem16({r(m(op))} + {h(i4(op) << 1)});'])
    f['i0000_nnnn_mmmm_1110'] = mem(lambda op, a: [f'{r(n(op))} = ReadMem32(ctx->r[0] + {r(m(op))});'], MEM | R0INDEX)
    f['i0000_nnnn_mmmm_1101'] = mem(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s16)ReadMem16(ctx->r[0] + {r(m(op))});'], MEM | R0INDEX)
    f['i0000_nnnn_mmmm_1100'] = mem(lambda op, a: [f'{r(n(op))} = (u32)(s32)(s8)ReadMem8(ctx->r[0] + {r(m(op))});'], MEM | R0INDEX)
    f['i0100_nnnn_0010_0110'] = mem(lambda op, a: [f'ctx->pr = ReadMem32({r(n(op))});', f'{r(n(op))} += 4;'])  # lds.l @Rn+,PR
    # stores
    f['i0010_nnnn_mmmm_0010'] = mem(lambda op, a: [f'WriteMem32({r(n(op))}, {r(m(op))});'])
    f['i0010_nnnn_mmmm_0001'] = mem(lambda op, a: [f'WriteMem16({r(n(op))}, (u16){r(m(op))});'])
    f['i0010_nnnn_mmmm_0000'] = mem(lambda op, a: [f'WriteMem8({r(n(op))}, (u8){r(m(op))});'])
    def predec(write, cast, size):
        return lambda op, a: [f'{{ const u32 addr = {r(n(op))} - {size};', f'{write}(addr, {cast}{r(m(op))});',
                              f'{r(n(op))} = addr; }}']
    f['i0010_nnnn_mmmm_0110'] = mem(predec('WriteMem32', '', 4))
    f['i0010_nnnn_mmmm_0101'] = mem(predec('WriteMem16', '(u16)', 2))
    f['i0010_nnnn_mmmm_0100'] = mem(predec('WriteMem8', '(u8)', 1))
    f['i0100_nnnn_0010_0010'] = mem(lambda op, a: [f'{{ const u32 addr = {r(n(op))} - 4;', 'WriteMem32(addr, ctx->pr);',
                                                   f'{r(n(op))} = addr; }}'])                 # sts.l PR,@-Rn
    f['i0001_nnnn_mmmm_iiii'] = mem(lambda op, a: [f'WriteMem32({r(n(op))} + {h(i4(op) << 2)}, {r(m(op))});'])
    f['i1000_0000_mmmm_iiii'] = mem(lambda op, a: [f'WriteMem8({r(m(op))} + {h(i4(op))}, (u8)ctx->r[0]);'])
    f['i1000_0001_mmmm_iiii'] = mem(lambda op, a: [f'WriteMem16({r(m(op))} + {h(i4(op) << 1)}, (u16)ctx->r[0]);'])
    f['i0000_nnnn_mmmm_0110'] = mem(lambda op, a: [f'WriteMem32(ctx->r[0] + {r(n(op))}, {r(m(op))});'], MEM | R0INDEX)
    f['i0000_nnnn_mmmm_0101'] = mem(lambda op, a: [f'WriteMem16(ctx->r[0] + {r(n(op))}, (u16){r(m(op))});'], MEM | R0INDEX)
    f['i0000_nnnn_mmmm_0100'] = mem(lambda op, a: [f'WriteMem8(ctx->r[0] + {r(n(op))}, (u8){r(m(op))});'], MEM | R0INDEX)
    # conditional branches without delay slot (always the last op of a block)
    f['i1000_1001_iiii_iiii'] = alu(lambda op, a: [f'ctx->pc = ctx->sr.T != 0 ? {h(a + 4 + s8(op) * 2)} : {h(a + 2)};'])  # bt
    f['i1000_1011_iiii_iiii'] = alu(lambda op, a: [f'ctx->pc = ctx->sr.T == 0 ? {h(a + 4 + s8(op) * 2)} : {h(a + 2)};'])  # bf
    return f


INLINE = inline_forms()
SETS_PC = {'i1000_1001_iiii_iiii', 'i1000_1011_iiii_iiii'}
STORES = {'i0010_nnnn_mmmm_0010', 'i0010_nnnn_mmmm_0001', 'i0010_nnnn_mmmm_0000', 'i0010_nnnn_mmmm_0110',
          'i0010_nnnn_mmmm_0101', 'i0010_nnnn_mmmm_0100', 'i0100_nnnn_0010_0010', 'i0001_nnnn_mmmm_iiii',
          'i1000_0000_mmmm_iiii', 'i1000_0001_mmmm_iiii', 'i0000_nnnn_mmmm_0110', 'i0000_nnnn_mmmm_0101',
          'i0000_nnnn_mmmm_0100'}
SELFTEST_PC = 0x8CFF8000
# Inline forms that build an address from the instruction's own location.
# Relative (static) blocks call the handler instead, which reads ctx->pc.
PC_FORMS = {'i1100_0111_iiii_iiii', 'i1101_nnnn_iiii_iiii', 'i1001_nnnn_iiii_iiii',
            'i1000_1001_iiii_iiii', 'i1000_1011_iiii_iiii'}


class Emitter:
    def __init__(self, table, types, declared, inline=True):
        self.table, self.types, self.declared = table, types, declared
        self.inline = inline
        self.checks = {}
        self.inlined = {}
        self.rel = False      # addresses relative to the block's entry PC ('base')
        self.base_pc = 0

    def a(self, value):
        """An address as C++: a constant, or base-relative in a relative block."""
        if self.rel:
            return f'(base + 0x{(value - self.base_pc) & 0xFFFFFFFF:X}u)'
        return f'0x{value & 0xFFFFFFFF:08X}u'

    def op(self, op, addr, indent, state, allow_inline=True):
        """Emit one instruction. state['pc'] is the value known to be in ctx->pc."""
        handler, kind = self.table[op]
        form = INLINE.get(handler) if self.inline and allow_inline and not (self.rel and handler in PC_FORMS) else None
        lines = []
        if form is not None:
            body, flags = form(op, addr)
            if flags & MEM and state['pc'] != addr + 2:
                lines.append(f'{indent}ctx->pc = {self.a(addr + 2)};')
                state['pc'] = addr + 2
            lines += [indent + line for line in body]
            if handler in SETS_PC:
                state['pc'] = None
            self.inlined[op] = (handler, flags)
            return lines
        if state['pc'] != addr + 2:
            lines.append(f'{indent}ctx->pc = {self.a(addr + 2)};')
        state['pc'] = None if kind & self.types['WritesPC'] else addr + 2
        if kind & self.types['UsesFPU']:
            lines.append(f'{indent}if (ctx->sr.FD) throw SH4ThrownException({self.a(addr)}, Sh4Ex_FpuDisabled);')
        if handler in self.declared:
            self.checks[op] = handler
            lines.append(f'{indent}{handler}(ctx, 0x{op:04X}u);')
        else:
            lines.append(f'{indent}OpPtr[0x{op:04X}](ctx, 0x{op:04X}u);')
        return lines

    def slot(self, op, addr, indent):
        inner = self.op(op, addr + 2, indent + '\t', {'pc': None})
        return [f'{indent}try {{', *inner,
                f'{indent}}} catch (SH4ThrownException& ex) {{ AdjustDelaySlotException(ex); throw; }}']

    def branch(self, op, ds, addr):
        n = (op >> 8) & 0xF
        if (op & 0xF000) in (0xA000, 0xB000):  # bra / bsr
            target = (addr + 4 + sext(op & 0xFFF, 12) * 2) & 0xFFFFFFFF
            out = self.slot(ds, addr, '\t')
            if op & 0xF000 == 0xB000:
                out.append(f'\tctx->pr = {self.a(addr + 4)};')
            return out + [f'\tctx->pc = {self.a(target)};']
        if (op & 0xFF00) in (0x8D00, 0x8F00):  # bt/s, bf/s
            target = (addr + 4 + sext(op & 0xFF, 8) * 2) & 0xFFFFFFFF
            cond = 'ctx->sr.T != 0' if op & 0xFF00 == 0x8D00 else 'ctx->sr.T == 0'
            plain = self.op(ds, addr + 2, '\t\t', {'pc': None})
            return ([f'\tif ({cond}) {{', *self.slot(ds, addr, '\t\t'), f'\t\tctx->pc = {self.a(target)};',
                     '\t} else {', *plain, f'\t\tctx->pc = {self.a(addr + 4)};', '\t}'])
        forms = {
            0x0023: (f'ctx->r[{n}] + {self.a(addr + 4)}', False),  # braf
            0x0003: (f'ctx->r[{n}] + {self.a(addr + 4)}', True),   # bsrf
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
            out.append(f'\tctx->pr = {self.a(addr + 4)};')
        return out + ['\tctx->pc = newpc; }']

    def pure_self_loop(self, pc, ops):
        """True if the block only loads and computes, then branches back to itself."""
        if not self.inline or not ops:
            return False
        last = ops[-1]
        if self.table[last][0] not in SETS_PC:
            return False
        addr = pc + (len(ops) - 1) * 2
        if (addr + 4 + sext(last & 0xFF, 8) * 2) & 0xFFFFFFFF != pc:
            return False
        for op in ops[:-1]:
            handler = self.table[op][0]
            form = INLINE.get(handler)
            if form is None or handler in STORES:
                return False
        return True

    def block(self, name, pc, ops):
        self.base_pc = pc
        body = ['\tconst u32 base = ctx->pc;', '\t(void)base;'] if self.rel else []
        state = {'pc': pc}
        i = 0
        while i < len(ops):
            addr = (pc + i * 2) & 0xFFFFFFFF
            op = ops[i]
            _, kind = self.table[op]
            delayed = (kind & self.types['Delayslot']) and (kind & self.types['WritesPC'])
            if delayed and i + 1 < len(ops):
                inline = self.branch(op, ops[i + 1], addr)
                if inline is None:
                    # rte and any other delayed branch: the handler runs its own slot
                    inline = self.op(op, addr, '\t', {'pc': None}, allow_inline=False)
                body += inline
                state['pc'] = None
                i += 2
                continue
            body += self.op(op, addr, '\t', state)
            i += 1
        last = (pc + (len(ops) - 1) * 2) & 0xFFFFFFFF
        if state['pc'] is not None and state['pc'] != last + 2:
            body.append(f'\tctx->pc = {self.a(last + 2)};')
        return [f'static void {name}(Sh4Context *ctx)', '{', *body, '}']

    def selftest(self):
        lines = ['// Generated by scripts/recomp/generate_blocks.py; do not edit.',
                 '// Inline forms paired with the Flycast handler they replace, for the startup self-test.',
                 '#include "sp_block_api.h"', '', 'namespace {']
        rows = []
        for op, (handler, flags) in sorted(self.inlined.items()):
            state = {'pc': SELFTEST_PC + 2}
            body = self.op(op, SELFTEST_PC, '\t', state)
            lines += [f'void t_{op:04X}(Sh4Context *ctx)', '{', *body, '}']
            base = (op >> 4) & 0xF if handler.endswith(('1100', '1101', '1110')) else (op >> 8) & 0xF
            skip = 1 if (flags & R0INDEX) and base == 0 else 0
            rows.append(f'\t{{0x{op:04X}, {handler}, t_{op:04X}, {flags | (skip << 2)}}},')
        lines += ['} // namespace', '',
                  f'const u32 sp_selftest_pc = 0x{SELFTEST_PC:08X}u;',
                  'const SpSelfTest sp_selftests[] = {', *(rows or ['\t{0, nullptr, nullptr, 0},']), '};',
                  f'const size_t sp_selftest_count = {len(rows)};', '']
        return '\n'.join(lines)



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
    parser.add_argument('--no-inline', action='store_true', help='call Flycast handlers for every opcode')
    parser.add_argument('--static', type=Path,
                        help='blocks found by discover_blocks.py; compiled in compact form, not saved to the manifest')
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
    emitter = Emitter(table, types, declared, inline=not args.no_inline)
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
            flags = 1 if emitter.pure_self_loop(pc, ops) else 0
            rows.append(f'\t{{0x{pc:08X}u, {len(ops)}, {min(cycles, 0xFFFF)}, {flags}, c_{pc:08X}_{variant}, {name}}},')
        lines += ['} // namespace', '',
                  f'extern const SpAotBlock sp_blocks_{number}[];',
                  f'extern const size_t sp_blocks_{number}_count;',
                  f'const SpAotBlock sp_blocks_{number}[] = {{', *rows, '};',
                  f'const size_t sp_blocks_{number}_count = {len(chunk)};', '']
        (args.out / f'sp_aot_blocks_{number:03d}.cpp').write_text('\n'.join(lines))

    # Static blocks: address-relative, one function per distinct instruction
    # sequence, shared by every address it occurs at. Cycles are counted by
    # the executor at startup (0 here).
    static_chunks, static_id, static_rows = [], None, 0
    if args.static:
        found = read_traces([args.static])
        extra = sorted(key for key in found if key not in blocks)
        static_id = hashlib.sha256(args.static.read_bytes()).hexdigest()[:16]
        uses = {}
        for pc, ops in extra:
            uses.setdefault(ops, []).append(pc)
        distinct = sorted(uses)
        static_rows = len(extra)
        emitter.rel = True
        for start in range(0, len(distinct), STATIC_PER_FILE):
            number = len(static_chunks)
            lines = ['// Generated by scripts/recomp/generate_blocks.py; do not edit.',
                     '// Statically discovered blocks; addresses are relative to the entry PC.',
                     '#include "sp_block_api.h"', '', 'namespace {']
            rows = []
            for i, ops in enumerate(distinct[start:start + STATIC_PER_FILE], start):
                lines += emitter.block(f'z_{i}', uses[ops][0], ops)
                lines.append(f'const u16 k_{i}[] = {{' + ', '.join(f'0x{op:04X}' for op in ops) + '};')
                rows += [f'\t{{0x{pc:08X}u, {len(ops)}, 0, 0, k_{i}, z_{i}}},' for pc in uses[ops]]
            lines += ['} // namespace', '',
                      f'extern const SpAotBlock sp_static_{number}[];',
                      f'extern const size_t sp_static_{number}_count;',
                      f'const SpAotBlock sp_static_{number}[] = {{', *rows, '};',
                      f'const size_t sp_static_{number}_count = {len(rows)};', '']
            (args.out / f'sp_aot_static_{number:03d}.cpp').write_text('\n'.join(lines))
            static_chunks.append(number)
        emitter.rel = False
        manifest_id = f'{manifest_id}+{static_id}'

    index = ['// Generated by scripts/recomp/generate_blocks.py; do not edit.', '#include "sp_block_api.h"', '']
    for number in range(len(chunks)):
        index += [f'extern const SpAotBlock sp_blocks_{number}[];', f'extern const size_t sp_blocks_{number}_count;']
    for number in static_chunks:
        index += [f'extern const SpAotBlock sp_static_{number}[];', f'extern const size_t sp_static_{number}_count;']
    index += ['', 'const SpAotTable sp_aot_tables[] = {']
    index += [f'\t{{sp_blocks_{n}, sp_blocks_{n}_count}},' for n in range(len(chunks))] or ['\t{nullptr, 0},']
    index += [f'\t{{sp_static_{n}, sp_static_{n}_count}},' for n in static_chunks]
    index += ['};', f'const size_t sp_aot_table_count = {len(chunks) + len(static_chunks)};', '',
              'const SpOpCheck sp_aot_opchecks[] = {']
    index += [f'\t{{0x{op:04X}, {handler}}},' for op, handler in sorted(emitter.checks.items())] or ['\t{0, nullptr},']
    index += ['};', f'const size_t sp_aot_opcheck_count = {len(emitter.checks)};',
              f'const char sp_aot_manifest_id[] = "{manifest_id}";', '']
    (args.out / 'sp_aot_index.cpp').write_text('\n'.join(index))
    (args.out / 'sp_aot_selftest.cpp').write_text(emitter.selftest())
    args.manifest.write_text(manifest_text)
    ops_total = sum(len(ops) for (_, ops), _ in ordered)
    print(f'manifest={manifest_id} blocks={len(ordered)} ops={ops_total} files={len(chunks) + 1} '
          f'handlers={len(emitter.checks)} inlined_forms={len(emitter.inlined)} '
          f'static_blocks={static_rows} static_files={len(static_chunks)}')


if __name__ == '__main__':
    main()
