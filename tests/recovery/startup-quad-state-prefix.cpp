// Original641F98 and real60A748, stopping at the original missing allocator.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void quad_state_register(DCRuntime&);}
struct QuadState : RequestModel {
 auto& f(){return(c.fpscr&(1u<<21))?c.xf_bits:c.fr_bits;}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void entry(){auto&r=c.r;push(f()[15]);push(c.pr);r[15]-=4;r[0]=w(0x8c642188);put(m,r[15],r[4]);r[4]<<=2;r[3]=w(0x8c64218c);f()[15]=f()[4];c.pr=0x8c641fac;r[4]=w(r[0]+r[4]);c.pc=r[3];need(c.pc==0x8c60a748,"Unreviewed header setter");cycles+=11;}
 void header(){auto&r=c.r;push(r[14]);r[14]=w(0x8c60a860);r[3]=w(0x8c60a864);r[1]=w(0x8c60a86c);r[2]=w(0x8c60a868);r[1]&=r[4];push(r[13]);r[5]=w(r[14]);r[6]=w(r[14]+8);r[5]&=r[3];r[3]=w(0x8c60a870);r[5]|=r[1];r[1]=w(0x8c60a874);r[3]&=r[4];r[6]&=r[2];r[13]=w(r[1]);r[6]|=r[3];r[3]=sw(0x8c60a854);t(!(r[13]&r[3]));r[7]=w(r[14]+4);cycles+=21;
  bool set=!(c.sr&1);if(!set){r[0]=sw(0x8c60a856);t(!(r[0]&r[13]));cycles+=3;if(!(c.sr&1)){r[2]=w(0x8c60a878);t(!(r[4]&r[2]));cycles+=3;set=c.sr&1;}}
  if(set){r[1]=sw(0x8c60a858);r[5]|=r[1];cycles+=3;}else{r[2]=sw(0x8c60a85a);r[5]&=r[2];cycles+=2;}
  r[2]=w(0x8c60a87c);put(m,r[14],r[5]);put(m,r[14]+4,r[7]);put(m,r[14]+8,r[6]);r[3]=w(r[2]);put(m,r[3],r[4]);r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;
 }
 unsigned missing(){auto&r=c.r;r[2]=w(r[15]);t(r[2]==0);cycles+=3;unsigned target;if(c.sr&1){r[1]=w(0x8c642198);target=r[1];c.pr=0x8c641fc4;c.pc=0x8c641fbe;}else{r[3]=w(0x8c642190);target=r[3];c.pr=0x8c641fb8;c.pc=0x8c641fb2;}cycles+=4;return target;}
};
struct QuadStatePrediction{SH4Context c;std::vector<std::uint8_t> ram;std::array<SH4TMUChannel,3> timers;unsigned cycles;};
static RecompiledFunction real_header;
static std::function<void(unsigned,SH4Context&)> observe_header;
static void watched_header(SH4Context&c,DCRuntime&rt){observe_header(0,c);real_header(c,rt);observe_header(1,c);}
int main(int argc,char**argv){try{
 need(argc==6,"usage: quad-state RAM STATE TIMERS FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);quad_state_register(rt);const auto ram=read(argv[1]),flash=read(argv[4]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[5];unsigned controls=0;
 real_header=rt.targets.at(0x8c60a748);rt.register_target(0x8c60a748,watched_header);
 auto check=[&](SH4Context c,bool capture){auto expected=rt.main_ram;QuadState model{{c,expected}};std::vector<QuadStatePrediction> predictions;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto predict=[&]{auto timers=rt.tmu;tick(timers,start,model.cycles);predictions.push_back({model.c,expected,timers,model.cycles});};model.entry();predict();model.header();predict();const auto target=model.missing();predict();need(target==0x8c609c7c||target==0x8c609ba8,"Unreviewed allocator target");
  if(capture)for(unsigned i=0;i<3;++i){const auto&p=predictions[i];const auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(p.timers,start,tocr,stem+"-timers.txt");}
  auto exact=[&](unsigned phase,SH4Context&actual){const auto&p=predictions[phase];same(p.c,actual);need(p.ram==rt.main_ram,"Quad header RAM differs");for(unsigned i=0;i<3;++i)timer_same(p.timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Quad header timer controls differ");};unsigned seen=0;observe_header=[&](unsigned phase,SH4Context&actual){need(seen==phase,"Unexpected setter boundary");exact(phase,actual);++seen;};
  std::string terminal;try{call_recompiled(c,rt,0x8c641f98);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}observe_header={};need(seen==2,"Header setter boundaries incomplete");const char*missing=target==0x8c609c7c?"0x8C609C7C":"0x8C609BA8";need(terminal.find("No recompiled/native target registered for Dreamcast address "+std::string(missing))!=std::string::npos,"Unexpected allocator outcome");exact(2,c);
  if(capture){save_ram(rt.main_ram,out+"/quad-observed-ram.bin");save_state(c,out+"/quad-observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/quad-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';std::ofstream(out+"/captured-cycles.json")<<"["<<predictions[0].cycles<<","<<predictions[1].cycles<<","<<predictions[2].cycles<<"]\n";std::cout<<"Captured cycles "<<predictions[0].cycles<<'/'<<predictions[1].cycles<<'/'<<predictions[2].cycles<<"; "<<terminal<<'\n';}else ++controls;
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 };
 for(unsigned flags:{0u,0x400u,0x1000u,0x1400u})for(unsigned kind:{0u,1u})for(unsigned material:{0u,0x2100000u,0xffffffffu})for(unsigned bank:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[3]);auto c=initial;c.r[4]=kind;c.r[15]=bank?0xacefe000:0x8cefe000;c.pr=bank?0xac112234:0x8c112234;c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);put(rt.main_ram,0x8c8938ac,flags);put(rt.main_ram,0x8c677410+4*kind,material);put(rt.main_ram,0x8c894384,0xa555aa55);put(rt.main_ram,0x8c894388,0x12345678);put(rt.main_ram,0x8c89438c,0x89abcdef);check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[3]);check(initial,true);rt.register_target(0x8c60a748,real_header);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"whole_RAM_CPU_TMU_verified\":true,\"header_setter_full_return_verified\":true,\"selector_full_return_verified\":false,\"allocation_verified\":false,\"draw_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original quad state: "<<controls<<" flag/material/kind/bank controls;real header setter full return and allocator entry prefixes verified\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
