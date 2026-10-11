// Original four-pair check and signed comparison, stopped at the real next helper.
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





#include "movie-pair-check/movie-pair-check-manifest.hpp"
#include "movie-pair-compare/movie-pair-compare-manifest.hpp"
#include "movie-third-validation/movie-third-validation-manifest.hpp"
struct PairModel : RequestModel {
 unsigned comparisons=0,missing_target=0;
 std::function<void(unsigned,const PairModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void compare(){auto&r=c.r;r[5]-=r[4];t((r[5]&0x80000000u)==0);r[1]=0xfffffff4;r[0]=r[5];cycles+=5;if(!(c.sr&1)){r[0]=0u-r[0];++cycles;}t((r[1]&r[0])==0);c.pc=c.pr;r[0]=c.sr&1;cycles+=4;}
 void check(){push(c.pr);++cycles;bool failed=false;const unsigned returns[]={0x8cedc366,0x8cedc370,0x8cedc37a,0x8cedc384};
  for(unsigned i=0;i<4;++i){c.r[4]=w(0x8cedc3e0+4*i);c.r[5]=w(0x8cedc3f0+4*i);c.pr=returns[i];c.pc=0x8cedc348;cycles+=5;phase(2*i);compare();++comparisons;phase(2*i+1);++cycles;if(!(c.sr&1)){failed=true;break;}}
  c.pr=pop();c.pc=c.pr;c.r[0]=failed?0xffffffffu:0;cycles+=4;phase(8);
 }
 void continuation(){need(c.pc==0x8ced8154,"Wrong saved continuation");t(c.r[0]==0);cycles+=2;if(c.sr&1){c.r[3]=w(0x8ced81c8);c.pr=0x8ced8164;c.pc=0x8ced815e;cycles+=4;missing_target=c.r[3];}
  else{c.r[5]=w(0x8ced81c4);cycles+=3;c.pr=0x8ced816e;c.r[4]=0;c.pc=0x8ced816a;cycles+=3;missing_target=c.r[14];}phase(9);
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_pair(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8cedc366?0:c.pr==0x8cedc370?2:c.pr==0x8cedc37a?4:6;if(boundary)boundary(phase);movie_recomp_8CEDC348(c,rt);if(boundary)boundary(phase+1);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned leaf_controls=0,pair_controls=0,connected_controls=0,activation_controls=0;std::array<unsigned,5> returns{};
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_pair_check_entries),std::end(movie_pair_check_entries));for(auto e:movie_pair_compare_entries){e.function=observed_pair;entries.push_back(e);}for(auto e:movie_third_validation_entries)entries.push_back(e);need(entries.size()==29,"Pair manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8cedc35cu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cedc35cu,0x8cedc35cu,0xacedc35cu,0x8dedc35cu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cedc35c),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cedc35e);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cedc35c);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};
 const std::array<unsigned,20> differences={0,1,2,3,4,7,8,9,10,11,12,16,0x7fffffff,0x80000000,0xffffffff,0xfffffffe,0xfffffffd,0xfffffffc,0xfffffff5,0xfffffff4};
 for(unsigned diff:differences)for(unsigned base:{0u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(0x8cedc348,bank?15:0,bank,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[4]=base;c.r[5]=base+diff;auto expected=rt.main_ram;PairModel m{{c,expected}};m.compare();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xacedc348:0x8cedc348);same(m.c,c);need(expected==rt.main_ram,"Pair leaf modified RAM");verify_devices(rt,vram,timers,start,tocr);++leaf_controls;}
 auto check=[&](unsigned fail_index,bool negative,unsigned stack,unsigned alias,unsigned bank,bool connected,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8cedc35c,bank?15:0,bank,bank);
  if(!capture){c.pr=connected?0x8ced8154:alias?0xacedabce:0x8cedabce;c.r[15]=stack|(alias?0x20000000u:0);c.r[14]=0x8ced7ecc;c.sr|=bank?0x300:0;if(fail_index<4){unsigned a=0x8cedc3f0+4*fail_index;dc_write32(rt,a,word(rt.main_ram,a)+(negative?0xfffffffcu:4u));}}
  auto expected=rt.main_ram;PairModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const PairModel&model){need(i==phases.size(),"Pair prediction order");phases.push_back({model.c,model.m,model.cycles});};m.check();if(connected)m.continuation();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Pair observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Pair boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<8);++seen;};
  if(capture)boundary=verify;call_recompiled(c,rt,alias?0xacedc35c:0x8cedc35c);if(capture)verify(8);if(connected)rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");boundary={};if(capture)verify(9);same(m.c,c);need(expected==rt.main_ram,"Pair complete RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,10> cycles={6,15,21,30,36,45,51,60,65,71};need(seen==10&&c.pr==0x8ced8164&&c.pc==0x8ced815e&&c.r[15]==0x8cfffebc,"Captured pair return differs");for(unsigned i=0;i<10;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured pair timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(c.r[3]==0x8cedc394&&c.r[0]==0&&c.r[1]==0xfffffff4&&c.r[4]==0&&c.r[5]==0&&word(rt.main_ram,0x8cfffeb8)==0x8ced8154,"Captured pair values differ");}
  else{if(connected){need(m.comparisons==(fail_index<4?fail_index+1:4),"Wrong early-failure pair");need(m.missing_target==(fail_index<4?0x8ced7eccu:0x8cedc394u),"Wrong continuation target");++connected_controls;}else{++pair_controls;++returns[c.r[0]==0?4:m.comparisons-1];}}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired pair entry remains");
 };
 for(unsigned index:{0u,1u,2u,3u,4u})for(bool negative:{false,true})for(unsigned stack:{0x8cefe000u,0x8cedc3e4u,0x8cedc3f4u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(index,negative,stack,alias,bank,false,false);
 for(unsigned index:{0u,1u,2u,3u,4u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(index,false,0x8cefe000,alias,bank,true,false);
 check(4,false,0,0,0,true,true);for(auto n:returns)need(n,"Pair branch coverage missing");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<leaf_controls<<",\"pair_controls\":"<<pair_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[6,15,21,30,36,45,51,60,65,71],\"new_original_bodies\":2,\"new_original_instructions\":36,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CEDC394\",\"full_startup_connection_verified\":false,\"captured_pair_check_return_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie pairs: "<<leaf_controls<<" leaf,"<<pair_controls<<" pair,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured71cycles; explicit missing EDC394\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
