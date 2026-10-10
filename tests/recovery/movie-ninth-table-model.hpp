#pragma once
// Independent TST-before-loop byte stores and return-delay model for EDBE14.
struct NinthTableModel : RequestModel {
 std::function<void(unsigned,const NinthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void repeat(unsigned value){auto&r=c.r;need(r[5]<=32,"Unbounded byte-table loop");
  for(;;){t(r[5]==0);cycles+=2;if(c.sr&1)break;
   m[r[4]&0xffffff]=r[value]&255;--r[5];++r[4];cycles+=3;}
 }
 void table(){auto&r=c.r;
  r[7]=2;r[4]=w(0x8cedbe58);r[6]=32;r[0]=r[7];r[5]=r[6];cycles+=6;repeat(0);
  r[0]=18;r[5]=r[6];cycles+=3;repeat(0);
  r[0]=34;r[5]=r[6];cycles+=3;repeat(0);
  r[6]=51;r[5]=16;cycles+=3;repeat(6);
  r[6]=68;r[5]=8;cycles+=3;repeat(6);
  r[6]=85;r[5]=4;cycles+=3;repeat(6);
  r[6]=102;r[5]=r[7];cycles+=3;repeat(6);
  r[5]=119;m[r[4]&0xffffff]=r[5];r[5]=unsigned(-120);++r[4];
  c.pc=c.pr;m[r[4]&0xffffff]=r[5]&255;cycles+=7;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab40,"Unexpected ninth-table continuation");c.r[3]=w(0x8cedabd8);c.pr=0x8cedab46;c.pc=0x8cedab40;cycles+=4;phase(1);}
};
