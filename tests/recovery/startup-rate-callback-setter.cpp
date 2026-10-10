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

namespace dcrecomp_generated {void resident_channel_register(DCRuntime&);void rate_callback_setter_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c65f3de,"Wrong actual return boundary");
 unsigned controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 for(unsigned continuation:{0u,1u})for(unsigned alternate:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host_mode=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host_mode)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);
  need(!rt.targets.contains(0x8c65f3de),"Work continuation already registered");auto count=rt.targets.size();resident_channel_register(rt);need(rt.targets.size()==count+16,"Work entries differ");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;
  need(!rt.targets.contains(0x8c66b3cc)&&!rt.targets.contains(0x8c65f346),"Next helper inventory differs");auto before=rt.targets.size();rate_callback_setter_register(rt);need(rt.targets.size()==before+1,"Setter entries differ");
  if(alternate){if(continuation)c.r[12]=1;else{c.r[4]+=256;c.r[5]^=0x1234;c.r[6]+=144;}}
  auto expected_ram=rt.main_ram;auto expected=c;
  const unsigned offset=static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(word(expected_ram,0x8c66b4fc)&65535)));
  need(offset==136,"Original field offset differs");
  put(expected_ram,c.r[4]+offset,c.r[5]);put(expected_ram,c.r[4]+offset+4,c.r[6]);expected.r[0]=offset+4;expected.pc=expected.pr;
  unsigned cycles=6;
  if(continuation){
   auto&r=expected.r;r[3]=word(expected_ram,r[14]+4);need(r[3]!=0&&r[13]==0&&r[12]>0&&r[12]<=2,"Only reached nonempty channel path modeled");expected.sr&=~1u;
   r[3]=word(expected_ram,r[15]);r[0]=r[12];r[5]=r[13];expected.sr|=1u;put(expected_ram,r[14]+8,r[3]);expected_ram[(r[14]+2)&0xffffff]=r[0];r[4]=r[13];cycles+=11;
   do{r[0]=r[4];r[3]=r[14];r[2]=word(expected_ram,r[9]+r[0]);r[3]+=12;++r[5];r[3]+=r[4];expected.sr=(expected.sr&~1u)|unsigned(int32_t(r[5])>=int32_t(r[12]));put(expected_ram,r[3],r[2]);r[4]+=4;cycles+=10;}while(!(expected.sr&1));
   r[0]=r[13];expected_ram[(r[14]+1)&0xffffff]=r[0];expected.pr=0x8c65f420;r[4]=r[14];expected.pc=0x8c65f418;cycles+=5;
  }
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c66b3cc);
  if(continuation){bool missing=false;try{call_recompiled(c,rt,0x8c65f3ea);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");missing=true;}need(missing,"Next channel helper not reached");}
  same(expected,c);need(expected_ram==rt.main_ram,"Work return RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host_mode,"Host mode leaked");
  if(!alternate&&!host_zero&&guest_zero){const auto stem=out+(continuation?"/observed":"/setter-return");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");write_observed_capture(stem+"-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Setter and caller coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"setter_controls":8,"caller_controls":8,"setter_cycles":6,"captured_combined_cycles":42,"alternate_combined_cycles":32,"next_target":"8C65F346","new_original_instructions":5,"new_original_bodies":1,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original callback setter and caller:16 controls,6/32/42cycles,to65F346\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
