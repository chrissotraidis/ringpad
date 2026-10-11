// Retained source-derived service return model; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
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
};


struct ChannelStartModel:SevenByteModel{
 void start(){auto&r=c.r;r[3]=0x8c66ee7c;cycles+=4;r[2]=w(0x8c66eec0);push(r[14]);push(c.pr);r[3]=w(r[2]);need(r[3]&&r[4],"Reached global/descriptor required");t(false);cycles+=8;
  r[2]=w(0x8c66eecc);r[3]=46;r[1]=w(0x8c66eed0);put(m,r[2],r[3]);r[0]=sb(r[4]+1);r[3]=w(0x8c66eed4);wb(r[1],r[0]);c.pr=0x8c66eeaa;r[4]=r[2];cycles+=10;queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(c.sr&1,"Channel start queue error");c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();cycles+=5;
 }
 SH4Context first_return{};
 void caller(){auto&r=c.r;
  auto check=[&](){r[4]=r[0];t(r[4]==0);cycles+=3;need(c.sr&1,"Channel start failed");++r[11];r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=4;};
  check();need(!(c.sr&1)&&r[11]==1,"Expected second channel");r[5]=r[11];c.pr=0x8c66bc1a;r[4]=w(r[14]+40);cycles+=4;start();check();need(c.sr&1&&r[11]==2,"Two-channel loop did not finish");
  r[9]=w(0x8c66bd18);r[11]=r[13];r[8]=w(0x8c66bd14);r[10]=127;cycles+=5;r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=3;need(!(c.sr&1),"No channel configuration");push(r[10]);r[6]=127;push(r[13]);r[5]=r[11];push(r[13]);r[7]=0;c.pr=0x8c66bc46;r[4]=w(r[14]+40);c.pc=0x8c66bc36;cycles+=9;need(r[8]==0x8c66d190,"Next configuration helper differs");
 }
};
struct ChannelConfigureModel:RequestModel{
 void field(){auto&r=c.r;r[3]=w(0x8c66ef4c);push(r[14]);push(c.pr);r[6]=w(r[3]);r[7]=w(0x8c66ef48);r[14]=0;need(r[6]&&r[4],"Reached global/descriptor required");t(false);cycles+=12;
  r[0]=sb(r[4]+1);r[3]=44;r[7]=r[0];r[4]=r[7];c.macl=44*r[4];r[0]=static_cast<int8_t>(r[5]);t(r[0]==127);r[4]=c.macl;cycles+=10;
  r[1]=0xa8;if(c.sr&1){r[1]+=r[6];r[4]+=r[1];r[0]=0;cycles+=5;}else{t(r[5]&0x80000000);r[5]<<=1;r[0]=0xffffffff;r[1]+=r[6];r[0]-=r[5];r[4]+=r[1];cycles+=6;}
  wb(r[4]+6,r[0]);cycles+=2;t(r[14]==0);cycles+=2;c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();cycles+=5;
 }

 SH4Context config_return{},service_return{};
 void configure(){auto&r=c.r;r[15]-=4;r[3]=w(0x8c66d19c);wb(r[15],r[6]);r[5]=sb(r[15]);r[15]+=4;cycles+=7;need(r[3]==0x8c66eee0,"Retained field target differs");field();}
 void caller(){auto&r=c.r;auto check=[&](){r[4]=r[0];t(r[4]==0);need(c.sr&1,"Configuration failed");r[15]+=12;++r[11];r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=8;};check();need(!(c.sr&1)&&r[11]==1,"Expected second configuration");push(r[10]);r[6]=127;push(r[13]);r[5]=r[11];push(r[13]);r[7]=0;c.pr=0x8c66bc46;r[4]=w(r[14]+40);cycles+=9;configure();check();need(c.sr&1&&r[11]==2,"Configuration loop did not end");
  r[0]=64;r[1]=0xffffffff;r[3]=1;put(m,r[14]+r[0],r[1]);r[0]=r[14];put(m,r[14]+60,r[13]);put(m,r[14],r[3]);cycles+=7;r[15]+=32;c.pr=pop();for(unsigned i=8;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;need(c.pc==0x8c6609f4,"Service constructor return differs");service_return=c;
  r[1]=r[14]+64;put(m,r[1],r[0]);r[0]=64;r[3]=w(r[14]+r[0]);t(r[3]==0);need(!(c.sr&1),"Service handle absent");cycles+=7;c.pr=0x8c660a10;r[4]=r[14];c.pc=0x8c660a0c;cycles+=3;
 }
};

static unsigned record(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const unsigned object=er[4],phase=2;const auto&ram=expected_ram;need(object==er[14],"Object differs");
  for(unsigned off:{1u,4u,5u,6u,7u})expected_ram[(object+off)&0xffffff]=0;
  for(unsigned off:{52u,56u,60u,80u,108u,124u,128u,132u,136u,140u,144u,152u})put(expected_ram,object+off,0);
  put(expected_ram,object+8,16);put(expected_ram,object+12,word(ram,0x8c6609b0));put(expected_ram,object+72,0x2000);put(expected_ram,object+76,0x2000);put(expected_ram,object+120,0xffffffff);put(expected_ram,object+148,word(ram,0x8c6609b4));er[0]=148;er[2]=16;er[3]=word(ram,0x8c6609b4);er[5]=0;er[6]=0x2000;expected.pc=expected.pr;const auto init_return=expected;unsigned cycles=46;
  if(phase==2){er[0]=static_cast<int8_t>(expected_ram[(er[14]+3)&0xffffff]);er[5]=er[0]&255;expected.pr=0x8c660a18;er[4]=er[14];expected.pc=0x8c660a10;cycles+=5;need(er[5]==2,"Captured channel count changed");}
return cycles;}

int main(int argc,char**argv){try{
 need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c66bc14,"Wrong initial caller");unsigned cycles=0;
 auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
 ChannelStartModel start{{{c,m}}};start.start();start.caller();need(start.cycles==281,"Start cycles differ");c=start.c;cycles+=start.cycles;save("start");
 ChannelConfigureModel configure{{c,m}};configure.configure();configure.caller();need(configure.cycles==139,"Configuration cycles differ");save_state(configure.service_return,out+"/service-return-state.txt");c=configure.c;cycles+=configure.cycles;save("configure");
 cycles+=record(m,c);save("end");need(cycles==471,"Total cycles differ");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":471,"guest_execution":false,"next_target":"8C6618D6"})"<<'\n';std::cout<<"PASS source model471cycles to6618D6\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
