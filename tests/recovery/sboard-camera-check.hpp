#pragma once
#include "camera-reference.hpp"
#include "startup-check.hpp"
#include <bit>
#include <cmath>

// One shared heap/task pool per case. No reset between original initializers/callbacks.
static void check_sboard_camera(DCRuntime&rt,const SH4Context&boot_context){
    constexpr unsigned heap=0x8CE10000,stage=0x8CE00000,player=0x8CE00800;
    constexpr unsigned camera=0x8CE03000,child=camera+64,auxiliary=camera+128,continuation=camera+192;
    for(const auto&e:callback_resident_entries)rt.register_target(e.address,e.function);
    check_original_rng(rt);
    check_original_draw_header(rt);
    check_original_controller_decode(rt);
    for(unsigned act:{0u,1u})for(unsigned nodes:{3u,4u,16u}){
        const bool available=nodes>3;
        SH4Context ctx{};ctx.vbr=boot_context.vbr;
        auto call=[&](unsigned entry,unsigned r4=0,unsigned r5=0){
            ctx.pc=entry;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;ctx.r[4]=r4;ctx.r[5]=r5;
            const auto before=ctx;call_recompiled(ctx,rt,ctx.pc);
            require(ctx.pc==before.pr && ctx.pr==before.pr && ctx.r[15]==before.r[15],"Joined call return/stack differs");
            for(unsigned i=8;i<15;++i)require(ctx.r[i]==before.r[i],"Joined call preserved GPR differs");
        };
        try{
            // Init decrements in-image resource counters. Reload original bytes per isolated case.
            dc_begin_code_module(rt,0x8C900000,0x200000);
            const auto reads=rt.gd_sector_reads;
            call(0x8C09F664,0x8C15D030,0x0C900000);
            require(ctx.r[0]==0 && rt.gd_sector_reads-reads==147,"Case stage reload failed");
            for(unsigned i=0;i<sizeof(board_bytes);++i)
                require(dc_read8(rt,0x8C900000+i)==board_bytes[i],"Case stage reload bytes differ");
            dc_activate_code_module(rt,0x8C900000,board_bytes,sizeof(board_bytes),callback_module_entries,std::size(callback_module_entries));
            // The owner camera reference is compared after the original filename builder/provider reads it.
            dc_write16(rt,0x8C74D43A,38);dc_write16(rt,0x8C74D43C,act);
            dc_write16(rt,0x8C74D448,act?2:0);
            const unsigned destination=0x0CB50000+act*0x8000;
            call(0x8C09E98E,act?0x8C15D060:0x8C15D054,destination);
            for(unsigned i=0;i<sizeof(camera_reference);++i)
                require(dc_read8(rt,destination+i)==camera_reference[i],"Original camera loader bytes differ");
            for(unsigned i=0;i<4128;++i)dc_write8(rt,heap-16+i,0xA5);
            dc_zero_bytes(rt,heap,4096+16);
            dc_write32(rt,heap+8,heap+4096);dc_write32(rt,heap+4096+12,heap);
            dc_write32(rt,0x8C7A32BC,heap);
            dc_zero_bytes(rt,stage,48);dc_zero_bytes(rt,player,64);
            dc_write16(rt,player+4,0x23);dc_write32(rt,0x8C790688,player);
            dc_zero_bytes(rt,0x8C7A32CC,32);dc_write32(rt,0x8C1C8270,0);
            for(unsigned n=0;n<nodes;++n){
                for(unsigned i=0;i<64;++i)dc_write8(rt,camera+n*64+i,i<48?0:0xA5);
                dc_write32(rt,camera+n*64,n+1==nodes?0:camera+(n+1)*64);
                dc_write32(rt,camera+n*64+4,n?camera+(n-1)*64:0);
            }
            dc_write32(rt,0x8C7A32C8,camera);
            call(0x8C0196FE);
            require(dc_read32(rt,camera+16)==0x8C01993A && dc_read32(rt,0x8C7A32CC)==camera,"Camera task not linked");
            require(dc_read32(rt,camera+32)==heap+16,"Camera work allocation differs");
            call(0x8C900040,stage);
            const auto state=dc_read32(rt,stage+36);
            require(state==heap+96 && dc_read8(rt,state)==0,"Stage was not initialized from shared heap");
            call(dc_read32(rt,stage+16),stage);
            require(dc_read8(rt,state)==2,"Joined state zero did not advance naturally");
            require(dc_read32(rt,0x8C7A32C8)==(available?continuation:0) && dc_read32(rt,0x8C7A32CC)==auxiliary
                && dc_read32(rt,auxiliary)==camera && dc_read32(rt,camera+4)==auxiliary
                && dc_read32(rt,0x8C7A32D4)==child,"Created task queues not preserved");
            require(dc_read32(rt,child+16)==(act?0x8C0C13EE:0x8C0B4B06),"Child callback differs");
            require(dc_read32(rt,child+32)==heap+128 && dc_read32(rt,child+36)==heap+208
                && dc_read32(rt,auxiliary+36)==heap+288 && dc_read32(rt,0x8C7A32BC)==heap+304,"Shared heap allocations differ");
            require(dc_read32(rt,heap+288)==77 && dc_read32(rt,heap+292)==8,"Auxiliary init arguments differ");
            // Change FOV through original code; the stage must restore its own angle through the same routine.
            call(0x8C0194FC,0x4000);
            require(std::abs(std::bit_cast<float>(dc_read32(rt,0x8C893864))-320.0f)<0.002f,"FOV control differs");
            const auto module_global=dc_read32(rt,0x0C97C030);
            dc_write8(rt,0x8C1A20E0,0);dc_write16(rt,0x8C1A1B5E,0);
            call(dc_read32(rt,stage+16),stage);
            require(dc_read8(rt,state)==3 && dc_read8(rt,0x8C1A20E0)==1
                && dc_read16(rt,0x8C1A1B5E)==1,"Original phase two did not complete");
            const float projection=std::bit_cast<float>(dc_read32(rt,0x8C893864));
            const double expected=640.0/(2.0*std::tan(6371.0*6.2831853071795864769/65536.0));
            require(std::isfinite(projection) && std::abs(projection-expected)<0.002 && dc_read32(rt,0x8C6BADA8)==0x31C7,"Stage projection not restored");
            require(dc_read32(rt,0x8C7A32BC)==heap+(nodes>4?896:available?336:304) && dc_read32(rt,auxiliary)==camera
                && dc_read32(rt,camera+4)==auxiliary,"Phase two damaged heap/queue");
            require(dc_read32(rt,0x8C7A32C8)==(nodes>4?camera+13*64:0),"Continuation pool consumption differs");
            if(available){
                require(dc_read32(rt,0x8C7A32D8)==continuation && dc_read32(rt,continuation+16)==0x0C90554A
                    && dc_read32(rt,continuation+36)==heap+320,"Continuation task/work differs");
                require(dc_read32(rt,0x0C97C030)==continuation && dc_read8(rt,heap+320)==38
                    && dc_read8(rt,heap+321)==act,"Original continuation init stage/act differs");
            }else{
                require(dc_read32(rt,0x8C7A32D8)==0 && dc_read32(rt,0x0C97C030)==module_global,
                    "Exhausted pool created a continuation or initialized it");
            }
            if(nodes>4){
                constexpr unsigned parameters[]={0x0C97BDC8,0x0C97BE0C,0x0C97BE50,0x0C97BEBC,
                    0x0C97BC4C,0x0C97BC68,0x0C97BC84};
                require(dc_read32(rt,continuation+12)==camera+12*64,"Child list head differs");
                for(unsigned i=0;i<9;++i){
                    const unsigned task=camera+(4+i)*64;
                    const unsigned callback=i<4?0x0C904B82:i<7?0x0C903ABE:i==7?0x0C9056C4:0x8C0A2822;
                    require(dc_read32(rt,task)==(i?task-64:0) && dc_read32(rt,task+4)==(i==8?0:task+64)
                        && dc_read32(rt,task+8)==continuation && dc_read32(rt,task+16)==callback,"Original child links/callback differ");
                    if(i<7){
                        const unsigned work=heap+352+i*64;
                        require(dc_read32(rt,task+36)==work && dc_read32(rt,work+(i<4?20:32))==parameters[i]
                            && dc_read8(rt,parameters[i])==0,"Original child work/parameter counter differs");
                    }
                }
                require(dc_read32(rt,camera+11*64+36)==heap+800 && dc_read32(rt,heap+800)==0
                    && dc_read32(rt,0x0C97C034)==heap+800,"Original child global/work differs");
                require(dc_read32(rt,camera+12*64+32)==heap+832 && dc_read32(rt,heap+864)==0
                    && dc_read32(rt,heap+868)==0x461C4000 && dc_read32(rt,heap+872)==0,"Original child position differs");
            }else if(available)require(dc_read32(rt,continuation+12)==0,"Exhausted child pool created tasks");
            for(unsigned i=0;i<16;++i){
                require(dc_read8(rt,heap-16+i)==0xA5,"Heap guard changed");
                for(unsigned n=0;n<nodes;++n)require(dc_read8(rt,camera+n*64+48+i)==0xA5,"Task guard changed");
            }
            // State3 uses this separate player-work table. Measure the missing producer;
            // do not execute its downstream dereference against zero-filled memory.
            call(0x8C07EC7E,0);
            require(ctx.r[0]==0 && dc_read32(rt,0x8C7906E8)==0,"Fixture player-work boundary changed");
            std::cout<<"OPEN original player-work producer: lookup 8C07EC7E(0) returns null at 8C7906E8; state3 update not run\n";
            std::cout<<"PASS joined SBOARD camera act="<<act<<" nodes="<<nodes<<" shared heap/tasks; original camera file load; state0->2->3; projection="<<projection<<"; continuation and child hierarchy checked; original flags set\n";
        }catch(const std::exception&e){
            std::cerr<<"JOIN FAIL "<<e.what()<<" pc="<<std::hex<<ctx.pc<<" pr="<<ctx.pr<<std::dec<<'\n';
            dc_trace_print_history(rt,std::cerr);throw;
        }
    }
    check_startup(rt,boot_context);
    check_startup(rt,boot_context,true);
}
