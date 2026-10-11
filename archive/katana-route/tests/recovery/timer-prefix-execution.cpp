// Independent original timer-init register, stack and MMIO expectations.
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
static SH4Context prefix_boundary(const SH4Context& b){auto c=b;c.pc=0x8c06dd60;c.pr=0x8c06dd96;c.r[0]=0xbebc20;c.r[1]=0x8c64507e;c.r[2]=0xffd80018;c.r[3]=2;c.r[4]=0x8c06de78;c.r[5]=0;c.r[6]=0x8c790680;c.r[14]=0;c.r[15]-=8;return c;}
static void prefix_ram(auto& m,const SH4Context& b){put(m,b.r[15]-4,b.r[14]);put(m,b.r[15]-8,b.pr);put(m,0x8c790680,0xbebc20);}
static void timer_same(const SH4TMUChannel&a,const SH4TMUChannel&b){need(a.tcor==b.tcor&&a.tcnt==b.tcnt&&a.tcr==b.tcr&&a.cycle_accumulator==b.cycle_accumulator&&a.lazy_sh4_cycles==b.lazy_sh4_cycles&&a.ticks==b.ticks&&a.underflows==b.underflows,"Timer channel differs from independent expectation");}
static auto prefix_timers(const auto& before,unsigned start){auto timers=before;
 // One original28-instruction block emits a28-cycle tick before its MMIO writes.
 // Synthetic counters cannot underflow; predict the current per-channel divider independently.
 for(unsigned i=0;i<3;++i)if(start&(1u<<i)){auto& ch=timers[i];const unsigned div=16u<<(2*(ch.tcr&7));const auto total=ch.cycle_accumulator+28;const auto ticks=total/div;need(ticks<=ch.tcnt,"Synthetic timer unexpectedly underflows");ch.cycle_accumulator=total%div;ch.tcnt-=ticks;ch.ticks+=ticks;}
 timers[1].tcor=timers[1].tcnt=0xbebc20;timers[1].tcr=0;timers[1].cycle_accumulator=0;return timers;}
int main(int argc,char**argv){try{
 need(argc==5,"usage: timer-prefix RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong timer fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 auto reset=[&](unsigned start,unsigned pattern){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=pattern&1;for(unsigned i=0;i<3;++i){auto& c=rt.tmu[i];c.tcor=0x11223300+i*17+pattern;c.tcnt=0x55667700+i*31+pattern;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m,"Timer prefix wholeRAM/stack/guards differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0&&!rt.device_clock_enabled,"Timer prefix changes frozen unrelated devices");need(rt.tmu_tstr==(start|2)&&rt.tmu_tocr==tocr,"Timer start/other control differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 unsigned cases=0;
 for(unsigned start=0;start<8;++start)for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset(start,pattern);auto c=context(0x8c06dd60,mask,t,bank);if(pattern==0xaa)c.sr|=0x8302;
  for(unsigned i=0;i<64;++i)rt.main_ram[(c.r[15]&0xffffff)-32+i]=static_cast<unsigned char>(pattern+13*i);
  for(unsigned i=0;i<32;++i)rt.main_ram[0x790670+i]=static_cast<unsigned char>(pattern+7*i);
  auto expected=rt.main_ram;prefix_ram(expected,c);auto wanted=prefix_boundary(c);auto timers=prefix_timers(rt.tmu,start);const unsigned tocr=rt.tmu_tocr;
  need(reject(c,rt,0x8c06dd60,"0x8C64507E"),"Timer prefix failed to reject original registration wrapper");same(wanted,c);exact(expected,timers,start,tocr);++cases;
 }
 auto c=state(argv[2]);need(c.pc==0x8c0556ca&&c.pr==0x8c0556ce&&c.r[13]==0x8c06dd60&&c.r[15]==0x8cffffc8&&c.sr==0x40000001&&c.dbr==0x8c000010,"Frozen timer boundary differs");
 auto expected=main;prefix_ram(expected,c);auto wanted=prefix_boundary(c);
 save_ram(expected,std::string(argv[4])+"/expected-timer-prefix-ram.bin");save_state(wanted,std::string(argv[4])+"/expected-timer-prefix-state.txt");
 for(unsigned start=0;start<8;++start){reset(start,0x55);auto actual=c;auto timers=prefix_timers(rt.tmu,start);const unsigned tocr=rt.tmu_tocr;need(reject(actual,rt,0x8c06dd60,"0x8C64507E"),"Frozen prefix failed to reject wrapper");same(wanted,actual);exact(expected,timers,start,tocr);}
 std::cout<<"PASS original timer prefix: "<<cases<<" synthetic cases plus frozen06DD60 resumed once per8 hypothetical TSTR states; source timer1 reload/count00BEBC20/control0, TSTR bit1 set/other bits preserved, RAM790680 and savedR14/PR; whole16MiB RAM/stack/guards/all69CPUfields and all3timer records/TOCR; all16IRQmasks/bothT/bothphysicalFPUbanks/two canaries; lazy cycles0/device clock disabled, original28-cycle block tick included; actual startup timer values not captured, no numeric real-TMU prediction; explicit64507E rejection, no stub/completion/normalboot/gameplay/G1 claim\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
