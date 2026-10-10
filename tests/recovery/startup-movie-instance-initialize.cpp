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





#include "movie-instance-initialize/movie-instance-initialize-manifest.hpp"
#include "movie-word-fill/movie-word-fill-manifest.hpp"
struct InstanceInitializeModel:RequestModel{
 std::function<void(unsigned,const InstanceInitializeModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 void copy60(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;need(r[3]==0x8c10e97a&&r[0]==56,"Unexpected60-byte copy dispatch");r[3]=w(r[2]+r[0]);
  for(int offset=52;offset>=0;offset-=4){const unsigned reg=(offset&4)?0:3,previous=reg==0?3:0;r[reg]=w(r[2]+unsigned(offset));put(m,r[1]+unsigned(offset)+4,r[previous]);}put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=39;
 }
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}

 void prefix(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=4;put(m,r[15],r[6]);r[13]=r[5];r[6]=unsigned(int(std::int16_t(w(0x8ced3f7c))));r[5]=0;r[3]=w(0x8ced3f8c);r[14]=r[4];c.pr=0x8ced3e6c;r[4]=r[14];c.pc=0x8ced67f8;cycles+=13;mark(0);fill();mark(1);
  r[2]=w(r[13]+4);r[3]=0xffffffe0;r[1]=r[14];r[2]+=31;r[2]&=r[3];r[3]=w(0x8ced3f90);put(m,r[13]+4,r[2]);r[2]=r[13];c.pr=0x8ced3e80;r[0]=60;c.pc=0x8c10e964;cycles+=11;mark(2);copy60();mark(3);
  r[4]=1;r[2]=w(r[15]);r[0]=64;r[3]=0;put(m,r[14]+60,r[2]);put(m,r[14]+r[0],r[4]);r[0]=68;put(m,r[14]+r[0],r[4]);r[0]=72;r[4]=r[14];put(m,r[14]+r[0],r[3]);c.pr=0x8ced3e9a;r[4]+=76;c.pc=0x8ced3e80;cycles+=14;mark(4);
 }
};
static thread_local std::function<void(unsigned)> initialize_boundary;
static thread_local RecompiledFunction real_copy=nullptr;
static void observed_initialize_copy(SH4Context&c,DCRuntime&rt){if(initialize_boundary)initialize_boundary(2);real_copy(c,rt);if(initialize_boundary)initialize_boundary(3);}
static void observed_initialize_fill(SH4Context&c,DCRuntime&rt){if(initialize_boundary)initialize_boundary(0);movie_recomp_8CED67F8(c,rt);if(initialize_boundary)initialize_boundary(1);}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced3e54;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_initialize_entries),std::end(movie_instance_initialize_entries));for(auto e:movie_word_fill_entries){e.function=observed_initialize_fill;entries.push_back(e);}need(entries.size()==20,"Initialize/fill manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10e964)&&!rt.targets.contains(0x8ced3ef8),"Copy/missing initializer inventory differs");real_copy=rt.targets.at(0x8c10e964);rt.targets.at(0x8c10e964)=observed_initialize_copy;for(unsigned target:{0x8ced3e54u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced3e54u,0x8ced3e54u,0xaced3e54u,0x8ded3e54u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced3e54),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced3e56);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced3e54);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 auto check=[&](unsigned count,unsigned layout,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced3e54,alias?15:0,placement,alias);
  if(!captured){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=0x8ced3df6|seg;c.r[4]=(layout==2?c.r[15]-64:(0x8cf60000+placement)|seg);c.r[5]=layout==1?c.r[4]+4:layout==2?c.r[15]-16:(0x8cf70000+placement)|seg;c.r[6]=0xf00dcafe;
   for(unsigned i=0;i<60;i+=4)put(rt.main_ram,c.r[5]+i,0xfffffff0+i);dc_write16(rt,0x8ced3f7c,count);dc_write32(rt,0x8ced3f8c,0x8ced67f8|seg);dc_write32(rt,0x8ced3f90,0x8c10e964|seg);
  }
  auto expected=rt.main_ram;InstanceInitializeModel m{{c,expected}};std::vector<Prediction> phases;m.capture=[&](unsigned i,const InstanceInitializeModel&v){need(i==phases.size(),"Initialize prediction order differs");phases.push_back({v.c,v.m,v.cycles});};m.prefix();
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  initialize_boundary=[&](unsigned which){need(which==seen&&seen<phases.size(),"Initialize observed order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Initialize boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,which!=4);if(captured){auto stem=out+"/phase"+std::to_string(which)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  rejection([&]{call_recompiled(c,rt,alias?0xaced3e54:0x8ced3e54);},"Inactive or unregistered");need(!rt.targets.contains(0x8ced3ef8)&&c.pr==0x8ced3e9a,"Initialize next missing call differs");initialize_boundary(4);initialize_boundary={};need(seen==5,"Initialize captures incomplete");
  if(captured){need(phases[0].cycles==13&&phases[1].cycles==4589&&phases[2].cycles==4600&&phases[3].cycles==4639&&m.cycles==4653&&c.r[14]==0x8cee2210&&c.r[4]==0x8cee225c,"Captured initializer cycles/slot differ");}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired initializer entry remains");need(rt.targets.contains(0x8c10e964),"Resident copy retired with module");
 };
 for(unsigned count:{0u,1u,15u,16u,17u,33u,4049u})for(unsigned layout:{0u,1u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(count,layout,alias,placement,false);
 check(0,0,0,0,true);need(prefix_controls==84,"Initializer coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[13,4589,4600,4639,4653],\"new_original_bodies\":1,\"new_original_instructions\":82,\"clear_words\":4049,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_clear_and_copy_returns_verified\":true,\"missing_target\":\"8CED3EF8\",\"full_instance_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original instance initializer: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; real4049-word clear and60-byte copy; captured4653cycles to missingED3EF8\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
