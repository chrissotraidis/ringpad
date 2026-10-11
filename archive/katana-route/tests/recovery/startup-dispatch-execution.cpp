// Independent original object selection, stack word and callback argument stores.
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
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}c.pc=entry;c.ssr=0x40008001;c.sgr=0x8ce34560;c.dbr=0xa1234567;for(unsigned i=0;i<8;++i)c.r_bank[i]=0xdead0010+i;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;}
static void control(const SH4Context& b,const SH4Context& c,unsigned macl){need((b.sr&~1u)==(c.sr&~1u)&&b.mach==c.mach&&c.macl==macl&&b.gbr==c.gbr&&b.vbr==c.vbr&&b.spc==c.spc&&b.fpscr==c.fpscr&&b.fpul==c.fpul&&b.fr_bits==c.fr_bits&&b.xf_bits==c.xf_bits,"Transfer control/FPU differs");}
static void returned(const SH4Context& b,const SH4Context& c,unsigned result,unsigned macl){control(b,c,macl);need(c.pc==b.pr&&c.pr==b.pr&&c.r[0]==result,"Transfer return differs");for(unsigned i=8;i<16;++i)need(b.r[i]==c.r[i],"Transfer saved register differs");}
static bool reject(SH4Context& c,DCRuntime& rt,unsigned entry,const char* target){try{call_recompiled(c,rt,entry);}catch(const std::runtime_error& e){if(std::string(e.what()).find(target)!=std::string::npos)return true;throw;}return false;}
static SH4Context state(const char* path){std::ifstream f(path);need(bool(f),"Missing transfer state");SH4Context c{};std::string l;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!(k=="pc"||k=="pr"||k=="sr"||k=="fpscr"||k=="vbr"||k=="spc"||k=="gbr"||k=="mach"||k=="macl"||k=="fpul"||k=="ssr"||k=="sgr"||k=="dbr"||k.starts_with("bank_r")||k[0]=='r'||k.starts_with("fr")||k.starts_with("xf")))continue;unsigned v=std::stoul(l.substr(eq+1),nullptr,16);if(k=="ssr")c.ssr=v;else if(k=="sgr")c.sgr=v;else if(k=="dbr")c.dbr=v;else if(k.starts_with("bank_r"))c.r_bank[std::stoul(k.substr(6))]=v;else if(k=="pc")c.pc=v;else if(k=="pr")c.pr=v;else if(k=="sr")c.sr=v;else if(k=="fpscr")c.fpscr=v;else if(k=="vbr")c.vbr=v;else if(k=="spc")c.spc=v;else if(k=="gbr")c.gbr=v;else if(k=="mach")c.mach=v;else if(k=="macl")c.macl=v;else if(k=="fpul")c.fpul=v;else if(k.starts_with("fr"))c.fr_bits[std::stoul(k.substr(2))]=v;else if(k.starts_with("xf"))c.xf_bits[std::stoul(k.substr(2))]=v;else if(k[0]=='r')c.r[std::stoul(k.substr(1))]=v;}return c;}
static void same(const SH4Context& a,const SH4Context& b){for(unsigned i=0;i<16;++i)if(a.r[i]!=b.r[i])std::cerr<<"Frozen register R"<<std::dec<<i<<" expected="<<std::hex<<a.r[i]<<" actual="<<b.r[i]<<"\n";if(a.pc!=b.pc||a.pr!=b.pr||a.sr!=b.sr||a.macl!=b.macl)std::cerr<<"Frozen PC/PR/SR/MACL expected="<<std::hex<<a.pc<<"/"<<a.pr<<"/"<<a.sr<<"/"<<a.macl<<" actual="<<b.pc<<"/"<<b.pr<<"/"<<b.sr<<"/"<<b.macl<<"\n";need(a.ssr==b.ssr&&a.sgr==b.sgr&&a.dbr==b.dbr&&a.r_bank==b.r_bank&&a.r==b.r&&a.pc==b.pc&&a.pr==b.pr&&a.sr==b.sr&&a.gbr==b.gbr&&a.vbr==b.vbr&&a.spc==b.spc&&a.mach==b.mach&&a.macl==b.macl&&a.fpul==b.fpul&&a.fpscr==b.fpscr&&a.fr_bits==b.fr_bits&&a.xf_bits==b.xf_bits,"Frozen transfer full state differs");}
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';s<<"dbr="<<std::hex<<c.dbr<<'\n';s<<"ssr="<<std::hex<<c.ssr<<"\nsgr="<<c.sgr<<'\n';for(unsigned i=0;i<8;++i)s<<"bank_r"<<std::dec<<i<<'='<<std::hex<<c.r_bank[i]<<'\n';need(bool(s),"Expected module state write failed");}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
static SH4Context callback_return(const SH4Context& b){auto c=b;c.pc=c.pr;c.r[0]=b.sr;c.r[2]=b.sr&0xffffff0fu;c.r[3]=0xffffff0f;c.r[6]=(b.sr>>4)&15;c.r[7]=0x8c677154;return c;}
static SH4Context dispatch_return(const SH4Context& b,bool selected){auto c=b;c.r[3]=0x8c677160;c.sr=(b.sr&~1u)|(selected?0:1);c.pc=c.pr;if(selected){c.r[1]=0x8c604490;c=callback_return(c);}return c;}
static SH4Context dispatch_reject(const SH4Context& b,unsigned target){auto c=b;c.pc=0x8c6054a8;c.sr&=~1u;c.r[0]=0x74;c.r[1]=target;c.r[3]=0x8c677160;return c;}
static SH4Context loop_boundary(const SH4Context& b){auto c=b;c.pc=0x8c0556ca;c.pr=0x8c0556ce;c.sr|=1;c.r[2]=0;c.r[8]=0x8c7649f0;c.r[9]=0x8c10b992;c.r[10]=0x8c164ae4;c.r[11]=0x8c010b7a;c.r[12]=0x8c010b58;c.r[13]=0x8c06dd60;c.r[14]=0x8c1a2c94;return c;}
int main(int argc,char**argv){try{
 need(argc==5,"usage: startup-dispatch RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong startup dispatch fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};
 auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Dispatch RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Dispatch RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Dispatch changes frozen devices");};unsigned cases[4]={};
 for(unsigned kind=0;kind<4;++kind)for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();const unsigned entry=kind==2?0x8c604490:0x8c60549e;auto c=context(entry,mask,t,bank);if(pattern==0xaa)c.sr|=0x8302;
  c.r[4]=0x12345678+pattern;c.r[5]=0x87654321-pattern;const unsigned object=0x8cde1000,destination=0x8cde2000;
  for(unsigned i=0;i<256;++i){rt.main_ram[(object&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);rt.main_ram[(destination&0xffffff)-32+i]=static_cast<unsigned char>(pattern+7*i);}
  for(unsigned i=0;i<64;++i)rt.main_ram[(c.r[15]&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);
  put(rt.main_ram,0x8c677160,kind==0?0:object);put(rt.main_ram,object+0x74,kind==3?0x8c123456:0x8c604490);put(rt.main_ram,0x8c677154,destination);auto expected=rt.main_ram;
  if(kind!=2)put(expected,c.r[15]-4,c.r[14]);
  if(kind==1||kind==2){put(expected,destination+36,c.r[4]);put(expected,destination+40,c.r[5]);}
  auto wanted=kind==2?callback_return(c):kind==3?dispatch_reject(c,0x8c123456):dispatch_return(c,kind==1);
  if(kind==3)need(reject(c,rt,entry,"0x8C123456"),"Unknown object callback did not reject");else call_recompiled(c,rt,entry);
  same(wanted,c);exact(expected);++cases[kind];
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c055862&&c.pr==0x8c0556b6&&c.r[2]==0x8c60549e&&c.r[15]==0x8cffffc8&&c.sr==0x40000001&&c.dbr==0x8c000010,"Frozen startup dispatcher boundary differs");
 need(word(main,0x8c677160)==0x8c6701f4&&word(main,0x8c6701f4+0x74)==0x8c604490&&word(main,0x8c677154)==0x8c698420&&word(main,0x8c1a2c94)==0,"Frozen original object/callback/loop flag differs");
 auto expected=main;put(expected,c.r[15]-4,c.r[14]);put(expected,0x8c698420+36,c.r[4]);put(expected,0x8c698420+40,c.r[5]);auto wanted=dispatch_return(c,true);auto final=loop_boundary(wanted);
 // All69 captured fields, wholeRAM; save independently derived expectations before execution.
 save_ram(expected,std::string(argv[4])+"/expected-dispatch-return-ram.bin");save_state(wanted,std::string(argv[4])+"/expected-dispatch-return-state.txt");save_state(final,std::string(argv[4])+"/expected-loop-boundary-state.txt");
 call_recompiled(c,rt,0x8c60549e);same(wanted,c);exact(expected);
 // Loop prefix expectation is source-derived for the later full run, not executed by this two-function fixture.
 std::cout<<"PASS original startup dispatcher: "<<cases[0]<<" null, "<<cases[1]<<" selected callback, "<<cases[2]<<" direct stores, "<<cases[3]<<" unregistered rejection cases; all16IRQmasks/SRflags/bothT/both physical FPUbanks/two patterns; entire16MiB RAM/stack/guards/all69CPUfields includingDBR; frozen60549E resumed once, original604490 storesR4/R5 with restored IRQmask and original return0556B6; loop06DD60 boundary predicted but not executed; no completion/device/response stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
