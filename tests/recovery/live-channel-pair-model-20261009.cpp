// Source-derived live channel model only; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static void channel_prefix(auto&m,SH4Context&c){
 auto&r=c.r;for(unsigned i=14;i>=9;--i){r[15]-=4;put(m,r[15],r[i]);}r[15]-=4;put(m,r[15],c.pr);r[15]-=4;
 r[2]=word(m,0x8c66d0b8);r[3]=word(m,r[2]);need(r[3]!=0,"Captured global absent");c.sr&=~1u;r[9]=r[4];r[0]=32;r[4]=m[(r[15]+32)&0xffffff];need(r[4]==2,"Only captured two-channel prefix modeled");r[10]=1;c.sr|=1;r[2]=2;c.sr&=~1u;
 r[4]=r[15]+33;r[5]=3;need((r[4]&3)!=0,"Unexpected vararg alignment");c.sr&=~1u;r[14]=0;r[4]=r[15]+36;r[6]=0;
 for(unsigned i=0;i<3;++i){
  r[0]=32;r[13]=m[(r[15]+32)&0xffffff];c.sr=(c.sr&~1u)|(r[6]>=r[13]);r[11]=r[15];if(c.sr&1)break;
  r[7]=r[4]+4;r[3]=r[7];need(!(r[3]&3),"Unexpected argument alignment");c.sr|=1;r[13]=r[6]+r[11];r[4]=r[7];r[12]=r[4]-4;r[3]=r[12];need(!(r[3]&3),"Unexpected argument source");c.sr|=1;r[2]=r[12];r[1]=word(m,r[2]);m[r[13]&0xffffff]=r[1];r[7]=static_cast<int8_t>(m[r[13]&0xffffff]);need(r[7]<=15,"Invalid channel ID");c.sr|=1;r[2]=15;c.sr&=~1u;++r[6];
 }
 r[5]=r[11]+2;r[4]=r[13]+r[11];need(r[4]>=r[5],"Unexpected padding path");c.sr|=1;r[2]=word(m,0x8c66d0c4);r[5]=3;c.pr=0x8c66d07a;r[4]=r[9];c.pc=0x8c66d072;
}
static unsigned retained_allocator(auto&m,SH4Context&c){
 auto&r=c.r;r[15]-=4;put(m,r[15],r[14]);r[0]=r[5];r[15]-=4;put(m,r[15],r[13]);r[13]=0;r[15]-=4;put(m,r[15],c.pr);r[3]=0x8c67ff10;r[14]=0;r[6]=8;r[7]=word(m,r[3]);need(r[0]==3,"Only reached third pool modeled");c.sr|=1;
 r[5]=r[7]+0x88;r[6]=0;r[7]=4;unsigned occupied=0;
 for(;;){r[0]=word(m,r[5]+4);c.sr=(c.sr&~1u)|(r[0]==1);if(r[0]!=1)break;++r[6];c.sr=(c.sr&~1u)|(r[6]>=r[7]);r[5]+=8;++occupied;need(occupied<4,"Successful allocation required");}
 r[0]=r[6];c.sr&=~1u;r[3]=1;r[2]=r[5];put(m,r[4],r[5]);put(m,r[2]+4,r[3]);c.sr|=1;c.pr=word(m,r[15]);r[15]+=4;r[0]=r[14];r[13]=word(m,r[15]);r[15]+=4;r[14]=word(m,r[15]);r[15]+=4;c.pc=c.pr;return 41+7*occupied;
}
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


struct SecondCallbackModel:SevenByteModel{
 SH4Context count_return{};
 void callback(){auto&r=c.r;need(r[5]==1&&r[6]==0&&r[7]==0,"Only reached second channel value modeled");
  for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=8;r[3]=w(0x8c66f1ac);r[14]=w(r[3]);need(r[14]&&r[4],"Missing callback global/descriptor");t(false);
  r[0]=sb(r[4]+1);r[2]=44;r[1]=0xa8;r[12]=0;r[11]=r[0];r[3]=r[11];c.macl=44*r[3];r[14]+=r[1];r[6]=0;r[13]=0;r[3]=c.macl;r[14]+=r[3];r[0]=r[14]+28;r[3]=4+r[0];put(m,r[3],r[12]);r[4]=r[15];need(sb(r[14]+5)==2,"Reached channel count differs");
  for(unsigned i=0;i<2;++i){r[0]=r[13]*4;++r[13];put(m,r[4]+r[0],r[12]);}r[0]=2;t(true);r[13]=128;r[12]=1;r[0]=1;t(true);
  r[5]=r[4]+4;r[2]=w(r[5]);r[3]=0xffffffff;r[2]-=r[13];r[1]=r[2];need(r[2]==0xffffff80,"Second channel split differs");t(false);put(m,r[5],r[2]);r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);r[2]=0;++r[5];t(static_cast<int>(r[3])<0);const auto total=std::uint64_t(r[3])+(c.sr&1);r[3]=total;t(total>>32);t(r[3]&1);r[3]=(r[3]>>1)|(r[3]&0x80000000);put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);++r[5];r[3]+=r[13];put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);
  r[3]=w(0x8c66f1b8);r[2]=38;r[0]=w(0x8c66f1c0);r[14]=r[7];put(m,r[3],r[2]);r[2]=3&r[12];r[1]=w(0x8c66f1bc);r[3]=sb(r[0]+r[2]);r[0]=r[12];r[5]=w(0x8c66f1c4);t(false);r[3]|=r[11];r[13]=r[7];wb(r[1],r[3]);r[3]=0xfffffff8;r[13]=0;t(true);
  r[3]=w(0x8c66f220);r[1]=w(0x8c66f228);r[0]=w(0x8c66f22c);r[2]=w(0x8c66f224);wb(r[5],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);r[3]=w(r[4]+4);wb(r[1],r[3]);r[3]=w(0x8c66f230);wb(r[0],r[14]);wb(r[3],r[13]);r[3]=w(0x8c66f238);r[4]=w(0x8c66f234);c.pr=0x8c66f202;c.pc=0x8c66f1fa;cycles+=160;
  queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(r[14]==0,"Queue failed");r[0]=r[14];r[15]+=8;c.pr=pop();for(unsigned i=11;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }
 void caller(){auto&r=c.r;++r[14];t(r[14]>=r[12]);cycles+=3;need(!(c.sr&1),"Second channel absent");r[3]=r[11];r[6]=r[14];r[3]+=28;r[5]=r[14];r[6]=(r[6]<<2)+r[3];r[6]=w(r[6]);r[7]=0;c.pr=0x8c66f368;r[4]=r[10];cycles+=11;callback();
  ++r[14];t(r[14]>=r[12]);cycles+=3;need(c.sr&1,"Unexpected third channel");t(r[13]==0);cycles+=2;need(c.sr&1,"Count setter failed");c.pr=pop();r[0]=r[13];for(unsigned i=9;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;need(c.pc==0x8c66d0aa,"Count setter return differs");count_return=c;
  r[2]=w(0x8c66d0cc);r[5]=0;c.pr=0x8c66d0b2;r[4]=w(r[9]);c.pc=0x8c66d0aa;cycles+=5;need(r[2]==0x8c66eee0,"Next original helper differs");
 }
};
struct ChannelFieldModel:RequestModel{
 void field(){auto&r=c.r;r[3]=w(0x8c66ef4c);push(r[14]);push(c.pr);r[6]=w(r[3]);r[7]=w(0x8c66ef48);r[14]=0;need(r[6]&&r[4],"Reached global/descriptor required");t(false);cycles+=12;
  r[0]=sb(r[4]+1);r[3]=44;r[7]=r[0];r[4]=r[7];c.macl=44*r[4];r[0]=static_cast<int8_t>(r[5]);t(r[0]==127);r[4]=c.macl;cycles+=10;
  r[1]=0xa8;if(c.sr&1){r[1]+=r[6];r[4]+=r[1];r[0]=0;cycles+=5;}else{t(r[5]&0x80000000);r[5]<<=1;r[0]=0xffffffff;r[1]+=r[6];r[0]-=r[5];r[4]+=r[1];cycles+=6;}
  wb(r[4]+6,r[0]);cycles+=2;t(r[14]==0);cycles+=2;c.pr=pop();r[0]=r[14];c.pc=c.pr;r[14]=pop();cycles+=5;
 }
 SH4Context field_return{},pair_return{};
 void tail(){auto&r=c.r;r[4]=r[14];cycles+=2;
  for(;;){r[0]=32;r[2]=sb(r[15]+32);t(r[4]>=r[2]);cycles+=4;if(c.sr&1)break;
   r[0]=r[4];r[3]=r[13];r[2]=sb(r[11]+r[0]);r[3]+=7;r[3]+=r[4];r[5]=r[0];wb(r[3],r[2]);r[3]=r[13]+12;r[2]=r[13];r[5]<<=2;r[3]+=r[5];put(m,r[3],r[10]);r[3]=r[13]+36;r[2]+=20;r[2]+=r[5];r[5]+=r[3];put(m,r[2],r[14]);++r[4];put(m,r[5],r[14]);cycles+=21;
  }
  t(r[12]==0);cycles+=2;need(c.sr&1,"Channel constructor error");r[0]=r[12];r[15]+=4;c.pr=pop();for(unsigned i=9;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;need(c.pc==0x8c66bbb8,"Pair constructor returned elsewhere");pair_return=c;
  r[15]+=12;r[3]=r[0];t(r[3]==0);put(m,r[15],r[0]);cycles+=5;need(c.sr&1,"Service constructor error");
  r[3]=w(r[15]+20);r[9]=w(0x8c66bd0c);r[10]=w(0x8c66bd10);put(m,r[14]+40,r[3]);put(m,r[14]+8,r[11]);put(m,r[14]+4,r[11]);r[11]=r[13];cycles+=8;
  r[3]=w(r[14]+4);t(r[11]>=r[3]);cycles+=3;need(!(c.sr&1),"No setup channel");r[5]=r[11];c.pr=0x8c66bc1a;r[4]=w(r[14]+40);c.pc=0x8c66bc14;cycles+=4;need(r[9]==0x8c66d184,"Next setup helper differs");
 }
};

static unsigned pair(auto& expected_ram,SH4Context&expected){auto&er=expected.r;channel_prefix(expected_ram,expected);auto cycles=106+retained_allocator(expected_ram,expected);er[12]=er[0];need(er[12]==0,"Allocator failed");expected.sr|=1;cycles+=3;
  er[4]=word(expected_ram,er[9]);er[2]=44;er[3]=0x8c67ff10;er[0]=static_cast<int8_t>(expected_ram[(er[4]+1)&0xffffff]);er[1]=0xa8;er[4]=er[0];expected.macl=44*er[4];er[13]=word(expected_ram,er[3]);er[0]=32;er[3]=word(expected_ram,0x8c66d0c8);er[13]+=er[1];er[4]=expected.macl;er[13]+=er[4];put(expected_ram,er[13],er[10]);er[0]=static_cast<int8_t>(expected_ram[(er[15]+32)&0xffffff]);expected_ram[(er[13]+4)&0xffffff]=er[0];er[0]=32;er[5]=static_cast<int8_t>(expected_ram[(er[15]+32)&0xffffff]);expected.pr=0x8c66d0aa;er[4]=word(expected_ram,er[9]);expected.pc=0x8c66d080;cycles+=23;need(er[3]==0x8c66f2ec&&cycles==173,"Source-derived next target/cycles differ");
return cycles;}
static void count(auto&expected_ram,SH4Context&expected){auto&er=expected.r;er[15]-=4;put(expected_ram,er[15],er[14]);er[7]=0;er[15]-=4;put(expected_ram,er[15],er[13]);er[15]-=4;put(expected_ram,er[15],er[12]);er[15]-=4;put(expected_ram,er[15],er[11]);er[3]=word(expected_ram,0x8c66f390);er[15]-=4;put(expected_ram,er[15],er[10]);er[10]=er[4];er[15]-=4;put(expected_ram,er[15],er[9]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[4]=word(expected_ram,er[3]);need(er[4]&&er[10],"Reached descriptor/global absent");expected.sr&=~1u;er[13]=0;er[2]=1;er[12]=static_cast<int8_t>(er[5]);need(er[12]>=1&&er[12]<=2,"Reached active count invalid");er[1]=2;expected.sr&=~1u;
  er[0]=static_cast<int8_t>(expected_ram[(er[10]+1)&0xffffff]);er[3]=44;er[2]=0xa8;expected.sr|=1;er[6]=er[0];er[11]=er[6];expected.macl=44*er[11];er[2]+=er[4];er[11]=expected.macl+er[2];er[0]=static_cast<int8_t>(expected_ram[(er[11]+4)&0xffffff]);need(er[0]>=er[12],"Active count exceeds configured count");expected.sr|=1;er[0]=er[5];expected_ram[(er[11]+5)&0xffffff]=er[0];er[14]=er[7];er[9]=word(expected_ram,0x8c66f3a0);er[3]=er[11];er[6]=er[14];er[3]+=28;er[5]=er[14];er[6]=(er[6]<<2)+er[3];er[6]=word(expected_ram,er[6]);er[7]=0;expected.pr=0x8c66f368;er[4]=er[10];expected.pc=0x8c66f354;const unsigned cycles=55;need(er[9]==0x8c66f0b4,"Channel callback differs");
}

struct FirstCallbackModel:SevenByteModel{
 void callback(){auto&r=c.r;need(r[5]==0&&r[6]==0&&r[7]==0,"Only channel0/count2/value0 prefix modeled");
 for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=8;r[3]=w(0x8c66f1ac);r[14]=w(r[3]);need(r[14]&&r[4],"Missing global/descriptor");t(false);
 r[0]=sb(r[4]+1);r[2]=44;r[1]=0xa8;r[12]=0;r[11]=r[0];r[3]=r[11];c.macl=44*r[3];r[14]+=r[1];r[6]=0;r[13]=0;r[3]=c.macl;r[14]+=r[3];r[0]=r[14]+28;r[3]=r[0];put(m,r[3],0);r[4]=r[15];need(sb(r[14]+5)==2,"Wrong channel count");
 for(unsigned i=0;i<2;++i){r[0]=i*4;++r[13];put(m,r[4]+r[0],0);}r[0]=2;t(true);r[13]=128;t(false);r[12]=0;t(true);r[0]=0;t(true);r[2]=w(r[4])+128;put(m,r[4],r[2]);r[5]=0;
 for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);r[2]=0;++r[5];t(false);t(false);t(r[3]&1);r[3]>>=1;put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);r[5]=0;
 for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);++r[5];r[3]+=128;put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);
 r[3]=w(0x8c66f1b8);r[2]=38;r[0]=w(0x8c66f1c0);r[14]=r[7];put(m,r[3],38);r[2]=3&r[12];r[1]=w(0x8c66f1bc);r[3]=sb(r[0]+r[2]);r[0]=r[12];r[5]=w(0x8c66f1c4);t(true);r[3]|=r[11];r[13]=r[7];wb(r[1],r[3]);r[3]=0xfffffff8;r[13]=0;
 r[2]=w(r[4]);r[3]=w(0x8c66f220);wb(r[5],r[2]);r[2]=w(0x8c66f224);r[1]=w(0x8c66f228);wb(r[3],r[14]);r[3]+=4;wb(r[2],r[13]);r[2]+=4;wb(r[1],0);wb(r[3],0);wb(r[2],0);r[3]=w(0x8c66f238);r[4]=w(0x8c66f234);c.pr=0x8c66f202;c.pc=0x8c66f1fa;need(r[3]==0x8c65e1a4,"Queue target differs");cycles+=152;
 queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(r[14]==0,"Queue failed");r[0]=r[14];r[15]+=8;c.pr=pop();for(unsigned i=11;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c66bba0,"Wrong initial caller");unsigned cycles=0;
 auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
 cycles+=pair(m,c);save("pair-prefix");count(m,c);cycles+=55;save("count-prefix");
 FirstCallbackModel first{{{c,m}}};first.callback();need(first.cycles==257,"First callback cycles differ");c=first.c;cycles+=first.cycles;save("first-callback");
 SecondCallbackModel second{{{c,m}}};second.caller();need(second.cycles==299,"Second callback cycles differ");c=second.c;cycles+=second.cycles;save("second-callback");
 ChannelFieldModel field{{c,m}};field.field();field.tail();need(field.cycles==126,"Field/tail cycles differ");c=field.c;cycles+=field.cycles;save("end");need(cycles==910,"Total cycles differ");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":910,"guest_execution":false,"next_target":"8C66D184"})"<<'\n';std::cout<<"PASS source model910cycles to66D184\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
