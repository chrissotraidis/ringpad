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






namespace dcrecomp_generated {void resident_channel_register(DCRuntime&);void resident_channel_getters_register(DCRuntime&);}
struct GettersModel:RequestModel{
 void leaf(unsigned offset){c.pc=c.pr;c.r[0]=w(c.r[4]+offset);cycles+=3;}
 void next(){auto&r=c.r;r[3]=w(0x8c65f458);put(m,r[15]+4,r[0]);c.pr=0x8c65f3c4;r[4]=r[10];cycles+=5;leaf(32);r[2]=w(0x8c65f45c);r[11]=r[0];t(r[11]&1);r[11]>>=1;c.pr=0x8c65f3ce;r[4]=r[10];cycles+=6;leaf(36);r[3]=w(0x8c65f460);t(r[0]&1);r[0]>>=1;r[5]=w(r[15]+4);r[7]=r[11];r[7]+=r[0];r[6]=r[11];c.pr=0x8c65f3de;r[4]=r[12];c.pc=0x8c65f3ce;cycles+=9;}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c6624d8;auto device=read_sq(argv[5]);const std::string out=argv[9];std::string last_ram=argv[1];unsigned controls=0,empty=0,ready=0;
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c65f364)&&!rt.targets.contains(0x8c6624d8)&&!rt.targets.contains(0x8c6624dc)&&!rt.targets.contains(0x8c6624e0)&&!rt.targets.contains(0x8c66b0f4),"Resident getter inventory differs");auto count=rt.targets.size();resident_channel_register(rt);resident_channel_getters_register(rt);need(rt.targets.size()==count+19,"Resident entries not unique");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto check=[&](unsigned offset,unsigned value,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;auto c=captured?initial:context(0x8c6624d8,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=(0x8cf40000+placement)|seg;c.pr=0x8c65f3bc;for(unsigned off:{28u,32u,36u})put(rt.main_ram,c.r[4]+off,off==offset?value:0x12345600+off);}
  auto expected=rt.main_ram;GettersModel m{{c,expected}};m.leaf(offset);std::vector<Prediction> phases{{m.c,m.m,m.cycles}};if(captured){m.next();phases.push_back({m.c,m.m,m.cycles});}const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](const Prediction&p,bool active){same(p.c,c);need(p.ram==rt.main_ram,"Pool RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Pool timer controls");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Pool SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Pool device effects");if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<seen<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  call_recompiled(c,rt,0x8c6624d8+(offset-28));verify(phases[0],false);++seen;
  if(captured){bool rejected=false;try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");rejected=true;}need(rejected,"Next missing caller absent");verify(phases[1],false);need(phases[0].cycles==3&&m.cycles==29&&c.r[3]==0x8c66b0f4&&c.r[4]==2&&c.r[5]==0x8cbc8040&&c.r[6]==0x2000&&c.r[7]==0x2020,"Captured getters continuation differs");}else ++controls;
 };
 for(unsigned offset:{28u,32u,36u})for(unsigned value:{0u,0xfffff800u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(offset,value,alias,placement,false);
 check(28,0,0,0,true);need(controls==36,"Getter coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":36,\"resident_entries\":19,\"new_original_bodies\":3,\"new_original_instructions\":6,\"captured_cycles\":[3,29],\"missing_target\":\"8C66B0F4\",\"three_getter_returns_verified\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident channel getters:36controls;3/29cycles,three original getter returns,nextmissing66B0F4\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
