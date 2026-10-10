// Original04562E zero-flag return; nonzero path observed only at first real callee entry.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <functional>
#include <stdexcept>
#include <sstream>
using namespace dcrecomp_generated;
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing transfer fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xffffff),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}c.pc=entry;c.ssr=0x40008001;c.sgr=0x8ce34560;c.dbr=0xa1234567;for(unsigned i=0;i<8;++i)c.r_bank[i]=0xdead0010+i;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;}
static SH4Context state(const char* path){std::ifstream f(path);need(bool(f),"Missing transfer state");SH4Context c{};std::string l;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!(k=="pc"||k=="pr"||k=="sr"||k=="fpscr"||k=="vbr"||k=="spc"||k=="gbr"||k=="mach"||k=="macl"||k=="fpul"||k=="ssr"||k=="sgr"||k=="dbr"||k.starts_with("bank_r")||k[0]=='r'||k.starts_with("fr")||k.starts_with("xf")))continue;unsigned v=std::stoul(l.substr(eq+1),nullptr,16);if(k=="ssr")c.ssr=v;else if(k=="sgr")c.sgr=v;else if(k=="dbr")c.dbr=v;else if(k.starts_with("bank_r"))c.r_bank[std::stoul(k.substr(6))]=v;else if(k=="pc")c.pc=v;else if(k=="pr")c.pr=v;else if(k=="sr")c.sr=v;else if(k=="fpscr")c.fpscr=v;else if(k=="vbr")c.vbr=v;else if(k=="spc")c.spc=v;else if(k=="gbr")c.gbr=v;else if(k=="mach")c.mach=v;else if(k=="macl")c.macl=v;else if(k=="fpul")c.fpul=v;else if(k.starts_with("fr"))c.fr_bits[std::stoul(k.substr(2))]=v;else if(k.starts_with("xf"))c.xf_bits[std::stoul(k.substr(2))]=v;else if(k[0]=='r')c.r[std::stoul(k.substr(1))]=v;}return c;}
static void same(const SH4Context& a,const SH4Context& b){for(unsigned i=0;i<16;++i)if(a.r[i]!=b.r[i])std::cerr<<"Frozen register R"<<std::dec<<i<<" expected="<<std::hex<<a.r[i]<<" actual="<<b.r[i]<<"\n";if(a.pc!=b.pc||a.pr!=b.pr||a.sr!=b.sr||a.macl!=b.macl)std::cerr<<"Frozen PC/PR/SR/MACL expected="<<std::hex<<a.pc<<"/"<<a.pr<<"/"<<a.sr<<"/"<<a.macl<<" actual="<<b.pc<<"/"<<b.pr<<"/"<<b.sr<<"/"<<b.macl<<"\n";need(a.ssr==b.ssr&&a.sgr==b.sgr&&a.dbr==b.dbr&&a.r_bank==b.r_bank&&a.r==b.r&&a.pc==b.pc&&a.pr==b.pr&&a.sr==b.sr&&a.gbr==b.gbr&&a.vbr==b.vbr&&a.spc==b.spc&&a.mach==b.mach&&a.macl==b.macl&&a.fpul==b.fpul&&a.fpscr==b.fpscr&&a.fr_bits==b.fr_bits&&a.xf_bits==b.xf_bits,"Frozen transfer full state differs");}
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';s<<"dbr="<<std::hex<<c.dbr<<'\n';s<<"ssr="<<std::hex<<c.ssr<<"\nsgr="<<c.sgr<<'\n';for(unsigned i=0;i<8;++i)s<<"bank_r"<<std::dec<<i<<'='<<std::hex<<c.r_bank[i]<<'\n';need(bool(s),"Expected module state write failed");}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
static void timer_same(const SH4TMUChannel&a,const SH4TMUChannel&b){need(a.tcor==b.tcor&&a.tcnt==b.tcnt&&a.tcr==b.tcr&&a.cycle_accumulator==b.cycle_accumulator&&a.lazy_sh4_cycles==b.lazy_sh4_cycles&&a.ticks==b.ticks&&a.underflows==b.underflows,"Timer channel differs");}
static void tick(auto& timers,unsigned start,unsigned cycles){for(unsigned i=0;i<3;++i)if(start&(1u<<i)){auto& ch=timers[i];need(ch.lazy_sh4_cycles==0&&(ch.tcr&7)<=4,"Unsupported synthetic timer initial state");const unsigned div=16u<<(2*(ch.tcr&7));const auto total=ch.cycle_accumulator+cycles;const auto n=total/div;need(n<=ch.tcnt,"Synthetic timer unexpectedly underflows");ch.cycle_accumulator=total%div;ch.tcnt-=n;ch.ticks+=n;}}
static void load_timers(DCRuntime& rt,const char* path){std::ifstream f(path);need(bool(f),"Missing observed timer state");std::string l;unsigned count=0;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!k.starts_with("tmu")&&!k.starts_with("sh4_tick_"))continue;auto v=std::stoull(l.substr(eq+1),nullptr,16);++count;if(k=="tmu_tocr")rt.tmu_tocr=v;else if(k=="tmu_tstr")rt.tmu_tstr=v;else if(k=="sh4_tick_pending_cycles")rt.sh4_tick_pending_cycles=v;else if(k=="sh4_tick_batch_cycles")rt.sh4_tick_batch_cycles=v;else{need(k.size()>5&&k[3]>='0'&&k[3]<='2'&&k[4]=='_',"Invalid observed timer key");auto& c=rt.tmu[k[3]-'0'];auto field=k.substr(5);if(field=="tcor")c.tcor=v;else if(field=="tcnt")c.tcnt=v;else if(field=="tcr")c.tcr=v;else if(field=="cycle_accumulator")c.cycle_accumulator=v;else if(field=="lazy_sh4_cycles")c.lazy_sh4_cycles=v;else if(field=="ticks")c.ticks=v;else if(field=="underflows")c.underflows=v;else throw std::runtime_error("Unknown timer field");}}
 need(count==25&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1&&(rt.tmu_tstr&~7u)==0,"Observed timer/batch fixture differs");}
static void save_timers(const auto&timers,unsigned start,unsigned tocr,const std::string& path){std::ofstream f(path);f<<std::hex<<"tmu_tocr="<<tocr<<"\ntmu_tstr="<<start<<"\nsh4_tick_pending_cycles=0\nsh4_tick_batch_cycles=1\n";for(unsigned i=0;i<3;++i){const auto& c=timers[i];auto out=[&](const char* key,auto v){f<<"tmu"<<std::dec<<i<<'_'<<key<<'='<<std::hex<<v<<'\n';};out("tcor",c.tcor);out("tcnt",c.tcnt);out("tcr",c.tcr);out("cycle_accumulator",c.cycle_accumulator);out("lazy_sh4_cycles",c.lazy_sh4_cycles);out("ticks",c.ticks);out("underflows",c.underflows);}need(bool(f),"Cannot save expected timers");}

namespace dcrecomp_generated { void callback_register(DCRuntime&); void zero_guard_register(DCRuntime&); }
struct Prediction{SH4Context c;unsigned cycles;bool observed_child;};
static Prediction model(const SH4Context& before,auto& m){
 auto c=before;c.r[5]=word(m,0x8c045684);c.r[15]-=4;put(m,c.r[15],c.pr);
 c.r[0]=static_cast<unsigned>(static_cast<int>(static_cast<std::int8_t>(m[(c.r[5]+3)&0xffffff])));
 c.sr=(c.sr&~1u)|(c.r[0]==0);
 if(!c.r[0]){c.pr=word(m,c.r[15]);c.r[15]+=4;c.pc=c.pr;return{c,9,false};}
 unsigned cycles=5;c.r[7]=word(m,0x8c045688);c.r[6]=c.r[7]+16;c.r[4]=c.r[7];
 c.sr=(c.sr&~1u)|(c.r[4]>=c.r[6]);cycles+=6;
 if(!(c.sr&1))do{c.r[3]=word(m,c.r[5]);put(m,c.r[4],c.r[3]);c.r[4]+=4;
 c.r[2]=word(m,c.r[5]);put(m,c.r[4],c.r[2]);c.r[4]+=4;c.sr=(c.sr&~1u)|(c.r[4]>=c.r[6]);cycles+=8;}while(!(c.sr&1));
 c.r[2]=word(m,0x8c04568c);need(c.r[2]==0x8c609368||c.r[2]==0xac609368,"Observed control target differs");
 c.r[5]=8;c.r[4]=0;c.pr=0x8c04565c;c.pc=0x8c609368;return{c,cycles+5,true};
}
static thread_local std::function<void()> observe_child;
static void stop_at_child(SH4Context&,DCRuntime&){need(bool(observe_child),"Missing test observer");observe_child();throw std::runtime_error("TEST_ONLY_FIRST_CHILD_OBSERVATION");}
int main(int argc,char**argv){try{
 need(argc==5,"usage: zeroguard RAM STATE FLASH OUTPUT");auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Guard fixtures differ");
 auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);callback_register(rt);zero_guard_register(rt);const auto sound=rt.aica_ram;
 for(unsigned a:{0x8c04562eu,0x8c609368u,0x8c642908u,0x8c05a2e6u,0x8c088644u,0x8c606c9cu,0x8c64b9e4u})need(rt.targets.contains(a),"Existing real catalog omitted");
 need(rt.targets.contains(0x8c659360)&&rt.targets.contains(0x8c60a6e4),"Real nested provider bindings omitted");
 auto child=rt.targets.at(0x8c609368);need(!rt.targets.contains(0xac609368),"P2 dispatch canonicalization differs");
 auto reset=[&](unsigned start){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=start&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto& c=rt.tmu[i];c.tcor=0x11223300+i;c.tcnt=0x55667700+i;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m&&rt.aica_ram==sound&&rt.system_flash==flash,"Guard wholeRAM/device mismatch");need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Guard unrelated device changed");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Guard timer control changed");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 unsigned zero=0,nonzero=0;
 auto one=[&](SH4Context c,unsigned start,bool expect_child){auto m=rt.main_ram;auto w=model(c,m);need(w.observed_child==expect_child,"Synthetic branch expectation differs");auto timers=rt.tmu;tick(timers,start,w.cycles);auto tocr=rt.tmu_tocr;
  if(expect_child){unsigned captures=0;observe_child=[&]{++captures;same(w.c,c);exact(m,timers,start,tocr);};rt.register_target(0x8c609368,stop_at_child);
   struct Restore{DCRuntime&rt;RecompiledFunction a;~Restore(){rt.register_target(0x8c609368,a);observe_child={};}}restore{rt,child};
   bool stopped=false;try{call_recompiled(c,rt,0x8c04562e);}catch(const std::runtime_error&e){if(std::string(e.what())=="TEST_ONLY_FIRST_CHILD_OBSERVATION")stopped=true;else throw;}need(stopped&&captures==1,"Bounded child observation absent");++nonzero;
  }else{call_recompiled(c,rt,0x8c04562e);++zero;}same(w.c,c);exact(m,timers,start,tocr);
 };
 // Original flag byte read after savedPR store; independent physical aliases and alignment.
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned gate_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment)for(unsigned flag:{0u,1u,0x80u,0xffu}){
  unsigned start=mask&7;reset(start);auto c=context(0x8c04562e,mask,t,bank);c.pr=0x8c0561e8;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);
  unsigned gate=(gate_alias?0xac000000u:0x8c000000u)|(0xefd300+alignment);put(rt.main_ram,0x8c045684,gate);put(rt.main_ram,gate,0x12345678|(flag<<24));rt.main_ram[(gate+3)&0xffffff]=flag;put(rt.main_ram,0x8c045688,0x8cefd400+alignment);put(rt.main_ram,0x8c04568c,gate_alias?0xac609368:0x8c609368);one(c,start,flag!=0);
 }
 need(zero==1024&&nonzero==3072,"Independent alias matrix differs");
 // Literal read precedes stack write; gate byte follows it. Partial PR overlaps can flip branch.
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned layout=0;layout<8;++layout){
  unsigned start=mask&7;reset(start);auto c=context(0x8c04562e,mask,t,bank);c.pr=0x8c001200;c.r[15]=0x8cefd104;unsigned gate=0x8cefd300;
  if(layout<4)gate=c.r[15]-7+layout;
  else if(layout==4)c.r[15]=0x8c045688; // push overwrites literal, already read
  else if(layout==5)gate=0x8c045681; // gate byte overlaps literal's low byte
  else if(layout==6)gate=0x8cefd401; // first destination word changes next source read
  else gate=0x8cefd409; // second pair changes next read
  if(alias){c.r[15]+=0x20000000;gate+=0x20000000;}
  put(rt.main_ram,0x8c045684,gate);put(rt.main_ram,0x8c045688,0x8cefd400);put(rt.main_ram,0x8c04568c,0x8c609368);
  if(layout!=5)rt.main_ram[(gate+3)&0xffffff]=layout>=6?0x80:(layout==4?0:0xff);
  auto m=rt.main_ram;auto w=model(c,m);one(c,start,w.observed_child);
 }
 need(zero==1408&&nonzero==3712,"Ordered overlap matrix differs");
 // Real captured path uses original zero flag and real catalog, with no observation hook.
 reset(0);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c0561e2&&c.pr==0x8c0561e8&&c.r[15]==0x8cffffa4&&c.r[0]==1&&c.sr==0x40000101,"Observed guard boundary differs");
 need(word(main,0x8c045684)==0x8c74d1bc&&main[0x74d1bf]==0,"Observed zero flag differs");auto m=main;auto w=model(c,m);need(!w.observed_child&&w.cycles==9&&w.c.pc==0x8c0561e8&&w.c.pc==w.c.pr&&w.c.r[15]==0x8cffffa4&&w.c.r[0]==0&&(w.c.sr&1)==1&&w.c.r[5]==0x8c74d1bc,"Actual zero-return model differs");
 auto timers=rt.tmu;tick(timers,rt.tmu_tstr,9);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
 save_state(w.c,std::string(argv[4])+"/expected-startup-zero-guard-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-zero-guard-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-zero-guard-ram.bin");
 need(rt.targets.at(0x8c609368)==child&&!rt.targets.contains(0xac609368),"Test observer leaked into actual execution");call_recompiled(c,rt,0x8c04562e);same(w.c,c);exact(m,timers,start,tocr);
 std::ofstream result(std::string(argv[4])+"/guard-result.json");result<<"{\"zero_return_cases\":"<<zero<<",\"test_only_first_child_observations\":"<<nonzero<<",\"actual_zero_return_cycles\":9,\"nonzero_full_return_verified\":false,\"passed\":true}\n";
 std::cout<<"PASS original04562E: "<<zero<<" zero-return cases/"<<nonzero<<" test-only first-child observations; actual9cycle return PC-PR0561E8/SPFFFFA4/R0zero/T1/R5=74D1BC, one savedPR store4bytes/frame4; whole16MiBRAM69CPU25TMU/P1-P2aliases/fouralignments/all16IRQ-T-FPU/signedflags/ordered literal-push-flag overlap/live word copies; real609368/642908 restored and retained; no synthetic missingcallee, nonzero full return, fullstartup or G1-boot-gameplay claim\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
