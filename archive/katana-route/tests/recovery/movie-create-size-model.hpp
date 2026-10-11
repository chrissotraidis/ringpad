// Source-derived integer/SH-4 model retained from startup-movie-size-return.cpp.
struct SizeModel : RequestModel {
 std::function<void(unsigned,const SizeModel&)> observe;
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

 void phase(unsigned i){if(observe)observe(i,*this);}
 void shar(unsigned n){const auto v=c.r[n];t(v&1);c.r[n]=(v>>1)|(v&0x80000000);}
 void stream(){auto&r=c.r;push(r[14]);r[14]=r[5];push(c.pr);t(!(r[14]>>31));cycles+=5;if(!(c.sr&1)){r[14]+=7;++cycles;}
  shar(14);r[0]=r[4];shar(14);shar(14);t(r[0]==2);cycles+=6;
  if(c.sr&1){r[4]=0;put(m,r[6],r[14]);put(m,r[7],r[4]);r[3]=w(r[15]+8);put(m,r[3],r[14]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=11;}
  else{r[4]=0x800;r[1]=r[14];r[3]=w(0x8ced0320);r[1]+=r[4];c.pr=0x8ced01f6;r[0]=r[4];c.pc=r[3];cycles+=7;phase(1);divide();phase(2);
   r[2]=w(0x8ced0324);r[0]<<=8;r[4]=w(0x8ced032c);r[0]<<=2;t(r[0]>>31);r[0]<<=1;put(m,r[6],r[0]);put(m,r[7],r[2]);r[1]=w(r[6]);r[2]=w(0x8ced0328);r[3]=w(r[15]+8);r[1]+=r[2];put(m,r[3],r[1]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=16;}
  r[3]=w(r[15]+20);r[2]=w(0x8ced0330);put(m,r[3],r[2]);c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
 }
 void addc(unsigned rm,unsigned rn){const std::uint64_t sum=std::uint64_t(c.r[rn])+c.r[rm]+(c.sr&1);c.r[rn]=unsigned(sum);t(sum>>32);}
 void shll(unsigned n){const unsigned old=c.r[n];c.r[n]<<=1;t(old>>31);}
 void dimensions(){auto&r=c.r;r[1]=0;r[2]=r[4];t(std::int32_t(r[2])<0);addc(1,2);shar(2);r[2]+=31;t(!(r[2]>>31));r[3]=w(r[15]);cycles+=9;
  if(!(c.sr&1)){r[2]+=31;++cycles;}r[1]=0xfffffffbu;r[2]=(r[2]>>5)|((r[2]&0x80000000)?0xf8000000:0);r[2]<<=2;r[2]<<=2;shll(2);put(m,r[3],r[2]);r[1]=0;r[2]=r[5];t(std::int32_t(r[2])<0);addc(1,2);r[3]=w(r[15]+4);shar(2);put(m,r[3],r[2]);put(m,r[6],r[4]);c.pc=c.pr;put(m,r[7],r[5]);cycles+=17;
 }
 void frame_tail(){auto&r=c.r;r[1]=w(r[15]+16);r[3]=w(r[15]+20);r[0]=w(r[15]+12);c.macl=r[1]*r[3];r[1]=w(r[15]+8);r[2]=w(r[15]+28);r[3]=c.macl;c.macl=r[1]*r[0];shll(3);r[0]=c.macl;r[3]+=r[0];shll(3);r[3]+=32;put(m,r[2],r[3]);r[1]=w(r[15]+8);r[3]=w(r[15]+12);r[2]=w(r[15]+36);c.macl=r[1]*r[3];r[3]=c.macl;r[0]=r[3];shll(3);r[3]+=r[0];r[0]=0;t(std::int32_t(r[3])<0);addc(0,3);r[0]=w(r[15]+24);shar(3);c.macl=r[3]*r[0];r[3]=c.macl;r[3]+=32;put(m,r[2],r[3]);r[15]+=32;c.pr=pop();c.pc=c.pr;cycles+=40;
 }
 void wrapper_tail(){auto&r=c.r;r[0]=w(r[15]+28);r[3]=w(r[15]+32);r[2]=w(r[15]+36);r[1]=w(r[15]+40);r[0]+=r[3];r[3]=w(r[15]+20);r[0]+=r[2];r[2]=w(r[15]+24);r[0]+=r[1];r[0]+=r[3];r[3]=w(r[15]+44);r[0]+=r[2];r[2]=w(r[15]+48);r[0]+=r[3];r[0]+=r[2];r[15]+=60;c.pr=pop();c.pc=c.pr;r[0]+=32;cycles+=20;}
 void frame_prefix(){auto&r=c.r;push(c.pr);r[15]-=24;r[3]=r[15]+12;put(m,r[15]+16,r[6]);put(m,r[15]+20,r[7]);push(r[3]);r[2]=r[15]+12;push(r[2]);r[7]=r[15]+12;r[6]=r[15]+8;c.pr=0x8ced0274;c.pc=0x8ced0256;cycles+=16;}
 void wrapper(){auto&r=c.r;push(c.pr);r[15]-=40;r[3]=r[15]+28;put(m,r[15]+32,r[6]);put(m,r[15]+36,r[7]);push(r[3]);r[2]=r[15]+28;push(r[2]);r[3]=r[15]+28;push(r[3]);r[2]=r[15]+28;push(r[2]);r[7]=r[15]+28;r[6]=r[15]+24;c.pr=0x8ced02e4;c.pc=0x8ced01bc;cycles+=22;phase(0);stream();phase(3);
  r[2]=r[15]+20;r[0]=64;push(r[2]);r[6]=w(r[0]+r[15]);r[7]=r[15]+20;r[5]=w(r[15]+56);r[4]=w(r[15]+52);c.pr=0x8ced02f8;c.pc=0x8ced0256;cycles+=11;phase(4);frame_prefix();c.pc=0x8ced0222;phase(5);dimensions();phase(6);frame_tail();phase(7);wrapper_tail();phase(8);
 }
};
