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



struct ChannelConfigureModel:RequestModel{
 void field(){auto&r=c.r;r[3]=w(0x8c66ef4c);push(r[14]);push(c.pr);r[6]=w(r[3]);r[7]=w(0x8c66ef48);r[14]=0;need(r[6]&&r[4],"Reached global/descriptor required");t(false);cycles+=12;
  r[0]=sb(r[4]+1);r[3]=44;r[7]=r[0];r[4]=r[7];c.macl=44*r[4];r[0]=static_cast<int8_t>(r[5]);t(r[0]==127);r[4]=c.macl;cycles+=10;
  r[1]=0xa8;if(c.sr&1){r[1]+=r[6];r[4]+=r[1];r[0]=0;cycles+=5;}else{t(r[5]&0x80000000);r[5]<<=1;r[0]=0xffffffff;r[1]+=r[6];r[0]-=r[5];r[4]+=r[1];cycles+=6;}
  wb(r[4]+6,r[0]);cycles+=2;t(r[14]==0);cycles+=2;c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();cycles+=5;
 }

 SH4Context config_return{},service_return{};
 void configure(){auto&r=c.r;r[15]-=4;r[3]=w(0x8c66d19c);wb(r[15],r[6]);r[5]=sb(r[15]);r[15]+=4;cycles+=7;need(r[3]==0x8c66eee0,"Retained field target differs");field();}
 void caller(){auto&r=c.r;auto check=[&](){r[4]=r[0];t(r[4]==0);need(c.sr&1,"Configuration failed");r[15]+=12;++r[11];r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=8;};check();need(!(c.sr&1)&&r[11]==1,"Expected second configuration");push(r[10]);r[6]=127;push(r[13]);r[5]=r[11];push(r[13]);r[7]=0;c.pr=0x8c66bc46;r[4]=w(r[14]+40);cycles+=9;configure();check();need(c.sr&1&&r[11]==2,"Configuration loop did not end");
  r[0]=64;r[1]=0xffffffff;r[3]=1;put(m,r[14]+r[0],r[1]);r[0]=r[14];put(m,r[14]+60,r[13]);put(m,r[14],r[3]);cycles+=7;r[15]+=32;c.pr=pop();for(unsigned i=8;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;need(c.pc==0x8c6609f4,"Service constructor return differs");service_return=c;
  r[1]=r[14]+64;put(m,r[1],r[0]);r[0]=64;r[3]=w(r[14]+r[0]);t(r[3]==0);need(!(c.sr&1),"Service handle absent");cycles+=7;c.pr=0x8c660a10;r[4]=r[14];c.pc=0x8c660a0c;cycles+=3;
 }
};
namespace dcrecomp_generated {void resident_channel_configure_register(DCRuntime&);void resident_channel_field_register(DCRuntime&);void resident_service_constructor_register(DCRuntime&);void resident_second_constructor_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66bc36,"Wrong configure input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned phase:{0u,1u,2u,3u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c66d190),"Configuration wrapper already present");resident_channel_configure_register(rt);resident_channel_field_register(rt);resident_service_constructor_register(rt);resident_second_constructor_register(rt);need(!rt.targets.contains(0x8c6608bc),"Next constructor helper already present");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;if(phase==1)c.r[6]=0;if(phase==2)c.r[6]=255;auto expected_ram=ram;ChannelConfigureModel model{{c,expected_ram}};model.configure();model.config_return=model.c;need(model.cycles==((phase==1||phase==2)?44u:43u),"Configure source cycles differ");if(phase==3){model.caller();need(model.cycles==139,"Service source cycles differ");}
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);const auto vram=rt.vram;call_recompiled(c,rt,0x8c66d190);same(model.config_return,c);
  if(phase==3){call_recompiled(c,rt,0x8c66bc46);same(model.service_return,c);bool missing=false;try{call_recompiled(c,rt,0x8c6609f4);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next helper error");missing=true;}need(missing,"Next helper boundary absent");}
  need(rt.relocated_code_matches==0,"Unexpected template fallback");same(model.c,c);need(expected_ram==rt.main_ram,"Configuration/service RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(phase==3&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Configure/service coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"configure_cycles":[43,44],"combined_cycles":139,"service_constructor_returned":true,"next_target":"8C6608BC","new_instructions":6,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original configuration wrapper and service constructor return:16controls,43/44/139cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
