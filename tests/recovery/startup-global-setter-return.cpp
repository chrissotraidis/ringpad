// Original09D0CA scalar store, followed by real090EB0 continuation.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void global_setup_register(DCRuntime&);void global_setter_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: global-setter RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);global_setup_register(rt);global_setter_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];unsigned controls=0;
 // Reuse the source-derived task-pool contract from startup-check.hpp on this actual capture.
 const auto heap=word(ram,0x8c09a708),pool=heap+16;need(word(ram,0x8c7a32c4)==pool&&word(ram,0x8c7a32c8)==pool,"Pool roots differ");
 for(unsigned i=0;i<4096;++i){unsigned node=pool+i*48;need(word(ram,node)==(i==4095?0:node+48)&&word(ram,node+4)==(i?node-48:0),"Observed pool links differ");for(unsigned off=8;off<48;off+=4)need(word(ram,node+off)==0,"Observed pool fields differ");}
 for(unsigned i=0;i<8;++i)need(word(ram,0x8c7a32cc+i*4)==0,"Observed queues differ");need(word(ram,0x8c7a32bc)==heap+0x30010,"Observed allocator use differs");
 auto check=[&](SH4Context c,bool capture){auto expected=rt.main_ram;auto model=c;model.r[3]=word(expected,0x8c09d130);put(expected,model.r[3],model.r[4]);model.pc=model.pr;auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,4);
  if(capture){save_ram(expected,out+"/setter-expected-ram.bin");save_state(model,out+"/setter-expected-state.txt");save_timers(timers,start,tocr,out+"/setter-expected-timers.txt");}
  call_recompiled(c,rt,0x8c09d0ca);same(model,c);need(expected==rt.main_ram,"Setter whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(rt.tmu_tstr==start&&rt.tmu_tocr==tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Setter timer controls differ");
  if(capture){save_state(c,out+"/setter-actual-state.txt");save_timers(rt.tmu,start,tocr,out+"/setter-actual-timers.txt");}else ++controls;return c;
 };
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned previous:{0u,0x12345678u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=value;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;const auto address=alias?0xacc01000:0x8cc01000;put(rt.main_ram,0x8c09d130,address);put(rt.main_ram,address,previous);check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);auto c=check(initial,true);need(c.pc==0x8c090ec6,"Captured setter return changed");std::string terminal;
 try{call_recompiled(c,rt,c.pc);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}
 save_ram(rt.main_ram,out+"/setup-observed-ram.bin");save_state(c,out+"/setup-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/setup-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';
 need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"setter_cycles\":4,\"whole_RAM_CPU_TMU_return_verified\":true,\"observed_pool_contract\":{\"nodes\":4096,\"node_bytes\":48,\"empty_queues\":8,\"heap_bytes\":196624},\"pool_whole_RAM_CPU_TMU_return_verified\":false,\"later_setup_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original global setter: "<<controls<<" controls;4cycle captured RAM CPU TMU return;4096-node observed pool contract;later state observed only: "<<terminal<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
