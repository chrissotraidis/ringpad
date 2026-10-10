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


#include "movie-mode-config/movie-mode-config-manifest.hpp"
#include "movie-mode-tail/movie-mode-tail-manifest.hpp"
struct ConfigModel : RequestModel {
 std::function<void(unsigned,const ConfigModel&)> observe;
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }
 void divide(){auto&r=c.r;t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e728);r[2]=w(0x8c10e72c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;div0s(2,1);const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}rot(1);const std::uint64_t sum=std::uint64_t(r[1])+r[2]+(c.sr&1);r[1]=unsigned(sum);t(sum>>32);r[0]=r[1];r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=80;
 }
 void tail(){auto&r=c.r;r[6]=w(0x8ced57a8);r[7]=w(0x8ced57a4);r[0]=r[4];t(r[0]==2);r[5]=2;cycles+=7;
  if(c.sr&1){r[2]=1;put(m,r[6],r[2]);put(m,r[7],r[5]);cycles+=4;}else{t(r[0]==3);cycles+=2;if(c.sr&1){r[3]=3;put(m,r[6],r[5]);put(m,r[7],r[3]);cycles+=3;}}
  c.pc=c.pr;cycles+=3;
 }
 void config(){auto&r=c.r;r[1]=w(0x8cecf144);r[3]=w(0x8cecf148);push(r[14]);r[2]=w(0x8cecf14c);push(c.pr);put(m,r[1],r[4]);put(m,r[3],r[5]);r[3]=4;r[14]=w(0x8cecf154);t((r[4]&r[3])==0);r[1]=w(0x8cecf150);put(m,r[2],r[6]);put(m,r[1],r[7]);cycles+=14;
  if(!(c.sr&1)){t(r[6]>>31);r[6]<<=1;++cycles;}
  r[2]=2;r[5]=w(0x8cecf158);t((r[2]&r[4])==0);put(m,r[14],r[6]);cycles+=5;
  if(!(c.sr&1)){r[1]=w(0x8cecf15c);put(m,r[5],r[1]);cycles+=3;}else{r[0]=w(0x8cecf160);put(m,r[5],r[0]);cycles+=2;}
  r[3]=w(0x8cecf164);r[1]=w(r[5]);c.pr=0x8cecf0d0;r[0]=w(r[14]);c.pc=r[3];cycles+=5;if(observe)observe(0,*this);divide();if(observe)observe(1,*this);
  r[2]=w(0x8cecf168);put(m,r[2],r[0]);r[0]=r[7];t(r[0]==2);cycles+=5;
  bool two=c.sr&1,three=false;if(!two){t(r[0]==3);cycles+=2;three=c.sr&1;}
  if(two||three){c.pr=pop();const unsigned reg=two?1:3;r[reg]=w(0x8cecf16c);r[4]=two?2:3;c.pc=r[reg];r[14]=pop();cycles+=6;if(observe)observe(2,*this);tail();}
  else{c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=4;}if(observe)observe(3,*this);
 }
};
struct ModePrediction {SH4Context c;std::vector<std::uint8_t> ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> mode_boundary;
static thread_local RecompiledFunction real_division=nullptr;
static void observed_division(SH4Context&c,DCRuntime&rt){if(mode_boundary)mode_boundary(0);real_division(c,rt);if(mode_boundary)mode_boundary(1);}
static void observed_mode_tail(SH4Context&c,DCRuntime&rt){if(mode_boundary)mode_boundary(2);movie_recomp_8CED56EE(c,rt);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: mode-config RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned config_controls=0,tail_controls=0,division_controls=0,activation_controls=0;
 const std::array<unsigned,8> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8},values={1,5,2,2,1,1,13,1};auto replay=movie;for(unsigned i=0;i<8;++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_mode_config_entries),std::end(movie_mode_config_entries));for(auto e:movie_mode_tail_entries){e.function=observed_mode_tail;entries.push_back(e);}need(entries.size()==16,"Mode manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(unsigned i=0;i<8;++i)put(rt.main_ram,addresses[i],word(movie,addresses[i]-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<8;++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cecf096u,0x8cecf096u,0xacecf096u,0x8decf096u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cecf096),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cecf098);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cecf096);},"Inactive or unregistered");++activation_controls;restore(rt);activate(rt);}
 auto check=[&](unsigned kind,unsigned flags,unsigned divisor,unsigned mode,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;
  if(!capture){c.r[4]=kind==1?mode:flags;c.r[5]=bank?13:5;c.r[6]=divisor;c.r[7]=mode;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.r[0]=divisor;c.r[1]=flags;c.sr=(c.sr&~0x20000301u)|(bank?0x20000301u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}}
  const auto before=c;auto expected=rt.main_ram;ConfigModel m{{c,expected}};std::vector<ModePrediction> phases;if(capture)m.observe=[&](unsigned i,const ConfigModel&model){need(i==phases.size(),"Mode prediction order");phases.push_back({model.c,model.m,model.cycles});};
  if(kind==0)m.config();else if(kind==1)m.tail();else m.divide();const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  if(kind==0){const unsigned f=before.r[4],d=(f&4)?before.r[6]<<1:before.r[6],q=d?unsigned(std::int64_t(std::int32_t((f&2)?50000:59940))/std::int64_t(std::int32_t(d))):0;
   need(word(expected,0x8cedcf34)==q,"Independent integer quotient differs");const unsigned predicted=26+bool(f&4)+bool(f&2)+(d?80:10)+(before.r[7]==2?25:before.r[7]==3?28:11);need(m.cycles==predicted,"Independent configuration timing differs");}
  if(kind==2){const unsigned d=before.r[0],n=before.r[1];if(d&&!(n==0x80000000&&d==0xffffffff))need(m.c.r[0]==unsigned(std::int64_t(std::int32_t(n))/std::int64_t(std::int32_t(d))),"Independent signed quotient differs");need(m.cycles==(d?80u:10u),"Division timing differs");}
  if(kind==1)need(m.cycles==(mode==2?14u:mode==3?15u:12u),"Tail timing differs");
  if(capture)for(unsigned i=0;i<phases.size();++i){const auto&p=phases[i];auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
  unsigned seen=0;auto verify_phase=[&](unsigned i){need(i==seen&&i<phases.size(),"Mode observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Mode boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);++seen;};
  if(capture){mode_boundary=verify_phase;real_division=rt.targets.at(0x8c10e67c);rt.register_target(0x8c10e67c,observed_division);}
  const unsigned entry=kind==0?0x8cecf096:kind==1?0x8ced56ee:0x8c10e67c;call_recompiled(c,rt,entry|(alias?0x20000000u:0));
  if(capture){verify_phase(3);mode_boundary={};need(seen==4&&phases[0].cycles==26&&phases[1].cycles==106&&phases[2].cycles==117&&phases[3].cycles==131,"Captured mode boundaries differ");}
  same(m.c,c);need(expected==rt.main_ram,"Complete mode RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Mode timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Mode SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected mode device effects");need(c.r[15]==before.r[15]&&c.pr==before.pr&&c.pc==before.pr,"Mode caller state not restored");
  if(capture){auto stem=out+"/mode-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else if(kind==0)++config_controls;else if(kind==1)++tail_controls;else ++division_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());need(!rt.targets.contains(0x8cecf096)&&!rt.targets.contains(0x8ced56ee),"Retired mode entry remains");
 };
 for(unsigned flags=0;flags<8;++flags)for(unsigned divisor:{0u,1u,2u,0xffffffffu,0x80000000u,0x40000000u})for(unsigned mode:{0u,2u,3u,4u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(0,flags,divisor,mode,alias,bank,false);
 for(unsigned mode:{0u,2u,3u,4u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(1,0,0,mode,alias,bank,false);
 for(unsigned dividend:{0u,1u,59940u,50000u,0xffffffffu,0x80000000u,0x7fffffffu})for(unsigned divisor:{0u,1u,2u,0xffffffffu,0x80000000u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(2,dividend,divisor,0,alias,bank,false);
 check(0,0,0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"config_controls\":"<<config_controls<<",\"tail_controls\":"<<tail_controls<<",\"division_controls\":"<<division_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[26,106,117,131],\"nonnull_division_cycles\":80,\"zero_division_cycles\":10,\"new_original_bodies\":2,\"new_original_instructions\":67,\"retained_division_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_image_validated_before_eight_store_replay\":true,\"complete_mode_configuration_return_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie mode configuration: "<<config_controls<<" config,"<<tail_controls<<" tail,"<<division_controls<<" division,"<<activation_controls<<" activation controls;26/106/117/131 captured boundaries;retained division and exact original MOVIE activation\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
