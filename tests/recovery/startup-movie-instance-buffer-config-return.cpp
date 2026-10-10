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





#include "movie-instance-buffer-config-return/movie-instance-buffer-config-return-manifest.hpp"
#include "movie-instance-buffer-config/movie-instance-buffer-config-manifest.hpp"
#include "movie-instance-buffer-acquire/movie-instance-buffer-acquire-manifest.hpp"
struct BufferConfigReturnModel:RequestModel{
 unsigned sw(unsigned a){return static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(w(a))));}
 void leaf(){auto&r=c.r;r[5]=w(0x8cedc46c);r[3]=w(r[5]);put(m,r[4],r[3]);r[2]=w(r[5]+4);put(m,r[4]+4,r[2]);r[3]=w(r[5]+8);put(m,r[4]+8,r[3]);r[2]=w(r[5]+12);c.pc=c.pr;put(m,r[4]+12,r[2]);cycles+=11;}
 void config_return(){auto&r=c.r;r[4]=w(r[15]);r[15]+=4;c.pr=pop();cycles+=4;r[15]-=16;r[0]=sw(0x8cedaace);r[7]=sw(0x8cedaaca);r[2]=w(r[4]+r[0]);r[7]+=r[4];r[6]=sw(0x8cedaacc);r[5]=r[7];t(r[2]==0);r[5]+=4;r[6]+=r[4];cycles+=11;if(c.sr&1){r[3]=0xffffffff;cycles+=2;}else{r[3]=4;++cycles;}
  put(m,r[7],r[3]);r[3]=r[5]+8;r[7]=r[5];r[4]=8;put(m,r[5],r[6]);put(m,r[15]+12,r[3]);r[7]+=40;put(m,r[3],r[6]);r[3]=r[5]+16;put(m,r[15]+8,r[3]);put(m,r[3],r[6]);r[3]=r[5]+24;put(m,r[15]+4,r[3]);put(m,r[3],r[6]);r[3]=r[5]+32;put(m,r[15],r[3]);put(m,r[3],r[6]);put(m,r[7],r[6]);put(m,r[5]+4,r[4]);r[3]=w(r[15]+12);put(m,r[3]+4,r[4]);r[3]=w(r[15]+8);put(m,r[3]+4,r[4]);r[3]=w(r[15]+4);put(m,r[3]+4,r[4]);r[3]=w(r[15]);put(m,r[3]+4,r[4]);put(m,r[7]+4,r[4]);c.pc=c.pr;r[15]+=16;cycles+=35;
 }
 void next(){c.pr=0x8ced8586;c.r[4]=c.r[14];c.pc=0x8ced8582;cycles+=3;}

};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8cedc458;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_buffer_config_return_entries),std::end(movie_instance_buffer_config_return_entries));for(auto e:movie_instance_buffer_config_entries)entries.push_back(e);for(auto e:movie_instance_buffer_acquire_entries)entries.push_back(e);need(entries.size()==17,"Instance manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced85a4),"Next initializer inventory differs");for(unsigned target:{0x8cedc458u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cedc458u,0x8cedc458u,0xacedc458u,0x8dedc458u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cedc458),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cedc45a);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cedc458);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned tail_controls=0;std::string last_ram=argv[1];
 auto check=[&](unsigned alias,unsigned layout,unsigned placement,bool tail,unsigned mode,bool negative,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=(capture||tail)?initial:context(0x8cedc458,alias?15:0,placement,alias);
  if(!capture&&!tail){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=alias?0xacedaa0a:0x8cedaa0a;unsigned src=(0x8cf60000+placement)|seg;c.r[4]=layout==1?src:layout==2?src+4:layout==3?src-4:layout==4?c.r[15]-4:(0x8cf40000+placement)|seg;for(unsigned i=0;i<4;++i)put(rt.main_ram,src+4*i,0x12345670+i);dc_write32(rt,0x8cedc46c,src);}
  if(tail&&!capture){unsigned owner_address=word(rt.main_ram,c.r[15]);if(negative){dc_write16(rt,0x8cedaace,0xfff0);dc_write16(rt,0x8cedaaca,0xffe0);dc_write16(rt,0x8cedaacc,0xffc0);}put(rt.main_ram,owner_address+(negative?0xfffffff0u:0x18cu),mode);}
  auto expected=rt.main_ram;BufferConfigReturnModel m{{c,expected}};m.leaf();std::vector<Prediction> phases{{m.c,m.m,m.cycles}};if(capture||tail){m.config_return();phases.push_back({m.c,m.m,m.cycles});m.next();phases.push_back({m.c,m.m,m.cycles});}
  const auto initial_timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;
  auto verify=[&](unsigned phase){const auto&p=phases[phase];same(p.c,c);need(p.ram==rt.main_ram,"Buffer configuration RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr);if(capture){auto stem=out+"/phase"+std::to_string(phase)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<std::filesystem::path(last_ram).filename().string()<<'\t'<<p.ram.size()<<'\t'<<(shared?"clone-verified":"write-verified")<<'\n';save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  call_recompiled(c,rt,alias?0xacedc458:0x8cedc458);verify(0);if(capture||tail){call_recompiled(c,rt,c.pc);verify(1);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");verify(2);need(c.pc==0x8ced8582&&c.pr==0x8ced8586&&c.r[4]==0x8eee8000,"Configuration next call differs");if(capture)need(m.cycles==65,"Captured configuration cycles differ");else ++tail_controls;}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired config remains");
 };
 for(unsigned alias:{0u,1u})for(unsigned layout:{0u,1u,2u,3u,4u})for(unsigned placement:{0u,1u})check(alias,layout,placement,false,0,false,false);
 for(unsigned mode:{0u,1u,0xffffffffu})for(bool negative:{false,true})check(0,0,0,true,mode,negative,false);
 check(0,0,0,true,1,false,true);need(prefix_controls==20&&tail_controls==6,"Configuration coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<prefix_controls<<",\"tail_controls\":"<<tail_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[11,62,65],\"new_original_bodies\":1,\"new_original_instructions\":10,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"whole_config_return_verified\":true,\"missing_target\":\"8CED85A4\",\"whole_acquire_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original buffer config return: "<<prefix_controls<<" copy,"<<tail_controls<<" tail,"<<activation_controls<<" activation controls;captured11/62/65cycles;whole configuration return,nextmissingED85A4\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
