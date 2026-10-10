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

namespace dcrecomp_generated {void resident_work_pool_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66b13c,"Wrong actual return boundary");
 unsigned controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 for(unsigned alternate:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host_mode=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host_mode)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);
  need(!rt.targets.contains(0x8c66b13c),"Work continuation already registered");auto count=rt.targets.size();resident_work_pool_register(rt);need(rt.targets.size()==count+11,"Work entries differ");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;
  const unsigned record=c.r[14],sp=c.r[15];if(alternate){c.r[0]+=52;put(rt.main_ram,sp,0x8cf10000);put(rt.main_ram,sp+4,0x3000);put(rt.main_ram,sp+8,0x3030);put(rt.main_ram,sp+12,3);}
  need(c.r[0]!=0,"Only the reached successful allocation path is modeled");auto expected_ram=rt.main_ram;put(expected_ram,record+8,c.r[0]);
  for(const auto&pair:std::array<std::pair<unsigned,unsigned>,4>{{{56,12},{60,0},{64,4},{68,8}}})put(expected_ram,record+pair.first,word(expected_ram,sp+pair.second));
  const unsigned first=word(expected_ram,0x8c66b1bc),second=word(expected_ram,0x8c66b1c0);
  put(expected_ram,record+136,first);put(expected_ram,record+140,record);put(expected_ram,record+144,second);put(expected_ram,record+148,record);
  auto expected=c;expected.r[0]=record;expected.r[2]=first;expected.r[3]=second;expected.sr&=~1u;expected.pr=word(expected_ram,sp+16);expected.pc=expected.pr;expected.r[13]=word(expected_ram,sp+20);expected.r[14]=word(expected_ram,sp+24);expected.r[15]=sp+28;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,31);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c66b13c);same(expected,c);need(expected_ram==rt.main_ram,"Work return RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host_mode,"Host mode leaked");
  if(!alternate&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Work return coverage differs");std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":8,\"cycles\":31,\"stores\":9,\"return_pc\":\"8C65F3DE\",\"record\":\"8C8DA42C\",\"internal_continuation_only\":true,\"candidate_only\":true,\"G1_resolved\":false}\n";std::cout<<"PASS original work continuation:8 controls,31cycles,nine stores,return65F3DE\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
