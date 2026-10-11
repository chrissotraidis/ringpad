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


static void channel_prefix(auto&m,SH4Context&c){
 auto&r=c.r;for(unsigned i=14;i>=9;--i){r[15]-=4;put(m,r[15],r[i]);}r[15]-=4;put(m,r[15],c.pr);r[15]-=4;
 r[2]=word(m,0x8c66d0b8);r[3]=word(m,r[2]);need(r[3]!=0,"Captured global absent");c.sr&=~1u;r[9]=r[4];r[0]=32;r[4]=m[(r[15]+32)&0xffffff];need(r[4]==2,"Only captured two-channel prefix modeled");r[10]=1;c.sr|=1;r[2]=2;c.sr&=~1u;
 r[4]=r[15]+33;r[5]=3;need((r[4]&3)!=0,"Unexpected vararg alignment");c.sr&=~1u;r[14]=0;r[4]=r[15]+36;r[6]=0;
 for(unsigned i=0;i<3;++i){
  r[0]=32;r[13]=m[(r[15]+32)&0xffffff];c.sr=(c.sr&~1u)|(r[6]>=r[13]);r[11]=r[15];if(c.sr&1)break;
  r[7]=r[4]+4;r[3]=r[7];need(!(r[3]&3),"Unexpected argument alignment");c.sr|=1;r[13]=r[6]+r[11];r[4]=r[7];r[12]=r[4]-4;r[3]=r[12];need(!(r[3]&3),"Unexpected argument source");c.sr|=1;r[2]=r[12];r[1]=word(m,r[2]);m[r[13]&0xffffff]=r[1];r[7]=static_cast<int8_t>(m[r[13]&0xffffff]);need(r[7]<=15,"Invalid channel ID");c.sr|=1;r[2]=15;c.sr&=~1u;++r[6];
 }
 r[5]=r[11]+2;r[4]=r[13]+r[11];need(r[4]>=r[5],"Unexpected padding path");c.sr|=1;r[2]=word(m,0x8c66d0c4);r[5]=3;c.pr=0x8c66d07a;r[4]=r[9];c.pc=0x8c66d072;
}
static unsigned retained_allocator(auto&m,SH4Context&c){
 auto&r=c.r;r[15]-=4;put(m,r[15],r[14]);r[0]=r[5];r[15]-=4;put(m,r[15],r[13]);r[13]=0;r[15]-=4;put(m,r[15],c.pr);r[3]=0x8c67ff10;r[14]=0;r[6]=8;r[7]=word(m,r[3]);need(r[0]==3,"Only reached third pool modeled");c.sr|=1;
 r[5]=r[7]+0x88;r[6]=0;r[7]=4;unsigned occupied=0;
 for(;;){r[0]=word(m,r[5]+4);c.sr=(c.sr&~1u)|(r[0]==1);if(r[0]!=1)break;++r[6];c.sr=(c.sr&~1u)|(r[6]>=r[7]);r[5]+=8;++occupied;need(occupied<4,"Successful allocation required");}
 r[0]=r[6];c.sr&=~1u;r[3]=1;r[2]=r[5];put(m,r[4],r[5]);put(m,r[2]+4,r[3]);c.sr|=1;c.pr=word(m,r[15]);r[15]+=4;r[0]=r[14];r[13]=word(m,r[15]);r[15]+=4;r[14]=word(m,r[15]);r[15]+=4;c.pc=c.pr;return 41+7*occupied;
}
namespace dcrecomp_generated {void resident_channel_pair_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66bba0,"Wrong service helper input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned occupied:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66cfc4),"Channel helper already present");resident_channel_pair_register(rt);need(rt.targets.size()==count+32,"Entry count differs");need(rt.targets.contains(0x8c65e770)&&!rt.targets.contains(0x8c66f2ec),"Dependency map differs");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;
  const auto global=word(ram,0x8c67ff10);for(unsigned i=0;i<4;++i)need(word(ram,global+0x8c+8*i)==0,"Captured third pool occupancy changed");if(occupied)put(rt.main_ram,global+0x8c,1);
  auto expected=c;auto expected_ram=rt.main_ram;auto&er=expected.r;channel_prefix(expected_ram,expected);auto cycles=106+retained_allocator(expected_ram,expected);er[12]=er[0];need(er[12]==0,"Allocator failed");expected.sr|=1;cycles+=3;
  er[4]=word(expected_ram,er[9]);er[2]=44;er[3]=0x8c67ff10;er[0]=static_cast<int8_t>(expected_ram[(er[4]+1)&0xffffff]);er[1]=0xa8;er[4]=er[0];expected.macl=44*er[4];er[13]=word(expected_ram,er[3]);er[0]=32;er[3]=word(expected_ram,0x8c66d0c8);er[13]+=er[1];er[4]=expected.macl;er[13]+=er[4];put(expected_ram,er[13],er[10]);er[0]=static_cast<int8_t>(expected_ram[(er[15]+32)&0xffffff]);expected_ram[(er[13]+4)&0xffffff]=er[0];er[0]=32;er[5]=static_cast<int8_t>(expected_ram[(er[15]+32)&0xffffff]);expected.pr=0x8c66d0aa;er[4]=word(expected_ram,er[9]);expected.pc=0x8c66d080;cycles+=23;need(er[3]==0x8c66f2ec&&cycles==173+7*occupied,"Source-derived next target/cycles differ");
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  bool missing=false;try{call_recompiled(c,rt,0x8c66cfc4);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong helper boundary error");missing=true;}need(missing,"Missing helper boundary not reached");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel creation RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!occupied&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Channel-prefix coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"canonical_cycles":173,"later_slot_cycles":180,"next_target":"8C66F2EC","retained_allocator":true,"new_instructions":162,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original channel pair prefix and retained allocator:8controls,173/180cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
