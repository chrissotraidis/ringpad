// Original08C25E: live word addition and RTS-delay store; no child/stack operations.
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


namespace dcrecomp_generated {void callback_register(DCRuntime&);void counter_register(DCRuntime&);}
struct Prediction{SH4Context c;unsigned cycles;};
static Prediction model(const SH4Context&before,auto&m){
 auto c=before;need(!(c.fpscr&(1u<<20)),"Only actual SZ0 frame modeled");
 auto T=[&](bool b){c.sr=(c.sr&~1u)|b;};auto push=[&](unsigned v){c.r[15]-=4;put(m,c.r[15],v);};auto pop=[&](){auto v=word(m,c.r[15]);c.r[15]+=4;return v;};
 for(int i=14;i>=8;--i)push(c.r[i]);push((c.fpscr&(1u<<21))?c.xf_bits[15]:c.fr_bits[15]);push(c.pr);c.r[15]-=44;
 c.r[3]=word(m,0x8c010c10);c.r[12]=76;c.r[9]=word(m,0x8c010c1c);c.r[4]=word(m,c.r[3]);c.r[5]=word(m,c.r[9]);
 c.macl=c.r[12]*c.r[4];c.r[2]=word(m,0x8c010c00);T(c.r[5]==0);c.r[13]=word(m,0x8c010c30);c.r[11]=word(m,0x8c010c2c);c.r[12]=c.macl;c.r[10]=word(m,0x8c010c28);c.r[12]+=c.r[2];c.r[14]=0;unsigned cycles=26;
 bool query=c.r[5]==0;
 if(!query){c.r[1]=word(m,0x8c010c20);c.r[0]=word(m,c.r[1]);T(c.r[0]==0);cycles+=4;query=c.r[0]==0;}
 if(query){
  cycles+=2;c.r[3]=word(m,0x8c010de4);need(c.r[3]==0x8c607124||c.r[3]==0xac607124,"Only real607124 continuation modeled");c.pr=0x8c010ca0;cycles+=4;
  // Original607124 pushes before reading its live child literal.
  push(c.pr);c.r[3]=word(m,0x8c607228);need(c.r[3]==0x8c64b9e4||c.r[3]==0xac64b9e4,"Only real64B9E4 modeled");c.pr=0x8c60712c;
  c.r[4]<<=2;c.r[3]=word(m,0x8c64ba50);c.r[4]<<=2;c.r[4]<<=2;T(c.r[4]>>31);c.r[4]<<=1;c.r[4]+=c.r[3];c.r[0]=word(m,c.r[4]+4);T(c.r[0]==0);
  bool zero=c.r[0]==0;c.pr=pop();c.pc=c.pr;c.r[0]=zero?0:0xffffffff;cycles+=zero?21:20;
  c.r[3]=word(m,0x8c010dec);c.r[2]=word(m,0x8c010de8);put(m,c.r[2],c.r[0]);c.r[1]=word(m,c.r[3]);--c.r[1];T(!(c.r[1]>>31));put(m,c.r[3],c.r[1]);cycles+=8;
  if(!(c.sr&1)){c.r[2]=word(m,0x8c010dec);put(m,c.r[2],c.r[14]);cycles+=2;}
  c.r[4]=word(m,c.r[12]+16);c.r[5]=word(m,c.r[12]+8);c.r[0]=c.r[4];T(c.r[0]==12);c.r[6]=word(m,c.r[12]+12);cycles+=6;need(!(c.sr&1),"Drawing record12 is unmodeled");cycles+=2;
 }else{
  c.r[0]=c.r[5];c.r[8]=word(m,0x8c010c34);T(c.r[0]==4);cycles+=4;need(c.r[0]!=4,"Drawing state4 is unmodeled");T(c.r[0]==3);cycles+=2;
  if(c.r[0]==3){c.r[1]=word(m,c.r[9]);--c.r[1];put(m,c.r[9],c.r[1]);cycles+=4;}
  else{T(c.r[0]==2);cycles+=2;need(c.r[0]!=2,"State2 input/drawing path unmodeled");T(c.r[0]==1);cycles+=2;if(c.r[0]==1)put(m,c.r[9],c.r[14]);cycles+=2;}
 }
 c.r[15]+=44;c.pr=pop();auto f=pop();if(c.fpscr&(1u<<21))c.xf_bits[15]=f;else c.fr_bits[15]=f;for(unsigned i=8;i<=13;++i)c.r[i]=pop();c.r[14]=pop();c.pc=c.pr;cycles+=12;return{c,cycles};
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: counter RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Counter fixtures differ");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);callback_register(rt);counter_register(rt);const auto sound=rt.aica_ram;
 for(unsigned a:{0x8c010b7au,0x8c607124u,0x8c64b9e4u,0x8c08c25eu,0x8c04562eu,0x8c08c26cu,0x8c05a2e6u,0x8c088644u})need(rt.targets.contains(a),"Real catalog binding omitted");need(!rt.targets.contains(0x8c042144)&&!rt.targets.contains(0x8c0113da),"Latent missing catalog differs");
 auto reset=[&](unsigned start){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=start&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto&c=rt.tmu[i];c.tcor=0x11223300+i;c.tcnt=0x55667700+i;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m&&rt.aica_ram==sound&&rt.system_flash==flash,"Counter wholeRAM/device differs");need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Counter unrelated device changed");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Counter timer controls differ");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 auto one=[&](SH4Context c,unsigned start,unsigned entry){auto m=rt.main_ram;auto w=model(c,m);auto timers=rt.tmu;tick(timers,start,w.cycles);auto tocr=rt.tmu_tocr;call_recompiled(c,rt,entry);same(w.c,c);exact(m,timers,start,tocr);};
 auto setup=[&](unsigned alias,unsigned alignment,unsigned index,unsigned count,unsigned guard,unsigned mode,unsigned flag){
  unsigned tag=alias?0xac000000u:0x8c000000u;auto addr=[&](unsigned x){return tag|x;};
  unsigned data=addr(0xefd300+alignment),rec=addr(0x6b6134),table=addr(0x8983b8),record=rec+index*76;
  put(rt.main_ram,0x8c010c10,data);put(rt.main_ram,data,index);put(rt.main_ram,0x8c010c1c,data+8);put(rt.main_ram,data+8,mode);put(rt.main_ram,0x8c010c20,data+16);put(rt.main_ram,data+16,flag);put(rt.main_ram,0x8c010c00,rec);put(rt.main_ram,0x8c010de4,alias?0xac607124:0x8c607124);put(rt.main_ram,0x8c607228,alias?0xac64b9e4:0x8c64b9e4);put(rt.main_ram,0x8c64ba50,table);put(rt.main_ram,table+(index<<7)+4,guard);put(rt.main_ram,0x8c010dec,data+24);put(rt.main_ram,data+24,count);put(rt.main_ram,0x8c010de8,data+32);put(rt.main_ram,record+16,1);put(rt.main_ram,record+8,0x11223344);put(rt.main_ram,record+12,0x55667788);
 };
 unsigned arithmetic=0,modes=0,overlaps=0;const unsigned counts[]={0,1,2,0xffffffff,0x80000000,0x7fffffff};const unsigned indices[]={0,1,0xffffffff,0x80000000};
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
  auto start=mask&7;unsigned entry=alias?0xac010b7a:0x8c010b7a;
  for(unsigned alignment=0;alignment<4;++alignment)for(auto count:counts)for(unsigned guard:{0u,1u}){reset(start);auto c=context(entry,mask,t,bank);c.pr=alias?0xac0556e4:0x8c0556e4;c.r[15]=(alias?0xacefd100:0x8cefd100)+alignment;setup(alias,alignment,indices[alignment],count,guard,0,1);one(c,start,entry);++arithmetic;}
  for(auto mode:{1u,3u,5u,0xffffffffu,2u,4u}){reset(start);auto c=context(entry,mask,t,bank);c.pr=0x8c0556e4;setup(alias,0,0,1,1,mode,mode==2||mode==4?0:1);one(c,start,entry);++modes;}
  for(unsigned layout=0;layout<8;++layout){reset(start);auto c=context(entry,mask,t,bank);c.pr=0x8c0556e4;setup(alias,0,0,layout==5?2:0,1,0,1);
   if(layout==0)put(rt.main_ram,0x8c010de8,c.r[15]-4);
   else if(layout==1)put(rt.main_ram,0x8c010dec,c.r[15]-8);
   else if(layout==2)put(rt.main_ram,0x8c010dec,c.r[15]-36);
   else if(layout==3)put(rt.main_ram,0x8c010de8,c.r[15]-84);
   else if(layout==4){put(rt.main_ram,0x8c64ba50,c.r[15]-88);put(rt.main_ram,c.r[15]-84,0);}
   else if(layout==5)put(rt.main_ram,0x8c010de8,0x8c010dec);
   else if(layout==6){c.r[15]=0x8c010c14;c.r[14]=word(rt.main_ram,0x8c010c10);c.r[10]=word(rt.main_ram,0x8c010c00);}
   else{put(rt.main_ram,0x8c010de8,0x8cefd600);put(rt.main_ram,0x8c010dec,0x8cefd601);put(rt.main_ram,0x8cefd601,0);}
   one(c,start,entry);++overlaps;
  }
 }
 need(arithmetic==6144&&modes==768&&overlaps==1024,"Counter test matrix differs");
 reset(0);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c0556e0&&c.pr==0x8c0556e4&&c.r[15]==0x8cffffc8&&c.sr==0x40000100,"Actual counter boundary differs");auto m=main;auto w=model(c,m);need(w.cycles==82&&w.c.pc==0x8c0556e4&&w.c.r[15]==0x8cffffc8&&word(m,0x8c164ae4)==0&&word(m,0x8c6ba594)==0xffffffff,"Actual82cycle model differs");auto timers=rt.tmu;tick(timers,rt.tmu_tstr,w.cycles);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
 save_state(w.c,std::string(argv[4])+"/expected-startup-counter-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-counter-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-counter-ram.bin");call_recompiled(c,rt,0x8c010b7a);same(w.c,c);exact(m,timers,start,tocr);
 std::ofstream result(std::string(argv[4])+"/counter-result.json");result<<"{\"arithmetic_cases\":"<<arithmetic<<",\"mode_cases\":"<<modes<<",\"overlap_cases\":"<<overlaps<<",\"actual_return_cycles\":82,\"passed\":true}\n";
 std::cout<<"PASS original010B7A: "<<arithmetic<<" arithmetic/guard/alias/alignment cases, "<<modes<<" non-drawing modes, "<<overlaps<<" ordered frame/global/literal overlaps; actual82cycle real607124-64B9E4 return, 80byte parent/84byte nested maximum, MACL/FPU/fullRAM69CPU25TMU; latent drawing/state2/SZ1 paths and fullstartup/G1/boot/gameplay remain unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
