// Original09A6B0 arena sentinels and connected090F0C/090EB0 boundaries.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void global_reset_register(DCRuntime&);void global_setup_register(DCRuntime&);void arena_init_register(DCRuntime&);}
struct ArenaModel : RequestModel {
 void prefix(){push(c.pr);c.pr=0x8c090f12;cycles+=4;push(c.pr);c.r[15]-=4;c.r[3]=w(0x8c090fb8);c.pr=0x8c090eba;c.pc=0x8c09a6b0;cycles+=6;}
 void arena(){auto&r=c.r;r[4]=w(0x8c09a708);r[1]=0;r[0]=w(0x8c09a710);r[5]=0xffffffff;r[7]=w(0x8c09a70c);r[5]&=255;push(r[14]);r[14]=r[4];put(m,r[7],r[4]);r[14]+=r[0];r[6]=r[1];cycles+=11;
  do{wb(r[4],r[5]);++r[4];wb(r[4],r[5]);r[6]+=3;++r[4];t(r[6]>=r[0]);wb(r[4],r[5]);++r[4];cycles+=9;}while(!(c.sr&1));
  r[2]=w(r[7]);put(m,r[2],r[14]);r[3]=w(r[7]);put(m,r[3]+4,r[1]);r[2]=w(r[7]);put(m,r[2]+8,r[14]);r[3]=w(r[7]);put(m,r[3]+12,r[1]);put(m,r[14],r[1]);r[2]=w(r[7]);put(m,r[14]+4,r[2]);r[2]=w(0x8c09a714);put(m,r[14]+8,r[1]);r[3]=w(r[7]);put(m,r[14]+12,r[3]);put(m,r[2],r[1]);c.pc=c.pr;r[14]=pop();cycles+=19;
 }
 void pool_entry(){c.r[2]=w(0x8c090fbc);need(c.r[2]==0x8c09a0ea,"Unreviewed pool target");c.pr=0x8c090ec0;c.pc=c.r[2];cycles+=4;}
};
struct ArenaPrediction {SH4Context c;std::vector<std::uint8_t> ram;std::array<SH4TMUChannel,3> timers;unsigned cycles;};
static RecompiledFunction real_arena,real_pool;
static std::function<void(unsigned,SH4Context&)> observe;
static void watched_arena(SH4Context&c,DCRuntime&rt){observe(0,c);real_arena(c,rt);observe(1,c);}
static void watched_pool(SH4Context&c,DCRuntime&rt){observe(2,c);real_pool(c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: arena-init RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);global_reset_register(rt);global_setup_register(rt);arena_init_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];unsigned controls=0;
 auto isolated=[&](SH4Context c,bool capture){auto expected=rt.main_ram;ArenaModel model{{c,expected}};model.arena();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);const auto size=word(rt.main_ram,0x8c09a710),base=word(rt.main_ram,0x8c09a708);
  need(model.cycles==30+9*std::max(1u,(size+2)/3),"Independent loop timing differs");
  if(capture){save_ram(expected,out+"/arena-return-expected-ram.bin");save_state(model.c,out+"/arena-return-expected-state.txt");save_timers(timers,start,tocr,out+"/arena-return-expected-timers.txt");}
  call_recompiled(c,rt,0x8c09a6b0);same(model.c,c);need(expected==rt.main_ram,"Arena whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  if(size>=16){const unsigned tail=base+size;need(word(rt.main_ram,base)==tail&&word(rt.main_ram,base+4)==0&&word(rt.main_ram,base+8)==tail&&word(rt.main_ram,base+12)==0&&word(rt.main_ram,tail)==0&&word(rt.main_ram,tail+4)==base&&word(rt.main_ram,tail+8)==0&&word(rt.main_ram,tail+12)==base,"Independent arena sentinel links differ");for(unsigned i=16;i<size;++i)need(rt.main_ram[(base+i)&0xffffff]==255,"Arena fill differs");}
  if(!capture)++controls;return model.cycles;
 };
 for(unsigned size:{0u,1u,2u,3u,4u,48u,0x90000u})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned fill:{0u,0x5au}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;
  const unsigned base=alias?0xacc01000:0x8cc01000;put(rt.main_ram,0x8c09a708,base);put(rt.main_ram,0x8c09a710,size);std::fill(rt.main_ram.begin()+0xc01000,rt.main_ram.begin()+0xc01000+size+32,fill);isolated(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);need(isolated(initial,true)==1769502,"Captured arena return timing differs");
 rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;auto expected=ram;ArenaModel model{{c,expected}};std::vector<ArenaPrediction> predictions;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;
 auto predict=[&]{auto timers=rt.tmu;tick(timers,start,model.cycles);predictions.push_back({model.c,expected,timers,model.cycles});};
 model.prefix();predict();model.arena();predict();model.pool_entry();predict();need(predictions[0].cycles==10&&predictions[1].cycles==1769512&&predictions[2].cycles==1769516,"Connected prospective cycles differ");
 for(unsigned i=0;i<3;++i){const auto&p=predictions[i];const auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(p.timers,start,tocr,stem+"-timers.txt");}
 real_arena=rt.targets.at(0x8c09a6b0);real_pool=rt.targets.at(0x8c09a0ea);unsigned seen=0;
 observe=[&](unsigned phase,SH4Context&actual){need(phase==seen,"Unexpected connected arena boundary");const auto&p=predictions.at(phase);same(p.c,actual);need(p.ram==rt.main_ram,"Connected arena RAM differs");for(unsigned i=0;i<3;++i)timer_same(p.timers[i],rt.tmu[i]);need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Connected timers differ");save_state(actual,out+"/phase"+std::to_string(phase)+"-actual-state.txt");++seen;};
 rt.register_target(0x8c09a6b0,watched_arena);rt.register_target(0x8c09a0ea,watched_pool);std::string terminal;
 try{call_recompiled(c,rt,0x8c090f0c);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}
 rt.register_target(0x8c09a6b0,real_arena);rt.register_target(0x8c09a0ea,real_pool);observe={};need(seen==3,"Connected boundaries not all observed");
 save_ram(rt.main_ram,out+"/setup-observed-ram.bin");save_state(c,out+"/setup-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/setup-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';
 need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"arena_controls\":"<<controls<<",\"arena_cycles\":1769502,\"connected_phases\":3,\"connected_cycles\":[10,1769512,1769516],\"whole_RAM_CPU_TMU_verified\":true,\"arena_return_verified\":true,\"later_pool_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original arena init: "<<controls<<" controls;1769502cycle return;3 prospective connected boundaries;subsequent state observed only: "<<terminal<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
