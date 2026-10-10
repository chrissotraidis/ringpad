// Generate independently named synthetic AOT units; no game bytes.
#include "dcrecomp/cpp_emitter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dcrecomp;
int main(int argc,char** argv) {
 try {
    if(argc!=2) throw std::runtime_error("usage: module-analysis OUTPUT");
    const std::filesystem::path root=argv[1];
    std::filesystem::create_directories(root);
    std::ofstream manifest(root/"module-manifest.hpp"),cmake(root/"module-sources.cmake");
    manifest<<"#pragma once\n#include \"dc_runtime.hpp\"\nnamespace dcrecomp_generated {\n";
    for(const std::string name:{"a","b","resident"}) {
        Elf32Image elf{};elf.type=2;elf.machine=42;elf.entry=name=="resident"?0x8C010000:0x8C900000;
        elf.bytes.resize(0x200);
        auto word=[&](unsigned off,unsigned v){elf.bytes.at(off)=v&255;elf.bytes.at(off+1)=v>>8;};
        // Save PR; BSR helper; NOP; restore PR; RTS; NOP.
        for(auto [off,v]:{std::pair{0u,0x4F22u},{2u,0xB01Du},{4u,9u},{6u,0x4F26u},{8u,11u},{10u,9u},
                         {0x40u,name=="b"?0xE016u:0xE00Bu},{0x42u,11u},{0x44u,9u},
                         {0x80u,0xE021u},{0x82u,11u},{0x84u,9u}})word(off,v);
        // Resident wrapper uses a statically discovered indirect call to the
        // overlay helper. Its old compiled helper must not bypass activation.
        for(auto [off,v]:{std::pair{0x100u,0x4F22u},{0x102u,0xD103u},{0x104u,0x410Bu},{0x106u,9u},
                         {0x108u,0x4F26u},{0x10Au,11u},{0x10Cu,9u},
                         {0x110u,0x0040u},{0x112u,0x8C90u}})word(off,v);
        auto section=[&](unsigned base,unsigned off,const char* label){
            ElfSection s{};s.name=label;s.type=1;s.flags=6;s.address=base;s.offset=off;s.size=0x100;elf.sections.push_back(s);
        };
        section(0x8C900000,0,".module");section(0x8C010000,0x100,".resident");
        auto symbol=[&](unsigned addr,unsigned size,const char* label){ElfSymbol s{};s.name=label;s.value=addr;s.size=size;s.info=0x12;s.section_index=addr==0x8C010000?1:0;elf.symbols.push_back(s);};
        symbol(0x8C900000,12,"module_entry");symbol(0x8C900040,6,"helper");
        symbol(0x8C900080,6,"old_only");symbol(0x8C010000,20,"resident_entry");
        ProgramAnalysisOptions options{};options.native_override_symbols.clear();
        if(name=="a") options.seed_addresses={0x8C900080};
        auto program=analyze_reachable_program(elf,name=="resident"?"resident_entry":"module_entry",options);
        auto result=emit_cpp_program(elf,program,{root/name,true,true,false});
        if(result.functions.size()!=(name=="a"?3u:2u))throw std::runtime_error("Unexpected module closure");
        std::string definitions="register_recompiled_program="+name+"_register";
        for(const auto& f:result.functions){
            if(f.raw_ops)throw std::runtime_error("RAW opcode");
            definitions+=";"+f.cpp_function_name+"="+name+"_"+f.cpp_function_name;
            manifest<<"void "<<name<<"_"<<f.cpp_function_name<<"(SH4Context&, DCRuntime&);\n";
        }
        for(const auto& source:result.program_sources)
            cmake<<"target_sources(recovery_oracle PRIVATE \""<<source.string()<<"\")\nset_source_files_properties(\""<<source.string()<<"\" PROPERTIES COMPILE_DEFINITIONS \""<<definitions<<"\")\n";
        manifest<<"inline constexpr std::uint8_t "<<name<<"_bytes[] = {";
        unsigned offset=name=="resident"?0x100:0;
        for(unsigned i=0;i<0x100;++i)manifest<<unsigned(elf.bytes[offset+i])<<",";
        manifest<<"};\ninline const CodeModuleEntry "<<name<<"_entries[] = {\n";
        for(const auto& f:result.functions){
            if(name=="resident" && f.address!=elf.entry)continue;
            manifest<<"{"<<f.address<<"u, &"<<name<<"_"<<f.cpp_function_name<<"},\n";
        }
        manifest<<"};\n";
        std::cout<<name<<": "<<result.functions.size()<<" functions\n";
    }
    manifest<<"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
