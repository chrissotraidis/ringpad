// Independent result oracle for pinned, synthetic upstream SH-4 ELF fixtures.
// No original game data, Katana interfaces or native function overrides.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace dcrecomp_generated;

static void equal(std::uint32_t actual, std::uint32_t expected, const char* label) {
    if (actual != expected) {
        throw std::runtime_error(std::string(label) + ": expected " +
                                 std::to_string(expected) + ", got " + std::to_string(actual));
    }
}

static SH4Context execute(DCRuntime& runtime, std::uint32_t argument = 0) {
    SH4Context ctx{};
    ctx.pc = 0x8C010000u;
    ctx.pr = 0xFFFFFFFFu;
    ctx.r[15] = 0x8CFFF000u;
    ctx.r[4] = argument;
    call_recompiled(ctx, runtime, ctx.pc);
    equal(ctx.pc, 0xFFFFFFFFu, "return PC");
    equal(ctx.r[15], 0x8CFFF000u, "restored stack");
    return ctx;
}

#if RECOVERY_FIXTURE == 4
static SH4Context check_gbr_stores(DCRuntime& runtime, std::uint32_t base, std::uint32_t value) {
    SH4Context ctx{};
    for (std::size_t n = 0; n < ctx.r.size(); ++n) ctx.r[n] = 0x12340000u + n;
    ctx.r[0] = value;
    ctx.r[15] = 0x8CFFF000u;
    ctx.gbr = base;
    ctx.sr = 1u; // Store must preserve T and other architectural registers.
    ctx.pr = 0xFFFFFFFFu;
    ctx.pc = 0x8C010000u;
    const auto before = ctx;
    for (std::uint32_t off = 0; off < 1032u; off += 4u)
        dc_write32(runtime, base - 4u + off, 0xDEADBEEFu);
    call_recompiled(ctx, runtime, ctx.pc);
    for (std::uint32_t displacement = 0; displacement < 256u; ++displacement)
        equal(dc_read32(runtime, base + 4u * displacement), value, "GBR disp8 store");
    equal(dc_read32(runtime, base - 4u), 0xDEADBEEFu, "guard before GBR page");
    equal(dc_read32(runtime, base + 1024u), 0xDEADBEEFu, "guard after GBR page");
    for (std::size_t n = 0; n < ctx.r.size(); ++n) equal(ctx.r[n], before.r[n], "preserved GPR");
    equal(ctx.gbr, before.gbr, "preserved GBR");
    equal(ctx.sr, before.sr, "preserved SR");
    equal(ctx.pr, before.pr, "preserved PR");
    equal(ctx.pc, before.pr, "return PC");
    return ctx;
}
#endif

#if RECOVERY_FIXTURE == 6
// https://www.renesas.com/en/document/mas/sh-4a-software-manual
static SH4Context check_gbr_loads(DCRuntime& runtime, std::uint32_t base,
                                std::uint32_t output, std::uint32_t seed) {
    SH4Context ctx{};
    for (std::size_t n = 0; n < ctx.r.size(); ++n) ctx.r[n] = 0x12340000u + n;
    ctx.r[1] = output;
    ctx.r[15] = 0x8CFFF000u;
    ctx.gbr = base;
    ctx.sr = seed & 1u;
    ctx.pr = 0xFFFFFFFFu;
    ctx.pc = 0x8C010000u;
    const auto before = ctx;
    const auto value = [seed](std::uint32_t d) { return seed ^ (d * 0x01010101u); };
    for (auto page : {base, output}) {
        dc_write32(runtime, page - 4u, 0xDEADBEEFu);
        dc_write32(runtime, page + 1024u, 0xDEADBEEFu);
    }
    for (std::uint32_t d = 0; d < 256u; ++d) {
        dc_write32(runtime, base + 4u * d, value(d));
        dc_write32(runtime, output + 4u * d, 0xDEADBEEFu);
    }
    call_recompiled(ctx, runtime, ctx.pc);
    for (std::uint32_t d = 0; d < 256u; ++d) {
        equal(dc_read32(runtime, output + 4u * d), value(d), "GBR disp8 loaded word");
        equal(dc_read32(runtime, base + 4u * d), value(d), "preserved source word");
    }
    for (auto page : {base, output}) {
        equal(dc_read32(runtime, page - 4u), 0xDEADBEEFu, "guard before page");
        equal(dc_read32(runtime, page + 1024u), 0xDEADBEEFu, "guard after page");
    }
    equal(ctx.r[0], value(255u), "last loaded word");
    equal(ctx.r[1], output + 1024u, "output cursor");
    for (std::size_t n = 2; n < ctx.r.size(); ++n) equal(ctx.r[n], before.r[n], "preserved GPR");
    equal(ctx.gbr, before.gbr, "preserved GBR");
    equal(ctx.sr, before.sr, "preserved SR");
    equal(ctx.pr, before.pr, "preserved PR");
    equal(ctx.pc, before.pr, "return PC");
    return ctx;
}
#endif

int main() {
    try {
        auto runtime = std::make_unique<DCRuntime>();
        load_embedded_elf_image(*runtime);
        register_recompiled_program(*runtime);
#if RECOVERY_FIXTURE == 4
        auto ctx = check_gbr_stores(*runtime, 0x8C030000u, 0x87654321u);
        ctx = check_gbr_stores(*runtime, 0x8C040000u, 0x12345678u);
        std::cout << "PASS GBR stores: all 256 displacements, two bases/values, guards and registers preserved\n";
#elif RECOVERY_FIXTURE == 6
        auto ctx = check_gbr_loads(*runtime, 0x8C030000u, 0x8C050000u, 0x87654321u);
        ctx = check_gbr_loads(*runtime, 0x8C040000u, 0x8C060000u, 0x12345678u);
        std::cout << "PASS GBR loads: all 256 displacements, two bases/patterns, guards and registers preserved\n";
#else
        auto ctx = execute(*runtime);
#endif
#if RECOVERY_FIXTURE == 1
        // DT/BF sums 5+4+3+2+1; compare/branch selects success value 42.
        equal(ctx.r[0], 42u, "branch result");
        std::cout << "PASS branch loop: R0=42, return PC and stack restored\n";
#elif RECOVERY_FIXTURE == 2
        equal(ctx.r[0], 150u, "sum 10+20+30+40+50");
        equal(dc_read32(*runtime, 0x8C030000u), 150u, "scratch sum");
        // Re-run the same native code with different data, including overflow.
        constexpr std::array<std::uint32_t, 5> inputs{0xFFFFFFFFu, 2u, 0u, 0x80000000u, 7u};
        for (std::size_t i = 0; i < inputs.size(); ++i)
            dc_write32(*runtime, 0x8C020000u + 4u * i, inputs[i]);
        ctx = execute(*runtime);
        equal(ctx.r[0], 0x80000008u, "changed inputs with modular sum");
        equal(dc_read32(*runtime, 0x8C030000u), 0x80000008u, "changed scratch sum");
        std::cout << "PASS memory: original sum=150, changed-input sum=0x80000008\n";
#elif RECOVERY_FIXTURE == 3
        equal(ctx.r[0], 42u, "guest FPU comparisons");
        constexpr std::array<std::uint32_t, 5> expected{
            0x40E00000u, 0x3F800000u, 0x41400000u, 0x40800000u, 0x41200000u};
        for (std::size_t i = 0; i < expected.size(); ++i)
            equal(dc_read32(*runtime, 0x8C020018u + 4u * i), expected[i], "float output bits");
        equal(dc_read32(*runtime, 0x8C020030u), 0u, "double 3.5 low bits");
        equal(dc_read32(*runtime, 0x8C020034u), 0x400C0000u, "double 3.5 high bits");
        equal(ctx.fpscr, 0u, "restored FPSCR");
        std::cout << "PASS FPU: float results 7,1,12,4,10; double result 3.5; R0=42\n";
#elif RECOVERY_FIXTURE == 5
        equal(ctx.r[0], 42u, "branch-selected callback A");
        ctx = execute(*runtime, 1u);
        equal(ctx.r[0], 0u, "other path skips callback");
        equal(ctx.r[9], 6u, "other path retains non-call value");
        dc_write32(*runtime, 0x8C010020u, 0x8C010046u);
        ctx = execute(*runtime);
        equal(ctx.r[0], 84u, "changed callback pointer B");
        std::cout << "PASS indirect call: both branch paths and changed callback pointer\n";
#elif RECOVERY_FIXTURE != 4 && RECOVERY_FIXTURE != 6
#error Unknown recovery fixture
#endif
        bool rejected = false;
        try {
            call_recompiled(ctx, *runtime, 0x8C700000u);
        } catch (const std::runtime_error& error) {
            rejected = std::string(error.what()).find("No recompiled/native target registered") != std::string::npos;
        }
        if (!rejected) throw std::runtime_error("Unregistered target did not fail explicitly");
        std::cout << "PASS missing-target negative control: explicit failure\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
