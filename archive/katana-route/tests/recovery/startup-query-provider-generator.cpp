// Original query provider and live table guard; retained original query/gate/copy closure.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
    if(argc!=4)throw std::runtime_error("usage: generator BOOT OUTPUT measure|emit");
    std::ifstream input(argv[1],std::ios::binary);
    if(!input)throw std::runtime_error("Missing original BOOT");
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C606C9C;
    elf.bytes.assign(std::istreambuf_iterator<char>(input),{});
    ElfSection section{};section.name="resident";section.type=1;section.flags=6;
    section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
    ProgramAnalysisOptions options{};options.max_functions=std::string(argv[3])=="measure"?16:18;options.native_override_symbols.clear();
    for(unsigned address:{0x8C089F20u,0x8C10EF3Cu,0x8C10E9FCu,0x8C0A36C0u,0x8C0A36F2u,0x8C0A3724u,0x8C089DC0u,0x8C089E1Eu,0x8C089E34u,0x8C08A394u,0x8C08BD14u,0x8C011238u,0x8C055798u,0x8C055768u,0x8C606C9Cu,0x8C64B9E4u}){
        ElfSymbol symbol{};symbol.name="fn_"+std::to_string(address);symbol.value=address;
        symbol.size=section.size-(address-section.address);symbol.info=0x12;symbol.section_index=0;
        elf.symbols.push_back(symbol);options.seed_addresses.push_back(address);
    }
    const auto p=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
    unsigned instructions=0;
    for(const auto& f:p.functions){
        if(p.functions.size()!=16)throw std::runtime_error("Startup checkquery closure differs");
        if(f.analysis.unknown)throw std::runtime_error("Unknown Startup checkquery prefix instruction");
        instructions+=f.analysis.instructions.size();
    }
    std::cout<<"functions="<<p.functions.size()<<" instructions="<<instructions
        <<" external_records="<<p.external_calls.size()<<" cap="<<options.max_functions<<'\n';
    if(std::string(argv[3])=="measure")return 0;
    if(p.functions.size()>=options.max_functions)throw std::runtime_error("Provider closure cap exhausted");
    const auto output=emit_cpp_program(elf,p,{argv[2],true,true,true});
    for(const auto& f:output.functions)if(f.raw_ops)throw std::runtime_error("RAW provider instruction");
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
