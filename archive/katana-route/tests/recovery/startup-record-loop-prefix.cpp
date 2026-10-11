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


namespace dcrecomp_generated {void callback_register(DCRuntime&);void parent_register(DCRuntime&);void record_helpers_register(DCRuntime&);}
static SH4Context predicate_model(SH4Context c,const auto& m){
 c.r[3]=1538;c.r[2]=word(m,0x8c10ba04);c.r[0]=word(m,c.r[2]);c.r[3]+=c.r[4];
 c.r[3]=unsigned(int(std::int8_t(m[c.r[3]&0xffffff])));c.r[0]&=7;
 c.sr=(c.sr&~1u)|unsigned(c.r[3]==c.r[0]);c.r[0]=c.sr&1;c.pc=c.pr;return c;
}
static RecompiledFunction original_predicate;static SH4Context expected_entry;static std::vector<std::uint8_t> expected_ram;static std::array<SH4TMUChannel,3> initial_timers;static unsigned start,tocr,observations;
static void timers_at(DCRuntime&rt,unsigned cycles){auto want=initial_timers;tick(want,start,cycles);for(unsigned i=0;i<3;++i)timer_same(want[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Parent timer controls differ");}
static void observe_predicate(SH4Context&c,DCRuntime&rt){
 if(observations++){original_predicate(c,rt);return;}
 same(expected_entry,c);need(rt.main_ram==expected_ram,"First predicate entry RAM differs");timers_at(rt,21);
 auto want=predicate_model(c,expected_ram);original_predicate(c,rt);same(want,c);need(rt.main_ram==expected_ram,"First predicate return RAM differs");timers_at(rt,34);
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: loop RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.main_ram=main;rt.system_flash=flash;register_recompiled_program(rt);callback_register(rt);record_helpers_register(rt);parent_register(rt);
 for(unsigned a:{0x8c10b992u,0x8c10b906u,0x8c10b226u,0x8c10b1ecu,0x8c10b2e0u,0x8c10a3f2u,0x8c6067f4u,0x8c6066d8u,0x8c109ae0u})need(rt.targets.contains(a),"Real binding missing");need(!rt.targets.contains(0x8c10a326),"Expected next missing helper is already registered");
 auto c=state(argv[2]);need(c.pc==0x8c0556ea&&c.pr==0x8c0556ee&&c.r[15]==0x8cffffc8&&c.r[4]==22,"Captured loop boundary differs");
 // Prospective model: actual gate2, ordered frame pushes/live literals, mode store,
 // original BSR delay argument; stop model at first real predicate entry.
 expected_entry=c;expected_ram=main;auto&e=expected_entry;auto push=[&](unsigned v){e.r[15]-=4;put(expected_ram,e.r[15],v);};
 push(e.r[14]);e.r[3]=word(expected_ram,0x8c10ba18);push(e.r[13]);push(e.r[12]);push(e.pr);e.r[0]=word(expected_ram,e.r[3]);need(e.r[0]==2,"Actual gate differs");e.sr&=~1u;
 e.r[13]=word(expected_ram,0x8c10ba08);e.r[12]=25<<8;e.r[14]=e.r[13];e.r[12]-=16;e.r[14]+=16;e.r[12]+=e.r[14];e.sr=(e.sr&~1u)|unsigned(e.r[14]>=e.r[12]);need(!(e.sr&1),"Loop bound unexpectedly skips");put(expected_ram,e.r[13],e.r[4]);e.pr=0x8c10b9c6;e.r[4]=e.r[14];e.pc=0x8c10b906;
 load_timers(rt,argv[2]);initial_timers=rt.tmu;start=rt.tmu_tstr;tocr=rt.tmu_tocr;
 const auto stem=std::string(argv[4]);save_state(e,stem+"/expected-first-predicate-state.txt");save_ram(expected_ram,stem+"/expected-first-predicate-ram.bin");auto t=initial_timers;tick(t,start,21);save_timers(t,start,tocr,stem+"/expected-first-predicate-timers.txt");auto after=predicate_model(e,expected_ram);save_state(after,stem+"/expected-first-predicate-return-state.txt");t=initial_timers;tick(t,start,34);save_timers(t,start,tocr,stem+"/expected-first-predicate-return-timers.txt");
 original_predicate=rt.targets.at(0x8c10b906);rt.register_target(0x8c10b906,observe_predicate);std::string error;
 try{call_recompiled(c,rt,0x8c10b992);}catch(const std::exception&ex){error=ex.what();}
 save_state(c,stem+"/observed-stop-state.txt");save_ram(rt.main_ram,stem+"/observed-stop-ram.bin");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/observed-stop-timers.txt");std::ofstream(stem+"/observed-stop-error.txt")<<error<<'\n';
 need(observations>=1&&!error.empty(),"Expected authentic missing helper boundary not reached");need(c.pr==0x8c10b8a4&&c.r[3]==0x8c10a326&&c.r[15]==0x8cffff88,"Observed missing boundary differs");need(rt.system_flash==flash,"Flash changed");
 std::ofstream(stem+"/loop-result.json")<<"{\"passed\":true,\"prospective_prefix_phases\":2,\"prefix_cycles\":[21,34],\"observed_missing_target\":\"0x8C10A326\",\"later_state_predicted\":false,\"parent_return_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original10B992 prefix: actual gate2/mode22/16byte frame and real10B906 return verify RAM/69CPU/25TMU at21/34cycles. Real calls reach missing10A326; later state only observed, parent four-record return and boot unproved\n";
}catch(const std::exception&ex){std::cerr<<"FAIL "<<ex.what()<<'\n';return 1;}}
