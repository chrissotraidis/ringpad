// Private, static coverage audit. Reuses the pinned analyzer, never executes SH-4.
#include "dcrecomp/program_analysis.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

using namespace dcrecomp;
static std::string hex(std::uint32_t value) {
    std::ostringstream out;
    out << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
    return out.str();
}
static std::string csv(const std::string& value) {
    std::string out = "\"";
    for (char c : value) out += c == '"' ? "\"\"" : std::string(1, c);
    return out + "\"";
}

int main(int argc, char** argv) {
    try {
        if (argc < 5) throw std::runtime_error("usage: coverage BOOT.BIN BASE functions.csv OUTPUT_DIR [--entry=ADDRESS] [inspect-address ...]");
        const auto base = static_cast<std::uint32_t>(std::stoul(argv[2], nullptr, 0));
        auto entry = base;
        for (int n = 5; n < argc; ++n) {
            if (std::string(argv[n]).starts_with("--entry="))
                entry = static_cast<std::uint32_t>(std::stoul(std::string(argv[n]).substr(8), nullptr, 0));
        }
        std::ifstream input(argv[1], std::ios::binary), map(argv[3]);
        if (!input || !map) throw std::runtime_error("Cannot open input/map");
        Elf32Image elf{};
        elf.machine = 42;
        elf.type = 2;
        elf.entry = entry;
        elf.bytes.assign(std::istreambuf_iterator<char>(input), {});
        if (elf.bytes.empty() || elf.bytes.size() > 0x1000000u) throw std::runtime_error("Invalid image size");
        ElfSection section{};
        section.name = ".raw_boot";
        section.type = 1;
        section.flags = 6;
        section.address = base;
        section.size = elf.bytes.size();
        elf.sections.push_back(section);
        ProgramAnalysisOptions options{};
        options.max_functions = 16384;
        options.native_override_symbols.clear();
        std::string line, root;
        std::getline(map, line); // CSV header
        while (std::getline(map, line)) {
            const auto comma = line.find(',');
            const auto address = static_cast<std::uint32_t>(std::stoul(line.substr(0, comma), nullptr, 0));
            if (address < base || address - base >= elf.bytes.size()) throw std::runtime_error("Map address outside image");
            ElfSymbol symbol{};
            symbol.name = "sub_" + hex(address).substr(2);
            symbol.value = address;
            symbol.size = elf.bytes.size() - (address - base);
            symbol.info = 0x12;
            if (address == entry) root = symbol.name;
            else options.seed_addresses.push_back(address);
            elf.symbols.push_back(symbol);
        }
        if (root.empty()) throw std::runtime_error("Map has no root entry");
        // Focused diagnosis uses exactly the same image/symbol construction as
        // the whole-program audit. This emits text only, never game source.
        std::string focused;
        for (int n = 5; n < argc; ++n)
            if (std::string(argv[n]).starts_with("--function=")) focused = std::string(argv[n]).substr(11);
        if (!focused.empty()) {
            const auto address = static_cast<std::uint32_t>(std::stoul(focused, nullptr, 0));
            const auto f = analyze_function_at(elf, address);
            const auto cfg = build_cfg(f);
            const std::filesystem::path out = argv[4];
            std::filesystem::create_directories(out);
            std::ofstream detail(out / "function.txt");
            if (!detail) throw std::runtime_error("Cannot create function report");
            detail << "ENTRY " << hex(f.start_address) << " END " << hex(f.end_address) << '\n';
            for (const auto& b : cfg.blocks) {
                detail << "BLOCK " << hex(b.start_address) << '\n';
                for (const auto& i : b.instructions)
                    detail << hex(i.address) << ' ' << sh4::to_string(i)
                           << " delay=" << i.has_delay_slot << '\n';
            }
            for (const auto& e : cfg.edges)
                detail << "EDGE " << hex(e.from) << ' ' << hex(e.to) << ' ' << to_string(e.kind) << '\n';
            for (const auto& c : f.calls)
                detail << "CALL " << hex(c.instruction_address) << ' ' << hex(c.target) << " resolved=" << c.resolved << '\n';
            for (const auto& d : f.dynamic_branches)
                for (const auto target : d.targets)
                    detail << "DYNAMIC " << hex(d.instruction_address) << ' ' << hex(target) << '\n';
            for (const auto& d : f.cfg_call_candidates)
                for (const auto target : d.targets)
                    detail << "CANDIDATE " << hex(d.instruction_address) << ' ' << hex(target) << '\n';
            for (const auto& l : f.literals)
                detail << "LITERAL " << hex(l.instruction_address) << ' ' << hex(l.storage_address) << ' ' << hex(l.value) << '\n';
            return 0;
        }
        const auto program = analyze_reachable_program(elf, root, options);
        const std::filesystem::path out = argv[4];
        std::filesystem::create_directories(out);
        std::ofstream calls(out / "external-calls.csv"), instructions(out / "instructions.csv"),
                      sites(out / "inspected-sites.csv"), all_calls(out / "all-calls.csv"),
                      candidates(out / "cfg-call-candidates.csv");
        if (!calls || !instructions || !sites || !all_calls || !candidates) throw std::runtime_error("Cannot create reports");
        all_calls << "caller,site,target,resolved,direct\n";
        candidates << "caller,site,target\n";
        calls << "caller,site,target,resolved,reason\n";
        for (const auto& c : program.external_calls)
            calls << hex(c.caller) << ',' << hex(c.instruction_address) << ',' << hex(c.target)
                  << ',' << c.resolved << ',' << csv(c.reason) << '\n';
        std::map<std::uint32_t, sh4::Instruction> unique;
        std::size_t occurrences = 0;
        for (const auto& f : program.functions) {
            for (const auto& d : f.analysis.cfg_call_candidates)
                for (const auto target : d.targets)
                    candidates << hex(f.analysis.start_address) << ',' << hex(d.instruction_address)
                               << ',' << hex(target) << '\n';
            for (const auto& c : f.analysis.calls)
                all_calls << hex(f.analysis.start_address) << ',' << hex(c.instruction_address)
                          << ',' << hex(c.target) << ',' << c.resolved << ',' << c.direct << '\n';
            occurrences += f.analysis.instructions.size();
            for (const auto& i : f.analysis.instructions) unique.emplace(i.address, i);
        }
        instructions << "address,word,known,assembly\n";
        for (const auto& [address, i] : unique)
            instructions << hex(address) << ',' << hex(i.raw) << ',' << sh4::is_known(i)
                         << ',' << csv(sh4::to_string(i)) << '\n';
        sites << "address,word,known,in_discovered_closure,assembly\n";
        for (int n = 5; n < argc; ++n) {
            if (std::string(argv[n]).starts_with("--entry=")) continue;
            const auto address = static_cast<std::uint32_t>(std::stoul(argv[n], nullptr, 0));
            const auto raw = read_u16_vaddr(elf, address);
            if (!raw) throw std::runtime_error("Inspection address outside image");
            const auto i = sh4::decode(*raw, address);
            sites << hex(address) << ',' << hex(*raw) << ',' << sh4::is_known(i)
                  << ',' << unique.contains(address) << ',' << csv(sh4::to_string(i)) << '\n';
        }
        std::cout << "{\"functions\":" << program.functions.size()
                  << ",\"instruction_occurrences\":" << occurrences
                  << ",\"unique_instruction_addresses\":" << unique.size()
                  << ",\"external_call_records\":" << program.external_calls.size() << "}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
