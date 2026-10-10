// Independent source model; native execution uses the original registered bodies.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void sevenbyte_register(DCRuntime&);}

struct SevenByteModel : RequestModel {
 void error(){
  auto&r=c.r;r[3]=w(0x8c65e9f8);r[2]=w(0x8c65e9fc);put(m,r[3],r[4]);c.pc=c.pr;put(m,r[2],r[5]);cycles+=6;
 }
 void copy20(){ //10E964, ordered descending word reads/stores
  auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);r[0]-=4;
  need(r[3]==0x8c10e9a2&&r[0]==16,"Unexpected twenty-byte copy target");
  r[3]=w(r[2]+r[0]);range(0x964,0x96e,1);
  r[0]=w(r[2]+12);put(m,r[1]+16,r[3]);r[3]=w(r[2]+8);put(m,r[1]+12,r[0]);
  r[0]=w(r[2]+4);put(m,r[1]+8,r[3]);r[3]=w(r[2]);put(m,r[1]+4,r[0]);put(m,r[1],r[3]);
  c.pc=c.pr;r[3]=pop();range(0x9a2,0x9b6,1);
 }
 void queue(){ //65E1A4, real critical section and full/absent errors
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=c.sr;r[3]=0xffffff0f;r[14]=0;
  r[0]=(r[0]>>4)&15;put(m,r[15],r[0]);r[0]=(c.sr&r[3])|240;c.sr=r[0];
  r[2]=w(0x8c65e240);r[5]=w(r[2]);t(r[5]==0);range(0x1a4,0x1c6,5);
  if(c.sr&1){r[14]=w(0x8c65e244);range(0x1c8,0x1cc);}else{
   r[2]=0x158;r[3]=0x100;r[5]+=r[2];r[6]=w(r[5]+20);t(r[6]==r[3]);r[7]=w(r[5]+16);range(0x1ce,0x1da);
   if(c.sr&1){r[14]=w(0x8c65e248);range(0x1dc,0x1e0);}else{
    r[6]=w(r[5]+24);r[3]=28;c.macl=r[3]*r[6];++r[6];put(m,r[5]+24,r[6]);r[1]=w(r[5]+24);
    r[6]=c.macl;r[2]=w(r[5]+12);t(r[1]==r[7]);r[6]+=r[2];r[3]=1;range(0x1e2,0x1f8,1);
    if(c.sr&1){r[0]=0;put(m,r[5]+24,r[0]);range(0x1fa,0x1fc);}
    put(m,r[6]+20,r[3]);r[2]=r[4];r[3]=w(0x8c65e24c);r[1]=r[6];c.pr=0x8c65e20a;r[0]=20;range(0x1fe,0x208,1);copy20();
    r[2]=w(r[5]+20);++r[2];put(m,r[5]+20,r[2]);range(0x20a,0x20e);
   }
  }
  r[0]=w(r[15]);r[3]=c.sr;r[1]=0xffffff0f;r[0]&=15;r[0]<<=2;r[3]&=r[1];r[0]<<=2;r[0]|=r[3];c.sr=r[0];t(r[14]==0);range(0x210,0x224,4);
  if(!(c.sr&1)){r[3]=w(0x8c65e254);r[4]=w(0x8c65e250);c.pr=0x8c65e22e;r[5]=r[14];range(0x226,0x22c,1);error();}
  r[0]=r[14];r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();range(0x22e,0x236,1);
 }
 void sevenbyte(){ //647428 including both original error branches
  auto&r=c.r;r[2]=w(0x8c647478);push(r[14]);push(c.pr);r[3]=w(r[2]);t(r[3]==0);range(0x428,0x432);
  if(c.sr&1){r[14]=w(0x8c64747c);range(0x434,0x438);}else{
   r[3]=w(0x8c647480);r[6]=7;r[2]=0x88;put(m,r[3],r[2]);r[5]=0;range(0x43a,0x444);
   for(;;){r[3]=unsigned(int(std::int8_t(r[5])));t(int(r[3])>=int(r[6]));range(0x450,0x454);if(c.sr&1)break;
    r[2]=sb(r[4]);++r[4];r[3]=unsigned(int(std::int8_t(r[5])));r[0]=w(0x8c647484);++r[5];wb(r[0]+r[3],r[2]);range(0x446,0x44e);
   }
   r[3]=w(0x8c647488);r[4]=w(0x8c647480);c.pr=0x8c64745e;range(0x456,0x45c,1);queue();r[14]=r[0];++cycles;
  }
  t(r[14]==0);range(0x460,0x462);
  if(!(c.sr&1)){r[3]=w(0x8c647490);r[4]=w(0x8c64748c);c.pr=0x8c64746c;r[5]=r[14];range(0x464,0x46a,1);error();}
  c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();range(0x46c,0x472,1);
 }
};

int main(int argc,char**argv){try{
 need(argc==5,"usage: sevenbyte RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);sevenbyte_register(rt);
 for(unsigned a:{0x8c647428u,0x8c65e1a4u,0x8c65e9ecu,0x8c10e964u})need(rt.targets.contains(a),"Missing original command binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;SevenByteModel model{{c,predicted}};model.sevenbyte();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/sevenbyte-expected-ram.bin");save_state(model.c,stem+"/sevenbyte-expected-state.txt");save_timers(timers,start,tocr,stem+"/sevenbyte-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<"}\n";}
  call_recompiled(c,rt,0x8c647428);
  if(capture){save_state(c,stem+"/sevenbyte-actual-state.txt");save_timers(rt.tmu,start,tocr,stem+"/sevenbyte-actual-timers.txt");}
  same(model.c,c);if(predicted!=rt.main_ram){unsigned n=0;for(unsigned i=0;i<predicted.size()&&n<12;++i)if(predicted[i]!=rt.main_ram[i]){std::cerr<<"RAM @"<<std::hex<<i<<" expected="<<unsigned(predicted[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';++n;}}
  need(predicted==rt.main_ram,"Seven-byte command RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Command timer controls differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 const auto queue=word(ram,0x8c67ff10)+0x158,buffer=word(ram,queue+12),packet=0x8c894844u;
 unsigned controls=0;
 for(unsigned mode=0;mode<4;++mode)for(unsigned bank:{0u,1u})for(unsigned initialT:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.r[14]^=0x12345678;c.sr=(c.sr&~0xf1u)|(bank?0xf0:0)|initialT;c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);
  for(unsigned i=0;i<7;++i)rt.main_ram[(c.r[4]+i)&0xffffff]=std::uint8_t(0x7f+31*i+mode);
  if(mode==1)put(rt.main_ram,queue+24,255);if(mode==2)put(rt.main_ram,queue+20,256);if(mode==3)put(rt.main_ram,0x8c67ff10,0);
  check(c,false);need(word(rt.main_ram,queue+20)==(mode<2?1u:(mode==2?256u:0u)),"Independent count contract differs");
  if(mode==1)need(word(rt.main_ram,queue+24)==0,"Ring did not wrap");
  if(mode>=2)need(word(rt.main_ram,0x8c67ff08)==0x8c647428&&word(rt.main_ram,0x8c67ff0c)==(mode==2?0x0a000001u:0x0d000001u),"Original error publication differs");++controls;
 }
 // Overlap and saved-frame sources distinguish ordered reads from a snapshot copy.
 for(unsigned source:{packet,packet+1,packet+3,packet+4,packet+5,packet+8,0x8cefdff8u,0x8cefdffcu}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.r[4]=source;check(c,false);++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true),slot=buffer+28*word(ram,queue+24);
 need(word(rt.main_ram,queue+20)==word(ram,queue+20)+1&&word(rt.main_ram,queue+24)==word(ram,queue+24)+1&&word(rt.main_ram,slot)==0x88&&word(rt.main_ram,slot+20)==1,"Captured queue contract differs");
 for(unsigned i=0;i<7;++i)need(rt.main_ram[(slot+4+i)&0xffffff]==ram[(initial.r[4]+i)&0xffffff],"Captured command payload differs");
 for(unsigned i=11;i<20;++i)need(rt.main_ram[(slot+i)&0xffffff]==ram[(packet+i)&0xffffff],"Preserved packet tail differs");
 need(word(rt.main_ram,slot+24)==word(ram,slot+24),"Queue slot tail changed");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"device_clocks_enabled\":false,\"delivery_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original seven-byte command return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" ring/full/gate/IRQ/FPU/overlap controls; real queue/copy/error handlers; delivery/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
