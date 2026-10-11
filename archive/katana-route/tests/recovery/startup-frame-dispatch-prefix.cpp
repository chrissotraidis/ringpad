// Original090F2A first real flag setter and input updater; later state is observed.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void frame_dispatch_register(DCRuntime&);}
struct FramePrefix : RequestModel {
 void first_entry(){push(c.r[14]);push(c.pr);c.r[3]=w(0x8c090ff4);need(c.r[3]==0x8c052700,"Unreviewed flag target");c.pr=0x8c090f34;c.pc=c.r[3];cycles+=6;}
 void flag(){c.r[2]=w(0x8c052780);c.r[3]=1;c.pc=c.pr;wb(c.r[2],c.r[3]);cycles+=5;}
 void input_entry(){c.r[1]=w(0x8c090ff8);need(c.r[1]==0x8c052944,"Unreviewed input updater");c.pr=0x8c090f3a;c.pc=c.r[1];cycles+=4;}
};
struct FramePrediction {SH4Context c;std::vector<std::uint8_t> ram;std::array<SH4TMUChannel,3> timers;unsigned cycles;};
static RecompiledFunction real_flag,real_input;
static std::function<void(unsigned,SH4Context&)> observe;
static void watched_flag(SH4Context&c,DCRuntime&rt){observe(0,c);real_flag(c,rt);observe(1,c);}
static void watched_input(SH4Context&c,DCRuntime&rt){observe(2,c);real_input(c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: frame-dispatch RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);frame_dispatch_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];unsigned controls=0;
 real_flag=rt.targets.at(0x8c052700);real_input=rt.targets.at(0x8c052944);rt.register_target(0x8c052700,watched_flag);rt.register_target(0x8c052944,watched_input);
 auto check=[&](SH4Context c,bool capture){auto expected=rt.main_ram;FramePrefix model{{c,expected}};std::vector<FramePrediction> predictions;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
  auto predict=[&]{auto timers=rt.tmu;tick(timers,start,model.cycles);predictions.push_back({model.c,expected,timers,model.cycles});};model.first_entry();predict();model.flag();predict();model.input_entry();predict();need(predictions[0].cycles==6&&predictions[1].cycles==11&&predictions[2].cycles==15,"Prospective frame timing differs");
  if(capture)for(unsigned i=0;i<3;++i){const auto&p=predictions[i];const auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(p.timers,start,tocr,stem+"-timers.txt");}
  unsigned seen=0;observe=[&](unsigned phase,SH4Context&actual){need(phase==seen,"Unexpected original frame boundary");const auto&p=predictions.at(phase);same(p.c,actual);need(p.ram==rt.main_ram,"Frame prefix RAM differs");for(unsigned i=0;i<3;++i)timer_same(p.timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Frame timer controls differ");++seen;};
  std::string terminal;try{call_recompiled(c,rt,0x8c090f2a);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}observe={};need(seen==3,"Frame boundaries incomplete");need(terminal.find("No recompiled/native target registered for Dreamcast address")!=std::string::npos,"Unexpected downstream outcome");
  if(capture){save_ram(rt.main_ram,out+"/frame-observed-ram.bin");save_state(c,out+"/frame-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/frame-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';std::cout<<"Observed later frame state: "<<terminal<<'\n';}else ++controls;
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effect");
 };
 for(unsigned value:{0u,1u,0x7fu,0xffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[14]=0x11223300|value;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;rt.main_ram[word(ram,0x8c052780)&0xffffff]=value;check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);check(initial,true);rt.register_target(0x8c052700,real_flag);rt.register_target(0x8c052944,real_input);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"prospective_connected_cycles\":[6,11,15],\"whole_RAM_CPU_TMU_verified\":true,\"real_flag_return_verified\":true,\"input_complete_return_verified\":false,\"frame_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original frame dispatch prefix: "<<controls<<" controls;6/11/15cycle whole RAM CPU TMU boundaries;real input updater delegated;later frame unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
