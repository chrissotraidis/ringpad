// Original second MOVIE callback table with retained interrupt-mask helpers.
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


#include "movie-callback-register2/movie-callback-register2-manifest.hpp"
#include "movie-callback-enter/movie-callback-enter-manifest.hpp"
#include "movie-callback-leave/movie-callback-leave-manifest.hpp"
struct CallbackModel : RequestModel {
 std::function<void(unsigned,const CallbackModel&)> observe;
 void enter(){auto&r=c.r;r[0]=c.sr;r[3]=0xffffff0f;r[0]>>=2;r[0]>>=2;r[0]&=15;put(m,r[4],r[0]);r[0]=c.sr;r[0]&=r[3];r[0]|=240;c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=18;}
 void leave(){auto&r=c.r;r[0]=w(r[4]);r[2]=c.sr;r[1]=0xffffff0f;r[0]&=15;r[0]<<=2;r[2]&=r[1];r[0]<<=2;r[0]|=r[2];c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=16;}
 bool parent(){auto&r=c.r;push(c.pr);r[15]-=12;put(m,r[15]+8,r[4]);put(m,r[15]+4,r[5]);c.pr=0x8ced54a0;r[4]=r[15];c.pc=0x8ced5324;cycles+=7;if(observe)observe(0,*this);enter();if(observe)observe(1,*this);
  r[7]=w(0x8ced5530);r[6]=16;r[4]=0;cycles+=4;
  for(;;){t(std::int32_t(r[4])>=std::int32_t(r[6]));cycles+=2;if(c.sr&1)break;
   r[5]=r[4];r[5]<<=2;t(r[5]>>31);r[5]<<=1;r[5]+=r[7];r[3]=w(r[5]);t(r[3]==0);cycles+=7;
   if(c.sr&1){r[2]=w(r[15]+8);put(m,r[5],r[2]);r[3]=w(r[15]+4);put(m,r[5]+4,r[3]);cycles+=5;break;}
   ++r[4];++cycles;
  }
  r[0]=r[4];t(r[0]==16);cycles+=4;
  if(c.sr&1){r[3]=w(0x8ced5534);c.pr=0x8ced54d4;push(r[3]);c.pc=0x8ced54ce;cycles+=4;return false;}
  c.pr=0x8ced54da;r[4]=r[15];c.pc=0x8ced533c;cycles+=3;if(observe)observe(2,*this);leave();if(observe)observe(3,*this);
  r[15]+=12;c.pr=pop();c.pc=c.pr;cycles+=5;if(observe)observe(4,*this);return true;
 }
};
struct CallbackPrediction {SH4Context c;std::vector<std::uint8_t> ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> callback_boundary;
static void observed_callback_enter(SH4Context&c,DCRuntime&rt){if(callback_boundary)callback_boundary(0);movie_recomp_8CED5324(c,rt);if(callback_boundary)callback_boundary(1);}
static void observed_callback_leave(SH4Context&c,DCRuntime&rt){if(callback_boundary)callback_boundary(2);movie_recomp_8CED533C(c,rt);if(callback_boundary)callback_boundary(3);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: callback-register RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned parent_controls=0,full_table_controls=0,lock_controls=0,activation_controls=0;
 const std::array<unsigned,17> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc},values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2};auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_callback_register2_entries),std::end(movie_callback_register2_entries));for(auto e:movie_callback_enter_entries){e.function=observed_callback_enter;entries.push_back(e);}for(auto e:movie_callback_leave_entries){e.function=observed_callback_leave;entries.push_back(e);}need(entries.size()==13,"Callback manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],word(movie,addresses[i]-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced5494u,0x8ced5494u,0xaced5494u,0x8ded5494u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced5494),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced5496);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced5494);},"Inactive or unregistered");++activation_controls;restore(rt);activate(rt);}
 auto variant=[&](SH4Context&c,unsigned mask,unsigned alias,unsigned bank){c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x200000f1u)|(mask<<4)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}};
 auto check=[&](unsigned slot,unsigned mask,unsigned alias,unsigned bank,unsigned null_callback,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;
  if(!capture){variant(c,mask,alias,bank);c.r[4]=null_callback?0:0x8cecfbdc;c.r[5]=(mask&1)?0xdeadbeef:0;for(unsigned i=0;i<16;++i){put(rt.main_ram,0x8cefb344+i*8,i<slot?0x8c123400+i*4:0);put(rt.main_ram,0x8cefb348+i*8,0xa5a50000+i);}}
  const auto before=c;auto expected=rt.main_ram;CallbackModel m{{c,expected}};std::vector<CallbackPrediction> phases;if(capture)m.observe=[&](unsigned i,const CallbackModel&model){need(i==phases.size(),"Callback prediction order");phases.push_back({model.c,model.m,model.cycles});};
  const bool completed=m.parent();need(completed==(slot<16),"Callback predicted path differs");need(m.cycles==(completed?71+10*slot:199),"Independent callback timing differs");const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  if(capture)for(unsigned i=0;i<phases.size();++i){const auto&p=phases[i];auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
  unsigned seen=0;auto verify_phase=[&](unsigned i){need(i==seen&&i<phases.size(),"Callback observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Callback boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);++seen;};
  if(capture)callback_boundary=verify_phase;
  bool missing=false;try{call_recompiled(c,rt,alias?0xaced5494:0x8ced5494);}catch(const std::runtime_error&e){if(completed||std::string(e.what()).find("Inactive or unregistered code module target")==std::string::npos)throw;missing=true;}
  need(missing!=completed,"Callback runtime outcome differs");if(capture){verify_phase(4);callback_boundary={};need(seen==5&&phases[0].cycles==7&&phases[1].cycles==25&&phases[2].cycles==50&&phases[3].cycles==66&&phases[4].cycles==71,"Captured callback boundaries differ");}
  need(std::equal(ram.begin()+0xefb2c4,ram.begin()+0xefb344,rt.main_ram.begin()+0xefb2c4),"First callback table changed");
  same(m.c,c);need(expected==rt.main_ram,"Complete callback RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Callback timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Callback SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected callback device effects");
  if(completed){need(c.r[15]==before.r[15]&&c.pr==before.pr&&c.pc==before.pr,"Callback caller state not restored");need((c.sr&0xf0)==(before.sr&0xf0),"Callback interrupt mask not restored");need(word(rt.main_ram,0x8cefb344+slot*8)==before.r[4]&&word(rt.main_ram,0x8cefb348+slot*8)==before.r[5],"Callback slot content differs");}
  else{need(!rt.targets.contains(0x8ced5374)&&c.pc==0x8ced54ce&&c.pr==0x8ced54d4&&c.r[15]==before.r[15]-20&&word(rt.main_ram,c.r[15])==0x8cedca50,"Full table did not reach original explicit error call");++full_table_controls;}
  if(capture){auto stem=out+"/callback-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else ++parent_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(unsigned address:{0x8ced5494u,0x8ced5324u,0x8ced533cu})need(!rt.targets.contains(address),"Retired callback entry remains");
 };
 for(unsigned slot=0;slot<=16;++slot)for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned null_callback:{0u,1u})check(slot,mask,alias,bank,null_callback,false);
 check(0,0,0,0,0,true);
 for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned high:{0u,0x5a5a0000u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;variant(c,mask,alias,bank);c.r[4]=alias?0xacedf000:0x8cedf000;const auto old_sr=c.sr;auto expected=rt.main_ram;CallbackModel m{{c,expected}};const auto timers0=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;
  auto verify=[&]{same(m.c,c);need(expected==rt.main_ram,"Mask pair RAM differs");auto timers=timers0;tick(timers,start,m.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Mask timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Mask SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected mask device effects");};
  m.enter();call_recompiled(c,rt,alias?0xaced5324:0x8ced5324);verify();need(c.sr==(old_sr|0xf0)&&word(rt.main_ram,c.r[4])==mask,"Enter mask/save differs");put(expected,c.r[4],mask|high);put(rt.main_ram,c.r[4],mask|high);m.leave();call_recompiled(c,rt,alias?0xaced533c:0x8ced533c);verify();need(c.sr==old_sr&&m.cycles==34,"Leave did not restore complete status");++lock_controls;
 }
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"parent_controls\":"<<parent_controls<<",\"full_table_controls\":"<<full_table_controls<<",\"lock_controls\":"<<lock_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[7,25,50,66,71],\"occupied_slot_extra_cycles\":10,\"full_table_error_entry_cycles\":199,\"interrupt_masks\":16,\"new_original_bodies\":1,\"new_original_instructions\":39,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_image_validated_before_seventeen_store_replay\":true,\"complete_available_slot_registration_verified\":true,\"retained_mask_helpers_executed\":true,\"first_callback_table_preserved\":true,\"full_table_error_target_explicit_unregistered\":true,\"full_table_error_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie second callback registration: "<<parent_controls<<" parent,"<<full_table_controls<<" full table,"<<lock_controls<<" mask,"<<activation_controls<<" activation controls;7/25/50/66/71 captured boundaries;full table reaches explicit ED5374 at199 cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
