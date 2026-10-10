// Retained source-derived stereo setup model; no guest execution.
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


struct CountForwardModel:SevenByteModel{
 void callback(){auto&r=c.r;const unsigned channel=r[5];need(channel<2&&r[6]==0&&r[7]==0,"Only reached zero-valued stereo callback modeled");
  for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=8;r[3]=w(0x8c66f1ac);r[14]=w(r[3]);need(r[14]&&r[4],"Missing callback global/descriptor");t(false);
  r[0]=sb(r[4]+1);r[2]=44;r[1]=0xa8;r[12]=0;r[11]=r[0];r[3]=r[11];c.macl=44*r[3];r[14]+=r[1];r[6]=0;r[13]=0;r[3]=c.macl;r[14]+=r[3];r[0]=r[14]+28;r[3]=4*channel+r[0];put(m,r[3],r[12]);r[4]=r[15];need(sb(r[14]+5)==2,"Reached channel count differs");
  for(unsigned i=0;i<2;++i){r[0]=r[13]*4;++r[13];put(m,r[4]+r[0],r[12]);}r[0]=2;t(true);r[13]=128;r[12]=channel;r[0]=channel;t(channel==0);
  if(channel==0){r[2]=w(r[4])+r[13];put(m,r[4],r[2]);}else{r[5]=r[4]+4;r[2]=w(r[5]);r[3]=0xffffffff;r[2]-=r[13];r[1]=r[2];need(r[2]==0xffffff80,"Second channel split differs");t(false);put(m,r[5],r[2]);}r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);r[2]=0;++r[5];t(static_cast<int>(r[3])<0);const auto total=std::uint64_t(r[3])+(c.sr&1);r[3]=total;t(total>>32);t(r[3]&1);r[3]=(r[3]>>1)|(r[3]&0x80000000);put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);++r[5];r[3]+=r[13];put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);
  r[3]=w(0x8c66f1b8);r[2]=38;r[0]=w(0x8c66f1c0);r[14]=r[7];put(m,r[3],r[2]);r[2]=3&r[12];r[1]=w(0x8c66f1bc);r[3]=sb(r[0]+r[2]);r[0]=r[12];r[5]=w(0x8c66f1c4);t(false);r[3]|=r[11];r[13]=r[7];wb(r[1],r[3]);r[3]=0xfffffff8;r[13]=0;t(true);
  if(channel==0){r[2]=w(r[4]);r[3]=w(0x8c66f220);wb(r[5],r[2]);r[2]=w(0x8c66f224);r[1]=w(0x8c66f228);wb(r[3],r[14]);r[3]+=4;wb(r[2],r[13]);r[2]+=4;wb(r[1],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);}else{r[3]=w(0x8c66f220);r[1]=w(0x8c66f228);r[0]=w(0x8c66f22c);r[2]=w(0x8c66f224);wb(r[5],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);r[3]=w(r[4]+4);wb(r[1],r[3]);r[3]=w(0x8c66f230);wb(r[0],r[14]);wb(r[3],r[13]);}r[3]=w(0x8c66f238);r[4]=w(0x8c66f234);c.pr=0x8c66f202;c.pc=0x8c66f1fa;cycles+=channel?160:152;
  queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(r[14]==0,"Queue failed");r[0]=r[14];r[15]+=8;c.pr=pop();for(unsigned i=11;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }

 SH4Context count_return{},adapter_return{};
 void count(){auto&r=c.r;r[3]=0x8c66f2ec;cycles+=4;push(r[14]);r[7]=0;push(r[13]);push(r[12]);push(r[11]);r[3]=w(0x8c66f390);push(r[10]);r[10]=r[4];push(r[9]);push(c.pr);r[4]=w(r[3]);need(r[4]&&r[10],"Descriptor/global required");t(false);r[13]=0;r[2]=1;r[12]=static_cast<int8_t>(r[5]);need(r[12]==2,"Captured stereo count required");r[1]=2;t(false);r[0]=sb(r[10]+1);r[3]=44;r[2]=0xa8;t(true);r[6]=r[0];r[11]=r[6];c.macl=44*r[11];r[2]+=r[4];r[11]=c.macl+r[2];r[0]=sb(r[11]+4);need(r[0]>=r[12],"Configured count too small");t(true);r[0]=r[5];wb(r[11]+5,r[0]);r[14]=r[7];r[9]=w(0x8c66f3a0);cycles+=44;
  for(unsigned i=0;i<2;++i){r[3]=r[11];r[6]=r[14];r[3]+=28;r[5]=r[14];r[6]=(r[6]<<2)+r[3];r[6]=w(r[6]);r[7]=0;c.pr=0x8c66f368;r[4]=r[10];cycles+=11;callback();++r[14];t(r[14]>=r[12]);cycles+=3;}
  t(r[13]==0);need(c.sr&1,"Count setter error");cycles+=2;c.pr=pop();r[0]=r[13];for(unsigned i=9;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;need(cycles==610&&c.pc==0x8c66c1c0,"Forwarded count return differs");count_return=c;
 }
 void caller(){auto&r=c.r;r[14]=r[0];t(r[14]==0);cycles+=3;need(c.sr&1,"Adapter error");r[0]=r[14];r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;need(c.pc==0x8c6618fe,"Count adapter return differs");adapter_return=c;
  r[4]=r[0];t(r[4]==0);cycles+=3;need(c.sr&1,"Outer count error");r[0]=r[13];t(r[0]==1);r[6]=r[14];cycles+=4;need(!(c.sr&1)&&r[0]==2,"Captured stereo branch differs");r[2]=r[14];r[3]=0xfffffff1;r[2]+=112;put(m,r[2],r[3]);r[0]=116;r[2]=15;put(m,r[14]+r[0],r[2]);cycles+=7;r[6]+=112;r[6]=w(r[6]);r[5]=0;c.pr=0x8c66193a;r[4]=r[14];cycles+=6;
  push(r[14]);push(c.pr);r[15]-=4;r[14]=r[4];t(r[14]==0);put(m,r[15],r[5]);need(!(c.sr&1),"Record absent");cycles+=7;r[4]=0xfffffff1;t(static_cast<int>(r[6])>=-15);cycles+=3;need(c.sr&1,"Unexpected lower clamp");r[4]=15;t(static_cast<int>(r[6])>15);cycles+=3;need(!(c.sr&1),"Unexpected upper clamp");r[2]=w(r[15]);r[3]=r[14];r[3]+=112;r[2]=(r[2]<<2)+r[3];r[3]=w(0x8c661ab8);put(m,r[2],r[6]);r[0]=64;r[5]=w(r[15]);c.pr=0x8c661a4c;r[4]=w(r[14]+64);c.pc=0x8c661a36;cycles+=12;need(r[3]==0x8c66c0bc&&cycles==664,"Next pan target/cycles differ");
 }
};
struct PanModel : RequestModel {
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }
 void divide(){auto&r=c.r;t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e728);r[2]=w(0x8c10e72c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;div0s(2,1);const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}rot(1);const std::uint64_t sum=std::uint64_t(r[1])+r[2]+(c.sr&1);r[1]=unsigned(sum);t(sum>>32);r[0]=r[1];r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=80;
 }
 void pan(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[1]=r[6];r[1]<<=2;r[1]<<=2;r[2]=w(0x8c66c154);r[1]<<=2;r[3]=r[6];t(r[1]>>31);r[1]<<=1;put(m,r[15],r[4]);r[1]-=r[3];c.pr=0x8c66c0d8;r[0]=15;cycles+=15;need(r[2]==0x8c10e67c,"Original division target differs");divide();
 r[3]=w(0x8c66c158);r[6]=r[0];wb(r[15]+4,r[0]);r[7]=0;r[4]=w(r[15]);c.pr=0x8c66c0e6;r[4]=w(r[4]+40);c.pc=0x8c66c0d8;cycles+=8;need(r[3]==0x8c66d1d8,"Original pan forwarding target differs");
 }
};
struct PanForwardModel:SevenByteModel{
 void callback(){auto&r=c.r;const unsigned channel=r[5];need(channel<2&&r[7]==0,"Only stereo callback with zero secondary value modeled");
  for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=8;r[3]=w(0x8c66f1ac);r[14]=w(r[3]);need(r[14]&&r[4],"Missing callback global/descriptor");t(false);
  r[0]=sb(r[4]+1);r[2]=44;r[1]=0xa8;r[12]=static_cast<int8_t>(r[6]);r[11]=r[0];r[3]=r[11];c.macl=44*r[3];r[14]+=r[1];r[6]=0;r[13]=0;r[3]=c.macl;r[14]+=r[3];r[0]=r[14]+28;r[3]=4*channel+r[0];put(m,r[3],r[12]);r[4]=r[15];need(sb(r[14]+5)==2,"Reached channel count differs");
  for(unsigned i=0;i<2;++i){r[0]=r[13]*4;++r[13];put(m,r[4]+r[0],r[12]);}r[0]=2;t(true);r[13]=128;r[12]=channel;r[0]=channel;t(channel==0);
  if(channel==0){r[2]=w(r[4])+r[13];put(m,r[4],r[2]);}else{r[5]=r[4]+4;r[2]=w(r[5]);r[3]=0xffffffff;r[2]-=r[13];r[1]=r[2];t(static_cast<int32_t>(r[1])>=-1);put(m,r[5],r[2]);if(c.sr&1){r[0]=0xfffffffe;put(m,r[4]+4,r[0]);cycles+=2;}}r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);r[2]=0;++r[5];t(static_cast<int>(r[3])<0);const auto total=std::uint64_t(r[3])+(c.sr&1);r[3]=total;t(total>>32);t(r[3]&1);r[3]=(r[3]>>1)|(r[3]&0x80000000);put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);r[5]=0;
  for(unsigned i=0;i<2;++i){r[0]=r[5]*4;r[3]=w(r[4]+r[0]);++r[5];r[3]+=r[13];put(m,r[4]+r[0],r[3]);}r[0]=2;t(true);
  r[3]=w(0x8c66f1b8);r[2]=38;r[0]=w(0x8c66f1c0);r[14]=r[7];put(m,r[3],r[2]);r[2]=3&r[12];r[1]=w(0x8c66f1bc);r[3]=sb(r[0]+r[2]);r[0]=r[12];r[5]=w(0x8c66f1c4);t(false);r[3]|=r[11];r[13]=r[7];wb(r[1],r[3]);r[3]=0xfffffff8;r[13]=0;t(true);
  if(channel==0){r[2]=w(r[4]);r[3]=w(0x8c66f220);wb(r[5],r[2]);r[2]=w(0x8c66f224);r[1]=w(0x8c66f228);wb(r[3],r[14]);r[3]+=4;wb(r[2],r[13]);r[2]+=4;wb(r[1],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);}else{r[3]=w(0x8c66f220);r[1]=w(0x8c66f228);r[0]=w(0x8c66f22c);r[2]=w(0x8c66f224);wb(r[5],r[6]);wb(r[3],r[6]);wb(r[2],r[6]);r[3]=w(r[4]+4);wb(r[1],r[3]);r[3]=w(0x8c66f230);wb(r[0],r[14]);wb(r[3],r[13]);}r[3]=w(0x8c66f238);r[4]=w(0x8c66f234);c.pr=0x8c66f202;c.pc=0x8c66f1fa;cycles+=channel?160:152;
  queue();r[14]=r[0];++cycles;t(r[14]==0);cycles+=2;need(r[14]==0,"Queue failed");r[0]=r[14];r[15]+=8;c.pr=pop();for(unsigned i=11;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }

 SH4Context callback_return{};
 void forward(){auto&r=c.r;r[3]=w(0x8c66d1e0);need(r[3]==0x8c66f0b4,"Wrong retained callback");cycles+=4;callback();callback_return=c;need(c.pc==0x8c66c0e6,"Callback return differs");}
 void tail(){auto&r=c.r;r[14]=r[0];t(r[14]==0);need(c.sr&1,"Pan callback failed");cycles+=3;r[0]=r[14];r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;need(c.pc==0x8c661a4c,"Pan adapter return differs");}
};
struct PanCompleteModel:PanForwardModel{
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }
 void divide(){auto&r=c.r;t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e728);r[2]=w(0x8c10e72c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;div0s(2,1);const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}rot(1);const std::uint64_t sum=std::uint64_t(r[1])+r[2]+(c.sr&1);r[1]=unsigned(sum);t(sum>>32);r[0]=r[1];r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=80;
 }
 void pan(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[1]=r[6];r[1]<<=2;r[1]<<=2;r[2]=w(0x8c66c154);r[1]<<=2;r[3]=r[6];t(r[1]>>31);r[1]<<=1;put(m,r[15],r[4]);r[1]-=r[3];c.pr=0x8c66c0d8;r[0]=15;cycles+=15;need(r[2]==0x8c10e67c,"Original division target differs");divide();
 r[3]=w(0x8c66c158);r[6]=r[0];wb(r[15]+4,r[0]);r[7]=0;r[4]=w(r[15]);c.pr=0x8c66c0e6;r[4]=w(r[4]+40);c.pc=0x8c66c0d8;cycles+=8;need(r[3]==0x8c66d1d8,"Original pan forwarding target differs");
 }
 SH4Context first_return{},outer_return{};
 void inner_tail(){auto&r=c.r;r[4]=r[0];t(r[4]==0);need(c.sr&1,"Pan adapter error");r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=8;}
 void second(){auto&r=c.r;inner_tail();need(c.pc==0x8c66193a,"First pan return differs");first_return=c;
 c.pr=pop();r[0]=116;r[4]=r[14];r[6]=w(r[14]+r[0]);r[5]=1;r[13]=pop();r[14]=pop();cycles+=8;need(r[6]==15,"Captured second pan differs");
 push(r[14]);push(c.pr);r[15]-=4;r[14]=r[4];t(r[14]==0);put(m,r[15],r[5]);need(!(c.sr&1),"Outer record absent");cycles+=7;r[4]=0xfffffff1;t(static_cast<int32_t>(r[6])>=-15);cycles+=3;need(c.sr&1,"Unexpected lower clamp");r[4]=15;t(static_cast<int32_t>(r[6])>15);cycles+=3;need(!(c.sr&1),"Unexpected upper clamp");r[2]=w(r[15]);r[3]=r[14]+112;r[2]=(r[2]<<2)+r[3];r[3]=w(0x8c661ab8);put(m,r[2],r[6]);r[0]=64;r[5]=w(r[15]);c.pr=0x8c661a4c;r[4]=w(r[14]+64);cycles+=12;
 pan();forward();tail();inner_tail();need(c.pc==0x8c660a18&&cycles==432,"Stereo setup return differs");outer_return=c;
 t(static_cast<int32_t>(r[13])>0);r[12]=r[11];need(c.sr&1,"Captured channel count absent");r[11]=w(0x8c660aa8);r[7]=r[15];r[5]=r[12];r[0]=64;r[6]=r[15];r[7]+=4;c.pr=0x8c660a2e;r[4]=w(r[14]+64);c.pc=0x8c660a20;cycles+=12;need(r[11]==0x8c66c37c&&cycles==444,"Next helper prediction differs");
 }
};

static void outer(auto&expected_ram,SH4Context&expected){auto&er=expected.r;
  er[15]-=4;put(expected_ram,er[15],er[14]);er[14]=er[4];er[15]-=4;put(expected_ram,er[15],er[13]);need(er[14]!=0,"Actual record absent");expected.sr&=~1u;er[15]-=4;put(expected_ram,er[15],expected.pr);er[13]=er[5];er[0]=er[13];er[3]=word(expected_ram,0x8c6619cc);er[5]=er[0];expected_ram[(er[14]+3)&0xffffff]=er[0];er[0]=64;expected.pr=0x8c6618fe;er[4]=word(expected_ram,er[14]+64);need(er[3]==0x8c66c1aa&&er[4]!=0,"Retained service handle differs");
  er[15]-=4;put(expected_ram,er[15],er[14]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;er[2]=static_cast<int8_t>(er[5]);er[3]=er[4];put(expected_ram,er[15],er[4]);put(expected_ram,er[3]+8,er[2]);er[3]=word(expected_ram,0x8c66c408);er[4]=word(expected_ram,er[15]);expected.pr=0x8c66c1c0;er[4]=word(expected_ram,er[4]+40);expected.pc=0x8c66c1aa;need(er[3]==0x8c66d1f8,"Next target differs");
}

int main(int argc,char**argv){try{
 need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c660a10,"Wrong initial caller");unsigned cycles=0;
 auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
 outer(m,c);cycles+=27;save("outer");
 CountForwardModel count{{{c,m}}};count.count();count.caller();need(count.cycles==664,"Count cycles differ");c=count.c;cycles+=count.cycles;save("count");
 PanModel pan{{c,m}};pan.pan();need(pan.cycles==103,"Pan cycles differ");c=pan.c;cycles+=pan.cycles;save("pan");
 PanForwardModel forward{{{c,m}}};forward.forward();forward.tail();need(forward.cycles==270,"Forward cycles differ");c=forward.c;cycles+=forward.cycles;save("forward");
 PanCompleteModel complete{{{{c,m}}}};complete.second();need(complete.cycles==444,"Stereo return cycles differ");save_state(complete.outer_return,out+"/stereo-return-state.txt");c=complete.c;cycles+=complete.cycles;save("end");need(cycles==1508,"Total cycles differ");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":1508,"guest_execution":false,"next_target":"8C66C37C"})"<<'\n';std::cout<<"PASS source model1508cycles to66C37C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
