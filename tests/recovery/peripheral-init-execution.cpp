// Original peripheral initialization; descriptors are prepared, never invoked.
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
// Source-derived high-level record layout, independent of generated execution.
static void records(auto& m,unsigned type,unsigned selected,unsigned data){
 unsigned kind=0;for(unsigned candidate:{15u,33u,65u,122u}){if(candidate==type)break;++kind;}
 if(kind==4)return;
 put(m,0x8c8a07bc,kind);
 if(!data)return;
 const unsigned count=word(m,0x8c6778c4+kind*4),tag=word(m,0x8c6778d4+kind*4);
 unsigned cursor=data;
 for(unsigned slot=0;slot<8;++slot){
  if(!(selected&(1u<<slot)))continue;
  unsigned r=0x8c8a07d0+slot*0x148;
  half(m,r,slot);m[(r+2)&0xffffff]=1;
  put(m,r+28,0x8c8a5230+slot*76);put(m,r+44,cursor);put(m,r+32,cursor+32);
  put(m,r+36,cursor+0x220);put(m,r+40,cursor+32+(count+1)*512);
  half(m,r+14,1);half(m,r+16,count);half(m,r+18,tag);half(m,r+20,32);
  fill(m,r+56,16,0);fill(m,r+72,256,0);
  fill(m,cursor,32,71);fill(m,cursor+32,32,83);fill(m,cursor+0x220,32,70);fill(m,cursor+32+(count+1)*512,32,68);
  cursor+=type*512+32;
 }
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: peripheral-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong fixture sizes");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 unsigned cases=0,mode_cases=0,slot_cases=0;
 auto verify=[&](auto& expected,const SH4Context& before,SH4Context& c,unsigned scratch,unsigned result,unsigned macl,bool has_result=true){
  call_recompiled(c,rt,c.pc);abi(before,c,result,macl,has_result);
  std::copy_n(rt.main_ram.begin()+((before.r[15]-scratch)&0xffffff),scratch,expected.begin()+((before.r[15]-scratch)&0xffffff));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"type="<<before.r[4]<<" selection="<<before.r[5]<<" output="<<std::hex<<before.r[6]<<" offset="<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Peripheral exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Peripheral flash/sound guards differ");
 };
 auto initialize=[&](unsigned type,unsigned selected,unsigned data,unsigned mask,unsigned t){
  rt.main_ram=main;
  // Nonzero sentinels expose omitted fields, incorrect clears and overflows.
  fill(rt.main_ram,0x8c8a07b8,0xa78,0xa5);fill(rt.main_ram,0x8c8a5490,576,0xa5);
  put(rt.main_ram,0x8c8a56d0,0x12345678);put(rt.main_ram,0x8c8a56d4,0x12345678);put(rt.main_ram,0x8c8a56d8,0x12345678);put(rt.main_ram,0x8c67ffbc,0x12345678);put(rt.main_ram,0x8c898394,0x12345678);
  auto expected=rt.main_ram;put(expected,0x8c8a07cc,0x8c8a07b8);fill(expected,0x8c8a1210,32,0);fill(expected,0x8c8a07b8,20,0);fill(expected,0x8c8a07d0,0xa40,0);
  put(expected,0x8c8a07c4,0x8c664556);put(expected,0x8c8a07c8,0x8c664566);
  unsigned bits=0;for(unsigned i=0;i<8;++i)bits+=(selected>>i)&1;put(expected,0x8c8a56d8,bits);
  records(expected,type,selected,data);put(expected,0x8c8a07c0,data?0:1);put(expected,0x8c8a56d0,8);put(expected,0x8c8a56d4,0);put(expected,0x8c898394,10);fill(expected,0x8c8a5490,576,0);put(expected,0x8c67ffbc,0);
  auto c=context(0x8c64bb56,mask,t);c.r[4]=type;c.r[5]=selected;c.r[6]=data;auto before=c;
  unsigned macl=before.macl;bool valid=type==15||type==33||type==65||type==122;
  if(data && valid)for(unsigned i=0;i<8;++i)if(selected&(1u<<i))macl=i*76;
  verify(expected,before,c,data?84:32,0,macl);++cases;
 };
 // Every low-byte mask on all four record types and both output branches.
 for(unsigned type:{15u,33u,65u,122u})for(unsigned selected=0;selected<256;++selected)for(unsigned data:{0u,0x8c6b4314u})initialize(type,selected,data,(selected%3)*5,selected&1);
 for(unsigned type:{0u,14u,16u,32u,34u,64u,66u,121u,123u,0xffffffffu})for(unsigned data:{0u,0x8c6b4314u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})initialize(type,0xffffffff,data,mask,t);
 for(unsigned selected:{0u,1u,0x80u,0xffu,0x100u,0xffffff00u,0xffffffffu})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})initialize(15,selected,0x8c6b4314,mask,t);
 // Original mode selector installs one of two callback pairs without calling it.
 for(unsigned value=0;value<256;++value){rt.main_ram=main;put(rt.main_ram,0x8c8a07cc,0x8cd00000);auto expected=rt.main_ram;unsigned mode=value&1;put(expected,0x8cd00000,mode);put(expected,0x8cd0000c,mode?0x8c66454c:0x8c664556);put(expected,0x8cd00010,mode?0x8c66455e:0x8c664566);auto c=context(0x8c64bb28,value%16,value&1);c.r[4]=value;auto before=c;verify(expected,before,c,0,0,before.macl,false);++mode_cases;}
 // Unsigned table bound includes the actual startup slot20 and invalid indices.
 for(unsigned slot:{0u,1u,20u,30u,31u,32u,255u,0xffffffffu})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){rt.main_ram=main;auto expected=rt.main_ram;if(slot<=31)put(expected,0x8c897dd0+slot*4,0x8c6069f8);auto c=context(0x8c64ac2e,mask,t);c.r[4]=slot;c.r[5]=0x8c6069f8;auto before=c;verify(expected,before,c,0,0,before.macl,false);++slot_cases;}
 std::cout<<"PASS original peripheral initialization: "<<cases<<" initialization, "<<mode_cases<<" mode, "<<slot_cases<<" slot cases; exact RAM/flash/sound, buffers/descriptors, bounds and preserved ABI; callbacks never invoked\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
