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



namespace dcrecomp_generated {void resident_state_request_register(DCRuntime&);void resident_state_query_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c648548,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned status:{1u,0u,128u,255u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c648240),"State query already registered");resident_state_request_register(rt);resident_state_query_register(rt);
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;rt.main_ram[(c.r[4]&0xffffff)+1]=status;auto expected_ram=rt.main_ram;auto expected=c;auto&er=expected.r;er[0]=static_cast<int8_t>(expected_ram[(er[4]&0xffffff)+1]);expected.pc=expected.pr;const auto query_return=expected;need(expected.pc==0x8c648556,"Query return differs");
  er[2]=word(expected_ram,er[15]);er[4]=er[0];er[0]=114;expected_ram[(er[14]+114)&0xffffff]=er[2]&255;er[0]=er[4];expected.sr&=~1u;er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pc==0x8ced13ea,"State request return differs");
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,18);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c648240);same(query_return,c);call_recompiled(c,rt,0x8c648556);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(status==1&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"cycles":18,"return_target":"8CED13EA","new_instructions":2,"signed_byte_getter":true,"states_3_and_4_excluded":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original reset forwarding leaf:16controls,18cycles,returnedED13EA\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
