// Original create prefix builder and retained parent, stopped at the real next helper.
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





#include "movie-create-instance/movie-create-instance-manifest.hpp"
#include "movie-instance-pool/movie-instance-pool-manifest.hpp"
struct InstancePoolModel:RequestModel{
 void leaf(){auto&r=c.r;r[0]=unsigned(int(std::int16_t(w(0x8ced3e3e))));r[5]=w(0x8ced3e50);r[1]=unsigned(int(std::int16_t(w(0x8ced3e40))));r[7]=w(r[5]+r[0]);r[0]+=4;r[4]=w(r[5]+r[0]);t(std::int32_t(r[7])>0);r[5]=0;cycles+=9;
  if(c.sr&1){do{r[0]=64;r[6]=r[4];r[2]=w(r[6]+r[0]);t(r[2]==0);cycles+=5;if(c.sr&1){c.pc=c.pr;r[0]=r[6];cycles+=3;return;}++r[5];t(std::int32_t(r[5])>=std::int32_t(r[7]));r[4]+=r[1];cycles+=4;}while(!(c.sr&1));}r[0]=0;c.pc=c.pr;cycles+=4;
 }
 bool next(){t(c.r[0]==0);c.r[4]=c.r[0];cycles+=3;if(!(c.sr&1)){c.r[6]=w(c.r[15]+4);c.pr=0x8ced3df6;c.r[5]=w(c.r[15]);c.pc=0x8ced3df0;cycles+=4;return true;}c.r[15]+=8;c.pr=pop();c.pc=c.pr;c.r[0]=0;cycles+=5;return false;}
};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced3e0e;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_create_instance_entries),std::end(movie_create_instance_entries));for(auto e:movie_instance_pool_entries)entries.push_back(e);need(entries.size()==14,"Instance/guard manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced3e54),"Guard next target inventory differs");for(unsigned target:{0x8ced3e0eu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced3e0eu,0x8ced3e0eu,0xaced3e0eu,0x8ded3e0eu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced3dc2),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced3e10);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced3dc2);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned selected=0,exhausted=0;
 auto check=[&](unsigned count,unsigned mask,unsigned alias,unsigned placement,unsigned reverse,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced3e0e,reverse?15:0,placement,alias);
  if(!capture){unsigned seg=alias?0x20000000u:0,header=(0x8cf60000+placement)|seg,pool=(0x8cf70020+placement)|seg;c.r[15]=(0x8cf90000+placement)|seg;c.pr=0x8ced3de2|seg;
   dc_write32(rt,0x8ced3e50,header);dc_write16(rt,0x8ced3e3e,reverse?0x10:0x170);dc_write16(rt,0x8ced3e40,reverse?0xfffc:0x3f44);put(rt.main_ram,header+(reverse?0x10:0x170),count);put(rt.main_ram,header+(reverse?0x14:0x174),pool);
   if(std::int32_t(count)>0)for(unsigned i=0;i<count;++i)put(rt.main_ram,pool+i*(reverse?0xfffffffcu:0x3f44u)+64,(mask&(1u<<i))?0x80000001:0);
   put(rt.main_ram,c.r[15],pool+100);put(rt.main_ram,c.r[15]+4,0x76543210);put(rt.main_ram,c.r[15]+8,0x8ced05c4|seg);
  }
  auto expected=rt.main_ram;InstancePoolModel m{{c,expected}};m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  auto save=[&](unsigned phase){auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(m.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);};
  call_recompiled(c,rt,alias?0xaced3e0e:0x8ced3e0e);same(m.c,c);need(expected==rt.main_ram,"Pool leaf RAM differs");verify_devices(rt,vram,timers,start,tocr);if(capture){need(m.cycles==17&&c.r[0]==0x8cee2210,"Captured first free slot differs");save(0);}
  const auto before=m.cycles;bool found=m.next();tick(timers,start,m.cycles-before);
  if(found)rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");else call_recompiled(c,rt,c.pc);
  same(m.c,c);need(expected==rt.main_ram,"Pool caller RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){need(found&&m.cycles==24&&c.r[4]==0x8cee2210&&c.r[5]==0x8cfffec8,"Captured allocation setup differs");save(1);}else{++prefix_controls;if(found)++selected;else{++exhausted;need(c.pc==(alias?0xaced05c4:0x8ced05c4)&&c.r[0]==0,"Original exhaustion return differs");}}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired pool entry remains");
 };
 for(auto scenario:std::array<std::array<unsigned,2>,10>{{{0,0},{0xffffffffu,0},{0x80000000u,0},{1,0},{1,1},{2,1},{2,3},{3,0},{3,3},{3,7}}})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})for(unsigned reverse:{0u,1u})check(scenario[0],scenario[1],alias,placement,reverse,false);
 check(0,0,0,0,0,true);need(prefix_controls==80&&selected==32&&exhausted==48,"Pool coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"selected_controls\":"<<selected<<",\"exhausted_controls\":"<<exhausted<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[17,24],\"new_original_bodies\":1,\"new_original_instructions\":23,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"missing_target\":\"8CED3E54\",\"original_exhaustion_return_verified\":true,\"captured_instance_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original instance pool: "<<prefix_controls<<" connected("<<selected<<" selected,"<<exhausted<<" exhausted),"<<activation_controls<<" activation controls; captured17/24cycles to missingED3E54\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
