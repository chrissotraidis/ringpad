// Emit the reviewed original MOVIE reviewed setup return leaves.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
 if(argc!=4)throw std::runtime_error("usage: generator IMAGE OUTPUT ADDRESS");
 std::ifstream f(argv[1],std::ios::binary);if(!f)throw std::runtime_error("Missing image");
 Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=std::stoul(argv[3],nullptr,16);
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
 ProgramAnalysisOptions options{};options.max_functions=3;options.native_override_symbols.clear();
 for(unsigned address:{0x8c6624d8u,0x8c6624dcu,0x8c6624e0u}){ElfSymbol s{};s.name="fn_"+std::to_string(address);s.value=address;s.size=4;s.info=0x12;s.section_index=0;elf.symbols.push_back(s);options.seed_addresses.push_back(address);}
 auto p=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
 if(p.functions.size()!=3||!p.external_calls.empty())throw std::runtime_error("Getter program bound differs");
 for(const auto&f:p.functions)if(f.analysis.instructions.size()!=2||f.analysis.unknown||build_cfg(f.analysis).blocks.size()!=1)throw std::runtime_error("Getter body differs");
 const auto emitted=emit_cpp_program(elf,p,{argv[2],false,false,false});
 for(const auto&f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 std::cout<<"functions=3 instructions=6 resident_entries=3;three reviewed getters only\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
