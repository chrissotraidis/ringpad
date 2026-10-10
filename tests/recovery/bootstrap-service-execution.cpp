// Original flag store and initialization guard; no initializer or response stubs.
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
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing transfer fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xffffff),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;}
static void control(const SH4Context& b,const SH4Context& c,unsigned macl){need((b.sr&~1u)==(c.sr&~1u)&&b.mach==c.mach&&c.macl==macl&&b.gbr==c.gbr&&b.vbr==c.vbr&&b.spc==c.spc&&b.fpscr==c.fpscr&&b.fpul==c.fpul&&b.fr_bits==c.fr_bits&&b.xf_bits==c.xf_bits,"Transfer control/FPU differs");}
static void returned(const SH4Context& b,const SH4Context& c,unsigned result,unsigned macl){control(b,c,macl);need(c.pc==b.pr&&c.pr==b.pr&&c.r[0]==result,"Transfer return differs");for(unsigned i=8;i<16;++i)need(b.r[i]==c.r[i],"Transfer saved register differs");}
static bool reject(SH4Context& c,DCRuntime& rt,unsigned entry,const char* target){try{call_recompiled(c,rt,entry);}catch(const std::runtime_error& e){if(std::string(e.what()).find(target)!=std::string::npos)return true;throw;}return false;}
static SH4Context state(const char* path){std::ifstream f(path);need(bool(f),"Missing transfer state");SH4Context c{};std::string l;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!(k=="pc"||k=="pr"||k=="sr"||k=="fpscr"||k=="vbr"||k=="spc"||k=="gbr"||k=="mach"||k=="macl"||k=="fpul"||k[0]=='r'||k.starts_with("fr")||k.starts_with("xf")))continue;unsigned v=std::stoul(l.substr(eq+1),nullptr,16);if(k=="pc")c.pc=v;else if(k=="pr")c.pr=v;else if(k=="sr")c.sr=v;else if(k=="fpscr")c.fpscr=v;else if(k=="vbr")c.vbr=v;else if(k=="spc")c.spc=v;else if(k=="gbr")c.gbr=v;else if(k=="mach")c.mach=v;else if(k=="macl")c.macl=v;else if(k=="fpul")c.fpul=v;else if(k.starts_with("fr"))c.fr_bits[std::stoul(k.substr(2))]=v;else if(k.starts_with("xf"))c.xf_bits[std::stoul(k.substr(2))]=v;else if(k[0]=='r')c.r[std::stoul(k.substr(1))]=v;}return c;}
static void same(const SH4Context& a,const SH4Context& b){for(unsigned i=0;i<16;++i)if(a.r[i]!=b.r[i])std::cerr<<"Frozen register R"<<std::dec<<i<<" expected="<<std::hex<<a.r[i]<<" actual="<<b.r[i]<<"\n";if(a.pc!=b.pc||a.pr!=b.pr||a.sr!=b.sr||a.macl!=b.macl)std::cerr<<"Frozen PC/PR/SR/MACL expected="<<std::hex<<a.pc<<"/"<<a.pr<<"/"<<a.sr<<"/"<<a.macl<<" actual="<<b.pc<<"/"<<b.pr<<"/"<<b.sr<<"/"<<b.macl<<"\n";need(a.r==b.r&&a.pc==b.pc&&a.pr==b.pr&&a.sr==b.sr&&a.gbr==b.gbr&&a.vbr==b.vbr&&a.spc==b.spc&&a.mach==b.mach&&a.macl==b.macl&&a.fpul==b.fpul&&a.fpscr==b.fpscr&&a.fr_bits==b.fr_bits&&a.xf_bits==b.xf_bits,"Frozen transfer full state differs");}
int main(int argc,char**argv){try{
 need(argc==4,"usage: bootstrap-service RAM STATE FLASH");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong bootstrap fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Bootstrap RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Bootstrap RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Bootstrap changes frozen devices");};unsigned stores=0,guards=0;
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c647d5c,mask,t,bank);c.r[4]=value;auto expected=rt.main_ram;put(expected,0x8c677894,value);auto wanted=c;wanted.pc=c.pr;wanted.r[3]=0x8c677894;call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected);++stores;
 }
 for(unsigned count:{0u,1u,2u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();put(rt.main_ram,0x8c677898,count);put(rt.main_ram,0x8c67ff20,0xa5a5a5a5);auto c=context(0x8c647d62,mask,t,bank);auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);auto wanted=c;wanted.r[4]=0x8c677898;bool skip=static_cast<int>(count)>0;
  if(skip){wanted.pc=c.pr;wanted.r[3]=count;wanted.sr|=1;call_recompiled(c,rt,c.pc);}
  else{put(expected,0x8c677898,count+1);put(expected,0x8c67ff20,0);wanted.pc=0x8c65f208;wanted.pr=0x8c647d7c;wanted.r[1]=count+1;wanted.r[2]=0x8c65f208;wanted.r[3]=0x8c6607d2;wanted.r[15]-=4;wanted.sr&=~1u;need(reject(c,rt,c.pc,"0x8C6607D2"),"Initializer tail did not reject");}
  same(wanted,c);exact(expected);++guards;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c055848&&c.pr==0x8c055856&&c.r[4]==0&&word(main,0x8c677894)==1&&word(main,0x8c677898)==0,"Actual frozen bootstrap differs");auto expected=main;put(expected,0x8c677894,0);auto wanted=c;wanted.pc=c.pr;wanted.r[3]=0x8c677894;call_recompiled(c,rt,0x8c647d5c);same(wanted,c);exact(expected);
 put(expected,c.r[15]-4,0x8c05585c);put(expected,0x8c677898,1);put(expected,0x8c67ff20,0);wanted=c;wanted.pc=0x8c65f208;wanted.pr=0x8c647d7c;wanted.r[1]=1;wanted.r[2]=0x8c65f208;wanted.r[3]=0x8c6607d2;wanted.r[4]=0x8c677898;wanted.r[15]-=4;wanted.sr&=~1u;need(reject(c,rt,0x8c055856,"0x8C6607D2"),"Actual saved caller initializer did not reject");same(wanted,c);exact(expected);
 // A positive guard skips initialization but must still reject the caller's next dependency.
 reset();put(rt.main_ram,0x8c677898,1);c=state(argv[2]);c.pc=0x8c055856;expected=rt.main_ram;put(expected,c.r[15]-4,0x8c05585c);wanted=c;wanted.pc=0x8c05585c;wanted.pr=0x8c055862;wanted.r[2]=0x8c647d62;wanted.r[3]=0x8c09f6d2;wanted.r[4]=0x8c677898;wanted.sr|=1;need(reject(c,rt,c.pc,"0x8C09F6D2"),"Skipped initializer did not reject later caller dependency");same(wanted,c);exact(expected);
 std::cout<<"PASS original bootstrap service: "<<stores<<" flag-store and "<<guards<<" signed initialization guard cases; all16 interrupt masks/both T values/both FPU banks; entire RAM/stack/all58 fields exact; actual frozen caller clears flag1->0, count0->1 and module global0, rejects initializer6607D2; positive skip rejects caller09F6D2; no initializer, responses or completion stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
