// SonicPad SH-4 executor: runs ahead-of-time compiled game blocks and uses
// Flycast's interpreter only for code that has not been recompiled yet.
// SONICPAD_TRACE=<file> records missed blocks so the generator can compile
// them in the next build. SONICPAD_STATS=1 reports AOT coverage.
// SONICPAD_INPUT_SCRIPT=<file> replays pad input at guest times, one event per
// line: "<start seconds> <duration seconds> <what>", where <what> is one of
// A|B|X|Y|START|UP|DOWN|LEFT|RIGHT|LT|RT, STICK:x,y or ANGLE:deg[,pct] (0 = up,
// 90 = right). Duration-0 actions: SHOT:<name> writes a PNG of the last frame to
// SONICPAD_SHOT_DIR (default generated/shots); STATE:<file> writes a development
// save state. SONICPAD_STATE_LOAD=<file> resumes from such a state at boot.
// SONICPAD_UNCAPPED=1 runs as fast as the host allows (no audio pacing) and
// SONICPAD_EXIT_AT=<guest seconds> prints a SONICPAD_PERF line and exits.
#include "sp_block_api.h"
#include "emulator.h"
#include "serialize.h"
#include "hw/pvr/Renderer_if.h"
#include "hw/sh4/sh4_interpreter.h"
#include "hw/sh4/sh4_interrupts.h"
#include "hw/sh4/sh4_mem.h"
#include "hw/sh4/sh4_sched.h"
#include "hw/sh4/sh4_cycles.h"
#include "debug/gdb_server.h"
#include "input/gamepad.h"
#include "input/gamepad_device.h"
#include "ui/gui.h"
#include "stb_image_write.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int UpdateSystem_INTC();

namespace {

constexpr int kMaxBlockOps = 64;
constexpr int kInterpreterCpuRatio = 8; // Sh4Interpreter's non-STRICT_MODE underclock
constexpr double kSlicesPerSecond = 200000000.0 / SH4_TIMESLICE;

double threadCpuSeconds()
{
	timespec ts;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
	return ts.tv_sec + ts.tv_nsec * 1e-9;
}

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
		idleSkipEnabled = std::getenv("SONICPAD_NO_IDLE_SKIP") == nullptr;
		if (const char *script = std::getenv("SONICPAD_INPUT_SCRIPT"))
			loadScript(script);
		if (const char *state = std::getenv("SONICPAD_STATE_LOAD"))
			stateLoadPath = state;
		uncapped = std::getenv("SONICPAD_UNCAPPED") != nullptr;
		if (const char *exitAt = std::getenv("SONICPAD_EXIT_AT"))
			exitAtSlices = u64(std::atof(exitAt) * kSlicesPerSecond);
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
		const double cpuAtStart = threadCpuSeconds();
		if (!wallStarted)
		{
			wallStarted = true;
			wallStart = std::chrono::steady_clock::now();
			slicesAtWallStart = slices;
		}
		cpuBase = cpuAccumulated - cpuAtStart;
		if (!selfTestDone)
		{
			selfTestDone = true;
			if (aotEnabled && std::getenv("SONICPAD_SKIP_SELFTEST") == nullptr && !runSelfTest())
			{
				aotEnabled = false;
				std::fprintf(stderr, "SONICPAD_AOT disabled: self-test failed\n");
			}
			if (std::getenv("SONICPAD_SELFTEST_ONLY") != nullptr)
			{
				std::fflush(stderr);
				std::_Exit(aotEnabled ? 0 : 1);
			}
		}
		try {
			do
			{
				try {
					if (uncapped)
						settings.input.fastForwardMode = true;
					do
					{
						runBlock();
					} while (ctx->cycle_counter > 0);
					ctx->cycle_counter += SH4_TIMESLICE;
					UpdateSystem_INTC();
					++slices;
					if (!stateLoadPath.empty() && slices == 64)
						loadState();
					if (!script.empty() && slices % 256 == 0)
						applyScript();
					if (slices % 8192 == 0)
						housekeeping();
					if (exitAtSlices != 0 && slices >= exitAtSlices)
						exitWithPerf();
				} catch (const SH4ThrownException& ex) {
					Do_Exception(ex.epc, ex.expEvn);
					ctx->cycle_counter -= 5;
				}
			} while (ctx->CpuRunning);
		} catch (const debugger::Stop&) {
		}
		cpuAccumulated = cpuBase + threadCpuSeconds();
		ctx->CpuRunning = false;
		Instance = nullptr;
	}

private:
	// Differential self-test: every inlined opcode value must leave the CPU
	// context and memory exactly as Flycast's handler does.
	bool runSelfTest()
	{
		if (sp_selftest_count == 0)
			return true;
		constexpr u32 kWindow = 0x8CFF0000;
		constexpr u32 kWindowSize = 0x1000;
		u8 *window = &mem_b[kWindow & RAM_MASK];
		std::vector<u8> saved(window, window + kWindowSize), initial(kWindowSize), afterHandler(kWindowSize);
		u64 seed = 0x9E3779B97F4A7C15ull;
		auto next = [&seed]() {
			seed ^= seed << 13;
			seed ^= seed >> 7;
			seed ^= seed << 17;
			return seed;
		};
		static const u32 edges[] = {0, 1, 2, 0x7F, 0x80, 0xFF, 0x7FFF, 0x8000, 0xFFFF, 0x7FFFFFFF, 0x80000000u, 0xFFFFFFFFu};
		size_t trials = 0, failures = 0, skipped = 0;
		for (size_t i = 0; i < sp_selftest_count; i++)
		{
			const SpSelfTest& test = sp_selftests[i];
			if (test.flags & 4)
			{
				skipped++;
				continue;
			}
			const bool touchesMemory = test.flags & 1;
			const int count = touchesMemory ? 8 : 32;
			for (int k = 0; k < count; k++)
			{
				Sh4Context base = *ctx;
				for (int r = 0; r < 16; r++)
				{
					const u64 v = next();
					if (touchesMemory)
						base.r[r] = kWindow + 0x100 + (u32(v) & 0x7F0);
					else
						base.r[r] = (v & 3) == 0 ? edges[(v >> 8) % (sizeof(edges) / sizeof(edges[0]))] : u32(v >> 16);
				}
				if (test.flags & 2)
					base.r[0] = u32(next()) & 0xFC;
				base.sr.T = next() & 1;
				base.pr = u32(next());
				base.pc = sp_selftest_pc + 2;
				for (u32 j = 0; j < kWindowSize; j += 8)
				{
					const u64 v = next();
					std::memcpy(window + j, &v, 8);
				}
				std::memcpy(initial.data(), window, kWindowSize);
				Sh4Context viaHandler = base;
				test.handler(&viaHandler, test.op);
				std::memcpy(afterHandler.data(), window, kWindowSize);
				std::memcpy(window, initial.data(), kWindowSize);
				Sh4Context viaInline = base;
				test.inlined(&viaInline);
				trials++;
				if (std::memcmp(&viaHandler, &viaInline, sizeof(Sh4Context)) != 0
						|| std::memcmp(afterHandler.data(), window, kWindowSize) != 0)
				{
					if (failures++ < 10)
						std::fprintf(stderr, "SONICPAD_SELFTEST mismatch op=%04X pc=%08X/%08X r0=%08X/%08X T=%u/%u\n",
								test.op, viaHandler.pc, viaInline.pc, viaHandler.r[0], viaInline.r[0],
								(unsigned)viaHandler.sr.T, (unsigned)viaInline.sr.T);
					break;
				}
			}
		}
		std::memcpy(window, saved.data(), kWindowSize);
		std::fprintf(stderr, "SONICPAD_SELFTEST forms=%zu trials=%zu skipped=%zu failures=%zu\n",
				sp_selftest_count, trials, skipped, failures);
		return failures == 0;
	}

	static constexpr u32 kPageBits = 12;
	static constexpr u32 kPageMask = (1u << kPageBits) - 1;
	static constexpr u32 kSlotsPerPage = 1u << (kPageBits - 1);

	struct RtBlock
	{
		const SpAotBlock *block = nullptr;
		RtBlock *variant = nullptr;   // other blocks compiled at the same RAM offset
		RtBlock *succ[2] = {};        // chained successors seen at runtime
		u32 succPc[2] = {};
		u8 nextSlot = 0;
	};

	void exitWithPerf()
	{
		const double wall = std::chrono::duration<double>(std::chrono::steady_clock::now() - wallStart).count();
		const double guest = (slices - slicesAtWallStart) / kSlicesPerSecond;
		const double cpu = cpuBase + threadCpuSeconds();
		const double total = double(aotOps + interpretedOps);
		std::fprintf(stderr, "SONICPAD_PERF guest_s=%.2f wall_s=%.2f speed=%.2fx sh4_cpu_s=%.2f sh4_cpu_per_guest_s=%.3f "
				"blocks=%llu chained=%llu idle_skip_pct=%.2f aot_pct=%.3f uncapped=%d\n",
				guest, wall, guest / wall, cpu, cpu / guest, (unsigned long long)blocksRun,
				(unsigned long long)chainedRun, 100.0 * idleSkipped / (guest * 200000000.0), total > 0 ? 100.0 * aotOps / total : 0.0, uncapped ? 1 : 0);
		writeTrace();
		report(true);
		std::fflush(stderr);
		std::_Exit(0);
	}

	struct ScriptEvent
	{
		enum Kind : u8 { Input, Shot, State };
		u64 start;
		u64 end;
		u32 buttons = 0;
		bool stick = false;
		s16 x = 0;
		s16 y = 0;
		u8 lt = 0;
		u8 rt = 0;
		Kind kind = Input;
		std::string arg;
		bool announced = false;
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
			ScriptEvent event;
			event.start = u64(start * kSlicesPerSecond);
			event.end = u64((start + duration) * kSlicesPerSecond);
			int x = 0, y = 0;
			double deg = 0, pct = 100;
			if (std::sscanf(what.c_str(), "STICK:%d,%d", &x, &y) == 2)
			{
				event.stick = true;
				event.x = s16(x);
				event.y = s16(y);
			}
			else if (std::sscanf(what.c_str(), "ANGLE:%lf,%lf", &deg, &pct) >= 1)
			{
				// Dreamcast stick axes: x grows to the right, y grows downwards.
				const double r = 32000.0 * std::clamp(pct, 0.0, 100.0) / 100.0;
				event.stick = true;
				event.x = s16(std::lround(std::sin(deg * M_PI / 180.0) * r));
				event.y = s16(std::lround(-std::cos(deg * M_PI / 180.0) * r));
			}
			else if (what == "LT")
				event.lt = 255;
			else if (what == "RT")
				event.rt = 255;
			else if (what.rfind("SHOT:", 0) == 0)
			{
				event.kind = ScriptEvent::Shot;
				event.arg = what.substr(5);
			}
			else if (what.rfind("STATE:", 0) == 0)
			{
				event.kind = ScriptEvent::State;
				event.arg = what.substr(6);
			}
			for (const auto& name : names)
				if (what == name.first)
					event.buttons = name.second;
			if (event.buttons != 0 || event.stick || event.lt != 0 || event.rt != 0 || event.kind != ScriptEvent::Input)
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
		u8 l = 0, r = 0;
		for (ScriptEvent& event : script)
		{
			if (event.kind != ScriptEvent::Input)
			{
				if (!event.announced && slices >= event.start)
				{
					event.announced = true;
					if (event.kind == ScriptEvent::Shot)
						takeShot(event.arg);
					else
						saveState(event.arg);
				}
				continue;
			}
			if (slices < event.start || slices >= event.end)
				continue;
			pressed |= event.buttons;
			l = std::max(l, event.lt);
			r = std::max(r, event.rt);
			if (event.stick)
			{
				x = event.x;
				y = event.y;
			}
			if (!event.announced)
			{
				event.announced = true;
				std::fprintf(stderr, "SONICPAD_SCRIPT t=%.2f buttons=%04X stick=%d,%d lt=%u rt=%u\n",
						slices / kSlicesPerSecond, event.buttons, event.x, event.y, event.lt, event.rt);
			}
		}
		kcode[0] = (kcode[0] | scriptMask) & ~pressed;
		joyx[0] = x;
		joyy[0] = y;
		lt[0] = l;
		rt[0] = r;
	}

	// Development aid: PNG of the last presented frame, written on the UI thread.
	void takeShot(const std::string& name)
	{
		const char *dir = std::getenv("SONICPAD_SHOT_DIR");
		const std::string folder = dir != nullptr ? dir : "generated/shots";
		mkdir(folder.c_str(), 0755);
		const std::string path = folder + "/" + name + ".png";
		const double t = slices / kSlicesPerSecond;
		gui_runOnUiThread([path, t]() {
			std::vector<u8> rgb;
			int w = 0, h = 0;
			const bool got = renderer != nullptr && renderer->GetLastFrame(rgb, w, h) && !rgb.empty();
			bool ok = false;
			if (got)
			{
				stbi_flip_vertically_on_write(0);
				ok = stbi_write_png(path.c_str(), w, h, 3, rgb.data(), w * 3) != 0;
			}
			std::fprintf(stderr, "SONICPAD_SHOT t=%.2f %s %dx%d ok=%d\n", t, path.c_str(), w, h, ok ? 1 : 0);
		});
	}

	// Development save states let a route be iterated segment by segment. They
	// are written at a timeslice boundary and carry the slice count so that the
	// script timeline continues where it stopped. Acceptance runs never use them.
	static constexpr char kStateMagic[8] = {'S', 'P', 'S', 'T', 'A', 'T', 'E', '1'};

	void saveState(const std::string& file)
	{
		Serializer sizer;
		dc_serialize(sizer);
		std::vector<u8> data(sizer.size());
		Serializer ser(data.data(), data.size());
		dc_serialize(ser);
		FILE *f = std::fopen(file.c_str(), "wb");
		bool ok = f != nullptr;
		if (f != nullptr)
		{
			ok = std::fwrite(kStateMagic, sizeof(kStateMagic), 1, f) == 1
					&& std::fwrite(&slices, sizeof(slices), 1, f) == 1
					&& std::fwrite(data.data(), data.size(), 1, f) == 1;
			ok = std::fclose(f) == 0 && ok;
		}
		std::fprintf(stderr, "SONICPAD_STATE save t=%.2f %s bytes=%zu ok=%d\n", slices / kSlicesPerSecond,
				file.c_str(), data.size(), ok ? 1 : 0);
	}

	void loadState()
	{
		const std::string file = stateLoadPath;
		stateLoadPath.clear();
		std::ifstream in(file, std::ios::binary);
		std::vector<u8> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		u64 savedSlices = 0;
		if (bytes.size() <= sizeof(kStateMagic) + sizeof(savedSlices)
				|| std::memcmp(bytes.data(), kStateMagic, sizeof(kStateMagic)) != 0)
		{
			std::fprintf(stderr, "SONICPAD_STATE load failed: %s is not a SonicPad state\n", file.c_str());
			return;
		}
		std::memcpy(&savedSlices, bytes.data() + sizeof(kStateMagic), sizeof(savedSlices));
		const size_t offset = sizeof(kStateMagic) + sizeof(savedSlices);
		try {
			Deserializer deser(bytes.data() + offset, bytes.size() - offset);
			emu.loadstate(deser);
		} catch (const std::exception& e) {
			std::fprintf(stderr, "SONICPAD_STATE load failed: %s\n", e.what());
			return;
		}
		Instance = this;
		prev = nullptr;
		slices = savedSlices;
		slicesAtWallStart = slices;
		wallStart = std::chrono::steady_clock::now();
		for (ScriptEvent& event : script)
			if (event.start < slices)
				event.announced = true;
		std::fprintf(stderr, "SONICPAD_STATE load t=%.2f %s\n", slices / kSlicesPerSecond, file.c_str());
	}

	void loadTables()
	{
		pages.clear();
		runtime.clear();
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
			blocks += sp_aot_tables[t].count;
		runtime.resize(blocks);
		pages.resize((RAM_MASK + 1) >> kPageBits);
		size_t n = 0;
		for (size_t t = 0; t < sp_aot_table_count; t++)
			for (size_t b = 0; b < sp_aot_tables[t].count; b++)
			{
				const SpAotBlock *block = &sp_aot_tables[t].blocks[b];
				if (!tracePath.empty())
					knownShapes.insert(shapeKey(block->pc, block->code, block->nops));
				if (!isRamAddress(block->pc))
					continue;
				RtBlock& rt = runtime[n++];
				rt.block = block;
				const u32 offset = block->pc & RAM_MASK;
				auto& page = pages[offset >> kPageBits];
				if (!page)
					page.reset(new RtBlock *[kSlotsPerPage]());
				RtBlock *& slot = page[(offset & kPageMask) >> 1];
				rt.variant = slot;
				slot = &rt;
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

	// Page-indexed lookup; returns the chain of blocks compiled at this RAM offset.
	RtBlock *tableLookup(u32 pc) const
	{
		if (!aotEnabled || !isRamAddress(pc))
			return nullptr;
		const u32 offset = pc & RAM_MASK;
		const auto& page = pages[offset >> kPageBits];
		return page ? page[(offset & kPageMask) >> 1] : nullptr;
	}

	// The compiled block runs only if guest memory still holds the exact bytes
	// it was generated from.
	static bool codeMatches(const SpAotBlock *block, const u8 *guest)
	{
		const u8 *code = reinterpret_cast<const u8 *>(block->code);
		u32 bytes = block->nops * sizeof(u16);
		while (bytes >= 8)
		{
			u64 a, b;
			std::memcpy(&a, code, 8);
			std::memcpy(&b, guest, 8);
			if (a != b)
				return false;
			code += 8;
			guest += 8;
			bytes -= 8;
		}
		if (bytes >= 4)
		{
			u32 a, b;
			std::memcpy(&a, code, 4);
			std::memcpy(&b, guest, 4);
			if (a != b)
				return false;
			code += 4;
			guest += 4;
			bytes -= 4;
		}
		if (bytes >= 2)
		{
			u16 a, b;
			std::memcpy(&a, code, 2);
			std::memcpy(&b, guest, 2);
			return a == b;
		}
		return true;
	}

	// A pure self-loop that leaves registers and T unchanged will spin until
	// something outside the SH-4 changes memory, which only happens at the next
	// timeslice boundary. Skip straight there (at most SH4_TIMESLICE cycles).
	void runIdleCandidate(const SpAotBlock *block)
	{
		u32 before[16];
		std::memcpy(before, ctx->r, sizeof(before));
		const u32 t = ctx->sr.T;
		block->fn(ctx);
		if (ctx->pc == block->pc && ctx->sr.T == t && std::memcmp(before, ctx->r, sizeof(before)) == 0
				&& ctx->cycle_counter > 0 && idleSkipEnabled)
		{
			idleSkipped += ctx->cycle_counter;
			ctx->cycle_counter = 0;
		}
	}

	void runBlock()
	{
		const u32 pc = ctx->pc;
		RtBlock *head = nullptr;
		if (prev != nullptr)
		{
			if (prev->succPc[0] == pc && prev->succ[0] != nullptr)
				head = prev->succ[0];
			else if (prev->succPc[1] == pc && prev->succ[1] != nullptr)
				head = prev->succ[1];
			if (head != nullptr)
				chainedRun++;
		}
		if (head == nullptr)
		{
			head = tableLookup(pc);
			if (head != nullptr && prev != nullptr)
			{
				const int slot = prev->nextSlot;
				prev->succ[slot] = head;
				prev->succPc[slot] = pc;
				prev->nextSlot = u8(slot ^ 1);
			}
		}
		if (head != nullptr)
		{
			const u8 *guest = &mem_b[pc & RAM_MASK];
			for (RtBlock *rt = head; rt != nullptr; rt = rt->variant)
			{
				const SpAotBlock *block = rt->block;
				if (block->pc != pc || !codeMatches(block, guest))
					continue;
				prev = rt;
				aotOps += block->nops;
				blocksRun++;
				ctx->cycle_counter -= block->cycles;
				if (block->flags & kSpPureSelfLoop)
				{
					runIdleCandidate(block);
					return;
				}
				block->fn(ctx);
				return;
			}
		}
		prev = nullptr;
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

	std::vector<RtBlock> runtime;
	std::vector<std::unique_ptr<RtBlock *[]>> pages;
	RtBlock *prev = nullptr;
	std::unordered_map<u32, u64> missCounts;
	std::unordered_set<std::string> knownShapes;
	std::vector<Traced> traced;
	std::vector<ScriptEvent> script;
	u32 scriptMask = 0;
	std::string stateLoadPath;
	std::string tracePath;
	bool traceDirty = false;
	bool aotEnabled = true;
	bool statsEnabled = false;
	u64 aotOps = 0;
	u64 interpretedOps = 0;
	u64 slices = 0;
	u64 blocksRun = 0;
	u64 chainedRun = 0;
	u64 exitAtSlices = 0;
	u64 slicesAtWallStart = 0;
	bool uncapped = false;
	bool wallStarted = false;
	bool selfTestDone = false;
	bool idleSkipEnabled = true;
	u64 idleSkipped = 0;
	double cpuBase = 0;
	double cpuAccumulated = 0;
	std::chrono::steady_clock::time_point wallStart;
	std::chrono::steady_clock::time_point lastReport;
};

} // namespace

Sh4Executor *SonicPad_CreateSh4Executor()
{
	return new SonicPadExecutor();
}
