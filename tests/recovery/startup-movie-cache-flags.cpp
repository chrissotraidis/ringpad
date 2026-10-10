// Original three-pair validation return and cache flags, stopped at the real next helper.
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





#include "movie-triple-check/movie-triple-check-manifest.hpp"
#include "movie-cache-flags/movie-cache-flags-manifest.hpp"
#include "movie-pair-compare/movie-pair-compare-manifest.hpp"
#include "movie-third-validation/movie-third-validation-manifest.hpp"
#include "movie-third-setup/movie-third-setup-manifest.hpp"
struct CacheModel : RequestModel {
 unsigned comparisons=0,missing_target=0,cache_value=0,mmio_reads=0;
 std::function<void(unsigned,const CacheModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void compare(){auto&r=c.r;r[5]-=r[4];t((r[5]&0x80000000u)==0);r[1]=0xfffffff4;r[0]=r[5];cycles+=5;if(!(c.sr&1)){r[0]=0u-r[0];++cycles;}t((r[1]&r[0])==0);c.pc=c.pr;r[0]=c.sr&1;cycles+=4;}
 void check(){push(c.pr);++cycles;bool failed=false;const unsigned returns[]={0x8cedc39e,0x8cedc3a8,0x8cedc3b2};
  for(unsigned i=0;i<3;++i){c.r[4]=w(0x8cedc3d0+4*i);c.r[5]=w(0x8cedc3c4+4*i);c.pr=returns[i];c.pc=0x8cedc348;cycles+=5;phase(2*i);compare();++comparisons;phase(2*i+1);++cycles;if(!(c.sr&1)){failed=true;break;}}
  c.pr=pop();c.pc=c.pr;c.r[0]=failed?0xffffffffu:0;cycles+=4;phase(6);
 }
 void flags(){auto&r=c.r;r[4]=w(0x8ced8308);r[0]=96;r[5]=0;r[3]=w(0x8ced830c);put(m,r[0]+r[4],r[5]);r[0]=100;put(m,r[0]+r[4],r[5]);need(r[3]==0xff00001c,"Unexpected cache register");r[5]=cache_value;++mmio_reads;r[2]=w(0x8ced8310);t((r[5]&r[2])==0);r[6]=1;cycles+=12;if(!(c.sr&1)){r[0]=96;put(m,r[0]+r[4],r[6]);cycles+=2;}
  r[3]=unsigned(int(std::int16_t(w(0x8ced82f0)&0xffff)));t((r[3]&r[5])==0);cycles+=3;if(!(c.sr&1)){r[0]=100;put(m,r[0]+r[4],r[6]);cycles+=2;}c.pc=c.pr;cycles+=3;
 }
 void continuation(){need(c.pc==0x8ced8164,"Wrong saved validation continuation");t(c.r[0]==0);cycles+=2;
  if(!(c.sr&1)){c.r[5]=w(0x8ced8184);++cycles;c.pr=0x8ced816e;c.r[4]=0;c.pc=0x8ced816a;cycles+=3;missing_target=c.r[14];return;}
  c.r[0]=0;c.pr=pop();c.pc=c.pr;c.r[14]=pop();cycles+=5;phase(7);need(c.pc==0x8ced80b8,"Wrong saved setup continuation");
  t(c.r[0]==0);c.r[4]=c.r[0];cycles+=3;need(c.sr&1,"Original validation returned nonzero");c.pr=0x8ced80d6;c.pc=0x8ced81cc;cycles+=3;phase(8);flags();phase(9);c.pr=0x8ced80da;c.r[4]=w(c.r[15]+4);c.pc=0x8ced80d6;cycles+=3;missing_target=0x8ced8206;phase(10);
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles,mmio_reads;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_triple_compare(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8cedc39e?0:c.pr==0x8cedc3a8?2:4;if(boundary)boundary(phase);movie_recomp_8CEDC348(c,rt);if(boundary)boundary(phase+1);}
static void observed_flags(SH4Context&c,DCRuntime&rt){if(boundary)boundary(8);movie_recomp_8CED81CC(c,rt);if(boundary)boundary(9);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
static unsigned cache_from_state(const char*p){std::ifstream f(p);std::string l;while(std::getline(f,l))if(l.starts_with("cache_shadow="))return std::stoul(l.substr(13),nullptr,16);throw std::runtime_error("Missing captured cache register");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned flag_controls=0,pair_controls=0,connected_controls=0,activation_controls=0;std::array<unsigned,4> returns{};const auto captured_cache=cache_from_state(argv[2]);need(captured_cache==0x98d,"Captured cache input differs");
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_triple_check_entries),std::end(movie_triple_check_entries));for(auto e:movie_cache_flags_entries){e.function=observed_flags;entries.push_back(e);}for(auto e:movie_pair_compare_entries){e.function=observed_triple_compare;entries.push_back(e);}for(auto e:movie_third_validation_entries)entries.push_back(e);for(auto e:movie_third_setup_entries)entries.push_back(e);need(entries.size()==48,"Cache manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8cedc394u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.mmio32[0x1f00001c]=captured_cache;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cedc394u,0x8cedc394u,0xacedc394u,0x8dedc394u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cedc394),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cedc396);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cedc394);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};
 for(unsigned cache:{0u,0x80u,0x8000u,0x8080u,0x98du,0xffffffffu,0x7fu,0x80000000u})for(unsigned dest:{0x8cefb908u,0x8ced8290u,0x8ced82b0u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);rt.mmio32[0x1f00001c]=cache;dc_write32(rt,0x8ced8308,dest|(alias?0x20000000u:0));auto c=context(0x8ced81cc,bank?15:0,bank,bank);c.pr=alias?0xacedabce:0x8cedabce;auto expected=rt.main_ram;CacheModel m{{c,expected}};m.cache_value=cache;m.flags();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;auto map=rt.mmio32;auto reads=rt.mmio_reads,writes=rt.mmio_writes;call_recompiled(c,rt,alias?0xaced81cc:0x8ced81cc);same(m.c,c);need(expected==rt.main_ram,"Cache flag RAM differs");need(rt.mmio32==map&&rt.mmio_reads==reads+1&&rt.mmio_writes==writes,"Cache register read effects differ");verify_devices(rt,vram,timers,start,tocr);++flag_controls;}
 auto check=[&](unsigned fail_index,bool negative,unsigned stack,unsigned alias,unsigned bank,unsigned cache,bool connected,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);rt.mmio32[0x1f00001c]=cache;auto c=capture?initial:context(0x8cedc394,bank?15:0,bank,bank);
  if(!capture){c.pr=connected?0x8ced8164:alias?0xacedabce:0x8cedabce;c.r[15]=stack|(alias?0x20000000u:0);c.r[14]=0x8ced7ecc;c.sr|=bank?0x300:0;if(fail_index<3){unsigned a=0x8cedc3c4+4*fail_index;dc_write32(rt,a,word(rt.main_ram,0x8cedc3d0+4*fail_index)+(negative?0xfffffffcu:4u));}if(connected){put(rt.main_ram,c.r[15],0x8ced80b8);put(rt.main_ram,c.r[15]+4,0x8cedc60c);put(rt.main_ram,c.r[15]+12,0x8cee72a0);}}
  auto expected=rt.main_ram;CacheModel m{{c,expected}};m.cache_value=cache;std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const CacheModel&model){need(i==phases.size(),"Cache prediction order");phases.push_back({model.c,model.m,model.cycles,model.mmio_reads});};m.check();if(connected)m.continuation();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;auto map=rt.mmio32;auto reads=rt.mmio_reads,writes=rt.mmio_writes;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Cache observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Cache boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<6||i==8||i==9);need(rt.mmio32==map&&rt.mmio_reads==reads+p.mmio_reads&&rt.mmio_writes==writes,"Cache boundary MMIO differs");++seen;};
  if(capture)boundary=verify;call_recompiled(c,rt,alias?0xacedc394:0x8cedc394);if(capture)verify(6);if(connected){if(c.r[0])rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");else{call_recompiled(c,rt,c.pc);if(capture)verify(7);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");}}boundary={};if(capture)verify(10);same(m.c,c);need(expected==rt.main_ram,"Cache complete RAM differs");verify_devices(rt,vram,timers,start,tocr);need(rt.mmio32==map&&rt.mmio_reads==reads+m.mmio_reads&&rt.mmio_writes==writes,"Cache MMIO differs");
  if(capture){const std::array<unsigned,11> cycles={6,15,21,30,36,45,50,57,63,83,86};need(seen==11&&c.pr==0x8ced80da&&c.pc==0x8ced80d6&&c.r[15]==0x8cfffec4,"Captured validation return differs");for(unsigned i=0;i<11;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured cache timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);std::ofstream(stem+"-cache.json")<<"{\"cache_shadow\":2445,\"original_mmio_reads\":"<<p.mmio_reads<<"}\n";}need(c.r[4]==0x8cee72a0&&c.r[5]==cache&&c.r[0]==100&&c.r[14]==0x8cedc60c&&word(rt.main_ram,0x8cfffeb8)==0x8ced8164&&word(rt.main_ram,0x8cefb968)==0&&word(rt.main_ram,0x8cefb96c)==1,"Captured cache values differ");}
  else{if(connected){need(m.comparisons==(fail_index<3?fail_index+1:3),"Wrong triple failure");need(m.missing_target==(fail_index<3?0x8ced7eccu:0x8ced8206u),"Wrong cache continuation target");++connected_controls;}else{++pair_controls;++returns[c.r[0]==0?3:m.comparisons-1];}}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired cache entry remains");
 };
 for(unsigned index:{0u,1u,2u,3u})for(bool negative:{false,true})for(unsigned stack:{0x8cefe000u,0x8cedc3d4u,0x8cedc3c8u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(index,negative,stack,alias,bank,captured_cache,false,false);
 for(unsigned index:{0u,1u,2u,3u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(index,false,0x8cefe000,alias,bank,captured_cache,true,false);
 for(unsigned cache:{0u,0x80u,0x8000u,0x8080u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(3,false,0x8cefe000,alias,bank,cache,true,false);
 check(3,false,0,0,0,captured_cache,true,true);for(auto n:returns)need(n,"Triple branch coverage missing");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"flag_controls\":"<<flag_controls<<",\"pair_controls\":"<<pair_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[6,15,21,30,36,45,50,57,63,83,86],\"new_original_bodies\":2,\"new_original_instructions\":43,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"captured_cache_shadow\":2445,\"captured_original_MMIO_reads\":1,\"MMIO_backing_unchanged\":true,\"next_missing_target\":\"8CED8206\",\"full_startup_connection_verified\":false,\"captured_validation_return_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie cache: "<<flag_controls<<" flags,"<<pair_controls<<" pairs,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured86cycles; explicit missing ED8206\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
