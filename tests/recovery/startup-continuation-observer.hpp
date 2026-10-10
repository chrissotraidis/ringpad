// Read-only diagnostics for the existing 256-continuation startup experiment.
// Delegates every registered target to its previous function, including observers.
#pragma once
#include <functional>
#include <unordered_map>

static thread_local std::unordered_map<std::uint32_t, RecompiledFunction> progress_originals;
static thread_local std::function<void(const char*, std::uint32_t)> progress_observe;
static void observed_progress(SH4Context& ctx, DCRuntime& rt) {
    const auto target = ctx.pc;
    const auto original = progress_originals.at(target);
    progress_observe("enter", target);
    try { original(ctx, rt); }
    catch (...) { progress_observe("throw", target); throw; }
    progress_observe("leave", target);
}

struct StartupContinuationObserver {
    DCRuntime& rt;
    SH4Context& ctx;
    std::ofstream log;
    unsigned sequence = 0;
    StartupContinuationObserver(DCRuntime& runtime, SH4Context& context, const std::string& path)
        : rt(runtime), ctx(context), log(path) {
        if (!log || !progress_originals.empty()) throw std::runtime_error("Continuation observer setup failed");
        // All addresses are already registered. No new guest implementation is added.
        for (auto target : {0x8C64E6E8u, 0x8C64E720u, 0x8C64E730u, 0x8C64EF00u,
                            0x8C64EF86u, 0x8C64EBA2u, 0x8C64EBDAu, 0x8C64EF76u,
                            0x8C64B350u, 0x8C64B386u, 0x8C6647BEu, 0x8C66486Eu})
            progress_originals.emplace(target, rt.targets.at(target));
        progress_observe = [this](const char* event, std::uint32_t target) { snapshot(event, target); };
        for (const auto& [target, original] : progress_originals) rt.register_target(target, observed_progress);
    }
    ~StartupContinuationObserver() {
        for (const auto& [target, original] : progress_originals) rt.register_target(target, original);
        progress_originals.clear();
        progress_observe = {};
    }
    void snapshot(const char* event, std::uint32_t target) {
        log << std::dec << sequence++ << ' ' << event << std::hex << " target=" << target
            << " pc=" << ctx.pc << " pr=" << ctx.pr << " sr=" << ctx.sr;
        for (unsigned i = 0; i < 16; ++i) log << " r" << std::dec << i << '=' << std::hex << ctx.r[i];
        log << std::dec << " cycles=" << rt.device_clock_sh4_cycles << " sectors=" << rt.gd_sector_reads
            << " bytes=" << rt.gd_bytes_read << " redirect=" << rt.sh4_async_redirect;
        // Only direct reads of the known main-RAM destination: no MMIO reads or guest ticks.
        for (unsigned block = 0; block < 13; ++block) {
            std::uint64_t hash = 14695981039346656037ull;
            for (unsigned i = 0; i < 512; ++i) { hash ^= rt.main_ram[0x6b4734 + block * 512 + i]; hash *= 1099511628211ull; }
            log << " block" << std::dec << block << '=' << std::hex << hash;
        }
        log << std::dec << '\n';
    }
};
