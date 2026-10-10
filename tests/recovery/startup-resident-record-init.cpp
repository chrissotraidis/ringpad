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




namespace dcrecomp_generated {void resident_record_init_register(DCRuntime&);void resident_second_constructor_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c660a0c,"Wrong initializer input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned phase:{0u,1u,2u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c6608bc),"Initializer already present");resident_record_init_register(rt);resident_second_constructor_register(rt);need(!rt.targets.contains(0x8c6618d6),"Next helper already present");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;const unsigned object=c.r[4];need(object==c.r[14],"Actual object differs");if(phase==1)std::fill_n(rt.main_ram.begin()+(object&0xffffff),156,0xa5);auto expected_ram=rt.main_ram;auto expected=c;auto&er=expected.r;
  for(unsigned off:{1u,4u,5u,6u,7u})expected_ram[(object+off)&0xffffff]=0;
  for(unsigned off:{52u,56u,60u,80u,108u,124u,128u,132u,136u,140u,144u,152u})put(expected_ram,object+off,0);
  put(expected_ram,object+8,16);put(expected_ram,object+12,word(ram,0x8c6609b0));put(expected_ram,object+72,0x2000);put(expected_ram,object+76,0x2000);put(expected_ram,object+120,0xffffffff);put(expected_ram,object+148,word(ram,0x8c6609b4));er[0]=148;er[2]=16;er[3]=word(ram,0x8c6609b4);er[5]=0;er[6]=0x2000;expected.pc=expected.pr;const auto init_return=expected;unsigned cycles=46;
  if(phase==2){er[0]=static_cast<int8_t>(expected_ram[(er[14]+3)&0xffffff]);er[5]=er[0]&255;expected.pr=0x8c660a18;er[4]=er[14];expected.pc=0x8c660a10;cycles+=5;need(er[5]==2,"Captured channel count changed");}
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;call_recompiled(c,rt,0x8c6608bc);same(init_return,c);
  if(phase==2){bool missing=false;try{call_recompiled(c,rt,0x8c660a10);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next helper error");missing=true;}need(missing,"Next helper boundary absent");}
  need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Initializer/caller RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(phase==2&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==12,"Initializer coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":12,"initializer_cycles":46,"combined_cycles":51,"stores":23,"next_target":"8C6618D6","new_instructions":45,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original record initializer and caller:12controls,46/51cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
