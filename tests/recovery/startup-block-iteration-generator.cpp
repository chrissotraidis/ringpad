// Original block iteration only; all real cached request bindings preserved.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
 if(argc!=4)throw std::runtime_error("usage: generator BOOT OUTPUT ADDRESS");
 std::ifstream f(argv[1],std::ios::binary);if(!f)throw std::runtime_error("Missing BOOT");
 Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=std::stoul(argv[3],nullptr,16);
 unsigned instructions=elf.entry==0x8C64EBA2?44:0;
 unsigned externals=1;
 if(!instructions)throw std::runtime_error("Unreviewed body");
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
 ElfSymbol s{};s.name="fn_"+std::to_string(elf.entry);s.value=elf.entry;s.size=section.size-(s.value-section.address);s.info=0x12;s.section_index=0;elf.symbols.push_back(s);
 ProgramAnalysisOptions options{};options.max_functions=1;options.native_override_symbols.clear();
 auto p=analyze_reachable_program(elf,s.name,options);
 if(p.functions.size()!=1||p.functions[0].analysis.instructions.size()!=instructions||p.functions[0].analysis.unknown||p.external_calls.size()!=externals)throw std::runtime_error("Bounded body differs from original review");
 const auto emitted=emit_cpp_program(elf,p,{argv[2],false,false,false});
 for(const auto&f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 std::cout<<"one_reviewed_original_body=1 cap=1; actual helpers/transport remain real or explicitly missing\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
