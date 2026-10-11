// Original idle peripheral worker context capture and yield; no hardware/input acceptance.
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
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static SH4Context saved_context(const auto& m,unsigned a){
 SH4Context c{};c.r[15]=word(m,a);c.r[14]=word(m,a+4);c.pr=word(m,a+8);
 for(unsigned i=0;i<14;++i)c.r[13-i]=word(m,a+12+4*i);
 c.mach=word(m,a+68);c.macl=word(m,a+72);c.gbr=word(m,a+76);c.fpul=word(m,a+144);c.fpscr=word(m,a+148);
 auto& fr=(c.fpscr&0x200000)?c.xf_bits:c.fr_bits;for(unsigned i=0;i<16;++i)fr[i]=word(m,a+80+4*i);return c;
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
constexpr unsigned manager=0x8c8dabb8,descriptor=manager+0x210;
static SH4Context expected_capture(const SH4Context& before,unsigned delay,unsigned sequence,bool worker){
 auto c=before;
 if(worker){c.r[4]=0;c.r[14]=0x8c8983b8;c.r[9]=0x8c898398;c.r[10]=0xffffff01;c.r[8]=0x8c64c0da;c.r[11]=0;c.r[13]=0;}
 c.r[15]-=8;c.r[14]=0x8c8ee058;c.pr=0x8c66cb88;c.r[0]=0x2690;c.r[1]=(delay&0x80000000)?0x7fffffff:sequence+delay;c.r[2]=descriptor;c.r[3]=0x8c112574;c.r[4]=descriptor+24;return c;
}
static void predict_yield(auto& expected,const SH4Context& before,unsigned delay,unsigned sequence,bool worker){
 auto captured=expected_capture(before,delay,sequence,worker);capture(expected,descriptor+24,captured);put(expected,descriptor+20,captured.r[1]);put(expected,0x8c7b0b44,1);
 put(expected,before.r[15]-4,worker?0x8c8983b8:before.r[14]);put(expected,before.r[15]-8,worker?0x8c64b8b4:before.pr);
}
int main(int argc,char**argv){try{
 need(argc==4,"usage: worker-oracle RAM STATE FLASH");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong worker fixtures");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;unsigned delay_cases=0,worker_cases=0;
 auto exact=[&](const auto& expected){
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<'\n';break;}throw std::runtime_error("Worker exact RAM differs");}
  need(rt.system_flash==flash && rt.aica_ram==sound,"Worker flash/sound guards differ");
 };
 auto setup=[&](unsigned sequence,unsigned bank,unsigned pattern){
  rt.main_ram=main;auto saved=incoming(bank,pattern);saved.pr=0x8cdead00;capture(rt.main_ram,manager+4,saved);put(rt.main_ram,manager+0x204,sequence);put(rt.main_ram,manager+0x2690,descriptor);put(rt.main_ram,0x8c8ee058,manager);return saved;
 };
 for(unsigned delay:{0u,1u,7u,0x80000000u,0xffffffffu})for(unsigned sequence:{0u,17u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned target_bank:{0u,1u})for(unsigned pattern=0;pattern<3;++pattern)for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u}){
  auto saved=setup(sequence,target_bank,pattern);auto c=context(0x8c66cb5e,mask,t,bank,pattern);c.r[4]=delay;c.pr=0x8c64b8b4;auto before=c;auto expected=rt.main_ram;predict_yield(expected,before,delay,sequence,false);call_recompiled(c,rt,c.pc);restored(before,saved,c,1);exact(expected);++delay_cases;
 }
 for(unsigned operation:{0u,2u,19u,21u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned sequence:{0u,17u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned target_bank:{0u,1u})for(unsigned pattern=0;pattern<3;++pattern)for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u}){
  auto saved=setup(sequence,target_bank,pattern);put(rt.main_ram,0x8c8983c0,operation);auto c=context(0x8c64b64e,mask,t,bank,pattern);auto before=c;auto expected=rt.main_ram;predict_yield(expected,before,1,sequence,true);call_recompiled(c,rt,c.pc);restored(before,saved,c,1);exact(expected);++worker_cases;
 }
 // Execute the actual frozen worker context, then resume its original scheduler block.
 rt.main_ram=main;auto c=load_state(argv[2]);need(c.pc==0x8c64b64e && word(main,0x8c8983c0)==0,"Frozen worker entry differs");const auto before=c,kernel=saved_context(main,manager+4);auto expected=main;const unsigned sequence=word(main,manager+0x204);predict_yield(expected,before,1,sequence,true);call_recompiled(c,rt,c.pc);restored(before,kernel,c,1);exact(expected);
 call_recompiled(c,rt,c.pc);put(expected,manager+0x204,sequence+1);put(expected,0x8c8ee058,word(main,0x8c8ee05c));const unsigned capacity=word(main,manager+0x208);need(capacity==8,"Frozen capacity differs");put(expected,manager+0x2690,manager+0x210+(capacity-1)*0x124);exact(expected);
 const unsigned stack=kernel.r[15];need(c.pc==word(main,stack+4) && c.pr==c.pc && c.r[15]==stack+32 && c.r[0]==0x204 && c.sr==((before.sr&~1u)|1u),"Frozen scheduler return differs");
 for(unsigned i=0;i<6;++i)need(c.r[9+i]==word(main,stack+8+4*i),"Frozen scheduler restored callee register differs");need(c.r[8]==kernel.r[8] && c.fpscr==word(main,stack),"Frozen scheduler preserved R8/FPSCR differs");
 // Unselected operation1 remains an explicit missing dependency, not success.
 rt.main_ram=main;put(rt.main_ram,0x8c8983c0,1);c=before;bool rejected=false;try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error&e){rejected=std::string(e.what()).find("0x8C64BFE0")!=std::string::npos;}need(rejected && c.pc==0x8c64b73e && c.pr==0x8c64b748,"Missing operation target did not reject at its source block");
 std::cout<<"PASS original peripheral worker: "<<delay_cases<<" direct delay/negative/wrap, "<<worker_cases<<" idle/out-of-range operation cases; both FPU banks, exact context/stack/RAM/flash/sound; actual frozen yield and scheduler return; uncompiled operation1 rejected\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
