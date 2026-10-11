// Independent original critical-count/IRQmask semantics, frozen missing target resumed once.
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
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';need(bool(s),"Expected module state write failed");}
static SH4Context leave_return(const SH4Context& b,unsigned count,unsigned saved){auto c=b;c.pc=c.pr;c.r[3]=count-1;c.r[2]=c.r[3];c.r[4]=0x8c67ff20;c.sr=(b.sr&~1u)|(count==1?1u:0u);if(count==1){c.r[1]=0xffffff0f;c.r[2]=c.sr&~0xf0u;c.sr=c.r[2]|((saved&15u)<<4);c.r[0]=c.sr;}return c;}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
int main(int argc,char**argv){try{
 need(argc==5,"usage: critical-leave RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong critical-leave fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};
 auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Critical RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Critical RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"CPU-only critical leave changes frozen devices");};unsigned restored=0,nonzero=0;
 for(unsigned count:{0u,1u,2u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned savedmask=0;savedmask<(count==1?16u:1u);++savedmask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c65eace,mask,t,bank);if(pattern==0xaa)c.sr|=0x8302;unsigned saved=0x123456a0|savedmask;
  for(unsigned i=0;i<48;++i)rt.main_ram[0x67ff10+i]=static_cast<unsigned char>(pattern+13*i);
  for(unsigned i=0;i<64;++i)rt.main_ram[(c.r[15]&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);
  put(rt.main_ram,0x8c67ff20,count);put(rt.main_ram,0x8c67ff24,saved);auto expected=rt.main_ram;put(expected,0x8c67ff20,count-1);auto wanted=leave_return(c,count,saved);
  call_recompiled(c,rt,0x8c65eace);same(wanted,c);exact(expected);if(count==1)++restored;else ++nonzero;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c647dc8&&c.pr==0x8c05585c&&c.r[2]==0x8c65eace&&c.r[15]==0x8cffffbc&&c.sr==0x400000f0&&word(main,0x8c67ff20)==1&&word(main,0x8c67ff24)==0,"Frozen critical-leave boundary differs");
 auto expected=main;put(expected,0x8c67ff20,0);auto wanted=leave_return(c,1,0);auto next=wanted;next.pc=0x8c05585c;next.pr=0x8c055862;next.r[3]=0x8c09f6d2;
 // Save expectations before executing the captured missing target once.
 save_ram(expected,std::string(argv[4])+"/expected-critical-leave-ram.bin");save_state(wanted,std::string(argv[4])+"/expected-critical-leave-return-state.txt");save_state(next,std::string(argv[4])+"/expected-file-pool-prerequisite-state.txt");
 call_recompiled(c,rt,0x8c65eace);same(wanted,c);exact(expected);
 need(reject(c,rt,0x8c05585c,"0x8C09F6D2"),"Original caller did not reject09F6D2");same(next,c);exact(expected);
 std::cout<<"PASS original critical leave: "<<restored<<" zero-count restoration and "<<nonzero<<" nonzero decrement cases; all16 incoming/saved IRQ masks, high saved bits ignored, preserved SRflags/both T/both physical FPUbanks/two canary patterns; entireRAM/stack/all58fields; captured65EACE resumed once,count1->0,IRQmask15->0,original return05585C and explicit09F6D2 prerequisite; frozen devices/no interrupt delivery or normalboot acceptance\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
