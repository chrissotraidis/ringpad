// Independent source model; every executed guest callee remains original.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void payload_validator_register(DCRuntime&);}

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

int main(int argc,char**argv){try{
 need(argc==5,"usage: validator RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);payload_validator_register(rt);
 for(unsigned a:{0x8c64e1f2u,0x8c664aecu,0x8c664b2cu,0x8c64a596u,0x8c64a5a8u,0x8c64a876u,0x8c6630d0u,0x8c6630e0u,0x8c66314eu})need(rt.targets.contains(a),"Missing original validator binding");
 need(!rt.targets.contains(0x8c64c24a),"Alternate scope changed");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture,unsigned expectedReturn){auto predicted=rt.main_ram;PayloadModel model{{c,predicted}};model.validate();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/validator-expected-ram.bin");save_state(model.c,stem+"/validator-expected-state.txt");save_timers(timers,start,tocr,stem+"/validator-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<"}\n";}
  call_recompiled(c,rt,0x8c64e1f2);
  if(capture){save_state(c,stem+"/validator-actual-state.txt");save_timers(rt.tmu,start,tocr,stem+"/validator-actual-timers.txt");}
  same(model.c,c);need(predicted==rt.main_ram,"Payload validator RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Validator timer controls differ");
  need(c.r[0]==expectedReturn,"Independent validation result differs");need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 const auto record=word(ram,0x8c64e288),payload=word(ram,record+32),device=word(ram,0x8c676234),busy=word(ram,word(ram,0x8c663118))+40+44*device+12,table=word(ram,0x8c64e298);
 unsigned controls=0;
 for(unsigned mode=0;mode<21;++mode)for(unsigned bank:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.r[13]^=0x987654;c.r[14]^=0x345678;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001u:0);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);
  rt.main_ram[(record+11)&0xffffff]=0x81;rt.main_ram[(record+12)&0xffffff]=0x7f;
  if(mode==1)rt.main_ram[(record+2)&0xffffff]=0;if(mode==2)rt.main_ram[(record+8)&0xffffff]=0;if(mode==3)rt.main_ram[busy&0xffffff]=0x80;if(mode==4)put(rt.main_ram,table,1);
  if(mode>=5)rt.main_ram[(payload+mode-5)&0xffffff]=std::uint8_t(0x80+mode);
  check(c,false,mode==0?0:(mode<4?0xffffff01:0xffffff03));
  need(rt.main_ram[(record+11)&0xffffff]==(mode>=5?0:0x81)&&rt.main_ram[(record+12)&0xffffff]==(mode>=5?0:0x7f),"Independent invalid-header flag contract differs");++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true,0);
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"device_clocks_enabled\":false,\"alternate64C24A_return_verified\":false,\"delivery_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original payload validator return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" gate/busy/table/16-header-byte/frame/bank/FPU controls; real query closure; alternate64C24A/delivery/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
