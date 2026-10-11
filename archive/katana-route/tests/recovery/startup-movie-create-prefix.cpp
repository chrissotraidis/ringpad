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





#include "movie-create-prefix/movie-create-prefix-manifest.hpp"
#include "movie-stream-size/movie-stream-size-manifest.hpp"
#include "movie-frame-size/movie-frame-size-manifest.hpp"
#include "movie-dimensions/movie-dimensions-manifest.hpp"
#include "movie-create-size-model.hpp"
struct CreatePrefixModel:SizeModel{
 CreatePrefixModel(SH4Context before,std::vector<std::uint8_t>&ram):SizeModel{{before,ram}}{}
 std::function<void(unsigned,const CreatePrefixModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 void prefix(){auto&r=c.r;for(unsigned i:{14u,13u,12u,11u,10u})push(r[i]);push(c.pr);r[15]-=100;r[0]=124;r[3]=w(0x8ced0538);r[2]=w(0x8ced053c);r[10]=r[7];put(m,r[15]+16,r[5]);put(m,r[15]+20,r[6]);r[13]=w(r[15]+r[0]);r[0]=unsigned(int(std::int16_t(w(0x8ced0530))));r[1]=w(0x8ced0540);r[12]=w(r[15]+r[0]);push(r[3]);r[3]=w(0x8ced0544);push(r[2]);push(r[1]);push(r[3]);r[5]=w(r[15]+36);r[6]=w(0x8ced054c);r[7]=w(0x8ced0548);c.pr=0x8ced042e;r[4]=w(r[15]+32);c.pc=0x8ced01bc;cycles+=28;mark(0);
 stream();mark(1);r[3]=w(0x8ced0550);r[5]=r[13];r[7]=w(0x8ced0554);r[6]=r[12];push(r[3]);c.pr=0x8ced043c;r[4]=r[10];c.pc=0x8ced0256;cycles+=8;mark(2);frame_prefix();c.pc=0x8ced0222;mark(3);dimensions();mark(4);frame_tail();mark(5);}
};
struct CaptureBoundaryStop{};
static thread_local std::function<void(unsigned)> create_boundary;
static void observed_create_stream(SH4Context&c,DCRuntime&rt){if(create_boundary)create_boundary(0);movie_recomp_8CED01BC(c,rt);if(create_boundary)create_boundary(1);}
static void observed_create_frame(SH4Context&c,DCRuntime&rt){if(create_boundary)create_boundary(2);movie_recomp_8CED0256(c,rt);if(create_boundary)create_boundary(5);throw CaptureBoundaryStop{};}
static void observed_create_dimensions(SH4Context&c,DCRuntime&rt){if(create_boundary)create_boundary(3);movie_recomp_8CED0222(c,rt);if(create_boundary)create_boundary(4);}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced03f8;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,27> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38};
 const std::array<unsigned,27> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0,1};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_create_prefix_entries),std::end(movie_create_prefix_entries));for(auto e:movie_stream_size_entries){e.function=observed_create_stream;entries.push_back(e);}for(auto e:movie_frame_size_entries){e.function=observed_create_frame;entries.push_back(e);}for(auto e:movie_dimensions_entries){e.function=observed_create_dimensions;entries.push_back(e);}need(entries.size()==36,"Create/retained helper manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced03f8u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced03f8u,0x8ced03f8u,0xaced03f8u,0x8ded03f8u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced03f8),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced03fa);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced03f8);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 struct Arguments{unsigned selector,count,width,height,frames;};
 auto check=[&](Arguments args,unsigned placement,unsigned alias,unsigned bank,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced03f8,bank?15:0,bank,bank);
  if(!captured){const unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[4]=0x8cf60000|seg;c.r[5]=args.selector;c.r[6]=args.count;c.r[7]=args.width;put(rt.main_ram,c.r[15],args.height);put(rt.main_ram,c.r[15]+4,args.frames);put(rt.main_ram,c.r[15]+8,3);put(rt.main_ram,c.r[15]+12,0x100000);c.pr=0x8cecf1b0|seg;
   const std::array<unsigned,8> literals={0x8ced0538,0x8ced053c,0x8ced0540,0x8ced0544,0x8ced054c,0x8ced0548,0x8ced0550,0x8ced0554};for(unsigned i=0;i<literals.size();++i)dc_write32(rt,literals[i],(0x8cf70000+placement+4*(bank&&i==2?4:i))|seg);dc_write16(rt,0x8ced0530,bank?132:128);
  }
  auto expected=rt.main_ram;CreatePrefixModel m(c,expected);std::vector<Prediction> phases;m.capture=[&](unsigned i,const CreatePrefixModel&v){need(i==phases.size(),"Prediction order differs");phases.push_back({v.c,v.m,v.cycles});};m.prefix();need(phases.size()==6,"Prediction count differs");
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  create_boundary=[&](unsigned which){need(which==seen&&seen<phases.size(),"Observed helper order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Create boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,true);if(captured){const auto stem=out+"/phase"+std::to_string(seen)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  bool stopped=false;try{call_recompiled(c,rt,alias?0xaced03f8:0x8ced03f8);}catch(const CaptureBoundaryStop&){stopped=true;}create_boundary={};need(stopped&&seen==6,"Real frame-return observation absent");same(m.c,c);need(expected==rt.main_ram,"Connected create RAM differs");auto timers=initial_timers;tick(timers,start,m.cycles);verify_devices(rt,vram,timers,start,tocr);
  if(captured){need(c.pc==0x8ced043c&&c.pr==0x8ced043c&&c.r[15]==0x8cfffe8c,"Captured helper return differs");const std::array<unsigned,6> wanted={28,149,157,173,199,239};for(unsigned i=0;i<6;++i)need(phases[i].cycles==wanted[i],"Captured cycles differ");}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired create/helper entry remains");
 };
 const std::array<Arguments,6> args={Arguments{1,0x384000,320,480,6},Arguments{2,0,0,0,0},Arguments{0,0xffffffff,1,1,1},Arguments{2,0x80000000,0xffffffff,0x80000000,0xffffffff},Arguments{1,0x7fffffff,0x7fffffff,0x7fffffff,0x80000000},Arguments{3,8,65,63,2}};
 for(auto a:args)for(unsigned placement:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(a,placement,alias,bank,false);
 check(args[0],0,0,0,true);need(prefix_controls==48,"Create control count differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[28,149,157,173,199,239],\"new_original_bodies\":1,\"new_original_instructions\":222,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"retained_stream_frame_dimensions_executed\":true,\"explicit_observation_stop_after_real_frame_return\":true,\"observed_return\":\"8CED043C\",\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original create prefix: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; registered stream/frame/dimensions execute; observed real frame return239cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
