// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}





#include "movie-instance-buffer-return/movie-instance-buffer-return-manifest.hpp"
#include "movie-instance-buffer-links/movie-instance-buffer-links-manifest.hpp"
#include "movie-instance-buffer-acquire/movie-instance-buffer-acquire-manifest.hpp"
#include "movie-instance-third-handler/movie-instance-third-handler-manifest.hpp"
struct BufferReturnModel:RequestModel{
 unsigned sw(unsigned a){return static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(w(a))));}
 void leaf(){auto&r=c.r;r[2]=w(0x8ceda87c);r[5]=0;r[3]=w(0x8ceda878);r[0]=r[5];for(unsigned i=0;i<8;++i)wb(r[4]+i,0);put(m,r[4]+8,r[3]);put(m,r[4]+12,r[2]);r[3]=w(0x8ceda880);r[2]=w(0x8ceda884);put(m,r[4]+16,r[3]);put(m,r[4]+20,r[2]);put(m,r[4]+24,r[0]);put(m,r[4]+28,r[0]);r[0]=0x8ceda888;r[3]=sw(0x8ceda85a);r[2]=sw(0x8ceda85c);auto&fp=(c.fpscr&(1u<<21))?c.xf_bits:c.fr_bits;fp[3]=w(r[0]);r[0]=32;put(m,r[4]+r[0],fp[3]);put(m,r[4]+36,r[3]);for(unsigned off:{40u,44u,48u,52u})put(m,r[4]+off,r[5]);put(m,r[4]+56,r[2]);r[1]=w(0x8ceda864);r[0]=64;r[3]-=128;put(m,r[4]+60,r[3]);r[2]=w(r[1]);put(m,r[4]+r[0],r[2]);r[0]=68;r[2]=w(0x8ceda860);r[3]=w(r[2]);put(m,r[4]+r[0],r[3]);r[0]=72;put(m,r[4]+r[0],r[5]);r[0]=76;put(m,r[4]+r[0],r[5]);r[0]=80;c.pc=c.pr;put(m,r[4]+r[0],r[5]);cycles+=50;}
 void binding_return(){auto&r=c.r;r[2]=sw(0x8ced863e);r[0]=72;r[3]=sw(0x8ced8640);r[2]+=r[13];put(m,r[14]+r[0],r[2]);r[3]+=r[13];r[2]=sw(0x8ced8642);put(m,r[14]+44,r[3]);r[2]+=r[13];put(m,r[14]+48,r[2]);c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=15;}
 void handler_return(){auto&r=c.r;r[0]=r[14];c.pr=pop();r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;}
 void next(){auto&r=c.r;r[14]=r[0];t(r[14]==0);need(!(c.sr&1),"Captured acquire returned empty");r[3]=w(0x8ced303c);r[6]=r[13];r[5]=w(0x8ced3038);c.pr=0x8ced2f98;r[4]=r[14];c.pc=0x8ced2f8e;cycles+=9;}

};

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ceda7f8;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,39> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,39> values={0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_instance_buffer_return_entries),std::end(movie_instance_buffer_return_entries));for(auto e:movie_instance_buffer_links_entries)entries.push_back(e);for(auto e:movie_instance_buffer_acquire_entries)entries.push_back(e);for(auto e:movie_instance_third_handler_entries)entries.push_back(e);need(entries.size()==32,"Instance manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8ced7e54),"Next initializer inventory differs");for(unsigned target:{0x8ceda7f8u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ceda7f8u,0x8ceda7f8u,0xaceda7f8u,0x8deda7f8u}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ceda7f8),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ceda7fa);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ceda7f8);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 std::string last_ram=argv[1];
 auto check=[&](unsigned alias,unsigned bank,unsigned bits,unsigned layout,unsigned placement,bool capture){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=capture?initial:context(0x8ceda7f8,alias?15:0,placement,bank);
  if(!capture){unsigned seg=alias?0x20000000:0;c.r[15]=(0x8cf90000+placement)|seg;c.pr=alias?0xaced85b6:0x8ced85b6;c.r[4]=layout==2?c.r[15]-40:(0x8cf40000+placement)|seg;unsigned global=(0x8cf60000+placement)|seg;dc_write32(rt,0x8ceda888,bits);dc_write32(rt,0x8ceda864,layout==1?c.r[4]+8:global);dc_write32(rt,0x8ceda860,layout==1?c.r[4]+64:global+4);put(rt.main_ram,global,0x12345678);put(rt.main_ram,global+4,0x87654321);}
  auto expected=rt.main_ram;BufferReturnModel m{{c,expected}};m.leaf();std::vector<Prediction> phases{{m.c,m.m,m.cycles}};if(capture){m.binding_return();phases.push_back({m.c,m.m,m.cycles});m.handler_return();phases.push_back({m.c,m.m,m.cycles});m.next();phases.push_back({m.c,m.m,m.cycles});}
  const auto initial_timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;
  auto verify=[&](unsigned phase){const auto&p=phases[phase];same(p.c,c);need(p.ram==rt.main_ram,"Buffer return RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr);if(capture){auto stem=out+"/phase"+std::to_string(phase)+"-expected";bool shared=write_observed_capture(stem+"-ram.bin",last_ram,p.ram.data(),p.ram.size());last_ram=stem+"-ram.bin";std::ofstream(out+"/capture-storage.tsv",std::ios::app)<<std::filesystem::path(last_ram).filename().string()<<'\t'<<p.ram.size()<<'\t'<<(shared?"clone-verified":"write-verified")<<'\n';save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}};
  call_recompiled(c,rt,alias?0xaceda7f8:0x8ceda7f8);verify(0);if(capture){call_recompiled(c,rt,c.pc);verify(1);call_recompiled(c,rt,c.pc);verify(2);rejection([&]{call_recompiled(c,rt,c.pc);},"Inactive or unregistered");verify(3);need(m.cycles==84&&c.pc==0x8ced2f8e&&c.pr==0x8ced2f98&&c.r[3]==0x8ced7e54&&c.r[4]==0x8eee8000,"Captured buffer continuation differs");}else ++prefix_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired buffer remains");
 };
 for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned bits:{0x3f800000u,0x80000000u,0x7fc12345u,1u})for(unsigned layout:{0u,1u,2u})for(unsigned placement:{0u,1u})check(alias,bank,bits,layout,placement,false);
 check(0,0,0,0,0,true);need(prefix_controls==96,"Buffer return coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"leaf_controls\":"<<prefix_controls<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[50,65,75,84],\"new_original_bodies\":1,\"new_original_instructions\":49,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"whole_links_return_verified\":true,\"whole_acquire_return_verified\":true,\"missing_target\":\"8CED7E54\",\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original buffer return: "<<prefix_controls<<" leaf,"<<activation_controls<<" activation controls;captured50/65/75/84cycles;whole links/acquire return,nextmissingED7E54\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
