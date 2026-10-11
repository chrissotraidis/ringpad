// SonicPad SH-4 executor: runs ahead-of-time compiled game blocks and uses
// Flycast's interpreter only for code that has not been recompiled yet.
// SONICPAD_TRACE=<file> records missed blocks so the generator can compile
// them in the next build. SONICPAD_STATS=1 reports AOT coverage.
// SONICPAD_INPUT_SCRIPT=<file> replays pad input at guest times, one event per
// line: "<start seconds> <duration seconds> <A|B|X|Y|START|UP|DOWN|LEFT|RIGHT|STICK:x,y>".
#include "sp_block_api.h"
#include "hw/sh4/sh4_interpreter.h"
#include "hw/sh4/sh4_interrupts.h"
#include "hw/sh4/sh4_mem.h"
#include "hw/sh4/sh4_sched.h"
#include "hw/sh4/sh4_cycles.h"
#include "debug/gdb_server.h"
#include "input/gamepad.h"
#include "input/gamepad_device.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int UpdateSystem_INTC();

namespace {

constexpr int kMaxBlockOps = 64;
constexpr int kInterpreterCpuRatio = 8; // Sh4Interpreter's non-STRICT_MODE underclock
constexpr double kSlicesPerSecond = 200000000.0 / SH4_TIMESLICE;

bool isRamAddress(u32 pc)
{
	return ((pc & 0x1FFFFFFF) >> 26) == 3;
}

// Block shape shared by the tracer and the miss path. Returns the number of
// opcodes (including a delay slot), or 0 if the code at pc stays interpreted.
int decodeBlock(u32 pc, u16 *ops)
{
	int count = 0;
	for (int i = 0; i < kMaxBlockOps - 1; i++)
	{
		const u32 addr = pc + i * 2;
		const u16 op = IReadMem16(addr);
		const sh4_opcodelistentry *desc = OpDesc[op];
		if (desc->type & Invalid)
			return count;
		ops[count++] = op;
		if (desc->SetPC())
		{
			if (desc->type & Delayslot)
			{
				const u16 ds = IReadMem16(addr + 2);
				const sh4_opcodelistentry *dsDesc = OpDesc[ds];
				if (dsDesc->SetPC() || (dsDesc->type & Invalid))
					return count - 1; // slot-illegal: leave the branch to the interpreter
				ops[count++] = ds;
			}
			return count;
		}
	}
	return count;
}

bool endsWithDelaySlot(const u16 *ops, int nops)
{
	return nops >= 2 && (OpDesc[ops[nops - 2]]->type & Delayslot) && OpDesc[ops[nops - 2]]->SetPC();
}

int countBlockCycles(const u16 *ops, int nops)
{
	Sh4Cycles cycles(1);
	cycles.reset();
	int total = 0;
	for (int i = 0; i < nops; i++)
		total += cycles.countCycles(ops[i]);
	return total;
}

class SonicPadExecutor final : public Sh4Interpreter
{
public:
	void Init() override
	{
		Sh4Interpreter::Init();
		if (const char *trace = std::getenv("SONICPAD_TRACE"))
			tracePath = trace;
		statsEnabled = std::getenv("SONICPAD_STATS") != nullptr;
		aotEnabled = std::getenv("SONICPAD_NO_AOT") == nullptr;
		if (const char *script = std::getenv("SONICPAD_INPUT_SCRIPT"))
			loadScript(script);
		loadTables();
		lastReport = std::chrono::steady_clock::now();
	}

	void Term() override
	{
		writeTrace();
		report(true);
		Sh4Interpreter::Term();
	}

	void Run() override
	{
		Instance = this;
		ctx->restoreHostRoundingMode();
		try {
			do
			{
				try {
					do
					{
						runBlock();
					} while (ctx->cycle_counter > 0);
					ctx->cycle_counter += SH4_TIMESLICE;
					UpdateSystem_INTC();
					++slices;
					if (!script.empty() && slices % 256 == 0)
						applyScript();
					if (slices % 8192 == 0)
						housekeeping();
				} catch (const SH4ThrownException& ex) {
					Do_Exception(ex.epc, ex.expEvn);
					ctx->cycle_counter -= 5;
				}
			} while (ctx->CpuRunning);
		} catch (const debugger::Stop&) {
		}
		ctx->CpuRunning = false;
		Instance = nullptr;
	}

private:
	struct ScriptEvent
	{
		u64 start;
		u64 end;
		u32 buttons;
		bool stick;
		s16 x;
		s16 y;
		bool announced;
	};

	void loadScript(const char *path)
	{
		static const std::pair<const char *, u32> names[] = {
			{"A", DC_BTN_A}, {"B", DC_BTN_B}, {"X", DC_BTN_X}, {"Y", DC_BTN_Y}, {"START", DC_BTN_START},
			{"UP", DC_DPAD_UP}, {"DOWN", DC_DPAD_DOWN}, {"LEFT", DC_DPAD_LEFT}, {"RIGHT", DC_DPAD_RIGHT},
		};
		std::ifstream in(path);
		std::string line;
		while (std::getline(in, line))
		{
			if (line.empty() || line[0] == '#')
				continue;
			std::istringstream fields(line);
			double start = 0, duration = 0;
			std::string what;
			if (!(fields >> start >> duration >> what))
				continue;
			ScriptEvent event{u64(start * kSlicesPerSecond), u64((start + duration) * kSlicesPerSecond), 0, false, 0, 0, false};
			int x = 0, y = 0;
			if (std::sscanf(what.c_str(), "STICK:%d,%d", &x, &y) == 2)
			{
				event.stick = true;
				event.x = s16(x);
				event.y = s16(y);
			}
			for (const auto& name : names)
				if (what == name.first)
					event.buttons = name.second;
			if (event.buttons != 0 || event.stick)
			{
				script.push_back(event);
				scriptMask |= event.buttons;
			}
		}
		std::fprintf(stderr, "SONICPAD_SCRIPT %s events=%zu\n", path, script.size());
	}

	void applyScript()
	{
		u32 pressed = 0;
		s16 x = 0, y = 0;
		for (ScriptEvent& event : script)
		{
			if (slices < event.start || slices >= event.end)
				continue;
			pressed |= event.buttons;
			if (event.stick)
			{
				x = event.x;
				y = event.y;
			}
			if (!event.announced)
			{
				event.announced = true;
				std::fprintf(stderr, "SONICPAD_SCRIPT t=%.2f buttons=%04X stick=%d,%d\n",
						slices / kSlicesPerSecond, event.buttons, event.x, event.y);
			}
		}
		kcode[0] = (kcode[0] | scriptMask) & ~pressed;
		joyx[0] = x;
		joyy[0] = y;
	}

	void loadTables()
	{
		index.clear();
		for (size_t i = 0; i < sp_aot_opcheck_count; i++)
		{
			const SpOpCheck& check = sp_aot_opchecks[i];
			if (OpPtr[check.op] != check.handler)
			{
				std::fprintf(stderr, "SONICPAD_AOT error: opcode %04X handler mismatch; AOT disabled\n", check.op);
				aotEnabled = false;
				return;
			}
		}
		size_t blocks = 0;
		for (size_t t = 0; t < sp_aot_table_count; t++)
			for (size_t b = 0; b < sp_aot_tables[t].count; b++)
			{
				const SpAotBlock *block = &sp_aot_tables[t].blocks[b];
				index[block->pc].push_back(block);
				knownShapes.insert(shapeKey(block->pc, block->code, block->nops));
				blocks++;
			}
		std::fprintf(stderr, "SONICPAD_AOT manifest=%s blocks=%zu opchecks=%zu enabled=%d trace=%s\n",
				sp_aot_manifest_id, blocks, sp_aot_opcheck_count, aotEnabled ? 1 : 0,
				tracePath.empty() ? "off" : tracePath.c_str());
	}

	static std::string shapeKey(u32 pc, const u16 *ops, int nops)
	{
		std::string key(reinterpret_cast<const char *>(&pc), sizeof(pc));
		key.append(reinterpret_cast<const char *>(ops), nops * sizeof(u16));
		return key;
	}

	const SpAotBlock *lookup(u32 pc)
	{
		if (!aotEnabled || !isRamAddress(pc))
			return nullptr;
		auto it = index.find(pc);
		if (it == index.end())
			return nullptr;
		const u8 *guest = &mem_b[pc & RAM_MASK];
		for (const SpAotBlock *block : it->second)
			if (std::memcmp(block->code, guest, block->nops * sizeof(u16)) == 0)
				return block;
		return nullptr;
	}

	void runBlock()
	{
		const u32 pc = ctx->pc;
		if (const SpAotBlock *block = lookup(pc))
		{
			aotOps += block->nops;
			ctx->cycle_counter -= block->cycles;
			block->fn(ctx);
			return;
		}
		u16 ops[kMaxBlockOps];
		const int nops = isRamAddress(pc) ? decodeBlock(pc, ops) : 0;
		if (nops > 0 && !tracePath.empty())
			record(pc, ops, nops);
		if (statsEnabled)
			missCounts[pc]++;
		int steps = 1;
		if (nops > 0)
			steps = endsWithDelaySlot(ops, nops) ? nops - 1 : nops;
		interpret(pc, steps);
	}

	// Interpret up to 'steps' instructions from pc; stop early when control
	// leaves the straight-line sequence (exception, interrupt or branch).
	void interpret(u32 pc, int steps)
	{
		for (int i = 0; i < steps; i++)
		{
			if (ctx->pc != pc + i * 2)
				break;
			const int before = ctx->cycle_counter;
			Step();
			Instance = this;
			ctx->cycle_counter = before - (before - ctx->cycle_counter) / kInterpreterCpuRatio;
			interpretedOps++;
		}
	}

	void record(u32 pc, const u16 *ops, int nops)
	{
		if (!knownShapes.insert(shapeKey(pc, ops, nops)).second)
			return;
		traced.push_back({pc, std::vector<u16>(ops, ops + nops), countBlockCycles(ops, nops)});
		traceDirty = true;
	}

	void writeTrace()
	{
		if (tracePath.empty() || !traceDirty)
			return;
		const std::string tmp = tracePath + ".tmp";
		FILE *f = std::fopen(tmp.c_str(), "w");
		if (f == nullptr)
			return;
		std::fprintf(f, "# sonicpad-trace v1: pc nops cycles ops...\n");
		for (const Traced& t : traced)
		{
			std::fprintf(f, "%08X %u %d", t.pc, (unsigned)t.ops.size(), t.cycles);
			for (u16 op : t.ops)
				std::fprintf(f, " %04X", op);
			std::fputc('\n', f);
		}
		std::fclose(f);
		std::rename(tmp.c_str(), tracePath.c_str());
		traceDirty = false;
	}

	void housekeeping()
	{
		writeTrace();
		report(false);
	}

	void report(bool final)
	{
		if (!statsEnabled)
			return;
		const auto now = std::chrono::steady_clock::now();
		if (!final && now - lastReport < std::chrono::seconds(2))
			return;
		lastReport = now;
		const double total = double(aotOps + interpretedOps);
		std::fprintf(stderr, "SONICPAD_STATS aot_ops=%llu interp_ops=%llu aot_pct=%.2f traced_new=%zu%s\n",
				(unsigned long long)aotOps, (unsigned long long)interpretedOps,
				total > 0 ? 100.0 * aotOps / total : 0.0, traced.size(), final ? " final" : "");
		if (final)
		{
			std::vector<std::pair<u64, u32>> top;
			for (const auto& [pc, count] : missCounts)
				top.emplace_back(count, pc);
			std::sort(top.rbegin(), top.rend());
			for (size_t i = 0; i < top.size() && i < 12; i++)
			{
				u16 ops[kMaxBlockOps];
				const int nops = isRamAddress(top[i].second) ? decodeBlock(top[i].second, ops) : 0;
				std::fprintf(stderr, "SONICPAD_MISS pc=%08X count=%llu nops=%d first=%04X\n", top[i].second,
						(unsigned long long)top[i].first, nops, nops > 0 ? ops[0] : 0);
			}
		}
	}

	struct Traced
	{
		u32 pc;
		std::vector<u16> ops;
		int cycles;
	};

	std::unordered_map<u32, std::vector<const SpAotBlock *>> index;
	std::unordered_map<u32, u64> missCounts;
	std::unordered_set<std::string> knownShapes;
	std::vector<Traced> traced;
	std::vector<ScriptEvent> script;
	u32 scriptMask = 0;
	std::string tracePath;
	bool traceDirty = false;
	bool aotEnabled = true;
	bool statsEnabled = false;
	u64 aotOps = 0;
	u64 interpretedOps = 0;
	u64 slices = 0;
	std::chrono::steady_clock::time_point lastReport;
};

} // namespace

Sh4Executor *SonicPad_CreateSh4Executor()
{
	return new SonicPadExecutor();
}
