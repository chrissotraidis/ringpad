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
 const bool first=elf.entry==0x8C66D1F8;
 unsigned instructions=first?3:0,externals=1;
 if(!instructions)throw std::runtime_error("Unreviewed body");
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
 ElfSymbol s{};s.name="fn_"+std::to_string(elf.entry);s.value=elf.entry;s.size=section.size-(s.value-section.address);s.info=0x12;s.section_index=0;elf.symbols.push_back(s);
 ProgramAnalysisOptions options{};options.max_functions=1;options.native_override_symbols.clear();
 auto p=analyze_reachable_program(elf,s.name,options);
 std::cout<<"functions="<<p.functions.size()<<" instructions="<<p.functions[0].analysis.instructions.size()<<" unknown="<<p.functions[0].analysis.unknown<<" call_sites="<<p.external_calls.size()<<"\n";
 if(p.functions.size()!=1||p.functions[0].analysis.instructions.size()!=instructions||p.functions[0].analysis.unknown||p.external_calls.size()!=externals)throw std::runtime_error("Bounded body differs from original review");
 const auto emitted=emit_cpp_program(elf,p,{argv[2],false,false,false});
 // Emit runtime support separately from a four-byte synthetic image. This
 // support image is never compiled or executed by the original-game fixture.
 Elf32Image support{};support.type=2;support.machine=42;support.entry=0x8c010000;
 support.bytes={0x0b,0x00,0x09,0x00};
 ElfSection support_section{};support_section.name=".support";support_section.type=1;support_section.flags=6;support_section.address=support.entry;support_section.size=4;support.sections.push_back(support_section);
 ElfSymbol support_symbol{};support_symbol.name="support_only";support_symbol.value=support.entry;support_symbol.size=4;support_symbol.info=0x12;support_symbol.section_index=0;support.symbols.push_back(support_symbol);
 auto support_program=analyze_reachable_program(support,"support_only",options);
 auto support_result=emit_cpp_program(support,support_program,{std::filesystem::path(argv[2])/"runtime-only",true,false,false});
 if(support_result.embedded_bytes>4)throw std::runtime_error("Support image unexpectedly large");
 for(const auto&f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 const auto cfg=build_cfg(p.functions[0].analysis);if(cfg.blocks.size()!=1)throw std::runtime_error("CFG entries differ");
 std::cout<<"resident_entries="<<cfg.blocks.size()<<";original resident body only\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
