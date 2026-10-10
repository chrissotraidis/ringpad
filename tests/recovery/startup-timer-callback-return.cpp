// Timer sampling follows the pinned emitter's block-entry cycle accounting.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void timer_callback_register(DCRuntime&);void timer_tail_register(DCRuntime&);}

struct TimerCallbackModel : RequestModel {
 unsigned sample=0;
 void provider(){ //608286, original optional callback remains absent here
  auto&r=c.r;push(r[14]);r[3]=0;push(c.pr);r[2]=w(0x8c608380);put(m,r[2],r[3]);r[0]=w(0x8c608384);r[1]=1;put(m,r[0],r[1]);r[3]=w(0x8c608388);r[14]=w(r[3]);t(r[14]==0);cycles+=12;
  need(c.sr&1,"Non-null provider callback needs a separate original-source model");c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=4;
 }
 void tail(){ //0108EA, zero gate or non-calling dispatch modes only
  auto&r=c.r;push(c.pr);r[1]=w(0x8c0109a4);r[2]=w(r[1]);t(r[2]==0);cycles+=5;
  if(!(c.sr&1)){
   r[2]=w(0x8c0109a8);r[0]=w(r[2]);t(r[0]==0);cycles+=4;
   if(!(c.sr&1)){for(unsigned mode=1;mode<=4;++mode){t(r[0]==mode);cycles+=2;need(!(c.sr&1),"Calling dispatch modes require real callee models");}cycles+=2;}
   r[3]=w(0x8c0109a4);r[1]=0;put(m,r[3],r[1]);cycles+=3;
  }
  c.pr=pop();c.pc=c.pr;cycles+=4;
 }
 void callback(unsigned counterAfterFirstBlock){
  auto&r=c.r;r[3]=w(0x8c06deb4);need(r[3]==0xffd80018,"Unexpected MMIO register");push(c.pr);r[1]=w(0x8c06ded8);r[2]=w(0x8c06ded4);
  // The compiled first block ticks all eight cycles before this hardware read.
  r[4]=counterAfterFirstBlock;sample=r[4];c.pr=0x8c06de86;put(m,r[2],r[4]);cycles+=8;provider();
  r[3]=w(0x8c06dedc);c.pr=pop();cycles+=4;tail();
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: timer-callback RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);timer_callback_register(rt);timer_tail_register(rt);
 for(unsigned a:{0x8c06de78u,0x8c608286u,0x8c0108eau})need(rt.targets.contains(a),"Missing original timer binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;auto sampledTimers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(sampledTimers,start,8);TimerCallbackModel model{{c,predicted}};model.callback(sampledTimers[1].tcnt);auto timers=rt.tmu;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/callback-expected-ram.bin");save_state(model.c,stem+"/callback-expected-state.txt");save_timers(timers,start,tocr,stem+"/callback-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"sample_at_block_cycles\":8,\"sampled_TCNT1\":"<<model.sample<<"}\n";}
  call_recompiled(c,rt,0x8c06de78);
  if(capture){save_state(c,stem+"/callback-actual-state.txt");save_timers(rt.tmu,start,tocr,stem+"/callback-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"Timer callback RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Callback timer controls differ");
  need(c.r[4]==sampledTimers[1].tcnt&&word(rt.main_ram,0x8c790684)==c.r[4],"Independent timer sample/store differs");need(word(rt.main_ram,0x8c893a50)==0&&word(rt.main_ram,0x8c893a58)==1&&word(rt.main_ram,0x8c6b00d8)==0,"Original provider/dispatch flag contract differs");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned controls=0;for(unsigned accumulator=0;accumulator<16;++accumulator)for(unsigned start:{0u,3u})for(unsigned mode:{0u,1u,2u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);rt.tmu_tstr=start;rt.tmu[1].tcr=0;rt.tmu[1].tcnt=1000;rt.tmu[1].cycle_accumulator=accumulator;
  put(rt.main_ram,0x8c893a50,0x12345678);put(rt.main_ram,0x8c893a58,0);put(rt.main_ram,0x8c6b00d8,mode?1:0);put(rt.main_ram,0x8c6b00b4,mode==2?5:0);
  auto c=initial;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.sr=(c.sr&~0x20000001u)|((controls&1)?0x20000001u:0);c.fpscr=(c.fpscr&~(1u<<21))|((controls&2)?(1u<<21):0);c.r[14]=0x12345678;
  need(check(c,false)==(mode==0?37u:(mode==1?44u:54u)),"Control path cycles differ");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true);need(cycles==37,"Captured callback timing differs");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"MMIO_sample_at_block_cycle\":8,\"exact_hardware_issue_timing_verified\":false,\"cold_callbacks_verified\":false,\"device_clocks_enabled\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original timer callback return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" accumulator/stopped-running/dispatch/frame/bank/FPU controls; real provider and original tail; block-tick sample, cold callbacks/exact hardware timing/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
