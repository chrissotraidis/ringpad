// Original G2 counter getters/caller; no counter synthesis or sound completion.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing G2 fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xffffff),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){
 SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}
 c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;
 c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;
}
static void control(const SH4Context& b,const SH4Context& c){
 need((b.sr&~1u)==(c.sr&~1u),"G2 SR mask differs");
 need(b.mach==c.mach && b.macl==c.macl && b.gbr==c.gbr && b.vbr==c.vbr && b.spc==c.spc && b.fpscr==c.fpscr && b.fpul==c.fpul && b.fr_bits==c.fr_bits && b.xf_bits==c.xf_bits,"G2 control/FPU differs");
}
static void returned(const SH4Context& b,const SH4Context& c){
 control(b,c);need(c.pc==b.pr && c.pr==b.pr && c.r[0]==0,"G2 return differs");
 for(unsigned i=8;i<16;++i)need(b.r[i]==c.r[i],"G2 saved register differs");
}
static SH4Context state(const char* path){
 std::ifstream f(path);need(bool(f),"Missing frozen G2 state");SH4Context c{};std::string l;
 while(std::getline(f,l)){
  auto eq=l.find('=');if(eq==std::string::npos)continue;auto key=l.substr(0,eq);unsigned v=0;
  if(key=="pc" || key=="pr" || key=="sr" || key=="fpscr" || key=="vbr" || key=="spc" || key=="gbr" || key=="mach" || key=="macl" || key=="fpul" || key[0]=='r' || key.starts_with("fr") || key.starts_with("xf"))v=std::stoul(l.substr(eq+1),nullptr,16);else continue;
  if(key=="pc")c.pc=v;else if(key=="pr")c.pr=v;else if(key=="sr")c.sr=v;else if(key=="fpscr")c.fpscr=v;else if(key=="vbr")c.vbr=v;else if(key=="spc")c.spc=v;else if(key=="gbr")c.gbr=v;else if(key=="mach")c.mach=v;else if(key=="macl")c.macl=v;else if(key=="fpul")c.fpul=v;
  else if(key.starts_with("fr"))c.fr_bits[std::stoul(key.substr(2))]=v;else if(key.starts_with("xf"))c.xf_bits[std::stoul(key.substr(2))]=v;else if(key[0]=='r')c.r[std::stoul(key.substr(1))]=v;
 }return c;
}
static void same(const SH4Context& a,const SH4Context& b){
 need(a.r==b.r && a.pc==b.pc && a.pr==b.pr && a.sr==b.sr && a.gbr==b.gbr && a.vbr==b.vbr && a.spc==b.spc && a.mach==b.mach && a.macl==b.macl && a.fpul==b.fpul && a.fpscr==b.fpscr && a.fr_bits==b.fr_bits && a.xf_bits==b.xf_bits,"Frozen G2 full register state differs");
}
int main(int argc,char**argv){try{
 need(argc==4,"usage: g2-counter-oracle MAIN_RAM STATE FLASH");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong G2 fixtures");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};auto exact=[&](const auto& wanted){need(rt.main_ram==wanted && rt.system_flash==flash && rt.aica_ram==sound,"G2 exact RAM/stack/flash/sound differs");need(rt.g2_dma_runs==0 && rt.g2_dma_bytes==0 && rt.aica_arm.instructions==0,"G2 reduction advances devices unexpectedly");};
 unsigned getters=0,comparisons=0,early=0;const unsigned manager=word(main,0x8c67ff10),table=word(main,0x8c5ff84c);
 for(unsigned entry:{0x8c10f048u,0x8c10f064u})for(unsigned channel:{0u,1u,2u,3u,0xffffffffu,0x80000000u})for(unsigned value:{0u,1u,3u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(entry,mask,t,bank);c.r[4]=channel;c.r[5]=0x8cd10004;unsigned offset=entry==0x8c10f048?0xec:0xdc,address=table+offset+channel*4;put(rt.main_ram,address,value);put(rt.main_ram,c.r[5],0xa5a5a5a5);auto expected=rt.main_ram;put(expected,c.r[5],value);auto before=c;call_recompiled(c,rt,c.pc);returned(before,c);need(c.sr==before.sr && c.r[1]==value && c.r[2]==table+offset && c.r[3]==0x8c5ff84c && c.r[4]==address && c.r[5]==before.r[5] && c.r[6]==before.r[6] && c.r[7]==before.r[7],"G2 getter address/T/volatile registers differ");exact(expected);++getters;
 }
 for(unsigned done:{0u,1u,3u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned queued:{0u,1u,3u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();put(rt.main_ram,manager+0x16c,0);put(rt.main_ram,manager+0x1ac,1);put(rt.main_ram,0x8c67ff14,1);put(rt.main_ram,table+0xec,done);put(rt.main_ram,table+0xdc,queued);auto c=context(0x8c647cd8,mask,t,bank);auto before=c;auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);put(expected,0x8c8948fc,done);put(expected,0x8c8948f8,queued);if(queued<done){put(expected,0x8c67ff08,0x8c647cd8);put(expected,0x8c67ff0c,0x7fffffff);}call_recompiled(c,rt,c.pc);returned(before,c);need((c.sr&1)==(queued>=done),"G2 unsigned comparison T differs");exact(expected);++comparisons;
 }
 for(unsigned branch=0;branch<4;++branch)for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();put(rt.main_ram,manager+0x16c,0);put(rt.main_ram,manager+0x1ac,0);put(rt.main_ram,0x8c67ff14,branch==2?2:branch==1?0:1);if(branch==0)put(rt.main_ram,0x8c67ff10,0);auto c=context(0x8c647cd8,mask,t,bank);auto before=c;auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);call_recompiled(c,rt,c.pc);returned(before,c);exact(expected);++early;
 }
 // Positive pending count must call the missing sound service; no stub branch.
 reset();auto c=context(0x8c647cd8,6,0,0);bool rejected=false;try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error& e){rejected=std::string(e.what()).find("0x8C65E0B8")!=std::string::npos;}need(rejected,"G2 caller silently bypasses original sound service");
 // Actual capture is already inside the caller with its saved return on stack.
 reset();c=state(argv[2]);need(c.pc==0x8c647d04 && c.pr==0x8c647d0c && c.r[15]==0x8c89693c && word(main,table+0xec)==2 && word(main,table+0xdc)==3,"Frozen G2 continuation differs");auto expected=main;put(expected,0x8c8948fc,2);put(expected,0x8c8948f8,3);auto wanted=c;wanted.r[0]=0;wanted.r[1]=0x8c8948fc;wanted.r[2]=3;wanted.r[3]=2;wanted.r[4]=table+0xdc;wanted.r[5]=0x8c8948f8;wanted.pr=word(main,c.r[15]);wanted.pc=wanted.pr;wanted.r[15]+=4;wanted.sr|=1;call_recompiled(c,rt,0x8c647d04);same(wanted,c);exact(expected);need(c.pc==0x8c0535d0,"Frozen G2 caller return differs");
 std::cout<<"PASS original G2 counters: "<<getters<<" getter, "<<comparisons<<" unsigned-comparison and "<<early<<" early-return cases; all16 interrupt masks/both T values/FPU banks, channel scaling/wrap, exact entire RAM including stack; missing sound service explicitly rejects; frozen saved continuation returns0535D0 with all58 control/register/FPU fields and entire RAM exact; no completion/playback\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
