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






namespace dcrecomp_generated {void resident_channel_register(DCRuntime&);}
struct ChannelModel:RequestModel{
 bool run(){auto&r=c.r;for(unsigned i=14;i>=9;--i)push(r[i]);push(c.pr);r[15]-=8;r[9]=r[6];r[13]=0;r[6]=w(0x8c65f448);r[12]=r[5];put(m,r[15],r[4]);r[5]=8;r[10]=w(r[9]);r[4]=0;cycles+=16;for(;;){r[3]=r[4];r[3]<<=2;t(r[3]>>31);r[3]<<=1;r[2]=r[4];r[3]+=r[2];r[3]<<=2;r[3]<<=2;r[3]+=r[6];r[1]=sb(r[3]);t(r[1]==0);cycles+=11;if(c.sr&1)break;++r[4];t(int32_t(r[4])>=int32_t(r[5]));cycles+=3;if(c.sr&1)break;}r[0]=r[4];t(r[0]==8);cycles+=3;if(c.sr&1){r[0]=0;r[15]+=8;c.pr=pop();for(unsigned i=9;i<14;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=12;return false;}r[14]=r[4];r[14]<<=2;t(r[14]>>31);r[14]<<=1;r[3]=r[4];r[14]+=r[3];r[2]=w(0x8c65f454);r[14]<<=2;r[14]<<=2;r[14]+=r[6];c.pr=0x8c65f3bc;r[4]=r[10];c.pc=0x8c65f3a6;cycles+=12;return true;}

};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c65f364;auto device=read_sq(argv[5]);const std::string out=argv[9];std::string last_ram=argv[1];unsigned controls=0,empty=0,ready=0;
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c65f364)&&!rt.targets.contains(0x8c6624d8),"Resident target inventory differs");auto count=rt.targets.size();resident_channel_register(rt);need(rt.targets.size()==count+16,"Resident entries not unique");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto check=[&](unsigned selected,unsigned length,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;auto c=captured?initial:context(0x8c65f364,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=(0x8cf40000+placement)|seg;c.r[5]=length;c.r[6]=(0x8cf60000+placement)|seg;put(rt.main_ram,c.r[6],0x8c8aa868|seg);c.pr=0x8c647f18;for(unsigned i=0;i<8;++i)rt.main_ram[0x8a7b90+144*i]=i<selected?0x80:0;}
  auto expected=rt.main_ram;ChannelModel m{{c,expected}};bool success=m.run();const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](const Prediction&p,bool active){same(p.c,c);need(p.ram==rt.main_ram,"Pool RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Pool timer controls");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Pool SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Pool device effects");if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<seen<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  if(success){bool rejected=false;try{call_recompiled(c,rt,0x8c65f364);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");rejected=true;}need(rejected,"Missing initializer not observed");verify({m.c,m.m,m.cycles},false);if(!captured)++ready;}
  else{call_recompiled(c,rt,0x8c65f364);verify({m.c,m.m,m.cycles},false);need(m.cycles==143,"Full descriptor pool path differs");++empty;}
  if(captured){need(m.cycles==42&&c.r[14]==0x8c8a7b90&&c.r[4]==0x8c8aa868&&c.r[12]==2&&c.r[15]==0x8cfffe00,"Captured descriptor boundary differs");}else ++controls;
 };
 for(unsigned selected:{0u,3u,8u})for(unsigned length:{0u,2u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(selected,length,alias,placement,false);
 check(0,0,0,0,true);need(controls==36&&ready==24&&empty==12,"Descriptor coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":36,\"ready_dispatches\":24,\"empty_returns\":12,\"resident_entries\":16,\"new_original_bodies\":1,\"new_original_instructions\":110,\"captured_cycles\":[42],\"missing_target\":\"8C6624D8\",\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident channel:36controls,24ready/12full;42cycles to missing6624D8;original full-pool return verified\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
