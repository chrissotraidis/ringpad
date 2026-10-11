// SonicPad static recompilation: interface between generated SH-4 blocks and the runtime.
#pragma once
#include "types.h"
#include "hw/sh4/sh4_if.h"
#include "hw/sh4/sh4_core.h"
#include "hw/sh4/sh4_mem.h"
#include "hw/sh4/sh4_opcode_list.h"
#include "hw/sh4/interpr/sh4_opcodes.h"
#include "reios/reios.h"

using SpBlockFn = void (*)(Sh4Context *ctx);
constexpr u16 kSpPureSelfLoop = 1;

// One ahead-of-time compiled guest block. The executor only runs it when the
// guest bytes at 'pc' still equal 'code', so overlays loaded later at the same
// address fall back to the interpreter instead of running stale code.
struct SpAotBlock
{
	u32 pc;
	u16 nops;      // instruction count including any delay slot
	u16 cycles;    // SH-4 issue cycles at ratio 1
	u16 flags;     // kSpPureSelfLoop: loads/ALU only, branches back to its own start
	const u16 *code;
	SpBlockFn fn;
};

// Every handler the generated code calls directly, so the runtime can confirm
// it matches Flycast's live opcode table before enabling AOT code.
struct SpOpCheck
{
	u16 op;
	OpCallFP *handler;
};

struct SpAotTable
{
	const SpAotBlock *blocks;
	size_t count;
};

// Provided by generated/recomp/sp_aot_index.cpp (or the empty stub).
extern const SpAotTable sp_aot_tables[];
extern const size_t sp_aot_table_count;
extern const SpOpCheck sp_aot_opchecks[];
extern const size_t sp_aot_opcheck_count;
extern const char sp_aot_manifest_id[];

// One inlined opcode value and the Flycast handler it replaces. The executor
// runs both on randomized state at startup and disables AOT on any difference.
struct SpSelfTest
{
	u16 op;
	OpCallFP *handler;
	SpBlockFn inlined;
	u32 flags;   // 1: touches memory, 2: R0-indexed address, 4: skip (R0 is also the base)
};

extern const u32 sp_selftest_pc;
extern const SpSelfTest sp_selftests[];
extern const size_t sp_selftest_count;
