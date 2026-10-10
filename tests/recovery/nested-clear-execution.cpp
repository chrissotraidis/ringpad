// Independent expectations for original clears and return stack; no completion stubs.
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
static void clear(auto& m,unsigned a,unsigned n){std::fill_n(m.begin()+(a&0xffffff),n,0);}
static SH4Context cleared(const SH4Context& b,unsigned a,unsigned n){auto c=b;c.pc=c.pr;c.r[0]=c.r[4]=a;c.r[3]=c.r[5]=0;c.r[6]=c.r[7]=n;c.sr|=1;return c;}
static SH4Context parent_return(const SH4Context& b){auto c=cleared(b,0x8c8a7b90,0x480);c.r[2]=0x8c10ef3c;return c;}
static SH4Context reset_return(const SH4Context& b){auto c=cleared(b,0x8c8a7a90,0x100);c.r[2]=0x8c67ff28;c.r[3]=0x8c67ff2c;c.r[4]=0;return c;}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
int main(int argc,char**argv){try{
 need(argc==5,"usage: nested-clear RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong nested-clear fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};
 auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Clear RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Clear RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Clear changes frozen devices");};unsigned cases=0;
 for(unsigned entry:{0x8c6488a4u,0x8c65f328u,0x8c66b0b0u,0x8c65f174u})for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(entry,mask,t,bank);
  for(auto [a,n]:{std::pair{0x894900u,0x1a0u},std::pair{0x8da42cu,0x560u},std::pair{0x8a7b90u,0x480u},std::pair{0x8a7a90u,0x100u}})for(unsigned i=0;i<n+32;++i)rt.main_ram[a-16+i]=static_cast<unsigned char>(pattern+13*i);
  for(unsigned i=0;i<64;++i)rt.main_ram[(c.r[15]&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);
  put(rt.main_ram,0x8c67ff28,0xaabbccdd);put(rt.main_ram,0x8c67ff2c,0x11223344);auto expected=rt.main_ram;auto wanted=c;
  if(entry==0x8c6488a4){clear(expected,0x8c894900,0x1a0);wanted=cleared(c,0x8c894900,0x1a0);}
  else if(entry==0x8c65f174){put(expected,c.r[15]-4,c.pr);clear(expected,0x8c8a7a90,0x100);put(expected,0x8c67ff28,0);put(expected,0x8c67ff2c,0);wanted=reset_return(c);}
  else{put(expected,c.r[15]-4,c.pr);clear(expected,0x8c894900,0x1a0);clear(expected,0x8c8da42c,0x560);if(entry==0x8c65f328){put(expected,c.r[15]-8,0x8c65f330);clear(expected,0x8c8a7b90,0x480);wanted=parent_return(c);}else{wanted=cleared(c,0x8c8da42c,0x560);wanted.r[2]=0x8c10ef3c;}}
  call_recompiled(c,rt,entry);same(wanted,c);exact(expected);++cases;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c66b0b0&&c.pr==0x8c66b0b8&&c.r[3]==0x8c6488a4&&word(main,c.r[15])==0x8c65f330&&word(main,c.r[15]+4)==0x8c647da0&&word(main,0x8c677894)==0,"Frozen missing-call context differs");
 auto expected=main;clear(expected,0x8c894900,0x1a0);clear(expected,0x8c8da42c,0x560);clear(expected,0x8c8a7b90,0x480);auto chain=parent_return(c);chain.pc=chain.pr=0x8c647da0;chain.r[15]+=8;
 auto final_ram=expected;clear(final_ram,0x8c8a7a90,0x100);put(final_ram,c.r[15]+4,0x8c647da6);put(final_ram,0x8c67ff28,0);put(final_ram,0x8c67ff2c,0);clear(final_ram,0x8c6774f0,0x3a0);
 auto final=cleared(reset_return(chain),0x8c6774f0,0x3a0);final.pc=0x8c647dc8;final.pr=word(main,chain.r[15]);final.r[15]+=4;final.r[0]=0;final.r[2]=0x8c65eace;final.r[3]=0x8c677894;final.sr&=~1u;
 // Save both independent predictions before executing the captured missing call.
 save_ram(expected,std::string(argv[4])+"/expected-nested-clear-return-ram.bin");save_state(chain,std::string(argv[4])+"/expected-nested-clear-return-state.txt");
 save_ram(final_ram,std::string(argv[4])+"/expected-critical-leave-boundary-ram.bin");save_state(final,std::string(argv[4])+"/expected-critical-leave-boundary-state.txt");
 call_recompiled(c,rt,0x8c6488a4);need(c.pc==0x8c66b0b8,"First clear return differs");call_recompiled(c,rt,0x8c66b0b8);need(c.pc==0x8c65f330,"Second clear return differs");call_recompiled(c,rt,0x8c65f330);same(chain,c);exact(expected);
 need(reject(c,rt,0x8c647da0,"0x8C65EACE"),"Frozen outer continuation did not reject65EACE");same(final,c);exact(final_ram);
 std::cout<<"PASS original nested clears: "<<cases<<" cases, four entries/two canary patterns/all16 IRQ masks/both T/both physical FPUbanks; entireRAM/stack/all58fields; frozen6488A4 resumed once, original1A0/560/480/100/3A0 clears and two global resets, saved returns; explicit65EACE reject; criticalcount1/IRQmask15 retained; no completion/device/response stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
