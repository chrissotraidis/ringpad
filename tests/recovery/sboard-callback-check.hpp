#pragma once
#include "callback-manifest.hpp"
#include <vector>

// Player, free pool and empty queues are fixture prerequisites, not startup proof.
static void check_sboard_callback(DCRuntime& rt) {
    constexpr unsigned task=0x8CE00000,heap=0x8CF00000,player=0x8CE00800;
    constexpr unsigned child=0x8CE01000,auxiliary=0x8CE01040;
    for(unsigned character:{0u,2u})for(bool available:{true,false}){
        dc_begin_code_module(rt,0x8C900000,0x200000);
        dc_activate_code_module(rt,0x8C900000,board_bytes,sizeof(board_bytes),board_module_entries,std::size(board_module_entries));
        check_registration(rt,256,true,true,true); // Original init supplies state zero.
        for(const auto&e:callback_resident_entries)rt.register_target(e.address,e.function);
        dc_begin_code_module(rt,0x8C900000,0x200000);
        std::vector<CodeModuleEntry> entries(std::begin(board_module_entries),std::end(board_module_entries));
        entries.insert(entries.end(),std::begin(callback_module_entries),std::end(callback_module_entries));
        dc_activate_code_module(rt,0x8C900000,board_bytes,sizeof(board_bytes),entries.data(),entries.size());
        for(unsigned i=0;i<96;++i)dc_write8(rt,player-16+i,0xA5);
        dc_write16(rt,player+4,0x23);
        dc_write32(rt,0x8C790688,player);dc_write16(rt,0x8C74D448,character);
        dc_write32(rt,0x8C1C8270,0);
        for(unsigned i=0;i<8;++i)dc_write32(rt,0x8C7A32CC+i*4,0);
        for(unsigned i=0;i<128;++i)dc_write8(rt,child+i,i%64<48?0:0xA5);
        dc_write32(rt,child,auxiliary);dc_write32(rt,auxiliary+4,child);
        dc_write32(rt,0x8C7A32C8,available?child:0);
        SH4Context ctx{};for(unsigned i=0;i<16;++i)ctx.r[i]=0x12340000+i;
        ctx.pc=dc_read32(rt,task+16);ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;ctx.r[4]=task;
        const auto before=ctx;const auto calls=rt.trace_call_count;
        try{call_recompiled(ctx,rt,ctx.pc);}catch(const std::exception&e){
            std::cerr<<"CALLBACK FAIL "<<e.what()<<" pc="<<std::hex<<ctx.pc<<" pr="<<ctx.pr<<std::dec<<'\n';
            dc_trace_print_history(rt,std::cerr);throw;
        }
        require(ctx.pc==before.pr && ctx.pr==before.pr && ctx.r[15]==before.r[15],"Callback return/stack mismatch");
        for(unsigned i=8;i<15;++i)require(ctx.r[i]==before.r[i],"Callback preserved GPR mismatch");
        for(unsigned i=0;i<96;++i){
            auto expected=std::uint8_t(0xA5);
            if(i==17)expected=44;if(i==20)expected=0x23;if(i==21)expected=0x10;
            require(dc_read8(rt,player-16+i)==expected,"Player write/guard mismatch");
        }
        require(dc_read8(rt,heap+16)==(available?2:1),"Callback phase differs");
        require(dc_read32(rt,0x8C7A32C8)==0,"Task pool head differs");
        require(dc_read32(rt,0x8C1C8270)==(available?auxiliary:0),"Auxiliary task global differs");
        require(dc_read32(rt,0x8C7A32CC)==(available?auxiliary:0),"Queue zero differs");
        require(dc_read32(rt,0x8C7A32D4)==(available?child:0),"Queue two differs");
        if(available){
            require(dc_read32(rt,child+16)==(character==0?0x8C0B4B06:0x8C0C13EE),"Character callback differs");
            require(dc_read32(rt,child+32)==heap+48 && dc_read32(rt,child+36)==heap+128,"Child work blocks differ");
            for(unsigned i=0;i<64;++i)require(dc_read8(rt,heap+48+i)==0 && dc_read8(rt,heap+128+i)==0,"Child blocks not zeroed");
            require(dc_read32(rt,auxiliary+16)==0x8C0A2B38 && dc_read32(rt,auxiliary+20)==0x8C0A2B38 && dc_read32(rt,auxiliary+24)==0x8C0A2B70,"Auxiliary metadata differs");
            require(dc_read32(rt,auxiliary+36)==heap+208,"Auxiliary allocation differs");
            require(dc_read32(rt,heap+208)==77 && dc_read32(rt,heap+212)==8,"Auxiliary arguments differ");
            require(dc_read32(rt,0x8C7A32BC)==heap+224,"Remaining heap differs");
        }else require(dc_read32(rt,0x8C7A32BC)==heap+32,"Empty task pool consumed heap");
        for(unsigned i=0;i<16;++i){
            require(dc_read8(rt,child+48+i)==0xA5 && dc_read8(rt,auxiliary+48+i)==0xA5,"Task guards changed");
            require(dc_read8(rt,heap-16+i)==0xA5,"Heap guard changed");
        }
        std::cout<<"PASS SBOARD callback character="<<character<<" tasks="<<available
            <<" state="<<unsigned(dc_read8(rt,heap+16))<<" native_calls="<<rt.trace_call_count-calls<<'\n';
        if(available){
            ctx.pc=dc_read32(rt,task+16);ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;ctx.r[4]=task;
            bool rejected=false;
            try{call_recompiled(ctx,rt,ctx.pc);}catch(const std::runtime_error&e){
                const auto& last=rt.trace_history[(rt.trace_history_next+rt.trace_history.size()-1)%rt.trace_history.size()];
                rejected=std::string(e.what()).find("No recompiled/native target registered for Dreamcast address 0x8C0194FC")!=std::string::npos
                    && (last.target&0x1FFFFFFF)==0x0C0194FC;
                std::cout<<"NEXT SBOARD boundary: "<<e.what()<<" pc=0x"<<std::hex<<ctx.pc<<std::dec<<'\n';
            }
            require(rejected && dc_read8(rt,heap+16)==2,"Next phase did not stop at missing camera initializer");
        }
    }
}
