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





#include "movie-instance-resource-init/movie-instance-resource-init-manifest.hpp"
#include "movie-word-fill/movie-word-fill-manifest.hpp"
struct ResourceInitModel:RequestModel{
 std::function<void(unsigned,const ResourceInitModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}

 void prefix(){auto&r=c.r;push(r[14]);r[6]=47;push(c.pr);r[3]=w(0x8ced7be0);r[14]=r[4];c.pr=0x8ced7b4c;r[5]=0;c.pc=0x8ced67f8;cycles+=8;mark(0);fill();mark(1);r[2]=2;r[4]=r[14];put(m,r[14],r[2]);c.pr=0x8ced7b56;r[4]+=4;c.pc=0x8ced7b4c;cycles+=6;mark(2);}

};
static thread_local std::function<void(unsigned)> resource_init_boundary;
static void observed_resource_init_fill(SH4Context&c,DCRuntime&rt){if(resource_init_boundary)resource_init_boundary(0);movie_recomp_8CED67F8(c,rt);if(resource_init_boundary)resource_init_boundary(1);}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced7b3e;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_resource_init_entries),std::end(movie_instance_resource_init_entries));for(auto e:movie_word_fill_entries){e.function=observed_resource_init_fill;entries.push_back(e);}need(entries.size()==16,"Initialize/fill manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced79a8),"Missing resource helper inventory differs");for(unsigned target:{0x8ced7b3eu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced7b3eu,0x8ced7b3eu,0xaced7b3eu,0x8ded7b3eu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced7b3e),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced7b40);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced7b3e);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 auto check=[&](unsigned alias,unsigned placement,unsigned overlap,unsigned target,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced7b3e,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=0x8ced7af2;c.r[4]=overlap?c.r[15]-100:(0x8cf60000+placement)|seg;for(unsigned i=0;i<188;i+=4)put(rt.main_ram,c.r[4]+i,0xdead0000+i);dc_write32(rt,0x8ced7be0,target);}
  auto expected=rt.main_ram;ResourceInitModel m{{c,expected}};std::vector<Prediction> phases;m.capture=[&](unsigned i,const ResourceInitModel&v){need(i==phases.size(),"Resource prediction order differs");phases.push_back({v.c,v.m,v.cycles});};m.prefix();
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  resource_init_boundary=[&](unsigned which){need(which==seen&&seen<phases.size(),"Resource observed order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Resource boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,which!=2);if(captured){auto stem=out+"/phase"+std::to_string(which)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  rejection([&]{call_recompiled(c,rt,alias?0xaced7b3e:0x8ced7b3e);},"Inactive or unregistered");need(!rt.targets.contains(0x8ced79a8)&&c.pr==0x8ced7b56,"Resource next missing call differs");resource_init_boundary(2);resource_init_boundary={};need(seen==3,"Resource captures incomplete");
  need(phases[0].cycles==8&&phases[1].cycles==108&&m.cycles==114,"Resource timing differs");if(captured)need(c.r[14]==0x8cee6fb0&&c.r[4]==0x8cee6fb4,"Captured resource differs");else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired resource initializer remains");
 };
 for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})for(unsigned overlap:{0u,1u})for(unsigned target:{0x0ced67f8u,0x8ced67f8u,0xaced67f8u})check(alias,placement,overlap,target,false);
 check(0,0,0,0x8ced67f8,true);need(prefix_controls==24,"Resource coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[8,108,114],\"new_original_bodies\":1,\"new_original_instructions\":34,\"clear_words\":47,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_clear_return_verified\":true,\"missing_target\":\"8CED79A8\",\"full_instance_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resource initializer: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; real47-word clear;captured8/108/114cycles to missingED79A8\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
