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


namespace dcrecomp_generated {void resident_channel_count_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66d080,"Wrong service helper input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned channels:{1u,2u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66f2ec),"Channel helper already present");resident_channel_count_register(rt);need(rt.targets.size()==count+16,"Entry count differs");need(!rt.targets.contains(0x8c66f0b4),"Dependency map differs");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;

  c.r[5]=channels;auto expected=c;auto expected_ram=rt.main_ram;auto&er=expected.r;
  er[15]-=4;put(expected_ram,er[15],er[14]);er[7]=0;er[15]-=4;put(expected_ram,er[15],er[13]);er[15]-=4;put(expected_ram,er[15],er[12]);er[15]-=4;put(expected_ram,er[15],er[11]);er[3]=word(expected_ram,0x8c66f390);er[15]-=4;put(expected_ram,er[15],er[10]);er[10]=er[4];er[15]-=4;put(expected_ram,er[15],er[9]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[4]=word(expected_ram,er[3]);need(er[4]&&er[10],"Reached descriptor/global absent");expected.sr&=~1u;er[13]=0;er[2]=1;er[12]=static_cast<int8_t>(er[5]);need(er[12]>=1&&er[12]<=2,"Reached active count invalid");er[1]=2;expected.sr&=~1u;
  er[0]=static_cast<int8_t>(expected_ram[(er[10]+1)&0xffffff]);er[3]=44;er[2]=0xa8;expected.sr|=1;er[6]=er[0];er[11]=er[6];expected.macl=44*er[11];er[2]+=er[4];er[11]=expected.macl+er[2];er[0]=static_cast<int8_t>(expected_ram[(er[11]+4)&0xffffff]);need(er[0]>=er[12],"Active count exceeds configured count");expected.sr|=1;er[0]=er[5];expected_ram[(er[11]+5)&0xffffff]=er[0];er[14]=er[7];er[9]=word(expected_ram,0x8c66f3a0);er[3]=er[11];er[6]=er[14];er[3]+=28;er[5]=er[14];er[6]=(er[6]<<2)+er[3];er[6]=word(expected_ram,er[6]);er[7]=0;expected.pr=0x8c66f368;er[4]=er[10];expected.pc=0x8c66f354;const unsigned cycles=55;need(er[9]==0x8c66f0b4,"Channel callback differs");
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  bool missing=false;try{call_recompiled(c,rt,0x8c66f2ec);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong helper boundary error");missing=true;}need(missing,"Missing helper boundary not reached");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel creation RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(channels==2&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Channel-prefix coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"cycles":55,"next_target":"8C66F0B4","active_channel_counts":[1,2],"new_instructions":80,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original active channel count prefix:8controls,55cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
