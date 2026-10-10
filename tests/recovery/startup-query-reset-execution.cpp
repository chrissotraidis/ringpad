// Original05A2E6/088644: reverse byte fills, ordered savedPR and live late-global read.
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
























struct Prediction{SH4Context c;unsigned cycles;bool rejected;unsigned target;};
static unsigned fill(SH4Context& c,auto& m){
 const auto count=c.r[6];need(count>0&&count<=1024,"Reduction only covers bounded positive fill counts");
 c.r[4]+=count;do{--c.r[6];c.sr=(c.sr&~1u)|(c.r[6]==0);--c.r[4];m[c.r[4]&0xffffff]=c.r[5]&255;}while(c.r[6]);
 c.pc=c.pr;return 3*count+4;
}
static Prediction reset_model(const SH4Context&b,auto& m){
 auto c=b;c.r[15]-=4;put(m,c.r[15],c.pr);c.r[4]=word(m,0x8c05a324);c.r[6]=18;c.r[3]=word(m,0x8c05a328);
 const auto target=c.r[3];c.r[5]=0;c.pr=0x8c05a2f2;c.pc=0x8c05a2e6;
 if(target!=0x8c088644&&target!=0xac088644)return{c,7,true,target};
 const auto cycles=7+fill(c,m)+6;c.pr=word(m,c.r[15]);c.r[15]+=4;
 c.r[3]=word(m,0x8c05a32c);c.r[2]=0;put(m,c.r[3],0);c.pc=c.pr;
 return{c,cycles,false,0};
}
static std::string address(unsigned v){std::ostringstream s;s<<"0x"<<std::uppercase<<std::hex<<v;return s.str();}
int main(int argc,char**argv){try{
 need(argc==5,"usage: queryreset RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Reset fixtures differ");
 auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 for(unsigned target:{0x8c05a2e6u,0x8c088644u,0x8c606c9cu,0x8c64b9e4u,0x8c055798u,0x8c055768u,0x8c011238u,0x8c08bd14u,0x8c08a394u,0x8c089f20u,0x8c10ef3cu,0x8c10e9fcu})need(rt.targets.contains(target),"Original reset/helper/prior contracts omitted");
 need(!rt.targets.contains(0x8c010fcc)&&!rt.targets.contains(0xac010fcc)&&!rt.targets.contains(0x8c64c766),"Missing providers replaced");
 auto reset=[&](unsigned start,unsigned pattern){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=pattern&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto&c=rt.tmu[i];c.tcor=0x11223300+i*17+pattern;c.tcnt=0x55667700+i*31+pattern;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){need(rt.main_ram==m&&rt.aica_ram==sound&&rt.system_flash==flash,"Reset wholeRAM/guards/devices differ");need(!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Reset unrelated runtime changed");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Reset timer controls differ");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 unsigned callers=0,helpers=0,overlaps=0,literals=0,rejected=0;
 auto one=[&](SH4Context c,unsigned start){auto m=rt.main_ram;auto wanted=reset_model(c,m);auto timers=rt.tmu;tick(timers,start,wanted.cycles);const auto tocr=rt.tmu_tocr;
  if(wanted.rejected){need(reject(c,rt,0x8c05a2e6,address(wanted.target).c_str()),"Reset missingcallee rejection absent");++rejected;}
  else call_recompiled(c,rt,0x8c05a2e6);
  same(wanted.c,c);exact(m,timers,start,tocr);
 };
 auto setup=[&](unsigned dest,unsigned target,unsigned global,unsigned pattern){for(unsigned i=0;i<1024;++i)rt.main_ram[0xefd000+i]=pattern+19*i;put(rt.main_ram,global,0xdead0cab);put(rt.main_ram,0x8c05a324,dest);put(rt.main_ram,0x8c05a328,target);put(rt.main_ram,0x8c05a32c,global);};
 for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned callee_alias:{0u,1u})for(unsigned dest_alias:{0u,1u})for(unsigned global_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment){
  unsigned start=mask&7;reset(start,pattern);auto c=context(0x8c05a2e6,mask,t,bank);c.pr=0x8c05595a;c.r[0]=pattern==0x55?1:0xffffffff;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);
  setup((dest_alias?0xac000000u:0x8c000000u)|(0xefd300+alignment),callee_alias?0xac088644:0x8c088644,(global_alias?0xac000000u:0x8c000000u)|(0xefd400+alignment),pattern);one(c,start);++callers;
 }
 need(callers==8192,"Reset caller alias matrix differs");
 for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned dest_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment)for(unsigned count:{1u,2u,3u,18u,127u,256u,1024u})for(unsigned value:{0u,0xdeadbe55u,0x123400aau}){
  unsigned start=mask&7;reset(start,pattern);auto c=context(0x8c088644,mask,t,bank);c.pr=0x8c05a2f2;c.r[4]=(dest_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);c.r[5]=value;c.r[6]=count;
  for(unsigned i=0;i<1100;++i)rt.main_ram[0xefd000+i]=pattern+19*i;
  auto m=rt.main_ram;auto wanted=c;auto cycles=fill(wanted,m);auto timers=rt.tmu;tick(timers,start,cycles);const auto tocr=rt.tmu_tocr;
  call_recompiled(c,rt,0x8c088644);same(wanted,c);exact(m,timers,start,tocr);++helpers;
 }
 need(helpers==21504,"Reset helper positive count/value matrix differs");
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment)for(unsigned layout=0;layout<8;++layout){
  unsigned start=mask&7;reset(start,0xaa);auto c=context(0x8c05a2e6,mask,t,bank);c.pr=0x8c05595a;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);auto dest=0x8cefd300u+alignment,global=0xacefd400u+alignment;
  if(layout==0)dest=c.r[15]-4;else if(layout==1)dest=c.r[15]-21;else if(layout==2)dest=c.r[15]-3;else if(layout==3)dest=c.r[15]-22;
  else if(layout==4)global=dest;else if(layout==5)global=c.r[15]-4;else if(layout==6)global=c.r[15]-1;else global=dest+17;
  setup(dest,0x8c088644,global,0xaa);one(c,start);++overlaps;
 }
 need(overlaps==4096,"Reset ordered overlap matrix differs");
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned callee_alias:{0u,1u})for(unsigned layout=0;layout<4;++layout){
  unsigned start=mask&7;reset(start,0x55);auto c=context(0x8c05a2e6,mask,t,bank);c.pr=0x8c05595a;unsigned dest=0x8cefd300,global=0xacefc37f,sp=0x8cefd100;
  if(layout==0){sp=0x8c05a328;c.pr=0x8cefd300;}// pushPR replaces destination literal before read
  else if(layout==1){sp=0x8c05a32c;c.pr=callee_alias?0xac088644:0x8c088644;}// pushPR replaces callee literal before read
  else {dest=layout==2?0x8c05a31b:0x8c05a31c;}// fill overwrites early literals and one/two low bytes of late global pointer
  c.r[15]=sp+(stack_alias?0x20000000u:0u);setup(dest,callee_alias?0xac088644:0x8c088644,global,0x55);one(c,start);++literals;
 }
 need(literals==1024,"Reset live literal matrix differs");
 for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack_alias:{0u,1u})for(unsigned callee_alias:{0u,1u})for(unsigned alignment=0;alignment<4;++alignment){
  unsigned start=mask&7;reset(start,0xaa);auto c=context(0x8c05a2e6,mask,t,bank);c.pr=0x8c05595a;c.r[15]=(stack_alias?0xac000000u:0x8c000000u)|(0xefd100+alignment);
  setup(0x8cefd300+alignment,callee_alias?0xac010fcc:0x8c010fcc,0x8cefd400+alignment,0xaa);one(c,start);
 }
 need(rejected==1024,"Reset genuine missingcallee controls differ");
 reset(0,0x55);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c055954&&c.pr==0x8c05595a&&c.r[15]==0x8cffffa4&&c.r[0]==1&&c.r[4]==0xffffffff&&c.sr==0x40000100,"Frozen reset boundary differs");
 need(word(main,0x8c05a324)==0x8c78f4d0&&word(main,0x8c05a328)==0x8c088644&&word(main,0x8c05a32c)==0x8c7902c8,"Frozen reset literals differ");
 auto m=main;auto wanted=reset_model(c,m);need(!wanted.rejected&&wanted.cycles==71&&wanted.c.pc==wanted.c.pr&&wanted.c.pc==0x8c05595a&&wanted.c.r[15]==0x8cffffa4&&wanted.c.r[0]==1&&wanted.c.r[4]==0x8c78f4d0&&wanted.c.r[6]==0&&(wanted.c.sr&1)==1,"Frozen71cycle return model differs");auto timers=rt.tmu;tick(timers,rt.tmu_tstr,71);auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
 save_state(wanted.c,std::string(argv[4])+"/expected-startup-query-reset-state.txt");save_timers(timers,start,tocr,std::string(argv[4])+"/expected-startup-query-reset-timers.txt");save_ram(m,std::string(argv[4])+"/expected-startup-query-reset-ram.bin");
 call_recompiled(c,rt,0x8c05a2e6);same(wanted.c,c);exact(m,timers,start,tocr);
 std::ofstream result(std::string(argv[4])+"/reset-result.json");result<<"{\"caller_cases\":"<<callers<<",\"helper_cases\":"<<helpers<<",\"overlap_cases\":"<<overlaps<<",\"literal_cases\":"<<literals<<",\"missing_callee_cases\":"<<rejected<<",\"actual_return_cycles\":71,\"passed\":true}\n";need(bool(result),"Cannot save reset result");
 std::cout<<"PASS original05A2E6/088644 reset: "<<callers<<" caller/"<<helpers<<" positive-count helper/"<<overlaps<<" ordered stack-destination-global overlaps/"<<literals<<" live literal overlaps/"<<rejected<<" genuine missingcallee rejects; actual18descending byte stores including finalBF-Sdelay, pushPR/pop before late globalzero RTSdelay;20stores26bytes/4byteframe/71cycles PC-PR05595A/SPFFFFA4/R0one unchanged/T1;7positive counts and3truncated-byte values/P1-P2aliases/fouralignments/all16IRQ-T-FPU/whole16MiBRAM69CPU25TMU/devices unchanged; no zero-negative termination, hostmemset, stub, fullstartup or G1-fullinit-boot-gameplay claim\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
