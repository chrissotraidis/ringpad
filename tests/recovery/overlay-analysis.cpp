// Source-free examples for code replacement and PC-relative literal lifetime.
#include "dcrecomp/cpp_emitter.hpp"
#include <iostream>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char** argv){
 try{
    if(argc!=2)throw std::runtime_error("usage: overlay-analysis OUTPUT");
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C900000;elf.bytes.resize(0x8C);
    auto word=[&](unsigned at,unsigned value){elf.bytes.at(at)=value&255;elf.bytes.at(at+1)=value>>8;};
    for(auto at:{0u,0x40u}){word(at,0xE000+(at?22:11));word(at+2,0x000B);word(at+4,0x0009);}
    word(0x80,0xD001);word(0x82,0x000B);word(0x84,0x0009);word(0x88,7);
    ElfSection section{};section.name=".fixture";section.type=1;section.flags=6;
    section.address=elf.entry;section.size=elf.bytes.size();elf.sections.push_back(section);
    for(auto at:{0u,0x40u,0x80u}){ElfSymbol s{};s.name="fn"+std::to_string(at);s.value=elf.entry+at;s.size=at==0x80?12:6;s.info=0x12;elf.symbols.push_back(s);}
    ProgramAnalysisOptions options{};options.native_override_symbols.clear();options.seed_addresses={elf.entry+0x40,elf.entry+0x80};
    const auto program=analyze_reachable_program(elf,"fn0",options);
    const auto result=emit_cpp_program(elf,program,{argv[1],true,true,true});
    if(result.functions.size()!=3)throw std::runtime_error("Wrong fixture closure");
    for(const auto& f:result.functions)if(f.raw_ops)throw std::runtime_error("RAW operation");
    std::cout<<"3 synthetic functions generated\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
