// Original material compiler under the current startup flags on frozen RAM.
// Pure descriptor/packet words only; no rendering, VRAM transfer or general material coverage.
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
static unsigned dimension(unsigned x){
 for(unsigned code=0;code<8;++code)if(x==(8u<<code))return code;return x;
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: material-oracle MAIN_RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);
 need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong material fixture size");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);
 SH4Context init{};dc_setup_commercial_boot(init,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 constexpr unsigned material=0x8C893DB4,texture=0x8CD20000,stack=0x8CEFF000;
 unsigned packets=0,dimensions=0;
 auto packet=[&](unsigned width,unsigned height,unsigned format,unsigned texflags,unsigned address,unsigned mask,unsigned t){
  rt.main_ram=main;
  // These are exactly the current original6443EC material inputs; texture metadata
  // varies only in isolated cases. No live startup or VRAM state is changed.
  for(const auto& [offset,value]:std::array<std::pair<unsigned,unsigned>,8>{{{0,0x00600608},{4,0},{8,2},{12,0},{32,2},{64,0},{68,1},{108,texture}}})dc_write32(rt,material+offset,value);
  dc_write32(rt,texture+8,format);dc_write32(rt,texture+12,width);dc_write32(rt,texture+16,height);dc_write32(rt,texture+24,texflags);dc_write32(rt,texture+28,address);
  auto expected=rt.main_ram;
  unsigned pcw=word(expected,material+144);
  pcw=(pcw&0xF8FFFFFFu)|(2u<<24);pcw=(pcw|0x00800000u)&0xFFF3FFFFu;
  pcw|=word(expected,0x8C8A71B0)<<18;
  pcw=(pcw&~0x30u);pcw=(pcw&~8u)|8u;pcw&=~6u;pcw&=0xE78F00FFu;
  unsigned isp=word(expected,material+148);
  isp=(isp&~0x02000000u)|0x02000000u;isp&=~0x01800000u;isp&=0xFFD00000u;
  unsigned tsp=word(expected,material+152)|0x00100000u;
  tsp=(tsp&~0x38u)|(dimension(width)<<3);tsp=(tsp&~7u)|dimension(height);
  unsigned tcw=word(expected,material+156);
  tcw=(tcw&0x7FFFFFFFu)|((texflags&1)<<31);tcw=(tcw&~0x40000000u)|((texflags&8)<<27);
  tcw=(tcw&~0x38000000u)|(format&0x38000000u);
  if((format&0x38000000u)==0x28000000u || (format&0x38000000u)==0x30000000u)
   tcw=(tcw&0xF81FFFFFu)|((word(expected,material+104)&63)<<21);
  else{tcw=(tcw&~0x04000000u)|((~texflags&4)<<24);tcw=(tcw&~0x02000000u)|((texflags&16)<<21);}
  tcw=(tcw&0xFFE00000u)|(address>>3);
  put(expected,material+144,pcw);put(expected,material+148,isp);put(expected,material+152,tsp);put(expected,material+156,tcw);
  auto c=context(0x8C658BA0,mask,t);c.r[4]=material;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,0,before.macl);exact_ram(rt,expected,stack,68);++packets;
 };
 for(unsigned width:{8u,16u,32u,64u,128u,256u,512u,1024u})for(unsigned height:{8u,16u,32u,64u,128u,256u,512u,1024u})packet(width,height,0,1,0x600000,0,0);
 for(unsigned format:{0u,0x10000000u,0x20000000u,0x28000000u,0x30000000u,0x38000000u})for(unsigned flags:{0u,1u,4u,8u,16u,29u})for(unsigned address:{0u,8u,0x600000u,0x7FFFF8u})packet(128,128,format,flags,address,0,0);
 for(unsigned format:{0u,0x28000000u})for(unsigned flags:{1u,29u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})packet(128,128,format,flags,0x600000,mask,t);
 for(unsigned value:{0u,3u,8u,9u,12u,16u,32u,64u,128u,255u,256u,512u,1024u,2048u,0xFFFFFFFFu})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  rt.main_ram=main;const auto expected=rt.main_ram;auto c=context(0x8C6588A0,mask,t);c.r[4]=value;const auto before=c;
  call_recompiled(c,rt,c.pc);abi(before,c,dimension(value),before.macl);need(rt.main_ram==expected,"Dimension encoder changed RAM");++dimensions;
 }
 need(rt.system_flash==flash && rt.aica_ram==sound,"Material flash/sound guards differ");
 std::cout<<"PASS original startup material: "<<packets<<" packet cases, "<<dimensions<<" dimension cases; current flags, texture sizes/format/flags/address, exact RAM and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
