#pragma once
#include <bit>
#include <cmath>
#include <optional>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <memory>
extern std::string recovery_frame_directory;
#include "sonic-texture-reference.hpp"
#include "texture-batch-check.hpp"
#include "startup-model-check.hpp"
#include "startup-motion-check.hpp"
#include "startup-mesh-check.hpp"
// Execute the original LCG in an isolated arithmetic fixture, without changing
// the live game's seed, stack, RAM or register state.
static void check_original_rng(const DCRuntime&source){
    auto owner=std::make_unique<DCRuntime>();auto&probe=*owner;
    for(unsigned i=0;i<44;++i)dc_write8(probe,0x8C10EF54+i,source.main_ram[0x10EF54+i]);
    probe.register_target(0x8C10EF54,source.targets.at(0x8C10EF54));
    require(dc_read32(probe,0x8C10EF78)==0x8C5FF840 && dc_read32(probe,0x8C10EF7C)==0x41C64E6D
        && dc_read16(probe,0x8C10EF72)==12345 && dc_read16(probe,0x8C10EF74)==32767,
        "Original random-number source literals differ");
    for(unsigned seed:{0u,1u,0x80000000u,0xFFFFFFFFu}){
        dc_write32(probe,0x8C5FF840,seed);
        for(unsigned step=0;step<8;++step){
            SH4Context c{};
            for(unsigned i=0;i<16;++i)c.r[i]=0xA5000000+i;
            c.pc=0x8C10EF54;c.pr=0xFFFFFFFF;c.r[15]=0x8CE3F000;c.macl=0xFEDCBA98;
            const auto before=c;auto expected_ram=probe.main_ram;
            const auto next=static_cast<unsigned>((std::uint64_t(seed)*1103515245ull+12345ull)&0xFFFFFFFFull);
            for(const auto&[address,value]:std::array<std::pair<unsigned,unsigned>,2>{{
                {0x8C5FF840,next},{before.r[15]-4,before.macl}}})
                for(unsigned i=0;i<4;++i)expected_ram[address-0x8C000000+i]=value>>(8*i);
            call_recompiled(c,probe,c.pc);
            require(c.r[0]==((next>>16)&32767) && dc_read32(probe,0x8C5FF840)==next,
                "Original random-number arithmetic differs");
            require(c.pc==before.pr && c.pr==before.pr && c.r[15]==before.r[15] && c.macl==before.macl,
                "Original random-number return/stack/MACL differs");
            for(unsigned i=5;i<15;++i)require(c.r[i]==before.r[i],"Original random-number preserved GPR differs");
            require(probe.main_ram==expected_ram,"Random-number routine changed unrelated RAM");
            seed=next;
        }
    }
    std::cout<<"PASS isolated original RNG: four seeds/eight draws; unsigned overflow; seed/output; MACL/stack/GPRs; exact RAM writes; live game unchanged\n";
}
// Original60A748 changes drawing-header fields and the current mode word.
// Exercise all flag branches in fresh RAM; never patch the live drawing state.
static void check_original_draw_header(const DCRuntime&source){
    auto owner=std::make_unique<DCRuntime>();auto&probe=*owner;
    for(unsigned i=0;i<0x138;++i)dc_write8(probe,0x8C60A748+i,source.main_ram[0x60A748+i]);
    probe.register_target(0x8C60A748,source.targets.at(0x8C60A748));
    const std::array<unsigned,8> literals{0x8C894384,0xFDFFFF00,0xFFE7FFFF,0x020000FF,
        0x00180000,0x8C8938AC,0x02100000,0x8C893F50};
    for(unsigned i=0;i<literals.size();++i)require(dc_read32(probe,0x8C60A860+4*i)==literals[i],
        "Original drawing-header literals differ");
    require(dc_read16(probe,0x8C60A854)==0x400 && dc_read16(probe,0x8C60A856)==0x1000
        && dc_read16(probe,0x8C60A858)==0x80 && dc_read16(probe,0x8C60A85A)==0xFF7F,
        "Original drawing-header flag literals differ");
    unsigned cases=0;
    for(unsigned inversion:{0u,0xFFFFFFFFu})for(unsigned flags:{0u,0x400u,0x1000u,0x1400u,0x36Cu})
    for(unsigned mode:{0u,0x02100028u,0x00080028u,0xFFFFFFFFu}){
        const std::array<unsigned,4> header{0x808C000E^inversion,0xC8800000^inversion,
            0x940984DB^inversion,0x100BD9F4^inversion};
        for(unsigned i=0;i<4;++i)dc_write32(probe,0x8C894384+4*i,header[i]);
        dc_write32(probe,0x8C8938AC,flags);dc_write32(probe,0x8C893F50,0x8C893F38);
        dc_write32(probe,0x8C893F38,0xDEADBEEF);
        SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0xA5000000+i;
        c.r[4]=mode;c.r[15]=0x8CE3F000;c.pc=0x8C60A748;c.pr=0xFFFFFFFF;
        c.mach=0x12345678;c.macl=0xFEDCBA98;
        const auto before=c;auto expected=probe.main_ram;
        const bool bit7=(flags&0x400) || ((flags&0x1000) && !(mode&0x02100000));
        const unsigned first=((header[0]&0xFDFFFF00)|(mode&0x0200007F))|(bit7?0x80:0);
        const unsigned third=(header[2]&0xFFE7FFFF)|(mode&0x00180000);
        const auto write=[&](unsigned address,unsigned value){
            for(unsigned i=0;i<4;++i)expected[address-0x8C000000+i]=value>>(8*i);
        };
        write(0x8C894384,first);write(0x8C89438C,third);write(0x8C893F38,mode);
        write(before.r[15]-4,before.r[14]);write(before.r[15]-8,before.r[13]);
        call_recompiled(c,probe,c.pc);
        require(probe.main_ram==expected,"Original drawing-header RAM differs");
        require(c.pc==before.pr && c.pr==before.pr && c.r[15]==before.r[15]
            && c.r[4]==mode && c.mach==before.mach && c.macl==before.macl,
            "Original drawing-header return/stack/argument/MAC differs");
        for(unsigned i=8;i<15;++i)require(c.r[i]==before.r[i],"Original drawing-header preserved GPR differs");
        if(inversion==0 && flags==0x36C && mode==0x02100028)
            require(first==0x828C0028 && third==0x941184DB,"Captured HUD header control differs");
        ++cases;
    }
    require(cases==40,"Drawing-header control count differs");
    std::cout<<"PASS isolated original drawing header:40 controls; all flag branches; exact RAM; preserved ABI; live game unchanged\n";
}
// Original task-pool and common player-work producers. Heap region, selection,
// and invocation order remain supplied; this is not normal boot.
// Reduce the newly reached controller decoder before accepting live polling.
// Explicit synthetic thresholds/packets live in a fresh runtime only.
static void check_original_controller_decode(const DCRuntime&source){
    auto owner=std::make_unique<DCRuntime>();auto&r=*owner;
    for(unsigned i=0;i<0x194;++i)dc_write8(r,0x8C605930+i,source.main_ram[0x605930+i]);
    r.register_target(0x8C605930,source.targets.at(0x8C605930));
    constexpr unsigned record=0x8CD00000,device=0x8CD00100,packet=0x8CD00200,slot=3;
    dc_write32(r,device+8,packet);dc_write8(r,0x8C893168,64);dc_write8(r,0x8C893169,32);
    unsigned cases=0;
    for(unsigned value:{0u,32u,33u,63u,64u,127u,255u})
    for(unsigned previous=0;previous<4;++previous)
    for(unsigned caps:{0u,0x000F0000u,0x003F0000u})for(unsigned polarity:{0u,1u}){
        for(unsigned i=0;i<52;++i)dc_write8(r,record+i,0xA5);
        dc_write32(r,record+4,caps);dc_write32(r,0x8C893164,polarity);
        dc_write8(r,0x8C89316A+slot,previous&1);dc_write8(r,0x8C893182+slot,previous>>1);
        const std::array<unsigned,8> bytes{0x5A,0xA5,value,255-value,0,255,64,192};
        for(unsigned i=0;i<8;++i)dc_write8(r,packet+8+i,bytes[i]);
        SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0xB5000000+i;
        c.pc=0x8C605930;c.pr=0xFFFFFFFF;c.r[15]=0x8CE3F000;
        c.r[4]=record;c.r[5]=device;c.r[6]=slot;const auto before=c;
        auto expected=r.main_ram;
        const auto write=[&](unsigned address,unsigned value,unsigned size){
            for(unsigned i=0;i<size;++i)expected[(address&0xFFFFFF)+i]=value>>(8*i);
        };
        const auto pressed=[](unsigned v,bool old){return v>=64 || (v>32 && old);};
        const bool left=pressed(bytes[3],previous&1),right=pressed(bytes[2],previous&2);
        unsigned buttons=0xFFFFA55A;
        if(left)buttons&=~0x20000u;if(right)buttons&=~0x10000u;
        if(!polarity)buttons=~buttons;
        write(record+8,buttons,4);write(0x8C89316A+slot,left,1);write(0x8C893182+slot,right,1);
        for(unsigned i=0;i<6;++i){
            const unsigned capability=0x10000u<<i;
            const int value=(caps&capability)?int(bytes[i+2])-(i>=2?128:0):0;
            write(record+24+i*2,static_cast<unsigned>(value),2);
        }
        for(unsigned i=0;i<3;++i)write(before.r[15]-4*(i+1),before.r[14-i],4);
        call_recompiled(c,r,c.pc);
        require(r.main_ram==expected,"Original controller decoder RAM differs");
        require(c.pc==before.pr && c.pr==before.pr && c.r[15]==before.r[15],"Controller decoder return/stack differs");
        for(unsigned i=8;i<15;++i)require(c.r[i]==before.r[i],"Controller decoder preserved GPR differs");
        ++cases;
    }
    require(cases==168,"Controller decoder case count differs");
    std::cout<<"PASS isolated original controller decoder:168 controls; trigger hysteresis; polarity; capability-gated axes; exact RAM/ABI; live game unchanged\n";
}
static void observe_cadence_clock(const char*phase,const SH4Context&c,DCRuntime&r){
    std::cerr<<"CADENCE CLOCK phase="<<phase<<" pc="<<std::hex<<c.pc
        <<" sr="<<c.sr<<" spc="<<c.spc<<" event="<<r.holly_last_intevt
        <<" ack="<<r.holly_last_ack_a_value<<" ack_pc="<<r.holly_last_ack_a_pc
        <<" pending=";for(auto value:r.holly_pending)std::cerr<<value<<',';
    std::cerr<<" masks=";for(const auto&row:r.holly_masks)for(auto value:row)std::cerr<<value<<',';
    std::cerr<<std::dec<<" counter="<<dc_read32(r,0x8C758F40)
        <<" cycles="<<r.device_clock_sh4_cycles<<" ticks="<<r.device_clock_pvr_ticks
        <<" idle="<<r.device_clock_idle_cycles<<" spg_cycles="<<r.pvr_spg_sh4_cycles
        <<" line="<<r.pvr_spg_last_scanline<<" line_cycles="<<r.pvr_spg_last_line_cycles
        <<" frame_cycles="<<r.pvr_spg_last_frame_cycles<<" vblanks="<<r.pvr_vblanks
        <<" raised="<<r.holly_irq_raised<<" taken="<<r.holly_irq_taken<<'\n';
}
static void check_startup(DCRuntime&rt,const SH4Context&boot_context,bool scheduler=false){
    // 0x0CD00000 is the original loader's compressed-data scratch area.
    constexpr unsigned heap=0x8CE40000,size=0x80000,pool=heap+16;
    // Preserve the established guest privilege/register-bank state. The original
    // clipping code switches SR.RB, which depends on SR.MD. This restores caller
    // context fidelity;223153 shows it does not by itself fix the bad UV read.
    require((boot_context.sr&0x40000000u)!=0,"Startup caller lost privileged SH-4 context");
    SH4Context ctx=boot_context;
    std::cerr<<"STARTUP INHERITED SR="<<std::hex<<ctx.sr<<" FPSCR="<<ctx.fpscr<<std::dec<<'\n';
    std::optional<GraphicsReservations> graphics;
    const auto capture_frame=[&]{
    std::cerr<<"FRAME RESULT logical="<<rt.pvr_logical_frames<<" completes="<<rt.pvr_render_completes
        <<" vblanks="<<rt.pvr_vblanks<<" flips="<<rt.pvr_page_flips<<'\n';
    std::cerr<<"FRAME RASTER vertices="<<rt.pvr_vertices<<" triangles="<<rt.pvr_triangles
        <<" background="<<rt.pvr_background_planes<<" background_failures="<<rt.pvr_background_failures
        <<" 3d="<<rt.pvr_3d_triangles<<" onscreen="<<rt.pvr_3d_onscreen_triangles
        <<" offscreen="<<rt.pvr_3d_offscreen_triangles<<" covered="<<rt.pvr_3d_pixels_covered
        <<" depth_pass="<<rt.pvr_3d_depth_pass_pixels<<" depth_fail="<<rt.pvr_3d_depth_fail_pixels
        <<" written="<<rt.pvr_3d_pixels_written<<'\n';
    std::cerr<<"FRAME MODES depth=";for(auto n:rt.pvr_depth_mode_triangles)std::cerr<<n<<',';
    std::cerr<<" cull=";for(auto n:rt.pvr_cull_mode_triangles)std::cerr<<n<<',';
    std::cerr<<" list=";for(auto n:rt.pvr_list_triangles)std::cerr<<n<<',';
    std::cerr<<" vertex_type=";for(auto n:rt.pvr_vertex_type_triangles)std::cerr<<n<<',';
    std::cerr<<'\n';
    for(const auto&item:std::array<std::pair<const char*,const std::vector<std::uint32_t>*>,3>{{
        {"registration",&rt.pvr_framebuffer},{"render",&rt.pvr_render_buffer},{"present",&rt.pvr_present_buffer}}}){
        std::cerr<<"FRAME BUFFER "<<item.first<<" pixels="<<item.second->size()<<'\n';
        if(item.second->size()!=640*480)continue;
        const auto path=recovery_frame_directory+"/scene-"+std::to_string(rt.pvr_isp_start_writes)+"-"+item.first+".ppm";
        std::ofstream image(path,std::ios::binary);require(bool(image),"Cannot create frame diagnostic");
        image<<"P6\n640 480\n255\n";
        for(auto pixel:*item.second){const char rgb[]{char(pixel>>16),char(pixel>>8),char(pixel)};image.write(rgb,3);}
        require(bool(image),"Frame diagnostic write failed");
    }
    };
    auto call=[&](unsigned entry,unsigned r4=0,unsigned r5=0,unsigned r6=0){
        ctx.pc=entry;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=r4;ctx.r[5]=r5;ctx.r[6]=r6;const auto before=ctx;
        const auto irq_before=rt.holly_irq_taken;
        const bool frame_entry=entry==0x8C608DF0 || entry==0x8C0534F0;
        if(entry==0x8C608DF0)std::cerr<<"FRAME OPTIONAL CALLBACK6771B4="<<std::hex
            <<dc_read32(rt,0x8C6771B4)<<std::dec<<'\n';
        unsigned continuations=0,graphics_interrupts=0,maple_interrupts=0;
        call_recompiled(ctx,rt,entry);
        // The public runner resumes at saved guest PCs after an async redirect.
        // Preserve the live stack; do not replay the interrupted entry.
        if(rt.sh4_async_redirect){
            while(ctx.pc!=before.pr){
                if(entry==0x8C0506BA || entry==0x8C0534F0)observe_cadence_clock(continuations==128?"exhausted":"continue",ctx,rt);
                require(++continuations<=128,"Startup interrupt continuation budget exhausted");
                const auto next=ctx.pc;rt.sh4_async_redirect=false;
                if(next==ctx.vbr+0x600){
                    const auto enabled=[&](unsigned row){
                        unsigned bits=0;for(unsigned bank=0;bank<3;++bank)
                            bits|=rt.holly_pending[bank]&rt.holly_masks[row][bank];
                        return bits;
                    };
                    const bool graphics=rt.holly_last_intevt==0x320 && enabled(2)!=0;
                    const bool maple=rt.holly_last_intevt==0x360 && enabled(2)==0
                        && enabled(1)==0x1000 && (rt.holly_pending[0]&rt.holly_masks[1][0]&0x1000)!=0;
                    require(graphics || maple,"Startup interrupt lacks expected enabled graphics/Maple source");
                    graphics_interrupts+=graphics;maple_interrupts+=maple;
                    std::cerr<<"STARTUP IRQ SOURCE event="<<std::hex<<rt.holly_last_intevt
                        <<" pending="<<rt.holly_pending[0]<<" enabled4="<<enabled(1)
                        <<" enabled6="<<enabled(2)<<std::dec<<'\n';
                }
                std::cerr<<"STARTUP ASYNC CONTINUE pc="<<std::hex<<next<<" spc="<<ctx.spc
                    <<" vbr="<<ctx.vbr<<" irq_event="<<rt.holly_last_intevt<<std::dec<<'\n';
                if(frame_entry && next==0x8C645CD0){
                    std::cerr<<"FRAME WAIT STATE pc="<<std::hex<<ctx.pc<<" sr="<<ctx.sr<<" values=";
                    for(unsigned address:{0x8C8A733Cu,0x8C8A7340u,0x8C8A723Cu,0x8C8A7240u,
                        0x8C8A71F8u,0x8C8A71FCu,0x8C8A7318u,0x8C8A731Cu,0x8C8A7320u,0x8C8A7324u})
                        std::cerr<<address<<':'<<dc_read32(rt,address)<<',';
                    std::cerr<<" pending=";for(auto value:rt.holly_pending)std::cerr<<value<<',';
                    std::cerr<<std::dec<<" device_clock="<<rt.device_clock_enabled
                        <<" cycles="<<rt.device_clock_sh4_cycles<<" pvr_ticks="<<rt.device_clock_pvr_ticks
                        <<" dma="<<rt.ch2_dma_runs<<" dma_bytes="<<rt.ch2_dma_bytes
                        <<" staged="<<rt.pvr_ta_stream.size()<<" parsed="<<rt.ta_packets
                        <<" irq_taken="<<rt.holly_irq_taken<<'\n';
                    // Preserve the exact direct+DMA packet stream before parsing.
                    // Inspection must not change guest colors or renderer state.
                    std::ofstream packets(recovery_frame_directory+"/frame-"+std::to_string(rt.pvr_isp_start_writes+1)+"-packets.bin",std::ios::binary);
                    require(bool(packets),"Cannot create raw packet diagnostic");
                    for(const auto&packet:rt.pvr_ta_stream)
                        packets.write(reinterpret_cast<const char*>(packet.bytes.data()),packet.bytes.size());
                    require(bool(packets),"Raw packet diagnostic write failed");
                }
                if(entry==0x8C608DF0 && next==0x8C608E66){
                    const auto maple=dc_read32(rt,0x8C8AC634),physical=maple&0x1FFFFFFF;
                    std::cerr<<"ORIGINAL INPUT WORK AREA="<<std::hex<<maple<<std::dec<<'\n';
                    require(physical>=0x0C000000 && physical+0x44C+24*120<=0x0D000000,
                        "Original controller poll lacks Maple work-area producer at 8C8AC634");
                }
                call_recompiled(ctx,rt,next);
            }
        }
        if(ctx.pc!=before.pr || ctx.pr!=before.pr || ctx.r[15]!=before.r[15]){
            std::cerr<<"STARTUP RETURN STATE entry="<<std::hex<<entry<<" pc="<<ctx.pc<<" pr="<<ctx.pr
                <<" sp="<<ctx.r[15]<<" vbr="<<ctx.vbr<<" sr="<<ctx.sr<<" spc="<<ctx.spc<<" ssr="<<ctx.ssr
                <<" irq_event="<<rt.holly_last_intevt<<" pending=";
            for(auto value:rt.holly_pending)std::cerr<<value<<',';
            std::cerr<<" masks=";for(const auto&row:rt.holly_masks)for(auto value:row)std::cerr<<value<<',';
            std::cerr<<std::dec<<" irq_taken="<<rt.holly_irq_taken<<'\n';
            throw std::runtime_error("Startup return/stack differs");
        }
        for(unsigned i=8;i<15;++i)require(ctx.r[i]==before.r[i],"Startup preserved GPR differs");
        if(continuations){
            require(ctx.vbr==boot_context.vbr && !rt.sh4_async_redirect,
                "Original interrupt continuation state differs");
            if(frame_entry){
                require(rt.holly_irq_taken>irq_before && rt.device_clock_enabled && !rt.device_clock_host_sync,
                    "Frame interrupt/guest-clock state differs");
                std::cout<<"PASS original frame interrupt continuation: verified graphics/Maple sources; interrupts="
                    <<(rt.holly_irq_taken-irq_before)<<" saved-PC resumes="<<continuations
                    <<"; final stack/PR/preserved GPRs restored; no entry replay\n";
            }else if(entry==0x8C0506BA){
                // The running wrapper waits for vblank and also services Maple.
                // Count validated vector entries rather than assuming one IRQ.
                require(rt.holly_irq_taken-irq_before==graphics_interrupts+maple_interrupts
                    && graphics_interrupts>0 && rt.device_clock_enabled && !rt.device_clock_host_sync,
                    "Running wrapper interrupt accounting differs");
                require(ctx.r[0]==0 && dc_read32(rt,0x8C758F48)==2,
                    "Supplied running wrapper result/update count differs");
                for(unsigned i=12;i<16;++i)require(ctx.fr_bits[i]==before.fr_bits[i],
                    "Running wrapper preserved FPR differs");
                std::cout<<"PASS original running wrapper interrupt continuation: graphics="
                    <<graphics_interrupts<<" maple="<<maple_interrupts<<" saved-PC resumes="<<continuations
                    <<"; two supplied updates; stack/PR/preserved GPRs/FPRs restored; frame pending\n";
            }else{
                require(rt.holly_irq_taken==irq_before+1,"Startup interrupt count differs");
                std::cout<<"PASS original interrupt continuation: event0320; copied vector; saved-PC resumes="<<continuations
                    <<"; final stack/PR/preserved GPRs restored; interrupted entry not replayed\n";
            }
        }
        return ctx.r[0];
    };
    try{
        // Clear the prior isolated case's auxiliary handle through its original cleanup callback.
        call(0x8C0A2B70,dc_read32(rt,0x8C1C8270));
        require(dc_read32(rt,0x8C1C8270)==0,"Original auxiliary cleanup differs");
        dc_zero_bytes(rt,heap,size+16);
        for(unsigned i=0;i<16;++i){dc_write8(rt,heap-16+i,0xA5);dc_write8(rt,heap+size+16+i,0xA5);}
        dc_write32(rt,heap+8,heap+size);dc_write32(rt,heap+size+12,heap);
        dc_write32(rt,0x8C7A32BC,heap);
        call(0x8C09A0EA);
        require(dc_read32(rt,0x8C7A32C4)==pool && dc_read32(rt,0x8C7A32C8)==pool,"Original pool allocation differs");
        for(unsigned i=0;i<4096;++i){
            const unsigned task=pool+i*48;
            require(dc_read32(rt,task)==(i==4095?0:task+48) && dc_read32(rt,task+4)==(i?task-48:0),"Original pool links differ");
            for(unsigned off=8;off<48;off+=4)require(dc_read32(rt,task+off)==0,"Original pool field not initialized");
        }
        for(unsigned i=0;i<8;++i)require(dc_read32(rt,0x8C7A32CC+i*4)==0,"Original queues not initialized");
        require(dc_read32(rt,0x8C7A32BC)==heap+0x30010,"Original pool heap use differs");
        std::cout<<"PASS original task pool: 4096 linked 48-byte nodes; eight queues; heap allocation checked\n";
        dc_write16(rt,0x8C74D43A,38);dc_write16(rt,0x8C74D43C,0);dc_write16(rt,0x8C74D448,0);
        call(0x8C051120);
        const unsigned task=pool;
        require(dc_read32(rt,task+16)==0x8C0A93DC && dc_read32(rt,task+40)!=0,"Original selector task/callback differs");
        std::cout<<"PASS original character selector: Sonic task/callback and additional work allocated\n";
        const unsigned main=dc_read32(rt,task+32),secondary=dc_read32(rt,task+36);
        require(main && secondary,"Player task work missing");
        require(dc_read32(rt,0x8C7A32D0)==task && dc_read32(rt,task)==0,"Original selector group1 differs");
        require(dc_read32(rt,main+24)==0xC000,"Original spawn yaw differs");
        for(unsigned i=0;i<12;++i)require(dc_read8(rt,main+32+i)==dc_read8(rt,0x8C1A0720+i),"Original stage38/act0 spawn differs");
        require(dc_read32(rt,0x8C790688)==main && dc_read32(rt,0x8C7906A8)==secondary,"Selector player tables differ");
        if(scheduler){
            std::cout<<"BEGIN original group1 scheduler with selected Sonic task\n";
            // Original outer graphics setup produces matrix, projection, camera,
            // clipping/render state and texture manager in its own call order.
            const std::array<unsigned,3> background_headers{dc_read32(rt,0x8C8A7174),
                dc_read32(rt,0x8C8A7178),dc_read32(rt,0x8C8A717C)};
            // 056420 calls this producer before 056242: original literal buffer
            // at056574 and R5=16<<8. This remains a supplied startup slice.
            // Earlier calls in056420 configure the polygon buffers; preserve
            // its exact R4-R7 and fifth stack argument for608B0E.
            call(0x8C60C954,0x8C7B0B60,0x40000);
            ctx.r[7]=0;dc_write32(rt,0x8CE3F000,0x8000);
            call(0x8C608B0E,0xFFFFFFFF,0,0x20000);
            const auto polygon_allocation=dc_read32(rt,0x8C894034+36);
            require((polygon_allocation&0x1FFFFFFF)>=0x0CF00000 && (polygon_allocation&0x1FFFFFFF)+0xA02F0<=0x0D000000,
                "Original polygon allocation absent/outside system heap");
            std::cout<<"PASS original polygon setup: system heap and original allocation; buffer="<<std::hex<<polygon_allocation<<std::dec<<'\n';
            call(0x8C612AF0,0x8C7B0B60,4096);
            require(dc_read32(rt,0x8C8938CC)==0x8C7B0B60 && dc_read32(rt,0x8C8938D0)==4096,
                "Original vertex workspace producer differs");
            std::cout<<"PASS original vertex workspace: source caller buffer and capacity4096; original producer returned\n";
            call(0x8C056242);
            require(dc_read32(rt,0x8C893918)==128 && dc_read32(rt,0x8C89391C)==1
                && dc_read32(rt,0x8C893878)==0x8C7F0B60 && dc_read32(rt,0x8C89387C)==0x8C7F0B60,
                "Original matrix stack initialization differs");
            // 6440F0 supplies origin and direction (0,0,-1); 644280 turns
            // the camera by pi about Y. 653074 prepends scale(-1,-1,1).
            for(unsigned i=0;i<16;++i){
                const double expected=i==0 || i==15?1:(i==5 || i==10?-1:0);
                const double actual=std::bit_cast<float>(dc_read32(rt,0x8C7F0B60+i*4));
                require(std::isfinite(actual) && std::abs(actual-expected)<0.00001,
                    "Original default camera matrix differs");
            }
            auto graphics_float=[&](unsigned address,double expected,double tolerance=0.00001){
                const double actual=std::bit_cast<float>(dc_read32(rt,address));
                if(!std::isfinite(actual) || std::abs(actual-expected)>tolerance)
                    throw std::runtime_error("Graphics state differs address="+std::to_string(address)
                        +" expected="+std::to_string(expected)+" actual="+std::to_string(actual));
            };
            graphics_float(0x8C78CA80,640);graphics_float(0x8C78CA84,480);
            graphics_float(0x8C893868,640);graphics_float(0x8C89386C,480);
            graphics_float(0x8C893870,320);graphics_float(0x8C893874,240);
            const double focal=640/(2*std::tan((0x31C7>>1)*6.2831853071795864769/65536));
            graphics_float(0x8C893864,focal,0.01);
            graphics_float(0x8C893890,3);graphics_float(0x8C893894,3000);
            for(unsigned i=0;i<6;++i)graphics_float(0x8C78CA44+i*4,i==5?-1:0);
            require(dc_read32(rt,0x8C8EE180)==0x8C78CA04,"Default camera pointer differs");
            // 665040 packs three background vertices after the three source
            // header words. This is RAM packet construction, not presentation.
            const unsigned z=dc_read32(rt,0x8C6090C4);
            const std::array<unsigned,15> packet{background_headers[0],background_headers[1],background_headers[2],
                0,0,z,0,0x44200000,0,z,0,0,0x43F00000,z,0};
            for(unsigned i=0;i<packet.size();++i)require(dc_read32(rt,0x8C8A7078+i*4)==packet[i],
                "Original background packet differs");
            require(dc_read32(rt,0x8C8A7170)==60 && dc_read32(rt,0x8C8A7184)==z,"Background length/depth differs");
            std::cout<<"PASS original graphics setup: camera matrix; 640x480 screen; projection; near/far 3/3000; exact 60-byte background packet; no Sonic draw claim\n";
            require(dc_read32(rt,0x8C6771C4)==0x8C7649F4 && dc_read32(rt,0x8C6771C8)==2048,
                "Original texture manager table/capacity differs");
            for(unsigned i=0;i<2048;++i){
                const unsigned entry=0x8C7649F4+i*68;
                require(dc_read32(rt,entry)==0xFFFFFFFF && dc_read32(rt,entry+4)==0xFFFFFFFF
                    && dc_read16(rt,entry+64)==0,"Original texture manager free record differs");
            }
            require(dc_read32(rt,0x8C677A58)==0 && dc_read32(rt,0x8C893920)==0,"Original texture manager reset differs");
            std::cout<<"PASS original texture manager: 2048 free 68-byte records at original caller address; produced by original outer graphics setup\n";
            graphics.emplace(rt);
            std::cout<<"PASS original graphics VRAM reservations: source-derived polygon and double-framebuffer extents; owners/free lists checked\n";
            for(unsigned i=0;i<16;++i){dc_write8(rt,0x0CD00000-16+i,0xA5);dc_write8(rt,0x0CD00000+sizeof(sonic_decoded)+i,0xA5);}
            const auto reads=rt.gd_sector_reads,bytes=rt.gd_bytes_read;
            const auto vram_before=rt.vram;
            const auto vram_epoch=rt.pvr_vram_write_epoch;
            const StartupModelReference model_reference(rt);
            const StartupMotionReference motion_reference(rt);
            const unsigned collision_count=dc_read16(rt,0x8C749418);
            const unsigned history_index=dc_read8(rt,0x8C790708);
            require(collision_count<8,"Collision fixture table already full");
            call(0x8C09A42C,1);
            require(dc_read32(rt,0x8C1C5A78)==0,"Scheduler did not clear current callback");
            check_texture_batches(rt,vram_before,*graphics);
            model_reference.check(rt,dc_read32(rt,secondary));
            motion_reference.check(rt,dc_read32(rt,secondary));
            check_startup_auxiliary(rt,pool);
            require(rt.pvr_vram_write_epoch>vram_epoch,"VRAM write evidence missing");
            require(rt.gd_sector_reads-reads==21 && rt.gd_bytes_read-bytes==21*2048,"Texture batch read count differs");
            // The second file reuses both original compressed and decoded scratch.
            for(unsigned i=0;i<0x7800;++i)require(dc_read8(rt,0x0CEF7000+i)==sonic_encoded[i],"Retained Sonic encoded prefix differs");
            for(unsigned i=0;i<sizeof(effect_encoded);++i)require(dc_read8(rt,0x0CEFE800+i)==effect_encoded[i],"Effect encoded bytes differ");
            for(unsigned i=0;i<sizeof(sonic_decoded);++i){
                const auto expected=i<sizeof(effect_decoded)?effect_decoded[i]:sonic_decoded[i];
                require(dc_read8(rt,0x0CD00000+i)==expected,"Texture decoded scratch reuse differs");
            }
            for(unsigned i=0;i<16;++i)require(dc_read8(rt,0x0CD00000-16+i)==0xA5 && dc_read8(rt,0x0CD00000+sizeof(sonic_decoded)+i)==0xA5,"Texture decode guard changed");
            const unsigned handle=dc_read32(rt,0x8C7A4D04);
            require(handle && dc_read16(rt,handle+76)==0,"Texture file handle not closed");
            constexpr char filename[]="SON_EFF.PRS";
            for(unsigned i=0;i<sizeof(filename);++i)require(dc_read8(rt,0x8CE3B680+i)==filename[i],"Original formatted texture filename differs");
            require(dc_read8(rt,0x8C1C61EC)==1,"Texture resource counter did not complete");
            const unsigned descriptor=dc_read32(rt,main+56),work=dc_read32(rt,secondary);
            require(descriptor>=heap && descriptor+28<heap+size,"Original descriptor outside heap");
            const unsigned record=dc_read32(rt,descriptor+12);
            require(record>=descriptor+28 && record+52<heap+size,"Original copied record outside heap");
            require(dc_read16(rt,descriptor)==0 && dc_read16(rt,descriptor+2)==32
                && dc_read16(rt,descriptor+4)==1 && dc_read32(rt,descriptor+16)==task,
                "Original descriptor mode/flags/count/owner differs");
            const float vx=std::bit_cast<float>(dc_read32(rt,work+56)),vy=std::bit_cast<float>(dc_read32(rt,work+60)),vz=std::bit_cast<float>(dc_read32(rt,work+64));
            const float radius=std::bit_cast<float>(dc_read32(rt,work+272))+std::sqrt(vx*vx+vy*vy+vz*vz);
            require(std::abs(std::bit_cast<float>(dc_read32(rt,descriptor+8))-radius)<0.000001f,"Final collision radius differs");
            require(dc_read16(rt,descriptor+20)==65535 && dc_read16(rt,descriptor+22)==65535
                && dc_read32(rt,descriptor+24)==0,"Collision descriptor reset differs");
            require(dc_read16(rt,0x8C749418)==collision_count+1
                && dc_read32(rt,0x8C749428+collision_count*4)==descriptor,"Collision registration differs");
            for(unsigned i=0;i<52;++i){
                if(i>=8 && i<20)continue; // Checked as world-space collision center below.
                const unsigned source=(i>=20 && i<24)?work+268+i-20:0x8C1C9BB8+i;
                unsigned expected=dc_read8(rt,source);
                if(i==1)expected&=254; // 02F780 clears record bit8.
                if(i==4)expected&=239;
                if(i==44 || i==45)expected=255;
                if(i>=48)expected=0;
                require(dc_read8(rt,record+i)==expected,"Original descriptor record copy/update differs");
            }
            require(dc_read32(rt,main+20)==0 && dc_read32(rt,main+28)==0,"Collision fixture tilt differs");
            for(unsigned k=0;k<3;++k){
                const float expected=std::bit_cast<float>(dc_read32(rt,main+32+k*4))
                    +(k==1?std::bit_cast<float>(dc_read32(rt,work+284)):0.0f);
                const float actual=std::bit_cast<float>(dc_read32(rt,record+8+k*4));
                require(std::isfinite(actual) && std::abs(actual-expected)<0.000001f,"Original collision center differs");
            }
            std::cout<<"PASS original collision center: spawn plus original height offset; yaw-only fixture; remaining descriptor bytes preserved\n";
            require(dc_read8(rt,main)==1 && dc_read32(rt,work+320)==0x8C1C92D8,
                "Sonic descriptor continuation differs");
            std::cout<<"PASS original descriptor: 28-byte descriptor, 52-byte record with original parameter update, task linkage, state1\n";
            std::cout<<"PASS original Sonic texture load: SONIC/SON_EFF; 21 sectors; exact retained encoded bytes and decoded scratch reuse; guards; closed handle; resource counter1\n";
            const unsigned next_history=(history_index+1)&255;
            require(dc_read8(rt,0x8C790708)==next_history,"Original position history index differs");
            for(unsigned k=0;k<3;++k){
                require(dc_read32(rt,0x8C790710+next_history*96+k*4)==dc_read32(rt,main+32+k*4),"Position history differs");
                require(dc_read32(rt,0x8C796710+next_history*96+k*4)==dc_read32(rt,main+20+k*4),"Rotation history differs");
            }
            require(dc_read16(rt,0x8C758F70)==0,"Ground-query fixture gained candidates; expand its oracle");
            require(dc_read32(rt,work+492)==0x49742400 && dc_read32(rt,work+496)==0xC9742400
                && dc_read32(rt,work+484)==0 && dc_read32(rt,work+488)==0,"Empty ground-query result differs");
            std::cout<<"PASS original group1 return: stack/registers restored; callback cleared; collision registration/radius; position/rotation history; empty ground-query result; no full scheduler tick\n";
        }else call(0x8C06DFF0,0,task);
        const unsigned work=call(0x8C07EC7E,0);
        require(work>=heap && work+512<heap+size && work==dc_read32(rt,secondary),"Original player-work producer differs");
        require(dc_read32(rt,0x8C790688)==main && dc_read32(rt,0x8C7906A8)==secondary
            && dc_read32(rt,0x8C7906C8)==task && dc_read32(rt,0x8C7906E8)==work,"Original player tables differ");
        for(unsigned i=0;i<132;++i)require(dc_read8(rt,work+156+i)==dc_read8(rt,0x8C1C1D1C+i),"Original player parameter copy differs");
        require(dc_read32(rt,work+312)==work+56 && dc_read32(rt,work+316)==work,"Original player self pointers differ");
        require(dc_read32(rt,work+56)==0 && dc_read32(rt,work+60)==0 && dc_read32(rt,work+64)==0,"Original velocity initialization differs");
        require(dc_read32(rt,secondary+60)==0x3F800000 && dc_read32(rt,main+44)==0x3F800000
            && dc_read32(rt,main+48)==0x3F800000 && dc_read32(rt,main+52)==0x3F800000,"Original player scale differs");
        for(unsigned i=0;i<16;++i)require(dc_read8(rt,heap-16+i)==0xA5 && dc_read8(rt,heap+size+16+i)==0xA5,"Startup heap guard changed");
        if(scheduler){
            std::cout<<"PASS original scheduler prefix: group1 -> Sonic -> SONIC/SON_EFF 57 textures; auxiliary tasks/model setup; motion pose; collision update; display culled; group1 returned; no full scheduler tick\n";
            // Original stage lighting initializer selects the current stage/act,
            // initializes the tables, and loads their original PL files. The
            // surrounding startup order remains supplied in this experiment.
            std::cerr<<"BEGIN original stage lighting012240 stage_act="<<std::hex<<call(0x8C051540)<<std::dec<<'\n';
            const auto check_lighting_files=[&]{
                for(unsigned i=0;i<sizeof(pl1c0b_reference);++i)
                    require(dc_read8(rt,0x8C039200+i)==pl1c0b_reference[i],"Original lighting palette differs from disc bytes");
                for(unsigned i=0;i<sizeof(sl1c0b_reference);++i)
                    require(dc_read8(rt,0x8C6B2CC4+i)==sl1c0b_reference[i],"Original lighting parameters differ from disc bytes");
                std::cout<<"PASS original lighting file bytes: PL1C0B12672 and SL1C0B1920; independently read owner disc; no table injection\n";
            };
            try{call(0x8C012240);}catch(...){check_lighting_files();throw;}
            check_lighting_files();
            unsigned lit_entries=0;
            for(unsigned offset=0;offset<6*2048;offset+=4)
                lit_entries+=(dc_read32(rt,0x8C039200+offset)&0xFFFFFF)!=0;
            std::cerr<<"RETURN original stage lighting012240 nonblack_words="<<lit_entries<<'\n';
            // Join the original camera task to the completed character's live
            // pool/work. Camera records are loaded through the original provider.
            call(0x8C09E98E,0x8C15D054,0x0CB50000);
            for(unsigned i=0;i<sizeof(camera_reference);++i)
                require(dc_read8(rt,0x0CB50000+i)==camera_reference[i],"Shared camera source bytes differ");
            call(0x8C0196FE);
            const unsigned camera_task=dc_read32(rt,0x8C7A32CC);
            require(camera_task==pool+240 && dc_read32(rt,camera_task+16)==0x8C01993A,
                "Camera task not from shared original pool");
            const unsigned camera_work=dc_read32(rt,camera_task+32);
            require(camera_work>=heap && camera_work+64<heap+size && dc_read8(rt,camera_work)==0,
                "Original camera work not initialized");
            call(0x8C09A42C,0);
            require(dc_read8(rt,camera_work)==1 && dc_read32(rt,0x8C18E620)==camera_work,
                "Camera phase0 did not register its work");
            std::cout<<"PASS original camera phase0: shared player/heap/task pool; original group0 returned; camera state1\n";
            call(0x8C09A42C,0);
            require(dc_read8(rt,camera_work)==2,"Camera phase1 did not advance naturally");
            for(unsigned i=0;i<50;++i)for(unsigned k=0;k<3;++k)
                require(dc_read32(rt,0x8C6BADF8+i*12+k*4)==dc_read32(rt,main+32+k*4),
                    "Original camera position-history initialization differs");
            require(dc_read32(rt,0x8C6BB050)==0 && dc_read32(rt,camera_task+20)==0x8C01C416,
                "Original camera history cursor/display callback differs");
            std::cout<<"PASS original camera phase1: original group0 returned; camera state2; 50 source-derived position-history records\n";
            call(0x8C09A42C,0);
            require(dc_read8(rt,camera_work)==2 && dc_read32(rt,0x8C1C5A78)==0,
                "Camera phase2 state/return differs");
            std::cout<<"PASS original camera phase2: original group0 returned; state2; callback cleared; view values not independently verified\n";
            std::cout<<"CAMERA view record:";
            for(unsigned i=0;i<6;++i){
                const float value=std::bit_cast<float>(dc_read32(rt,0x8C78CA44+i*4));
                require(std::isfinite(value),"Camera produced non-finite view record");
                std::cout<<' '<<value;
            }
            std::cout<<'\n';
            {
                const auto render_flags=dc_read32(rt,0x8C8938E0);
                const auto render_control=dc_read32(rt,0x8C8938E4);
                StartupDisplayObservation observation(rt);
                const auto display_sq_before=rt.sq_commits,display_ta_before=rt.ta_packets;
                try{call(0x8C0AFD9A,task);}
                catch(const std::exception&e){
                    std::cerr<<"OBSERVED display boundary: "<<e.what()<<" pc="<<std::hex<<ctx.pc
                        <<" pr="<<ctx.pr<<" sp="<<ctx.r[15]<<" r4="<<ctx.r[4]<<std::dec
                        <<" pointer_updates="<<observation.updates
                        <<" staged_packets="<<(rt.pvr_ta_stream.size()-observation.strip_reference.prefix.size())<<'\n';
                    throw;
                }
                require(observation.updates==53,"Display material completion count changed; inspect coverage");
                require(rt.pvr_ta_stream.size()==observation.strip_reference.prefix.size()+819,
                    "Display staged FIFO count changed; inspect coverage");
                require(dc_read32(rt,0x8C8938E0)==render_flags && dc_read32(rt,0x8C8938E4)==render_control,
                    "Original display render state not restored");
                std::cout<<"PASS original mesh pointer updates: 53 original routine returns; each destination/value checked against incoming cursor/segment\n";
                std::cout<<"PASS original post-camera visibility: render branch returned; first projection and strip packets checked; full geometry open\n";
                std::cout<<"PASS original character display return: 0AFD9A; ABI/stack/preserved GPRs; render flags/control restored; 819 staged FIFO packets; no rasterized frame\n";
                std::cout<<"OBSERVED display counters: SQ="<<(rt.sq_commits-display_sq_before)
                    <<" parsed_TA="<<(rt.ta_packets-display_ta_before)
                    <<" staged_TA="<<(rt.pvr_ta_stream.size()-observation.strip_reference.prefix.size())<<'\n';
            }
            // Original frame-boundary entry used by the game. Invocation order
            // is still supplied; do not force ready-scene state or STARTRENDER.
            std::cerr<<"BEGIN original frame boundary608DF0 staged="<<rt.pvr_ta_stream.size()
                <<" parsed="<<rt.ta_packets<<" starts="<<rt.pvr_isp_start_writes<<'\n';
            require(!rt.device_clock_enabled,"Frame fixture clock unexpectedly already enabled");
            const auto parsed_before=rt.ta_packets,starts_before=rt.pvr_isp_start_writes;
            const auto frames_before=rt.pvr_logical_frames,completes_before=rt.pvr_render_completes;
            const auto dma_before=rt.ch2_dma_runs,dma_bytes_before=rt.ch2_dma_bytes;
            const bool prior_detailed_stats=rt.pvr_detailed_stats;
            rt.pvr_detailed_stats=true; // Observe raster decisions; no rendering override.
            // Public runtime guest-cycle scheduler: VBlank must advance during
            // the original scene wait. No host-time catchup or injected event.
            dc_device_clock_enable(rt,true,false);
            try{call(0x8C608DF0);}catch(...){capture_frame();dc_device_clock_enable(rt,false);throw;}
            dc_device_clock_enable(rt,false);
            std::cerr<<"RETURN original frame boundary608DF0 staged="<<rt.pvr_ta_stream.size()
                <<" parsed="<<rt.ta_packets<<" starts="<<rt.pvr_isp_start_writes<<'\n';
            capture_frame();
            rt.pvr_detailed_stats=prior_detailed_stats;
            require(rt.pvr_ta_stream.empty() && rt.ta_packets-parsed_before==1483
                && rt.pvr_isp_start_writes-starts_before==1 && rt.pvr_logical_frames-frames_before==1
                && rt.pvr_render_completes-completes_before==1 && rt.ch2_dma_runs-dma_before==2
                && rt.ch2_dma_bytes-dma_bytes_before==21216,"Original frame handoff changed; inspect DMA/render evidence");
            unsigned nonblack=0;
            for(auto pixel:rt.pvr_render_buffer)nonblack+=(pixel&0xFFFFFF)!=0;
            require(nonblack>0 && nonblack<640*480,"Fixture render lacks bounded nonblack output");
            std::cout<<"PASS original frame handoff: 1483 parsed packets; two DMA/21216 bytes; one render completion; nonblack_pixels="
                <<nonblack<<"; no complete scene or presentation acceptance\n";
            // Join stage tasks to this live character/camera after preserving
            // the isolated render checkpoint. No new pool or player state.
            const unsigned live_size=dc_read32(rt,0x8C7A32BC)-heap;
            require(live_size<size,"Live stage join heap range differs");
            std::vector<std::uint8_t> live_stage_snapshot(live_size);
            for(unsigned i=0;i<live_size;++i)live_stage_snapshot[i]=dc_read8(rt,heap+i);
            dc_begin_code_module(rt,0x8C900000,0x200000);
            const auto stage_reads=rt.gd_sector_reads;
            require(call(0x8C09F664,0x8C15D030,0x0C900000)==0 && rt.gd_sector_reads-stage_reads==147,
                "Live stage reload failed");
            for(unsigned i=0;i<sizeof(board_bytes);++i)
                require(dc_read8(rt,0x8C900000+i)==board_bytes[i],"Live stage source bytes differ");
            for(unsigned i=0;i<live_size;++i)
                require(dc_read8(rt,heap+i)==live_stage_snapshot[i],"Stage reload changed live character/camera allocations");
            dc_activate_code_module(rt,0x8C900000,board_bytes,sizeof(board_bytes),callback_module_entries,std::size(callback_module_entries));
            // Original stage38 loader09F3A8/09F3B4 selects these for characters0/2.
            // Read-only reference arrays come independently from the owner disc.
            require(call(0x8C0513F8)==0,"Snowboard fixture character differs from original loader branch");
            const auto character_motion_reads=rt.gd_sector_reads;
            require(call(0x8C09F5A0,call(0x8C0513F8))==0 && rt.gd_sector_reads-character_motion_reads==105,
                "Original character motion selector/load failed");
            for(unsigned i=0;i<sizeof(s_mot_decoded);++i)
                require(dc_read8(rt,0x8CC00000+i)==s_mot_decoded[i],"Sonic motion bytes differ from independent decode");
            const auto motion_reads=rt.gd_sector_reads;
            require(call(0x8C09F664,0x8C15D03C,0x0CB08000)==0,"Original Sonic snowboard motion load failed");
            require(call(0x8C09F664,0x8C15D048,0x0CADC000)==0,"Original Tails snowboard motion load failed");
            require(rt.gd_sector_reads-motion_reads==95,"Snowboard motion sector extent differs");
            for(unsigned i=0;i<sizeof(s_sbmot_decoded);++i)
                require(dc_read8(rt,0x8CB08000+i)==s_sbmot_decoded[i],"Sonic snowboard bytes differ from independent decode");
            for(unsigned i=0;i<sizeof(m_sbmot_decoded);++i)
                require(dc_read8(rt,0x8CADC000+i)==m_sbmot_decoded[i],"Tails snowboard bytes differ from independent decode");
            for(unsigned i=0;i<live_size;++i)
                require(dc_read8(rt,heap+i)==live_stage_snapshot[i],"Snowboard motion load changed live task allocations");
            for(unsigned i=0;i<sizeof(board_bytes);++i)
                require(dc_read8(rt,0x8C900000+i)==board_bytes[i],"Snowboard motion load changed stage bytes");
            for(unsigned i=0;i<sizeof(camera_reference);++i)
                require(dc_read8(rt,0x8CB50000+i)==camera_reference[i],"Snowboard motion load changed camera bytes");
            std::cout<<"PASS original snowboard file bytes: S_SBMOT261688/M_SBMOT165232; 95 sectors; live heap/stage/camera preserved\n";
            for(unsigned i=0;i<sizeof(s_mot_decoded);++i)
                require(dc_read8(rt,0x8CC00000+i)==s_mot_decoded[i],"Snowboard load changed Sonic motion data");
            std::cout<<"PASS original character motion file: S_MOT488608 bytes; selector09F5A0; 105 sectors; live heap/stage/camera/snowboard preserved\n";
            const unsigned live_stage=call(0x8C09A1CE,0,1,0x8C900040);
            require(live_stage>=pool && live_stage<pool+0x30000,"Live stage task outside original pool");
            call(dc_read32(rt,live_stage+16),live_stage);
            const unsigned live_stage_work=dc_read32(rt,live_stage+36);
            require(live_stage_work>=heap && live_stage_work<heap+size && dc_read8(rt,live_stage_work)==0,
                "Live stage registration differs");
            for(unsigned expected:{2u,3u,3u}){
                call(dc_read32(rt,live_stage+16),live_stage);
                require(dc_read8(rt,live_stage_work)==expected,"Live stage original phase differs");
            }
            require(call(0x8C07EC7E,0)==work && dc_read32(rt,camera_task+32)==camera_work,
                "Stage join lost live character/camera work");
            std::cout<<"PASS live stage join: original reload147 sectors/508012 bytes; live allocations preserved; original pool; state0/2/3/update; player and camera retained\n";
            std::cerr<<"LIVE TASK GROUPS";
            for(unsigned group=0;group<8;++group){
                const auto head=dc_read32(rt,0x8C7A32CC+group*4);
                std::cerr<<' '<<group<<':'<<std::hex<<head<<'/'<<(head?dc_read32(rt,head+16):0)<<std::dec;
            }
            std::cerr<<"\nBEGIN joined original task group2\n";
            const unsigned snowboard_task=dc_read32(rt,0x8C7A32CC+2*4);
            require(dc_read32(rt,snowboard_task+16)==0x8C0B4B06,"Expected original snowboard constructor at group2 head");
            const auto before_snowboard_vram=rt.vram;
            const auto snowboard_reads=rt.gd_sector_reads;
            auto check_snowboard_upload=[&]{
                const auto sw=dc_read32(rt,snowboard_task+32),mw=dc_read32(rt,snowboard_task+36);
                require(dc_read32(rt,snowboard_task+16)==0x8C0B4B66
                    && dc_read32(rt,snowboard_task+20)==0x8C0B4C88
                    && dc_read32(rt,snowboard_task+24)==0x8C0B4DE6,
                    "Original snowboard callback installation differs");
                require(dc_read32(rt,sw+12)==0x0CB37454 && dc_read32(rt,sw+16)==0x8C1CA0B8
                    && dc_read32(rt,mw+60)==dc_read32(rt,0x8C0B4C08),"Original snowboard model/texture/speed differs");
                constexpr unsigned record=0x8C7649F4+57*68,offset=0x54B480;
                require(dc_read32(rt,0x8C1CA0AC+8)==record && dc_read32(rt,record)==500
                    && dc_read32(rt,record+36)==64 && dc_read32(rt,record+40)==64
                    && dc_read32(rt,record+44)==10928 && dc_read32(rt,record+48)==5
                    && dc_read32(rt,record+52)==offset && dc_read16(rt,record+64)==1
                    && dc_read16(rt,record+66)==0 && dc_read32(rt,0x8C893A30)==0,
                    "Streamed snowboard texture registration differs");
                // The file carries10924 bytes after its GBIX/PVRT headers.
                // Mip upload starts four bytes into the previously verified free extent.
                auto expected=before_snowboard_vram;
                for(unsigned i=28;i<sizeof(miw_b001_reference);++i)expected[offset+4+i-28]=miw_b001_reference[i];
                require(rt.vram==expected,"Streamed snowboard texture bytes or outside-range preservation differs");
                require(rt.gd_sector_reads-snowboard_reads==6,"Streamed snowboard texture sector count differs");
                std::cout<<"PASS original snowboard constructor and texture: original callbacks/model; MIW_B00164x64 mipmap; 10924 exact VRAM bytes; outside bytes preserved; six sectors; no snowboard display\n";
            };
            try{call(0x8C09A42C,2);}catch(...){check_snowboard_upload();throw;}
            check_snowboard_upload();
            require(dc_read32(rt,0x8C1C5A78)==0,"Group2 current callback was not cleared");
            const unsigned aux_task=pool+96,aux_main=dc_read32(rt,aux_task+32),aux=dc_read32(rt,aux_task+36);
            require(dc_read8(rt,aux_main)==1 && dc_read32(rt,aux_task+24)==0x8C0B44B0,
                "Original character auxiliary phase0 differs");
            for(unsigned node=0;node<3;++node)for(unsigned component=0;component<3;++component)
                require(dc_read32(rt,aux+100+node*12+component*4)
                    ==dc_read32(rt,dc_read32(rt,aux+88+node*4)+20+component*4),"Auxiliary source coordinate copy differs");
            constexpr std::int32_t angles[]={8192,-8192,1024,-6144,512,-6912,2048,-3328,6912,-512,2048,-3328};
            for(unsigned i=0;i<std::size(angles);++i)
                require(dc_read32(rt,aux+40+i*4)==static_cast<unsigned>(angles[i]),"Auxiliary phase0 rotation limits differ");
            for(unsigned i=0;i<sizeof(s_mot_decoded);++i){
                unsigned expected=s_mot_decoded[i];
                if(i>=0x252C+24 && i<0x252C+32)
                    expected=dc_read8(rt,dc_read32(rt,aux+88)+24+i-(0x252C+24));
                require(dc_read8(rt,0x8CC00000+i)==expected,"Sonic motion model update or outside bytes differ");
            }
            const unsigned effect_task=pool+48,effect_work=dc_read32(rt,effect_task+32);
            require(dc_read32(rt,effect_task+16)==0x8C0B573A && dc_read32(rt,effect_task+20)==0x8C0B5B4C
                && dc_read32(rt,effect_task+24)==0x8C0B571C && dc_read8(rt,effect_work+1)==0
                && dc_read16(rt,effect_work+6)==0 && dc_read32(rt,effect_work+16)==0,
                "Original auxiliary effect initializer differs");
            unsigned selected_effect=0;
            const auto player_id=dc_read8(rt,dc_read32(rt,0x8C790688)+9);
            for(unsigned i=0;i<5;++i){const unsigned row=0x8C1CA0DC+i*216;
                if(dc_read32(rt,row)==player_id){selected_effect=row;break;}}
            require(dc_read32(rt,effect_work+8)==selected_effect,"Original auxiliary effect selection differs");
            std::cout<<"PASS original group2 return: original snowboard and two character auxiliary initializers; callback cleared; model coordinates/rotation limits; two S_MOT word destinations only; effect selection; ABI/stack preserved\n";
            const auto stage_head=dc_read32(rt,0x8C7A32D8);
            require(dc_read32(rt,stage_head+16)==0x0C90554A,"Group3 stage head differs");
            std::vector<unsigned> stage_children,child_words;
            for(unsigned child=dc_read32(rt,stage_head+12);child;child=dc_read32(rt,child)){
                const unsigned index=stage_children.size();
                require(index<9,"Stage child list bound exceeded");
                const unsigned expected=index==0?0x8C0A2822:index==1?0x0C9056C4:index<5?0x0C903ABE:0x0C904B82;
                require(dc_read32(rt,child+16)==expected,"Stage child callback order differs");
                stage_children.push_back(child);
                for(unsigned offset=0;offset<48;offset+=4)child_words.push_back(dc_read32(rt,child+offset));
                if(index<2)continue;
                const auto child_work=dc_read32(rt,child+36);
                require(dc_read8(rt,child_work)==0,"Expected initial stage proximity phase");
                const auto origin=dc_read32(rt,child_work+(index<5?36:28));
                const double radius=std::bit_cast<float>(dc_read32(rt,index<5?0x8C903B38:0x8C904BFC));
                require(std::isfinite(radius) && radius>0,"Invalid source proximity radius");
                unsigned live_players=0;
                for(unsigned slot=0;slot<2;++slot){
                    const auto player=dc_read32(rt,0x8C790688+slot*4);
                    if(!player)continue;
                    const auto collision=dc_read32(rt,player+56);
                    if(!collision)continue;
                    const auto center=dc_read32(rt,collision+12)+8;
                    double squared=0;
                    for(unsigned component=0;component<3;++component){
                        const double delta=double(std::bit_cast<float>(dc_read32(rt,center+component*4)))
                            -double(std::bit_cast<float>(dc_read32(rt,origin+component*4)));
                        squared+=delta*delta;
                    }
                    // Original07E9B6 uses a strict distance comparison. Keep this
                    // supplied initial fixture comfortably outside every trigger.
                    require(std::isfinite(squared) && std::sqrt(squared)>radius+0.01,
                        "Stage proximity branch changed; inspect original child activation");
                    ++live_players;
                }
                require(live_players==1,"Stage proximity test lacks its live player collision center");
            }
            require(stage_children.size()==9,"Stage child count differs");
            const auto stage_vram=rt.vram;
            const auto model_before_group3=dc_read32(rt,0x8C1D1568);
            const auto attachment_before_group3=dc_read32(rt,0x8C1D1824);
            std::cerr<<"BEGIN joined original task group3\n";
            call(0x8C09A42C,3);
            require(dc_read32(rt,0x8C1C5A78)==0,"Group3 current callback was not cleared");
            for(unsigned i=0;i<stage_children.size();++i){
                const auto child=stage_children[i];
                for(unsigned offset=0;offset<48;offset+=4)
                    require(dc_read32(rt,child+offset)==child_words[i*12+offset/4],"Stage child links/callbacks changed");
                if(i>=2)require(dc_read8(rt,dc_read32(rt,child+36))==0,"Out-of-range stage child activated");
            }
            const auto motion_task=pool+192,motion_main=dc_read32(rt,motion_task+32);
            require(dc_read8(rt,motion_main)==1 && dc_read32(rt,motion_task+24)==0x8C0B3F8C
                && dc_read32(rt,0x8C1D1568)==dc_read32(rt,0x8C1C9C4C),"Original group3 model initializer differs");
            const auto attachment_task=pool+144,attachment=dc_read32(rt,attachment_task+36);
            require(attachment>=heap && attachment+32<heap+size
                && dc_read32(rt,attachment_task+16)==0x8C07CDA6
                && dc_read32(rt,attachment_task+24)==0x8C07CD96,"Original group3 attachment initialization differs");
            constexpr unsigned attachment_sources[]={0x8C1D17F4,0x8C1D1564,0x8C1E3910,0x8C1FF924};
            for(unsigned i=0;i<4;++i)require(dc_read32(rt,attachment+16+i*4)==attachment_sources[i],
                "Original attachment model source differs");
            require(rt.vram==stage_vram,"Group3 initialization unexpectedly changed VRAM");
            std::cout<<"PASS original group3 return: nine stage children; seven independently out-of-range player triggers; child links preserved; two original auxiliary initializers; callback cleared; ABI/stack preserved; no full scheduling\n";
            const auto timer_task=dc_read32(rt,0x8C7A32CC);
            require(dc_read32(rt,timer_task+16)==0x8C0A2B38,"Update timer head differs");
            const auto timer_work=dc_read32(rt,timer_task+36);
            const auto observe_update=[&](const char*phase){
                std::cerr<<"UPDATE STATE "<<phase<<" player_state="<<unsigned(dc_read8(rt,main))
                    <<" flags="<<std::hex<<dc_read16(rt,main+4)<<" position_bits=";
                for(unsigned i=0;i<3;++i)std::cerr<<dc_read32(rt,main+32+i*4)<<',';
                std::cerr<<" velocity_bits=";
                for(unsigned i=0;i<3;++i)std::cerr<<dc_read32(rt,work+56+i*4)<<',';
                std::cerr<<std::dec<<" countdown="<<dc_read32(rt,timer_work+4)<<'\n';
                std::cerr<<"UPDATE TERRAIN "<<phase<<" static_count="<<dc_read32(rt,0x8C1A26A0)
                    <<" dynamic_count="<<dc_read32(rt,0x8C1A269C)
                    <<" mode="<<dc_read32(rt,0x8C1A26B0)
                    <<" candidates="<<dc_read16(rt,0x8C758F70)
                    <<" nearby="<<dc_read32(rt,0x8C741FBC)
                    <<" list="<<std::hex<<dc_read32(rt,0x8C02D6E8)
                    <<" enabled="<<dc_read32(rt,0x8C75D774)
                    <<" stage38_table="<<dc_read32(rt,0x8C1A2734+38*32)<<std::dec<<'\n';
            };
            // Original0503F2 creates this terrain task through09A1CE(2,1,053720).
            // Join after the retained baseline checks for this isolated experiment;
            // full boot order and task-tree teardown remain unproved.
            const auto terrain_task=call(0x8C09A1CE,2,1,0x8C053720);
            require(terrain_task>=pool && terrain_task<pool+4096*48
                && dc_read32(rt,terrain_task+16)==0x8C053720
                && dc_read32(rt,terrain_task+32)>=heap,"Original terrain task allocation differs");
            std::cerr<<"BEGIN original terrain task053720 task="<<std::hex<<terrain_task<<std::dec<<'\n';
            observe_update("terrain-before");
            const auto capture_terrain=[&]{
                const auto dump=[&](const char*name,unsigned address,unsigned length){
                    std::ofstream f(recovery_frame_directory+"/terrain-"+name+".bin",std::ios::binary);
                    require(bool(f),"Cannot capture terrain diagnostic");
                    for(unsigned i=0;i<length;++i){const char b=dc_read8(rt,address+i);f.write(&b,1);}
                    require(bool(f),"Terrain diagnostic write failed");
                };
                dump("scratch",0x0CD00000,648832);
                dump("texture-records",0x8C7649F4,2048*68);
                dump("texture-list",0x0C90A61C,50*12);
                dump("records",0x0C9001A0,315*36);
                dump("visible",0x8C75E388,4+1024*4);
                std::ofstream vram(recovery_frame_directory+"/terrain-vram.bin",std::ios::binary);
                require(bool(vram),"Cannot capture terrain VRAM");
                vram.write(reinterpret_cast<const char*>(rt.vram.data()),rt.vram.size());
                require(bool(vram),"Terrain VRAM capture failed");
                std::cerr<<"TERRAIN COUNTERS textures="<<dc_read32(rt,0x8C893920)
                    <<" visible="<<dc_read32(rt,0x8C75E388)
                    <<" staged_packets="<<rt.pvr_ta_stream.size()<<'\n';
            };
            // Observe the original calls without changing guest registers or RAM.
            // Restore dispatch even on the expected failing path.
            static RecompiledFunction mesh_original,clip_original;
            mesh_original=rt.targets.at(0x8C037F6C);
            clip_original=rt.targets.at(0x8C038180);
            struct RestoreTerrainDispatch{
                DCRuntime&rt;RecompiledFunction mesh,clip;
                ~RestoreTerrainDispatch(){rt.targets.at(0x8C037F6C)=mesh;rt.targets.at(0x8C038180)=clip;}
            } restore_dispatch{rt,mesh_original,clip_original};
            static unsigned trace_count;
            trace_count=0;
            static const auto trace=[](const char*phase,const SH4Context&c){
                if(++trace_count>24)return;
                std::cerr<<"TERRAIN REGISTERS "<<phase<<" sr="<<std::hex<<c.sr<<" regs=";
                for(auto value:c.r)std::cerr<<value<<',';
                std::cerr<<" banks=";for(auto value:c.r_bank)std::cerr<<value<<',';
                std::cerr<<std::dec<<'\n';
            };
            rt.targets.at(0x8C037F6C)=+[](SH4Context&c,DCRuntime&r){
                trace("mesh-entry",c);
                try{mesh_original(c,r);}catch(...){trace("mesh-failure",c);throw;}
                trace("mesh-return",c);
            };
            rt.targets.at(0x8C038180)=+[](SH4Context&c,DCRuntime&r){
                trace("clip-entry",c);
                try{clip_original(c,r);}catch(...){trace("clip-failure",c);throw;}
                trace("clip-return",c);
            };
            try{call(0x8C053720,terrain_task);}catch(...){observe_update("terrain-failure");capture_terrain();throw;}
            observe_update("terrain-returned");capture_terrain();
            require(dc_read8(rt,dc_read32(rt,terrain_task+32))==1
                && dc_read32(rt,terrain_task+20)==0x8C053B60
                && dc_read32(rt,terrain_task+24)==0x8C053A98
                && dc_read32(rt,0x8C75D774)==0x0C902DEC,
                "Original terrain initializer state/callbacks differ");
            std::cout<<"PASS original terrain task return: state1; original display/destructor; selected descriptor; supplied bank1 experiment only\n";
            // Inspect output via the original frame boundary. Terrain was joined
            // after the first isolated character frame; normal order remains open.
            const auto terrain_starts=rt.pvr_isp_start_writes,terrain_completes=rt.pvr_render_completes;
            require(!rt.device_clock_enabled,"Terrain frame clock already enabled");
            dc_device_clock_enable(rt,true,false);
            try{call(0x8C608DF0);}catch(...){capture_frame();dc_device_clock_enable(rt,false);throw;}
            dc_device_clock_enable(rt,false);
            capture_frame();
            std::cerr<<"TERRAIN FRAME DELTA starts="<<(rt.pvr_isp_start_writes-terrain_starts)
                <<" completes="<<(rt.pvr_render_completes-terrain_completes)
                <<" staged="<<rt.pvr_ta_stream.size()<<'\n';
            // Original029062 calls054278 to select nearby source terrain, then
            // prepares the collision list consumed by028D9E. No injected list.
            const auto capture_collision=[&](const char*name="collision-ram.bin"){
                std::ofstream f(recovery_frame_directory+"/"+name,std::ios::binary);
                require(bool(f),"Cannot capture collision diagnostic");
                f.write(reinterpret_cast<const char*>(rt.main_ram.data()),rt.main_ram.size());
                require(bool(f),"Collision diagnostic write failed");
            };
            // Source029000 supplies both static pools; do not manufacture free
            // list links when029062 reaches its first original allocation.
            // This is a fresh supplied startup, including on repeat.029000 is
            // the original pool reset; the prior fixture need not be virgin RAM.
            require(call(0x8C029000)==1,"Original collision pool initialization failed");
            for(const auto&spec:std::array<std::pair<unsigned,unsigned>,2>{{
                {0x8C6BEFBC,192},{0x8C6C1FBC,8192}}}){
                for(unsigned i=0;i<spec.second;++i)
                    require(dc_read32(rt,spec.first+i*64)==(i+1<spec.second?spec.first+(i+1)*64:0),
                        "Original collision free-chain differs");
            }
            require(dc_read32(rt,0x8C02D6E4)==0x8C6BEFBC
                && dc_read32(rt,0x8C02D6E0)==0x8C6C1FBC
                && dc_read32(rt,0x8C02D6E8)==0,"Original collision pool heads differ");
            std::cout<<"PASS original collision pool initialization:192 object nodes and8192 geometry nodes; exact free chains\n";
            std::cerr<<"BEGIN original collision preparation029062\n";
            try{call(0x8C029062);}catch(...){observe_update("collision-failure");capture_collision();throw;}
            observe_update("collision-returned");capture_collision();
            //230729: independent source oracle verifies929 triangles and pool partition.
            // Continue the original update; preserve pre-update RAM for that oracle.
            std::cout<<"PASS original collision preparation returned; triangle audit remains external\n";
            // Observe original narrow-phase calls; never change guest inputs/results.
            static RecompiledFunction query_original;
            query_original=rt.targets.at(0x8C029CA0);
            struct RestoreQueryDispatch{
                DCRuntime&rt;RecompiledFunction original;
                ~RestoreQueryDispatch(){rt.targets.at(0x8C029CA0)=original;}
            } restore_query{rt,query_original};
            static unsigned query_count;query_count=0;
            static std::string query_filename;query_filename="collision-queries.csv";
            {std::ofstream clear(recovery_frame_directory+"/collision-queries.csv",std::ios::trunc);
                require(bool(clear),"Cannot reset per-startup collision observations");}
            rt.targets.at(0x8C029CA0)=+[](SH4Context&c,DCRuntime&r){
                require(++query_count<=64,"Collision observation budget exhausted");
                const auto query=c.r[4],object=c.r[5],flags=c.r[6],caller=c.pr;
                std::array<unsigned,7> before;
                for(unsigned i=0;i<before.size();++i)before[i]=dc_read32(r,query+4*i);
                query_original(c,r);
                std::ofstream f(recovery_frame_directory+"/"+query_filename,std::ios::app);
                require(bool(f),"Cannot capture collision query");
                f<<std::hex<<caller<<','<<query<<','<<object<<','<<flags<<','<<c.r[0]<<','<<dc_read32(r,0x8C742140);
                for(auto word:before)f<<','<<word;
                for(unsigned i=0;i<before.size();++i)f<<','<<dc_read32(r,query+4*i);
                f<<'\n';require(bool(f),"Collision query capture failed");
            };
            std::cerr<<"BEGIN original full update traversal09A544\n";
            std::array<std::vector<unsigned>,8> update_groups;
            std::vector<std::pair<unsigned,unsigned>> update_callbacks;
            for(unsigned group=0;group<8;++group){
                unsigned node=dc_read32(rt,0x8C7A32CC+4*group),previous=0;
                while(node){
                    require(node>=pool && node<pool+4096*48 && (node-pool)%48==0
                        && update_callbacks.size()<13 && dc_read32(rt,node+4)==previous,
                        "Pre-update root task chain differs");
                    for(const auto&item:update_callbacks)require(item.first!=node,"Repeated root task");
                    update_groups[group].push_back(node);
                    update_callbacks.push_back({node,dc_read32(rt,node+16)});
                    previous=node;node=dc_read32(rt,node);
                }
            }
            require(update_callbacks.size()==13,"Pre-update root task count differs");
            std::array<float,3> position_before;
            for(unsigned i=0;i<3;++i)position_before[i]=std::bit_cast<float>(dc_read32(rt,main+32+4*i));
            observe_update("before");
            if(std::getenv("SONICPAD_RECOVERY_OUTER_LOOP")){
                // Finish the earlier supplied initialization/update prefix.
                // Starting more updates before034480 leaves duplicate collision
                // registrations;002431 incorrectly paired Sonic with himself.
                require(dc_read16(rt,0x8C749418)==1 && dc_read32(rt,0x8C749428)==dc_read32(rt,main+56),
                    "Prefix collision registration differs");
                std::cerr<<"BEGIN original prefix collision completion034480\n";
                try{call(0x8C034480);}catch(...){capture_collision("prefix-collision-ram.bin");throw;}
                for(unsigned address:{0x8C749418u,0x8C74941Au,0x8C74941Cu,0x8C74941Eu,
                    0x8C749420u,0x8C749422u,0x8C749424u,0x8C756C58u,0x8C756C5Au})
                    require(dc_read16(rt,address)==0,"Original prefix collision pass left registrations");
                capture_collision("prefix-collision-ram.bin");
                call(0x8C09B620);
                const auto coroutine_root=dc_read32(rt,0x8C09B664);
                require(dc_read32(rt,0x8C7A4C88)==coroutine_root,"Coroutine root producer differs");
                for(unsigned i=0;i<9;++i)require(dc_read32(rt,coroutine_root+4*i)==(i<2?coroutine_root:0),
                    "Original coroutine root fields differ");
                std::cout<<"PASS original coroutine root:36 bytes; self-linked empty root; no fabricated pointer\n";
                std::cerr<<"BEGIN first original outer update050474\n";
            }
            try{call(std::getenv("SONICPAD_RECOVERY_OUTER_LOOP")?0x8C050474:0x8C09A544);}
            catch(...){observe_update("failure");capture_collision("update-ram.bin");throw;}
            observe_update("returned");capture_collision("update-ram.bin");
            //233831: independent captured-state audit and four negative controls.
            // These assertions cover the supplied fixture, not general gameplay.
            for(unsigned group=0;group<8;++group){
                unsigned node=dc_read32(rt,0x8C7A32CC+4*group),previous=0;
                for(unsigned expected:update_groups[group]){
                    require(node==expected && dc_read32(rt,node+4)==previous,"Update root links changed");
                    previous=node;node=dc_read32(rt,node);
                }
                require(node==0,"Update added an unexpected root task");
            }
            unsigned deferred=0;
            for(const auto&[node,callback]:update_callbacks){
                const auto expected=callback==0x8C01B4D8?0x8C09A2FC:callback;
                deferred+=callback==0x8C01B4D8;
                require(dc_read32(rt,node+16)==expected,"Update callback/deferred destruction differs");
            }
            require(deferred==2 && dc_read32(rt,0x8C1C5A78)==0 && dc_read32(rt,timer_work+4)==7,
                "Update deferred count/current callback/countdown differs");
            for(unsigned i=0;i<3;++i){
                const float p=std::bit_cast<float>(dc_read32(rt,main+32+4*i));
                const float v=std::bit_cast<float>(dc_read32(rt,work+56+4*i));
                require(std::isfinite(p) && std::isfinite(v) && p==position_before[i]+v,
                    "Update finite position/velocity integration differs");
            }
            require(dc_read8(rt,effect_work+1)==1 && dc_read16(rt,effect_work+6)==41
                && dc_read32(rt,effect_work+16)==0,"Update empty effect state differs");
            const auto motion_work=dc_read32(rt,motion_task+36);
            require(dc_read32(rt,0x8C19FDF0)==0 && dc_read32(rt,motion_work+8)==0
                && dc_read32(rt,motion_work+12)==0 && dc_read32(rt,motion_work+44)==4096
                && dc_read32(rt,motion_work+48)==4096,"Update absent-provider motion differs");
            for(unsigned i=0;i<3;++i)require(dc_read32(rt,motion_main+20+4*i)==0,"Update motion model angle differs");
            require(dc_read8(rt,dc_read32(rt,attachment_task+32))==1 && dc_read32(rt,work+112)==attachment
                && dc_read32(rt,attachment+4)==0 && dc_read32(rt,attachment+8)==0
                && dc_read32(rt,dc_read32(rt,attachment+16)+48)==dc_read32(rt,attachment+20),
                "Update attachment state/model link differs");
            std::cout<<"PASS original full update:13 root links; two deferred callbacks; countdown7; finite integration; empty effects; attachment; supplied-state scope only\n";
            // Finish the original display traversal and frame before freeing
            // textures referenced by the update's queued drawing packets.
            std::cerr<<"BEGIN original full display09A564 queued_packets="<<rt.pvr_ta_stream.size()<<'\n';
            try{call(0x8C09A564);}catch(...){capture_frame();capture_collision("display-ram.bin");throw;}
            capture_collision("display-ram.bin");
            const auto joined_starts=rt.pvr_isp_start_writes,joined_completes=rt.pvr_render_completes;
            dc_device_clock_enable(rt,true);
            try{call(0x8C608DF0);}catch(...){capture_frame();dc_device_clock_enable(rt,false);throw;}
            dc_device_clock_enable(rt,false);capture_frame();
            require(rt.pvr_isp_start_writes==joined_starts+1 && rt.pvr_render_completes==joined_completes+1
                && rt.pvr_ta_stream.empty(),"Joined display frame did not drain before cleanup");
            std::cout<<"PASS original full display/frame: original traversal returned; one render completion; queued packets drained before release; visual/gameplay acceptance open\n";
            if(std::getenv("SONICPAD_RECOVERY_OUTER_LOOP")){
                // Keep the live coroutine and collision state between updates.
                // The surrounding startup order remains supplied, not normal boot.
                capture_collision("loop-before-ram.bin");
                query_count=0;query_filename="loop-collision-queries.csv";
                {std::ofstream clear(recovery_frame_directory+"/"+query_filename,std::ios::trunc);
                    require(bool(clear),"Cannot reset loop collision observations");}
                //09AD7E's CON_REGULAR row supplies the HUD's shared texture
                // records. The surrounding common-startup order remains open.
                require(dc_read32(rt,0x8C1C6198)==0x8C15C244
                    && dc_read32(rt,0x8C1C619C)==0x8C1C7048
                    && dc_read32(rt,0x8C1C7048)==0x8C1C6DA8
                    && dc_read32(rt,0x8C1C704C)==56,"Original HUD load row differs");
                for(unsigned i=0;i<56;++i)require(dc_read32(rt,0x8C1C6DA8+i*12+8)==0,
                    "HUD texture record already populated");
                const auto capture_hud_vram=[&](const char*name){
                    std::ofstream f(recovery_frame_directory+"/"+name,std::ios::binary);
                    f.write(reinterpret_cast<const char*>(rt.vram.data()),rt.vram.size());
                    require(bool(f),"Cannot capture HUD VRAM");
                };
                capture_collision("hud-before-ram.bin");capture_hud_vram("hud-before-vram.bin");
                const auto hud_reads=rt.gd_sector_reads;
                std::cerr<<"BEGIN original CON_REGULAR load09B3F0\n";
                try{call(0x8C09B3F0,0x8C15C244,0x8C1C7048);}
                catch(...){capture_collision("hud-after-ram.bin");capture_hud_vram("hud-after-vram.bin");throw;}
                capture_collision("hud-after-ram.bin");capture_hud_vram("hud-after-vram.bin");
                require(rt.gd_sector_reads-hud_reads==14,"HUD archive sector count differs");
                // The archive repeats global ID3B7 at entries12/13/16 with
                // identical payloads. Original registration shares that record.
                std::vector<unsigned> hud_records;
                for(unsigned i=0;i<56;++i){
                    const unsigned record=dc_read32(rt,0x8C1C6DA8+i*12+8);
                    require(record>=0x8C7649F4+108*68 && record<0x8C7649F4+162*68
                        && (record-0x8C7649F4)%68==0 && dc_read16(rt,record+64)>0,
                        "HUD original texture registration differs");
                    if(std::find(hud_records.begin(),hud_records.end(),record)==hud_records.end())
                        hud_records.push_back(record);
                }
                unsigned hud_references=0;
                for(unsigned record:hud_records)hud_references+=dc_read16(rt,record+64);
                require(hud_records.size()==54 && hud_references==56
                    && dc_read32(rt,0x8C677A58)==162 && dc_read32(rt,0x8C893920)==162,
                    "HUD original registration sharing/count differs");
                std::cout<<"PASS original HUD load:56 links/54 registrations;14 sectors; independent payload audit pending\n";
                //0503C0 supplies(2,2) to this cadence producer.0506BA is
                // the running-game wrapper;0508DE belongs to pause handling.
                std::cerr<<"BEGIN original cadence producer0534C0(2,2)\n";
                try{call(0x8C0534C0,2,2);}catch(...){capture_collision("cadence-ram.bin");throw;}
                capture_collision("cadence-ram.bin");
                require(dc_read32(rt,0x8C758F3C)==2 && dc_read32(rt,0x8C758F40)==2
                    && dc_read32(rt,0x8C758F44)==2 && dc_read32(rt,0x8C758F48)==0,
                    "Original cadence fields differ");
                const unsigned cadence_handle=dc_read32(rt,0x8C89437C);
                require(cadence_handle>0 && cadence_handle<256
                    && dc_read32(rt,0x8C896AC0+cadence_handle*16)==0x8C053570,
                    "Original cadence callback registration differs");
                std::cout<<"PASS original cadence producer:2/2/2/0; registered053570; normal loop pending\n";
                std::cerr<<"BEGIN original running-game wrapper0506BA\n";
                static RecompiledFunction cadence_wait_original,cadence_callback_original;
                cadence_wait_original=rt.targets.at(0x8C053556);
                cadence_callback_original=rt.targets.at(0x8C053570);
                struct RestoreCadenceWait{
                    DCRuntime&rt;RecompiledFunction wait,callback;
                    ~RestoreCadenceWait(){rt.targets.at(0x8C053556)=wait;rt.targets.at(0x8C053570)=callback;}
                } restore_cadence_wait{rt,cadence_wait_original,cadence_callback_original};
                rt.targets.at(0x8C053556)=+[](SH4Context&c,DCRuntime&r){
                    observe_cadence_clock("wait-entry",c,r);cadence_wait_original(c,r);
                };
                rt.targets.at(0x8C053570)=+[](SH4Context&c,DCRuntime&r){
                    observe_cadence_clock("callback-entry",c,r);
                    try{cadence_callback_original(c,r);}
                    catch(...){observe_cadence_clock("callback-failure",c,r);throw;}
                    observe_cadence_clock("callback-return",c,r);
                };
                dc_device_clock_enable(rt,true);
                for(unsigned cycle=1;cycle<=2;++cycle){
                    const std::string prefix=cycle==1?"loop-":"cycle-2-";
                    std::cerr<<"BEGIN original running/endframe cycle="<<cycle<<'\n';
                    const auto cycles_before=rt.device_clock_sh4_cycles;
                    const auto starts_before=rt.pvr_isp_start_writes,completes_before=rt.pvr_render_completes;
                    capture_collision((prefix+"before-update-ram.bin").c_str());
                    try{call(0x8C0506BA);}catch(...){
                        observe_update("running-failure");capture_collision((prefix+"update-ram.bin").c_str());
                        capture_collision((prefix+"display-ram.bin").c_str());dc_device_clock_enable(rt,false);throw;
                    }
                    observe_cadence_clock("wrapper-return",ctx,rt);
                    observe_update("running-returned");capture_collision((prefix+"update-ram.bin").c_str());
                    capture_collision((prefix+"display-ram.bin").c_str());
                    //0556A0 ends its iteration through0534F0, which waits for zero
                    // and reloads the original cadence before submitting the frame.
                    // Keep the clock running: toggling it resets all cycle counters.
                    observe_cadence_clock("endframe-entry",ctx,rt);
                    try{call(0x8C0534F0);}catch(...){
                        observe_cadence_clock("endframe-failure",ctx,rt);
                        capture_collision((prefix+"endframe-ram.bin").c_str());capture_frame();
                        dc_device_clock_enable(rt,false);throw;
                    }
                    observe_cadence_clock("endframe-return",ctx,rt);
                    capture_collision((prefix+"endframe-ram.bin").c_str());capture_frame();
                    require(rt.device_clock_sh4_cycles>cycles_before && rt.device_clock_enabled,
                        "Running cycle clock did not advance continuously");
                    require(rt.pvr_isp_start_writes==starts_before+1
                        && rt.pvr_render_completes==completes_before+1 && rt.pvr_ta_stream.empty(),
                        "Running cycle did not complete exactly one queued frame");
                    require(dc_read32(rt,0x8C758F48)==2 && ctx.r[0]==dc_read32(rt,0x8C8940A0),
                        "Running cycle update count or endframe timestamp differs");
                    std::cout<<"PASS supplied original running/endframe cycle="<<cycle
                        <<" two updates; one render completion; continuous guest clock; normal startup open\n";
                }
                dc_device_clock_enable(rt,false);
                throw std::runtime_error("Two supplied original running/endframe cycles returned; inspect cadence, state and images before acceptance");
            }
            // Both group3 initializers install original model-link destructors.
            // Without them the next supplied startup renders a changed model.
            const auto cleanup_vram=rt.vram;
            // 0B450E phase0 saves nine model angles in auxiliary work+100.
            // Its installed0B44B0 destructor restores them. Omitting this leaves
            // the first model's yaw in the next supplied startup's first pose.
            require(dc_read32(rt,aux_task+24)==0x8C0B44B0,"Auxiliary angle destructor differs");
            std::array<unsigned,9> saved_angles;
            for(unsigned i=0;i<9;++i)saved_angles[i]=dc_read32(rt,aux+100+4*i);
            call(dc_read32(rt,aux_task+24),aux_task);
            for(unsigned node=0;node<3;++node)for(unsigned component=0;component<3;++component)
                require(dc_read32(rt,dc_read32(rt,aux+88+4*node)+20+4*component)
                    ==saved_angles[node*3+component],"Original auxiliary destructor failed to restore model angles");
            require(rt.vram==cleanup_vram,"Auxiliary angle destructor changed VRAM");
            capture_collision("model-cleanup-ram.bin");
            std::cout<<"PASS original auxiliary angle release: nine saved model angles restored; VRAM unchanged; full task-tree teardown unproved\n";
            call(dc_read32(rt,attachment_task+24),attachment_task);
            call(dc_read32(rt,motion_task+24),motion_task);
            require(dc_read32(rt,0x8C1D1568)==model_before_group3
                && dc_read32(rt,0x8C1D1824)==attachment_before_group3
                && rt.vram==cleanup_vram,"Original group3 destructors did not restore model links/VRAM");
            std::cout<<"PASS original group3 model release: both installed destructors; original model and attachment links restored; VRAM unchanged; full task-tree teardown unproved\n";
            // Terrain053720 installs053A98, which releases this stage's texture
            // list through05390E, then clears terrain globals through053776.
            require(dc_read32(rt,terrain_task+24)==0x8C053A98,"Terrain destructor differs");
            call(dc_read32(rt,terrain_task+24),terrain_task);
            require(rt.vram==cleanup_vram && dc_read32(rt,0x8C75D774)==0
                && dc_read32(rt,0x8C75D778)==0 && dc_read32(rt,0x8C1A26A0)==0,
                "Original terrain destructor state/VRAM differs");
            for(unsigned i=58;i<108;++i)require(dc_read32(rt,0x8C7649F4+i*68)==0xFFFFFFFF
                && dc_read16(rt,0x8C7649F4+i*68+64)==0,"Terrain texture remains live");
            std::cout<<"PASS original terrain release:50 free texture records; terrain globals clear; VRAM bytes preserved; full task-tree teardown unproved\n";
            // Release this added texture through its installed original destructor.
            // Full task-tree destruction remains separate from fixture reset.
            const auto snowboard_uploaded_vram=rt.vram;
            call(dc_read32(rt,snowboard_task+24),snowboard_task);
            require(rt.vram==snowboard_uploaded_vram,"Snowboard destructor changed VRAM bytes");
            for(unsigned i=0;i<57;++i)
                require(dc_read16(rt,0x8C7649F4+i*68+64)==1,"Snowboard destructor changed a live character texture");
            require(dc_read32(rt,0x8C7649F4+57*68)==0xFFFFFFFF && dc_read16(rt,0x8C7649F4+57*68+64)==0,
                "Original snowboard destructor left its texture live");
            std::cout<<"PASS original snowboard texture release: installed destructor; texture record free; full task-tree teardown unproved\n";

            // Original resource cleanup, followed by fixture reset for a new isolated case.
            const auto uploaded_vram=rt.vram;
            call(0x8C09AC9C,0);
            require(dc_read8(rt,0x8C1C61EC)==0,"Original Sonic resource release differs");
            for(unsigned i=0;i<57;++i){
                const unsigned entry=0x8C7649F4+i*68;
                require(dc_read32(rt,entry)==0xFFFFFFFF && dc_read16(rt,entry+64)==0,
                    "Original texture release left a live record");
            }
            graphics->check(rt);
            require(rt.vram==uploaded_vram,"Original texture release changed VRAM bytes");
            std::cout<<"PASS original texture release: resource counter0; 57 free records; graphics reservations preserved; texture extents reclaimed; VRAM bytes preserved\n";
            call(0x8C06DFA0);
            require(call(0x8C666E80)==0,"Original VRAM reset failed");
            for(unsigned bank=0;bank<2;++bank){
                const unsigned root=0x8C680020+bank*20,node=dc_read32(rt,root+8);
                require(node && dc_read32(rt,root)==0 && dc_read32(rt,root+4)==0
                    && dc_read32(rt,root+12)==node && dc_read32(rt,root+16)==0x400000
                    && dc_read32(rt,node+4)==0 && dc_read32(rt,node+8)==0
                    && dc_read32(rt,node+12)==bank*0x400000 && dc_read32(rt,node+16)==0x400000,
                    "Original VRAM reset extent/list differs");
            }
            std::cout<<"PASS original VRAM reset: two free 4 MiB banks restored after completed group1 invocation\n";
            // Retain each startup's observations before the next case replaces
            // the canonical capture files. No cross-case query/RAM mixing.
            static unsigned completed_startups=0;
            const auto archive=std::filesystem::path(recovery_frame_directory)/("startup-"+std::to_string(++completed_startups));
            std::filesystem::create_directory(archive);
            for(const auto&entry:std::filesystem::directory_iterator(recovery_frame_directory)){
                const auto name=entry.path().filename().string();
                if(entry.is_regular_file() && (name.starts_with("terrain-") || name.starts_with("collision-")
                    || name=="update-ram.bin" || name=="display-ram.bin" || name=="model-cleanup-ram.bin"))
                    std::filesystem::copy_file(entry.path(),archive/entry.path().filename());
            }
            return;
        }
        std::cout<<"PASS original common player-work producer: task="<<std::hex<<task<<" work="<<work<<std::dec<<"; tables/parameters/self-pointers/velocity/scale; outer character init and scheduler not run\n";
        // Rejoin the same stage path using the produced player tables and task pool.
        const unsigned allocated=dc_read32(rt,0x8C7A32BC)-heap;
        require(allocated<size,"Startup allocated range differs");
        std::vector<std::uint8_t> live(allocated);
        for(unsigned i=0;i<allocated;++i)live[i]=dc_read8(rt,heap+i);
        dc_begin_code_module(rt,0x8C900000,0x200000);
        const auto reads=rt.gd_sector_reads;
        require(call(0x8C09F664,0x8C15D030,0x0C900000)==0 && rt.gd_sector_reads-reads==147,"Startup stage reload failed");
        for(unsigned i=0;i<sizeof(board_bytes);++i)require(dc_read8(rt,0x8C900000+i)==board_bytes[i],"Startup stage bytes differ");
        dc_activate_code_module(rt,0x8C900000,board_bytes,sizeof(board_bytes),callback_module_entries,std::size(callback_module_entries));
        dc_write16(rt,0x8C74D43A,38);dc_write16(rt,0x8C74D43C,0);dc_write16(rt,0x8C74D448,0);
        call(0x8C09E98E,0x8C15D054,0x0CB50000);
        for(unsigned i=0;i<allocated;++i)require(dc_read8(rt,heap+i)==live[i],"Loader changed live startup allocations");
        call(0x8C0196FE);
        const unsigned stage=call(0x8C09A1CE,0,1,0x8C900040);
        require(stage>=pool && stage<pool+0x30000,"Stage task not from original pool");
        call(dc_read32(rt,stage+16),stage);
        const unsigned state=dc_read32(rt,stage+36);
        require(state>=heap && state<heap+size && dc_read8(rt,state)==0,"Original stage registration differs");
        call(dc_read32(rt,stage+16),stage);require(dc_read8(rt,state)==2,"Produced startup stage0 differs");
        call(dc_read32(rt,stage+16),stage);require(dc_read8(rt,state)==3,"Produced startup stage2 differs");
        require(call(0x8C07EC7E,0)==work,"Player lookup lost original allocation");
        call(0x8C0194FC,0x4000);
        require(std::abs(std::bit_cast<float>(dc_read32(rt,0x8C893864))-320.0f)<0.002f,"Startup projection control differs");
        call(dc_read32(rt,stage+16),stage);
        require(dc_read8(rt,state)==3 && dc_read32(rt,0x8C7906E8)==work,"Produced startup stage3 differs");
        const double expected=640.0/(2.0*std::tan(6371.0*6.2831853071795864769/65536.0));
        require(dc_read32(rt,0x8C6BADA8)==0x31C7 && std::abs(std::bit_cast<float>(dc_read32(rt,0x8C893864))-expected)<0.002,
            "State3 did not restore original zero-velocity projection");
        std::cout<<"PASS original producers -> camera/stage registration -> state0/2/3 and one state3 update; valid player work; projection angle="
            <<std::hex<<dc_read32(rt,0x8C6BADA8)<<std::dec<<"; scheduler and outer character init remain open\n";
        call(0x8C06DFA0); // Isolate the next actual module-load round with original reset code.
    }catch(const std::exception&e){
        std::cerr<<"STARTUP FAIL "<<e.what()<<" pc="<<std::hex<<ctx.pc<<" pr="<<ctx.pr<<std::dec<<'\n';
        dc_trace_print_history(rt,std::cerr);throw;
    }
}
