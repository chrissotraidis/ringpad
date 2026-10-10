// Original global-value adjustment, including its asymmetric limits and wrap.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void frame_dispatch_register(DCRuntime&);void frame_adjust_register(DCRuntime&);}
struct FrameAdjust : RequestModel {
 void adjust(){auto&r=c.r;r[5]=w(0x8c09d130);r[3]=unsigned(-30);r[6]=w(r[5]);t(int(r[6])>=int(r[3]));cycles+=5;
  if(!(c.sr&1)){t(int(r[4])>=0);cycles+=2;if(!(c.sr&1)){r[3]=0;put(m,r[5],r[3]);c.pc=c.pr;r[0]=0xffffffff;cycles+=5;return;}}
  r[7]=255;t(int(r[6])>255);cycles+=4;
  if(c.sr&1){t(int(r[4])>0);cycles+=2;if(c.sr&1){put(m,r[5],r[7]);c.pc=c.pr;r[0]=1;cycles+=4;return;}}
  r[2]=w(r[5]);r[0]=0;r[2]+=r[4];put(m,r[5],r[2]);c.pc=c.pr;cycles+=7;
 }
 void counter(){auto&r=c.r;r[4]=w(0x8c090fd4);r[2]=150;r[3]=w(r[4]);t(int(r[3])>150);++r[3];put(m,r[4],r[3]);cycles+=8;if(c.sr&1){r[1]=2;put(m,r[14],r[1]);cycles+=2;}r[3]=w(0x8c091010);need(r[3]==0x8c09d14c,"Unreviewed frame-update target");c.pr=0x8c090f8a;c.pc=0x8c090f84;cycles+=4;}
};
int main(int argc,char**argv){try{
 need(argc==6,"usage: frame-adjust RAM STATE TIMERS FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);frame_dispatch_register(rt);frame_adjust_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[4]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[5];unsigned controls=0;
 auto exact=[&](const FrameAdjust&model,const SH4Context&c,const auto&expected,const auto&timers,unsigned start,unsigned tocr){same(model.c,c);need(expected==rt.main_ram,"Frame-adjust whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Frame-adjust timer controls differ");};
 for(unsigned value:{0x80000000u,unsigned(-32),unsigned(-31),unsigned(-30),unsigned(-29),0u,254u,255u,256u,0x7fffffffu})for(unsigned step:{0x80000000u,unsigned(-8),unsigned(-1),0u,1u,8u,0x7fffffffu})for(unsigned bank=0;bank<2;++bank){
  rt.main_ram=ram;load_timers(rt,argv[3]);auto c=initial;c.r[4]=step;c.pr=bank?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;const unsigned addr=bank?0xacc01000:0x8cc01000;put(rt.main_ram,0x8c09d130,addr);put(rt.main_ram,addr,value);
  auto expected=rt.main_ram;FrameAdjust model{{c,expected}};model.adjust();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);call_recompiled(c,rt,0x8c09d0d0);exact(model,c,expected,timers,start,tocr);
  const bool lower=int(value)<-30&&int(step)<0,upper=int(value)>255&&int(step)>0;need(word(rt.main_ram,addr)==(lower?0u:upper?255u:value+step)&&c.r[0]==(lower?0xffffffffu:upper?1u:0u),"Independent adjustment result differs");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[3]);auto c=initial;auto expected=ram;FrameAdjust model{{c,expected}};const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto timers=rt.tmu;
 auto save=[&](unsigned phase){const auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(model.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");};
 model.adjust();need(model.cycles==16,"Captured adjustment timing differs");tick(timers,start,16);save(0);call_recompiled(c,rt,0x8c09d0d0);exact(model,c,expected,timers,start,tocr);need(c.pc==0x8c090f70&&word(rt.main_ram,0x8c7a4d00)==8,"Captured adjustment return differs");
 model.counter();need(model.cycles==28,"Captured counter timing differs");tick(timers,start,12);save(1);std::string terminal;try{call_recompiled(c,rt,0x8c090f70);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}
 need(terminal.find("No recompiled/native target registered for Dreamcast address 0x8C09D14C")!=std::string::npos,"Unexpected downstream outcome");exact(model,c,expected,timers,start,tocr);need(word(rt.main_ram,0x8c79d83c)==1&&word(rt.main_ram,0x8c79d834)==1,"Captured mode/counter differ");
 save_ram(rt.main_ram,out+"/frame-observed-ram.bin");save_state(c,out+"/frame-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/frame-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';
 need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"adjustment_cycles\":16,\"prospective_connected_cycles\":[16,28],\"whole_RAM_CPU_TMU_verified\":true,\"mode1_counter_increment_verified\":true,\"frame_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original frame adjustment: "<<controls<<" threshold/sign/wrap controls;16/28cycle whole RAM CPU TMU boundaries;counter increments0 to1; "<<terminal<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
