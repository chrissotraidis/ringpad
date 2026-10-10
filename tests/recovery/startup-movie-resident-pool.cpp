// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}






namespace dcrecomp_generated {void resident_pool_register(DCRuntime&);}
struct PoolModel:RequestModel{
 std::function<void(unsigned,const PoolModel&)> capture;
 void mark(unsigned n){if(capture)capture(n,*this);}
 void shl(unsigned&v){t(v>>31);v<<=1;}void shr(unsigned&v){t(v&1);v>>=1;}
 bool run(){auto&r=c.r;for(unsigned i=14;i>=8;--i)push(r[i]);push(c.pr);r[15]-=4;r[11]=r[5];r[3]=0xffffffe0;r[11]+=31;r[12]=r[4];r[11]&=r[3];r[2]=r[11]-r[5];r[6]-=r[2];r[5]=w(0x8c647f98);r[13]=0;put(m,r[15],r[6]);r[6]=8;r[4]=0;r[10]=116;cycles+=23;
  for(;;){c.macl=r[4]*r[10];r[3]=c.macl;r[3]+=r[5];r[2]=sb(r[3]);t(r[2]==0);cycles+=7;if(c.sr&1)break;++r[4];t(int32_t(r[4])>=int32_t(r[6]));cycles+=3;if(c.sr&1)break;}
  r[0]=r[4];t(r[0]==8);cycles+=3;
  if(c.sr&1){r[0]=0;r[15]+=4;c.pr=pop();for(unsigned i=8;i<14;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=13;return false;}
  c.macl=r[4]*r[10];r[3]=w(0x8c647f9c);r[6]=116;r[14]=c.macl+r[5];r[5]=0;c.pr=0x8c647ed2;r[4]=r[14];c.pc=0x8c10ef3c;cycles+=10;mark(0);
  r[7]=r[3]=0;t(r[3]>=r[6]);r[0]=r[4];for(unsigned i=0;i<r[6];++i){++r[7];wb(r[0],r[5]);t(r[7]>=r[6]);++r[0];}c.pc=c.pr;r[0]=r[4];cycles+=8+5*r[6];mark(1);
  r[0]=r[12];r[4]=r[0]<<8;r[3]=r[0];r[4]+=r[3];r[10]=w(0x8c647fa0);r[4]<<=2;wb(r[14]+3,r[0]);r[4]<<=2;shl(r[4]);shl(r[4]);r[2]=r[4]+r[11];put(m,r[14]+32,r[2]);r[1]=36;r[3]=w(r[15]);r[2]=unsigned(int(int16_t(w(0x8c647f94))));r[3]-=r[4];r[3]-=36;r[3]>>=8;r[3]>>=2;shr(r[3]);r[3]<<=8;r[3]<<=2;shl(r[3]);put(m,r[14]+36,r[3]);r[3]=unsigned(int(int16_t(w(0x8c647f92))));put(m,r[14]+40,r[1]);put(m,r[14]+44,r[11]);put(m,r[14]+48,r[3]);put(m,r[14]+52,r[2]);r[6]=w(r[14]+40);r[5]=w(r[14]+36);c.pr=0x8c647f18;r[4]=w(r[14]+32);c.pc=0x8c647ed2;cycles+=36;mark(2);return true;
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_clear=nullptr;
static void observed_clear(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);real_clear(c,rt);if(boundary)boundary(1);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c647e7c;auto device=read_sq(argv[5]);const std::string out=argv[9];std::string last_ram=argv[1];unsigned controls=0,empty=0,ready=0;
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10ef3c)&&!rt.targets.contains(0x8c647e7c)&&!rt.targets.contains(0x8c662106),"Resident target inventory differs");auto count=rt.targets.size();resident_pool_register(rt);need(rt.targets.size()==count+24,"Resident entries not unique");real_clear=rt.targets.at(0x8c10ef3c);rt.targets.at(0x8c10ef3c)=observed_clear;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto check=[&](unsigned selected,unsigned alignment,unsigned channels,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;auto c=captured?initial:context(0x8c647e7c,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=channels;c.r[5]=(0x8cf40000+alignment)|seg;c.r[6]=0x80a0;c.pr=0x8ced144c;for(unsigned i=0;i<8;++i)rt.main_ram[0x6774f0+116*i]=i<selected?0x80:0;}
  auto expected=rt.main_ram;PoolModel m{{c,expected}};std::vector<Prediction> phases;m.capture=[&](unsigned n,const PoolModel&v){need(n==phases.size(),"Pool prediction order");phases.push_back({v.c,v.m,v.cycles});};bool success=m.run();const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](const Prediction&p,bool active){same(p.c,c);need(p.ram==rt.main_ram,"Pool RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Pool timer controls");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Pool SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Pool device effects");if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<seen<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  boundary=[&](unsigned n){need(n==seen&&seen<phases.size(),"Pool observed order");verify(phases[seen],n<2);++seen;};
  if(success){bool rejected=false;try{call_recompiled(c,rt,0x8c647e7c);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");rejected=true;}need(rejected,"Missing next resident target not observed");boundary(2);if(!captured)++ready;}
  else{call_recompiled(c,rt,0x8c647e7c);verify({m.c,m.m,m.cycles},false);need(seen==0&&m.cycles==119,"Empty pool path differs");++empty;}
  boundary={};if(captured){need(seen==3&&phases[0].cycles==43&&phases[1].cycles==631&&phases[2].cycles==667&&c.r[10]==0x8c662106&&c.r[5]==0xfffff800&&c.r[4]==0x8cbd00c0&&c.r[6]==36,"Captured pool boundary differs");}else ++controls;
 };
 for(unsigned selected:{0u,3u,8u})for(unsigned alignment:{0u,1u,31u})for(unsigned channels:{0u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(selected,alignment,channels,alias,placement,false);
 check(0,0,2,0,0,true);need(controls==72&&ready==48&&empty==24,"Pool coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":72,\"ready_dispatches\":48,\"empty_returns\":24,\"resident_entries\":24,\"new_original_bodies\":1,\"new_original_instructions\":206,\"captured_cycles\":[43,631,667],\"missing_target\":\"8C662106\",\"captured_wrapped_length\":\"FFFFF800\",\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident pool:72controls,48ready/24empty;43/631/667cycles with real116byte clear;nextmissing662106,observed wrapped lengthFFFFF800\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
