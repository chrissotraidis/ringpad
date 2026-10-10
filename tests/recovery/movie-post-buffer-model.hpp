#pragma once
struct PostBufferModel : RequestModel {
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void at(unsigned offset,unsigned value){put(m,c.r[4]+offset,value);}
 void leaf(){auto&r=c.r;
  r[0]=sw(0x8ced82f4);r[3]=1;r[1]=w(0x8ced8324);at(r[0],r[3]);r[3]=w(r[1]);r[2]=w(0x8ced8320);r[5]=w(r[2]);at(16,r[3]);
  r[3]=w(0x8ced8328);r[0]=w(r[3]);r[0]-=16;at(20,r[0]);r[0]=w(0x8ced832c);r[2]=w(r[0]);r[2]-=32;at(24,r[2]);r[2]=w(0x8ced8330);r[1]=w(r[2]);r[1]-=32;at(28,r[1]);
  r[1]=w(0x8ced8334);r[3]=w(r[1]);r[1]=r[5];r[1]+=32;at(32,r[3]);r[3]=w(0x8ced8338);r[0]=w(r[3]);at(36,r[0]);r[0]=w(0x8ced833c);r[2]=w(r[0]);at(40,r[2]);at(44,r[1]);r[1]=r[5];r[1]+=96;at(48,r[5]);at(52,r[1]);
  r[1]=w(0x8ced8340);r[5]=sw(0x8ced82f6);r[2]=w(r[1]);r[5]+=r[4];r[6]=sw(0x8ced82fc);r[3]=r[2];at(56,r[2]);put(m,r[5],r[2]);
  r[2]=sw(0x8ced82f8);r[3]=sw(0x8ced82fa);r[2]+=r[4];r[0]=sw(0x8ced8300);r[3]+=r[4];put(m,r[5]+4,r[2]);r[2]=r[3];put(m,r[5]+8,r[3]);r[3]=sw(0x8ced82fe);r[2]+=r[6];put(m,r[5]+12,r[2]);
  r[2]=sw(0x8ced82f8);r[3]+=r[4];at(r[0],r[3]);r[0]+=4;r[3]=sw(0x8ced8302);r[2]+=r[4];r[6]+=r[2];r[3]+=r[4];at(r[0],r[6]);r[2]=sw(0x8ced8304);r[0]+=4;at(r[0],r[3]);r[0]+=4;r[3]=sw(0x8ced82f8);r[2]+=r[4];at(r[0],r[2]);r[0]+=4;r[2]=sw(0x8ced8306);r[3]+=r[4];at(r[0],r[3]);r[0]+=4;r[2]+=r[4];c.pc=c.pr;at(r[0],r[2]);cycles+=80;
 }
 void parent(){auto&r=c.r;r[4]=w(0x8ced8308);r[0]=112;push(r[14]);push(r[13]);push(r[12]);push(c.pr);r[13]=w(r[4]+r[0]);r[0]=116;r[12]=w(r[4]+r[0]);t(int(r[13])>0);r[14]=0;cycles+=12;
  if(c.sr&1){need(r[13]<=8,"Unbounded count fixture");do{r[3]=sw(0x8ced82f2);r[4]=r[12];c.pr=0x8ced8236;r[12]+=r[3];cycles+=5;leaf();++r[14];t(int(r[14])>=int(r[13]));cycles+=3;}while(!(c.sr&1));}
  c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void continuation(){auto&r=c.r;r[3]=w(0x8ced81a0);r[4]=r[14];c.pr=0x8ced8110;r[4]+=96;c.pc=0x8ced8108;cycles+=5;}
};
