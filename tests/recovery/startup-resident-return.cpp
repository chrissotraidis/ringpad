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



struct ResidentReturnModel:RequestModel{
 void half(unsigned a,unsigned v){wb(a,v);wb(a+1,v>>8);}
 void finish(){auto&r=c.r;t(r[0]==0);put(m,r[14]+12,r[0]);need(!(c.sr&1),"Outer constructor returned null");cycles+=3;r[3]=60;put(m,r[14]+56,r[3]);r[2]=w(r[14]+36);t(static_cast<int32_t>(r[2])>=0);cycles+=5;
 if(!(c.sr&1)){r[3]=static_cast<int16_t>(w(0x8c6480dc));r[2]+=r[3];cycles+=2;}
 r[3]=0xfffffff5;r[4]=r[14];r[0]=60;t(static_cast<int32_t>(r[12])>0);r[2]=unsigned(static_cast<int32_t>(r[2])>>11);r[5]=r[13];half(r[14]+r[0],r[2]);r[3]=static_cast<int16_t>(w(r[14]+r[0]));r[0]=0x8c6480e0;c.fr_bits[2]=w(r[0]);r[0]=62;c.fpul=r[3];need(r[3]==0xffffffff||r[3]==0||r[3]==1,"Only exact unit/zero scalar path modeled");need(c.fr_bits[2]==0x3f599999,"Original coefficient differs");c.fr_bits[3]=r[3]==0?0:r[3]==1?0x3f599999:0xbf599999;c.fpul=0;r[3]=c.fpul;half(r[14]+r[0],r[3]);r[0]=64;half(r[14]+r[0],r[13]);r[4]+=66;cycles+=21;
 need((c.sr&1)&&r[12]==2&&r[13]==0,"Captured stereo loop differs");r[6]=0xffffff80;++cycles;for(unsigned i=0;i<2;++i){++r[5];half(r[4],r[6]);t(static_cast<int32_t>(r[5])>=static_cast<int32_t>(r[12]));r[4]+=2;cycles+=5;}
 r[0]=108;r[4]=1;wb(r[14]+r[0],r[4]);for(unsigned offset:{84u,88u,92u}){r[0]=offset;put(m,r[14]+r[0],r[13]);}r[0]=96;half(r[14]+r[0],r[13]);r[0]=100;put(m,r[14]+r[0],r[13]);for(unsigned offset:{104u,106u}){r[0]=offset;half(r[14]+r[0],r[13]);}r[0]=109;wb(r[14]+r[0],r[4]);r[0]=114;wb(r[14]+r[0],r[13]);r[0]=r[14];wb(r[14],r[4]);cycles+=23;
 r[15]+=4;c.pr=pop();for(unsigned i=8;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;need(c.pc==0x8ced144c&&r[0]==0x8c6774f0,"Resident constructor return differs");
 }
};
namespace dcrecomp_generated {void resident_pool_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c647f84,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(int value:{-2048,0,2048})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u})for(unsigned mode:{0u,1u,2u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);resident_pool_register(rt);need(rt.targets.contains(0x8c647f84),"Original parent continuation absent");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;rt.fpu_aot_mode=mode;put(rt.main_ram,c.r[14]+36,unsigned(value));auto expected_ram=rt.main_ram;ResidentReturnModel m{{c,expected_ram}};m.finish();need(m.cycles==(value<0?76u:74u),"Resident tail timing differs");auto expected=m.c;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c647f84);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(value==-2048&&!host_zero&&guest_zero&&!mode){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==36,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":36,"captured_cycles":76,"nonnegative_cycles":74,"return_pc":"8CED144C","resident_constructor_returned":true,"scalar_exact_unit_zero_inputs":true,"new_instructions":0,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original resident constructor return:36controls,76/74cycles,toMOVIE144C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
