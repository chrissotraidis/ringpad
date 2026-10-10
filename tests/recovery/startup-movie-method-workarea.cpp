// Original work-area initialization through its real caller and next missing service.
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





#include "movie-method-setup/movie-method-setup-manifest.hpp"
#include "movie-method-workarea/movie-method-workarea-manifest.hpp"
#include "movie-method-record/movie-method-record-manifest.hpp"
#include "movie-word-fill/movie-word-fill-manifest.hpp"
namespace dcrecomp_generated {void movie_bounded_string_register(DCRuntime&);void recomp_8C11157C(SH4Context&,DCRuntime&);}
struct WorkareaModel : RequestModel {
 std::function<void(unsigned,const WorkareaModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;need(n<=1024,"Unbounded test fill");for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}
 void copy(){auto&r=c.r;const unsigned n=r[6];r[1]=0;r[0]=r[5];r[7]=r[4];r[5]=0;unsigned copied=0;
  while(r[5]<n){r[3]=sb(r[0]);if(!r[3])break;wb(r[7]++,r[3]);++r[0];++r[5];++copied;}
  while(r[5]<n){wb(r[7]++,0);++r[5];}r[0]=r[4];t(true);c.pc=c.pr;cycles+=copied==n?10+9*n:15+5*n+4*copied;
 }
 void record(){c.r[5]=0;put(m,c.r[4],0);put(m,c.r[4]+4,0);c.pc=c.pr;put(m,c.r[4]+8,0);cycles+=8;}
 void init(){auto&r=c.r;push(r[14]);push(r[13]);push(r[12]);r[12]=r[4];r[13]=unsigned(int(std::int16_t(w(0x8ced79e2)&0xffff)));r[6]=r[12]-1;r[3]=unsigned(int(std::int16_t(w(0x8ced79e4)&0xffff)));c.macl=r[13]*r[6];r[14]=w(0x8ced79f0);push(c.pr);r[1]=w(0x8ced79f4);r[6]=c.macl;put(m,r[14],r[5]);r[5]=0;r[6]+=r[3];r[6]>>=2;c.pr=0x8ced7910;r[4]=w(r[14]);c.pc=0x8ced67f8;cycles+=21;phase(1);fill();phase(2);
  r[3]=w(0x8ced79fc);r[6]=32;r[5]=w(0x8ced79f8);c.pr=0x8ced791a;r[4]=w(r[14]);c.pc=0x8c11157c;cycles+=6;phase(3);copy();phase(4);
  r[4]=w(r[14]);r[6]=32;r[3]=w(0x8ced79fc);r[5]=w(0x8ced7a00);c.pr=0x8ced7926;r[4]+=32;c.pc=0x8c11157c;cycles+=7;phase(5);copy();phase(6);
  r[4]=w(r[14]);c.pr=0x8ced792c;r[4]+=64;c.pc=0x8ced79a4;cycles+=4;phase(7);record();phase(8);
  r[2]=w(r[14]);r[0]=76;r[5]=0;t(int(r[12])>0);put(m,r[2]+r[0],r[12]);r[4]=w(r[14]);r[4]+=80;r[6]=1;cycles+=9;
  if(c.sr&1)do{++r[5];put(m,r[4],r[6]);t(int(r[5])>=int(r[12]));r[4]+=r[13];cycles+=5;}while(!(c.sr&1));
  c.pr=pop();r[0]=0;r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;phase(9);
 }
 void wrapper(){push(c.pr);c.pr=0x8ced78ca;c.pc=0x8ced78ea;cycles+=4;phase(0);init();t(c.r[0]==0);c.r[6]=c.r[0];cycles+=3;need(c.sr&1,"Initializer failed to return zero");c.r[3]=w(0x8ced79e8);c.pr=0x8ced78dc;c.pc=0x8ced78d6;cycles+=4;phase(10);}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_init(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED78EA(c,rt);if(boundary)boundary(9);}
static void observed_fill(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);movie_recomp_8CED67F8(c,rt);if(boundary)boundary(2);}
static void observed_copy(SH4Context&c,DCRuntime&rt){const unsigned i=c.pr==0x8ced791a?3:5;if(boundary)boundary(i);recomp_8C11157C(c,rt);if(boundary)boundary(i+1);}
static void observed_record(SH4Context&c,DCRuntime&rt){if(boundary)boundary(7);movie_recomp_8CED79A4(c,rt);if(boundary)boundary(8);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: workarea RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned record_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_method_setup_entries),std::end(movie_method_setup_entries));for(auto e:movie_method_workarea_entries){e.function=observed_init;entries.push_back(e);}for(auto e:movie_method_record_entries){e.function=observed_record;entries.push_back(e);}for(auto e:movie_word_fill_entries){e.function=observed_fill;entries.push_back(e);}need(entries.size()==22,"Workarea manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced6840)&&!rt.targets.contains(0x8c11157c),"Unexpected existing helper binding");movie_bounded_string_register(rt);rt.targets.at(0x8c11157c)=observed_copy;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced78eau,0x8ced78eau,0xaced78eau,0x8ded78eau}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced78ea),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced78ec);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced78ea);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Workarea timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Workarea SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected workarea device effects");};
 for(unsigned value:{0u,1u,0xffffffffu})for(unsigned offset:{0u,4u,64u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(0x8ced79a4,bank?15:0,bank,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[4]=(alias?0xacef4000:0x8cef4000)+offset;c.r[5]=value;std::fill(rt.main_ram.begin()+0xef3ff0,rt.main_ram.begin()+0xef4070,0xa5);auto expected=rt.main_ram;WorkareaModel m{{c,expected}};m.record();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xaced79a4:0x8ced79a4);same(m.c,c);need(expected==rt.main_ram,"Record clear RAM differs");verify_devices(rt,vram,timers,start,tocr);++record_controls;}
 auto check=[&](unsigned count,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced78c4,bank?15:0,bank,bank);
  if(!capture){c.pr=alias?0xacedabce:0x8cedabce;c.r[4]=count;c.r[5]=alias?0xacf20000:0x8cf20000;c.r[15]|=alias?0x20000000u:0;c.sr|=bank?0x300:0;std::fill(rt.main_ram.begin()+0xf1fff0,rt.main_ram.begin()+0xf21010,0xa5);}
  auto expected=rt.main_ram;WorkareaModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const WorkareaModel&model){need(i==phases.size(),"Workarea prediction order");phases.push_back({model.c,model.m,model.cycles});};m.wrapper();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Workarea observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Workarea boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<10);++seen;};
  if(capture)boundary=verify;rejection([&]{call_recompiled(c,rt,alias?0xaced78c4:0x8ced78c4);},"Inactive or unregistered");boundary={};if(capture)verify(10);same(m.c,c);need(expected==rt.main_ram,"Workarea complete RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,11> cycles={4,25,278,284,567,574,857,861,869,905,912};need(seen==cycles.size()&&c.pr==0x8ced78dc&&c.pc==0x8ced78d6&&c.r[3]==0x8ced6840&&c.r[15]==0x8cfffed0,"Captured continuation differs");for(unsigned i=0;i<cycles.size();++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured workarea timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(word(rt.main_ram,0x8cefb8ec)==0x8cee6f60&&word(rt.main_ram,0x8cee6fac)==4,"Captured work pointer/count differs");for(unsigned i=0;i<4;++i)need(word(rt.main_ram,0x8cee6fb0+i*188)==1,"Captured record flag differs");}
  else{const unsigned bytes=188*count+80;need(bytes<=4096&&rt.main_ram[0xf1ffff]==0xa5&&rt.main_ram[0xf20000+bytes]==0xa5,"Workarea clear bounds differ");need(word(rt.main_ram,0x8cf20000+76)==count,"Stored original count differs");++connected_controls;}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired workarea entry remains");need(rt.targets.contains(0x8c11157c),"Resident copy retired with module");
 };
 for(unsigned count:{0u,1u,2u,4u,0x80000000u,0xc0000000u,0x80000001u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(count,alias,bank,false);
 check(4,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"record_controls\":"<<record_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[4,25,278,284,567,574,857,861,869,905,912],\"new_original_bodies\":2,\"new_original_instructions\":60,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED6840\",\"captured_initializer_return_verified\":true,\"captured_wrapper_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie workarea: "<<record_controls<<" record,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured912cycles; explicit missing ED6840\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
