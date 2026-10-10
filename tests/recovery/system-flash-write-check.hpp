// Isolated raw BIOS flash ABI; expected data follows the public NOR contract.
#pragma once
#include "system-flash-check.hpp"

inline void check_system_flash_writes() {
    using namespace dcrecomp_generated;
    auto owner=std::make_unique<DCRuntime>(); auto& rt=*owner;
    SH4Context ctx{}; dc_setup_commercial_boot(ctx,rt);
    const auto flash=rt.targets.at(dc_read32(rt,0x8C0000B8));
    unsigned cases=0;
    auto need=[](bool ok,const char* message){if(!ok)throw std::runtime_error(message);};
    auto prepare=[&](unsigned command,unsigned offset,unsigned source,unsigned count){
        ctx={};
        for(unsigned i=0;i<16;++i){ctx.r[i]=0xA1000000+i;ctx.fr_bits[i]=0x3F800000+i;ctx.xf_bits[i]=0x40000000+i;}
        for(unsigned i=0;i<8;++i)ctx.r_bank[i]=0xB1000000+i;
        ctx.sr=0x600000F1;ctx.fpscr=0x40001;ctx.gbr=0x8C000000;ctx.vbr=0x8C000000;
        ctx.mach=0x12345678;ctx.macl=0x87654321;ctx.fpul=0x55667788;
        ctx.pc=0x8C001004;ctx.pr=0x8C123456;
        ctx.r[7]=command;ctx.r[4]=offset;ctx.r[5]=source;ctx.r[6]=count;
    };
    auto execute=[&](unsigned result,const std::vector<std::uint8_t>& expected_flash,const char* error=nullptr){
        const auto prior=ctx;const auto ram=rt.main_ram;bool rejected=false;
        try{flash(ctx,rt);}catch(const std::runtime_error& e){
            rejected=true;need(error && std::string(e.what()).find(error)!=std::string::npos,"Wrong flash rejection");
        }
        need(rejected==bool(error),"Missing flash rejection");
        auto wanted=prior;if(!error){wanted.r[0]=result;wanted.pc=prior.pr;}
        need(std::memcmp(&wanted,&ctx,sizeof(ctx))==0,"Flash result/context differs");
        need(rt.main_ram==ram,"Flash programming changed guest RAM");
        need(rt.system_flash==expected_flash,"Flash exact image/guards differ");++cases;
    };
    prepare(2,0,0x8C004001,1);execute(0,{},"explicit system flash image");
    prepare(3,0,0,0);execute(0,{},"explicit system flash image");
    std::vector<std::uint8_t> pattern(131072);
    for(unsigned i=0;i<pattern.size();++i)pattern[i]=(i*37+91)&255;
    std::fill(rt.main_ram.begin(),rt.main_ram.end(),0xD6);
    const std::uint8_t payload[]={0xF0,0x0F,0x55,0xAA,0x00,0xFF,0x81,0x7E};
    std::copy(std::begin(payload),std::end(payload),rt.main_ram.begin()+0x4001);
    for(unsigned address:{0x8C004001u,0xAC004001u,0x0C004001u,0x8D004001u}){
        dc_set_system_flash_image(rt,pattern);auto expected=pattern;
        for(unsigned i=0;i<8;++i)expected[0x123+i]=pattern[0x123+i]&payload[i];
        prepare(2,0x123,address,8);execute(8,expected);
    }
    for(auto range:{std::pair{0u,1u},std::pair{0x1FFFFu,1u},std::pair{0x20000u,0u}}){
        dc_set_system_flash_image(rt,pattern);auto expected=pattern;
        if(range.second)expected[range.first]&=payload[0];
        prepare(2,range.first,0x8C004001,range.second);execute(range.second,expected);
    }
    for(auto range:{std::pair{0x20001u,0u},std::pair{0x1FFFFu,2u},std::pair{1u,0xFFFFFFFFu}}){
        dc_set_system_flash_image(rt,pattern);
        prepare(2,range.first,0x8C004001,range.second);execute(0xFFFFFFFF,pattern);
    }
    for(unsigned address:{0x8CFFFFFFu,0xA05F8000u}){
        prepare(2,0,address,2);execute(0,pattern,"outside supported RAM range");
    }
    prepare(2,0,0xA05F8000,0);execute(0,pattern);
    dc_set_system_flash_image(rt,std::vector<std::uint8_t>(131072,0xFF));
    auto expected=rt.system_flash;
    for(unsigned byte:{0xA5u,0xFFu,0u,0xFFu}){
        rt.main_ram[0x4001]=byte;expected[0x101]&=byte;
        prepare(2,0x101,0x8C004001,1);execute(1,expected);
    }
    const unsigned parts[][2]={{0x1A000,0x2000},{0x18000,0x2000},{0x1C000,0x4000},{0x10000,0x8000},{0,0x10000}};
    for(const auto& part:parts){
        dc_set_system_flash_image(rt,pattern);expected=pattern;
        std::fill_n(expected.begin()+part[0],part[1],0xFF);
        prepare(3,part[0],0xA05F8000,0xFFFFFFFF);execute(0,expected);
    }
    for(unsigned offset:{1u,0xFFFFFFFFu,0x20000u,0x1C001u,0x1A001u}){
        dc_set_system_flash_image(rt,pattern);
        prepare(3,offset,0,0);execute(0xFFFFFFFF,pattern);
    }
    dc_set_system_flash_image(rt,pattern);expected=pattern;
    std::fill_n(expected.begin()+0x1C000,0x4000,0xFF);
    prepare(3,0x1C000,0,0);execute(0,expected);
    std::copy(std::begin(payload),std::end(payload),rt.main_ram.begin()+0x4001);
    std::copy(std::begin(payload),std::end(payload),expected.begin()+0x1C003);
    prepare(2,0x1C003,0x8C004001,8);execute(8,expected);
    // A raw read must expose the programmed bytes with guards and context intact.
    prepare(1,0x1C003,0x8C005001,8);auto wanted=ctx;auto ram=rt.main_ram;
    wanted.r[0]=8;wanted.pc=wanted.pr;
    std::copy(std::begin(payload),std::end(payload),ram.begin()+0x5001);
    flash(ctx,rt);need(std::memcmp(&wanted,&ctx,sizeof(ctx))==0,"Program readback context differs");
    need(rt.main_ram==ram && rt.system_flash==expected,"Program readback/guards differ");++cases;
    std::cout<<"PASS isolated BIOS flash writes: "<<cases
        <<" cases; bit clearing; exact partition erase; bounds/aliases; full RAM/context/flash\n";
}
