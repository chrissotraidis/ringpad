// Original status-word query and zero/nonzero readiness conversion.
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
 need(argc==3,"usage: readiness-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong readiness fixtures");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 unsigned raw=0,converted=0;
 for(unsigned slot=0;slot<8;++slot)for(unsigned value:{0u,1u,2u,0x7fffffffu,0x80000000u,0x80000001u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned entry:{0x8c64b9e4u,0x8c607124u}){
  rt.main_ram=main;put(rt.main_ram,0x8c8983b8+slot*128+4,value);auto expected=rt.main_ram;auto c=context(entry,mask,t);c.r[4]=slot;const auto before=c;call_recompiled(c,rt,c.pc);bool wrapper=entry==0x8c607124;
  abi(before,c,wrapper?(value?0xffffffff:0):value,before.macl);
  need(c.sr==((before.sr&~1u)|(wrapper?(value==0):0u)),"Readiness T state differs");
  if(wrapper)std::copy_n(rt.main_ram.begin()+((before.r[15]-4)&0xffffff),4,expected.begin()+((before.r[15]-4)&0xffffff));
  need(rt.main_ram==expected && rt.system_flash==flash && rt.aica_ram==sound,"Readiness memory guards differ");
  if(wrapper)++converted;else ++raw;
 }
 std::cout<<"PASS original peripheral readiness: "<<raw<<" raw and "<<converted<<" converted cases; all eight records, zero/nonzero/sign-bit values, all interrupt masks, exact memory and preserved ABI; status never changed\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
