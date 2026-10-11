// Original MOVIE mode configuration with an independent SH-4 division oracle.
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




#include "movie-slot-setup/movie-slot-setup-manifest.hpp"
#include "movie-setup-validation/movie-setup-validation-manifest.hpp"
struct SetupModel : RequestModel {
 std::function<void(unsigned,const SetupModel&)> observe;
 unsigned missing=0;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void copy16(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[0]+r[3]);r[0]-=4;need(r[3]==0x8c10e9a6,"Unexpected16-byte copy dispatch");r[0]=w(r[0]+r[2]);r[3]=w(r[2]+8);put(m,r[1]+12,r[0]);r[0]=w(r[2]+4);put(m,r[1]+8,r[3]);r[3]=w(r[2]);put(m,r[1]+4,r[0]);put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=17;}
 void validate(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=16;r[0]=r[5];r[14]=w(0x8ced03d8);t(r[0]==1);cycles+=7;bool mode1=c.sr&1;
  if(!mode1){t(r[0]==2);cycles+=2;}
  if(mode1||(c.sr&1)){r[3]=w(0x8ced03e0);r[1]=r[15];r[2]=w(mode1?0x8ced03dc:0x8ced03e4);c.pr=mode1?0x8ced0370:0x8ced037e;r[0]=16;c.pc=r[3];cycles+=6;phase(1);copy16();phase(2);cycles+=2;r[3]=w(0x8ced03e8);put(m,r[15]+12,r[6]);c.pr=0x8ced0390;r[4]=r[15];c.pc=0x8ced0388;cycles+=5;missing=r[3];}
  else{cycles+=2;r[4]=0xfffffed2;cycles+=3;c.pr=0x8ced03b2;c.pc=0x8ced03ae;cycles+=3;missing=r[14];}phase(3);
 }
 void setup(){auto&r=c.r;for(unsigned i:{14u,13u,12u,11u,10u})push(r[i]);push(c.pr);r[15]-=24;r[6]=w(0x8cecf170);r[13]=0;r[12]=r[4];r[5]=32;r[4]=0;cycles+=12;
  for(;;){r[14]=r[4];r[14]<<=2;r[14]<<=2;r[3]=r[6];r[14]<<=2;r[3]+=4;t(r[14]>>31);r[14]<<=1;r[14]+=r[3];r[2]=w(r[14]+4);t(r[2]==0);cycles+=11;if(c.sr&1)break;r[4]+=1;t(std::int32_t(r[4])>=std::int32_t(r[5]));cycles+=3;if(c.sr&1)break;}
  r[0]=r[4];t(r[0]==32);cycles+=3;
  if(c.sr&1){c.pr=0x8cecf136;r[4]=0xfffffff5;c.pc=0x8cecf132;cycles+=3;missing=0x8cecf512;return;}
  r[3]=w(0x8cecf2ac);r[2]=w(0x8cecf2b0);r[5]=w(r[12]);r[6]=w(r[3]);c.pr=0x8cecf188;r[4]=r[14];c.pc=r[2];cycles+=7;phase(0);validate();
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_copy=nullptr;
static void observed_copy(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);real_copy(c,rt);if(boundary)boundary(2);}
static void observed_validation(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED0350(c,rt);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}

int main(int argc,char**argv){try{
 need(argc==10,"usage: size RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,full_table_controls=0,invalid_selector_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_slot_setup_entries),std::end(movie_slot_setup_entries));for(auto e:movie_setup_validation_entries){e.function=observed_validation;entries.push_back(e);}need(entries.size()==45,"Setup manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cecf0f8u,0x8cecf0f8u,0xacecf0f8u,0x8decf0f8u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cecf0f8),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cecf0fa);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cecf0f8);},"Inactive or unregistered");++activation_controls;}
 auto check=[&](unsigned slot,unsigned selector,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ced18e4)&&!rt.targets.contains(0x8cecf512),"Missing helper unexpectedly registered");auto c=initial;
  if(!capture){c.r[4]=alias?0xacefd000:0x8cefd000;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000301u)|(bank?0x20000301u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}put(rt.main_ram,c.r[4],selector);for(unsigned i=0;i<32;++i)put(rt.main_ram,0x8cedd118+i*128,i<slot?(bank?0xffffffff:1):0);put(rt.main_ram,0x8cedcf2c,bank?0x80000000:59940);}
  const auto before=c;auto expected=rt.main_ram;SetupModel m{{c,expected}};std::vector<Prediction> phases;
  m.observe=[&](unsigned i,const SetupModel&model){if(i==0){need(model.c.r[4]==0x8cedd114+slot*128&&model.c.r[5]==word(expected,before.r[4])&&model.c.r[6]==word(expected,0x8cedcf2c),"Independent selected slot/arguments differ");}
   if(capture){need(i==phases.size(),"Setup prediction order");phases.push_back({model.c,model.m,model.cycles});}};m.setup();const unsigned mode=word(expected,before.r[4]);
  const unsigned predicted=slot==32?466:33+slot*14+(mode==1?37:mode==2?39:17);need(m.cycles==predicted,"Independent setup timing differs");
  if(slot<32&&(mode==1||mode==2)){const unsigned src=mode==1?0x8cedc67c:0x8cedc68c,sp=before.r[15]-72;for(unsigned i=0;i<3;++i)need(word(expected,sp+i*4)==word(ram,src+i*4),"Independent descriptor word differs");need(word(expected,sp+12)==word(expected,0x8cedcf2c)&&m.missing==0x8ced18e4,"Descriptor timing/missing target differs");}
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  unsigned seen=0;auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Size observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Setup boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);need(device==sqfields(rt)&&std::equal(sq.begin(),sq.end(),rt.store_queues.begin()),"Boundary SQ differs");++seen;};
  if(capture){boundary=verify;real_copy=rt.targets.at(0x8c10e958);rt.register_target(0x8c10e958,observed_copy);}
  rejection([&]{call_recompiled(c,rt,0x8cecf0f8|(alias?0x20000000u:0));},"Inactive or unregistered");
  if(capture){verify(3);boundary={};const std::array<unsigned,4> cycles={33,46,63,70};need(seen==4,"Missing setup observations");for(unsigned i=0;i<4;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured setup timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}}
  same(m.c,c);need(expected==rt.main_ram,"Complete setup RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected setup device effects");
  if(capture){auto stem=out+"/setup-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else{++controls;if(slot==32)++full_table_controls;else if(selector!=1&&selector!=2)++invalid_selector_controls;}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired setup entry remains");
 };

 for(unsigned slot=0;slot<=32;++slot)for(unsigned selector:{0u,1u,2u,3u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(slot,selector,alias,bank,false);
 check(0,1,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"setup_controls\":"<<controls<<",\"full_table_controls\":"<<full_table_controls<<",\"invalid_selector_controls\":"<<invalid_selector_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[33,46,63,70],\"new_original_bodies\":2,\"new_original_instructions\":245,\"retained_copy_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED18E4\",\"complete_setup_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie slot setup: "<<controls<<" setup,"<<activation_controls<<" activation controls;33/46/63/70 boundaries;explicit missing ED18E4 and ECF512 error paths\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
