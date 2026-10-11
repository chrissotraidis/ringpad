// Original state initializer prefix to genuine missing child; real known later helpers retained: full original RTS/delay return and exact RAM/CPU/TMU.
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
static void timer_same(const SH4TMUChannel&a,const SH4TMUChannel&b){need(a.tcor==b.tcor&&a.tcnt==b.tcnt&&a.tcr==b.tcr&&a.cycle_accumulator==b.cycle_accumulator&&a.lazy_sh4_cycles==b.lazy_sh4_cycles&&a.ticks==b.ticks&&a.underflows==b.underflows,"Timer channel differs");}
static void tick(auto& timers,unsigned start,unsigned cycles){for(unsigned i=0;i<3;++i)if(start&(1u<<i)){auto& ch=timers[i];need(ch.lazy_sh4_cycles==0&&(ch.tcr&7)<=4,"Unsupported synthetic timer initial state");const unsigned div=16u<<(2*(ch.tcr&7));const auto total=ch.cycle_accumulator+cycles;const auto n=total/div;need(n<=ch.tcnt,"Synthetic timer unexpectedly underflows");ch.cycle_accumulator=total%div;ch.tcnt-=n;ch.ticks+=n;}}
static void load_timers(DCRuntime& rt,const char* path){std::ifstream f(path);need(bool(f),"Missing observed timer state");std::string l;unsigned count=0;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!k.starts_with("tmu")&&!k.starts_with("sh4_tick_"))continue;auto v=std::stoull(l.substr(eq+1),nullptr,16);++count;if(k=="tmu_tocr")rt.tmu_tocr=v;else if(k=="tmu_tstr")rt.tmu_tstr=v;else if(k=="sh4_tick_pending_cycles")rt.sh4_tick_pending_cycles=v;else if(k=="sh4_tick_batch_cycles")rt.sh4_tick_batch_cycles=v;else{need(k.size()>5&&k[3]>='0'&&k[3]<='2'&&k[4]=='_',"Invalid observed timer key");auto& c=rt.tmu[k[3]-'0'];auto field=k.substr(5);if(field=="tcor")c.tcor=v;else if(field=="tcnt")c.tcnt=v;else if(field=="tcr")c.tcr=v;else if(field=="cycle_accumulator")c.cycle_accumulator=v;else if(field=="lazy_sh4_cycles")c.lazy_sh4_cycles=v;else if(field=="ticks")c.ticks=v;else if(field=="underflows")c.underflows=v;else throw std::runtime_error("Unknown timer field");}}
 need(count==25&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1&&(rt.tmu_tstr&~7u)==0,"Observed timer/batch fixture differs");}
static void save_timers(const auto&timers,unsigned start,unsigned tocr,const std::string& path){std::ofstream f(path);f<<std::hex<<"tmu_tocr="<<tocr<<"\ntmu_tstr="<<start<<"\nsh4_tick_pending_cycles=0\nsh4_tick_batch_cycles=1\n";for(unsigned i=0;i<3;++i){const auto& c=timers[i];auto out=[&](const char* key,auto v){f<<"tmu"<<std::dec<<i<<'_'<<key<<'='<<std::hex<<v<<'\n';};out("tcor",c.tcor);out("tcnt",c.tcnt);out("tcr",c.tcr);out("cycle_accumulator",c.cycle_accumulator);out("lazy_sh4_cycles",c.lazy_sh4_cycles);out("ticks",c.ticks);out("underflows",c.underflows);}need(bool(f),"Cannot save expected timers");}













static void put16(auto&m,unsigned a,unsigned v){m[a&0xffffff]=v&255;m[(a+1)&0xffffff]=(v>>8)&255;}
static SH4Context model(const SH4Context&b,auto&m){
 auto c=b;c.r[2]=word(m,0x8c04e684);c.r[3]=21;c.r[15]-=4;put(m,c.r[15],b.r[14]);c.r[14]=0;c.r[15]-=4;put(m,c.r[15],b.pr);put16(m,c.r[2],21);
 c.r[3]=word(m,0x8c04e688);c.r[1]=word(m,0x8c04e68c);c.r[2]=word(m,0x8c04e690);put16(m,c.r[3],0);c.pr=0x8c04e626;put(m,c.r[1],0);c.pc=0x8c04e60e;return c;
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: startup-state RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"State fixtures differ");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 for(unsigned target:{0x8c04e60eu,0x8c08c212u,0x8c08bd1cu,0x8c0513f8u,0x8c04e5e0u,0x8c04e60au})need(rt.targets.contains(target),"Real known later helper or tail continuation omitted");need(!rt.targets.contains(0x8c0547ea),"Genuine unregistered first child was silently replaced");
 auto reset=[&](unsigned start,unsigned pattern){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=pattern&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto&c=rt.tmu[i];c.tcor=0x11223300+i*17+pattern;c.tcnt=0x55667700+i*31+pattern;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m&&rt.aica_ram==sound&&rt.system_flash==flash,"State wholeRAM/guards/devices differ");need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"State unrelated runtime state changed");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"State timer controls differ");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 unsigned cases=0;
 for(unsigned pattern:{0x55u,0xaau})for(unsigned lanes=0;lanes<16;++lanes)for(unsigned saved_r14:{0u,0xffffffffu,0x89abcdefu,0x8ce34560u})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  const unsigned start=mask&7;reset(start,pattern);auto c=context(0x8c04e60e,mask,t,bank);c.pr=0x8c04e332;c.r[14]=saved_r14;c.r[15]=((lanes&8)?0xac000000u:0x8c000000u)|0xeff000;
  auto alias=[&](unsigned bit,unsigned offset){return((lanes&bit)?0xac000000u:0x8c000000u)|offset;};put(rt.main_ram,0x8c04e684,alias(1,0x74d434));put(rt.main_ram,0x8c04e688,alias(2,0x1a1b5c));put(rt.main_ram,0x8c04e68c,alias(4,0x7ac25c));
  for(auto [off,len]:{std::pair{0x74d424u,32u},std::pair{0x1a1b4cu,32u},std::pair{0x7ac24cu,32u},std::pair{0xefefd0u,96u}})for(unsigned i=0;i<len;++i)rt.main_ram[off+i]=pattern+19*i;
  auto m=rt.main_ram;auto wanted=model(c,m);auto timers=rt.tmu;tick(timers,start,13);const auto tocr=rt.tmu_tocr;need(reject(c,rt,0x8c04e60e,"0x8C0547EA"),"State prefix did not reject at genuine first child");same(wanted,c);exact(m,timers,start,tocr);++cases;
 }
 reset(0,0x55);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c04e32a&&c.pr==0x8c04e332&&c.r[15]==0x8cffffa0&&c.sr==0x40000101&&c.r[14]==0,"Frozen state caller differs");need(word(main,0x8c04e684)==0x8c74d434&&word(main,0x8c04e688)==0x8c1a1b5c&&word(main,0x8c04e68c)==0x8c7ac25c&&word(main,0x8c04e690)==0x8c0547ea,"Frozen state literals differ");
 auto m=main;auto wanted=model(c,m);auto timers=rt.tmu;tick(timers,rt.tmu_tstr,13);const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;save_state(wanted,std::string(argv[4])+"/expected-startup-state-prefix-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-state-prefix-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-state-prefix-ram.bin");need(reject(c,rt,0x8c04e60e,"0x8C0547EA"),"Frozen original first missing child differs");same(wanted,c);exact(m,timers,start,tocr);
 std::cout<<"PASS original04E60E startup state prefix: "<<cases<<" cases; savedR14-PR/twowords8byteframe/store16(21)74D434/zero16(1A1B5C)/JSR-zero32(7AC25C)delay/R1-R2-R3-R14/unchangedSR-MACL-FPU/P1-P2 independent three destinations-stack/whole16MiB RAM/69CPU/25TMU/13emittercycles; genuine missing0547EA/PC04E60E/PR04E626/SPFFFF98; real registered later08C212-08BD1C-0513F8-tail04E5E0/04E60A continuation retained. ChildFPU/fullinitializer/conditionalbranches/tail/fullreturn unexecuted; no hostglobal writes/knownhelper omission/initializer-completion/IRQ/boot/gameplay/audio/VMU/G1 claim\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
