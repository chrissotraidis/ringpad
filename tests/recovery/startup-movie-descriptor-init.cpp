// Original MOVIE descriptor initialization through its first unresolved service.
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
struct DescriptorModel : RequestModel {
 std::function<void(unsigned,const DescriptorModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void fields(){auto&r=c.r;push(r[14]);r[6]=94;push(r[13]);push(r[12]);r[12]=r[4];push(r[11]);push(r[10]);push(c.pr);r[4]=w(0x8ced1a08);r[3]=w(0x8ced1a0c);c.pr=0x8ced1920;r[5]=0;c.pc=0x8ced1908;cycles+=13;phase(1);}
 void init(){push(c.pr);c.pr=0x8ced18ea;c.pc=0x8ced1908;cycles+=4;phase(0);fields();}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_fields(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED1908(c,rt);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}

int main(int argc,char**argv){try{
 need(argc==10,"usage: size RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_descriptor_init_entries),std::end(movie_descriptor_init_entries));for(auto e:movie_descriptor_fields_entries){e.function=observed_fields;entries.push_back(e);}need(entries.size()==22,"Descriptor manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced18e4u,0x8ced18e4u,0xaced18e4u,0x8ded18e4u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced18e4),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced18e6);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced18e4);},"Inactive or unregistered");++activation_controls;}
 auto check=[&](unsigned sentinel,unsigned descriptor,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ced67f8),"Missing helper unexpectedly registered");auto c=initial;
  if(!capture){for(unsigned i=0;i<15;++i)c.r[i]=sentinel^(0x01010101u*i);c.r[4]=descriptor;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000301u)|(bank?0x20000301u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}}
  const auto before=c;auto expected=rt.main_ram;DescriptorModel m{{c,expected}};std::vector<Prediction> phases;
  if(capture)m.observe=[&](unsigned i,const DescriptorModel&model){need(i==phases.size(),"Descriptor prediction order");phases.push_back({model.c,model.m,model.cycles});};m.init();need(m.cycles==17,"Descriptor timing differs");
  const std::array<unsigned,7> saved={before.pr,before.r[14],before.r[13],before.r[12],before.r[11],before.r[10],0x8ced18ea};for(unsigned i=0;i<7;++i)need(word(expected,before.r[15]-4*(i+1))==saved[i],"Independent saved register frame differs");
  need(m.c.r[12]==before.r[4]&&m.c.r[4]==0x8cee6de4&&m.c.r[5]==0&&m.c.r[6]==94&&m.c.r[15]==before.r[15]-28,"Independent descriptor arguments differ");
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  unsigned seen=0;auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Size observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Setup boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);need(device==sqfields(rt)&&std::equal(sq.begin(),sq.end(),rt.store_queues.begin()),"Boundary SQ differs");++seen;};
  if(capture){boundary=verify;}
  rejection([&]{call_recompiled(c,rt,0x8ced18e4|(alias?0x20000000u:0));},"Inactive or unregistered");
  if(capture){verify(1);boundary={};const std::array<unsigned,2> cycles={4,17};need(seen==2,"Missing descriptor observations");for(unsigned i=0;i<2;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured setup timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}}
  same(m.c,c);need(expected==rt.main_ram,"Complete setup RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected setup device effects");
  if(capture){auto stem=out+"/descriptor-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else ++controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired setup entry remains");
 };


 for(unsigned sentinel:{0u,1u,0x80000000u,0xffffffffu,0x12345678u,0xdeadbeefu,0x7fffffffu,0x40000000u})for(unsigned descriptor:{0u,0xffffffffu,0x8cffff14u,0xacffff14u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(sentinel,descriptor,alias,bank,false);
 check(0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"initializer_controls\":"<<controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[4,17],\"new_original_bodies\":2,\"new_original_instructions\":113,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED67F8\",\"complete_initializer_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie descriptor prefix: "<<controls<<" initializer,"<<activation_controls<<" activation controls;4/17 boundaries;explicit missing ED67F8\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
