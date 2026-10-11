// Original counter/table progression, including unsigned halfword wrap.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void record_counter_register(DCRuntime&);}
struct RecordCounterModel : RequestModel {
 unsigned sw(unsigned a){std::int16_t v;std::memcpy(&v,m.data()+(a&0xffffff),2);return unsigned(int(v));}
 void wh(unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(m.data()+(a&0xffffff),&x,2);}
 // Same source-derived predicate as the verified record-parent reduction.
 void predicate(){auto&r=c.r;r[3]=1538;r[2]=w(0x8c10ba04);r[0]=w(r[2]);r[3]+=r[4];r[3]=sb(r[3]);r[0]&=7;t(r[3]==r[0]);r[0]=c.sr&1;c.pc=c.pr;cycles+=13;}
 void advance(){
  auto&r=c.r;push(r[14]);push(c.pr);r[3]=w(0x8c109f20);c.pr=0x8c109ea8;r[14]=r[4];cycles+=6;predicate();t(r[0]==0);cycles+=2;
  if(!(c.sr&1)){
   r[0]=6;r[0]<<=8;r[0]+=12;r[4]=sw(r[14]+r[0]);r[0]+=4;r[3]=w(r[14]+r[0]);r[0]-=2;r[4]&=65535;r[4]<<=2;t(r[4]>>31);r[4]<<=1;r[4]+=r[3];
   r[3]=sw(r[14]+r[0]);++r[3];wh(r[14]+r[0],r[3]);r[2]=sw(r[14]+r[0]);r[3]=w(r[4]+4);r[2]&=65535;t(r[2]>=r[3]);cycles+=19;
   if(c.sr&1){
    r[0]=6;r[0]<<=8;r[0]+=14;r[5]=0;wh(r[14]+r[0],r[5]);r[0]-=2;r[4]+=8;r[3]=sw(r[14]+r[0]);++r[3];wh(r[14]+r[0],r[3]);r[2]=w(r[4]);t(r[2]==0);cycles+=13;
    if(c.sr&1){r[0]=6;r[0]<<=8;r[0]+=12;wh(r[14]+r[0],r[5]);cycles+=4;}
   }
  }
  c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=4;
 }
};
static unsigned half(const auto&m,unsigned a){std::uint16_t x;std::memcpy(&x,m.data()+(a&0xffffff),2);return x;}
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-counter RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);record_counter_register(rt);
 for(unsigned a:{0x8c109e9eu,0x8c10b906u})need(rt.targets.contains(a),"Missing original record-counter binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;RecordCounterModel model{{c,predicted}};model.advance();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/counter-expected-ram.bin");save_state(model.c,stem+"/counter-expected-state.txt");save_timers(timers,start,tocr,stem+"/counter-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"counter\":"<<half(predicted,c.r[4]+1550)<<",\"index\":"<<half(predicted,c.r[4]+1548)<<"}\n";}
  call_recompiled(c,rt,0x8c109e9e);
  if(capture){save_state(c,stem+"/counter-actual-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/counter-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"Record-counter RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Record-counter timers differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned controls=0;const unsigned cases[][2]={{0,22},{21,22},{65535,1},{65535,0},{32767,32768},{32768,0xffffffff}};
 for(unsigned index:{0u,1u,0x7fffu,0x8000u,0xffffu})for(const auto&pair:cases)for(unsigned next:{0u,0x8c161124u})for(unsigned enabled:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]+=((controls&4)?0x20000000u:0);const auto record=c.r[4];const unsigned table=0x8cd00000+((controls&8)?0x20000000u:0);
  c.r[15]=0x8cefe000;c.pr=0x8c112234;c.r[14]=0xabcdef00+controls;c.sr=(c.sr&~0x20000001u)|((controls&2)?0x20000000u:0)|(controls&1);c.fpscr=(c.fpscr&~(1u<<21))|((controls&4)?(1u<<21):0);
  put(rt.main_ram,word(ram,0x8c10ba04),0x1234567b);rt.main_ram[(record+1538)&0xffffff]=enabled?3:255;
  auto wh=[&](unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(rt.main_ram.data()+(a&0xffffff),&x,2);};
  wh(record+1548,index);wh(record+1550,pair[0]);put(rt.main_ram,record+1552,table);put(rt.main_ram,table+index*8,0x8c161124);put(rt.main_ram,table+index*8+4,pair[1]);put(rt.main_ram,table+(index+1)*8,next);
  const unsigned count=(pair[0]+1)&65535;const bool advance=enabled&&count>=pair[1];const unsigned expected=!enabled?25:(!advance?44:(next?57:61));
  need(check(c,false)==expected,"Counter path cycles differ");need(half(rt.main_ram,record+1550)==(!enabled?pair[0]:(advance?0:count)),"Counter wrap contract differs");need(half(rt.main_ram,record+1548)==(!advance?index:(next?((index+1)&65535):0)),"Index/sentinel contract differs");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);need((word(ram,word(ram,0x8c10ba04))&7)==2&&ram[(initial.r[4]+1538)&0xffffff]==0,"Captured predicate inputs differ");const auto cycles=check(initial,true);need(cycles==25&&half(rt.main_ram,initial.r[4]+1550)==0&&half(rt.main_ram,initial.r[4]+1548)==0,"Captured counter path differs");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"real_predicate\":true,\"device_clocks_enabled\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record counter return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" predicate/threshold/counter/index/sentinel/alias/frame/bank/FPU controls; real predicate; normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
