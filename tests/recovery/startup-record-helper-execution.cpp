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


namespace dcrecomp_generated {void callback_register(DCRuntime&);void record_helpers_register(DCRuntime&);void helper_register(DCRuntime&);void serializer_register(DCRuntime&);void transport_register(DCRuntime&);}
static SH4Context predicate_model(SH4Context c,const auto& m){
 c.r[3]=1538;c.r[2]=word(m,0x8c10ba04);c.r[0]=word(m,c.r[2]);c.r[3]+=c.r[4];
 c.r[3]=unsigned(int(std::int8_t(m[c.r[3]&0xffffff])));c.r[0]&=7;
 c.sr=(c.sr&~1u)|unsigned(c.r[3]==c.r[0]);c.r[0]=c.sr&1;c.pc=c.pr;return c;
}
struct Packed {SH4Context c;unsigned cycles;};
static Packed pack_model(SH4Context c,auto&m){
 auto push=[&](unsigned v){c.r[15]-=4;put(m,c.r[15],v);};auto pop=[&](){auto v=word(m,c.r[15]);c.r[15]+=4;return v;};
 push(c.r[14]);c.r[14]=1536;push(c.r[13]);c.r[3]=0xffffffbf;push(c.r[12]);c.r[13]=1;c.r[14]+=c.r[4];c.r[7]=1;
 need(c.r[4]<c.r[14],"Only non-wrapping 1536-byte source modeled");c.r[3]=191;c.r[6]=1;c.r[13]=256;c.r[5]+=191;c.r[12]=0;
 unsigned nonzero=0;
 // Bits ascend within each output byte; output bytes descend. Reads/stores are
 // deliberately ordered for aliases to source or the saved register frame.
 for(unsigned i=0;i<1536;++i){c.r[0]=c.r[6];if(c.r[6]==1)m[c.r[5]&0xffffff]=0;
  c.r[2]=unsigned(int(std::int8_t(m[c.r[4]&0xffffff])));
  if(c.r[2]){c.r[1]=unsigned(int(std::int8_t(m[c.r[5]&0xffffff])));c.r[1]|=c.r[6];m[c.r[5]&0xffffff]=std::uint8_t(c.r[1]);++nonzero;}
  c.r[6]*=2;++c.r[4];if(c.r[6]==256){c.r[6]=1;--c.r[5];}
 }
 c.sr|=1;c.r[12]=pop();c.r[13]=pop();c.pc=c.pr;c.r[14]=pop();return {c,19029+3*nonzero};
}
static RecompiledFunction native_serializer,native_transport;
static SH4Context expected_pack_entry,expected_pack_return,expected_transport_entry;
static std::vector<std::uint8_t> pack_entry_ram,pack_return_ram;
static std::array<SH4TMUChannel,3> actual_initial_timers;static unsigned timer_start,timer_tocr,serializer_cycles,serial_observations,transport_observations;
static void timer_at(DCRuntime&rt,unsigned cycles){auto t=actual_initial_timers;tick(t,timer_start,cycles);for(unsigned i=0;i<3;++i)timer_same(t[i],rt.tmu[i]);need(rt.tmu_tstr==timer_start&&rt.tmu_tocr==timer_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Helper timer controls differ");}
static void observe_serializer(SH4Context&c,DCRuntime&rt){need(serial_observations++==0,"Unexpected extra serialization before missing boundary");same(expected_pack_entry,c);need(rt.main_ram==pack_entry_ram,"Actual serializer entry RAM differs");timer_at(rt,42);native_serializer(c,rt);same(expected_pack_return,c);need(rt.main_ram==pack_return_ram,"Actual serializer return RAM differs");timer_at(rt,42+serializer_cycles);}
static void observe_transport(SH4Context&c,DCRuntime&rt){need(transport_observations++==0,"Unexpected extra transport before missing boundary");same(expected_transport_entry,c);need(rt.main_ram==pack_return_ram,"Actual transport entry RAM differs");timer_at(rt,52+serializer_cycles);native_transport(c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: helper RAM STATE FLASH OUTPUT");auto heap=std::make_unique<DCRuntime>();auto&rt=*heap;load_embedded_elf_image(rt);
 const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.main_ram=main;rt.system_flash=flash;register_recompiled_program(rt);callback_register(rt);record_helpers_register(rt);helper_register(rt);serializer_register(rt);transport_register(rt);const auto sound=rt.aica_ram;
 for(unsigned a:{0x8c10a326u,0x8c10a2dau,0x8c6067aau,0x8c10b906u,0x8c6067f4u})need(rt.targets.contains(a),"Real required binding absent");need(!rt.targets.contains(0x8c64a734),"Observed next transport dependency already registered");
 native_serializer=rt.targets.at(0x8c10a2da);native_transport=rt.targets.at(0x8c6067aa);
 auto pack_check=[&](SH4Context c){auto m=rt.main_ram;auto want=pack_model(c,m);auto timers=rt.tmu;tick(timers,rt.tmu_tstr,want.cycles);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;call_recompiled(c,rt,0x8c10a2da);same(want.c,c);need(rt.main_ram==m&&rt.system_flash==flash&&rt.aica_ram==sound,"Packing memory/device differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Packing timer controls differ");};
 unsigned patterns=0,overlaps=0;
 for(unsigned pattern=0;pattern<6;++pattern)for(unsigned alias=0;alias<2;++alias)for(unsigned t=0;t<2;++t){rt.main_ram=main;const unsigned tag=alias?0xac000000:0x8c000000;auto c=context(0x8c10a2da,pattern*2,t,alias);c.r[4]=tag|0xedd000;c.r[5]=tag|0xedc000;c.r[15]=tag|0xefd000;c.pr=0x8c10a35c;
  for(unsigned i=0;i<1536;++i){const std::uint8_t values[]={0,0xff,std::uint8_t((i%8)==(i/8)%8?0x80:0),std::uint8_t((i&1)?0x80:0),std::uint8_t(i==1535),std::uint8_t((i*17)^0xa5)};rt.main_ram[(c.r[4]+i)&0xffffff]=values[pattern];}
  // Independent disjoint buffer calculation checks reversed byte order and
  // boolean normalization before comparing the source-derived state model.
  auto predicted=rt.main_ram;auto result=pack_model(c,predicted);
  for(unsigned group=0;group<192;++group){unsigned bits=0;for(unsigned bit=0;bit<8;++bit)bits|=unsigned(rt.main_ram[(c.r[4]+group*8+bit)&0xffffff]!=0)<<bit;need(predicted[(c.r[5]+191-group)&0xffffff]==bits,"Independent packing definition differs");}
  load_timers(rt,argv[2]);pack_check(c);++patterns;
 }
 for(unsigned alias=0;alias<2;++alias)for(unsigned layout=0;layout<8;++layout){rt.main_ram=main;const unsigned tag=alias?0xac000000:0x8c000000;auto c=context(0x8c10a2da,layout*2,layout&1,alias);c.r[4]=tag|0xedd000;c.r[15]=tag|0xefd000;c.pr=0x8c10a35c;
  for(unsigned i=0;i<1536;++i)rt.main_ram[(c.r[4]+i)&0xffffff]=i%3?0x80:0;
  const unsigned destinations[]={c.r[4],c.r[4]+1535,c.r[4]+8,c.r[4]-191,c.r[4]+768,c.r[15]-192,c.r[15]-196,c.r[15]-200};c.r[5]=destinations[layout];load_timers(rt,argv[2]);pack_check(c);++overlaps;
 }
 need(patterns==24&&overlaps==16,"Packing matrix differs");rt.main_ram=main;auto c=state(argv[2]);need(c.pc==0x8c10b89e&&c.pr==0x8c10b8a4&&c.r[15]==0x8cffff88&&c.r[4]==0x8c7aa76c,"Actual helper boundary differs");
 pack_entry_ram=main;auto e=c;auto push=[&](unsigned v){e.r[15]-=4;put(pack_entry_ram,e.r[15],v);};push(e.r[14]);e.r[0]=0xffffffff;push(e.r[13]);e.r[0]<<=8;push(e.pr);e.r[0]+=64;e.r[15]+=e.r[0];e.r[3]=word(pack_entry_ram,0x8c10a3a0);need(e.r[3]==0x8c10b906,"Real predicate literal differs");e.pr=0x8c10a33a;e.r[14]=e.r[4];e=predicate_model(e,pack_entry_ram);need(e.r[0]==1,"Actual record not selected");
 e.r[0]=1536;e.r[0]=unsigned(int(std::int8_t(pack_entry_ram[(e.r[14]+e.r[0])&0xffffff])));need(e.r[0]==1,"Active record differs");e.r[0]=1537;e.r[0]=unsigned(int(std::int8_t(pack_entry_ram[(e.r[14]+e.r[0])&0xffffff])));need(e.r[0]==1,"Actual route1 differs");e.sr|=1;e.r[13]=e.r[15];e.r[5]=e.r[13];e.pr=0x8c10a35c;e.r[4]=e.r[14];e.pc=0x8c10a2da;expected_pack_entry=e;
 pack_return_ram=pack_entry_ram;auto packed=pack_model(e,pack_return_ram);serializer_cycles=packed.cycles;need(serializer_cycles==19029,"Captured payload unexpectedly nonzero");expected_pack_return=packed.c;e=packed.c;e.r[0]=1584;e.r[3]=word(pack_return_ram,0x8c10a3a4);need(e.r[3]==0x8c6067aa,"Transport literal differs");e.r[5]=e.r[13];e.pr=0x8c10a374;e.r[4]=word(pack_return_ram,e.r[14]+e.r[0]);need(e.r[4]==1,"Captured transport unit differs");e.pc=0x8c6067aa;expected_transport_entry=e;
 load_timers(rt,argv[2]);actual_initial_timers=rt.tmu;timer_start=rt.tmu_tstr;timer_tocr=rt.tmu_tocr;const auto stem=std::string(argv[4]);
 for(unsigned i=0;i<3;++i){const auto& want=i==0?expected_pack_entry:i==1?expected_pack_return:expected_transport_entry;const unsigned cycles=i==0?42:i==1?42+serializer_cycles:52+serializer_cycles;auto t=actual_initial_timers;tick(t,timer_start,cycles);const auto label=stem+"/phase-"+std::to_string(i);save_state(want,label+"-expected-state.txt");save_timers(t,timer_start,timer_tocr,label+"-expected-timers.txt");}
 save_ram(pack_entry_ram,stem+"/expected-pack-entry-ram.bin");save_ram(pack_return_ram,stem+"/expected-pack-return-ram.bin");rt.register_target(0x8c10a2da,observe_serializer);rt.register_target(0x8c6067aa,observe_transport);
 std::string error;try{call_recompiled(c,rt,0x8c10a326);}catch(const std::exception&ex){error=ex.what();}
 save_state(c,stem+"/observed-stop-state.txt");save_ram(rt.main_ram,stem+"/observed-stop-ram.bin");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/observed-stop-timers.txt");std::ofstream(stem+"/observed-stop-error.txt")<<error<<'\n';
 need(serial_observations==1&&transport_observations==1&&!error.empty(),"Actual helper path did not reach missing transport dependency");need(c.pr==0x8c6067da&&c.r[2]==0x8c64a734&&c.r[15]==0x8cfffea4,"Missing transport boundary differs");need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unrelated device changed");
 std::ofstream(stem+"/helper-result.json")<<"{\"passed\":true,\"patterns\":24,\"overlaps\":16,\"actual_serialization_cycles\":19029,\"prospective_phases\":[42,19071,19081],\"observed_missing_target\":\"0x8C64A734\",\"later_state_predicted\":false,\"helper_return_verified\":false,\"parent_return_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original10A326 route1: 24 packing patterns/16 ordered source or stack overlaps; real10A2DA19029cycles; actual42/19071/19081cycle wholeRAM/69CPU/25TMU phases. Real6067AA/query reaches missing64A734; later state observed only, transport/helper/parent return and G1/boot unproved\n";
}catch(const std::exception&ex){std::cerr<<"FAIL "<<ex.what()<<'\n';return 1;}}
