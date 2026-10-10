// Extract exact private routines/metadata; no replacement guest instructions.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
static std::vector<std::uint8_t> read(const char* path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open private input");
    return {std::istreambuf_iterator<char>(f), {}};
}
int main(int argc, char** argv) {
    try {
        if (argc != 4) throw std::runtime_error("usage: generator BOOT SBOARD OUTPUT");
        const auto boot = read(argv[1]), board = read(argv[2]);
        Elf32Image elf{}; elf.type = 2; elf.machine = 42; elf.entry = 0x8C900040;
        auto section = [&](const auto& data, std::uint32_t base, std::uint32_t start,
                           std::uint32_t end, const char* name, bool code) {
            if (start < base || end < start || end - base > data.size())
                throw std::runtime_error("Invalid extraction boundary");
            ElfSection s{}; s.name = name; s.type = 1; s.flags = code ? 6 : 2;
            s.address = start; s.offset = elf.bytes.size(); s.size = end - start;
            elf.bytes.insert(elf.bytes.end(), data.begin() + start - base, data.begin() + end - base);
            elf.sections.push_back(s);
            if (code) {
                ElfSymbol f{}; f.name = name; f.value = start; f.size = end - start;
                f.info = 0x12; f.section_index = elf.sections.size() - 1;
                elf.symbols.push_back(f);
            }
        };
        section(boot, 0x8C010000, 0x8C09A426, 0x8C09A42C, "schedule_cleanup", true);
        section(boot, 0x8C010000, 0x8C09A488, 0x8C09A48C, "cleanup_literal", false);
        section(boot, 0x8C010000, 0x8C09A720, 0x8C09A7AC, "allocate", true);
        section(boot, 0x8C010000, 0x8C09A800, 0x8C09A804, "heap_literal", false);
        section(boot, 0x8C010000, 0x8C0A1690, 0x8C0A16E6, "registration", true);
        section(boot, 0x8C010000, 0x8C0A170C, 0x8C0A1714, "registration_literals", false);
        section(board, 0x8C900000, 0x8C900040, 0x8C900048, "stage_entry", true);
        section(board, 0x8C900000, 0x8C900060, 0x8C900068, "entry_literals", false);
        section(board, 0x8C900000, 0x8C900188, 0x8C9001A0, "metadata", false);
        section(board, 0x8C900000, 0x8C903140, 0x8C903148, "initialize", true);
        ProgramAnalysisOptions options{}; options.max_functions = 8;
        options.native_override_symbols.clear(); options.seed_addresses = {0x8C903140};
        const auto program = analyze_reachable_program(elf, "stage_entry", options);
        if (program.functions.size() != 5) throw std::runtime_error("Unexpected routine closure");
        unsigned instructions = 0;
        for (const auto& f : program.functions) {
            if (f.analysis.unknown) throw std::runtime_error("Unknown instruction in selected routine");
            instructions += f.analysis.instructions.size();
        }
        const auto result = emit_cpp_program(elf, program, {argv[3], true, true, true});
        for (const auto& f : result.functions)
            if (f.raw_ops) throw std::runtime_error("RAW_SH4 emitted");
        std::cout << "{\"functions\":" << program.functions.size()
                  << ",\"instructions\":" << instructions << ",\"embedded_bytes\":" << result.embedded_bytes
                  << ",\"external_records\":" << program.external_calls.size() << "}\n";
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
