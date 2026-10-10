// Frozen original startup sound queue. DMA completion and ARM7 are not supplied.
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
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* path){std::ifstream f(path,std::ios::binary);need(bool(f),"Missing sound fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned address){unsigned v;std::memcpy(&v,m.data()+(address&0xffffff),4);return v;}
static void put(auto& m,unsigned address,unsigned value){std::memcpy(m.data()+(address&0xffffff),&value,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned bank){
    SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}
    c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000001|(mask<<4);
    c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;
}
static void control(const SH4Context& b,const SH4Context& c,unsigned macl){
    need((b.sr&~1u)==(c.sr&~1u),"Sound SR mask differs");
    need(c.mach==b.mach && c.macl==macl && c.gbr==b.gbr && c.vbr==b.vbr && c.spc==b.spc && c.fpscr==b.fpscr && c.fpul==b.fpul && c.fr_bits==b.fr_bits && c.xf_bits==b.xf_bits,"Sound control/FPU differs");
}
static void returned(const SH4Context& b,const SH4Context& c,unsigned result,unsigned macl){
    control(b,c,macl);need(c.pc==b.pr && c.pr==b.pr && c.r[0]==result,"Sound return differs");
    for(unsigned i=8;i<16;++i)need(b.r[i]==c.r[i],"Sound saved register differs");
}
static void telemetry(auto& m,unsigned site,unsigned error){put(m,0x8c67ff08,site);put(m,0x8c67ff0c,error);}
static void pack(auto& m,unsigned manager,unsigned descriptor){
    unsigned count=word(m,manager+0x15c),packet=word(m,manager+0x158)+count*16;
    m[packet&0xffffff]=word(m,descriptor)&255;
    std::memmove(m.data()+((packet+2)&0xffffff),m.data()+((descriptor+4)&0xffffff),14);
    put(m,manager+0x15c,count+1);put(m,descriptor+20,0);
    put(m,manager+0x16c,word(m,manager+0x16c)-1);
}
static bool dma_boundary(SH4Context& c,DCRuntime& rt,unsigned entry){
    try{call_recompiled(c,rt,entry);}catch(const std::runtime_error& e){
        const std::string why=e.what();
        if(why.find("0x8C647364")!=std::string::npos)return true;
        throw;
    }return false;
}
int main(int argc,char**argv){try{
    need(argc==4,"usage: sound-service-oracle MAIN_RAM SOUND_RAM FLASH");
    const auto main=read(argv[1]),sound=read(argv[2]),flash=read(argv[3]);
    need(main.size()==16*1024*1024 && sound.size()==2*1024*1024 && flash.size()==131072,"Wrong sound fixtures");
    const unsigned manager=word(main,0x8c67ff10),table=word(main,0x8c5ff84c);
    need(manager==0x8c8d7680 && word(main,table+0xd4)==1 && sound[word(main,manager+0x1bc)]==0,"Frozen sound manager differs");
    need(word(main,manager+0x15c)==0 && word(main,manager+0x160)==0 && word(main,manager+0x168)==256 && word(main,manager+0x16c)==39 && word(main,manager+0x174)==0,"Frozen queue differs");
    auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;
    load_embedded_elf_image(rt);register_recompiled_program(rt);
    auto expected_sound=sound;
    auto reset=[&]{rt.main_ram=main;rt.aica_ram=sound;expected_sound=sound;rt.system_flash=flash;};
    auto exact=[&](auto expected,const SH4Context& before,unsigned scratch){
        if(scratch)std::memcpy(expected.data()+((before.r[15]-scratch)&0xffffff),rt.main_ram.data()+((before.r[15]-scratch)&0xffffff),scratch);
        if(rt.main_ram!=expected){auto i=std::mismatch(expected.begin(),expected.end(),rt.main_ram.begin()).first-expected.begin();std::cerr<<"RAM mismatch entry="<<std::hex<<before.pc<<" r4="<<before.r[4]<<" r5="<<before.r[5]<<" address="<<(0x8c000000+i)<<" expected="<<unsigned(expected[i])<<" actual="<<unsigned(rt.main_ram[i])<<"\n";throw std::runtime_error("Sound packet/count/descriptor/RAM guard differs");}
        need(rt.aica_ram==expected_sound && rt.system_flash==flash,"Sound frozen AICA/flash differs");
        need(rt.g2_dma_runs==0 && rt.g2_dma_bytes==0 && rt.aica_arm.instructions==0,"Reduction executes DMA/ARM7 unexpectedly");
    };
    unsigned readers=0,packers=0,flushes=0,services=0;
    for(unsigned offset:{0u,1u,2u,3u,0x13400u,0x1ffffcu,0x1ffffdu,0x1ffffeu,0x1fffffu,0x200000u,0x200001u,0x7fffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
        reset();auto c=context(0x8c66af04,mask,bank);c.r[4]=offset;c.r[5]=0x8cd10004;put(rt.main_ram,c.r[5],0xa5a5a5a5);auto expected=rt.main_ram;auto before=c;
        if(offset<0x200000){expected[c.r[5]&0xffffff]=sound[offset];put(expected,table+0x10c,word(expected,table+0x10c)+1);}
        call_recompiled(c,rt,c.pc);returned(before,c,0,before.macl);exact(expected,before,100);++readers;
    }
    for(unsigned count:{0u,1u,30u,31u,32u,33u,0x7fffffffu,0xffffffffu,0x80000000u})for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
        reset();put(rt.main_ram,manager+0x15c,count);put(rt.main_ram,manager+0x16c,0);
        auto c=context(0x8c66a8e8,mask,bank);c.r[4]=word(main,manager+0x164);auto before=c;auto expected=rt.main_ram;
        const bool full=static_cast<std::int32_t>(count)>=32;unsigned sp=c.r[15];
        put(expected,sp-4,c.r[14]);put(expected,sp-8,c.r[13]);put(expected,sp-12,c.r[12]);put(expected,sp-16,c.pr);
        if(full)telemetry(expected,0x8c66a8e8,0x0a000001);
        else{put(expected,sp-20,c.r[4]);put(expected,sp-24,word(main,manager+0x158)+count*16);put(expected,sp-28,c.r[11]);pack(expected,manager,c.r[4]);}
        call_recompiled(c,rt,c.pc);returned(before,c,full?0x0a000001:0,before.macl);
        need((c.sr&1)==!full,"Packer T differs");exact(expected,before,0);++packers;
    }
    for(unsigned count:{0u,1u,2u,31u,32u,33u})for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
        reset();put(rt.main_ram,manager+0x15c,count);auto expected=rt.main_ram;
        auto c=context(0x8c66a7a8,mask,bank);auto before=c;unsigned sp=c.r[15];
        put(expected,sp-4,c.r[14]);put(expected,sp-8,c.pr);
        if(count){put(expected,manager+0x160,1);put(expected,manager+0x15c,(count+1)&~1u);put(expected,sp-12,manager+0x158);}
        const bool boundary=dma_boundary(c,rt,c.pc);need(boundary==(count!=0),"Flush DMA boundary differs");
        if(!count)returned(before,c,0,before.macl);
        else{control(before,c,before.macl);need(c.r[15]==sp-12 && c.pr==0x8c66a7f6 && c.r[4]==word(main,manager+0x178) && c.r[5]==word(main,manager+0x158) && c.r[6]==((count+1)&~1u)*16 && c.r[7]==0x8c66a810,"Flush DMA arguments differ");}
        exact(expected,before,0);++flushes;
    }
    // Source-derived scenarios: sparse/full rings, end wrap, signed capacity,
    // pending termination and packet overflow. No arbitrary completion callback.
    struct Case{unsigned capacity,cursor,pending,count,pattern,busy,driver;};
    const Case cases[]={
        {0,0,0,0,0,0,0},{0xffffffff,0,39,0,0,0,0},{0x80000000,0,39,0,0,0,0},
        {1,0,1,0,1,0,0},{2,1,2,0,1,0,0},{4,3,4,0,1,0,0},
        {4,0,39,0,0,0,0},{4,3,39,0,2,0,0},{256,0,39,0,1,0,0},
        {256,255,0,0,1,0,0},{256,255,1,0,1,0,0},{256,255,0xffffffff,0,1,0,0},
        {4,0,39,31,1,0,0},{4,0,39,32,1,0,0},{4,3,39,32,2,0,0},
        {256,0,39,0,1,1,0},{256,0,39,0,1,2,0},{256,0,39,0,1,0,0xff},
        {256,0,39,0,1,0,1}};
    for(const auto& test:cases)for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
        reset();rt.aica_ram[word(main,manager+0x1bc)]=test.driver;expected_sound=rt.aica_ram;
        put(rt.main_ram,manager+0x168,test.capacity);put(rt.main_ram,manager+0x174,test.cursor);put(rt.main_ram,manager+0x16c,test.pending);put(rt.main_ram,manager+0x15c,test.count);put(rt.main_ram,manager+0x160,test.busy);
        const unsigned ring=word(main,manager+0x164);
        for(unsigned i=0;i<256;++i){for(unsigned j=0;j<20;++j)rt.main_ram[(ring+28*i+j)&0xffffff]=(i*23+j*37)&255;put(rt.main_ram,ring+28*i+20,test.pattern==1?1:(test.pattern==2 && i%3==1?1:2));}
        auto expected=rt.main_ram;auto c=context(0x8c65e0b8,mask,bank);auto before=c;
        unsigned error=0,macl=before.macl;
        // Only manager busy==1 skips the driver read. Nonzero driver
        // bytes report busy after the original G2 counter increments.
        if(test.busy!=1)put(expected,table+0x10c,word(expected,table+0x10c)+1);
        if(test.busy==1 || test.driver)error=0x0b000003;
        else{
            if(static_cast<std::int32_t>(test.capacity)>0)for(unsigned visited=0,packed=0;visited<test.capacity;++visited){
                unsigned cursor=word(expected,manager+0x174),descriptor=ring+cursor*28;macl=cursor*28;
                put(expected,manager+0x174,cursor+1==test.capacity?0:cursor+1);
                if(word(expected,descriptor+20)!=1)continue;
                if(static_cast<std::int32_t>(word(expected,manager+0x15c))>=32){error=0x0a000001;break;}
                pack(expected,manager,descriptor);if(++packed==32 || word(expected,manager+0x16c)==0)break;
            }
        }
        if(error)telemetry(expected,0x8c65e0b8,error);
        unsigned count=word(expected,manager+0x15c);bool expected_boundary=!error && count!=0;
        if(expected_boundary){put(expected,manager+0x160,1);put(expected,manager+0x15c,(count+1)&~1u);}
        bool boundary=dma_boundary(c,rt,c.pc);need(boundary==expected_boundary,"Service DMA boundary differs");
        if(boundary){control(before,c,macl);need(c.pr==0x8c66a7f6 && c.r[15]==before.r[15]-48 && c.r[4]==word(main,manager+0x178) && c.r[5]==word(main,manager+0x158) && c.r[6]==((count+1)&~1u)*16 && c.r[7]==0x8c66a810 && word(rt.main_ram,c.r[15])==manager+0x158,"Service DMA argument/stack differs");}
        else returned(before,c,error,macl);
        // Max source stack: service36 + reader16 + word/G2args16 +
        // G2copy36 + wrapper20 + leaf12 =136. Reader alone uses100.
        // Standalone
        // pack/flush above predict every stack byte and its guards exactly.
        exact(expected,before,136);++services;
    }
    reset();put(rt.main_ram,0x8c67ff10,0);auto expected=rt.main_ram;auto c=context(0x8c65e0b8,0,0);auto before=c;telemetry(expected,0x8c65e0b8,0x0d000001);call_recompiled(c,rt,c.pc);returned(before,c,0x0d000001,before.macl);exact(expected,before,36);++services;
    // Actual frozen startup queue: independently predict its first32 packets.
    reset();expected=main;c=context(0x8c65e0b8,6,0);before=c;
    const unsigned ring=word(main,manager+0x164);for(unsigned i=0;i<32;++i){need(word(main,ring+i*28+20)==1,"Frozen command missing");pack(expected,manager,ring+i*28);}
    put(expected,manager+0x174,32);put(expected,manager+0x160,1);put(expected,table+0x10c,word(main,table+0x10c)+1);
    need(dma_boundary(c,rt,c.pc),"Frozen queue does not reach original DMA boundary");control(before,c,31*28);exact(expected,before,136);
    std::cout<<"PASS original sound service: "<<readers<<" byte-reader, "<<packers<<" packet-packer, "<<flushes<<" flush and "<<services<<" service cases; all16 interrupt masks/both FPU banks; frozen39-command queue packs32, leaves7 pending and rejects before DMA647364; exact RAM outside136/100-byte service/reader stack scratch, exact standalone pack/flush stacks; no completion or playback\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
