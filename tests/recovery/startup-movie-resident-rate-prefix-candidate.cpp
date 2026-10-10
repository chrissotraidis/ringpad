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

namespace dcrecomp_generated { void resident_rate_prefix_register(DCRuntime&); }
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);
 const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];
 unsigned controls=0;
 const int saved_rounding=std::fegetround();
 struct Restore {int mode;~Restore(){std::fesetround(mode);}} restore{saved_rounding};
 // Both guest modes must win over both incoming host modes, with direct
 // context assignment and the FPSCR writer exercised separately.
 for(unsigned host_zero:{0u,1u})for(unsigned setter:{0u,1u})for(unsigned guest_zero:{0u,1u}){

  need(std::fesetround(host_zero?FE_TOWARDZERO:FE_TONEAREST)==0,"Cannot establish host control");
  auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
  load_embedded_elf_image(rt);registers(rt);
  need(!rt.targets.contains(0x8c65eb00)&&!rt.targets.contains(0x8c11152c),"Resident inventory differs");
  auto count=rt.targets.size();resident_rate_prefix_register(rt);need(rt.targets.size()==count+2,"Resident entries differ");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);
  auto c=initial;c.pc=0x8c65eb00;c.fpscr=0x40000|guest_zero;
  if(setter)dc_write_fpscr(c,c.fpscr);
  auto expected=state((out+(guest_zero?"/zero-state.txt":"/nearest-state.txt")).c_str());expected.fpscr=c.fpscr;
  auto expected_ram=ram;auto sp=c.r[15];put(expected_ram,sp-4,c.fr_bits[12]);put(expected_ram,sp-8,c.pr);sp-=24;
  for(unsigned i=0;i<4;++i)put(expected_ram,sp+12-i*4,c.r[4+i]);
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,25);const auto vram=rt.vram;
  bool stopped=false;try{call_recompiled(c,rt,0x8c65eb00);}catch(const std::runtime_error&e){
   need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong resident failure");stopped=true;}
  need(stopped&&c.r[3]==0x8c11152c&&c.pr==0x8c65eb30,"Missing next call not observed");
  same(expected,c);need(expected_ram==rt.main_ram,"Rate prefix RAM differs");
  for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
  need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");
  need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");

  if(!host_zero&&!setter&&guest_zero){
   save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");
   write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());
  }
  ++controls;
 }
 need(controls==8,"Diagnostic coverage differs");
 std::ofstream(out+"/result.json")<<"{\"diagnostic_completed\":true,\"controls\":8,\"guest_zero_defects_reproduced\":0,\"native_guest_prefix_executed\":true,\"cycles\":25,\"next_missing_target\":\"8C11152C\",\"runtime_corrected_for_captured_prefix\":true,\"G1_resolved\":false}\n";
 std::cout<<"PASS candidate original resident rate prefix: eight controls; both guest rounding modes verified; next missing11152C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}





