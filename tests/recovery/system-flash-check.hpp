// Isolated BIOS-service contract checks. No game data or live runtime changes.
#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

inline void check_system_flash_services() {
    using namespace dcrecomp_generated;
    auto owner=std::make_unique<DCRuntime>(); auto& rt=*owner;
    SH4Context ctx{}; dc_setup_commercial_boot(ctx,rt);
    const auto system=rt.targets.at(dc_read32(rt,0x8C0000B0));
    const auto flash=rt.targets.at(dc_read32(rt,0x8C0000B8));
    unsigned cases=0;
    auto need=[](bool ok,const char* message){if(!ok)throw std::runtime_error(message);};
    auto prepare=[&](unsigned command,unsigned offset=0,unsigned destination=0x8C003FFD,unsigned count=0){
        ctx={};
        for(unsigned i=0;i<16;++i){ctx.r[i]=0xA1000000+i;ctx.fr_bits[i]=0x3F800000+i;ctx.xf_bits[i]=0x40000000+i;}
        for(unsigned i=0;i<8;++i)ctx.r_bank[i]=0xB1000000+i;
        ctx.sr=0x600000F1;ctx.fpscr=0x40001;ctx.gbr=0x8C000000;ctx.vbr=0x8C000000;
        ctx.mach=0x12345678;ctx.macl=0x87654321;ctx.fpul=0x55667788;
        ctx.pc=0x8C001000;ctx.pr=0x8C123456;
        ctx.r[7]=command;ctx.r[4]=offset;ctx.r[5]=destination;ctx.r[6]=count;
    };
    auto execute=[&](RecompiledFunction fn,unsigned result,const std::vector<std::uint8_t>& expected,const char* error=nullptr){
        const auto prior=ctx; const auto prior_flash=rt.system_flash;
        bool rejected=false;
        try{fn(ctx,rt);}catch(const std::runtime_error& e){
            rejected=true;need(error && std::string(e.what()).find(error)!=std::string::npos,"Wrong BIOS rejection");
        }
        need(rejected==bool(error),"Missing BIOS rejection");
        auto wanted=prior;
        if(!error){wanted.r[0]=result;wanted.pc=prior.pr;}
        need(std::memcmp(&wanted,&ctx,sizeof(ctx))==0,"BIOS result or preserved context differs");
        need(rt.main_ram==expected,"BIOS exact RAM/guards differ");
        need(rt.system_flash==prior_flash,"Read-only BIOS service changed flash");
        ++cases;
    };
    auto unchanged=[&](RecompiledFunction fn,const char* error){const auto ram=rt.main_ram;execute(fn,0,ram,error);};
    prepare(0);unchanged(system,"explicit system flash image");
    prepare(1,0,0x8C004000,1);unchanged(flash,"explicit system flash image");
    std::vector<std::uint8_t> data(131072,0xD3);
    const std::uint8_t identity[]={0xE1,0x12,0xC3,0x34,0xA5,0x56,0x87,0x78};
    std::copy(std::begin(identity),std::end(identity),data.begin()+0x1A056);
    const char properties[]="00110";
    std::copy_n(properties,5,data.begin()+0x1A000);
    dc_set_system_flash_image(rt,data);
    const auto installed=rt.system_flash;
    bool bad_size=false;
    try{dc_set_system_flash_image(rt,std::vector<std::uint8_t>(131071));}
    catch(const std::runtime_error&){bad_size=true;}
    need(bad_size && rt.system_flash==installed,"Wrong-size flash replaced input");++cases;
    prepare(3);unchanged(system,"requires SYSINFO_INIT");
    std::fill(rt.main_ram.begin(),rt.main_ram.end(),0xA5);
    prepare(0);auto expected=rt.main_ram;
    const std::uint8_t record[]={0xE1,0x12,0xC3,0x34,0xA5,0x56,0x87,0x78,'0','0','1','1','0',0,0,0,0,0,0,0,0,0,0,0};
    std::copy(std::begin(record),std::end(record),expected.begin()+0x68);
    execute(system,0,expected);need(rt.system_info_initialized,"SYSINFO not initialized");
    prepare(3);execute(system,0x8C000068,expected);
    prepare(2);unchanged(system,"Unsupported BIOS system command");
    // Replacing input invalidates the old record; subsequent init reads new bytes.
    data[0x1A056]=0x6B;data[0x1A004]='1';dc_set_system_flash_image(rt,data);
    prepare(3);unchanged(system,"requires SYSINFO_INIT");
    prepare(0);expected[0x68]=0x6B;expected[0x74]='1';execute(system,0,expected);
    for(const auto address:{0x8C003FFDu,0xAC003FFDu,0x0C003FFDu,0x8D003FFDu}){
        std::fill_n(rt.main_ram.begin()+0x3FFD,10,0xA5);
        prepare(1,0x1A056,address,10);expected=rt.main_ram;
        std::copy_n(data.begin()+0x1A056,10,expected.begin()+0x3FFD);
        execute(flash,10,expected);
    }
    for(const auto pair:{std::pair{0u,1u},std::pair{0x1FFFFu,1u},std::pair{0x20000u,0u}}){
        prepare(1,pair.first,0x8C005000,pair.second);expected=rt.main_ram;
        if(pair.second)expected[0x5000]=data[pair.first];execute(flash,pair.second,expected);
    }
    for(const auto pair:{std::pair{0x20001u,0u},std::pair{0x1FFFFu,2u},std::pair{1u,0xFFFFFFFFu}}){
        prepare(1,pair.first,0x8C005000,pair.second);expected=rt.main_ram;execute(flash,0xFFFFFFFF,expected);
    }
    prepare(1,0,0x8CFFFFFF,2);unchanged(flash,"outside supported RAM range");
    prepare(1,0,0xA05F8000,2);unchanged(flash,"outside supported RAM range");
    const unsigned parts[][2]={{0x1A000,0x2000},{0x18000,0x2000},{0x1C000,0x4000},{0x10000,0x8000},{0,0x10000}};
    for(unsigned part=0;part<5;++part){
        prepare(0,part,0xAC005000);expected=rt.main_ram;
        for(unsigned word=0;word<2;++word)for(unsigned byte=0;byte<4;++byte)
            expected[0x5000+word*4+byte]=parts[part][word]>>(byte*8);
        execute(flash,0,expected);
    }
    prepare(0,5);expected=rt.main_ram;execute(flash,0xFFFFFFFF,expected);
    prepare(0,0,0x8CFFFFFC);unchanged(flash,"outside supported RAM range");
    for(unsigned command:{4u,5u,6u}){prepare(command);unchanged(flash,"Unsupported BIOS flash command");}
    std::cout<<"PASS isolated BIOS system/flash services: "<<cases
        <<" cases; explicit image; exact RAM/context; aliases/bounds; unsupported commands rejected\n";
}
