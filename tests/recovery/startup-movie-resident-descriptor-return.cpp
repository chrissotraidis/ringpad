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






namespace dcrecomp_generated {void resident_descriptor_register(DCRuntime&);void resident_pool_register(DCRuntime&);void resident_descriptor_return_register(DCRuntime&);}
struct DescriptorModel:RequestModel{
 bool run(){auto&r=c.r;push(r[14]);r[0]=64;push(c.pr);r[14]=w(0x8c6621e4);r[7]=0;cycles+=5;for(;;){r[3]=r[7];t(r[3]>>31);r[3]<<=1;r[2]=r[7];r[3]+=r[2];r[3]<<=2;r[3]<<=2;r[3]+=r[14];r[1]=w(r[3]+4);t(r[1]==0);cycles+=10;if(c.sr&1)break;++r[7];t(int32_t(r[7])>=int32_t(r[0]));cycles+=3;if(c.sr&1)break;}r[0]=r[7];t(r[0]==64);cycles+=4;if(c.sr&1){c.pr=pop();r[0]=0;c.pc=c.pr;r[14]=pop();cycles+=5;return false;}r[3]=r[7];t(r[7]>>31);r[7]<<=1;r[7]+=r[3];r[3]=w(0x8c6621ec);r[7]<<=2;r[7]<<=2;r[2]=1;r[14]+=r[7];put(m,r[14]+4,r[2]);put(m,r[14],r[3]);r[3]=w(0x8c6621f4);r[2]=w(0x8c6621f0);put(m,r[14]+28,r[4]);put(m,r[14]+32,r[5]);put(m,r[14]+36,r[6]);put(m,r[14]+8,r[2]);put(m,r[14]+40,r[3]);put(m,r[14]+44,r[14]);c.pr=0x8c662162;r[4]=r[14];c.pc=0x8c66213a;cycles+=21;return true;}

};
struct DescriptorReturnModel:DescriptorModel{
 void leaf(){auto&r=c.r;r[5]=0;put(m,r[4]+12,r[5]);r[3]=w(r[4]+32);put(m,r[4]+16,r[3]);put(m,r[4]+20,r[5]);c.pc=c.pr;put(m,r[4]+24,r[5]);cycles+=8;}
 void descriptor_return(){auto&r=c.r;r[0]=r[14];c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;}
 void whole_descriptor(){need(run(),"Captured descriptor pool unexpectedly full");leaf();descriptor_return();}
 void pool_next(){auto&r=c.r;r[3]=r[0];t(r[3]==0);put(m,r[14]+16,r[0]);put(m,r[14]+20,r[0]);need(!(c.sr&1),"Captured descriptor return zero");t(int32_t(r[12])>0);r[11]=r[13];need(c.sr&1,"Captured channel count nonpositive");cycles+=8;
  do{r[3]=r[11];r[2]=r[14];r[3]<<=2;put(m,r[15],r[3]);r[2]+=24;r[3]+=r[2];push(r[3]);r[9]=w(r[14]+52);r[8]=w(r[14]+48);c.macl=r[9]*r[11];r[6]=r[9];r[5]=r[8];r[6]-=r[8];t(r[6]>>31);r[6]<<=1;r[3]=w(r[14]+44);r[4]=c.macl;t(r[5]>>31);r[5]<<=1;t(r[4]>>31);r[4]<<=1;c.pr=0x8c647f50;r[4]+=r[3];cycles+=22;whole_descriptor();r[2]=pop();put(m,r[2],r[0]);r[2]=r[14];r[3]=w(r[15]);r[2]+=24;r[2]+=r[3];r[1]=w(r[2]);t(r[1]==0);need(!(c.sr&1),"Captured channel descriptor zero");++r[11];t(int32_t(r[11])>=int32_t(r[12]));cycles+=12;}while(!(c.sr&1));
  r[3]=w(0x8c647fa4);r[6]=r[14];r[5]=r[12];r[6]+=24;c.pr=0x8c647f74;r[4]=w(r[14]+20);c.pc=0x8c647f68;cycles+=7;
 }
};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);auto initial=state(argv[2]);initial.pc=0x8c662194;auto device=read_sq(argv[5]);const std::string out=argv[9];std::string last_ram=argv[1];unsigned controls=0,empty=0,ready=0;
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c662106)&&!rt.targets.contains(0x8c662194)&&!rt.targets.contains(0x8c647e7c)&&!rt.targets.contains(0x8c65f364),"Resident target inventory differs");auto count=rt.targets.size();resident_descriptor_register(rt);resident_pool_register(rt);resident_descriptor_return_register(rt);need(rt.targets.size()==count+32,"Resident entries not unique");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto check=[&](unsigned length,unsigned alias,unsigned placement,unsigned overlap,bool captured){
  auto owner=make();auto&rt=*owner;auto c=captured?initial:context(0x8c662194,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=overlap?c.r[15]-20:(0x8cf40000+placement)|seg;c.pr=0x8c662162;put(rt.main_ram,c.r[4]+32,length);}
  auto expected=rt.main_ram;DescriptorReturnModel m{{{c,expected}}};m.leaf();std::vector<Prediction> phases{{m.c,m.m,m.cycles}};if(captured){m.descriptor_return();phases.push_back({m.c,m.m,m.cycles});m.pool_next();phases.push_back({m.c,m.m,m.cycles});}const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](const Prediction&p,bool active){same(p.c,c);need(p.ram==rt.main_ram,"Pool RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Pool timer controls");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Pool SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Pool device effects");if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<seen<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  call_recompiled(c,rt,0x8c662194);verify(phases[0],false);++seen;
  if(captured){call_recompiled(c,rt,c.pc);verify(phases[1],false);++seen;bool rejected=false;try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error&e){need(std::string(e.what()).find("No recompiled/native target")!=std::string::npos,"Wrong next failure");rejected=true;}need(rejected,"Next missing caller absent");verify(phases[2],false);need(phases[0].cycles==8&&phases[1].cycles==14&&m.cycles==244&&c.r[3]==0x8c65f364,"Captured descriptor continuation differs");}else ++controls;
 };
 for(unsigned length:{0u,36u,0xfffff800u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})for(unsigned overlap:{0u,1u})check(length,alias,placement,overlap,false);
 check(0,0,0,0,true);need(controls==32,"Descriptor return coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":32,\"resident_entries\":32,\"new_original_bodies\":1,\"new_original_instructions\":7,\"captured_cycles\":[8,14,244],\"missing_target\":\"8C65F364\",\"three_descriptor_returns_verified\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident descriptor return:32controls;8/14/244cycles,three original descriptors return,nextmissing65F364\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
