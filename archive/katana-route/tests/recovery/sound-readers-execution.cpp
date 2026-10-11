// Frozen original firmware captures; this does not run live startup or ARM7.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool value,const char* why){if(!value)throw std::runtime_error(why);}
static std::vector<std::uint8_t> read(const char* path){
    std::ifstream f(path,std::ios::binary);need(bool(f),"Cannot open reader capture");
    return {std::istreambuf_iterator<char>(f),{}};
}
static unsigned word(const std::vector<std::uint8_t>& data,unsigned offset){
    unsigned value{};std::memcpy(&value,data.data()+(offset&0xFFFFFF),4);return value;
}
int main(int argc,char**argv){try{
    need(argc==3,"usage: oracle MAIN_RAM SOUND_RAM");
    const auto main=read(argv[1]),sound=read(argv[2]);
    need(main.size()==16*1024*1024 && sound.size()==2*1024*1024,"Wrong reader capture size");
    auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
    load_embedded_elf_image(rt);register_recompiled_program(rt);
    constexpr unsigned output=0x8CD10004,stack=0x8CEFF000;
    const unsigned table=word(main,0x8C5FF84C);
    need(word(main,table+0xD4)==1,"Captured G2 critical-section contract differs");
    unsigned cases=0;
    for(unsigned entry:{0x8C66AEC8u,0x8C66AE90u,0x8C66CE4Cu}){
        const bool byte=entry==0x8C66AEC8;
        const std::vector<unsigned> offsets=byte
            ?std::vector<unsigned>{0x20,0x21,0x22,0x23,0xF8,0xF9,0xFA,0xFB,0x1FFFFC,0x1FFFFD,0x1FFFFE,0x1FFFFF,0x200000,0x200001,0x7FFFFFFF}
            :std::vector<unsigned>{0x20,0xF8,0xA0,0xA4,0xA8,0xAC,0xB0,0xB4,0x1FFFFC,0x200000,0x200001,0x7FFFFFFF};
        for(unsigned offset:offsets)for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
            rt.main_ram=main;rt.aica_ram=sound;
            dc_write32(rt,output-4,0x12345678);dc_write32(rt,output,0xA5A5A5A5);dc_write32(rt,output+4,0x87654321);
            auto expected=rt.main_ram;
            const bool valid=offset<0x200000;
            if(valid){
                std::memcpy(expected.data()+(output&0xFFFFFF),sound.data()+offset,byte?1:4);
                const unsigned count=word(main,table+0x10C)+1;
                std::memcpy(expected.data()+((table+0x10C)&0xFFFFFF),&count,4);
            }
            SH4Context ctx{};for(unsigned i=0;i<16;++i)ctx.r[i]=0x11110000+i;
            ctx.r[4]=offset;ctx.r[5]=output;ctx.r[15]=stack;
            ctx.pc=entry;ctx.pr=0xFFFFFFFF;ctx.sr=0x40000000|(mask<<4)|t;
            ctx.gbr=0x8CD12340;ctx.vbr=0x8C000000;ctx.fpscr=0x40001;
            const auto before=ctx;
            call_recompiled(ctx,rt,entry);
            need(ctx.pc==before.pr && ctx.pr==before.pr && ctx.r[0]==0,"Reader return ABI differs");
            for(unsigned i=8;i<16;++i)need(ctx.r[i]==before.r[i],"Reader callee-saved register differs");
            need((ctx.sr&~1u)==(before.sr&~1u),"Reader SR interrupt mask differs");
            need(ctx.gbr==before.gbr && ctx.vbr==before.vbr && ctx.fpscr==before.fpscr && ctx.fpul==before.fpul && ctx.mach==before.mach && ctx.macl==before.macl,"Reader preserved control register differs");
            // Original nested routines use at most100 bytes below SP. Permit
            // their scratch only; compare all other RAM including stack guards.
            std::memcpy(expected.data()+((stack-128)&0xFFFFFF),rt.main_ram.data()+((stack-128)&0xFFFFFF),128);
            need(rt.main_ram==expected,"Reader output, counter or RAM guards differ");
            need(rt.aica_ram==sound,"Reader changes frozen sound RAM");
            ++cases;
        }
    }
    std::cout<<"PASS original sound readers: "<<cases<<" frozen-capture cases; byte lanes/aligned words, nonnegative bounds, interrupt-mask and ABI preservation; exact RAM outside original stack scratch\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
