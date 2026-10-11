// Independent file-record words and original caller stack restoration.
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
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';s<<"ssr="<<std::hex<<c.ssr<<"\nsgr="<<c.sgr<<'\n';for(unsigned i=0;i<8;++i)s<<"bank_r"<<std::dec<<i<<'='<<std::hex<<c.r_bank[i]<<'\n';need(bool(s),"Expected module state write failed");}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
static void reset_records(auto& m){for(unsigned i=0;i<16;++i)put(m,0x8c7a4d0c+i*40,0);put(m,0x8c7a4f8c,0);}
static SH4Context record_return(const SH4Context& b){auto c=b;c.pc=c.pr;c.r[3]=c.r[4]=c.r[6]=0x8c7a4f8c;c.r[5]=0;c.r[7]=0x8c7a4d0c;c.sr|=1;return c;}
static SH4Context tail_boundary(const SH4Context& b,const auto& m){auto c=b;c.pc=0x8c055862;c.pr=word(m,c.r[15]);c.r[13]=word(m,c.r[15]+4);c.r[14]=word(m,c.r[15]+8);c.r[15]+=12;c.r[2]=0x8c60549e;c.r[3]=0x8c7869f4;c.r[4]=0x8c09ea9a;c.r[5]=0;return c;}
int main(int argc,char**argv){try{
 need(argc==5,"usage: file-pool RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong file-pool fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};
 auto exact=[&](const auto&m){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"File-pool RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("File-pool RAM/stack/guards differ");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"File-pool changes frozen devices");};unsigned records=0,tails=0;
 for(unsigned entry:{0x8c09f6d2u,0x8c055862u})for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(entry,mask,t,bank);if(pattern==0xaa)c.sr|=0x8302;
  for(unsigned i=0;i<0x2a4;++i)rt.main_ram[0x7a4cfc+i]=static_cast<unsigned char>(pattern+13*i);
  for(unsigned i=0;i<64;++i)rt.main_ram[(c.r[15]&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);
  put(rt.main_ram,c.r[15],0xffffffff);put(rt.main_ram,c.r[15]+4,0x13579bdf);put(rt.main_ram,c.r[15]+8,0x2468ace0);put(rt.main_ram,0x8c7869f4,0xaabbccdd);auto expected=rt.main_ram;
  if(entry==0x8c09f6d2){reset_records(expected);auto wanted=record_return(c);call_recompiled(c,rt,entry);same(wanted,c);++records;}
  else{put(expected,0x8c7869f4,0);auto wanted=tail_boundary(c,expected);need(reject(c,rt,entry,"0x8C60549E"),"Caller tail did not reject60549E");same(wanted,c);++tails;}
  exact(expected);
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c05585c&&c.pr==0x8c055862&&c.r[3]==0x8c09f6d2&&c.r[15]==0x8cffffbc&&c.sr==0x40000001&&word(main,0x8c67ff20)==0,"Frozen file-pool boundary differs");
 auto expected=main;reset_records(expected);auto wanted=record_return(c);auto final_ram=expected;put(final_ram,0x8c7869f4,0);auto final=tail_boundary(wanted,final_ram);
 // Expectations include58 prior CPU fields plus SSR/SGR/eight banked registers.
 // DBR is preserved in synthetic tests; its prior value was not captured.
 save_ram(expected,std::string(argv[4])+"/expected-file-pool-return-ram.bin");save_state(wanted,std::string(argv[4])+"/expected-file-pool-return-state.txt");save_ram(final_ram,std::string(argv[4])+"/expected-caller-tail-ram.bin");save_state(final,std::string(argv[4])+"/expected-caller-tail-state.txt");
 call_recompiled(c,rt,0x8c09f6d2);same(wanted,c);exact(expected);
 need(reject(c,rt,0x8c055862,"0x8C60549E"),"Original caller tail did not reject60549E");same(final,c);exact(final_ram);
 std::cout<<"PASS original file-pool reset: "<<records<<" record and "<<tails<<" caller-tail cases;16 zero words/40-byte stride/global, other record bytes/guards preserved; two patterns/all16 IRQmasks/SRflags/bothT/both physical FPUbanks; entireRAM/stack/68 captured CPUfields, DBR preserved in synthetic tests; frozen09F6D2 resumed once, original055862 flag reset and PR/R13/R14/SP restores, explicit60549E reject; no completion/device/response stub\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
