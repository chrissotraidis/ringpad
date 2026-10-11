// Private original-code closure. Entire owner boot is loaded unchanged.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <set>
using namespace dcrecomp;
int main(int argc,char** argv){
 try{
    if(argc!=5)throw std::runtime_error("usage: generator BOOT MAP SEEDS OUTPUT");
    Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=0x8C603D72;
    std::ifstream in(argv[1],std::ios::binary),map(argv[2]),seed(argv[3]);
    if(!in||!map||!seed)throw std::runtime_error("Input unavailable");
    elf.bytes.assign(std::istreambuf_iterator<char>(in),{});
    constexpr unsigned base=0x8C010000;
    ElfSection section{};section.name=".original_boot";section.type=1;section.flags=6;
    section.address=base;section.size=elf.bytes.size();elf.sections.push_back(section);
    std::string line;std::getline(map,line);std::set<unsigned> entries;
    while(std::getline(map,line)){
        const auto a=static_cast<unsigned>(std::stoul(line.substr(0,line.find(',')),nullptr,0));
        if(a<base || a-base>=elf.bytes.size())throw std::runtime_error("Map address outside boot");
        ElfSymbol s{};s.name="fn_"+std::to_string(a);s.value=a;s.size=elf.bytes.size()-(a-base);s.info=0x12;
        elf.symbols.push_back(s);entries.insert(a);
    }
    ProgramAnalysisOptions options{};options.max_functions=512;options.native_override_symbols.clear();
    while(std::getline(seed,line)){
        if(line.empty()||line[0]=='#')continue;
        const auto a=static_cast<unsigned>(std::stoul(line,nullptr,0));
        if(!entries.contains(a))throw std::runtime_error("Seed has no audited entry: "+line);
        options.seed_addresses.push_back(a);
    }
    const auto program=analyze_reachable_program(elf,"fn_"+std::to_string(elf.entry),options);
    const std::filesystem::path out=argv[4];std::filesystem::create_directories(out);
    std::ofstream functions(out/"provider-functions.csv"),calls(out/"provider-external.csv");
    functions<<"address,instructions\n";calls<<"caller,site,target,resolved\n";
    unsigned instructions=0;
    for(const auto& f:program.functions){
        if(f.analysis.unknown)throw std::runtime_error("Unknown instruction in provider closure");
        functions<<"0x"<<std::hex<<f.analysis.start_address<<std::dec<<','<<f.analysis.instructions.size()<<'\n';
        instructions+=f.analysis.instructions.size();
    }
    for(const auto& c:program.external_calls)calls<<"0x"<<std::hex<<c.caller<<",0x"<<c.instruction_address<<",0x"<<c.target<<std::dec<<','<<c.resolved<<'\n';
    const auto emitted=emit_cpp_program(elf,program,{out,true,true,true});
    for(const auto& f:emitted.functions)if(f.raw_ops)throw std::runtime_error("RAW_SH4 in provider closure");
    std::cout<<"{\"functions\":"<<program.functions.size()<<",\"instructions\":"<<instructions<<",\"external_records\":"<<program.external_calls.size()<<"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
