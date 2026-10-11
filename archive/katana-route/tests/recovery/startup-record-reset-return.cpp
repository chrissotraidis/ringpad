// Expectations from original SH-4 reviews, including signed word queries.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void record_reset_register(DCRuntime&);void record_query_register(DCRuntime&);}
struct RecordResetModel : RequestModel {
 void busy(){auto&r=c.r;r[3]=w(0x8c04b768);r[0]=w(r[3]);t(r[0]==0);r[0]=c.sr&1;c.pc=c.pr;r[0]^=1;cycles+=7;}
 unsigned sw(unsigned a){std::int16_t x;std::memcpy(&x,m.data()+(a&0xffffff),2);return unsigned(int(x));}
 void code(){auto&r=c.r;r[2]=w(0x8c05157c);r[1]=w(0x8c051580);r[0]=sw(r[2]);r[3]=sw(r[1]);r[0]<<=8;c.pc=c.pr;r[0]|=r[3];cycles+=8;}
 void query(){
  auto&r=c.r;r[3]=w(0x8c10a518);push(c.pr);r[2]=w(r[3]);t(r[2]==0);cycles+=5;
  bool matched=false;
  if(!(c.sr&1)){
   r[2]=w(0x8c10a51c);c.pr=0x8c10a464;cycles+=4;busy();t(r[0]==1);cycles+=2;
   if(!(c.sr&1)){
    r[2]=w(0x8c10a520);r[0]=w(r[2]);t(r[0]==5);cycles+=4;
    if(c.sr&1){
     for(unsigned n=0;n<4;++n){
      r[1]=w(0x8c10a524);c.pr=0x8c10a476+20*n;cycles+=4;code();
      r[3]=w(0x8c10a528);r[2]=39+n;r[0]&=65535;r[2]<<=8;r[0]&=r[3];t(r[2]==r[0]);cycles+=7;
      if(c.sr&1){matched=true;break;}
     }
    }
   }
  }
  if(matched){c.pr=pop();c.pc=c.pr;r[0]=1;cycles+=4;}
  else{r[0]=0;c.pr=pop();c.pc=c.pr;cycles+=5;}
 }
 void update(){
  auto&r=c.r;push(c.pr);r[15]-=4;c.pr=0x8c10abc8;put(m,r[15],r[4]);cycles+=5;query();t(r[0]==0);cycles+=2;
  if(!(c.sr&1)){r[0]=6;r[3]=w(r[15]);r[0]<<=8;r[0]+=3;r[2]=0;wb(r[3]+r[0],r[2]);r[15]+=4;c.pr=pop();c.pc=c.pr;r[0]=1;cycles+=11;}
  else{r[0]=0;r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=6;}
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-reset RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);record_reset_register(rt);record_query_register(rt);
 for(unsigned a:{0x8c10abc0u,0x8c10a454u,0x8c04b6ceu,0x8c051540u})need(rt.targets.contains(a),"Missing original state-1 binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){const auto beforeRecord=rt.main_ram[(c.r[4]+1539)&0xffffff];auto predicted=rt.main_ram;RecordResetModel model{{c,predicted}};model.update();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/reset-expected-ram.bin");save_state(model.c,stem+"/reset-expected-state.txt");save_timers(timers,start,tocr,stem+"/reset-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"R0\":"<<model.c.r[0]<<"}\n";}
  call_recompiled(c,rt,0x8c10abc0);
  if(capture){save_state(c,stem+"/reset-actual-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/reset-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"Record reset RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Update timer controls differ");
  need(rt.main_ram[(c.r[4]+1539)&0xffffff]==(c.r[0]?0:beforeRecord),"Original record transition differs");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned controls=0;
 for(unsigned gate:{0u,1u})for(unsigned busy:{0u,1u,0x80000000u})for(unsigned mode:{5u,24u})for(unsigned high:{0x26u,0x27u,0x28u,0x29u,0x2au,0x2bu,0xff27u})for(unsigned low:{0x34u,0x8000u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);put(rt.main_ram,0x8c790688,gate);put(rt.main_ram,0x8c74d32c,busy);put(rt.main_ram,0x8c7aa75c,mode);
  std::uint16_t hi=high,lo=low;std::memcpy(rt.main_ram.data()+0x74d43a,&hi,2);std::memcpy(rt.main_ram.data()+0x74d43c,&lo,2);
  auto c=initial;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.sr=(c.sr&~0x20000001u)|((controls&1)?0x20000001u:0);c.fpscr=(c.fpscr&~(1u<<21))|((controls&2)?(1u<<21):0);
  const unsigned masked=((unsigned(int(std::int16_t(hi)))<<8)|unsigned(int(std::int16_t(lo))))&0xff00;
  unsigned expected=!gate?23:(busy?36:(mode!=5?40:((masked>=0x2700&&masked<=0x2a00)?63+19*((masked>>8)-39):116)));
  need(check(c,false)==expected,"Control path cycles differ");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);need(word(ram,0x8c790688)==0,"Captured gate changed");const auto cycles=check(initial,true);need(cycles==23,"Captured update timing differs");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"real_query_helpers\":true,\"all_query_control_flow_branches_tested\":true,\"device_clocks_enabled\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record reset return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" gate/busy/mode/signed-code/frame/bank/FPU controls; original query helpers; normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
