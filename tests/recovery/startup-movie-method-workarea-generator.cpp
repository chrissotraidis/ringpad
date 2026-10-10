// Emit only the reviewed original MOVIE work-area initializer or three-word record clear.
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
 const bool wrapper=elf.entry==0x8CED78EA;
 unsigned instructions=wrapper?53:elf.entry==0x8CED79A4?7:0,externals=wrapper?4:0;
 if(!instructions)throw std::runtime_error("Unreviewed body");
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=0x8CEB0000;section.size=elf.bytes.size();elf.sections.push_back(section);
 ElfSymbol s{};s.name="fn_"+std::to_string(elf.entry);s.value=elf.entry;s.size=section.size-(s.value-section.address);s.info=0x12;s.section_index=0;elf.symbols.push_back(s);
 ProgramAnalysisOptions options{};options.max_functions=1;options.native_override_symbols.clear();
 auto p=analyze_reachable_program(elf,s.name,options);
 std::cout<<"functions="<<p.functions.size()<<" instructions="<<p.functions[0].analysis.instructions.size()<<" unknown="<<p.functions[0].analysis.unknown<<" call_sites="<<p.external_calls.size()<<"\n";
 if(p.functions.size()!=1||p.functions[0].analysis.instructions.size()!=instructions||p.functions[0].analysis.unknown||p.external_calls.size()!=externals)throw std::runtime_error("Bounded body differs from original review");
 const auto emitted=emit_cpp_program(elf,p,{argv[2],false,false,false});
 for(const auto&f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 const auto cfg=build_cfg(p.functions[0].analysis);if(cfg.blocks.size()!=(wrapper?7:2))throw std::runtime_error("CFG entries differ");
 const std::string name=wrapper?"movie-method-workarea":"movie-method-record",symbol=wrapper?"movie_recomp_8CED78EA":"movie_recomp_8CED79A4",entries=wrapper?"movie_method_workarea_entries":"movie_method_record_entries";
 std::ofstream manifest(std::filesystem::path(argv[2])/(name+"-manifest.hpp"));
 manifest<<"#pragma once\n#include \"dc_runtime.hpp\"\nnamespace dcrecomp_generated {\nvoid "<<symbol<<"(SH4Context&,DCRuntime&);\ninline const CodeModuleEntry "<<entries<<"[]={\n";
 for(const auto&b:cfg.blocks)manifest<<"{0x"<<std::hex<<b.start_address<<"u,&"<<symbol<<"},\n";
 manifest<<"};\n}\n";if(!manifest)throw std::runtime_error("Cannot write movie manifest");
 std::cout<<"activation_manifest_entries="<<std::dec<<cfg.blocks.size()<<"\n";
 std::cout<<"one_reviewed_original_body=1 cap=1; following ED6840 helper remains explicit missing; no further reachability expansion\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
