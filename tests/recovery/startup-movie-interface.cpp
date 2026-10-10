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





#include "movie-interface-wrapper/movie-interface-wrapper-manifest.hpp"
#include "movie-interface-bind/movie-interface-bind-manifest.hpp"
struct InterfaceModel : RequestModel {
 std::function<void(unsigned,const InterfaceModel&)> observe;
 bool missing=false;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void copy60(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;need(r[3]==0x8c10e97a&&r[0]==56,"Unexpected60-byte copy dispatch");r[3]=w(r[2]+r[0]);
  for(int offset=52;offset>=0;offset-=4){const unsigned reg=(offset&4)?0:3,previous=reg==0?3:0;r[reg]=w(r[2]+unsigned(offset));put(m,r[1]+unsigned(offset)+4,r[previous]);}put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=39;
 }
 void finish(){auto&r=c.r;c.pr=pop();r[0]=r[6];r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;}
 void bind(){auto&r=c.r;push(r[14]);r[14]=r[4];push(r[13]);r[6]=0;push(r[12]);r[12]=r[4];push(r[11]);r[12]+=60;push(r[10]);t(r[14]>=r[12]);push(c.pr);r[10]=r[5];r[13]=r[4];r[10]<<=2;cycles+=15;
  if(!(c.sr&1)){r[2]=w(r[14]);t(r[2]==0);cycles+=3;if(!(c.sr&1)){r[11]=w(r[13]);r[0]=r[10];r[7]=0;r[3]=w(r[11]+r[0]);r[5]=r[6]=0;c.pr=0x8ceda2e6;r[4]=0;c.pc=0x8ceda2d6;cycles+=9;missing=true;phase(3);return;}}
  finish();
 }
 void wrapper(){auto&r=c.r;push(c.pr);r[15]-=4;r[3]=w(0x8ceda3b4);r[1]=r[4];put(m,r[15],r[5]);r[2]=r[5];c.pr=0x8ceda298;r[0]=60;c.pc=0x8c10e964;cycles+=9;phase(0);copy60();phase(1);
  r[5]=0;c.pr=0x8ceda29e;r[4]=w(r[15]);c.pc=0x8ceda2b2;cycles+=4;phase(2);bind();if(missing)return;r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=5;
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_copy=nullptr;
static void observed_copy(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);real_copy(c,rt);if(boundary)boundary(1);}
static void observed_bind(SH4Context&c,DCRuntime&rt){if(boundary)boundary(2);movie_recomp_8CEDA2B2(c,rt);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: interface RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned copy_controls=0,bind_controls=0,wrapper_controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_interface_wrapper_entries),std::end(movie_interface_wrapper_entries));for(auto e:movie_interface_bind_entries){e.function=observed_bind;entries.push_back(e);}need(entries.size()==9,"Interface manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10e964),"Original60-byte copy absent");real_copy=rt.targets.at(0x8c10e964);rt.targets.at(0x8c10e964)=observed_copy;rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ceda288u,0x8ceda288u,0xaceda288u,0x8deda288u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ceda288),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ceda28a);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ceda288);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Interface timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Interface SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected interface device effects");};
 auto check=[&](unsigned kind,unsigned variant,unsigned index,int overlap,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ced1c10),"Missing method unexpectedly registered");auto c=capture?initial:context(kind==0?0x8c10e964:kind==1?0x8ceda2b2:0x8ceda288,15,bank,bank);
  if(!capture){c.pr=alias?0xac112234:0x8c112234;c.r[15]=alias?0xacefe000:0x8cefe000;c.sr|=bank?0x300:0;const unsigned base=alias?0xacef4000:0x8cef4000;
   for(unsigned i=0;i<1024;i+=4)put(rt.main_ram,0x8cef3f00+i,0x81fe37a5u^(i*0x01020304u));
   if(kind==0){c.r[0]=60;c.r[1]=base;c.r[2]=base+overlap;if(variant)c.r[15]=base+8;}
   else{const unsigned table=alias?0xacef5000:0x8cef5000;put(rt.main_ram,base,variant==0?0:table);for(int i=-4;i<=16;i+=4)put(rt.main_ram,table+unsigned(i),0x8ced1c10);if(kind==1){c.r[4]=variant==2?0xfffffff0:base;c.r[5]=index;}else{c.r[4]=base+512;c.r[5]=base;}}
  }
  auto expected=rt.main_ram;InterfaceModel m{{c,expected}};std::vector<Prediction> phases;if(capture)m.observe=[&](unsigned i,const InterfaceModel&model){need(i==phases.size(),"Interface prediction order");phases.push_back({model.c,model.m,model.cycles});};
  if(kind==0)m.copy60();else if(kind==1)m.bind();else m.wrapper();
  auto initial_timers=rt.tmu,timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;unsigned seen=0;
  auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Interface observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Interface boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);verify_devices(rt,vram,t,start,tocr,i<3);++seen;};
  if(capture)boundary=verify;auto execute=[&]{call_recompiled(c,rt,(kind==0?0x8c10e964u:kind==1?0x8ceda2b2u:0x8ceda288u)|(alias?0x20000000u:0));};if(m.missing)rejection(execute,"Inactive or unregistered");else execute();boundary={};if(capture)verify(3);
  same(m.c,c);need(expected==rt.main_ram,"Interface complete RAM differs");verify_devices(rt,vram,timers,start,tocr);
  if(capture){const std::array<unsigned,4> cycles={9,48,52,79};need(seen==4&&m.missing&&m.c.r[3]==0x8ced1c10,"Captured callback stop differs");for(unsigned i=0;i<4;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured interface timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
   need(std::equal(ram.begin()+0xedc604,ram.begin()+0xedc640,rt.main_ram.begin()+0xee6f18),"Captured60-byte table copy differs");need(c.r[4]==0&&c.r[5]==0&&c.r[6]==0&&c.r[7]==0&&c.r[11]==0x8cedc844,"Captured method arguments differ");
  }else if(kind==0)++copy_controls;else if(kind==1)++bind_controls;else ++wrapper_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired interface entry remains");need(rt.targets.contains(0x8c10e964),"Resident copy retired with MOVIE");
 };
 for(int overlap:{-64,-32,-4,0,4,32,512})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned stack:{0u,1u})check(0,stack,0,overlap,alias,bank,false);
 for(unsigned variant:{0u,1u,2u})for(unsigned index:{0u,1u,3u,0x40000000u,0x80000000u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(1,variant,index,0,alias,bank,false);
 for(unsigned variant:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(2,variant,0,0,alias,bank,false);
 check(2,1,0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"copy_controls\":"<<copy_controls<<",\"bind_controls\":"<<bind_controls<<",\"wrapper_controls\":"<<wrapper_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[9,48,52,79],\"retained_copy_bytes\":60,\"retained_copy_cycles\":39,\"new_original_bodies\":2,\"new_original_instructions\":56,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"next_missing_target\":\"8CED1C10\",\"captured_wrapper_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie interface: "<<copy_controls<<" copy,"<<bind_controls<<" bind,"<<wrapper_controls<<" wrapper,"<<activation_controls<<" activation controls; captured79cycles; explicit missing ED1C10\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
