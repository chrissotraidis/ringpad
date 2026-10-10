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
struct CreateLayoutModel:SizeModel{
 CreateLayoutModel(SH4Context before,std::vector<std::uint8_t>&ram):SizeModel{{before,ram}}{}
 std::function<void(unsigned,const CreateLayoutModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 bool layout(){auto&r=c.r;r[15]+=20;r[0]=w(0x8ced0548);r[1]=w(0x8ced0544);r[2]=w(r[0]);r[3]=w(r[1]);r[5]=w(0x8ced0558);r[2]+=r[3];r[3]=w(0x8ced0540);r[6]=w(0x8ced055c);r[0]=w(r[3]);r[2]+=r[0];r[0]=unsigned(int(std::int16_t(w(0x8ced0532))));put(m,r[5],r[2]);r[7]=w(r[15]+r[0]);r[4]=r[7];r[4]+=31;r[4]>>=2;r[4]>>=2;t(r[4]&1);r[4]>>=1;r[4]<<=2;r[4]<<=2;shll(4);r[2]=r[4];r[2]-=r[7];put(m,r[6],r[2]);r[11]=r[4];r[3]=w(r[5]);r[2]=w(0x8ced054c);r[4]+=r[3];put(m,r[15]+28,r[4]);r[3]=w(r[2]);r[4]+=r[3];put(m,r[15]+24,r[4]);r[0]=w(0x8ced0554);r[1]=w(0x8ced0550);r[3]=w(r[0]);r[0]=w(0x8ced053c);r[4]+=r[3];r[2]=w(0x8ced0538);put(m,r[15]+20,r[4]);r[3]=w(r[1]);r[5]=w(0x8ced0560);r[4]+=r[3];put(m,r[15]+36,r[4]);r[3]=w(r[0]);r[0]=unsigned(int(std::int16_t(w(0x8ced0534))));r[4]+=r[3];put(m,r[15]+32,r[4]);r[3]=w(r[2]);r[4]+=r[3];r[3]=w(r[6]);r[4]-=r[11];r[4]+=r[3];r[14]=r[4];put(m,r[5],r[4]);r[0]=w(r[15]+r[0]);t(std::int32_t(r[14])>std::int32_t(r[0]));cycles+=58;
 if(c.sr&1){r[0]=unsigned(int(std::int16_t(w(0x8ced0534))));r[5]=r[14];r[4]=w(0x8ced0564);r[1]=w(0x8ced0568);r[3]=w(r[15]+r[0]);c.pr=0x8ced04be;r[5]-=r[3];c.pc=0x8ced04b0;cycles+=8;return false;}
 r[3]=r[15]+12;push(r[3]);r[5]=r[13];r[2]=r[15]+12;push(r[2]);r[6]=r[15]+8;r[7]=r[15]+12;c.pr=0x8ced04dc;r[4]=r[10];c.pc=0x8ced0222;cycles+=14;mark(0);dimensions();mark(1);return true;}
};
struct CaptureBoundaryStop{};
static thread_local std::function<void(unsigned)> create_boundary;
static void observed_create_dimensions(SH4Context&c,DCRuntime&rt){if(create_boundary)create_boundary(0);movie_recomp_8CED0222(c,rt);if(create_boundary)create_boundary(1);throw CaptureBoundaryStop{};}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced043c;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,27> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38};
 const std::array<unsigned,27> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0,1};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_create_prefix_entries),std::end(movie_create_prefix_entries));for(auto e:movie_stream_size_entries){entries.push_back(e);}for(auto e:movie_frame_size_entries){entries.push_back(e);}for(auto e:movie_dimensions_entries){e.function=observed_create_dimensions;entries.push_back(e);}need(entries.size()==36,"Create/retained helper manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced043cu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced043cu,0x8ced043cu,0xaced043cu,0x8ded043cu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced03f8),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced043e);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced03f8);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned accepted=0,rejected=0;
 auto check=[&](unsigned budget,unsigned placement,unsigned alias,unsigned bank,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced043c,bank?15:0,bank,bank);
  if(!captured){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[10]=bank?0xffffffffu:320u;c.r[13]=bank?0x80000000u:480u;c.r[12]=6;c.pr=0x8ced043c|seg;put(rt.main_ram,c.r[15]+20+132,(0x8c900000+placement)|seg);put(rt.main_ram,c.r[15]+20+136,budget);
   dc_write32(rt,0x8ced0558,(0x8cf70000+placement)|seg);dc_write32(rt,0x8ced055c,(bank?0x8cf70000+placement:0x8cf70010+placement)|seg);dc_write32(rt,0x8ced0560,(0x8cf70020+placement)|seg);dc_write32(rt,0x8ced0568,0x8ced5374|seg);dc_write32(rt,0x8ced0564,(0x8cf80000+placement)|seg);
  }
  auto expected=rt.main_ram;CreateLayoutModel m(c,expected);std::vector<Prediction> phases;m.capture=[&](unsigned i,const CreateLayoutModel&v){need(i==phases.size(),"Layout prediction order differs");phases.push_back({v.c,v.m,v.cycles});};bool fits=m.layout();
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  create_boundary=[&](unsigned which){need(which==seen&&seen<phases.size(),"Observed dimension order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Layout boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,true);if(captured){auto stem=out+"/phase"+std::to_string(seen)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  if(fits){bool stopped=false;try{call_recompiled(c,rt,alias?0xaced043c:0x8ced043c);}catch(const CaptureBoundaryStop&){stopped=true;}need(stopped&&seen==2,"Real dimensions return not observed");if(!captured)++accepted;}else{rejection([&]{call_recompiled(c,rt,alias?0xaced043c:0x8ced043c);},"Inactive or unregistered");need(c.r[1]==(alias?0xaced5374:0x8ced5374)&&m.cycles==66&&seen==0,"Oversize call boundary differs");if(!captured)++rejected;}
  create_boundary={};same(m.c,c);need(expected==rt.main_ram,"Layout full RAM differs");auto timers=initial_timers;tick(timers,start,m.cycles);verify_devices(rt,vram,timers,start,tocr);
  if(captured){need(fits&&c.pc==0x8ced04dc&&c.pr==0x8ced04dc&&c.r[14]==0x2d0100&&c.r[15]==0x8cfffe98,"Captured dimension return differs");need(phases[0].cycles==72&&phases[1].cycles==98,"Captured layout cycles differ");}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired layout/helper entry remains");
 };
 for(unsigned budget:{0u,0x2d0120u,0xffffffffu,0x7fffffffu})for(unsigned placement:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(budget,placement,alias,bank,false);
 check(0,0,0,0,true);need(prefix_controls==32&&accepted>0&&rejected>0,"Layout branch coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"layout_controls\":"<<prefix_controls<<",\"accepted_controls\":"<<accepted<<",\"rejected_controls\":"<<rejected<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[72,98],\"oversize_call_cycles\":66,\"required_bytes\":2949376,\"new_original_bodies\":0,\"new_original_instructions\":0,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"retained_dimensions_executed\":true,\"explicit_observation_stop_after_real_dimensions_return\":true,\"observed_return\":\"8CED04DC\",\"oversize_missing_target\":\"8CED5374\",\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original create layout: "<<prefix_controls<<" layout("<<accepted<<" accepted,"<<rejected<<" rejected),"<<activation_controls<<" activation controls; captured72/98cycles;oversize66cycles to ED5374\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
