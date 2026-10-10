// Original module buffer/table writes and explicit tail dependency; no completion stubs.
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
static void table(auto& m){std::fill_n(m.begin()+0x8ee064,64,0);for(unsigned i=0;i<8;++i)put(m,0x8c8ee068+8*i,15-i);}
static SH4Context table_return(const SH4Context& b){auto c=b;c.pc=c.pr;c.r[0]=c.r[6]=0x8c8ee064;c.r[3]=0;c.r[4]=c.r[7]=0x8c8ee0a4;c.r[5]=7;c.sr|=1;return c;}
static SH4Context module_return(const SH4Context& b){auto c=table_return(b);c.pc=c.pr=0x8c647d7c;c.r[15]+=4;c.r[2]=0x8c6800c0;c.r[3]=0x8c6800bc;c.r[4]=c.r[5]=0;return c;}
static SH4Context critical_return(const SH4Context& b,unsigned count){auto c=b;c.pc=c.pr;c.r[4]=0x8c67ff20;c.r[3]=count+1;c.sr=(b.sr&~1u)|(count==0);if(count==0){c.r[2]=0xffffff0f;c.r[0]=c.sr=(c.sr&0xffffff0f)|0xf0;}return c;}
// Captured missing-target boundary is after66B8F0 tail delay-slot POP_PR.
// Resume its missing target exactly once; rerunning the tail would pop twice.
static void resume_module(SH4Context& c,DCRuntime&rt){call_recompiled(c,rt,0x8c66e8dc);for(unsigned n=0;c.pc!=0x8c647d7c;++n){need(n<8,"Module continuation bound exhausted");call_recompiled(c,rt,c.pc);}}
static void save(const auto& m,const SH4Context& c,const std::string& stem){std::ofstream f(stem+"-ram.bin",std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save expected table RAM");save_state(c,stem+"-state.txt");}
int main(int argc,char**argv){try{
 need(argc==5,"usage: table-init RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong table fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Table RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Table RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Table changes frozen devices");};unsigned tables=0,modules=0,critical=0;
 for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();for(unsigned i=0;i<96;++i)rt.main_ram[0x8ee054+i]=static_cast<unsigned char>(pattern+13*i);auto c=context(0x8c66e8dc,mask,t,bank);auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);table(expected);auto wanted=table_return(c);call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected);++tables;
 }
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c66b8f0,mask,t,bank);c.pr=0x8c660800;put(rt.main_ram,c.r[15],0x8c647d7c);auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);table(expected);put(expected,0x8c6800bc,0);put(expected,0x8c6800c0,0);auto wanted=module_return(c);resume_module(c,rt);same(wanted,c);exact(expected);++modules;
 }
 for(unsigned count:{0u,1u,2u,0x7fffffffu,0x80000000u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();put(rt.main_ram,0x8c67ff20,count);put(rt.main_ram,0x8c67ff24,0xa5a5a5a5);auto c=context(0x8c65eaa8,mask,t,bank);auto expected=rt.main_ram;put(expected,0x8c67ff20,count+1);if(count==0)put(expected,0x8c67ff24,mask);auto wanted=critical_return(c,count);call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected);++critical;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c66b8f0&&c.pr==0x8c660800&&word(main,c.r[15])==0x8c647d7c&&word(main,0x8c67ff20)==0,"Frozen table caller differs");auto expected=main;put(expected,c.r[15]-4,c.pr);table(expected);put(expected,0x8c6800bc,0);put(expected,0x8c6800c0,0);auto wanted=module_return(c);save(expected,wanted,std::string(argv[4])+"/expected-module-return");resume_module(c,rt);same(wanted,c);exact(expected);
 put(expected,0x8c67ff20,1);put(expected,0x8c67ff24,(wanted.sr>>4)&15);wanted.pr=0x8c647d82;wanted=critical_return(wanted,0);wanted.pc=0x8c647d82;wanted.pr=0x8c647d88;wanted.r[2]=0x8c10f6ac;save(expected,wanted,std::string(argv[4])+"/expected-critical-boundary");need(reject(c,rt,0x8c647d7c,"0x8C10F6AC"),"Outer initializer dependency did not reject");same(wanted,c);exact(expected);
 std::cout<<"PASS original table initialization: "<<tables<<" table, "<<modules<<" module return and "<<critical<<" nested critical entry cases; all16 IRQ masks/both T/both physical FPU banks; entireRAM/stack/all58fields; frozen66B8F0 module return647D7C,65EAA8 count0->1/savedIRQmask0/IRQmask15; explicit10F6AC rejection; no outer initializer/interrupt delivery/device/response/playback stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
