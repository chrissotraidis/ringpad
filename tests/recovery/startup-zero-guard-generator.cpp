// One original body, linked to the verified existing provider/callback catalog.
// max_functions=1 deliberately bounds emission; this is not a complete closure claim.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
 if(argc!=3)throw std::runtime_error("usage: generator BOOT OUTPUT");
 std::ifstream f(argv[1],std::ios::binary);if(!f)throw std::runtime_error("Missing BOOT");
 Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C04562E;
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
 for(unsigned a:{0x8C04562Eu}){ElfSymbol s{};s.name="fn_"+std::to_string(a);s.value=a;s.size=section.size-(a-section.address);s.info=0x12;s.section_index=0;elf.symbols.push_back(s);}
 ProgramAnalysisOptions options{};options.max_functions=1;options.native_override_symbols.clear();
 auto program=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
 if(program.functions.size()!=1||program.functions[0].analysis.start_address!=elf.entry||program.functions[0].analysis.instructions.size()!=38||program.functions[0].analysis.unknown)throw std::runtime_error("One-body source review differs");
 if(program.external_calls.size()!=3)throw std::runtime_error("Existing catalog boundary count differs");
 auto result=emit_cpp_program(elf,program,{argv[2],true,true,true});
 for(const auto& f:result.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 std::cout<<"one_original_body=1 instructions=38 children_from_cached_catalog=true; no full closure claim\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
