// Isolated original descriptor construction and queue submission on frozen RAM.
// Diagnostic mode is exactly the captured disabled state; enabled output is untested.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool ok,const char* m){if(!ok)throw std::runtime_error(m);}
static std::vector<std::uint8_t> read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing sound-program fixture");return {std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const std::vector<std::uint8_t>& b,unsigned a){unsigned v;std::memcpy(&v,b.data()+(a&0xFFFFFF),4);return v;}
static void put(std::vector<std::uint8_t>& b,unsigned a,unsigned v){std::memcpy(b.data()+(a&0xFFFFFF),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned sp=0x8CEFF000){
 SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0x11110000+i;
 c.pc=entry;c.pr=0xFFFFFFFF;c.r[15]=sp;c.sr=0x40000000|(mask<<4)|t;
 c.gbr=0x8CD12340;c.vbr=0x8C000000;c.fpscr=0x40001;c.fpul=0x01234567;c.mach=0x87654321;c.macl=0x12345678;return c;
}
static void abi(const SH4Context& b,const SH4Context& a,unsigned result,unsigned macl){
 need(a.pc==b.pr && a.pr==b.pr && a.r[0]==result,"Sound-program return ABI differs");
 for(unsigned i=8;i<16;++i)need(a.r[i]==b.r[i],"Sound-program callee-saved register differs");
 need((a.sr&~1u)==(b.sr&~1u),"Sound-program interrupt mask differs");
 need(a.gbr==b.gbr && a.vbr==b.vbr && a.fpscr==b.fpscr && a.fpul==b.fpul && a.mach==b.mach && a.macl==macl,"Sound-program control register differs");
}
static void exact_ram(const DCRuntime& rt,std::vector<std::uint8_t> expected,unsigned sp,unsigned scratch){
 // Only the reviewed nested stack frame may retain scratch bytes; outer guards stay exact.
 std::copy_n(rt.main_ram.begin()+((sp-scratch)&0xFFFFFF),scratch,expected.begin()+((sp-scratch)&0xFFFFFF));
 if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){
  std::cerr<<"RAM offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<std::dec<<'\n';break;}
  throw std::runtime_error("Sound-program exact RAM differs");}
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: sound-program-oracle MAIN_RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);
 need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong sound-program fixture size");
 need(word(main,0x8C67FF10)==0x8C8D7680 && word(main,0x8C67FF1C)==0,"Captured sound/diagnostic state differs");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);
 SH4Context init{};dc_setup_commercial_boot(init,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 constexpr unsigned handle=0x8C8D7688,output=0x8CD10004,stack=0x8CEFF000;
 const unsigned queue=word(main,0x8C67FF10)+0x158,records=word(main,queue+12);
 need(word(main,queue+16)==256 && word(main,queue+20)==7 && word(main,queue+24)==7,"Captured queue layout differs");
 unsigned descriptor_cases=0,queue_cases=0;
 auto descriptor=[&](unsigned command,unsigned channel,unsigned arg1,unsigned arg2,unsigned mask,unsigned t){
  rt.main_ram=main;rt.main_ram[(handle+1)&0xFFFFFF]=channel;
  dc_write32(rt,output,0xA5A5A5A5);dc_write32(rt,stack,command|0xABCD0000);dc_write32(rt,stack+4,arg1);dc_write32(rt,stack+8,arg2);
  auto expected=rt.main_ram;std::array<std::uint8_t,4> bytes{};
  const unsigned family=command>>4;
  if(family!=15){bytes={static_cast<std::uint8_t>(family==10||family==12?0:arg2),static_cast<std::uint8_t>(arg1),static_cast<std::uint8_t>((channel<<4)|(command&15)),static_cast<std::uint8_t>(family&7)};}
  std::copy(bytes.begin(),bytes.end(),expected.begin()+(output&0xFFFFFF));
  auto c=context(0x8C647958,mask,t);c.r[4]=handle;c.r[5]=output;const auto before=c;call_recompiled(c,rt,c.pc);
  abi(before,c,0,before.macl);exact_ram(rt,expected,stack,64);++descriptor_cases;
 };
 for(unsigned command=0;command<256;++command)descriptor(command,0,0x12345679,0x89ABCD80,0,0);
 for(unsigned family=0;family<16;++family)for(unsigned channel:{1u,15u,16u,128u,255u})for(unsigned high:{0u,1u})
  for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})descriptor((family<<4)|15,channel,high?0xFFFFFFFF:0,high?0x123456FF:0,mask,t);
 // Replay the actual stack-passed bytes; R0=4 in the capture is not the caller's channel.
 {
  rt.main_ram=main;auto expected=main;const std::uint8_t bytes[]={0,121,0,3};
  need(word(main,0x8CFFFF84)==0xB0 && word(main,0x8CFFFF88)==121 && word(main,0x8CFFFF8C)==0,"Captured descriptor arguments differ");
  std::copy(std::begin(bytes),std::end(bytes),expected.begin()+0x7A2660);
  auto c=context(0x8C647958,0,0,0x8CFFFF84);c.r[4]=handle;c.r[5]=0x8C7A2660;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,0,before.macl);exact_ram(rt,expected,before.r[15],64);++descriptor_cases;
 }
 for(unsigned missing:{0u,1u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  rt.main_ram=main;if(!missing)dc_write32(rt,0x8C67FF10,0);auto expected=rt.main_ram;
  const unsigned error=missing?0x07000103:0x0D000001;put(expected,0x8C67FF08,0x8C647958);put(expected,0x8C67FF0C,error);
  auto c=context(0x8C647958,mask,t);c.r[4]=missing?0:handle;c.r[5]=output;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,error,before.macl);exact_ram(rt,expected,stack,32);++descriptor_cases;
 }
 for(unsigned pattern=0;pattern<4;++pattern)for(unsigned pending:{0u,7u,255u,256u})for(unsigned index:{0u,7u,255u})
  for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  rt.main_ram=main;const std::array<std::uint8_t,4> bytes=pattern==0?std::array<std::uint8_t,4>{0,121,0,3}:pattern==1?std::array<std::uint8_t,4>{255,128,255,7}:pattern==2?std::array<std::uint8_t,4>{1,2,3,4}:std::array<std::uint8_t,4>{0,0,0,0};
  std::copy(bytes.begin(),bytes.end(),rt.main_ram.begin()+(output&0xFFFFFF));dc_write32(rt,queue+20,pending);dc_write32(rt,queue+24,index);
  // Guard bytes in the command scratch area must be carried unchanged into the queue.
  for(unsigned i=0;i<20;++i)rt.main_ram[0x8948A8+i]=0xA0+i;
  auto expected=rt.main_ram;put(expected,0x8C8948A8,0x86);std::copy(bytes.begin(),bytes.end(),expected.begin()+0x8948AE);
  const unsigned error=pending==256?0x0A000001:0;
  if(!error){
   std::copy_n(expected.begin()+0x8948A8,20,expected.begin()+((records+index*28)&0xFFFFFF));
   put(expected,records+index*28+20,1);put(expected,queue+20,pending+1);put(expected,queue+24,(index+1)%256);
  }else{put(expected,0x8C67FF08,0x8C6478DC);put(expected,0x8C67FF0C,error);}
  auto c=context(0x8C6478DC,mask,t);c.r[4]=handle;c.r[5]=output;c.r[6]=0xDEADBEEF;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,error,error?before.macl:index*28);exact_ram(rt,expected,stack,24);++queue_cases;
 }
 for(unsigned missing:{0u,1u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  rt.main_ram=main;if(!missing)dc_write32(rt,0x8C67FF10,0);auto expected=rt.main_ram;
  const unsigned error=missing?0x07000103:0x0D000001;put(expected,0x8C67FF08,0x8C6478DC);put(expected,0x8C67FF0C,error);
  auto c=context(0x8C6478DC,mask,t);c.r[4]=missing?0:handle;c.r[5]=output;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,error,before.macl);exact_ram(rt,expected,stack,8);++queue_cases;
 }
 need(rt.system_flash==flash && rt.aica_ram==sound,"Sound-program flash/sound guards differ");
 std::cout<<"PASS original sound program: "<<descriptor_cases<<" descriptor cases, "<<queue_cases<<" queue cases; all command bytes, variadic truncation, captured channel0, wrap/full/missing, exact RAM and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
