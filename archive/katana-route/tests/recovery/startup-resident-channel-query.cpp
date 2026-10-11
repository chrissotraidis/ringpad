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



struct QueryModel:RequestModel{
 void query(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=16;r[14]=r[4];r[3]=r[14]+16;r[2]=w(0x8c66c438);wb(r[15],r[5]);put(m,r[15]+4,r[6]);put(m,r[15]+8,r[7]);r[4]=sb(r[15]);r[4]<<=2;put(m,r[15]+12,r[4]);r[4]+=r[3];c.pr=0x8c66c39e;r[4]=w(r[4]);need(r[2]==0x8c66e930,"Original getter target differs");cycles+=18;
 r[0]=w(r[4]+4);c.pc=c.pr;cycles+=3;
 r[4]=w(0x8c66c43c);r[3]=w(0x8c66c440);put(m,r[15],r[0]);r[6]=w(r[15]+4);r[7]=w(r[15]+8);c.pr=0x8c66c3ac;r[5]=w(r[15]);c.pc=0x8c66c39e;cycles+=8;need(r[3]==0x8c65e640&&r[4]==0x52535053,"Original query target differs");
 }
};
namespace dcrecomp_generated {void resident_channel_query_register(DCRuntime&);void resident_resource_id_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c660a20,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned channel:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66c37c),"Query adapter already registered");resident_channel_query_register(rt);resident_resource_id_register(rt);need(rt.targets.size()==count+6&&rt.targets.contains(0x8c66e930),"Original entries or retained getter differ");need(!rt.targets.contains(0x8c65e640),"Next target already registered");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;c.r[5]=channel;auto expected_ram=ram;QueryModel m{{c,expected_ram}};m.query();need(m.cycles==29&&m.c.r[5]==15-channel,"Resource ID or timing differs");auto expected=m.c;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  bool missing=false;try{call_recompiled(c,rt,0x8c66c37c);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong helper boundary error");missing=true;}need(missing,"Missing helper boundary absent");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(channel==0&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"cycles":29,"next_target":"8C65E640","new_instructions":47,"retained_resource_getter_executed":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original query adapter through retained getter:8controls,29cycles,to65E640\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
