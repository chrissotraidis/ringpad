// Original movie interface init method and retained binder continuation.
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





#include "movie-interface-bind/movie-interface-bind-manifest.hpp"
#include "movie-method-init/movie-method-init-manifest.hpp"
struct MethodModel : RequestModel {
 bool missing=false;
 unsigned methods=0;
 std::function<void(unsigned,const MethodModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void leaf(){c.r[0]=0;c.pc=c.pr;cycles+=3;}
 void finish(){auto&r=c.r;c.pr=pop();r[0]=r[6];r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;}
 void bind(){auto&r=c.r;push(r[14]);r[14]=r[4];push(r[13]);r[6]=0;push(r[12]);r[12]=r[4];push(r[11]);r[12]+=60;push(r[10]);t(r[14]>=r[12]);push(c.pr);r[10]=r[5];r[13]=r[4];r[10]<<=2;cycles+=15;
  if(!(c.sr&1))for(unsigned bounded=0;bounded<15;++bounded){
   r[2]=w(r[14]);t(r[2]==0);cycles+=3;if(c.sr&1)break;
   r[11]=w(r[13]);r[0]=r[10];r[7]=0;r[3]=w(r[11]+r[0]);r[5]=r[6]=0;c.pr=0x8ceda2e6;r[4]=0;cycles+=9;
   if((r[3]&0x1fffffff)!=0x0ced1c10){c.pc=0x8ceda2d6;missing=true;phase(2);return;}
   c.pc=0x8ced1c10;phase(0);leaf();++methods;phase(1);
   r[6]=r[0];t(r[6]==0);cycles+=3;need(c.sr&1,"Original zero leaf returned nonzero");
   r[14]+=4;t(r[14]>=r[12]);r[13]+=4;cycles+=4;if(c.sr&1)break;
  }
  finish();
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static void observed_method(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED1C10(c,rt);if(boundary)boundary(1);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: method RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned leaf_controls=0,connected_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_interface_bind_entries),std::end(movie_interface_bind_entries));for(auto e:movie_method_init_entries){e.function=observed_method;entries.push_back(e);}need(entries.size()==7,"Method manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced1cac)&&!rt.targets.contains(0x8ced1c10),"Unexpected existing method binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced1c10u,0x8ced1c10u,0xaced1c10u,0x8ded1c10u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced1c10),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced1c12);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced1c10);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Method timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Method SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected method device effects");};
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=context(0x8ced1c10,bank?15:0,t,bank);c.pr=alias?0xacedabce:0x8cedabce;c.r[0]=value;auto expected=rt.main_ram;MethodModel m{{c,expected}};m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;call_recompiled(c,rt,alias?0xaced1c10:0x8ced1c10);same(m.c,c);need(expected==rt.main_ram,"Leaf modified RAM");verify_devices(rt,vram,timers,start,tocr);++leaf_controls;}
 auto connected=[&](unsigned count,bool missing,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ceda2b2,bank?15:0,bank,bank);
  if(!capture){const unsigned base=alias?0xacef4000:0x8cef4000,table=base+256;c.pr=alias?0xacedabce:0x8cedabce;c.r[4]=base;c.r[5]=bank?0x80000000u:0;for(unsigned i=0;i<16;++i)put(rt.main_ram,base+4*i,i<count?table:missing&&i==count?table+16:0);put(rt.main_ram,table,alias?0xaced1c10:0x8ced1c10);put(rt.main_ram,table+16,0x8ced1cac);}
  auto expected=rt.main_ram;MethodModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const MethodModel&model){need(i==phases.size(),"Method prediction order");phases.push_back({model.c,model.m,model.cycles});};m.bind();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Method observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Method boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<2);++seen;};
  if(capture)boundary=verify;auto execute=[&]{call_recompiled(c,rt,alias?0xaceda2b2:0x8ceda2b2);};if(m.missing)rejection(execute,"Inactive or unregistered");else execute();boundary={};if(capture)verify(2);same(m.c,c);need(expected==rt.main_ram,"Method connected RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,3> cycles={27,30,49};need(seen==3&&m.methods==1&&m.missing&&m.c.r[3]==0x8ced1cac,"Captured second method differs");for(unsigned i=0;i<3;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured method timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}need(c.r[4]==0&&c.r[5]==0&&c.r[6]==0&&c.r[7]==0&&c.r[11]==0x8cedc878&&c.r[13]==0x8cedc608&&c.r[14]==0x8cedc608,"Captured table advance differs");}
  else{need(m.methods==count&&m.missing==(missing&&count<15),"Loop count/termination differs");++connected_controls;}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired method entry remains");
 };
 for(unsigned count:{0u,1u,2u,15u})for(bool missing:{false,true})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})connected(count,missing,alias,bank,false);
 connected(1,true,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<leaf_controls<<",\"connected_controls\":"<<connected_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[27,30,49],\"new_original_bodies\":1,\"new_original_instructions\":2,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED1CAC\",\"captured_binder_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie method: "<<leaf_controls<<" leaf,"<<connected_controls<<" connected,"<<activation_controls<<" activation controls; captured49cycles; explicit missing ED1CAC\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
