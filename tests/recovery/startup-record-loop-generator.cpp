// Selected original record-loop bodies; bounded emission, no title closure claim.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
    if(argc!=4)throw std::runtime_error("usage: generator BOOT OUTPUT parent|helpers");
    std::ifstream input(argv[1],std::ios::binary);
    if(!input)throw std::runtime_error("Missing original BOOT");
    const bool parent=std::string(argv[3])=="parent";
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=parent?0x8C10B992:0x8C10B226;
    elf.bytes.assign(std::istreambuf_iterator<char>(input),{});
    ElfSection section{};section.name="resident";section.type=1;section.flags=6;
    section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
    ProgramAnalysisOptions options{};options.max_functions=parent?1:4;options.native_override_symbols.clear();
    for(unsigned address:{0x8C10B992u,0x8C10B226u,0x8C10B906u,0x8C10B1ECu,0x8C10B2E0u}){
        if((address==0x8C10B992)!=parent)continue;
        ElfSymbol symbol{};symbol.name="fn_"+std::to_string(address);symbol.value=address;
        symbol.size=section.size-(address-section.address);symbol.info=0x12;symbol.section_index=0;
        elf.symbols.push_back(symbol);options.seed_addresses.push_back(address);
    }
    const auto p=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
    if(p.functions.size()!=options.max_functions)throw std::runtime_error("Selected original body inventory differs");
    unsigned instructions=0;
    for(const auto& f:p.functions){
        if(f.analysis.unknown)throw std::runtime_error("Unknown peripheral instruction");
        instructions+=f.analysis.instructions.size();
    }
    if(instructions!=(parent?51:710))throw std::runtime_error("Reviewed original instruction count differs");
    const auto output=emit_cpp_program(elf,p,{argv[2],false,false,false});
    for(const auto& f:output.functions)if(f.raw_ops)throw std::runtime_error("RAW peripheral instruction");
    std::cout<<"functions="<<p.functions.size()<<" instructions="<<instructions
        <<" external_records="<<p.external_calls.size()<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
