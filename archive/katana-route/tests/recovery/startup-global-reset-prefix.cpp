// Reviewed original090F0C and090EB0 prefixes; all missing callees reject.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void global_reset_register(DCRuntime&);void global_setup_register(DCRuntime&);}
struct ResetPrefix : RequestModel {
 void parent(){push(c.pr);c.pr=0x8c090f12;c.pc=0x8c090f0c;cycles+=4;}
 void child(){push(c.pr);c.r[15]-=4;c.r[3]=w(0x8c090fb8);need(c.r[3]==0x8c09a6b0,"Unreviewed first setup helper");c.pr=0x8c090eba;c.pc=0x8c090eb0;cycles+=6;}
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: global-reset-prefix RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);global_reset_register(rt);
 need(!rt.targets.contains(0x8c090eb0)&&!rt.targets.contains(0x8c09a6b0),"Missing callee unexpectedly bound");need(rt.targets.contains(0x8c089f20),"Real tail absent");
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];unsigned controls=0;
 auto check=[&](SH4Context c,bool connected,bool capture){auto expected=rt.main_ram;ResetPrefix model{{c,expected}};model.parent();if(connected)model.child();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);need(model.cycles==(connected?10:4),"Prospective prefix timing differs");const std::string stem=connected?"setup":"reset";
  if(capture){save_ram(expected,out+"/"+stem+"-expected-ram.bin");save_state(model.c,out+"/"+stem+"-expected-state.txt");save_timers(timers,start,tocr,out+"/"+stem+"-expected-timers.txt");}
  bool rejected=false;try{call_recompiled(c,rt,0x8c090f0c);}catch(const std::runtime_error&e){need(std::string(e.what()).find(connected?"No recompiled/native target registered for Dreamcast address 0x8C09A6B0":"No recompiled/native target registered for Dreamcast address 0x8C090EB0")!=std::string::npos,e.what());rejected=true;}
  need(rejected,"Missing helper must reject");same(model.c,c);need(expected==rt.main_ram,"Reset prefix whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
  if(capture){save_ram(rt.main_ram,out+"/"+stem+"-actual-ram.bin");save_state(c,out+"/"+stem+"-actual-state.txt");save_timers(rt.tmu,start,tocr,out+"/"+stem+"-actual-timers.txt");}else ++controls;
 };
 for(unsigned connected:{0u,1u}){
  if(connected)global_setup_register(rt);
  for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
   rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=value;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;check(c,connected,false);
  }
  rt.main_ram=ram;load_timers(rt,argv[2]);check(initial,connected,true);
 }
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"parent_prefix_cycles\":4,\"connected_prefix_cycles\":10,\"missing_target\":\"0x8C09A6B0\",\"whole_RAM_CPU_TMU_verified\":true,\"parent_full_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original global reset prefix: "<<controls<<" controls;captured4/10cycle boundaries;whole RAM CPU TMU;missing09A6B0;parent return unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
