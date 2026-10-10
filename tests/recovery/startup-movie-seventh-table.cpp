// Original seventh table builder and retained parent, stopped at the real next helper.
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





#include "movie-tail-helper/movie-tail-helper-manifest.hpp"
#include "movie-seventh-table/movie-seventh-table-manifest.hpp"
#include "movie-seventh-table-model.hpp"
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned table_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_seventh_table_entries),std::end(movie_seventh_table_entries));for(auto e:movie_tail_helper_entries)entries.push_back(e);need(entries.size()==207,"Seventh table manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8cedb486u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cedb486u,0x8cedb486u,0xacedb486u,0x8dedb486u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cedb486),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cedb488);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cedb486);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 auto fixture=[&](DCRuntime&rt,SH4Context&c,unsigned placement,unsigned variant,unsigned alias){
  c.r[15]=0x8cf30000|(alias?0x20000000u:0);
  // Own twelve-byte frame stays disjoint. Placement3 starts at the caller SP;
  // placement4 changes a late literal through early stores, after current code.
  unsigned dest=placement==0?0x8cefc608:placement==3?c.r[15]:placement==4?0x8cedbe50:0x8cf20020;
  if(placement==2)dest|=0x20000000u;if(placement==1)++dest;
  dc_write32(rt,0x8cedb570,dest);dc_write32(rt,0x8cedabd0,alias?0xacedbd9c:0x8cedbd9c);
  for(const auto&g:SeventhTableModel::groups){auto a=g[0];
   if(variant==1)dc_write16(rt,a,0x8001u+(a&15));
   if(variant==2)dc_write16(rt,a,0xffffu);
   if(variant==3)dc_write16(rt,a,0);
  }
 };
 for(unsigned placement:{0u,1u,2u,3u,4u})for(unsigned variant:{0u,1u,2u,3u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(0x8cedb486,bank?15:0,bank,bank);c.pr=alias?0xacedabe6:0x8cedabe6;fixture(rt,c,placement,variant,alias);auto expected=rt.main_ram;SeventhTableModel m{{c,expected}};m.table();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xacedb486:0x8cedb486);same(m.c,c);need(expected==rt.main_ram,"Seventh table full RAM differs");verify_devices(rt,vram,timers,start,tocr);need(m.cycles==6726,"Original table timing differs");++table_controls;
 }
 auto check=[&](unsigned placement,unsigned variant,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8cedb486,bank?15:0,bank,bank);
  if(!capture){c.pr=0x8cedab34;fixture(rt,c,placement,variant,alias);}
  auto expected=rt.main_ram;SeventhTableModel m{{c,expected}};std::vector<Prediction>phases;if(capture)m.observe=[&](unsigned i,const SeventhTableModel&model){need(i==phases.size(),"Seventh table prediction order");phases.push_back({model.c,model.m,model.cycles});};m.connected();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Seventh table observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Seventh table boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr);++seen;};
  call_recompiled(c,rt,alias?0xacedb486:0x8cedb486);if(capture)verify(0);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");if(capture)verify(1);same(m.c,c);need(expected==rt.main_ram,"Seventh table connected RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,2>cycles={6726,6730};need(seen==2&&c.pr==0x8cedab3a&&c.pc==0x8cedab34&&c.r[15]==0x8cfffebc&&c.r[0]==0x3c03&&c.r[2]==0x8cedbd9c,"Captured seventh table return differs");for(unsigned i=0;i<2;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured seventh table timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(word(rt.main_ram,0x8cfffec0)==0x8ced80f6&&word(rt.main_ram,0x8cfffebc)==0x8eef9650,"Caller locals differ");}
  else ++connected_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired seventh table entry remains");
 };
 for(unsigned placement:{0u,1u,2u,3u,4u})for(unsigned variant:{0u,1u,2u,3u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(placement,variant,alias,bank,false);
 check(0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"table_controls\":"<<table_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[6726,6730],\"new_original_bodies\":1,\"new_original_instructions\":1092,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"captured_table_bytes\":[1024],\"captured_table_cycles\":6726,\"next_missing_target\":\"8CEDBD9C\",\"full_startup_connection_verified\":false,\"captured_table_return_verified\":true,\"complete_parent_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original seventh table: "<<table_controls<<" table,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured6730cycles; explicit missing EDBD9C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
