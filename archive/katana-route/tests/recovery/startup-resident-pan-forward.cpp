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


struct PanForwardModel:SevenByteModel{
 void callback(){auto&r=c.r;const unsigned channel=r[5];need(channel<2&&r[7]==0,"Only stereo callback with zero secondary value modeled");
  for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=8;r[3]=w(0x8c66f1ac);r[14]=w(r[3]);need(r[14]&&r[4],"Missing callback global/descriptor");t(false);
  r[0]=sb(r[4]+1);r[2]=44;r[1]=0xa8;r[12]=static_cast<int8_t>(r[6]);r[11]=r[0];r[3]=r[11];c.macl=44*r[3];r[14]+=r[1];r[6]=0;r[13]=0;r[3]=c.macl;r[14]+=r[3];r[0]=r[14]+28;r[3]=4*channel+r[0];put(m,r[3],r[12]);r[4]=r[15];need(sb(r[14]+5)==2,"Reached channel count differs");
  for(unsigned i=0;i<2;++i){r[0]=r[13]*4;++r[13];put(m,r[4]+r[0],r[12]);}r[0]=2;t(true);r[13]=128;r[12]=channel;r[0]=channel;t(channel==0);
  if(channel==0){r[2]=w(r[4])+r[13];put(m,r[4],r[2]);}else{r[5]=r[4]+4;r[2]=w(r[5]);r[3]=0xffffffff;r[2]-=r[13];r[1]=r[2];t(static_cast<int32_t>(r[1])>=-1);put(m,r[5],r[2]);if(c.sr&1){r[0]=0xfffffffe;put(m,r[4]+4,r[0]);cycles+=2;}}r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);r[2]=0;++r[5];t(static_cast<int>(r[3])<0);const auto total=std::uint64_t(r[3])+(c.sr&1);r[3]=total;t(total>>32);t(r[3]&1);r[3]=(r[3]>>1)|(r[3]&0x80000000);put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);++r[5];r[3]+=r[13];put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);
  r[3]=w(0x8c66f1b8);r[2]=38;r[0]=w(0x8c66f1c0);r[14]=r[7];put(m,r[3],r[2]);r[2]=3&r[12];r[1]=w(0x8c66f1bc);r[3]=sb(r[0]+r[2]);r[0]=r[12];r[5]=w(0x8c66f1c4);t(false);r[3]|=r[11];r[13]=r[7];wb(r[1],r[3]);r[3]=0xfffffff8;r[13]=0;t(true);
  if(channel==0){r[2]=w(r[4]);r[3]=w(0x8c66f220);wb(r[5],r[2]);r[2]=w(0x8c66f224);r[1]=w(0x8c66f228);wb(r[3],r[14]);r[3]+=4;wb(r[2],r[13]);r[2]+=4;wb(r[1],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);}else{r[3]=w(0x8c66f220);r[1]=w(0x8c66f228);r[0]=w(0x8c66f22c);r[2]=w(0x8c66f224);wb(r[5],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);r[3]=w(r[4]+4);wb(r[1],r[3]);r[3]=w(0x8c66f230);wb(r[0],r[14]);wb(r[3],r[13]);}r[3]=w(0x8c66f238);r[4]=w(0x8c66f234);c.pr=0x8c66f202;c.pc=0x8c66f1fa;cycles+=channel?160:152;
  queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(r[14]==0,"Queue failed");r[0]=r[14];r[15]+=8;c.pr=pop();for(unsigned i=11;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }

 SH4Context callback_return{};
 void forward(){auto&r=c.r;r[3]=w(0x8c66d1e0);need(r[3]==0x8c66f0b4,"Wrong retained callback");cycles+=4;callback();callback_return=c;need(c.pc==0x8c66c0e6,"Callback return differs");}
 void tail(){auto&r=c.r;r[14]=r[0];t(r[14]==0);need(c.sr&1,"Pan callback failed");cycles+=3;r[0]=r[14];r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;need(c.pc==0x8c661a4c,"Pan adapter return differs");}
};
namespace dcrecomp_generated {void resident_pan_forward_register(DCRuntime&);void resident_channel_callback_register(DCRuntime&);void resident_pan_adapter_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66c0d8,"Wrong forwarding input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned channel:{0u,1u})for(int pan:{-127,0,127})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c66d1d8),"Forwarder already registered");auto count=rt.targets.size();resident_pan_forward_register(rt);resident_channel_callback_register(rt);resident_pan_adapter_register(rt);need(rt.targets.size()==count+37,"Pan entries differ");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;c.r[5]=channel;c.r[6]=unsigned(pan);auto expected_ram=ram;PanForwardModel model{{{c,expected_ram}}};model.forward();model.tail();need(model.cycles==(channel?278u+(pan==127?2u:0u):270u),"Independent callback timing differs");auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c66d1d8);same(model.callback_return,c);call_recompiled(c,rt,0x8c66c0e6);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(model.c,c);need(expected_ram==rt.main_ram,"Forwarded count/pan RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");const auto q=word(ram,0x8c67ff10)+0x158;need(word(rt.main_ram,q+20)==7&&word(rt.main_ram,q+24)==47,"Repeated command submission differs");
  if(!channel&&pan==-127&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==24,"Forwarded count coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":24,"captured_cycles":270,"channel1_cycles":[278,280],"return_pc":"8C661A4C","new_instructions":3,"retained_callback_executed":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original pan forwarder, retained callback and adapter return:24controls,270/278/280cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
