// Original BIOS wrappers, native AOT, using the candidate's explicit GD-ROM HLE.
// This checks sector I/O, not the game's file-provider initialization.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dcrecomp_generated;
static void require(bool v,const char* message){if(!v)throw std::runtime_error(message);}
static std::uint32_t invoke(DCRuntime& rt,std::uint32_t pc,std::uint32_t a=0,std::uint32_t b=0){
    SH4Context c{};c.pc=pc;c.pr=0xFFFFFFFF;c.r[15]=0x8CFFF000;c.r[4]=a;c.r[5]=b;
    for(unsigned i=8;i<15;++i)c.r[i]=0x12340000+i;
    call_recompiled(c,rt,pc);
    require(c.pc==0xFFFFFFFF && c.pr==0xFFFFFFFF && c.r[15]==0x8CFFF000,"Wrapper return/stack mismatch");
    for(unsigned i=8;i<15;++i)require(c.r[i]==0x12340000+i,"Preserved register changed");
    return c.r[0];
}
static std::unique_ptr<DCRuntime> make(){
    auto rt=std::make_unique<DCRuntime>();load_embedded_elf_image(*rt);register_recompiled_program(*rt);
    SH4Context boot{};dc_setup_commercial_boot(boot,*rt); // Public BIOS services, not a downloaded BIOS.
    invoke(*rt,0x8C10F594);return rt;
}
int main(int argc,char** argv){
 try{
    require(argc==4,"usage: disc-oracle DISC_MAP EXPECTED_SECTORS FAD");
    std::ifstream f(argv[2],std::ios::binary);require(bool(f),"Expected bytes unavailable");
    std::vector<std::uint8_t> expected{std::istreambuf_iterator<char>(f),{}};
    require(!expected.empty() && expected.size()%2048==0 && expected.size()<0x100000,"Invalid expected sector payload");
    const auto fad=static_cast<std::uint32_t>(std::stoul(argv[3],nullptr,0));
    constexpr std::uint32_t params=0x8CE00000,result=0x8CE00100,dest=0x8CD00000;
    auto owner=make();auto& rt=*owner;dc_gdrom_load_disc_map(rt,argv[1]);
    auto submit=[&](DCRuntime& r,unsigned command,unsigned sector){
        dc_write32(r,params,sector);dc_write32(r,params+4,expected.size()/2048);
        dc_write32(r,params+8,dest);dc_write32(r,params+12,0);
        return invoke(r,0x8C10F558,command,params);
    };
    for(unsigned command:{16u,17u}){
        for(unsigned i=0;i<expected.size()+32;++i)dc_write8(rt,dest-16+i,0xA5);
        const auto before_sectors=rt.gd_sector_reads,before_bytes=rt.gd_bytes_read;
        const auto id=submit(rt,command,fad);require(id!=0,"Read request refused");
        require(invoke(rt,0x8C10F56C,id,result)==1,"Request did not become busy");
        require(invoke(rt,0x8C10F580)==0,"Disc server error");
        require(invoke(rt,0x8C10F56C,id,result)==2,"Read did not complete");
        require(rt.gd_sector_reads-before_sectors==expected.size()/2048,"Sector count mismatch");
        require(rt.gd_bytes_read-before_bytes==expected.size(),"Byte count mismatch");
        for(unsigned i=0;i<expected.size();++i)require(dc_read8(rt,dest+i)==expected[i],"Disc byte mismatch");
        for(unsigned i=0;i<16;++i){require(dc_read8(rt,dest-16+i)==0xA5,"Leading guard changed");require(dc_read8(rt,dest+expected.size()+i)==0xA5,"Trailing guard changed");}
        require(invoke(rt,0x8C10F56C,id,result)==0,"Completed request was not acknowledged");
        std::cout<<"PASS command="<<command<<" sectors="<<expected.size()/2048<<" bytes="<<expected.size()<<" status/guards/registers\n";
    }
    bool outside=false;submit(rt,16,0);
    try{invoke(rt,0x8C10F580);}catch(const std::runtime_error& e){outside=std::string(e.what()).find("outside mapped data tracks")!=std::string::npos;}
    require(outside,"Unmapped FAD did not fail");
    auto missing=make();submit(*missing,16,fad);bool rejected=false;
    try{invoke(*missing,0x8C10F580);}catch(const std::runtime_error& e){
        std::cerr<<"Expected missing-disc failure: "<<e.what()<<'\n';
        rejected=std::string(e.what())=="GD-ROM HLE command requires --disc-map generated from the original CDI";
    }
    require(rejected,"Missing disc did not fail");
    std::cout<<"PASS unmapped-sector and missing-disc rejection\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
