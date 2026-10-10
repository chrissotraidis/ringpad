// Original handler enqueue/context-copy routines; no handler is invoked.
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
 need(argc==3,"usage: enqueue-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong enqueue fixture size");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context initial{};dc_setup_commercial_boot(initial,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 constexpr unsigned manager=0x8C8DABB8,stack=0x8CEFF000,data=0x8CD20000,callback=0x8C64B64E;unsigned cases=0;
 auto run=[&](unsigned busy_prefix,unsigned capacity,unsigned used,unsigned size,unsigned mask,unsigned t,unsigned bank=0){
  rt.main_ram=main;std::fill_n(rt.main_ram.begin()+((manager+0x210)&0xFFFFFF),0x2480,0xA5);put(rt.main_ram,manager+0x208,capacity);put(rt.main_ram,manager+0x20C,used);put(rt.main_ram,manager+0x204,0x13572468);
  for(unsigned i=0;i<32;++i)put(rt.main_ram,manager+0x210+i*0x124+12,i<busy_prefix?3:0);
  auto expected=rt.main_ram;auto c=context(0x8C66C92E,mask,t,bank);c.r[4]=manager;c.r[5]=callback;c.r[6]=data;c.r[7]=size;const auto before=c;const bool full=capacity==used;
  if(!full){
   need(busy_prefix<32,"Invalid unbounded all-busy input");const unsigned record=manager+0x210+busy_prefix*0x124;
   put(expected,record,data);put(expected,record+4,size/4);put(expected,record+8,0);put(expected,record+12,1);put(expected,record+16,0);put(expected,record+20,0x13572468);put(expected,record+0x118,0);put(expected,record+0x11C,0);
   // The temporary256-byte image starts with an original152-byte context;
   // its remaining104 bytes are saved caller registers and80 caller-stack bytes. The first copy
   // runs backwards because record+24 is above manager+104, all aligned.
   std::vector<std::uint8_t> saved_bytes(256,0);auto saved=before;saved.r[0]=record+24;saved.r[1]=3;saved.r[2]=0x8C112574;std::memcpy(&saved.r[3],rt.main_ram.data()+((manager+0x104)&0xFFFFFF),4);saved.r[4]=stack-176;saved.r[5]=record+24;saved.r[6]=0;saved.r[7]=manager+0x104;saved.r[10]=stack-176;saved.r[11]=size/4;saved.r[12]=busy_prefix;saved.r[13]=manager;saved.r[14]=record;saved.r[15]=stack-184;saved.pr=0x8C66C9DC;capture(saved_bytes,0,saved);put(saved_bytes,152,before.pr);for(unsigned i=0;i<5;++i)put(saved_bytes,156+4*i,before.r[10+i]);std::copy_n(rt.main_ram.begin()+(stack&0xFFFFFF),80,saved_bytes.begin()+176);
   std::copy(saved_bytes.begin(),saved_bytes.end(),expected.begin()+((record+24)&0xFFFFFF));put(expected,record+24,data+4*((size/4)-1));put(expected,record+32,callback);
  }
  call_recompiled(c,rt,c.pc);abi(before,c,full?0xFFFFFFFF:busy_prefix);
  //160-byte temporary area,24-byte frame and12-byte copy helper frame.
  std::copy_n(rt.main_ram.begin()+((stack-196)&0xFFFFFF),196,expected.begin()+((stack-196)&0xFFFFFF));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"prefix="<<busy_prefix<<" size="<<size<<" offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Enqueue exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Enqueue flash/sound guards differ");++cases;
 };
 for(unsigned prefix=0;prefix<32;++prefix)for(unsigned size:{0u,1u,4096u,4099u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(prefix,8,0,size,mask,t);
 for(unsigned capacity:{0u,1u,8u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(32,capacity,capacity,4096,mask,t);
 for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(0,8,0,4096,mask,t,1);
 std::cout<<"PASS original handler enqueue: "<<cases<<" cases;32 first-free positions, full metadata, truncated sizes, both FPU banks, exact context/descriptor/RAM, preserved ABI; callbacks never executed\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
