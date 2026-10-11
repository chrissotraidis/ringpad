// Original64C766 through real status queries; active path may reject at a missing callee.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void record_info_register(DCRuntime&);void info_wrapper_register(DCRuntime&);void payload_validator_register(DCRuntime&);}
// Query models retained from startup-block-iteration-execution.cpp, derived from SH-4.
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

struct InfoModel : PayloadModel {
 unsigned missing=0;
 void unavailable(){auto&r=c.r;r[0]=unsigned(int(std::int16_t(w(0x8c64c7f2))));cycles+=3;r[15]+=12;c.pr=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=8;}
 void run(){
  auto&r=c.r;for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=12;
  r[13]=unsigned(int(std::int16_t(w(0x8c64c7f0))));r[12]=r[4];r[3]=w(0x8c64c7f8);c.macl=r[13]*r[12];r[13]=c.macl;put(m,r[15],r[13]);r[13]+=r[3];r[0]=sb(r[13]+2);t(r[0]==0);r[14]=r[5];range(0xc766,0xc786,1);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64c800);c.pr=0x8c64c78e;r[4]=r[12];range(0xc788,0xc78c,1);first_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){c.pr=0x8c64c796;r[4]=r[12];c.pc=0x8c64c792;missing=0x8c64c24a;cycles+=3;return;}
  r[2]=w(0x8c64c804);c.pr=0x8c64c7a0;r[4]=r[12];range(0xc79a,0xc79e,1);second_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){unavailable();return;}
  r[2]=w(r[15]);r[3]=w(0x8c64c7f8);r[2]+=r[3];r[0]=sb(r[2]+8);t(r[0]==0);range(0xc7a4,0xc7ae);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64c808);c.pr=0x8c64c7bc;r[4]=r[12];c.pc=0x8c64c7b6;range(0xc7b6,0xc7ba,1);need(r[3]==0x8c606bdc,"Unexpected wrapper target");
  r[3]=w(0x8c606c90);need(r[3]==0x8c64e1f2,"Unexpected validator target");cycles+=4;validate();
  t(int(r[0])>=0);cycles+=2;
  if(!(c.sr&1)){r[0]=unsigned(int(std::int16_t(w(0x8c64c7f4))));cycles+=3;r[15]+=12;c.pr=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=8;return;}
  r[4]=0;r[5]=32;cycles+=3;
  for(;;){t(int(r[4])>=int(r[5]));cycles+=2;if(c.sr&1)break;r[3]=r[4];r[0]=w(r[13]+32);r[3]+=16;r[2]=r[14];r[1]=sb(r[0]+r[3]);r[2]+=r[4];++r[4];wb(r[2],r[1]);cycles+=8;}
  r[11]=r[15];r[5]=w(r[13]+32);r[11]+=4;r[6]=r[11];r[4]=r[11];r[5]+=48;r[6]+=8;cycles+=8;
  for(;;){t(r[4]>=r[6]);cycles+=2;if(c.sr&1)break;r[3]=sb(r[5]);++r[5];wb(r[4],r[3]);++r[4];cycles+=3;}
  r[3]=w(0x8c64c914);r[4]=r[14];r[5]=r[11];c.pr=0x8c64c820;r[4]+=48;c.pc=0x8c64c816;cycles+=6;missing=r[3];need(missing==0x8c64f1b2,"Unexpected conversion target");
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-info RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);record_info_register(rt);info_wrapper_register(rt);payload_validator_register(rt);
 for(unsigned a:{0x8c64c766u,0x8c664aecu,0x8c664b2cu})need(rt.targets.contains(a),"Original query bindings absent");
 for(unsigned a:{0x8c64c24au,0x8c64f1b2u})need(!rt.targets.contains(a),"Missing callee unexpectedly bound");
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;std::string out=argv[4];
 unsigned returns=0,rejections=0;
 auto check=[&](SH4Context c,bool capture){auto expected=rt.main_ram;InfoModel model{{{c,expected}}};model.run();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(expected,out+"/info-expected-ram.bin");save_state(model.c,out+"/info-expected-state.txt");save_timers(timers,start,tocr,out+"/info-expected-timers.txt");std::ofstream(out+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"missing_target_decimal\":"<<model.missing<<"}\n";}
  bool rejected=false;try{call_recompiled(c,rt,0x8c64c766);}catch(const std::runtime_error&e){std::ostringstream target;target<<"No recompiled/native target registered for Dreamcast address 0x"<<std::uppercase<<std::hex<<model.missing;need(model.missing&&std::string(e.what()).find(target.str())!=std::string::npos,e.what());rejected=true;}
  need(rejected==bool(model.missing),"Unexpected provider outcome");same(model.c,c);need(expected==rt.main_ram,"Record information RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effect");
  if(capture){save_state(c,out+"/info-actual-state.txt");save_timers(rt.tmu,start,tocr,out+"/info-actual-timers.txt");}else if(rejected)++rejections;else ++returns;
  return std::pair{model.cycles,model.missing};
 };
 const auto record=word(ram,0x8c64c7f8);const auto port=word(ram,word(ram,0x8c664b7c));const auto status=word(ram,0x8c64a5f0)+24*port;const auto shared=word(ram,word(ram,0x8c663118));
 for(unsigned present:{0u,1u})for(unsigned connected:{0u,1u})for(unsigned capability:{0u,2u})for(unsigned busy:{0u,1u})for(unsigned valid:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned corrupt:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=alias?0xacefe000:0x8cefe000;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;
  rt.main_ram[(record+2)&0xffffff]=present;rt.main_ram[(record+8)&0xffffff]=valid;rt.main_ram[status&0xffffff]=connected;put(rt.main_ram,shared+0x44c+120*port,capability);rt.main_ram[(shared+40+44*port+12)&0xffffff]=busy;
  if(corrupt)rt.main_ram[word(ram,record+32)&0xffffff]=0;
  check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto [cycles,missing]=check(initial,true);need(cycles==906&&missing==0x8c64f1b2,"Captured active path differs from source prediction");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<returns+rejections<<",\"returns\":"<<returns<<",\"rejections\":"<<rejections<<",\"captured_cycles\":"<<cycles<<",\"captured_missing_target\":\"0x8C64F1B2\",\"full_RAM_CPU_TMU_boundary_verified\":true,\"provider_full_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record info copy prefix: "<<std::dec<<returns+rejections<<" controls;"<<returns<<" unavailable returns,"<<rejections<<" original missing-callee rejections;actual906cycle missing64F1B2;whole RAM CPU TMU;active return unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
