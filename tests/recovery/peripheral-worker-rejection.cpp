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
 need(argc==4,"usage: rejection-oracle RAM STATE FLASH");auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,read(argv[3]));rt.main_ram=read(argv[1]);auto c=load_state(argv[2]);put(rt.main_ram,0x8c8983c0,1);bool rejected=false;
 try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error&e){std::cout<<e.what()<<'\n';rejected=std::string(e.what()).find("0x8C64BFE0")!=std::string::npos;}
 std::cout<<std::hex<<"pc="<<c.pc<<" pr="<<c.pr<<" sp="<<c.r[15]<<'\n';need(rejected && c.pc==0x8c64b73e && c.pr==0x8c64b748,"Missing operation target did not reject at its source block");std::cout<<"PASS isolated uncompiled operation1 rejection; source block73E, return748, target64BFE0\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
