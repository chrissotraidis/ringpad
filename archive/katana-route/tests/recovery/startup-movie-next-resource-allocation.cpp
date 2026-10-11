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



struct HandlerPoolModel:RequestModel{
 bool full=false;unsigned slot=0;
 void pool(){auto&r=c.r;push(r[14]);r[0]=64;push(c.pr);r[14]=w(0x8c6621e4);r[7]=0;cycles+=5;
 for(;;){r[3]=r[7];t(r[3]>>31);r[3]<<=1;r[2]=r[7];r[3]+=r[2];r[3]<<=4;r[3]+=r[14];r[1]=w(r[3]+4);t(r[1]==0);cycles+=10;if(c.sr&1)break;++r[7];t(static_cast<int32_t>(r[7])>=static_cast<int32_t>(r[0]));cycles+=3;if(c.sr&1)break;}
 slot=r[7];r[0]=r[7];t(r[0]==64);cycles+=4;full=c.sr&1;
 if(full){c.pr=pop();r[0]=0;c.pc=c.pr;r[14]=pop();cycles+=5;return;}
 r[3]=r[7];t(r[7]>>31);r[7]<<=1;r[7]+=r[3];r[3]=w(0x8c6621ec);r[7]<<=4;r[2]=1;r[14]+=r[7];put(m,r[14]+4,r[2]);put(m,r[14],r[3]);r[3]=w(0x8c6621f4);r[2]=w(0x8c6621f0);put(m,r[14]+28,r[4]);put(m,r[14]+32,r[5]);put(m,r[14]+36,r[6]);put(m,r[14]+8,r[2]);put(m,r[14]+40,r[3]);put(m,r[14]+44,r[14]);c.pr=0x8c662162;r[4]=r[14];c.pc=0x8c66213a;cycles+=21;
 }
};
namespace dcrecomp_generated {void resident_handler_pool_register(DCRuntime&);void resident_handler_init_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8ced0854,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned dirty:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c662194),"Original initializer already registered");resident_handler_init_register(rt);resident_handler_pool_register(rt);need(rt.targets.size()==count+8,"Initializer/pool entries differ");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;const auto pool=word(ram,0x8c6621e4);for(unsigned i=0;i<64;++i)need(word(ram,pool+i*48+4)==unsigned(i<4),"Current pool occupancy differs");if(dirty)for(unsigned offset:{12u,16u,20u,24u})put(rt.main_ram,pool+4*48+offset,0xa5a5a5a5);auto expected_ram=rt.main_ram;HandlerPoolModel m{{c,expected_ram}};m.pool();need(m.slot==4&&m.cycles==92&&!m.full,"Next slot prediction differs");auto expected=m.c;auto&er=expected.r;er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=word(expected_ram,er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);expected.pc=expected.pr;put(expected_ram,er[4]+24,er[5]);auto init_return=expected;need(expected.pc==0x8c662162,"Initializer return differs");er[0]=er[14];expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pc==0x8cecf1fc&&er[0]==0x8c8aa8f8,"Pool return differs");
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,106);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c662106);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(dirty==0&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"cycles":106,"initializer_cycles":8,"return_pc":"8CECF1FC","new_instructions":0,"dirty_record_verified":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original handler initializer and constructor return:8controls,106cycles,toECF1FC\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
