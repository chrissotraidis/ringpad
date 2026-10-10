// Original post buffer builder and retained parent, stopped at the real next helper.
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





#include "movie-post-buffer/movie-post-buffer-manifest.hpp"
#include "movie-post-work/movie-post-work-manifest.hpp"
#include "movie-third-setup/movie-third-setup-manifest.hpp"
#include "movie-post-buffer-model.hpp"
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced8216;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned leaf_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_post_buffer_entries),std::end(movie_post_buffer_entries));for(auto e:movie_post_work_entries)entries.push_back(e);for(auto e:movie_third_setup_entries)entries.push_back(e);need(entries.size()==21,"Post buffer manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced8246u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced8246u,0x8ced8246u,0xaced8246u,0x8ded8246u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced8246),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced8248);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced8246);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};


 auto fixture=[&](DCRuntime&rt,SH4Context&c,unsigned placement,unsigned count,unsigned alias){
  unsigned seg=alias?0x20000000u:0,buffer=(placement==2?0x8cefbf04-0x170:placement==3?0x8ced8300:0x8cf50000)|seg;
  if(placement==1)++buffer;c.r[15]=(0x8cf90000+(placement==1?1:0))|seg;c.r[4]=buffer;
  dc_write32(rt,0x8ced8308,0x8cf40000|seg);dc_write32(rt,0x8cf40070,count);dc_write32(rt,0x8cf40074,buffer);dc_write16(rt,0x8ced82f2,0x2000);
  if(placement==3)dc_write16(rt,0x8ced82f4,0xfffc);
 };
 auto check=[&](unsigned placement,unsigned count,unsigned alias,unsigned bank,bool connected,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(connected?0x8ced8216:0x8ced8246,bank?15:0,bank,bank);
  if(!capture){c.pr=connected?0x8ced8108:0x8ced8236;fixture(rt,c,placement,count,alias);}
  if(!connected){c.pc=0x8ced8246;c.pr=0x8ced8236;if(capture)c.r[4]=word(rt.main_ram,0x8cefb97c);}
  auto expected=rt.main_ram;PostBufferModel m{{c,expected}};if(connected)m.parent();else m.leaf();
  auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto original_timers=rt.tmu,timers=original_timers;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,(connected?0x8ced8216:0x8ced8246)|(alias?0x20000000u:0));same(m.c,c);need(expected==rt.main_ram,"Post buffer whole RAM differs");verify_devices(rt,vram,timers,start,tocr);
  auto save=[&](unsigned phase){auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);};
  if(capture){need(m.cycles==(connected?722:80),"Captured buffer timing differs");save(connected?1:0);if(connected)need(c.pc==0x8ced8108&&c.pr==initial.pr&&c.r[12]==initial.r[12]&&c.r[13]==initial.r[13]&&c.r[14]==initial.r[14]&&c.r[15]==initial.r[15],"Parent saved registers differ");}
  if(connected){m.continuation();timers=original_timers;tick(timers,start,m.cycles);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");same(m.c,c);need(expected==rt.main_ram,"Caller changed RAM");verify_devices(rt,vram,timers,start,tocr);if(capture){need(m.cycles==727&&c.pr==0x8ced8110&&c.pc==0x8ced8108&&c.r[3]==0x8ceda7c4&&c.r[4]==0x8eef95c0,"Captured next call differs");save(2);}else ++connected_controls;}
  else if(!capture)++leaf_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired entry remains");
 };
 for(unsigned placement:{0u,1u,2u,3u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(placement,1,alias,bank,false,false);
 for(unsigned placement:{0u,1u,2u})for(unsigned count:{0u,1u,3u,8u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(placement,count,alias,bank,true,false);
 check(0,8,0,0,false,true);check(0,8,0,0,true,true);
 need(leaf_controls==16&&connected_controls==48&&activation_controls==9,"Control counts differ");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":16,\"connected_controls\":48,\"activation_controls\":9,\"activation_entries\":21,\"captured_leaf_cycles\":80,\"captured_parent_cycles\":722,\"captured_connected_cycles\":727,\"new_original_instructions\":79,\"captured_records\":8,\"stores_per_record\":22,\"captured_store_bytes\":704,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"complete_parent_return_verified\":true,\"next_missing_target\":\"8CEDA7C4\",\"full_startup_connection_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original post buffer:16leaf/48connected/9activation controls;8records/704store bytes;80leaf/722parent/727nextcall cycles; missing EDA7C4\n";

}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
