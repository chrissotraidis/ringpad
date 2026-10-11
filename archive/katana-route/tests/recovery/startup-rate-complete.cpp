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

namespace dcrecomp_generated {void resident_rate_prefix_register(DCRuntime&);void rate_math_wrapper_register(DCRuntime&);void rate_math_classify_register(DCRuntime&);void rate_polynomial_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];
 const auto rn_ram=read((out+"/expected-0-ram.bin").c_str()),rz_ram=read((out+"/expected-1-ram.bin").c_str());
 unsigned controls=0;const int saved_rounding=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved_rounding};
 for(unsigned host_zero:{0u,1u})for(unsigned setter:{0u,1u})for(unsigned guest_zero:{0u,1u})for(unsigned mode=0;mode<3;++mode){
  const int host_mode=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host_mode)==0,"Host setup failed");
  auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);
  for(unsigned entry:{0x8c65eb00u,0x8c11152cu,0x8c1118dcu,0x8c111e38u})need(!rt.targets.contains(entry),"Unexpected preexisting generated entry");
  need(rt.targets.contains(0x8c1117d8),"Retained original range reducer absent");auto count=rt.targets.size();
  resident_rate_prefix_register(rt);rate_math_wrapper_register(rt);rate_math_classify_register(rt);rate_polynomial_register(rt);need(rt.targets.size()==count+45,"Combined generated entries differ");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);rt.fpu_aot_mode=mode;
  auto c=initial;c.pc=0x8c65eb00;c.fpscr=0x40000|guest_zero;if(setter)dc_write_fpscr(c,c.fpscr);
  const auto expected=state((out+"/expected-"+std::to_string(guest_zero)+"-state.txt").c_str());auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,284);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65eb00);same(expected,c);need((guest_zero?rz_ram:rn_ram)==rt.main_ram,"Complete rate RAM differs");
  for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");
  need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host_mode,"Guest return leaked host rounding");
  if(!host_zero&&!setter&&guest_zero&&mode==0){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}
  ++controls;
 }
 need(controls==24,"Complete rate coverage differs");std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":24,\"cycles\":284,\"return_pc\":\"8C64890C\",\"output_halfwords\":[\"1CA6\",\"F32D\"],\"original_rate_return\":true,\"candidate_only\":true,\"G1_resolved\":false}\n";std::cout<<"PASS original65EB00 complete rate calculation:24 controls,284cycles,two original output stores\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
