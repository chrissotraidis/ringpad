#pragma once
// The argument table is loaded before four pushes; count/buffer are loaded after.
struct PostWorkModel : RequestModel {
 bool prefix(){auto&r=c.r;c.pc=0x8ced8216;r[4]=w(0x8ced8308);r[0]=112;push(r[14]);push(r[13]);push(r[12]);push(c.pr);r[13]=w(r[4]+r[0]);r[0]=116;r[12]=w(r[4]+r[0]);t(int(r[13])>0);r[14]=0;cycles+=12;
  if(c.sr&1){r[3]=unsigned(int(std::int16_t(w(0x8ced82f2)&0xffff)));r[4]=r[12];c.pr=0x8ced8236;r[12]+=r[3];c.pc=0x8ced822e;cycles+=5;return false;}
  c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=6;return true;
 }
};
