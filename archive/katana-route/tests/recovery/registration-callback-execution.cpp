// Original registration callback; stored callbacks are never invoked.
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
static void need(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static auto read(const char* path){std::ifstream f(path,std::ios::binary);need(bool(f),"Missing peripheral fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned address){unsigned v;std::memcpy(&v,m.data()+(address&0xffffff),4);return v;}
static void put(auto& m,unsigned address,unsigned value){std::memcpy(m.data()+(address&0xffffff),&value,4);}
static void half(auto& m,unsigned address,unsigned value){std::uint16_t v=value;std::memcpy(m.data()+(address&0xffffff),&v,2);}
static void fill(auto& m,unsigned address,unsigned size,unsigned value){std::fill_n(m.begin()+(address&0xffffff),size,value);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t){
 SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}
 c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;
 c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.fpscr=0x40001;c.fpul=0xcafebabe;return c;
}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result,unsigned macl,bool has_result=true){
 need(after.pc==before.pr && after.pr==before.pr,"Peripheral return PC/PR differs");
 if(has_result)need(after.r[0]==result,"Peripheral result differs");
 for(unsigned i=8;i<16;++i)need(before.r[i]==after.r[i],"Peripheral saved register differs");
 need((before.sr&~1u)==(after.sr&~1u),"Peripheral interrupt mask differs");
 need(before.mach==after.mach && after.macl==macl,"Peripheral multiply state differs");
 need(before.gbr==after.gbr && before.vbr==after.vbr && before.fpscr==after.fpscr && before.fpul==after.fpul && before.fr_bits==after.fr_bits && before.xf_bits==after.xf_bits,"Peripheral control/FPU state differs");
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: callback-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong callback fixture sizes");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 unsigned combined=0,disable=0,setters=0;
 auto run=[&](unsigned entry,unsigned slot,unsigned value,unsigned mask,unsigned t){
  rt.main_ram=main;fill(rt.main_ram,0x8c897dd0,128,0xa5);fill(rt.main_ram,0x8c8983b8,1024,0xa5);put(rt.main_ram,0x8c6707d0,slot);auto expected=rt.main_ram;
  bool callback=entry==0x8c010f22,clear=entry==0x8c606930;unsigned scratch=callback?12:clear?8:4;
  if(callback||clear){if(slot<=31)put(expected,0x8c897dd0+slot*4,0);}
  if(callback){for(unsigned i=0;i<8;++i){put(expected,0x8c8983b8+i*128+116,0x8c010e9e);put(expected,0x8c8983b8+i*128+120,0x8c010f0e);}}
  if(!callback&&!clear){unsigned offset=entry==0x8c606b1a?116:120;for(unsigned i=0;i<8;++i)put(expected,0x8c8983b8+i*128+offset,value);}
  auto c=context(entry,mask,t);c.r[4]=value;auto before=c;call_recompiled(c,rt,c.pc);
  unsigned sr=(before.sr&~1u)|(clear?(slot>31):1u);unsigned result=clear?sr:callback?120:entry==0x8c606b1a?116:120;
  abi(before,c,result,before.macl);need(c.sr==sr,"Callback final SR/T differs");
  std::copy_n(rt.main_ram.begin()+((before.r[15]-scratch)&0xffffff),scratch,expected.begin()+((before.r[15]-scratch)&0xffffff));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"entry="<<std::hex<<entry<<" slot="<<slot<<" offset="<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Callback exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Callback flash/sound guards differ");
  if(callback)++combined;else if(clear)++disable;else ++setters;
 };
 for(unsigned slot=0;slot<35;++slot)for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u}){
  unsigned index=slot==34?0xffffffff:slot;run(0x8c010f22,index,0x8c010f0e,mask,t);run(0x8c606930,index,0,mask,t);
 }
 for(unsigned entry:{0x8c606b1au,0x8c606b20u})for(unsigned value:{0u,1u,0x8c010e9eu,0x8c010f0eu,0xffffffffu})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(entry,20,value,mask,t);
 std::cout<<"PASS original registration callback: "<<combined<<" combined, "<<disable<<" disable, "<<setters<<" setter cases; all interrupt masks, exact RAM/flash/sound and preserved ABI; stored callbacks never invoked\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
