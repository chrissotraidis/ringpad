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





#include "movie-create-prefix/movie-create-prefix-manifest.hpp"
struct CreateDescriptorModel:RequestModel{
 CreateDescriptorModel(SH4Context before,std::vector<std::uint8_t>&ram):RequestModel{before,ram}{}
 std::function<void(unsigned,const CreateDescriptorModel&)> capture;
 void mark(unsigned i){if(capture)capture(i,*this);}
 void copy60(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;need(r[3]==0x8c10e97a&&r[0]==56,"Unexpected60-byte copy dispatch");r[3]=w(r[2]+r[0]);
  for(int offset=52;offset>=0;offset-=4){const unsigned reg=(offset&4)?0:3,previous=reg==0?3:0;r[reg]=w(r[2]+unsigned(offset));put(m,r[1]+unsigned(offset)+4,r[previous]);}put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=39;
 }

 void descriptor(){auto&r=c.r;r[15]+=8;r[2]=w(r[15]+8);r[4]=w(0x8ced056c);r[1]=w(0x8ced0574);put(m,r[4],r[2]);r[3]=w(r[15]+12);put(m,r[4]+4,r[3]);r[2]=w(r[15]);put(m,r[4]+8,r[2]);r[3]=w(r[15]+4);put(m,r[4]+12,r[3]);r[2]=w(r[15]+24);put(m,r[4]+16,r[2]);r[3]=w(r[15]);put(m,r[4]+20,r[3]);r[2]=w(r[15]+4);put(m,r[4]+24,r[2]);put(m,r[4]+28,r[12]);r[3]=w(r[15]+20);put(m,r[4]+32,r[3]);r[3]=w(r[15]+36);r[4]=w(0x8ced0570);put(m,r[4]+8,r[3]);r[2]=w(r[15]+32);put(m,r[4]+24,r[2]);r[3]=w(r[15]+28);put(m,r[1],r[3]);r[0]=w(r[15]+16);t(r[0]==1);cycles+=30;
 unsigned selector=r[0];if(selector!=1){t(r[0]==2);cycles+=2;}
 if(selector==1||selector==2){r[2]=w(selector==1?0x8ced0578:0x8ced06bc);r[1]=r[15];r[3]=w(selector==1?0x8ced057c:0x8ced06c0);r[1]+=40;c.pr=selector==1?0x8ced052c:0x8ced058c;r[0]=60;c.pc=0x8c10e964;cycles+=7;mark(0);copy60();mark(1);if(selector==1)cycles+=2;}else cycles+=2;
 r[2]=w(0x8ced06c4);r[0]=w(0x8ced06cc);r[4]=w(0x8ced06d0);put(m,r[15]+44,r[11]);r[3]=w(r[2]);r[2]=w(0x8ced06d4);put(m,r[15]+48,r[3]);r[3]=w(0x8ced06c8);r[1]=w(r[3]);put(m,r[15]+52,r[1]);r[1]=w(r[0]);r[0]=84;put(m,r[15]+56,r[1]);put(m,r[15]+r[0],r[12]);r[0]=88;put(m,r[15]+r[0],r[10]);r[0]=92;c.pr=0x8ced05b2;put(m,r[15]+r[0],r[13]);c.pc=0x8ced058c;cycles+=20;mark(2);
 }
};
static thread_local std::function<void(unsigned)> create_boundary;
static thread_local RecompiledFunction real_copy=nullptr;
static void observed_create_copy(SH4Context&c,DCRuntime&rt){if(create_boundary)create_boundary(0);real_copy(c,rt);if(create_boundary)create_boundary(1);}

struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
int main(int argc,char**argv){try{
 need(argc==10,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);initial.pc=0x8ced04dc;const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned prefix_controls=0,activation_controls=0;
 const std::array<unsigned,28> addresses={0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c};
 const std::array<unsigned,28> values={1,5,2,2,1,1,13,1,1,5,2,2,2,59940,29970,1,2,1,0x384000,320,480,6,0x8c900000,2949408,0,0,1,0x2d0100};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_create_prefix_entries),std::end(movie_create_prefix_entries));need(entries.size()==24,"Create manifest boundaries differ");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(rt.targets.contains(0x8c10e964)&&!rt.targets.contains(0x8ced2538),"Copy/missing initializer inventory differs");real_copy=rt.targets.at(0x8c10e964);rt.targets.at(0x8c10e964)=observed_create_copy;for(unsigned target:{0x8ced04dcu,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 {auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());
  for(unsigned address:{0x0ced04dcu,0x8ced04dcu,0xaced04dcu,0x8ded04dcu}){auto c=initial;rejection([&]{call_recompiled(c,rt,address);},"Inactive or unregistered");++activation_controls;}
  for(unsigned offset:{0u,184591u}){rt.main_ram[0xeb0000+offset]^=1;rejection([&]{activate(rt);},"image mismatch");need(!rt.targets.contains(0x8ced03f8),"Mismatch installed entry");rt.main_ram[0xeb0000+offset]^=1;++activation_controls;}
  activate(rt);rejection([&]{activate(rt);},"retired slot");++activation_controls;auto c=initial;rejection([&]{call_recompiled(c,rt,0x8ced04de);},"Inactive or unregistered");++activation_controls;
  dc_begin_code_module(rt,0x8ceb0000,movie.size());rejection([&]{call_recompiled(c,rt,0x8ced03f8);},"Inactive or unregistered");++activation_controls;}
 auto verify_devices=[&](DCRuntime&rt,const auto&vram,const auto&timers,unsigned start,unsigned tocr,bool active=false){for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setup timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Setup SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&(active||!rt.code_module_dispatch_depth),"Unexpected setup device effects");};

 unsigned copied=0,skipped=0;
 auto check=[&](unsigned selector,unsigned overlap,unsigned alias,unsigned placement,bool captured){
  auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=captured?initial:context(0x8ced04dc,alias?15:0,alias,alias);
  if(!captured){unsigned seg=alias?0x20000000u:0;c.r[15]=(0x8cf90000+placement)|seg;const unsigned sp=c.r[15]+8;c.pr=0x8ced04dc|seg;
   for(unsigned i=0;i<120;i+=4)put(rt.main_ram,sp+i,0x10203040+i);put(rt.main_ram,sp+16,selector);
   const unsigned desc=(overlap==1?sp+48:(0x8cf70000+placement)|seg),src=overlap==0?(0x8cf80000+placement)|seg:sp+40+(overlap==1?4:-4);
   dc_write32(rt,0x8ced056c,desc);dc_write32(rt,0x8ced0570,(0x8cf70100+placement)|seg);dc_write32(rt,0x8ced0574,(0x8cf70200+placement)|seg);
   dc_write32(rt,0x8ced0578,src);dc_write32(rt,0x8ced06bc,src);dc_write32(rt,0x8ced057c,0x8c10e964|seg);dc_write32(rt,0x8ced06c0,0x8c10e964|seg);
   dc_write32(rt,0x8ced06c4,sp+44);dc_write32(rt,0x8ced06c8,sp+48);dc_write32(rt,0x8ced06cc,sp+52);dc_write32(rt,0x8ced06d0,desc);dc_write32(rt,0x8ced06d4,0x8ced2538|seg);
  }
  auto expected=rt.main_ram;CreateDescriptorModel m(c,expected);std::vector<Prediction> phases;std::vector<unsigned> labels;m.capture=[&](unsigned i,const CreateDescriptorModel&v){labels.push_back(i);phases.push_back({v.c,v.m,v.cycles});};m.descriptor();
  const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;auto initial_timers=rt.tmu;const auto vram=rt.vram;unsigned seen=0;
  create_boundary=[&](unsigned which){need(seen<phases.size()&&which==labels[seen],"Observed copy order differs");const auto&p=phases[seen];same(p.c,c);need(p.ram==rt.main_ram,"Descriptor boundary RAM differs");auto timers=initial_timers;tick(timers,start,p.cycles);verify_devices(rt,vram,timers,start,tocr,which!=2);if(captured){auto stem=out+"/phase"+std::to_string(which)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}++seen;};
  rejection([&]{call_recompiled(c,rt,alias?0xaced04dc:0x8ced04dc);},"Inactive or unregistered");need(c.r[2]==(alias?0xaced2538:0x8ced2538)&&!rt.targets.contains(0x8ced2538),"Missing initializer boundary differs");create_boundary(2);create_boundary={};need(seen==phases.size(),"Copy captures incomplete");same(m.c,c);need(expected==rt.main_ram,"Descriptor full RAM differs");auto timers=initial_timers;tick(timers,start,m.cycles);verify_devices(rt,vram,timers,start,tocr);
  if(captured){need(phases.size()==3&&phases[0].cycles==37&&phases[1].cycles==76&&m.cycles==98&&c.pc==0x8ced058c&&c.pr==0x8ced05b2,"Captured descriptor boundary differs");}else{++prefix_controls;if(phases.size()==3)++copied;else ++skipped;}
  dc_begin_code_module(rt,0x8ceb0000,movie.size());for(auto e:entries)need(!rt.targets.contains(e.address),"Retired create entry remains");need(rt.targets.contains(0x8c10e964),"Resident copy retired with module");
 };
 for(unsigned selector:{0u,1u,2u,0xffffffffu})for(unsigned overlap:{0u,1u,2u})for(unsigned alias:{0u,1u})for(unsigned placement:{0u,1u})check(selector,overlap,alias,placement,false);
 check(1,0,0,0,true);need(prefix_controls==48&&copied==24&&skipped==24,"Descriptor branch coverage differs");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"descriptor_controls\":"<<prefix_controls<<",\"copied_controls\":"<<copied<<",\"skipped_controls\":"<<skipped<<",\"activation_controls\":"<<activation_controls<<",\"activation_entries\":"<<entries.size()<<",\"captured_connected_cycles\":[37,76,98],\"new_original_bodies\":0,\"new_original_instructions\":0,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"retained_copy_executed\":true,\"missing_target\":\"8CED2538\",\"full_create_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original create descriptor: "<<prefix_controls<<" descriptor("<<copied<<" copied,"<<skipped<<" skipped),"<<activation_controls<<" activation controls; captured37/76/98cycles to missingED2538\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
