// Original045618 and exact caller continuations; retained089F20 is delegated.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void global_setup_register(DCRuntime&);void global_reset_register(DCRuntime&);void global_word_copy_register(DCRuntime&);}
struct GlobalTail : RequestModel {
 void copy(){c.r[2]=w(0x8c045684);c.r[3]=w(c.r[4]);c.pc=c.pr;put(m,c.r[2],c.r[3]);cycles+=5;}
 void setup_return(){c.r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=5;}
 void record_entry(){auto&r=c.r;r[2]=w(0x8c090fe0);r[4]=0;r[3]=w(0x8c090fe4);put(m,r[2],r[4]);r[2]=w(0x8c090fec);r[1]=w(0x8c090fe8);put(m,r[3],r[4]);r[3]=w(0x8c090ff0);put(m,r[1],r[4]);put(m,r[2],r[4]);c.pr=pop();c.pc=r[3];need(c.pc==0x8c089f20,"Unreviewed record tail");cycles+=13;}
};
static RecompiledFunction real_record;
static std::function<void(SH4Context&)> observe_record;
static void watched_record(SH4Context&c,DCRuntime&rt){observe_record(c);real_record(c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: global-word-copy RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);global_setup_register(rt);global_reset_register(rt);global_word_copy_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];unsigned controls=0;
 auto exact=[&](const GlobalTail&model,const SH4Context&c,const auto&expected,const auto&timers,unsigned start,unsigned tocr){same(model.c,c);need(expected==rt.main_ram,"Global tail whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Global tail timer controls differ");};
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(int delta:{-4,0,4,32})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=(alias?0xacc01000:0x8cc01000)+delta;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;const unsigned dest=0x8cc01000;put(rt.main_ram,0x8c045684,dest);put(rt.main_ram,dest,0x12345678);put(rt.main_ram,c.r[4],value);auto expected=rt.main_ram;GlobalTail model{{c,expected}};model.copy();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,5);call_recompiled(c,rt,0x8c045618);exact(model,c,expected,timers,start,tocr);need(word(rt.main_ram,dest)==value,"Independent copied word differs");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;auto expected=ram;GlobalTail model{{c,expected}};const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto timers=rt.tmu;
 auto save=[&](unsigned phase){const auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(model.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");};
 model.copy();tick(timers,start,5);save(0);call_recompiled(c,rt,0x8c045618);exact(model,c,expected,timers,start,tocr);need(c.pc==0x8c090f04,"Captured copy return differs");
 model.setup_return();tick(timers,start,5);save(1);call_recompiled(c,rt,0x8c090f04);exact(model,c,expected,timers,start,tocr);need(c.pc==0x8c090f12,"Captured setup epilogue differs");
 model.record_entry();tick(timers,start,13);save(2);unsigned seen=0;real_record=rt.targets.at(0x8c089f20);observe_record=[&](SH4Context&actual){need(seen++==0,"Repeated record tail");exact(model,actual,expected,timers,start,tocr);};rt.register_target(0x8c089f20,watched_record);
 std::string terminal;try{call_recompiled(c,rt,0x8c090f12);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}rt.register_target(0x8c089f20,real_record);observe_record={};need(seen==1,"Record tail not reached");
 save_ram(rt.main_ram,out+"/record-observed-ram.bin");save_state(c,out+"/record-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/record-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';
 need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"copy_cycles\":5,\"prospective_connected_cycles\":[5,10,23],\"whole_RAM_CPU_TMU_verified\":true,\"setup_epilogue_verified\":true,\"global_clears_and_record_tail_entry_verified\":true,\"record_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original global word copy: "<<controls<<" controls;5/10/23cycle whole RAM CPU TMU boundaries;real record tail subsequent state observed only: "<<terminal<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
