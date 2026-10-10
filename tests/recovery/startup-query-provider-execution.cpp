// Original606C9C/64B9E4: ordered stack writes, live literals/table read, real return or explicit missing tail.
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























struct Prediction {SH4Context c;unsigned cycles;bool rejected;unsigned target;};
static Prediction provider(const SH4Context& b,auto& m){
 auto c=b;auto push=[&](unsigned v){c.r[15]-=4;put(m,c.r[15],v);};
 push(c.pr);c.r[15]-=8;c.r[3]=word(m,0x8c606f50);put(m,c.r[15],c.r[4]);put(m,c.r[15]+4,c.r[5]);
 const auto target=c.r[3];c.pr=0x8c606caa;c.r[4]=word(m,c.r[15]);c.pc=0x8c606c9c;
 if(target!=0x8c64b9e4&&target!=0xac64b9e4)return {c,8,true,target};
 c.r[4]<<=2;c.r[3]=word(m,0x8c64ba50);c.r[4]<<=2;c.r[4]<<=2;
 c.sr=(c.sr&~1u)|(c.r[4]>>31);c.r[4]<<=1;c.r[4]+=c.r[3];c.r[0]=word(m,c.r[4]+4);
 c.pc=c.pr;c.sr=(c.sr&~1u)|(c.r[0]==0);
 if(c.r[0]!=0){c.r[15]+=8;c.pr=word(m,c.r[15]);c.r[15]+=4;c.pc=c.pr;c.r[0]=0xffffffff;return {c,24,false,0};}
 c.r[4]=word(m,c.r[15]);c.r[5]=word(m,c.r[15]+4);c.r[15]+=8;c.r[3]=word(m,0x8c606f54);
 c.pr=word(m,c.r[15]);c.r[15]+=4;c.pc=0x8c606cb6;return {c,26,true,c.r[3]};
}
static Prediction first_query(const SH4Context& b,auto& m){
 auto c=b;auto push=[&](unsigned v){c.r[15]-=4;put(m,c.r[15],v);};
 push(c.r[14]);c.r[14]=0;push(c.r[13]);c.r[13]=8;push(c.pr);c.pr=0x8c0557a6;c.r[4]=0;
 push(c.pr);c.r[15]-=80;m[c.r[15]&0xffffff]=c.r[4]&255;c.r[5]=c.r[15]+4;
 c.r[4]=m[c.r[15]&0xffffff];c.r[3]=word(m,0x8c055810);need(c.r[3]==0x8c606c9c,"Unexpected connected provider literal");c.pr=0x8c05577a;
 auto p=provider(c,m);need(!p.rejected&&p.c.r[0]==0xffffffff,"Actual first query must use nonzero guard return");c=p.c;
 c.r[2]=0xffffff01;c.r[4]=c.r[0];c.sr&=~1u;c.r[0]=1;c.r[15]+=80;c.pr=word(m,c.r[15]);c.r[15]+=4;c.pc=c.pr;
 c.sr&=~1u;c.r[0]=1;c.pr=word(m,c.r[15]);c.r[15]+=4;c.r[13]=word(m,c.r[15]);c.r[15]+=4;
 c.r[14]=word(m,c.r[15]);c.r[15]+=4;c.pc=c.pr;
 return {c,18+p.cycles+12+9,false,0};
}
static std::string address(unsigned v){std::ostringstream s;s<<"0x"<<std::uppercase<<std::hex<<v;return s.str();}
int main(int argc,char**argv){try{
 need(argc==7,"usage: provider RAM STATE FLASH OUTPUT CALLER_RAM CALLER_STATE");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Provider fixtures differ");
 auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 for(unsigned target:{0x8c606c9cu,0x8c64b9e4u,0x8c055798u,0x8c055768u,0x8c011238u,0x8c08bd14u,0x8c08a394u,0x8c089f20u,0x8c10ef3cu,0x8c10e9fcu})need(rt.targets.contains(target),"Original provider/query/prior contracts omitted");
 need(!rt.targets.contains(0x8c64c766)&&!rt.targets.contains(0xac64c766)&&!rt.targets.contains(0x8c010fcc),"Unavailable tail/providers substituted");
 auto reset=[&](unsigned start,unsigned pattern){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=pattern&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto&c=rt.tmu[i];c.tcor=0x11223300+i*17+pattern;c.tcnt=0x55667700+i*31+pattern;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m&&rt.aica_ram==sound&&rt.system_flash==flash,"Provider wholeRAM/guards/devices differ");need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Provider unrelated runtime state changed");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Provider timer controls differ");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 unsigned returns=0,rejections=0,overlaps=0,literals=0;
 auto one=[&](SH4Context c,unsigned start,unsigned pattern){auto m=rt.main_ram;auto wanted=provider(c,m);auto timers=rt.tmu;tick(timers,start,wanted.cycles);const auto tocr=rt.tmu_tocr;
  if(wanted.rejected){need(reject(c,rt,0x8c606c9c,address(wanted.target).c_str()),"Provider missing target rejection absent");++rejections;}
  else {call_recompiled(c,rt,0x8c606c9c);++returns;}
  same(wanted.c,c);exact(m,timers,start,tocr);
 };
 const unsigned indices[]={0,1,7,127,128,255,256,0x01ffffff,0x02000000,0x03ffffff,0xffffff80,0xffffff01,0xdead0cab};
 for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned target_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment)
 for(unsigned index:indices)for(unsigned status:{0u,1u}){
  unsigned start=mask&7;reset(start,pattern);auto c=context(0x8c606c9c,mask,t,bank);c.pr=0x8c05577a;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);c.r[4]=index;c.r[5]=0x8cfe1200+alignment;
  for(unsigned i=0;i<256;++i)rt.main_ram[0xefd000+i]=pattern+19*i;
  put(rt.main_ram,0x8c606f50,target_alias?0xac64b9e4:0x8c64b9e4);put(rt.main_ram,0x8c606f54,target_alias?0xac64c766:0x8c64c766);
  unsigned table=(target_alias?0xac900000u:0x8c900000u)+alignment;put(rt.main_ram,0x8c64ba50,table-(index<<7)-4);put(rt.main_ram,table,status);one(c,start,pattern);
 }
 need(returns==26624&&rejections==26624,"Provider main alias/index/status matrix differs");
 // Table aliases each of the three ordered frame stores. Preload zero: the actual
 // write may change the branch, so the model reads it only after the stores.
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment)for(unsigned index:{0u,1u,0x02000000u,0xffffff01u})for(unsigned slot:{0u,1u,2u}){
  unsigned start=mask&7;reset(start,0xaa);auto c=context(0x8c606c9c,mask,t,bank);c.pr=0x8c05577a;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);c.r[4]=index;c.r[5]=0xacfe1200+alignment;
  auto table=c.r[15]-12+slot*4;put(rt.main_ram,table,0);put(rt.main_ram,0x8c64ba50,table-(index<<7)-4);put(rt.main_ram,0x8c606f50,0x8c64b9e4);put(rt.main_ram,0x8c606f54,0x8c64c766);one(c,start,0xaa);++overlaps;
 }
 // Live literals overlap frame writes: guard target is fetched before the two
 // local stores, guard base after them, alternate tail literal after its guard.
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned layout=0;layout<6;++layout){
  unsigned start=mask&7;reset(start,0x55);auto c=context(0x8c606c9c,mask,t,bank);c.pr=0x8c05577a;c.r[4]=1;c.r[5]=0x8cfe1200;unsigned sp=0;
  put(rt.main_ram,0x8c606f50,0x8c64b9e4);put(rt.main_ram,0x8c606f54,0x8c64c766);
  if(layout==0){sp=0x8c606f5c;}// index store overwrites guard literal after fetched
  else if(layout==1){sp=0x8c606f54;c.pr=0xac64b9e4;}// savedPR overwrites guard literal before read
  else if(layout==2){sp=0x8c64ba54;c.pr=0x8c900000-(c.r[4]<<7);}// savedPR overwrites guard base
  else if(layout==3){sp=0x8c64ba5c;c.r[4]=0x44900000;}// 129*index wraps to 0x8c900000
  else if(layout==4){sp=0x8c64ba58;c.r[5]=0x8c900000-(c.r[4]<<7);}// output local overwrites guard base
  else {sp=0x8c606f60;c.r[4]=0xac64c766;}// index store overwrites alternate tail literal
  c.r[15]=sp+(stack_alias?0x20000000u:0u);
  put(rt.main_ram,0x8c64ba50,0x8c900000-(c.r[4]<<7));put(rt.main_ram,0x8c900004,layout==5?0:1);one(c,start,0x55);++literals;
 }
 reset(0,0x55);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c055768&&c.pr==0x8c05577a&&c.r[15]==0x8cffff44&&c.r[4]==0&&c.r[5]==0x8cffff48,"Frozen provider entry differs");
 need(word(main,0x8c8983bc)==1&&word(main,0x8c606f50)==0x8c64b9e4&&word(main,0x8c64ba50)==0x8c8983b8,"Actual registered guard/table differs");
 auto m=main;auto wanted=provider(c,m);need(!wanted.rejected&&wanted.cycles==24&&wanted.c.r[0]==0xffffffff&&wanted.c.pc==0x8c05577a,"Actual provider prediction differs");auto timers=rt.tmu;tick(timers,rt.tmu_tstr,24);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
 save_state(wanted.c,std::string(argv[4])+"/expected-startup-query-provider-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-query-provider-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-query-provider-ram.bin");
 call_recompiled(c,rt,0x8c606c9c);same(wanted.c,c);exact(m,timers,start,tocr);
 // Retained original query caller: live guard1 returnsminus1, helperreturns1,
 // outer exits at index0. This does not claim successful eight-query iteration.
 auto caller_ram=read(argv[5]);reset(0,0x55);rt.main_ram=caller_ram;load_timers(rt,argv[6]);c=state(argv[6]);need(c.pc==0x8c055942&&c.pr==0x8c055946&&c.r[15]==0x8cffffa4,"Frozen original query caller differs");m=caller_ram;wanted=first_query(c,m);need(wanted.cycles==63&&wanted.c.r[0]==1&&wanted.c.pc==0x8c055946,"Firstquery return prediction differs");timers=rt.tmu;tick(timers,rt.tmu_tstr,63);start=rt.tmu_tstr;tocr=rt.tmu_tocr;
 save_state(wanted.c,std::string(argv[4])+"/expected-startup-first-query-return-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-first-query-return-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-first-query-return-ram.bin");
 call_recompiled(c,rt,0x8c055798);same(wanted.c,c);exact(m,timers,start,tocr);
 std::ofstream result(std::string(argv[4])+"/provider-result.json");result<<"{\"returned_cases\":"<<returns<<",\"rejected_cases\":"<<rejections<<",\"overlap_cases\":"<<overlaps<<",\"literal_cases\":"<<literals<<",\"actual_provider_cycles\":24,\"actual_first_query_return_cycles\":63,\"passed\":true}\n";need(bool(result),"Cannot save provider result");
 std::cout<<"PASS original606C9C/64B9E4 provider: "<<returns<<" returns/"<<rejections<<" explicitmissing rejects; "<<overlaps<<" ordered stack-table overlaps/"<<literals<<" live literal overlaps;13wrap-highbit indices/independentP1-P2stack-guard-tailaliases/fouralignments/all16IRQ-T-FPU; whole16MiBRAM69CPU25TMU/devices unchanged; actualguard1 returnsminus1 in24cycles; zero tail rejects64C766 in26cycles; actual connected055798/055768 firstquery earlyreturn1 in63cycles, not eight-query success; prior contracts retained/no stub/G1-fullinit-boot-gameplay open\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
