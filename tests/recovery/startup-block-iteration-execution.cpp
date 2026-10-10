#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void block_iteration_register(DCRuntime&);void sevenbyte_register(DCRuntime&);void word_copy_register(DCRuntime&);void payload_validator_register(DCRuntime&);}
struct PayloadModel : RequestModel {
 void status_pointer(){ //64A596
  auto&r=c.r;r[0]=r[4];t(r[0]>>31);r[0]<<=1;r[3]=r[4];r[0]+=r[3];r[2]=w(0x8c64a5f0);r[0]<<=2;t(r[0]>>31);r[0]<<=1;c.pc=c.pr;r[0]+=r[2];cycles+=10;
 }
 void device(){ //64A5A8 ->6630E0
  auto&r=c.r;push(c.pr);r[3]=w(0x8c64a5f8);c.pr=0x8c64a5b0;cycles+=5;
  r[2]=120;r[3]=w(0x8c663118);c.macl=r[2]*r[4];r[1]=0x44c;r[0]=w(r[3]);r[4]=c.macl;r[0]+=r[1];c.pc=c.pr;r[0]+=r[4];cycles+=11;
  c.pr=pop();c.pc=c.pr;cycles+=4;
 }
 void busy(){ //64A876 ->66314E ->6630D0
  auto&r=c.r;r[3]=w(0x8c64a974);cycles+=4;push(c.pr);c.pr=0x8c663154;cycles+=4;
  r[2]=44;r[3]=w(0x8c663118);c.macl=r[2]*r[4];r[0]=w(r[3]);r[0]+=40;r[4]=c.macl;c.pc=c.pr;r[0]+=r[4];cycles+=10;
  c.pr=pop();r[4]=r[0];r[0]=sb(r[4]+12);c.pc=c.pr;r[0]&=255;cycles+=6;
 }
 void first_query(){ //664AEC
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=w(0x8c664b7c);r[14]=r[4];r[14]<<=2;r[3]=w(0x8c664b80);r[14]=w(r[0]+r[14]);c.pr=0x8c664b00;r[4]=r[14];range(0xaec,0xafe,1);status_pointer();
  r[3]=w(0x8c664b84);put(m,r[15],r[0]);c.pr=0x8c664b08;r[4]=r[14];range(0xb00,0xb06,1);device();
  r[2]=w(r[15]);r[4]=r[0];r[3]=sb(r[2]);t(r[3]==0);range(0xb08,0xb10);bool ready=false;
  if(!(c.sr&1)){r[0]=w(r[4]);t((r[0]&2)==0);range(0xb12,0xb16);ready=!(c.sr&1);}
  r[0]=ready?1:0;r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void second_query(){ //664B2C
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[0]=w(0x8c664b7c);r[14]=r[4];r[14]<<=2;r[3]=w(0x8c664b80);r[14]=w(r[0]+r[14]);c.pr=0x8c664b40;r[4]=r[14];range(0xb2c,0xb3e,1);status_pointer();
  r[3]=w(0x8c664b84);put(m,r[15]+4,r[0]);c.pr=0x8c664b48;r[4]=r[14];range(0xb40,0xb46,1);device();
  put(m,r[15],r[0]);r[3]=w(r[15]+4);r[2]=sb(r[3]);t(r[2]==0);range(0xb48,0xb50);bool ready=false;
  if(!(c.sr&1)){r[3]=w(0x8c664b88);c.pr=0x8c664b58;r[4]=r[14];range(0xb52,0xb56,1);busy();t(r[0]==0);range(0xb58,0xb5a);
   if(c.sr&1){r[0]=w(r[15]);r[0]=w(r[0]);t((r[0]&2)==0);range(0xb5c,0xb62);ready=!(c.sr&1);}
  }
  r[0]=ready?1:0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void finish(){auto&r=c.r;r[15]+=4;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();range(0x270,0x278,1);}
 void unavailable(){c.r[0]=0xffffff01;range(0x238,0x23c);finish();}
 void validate(){ //64E1F2, actual first-query success and all downstream branches
  auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=4;r[14]=0x148;r[13]=r[4];r[3]=w(0x8c64e288);c.macl=r[14]*r[13];r[14]=c.macl;put(m,r[15],r[14]);r[14]+=r[3];r[0]=sb(r[14]+2);t(r[0]==0);range(0x1f2,0x20c,1);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64e28c);c.pr=0x8c64e214;r[4]=r[13];range(0x20e,0x212,1);first_query();t(r[0]==0);range(0x214,0x216);
  need(!(c.sr&1),"Missing alternate64C24A requires its own source reduction; never assume return");
  r[2]=w(0x8c64e294);c.pr=0x8c64e228;r[4]=r[13];range(0x222,0x226,1);second_query();t(r[0]==0);range(0x228,0x22a);
  if(c.sr&1){unavailable();return;}
  r[2]=w(r[15]);r[3]=w(0x8c64e288);r[2]+=r[3];r[0]=sb(r[2]+8);t(r[0]==0);range(0x22c,0x236);
  if(c.sr&1){unavailable();return;}
  r[5]=w(r[14]+32);r[4]=0;r[7]=16;r[6]=r[4];range(0x23e,0x244);
  do{r[0]=sb(r[5]);r[0]&=255;t(r[0]==85);range(0x246,0x24c);
   if(!(c.sr&1)){r[0]=r[4];wb(r[14]+11,r[0]);wb(r[14]+12,r[0]);range(0x24e,0x254);r[0]=0xffffff03;range(0x268,0x26c);finish();return;}
   ++r[6];t(int(r[6])>=int(r[7]));++r[5];range(0x256,0x25c);
  }while(!(c.sr&1));
  r[0]=w(0x8c64e298);r[13]<<=2;r[3]=w(r[0]+r[13]);t(r[3]==0);range(0x25e,0x266);
  if(c.sr&1){r[0]=0;++cycles;}else{r[0]=0xffffff03;range(0x268,0x26c);}finish();
 }
};

struct BlockModel : PayloadModel {
 bool parent_prefix(){auto&r=c.r;push(r[14]);push(r[13]);push(r[12]);r[13]=0x148;push(r[11]);r[11]=r[4];c.macl=r[13]*r[11];r[3]=w(0x8c64ec00);push(r[10]);push(c.pr);r[13]=c.macl;r[10]=w(0x8c64ec04);r[13]+=r[3];r[12]=w(r[13]+28);r[12]=w(r[12]+16);t(int(r[12])>0);r[14]=0;range(0xba2,0xbc4,1);
  if(!(c.sr&1)){r[0]=0;c.pr=pop();r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();range(0xbea,0xbf8,1);return false;}
  r[5]=w(r[13]+28);r[6]=r[14];r[3]=w(r[13]+36);r[6]<<=8;r[5]=w(r[5]+12);t(r[6]>>31);r[6]<<=1;r[6]+=r[3];r[5]+=r[14];c.pr=0x8c64ebda;r[4]=r[11];c.pc=0x8c64b350;range(0xbc6,0xbd8,1);return true;
 }
 void read_prefix(){auto&r=c.r;for(unsigned i=14;i>=8;--i)push(r[i]);push(c.pr);r[15]-=12;r[12]=0x148;r[13]=r[4];r[3]=w(0x8c64b40c);c.macl=r[12]*r[13];r[2]=w(0x8c64b410);r[8]=w(0x8c64b414);r[11]=0xffffff30;r[12]=c.macl;r[10]=w(0x8c64b418);put(m,r[15],r[5]);put(m,r[15]+4,r[6]);r[9]=w(r[3]);put(m,r[15]+8,r[12]);r[12]+=r[2];range(0x350,0x37c,1);
  r[5]=w(r[15]);r[6]=w(r[15]+4);c.pr=0x8c64b386;r[4]=r[13];c.pc=0x8c6647be;range(0x37e,0x384,1);
 }
 void worker_prefix(){auto&r=c.r;for(unsigned i=14;i>=8;--i)push(r[i]);push(c.pr);r[15]-=12;r[11]=0x148;r[14]=r[4];r[3]=w(0x8c664824);c.macl=r[11]*r[14];r[13]=w(0x8c664828);put(m,r[15]+8,r[5]);put(m,r[15],r[6]);r[11]=c.macl;r[11]+=r[3];c.pr=0x8c6647e6;r[4]=r[14];c.pc=0x8c664b2c;range(0x7be,0x7e4,2);}
};
struct Prediction {SH4Context c;std::vector<std::uint8_t> ram;std::array<SH4TMUChannel,3> timers;unsigned cycles;};
static std::vector<Prediction> predictions;static unsigned seen=0,tstart,tocr;static RecompiledFunction real_read,real_worker,real_query;
static void compare_phase(unsigned phase,SH4Context&c,DCRuntime&rt){need(seen==phase,"Unexpected ordered block boundary");const auto&p=predictions.at(phase);same(p.c,c);need(p.ram==rt.main_ram,"Block prefix whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(p.timers[i],rt.tmu[i]);need(rt.tmu_tstr==tstart&&rt.tmu_tocr==tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Block prefix timer controls differ");++seen;}
static void observe_read(SH4Context&c,DCRuntime&rt){if(seen==0&&c.pr==0x8c64ebda)compare_phase(0,c,rt);real_read(c,rt);}
static void observe_worker(SH4Context&c,DCRuntime&rt){if(seen==1&&c.pr==0x8c64b386)compare_phase(1,c,rt);real_worker(c,rt);}
static void observe_query(SH4Context&c,DCRuntime&rt){const bool selected=seen==2&&c.pr==0x8c6647e6;if(selected)compare_phase(2,c,rt);real_query(c,rt);if(selected)compare_phase(3,c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: block-iteration RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);sevenbyte_register(rt);word_copy_register(rt);payload_validator_register(rt);block_iteration_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];const auto record=word(ram,0x8c64ec00),descriptor=word(ram,record+28);
 unsigned controls=0;for(unsigned count:{0u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned ft:{0u,1u}){
  rt.main_ram=ram;put(rt.main_ram,descriptor+16,count);load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000000u:0)|ft;c.fpscr=(c.fpscr&~(1u<<21))|(ft<<21);auto expected=rt.main_ram;BlockModel model{{{c,expected}}};need(!model.parent_prefix()&&model.cycles==28,"Nonpositive source model differs");auto timers=rt.tmu;tick(timers,rt.tmu_tstr,28);call_recompiled(c,rt,0x8c64eba2);same(model.c,c);need(expected==rt.main_ram&&c.r[0]==0,"Nonpositive loop contract differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);tstart=rt.tmu_tstr;tocr=rt.tmu_tocr;auto c=initial;auto expected=ram;BlockModel model{{{c,expected}}};const auto timers=rt.tmu;
 auto save=[&](const char*label){auto ticks=timers;tick(ticks,tstart,model.cycles);predictions.push_back({model.c,expected,ticks,model.cycles});save_ram(expected,stem+"/"+label+"-expected-ram.bin");save_state(model.c,stem+"/"+label+"-expected-state.txt");save_timers(ticks,tstart,tocr,stem+"/"+label+"-expected-timers.txt");};
 need(model.parent_prefix()&&model.cycles==30,"Captured parent prefix differs");save("read-entry");model.read_prefix();need(model.cycles==59,"Read prefix differs");save("worker-entry");model.worker_prefix();need(model.cycles==81,"Worker prefix differs");save("query-entry");model.second_query();need(model.cycles==172&&model.c.r[0]==1,"Ready query differs");save("query-exit");
 real_read=rt.targets.at(0x8c64b350);real_worker=rt.targets.at(0x8c6647be);real_query=rt.targets.at(0x8c664b2c);rt.register_target(0x8c64b350,observe_read);rt.register_target(0x8c6647be,observe_worker);rt.register_target(0x8c664b2c,observe_query);
 std::string error;try{call_recompiled(c,rt,0x8c64eba2);}catch(const std::exception&e){error=e.what();}
 save_ram(rt.main_ram,stem+"/observed-stop-ram.bin");save_state(c,stem+"/observed-stop-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/observed-stop-timers.txt");std::ofstream(stem+"/observed-error.txt")<<error<<'\n';
 need(seen==4,"Not all real block/query boundaries executed");need(rt.system_flash==flash&&!rt.device_clock_enabled,"Isolated flash or clock mode changed");
 const bool returned=error.empty()&&!rt.sh4_async_redirect&&c.pc==initial.pr&&c.pr==initial.pr&&c.r[15]==initial.r[15];
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"verified_prefix_phases\":4,\"prefix_cycles\":[30,59,81,172],\"full_return_RAM_CPU_TMU_verified\":false,\"return_observed\":"<<(returned?"true":"false")<<",\"async_redirect_observed\":"<<(rt.sh4_async_redirect?"true":"false")<<",\"device_clocks_enabled\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original block iteration boundaries: 30/59/81/172 cycles whole16MiB RAM69CPU25TMU; "<<controls<<" nonpositive-count controls; real helpers delegated, return_observed="<<returned<<" async_redirect="<<rt.sh4_async_redirect<<" error="<<error<<"; subsequent state/delivery/full return/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
