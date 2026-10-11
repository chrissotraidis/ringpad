// Original create prefix builder and retained parent, stopped at the real next helper.
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





#include "movie-instance-rate/movie-instance-rate-manifest.hpp"
#include "movie-instance-rate-fields/movie-instance-rate-fields-manifest.hpp"
struct InstanceRateFieldsModel:RequestModel{
 unsigned half(unsigned a){return std::uint32_t(std::int32_t(std::int16_t(m[a&0xffffff]|(m[(a+1)&0xffffff]<<8))));}
 void leaf(){c.r[5]=0;for(unsigned i=0;i<7;++i)put(m,c.r[4]+i*4,0);c.pc=c.pr;cycles+=10;}
 void next(){auto&r=c.r;r[4]=0xfffffffb;r[5]=w(0x8ced4764);r[0]=64;r[2]=1;put(m,r[14]+56,r[5]);r[3]=r[2];put(m,r[14]+60,r[2]);put(m,r[14]+64,r[4]);put(m,r[14]+68,r[2]);put(m,r[14]+72,r[4]);put(m,r[14]+76,r[3]);put(m,r[14]+80,r[4]);put(m,r[14]+84,r[5]);r[3]=0;r[2]=r[3];put(m,r[14]+88,r[3]);put(m,r[14]+92,r[5]);put(m,r[14]+96,r[3]);put(m,r[14]+100,r[3]);r[3]=half(0x8ced4756);r[2]=0;c.fr_bits[4]=0x3f800000;put(m,r[14]+104,r[3]);put(m,r[14]+108,r[2]);put(m,r[14]+112,r[2]);r[3]=r[2];c.fr_bits[4]^=0x80000000;r[2]=1;put(m,r[14]+116,r[2]);r[3]=100;put(m,r[14]+120,r[3]);put(m,r[14]+124,c.fr_bits[4]);r[3]=0;put(m,r[14]+128,r[3]);put(m,r[14]+132,c.fr_bits[4]);r[3]=0xffffffff;r[0]=136;put(m,r[14]+r[0],r[3]);c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=60;}
};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced46de;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_rate_entries),std::end(movie_instance_rate_entries));for(auto e:movie_instance_rate_fields_entries)entries.push_back(e);need(entries.size()==8,"Create/init manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced46de),"Next initializer inventory differs");for(unsigned target:{0x8ced46deu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced46deu,0x8ced46deu,0xaced46deu,0x8ded46deu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced462e),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced46e0);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced462e);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 auto check=[&](unsigned value,unsigned overlap,unsigned alias,unsigned placement,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced46de,alias?15:0,placement,alias);
  if(!capture){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[14]=(0x8cf70000+placement)|seg;c.r[4]=overlap==0?c.r[14]+28:overlap==1?c.r[15]:c.r[14]+56;c.pr=0x8ced4668;
   for(unsigned i=0;i<160;i+=4)put(rt.main_ram,c.r[14]+i,0x12345678+i);for(unsigned i=0;i<28;i+=4)put(rt.main_ram,c.r[15]+i,0x8ced3ebe+i);dc_write16(rt,0x8ced4756,value);dc_write32(rt,0x8ced4764,value^0x7fffffff);
  }
  auto expected=rt.main_ram;InstanceRateFieldsModel m{{c,expected}};m.leaf();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,10);const auto vram=rt.vram;
  auto save=[&](unsigned phase){auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(expected,stem+"-ram.bin");save_state(m.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);};
  call_recompiled(c,rt,alias?0xaced46de:0x8ced46de);same(m.c,c);need(expected==rt.main_ram,"Initializer full RAM differs");verify_devices(rt,vram,timers,start,tocr);if(capture)save(0);
  m.next();tick(timers,start,60);call_recompiled(c,rt,c.pc);same(m.c,c);need(expected==rt.main_ram,"Initializer caller full RAM differs");verify_devices(rt,vram,timers,start,tocr);
  need(m.cycles==70,"Rate return cycles differ");if(capture)save(1);else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired create/init entry remains");
 };
 for(unsigned value:{0u,1u,0x7fffu,0x8000u,0xffffu})for(unsigned overlap:{0u,1u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(value,overlap,alias,placement,false);
 check(0,0,0,0,true);need(prefix_controls==60,"Initializer coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[10,70],\"new_original_bodies\":1,\"new_original_instructions\":9,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_initializer_return_verified\":true,\"rate_return_verified\":true,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original instance rate fields: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; captured10/70cycles to rate returnED3EBE\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
