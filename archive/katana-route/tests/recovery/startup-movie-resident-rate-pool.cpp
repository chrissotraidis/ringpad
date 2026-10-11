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






namespace dcrecomp_generated {void resident_rate_pool_register(DCRuntime&);}
struct RatePoolModel:RequestModel{
 std::function<void(unsigned,const RatePoolModel&)> capture;
 void mark(unsigned n){if(capture)capture(n,*this);}
 bool run(){auto&r=c.r;push(r[14]);push(r[13]);push(r[12]);push(c.pr);r[15]-=4;r[5]=w(0x8c6489dc);r[12]=52;r[4]=8;r[14]=0;cycles+=9;
  for(;;){c.macl=r[14]*r[12];r[3]=c.macl+r[5];r[2]=w(r[3]);t(r[2]==0);cycles+=7;if(c.sr&1)break;++r[14];t(int32_t(r[14])>=int32_t(r[4]));cycles+=3;if(c.sr&1)break;}
  r[0]=r[14];t(r[0]==8);cycles+=3;if(c.sr&1){r[0]=0;r[15]+=4;c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;return false;}
  c.macl=r[14]*r[12];r[3]=w(0x8c6489e0);r[6]=52;r[13]=c.macl+r[5];r[5]=0;c.pr=0x8c6488ec;r[4]=r[13];c.pc=0x8c10ef3c;cycles+=10;mark(0);
  r[7]=r[3]=0;t(r[3]>=r[6]);r[0]=r[4];for(unsigned i=0;i<r[6];++i){++r[7];wb(r[0],r[5]);t(r[7]>=r[6]);++r[0];}c.pc=c.pr;r[0]=r[4];cycles+=8+5*r[6];mark(1);
  r[2]=1;r[7]=r[13];r[3]=0;put(m,r[13],r[2]);r[7]+=48;r[2]=r[3];r[5]=w(0x8c6489e4);r[4]=unsigned(int(int16_t(w(0x8c6489da))));put(m,r[13]+4,r[14]);put(m,r[13]+8,r[3]);put(m,r[13]+12,r[3]);r[3]=w(0x8c6489e8);put(m,r[15],r[7]);r[7]+=2;c.pr=0x8c64890c;r[6]=w(r[15]);c.pc=0x8c6488ec;cycles+=17;mark(2);return true;
 }

};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_clear=nullptr;
static void observed_clear(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);real_clear(c,rt);if(boundary)boundary(1);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c6488ae;auto device=read_sq(argv[5]);const std::string out=argv[9];std::string last_ram=argv[1];unsigned controls=0,empty=0,ready=0;
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10ef3c)&&!rt.targets.contains(0x8c6488ae)&&!rt.targets.contains(0x8c65eb00),"Resident target inventory differs");auto count=rt.targets.size();resident_rate_pool_register(rt);need(rt.targets.size()==count+10,"Resident entries not unique");real_clear=rt.targets.at(0x8c10ef3c);rt.targets.at(0x8c10ef3c)=observed_clear;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto check=[&](unsigned selected,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;auto c=captured?initial:context(0x8c6488ae,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=2;c.r[5]=(0x8cf40000+placement)|seg;c.r[6]=0x2000;c.r[7]=0x2020;c.pr=0x8c65f3de;for(unsigned i=0;i<8;++i){unsigned record=0x894900+52*i;for(unsigned off=0;off<52;++off)rt.main_ram[record+off]=0x5a;put(rt.main_ram,record,i<selected?0xffffffff:0);}}
  auto expected=rt.main_ram;RatePoolModel m{{c,expected}};std::vector<Prediction> phases;m.capture=[&](unsigned n,const RatePoolModel&v){need(n==phases.size(),"Pool prediction order");phases.push_back({v.c,v.m,v.cycles});};bool success=m.run();const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](const Prediction&p,bool active){same(p.c,c);need(p.ram==rt.main_ram,"Pool RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Pool timer controls");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Pool SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Pool device effects");if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<seen<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  boundary=[&](unsigned n){need(n==seen&&seen<phases.size(),"Pool observed order");verify(phases[seen],n<2);++seen;};
  if(success){bool rejected=false;try{call_recompiled(c,rt,0x8c6488ae);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");rejected=true;}need(rejected,"Missing next resident target not observed");boundary(2);if(!captured)++ready;}
  else{call_recompiled(c,rt,0x8c6488ae);verify({m.c,m.m,m.cycles},false);need(seen==0&&m.cycles==101,"Empty pool path differs");++empty;}
  boundary={};if(captured){need(seen==3&&phases[0].cycles==29&&phases[1].cycles==297&&phases[2].cycles==314&&c.r[3]==0x8c65eb00&&c.r[4]==500&&c.r[5]==44100&&c.r[6]==0x8c894930&&c.r[7]==0x8c894932,"Captured pool boundary differs");}else ++controls;
 };
 for(unsigned selected:{0u,4u,8u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(selected,alias,placement,false);
 check(0,0,0,true);need(controls==12&&ready==8&&empty==4,"Rate pool coverage differs");

 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":12,\"ready_dispatches\":8,\"empty_returns\":4,\"resident_entries\":10,\"new_original_bodies\":1,\"new_original_instructions\":60,\"captured_cycles\":[29,297,314],\"missing_target\":\"8C65EB00\",\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident rate pool:12controls,8ready/4empty;29/297/314cycles with real52byte clear;nextmissing65EB00\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
