// Original return leaves and exact retained continuation entries; no prefix replay.
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





#include "movie-method-setup/movie-method-setup-manifest.hpp"
#include "movie-method-callback/movie-method-callback-manifest.hpp"
#include "movie-interface-bind/movie-interface-bind-manifest.hpp"
#include "movie-followup-return/movie-followup-return-manifest.hpp"
#include "movie-final-return/movie-final-return-manifest.hpp"
struct ReturnModel : RequestModel {
 std::function<void(unsigned,const ReturnModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void leaf(){c.pc=c.pr;cycles+=3;}
 void connected(){auto&r=c.r;leaf();phase(0);r[2]=w(0x8ced79ec);need(r[2]==0x8ced7800,"Wrong final helper literal");c.pr=0x8ced78e2;c.pc=0x8ced7800;cycles+=4;phase(1);leaf();phase(2);r[0]=0;c.pr=pop();c.pc=c.pr;cycles+=5;phase(3);
  r[4]=r[0];t(r[4]==0);cycles+=3;need(c.sr&1,"Wrong second callback branch");c.pr=pop();c.pc=c.pr;r[0]=0;cycles+=4;phase(4);
  r[6]=r[0];t(r[6]==0);cycles+=3;need(c.sr&1,"Wrong binder result branch");r[14]+=4;t(r[14]>=r[12]);r[13]+=4;cycles+=4;need(!(c.sr&1),"Wrong binder range branch");r[2]=w(r[14]);t(r[2]==0);cycles+=3;need(!(c.sr&1),"Wrong binder null branch");r[11]=w(r[13]);r[0]=r[10];r[7]=0;r[3]=w(r[11]+r[0]);r[5]=r[6]=0;c.pr=0x8ceda2e6;r[4]=0;c.pc=0x8ceda2d6;cycles+=9;phase(5);
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_final(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);movie_recomp_8CED7800(c,rt);if(boundary)boundary(2);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: returns RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned leaf_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_method_setup_entries),std::end(movie_method_setup_entries));for(auto e:movie_method_callback_entries)entries.push_back(e);for(auto e:movie_interface_bind_entries)entries.push_back(e);for(auto e:movie_followup_return_entries)entries.push_back(e);for(auto e:movie_final_return_entries){e.function=observed_final;entries.push_back(e);}need(entries.size()==18,"Return manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);for(unsigned target:{0x8ced6840u,0x8ced7800u,0x8ced256au})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced6840u,0x8ced6840u,0xaced6840u,0x8ded6840u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced6840),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced6842);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced6840);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};
 for(unsigned entry:{0x8ced6840u,0x8ced7800u})for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(entry,bank?15:0,t,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[0]=value;auto expected=rt.main_ram;ReturnModel m{{c,expected}};m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,entry|(alias?0x20000000u:0));same(m.c,c);need(expected==rt.main_ram,"Return leaf modified RAM");verify_devices(rt,vram,timers,start,tocr);++leaf_controls;}
 auto check=[&](unsigned value,unsigned index,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;
  if(!capture){c.r[0]=value;c.sr=(c.sr&~0x301u)|(bank?0x301:0);c.fpscr^=bank<<21;const unsigned base=alias?0xacef4000:0x8cef4000,table=base+256;c.r[10]=index<<2;c.r[12]=base+60;c.r[13]=c.r[14]=base+4;put(rt.main_ram,base+8,table);put(rt.main_ram,table+c.r[10],0x8ced256a);c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xaced78dc:0x8ced78dc;put(rt.main_ram,c.r[15],alias?0xaced1cb6:0x8ced1cb6);put(rt.main_ram,c.r[15]+4,alias?0xaceda2e6:0x8ceda2e6);}
  auto expected=rt.main_ram;ReturnModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const ReturnModel&model){need(i==phases.size(),"Return prediction order");phases.push_back({model.c,model.m,model.cycles});};m.connected();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Return observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Return boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i==1||i==2);++seen;};
  if(capture)boundary=verify;
  call_recompiled(c,rt,alias?0xaced6840:0x8ced6840);if(capture)verify(0);
  need((c.pc&0x1fffffff)==0x0ced78dc,"Wrong retained setup resume");call_recompiled(c,rt,c.pc);if(capture)verify(3);
  need((c.pc&0x1fffffff)==0x0ced1cb6,"Wrong retained callback resume");call_recompiled(c,rt,c.pc);if(capture)verify(4);
  need((c.pc&0x1fffffff)==0x0ceda2e6,"Wrong retained binder resume");rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");boundary={};if(capture)verify(5);
  same(m.c,c);need(expected==rt.main_ram,"Returns or binder changed RAM");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,6> cycles={3,7,10,15,22,41};need(seen==cycles.size()&&c.pr==0x8ceda2e6&&c.pc==0x8ceda2d6&&c.r[3]==0x8ced256a&&c.r[15]==0x8cfffed8,"Captured third callback differs");for(unsigned i=0;i<cycles.size();++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured return timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(c.r[11]==0x8cedc8bc&&c.r[13]==c.r[14]&&c.r[14]==0x8cedc60c,"Captured table advance differs");}
  else ++connected_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired return entry remains");
 };
 for(unsigned value:{0u,0x80000000u,0xffffffffu})for(unsigned index:{0u,3u,0x80000000u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(value,index,alias,bank,false);
 check(0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<leaf_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[3,7,10,15,22,41],\"new_original_bodies\":2,\"new_original_instructions\":4,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED256A\",\"captured_second_callback_return_verified\":true,\"full_startup_connection_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie returns: "<<leaf_controls<<" leaf,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured41cycles; explicit missing ED256A\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
