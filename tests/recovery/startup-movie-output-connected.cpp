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





#include "movie-output-setup-prefix-manifest.hpp"
#include "movie-output-mode-eight-manifest.hpp"
#include "movie-validity-return-manifest.hpp"
#include "movie-mode-slot-manifest.hpp"
#include "movie-mode-validation-manifest.hpp"
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);need(initial.pc==0x8cecf1e4,"Wrong resident return boundary");const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,activation_controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_output_mode_eight_entries),std::end(movie_output_mode_eight_entries));for(auto e:movie_validity_return_entries)entries.push_back(e);for(auto e:movie_mode_slot_entries)entries.push_back(e);for(auto e:movie_mode_validation_entries)entries.push_back(e);for(auto e:movie_output_setup_prefix_entries)entries.push_back(e);need(entries.size()==39,"Instance manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c647e7c),"Next initializer inventory differs");for(unsigned target:{0x8ced0620u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced0620u,0x8ced0620u,0xaced0620u,0x8ded0620u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced0620),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced0622);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced0620);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 for(unsigned dirty:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u})for(unsigned mode:{0u,1u,2u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;c.fpscr=0x40000|guest_zero;rt.fpu_aot_mode=mode;
  const auto instance=word(ram,c.r[4]+48);need(instance==0x8cee2210,"Real instance absent");if(dirty)for(unsigned index:{8u,1u,0u,23u,45u,44u,42u,14u})put(rt.main_ram,instance+0xe0+index*4,0xa5a5a5a5);auto expected_ram=rt.main_ram;RequestModel m{c,expected_ram};auto&er=m.c.r;auto setter=[&]{m.push(er[14]);er[14]=er[4];m.push(er[13]);m.t(er[14]==0);m.push(er[12]);er[12]=er[5];m.push(m.c.pr);er[13]=er[6];er[2]=m.w(0x8ced4444);need(er[2]==0x8ced1baa,"Validity target differs");er[4]=er[14];m.c.pr=0x8ced4390;
 er[3]=m.w(0x8ced1c0c);put(expected_ram,er[3],er[4]);er[0]=64;er[3]=m.w(er[4]+64);need(er[3]==1,"Instance invalid");er[0]=0;m.t(true);er[5]=er[12];er[6]=er[13];m.c.pr=0x8ced43ac;er[4]=er[14];
 m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=4;er[13]=er[5];er[5]=er[13];put(expected_ram,er[15],er[6]);er[14]=er[4];m.c.pr=0x8ced440c;
 m.push(m.c.pr);er[15]-=12;put(expected_ram,er[15],er[4]);put(expected_ram,er[15]+8,er[5]);put(expected_ram,er[15]+4,er[6]);er[0]=1;m.t(false);er[15]+=12;m.c.pr=m.pop();
 m.t(false);er[2]=static_cast<int16_t>(m.w(0x8ced443e));er[3]=er[13];er[1]=m.w(er[15]);er[3]<<=2;er[2]+=er[14];er[0]=er[13];er[3]+=er[2];m.t(er[0]==15);put(expected_ram,er[3],er[1]);er[15]+=4;m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();need(m.c.pr==0x8ced43ac,"Mode-slot return differs");m.c.pr=m.pop();er[0]=0;er[12]=m.pop();er[13]=m.pop();er[14]=m.pop();m.c.pc=m.c.pr;m.cycles+=85;};
 for(unsigned reg:{14u,13u,12u})m.push(er[reg]);m.push(m.c.pr);er[15]-=8;er[13]=m.w(0x8ced06f4);er[5]=8;put(expected_ram,er[15]+4,er[6]);er[6]=0;put(expected_ram,er[15],er[7]);er[14]=m.w(er[4]+48);m.c.pr=0x8ced063a;er[4]=er[14];m.cycles=14;setter();
 auto set=[&](unsigned index,unsigned value,unsigned ret){er[5]=index;er[6]=value;er[4]=er[14];m.c.pr=ret;m.cycles+=5;setter();};set(1,1,0x8ced0642);set(0,0,0x8ced064a);set(23,4,0x8ced0652);
 er[2]=m.w(er[15]+4);er[0]=0x8ced06f8;er[3]=m.w(er[15]);er[1]=static_cast<int16_t>(m.w(0x8ced06ba));need(er[2]==2&&er[3]==2&&er[1]==1000&&m.w(er[0])==0x3f000000,"Captured FPU inputs differ");m.c.macl=4000;er[3]=4000;m.c.fpul=4000;m.c.fr_bits[2]=0x3f000000;m.c.fr_bits[3]=0x457a0000;m.c.fr_bits[4]=0x457a0800;er[4]=4000;m.t(false);er[12]=4000;m.cycles+=22;
 set(45,4000,0x8ced0688);set(44,4000,0x8ced0690);set(42,4000,0x8ced0698);set(14,1,0x8ced06a0);
 er[4]=er[14];er[5]=5;er[6]=0;er[15]+=8;m.c.pr=m.pop();er[2]=m.w(0x8ced06fc);er[12]=m.pop();er[13]=m.pop();er[14]=m.pop();m.c.pc=0x8ced06a0;m.cycles+=11;need(er[2]==0x8ced2ec0&&!rt.targets.contains(er[2]),"Next output helper inventory differs");need(m.cycles==762,"Full output timing differs");const auto expected=m.c;const unsigned cycles=m.cycles;auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  rejection([&]{call_recompiled(c,rt,0x8ced0620);},"Inactive or unregistered");same(expected,c);need(rt.main_ram==expected_ram,"Initializer return RAM differs");verify_devices(rt,vram,timers,start,tocr);need(rt.relocated_code_matches==0,"Unexpected template fallback");need(std::fegetround()==host,"Host mode leaked");
  if(!dirty&&!host_zero&&guest_zero&&!mode){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired MOVIE entry remains");
 }
 need(controls==24&&activation_controls==9,"Post-resident coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":24,"activation_controls":9,"cycles":762,"next_target":"8CED2EC0","output_value":4000,"original_setters":8,"new_instructions":0,"exact_movie_activation":true,"replayed_data_words":39,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original post-wrapper handler:24controls,9activation checks,762cycles,toED2EC0\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
