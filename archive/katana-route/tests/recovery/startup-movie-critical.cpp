// Original movie allocation prefix and interrupt-mask enter/leave contracts.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void movie_allocation_register(DCRuntime&);void movie_lock_enter_register(DCRuntime&);void movie_lock_leave_register(DCRuntime&);}

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}







struct CriticalModel : RequestModel {
 bool parent(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[3]=w(0x8c5ffa44);put(m,r[15]+4,r[4]);r[0]=w(r[3]);t(r[0]==0xffffffff);r[14]=0;cycles=9;
  if(!(c.sr&1)){r[0]=0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;return false;}
  c.pr=0x8c5ff952;r[4]=r[15];c.pc=0x8c5ffa5c;cycles+=3;return true;
 }
 // Pinned issue costs: STC_SR=2, LDC_SR=4, RTS=2, other instructions=1.
 void enter(){auto&r=c.r;r[0]=c.sr;r[3]=0xffffff0f;r[0]>>=2;r[0]>>=2;r[0]&=15;put(m,r[4],r[0]);r[0]=c.sr;r[0]&=r[3];r[0]|=240;c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=18;}
 void leave(){auto&r=c.r;r[0]=w(r[4]);r[2]=c.sr;r[1]=0xffffff0f;r[0]&=15;r[0]<<=2;r[2]&=r[1];r[0]<<=2;r[0]|=r[2];c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=16;}
 void missing(){c.pr=0x8c5ff956;c.pc=0x8c5ff952;cycles+=3;}
};
static RecompiledFunction real_enter;
static std::function<void(bool,const SH4Context&,const DCRuntime&)> observe_enter;
static void watched_enter(SH4Context&c,DCRuntime&rt){if(observe_enter)observe_enter(false,c,rt);real_enter(c,rt);if(observe_enter)observe_enter(true,c,rt);}
int main(int argc,char**argv){try{
 need(argc==9,"usage: movie-critical RAM STATE TIMERS SQ SQSTATE FLASH AICA OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[8];unsigned parent_controls=0,lock_controls=0;
 need(ram.size()==16777216&&sq.size()==64&&flash.size()==131072&&sound.size()==2097152,"Fixture sizes differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);movie_allocation_register(rt);movie_lock_enter_register(rt);movie_lock_leave_register(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);return owner;};
 auto variant=[&](SH4Context&c,unsigned mask,unsigned alias,unsigned bank){c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8cecfd58;c.sr=(c.sr&~0x200000f1u)|(mask<<4)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}};
 auto check=[&](unsigned guard,unsigned mask,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;auto c=initial;if(!capture){variant(c,mask,alias,bank);c.r[4]=(mask&1)?0xa5a55a5a:0;put(rt.main_ram,0x8c677144,guard);}
  auto expected=rt.main_ram;CriticalModel m{{c,expected}};const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto initial_timers=rt.tmu;const auto vram=rt.vram;
  auto verify=[&](const SH4Context&actual,const DCRuntime&actual_rt){same(m.c,actual);need(expected==actual_rt.main_ram,"Critical phase RAM differs");auto timers=initial_timers;tick(timers,start,m.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],actual_rt.tmu[i]);need(start==actual_rt.tmu_tstr&&tocr==actual_rt.tmu_tocr&&!actual_rt.sh4_tick_pending_cycles&&actual_rt.sh4_tick_batch_cycles==1,"Critical phase timer controls differ");need(std::equal(sq.begin(),sq.end(),actual_rt.store_queues.begin())&&device==sqfields(actual_rt),"Critical SQ/device differs");need(actual_rt.aica_ram==sound&&actual_rt.system_flash==flash&&actual_rt.vram==vram&&!actual_rt.device_clock_enabled&&!actual_rt.aica_arm.instructions&&!actual_rt.holly_irq_taken&&!actual_rt.sh4_async_redirect&&actual_rt.pvr_ta_stream.empty(),"Unexpected critical device effects");};
  auto save=[&](unsigned phase){if(!capture)return;auto timers=initial_timers;tick(timers,start,m.cycles);auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(m.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);};
  const bool active=m.parent();unsigned visits=0;real_enter=rt.targets.at(0x8c5ffa5c);
  observe_enter=[&](bool after,const SH4Context&actual,const DCRuntime&actual_rt){need(active,"Busy guard unexpectedly entered lock");if(after){need(visits==1,"Unexpected enter return");m.enter();}else need(visits==0,"Repeated enter");verify(actual,actual_rt);save(after?1:0);++visits;};
  rt.register_target(0x8c5ffa5c,watched_enter);bool missing=false;try{call_recompiled(c,rt,alias?0xac5ff932:0x8c5ff932);}catch(const std::runtime_error&e){if(!active||std::string(e.what()).find("0x8C5FF980")==std::string::npos)throw std::runtime_error(std::string("Parent boundary guard=")+std::to_string(guard)+" mask="+std::to_string(mask)+" alias="+std::to_string(alias)+" bank="+std::to_string(bank)+" visits="+std::to_string(visits)+": "+e.what());missing=true;}
  rt.register_target(0x8c5ffa5c,real_enter);observe_enter={};need(missing==active&&visits==(active?2u:0u),"Unexpected parent boundary");if(active)m.missing();verify(c,rt);need(!rt.code_module_dispatch_depth,"Dispatch depth leaked");need(m.cycles==(active?33u:15u),"Parent cycles differ");
  if(capture){save(2);save_ram(rt.main_ram,out+"/critical-observed-ram.bin");save_state(c,out+"/critical-observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/critical-observed-timers.txt");save_sq(rt.store_queues,sqfields(rt),out+"/critical-observed");}else ++parent_controls;
 };
 for(unsigned guard:{0xffffffffu,0u,1u})for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(guard,mask,alias,bank,false);check(0,0,0,0,true);
 for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned high:{0u,0x5a5a0000u}){auto owner=make();auto&rt=*owner;auto c=initial;variant(c,mask,alias,bank);c.r[4]=alias?0xacedf000:0x8cedf000;const auto old_sr=c.sr;auto expected=rt.main_ram;CriticalModel m{{c,expected}};const auto timers0=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;
  auto verify=[&]{same(m.c,c);need(expected==rt.main_ram,"Lock pair RAM differs");auto timers=timers0;tick(timers,start,m.cycles);for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Lock timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Lock SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty(),"Unexpected lock device effects");};
  m.enter();call_recompiled(c,rt,alias?0xac5ffa5c:0x8c5ffa5c);verify();need(c.sr==(old_sr|0xf0)&&word(rt.main_ram,c.r[4])==mask,"Enter mask/save differs");put(expected,c.r[4],mask|high);put(rt.main_ram,c.r[4],mask|high);m.leave();call_recompiled(c,rt,alias?0xac5ffa74:0x8c5ffa74);verify();need(c.sr==old_sr&&m.cycles==34,"Leave did not restore complete status");++lock_controls;
 }
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"parent_controls\":"<<parent_controls<<",\"lock_controls\":"<<lock_controls<<",\"captured_connected_cycles\":[12,30,33],\"busy_return_cycles\":15,\"enter_cycles\":18,\"leave_cycles\":16,\"interrupt_masks\":16,\"original_bodies\":3,\"original_instructions\":62,\"original_SR_instructions_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"parent_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie critical section: "<<parent_controls<<" parent controls,"<<lock_controls<<" lock controls;all16 masks,12/30/33cycle prefix,busy15;enter18/leave16;next5FF980 explicit\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
