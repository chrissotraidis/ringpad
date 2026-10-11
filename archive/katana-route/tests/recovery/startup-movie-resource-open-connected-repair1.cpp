// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"
#include <cfenv>

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}





#include "movie-resource-reset/movie-resource-reset-manifest.hpp"
#include "movie-resource-lock/movie-resource-lock-manifest.hpp"
#include "movie-resource-unlock/movie-resource-unlock-manifest.hpp"
#include "movie-resource-drain/movie-resource-drain-manifest.hpp"
#include "movie-resource-method-manifest.hpp"
#include "movie-validity-return-manifest.hpp"
#include "movie-resource-open-dispatch-manifest.hpp"
#include "movie-resource-open-connected-manifest.hpp"
namespace dcrecomp_generated {void resident_handler_init_register(DCRuntime&);}
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);need(initial.pc==0x8ceb023c,"Wrong resident return boundary");const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,activation_controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 const std::array<unsigned,40> addresses={0x8cecefe8,0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,40> values={0x8cedd114,0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries;for(auto e:movie_resource_reset_entries)entries.push_back(e);for(auto e:movie_resource_lock_entries)entries.push_back(e);for(auto e:movie_resource_unlock_entries)entries.push_back(e);for(auto e:movie_resource_drain_entries)entries.push_back(e);for(auto e:movie_resource_method_entries)entries.push_back(e);for(auto e:movie_validity_return_entries)entries.push_back(e);for(auto e:movie_resource_open_dispatch_entries)entries.push_back(e);for(auto e:movie_resource_open_connected_entries)entries.push_back(e);need(entries.size()==72,"Combined reset manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);resident_handler_init_register(rt);need(!rt.targets.contains(0x8c647e7c),"Next initializer inventory differs");for(unsigned target:{0x8cecf35eu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0cecf35eu,0x8cecf35eu,0xacecf35eu,0x8decf35eu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8cecf35e),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8cecf360);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8cecf35e);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 for(unsigned dirty:{0u,1u})for(unsigned mask:{0u,5u,15u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;c.fpscr=0x40000|guest_zero;c.sr=(c.sr&~0xf0u)|(mask<<4);if(dirty){put(rt.main_ram,0x8cedcf40,0xa5a5a5a5);put(rt.main_ram,0x8cedd114+8,0xa5a5a5a5);for(unsigned off:{12u,16u,20u,24u})put(rt.main_ram,0x8c8aa8f8+off,0xa5a5a5a5);}
  auto expected_ram=rt.main_ram;RequestModel m{c,expected_ram};auto&er=m.c.r;need(er[4]==0x8cedd114&&m.c.pr==0x8ceb0254,"Method arguments differ");
  m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=8;er[14]=er[4];put(expected_ram,er[15],er[5]);er[3]=m.w(er[14]+56);put(expected_ram,er[15]+4,er[3]);m.c.pr=0x8cecf372;er[4]=er[14];m.cycles=11;
  m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=8;er[14]=er[4];er[3]=m.w(er[14]+48);m.c.pr=0x8cecf460;put(expected_ram,er[15]+4,er[3]);m.cycles+=9;
  er[0]=m.c.sr;er[3]=0xffffff0f;er[0]>>=4;er[0]&=15;er[4]=er[0];er[0]=m.c.sr;er[0]&=er[3];er[0]|=240;m.c.sr=er[0]&0x700083f3;er[0]=er[4];m.cycles+=18;
  m.c.pr=0x8cecf464;put(expected_ram,er[15],er[0]);m.cycles+=3;
  er[3]=m.w(0x8cecfb8c);m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[0]=m.w(er[3]);m.t(er[0]==1);need(m.c.sr&1,"Captured drain flag differs");er[2]=m.w(0x8cecfb90);er[1]=1;put(expected_ram,er[2],er[1]);er[14]=m.w(0x8cecfb84);er[13]=m.w(0x8cecfb80);er[3]=m.w(0x8cecfb94);er[0]=m.w(er[3]);m.t(er[0]==1);need(!(m.c.sr&1),"Active drain loop unmodeled");er[2]=m.w(0x8cecfb90);er[1]=0;put(expected_ram,er[2],er[1]);m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();m.cycles+=26;
  er[13]=0;put(expected_ram,er[14]+8,er[13]);m.c.pr=0x8cecf46c;er[4]=m.w(er[15]);m.cycles+=5;
  er[3]=m.c.sr;er[0]=er[4];er[2]=0xffffff0f;er[0]&=15;er[0]<<=2;er[3]&=er[2];er[0]<<=2;er[0]|=er[3];m.c.sr=er[0]&0x700083f3;m.cycles+=16;
  er[3]=m.w(0x8cecf4e0);m.c.pr=0x8cecf472;er[4]=m.w(er[15]+4);m.c.pc=0x8cecf46c;m.cycles+=4;need(m.cycles==92&&er[3]==0x8ced4084,"Inner reset boundary differs");
  m.push(er[14]);m.push(m.c.pr);er[3]=m.w(0x8ced40cc);need(er[3]==0x8ced1baa,"Validity target differs");m.c.pr=0x8ced408e;er[14]=er[4];m.cycles+=6;
  er[3]=m.w(0x8ced1c0c);m.t(er[4]==0);put(expected_ram,er[3],er[4]);need(er[4]!=0,"Null instance");er[0]=64;er[3]=m.w(er[4]+64);m.t(er[3]==0);need(er[3]==1,"Unmodeled active instance");er[0]=0;m.cycles+=12;
  m.t(er[0]==0);er[0]=64;er[0]=m.w(er[14]+er[0]);m.t(er[0]==1);er[0]=0;m.c.pr=m.pop();er[14]=m.pop();m.cycles+=11;
  er[4]=er[0];m.t(er[4]==0);need(m.c.sr&1,"Inner reset failed");er[1]=m.w(er[14]+52);m.t(er[1]==0);need(m.c.sr&1,"Open stream branch unmodeled");er[0]=64;er[2]=m.w(er[14]+er[0]);m.t(er[2]==0);need(m.c.sr&1,"Active object branch unmodeled");m.cycles+=10;
  er[3]=m.w(er[14]+56);er[4]=er[3];put(expected_ram,er[15],er[3]);er[2]=m.w(er[3]);er[1]=m.w(er[2]+20);need(er[1]==0x8c662194,"Original resource initializer differs");er[15]+=8;m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();m.cycles+=11;
  er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=m.w(er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);put(expected_ram,er[4]+24,er[5]);m.cycles+=8;need(m.c.pr==0x8cecf372,"Resource reset did not return to method");
  er[3]=m.w(0x8cecf3ac);er[5]=m.w(er[15]+4);m.c.pr=0x8cecf37a;er[4]=m.w(er[15]);m.c.pc=0x8cecf372;m.cycles+=5;need(m.cycles==155&&er[3]==0x8ced094c&&rt.targets.contains(er[3]),"Next open wrapper inventory differs");need(((m.c.sr>>4)&15)==mask,"IRQ mask not restored");er[3]=m.w(0x8ced09c4);need(er[3]==0x8c65fb12&&!rt.targets.contains(er[3]),"Resident open target inventory differs");m.c.pc=0x8ced094c;m.cycles+=4;need(m.cycles==159,"Connected open cycles differ");const auto expected=m.c;const unsigned cycles=m.cycles;auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  rejection([&]{call_recompiled(c,rt,0x8cecf35e);},"No recompiled/native target registered for Dreamcast address 0x8C65FB12");same(expected,c);need(rt.main_ram==expected_ram,"Initializer return RAM differs");verify_devices(rt,vram,timers,start,tocr);need(rt.relocated_code_matches==0,"Unexpected template fallback");need(std::fegetround()==host,"Host mode leaked");
  if(!dirty&&!mask&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired MOVIE entry remains");
 }
 need(controls==24&&activation_controls==9,"Post-resident coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":24,"activation_controls":9,"cycles":159,"next_target":"8C65FB12","module_data_words_after":40,"new_instructions":3,"exact_movie_activation":true,"replayed_data_words":40,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original post-wrapper handler:24controls,9activation checks,159cycles,to65FB12\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
