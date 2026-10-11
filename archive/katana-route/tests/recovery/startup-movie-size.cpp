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



#include "movie-size-wrapper/movie-size-wrapper-manifest.hpp"
#include "movie-stream-size/movie-stream-size-manifest.hpp"
#include "movie-frame-size/movie-frame-size-manifest.hpp"
struct SizeModel : RequestModel {
 std::function<void(unsigned,const SizeModel&)> observe;
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

 void phase(unsigned i){if(observe)observe(i,*this);}
 void shar(unsigned n){const auto v=c.r[n];t(v&1);c.r[n]=(v>>1)|(v&0x80000000);}
 void stream(){auto&r=c.r;push(r[14]);r[14]=r[5];push(c.pr);t(!(r[14]>>31));cycles+=5;if(!(c.sr&1)){r[14]+=7;++cycles;}
  shar(14);r[0]=r[4];shar(14);shar(14);t(r[0]==2);cycles+=6;
  if(c.sr&1){r[4]=0;put(m,r[6],r[14]);put(m,r[7],r[4]);r[3]=w(r[15]+8);put(m,r[3],r[14]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=11;}
  else{r[4]=0x800;r[1]=r[14];r[3]=w(0x8ced0320);r[1]+=r[4];c.pr=0x8ced01f6;r[0]=r[4];c.pc=r[3];cycles+=7;phase(1);divide();phase(2);
   r[2]=w(0x8ced0324);r[0]<<=8;r[4]=w(0x8ced032c);r[0]<<=2;t(r[0]>>31);r[0]<<=1;put(m,r[6],r[0]);put(m,r[7],r[2]);r[1]=w(r[6]);r[2]=w(0x8ced0328);r[3]=w(r[15]+8);r[1]+=r[2];put(m,r[3],r[1]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=16;}
  r[3]=w(r[15]+20);r[2]=w(0x8ced0330);put(m,r[3],r[2]);c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
 }
 void frame_prefix(){auto&r=c.r;push(c.pr);r[15]-=24;r[3]=r[15]+12;put(m,r[15]+16,r[6]);put(m,r[15]+20,r[7]);push(r[3]);r[2]=r[15]+12;push(r[2]);r[7]=r[15]+12;r[6]=r[15]+8;c.pr=0x8ced0274;c.pc=0x8ced0256;cycles+=16;}
 void wrapper(){auto&r=c.r;push(c.pr);r[15]-=40;r[3]=r[15]+28;put(m,r[15]+32,r[6]);put(m,r[15]+36,r[7]);push(r[3]);r[2]=r[15]+28;push(r[2]);r[3]=r[15]+28;push(r[3]);r[2]=r[15]+28;push(r[2]);r[7]=r[15]+28;r[6]=r[15]+24;c.pr=0x8ced02e4;c.pc=0x8ced01bc;cycles+=22;phase(0);stream();phase(3);
  r[2]=r[15]+20;r[0]=64;push(r[2]);r[6]=w(r[0]+r[15]);r[7]=r[15]+20;r[5]=w(r[15]+56);r[4]=w(r[15]+52);c.pr=0x8ced02f8;c.pc=0x8ced0256;cycles+=11;phase(4);frame_prefix();phase(5);
 }
};
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_division=nullptr;
static void observed_division(SH4Context&c,DCRuntime&rt){if(boundary)boundary(1);real_division(c,rt);if(boundary)boundary(2);}
static void observed_stream(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);movie_recomp_8CED01BC(c,rt);if(boundary)boundary(3);}
static void observed_frame(SH4Context&c,DCRuntime&rt){if(boundary)boundary(4);movie_recomp_8CED0256(c,rt);}
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}

int main(int argc,char**argv){try{
 need(argc==10,"usage: size RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,activation_controls=0;
 const std::array<unsigned,26> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030};
 const std::array<unsigned,26> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0,0,0,0};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_size_wrapper_entries),std::end(movie_size_wrapper_entries));for(auto e:movie_stream_size_entries){e.function=observed_stream;entries.push_back(e);}for(auto e:movie_frame_size_entries){e.function=observed_frame;entries.push_back(e);}need(entries.size()==12,"Size manifest boundaries differ");
 auto restore=[&](DCRuntime&rt){for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));};
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);restore(rt);need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Original captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced02bau,0x8ced02bau,0xaced02bau,0x8ded02bau}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced02ba),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced02bc);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced02ba);},"Inactive or unregistered");++activation_controls;}
 auto check=[&](unsigned selector,unsigned count,unsigned alias,unsigned bank,bool capture){auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);need(!rt.targets.contains(0x8ced0222),"Missing helper unexpectedly registered");auto c=initial;
  if(!capture){c.r[4]=selector;c.r[5]=count;c.r[6]=bank?0xffffffff:320;c.r[7]=bank?0:480;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000301u)|(bank?0x20000301u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}put(rt.main_ram,c.r[15],bank?0x80000000:6);}
  const auto before=c;auto expected=rt.main_ram;SizeModel m{{c,expected}};std::vector<Prediction> phases;
  m.observe=[&](unsigned i,const SizeModel&model){if(i==3){const auto s=std::int64_t(std::int32_t(before.r[5]))/8;const unsigned a=before.r[4]==2?unsigned(s):unsigned((s+2048)/2048)<<11;const std::array<unsigned,6> wanted={a,before.r[4]==2?0u:0x10000u,before.r[4]==2?a:a-0x10000u,before.r[4]==2?0u:0x12000u,before.r[4]==2?0u:0x12000u,0x80c0};for(unsigned j=0;j<6;++j)need(word(model.m,before.r[15]-36+4*j)==wanted[j],"Independent six-word size calculation differs");}
   if(capture){need(i==phases.size(),"Size prediction order");phases.push_back({model.c,model.m,model.cycles});}};m.wrapper();need(m.cycles==22+(before.r[4]==2?29:121)+(before.r[5]>>31)+11+16,"Independent size timing differs");
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  unsigned seen=0;auto verify=[&](unsigned i){need(i==seen&&i<phases.size(),"Size observation order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Size boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);need(device==sqfields(rt)&&std::equal(sq.begin(),sq.end(),rt.store_queues.begin()),"Boundary SQ differs");++seen;};
  if(capture){boundary=verify;real_division=rt.targets.at(0x8c10e67c);rt.register_target(0x8c10e67c,observed_division);}
  rejection([&]{call_recompiled(c,rt,0x8ced02ba|(alias?0x20000000u:0));},"Inactive or unregistered");
  if(capture){verify(5);boundary={};const std::array<unsigned,6> cycles={22,40,120,143,154,170};need(seen==6,"Missing size observations");for(unsigned i=0;i<6;++i){const auto&p=phases[i];need(p.cycles==cycles[i],"Captured size timing differs");auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}}
  same(m.c,c);need(expected==rt.main_ram,"Complete size RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Size timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Size SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected size device effects");
  if(capture){auto stem=out+"/size-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else ++controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired size entry remains");
 };
 for(unsigned selector:{0u,1u,2u,3u,0xffffffffu})for(unsigned count:{0u,1u,7u,8u,0x384000u,0xffffffffu,0xfffffff8u,0x80000000u,0x7fffffffu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(selector,count,alias,bank,false);
 check(1,0x384000,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"wrapper_controls\":"<<controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[22,40,120,143,154,170],\"new_original_bodies\":3,\"new_original_instructions\":151,\"retained_division_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"missing_target\":\"8CED0222\",\"complete_size_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie size prefix: "<<controls<<" wrapper,"<<activation_controls<<" activation controls;22/40/120/143/154/170 boundaries;explicit missing ED0222\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
