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



namespace dcrecomp_generated {void resident_object_reset_register(DCRuntime&);void resident_handler_init_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c6481d0,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned nesting:{1u,2u})for(unsigned dirty:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c6481a8),"Object setup already registered");resident_object_reset_register(rt);need(rt.targets.size()==count+17,"Object setup entries differ");resident_handler_init_register(rt);need(rt.targets.contains(0x8c65eace),"Retained critical exit absent");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;put(rt.main_ram,0x8c67ff20,nesting);if(dirty)for(unsigned i=0;i<2;++i){auto child=word(rt.main_ram,c.r[14]+24+4*i);for(unsigned o:{12u,16u,20u,24u})put(rt.main_ram,child+o,0xa5a5a5a5);}auto expected_ram=rt.main_ram;auto expected=c;auto&er=expected.r;
  er[3]=word(expected_ram,er[14]+8);need(er[3]==0,"Nonempty auxiliary object outside captured path");expected.sr|=1u;er[12]=0;er[0]=expected_ram[(er[14]&0xffffff)+2];need(er[0]==0,"Captured object type differs");expected.sr&=~1u;put(expected_ram,er[14]+20,er[12]);er[13]=er[12];unsigned cycles=10;
  while(true){er[0]=static_cast<int8_t>(expected_ram[(er[14]&0xffffff)+3]);expected.sr=(expected.sr&~1u)|unsigned(static_cast<int32_t>(er[13])>=static_cast<int32_t>(er[0]));cycles+=3;if(expected.sr&1)break;need(er[13]<2,"Unexpected child count");er[3]=er[14]+24;er[11]=(er[13]<<2)+er[3];er[11]=word(expected_ram,er[11]);er[2]=word(expected_ram,er[11]);er[3]=word(expected_ram,er[2]+20);need(er[3]==0x8c662194,"Unexpected child initializer");expected.pr=0x8c648224;er[4]=er[11];cycles+=11;
   er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=word(expected_ram,er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);put(expected_ram,er[4]+24,er[5]);cycles+=8;++er[13];cycles+=1;
  }
  er[0]=er[12];expected_ram[(er[14]&0xffffff)+1]=er[0];er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[3]=word(expected_ram,0x8c648364);need(er[3]==0x8c65eace,"Critical exit target differs");for(unsigned i=11;i<=14;++i){er[i]=word(expected_ram,er[15]);er[15]+=4;}cycles+=11;
  er[4]=word(expected_ram,0x8c65eaf8);er[3]=word(expected_ram,er[4])-1;er[2]=er[3];expected.sr=(expected.sr&~1u)|unsigned(er[2]==0);put(expected_ram,er[4],er[3]);cycles+=7;
  if(er[2]==0){er[2]=word(expected_ram,0x8c65eafc);er[1]=static_cast<int16_t>(word(expected_ram,0x8c65eaf4));er[0]=word(expected_ram,er[2]);er[2]=expected.sr;er[0]=((er[0]&15)<<4)|(er[2]&er[1]);er[2]&=er[1];expected.sr=er[0];cycles+=14;}expected.pc=expected.pr;cycles+=3;need(cycles==(nesting==1?94:80),"Reset tail cycle breakdown differs");
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c6481d0);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(nesting==1&&!dirty&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"captured_cycles":94,"nested_cycles":80,"return_target":"8C648178","new_instructions":0,"retained_child_initializers":2,"critical_exit_verified":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original reset forwarding leaf:16controls,94/80cycles,returned648178\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
