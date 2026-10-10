// Original settings code on declared synthetic flash and a frozen RAM fixture.
// These cases never change the live startup or run the sound CPU.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static std::vector<std::uint8_t> read(const char* path){
    std::ifstream f(path,std::ios::binary);need(bool(f),"Missing settings fixture");
    return {std::istreambuf_iterator<char>(f),{}};
}
static unsigned word(const std::vector<std::uint8_t>& data,unsigned address){
    unsigned value;std::memcpy(&value,data.data()+(address&0xFFFFFF),4);return value;
}
static void put(std::vector<std::uint8_t>& data,unsigned address,unsigned value){
    std::memcpy(data.data()+(address&0xFFFFFF),&value,4);
}
static unsigned crc(const std::vector<std::uint8_t>& data,unsigned offset){
    unsigned value=0xFFFF;
    for(unsigned i=0;i<62;++i)for(int bit=7;bit>=0;--bit){
        const bool different=((value>>15)^((data[offset+i]>>bit)&1))&1;
        value=(value<<1)&0xFFFF;if(different)value^=0x1021;
    }
    return value^0xFFFF;
}
static SH4Context context(unsigned entry,unsigned mask,unsigned t){
    SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0x11110000+i;
    c.r[15]=0x8CEFF000;c.pc=entry;c.pr=0xFFFFFFFF;c.sr=0x40000000|(mask<<4)|t;
    c.gbr=0x8CD12340;c.vbr=0x8C000000;c.fpscr=0x40001;return c;
}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result){
    need(after.pc==before.pr && after.pr==before.pr && after.r[0]==result,"Settings return ABI differs");
    for(unsigned i=8;i<16;++i)need(after.r[i]==before.r[i],"Settings callee-saved register differs");
    need((after.sr&~1u)==(before.sr&~1u),"Settings interrupt mask differs");
    need(after.gbr==before.gbr && after.vbr==before.vbr && after.fpscr==before.fpscr &&
        after.fpul==before.fpul && after.mach==before.mach && after.macl==before.macl,"Settings control register differs");
}
int main(int argc,char**argv){try{
    need(argc==3,"usage: settings-oracle MAIN_RAM FACTORY_FLASH");
    const auto main=read(argv[1]),factory=read(argv[2]);
    need(main.size()==16*1024*1024 && factory.size()==131072,"Wrong settings fixture size");
    auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;
    load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context initial{};dc_setup_commercial_boot(initial,rt);
    const auto sound=rt.aica_ram;
    constexpr unsigned output=0x8CD10004,stack=0x8CEFF000;
    const unsigned descriptor=word(main,0x8C677190),cache=word(main,0x8C67714C);
    need(descriptor==0x8C8937F0 && cache==0x8CFFBF8C,"Captured settings pointers differ");
    unsigned cases=0;
    auto initialize_info=[&]{
        // Replay the declared raw INIT service instead of fabricating its ready flag.
        SH4Context c{};c.r[7]=0;c.pr=0xFFFFFFFF;
        rt.targets.at(dc_read32(rt,0x8C0000B0))(c,rt);
        need(c.r[0]==0 && rt.system_info_initialized,"Settings SYSINFO prerequisite failed");
    };
    for(unsigned scenario=0;scenario<6;++scenario)for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
        auto flash=factory;
        const char magic[]="KATANA_FLASH____";
        if(scenario){std::copy_n(magic,16,flash.begin()+0x1C000);flash[0x1C010]=2;flash[0x1C011]=0;}
        const bool record=scenario>=2,valid_crc=scenario!=4;
        const unsigned mode=scenario==3?1:scenario==5?2:0;
        if(record){
            flash[0x1C040]=5;flash[0x1C041]=0;
            const std::uint8_t payload[]={0x34,0x12,0x78,0x56,0,1,static_cast<std::uint8_t>(mode),1};
            std::copy(std::begin(payload),std::end(payload),flash.begin()+0x1C042);
            const unsigned sum=crc(flash,0x1C040);flash[0x1C07E]=sum&255;flash[0x1C07F]=sum>>8;
            if(!valid_crc)flash[0x1C07E]^=1;
            flash[0x1FFC0]=0x7F; // Physical block1 allocated, remaining bits free.
        }
        rt.main_ram=main;dc_set_system_flash_image(rt,flash);initialize_info();
        dc_write32(rt,output-4,0x12345678);dc_write32(rt,output,0xA5A5A5A5);dc_write32(rt,output+4,0x87654321);
        auto expected=rt.main_ram,expected_flash=flash;
        if(!scenario){std::copy_n(magic,16,expected_flash.begin()+0x1C000);expected_flash[0x1C010]=2;expected_flash[0x1C011]=0;}
        std::vector<std::uint8_t> expected_cache(0x4000,0xFF);
        std::copy_n(expected_flash.begin()+0x1C000,64,expected_cache.begin());
        std::copy_n(expected_flash.begin()+0x1FFC0,64,expected_cache.end()-64);
        if(record && valid_crc)std::copy_n(flash.begin()+0x1C040,64,expected_cache.begin()+6*64);
        std::copy(expected_cache.begin(),expected_cache.end(),expected.begin()+(cache&0xFFFFFF));
        put(expected,descriptor,2);put(expected,descriptor+4,0x1C000);put(expected,descriptor+8,0x4000);
        put(expected,descriptor+12,cache);put(expected,descriptor+16,record?1:0);
        put(expected,descriptor+20,254);put(expected,descriptor+24,1);put(expected,descriptor+28,0);
        put(expected,descriptor+32,1);put(expected,descriptor+40,5);put(expected,descriptor+44,0);
        put(expected,descriptor+48,stack-88);
        std::copy_n(expected_flash.begin()+(record?0x1C040:0x1C000),64,expected.begin()+((descriptor+52)&0xFFFFFF));
        const bool success=record && valid_crc && mode<2;
        put(expected,output,success?mode:0);
        if(record && valid_crc)std::copy_n(flash.begin()+0x1C042,8,expected.begin()+0x78);
        else std::fill_n(expected.begin()+0x78,8,0xFF);
        auto ctx=context(0x8C60305E,mask,t);ctx.r[4]=output;const auto before=ctx;
        call_recompiled(ctx,rt,ctx.pc);abi(before,ctx,success?0:0xFFFFFFFF);
        // Reviewed nesting uses at most148 bytes below SP; preserve outer guards.
        std::copy_n(rt.main_ram.begin()+((stack-160)&0xFFFFFF),160,expected.begin()+((stack-160)&0xFFFFFF));
        if(rt.main_ram!=expected){
            for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){
                std::cerr<<"scenario="<<scenario<<" RAM offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<std::dec<<'\n';break;
            }
            throw std::runtime_error("Settings exact RAM differs");
        }
        need(rt.system_flash==expected_flash && rt.aica_ram==sound,"Settings flash/sound guards differ");++cases;
    }
    for(bool missing_output:{false,true})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
        rt.main_ram=main;dc_set_system_flash_image(rt,factory);dc_write32(rt,output,0xA5A5A5A5);
        if(!missing_output)dc_write32(rt,0x8C67714C,0);
        auto expected=rt.main_ram;if(!missing_output)put(expected,output,0);
        auto ctx=context(0x8C60305E,mask,t);ctx.r[4]=missing_output?0:output;const auto before=ctx;
        call_recompiled(ctx,rt,ctx.pc);abi(before,ctx,0xFFFFFFFF);
        std::copy_n(rt.main_ram.begin()+((stack-8)&0xFFFFFF),8,expected.begin()+((stack-8)&0xFFFFFF));
        need(rt.main_ram==expected && rt.system_flash==factory,"Missing settings argument changed state");++cases;
    }
    for(unsigned block:{0u,5u,253u,254u,0xFFFFFFFFu,0x7FFFFFFFu})for(unsigned busy:{0u,1u})
        for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
        rt.main_ram=main;dc_write32(rt,descriptor+28,busy);auto expected=rt.main_ram;
        const bool valid=block<254 && !busy;
        if(valid){put(expected,descriptor+28,1);put(expected,descriptor+32,2);put(expected,descriptor+40,block);put(expected,descriptor+48,output);}
        auto ctx=context(0x8C607F14,mask,t);ctx.r[4]=block;ctx.r[5]=output;const auto before=ctx;
        call_recompiled(ctx,rt,ctx.pc);abi(before,ctx,busy?unsigned(-9):valid?0:unsigned(-5));
        need(rt.main_ram==expected,"Logical-block submission changed wrong RAM");++cases;
    }
    for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
        rt.main_ram=main;dc_write32(rt,0x8C677190,0);const auto expected=rt.main_ram;
        auto ctx=context(0x8C607F14,mask,t);ctx.r[4]=5;ctx.r[5]=output;const auto before=ctx;
        call_recompiled(ctx,rt,ctx.pc);abi(before,ctx,unsigned(-11));need(rt.main_ram==expected,"Missing logical-block root changed RAM");++cases;
    }
    std::cout<<"PASS original system settings: "<<cases<<" cases; erased/empty/stereo/mono/invalid-mode/bad-CRC; missing arguments; logical-block busy/bounds; exact RAM/flash and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
