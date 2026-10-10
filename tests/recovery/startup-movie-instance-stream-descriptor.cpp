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





#include "movie-instance-stream-prefix/movie-instance-stream-prefix-manifest.hpp"
#include "movie-instance-stream-descriptor/movie-instance-stream-descriptor-manifest.hpp"
struct StreamDescriptorModel:RequestModel{
 std::function<void(unsigned,const StreamDescriptorModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }

 void remainder(){auto&r=c.r;const auto numerator=std::int64_t(std::int32_t(r[1])),denominator=std::int64_t(std::int32_t(r[0]));t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e898);r[2]=w(0x8c10e89c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;push(r[4]);div0s(2,1);r[4]=c.sr&1;const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}div0s(2,3);r[2]=c.sr&1;r[2]^=r[4];const unsigned bit=r[2]&1,old_t=c.sr&1;r[2]=(r[2]>>1)|(old_t<<31);t(bit);cycles+=80;
  if(c.sr&1){div0s(0,3);const unsigned old=r[3];t(old&1);r[3]=(old>>1)|(old&0x80000000);step();cycles+=3;}
  r[3]+=r[4];r[0]=r[3];need(r[0]==std::uint32_t(numerator%denominator),"Signed remainder math differs");r[4]=pop();r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=7;
 }
 void leaf(){auto&r=c.r;r[0]=0;r[7]=r[6];r[1]=32;for(unsigned i=0;i<8;++i){put(m,r[4]+r[0],r[7]);r[3]=w(r[5]+r[0]);r[0]+=4;t(r[0]>=r[1]);r[7]+=r[3];}c.pc=c.pr;cycles+=54;phase(0);}
 void next(){auto&r=c.r;r[2]=w(0x8ced9910);r[1]=w(r[14]);c.pr=0x8ced97d6;r[0]=w(r[11]+40);c.pc=r[2];cycles+=5;phase(1);remainder();phase(2);
  r[11]=0;r[7]=r[0];push(r[11]);r[5]=r[12];push(r[7]);r[6]=r[14];r[7]=r[11];c.pr=0x8ced97e8;r[4]=r[13];c.pc=0x8ced97d6;cycles+=10;phase(3);
 }
};
static thread_local RecompiledFunction real_stream_remainder=nullptr;
static thread_local std::function<void(unsigned)> stream_descriptor_boundary;
static void observed_stream_remainder(SH4Context&c,DCRuntime&rt){if(stream_descriptor_boundary)stream_descriptor_boundary(1);real_stream_remainder(c,rt);if(stream_descriptor_boundary)stream_descriptor_boundary(2);}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced984e;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_stream_prefix_entries),std::end(movie_instance_stream_prefix_entries));for(auto e:movie_instance_stream_descriptor_entries)entries.push_back(e);need(entries.size()==14,"Create/init manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10e7d8),"Original resident remainder absent");real_stream_remainder=rt.targets.at(0x8c10e7d8);rt.targets[0x8c10e7d8]=observed_stream_remainder;need(!rt.targets.contains(0x8ced9864),"Next initializer inventory differs");for(unsigned target:{0x8ced984eu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced984eu,0x8ced984eu,0xaced984eu,0x8ded984eu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced97b0),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced9850);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced97b0);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 std::array<unsigned,4> captured_cycles{};
 auto check=[&](unsigned value,unsigned overlap,unsigned alias,unsigned placement,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ced984e,alias?15:0,placement,alias);
  if(!capture){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;c.r[5]=(0x8cf70000+placement)|seg;c.r[4]=overlap==0?(0x8cf80000+placement)|seg:c.r[5]+(overlap==1?4:-4);c.r[6]=value;c.pr=0x8ced97ce;c.r[11]=(0x8cf60000+placement)|seg;c.r[12]=c.r[4];c.r[13]=(0x8cf50000+placement)|seg;c.r[14]=c.r[5];
   for(unsigned i=0;i<32;i+=4)put(rt.main_ram,c.r[5]+i,(value^0xa5a5a5a5)+i);put(rt.main_ram,c.r[11]+40,value);dc_write32(rt,0x8ced9910,0x8c10e7d8|seg);
  }
  auto expected=rt.main_ram;StreamDescriptorModel m{{c,expected}};std::vector<Prediction> phases;m.observe=[&](unsigned i,const auto&v){need(i==phases.size(),"Descriptor prediction order differs");phases.push_back({v.c,v.m,v.cycles});};m.leaf();m.next();
  auto initial_timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;unsigned seen=0;
  stream_descriptor_boundary=[&](unsigned which){need(which==seen&&which<phases.size(),"Descriptor observed order differs");const auto&p=phases[which];same(p.c,c);need(p.ram==rt.main_ram,"Descriptor boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,which==1||which==2);if(capture){captured_cycles[which]=p.cycles;auto stem=out+"/phase"+std::to_string(which)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  call_recompiled(c,rt,alias?0xaced984e:0x8ced984e);stream_descriptor_boundary(0);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");stream_descriptor_boundary(3);stream_descriptor_boundary={};need(seen==4&&c.pc==0x8ced97d6&&c.pr==0x8ced97e8&&!rt.targets.contains(0x8ced9864),"Descriptor next original target differs");if(!capture)++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired descriptor entry remains");
 };
 for(unsigned value:{0u,1u,17u,0xfffffff9u,0x80000000u})for(unsigned overlap:{0u,1u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(value,overlap,alias,placement,false);
 check(0,0,0,0,true);need(prefix_controls==60,"Descriptor coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"connected_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":["<<captured_cycles[0]<<','<<captured_cycles[1]<<','<<captured_cycles[2]<<','<<captured_cycles[3]<<"],\"new_original_bodies\":1,\"new_original_instructions\":11,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"original_descriptor_return_verified\":true,\"original_resident_remainder_executed\":true,\"missing_target\":\"8CED9864\",\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original stream descriptor: "<<prefix_controls<<" connected,"<<activation_controls<<" activation controls; captured cycles ";for(auto x:captured_cycles)std::cout<<x<<',';std::cout<<" nextmissingED9864\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
