// Original console language reader, region fallback and retained settings dependencies.
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
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C6030DC;
    elf.bytes.assign(std::istreambuf_iterator<char>(input),{});
    ElfSection section{};section.name="resident";section.type=1;section.flags=6;
    section.address=0x8C010000;section.size=elf.bytes.size();elf.sections.push_back(section);
    ProgramAnalysisOptions options{};options.max_functions=32;options.native_override_symbols.clear();
    for(unsigned address:{0x8C60305Eu,0x8C64FB1Cu,0x8C607D84u,0x8C607E88u,
        0x8C10F65Cu,0x8C10F670u,0x8C10F698u,0x8C10F684u,0x8C10DD18u,
        0x8C60792Cu,0x8C607964u,0x8C607978u,0x8C607A62u,0x8C60798Cu,
        0x8C607948u,0x8C64FAE0u,0x8C607F14u,0x8C607F90u,0x8C607CCCu,0x8C60300Cu,0x8C6030DCu,0x8C60313Au,0x8C6030D6u,
        0x8C6031D4u,0x8C6078C4u,0x8C10DCF0u,0x8C6002CAu,0x8C600514u}){
        ElfSymbol symbol{};symbol.name="fn_"+std::to_string(address);symbol.value=address;
        symbol.size=section.size-(address-section.address);symbol.info=0x12;symbol.section_index=0;
        elf.symbols.push_back(symbol);options.seed_addresses.push_back(address);
    }
    const auto p=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
    if(p.functions.size()>=options.max_functions)throw std::runtime_error("Language function bound exhausted");
    unsigned instructions=0;
    for(const auto& f:p.functions){
        if(f.analysis.unknown)throw std::runtime_error("Unknown language instruction");
        instructions+=f.analysis.instructions.size();
    }
    const auto output=emit_cpp_program(elf,p,{argv[2],true,true,true});
    for(const auto& f:output.functions)if(f.raw_ops)throw std::runtime_error("RAW language instruction");
    std::cout<<"functions="<<p.functions.size()<<" instructions="<<instructions
        <<" external_records="<<p.external_calls.size()<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
