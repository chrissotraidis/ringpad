// Original MOVIE rate setup, original return leaf and retained buffer clear through the next unresolved service.
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
#include "movie-word-copy/movie-word-copy-manifest.hpp"
#include "movie-record-clear/movie-record-clear-manifest.hpp"
#include "movie-rate-init/movie-rate-init-manifest.hpp"
#include "movie-context-return/movie-context-return-manifest.hpp"
namespace dcrecomp_generated {void movie_bounded_string_register(DCRuntime&);void recomp_8C11157C(SH4Context&,DCRuntime&);}
struct RateModel : RequestModel {
 unsigned prefs=0;
 std::function<void(unsigned,const RateModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}
 void copy(){auto&r=c.r;const unsigned n=r[6];r[1]=0;r[0]=r[5];r[7]=r[4];r[5]=0;unsigned copied=0;
  while(r[5]<n){r[3]=sb(r[0]);if(!r[3])break;wb(r[7]++,r[3]);++r[0];++r[5];++copied;}
  while(r[5]<n){wb(r[7]++,0);++r[5];}r[0]=r[4];t(true);c.pc=c.pr;cycles+=copied==n?10+9*n:15+5*n+4*copied;
 }
 void words(){auto&r=c.r;const unsigned n=r[6];r[7]=15;push(r[12]);r[7]&=r[6];for(unsigned i=0;i<(n&15);++i){r[3]=w(r[5]);r[5]+=4;put(m,r[4],r[3]);r[4]+=4;}r[7]=0;r[6]>>=4;r[12]=r[6];
  for(unsigned block=0;block<(n>>4);++block){
   for(unsigned reg:{6u,7u,0u,1u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+4*i,r[std::array<unsigned,4>{6,7,0,1}[i]]);
   for(unsigned reg:{6u,7u,0u,1u}){r[reg]=w(r[5]);r[5]+=4;}put(m,r[4]+16,r[6]);r[3]=r[5]+32;put(m,r[4]+20,r[7]);put(m,r[4]+24,r[0]);put(m,r[4]+28,r[1]);++prefs;
   for(unsigned reg:{7u,0u,1u,6u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+32+4*i,r[std::array<unsigned,4>{7,0,1,6}[i]]);
   for(unsigned reg:{1u,0u,7u,6u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+48+4*i,r[std::array<unsigned,4>{1,0,7,6}[i]]);r[4]+=64;
  }t(true);c.pc=c.pr;r[12]=pop();cycles+=17+5*(n&15)+38*(n>>4);
 }
 void copy16(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[0]+r[3]);r[0]-=4;need(r[3]==0x8c10e9a6,"Unexpected16-byte copy dispatch");r[0]=w(r[0]+r[2]);r[3]=w(r[2]+8);put(m,r[1]+12,r[0]);r[0]=w(r[2]+4);put(m,r[1]+8,r[3]);r[3]=w(r[2]);put(m,r[1]+4,r[0]);put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=17;}
 void clear(){c.r[5]=0;for(unsigned i=0;i<5;++i)put(m,c.r[4]+4*i,0);c.pc=c.pr;cycles+=10;}
 void rate(){c.r[6]=0;put(m,c.r[4],0);put(m,c.r[4]+4,0);put(m,c.r[4]+8,c.r[5]);c.pc=c.pr;cycles+=6;}
 void leaf(){c.pc=c.pr;cycles+=3;}
 void fields(){auto&r=c.r;push(r[14]);r[6]=94;push(r[13]);push(r[12]);r[12]=r[4];push(r[11]);push(r[10]);push(c.pr);r[4]=w(0x8ced1a08);r[3]=w(0x8ced1a0c);c.pr=0x8ced1920;r[5]=0;c.pc=0x8ced67f8;cycles+=13;phase(1);
  fill();phase(2);
  r[5]=w(0x8ced1a10);r[6]=32;r[2]=w(0x8ced1a14);r[14]=w(0x8ced1a08);r[4]=r[14];c.pr=0x8ced192c;c.pc=0x8c11157c;cycles+=7;phase(3);copy();phase(4);
  r[3]=w(0x8ced1a14);r[6]=32;r[5]=w(0x8ced1a18);r[4]=r[14]+32;c.pr=0x8ced1938;c.pc=0x8c11157c;cycles+=7;phase(5);copy();phase(6);
  r[2]=w(0x8ced1a20);r[6]=47;r[5]=w(0x8ced1a1c);r[4]=r[14]+64;c.pr=0x8ced1944;c.pc=0x8ced6708;cycles+=7;phase(7);words();phase(8);
  r[1]=unsigned(int(std::int16_t(w(0x8ced19f2)&0xffff)));r[2]=r[12];r[3]=w(0x8ced1a24);r[1]+=r[14];c.pr=0x8ced1950;r[0]=16;c.pc=0x8c10e958;cycles+=7;phase(9);copy16();phase(10);
  r[0]=unsigned(int(std::int16_t(w(0x8ced19f4)&0xffff)));r[2]=0;r[4]=unsigned(int(std::int16_t(w(0x8ced19f6)&0xffff)));put(m,r[14]+r[0],r[2]);c.pr=0x8ced195c;r[4]+=r[14];c.pc=0x8ced1a90;cycles+=7;phase(11);clear();phase(12);
  r[2]=w(0x8ced1a28);r[4]=unsigned(int(std::int16_t(w(0x8ced19f8)&0xffff)));r[5]=w(r[12]+12);c.pr=0x8ced1966;r[4]+=r[14];c.pc=0x8ced4620;cycles+=6;phase(13);rate();phase(14);
  r[3]=w(0x8ced1a2c);r[4]=unsigned(int(std::int16_t(w(0x8ced19fa)&0xffff)));c.pr=0x8ced196e;r[4]+=r[14];c.pc=0x8ced97a8;cycles+=5;phase(15);leaf();phase(16);
  r[11]=unsigned(int(std::int16_t(w(0x8ced19fe)&0xffff)));r[5]=0;r[13]=w(r[12]+8);r[0]=unsigned(int(std::int16_t(w(0x8ced19fc)&0xffff)));c.macl=r[11]*r[13];r[10]=w(r[12]+4);put(m,r[14]+r[0],r[13]);r[0]+=4;r[2]=w(0x8ced1a0c);r[6]=c.macl;put(m,r[14]+r[0],r[10]);r[6]>>=2;c.pr=0x8ced198a;r[4]=r[10];c.pc=0x8ced67f8;cycles+=16;phase(17);fill();phase(18);
  r[6]=0;t(int(r[13])>0);r[4]=0;r[5]=r[6];r[4]+=r[10];cycles+=6;
  if(c.sr&1)do{++r[5];r[0]=64;t(int(r[5])>=int(r[13]));put(m,r[4]+r[0],r[6]);r[4]+=r[11];cycles+=6;}while(!(c.sr&1));
  r[2]=w(0x8ced1a30);r[4]=unsigned(int(std::int16_t(w(0x8ced1a00)&0xffff)));r[5]=w(r[12]);c.pr=0x8ced19ac;r[4]+=r[14];c.pc=0x8ced19a2;cycles+=6;phase(19);
 }
 void init(){push(c.pr);c.pr=0x8ced18ea;c.pc=0x8ced1908;cycles+=4;phase(0);fields();}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles,prefs;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_fields(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED1908(c,rt);}
static void observed_fill(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8ced1920?1:17;if(boundary)boundary(phase);movie_recomp_8CED67F8(c,rt);if(boundary)boundary(phase+1);}
static void observed_copy(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8ced192c?3:5;if(boundary)boundary(phase);recomp_8C11157C(c,rt);if(boundary)boundary(phase+1);}
static thread_local RecompiledFunction real_descriptor_copy=nullptr;
static void observed_words(SH4Context&c,DCRuntime&rt){if(boundary)boundary(7);movie_recomp_8CED6708(c,rt);if(boundary)boundary(8);}
static void observed_descriptor_copy(SH4Context&c,DCRuntime&rt){if(boundary)boundary(9);real_descriptor_copy(c,rt);if(boundary)boundary(10);}
static void observed_clear(SH4Context&c,DCRuntime&rt){if(boundary)boundary(11);movie_recomp_8CED1A90(c,rt);if(boundary)boundary(12);}
static void observed_rate(SH4Context&c,DCRuntime&rt){if(boundary)boundary(13);movie_recomp_8CED4620(c,rt);if(boundary)boundary(14);}
static void observed_leaf(SH4Context&c,DCRuntime&rt){if(boundary)boundary(15);movie_recomp_8CED97A8(c,rt);if(boundary)boundary(16);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}

int main(int argc,char**argv){try{
 need(argc==10,"usage: size RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned rate_controls=0,leaf_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_descriptor_init_entries),std::end(movie_descriptor_init_entries));for(auto e:movie_descriptor_fields_entries){e.function=observed_fields;entries.push_back(e);}for(auto e:movie_word_fill_entries){e.function=observed_fill;entries.push_back(e);}for(auto e:movie_word_copy_entries){e.function=observed_words;entries.push_back(e);}for(auto e:movie_record_clear_entries){e.function=observed_clear;entries.push_back(e);}for(auto e:movie_rate_init_entries){e.function=observed_rate;entries.push_back(e);}for(auto e:movie_context_return_entries){e.function=observed_leaf;entries.push_back(e);}need(entries.size()==40,"Rate setup manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c11157c),"BOOT copy already registered");movie_bounded_string_register(rt);rt.targets.at(0x8c11157c)=observed_copy;real_descriptor_copy=rt.targets.at(0x8c10e958);rt.targets.at(0x8c10e958)=observed_descriptor_copy;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced4620u,0x8ced4620u,0xaced4620u,0x8ded4620u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced18e4),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced4622);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced18e4);},"Inactive or unregistered");++activation_controls;}

 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,unsigned prefs,bool active=false){auto expected_device=device;expected_device[3]+=prefs;for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Rate setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&expected_device==sqfields(rt),"Rate setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected rate-init device effects");};
 for(bool rate:{false,true})for(unsigned value:{0u,1u,59940u,0x80000000u,0xffffffffu,0x12345678u})for(unsigned offset:{0u,4u,28u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(rate?0x8ced4620:0x8ced97a8,15,bank,bank);c.sr|=bank?0x300:0;c.pr=alias?0xac112234:0x8c112234;c.r[4]=(alias?0xacef4000:0x8cef4000)+offset;c.r[5]=value;for(unsigned i=0;i<80;++i)rt.main_ram[0xef3ff0+i]=0xa5;auto expected=rt.main_ram;RateModel m{{c,expected}};if(rate)m.rate();else m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,(rate?0x8ced4620u:0x8ced97a8u)|(alias?0x20000000u:0));same(m.c,c);need(expected==rt.main_ram,"Rate/leaf complete RAM differs");verify_devices(rt,vram,timers,start,tocr,0);if(rate)++rate_controls;else ++leaf_controls;}
 auto connected=[&](unsigned count,unsigned alias,unsigned bank,bool capture) {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ceda288),"Next helper unexpectedly registered");auto c=initial;
  if(!capture){c.r[4]|=alias?0x20000000u:0;c.r[15]|=alias?0x20000000u:0;c.sr=(c.sr&~0x301u)|(bank?0x301:0);c.fpscr^=bank<<21;put(rt.main_ram,c.r[4]+4,alias?0xacf40000:0x8cf40000);put(rt.main_ram,c.r[4]+8,count);put(rt.main_ram,c.r[4]+12,bank?0xffffffff:0x80000000);std::fill(rt.main_ram.begin()+0xf3fff0,rt.main_ram.begin()+0xf50010,0xa5);}
  auto expected=rt.main_ram;RateModel m{{c,expected}};std::vector<Prediction> phases;
  if(capture)m.observe=[&](unsigned i,const RateModel&model){need(i==phases.size(),"Rate setup prediction order");phases.push_back({model.c,model.m,model.cycles,model.prefs});};m.init();if(capture)need(m.cycles==5594,"Connected rate-init timing differs");
  need(m.c.r[4]==0x8cee6f18&&m.c.r[5]==0x8cedc604&&m.c.r[2]==0x8ceda288,"Next original service arguments differ");
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Rate setup observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Rate setup boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,p.prefs,i<19);++seen;};
  if(capture)boundary=verify;
  rejection([&]{call_recompiled(c,rt,0x8ced18e4);},"Inactive or unregistered");boundary={};if(capture)verify(19);same(m.c,c);need(expected==rt.main_ram,"Complete connected RAM differs");verify_devices(rt,vram,timers,start,tocr,m.prefs);
  if(capture){const std::array<unsigned,20> cycles={4,17,168,175,458,465,748,755,923,930,947,954,964,970,976,981,984,1000,5576,5594};need(seen==cycles.size(),"Missing rate-init observations");
  for(unsigned i=0;i<cycles.size();++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured rate-init timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");auto expected_device=device;expected_device[3]+=p.prefs;save_sq(sq,expected_device,stem);}
  need(word(rt.main_ram,0x8cee6f08)==0&&word(rt.main_ram,0x8cee6f0c)==0&&word(rt.main_ram,0x8cee6f10)==59940,"Captured three rate words differ");
  need(std::all_of(rt.main_ram.begin()+0xee2210,rt.main_ram.begin()+0xee6154,[](auto b){return b==0;})&&m.prefs==2,"Captured16196-byte clear/PREF differs");
  need(word(rt.main_ram,0x8cee6f54)==1&&word(rt.main_ram,0x8cee6f58)==0x8cee2210&&m.c.macl==0x3f44,"Captured count/buffer/MUL.L stores differ");
  }else{++connected_controls;const unsigned bytes=unsigned(0x3f44u*count)&~3u;need(bytes<=65536&&std::all_of(rt.main_ram.begin()+0xf40000,rt.main_ram.begin()+0xf40000+bytes,[](auto b){return b==0;}),"Variant clear contract differs");need(rt.main_ram[0xf3ffff]==0xa5&&rt.main_ram[0xf40000+bytes]==0xa5,"Variant clear crossed bounds");}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired rate-init entry remains");need(rt.targets.contains(0x8c11157c),"Resident BOOT helper retired with MOVIE");
 }
 ;
 for(unsigned count:{0u,1u,2u,3u,0x80000000u,0xc0000000u,0x80000001u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})connected(count,alias,bank,false);
 connected(1,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"rate_controls\":"<<rate_controls<<",\"leaf_controls\":"<<leaf_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[4,17,168,175,458,465,748,755,923,930,947,954,964,970,976,981,984,1000,5576,5594],\"captured_PREF_calls\":2,\"new_original_bodies\":2,\"new_original_instructions\":7,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CEDA288\",\"complete_initializer_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie rate setup: "<<rate_controls<<" rate,"<<leaf_controls<<" leaf,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; connected5594cycles;explicit missing EDA288\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
