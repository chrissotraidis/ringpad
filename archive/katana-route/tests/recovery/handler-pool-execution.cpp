// Original context capture and eight-slot allocation; no handler is invoked.
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
static void need(bool ok,const char* msg){if(!ok)throw std::runtime_error(msg);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing handler fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xFFFFFF),&v,4);}
static SH4Context context(unsigned pc,unsigned mask,unsigned t,unsigned bank=0,unsigned pattern=0){
 SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+17*i;c.fr_bits[i]=pattern==0?0x3F800000+i:pattern==1?0x7FC01000+i:0x80000000+i;c.xf_bits[i]=~c.fr_bits[i];}
 c.r[15]=0x8CEFF000;c.pc=pc;c.pr=0xFFFFFFFF;c.sr=0x40000000|(mask<<4)|t;c.gbr=0x8CD12340;c.vbr=0x8C000000;c.fpscr=0x40001|(bank<<21);c.fpul=0xCAFEBABE;c.mach=0x12345678;c.macl=0x89ABCDEF;return c;
}
// The original routine's documented-by-source152-byte context layout.
static void capture(auto& m,unsigned a,const SH4Context& c){
 put(m,a,c.r[15]);put(m,a+4,c.r[14]);put(m,a+8,c.pr);
 for(unsigned i=0;i<14;++i)put(m,a+12+4*i,c.r[13-i]);
 put(m,a+68,c.mach);put(m,a+72,c.macl);put(m,a+76,c.gbr);
 const auto& fr=(c.fpscr&0x200000)?c.xf_bits:c.fr_bits;
 for(unsigned i=0;i<16;++i)put(m,a+80+4*i,fr[i]);
 put(m,a+144,c.fpul);put(m,a+148,c.fpscr);
}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result){
 need(after.pc==before.pr && after.pr==before.pr && after.r[0]==result,"Handler return differs");
 for(unsigned i=8;i<16;++i)need(after.r[i]==before.r[i],"Handler callee-saved register differs");
 need((after.sr&~1u)==(before.sr&~1u),"Handler interrupt mask differs");
 need(after.gbr==before.gbr && after.vbr==before.vbr && after.fpscr==before.fpscr && after.fpul==before.fpul && after.mach==before.mach && after.macl==before.macl && after.fr_bits==before.fr_bits && after.xf_bits==before.xf_bits,"Handler control/FPU state differs");
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: handler-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong handler fixture size");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context initial{};dc_setup_commercial_boot(initial,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 constexpr unsigned pool=0x8C8DABB8,stride=0x2694,bytes=8*stride,stack=0x8CEFF000,destination=0x8CD10004;
 unsigned contexts=0,resets=0,allocations=0;
 auto exact=[&](auto expected,unsigned scratch){
  std::copy_n(rt.main_ram.begin()+((stack-scratch)&0xFFFFFF),scratch,expected.begin()+((stack-scratch)&0xFFFFFF));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"RAM offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Handler exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Handler flash/sound guards differ");
 };
 for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned pattern=0;pattern<3;++pattern){
  rt.main_ram=main;std::fill_n(rt.main_ram.begin()+(destination&0xFFFFFF)-4,160,0xA5);auto c=context(0x8C112574,mask,t,bank,pattern);c.r[4]=destination;const auto before=c;auto expected=rt.main_ram;capture(expected,destination,before);call_recompiled(c,rt,c.pc);abi(before,c,0);need(c.r[4]==destination && c.sr==before.sr,"Context capture changes source state");exact(expected,0);++contexts;
 }
 for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  rt.main_ram=main;std::fill_n(rt.main_ram.begin()+(pool&0xFFFFFF),bytes,0xA5);for(unsigned a:{0x8C8EE058u,0x8C8EE05Cu,0x8C8EE060u})put(rt.main_ram,a,0xDEADBEEF);auto expected=rt.main_ram;std::fill_n(expected.begin()+(pool&0xFFFFFF),bytes,0);for(unsigned a:{0x8C8EE058u,0x8C8EE05Cu,0x8C8EE060u})put(expected,a,0);
  auto c=context(0x8C66C878,mask,t);const auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,0x8C8EE060);exact(expected,4);++resets;
 }
 auto allocate=[&](unsigned busy,unsigned capacity,unsigned mask,unsigned t){
  rt.main_ram=main;std::fill_n(rt.main_ram.begin()+(pool&0xFFFFFF),bytes,0xA5);put(rt.main_ram,0x8C8EE060,20);
  for(unsigned i=0;i<8;++i)put(rt.main_ram,pool+i*stride,(busy&(1u<<i))?(i&1?0xFFFFFFFF:3):0);
  unsigned index=0;while(index<8 && (busy&(1u<<index)))++index;const unsigned selected=index==8?0:pool+index*stride;
  auto c=context(0x8C66C898,mask,t);c.r[4]=capacity;const auto before=c;auto expected=rt.main_ram;
  if(selected){
   std::fill_n(expected.begin()+((selected+0x210)&0xFFFFFF),0x2480,0);std::fill_n(expected.begin()+((selected+0x104)&0xFFFFFF),0x100,0);
   put(expected,selected+0x204,0);put(expected,selected+0x208,capacity);put(expected,selected+0x20C,0);put(expected,selected,1);put(expected,0x8C8EE060,21);
   // Source-derived state at the original112574 call after the two byte fills.
   auto saved=before;saved.r[0]=saved.r[4]=selected+0x104;saved.r[2]=0x8C10EF3C;saved.r[3]=0x8C112574;saved.r[5]=0;saved.r[6]=saved.r[7]=0x100;saved.r[13]=0;saved.r[14]=selected;saved.r[15]-=16;saved.pr=0x8C66C904;capture(expected,selected+0x104,saved);
  }
  call_recompiled(c,rt,c.pc);abi(before,c,selected);exact(expected,16);++allocations;
 };
 for(unsigned busy=0;busy<256;++busy)for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})allocate(busy,8,mask,t);
 for(unsigned capacity:{0u,1u,0xFFFFFFFFu})for(unsigned busy:{0u,127u,255u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})allocate(busy,capacity,mask,t);
 std::cout<<"PASS original handler pool: "<<contexts<<" context, "<<resets<<" reset, "<<allocations<<" allocation cases; both FPU banks, all occupancy masks, first-free/full behavior, exact RAM/flash/sound and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
