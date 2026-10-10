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





#include "movie-instance-fourth-setup/movie-instance-fourth-setup-manifest.hpp"
#include "movie-instance-fourth-handler/movie-instance-fourth-handler-manifest.hpp"
#include "movie-instance-indexed-query/movie-instance-indexed-query-manifest.hpp"
struct FourthSetupModel:RequestModel{
 std::function<void(unsigned,const FourthSetupModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 void copy28(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;need(r[3]==0x8c10e99a&&r[0]==24,"Unexpected28-byte copy dispatch");r[3]=w(r[2]+r[0]);
  for(int offset=20;offset>=0;offset-=4){const unsigned reg=(offset&4)?0:3,previous=reg==0?3:0;r[reg]=w(r[2]+unsigned(offset));put(m,r[1]+unsigned(offset)+4,r[previous]);}put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=23;
 }
 void prefix(){auto&r=c.r;r[6]=w(0x8ced14a4);push(c.pr);r[3]=w(r[6]+8);t(r[3]==0);cycles+=5;bool fail=c.sr&1;if(!fail){r[3]=w(r[6]+24);t(r[3]==0);cycles+=3;fail=c.sr&1;}if(fail){r[3]=w(0x8ced149c);r[5]=w(0x8ced14a8);c.pr=pop();c.pc=0x8ced140c;cycles+=5;return;}r[3]=w(0x8ced14ac);r[1]=r[5]+8;r[2]=r[6];c.pr=0x8ced1420;r[0]=28;c.pc=0x8c10e964;cycles+=7;mark(0);copy28();mark(1);r[2]=0xffffffff;r[3]=1;r[4]=0;put(m,r[5],r[4]);put(m,r[5]+4,r[4]);put(m,r[5]+36,r[2]);put(m,r[5]+40,r[3]);put(m,r[5]+44,r[4]);put(m,r[5]+48,r[4]);c.pr=pop();c.pc=c.pr;r[0]=r[4];cycles+=13;mark(2);}
 void next(){auto&r=c.r;r[4]=r[0];t(r[4]==0);need(c.sr&1,"Captured setup failed");r[2]=w(0x8ced139c);r[5]=27;c.pr=0x8ced1364;r[4]=r[14];cycles+=8;r[3]=unsigned(int(std::int16_t(w(0x8ced456e))));r[0]=r[5]<<2;r[3]+=r[4];r[0]=w(r[3]+r[0]);cycles+=7;r[5]=r[0];c.pr=0x8ced136a;r[4]=r[11];c.pc=0x8ced1364;cycles+=4;mark(3);}

};
static thread_local std::function<void(unsigned)> fourth_setup_boundary;
static thread_local RecompiledFunction real_copy=nullptr;
static void observed_fourth_setup_copy(SH4Context&c,DCRuntime&rt){if(fourth_setup_boundary)fourth_setup_boundary(0);real_copy(c,rt);if(fourth_setup_boundary)fourth_setup_boundary(1);}


struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced13fc;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_fourth_setup_entries),std::end(movie_instance_fourth_setup_entries));for(auto e:movie_instance_fourth_handler_entries)entries.push_back(e);for(auto e:movie_instance_indexed_query_entries)entries.push_back(e);need(entries.size()==23,"Video setup manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10e964)&&!rt.targets.contains(0x8ced1438),"Copy/missing initializer inventory differs");real_copy=rt.targets.at(0x8c10e964);rt.targets.at(0x8c10e964)=observed_fourth_setup_copy;for(unsigned target:{0x8ced13fcu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced13fcu,0x8ced13fcu,0xaced13fcu,0x8ded13fcu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced13fc),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced13fe);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced13fc);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned successes=0,failures=0;std::string last_ram=argv[1];
 auto check=[&](unsigned flags,unsigned layout,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced13fc,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=0x8ced1352;c.r[4]=(0x8cf40000+placement)|seg;unsigned src=(0x8cf70000+placement)|seg;c.r[5]=layout==1?src:layout==2?c.r[15]-40:(0x8cf60000+placement)|seg;
   for(unsigned i=0;i<28;i+=4)put(rt.main_ram,src+i,0x12340000+i);put(rt.main_ram,src+8,(flags&1)?1:0);put(rt.main_ram,src+24,(flags&2)?2:0);dc_write32(rt,0x8ced14a4,src);dc_write32(rt,0x8ced14ac,0x8c10e964|seg);
  }
  auto expected=rt.main_ram;FourthSetupModel m{{c,expected}};std::vector<Prediction> phases;m.capture=[&](unsigned i,const FourthSetupModel&v){need(i==phases.size(),"Video setup prediction order differs");phases.push_back({v.c,v.m,v.cycles});};m.prefix();if(captured)m.next();const bool success=!phases.empty();
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  fourth_setup_boundary=[&](unsigned which){need(which==seen&&seen<phases.size(),"Video setup observed order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Video setup boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,which<2);if(captured){auto stem=out+"/phase"+std::to_string(which)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<which<<"\t"<<(shared?"clone-verified":"write-verified")<<"\n";save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  if(success){call_recompiled(c,rt,alias?0xaced13fc:0x8ced13fc);fourth_setup_boundary(2);need(seen==3,"Setup captures incomplete");if(captured){rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");fourth_setup_boundary(3);}else ++successes;}
  else{rejection([&]{call_recompiled(c,rt,alias?0xaced13fc:0x8ced13fc);},"Inactive or unregistered");same(m.c,c);need(expected==rt.main_ram&&seen==0,"Error path RAM/copy differs");auto timers=initial_timers;tick(timers,start,m.cycles);verify_devices(rt,vram,timers,start,tocr);need(c.r[3]==0x8ced1aa2&&c.r[5]==0xff000c06&&c.pc==0x8ced140c,"Error tail target differs");++failures;}

  fourth_setup_boundary={};
  if(captured){need(phases[0].cycles==15&&phases[1].cycles==38&&phases[2].cycles==51&&m.cycles==70&&c.r[5]==60&&c.r[4]==0x8cee497c,"Captured video setup cycles/slot differ");}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired setup entry remains");need(rt.targets.contains(0x8c10e964),"Resident copy retired with module");
 };
 for(unsigned flags:{0u,1u,2u,3u})for(unsigned layout:{0u,1u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(flags,layout,alias,placement,false);
 check(3,0,0,0,true);need(prefix_controls==48&&successes==12&&failures==36,"Video setup coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[15,38,51,70],\"new_original_bodies\":1,\"new_original_instructions\":30,\"successful_setups\":12,\"error_tails\":36,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_28byte_copy_return_verified\":true,\"missing_target\":\"8CED1438\",\"full_instance_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original fourth setup: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; real28-byte copy and flag stores;captured15/38/51/70cycles to missingED1438;36original error tails\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
