// Source-free counterexample: the R9=6 path never reaches JSR @R9.
// Reports discovery accuracy separately from generated execution correctness.
#include "dcrecomp/cpp_emitter.hpp"
#include <iostream>
#include <optional>
#include <stdexcept>

using namespace dcrecomp;
int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("usage: indirect-analysis OUTPUT_DIR");
        constexpr std::uint32_t base = 0x8C010000u;
        Elf32Image elf{};
        elf.machine = 42;
        elf.type = 2;
        elf.entry = base;
        elf.bytes.resize(0x4C);
        auto word = [&](unsigned offset, std::uint16_t value) {
            elf.bytes.at(offset) = value & 255;
            elf.bytes.at(offset + 1) = value >> 8;
        };
        word(0x00, 0x4F22); // sts.l pr,@-r15
        word(0x02, 0xD907); // mov.l literal at +0x20,r9
        word(0x04, 0x2448); // tst r4,r4
        word(0x06, 0x8904); // bt +0x12: only this path calls
        word(0x08, 0xE906); // mov #6,r9: other path
        word(0x0A, 0xA005); // bra +0x18: skips the call
        word(0x0C, 0x0009); // delay slot
        word(0x0E, 0x0009); // reachable in loop/straight-line variants
        word(0x10, 0x0009);
        word(0x12, 0x490B); // jsr @r9 (must use the literal)
        word(0x14, 0x0009); // delay slot
        word(0x16, 0x0009);
        word(0x18, 0x4F26); // lds.l @r15+,pr
        word(0x1A, 0x000B); // rts
        word(0x1C, 0x0009); // delay slot
        word(0x20, 0x0040); word(0x22, 0x8C01); // callback A
        word(0x40, 0xE02A); // callback A: r0=42
        word(0x42, 0x000B); word(0x44, 0x0009);
        word(0x46, 0xE054); // callback B: r0=84
        word(0x48, 0x000B); word(0x4A, 0x0009);
        ElfSection section{};
        section.name = ".text"; section.type = 1; section.flags = 6;
        section.address = base; section.size = elf.bytes.size();
        elf.sections.push_back(section);
        auto symbol = [&](const char* name, unsigned offset, unsigned size) {
            ElfSymbol s{};
            s.name = name; s.value = base + offset; s.size = size; s.info = 0x12;
            elf.symbols.push_back(s);
        };
        symbol("entry", 0, 0x24);
        symbol("callback_a", 0x40, 6);
        symbol("callback_b", 0x46, 6);
        const auto analysis = analyze_function(elf, "entry");
        if (analysis.unknown || analysis.calls.size() != 1)
            throw std::runtime_error("Unexpected fixture discovery");
        const auto& call = analysis.calls.front();
        const bool false_target = call.resolved && call.target != base + 0x40;
        if (!call.resolved || false_target)
            throw std::runtime_error("Mutually exclusive path contaminated call target");
        if (analysis.cfg_call_candidates.size() != 1 ||
            analysis.cfg_call_candidates[0].targets != std::vector<std::uint32_t>{base + 0x40})
            throw std::runtime_error("Impossible path leaked into CFG candidates");
        unsigned controls = 0;
        auto check = [&](const char* label, Elf32Image variant,
                         std::initializer_list<std::pair<unsigned, std::uint16_t>> changes,
                         std::optional<std::uint32_t> expected, bool keep_pointer_candidate = false) {
            for (const auto& [offset, value] : changes) {
                variant.bytes.at(offset) = value & 255;
                variant.bytes.at(offset + 1) = value >> 8;
            }
            const auto result = analyze_function(variant, "entry");
            if (result.unknown) throw std::runtime_error(std::string(label) + ": unknown instruction");
            if (keep_pointer_candidate) {
                bool found = false;
                for (const auto& ref : result.cfg_call_candidates)
                    for (const auto target : ref.targets)
                        if (ref.instruction_address == base + 0x12 && target == base + 0x40) found = true;
                if (!found) throw std::runtime_error(std::string(label) + ": lost candidate");
            }
            for (const auto& candidate : result.calls) {
                if (candidate.instruction_address != base + 0x12) continue;
                if (candidate.resolved != expected.has_value() ||
                    (expected && candidate.target != *expected))
                    throw std::runtime_error(std::string(label) + ": incorrect resolution");
                ++controls;
                return;
            }
            throw std::runtime_error(std::string(label) + ": call disappeared");
        };
        const auto pointer = base + 0x40;
        check("conflicting join", elf, {{0x0A, 0xA002}}, std::nullopt);
        check("equal join", elf, {{0x08, 0x0009}, {0x0A, 0xA002}}, pointer);
        check("unknown join", elf, {{0x08, 0x6943}, {0x0A, 0xA002}}, std::nullopt);
        check("unchanged pointer across loop", elf,
              {{4, 0xE402}, {6, 0x4410}, {8, 0x8BFD}, {10, 0x0009}}, pointer);
        check("changed pointer across loop", elf,
              {{4, 0xE402}, {6, 0x7901}, {8, 0x4410}, {10, 0x8BFC}}, std::nullopt);
        check("target sampled before delay slot", elf, {{0x14, 0xE906}}, pointer);
        auto straight = elf;
        for (unsigned offset = 4; offset < 0x12; offset += 2) {
            straight.bytes[offset] = 9; straight.bytes[offset + 1] = 0;
        }
        check("straight line", straight, {}, pointer);
        check("GBR load invalidates old R0 call target", straight,
              {{2, 0xD007}, {8, 0xC600}, {0x12, 0x400B}}, std::nullopt);
        check("callee may clobber registers", straight, {{4, 0x420B}}, std::nullopt);
        check("FPU store predecrement", straight, {{8, 0xF90B}}, std::nullopt);
        check("special register load postincrement", straight, {{8, 0x4926}}, std::nullopt);
        check("unknown jump on separate path", elf,
              {{8, 0x6943}, {10, 0x492B}}, std::nullopt, true);
        check("first iteration candidate survives callee clobber", straight,
              {{4, 0xE402}, {0x16, 0x4410}, {0x18, 0x8BFB}, {0x1A, 0x4F26},
               {0x1C, 0x000B}, {0x1E, 0x0009}}, std::nullopt, true);
        std::cout << "{\"static_false_target\":" << (false_target ? "true" : "false")
                  << ",\"resolved\":" << (call.resolved ? "true" : "false")
                  << ",\"inferred_target\":" << call.target
                  << ",\"expected_initial_target\":" << base + 0x40
                  << ",\"callbacks_explicitly_seeded\":true,\"analysis_controls_passed\":"
                  << controls << "}\n";
        ProgramAnalysisOptions options{};
        options.native_override_symbols.clear();
        // Explicit seeds isolate dispatch semantics from the discovery gap.
        options.seed_addresses = {base + 0x40, base + 0x46};
        const auto program = analyze_reachable_program(elf, "entry", options);
        bool dynamic = false;
        for (const auto& f : program.functions)
            for (const auto& b : f.ir.blocks)
                for (const auto& i : b.instructions)
                    if (i.source_address == base + 0x12 && i.op == DCIROp::DynamicCall)
                        dynamic = true;
        if (!dynamic) throw std::runtime_error("Indirect call lost dynamic semantics");
        CppEmitOptions emit{};
        emit.output_directory = argv[1];
        emit_cpp_program(elf, program, emit);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
