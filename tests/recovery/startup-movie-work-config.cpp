// Original work-pointer adjustment and complete work configuration, stopped at the real next helper.
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





#include "movie-work-pointer/movie-work-pointer-manifest.hpp"
#include "movie-work-config/movie-work-config-manifest.hpp"
#include "movie-word-fill/movie-word-fill-manifest.hpp"
#include "movie-word-copy/movie-word-copy-manifest.hpp"
#include "movie-third-setup/movie-third-setup-manifest.hpp"
struct ConfigModel : RequestModel {
 std::function<void(unsigned,const ConfigModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void pointer(){c.r[2]=w(0x8ced831c);c.r[3]=w(c.r[2]);t(c.r[3]==0);cycles+=4;if(!(c.sr&1)){c.r[3]=w(0x8ced8318);c.r[4]|=c.r[3];cycles+=2;}c.pc=c.pr;c.r[0]=c.r[4];cycles+=3;}
 void fill(){auto&r=c.r;const unsigned n=r[6],end=r[4]+4*n;need(n<=32768,"Unbounded fill");for(unsigned i=0;i<n;++i)put(m,end-4*(i+1),r[5]);r[3]=n<<2;r[6]=n>>4;r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=19+3*(n&15)+18*(n>>4);}
 void words(){auto&r=c.r;need(r[6]==8,"Unexpected word copy size");r[7]=15;push(r[12]);r[7]&=r[6];for(unsigned i=0;i<8;++i){r[3]=w(r[5]);r[5]+=4;put(m,r[4],r[3]);r[4]+=4;}r[7]=0;r[6]>>=4;r[12]=r[6];t(true);c.pc=c.pr;r[12]=pop();cycles+=57;}
 void config(){auto&r=c.r;push(r[14]);push(r[13]);push(r[12]);push(c.pr);r[15]-=12;r[3]=sw(0x8ced84fc);r[2]=w(0x8ced8504);put(m,r[15]+4,r[4]);put(m,r[15],r[5]);r[5]=0;r[6]=w(r[15]+4);r[6]<<=8;r[6]<<=2;r[6]<<=2;t((r[6]&0x80000000u)!=0);r[6]<<=1;put(m,r[15]+8,r[6]);r[6]+=r[3];r[6]>>=2;c.pr=0x8ced8370;r[4]=w(r[15]);c.pc=0x8ced67f8;need(r[2]==c.pc,"Unexpected fill target");cycles+=21;phase(2);fill();phase(3);
  r[5]=w(r[15]);r[2]=0xfffffff3;r[3]=sw(0x8ced84fe);r[12]=w(r[15]+8);r[5]+=r[3];r[1]=sw(0x8ced8500);r[5]>>=13;r[3]=w(0x8ced850c);r[2]=13;r[14]=w(0x8ced8508);r[5]<<=13;r[13]=r[5];r[12]+=r[13];r[1]+=r[12];put(m,r[15],r[1]);r[1]=r[14];
  for(unsigned i=0;i<28;++i){r[0]=sb(r[3]++);if(i==27)++r[1];wb(r[1],r[0]);if(i<26)++r[1];}
  r[0]=r[14];r[1]=w(0x8ced8510);r[0]+=32;
  for(unsigned i=0;i<28;++i){r[2]=sb(r[1]++);if(i==9)++r[0];if(i==20){r[4]=r[14];r[5]=w(0x8ced8514);r[6]=8;}wb(r[0],r[2]);if(i!=8&&i!=27)++r[0];}
  r[2]=w(0x8ced8518);c.pr=0x8ced84ee;r[4]+=64;c.pc=0x8ced6708;need(r[2]==c.pc,"Unexpected copy target");cycles+=192;phase(4);words();phase(5);
  r[0]=104;put(m,r[14]+r[0],r[12]);r[0]=108;r[3]=w(r[15]);put(m,r[14]+r[0],r[3]);r[2]=w(r[15]+4);r[0]=112;put(m,r[14]+r[0],r[2]);r[0]=116;put(m,r[14]+r[0],r[13]);r[15]+=12;c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=19;phase(6);
 }
 void connected(){pointer();phase(0);need(c.pc==0x8ced80da,"Wrong pointer continuation");c.r[5]=c.r[0];c.pr=0x8ced80e0;c.r[4]=w(c.r[15]);c.pc=0x8ced8348;cycles+=4;phase(1);config();need(c.pc==0x8ced80e0,"Wrong config continuation");c.r[3]=w(0x8ced818c);c.r[2]=w(0x8ced8188);c.pr=0x8ced80e8;c.r[14]=w(c.r[2]);c.pc=0x8ced80e0;cycles+=5;phase(7);}
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_config(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);movie_recomp_8CED8348(c,rt);if(boundary)boundary(6);}
static void observed_config_fill(SH4Context&c,DCRuntime&rt){if(boundary)boundary(2);movie_recomp_8CED67F8(c,rt);if(boundary)boundary(3);}
static void observed_config_words(SH4Context&c,DCRuntime&rt){if(boundary)boundary(4);movie_recomp_8CED6708(c,rt);if(boundary)boundary(5);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned pointer_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_work_pointer_entries),std::end(movie_work_pointer_entries));for(auto e:movie_work_config_entries){e.function=observed_config;entries.push_back(e);}for(auto e:movie_word_fill_entries){e.function=observed_config_fill;entries.push_back(e);}for(auto e:movie_word_copy_entries){e.function=observed_config_words;entries.push_back(e);}for(auto e:movie_third_setup_entries)entries.push_back(e);need(entries.size()==37,"Config manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced8206u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced8206u,0x8ced8206u,0xaced8206u,0x8ded8206u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced8206),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced8208);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced8206);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};
 for(unsigned flag:{0u,1u,0xffffffffu,0x80000000u})for(unsigned address:{0u,0x8cee72a0u,0xacee72a0u,0x8eee72a0u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);put(rt.main_ram,0x8cefb96c,flag);auto c=context(0x8ced8206,bank?15:0,bank,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[4]=address;auto expected=rt.main_ram;ConfigModel m{{c,expected}};m.pointer();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xaced8206:0x8ced8206);same(m.c,c);need(expected==rt.main_ram,"Pointer modified RAM");verify_devices(rt,vram,timers,start,tocr);++pointer_controls;}
 auto check=[&](unsigned count,unsigned dest,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced8206,bank?15:0,bank,bank);
  if(!capture){c.pr=0x8ced80da;c.r[15]=0x8cffe000|(alias?0x20000000u:0);c.r[4]=alias?0xacf20020:0x8cf20020;c.sr|=bank?0x300:0;put(rt.main_ram,c.r[15],count);dc_write32(rt,0x8ced8508,dest);std::fill(rt.main_ram.begin()+0xf20000,rt.main_ram.begin()+0xf36000,0xa5);}
  auto expected=rt.main_ram;ConfigModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const ConfigModel&model){need(i==phases.size(),"Config prediction order");phases.push_back({model.c,model.m,model.cycles});};m.connected();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Config observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Config boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i>=1&&i<=6);++seen;};
  if(capture)boundary=verify;call_recompiled(c,rt,alias?0xaced8206:0x8ced8206);if(capture)verify(0);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");boundary={};if(capture)verify(7);same(m.c,c);need(expected==rt.main_ram,"Config complete RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,8> cycles={9,13,34,23093,23285,23342,23361,23366};need(seen==8&&c.pr==0x8ced80e8&&c.pc==0x8ced80e0&&c.r[15]==0x8cfffec4,"Captured config return differs");for(unsigned i=0;i<8;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured config timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(c.r[3]==0x8ced7e44&&c.r[14]==0x8eef9560&&word(rt.main_ram,0x8cefb970)==0x8eef8000&&word(rt.main_ram,0x8cefb974)==0x8eef9560&&word(rt.main_ram,0x8cefb978)==8&&word(rt.main_ram,0x8cefb97c)==0x8eee8000,"Captured descriptor values differ");}
  else{const unsigned bytes=(count<<13)+0x4000;need(bytes<=131072&&rt.main_ram[0xf2001f]==0xa5&&rt.main_ram[0xf20020+bytes]==0xa5,"Config clear bounds differ");++connected_controls;}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired config entry remains");
 };
 for(unsigned count:{0u,1u,8u,0x80000000u,0xffffffffu,0x80000001u})for(unsigned dest:{0x8cefb908u,0x8cedcb50u,0x8cedcb4cu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(count,dest,alias,bank,false);
 check(8,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"pointer_controls\":"<<pointer_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[9,13,34,23093,23285,23342,23361,23366],\"new_original_bodies\":2,\"new_original_instructions\":237,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"captured_clear_bytes\":81920,\"captured_copy_words\":8,\"captured_new_PREF_calls\":0,\"next_missing_target\":\"8CED7E44\",\"full_startup_connection_verified\":false,\"captured_config_return_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie config: "<<pointer_controls<<" pointer,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured23366cycles; explicit missing ED7E44\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
