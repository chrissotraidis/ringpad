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


struct SevenByteModel : RequestModel {
 void error(){
  auto&r=c.r;r[3]=w(0x8c65e9f8);r[2]=w(0x8c65e9fc);put(m,r[3],r[4]);c.pc=c.pr;put(m,r[2],r[5]);cycles+=6;
 }
 void copy20(){ //10E964, ordered descending word reads/stores
  auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;
  need(r[3]==0x8c10e9a2&&r[0]==16,"Unexpected twenty-byte copy target");
  r[3]=w(r[2]+r[0]);range(0x964,0x96e,1);
  r[0]=w(r[2]+12);put(m,r[1]+16,r[3]);r[3]=w(r[2]+8);put(m,r[1]+12,r[0]);
  r[0]=w(r[2]+4);put(m,r[1]+8,r[3]);r[3]=w(r[2]);put(m,r[1]+4,r[0]);put(m,r[1],r[3]);
  c.pc=c.pr;r[3]=pop();range(0x9a2,0x9b6,1);
 }
 void queue(){ //65E1A4, real critical section and full/absent errors
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=c.sr;r[3]=0xffffff0f;r[14]=0;
  r[0]=(r[0]>>4)&15;put(m,r[15],r[0]);r[0]=(c.sr&r[3])|240;c.sr=r[0];
  r[2]=w(0x8c65e240);r[5]=w(r[2]);t(r[5]==0);range(0x1a4,0x1c6,5);
  if(c.sr&1){r[14]=w(0x8c65e244);range(0x1c8,0x1cc);}else{
   r[2]=0x158;r[3]=0x100;r[5]+=r[2];r[6]=w(r[5]+20);t(r[6]==r[3]);r[7]=w(r[5]+16);range(0x1ce,0x1da);
   if(c.sr&1){r[14]=w(0x8c65e248);range(0x1dc,0x1e0);}else{
    r[6]=w(r[5]+24);r[3]=28;c.macl=r[3]*r[6];++r[6];put(m,r[5]+24,r[6]);r[1]=w(r[5]+24);
    r[6]=c.macl;r[2]=w(r[5]+12);t(r[1]==r[7]);r[6]+=r[2];r[3]=1;range(0x1e2,0x1f8,1);
    if(c.sr&1){r[0]=0;put(m,r[5]+24,r[0]);range(0x1fa,0x1fc);}
    put(m,r[6]+20,r[3]);r[2]=r[4];r[3]=w(0x8c65e24c);r[1]=r[6];c.pr=0x8c65e20a;r[0]=20;range(0x1fe,0x208,1);copy20();
    r[2]=w(r[5]+20);++r[2];put(m,r[5]+20,r[2]);range(0x20a,0x20e);
   }
  }
  r[0]=w(r[15]);r[3]=c.sr;r[1]=0xffffff0f;r[0]&=15;r[0]<<=2;r[3]&=r[1];r[0]<<=2;r[0]|=r[3];c.sr=r[0];t(r[14]==0);range(0x210,0x224,4);
  if(!(c.sr&1)){r[3]=w(0x8c65e254);r[4]=w(0x8c65e250);c.pr=0x8c65e22e;r[5]=r[14];range(0x226,0x22c,1);error();}
  r[0]=r[14];r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();range(0x22e,0x236,1);
 }
};


struct ChannelStartModel:SevenByteModel{
 void start(){auto&r=c.r;r[3]=0x8c66ee7c;cycles+=4;r[2]=w(0x8c66eec0);push(r[14]);push(c.pr);r[3]=w(r[2]);need(r[3]&&r[4],"Reached global/descriptor required");t(false);cycles+=8;
  r[2]=w(0x8c66eecc);r[3]=46;r[1]=w(0x8c66eed0);put(m,r[2],r[3]);r[0]=sb(r[4]+1);r[3]=w(0x8c66eed4);wb(r[1],r[0]);c.pr=0x8c66eeaa;r[4]=r[2];cycles+=10;queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(c.sr&1,"Channel start queue error");c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();cycles+=5;
 }
 SH4Context first_return{};
 void caller(){auto&r=c.r;
  auto check=[&](){r[4]=r[0];t(r[4]==0);cycles+=3;need(c.sr&1,"Channel start failed");++r[11];r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=4;};
  check();need(!(c.sr&1)&&r[11]==1,"Expected second channel");r[5]=r[11];c.pr=0x8c66bc1a;r[4]=w(r[14]+40);cycles+=4;start();check();need(c.sr&1&&r[11]==2,"Two-channel loop did not finish");
  r[9]=w(0x8c66bd18);r[11]=r[13];r[8]=w(0x8c66bd14);r[10]=127;cycles+=5;r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=3;need(!(c.sr&1),"No channel configuration");push(r[10]);r[6]=127;push(r[13]);r[5]=r[11];push(r[13]);r[7]=0;c.pr=0x8c66bc46;r[4]=w(r[14]+40);c.pc=0x8c66bc36;cycles+=9;need(r[8]==0x8c66d190,"Next configuration helper differs");
 }
};
namespace dcrecomp_generated {void resident_channel_start_register(DCRuntime&);void resident_channel_start_body_register(DCRuntime&);void resident_service_constructor_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66bc14,"Wrong start helper input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned phase:{0u,1u,2u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66d184)&&!rt.targets.contains(0x8c66ee7c),"Start helper already present");resident_channel_start_register(rt);resident_channel_start_body_register(rt);need(rt.targets.size()==count+10,"Entry count differs");resident_service_constructor_register(rt);need(!rt.targets.contains(0x8c66d190),"Next setup helper already present");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;const auto q=word(ram,0x8c67ff10)+0x158;need(word(ram,q+20)==2&&word(ram,q+24)==42,"Captured queue differs");if(phase==1)put(rt.main_ram,q+24,255);auto expected_ram=rt.main_ram;ChannelStartModel model{{{c,expected_ram}}};model.start();model.first_return=model.c;need(model.cycles==(phase==1?125u:123u),"Start source cycles differ");if(phase==2){model.caller();need(model.cycles==281,"Caller source cycles differ");}
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);const auto vram=rt.vram;call_recompiled(c,rt,0x8c66d184);same(model.first_return,c);
  if(phase==2){bool missing=false;try{call_recompiled(c,rt,0x8c66bc1a);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next helper error");missing=true;}need(missing,"Next helper boundary absent");}
  need(rt.relocated_code_matches==0,"Unexpected template fallback");same(model.c,c);need(expected_ram==rt.main_ram,"Start commands/caller RAM differ");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");need(word(rt.main_ram,q+20)==(phase==2?4u:3u),"Queued start commands differ");
  if(phase==2&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==12,"Start helper coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":12,"start_cycles":[123,125],"combined_cycles":281,"next_target":"8C66D190","command":46,"new_instructions":37,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original start wrapper/queue and both caller iterations:12controls,123/125/281cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
