// Source-derived ordered copy contract; executes the emitted original body.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void word_copy_register(DCRuntime&);}

struct WordCopyModel : RequestModel {
 void copy(){
  auto&r=c.r;r[6]>>=2;need(r[6]>0&&r[6]<=256,"Copy reduction requires 1..256 words");++cycles;
  do{r[1]=w(r[5]);r[5]+=4;--r[6];t(r[6]==0);put(m,r[4],r[1]);r[4]+=4;cycles+=5;}while(!(c.sr&1));
  c.pc=c.pr;cycles+=3;
 }
};

int main(int argc,char**argv){try{
 need(argc==5,"usage: word-copy RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);word_copy_register(rt);
 need(rt.targets.contains(0x8c66453c),"Missing original copy binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;WordCopyModel model{{c,predicted}};model.copy();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/copy-expected-ram.bin");save_state(model.c,stem+"/copy-expected-state.txt");save_timers(timers,start,tocr,stem+"/copy-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"words\":"<<(c.r[6]>>2)<<"}\n";}
  call_recompiled(c,rt,0x8c66453c);
  if(capture){save_state(c,stem+"/copy-actual-state.txt");save_timers(rt.tmu,start,tocr,stem+"/copy-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"Ordered copy RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Copy timer controls differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned controls=0;
 for(unsigned bytes:{4u,8u,12u,511u,512u,1024u})for(unsigned alias:{0x8c000000u,0xac000000u})for(int delta:{-8,-4,0,4,8,2048}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=0x8ce04000+delta;c.r[5]=alias+0xe04000;c.r[6]=bytes;c.r[15]=0x8cefe000;c.pr=0x8c112234;
  c.sr=(c.sr&~0x20000001u)|((controls&1)?0x20000001u:0);c.fpscr=(c.fpscr&~(1u<<21))|((controls&2)?(1u<<21):0);
  for(unsigned i=0;i<1080;++i)rt.main_ram[0xe03fe0+i]=std::uint8_t(i*37+(i>>3));check(c,false);++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true);need(cycles==644&&initial.r[6]==512,"Captured copy length differs");
 const auto dst=initial.r[4]&0xffffff,src=initial.r[5]&0xffffff;
 need(std::equal(ram.begin()+src,ram.begin()+src+512,rt.main_ram.begin()+dst),"Captured copy payload differs");
 need(std::equal(ram.begin(),ram.begin()+dst,rt.main_ram.begin())&&std::equal(ram.begin()+dst+512,ram.end(),rt.main_ram.begin()+dst+512),"Copy wrote outside destination");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"words\":128,\"full_RAM_CPU_TMU_return_verified\":true,\"device_clocks_enabled\":false,\"zero_word_path_verified\":false,\"delivery_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original ordered word copy return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" length/overlap/AC-alias/bank/FPU controls; delivery/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
