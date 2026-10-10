// One uninterrupted original channel-constructor invocation, compared with
// the independently checked continuation chain. No helper interception.
#define main work_continuation_standalone_main
#include "startup-rate-work-return.cpp"
#undef main
namespace dcrecomp_generated {
void resident_channel_register(DCRuntime&);
void resident_channel_getters_register(DCRuntime&);
void resident_rate_pool_register(DCRuntime&);
void resident_rate_prefix_register(DCRuntime&);
void rate_math_wrapper_register(DCRuntime&);
void rate_math_classify_register(DCRuntime&);
void rate_polynomial_register(DCRuntime&);
void rate_callback_setter_register(DCRuntime&);
void channel_initialize_register(DCRuntime&);
}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c65f364;
 const auto device=read_sq(argv[5]);const std::string out=argv[9];const auto expected=state((out+"/expected-state.txt").c_str());const auto expected_ram=read((out+"/expected-ram.bin").c_str());
 need(initial.fpscr==0x40001&&expected.pc==0x8c647f18,"Captured constructor boundary differs");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned host_zero:{0u,1u})for(unsigned mode:{0u,1u,2u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();
  for(unsigned address:{0x8c65f364u,0x8c6624d8u,0x8c66b0f4u,0x8c6488aeu,0x8c65eb00u,0x8c11152cu,0x8c1118dcu,0x8c111e38u,0x8c66b3ccu,0x8c65f346u})need(!rt.targets.contains(address),"Unexpected duplicate original root");
  resident_channel_register(rt);resident_channel_getters_register(rt);resident_work_pool_register(rt);resident_rate_pool_register(rt);resident_rate_prefix_register(rt);rate_math_wrapper_register(rt);rate_math_classify_register(rt);rate_polynomial_register(rt);rate_callback_setter_register(rt);channel_initialize_register(rt);
  need(rt.targets.size()==count+87,"Combined original entries differ");need(rt.targets.contains(0x8c1117d8)&&rt.targets.contains(0x8c10ef3c),"Retained reducer/clear absent");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);rt.fpu_aot_mode=mode;auto c=initial;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,1750);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65f364);same(expected,c);need(expected_ram==rt.main_ram,"Whole constructor RAM differs from verified chain");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!host_zero&&!mode){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==6,"Whole constructor coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":6,"cycles":1750,"return_pc":"8C647F18","record":"8C8A7B90","whole_constructor_call":true,"helper_interception":false,"guest_rounding":"toward-zero","host_rounding_modes":2,"fpu_modes":3,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS whole original channel constructor:6 controls,1750cycles,return647F18\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
