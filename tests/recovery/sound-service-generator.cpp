// Original sound service through packet packing and the explicit DMA boundary.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char**argv){try{
    if(argc!=3)throw std::runtime_error("usage: generator BOOT OUTPUT");
    std::ifstream input(argv[1],std::ios::binary);
    if(!input)throw std::runtime_error("Missing original BOOT");
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C65E0B8;
    elf.bytes.assign(std::istreambuf_iterator<char>(input),{});
    ElfSection section{};section.name="resident";section.type=1;section.flags=6;
    section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
    ProgramAnalysisOptions options{};options.max_functions=24;options.native_override_symbols.clear();
    for(unsigned address:{0x8C65E0B8u,0x8C66AF04u,0x8C66A8E8u,0x8C66A7A8u,
        0x8C65E9ECu,0x8C10EE7Cu,0x8C66CE4Cu,0x8C10F080u,
        0x8C110ABCu,0x8C110B00u,0x8C110A40u,0x8C111680u,0x8C111768u,0x8C1115DCu,0x8C111E10u}){
        ElfSymbol symbol{};symbol.name="fn_"+std::to_string(address);symbol.value=address;
        symbol.size=section.size-(address-section.address);symbol.info=0x12;symbol.section_index=0;
        elf.symbols.push_back(symbol);options.seed_addresses.push_back(address);
    }
    const auto p=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
    if(p.functions.size()>=options.max_functions)throw std::runtime_error("Sound service function bound exhausted");
    unsigned instructions=0;
    for(const auto& f:p.functions){
        if(f.analysis.start_address==0x8C647364 || f.analysis.start_address==0x8C65E31C)throw std::runtime_error("DMA boundary unexpectedly compiled");
        if(f.analysis.unknown)throw std::runtime_error("Unknown sound service instruction");
        instructions+=f.analysis.instructions.size();
    }
    const auto output=emit_cpp_program(elf,p,{argv[2],true,true,true});
    for(const auto& f:output.functions)if(f.raw_ops)throw std::runtime_error("RAW sound service instruction");
    std::cout<<"functions="<<p.functions.size()<<" instructions="<<instructions
        <<" external_records="<<p.external_calls.size()<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
