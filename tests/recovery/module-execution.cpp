#include "module-manifest.hpp"
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
using namespace dcrecomp_generated;
static void require(bool v,const char* why){if(!v)throw std::runtime_error(why);}
static unsigned run(DCRuntime& r,unsigned pc){
    SH4Context c{};c.pc=pc;c.pr=0xFFFFFFFF;c.r[15]=0x8CFFF000;
    for(unsigned n=8;n<15;++n)c.r[n]=0x12340000+n;
    call_recompiled(c,r,pc);
    require(c.pc==0xFFFFFFFF && c.pr==0xFFFFFFFF && c.r[15]==0x8CFFF000,"Return/stack differs");
    for(unsigned n=8;n<15;++n)require(c.r[n]==0x12340000+n,"Preserved register differs");
    return c.r[0];
}
template<class F>static void rejects(F f,const char* message){
    try{f();}catch(const std::runtime_error& e){require(std::string(e.what()).find(message)!=std::string::npos,"Unexpected rejection");return;}
    throw std::runtime_error("Expected rejection did not occur");
}
int main(){try{
    auto ptr=std::make_unique<DCRuntime>();auto& r=*ptr;
    dc_load_bytes(r,0x8C010000,resident_bytes,sizeof resident_bytes);
    r.register_target(0x8C010000,resident_entries[0].function);
    r.direct_dispatch_enabled=true;r.fast_dispatch_enabled=true;dc_refresh_dispatch_ready(r);
    dc_load_bytes(r,0x8C900000,a_bytes,sizeof a_bytes);
    r.register_target(0x8C900040,&resident_recomp_8C900040);
    require(run(r,0x8C010000)==11 && r.dynamic_direct_hint_hits>0,"Direct-hint positive control absent");
    const auto original_hints=r.dynamic_direct_hint_hits;
    unsigned activations=0;
    for(bool use_b:{false,true,false}){
        // Retire BEFORE writing. No previous code or signature fallback may run.
        dc_begin_code_module(r,0xAC900000,0x100);
        require(!r.direct_dispatch_ready && !r.fast_dynamic_dispatch_ready,"Unchecked dispatch bypass enabled");
        for(unsigned base:{0x0C900000u,0x8C900000u,0xAC900000u,0x8D900000u})
            rejects([&]{run(r,base);},"Inactive or unregistered");
        const auto* bytes=use_b?b_bytes:a_bytes;
        dc_load_bytes(r,0x0C900000,bytes,0x100);
        const auto* entries=use_b?b_entries:a_entries;
        auto count=use_b?std::size(b_entries):std::size(a_entries);
        // Same size/base is insufficient: reject a different compiled image.
        rejects([&]{dc_activate_code_module(r,0x8C900000,use_b?a_bytes:b_bytes,0x100,entries,count);},"image mismatch");
        rejects([&]{run(r,0x8C900000);},"Inactive or unregistered");
        CodeModuleEntry bad{0x8C910000,entries[0].function};
        rejects([&]{dc_activate_code_module(r,0x8C900000,bytes,0x100,&bad,1);},"Invalid code module entry");
        const CodeModuleEntry duplicate[]={entries[0],entries[0]};
        rejects([&]{dc_activate_code_module(r,0x8C900000,bytes,0x100,duplicate,2);},"Duplicate code module entry");
        const CodeModuleEntry partial[]={entries[0],bad};
        rejects([&]{dc_activate_code_module(r,0x8C900000,bytes,0x100,partial,2);},"Invalid code module entry");
        require(!r.targets.contains(0x8C900000),"Failed activation installed partial entries");
        dc_activate_code_module(r,0x8C900000,bytes,0x100,entries,count);++activations;
        for(unsigned base:{0x0C900000u,0x8C900000u,0xAC900000u,0x8D900000u})
            for(unsigned n=0;n<2;++n)require(run(r,base)==(use_b?22u:11u),"Wrong activated function");
        require(run(r,0x8C010000)==(use_b?22u:11u),"Resident call used old compiled helper");
        require(r.dynamic_direct_hint_hits==original_hints,"Direct hint bypassed active module selection");
        if(use_b)rejects([&]{run(r,0x8C900080);},"Inactive or unregistered");
        else require(run(r,0x8C900080)==33,"A-only entry differs");
        // Undeclared entry must fail even if bytes match a known function.
        dc_load_bytes(r,0x8C9000C0,bytes+0x40,32);
        rejects([&]{run(r,0x8C9000C0);},"Inactive or unregistered");
        // Derived relocation and cache state must be discarded next retirement.
        r.relocated_code_aliases[0x8CA00000]=0x8C900040;
    }
    require(r.dispatch_cache_hits>0 && r.dispatch_cache_misses>0,"Cache controls absent");
    dc_begin_code_module(r,0x8C900000,0x100);
    require(r.relocated_code_aliases.empty(),"Stale relocation mapping remains");
    for(const auto& cached:r.dispatch_cache)require(!cached.function,"Stale dispatch cache remains");
    require(r.targets.contains(0x8C010000),"Resident registration lost");
    rejects([&]{run(r,0x8C010000);},"Inactive or unregistered");
    rejects([&]{dc_begin_code_module(r,0x8C900080,0x100);},"Overlapping");
    require(r.code_module_dispatch_depth==0,"Dispatch depth leaked after return or exception");
    r.register_target(0x8C010080,+[](SH4Context&,DCRuntime& rt){dc_begin_code_module(rt,0x8C900000,0x100);});
    rejects([&]{run(r,0x8C010080);},"non-quiescent");
    require(r.code_module_dispatch_depth==0,"Dispatch depth leaked after nested rejection");
    require(activations==3,"Missing reload");
    std::cout<<"PASS A -> B -> A; exact image binding; physical/P1/P2/mirror aliases; cached calls; resident direct-hint fallback; retired/unknown entries; preserved state; relocation/cache retirement\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
