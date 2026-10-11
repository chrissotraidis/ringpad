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
static void writes(auto& m,bool whole){
 if(whole){std::fill_n(m.begin()+0x8a8c90,0x4e0,0);std::fill_n(m.begin()+0x8a9170,0x820,0);for(unsigned i=0;i<512;++i)put(m,0x8c8a9990+4*i,0x80808080);}
 std::fill_n(m.begin()+0x8da98c,0x220,0);
}
static SH4Context expected_clear(const SH4Context& before,bool whole){
 auto c=before;c.pc=0x8c66b8f0;c.pr=whole?0x8c660800:before.pr;c.r[0]=0x8c8da98c;c.r[2]=0x8c66e8dc;c.r[3]=0;c.r[4]=0x8c8da98c;c.r[5]=0;c.r[6]=c.r[7]=0x220;c.sr|=1;if(whole)c.r[15]-=4;return c;
}
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';need(bool(s),"Expected module state write failed");}
int main(int argc,char**argv){try{
 need(argc==5,"usage: module-init RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong module fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Module RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Module RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Module changes frozen devices");};unsigned init=0,clear=0,noop=0,stores=0;
 for(unsigned entry:{0x8c6607d2u,0x8c65f208u,0x8c66b8e4u})for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();for(auto range:{std::pair{0x8a8c90u,0x1500u},std::pair{0x8da98cu,0x220u}})for(unsigned i=0;i<range.second+32;++i)rt.main_ram[range.first-16+i]=static_cast<unsigned char>(pattern+i*13);auto c=context(entry,mask,t,bank);auto expected=rt.main_ram;const bool whole=entry!=0x8c66b8e4;put(expected,c.r[15]-4,c.pr);if(whole)put(expected,c.r[15]-8,0x8c660800);writes(expected,whole);auto wanted=expected_clear(c,whole);need(reject(c,rt,entry,"0x8C66E8DC"),"Module tail did not reject");same(wanted,c);exact(expected);if(whole)++init;else ++clear;
 }
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c6607ce,mask,t,bank);auto wanted=c;wanted.pc=c.pr;call_recompiled(c,rt,c.pc);same(wanted,c);exact(main);++noop;
 }
 for(auto values:{std::pair{0u,0u},std::pair{1u,0xffffffffu},std::pair{0x80000000u,0x12345678u},std::pair{0xffffffffu,0x80000000u}})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c66b8b0,mask,t,bank);c.r[4]=values.first;c.r[5]=values.second;auto expected=main;put(expected,0x8c6800bc,values.first);put(expected,0x8c6800c0,values.second);auto wanted=c;wanted.pc=c.pr;wanted.r[3]=0x8c6800bc;wanted.r[2]=0x8c6800c0;call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected);++stores;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c65f208&&c.pr==0x8c647d7c&&c.r[3]==0x8c6607d2,"Actual frozen module caller differs");auto expected=main;put(expected,c.r[15]-4,c.pr);put(expected,c.r[15]-8,0x8c660800);writes(expected,true);auto wanted=expected_clear(c,true);
 // Independent expected whole RAM and58-field state saved before actual frozen execution.
 {std::ofstream out(std::string(argv[4])+"/expected-initializer-ram.bin",std::ios::binary);out.write(reinterpret_cast<const char*>(expected.data()),expected.size());need(bool(out),"Cannot save expected module RAM");}save_state(wanted,std::string(argv[4])+"/expected-initializer-state.txt");
 need(reject(c,rt,c.pc,"0x8C66E8DC"),"Actual frozen module tail did not reject");same(wanted,c);exact(expected);
 std::cout<<"PASS original module initialization: "<<init<<" init/caller, "<<clear<<" child clear, "<<noop<<" original RTS and "<<stores<<" two-global setter cases; all16 interrupt masks/both T/both physical FPU banks; entire RAM/stack/all58 fields; actual frozen65F208 caller and saved prospective states; zero4E0/820/220 bytes and51280808080 words; explicit66E8DC reject, initializer not complete; no device/response stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
