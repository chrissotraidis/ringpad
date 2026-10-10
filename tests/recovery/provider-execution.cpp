// Bounded provider initialization with traced caller arguments, no forced globals.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dcrecomp_generated;
#include "system-flash-write-check.hpp"
std::string recovery_frame_directory;
#ifdef RECOVERY_SBOARD_JOIN
void verify_loaded_sboard(DCRuntime&, SH4Context&);
#ifdef RECOVERY_SBOARD_CAMERA
void run_original_startup(DCRuntime&, SH4Context&);
#endif
#endif
int main(int argc,char** argv){
    auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;SH4Context ctx{};
    try{
        if(argc!=5)throw std::runtime_error("usage: provider-oracle DISC_MAP IP.BIN EXPECTED_SECTORS EXPECTED_DECODED");
        check_system_flash_services();check_system_flash_writes();
        recovery_frame_directory=std::filesystem::path(argv[4]).parent_path().string();
        rt.maple_vmu_path=recovery_frame_directory+"/dreamcast_vmu_a1.bin";
        load_embedded_elf_image(rt);register_recompiled_program(rt);
        std::ifstream in(argv[2],std::ios::binary);if(!in)throw std::runtime_error("IP unavailable");
        std::vector<std::uint8_t> ip{std::istreambuf_iterator<char>(in),{}};
        if(ip.size()!=32768)throw std::runtime_error("Wrong IP size");
        dc_load_bytes(rt,0x8C008000,ip.data(),ip.size());
        dc_setup_commercial_boot(ctx,rt);dc_gdrom_load_disc_map(rt,argv[1]);
        rt.trace_history_enabled=true;
#ifdef RECOVERY_SBOARD_CAMERA
        // Fresh HLE post-BIOS state, before supplied register-bank/video/task
        // fixtures. Original main entry still does not prove IP.BIN handoff.
        if(std::getenv("SONICPAD_RECOVERY_ORIGINAL_STARTUP")){
            run_original_startup(rt,ctx);
            throw std::runtime_error("Original main entry returned; startup acceptance remains open");
        }
#endif
#ifdef RECOVERY_STARTUP_BANK1
        // Bounded caller-state hypothesis:038180 uses bank0 as scratch and
        // unconditionally returns to bank1. Establishing the normal startup
        // producer of this state remains required; this is not normal boot.
        dc_write_sr(ctx,0x60000001);
        std::cerr<<"SUPPLIED STARTUP BANK1 HYPOTHESIS; normal startup producer unverified\n";
#else
        dc_write_sr(ctx,0x40000001);
#endif
#ifdef RECOVERY_SBOARD_CAMERA
        // 056420 configures this size before010180 invokes video setup.
        ctx.pc=0x8C608250;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;ctx.r[4]=0x11C000;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || dc_read32(rt,0x8C677194)!=0x11C000)
            throw std::runtime_error("Original graphics memory size producer differs");
        // 010180 ->606210 ->649B20 installs original exception vectors.
        const auto vector_base=ctx.vbr;
        if(vector_base!=0x8C000000)throw std::runtime_error("Post-BIOS VBR differs");
        ctx.pc=0x8C649B20;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=dc_read32(rt,0x8C6703F8);ctx.r[5]=dc_read32(rt,0x8C677164);
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000 || ctx.vbr!=vector_base)
            throw std::runtime_error("Original vector initialization return differs");
        for(const unsigned offset:{0x100u,0x400u,0x600u})for(unsigned k=0;k<56;++k)
            if(dc_read8(rt,vector_base+offset+k)!=dc_read8(rt,(offset==0x600?0x8C662518:0x8C6624E4)+k))
                throw std::runtime_error("Original copied exception-vector bytes differ");
        std::cout<<"PASS original exception vector copy: three56-byte templates; caller VBR preserved\n";
        // Original606210 initializes the TMU after installing the vectors.
        // Frame-boundary608DF0 reads this clock; retain the original producer.
        ctx.pc=0x8C606328;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000 || (dc_read8(rt,0xFFD80004)&1)!=1
            || dc_read16(rt,0xFFD80010)!=2 || dc_read32(rt,0xFFD80008)!=0xFFFFFFFF)
            throw std::runtime_error("Original TMU setup differs");
        std::cout<<"PASS original TMU startup: channel0 enabled; source prescaler2/reloadFFFFFFFF; timing accuracy unproved\n";
        // 010180 initializes this heap once, before video. Repeated graphics
        // cases must free/reallocate through the same live allocator.
        const unsigned system_heap=((dc_read32(rt,0x8C670038)&0xE0000000)|0x0D000000)+0xFFF00000u;
        if((system_heap&0x1FFFFFFF)!=0x0CF00000)throw std::runtime_error("System heap caller address differs");
        ctx.pc=0x8C605E68;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=system_heap;ctx.r[5]=0x100000;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || dc_read32(rt,0x8C8931C0)!=system_heap
            || dc_read32(rt,0x8C8931C4)!=0x100000 || dc_read32(rt,0x8C8931BC)!=0x8C89319C
            || dc_read32(rt,system_heap+4)!=32768)
            throw std::runtime_error("Original system heap initialization differs");
        ctx.pc=0x8C608442;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=1;ctx.r[5]=0;ctx.r[6]=1;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000 || dc_read32(rt,0x8C893868)!=0x44200000 || dc_read32(rt,0x8C89386C)!=0x43F00000)
            throw std::runtime_error("Original video initialization differs");
        std::cout<<"PASS original video initialization: 640x480 globals; no displayed frame"<<std::endl;
        // Original010180 calls these after video, before the disc provider.
        // Keep its exact input-buffer arguments and let the guest create Maple state.
        ctx.pc=0x8C60623A;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000)
            throw std::runtime_error("Original peripheral prerequisite return differs");
        ctx.pc=0x8C605774;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=0;ctx.r[5]=0x8C6803C0;ctx.r[6]=0x8C68C3E0;
        call_recompiled(ctx,rt,ctx.pc);
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000 || dc_read32(rt,0x8C8AC634)!=0x8C8AB538)
            throw std::runtime_error("Original Maple initialization return/work area differs");
        for(unsigned slot=0;slot<24;++slot)
            if(dc_read32(rt,0x8C892C24+slot*52+48)!=0x8C8AB538+0x44C+slot*120)
                throw std::runtime_error("Original controller record pointer differs");
        std::cout<<"PASS original Maple startup: original work area;24 controller record pointers; input response unproved\n";
#endif
        const auto observe_work_callbacks=[&](const char*phase){
            const auto work=dc_read32(rt,0x8C67FD44);
            std::cerr<<"WORK CALLBACK phase="<<phase<<" work="<<std::hex<<work
                <<" event0900="<<dc_read32(rt,0x8C897D00);
            if((work&0x1FFFFFFF)>=0x0C000000 && (work&0x1FFFFFFF)<0x0CFFF000)
                std::cerr<<" callback="<<dc_read32(rt,work+0x238)
                    <<" argument="<<dc_read32(rt,work+0x23C);
            std::cerr<<std::dec<<'\n';
        };
        observe_work_callbacks("before-work-area-setup");
#ifdef RECOVERY_SBOARD_CAMERA
        // Video startup already initialized this work area and connected its
        // vblank callback to the event0900 chain. Repeating654DE4 calls644EA0,
        // restoring empty defaults while leaving that chain registered.
        const auto work=dc_read32(rt,0x8C67FD44);
        if(work!=0x8C8A6FD4 || dc_read32(rt,work+0x238)!=0x8C64985E)
            throw std::runtime_error("Video startup event callback missing before provider initialization");
        std::cout<<"PASS video-owned work area retained: original event0900 dispatch; no duplicate initialization\n";
#else
        ctx.pc=0x8C654DE4;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        std::cout<<"BEGIN original work-area initialization"<<std::endl;
        call_recompiled(ctx,rt,ctx.pc);
#endif
        observe_work_callbacks("after-work-area-setup");
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000)throw std::runtime_error("Work-area initializer return mismatch");
        std::cout<<"WORK_AREA=0x"<<std::hex<<dc_read32(rt,0x8C67FD44)<<std::dec<<std::endl;
        ctx.pc=0x8C603D72;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=8;ctx.r[5]=0x8C698420;ctx.r[6]=2100;ctx.r[7]=0x8C6997A0;
        dc_write32(rt,ctx.r[15],0); // Fifth argument observed in original callers.
        std::cout<<"BEGIN provider global="<<std::hex<<dc_read32(rt,0x8C677160)<<std::dec<<std::endl;
        call_recompiled(ctx,rt,ctx.pc);
        std::cout<<"RETURN r0="<<std::hex<<ctx.r[0]<<" pc="<<ctx.pc<<" sp="<<ctx.r[15]
                 <<" provider="<<dc_read32(rt,0x8C677160)<<" state="<<dc_read32(rt,0x8C677154)<<std::dec
                 <<" calls="<<rt.trace_call_count<<" sectors="<<rt.gd_sector_reads<<std::endl;
        if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000)throw std::runtime_error("Initializer return mismatch");
        if(ctx.r[0]!=0)throw std::runtime_error("Provider initialization returned failure");
        auto call=[&](std::uint32_t entry,std::uint32_t r4,std::uint32_t r5){
            ctx.pc=entry;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;ctx.r[4]=r4;ctx.r[5]=r5;
            const auto before=ctx;
            call_recompiled(ctx,rt,entry);
            for(unsigned n=8;n<15;++n)
                if(ctx.r[n]!=before.r[n])throw std::runtime_error("File operation changed preserved GPR");
            if(ctx.pr!=before.pr)throw std::runtime_error("File operation changed PR");
            if(ctx.pc!=0xFFFFFFFF || ctx.r[15]!=0x8CE3F000)throw std::runtime_error("File operation return mismatch");
            return ctx.r[0];
        };
        const auto directory_result=call(0x8C604B1C,0x8C112E0C,0);
        std::cout<<"DIRECTORY result=0x"<<std::hex<<directory_result<<std::dec<<std::endl;
        if(directory_result!=0)throw std::runtime_error("Original directory selection failed");
        std::ifstream expected_file(argv[3],std::ios::binary);
        if(!expected_file)throw std::runtime_error("Expected sectors unavailable");
        std::vector<std::uint8_t> expected{std::istreambuf_iterator<char>(expected_file),{}};
        if(expected.size()!=331776)throw std::runtime_error("Expected sector size mismatch");
        constexpr unsigned destination=0x0CD00000;
        for(unsigned n=0;n<expected.size()+32;++n)dc_write8(rt,destination-16+n,0xA5);
        const auto before_bytes=rt.gd_bytes_read, before_sectors=rt.gd_sector_reads;
        if(call(0x8C09E8A0,0x8C15CFB0,destination)!=0)throw std::runtime_error("Original filename read failed");
        for(unsigned n=0;n<expected.size();++n)
            if(dc_read8(rt,destination+n)!=expected[n])throw std::runtime_error("Filename-read byte mismatch at "+std::to_string(n));
        for(unsigned n=0;n<16;++n)
            if(dc_read8(rt,destination-16+n)!=0xA5 || dc_read8(rt,destination+expected.size()+n)!=0xA5)throw std::runtime_error("File output guard changed");
        if(rt.gd_bytes_read-before_bytes!=expected.size() || rt.gd_sector_reads-before_sectors!=162)throw std::runtime_error("Filename-read count mismatch");
        const auto closed_handle=dc_read32(rt,0x8C7A4D04);
        if(!closed_handle || dc_read16(rt,closed_handle+76)!=0)throw std::runtime_error("File handle not closed");
        std::cout<<"PASS original filename read: 162 sectors, 331776 exact bytes, guards, closed handle and preserved return/register state"<<std::endl;
        const char missing[]="__MISSING_RECOVERY__.PRS";
        dc_load_bytes(rt,0x8CE00400,reinterpret_cast<const std::uint8_t*>(missing),sizeof(missing));
        const auto before_missing=rt.gd_sector_reads;
        if(call(0x8C09E8A0,0x8CE00400,destination)!=0xFFFFFFFF)throw std::runtime_error("Missing filename did not fail");
        if(rt.gd_sector_reads!=before_missing)throw std::runtime_error("Missing filename read data sectors");
        for(unsigned n=0;n<expected.size();++n)
            if(dc_read8(rt,destination+n)!=expected[n])throw std::runtime_error("Missing filename changed output");
        std::cout<<"PASS missing filename: -1, no data read, output unchanged"<<std::endl;
        std::ifstream decoded_file(argv[4],std::ios::binary);
        if(!decoded_file)throw std::runtime_error("Decoded reference unavailable");
        std::vector<std::uint8_t> decoded{std::istreambuf_iterator<char>(decoded_file),{}};
        if(decoded.size()!=1434052)throw std::runtime_error("Decoded reference size differs");
        constexpr unsigned module_base=0x0C900000;
        for(unsigned n=0;n<decoded.size()+32;++n)dc_write8(rt,module_base-16+n,0xA5);
        const auto sectors_before_loader=rt.gd_sector_reads;
        const auto ccr_before=dc_read32(rt,0xFF00001C);
        if(call(0x8C09F664,0x8C15CFB0,module_base)!=0)throw std::runtime_error("Combined loader returned failure");
        for(unsigned n=0;n<decoded.size();++n)
            if(dc_read8(rt,module_base+n)!=decoded[n])throw std::runtime_error("Combined loader output differs at "+std::to_string(n));
        for(unsigned n=0;n<16;++n)
            if(dc_read8(rt,module_base-16+n)!=0xA5 || dc_read8(rt,module_base+decoded.size()+n)!=0xA5)throw std::runtime_error("Decoded output guard changed");
        if(rt.gd_sector_reads-sectors_before_loader!=162)throw std::runtime_error("Combined loader sector count differs");
        if(dc_read32(rt,0xFF00001C)!=(ccr_before|0x800))throw std::runtime_error("Post-decode shadow register differs");
        std::cout<<"PASS combined loader: 162 sectors to 1434052 exact decoded bytes; guards/return/registers; MMIO shadow only"<<std::endl;
        const auto before_failed_load=rt.gd_sector_reads;
        if(call(0x8C09F664,0x8CE00400,module_base)!=0xFFFFFFFF)throw std::runtime_error("Missing module load did not fail");
        if(rt.gd_sector_reads!=before_failed_load)throw std::runtime_error("Missing module read data");
        for(unsigned n=0;n<decoded.size();++n)
            if(dc_read8(rt,module_base+n)!=decoded[n])throw std::runtime_error("Failed loader changed decoded module");
        std::cout<<"PASS missing module load: -1, no read/decompression mutation"<<std::endl;
#ifdef RECOVERY_SBOARD_JOIN
        verify_loaded_sboard(rt,ctx);
#endif
    }catch(const std::exception& e){
        std::cerr<<"FAIL "<<e.what()<<" pc=0x"<<std::hex<<ctx.pc<<" pr=0x"<<ctx.pr
                 <<" sp=0x"<<ctx.r[15]<<std::dec<<" calls="<<rt.trace_call_count<<" sectors="<<rt.gd_sector_reads<<'\n';
        dc_trace_print_history(rt,std::cerr);
        return 1;
    }
}
