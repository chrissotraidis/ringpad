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





#include "movie-instance-video-geometry/movie-instance-video-geometry-manifest.hpp"
#include "movie-instance-video-layout/movie-instance-video-layout-manifest.hpp"
struct VideoGeometryModel:RequestModel{
 unsigned sw(unsigned a){return static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(w(a))));}
 void leaf(){auto&r=c.r;r[0]=c.sr;r[3]=sw(0x8ced1bfc);r[0]=(r[0]>>4)&15;put(m,r[4],r[0]);r[0]=(c.sr&r[3])|240;c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=18;}
 void shll(unsigned n){auto v=c.r[n];t(v>>31);c.r[n]=v<<1;}
 void shar(unsigned n){auto v=c.r[n];t(v&1);c.r[n]=(v>>1)|(v&0x80000000);}
 void addc(unsigned src,unsigned dst){auto sum=std::uint64_t(c.r[dst])+c.r[src]+(c.sr&1);c.r[dst]=unsigned(sum);t(sum>>32);}
 void half(unsigned a,unsigned v){m[a&0xffffff]=v;m[(a+1)&0xffffff]=v>>8;}
 void next(){auto&r=c.r;
  r[2]=sw(0x8ced3438);r[4]=w(r[15]);r[3]=sw(0x8ced3436);r[4]+=r[2];r[4]+=r[3];r[5]=r[4]+8;r[4]=r[5]+4;
  r[13]=w(r[4]);r[1]=w(r[4]+4);put(m,r[15]+8,r[1]);r[2]=w(r[4]+12);r[12]=w(r[4]+8);r[10]=r[2];put(m,r[15],r[2]);r[14]=w(r[15]+8);r[7]=w(r[4]+20);c.macl=r[14]*r[13];r[6]=w(r[4]+24);r[14]=c.macl;c.macl=r[10]*r[12];r[10]=c.macl;c.macl=r[6]*r[7];r[9]=c.macl;r[2]=r[9];shll(9);r[9]+=r[2];r[2]=0;t(std::int32_t(r[2])>std::int32_t(r[9]));addc(2,9);shar(9);
  r[7]=r[5];r[6]=w(r[4]+16);r[2]=r[14];r[7]+=64;r[2]+=r[6];r[1]=r[14];r[3]=r[7];r[1]+=r[14];r[3]+=16;r[0]=r[13];half(r[7]+12,r[0]);half(r[3]+12,r[0]);r[0]=r[12];half(r[7]+14,r[0]);half(r[3]+14,r[0]);put(m,r[7],r[6]);put(m,r[7]+4,r[2]);put(m,r[15]+8,r[1]);r[1]+=r[6];put(m,r[7]+8,r[1]);r[2]=w(r[15]+8);r[2]+=r[10];put(m,r[15],r[2]);r[2]+=r[6];put(m,r[3]+8,r[2]);r[7]=w(r[15]);r[7]+=r[10];r[2]=r[7];r[14]+=r[7];r[2]+=r[6];r[14]+=r[6];put(m,r[3],r[2]);put(m,r[3]+4,r[14]);r[7]=0;r[12]=w(r[4]+32);r[14]=56;r[6]=r[7];cycles+=73;
  for(;;){r[2]=w(r[4]+28);t(std::int32_t(r[6])>=std::int32_t(r[2]));cycles+=3;if(c.sr&1)break;need(r[6]<16,"Unbounded synthetic descriptor count");c.macl=r[14]*r[6];r[3]=r[5]+96;r[2]=r[5]+96;r[13]=c.macl;c.macl=r[9]*r[6];++r[6];r[3]+=r[13];put(m,r[3],r[7]);r[13]+=r[2];r[3]=c.macl;r[3]+=r[12];put(m,r[13]+4,r[3]);cycles+=16;}
  r[2]=1;r[3]=w(0x8ced3570);r[4]=r[15];put(m,r[5]+56,r[7]);put(m,r[5]+60,r[2]);put(m,r[5]+48,r[7]);c.pr=0x8ced3482;r[4]+=4;c.pc=0x8ced3472;cycles+=9;
 }

};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced1bcc;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_video_geometry_entries),std::end(movie_instance_video_geometry_entries));for(auto e:movie_instance_video_layout_entries)entries.push_back(e);need(entries.size()==7,"Instance manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced1be4),"Next initializer inventory differs");for(unsigned target:{0x8ced1bccu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced1bccu,0x8ced1bccu,0xaced1bccu,0x8ded1bccu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced1bcc),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced1bce);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced1bcc);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned layout_controls=0;
 auto check=[&](unsigned mask,unsigned alias,unsigned placement,bool layout,unsigned count,unsigned dimensions,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=(capture||layout)?initial:context(0x8ced1bcc,alias?15:0,placement,alias);
  if(!capture){c.sr=(c.sr&~0xf0)|(mask<<4);if(!layout){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=0x8ced33a8;c.r[4]=(0x8cf40000+placement)|seg;}else{unsigned p=0x8cee457c;const unsigned first[]={0,160,0x80000001,0xffffffff},second[]={0,320,0x40000001,0xffffffff},height[]={0,240,1,0xffffffff};put(rt.main_ram,p,first[dimensions]);put(rt.main_ram,p+4,height[dimensions]);put(rt.main_ram,p+8,second[dimensions]);put(rt.main_ram,p+12,height[dimensions]);put(rt.main_ram,p+20,second[dimensions]);put(rt.main_ram,p+24,height[dimensions]);put(rt.main_ram,p+28,count);}}
  auto expected=rt.main_ram;VideoGeometryModel m{{c,expected}};m.leaf();const Prediction leaf{m.c,m.m,m.cycles};const auto initial_timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;
  auto verify=[&](const Prediction&p,unsigned phase){same(p.c,c);need(p.ram==rt.main_ram,"Video geometry full RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr);if(capture){auto stem=out+"/phase"+std::to_string(phase)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  call_recompiled(c,rt,alias?0xaced1bcc:0x8ced1bcc);verify(leaf,0);
  if(layout||capture){m.next();const Prediction next{m.c,m.m,m.cycles};rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");verify(next,1);need(c.pc==0x8ced3472&&c.pr==0x8ced3482&&c.r[3]==0x8ced1be4,"Layout exit helper differs");if(capture)need(m.cycles==217&&c.r[6]==6,"Captured layout cycles/count differ");else ++layout_controls;}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired video geometry remains");
 };
 for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(mask,alias,placement,false,0,0,false);
 for(unsigned count:{0u,1u,6u,0xffffffffu})for(unsigned dimensions:{0u,1u,2u,3u})check(5,0,0,true,count,dimensions,false);
 check(0,0,0,true,6,1,true);need(prefix_controls==64&&layout_controls==16,"Video geometry coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<prefix_controls<<",\"layout_controls\":"<<layout_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[18,217],\"new_original_bodies\":1,\"new_original_instructions\":12,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"six_buffer_records_verified\":true,\"missing_target\":\"8CED1BE4\",\"whole_layout_return_verified\":false,\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original video geometry: "<<prefix_controls<<" mask,"<<layout_controls<<" layout,"<<activation_controls<<" activation controls;captured18/217cycles to missingED1BE4;six records and overflow/signed arithmetic verified\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
