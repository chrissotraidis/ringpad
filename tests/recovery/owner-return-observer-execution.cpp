// Original011238: live gate read after ordered frame and sixteen-byte record writes.
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























namespace dcrecomp_generated { void callback_register(DCRuntime&); }
static void require(bool ok,const char* why){need(ok,why);}
static thread_local RecompiledFunction original_startup_owner_return=nullptr;
static thread_local std::function<void(bool)> startup_owner_return_observer;
static void observed_startup_owner_return(SH4Context& ctx,DCRuntime& rt){
    if(startup_owner_return_observer)startup_owner_return_observer(false);
    original_startup_owner_return(ctx,rt);
    if(startup_owner_return_observer)startup_owner_return_observer(true);
}
int main(int argc,char**argv){try{
 need(argc==8,"usage: ownerreturn actualRAM actualSTATE expectedRAM expectedSTATE expectedTIMERS flash OUTPUT");auto owner=std::make_unique<DCRuntime>(),want=std::make_unique<DCRuntime>();auto&rt=*owner;const auto m=read(argv[1]),wm=read(argv[3]),flash=read(argv[6]);need(m.size()==16777216&&wm.size()==16777216&&flash.size()==131072,"Owner fixtures differ");load_embedded_elf_image(rt);callback_register(rt);rt.main_ram=m;rt.system_flash=flash;load_timers(rt,argv[2]);load_timers(*want,argv[5]);auto ctx=state(argv[2]);const auto wanted=state(argv[4]);need(ctx.pc==ctx.pr&&ctx.pc==0x8c04e33e&&ctx.r[15]==0x8cffffa0&&ctx.r[0]==0,"Gate-return continuation fixture differs");need(rt.targets.at(0x8c04e33e)==rt.targets.at(0x8c04e310),"Actual original owning block alias differs");const auto sound=rt.aica_ram;unsigned captures=0;
 auto exact=[&]{same(wanted,ctx);need(rt.main_ram==wm&&rt.system_flash==flash&&rt.aica_ram==sound,"Owner fullRAM/device contract differs");for(unsigned i=0;i<3;++i)timer_same(rt.tmu[i],want->tmu[i]);need(rt.tmu_tstr==want->tmu_tstr&&rt.tmu_tocr==want->tmu_tocr&&rt.sh4_tick_pending_cycles==want->sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==want->sh4_tick_batch_cycles,"Owner timer controls differ");need(!rt.device_clock_enabled&&!rt.sh4_async_redirect&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&rt.g2_dma_runs==0,"Owner unrelated runtime state changed");};
 auto capture=[&](const char* phase){need(std::string(phase)=="first-startup-owner-return-exit","Observer phase differs");++captures;exact();};
    original_startup_owner_return=rt.targets.at(0x8C10EFBC);
    require(original_startup_owner_return!=nullptr,"Original startup owner_return absent");
    bool startup_owner_return_captured=false,startup_owner_return_pending=false;
    startup_owner_return_observer=[&](bool after){
        if(!after&&!startup_owner_return_captured&&ctx.pc==0x8C10EFBC&&ctx.pr==0x8C055942){
            startup_owner_return_captured=true;startup_owner_return_pending=true;
        }else if(after&&startup_owner_return_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C055942&&ctx.pr==0x8C055942){
            capture("first-startup-owner-return-exit");startup_owner_return_pending=false;
        }
    };
    rt.register_target(0x8C10EFBC,observed_startup_owner_return);
    struct RestoreStartupowner_return{DCRuntime& rt;~RestoreStartupowner_return(){
        rt.register_target(0x8C10EFBC,original_startup_owner_return);startup_owner_return_observer={};original_startup_owner_return=nullptr;
    }}restore_startup_owner_return{rt};
 call_recompiled(ctx,rt,0x8c04e33e);exact();need(captures==1,"Original tail observer did not capture once");save_state(ctx,std::string(argv[7])+"/owner-return-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,std::string(argv[7])+"/owner-return-timers.txt");save_ram(rt.main_ram,std::string(argv[7])+"/owner-return-ram.bin");
 std::cout<<"PASS original04E310 continuation+10EFBC tail:16cycles/whole16MiB RAM69CPU25TMU/PC-PR055942/SPFFFFA4/two original stores byte0 and longDEAD0CAB; exact pending observer captures once at registeredtail after savedPR restored. Uses existing853-function compiledcallback objects and unchanged original prediction; no original work replay, provider stub, new generated game body or fullstartup rerun; fullstartup owner capture remains absent\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
