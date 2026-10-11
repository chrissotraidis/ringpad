// Original MOVIE work-area fill and bounded string copies through the next unresolved service.
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





#include "movie-descriptor-init/movie-descriptor-init-manifest.hpp"
#include "movie-descriptor-fields/movie-descriptor-fields-manifest.hpp"
#include "movie-word-fill/movie-word-fill-manifest.hpp"
namespace dcrecomp_generated {void movie_bounded_string_register(DCRuntime&);void recomp_8C11157C(SH4Context&,DCRuntime&);}
struct WorkareaModel : RequestModel {
 std::function<void(unsigned,const WorkareaModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}
 void copy(){auto&r=c.r;const unsigned n=r[6];r[1]=0;r[0]=r[5];r[7]=r[4];r[5]=0;unsigned copied=0;
  while(r[5]<n){r[3]=sb(r[0]);if(!r[3])break;wb(r[7]++,r[3]);++r[0];++r[5];++copied;}
  while(r[5]<n){wb(r[7]++,0);++r[5];}r[0]=r[4];t(true);c.pc=c.pr;cycles+=copied==n?10+9*n:15+5*n+4*copied;
 }
 void fields(){auto&r=c.r;push(r[14]);r[6]=94;push(r[13]);push(r[12]);r[12]=r[4];push(r[11]);push(r[10]);push(c.pr);r[4]=w(0x8ced1a08);r[3]=w(0x8ced1a0c);c.pr=0x8ced1920;r[5]=0;c.pc=0x8ced67f8;cycles+=13;phase(1);
  fill();phase(2);
  r[5]=w(0x8ced1a10);r[6]=32;r[2]=w(0x8ced1a14);r[14]=w(0x8ced1a08);r[4]=r[14];c.pr=0x8ced192c;c.pc=0x8c11157c;cycles+=7;phase(3);copy();phase(4);
  r[3]=w(0x8ced1a14);r[6]=32;r[5]=w(0x8ced1a18);r[4]=r[14]+32;c.pr=0x8ced1938;c.pc=0x8c11157c;cycles+=7;phase(5);copy();phase(6);
  r[2]=w(0x8ced1a20);r[6]=47;r[5]=w(0x8ced1a1c);r[4]=r[14]+64;c.pr=0x8ced1944;c.pc=0x8ced1938;cycles+=7;phase(7);
 }
 void init(){push(c.pr);c.pr=0x8ced18ea;c.pc=0x8ced1908;cycles+=4;phase(0);fields();}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_fields(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED1908(c,rt);}
static void observed_fill(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);movie_recomp_8CED67F8(c,rt);if(boundary)boundary(2);}
static void observed_copy(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8ced192c?3:5;if(boundary)boundary(phase);recomp_8C11157C(c,rt);if(boundary)boundary(phase+1);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}

int main(int argc,char**argv){try{
 need(argc==10,"usage: size RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned fill_controls=0,copy_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_descriptor_init_entries),std::end(movie_descriptor_init_entries));for(auto e:movie_descriptor_fields_entries){e.function=observed_fields;entries.push_back(e);}for(auto e:movie_word_fill_entries){e.function=observed_fill;entries.push_back(e);}need(entries.size()==29,"Workarea manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c11157c),"BOOT copy already registered");movie_bounded_string_register(rt);rt.targets.at(0x8c11157c)=observed_copy;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced67f8u,0x8ced67f8u,0xaced67f8u,0x8ded67f8u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced18e4),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced67fa);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced18e4);},"Inactive or unregistered");++activation_controls;}

 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Workarea timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Workarea SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected workarea device effects");};
 auto check_helper=[&](bool fill,unsigned n,unsigned pattern,int overlap,unsigned alias,unsigned bank){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(fill?0x8ced67f8:0x8c11157c,15,bank,bank);c.pr=alias?0xac112234:0x8c112234;c.sr|=bank?0x300:0;c.r[4]=alias?0xacef4000:0x8cef4000;c.r[6]=n;
  for(unsigned i=0;i<600;++i)rt.main_ram[0xef3ff0+i]=0xa5;
  if(fill)c.r[5]=pattern;
  else{c.r[5]=c.r[4]+overlap;for(unsigned i=0;i<100;++i)rt.main_ram[(c.r[5]+i)&0xffffff]=i%2?0xff:0x81;if(pattern<100)rt.main_ram[(c.r[5]+pattern)&0xffffff]=0;}
  auto expected=rt.main_ram;WorkareaModel m{{c,expected}};if(fill)m.fill();else m.copy();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,(fill?0x8ced67f8u:0x8c11157cu)|(alias?0x20000000u:0));same(m.c,c);need(expected==rt.main_ram,"Helper full RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(fill)++fill_controls;else ++copy_controls;
 };
 for(unsigned n:{0u,1u,15u,16u,17u,31u,32u,63u,94u,127u})for(unsigned value:{0u,0x12345678u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check_helper(true,n,value,0,alias,bank);
 for(unsigned n:{0u,1u,15u,16u,32u,48u})for(unsigned terminator:{0u,1u,27u,100u})for(int overlap:{-1,1,256})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check_helper(false,n,terminator,overlap,alias,bank);
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ced6708),"Next helper unexpectedly registered");auto c=initial;auto expected=rt.main_ram;WorkareaModel m{{c,expected}};std::vector<Prediction> phases;
  m.observe=[&](unsigned i,const WorkareaModel&model){need(i==phases.size(),"Workarea prediction order");phases.push_back({model.c,model.m,model.cycles});};m.init();need(m.cycles==755,"Connected workarea timing differs");
  need(m.c.r[4]==0x8cee6e24&&m.c.r[5]==0x8cedc8f0&&m.c.r[6]==47&&m.c.r[2]==0x8ced6708,"Next original service arguments differ");
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Workarea observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Workarea boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<7);++seen;};
  boundary=verify;
  rejection([&]{call_recompiled(c,rt,0x8ced18e4);},"Inactive or unregistered");boundary={};verify(7);same(m.c,c);need(expected==rt.main_ram,"Complete connected RAM differs");verify_devices(rt,vram,timers,start,tocr);
  const std::array<unsigned,8> cycles={4,17,168,175,458,465,748,755};need(seen==cycles.size(),"Missing workarea observations");
  for(unsigned i=0;i<cycles.size();++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured workarea timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
  // Independent contract for the captured 94-word clear followed by two padded strings.
  need(std::all_of(phases[2].ram.begin()+0xee6de4,phases[2].ram.begin()+0xee6f5c,[](auto b){return b==0;}),"Captured 376-byte clear differs");
  for(unsigned i=0;i<32;++i)for(unsigned string=0;string<2;++string)need(rt.main_ram[0xee6de4+32*string+i]==(i<27?ram[0xedc80c+28*string+i]:0),"Captured padded strings differ");
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired workarea entry remains");need(rt.targets.contains(0x8c11157c),"Resident BOOT helper retired with MOVIE");
 }
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"fill_controls\":"<<fill_controls<<",\"copy_controls\":"<<copy_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[4,17,168,175,458,465,748,755],\"new_original_bodies\":2,\"new_original_instructions\":58,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED6708\",\"complete_initializer_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie workarea: "<<fill_controls<<" fill,"<<copy_controls<<" copy,"<<activation_controls<<" activation controls; connected755cycles;explicit missing ED6708\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
