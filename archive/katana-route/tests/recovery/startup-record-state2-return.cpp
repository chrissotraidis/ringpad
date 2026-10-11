// Original17-instruction state-2 wrapper plus the real original109EFA setter.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void record_state2_register(DCRuntime&);void ready_setter_register(DCRuntime&);}
struct RecordState2Model : RequestModel {
 void wh(unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(m.data()+(a&0xffffff),&x,2);}
 void setter(){auto&r=c.r;r[0]=6;r[0]<<=8;r[0]+=16;put(m,r[4]+r[0],r[5]);r[0]-=4;r[5]=0;wh(r[4]+r[0],r[5]);r[0]+=2;c.pc=c.pr;wh(r[4]+r[0],r[5]);cycles+=11;}
 void state2(){
  auto&r=c.r;push(c.pr);r[15]-=4;r[3]=w(0x8c10aba0);r[5]=w(0x8c10aba4);c.pr=0x8c10ab5c;put(m,r[15],r[4]);cycles+=7;setter();
  r[0]=6;r[2]=w(r[15]);r[0]<<=8;r[0]+=3;r[3]=2;wb(r[2]+r[0],r[3]);r[0]=1;r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=12;
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-state2 RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);record_state2_register(rt);ready_setter_register(rt);
 for(unsigned a:{0x8c10ab50u,0x8c109efau})need(rt.targets.contains(a),"Missing original state-2 binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;RecordState2Model model{{c,predicted}};model.state2();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/state2-expected-ram.bin");save_state(model.c,stem+"/state2-expected-state.txt");save_timers(timers,start,tocr,stem+"/state2-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"R0\":"<<model.c.r[0]<<"}\n";}
  call_recompiled(c,rt,0x8c10ab50);
  if(capture){save_state(c,stem+"/state2-actual-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/state2-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"State-2 RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"State-2 timers differ");
  need(c.r[0]==1&&rt.main_ram[(c.r[4]+1539)&0xffffff]==2&&word(rt.main_ram,c.r[4]+1552)==0x8c161f34&&word(rt.main_ram,c.r[4]+1548)==0,"Original state-2 writes differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned controls=0;
 for(unsigned record=0;record<4;++record)for(unsigned bits=0;bits<4;++bits)for(unsigned alias:{0u,0x20000000u})for(unsigned pattern:{0x12345678u,0xffffffffu}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=0x8ce00000+record*0x1000+alias;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.sr=(c.sr&~0x20000001u)|((bits&1)?0x20000001u:0);c.fpscr=(c.fpscr&~(1u<<21))|((bits&2)?(1u<<21):0);
  put(rt.main_ram,c.r[4]+1548,pattern);put(rt.main_ram,c.r[4]+1552,pattern^0xdeadbeef);rt.main_ram[(c.r[4]+1539)&0xffffff]=0x87;
  need(check(c,false)==30,"State-2 control cycles differ");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true);need(cycles==30,"Captured state-2 timing differs");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"real_setter\":true,\"device_clocks_enabled\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record state2 return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" record/alias/counter/frame/bank/FPU controls; real original setter; normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
