// Original MOVIE zero return and existing exact-image module activation.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void movie_resident_init_register(DCRuntime&);void movie_resident_count_register(DCRuntime&);void movie_resident_buffer_register(DCRuntime&);}

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}






struct ResidentInit : RequestModel {
 bool prefix(){auto&r=c.r;push(c.pr);t(r[4]==0);cycles=3;
  if(!(c.sr&1)){r[2]=2;t(std::int32_t(r[5])>=2);cycles+=3;}else return invalid();
  if(!(c.sr&1))return invalid();
  r[2]=w(0x8c5ffa38);r[3]=w(0x8c5ffa3c);put(m,r[2],r[4]);c.pr=0x8c5ff87c;put(m,r[3],r[5]);cycles+=6;
  r[1]=w(0x8c5ffa3c);r[0]=w(r[1]);cycles+=4;
  r[6]=r[0];r[6]<<=2;t((r[6]>>31)!=0);r[6]<<=1;push(r[6]);c.pr=0x8c5ff888;r[4]=0;cycles+=7;
  r[2]=w(0x8c5ffa38);r[0]=r[4];r[0]<<=2;r[3]=w(r[2]);t((r[0]>>31)!=0);r[0]<<=1;r[0]+=r[3];cycles+=8;
  r[2]=w(0x8c5ffa40);r[5]=0;r[4]=r[0];c.pr=0x8c5ff892;r[6]=pop();c.pc=r[2];cycles+=6;
  need(c.pc==0x8c10ef3c&&r[6]<=40,"Unexpected real clear contract");return true;
 }
 bool invalid(){c.pr=pop();c.pc=c.pr;c.r[0]=0xffffffff;cycles+=4;return false;}
 void finish(){auto&r=c.r;r[7]=0;r[3]=0;t(r[3]>=r[6]);r[0]=r[4];cycles+=5;
  while(!(c.sr&1)){++r[7];wb(r[0],r[5]);t(r[7]>=r[6]);++r[0];cycles+=5;}
  r[0]=r[4];cycles+=3;r[2]=w(0x8c5ffa44);r[3]=0xffffffff;r[0]=0;put(m,r[2],r[3]);c.pr=pop();c.pc=c.pr;cycles+=8;
 }
};
static RecompiledFunction real_clear;
static std::function<void(const SH4Context&,const DCRuntime&)> observe_clear;
static void watched_clear(SH4Context&c,DCRuntime&rt){if(observe_clear)observe_clear(c,rt);real_clear(c,rt);}
int main(int argc,char**argv){try{
 need(argc==9,"usage: resident-init RAM STATE TIMERS SQ SQSTATE FLASH AICA OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[8];unsigned controls=0;
 need(ram.size()==16777216&&sq.size()==64&&flash.size()==131072&&sound.size()==2097152,"Fixture sizes differ");
 auto check=[&](unsigned count,bool null_buffer,unsigned fill,unsigned saved_pr,unsigned alias,unsigned bank,bool capture){auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
  load_embedded_elf_image(rt);registers(rt);movie_resident_init_register(rt);movie_resident_count_register(rt);movie_resident_buffer_register(rt);
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;
  if(!capture){c.r[4]=null_buffer?0:(alias?0xacedf000:0x8cedf000);c.r[5]=count;std::fill_n(rt.main_ram.begin()+0xedf000,64,std::uint8_t(fill));c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=saved_pr;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}}
  auto expected=rt.main_ram;ResidentInit m{{c,expected}};const bool valid=m.prefix();const auto prefix_state=m.c;const auto prefix_ram=expected;const auto initial_timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;const auto vram=rt.vram;auto prefix_timers=initial_timers;tick(prefix_timers,start,m.cycles);
  if(capture){need(valid&&m.cycles==37,"Captured clear-entry timing differs");save_ram(expected,out+"/phase0-expected-ram.bin");save_state(m.c,out+"/phase0-expected-state.txt");save_timers(prefix_timers,start,tocr,out+"/phase0-expected-timers.txt");save_sq(sq,device,out+"/phase0-expected");}
  if(valid)m.finish();auto timers=initial_timers;tick(timers,start,m.cycles);
  const unsigned predicted=valid?53+40*c.r[5]:(c.r[4]?10:7);need(m.cycles==predicted,"Independent return timing differs");
  if(capture){need(m.cycles==253,"Captured complete return timing differs");save_ram(expected,out+"/phase1-expected-ram.bin");save_state(m.c,out+"/phase1-expected-state.txt");save_timers(timers,start,tocr,out+"/phase1-expected-timers.txt");save_sq(sq,device,out+"/phase1-expected");}
  unsigned visits=0;real_clear=rt.targets.at(0x8c10ef3c);observe_clear=[&](const SH4Context&actual,const DCRuntime&device_rt){++visits;need(valid&&visits==1,"Unexpected clear call");same(prefix_state,actual);need(prefix_ram==device_rt.main_ram,"Clear-entry whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(prefix_timers[i],device_rt.tmu[i]);need(std::equal(sq.begin(),sq.end(),device_rt.store_queues.begin())&&device==sqfields(device_rt),"Clear-entry SQ/device differs");};
  rt.register_target(0x8c10ef3c,watched_clear);call_recompiled(c,rt,alias?0xac5ff860:0x8c5ff860);rt.register_target(0x8c10ef3c,real_clear);observe_clear={};need(visits==unsigned(valid),"Real clear visits differ");
  same(m.c,c);need(expected==rt.main_ram,"Complete initializer RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");
  if(capture){save_ram(rt.main_ram,out+"/resident-observed-ram.bin");save_state(c,out+"/resident-observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/resident-observed-timers.txt");save_sq(rt.store_queues,sqfields(rt),out+"/resident-observed");}else ++controls;
 };
 for(unsigned count:{0u,1u,2u,5u,0x80000000u,0xffffffffu})for(bool null_buffer:{false,true})for(unsigned fill:{0x55u,0xffu})for(unsigned pr:{0x8cecfd52u,0xac112234u})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(count,null_buffer,fill,pr,alias,bank,false);
 check(0,false,0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"captured_connected_cycles\":[37,253],\"null_return_cycles\":7,\"invalid_count_return_cycles\":10,\"original_bodies\":3,\"original_instructions\":42,\"real_clear_executed\":true,\"complete_return_verified\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original resident initializer: "<<controls<<" argument/fill/PR/alias/FPU controls;37cycle real-clear entry,253cycle captured complete return;null7/invalid10\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
