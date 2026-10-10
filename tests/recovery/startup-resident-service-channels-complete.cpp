// One uninterrupted original caller-to-buffer prefix, compared with
// the independently checked continuation chain. No helper interception.
// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"
#include <cfenv>

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}

namespace dcrecomp_generated {
void resident_pool_register(DCRuntime&);
void resident_channel_pair_register(DCRuntime&);
void resident_channel_count_register(DCRuntime&);
void resident_buffer_bind_register(DCRuntime&);
void resident_sound_word_register(DCRuntime&);
void resident_constructor_tail_register(DCRuntime&);
void resident_second_constructor_register(DCRuntime&);
void resident_second_pool_register(DCRuntime&);
void resident_service_constructor_register(DCRuntime&);
void resident_service_pool_register(DCRuntime&);
void resident_channel_resource_register(DCRuntime&);
void resident_resource_id_register(DCRuntime&);
}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);need(initial.pc==0x8c647f74,"Wrong original caller boundary");
 const auto device=read_sq(argv[5]);const std::string out=argv[9];const auto expected=state((out+"/expected-state.txt").c_str());const auto expected_ram=read((out+"/expected-ram.bin").c_str());
 need(initial.fpscr==0x40001&&expected.pc==0x8c66f354,"Captured constructor boundary differs");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned host_zero:{0u,1u})for(unsigned mode:{0u,1u,2u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();
  for(unsigned address:{0x8c647f74u,0x8c65f214u,0x8c66093eu,0x8c660916u,0x8c66b91cu,0x8c66b8f6u,0x8c66e902u,0x8c66e930u,0x8c65e470u})need(!rt.targets.contains(address),"Unexpected duplicate original entry");
  resident_pool_register(rt);resident_constructor_tail_register(rt);resident_second_constructor_register(rt);resident_second_pool_register(rt);resident_service_constructor_register(rt);resident_service_pool_register(rt);resident_channel_resource_register(rt);resident_resource_id_register(rt);resident_buffer_bind_register(rt);resident_sound_word_register(rt);resident_channel_pair_register(rt);resident_channel_count_register(rt);
  need(rt.targets.size()==count+254,"Combined original entries differ");need(rt.targets.contains(0x8c6607a0),"Retained interrupt helper absent");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);rt.fpu_aot_mode=mode;auto c=initial;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,4060);const auto vram=rt.vram;
  bool missing=false;try{call_recompiled(c,rt,0x8c647f74);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong buffer boundary");missing=true;}need(missing,"Next service helper unexpectedly present");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Whole constructor RAM differs from verified chain");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==read((out+"/expected-aica.bin").c_str())&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!host_zero&&!mode){save_ram(rt.aica_ram,out+"/observed-aica.bin");save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==6,"Integrated constructor-prefix coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":6,"cycles":4060,"start_pc":"8C647F74","next_target":"8C66F0B4","single_continuous_prefix":true,"whole_constructor_return":false,"helper_interception":false,"guest_rounding":"toward-zero","host_rounding_modes":2,"fpu_modes":3,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS integrated original constructor prefix:6controls,4060cycles,to66F0B4\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
