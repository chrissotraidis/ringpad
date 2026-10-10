// Original scheduler/context transfers; restored handler target is not executed.
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
static void restored(const SH4Context& before,const SH4Context& saved,const SH4Context& actual,unsigned value){
 need(actual.pc==saved.pr && actual.pr==saved.pr,"Restored target PC/PR differs");
 for(unsigned i=1;i<16;++i)need(actual.r[i]==saved.r[i],"Restored GPR differs");
 need(actual.r[0]==(value?value:1) && actual.sr==((before.sr&~1u)|(value==0)),"Restored value/SR differs");
 need(actual.gbr==saved.gbr && actual.mach==saved.mach && actual.macl==saved.macl && actual.fpul==saved.fpul && actual.fpscr==saved.fpscr,"Restored control state differs");
 auto fr=before.fr_bits,xf=before.xf_bits;const auto& incoming=(saved.fpscr&0x200000)?saved.xf_bits:saved.fr_bits;
 if(before.fpscr&0x200000)xf=incoming;else fr=incoming;
 need(actual.fr_bits==fr && actual.xf_bits==xf && actual.vbr==before.vbr && actual.spc==before.spc,"Restored physical FPU banks/vector state differ");
}
static SH4Context incoming(unsigned bank,unsigned pattern){
 auto c=context(0,6,1,bank,pattern);for(unsigned i=0;i<15;++i)c.r[i]=0xab000000+i*0x113;c.r[15]=0x8cdd0000;c.pr=0x8c64b64e;c.gbr=0x8cdef020;c.mach=0x13579bdf;c.macl=0x2468ace0;c.fpul=0x10203040;return c;
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: scheduler-oracle RAM FLASH");const auto main=read(argv[1]),flash=read(argv[2]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong scheduler fixtures");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;
 constexpr unsigned manager=0x8c8dabb8,stack=0x8ceff000,destination=0x8cd10004,old_manager=0x8cd40000;unsigned restore_cases=0,idle_cases=0,transfer_cases=0;
 auto exact=[&](auto expected,unsigned scratch){
  if(scratch)std::copy_n(rt.main_ram.begin()+((stack-scratch)&0xffffff),scratch,expected.begin()+((stack-scratch)&0xffffff));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Scheduler exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Scheduler flash/sound guards differ");
 };
 for(unsigned current_bank:{0u,1u})for(unsigned target_bank:{0u,1u})for(unsigned pattern=0;pattern<3;++pattern)for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u})for(unsigned rb:{0u,1u})for(unsigned value:{0u,1u,0x80000000u,0xffffffffu}){
  rt.main_ram=main;auto saved=incoming(target_bank,pattern);capture(rt.main_ram,destination,saved);auto expected=rt.main_ram;put(expected,0x8c7b0b44,value);auto c=context(0x8c112500,mask,t,current_bank,1);c.sr|=rb<<29;c.r[4]=destination;c.r[5]=value;auto before=c;call_recompiled(c,rt,c.pc);restored(before,saved,c,value);exact(expected,0);++restore_cases;
 }
 auto setup=[&](unsigned capacity,unsigned status,unsigned sequence){
  rt.main_ram=main;std::fill_n(rt.main_ram.begin()+(manager&0xffffff),0x2694,0xa5);put(rt.main_ram,manager+0x204,sequence);put(rt.main_ram,manager+0x208,capacity);put(rt.main_ram,manager+0x20c,capacity);
  for(unsigned i=0;i<32;++i){unsigned r=manager+0x210+i*0x124;put(rt.main_ram,r+12,status);put(rt.main_ram,r+20,sequence+1);put(rt.main_ram,r+0x118,0);}
  put(rt.main_ram,0x8ee058,old_manager);put(rt.main_ram,0x8ee05c,0xdeadbeef);put(rt.main_ram,0x8ac638,manager);
 };
 auto scheduled_capture=[&](auto& expected,const SH4Context& before){
  auto saved=before;saved.fpscr=0x40001;saved.r[15]=stack-32;saved.r[14]=manager+0x210;saved.pr=0x8c66cbd2;saved.r[13]=manager;saved.r[12]=0;saved.r[9]=0x8c8ee058;saved.r[4]=manager+4;saved.r[3]=0x8c112574;saved.r[2]=0x8c8ee05c;saved.r[1]=0x40001;capture(expected,manager+4,saved);put(expected,0x8ee05c,old_manager);
 };
 for(unsigned capacity:{0u,1u,8u,32u})for(unsigned status:{0u,1u,2u,3u,4u,0xffffffffu})for(unsigned sequence:{0u,17u,0xffffffffu})for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  if(status==1 && sequence==0xffffffff)continue;setup(capacity,status,sequence);auto expected=rt.main_ram;auto c=context(0x8c64b648,mask,t,bank,2);c.r[4]=0x8c758f40;const auto before=c;scheduled_capture(expected,before);put(expected,manager+0x204,sequence+1);
  for(unsigned i=0;i<capacity;++i){unsigned r=manager+0x210+i*0x124;put(expected,manager+0x2690,r);if(status==2){put(expected,r+12,3);put(expected,r+32,0x8c66ca96);}if(status==3){put(expected,r+12,0);put(expected,manager+0x20c,capacity-i-1);}}
  call_recompiled(c,rt,c.pc);abi(before,c,0x204);need(c.sr==((before.sr&~1u)|1u),"Idle scheduler T differs");exact(expected,status==3 && capacity?36:32);++idle_cases;
 }
 // One eligible first-free handler; stop at its restored PC without invoking it.
 for(unsigned index=0;index<8;++index)for(unsigned target_bank:{0u,1u})for(unsigned pattern=0;pattern<3;++pattern)for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u}){
  setup(8,0,17);unsigned r=manager+0x210+index*0x124;auto saved=incoming(target_bank,pattern);capture(rt.main_ram,r+24,saved);put(rt.main_ram,r+12,1);put(rt.main_ram,r+20,17);auto expected=rt.main_ram;auto c=context(0x8c64b648,mask,t,1,1);c.r[4]=0x8c758f40;auto before=c;scheduled_capture(expected,before);put(expected,0x8ee058,manager);put(expected,manager+0x2690,r);put(expected,0x8c7b0b44,1);call_recompiled(c,rt,c.pc);
  auto restore_before=before;restore_before.fpscr=0x40001;restored(restore_before,saved,c,1);exact(expected,32);++transfer_cases;
 }
 std::cout<<"PASS original handler scheduler: "<<restore_cases<<" restore, "<<idle_cases<<" idle/deferred/cancel/release, "<<transfer_cases<<" transfer cases; both FPU banks, exact context/metadata/RAM/flash/sound; restored handler target not executed\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
