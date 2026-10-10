// Independent expectations for original timed work and eight-record scanning.
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
static void scan_stack(auto& m,const SH4Context& b){put(m,b.r[15]-4,b.r[14]);put(m,b.r[15]-8,b.r[13]);put(m,b.r[15]-12,b.r[12]);put(m,b.r[15]-16,b.pr);}
static SH4Context scan_boundary(const SH4Context& b,int active,unsigned last){
 auto c=b;
 if(active<0){c.pc=b.pr;c.r[0]=static_cast<unsigned>(static_cast<int>(static_cast<signed char>(last)));c.r[4]=0x8c8a7f80;c.sr|=1;}
 else{c.pc=0x8c65f92e;c.pr=0x8c65f932;c.r[0]=1;c.r[4]=c.r[14]=0x8c8a7b90+active*0x90;c.r[12]=0x8c8a8010;c.r[13]=0x90;c.r[15]-=16;c.sr|=1;}
 return c;
}
static SH4Context work_boundary(const SH4Context& b,const auto& initial,unsigned start,auto& timers){
 auto c=b;timers=initial;tick(timers,start,18);const unsigned first=0xffffffff-timers[0].tcnt;
 tick(timers,start,84);const unsigned second=0xffffffff-timers[0].tcnt;tick(timers,start,15);
 c.pc=0x8c64844c;c.pr=0x8c648454;c.r[0]=second-first;c.r[2]=0x8c6778a0;c.r[3]=0x8c65f25a;
 c.r[4]=c.r[13]=first;c.r[5]=c.r[14]=second;c.r[8]=0x8c606358;c.r[9]=0x8c60634e;c.r[15]-=32;c.sr|=1;
 return c;
}
static void work_ram(auto& m,const SH4Context& b,const SH4Context& result){
 unsigned sp=b.r[15];for(unsigned i=0;i<7;++i)put(m,sp-4*(i+1),b.r[14-i]);put(m,sp-32,b.pr);
 auto inner=b;inner.r[15]-=32;inner.pr=0x8c64843e;inner.r[13]=result.r[13];scan_stack(m,inner);
 put(m,0x8c6778a0,result.r[0]);
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: startup-work RAM STATE FLASH OUTPUT");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16777216&&flash.size()==131072,"Wrong startup work fixtures");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;
 auto reset=[&](unsigned start,unsigned pattern){rt.main_ram=main;rt.system_flash=flash;rt.tmu_tstr=start;rt.tmu_tocr=pattern&1;rt.sh4_tick_pending_cycles=0;rt.sh4_tick_batch_cycles=1;for(unsigned i=0;i<3;++i){auto& c=rt.tmu[i];c.tcor=0x11223300+i*17+pattern;c.tcnt=0x55667700+i*31+pattern;c.tcr=0x101+i;c.cycle_accumulator=123+i;c.lazy_sh4_cycles=0;c.ticks=0x12345+i;c.underflows=0x987+i;}};
 auto exact=[&](const auto&m,const auto&timers,unsigned start,unsigned tocr){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Work RAM mismatch "<<std::hex<<(0x8c000000+i)<<'\n';throw std::runtime_error("Startup work wholeRAM/stack/guards differs");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0&&!rt.device_clock_enabled,"Startup work changes frozen unrelated devices");need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Startup work timer control/batch differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);};
 auto records=[&](unsigned value){for(unsigned i=0;i<8;++i)rt.main_ram[0x8a7b90+i*0x90]=value;};
 auto guards=[&](const SH4Context& c,unsigned pattern){for(unsigned i=0;i<80;++i)rt.main_ram[(c.r[15]&0xffffff)-64+i]=static_cast<unsigned char>(pattern+13*i);};
 unsigned work_cases=0,inactive_cases=0,active_cases=0;
 for(unsigned start=0;start<8;++start)for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset(start,pattern);auto c=context(0x8c648422,mask,t,bank);if(pattern==0xaa)c.sr|=0x8302;records(0);guards(c,pattern);auto expected=rt.main_ram;auto timers=rt.tmu;auto wanted=work_boundary(c,rt.tmu,start,timers);work_ram(expected,c,wanted);auto tocr=rt.tmu_tocr;
  need(reject(c,rt,0x8c648422,"0x8C65F25A"),"Original timed work did not reject its next missing helper");same(wanted,c);exact(expected,timers,start,tocr);++work_cases;
 }
 for(unsigned flag:{0u,2u,255u})for(unsigned pattern:{0x55u,0xaau})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset(7,pattern);auto c=context(0x8c65f914,mask,t,bank);records(flag);guards(c,pattern);auto expected=rt.main_ram;scan_stack(expected,c);auto wanted=scan_boundary(c,-1,flag);auto timers=rt.tmu;tick(timers,7,71);auto tocr=rt.tmu_tocr;
  call_recompiled(c,rt,0x8c65f914);same(wanted,c);exact(expected,timers,7,tocr);++inactive_cases;
 }
 for(unsigned active=0;active<8;++active)for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset(7,0x55);auto c=context(0x8c65f914,mask,t,bank);records(0);rt.main_ram[0x8a7b90+active*0x90]=1;guards(c,0x55);auto expected=rt.main_ram;scan_stack(expected,c);auto wanted=scan_boundary(c,active,0);auto timers=rt.tmu;tick(timers,7,16+7*active);auto tocr=rt.tmu_tocr;
  need(reject(c,rt,0x8c65f914,"0x8C65F8C8"),"Active record did not reject original missing helper");same(wanted,c);exact(expected,timers,7,tocr);++active_cases;
 }
 reset(0,0x55);load_timers(rt,argv[2]);auto c=state(argv[2]);need(c.pc==0x8c0556ce&&c.pr==0x8c0556d4&&c.r[15]==0x8cffffc8&&c.sr==0x40000001,"Frozen work boundary differs");
 for(unsigned i=0;i<8;++i)need(main[0x8a7b90+i*0x90]==0,"Actual captured task record is not inactive");
 auto expected=main;auto timers=rt.tmu;auto wanted=work_boundary(c,rt.tmu,rt.tmu_tstr,timers);work_ram(expected,c,wanted);auto tocr=rt.tmu_tocr;
 save_ram(expected,std::string(argv[4])+"/expected-startup-work-ram.bin");save_state(wanted,std::string(argv[4])+"/expected-startup-work-state.txt");save_timers(timers,rt.tmu_tstr,tocr,std::string(argv[4])+"/expected-startup-work-timers.txt");
 need(reject(c,rt,0x8c648422,"0x8C65F25A"),"Actual original work failed to reject next helper");same(wanted,c);exact(expected,timers,rt.tmu_tstr,tocr);
 std::cout<<"PASS original startup work: "<<work_cases<<" timed work, "<<inactive_cases<<" inactive record, "<<active_cases<<" active helper rejection cases; whole16MiB RAM/stack/guards/all69CPU/all25TMU fields; original eight-record65F914 loop and60634E/606358 timestamps, independent117 modeledcycles, actual captured call once; explicit65F25A and65F8C8 rejection, no stub/normalboot/gameplay/G1 claim\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
