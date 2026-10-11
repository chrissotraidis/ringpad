// Original readers only: captured device records and explicitly synthetic fields.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool ok,const char* msg){if(!ok)throw std::runtime_error(msg);}
static std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);need(bool(f),"Missing peripheral fixture");return {std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned x;std::memcpy(&x,m.data()+(a&0xFFFFFF),4);return x;}
static void put(auto& m,unsigned a,unsigned x){std::memcpy(m.data()+(a&0xFFFFFF),&x,4);}
static SH4Context context(unsigned pc,unsigned mask,unsigned t){SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0x11110000+i;c.r[15]=0x8CEFF000;c.pc=pc;c.pr=0xFFFFFFFF;c.sr=0x40000000|(mask<<4)|t;c.gbr=0x8CD12340;c.vbr=0x8C000000;c.fpscr=0x40001;c.fpul=0x24680;c.mach=0x13579;c.macl=0xABCDEF;return c;}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result,unsigned macl){
 need(after.pc==before.pr && after.pr==before.pr && after.r[0]==result,"Peripheral return differs");
 for(unsigned i=8;i<16;++i)need(after.r[i]==before.r[i],"Peripheral callee-saved register differs");
 need((after.sr&~1u)==(before.sr&~1u),"Peripheral interrupt mask differs");
 need(after.gbr==before.gbr && after.vbr==before.vbr && after.fpscr==before.fpscr && after.fpul==before.fpul && after.mach==before.mach && after.macl==macl,"Peripheral control register differs");
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: peripheral-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong peripheral fixture size");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context initial{};dc_setup_commercial_boot(initial,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 const unsigned root=word(main,0x8C8AC634);need(root==0x8C8AB538,"Captured peripheral root differs");
 constexpr unsigned output=0x8CD10004,stack=0x8CEFF000;unsigned geometry=0,compatibility=0,routes=0;
 auto device=[&](unsigned unit){return root+0x44C+120*unit;};
 auto exact=[&](auto& expected,unsigned scratch){
  std::copy_n(rt.main_ram.begin()+((stack-scratch)&0xFFFFFF),scratch,expected.begin()+((stack-scratch)&0xFFFFFF));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Peripheral exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Peripheral flash/sound differs");
 };
 auto query=[&](const auto& fixture,unsigned unit,unsigned mask,unsigned t){
  const unsigned addr=device(unit),functions=word(fixture,addr);const bool supported=functions&4;
  // Function definitions are packed in descending function-bit order.
  const unsigned definition=addr+4+4*std::popcount(functions&0xFFFFFFF8u);
  std::array<unsigned,5> fields{};
  if(supported){const unsigned off=definition&0xFFFFFF;fields={unsigned(fixture[off])+1,(unsigned(fixture[off+1])+1)*32,unsigned(fixture[off+2]>>4),unsigned(fixture[off+3]>>6),unsigned((fixture[off+3]>>5)&1)};}
  rt.main_ram=fixture;std::fill_n(rt.main_ram.begin()+(output&0xFFFFFF)-4,16,0xA5);auto expected=rt.main_ram;
  if(supported){for(unsigned i=0;i<3;++i){expected[(output&0xFFFFFF)+2*i]=fields[i]&255;expected[(output&0xFFFFFF)+2*i+1]=(fields[i]>>8)&255;}expected[(output&0xFFFFFF)+6]=fields[3];expected[(output&0xFFFFFF)+7]=fields[4];}
  auto c=context(0x8C64A9CA,mask,t);c.r[4]=output;c.r[5]=addr;const auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,supported?0:0xFFFFFFFF,before.macl);exact(expected,16);++geometry;
  rt.main_ram=fixture;expected=fixture;c=context(0x8C6067F4,mask,t);c.r[4]=unit;const auto check_before=c;call_recompiled(c,rt,c.pc);
  const unsigned valid=functions==14 && fields==std::array<unsigned,5>{1,192,1,0,0};abi(check_before,c,valid,120*unit);exact(expected,36);++compatibility;
 };
 // Read the actual captured VMU and absent slots without synthesizing replies.
 for(unsigned unit:{1u,7u,13u,19u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})query(main,unit,mask,t);
 for(unsigned field=0;field<4;++field)for(unsigned value=0;value<256;++value){auto m=main;const unsigned a=device(1);put(m,a,14);const std::uint8_t bytes[]={0,5,0x10,0};std::copy_n(bytes,4,m.begin()+((a+8)&0xFFFFFF));m[((a+8)&0xFFFFFF)+field]=value;query(m,1,0,0);}
 for(unsigned functions:{0u,1u,2u,4u,6u,8u,10u,14u,0x80000004u,0xFFFFFFFFu,0x80000000u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  auto m=main;const unsigned a=device(1);put(m,a,functions);const unsigned d=a+4+4*std::popcount(functions&0xFFFFFFF8u);const std::uint8_t bytes[]={0,5,0x10,0};std::copy_n(bytes,4,m.begin()+(d&0xFFFFFF));query(m,1,mask,t);
 }
 auto route=[&](auto m,unsigned unit,unsigned mask,unsigned t){
  const unsigned parent=m[0x670758+unit];need(parent<24,"Route parent outside captured range");
  const unsigned select=unit==2 || unit==8 || unit==14 || unit==20;const unsigned a=m[((device(parent)+18+select)&0xFFFFFF)],b=m[((device(unit)+18)&0xFFFFFF)];need(a<4 && b<4,"Synthetic route status out of range");
  const unsigned result=m[0x670730+4*a+b];rt.main_ram=m;auto c=context(0x8C6066D8,mask,t);c.r[4]=unit;const auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,result,120*unit);exact(m,16);++routes;
 };
 for(unsigned unit=0;unit<24;++unit)for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})route(main,unit,mask,t);
 for(unsigned unit=0;unit<24;++unit)for(unsigned a=0;a<4;++a)for(unsigned b=0;b<4;++b){auto m=main;unsigned parent=m[0x670758+unit],select=unit==2 || unit==8 || unit==14 || unit==20;m[((device(parent)+18+select)&0xFFFFFF)]=a;m[((device(unit)+18)&0xFFFFFF)]=b;route(m,unit,0,0);}
 std::cout<<"PASS original peripheral queries: "<<geometry<<" geometry, "<<compatibility<<" compatibility, "<<routes<<" route cases; captured/absent records, all definition bytes, packed function order, exact RAM/flash/sound and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
