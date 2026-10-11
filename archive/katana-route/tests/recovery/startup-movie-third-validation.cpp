// Original third callback validation and zero-return guard, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}





#include "movie-third-validation/movie-third-validation-manifest.hpp"
#include "movie-third-guard/movie-third-guard-manifest.hpp"
struct ValidationModel : RequestModel {
 unsigned missing_target=0,route=0;
 std::function<void(unsigned,const ValidationModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void leaf(){c.r[0]=0;c.pc=c.pr;cycles+=3;}
 void error(unsigned literal){c.r[5]=w(literal);cycles+=3;c.pr=0x8ced816e;c.r[4]=0;c.pc=0x8ced816a;cycles+=3;missing_target=c.r[14];}
 void validate(){auto&r=c.r;push(r[14]);r[3]=31;r[4]=unsigned(int(std::int16_t(w(0x8ced8180)&0xffff)));push(c.pr);r[14]=w(0x8ced81a4);t((r[3]&r[4])==0);cycles+=7;
  if(!(c.sr&1)){route=1;error(0x8ced81a8);return;}
  r[3]=w(0x8ced81ac);r[2]=w(0x8ced81b0);r[1]=w(r[3]);t(r[2]==r[1]);cycles+=5;
  if(!(c.sr&1)){route=2;error(0x8ced81b4);return;}
  r[3]=w(0x8ced81b8);need((r[3]&0x1fffffff)==0x0cedab10,"Unexpected leaf target");c.pr=0x8ced8144;cycles+=4;c.pc=0x8cedab10;phase(0);leaf();phase(1);
  t(r[0]==0);cycles+=2;r[3]=w(0x8ced81c0);c.pr=0x8ced8154;c.pc=0x8ced814e;cycles+=4;missing_target=r[3];phase(2);
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_guard(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CEDAB10(c,rt);if(boundary)boundary(1);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned leaf_controls=0,validation_controls=0,activation_controls=0;std::array<unsigned,3> routes{};
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_third_validation_entries),std::end(movie_third_validation_entries));for(auto e:movie_third_guard_entries){e.function=observed_guard;entries.push_back(e);}need(entries.size()==17,"Setup manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced811au,0x8cedab10u,0x8cedc35cu,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced811au,0x8ced811au,0xaced811au,0x8ded811au}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced811a),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced811c);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced811a);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(0x8cedab10,bank?15:0,t,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[0]=value;auto expected=rt.main_ram;ValidationModel m{{c,expected}};m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xacedab10:0x8cedab10);same(m.c,c);need(expected==rt.main_ram,"Leaf modified RAM");verify_devices(rt,vram,timers,start,tocr);++leaf_controls;}
 auto check=[&](unsigned mode,unsigned stack,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced811a,bank?15:0,bank,bank);
  if(!capture){c.pr=alias?0xaced80b8:0x8ced80b8;c.r[15]=stack|(alias?0x20000000u:0);c.r[14]=bank?0xffffffff:0x8cedc60c;c.sr|=bank?0x300:0;
   if(mode==1||mode==3){unsigned value=mode==1?0x2e1:0xffe0;dc_write16(rt,0x8ced8180,value);}
   if(mode==2)put(rt.main_ram,0x8cedcb38,0);
  }
  auto expected=rt.main_ram;ValidationModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const ValidationModel&model){need(i==phases.size(),"Validation prediction order");phases.push_back({model.c,model.m,model.cycles});};m.validate();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Validation observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Validation boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<2);++seen;};
  if(capture)boundary=verify;rejection([&]{call_recompiled(c,rt,alias?0xaced811a:0x8ced811a);},"Inactive or unregistered");boundary={};if(capture)verify(2);same(m.c,c);need(expected==rt.main_ram,"Validation complete RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,3> cycles={16,19,25};need(seen==3&&c.pr==0x8ced8154&&c.pc==0x8ced814e&&c.r[15]==0x8cfffebc,"Captured validation frame differs");for(unsigned i=0;i<3;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured validation timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(c.r[3]==0x8cedc35c&&c.r[14]==0x8ced7ecc&&c.r[4]==0x2e0&&c.r[5]==0x8cee72a0&&word(rt.main_ram,0x8cfffec0)==0x8cedc60c&&word(rt.main_ram,0x8cfffebc)==0x8ced80b8,"Captured validation arguments or saved returns differ");}
  else{need(m.cycles==(m.route==1?13u:m.route==2?18u:25u),"Validation cycles differ");need(m.missing_target==(m.route?0x8ced7eccu:0x8cedc35cu),"Validation target differs");++validation_controls;++routes[m.route];}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired validation entry remains");
 };
 for(unsigned mode:{0u,1u,2u,3u})for(unsigned stack:{0x8cefe000u,0x8ced8188u,0x8cedcb3cu,0x8ced81b8u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(mode,stack,alias,bank,false);
 check(0,0,0,0,true);need(routes[0]&&routes[1]&&routes[2],"Guard coverage missing");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<leaf_controls<<",\"validation_controls\":"<<validation_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"guard_routes\":["<<routes[0]<<","<<routes[1]<<","<<routes[2]<<"],\"captured_connected_cycles\":[16,19,25],\"new_original_bodies\":2,\"new_original_instructions\":51,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CEDC35C\",\"full_startup_connection_verified\":false,\"captured_validation_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie validation: "<<leaf_controls<<" leaf,"<<validation_controls<<" validation,"<<activation_controls<<" activation controls; captured25cycles; explicit missing EDC35C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
