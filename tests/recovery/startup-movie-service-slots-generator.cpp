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
 unsigned instructions=0,externals=0,blocks=0;std::string name,symbol,entries;
 switch(elf.entry){
 case 0x8CED1C18:instructions=15;externals=1;blocks=2;name="movie-service-slots-enable";symbol="movie_recomp_8CED1C18";entries="movie_service_slots_enable_entries";break;
 case 0x8CEDA244:instructions=8;externals=0;blocks=1;name="movie-service-slots-set";symbol="movie_recomp_8CEDA244";entries="movie_service_slots_set_entries";break;
 case 0x8CED1CDA:instructions=24;externals=4;blocks=7;name="movie-service-slots-handler";symbol="movie_recomp_8CED1CDA";entries="movie_service_slots_handler_entries";break;
 case 0x8CED2164:instructions=35;externals=3;blocks=4;name="movie-service-slots-status";symbol="movie_recomp_8CED2164";entries="movie_service_slots_status_entries";break;
 case 0x8CEDA274:instructions=8;externals=0;blocks=1;name="movie-service-slots-get";symbol="movie_recomp_8CEDA274";entries="movie_service_slots_get_entries";break;
 case 0x8CED1D0A:instructions=47;externals=3;blocks=10;name="movie-service-slots-packet";symbol="movie_recomp_8CED1D0A";entries="movie_service_slots_packet_entries";break;
 default:throw std::runtime_error("Unreviewed body");}
 elf.bytes.assign(std::istreambuf_iterator<char>(f),{});
 ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;section.address=elf.entry>=0x8CEB0000?0x8CEB0000:0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
 ElfSymbol s{};s.name="fn_"+std::to_string(elf.entry);s.value=elf.entry;s.size=section.size-(s.value-section.address);s.info=0x12;s.section_index=0;elf.symbols.push_back(s);
 ProgramAnalysisOptions options{};options.max_functions=1;options.native_override_symbols.clear();
 auto p=analyze_reachable_program(elf,s.name,options);
 std::cout<<"functions="<<p.functions.size()<<" instructions="<<p.functions[0].analysis.instructions.size()<<" unknown="<<p.functions[0].analysis.unknown<<" call_sites="<<p.external_calls.size()<<"\n";
 if(p.functions.size()!=1||p.functions[0].analysis.instructions.size()!=instructions||p.functions[0].analysis.unknown||p.external_calls.size()!=externals)throw std::runtime_error("Bounded body differs from original review");
 const auto emitted=emit_cpp_program(elf,p,{argv[2],false,false,false});
 for(const auto&f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW instruction");
 const auto cfg=build_cfg(p.functions[0].analysis);if(cfg.blocks.size()!=blocks)throw std::runtime_error("CFG entries differ");

 std::ofstream manifest(std::filesystem::path(argv[2])/(name+"-manifest.hpp"));
 manifest<<"#pragma once\n#include \"dc_runtime.hpp\"\nnamespace dcrecomp_generated {\nvoid "<<symbol<<"(SH4Context&,DCRuntime&);\ninline const CodeModuleEntry "<<entries<<"[]={\n";
 for(const auto&b:cfg.blocks)manifest<<"{0x"<<std::hex<<b.start_address<<"u,&"<<symbol<<"},\n";
 manifest<<"};\n}\n";if(!manifest)throw std::runtime_error("Cannot write movie manifest");
 std::cout<<"activation_manifest_entries="<<std::dec<<cfg.blocks.size()<<"\n";
 std::cout<<"one_reviewed_original_body=1 cap=1; prefix reaches first real missing callee; no further reachability expansion\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
