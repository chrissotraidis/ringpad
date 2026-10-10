// Original signed-byte getter and the actual zero-status dispatcher branch.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void frame_dispatch_register(DCRuntime&);void input_status_register(DCRuntime&);}
struct InputStatus : RequestModel {
 void get(){c.r[2]=w(0x8c05278c);c.pc=c.pr;c.r[0]=sb(c.r[2]);cycles+=4;}
 // Pinned emitter issue model: TST+BT=2, literal+JSR+delay=4; no pipeline penalty.
 void zero_entry(){t(c.r[0]==0);need(c.sr&1,"Captured status is not zero");c.r[3]=w(0x8c091008);need(c.r[3]==0x8c09a544,"Unreviewed common frame target");c.pr=0x8c090f56;c.pc=c.r[3];cycles+=6;}
};
static RecompiledFunction real_common;
static std::function<void(SH4Context&)> observe_common;
static void watched_common(SH4Context&c,DCRuntime&rt){observe_common(c);real_common(c,rt);}
int main(int argc,char**argv){try{
 need(argc==6,"usage: input-status RAM STATE TIMERS FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);frame_dispatch_register(rt);input_status_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[4]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[5];unsigned controls=0;
 auto exact=[&](const InputStatus&model,const SH4Context&c,const auto&expected,const auto&timers,unsigned start,unsigned tocr){same(model.c,c);need(expected==rt.main_ram,"Input-status whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Input-status timer controls differ");};
 for(unsigned value=0;value<256;++value)for(unsigned bank=0;bank<2;++bank){
  rt.main_ram=ram;load_timers(rt,argv[3]);auto c=initial;c.pr=bank?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;
  const unsigned addr=bank?0xacc01003:0x8cc01003;put(rt.main_ram,0x8c05278c,addr);rt.main_ram[addr&0xffffff]=value;
  auto expected=rt.main_ram;InputStatus model{{c,expected}};model.get();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,4);call_recompiled(c,rt,0x8c052766);exact(model,c,expected,timers,start,tocr);need(c.r[0]==(value<128?value:0xffffff00u|value),"Signed byte result differs");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[3]);auto c=initial;auto expected=ram;InputStatus model{{c,expected}};const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto timers=rt.tmu;
 auto save=[&](unsigned phase){const auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(model.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");};
 model.get();tick(timers,start,4);save(0);call_recompiled(c,rt,0x8c052766);exact(model,c,expected,timers,start,tocr);need(c.pc==0x8c090f40&&c.r[0]==0,"Captured getter return differs");
 model.zero_entry();tick(timers,start,6);save(1);unsigned seen=0;real_common=rt.targets.at(0x8c09a544);observe_common=[&](SH4Context&actual){need(seen==0,"Repeated common frame entry");exact(model,actual,expected,timers,start,tocr);++seen;};rt.register_target(0x8c09a544,watched_common);
 std::string terminal;try{call_recompiled(c,rt,0x8c090f40);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}rt.register_target(0x8c09a544,real_common);observe_common={};need(seen==1,"Common frame routine not reached");
 save_ram(rt.main_ram,out+"/frame-observed-ram.bin");save_state(c,out+"/frame-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/frame-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';
 need(terminal.find("No recompiled/native target registered for Dreamcast address")!=std::string::npos,"Unexpected downstream outcome");need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"getter_cycles\":4,\"prospective_connected_cycles\":[4,10],\"whole_RAM_CPU_TMU_verified\":true,\"zero_status_branch_verified\":true,\"common_frame_return_verified\":false,\"frame_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original input status: "<<controls<<" signed-byte controls;4/10cycle whole RAM CPU TMU boundaries;real common frame routine delegated;later state observed only: "<<terminal<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
