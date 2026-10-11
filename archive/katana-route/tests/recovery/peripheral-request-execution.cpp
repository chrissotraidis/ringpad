// Original peripheral request callback and setters, busy-path preservation.
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
static SH4Context load_state(const char* path){
 std::ifstream f(path);need(bool(f),"Missing worker state");SH4Context c{};std::string l;
 while(std::getline(f,l)){
  auto eq=l.find('=');if(eq==std::string::npos)continue;auto key=l.substr(0,eq);unsigned v=0;
  if(key=="pc" || key=="pr" || key=="sr" || key=="fpscr" || key=="vbr" || key=="spc" || key=="gbr" || key=="mach" || key=="macl" || key=="fpul" || key[0]=='r' || key.starts_with("fr") || key.starts_with("xf"))v=std::stoul(l.substr(eq+1),nullptr,16);else continue;
  if(key=="pc")c.pc=v;else if(key=="pr")c.pr=v;else if(key=="sr")c.sr=v;else if(key=="fpscr")c.fpscr=v;else if(key=="vbr")c.vbr=v;else if(key=="spc")c.spc=v;else if(key=="gbr")c.gbr=v;else if(key=="mach")c.mach=v;else if(key=="macl")c.macl=v;else if(key=="fpul")c.fpul=v;
  else if(key.starts_with("fr"))c.fr_bits[std::stoul(key.substr(2))]=v;else if(key.starts_with("xf"))c.xf_bits[std::stoul(key.substr(2))]=v;else if(key[0]=='r')c.r[std::stoul(key.substr(1))]=v;
 }
 return c;
}
constexpr unsigned records=0x8c8983b8;
static void request(auto& m,unsigned slot){const unsigned a=records+128*slot;put(m,a+4,1);put(m,a+8,1);put(m,a+20,slot);for(unsigned off:{24u,28u,32u,36u,40u})put(m,a+off,0);}
static void callback_stack(auto& m,const SH4Context& before,bool busy){const unsigned s=before.r[15];put(m,s-4,before.r[14]);put(m,s-8,before.r[13]);put(m,s-12,before.pr);if(!busy)for(unsigned off:{16u,20u,24u})put(m,s-off,0);}
int main(int argc,char**argv){try{
 need(argc==4,"usage: request-oracle RAM STATE FLASH");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong request fixtures");auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;unsigned callback_cases=0,operation_cases=0,argument_cases=0;
 auto exact=[&](const auto& expected){need(rt.main_ram==expected && rt.system_flash==flash && rt.aica_ram==sound,"Request exact RAM/flash/sound differs");};
 for(unsigned slot=0;slot<8;++slot)for(unsigned status:{0u,1u,2u,0x7fffffffu,0x80000000u,0x80000001u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  rt.main_ram=main;put(rt.main_ram,records+128*slot+4,status);auto expected=rt.main_ram;auto c=context(0x8c606b38,mask,t);c.fpscr|=bank<<21;c.r[4]=slot;auto before=c;callback_stack(expected,before,status!=0);if(!status)request(expected,slot);call_recompiled(c,rt,c.pc);abi(before,c,status?0xffffffff:0,before.macl);need(c.sr==(before.sr&~1u),"Request callback T differs");exact(expected);++callback_cases;
 }
 for(unsigned slot=0;slot<8;++slot)for(unsigned operation:{0u,1u,2u,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  rt.main_ram=main;auto expected=rt.main_ram;put(expected,records+128*slot+8,operation);auto c=context(0x8c64b9f4,mask,t);c.fpscr|=bank<<21;c.r[4]=slot;c.r[5]=operation;auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,before.r[0],before.macl);need(c.r[4]==records+128*slot && c.sr==(before.sr&~1u),"Operation address/T differs");exact(expected);++operation_cases;
 }
 for(unsigned slot=0;slot<8;++slot)for(unsigned pattern:{0u,1u,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  rt.main_ram=main;auto c=context(0x8c64ba04,mask,t);c.fpscr|=bank<<21;c.r[4]=slot;c.r[5]=pattern;c.r[6]=pattern^0x12345678;c.r[7]=pattern^0x87654321;for(unsigned i=0;i<3;++i)put(rt.main_ram,c.r[15]+4*i,pattern^(0xabc00000+i*0x13579));auto expected=rt.main_ram;put(expected,records+128*slot+4,1);put(expected,records+128*slot+20,c.r[5]);put(expected,records+128*slot+24,c.r[6]);put(expected,records+128*slot+28,c.r[7]);for(unsigned i=0;i<3;++i)put(expected,records+128*slot+32+4*i,word(expected,c.r[15]+4*i));auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,before.r[0],before.macl);need(c.r[4]==records+128*slot && c.sr==(before.sr&~1u),"Argument address/T differs");exact(expected);++argument_cases;
 }
 rt.main_ram=main;auto c=load_state(argv[2]);need(c.pc==0x8c606a76 && c.pr==0x8c606a7a && c.r[4]==0 && word(main,records+4)==0,"Frozen request call differs");auto before=c;auto expected=main;callback_stack(expected,before,false);request(expected,0);call_recompiled(c,rt,0x8c606b38);abi(before,c,0,before.macl);need(c.sr==(before.sr&~1u),"Frozen request T differs");exact(expected);
 std::cout<<"PASS original peripheral request: "<<callback_cases<<" callback busy/free, "<<operation_cases<<" operation and "<<argument_cases<<" argument cases; all eight slots/all16 interrupt masks/both FPU banks, all exact RAM including stack, preserved ABI and neighbor records; actual frozen callback sets operation1/status1 without completing it\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
