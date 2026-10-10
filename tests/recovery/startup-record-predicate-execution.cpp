// Original10B906: signed record byte versus live counter low three bits.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
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


namespace dcrecomp_generated {void predicate_register(DCRuntime&);}
static SH4Context predicate_model(SH4Context c,const auto& m){
 c.r[3]=1538;c.r[2]=word(m,0x8c10ba04);c.r[0]=word(m,c.r[2]);c.r[3]+=c.r[4];
 c.r[3]=unsigned(int(std::int8_t(m[c.r[3]&0xffffff])));c.r[0]&=7;
 c.sr=(c.sr&~1u)|unsigned(c.r[3]==c.r[0]);c.r[0]=c.sr&1;c.pc=c.pr;return c;
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: oracle RAM STATE FLASH OUTPUT");auto heap=std::make_unique<DCRuntime>();auto&rt=*heap;load_embedded_elf_image(rt);
 const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024&&flash.size()==131072,"Fixture size");rt.main_ram=main;rt.system_flash=flash;register_recompiled_program(rt);predicate_register(rt);
 need(rt.targets.contains(0x8c10b906),"Original predicate missing");const auto sound=rt.aica_ram;
 auto check=[&](SH4Context c){auto want=predicate_model(c,rt.main_ram);const auto m=rt.main_ram;auto timers=rt.tmu;tick(timers,rt.tmu_tstr,13);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
  call_recompiled(c,rt,0x8c10b906);same(want,c);need(m==rt.main_ram&&flash==rt.system_flash&&sound==rt.aica_ram,"Predicate wrote memory/device");
  need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Timer controls changed");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
  need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unrelated device effects");return want;
 };
 unsigned controls=0;for(unsigned counter=0;counter<8;++counter)for(unsigned type=0;type<4;++type)for(unsigned alias=0;alias<2;++alias){
  rt.main_ram=main;auto c=context(0x8c10b906,counter*2,type&1,alias);const unsigned tag=alias?0xac000000:0x8c000000;const auto a=tag|0x7aa76c;c.r[4]=a;
  put(rt.main_ram,0x8c10ba04,tag|0x7aa764);put(rt.main_ram,tag|0x7aa764,0xfffffff8|counter);
  rt.main_ram[(a+1538)&0xffffff]=type==0?counter:type==1?(counter+1)&7:type==2?0xff:0x80;
  load_timers(rt,argv[2]);auto want=check(c);need(want.r[0]==(type==0),"Predicate signed-byte expectation");++controls;
 }
 unsigned actual=0;for(unsigned i=0;i<4;++i){rt.main_ram=main;auto c=state(argv[2]);need(c.pc==0x8c0556ea&&c.pr==0x8c0556ee&&c.r[15]==0x8cffffc8,"Actual parent boundary");c.r[4]=0x8c7aa76c+i*1596;c.pr=0x8c10b9c6;load_timers(rt,argv[2]);
  auto want=predicate_model(c,rt.main_ram);need(want.r[0]==(i==0),"Actual four predicates differ");
  const auto stem=std::string(argv[4])+"/record-"+std::to_string(i);save_state(want,stem+"-expected-state.txt");auto timers=rt.tmu;tick(timers,rt.tmu_tstr,13);save_timers(timers,rt.tmu_tstr,rt.tmu_tocr,stem+"-expected-timers.txt");check(c);++actual;
 }
 need(controls==64&&actual==4,"Matrix size");std::ofstream f(std::string(argv[4])+"/predicate-result.json");f<<"{\"passed\":true,\"controls\":64,\"captured_record_checks\":4,\"modeled_cycles\":13,\"parent_return_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original10B906: 64 signed-byte/counter/alias controls and four captured record predicates, 13 cycles/full RAM/69CPU/25TMU; 10B992 parent loop not yet executed or returned\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
