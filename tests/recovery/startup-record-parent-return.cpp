// Whole original four-record iteration, derived from source and initial state.
// Keep the already verified request contract unchanged and reuse its helpers.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main

struct ParentModel : RequestModel {
 unsigned records=0,queries=0,requests=0;
 unsigned half(unsigned a){return unsigned(int(std::int16_t(m[a&0xffffff]|(unsigned(m[(a+1)&0xffffff])<<8))));}
 void wh(unsigned a,unsigned v){wb(a,v);wb(a+1,v>>8);}
 void device(){ //64A5A8 ->6630E0
  auto&r=c.r;push(c.pr);r[3]=w(0x8c64a5f8);need(r[3]==0x8c6630e0,"Device source differs");c.pr=0x8c64a5b0;cycles+=5;
  r[2]=120;r[3]=w(0x8c663118);c.macl=r[2]*r[4];r[1]=0x44c;r[0]=w(r[3]);r[4]=c.macl;r[0]+=r[1];c.pc=c.pr;r[0]+=r[4];cycles+=11;
  c.pr=pop();c.pc=c.pr;cycles+=4;
 }
 void definition(){ //64A882 packed function definitions in descending bit order
  auto&r=c.r;r[7]=r[4];push(r[12]);r[6]=w(0x8c64a97c);r[0]=32;r[12]=w(r[4]);r[4]+=4;r[7]+=4;r[1]=0;cycles+=7;
  do{r[3]=r[6];t((r[12]&r[3])==0);cycles+=3;
   if(!(c.sr&1)){t(r[6]==r[5]);cycles+=2;if(c.sr&1){r[0]=r[7];c.pc=c.pr;r[12]=pop();cycles+=4;return;}r[4]+=4;r[7]=r[4];cycles+=2;}
   ++r[1];t(r[6]&1);r[6]>>=1;t(int(r[1])>=int(r[0]));cycles+=4;
  }while(!(c.sr&1));r[0]=0;c.pc=c.pr;r[12]=pop();cycles+=4;
 }
 void geometry(){ //64A9CA
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;put(m,r[15],r[5]);r[5]=4;r[14]=r[4];c.pr=0x8c64a9da;r[4]=w(r[15]);cycles+=9;definition();
  r[4]=r[0];t(r[4]==0);cycles+=3;if(c.sr&1){r[0]=unsigned(-1);r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;return;}
  r[5]=r[4]+3;r[3]=(sb(r[4])&255)+1;wh(r[14],r[3]);r[0]=(sb(r[4]+1)&255)+1;r[0]<<=4;t(r[0]>>31);r[0]<<=1;wh(r[14]+2,r[0]);
  r[0]=((sb(r[4]+2)&255)>>4)&15;wh(r[14]+4,r[0]);r[0]=((sb(r[5])&255)>>6)&3;wb(r[14]+6,r[0]);
  r[0]=(sb(r[5])&255)>>4;t(r[0]&1);r[0]=(r[0]>>1)&1;wb(r[14]+7,r[0]);r[0]=0;r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();range(0xa9ea,0xaa34,1);
 }
 void compatible(){ //6067F4
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=12;r[3]=w(0x8c606860);need(r[3]==0x8c64a5a8,"Device wrapper differs");c.pr=0x8c606800;put(m,r[15],r[4]);cycles+=7;device();
  r[2]=w(0x8c606864);need(r[2]==0x8c64a9ca,"Geometry provider differs");r[4]=r[15];r[14]=r[0];r[5]=r[0];c.pr=0x8c60680c;r[4]+=4;cycles+=7;geometry();
  r[4]=r[0];t(int(r[4])>=0);cycles+=3;bool valid=c.sr&1;
  if(valid){r[0]=w(r[14]);t(r[0]==14);cycles+=3;valid=c.sr&1;}
  if(valid){r[0]=half(r[15]+4)&65535;t(r[0]==1);cycles+=4;valid=c.sr&1;}
  if(valid){r[0]=half(r[15]+6);r[3]=0xc0;r[0]&=65535;t(r[0]==r[3]);cycles+=5;valid=c.sr&1;}
  if(valid){r[0]=half(r[15]+8)&65535;t(r[0]==1);cycles+=4;valid=c.sr&1;}
  if(valid){r[0]=sb(r[15]+10);t(r[0]==0);cycles+=3;valid=c.sr&1;}
  if(valid){r[0]=sb(r[15]+11);t(r[0]==0);cycles+=3;valid=c.sr&1;}
  r[0]=valid?1:0;r[15]+=12;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void route(){ //6066D8
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=w(0x8c606738);r[14]=r[4];r[3]=w(0x8c60673c);r[4]=sb(r[0]+r[14]);c.pr=0x8c6066ea;r[4]&=255;cycles+=10;device();
  r[3]=w(0x8c60673c);put(m,r[15],r[0]);c.pr=0x8c6066f2;r[4]=r[14];cycles+=5;device();
  r[5]=r[0];r[0]=r[14];t(r[0]==2);cycles+=4;
  for(unsigned n:{8u,14u,20u}){if(c.sr&1)break;t(r[0]==n);cycles+=2;}
  if(c.sr&1){r[4]=1;cycles+=2;}else{r[4]=0;cycles+=3;}
  r[0]=w(r[15]);r[3]=r[5]+18;r[1]=w(0x8c606740);r[0]+=18;r[2]=sb(r[3]);r[0]=sb(r[0]+r[4]);r[2]&=255;r[0]&=255;r[0]<<=2;r[0]+=r[2];r[0]=sb(r[0]+r[1])&255;r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=18;
 }
 void refresh(){ //10A3F2
  auto&r=c.r;++queries;push(r[14]);r[0]=6;push(c.pr);r[3]=w(0x8c10a50c);r[0]<<=8;r[14]=r[4];r[0]+=48;c.pr=0x8c10a404;r[4]=w(r[14]+r[0]);cycles+=10;compatible();
  r[1]=6;r[3]=w(0x8c10a510);r[1]<<=8;r[1]+=r[14];wb(r[1],r[0]);r[0]=1584;c.pr=0x8c10a418;r[4]=w(r[14]+r[0]);cycles+=11;route();
  r[1]=1537+r[14];wb(r[1],r[0]);c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }
 void predicate(){auto&r=c.r;r[3]=1538;r[2]=w(0x8c10ba04);r[0]=w(r[2]);r[3]+=r[4];r[3]=sb(r[3]);r[0]&=7;t(r[3]==r[0]);r[0]=c.sr&1;c.pc=c.pr;cycles+=13;}
 void lookup(){ //10B1EC
  auto&r=c.r;r[0]=1538;r[3]=w(0x8c10b270);r[1]=w(r[3]);r[2]=sb(r[4]+r[0]);r[5]=0;t(r[2]==r[1]);r[6]=4;cycles+=10;
  while(!(c.sr&1)){++r[5];t(int(r[5])>=int(r[6]));cycles+=3;if(c.sr&1)break;
   r[0]=w(0x8c10b270);r[2]=r[5]<<2;r[3]=w(r[0]+r[2]);r[0]=1538;r[1]=sb(r[4]+r[0]);t(r[1]==r[3]);cycles+=10;
  }
  r[0]=r[5];t(r[0]==4);cycles+=3;if(c.sr&1){r[0]=unsigned(-1);++cycles;}c.pc=c.pr;cycles+=3;
 }
 void mapping(){ //10B226
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=1588;r[14]=r[4];r[3]=w(r[14]+r[0]);put(m,r[15],r[3]);c.pr=0x8c10b23c;r[4]=r[14];cycles+=12;lookup();
  t(r[0]==unsigned(-1));r[5]=r[0];cycles+=3;
  if(c.sr&1){r[4]=0;cycles+=2;}else{r[0]=w(0x8c10b2c0);r[4]=r[5]<<2;r[4]=w(r[0]+r[4]);r[4]=w(r[4]+8);cycles+=5;}
  r[0]=1588;put(m,r[14]+r[0],r[4]);r[0]=1592;r[3]=w(r[15]);r[3]=(r[3]^r[4])&r[4];put(m,r[14]+r[0],r[3]);r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=16;
 }
 void clear_record(){auto&r=c.r;r[6]=1536;r[5]=r[4];r[6]+=r[5];t(r[5]>=r[6]);r[4]=0;cycles+=7;while(!(c.sr&1)){wb(r[5],r[4]);++r[5];t(r[5]>=r[6]);cycles+=4;}c.pc=c.pr;cycles+=3;}
 void pack(){ //10A2DA, preserved model from prior serializer check
  auto&r=c.r;push(r[14]);r[14]=1536;push(r[13]);r[3]=0xffffffbf;push(r[12]);r[13]=1;r[14]+=r[4];r[7]=1;
  need(r[4]<r[14],"Wrapping serializer input");r[3]=191;r[6]=1;r[13]=256;r[5]+=191;r[12]=0;unsigned nonzero=0;
  for(unsigned i=0;i<1536;++i){r[0]=r[6];if(r[6]==1)wb(r[5],0);r[2]=sb(r[4]);if(r[2]){r[1]=sb(r[5])|r[6];wb(r[5],r[1]);++nonzero;}r[6]*=2;++r[4];if(r[6]==256){r[6]=1;--r[5];}}
  t(true);r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=19029+3*nonzero;
 }
 void command(){ //64A734 ->664286, authentic free-lock path
  auto&r=c.r;push(c.pr);r[15]-=4;r[3]=w(r[15]+16);push(r[3]);r[2]=w(r[15]+16);push(r[2]);r[3]=w(r[15]+16);push(r[3]);r[3]=w(0x8c64a810);c.pr=0x8c64a74a;cycles+=12;
  r[3]=w(0x8c664350);push(c.pr);r[0]=w(r[3]);need(m[r[0]&0xffffff]==0,"Busy caller lock not modeled");t(true);wb(r[0],128);cycles+=5;
  r[3]=w(r[15]+12);push(r[3]);r[2]=w(r[15]+12);push(r[2]);r[3]=w(r[15]+12);push(r[3]);c.pr=0x8c6642b0;cycles+=9;++requests;worker();
  r[15]+=12;r[3]=w(0x8c664350);r[1]=0;r[2]=w(r[3]);wb(r[2],r[1]);c.pr=pop();c.pc=c.pr;cycles+=9;
  r[15]+=12;r[2]=w(0x8c64a80c);put(m,r[15],r[0]);r[0]=0x10e8;r[3]=w(r[2]);r[1]=w(r[3]+r[0]);t(r[1]==0);cycles+=8;need(c.sr&1,"Automatic dispatch path not modeled");
  r[0]=w(r[15]);r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=6;
 }
 void transport(){ //6067AA
  auto&r=c.r;push(c.pr);r[15]-=8;put(m,r[15],r[4]);put(m,r[15]+4,r[5]);c.pr=0x8c6067b6;r[4]=w(r[15]);cycles+=7;compatible();
  t(r[0]==0);cycles+=2;if(c.sr&1){r[15]+=8;c.pr=pop();c.pc=c.pr;r[0]=unsigned(-2);cycles+=5;return;}
  r[3]=48;push(r[3]);r[1]=0;r[2]=w(r[15]+8);r[5]=4;r[6]=r[1];r[7]=r[1];push(r[2]);r[2]=w(0x8c60685c);push(r[1]);c.pr=0x8c6067da;r[4]=w(r[15]+12);cycles+=13;command();
  r[4]=r[0];t(int(r[4])>=0);r[15]+=12;cycles+=4;
  if(c.sr&1){r[0]=0;r[15]+=8;c.pr=pop();c.pc=c.pr;cycles+=6;}else{r[15]+=8;c.pr=pop();c.pc=c.pr;r[0]=unsigned(-1);cycles+=5;}
 }
 void helper(){ //10A326 actual predicate-selected route1 or original skip
  auto&r=c.r;push(r[14]);r[0]=unsigned(-1);push(r[13]);r[0]<<=8;push(c.pr);r[0]+=64;r[15]+=r[0];r[3]=w(0x8c10a3a0);c.pr=0x8c10a33a;r[14]=r[4];cycles+=11;predicate();
  t(r[0]==0);cycles+=2;if(!(c.sr&1)){
   r[0]=sb(r[14]+1536);t(r[0]==1);cycles+=5;if(c.sr&1){
    r[0]=sb(r[14]+1537);t(r[0]==1);r[13]=r[15];cycles+=7;need(c.sr&1,"Other serializer route not modeled");
    r[5]=r[13];c.pr=0x8c10a35c;r[4]=r[14];cycles+=4;pack();cycles+=2;
    r[0]=1584;r[3]=w(0x8c10a3a4);r[5]=r[13];c.pr=0x8c10a374;r[4]=w(r[14]+r[0]);cycles+=8;transport();t(r[0]==0);++cycles;
   }
  }
  r[1]=192;r[15]+=r[1];c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=8;
 }
 void record(){ //10B2E0 state0 only, selected by the captured initial state
  auto&r=c.r;for(unsigned i=14;i>=8;--i)push(r[i]);push(c.pr);r[15]-=16;r[0]=1536;r[14]=r[4];r[3]=sb(r[14]+r[0]);t(r[3]==0);r[10]=0;cycles+=16;
  if(c.sr&1){r[0]=1539;wb(r[14]+r[0],r[10]);cycles+=4;}
  r[0]=1539;r[13]=w(0x8c10b340);r[9]=w(0x8c10b334);r[8]=w(0x8c10b330);r[0]=sb(r[14]+r[0]);r[12]=w(0x8c10b33c);r[11]=w(0x8c10b338);r[1]=25;t(r[0]>=r[1]);cycles+=12;
  need(!(c.sr&1)&&r[0]==0,"Only captured state0 dispatch modeled");t(r[0]>>31);r[0]<<=1;r[1]=r[0];r[0]=0x8c10b344;r[0]=half(r[0]+r[1]);need(r[0]+0x8c10b330==0x8c10b376,"State0 target differs");cycles+=7;
  c.pr=0x8c10b37a;r[4]=r[14];cycles+=3;predicate();t(r[0]==0);cycles+=2;
  if(c.sr&1){cycles+=2;}else{
   c.pr=0x8c10b386;r[4]=r[14];cycles+=3;clear_record();r[3]=1;cycles+=2;r[0]=1539;cycles+=5;wb(r[14]+r[0],r[3]);cycles+=2;
  }
  r[3]=w(0x8c10b8e8);c.pr=0x8c10b8a4;r[4]=r[14];cycles+=4;helper();
  r[15]+=16;c.pr=pop();for(unsigned i=8;i<14;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;
 }
 void parent(){ //10B992 gate2 (normal captured iteration) or gate1 toggle
  auto&r=c.r;push(r[14]);r[3]=w(0x8c10ba18);push(r[13]);push(r[12]);push(c.pr);r[0]=w(r[3]);t(r[0]==1);cycles+=8;bool skip=false;
  if(c.sr&1){r[2]=w(0x8c10ba1c);r[0]=w(r[2])^1;t(r[0]==0);put(m,r[2],r[0]);cycles+=6;skip=!(c.sr&1);}
  if(!skip){r[13]=w(0x8c10ba08);r[12]=(25<<8)-16;r[14]=r[13]+16;r[12]+=r[14];t(r[14]>=r[12]);put(m,r[13],r[4]);cycles+=10;
   while(!(c.sr&1)){
    ++records;c.pr=0x8c10b9c6;r[4]=r[14];cycles+=3;predicate();t(r[0]==0);cycles+=2;
    if(!(c.sr&1)){r[2]=w(0x8c10ba20);c.pr=0x8c10b9d0;r[4]=r[14];cycles+=4;refresh();}
    c.pr=0x8c10b9d4;r[4]=r[14];cycles+=3;mapping();c.pr=0x8c10b9d8;r[4]=r[14];cycles+=3;record();
    r[3]=1596;r[14]+=r[3];t(r[14]>=r[12]);cycles+=6;
   }
   r[3]=w(r[13]+8);r[2]=0;++r[3];put(m,r[13]+8,r[3]);put(m,r[13]+12,r[2]);cycles+=5;
  }
  c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
};

int main(int argc,char**argv){try{
 need(argc==5,"usage: parent-return RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,bool capture){auto predicted=rt.main_ram;ParentModel model{{c,predicted}};model.parent();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(predicted,stem+"/parent-expected-ram.bin");save_state(model.c,stem+"/parent-expected-state.txt");save_timers(timers,start,tocr,stem+"/parent-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"records\":"<<model.records<<",\"queries\":"<<model.queries<<",\"requests\":"<<model.requests<<"}\n";}
  call_recompiled(c,rt,0x8c10b992);
  if(capture){save_state(c,stem+"/parent-actual-state.txt");save_timers(rt.tmu,start,tocr,stem+"/parent-actual-timers.txt");}
  same(model.c,c);if(predicted!=rt.main_ram){unsigned n=0;for(unsigned i=0;i<predicted.size()&&n<12;++i)if(predicted[i]!=rt.main_ram[i]){std::cerr<<"RAM @"<<std::hex<<i<<" expected="<<unsigned(predicted[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';++n;}}
  need(predicted==rt.main_ram,"Complete parent RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Parent timer controls differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 // Alternate-frame/FPU/T/mode inputs catch hardcoded endpoint and saved-frame assumptions.
 unsigned controls=0;for(unsigned bank:{0u,1u})for(unsigned initialT:{0u,1u})for(unsigned mode:{0u,22u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.r[4]=mode;c.r[12]^=0x12345678;c.r[13]^=0xabcdef;c.r[14]^=0x100000;c.sr=(c.sr&~1u)|initialT;c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);check(c,false);++controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);const auto cycles=check(initial,true);
 need(word(rt.main_ram,0x8c7aa75c)==22&&word(rt.main_ram,0x8c7aa764)==1&&word(rt.main_ram,0x8c7aa768)==0&&rt.main_ram[0x664538]==0,"Independent loop/lock contract differs");
 for(unsigned i=0;i<4;++i){auto a=0x8c7aa76c+1596*i;need(rt.main_ram[(a+1539)&0xffffff]==unsigned(i==0),"Independent record state differs");}
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"parent_cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"device_clocks_enabled\":false,\"delivery_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original four-record parent return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<controls<<" frame/FPU/T/mode controls; real query/map/clear/serialize/request path, mode22/counter1/state1/lock0; device delivery/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
