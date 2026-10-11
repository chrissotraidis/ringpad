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



struct PanModel : RequestModel {
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }
 void divide(){auto&r=c.r;t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e728);r[2]=w(0x8c10e72c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;div0s(2,1);const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}rot(1);const std::uint64_t sum=std::uint64_t(r[1])+r[2]+(c.sr&1);r[1]=unsigned(sum);t(sum>>32);r[0]=r[1];r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=80;
 }
 void pan(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[1]=r[6];r[1]<<=2;r[1]<<=2;r[2]=w(0x8c66c154);r[1]<<=2;r[3]=r[6];t(r[1]>>31);r[1]<<=1;put(m,r[15],r[4]);r[1]-=r[3];c.pr=0x8c66c0d8;r[0]=15;cycles+=15;need(r[2]==0x8c10e67c,"Original division target differs");divide();
 r[3]=w(0x8c66c158);r[6]=r[0];wb(r[15]+4,r[0]);r[7]=0;r[4]=w(r[15]);c.pr=0x8c66c0e6;r[4]=w(r[4]+40);c.pc=0x8c66c0d8;cycles+=8;need(r[3]==0x8c66d1d8,"Original pan forwarding target differs");
 }
};
namespace dcrecomp_generated {void resident_pan_adapter_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c661a36,"Wrong pan input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(int pan:{-15,-1,0,1,15})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66c0bc),"Pan adapter already registered");resident_pan_adapter_register(rt);need(rt.targets.size()==count+5&&rt.targets.contains(0x8c10e67c),"Original entries or retained division differ");need(!rt.targets.contains(0x8c66d1d8),"Next target already registered");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;c.r[6]=unsigned(pan);auto expected_ram=ram;PanModel m{{c,expected_ram}};m.pan();need(m.cycles==103&&m.c.r[6]==unsigned(pan*127/15),"Pan quotient or timing differs");auto expected=m.c;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  bool missing=false;try{call_recompiled(c,rt,0x8c66c0bc);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong helper boundary error");missing=true;}need(missing,"Missing helper boundary absent");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Pan adapter RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(pan==-15&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==20,"Pan adapter coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":20,"cycles":103,"next_target":"8C66D1D8","new_instructions":33,"retained_division_executed":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original pan adapter through retained division:20controls,103cycles,to66D1D8\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
