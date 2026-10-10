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



namespace dcrecomp_generated {void resident_movie_stream_register(DCRuntime&);void resident_movie_stream_query_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c65fb5a,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned dirty:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c65fa86)&&!rt.targets.contains(0x8c6621a2),"Original stream bodies already registered");resident_movie_stream_register(rt);resident_movie_stream_query_register(rt);need(rt.targets.size()==count+17,"Stream entries differ");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;need(c.r[4]==0x8c6994f4&&c.r[5]==0&&c.r[6]==0x2f9800&&c.r[7]==0x8c8aa8f8,"Stream arguments differ");if(dirty){for(unsigned off:{4u,8u,12u,16u,20u,24u,28u,44u,48u})put(rt.main_ram,0x8c8a8010+off,0xa5a5a5a5);rt.main_ram[0x8a8011]=0xa5;rt.main_ram[0x8a8012]=0xa5;}auto expected_ram=rt.main_ram;RequestModel m{c,expected_ram};auto&er=m.c.r;m.push(er[14]);er[14]=100;m.push(er[13]);er[0]=32;m.push(er[11]);er[11]=er[7];m.push(er[10]);er[10]=0;m.push(m.c.pr);er[13]=m.w(0x8c65fb84);er[7]=er[10];m.cycles=11;
  m.c.macl=er[14]*er[7];er[3]=m.c.macl;er[3]+=er[13];er[2]=m.sb(er[3]);m.t(er[2]==0);need(m.c.sr&1,"Captured stream slot occupied");m.cycles+=7;er[0]=er[7];m.t(er[0]==32);need(!(m.c.sr&1),"Stream pool full");m.cycles+=3;
  m.c.macl=er[14]*er[7];er[3]=m.w(0x8c65fb8c);er[7]=m.c.macl;er[14]=er[7]+er[13];er[7]=1;er[0]=er[7];m.wb(er[14]+1,er[0]);er[0]=er[10];m.wb(er[14]+2,er[0]);put(expected_ram,er[14]+4,er[11]);put(expected_ram,er[14]+8,er[4]);put(expected_ram,er[14]+12,er[0]);put(expected_ram,er[14]+16,er[5]);put(expected_ram,er[14]+20,er[6]);put(expected_ram,er[14]+44,er[3]);er[2]=m.w(er[14]+20);er[3]=0x7ff;er[2]+=er[3];m.t(int(er[2])>=0);need(m.c.sr&1,"Negative stream size unmodeled");m.cycles+=22;
  er[1]=0xfffffff5;er[2]=unsigned(int(er[2])>>11);put(expected_ram,er[14]+48,er[2]);er[5]=0;m.wb(er[14],er[7]);er[2]=m.w(er[11]);er[1]=m.w(er[2]+36);need(er[1]==0x8c6621a2,"Stream query differs");m.c.pr=0x8c65faf8;er[4]=er[11];m.cycles+=10;
  m.push(er[14]);m.push(m.c.pr);er[0]=er[5];m.t(er[0]==1);er[14]=er[4];need(!(m.c.sr&1),"Query mode differs");m.cycles+=7;m.t(er[5]==0);need(m.c.sr&1,"Query zero mode differs");m.cycles+=2;m.c.pr=m.pop();er[0]=m.w(er[14]+16);er[14]=m.pop();m.cycles+=5;
  put(expected_ram,er[14]+24,er[0]);put(expected_ram,er[14]+28,er[0]);er[0]=er[14];m.c.pr=m.pop();er[10]=m.pop();er[11]=m.pop();er[13]=m.pop();m.c.pc=m.c.pr;er[14]=m.pop();m.cycles+=10;need(m.cycles==77&&m.c.pc==0x8c65fb64&&er[0]==0x8c8a8010,"Stream constructor return differs");const auto expected=m.c;

  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,77);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65fa86);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(dirty==0&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"cycles":77,"return_pc":"8C65FB64","stream_handle":"8C8A8010","new_instructions":94,"dirty_record_verified":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original movie stream constructor return:8controls,77cycles,to65FB64\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
