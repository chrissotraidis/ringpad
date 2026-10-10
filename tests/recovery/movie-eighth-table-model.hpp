#pragma once
// Independent DT, byte-store and branch-delay model for original EDBD9C.
struct EighthTableModel : RequestModel {
 std::function<void(unsigned,const EighthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void repeat(unsigned value){auto&r=c.r;need(r[5]&&r[5]<=32,"Unbounded byte-table loop");do{--r[5];t(r[5]==0);m[r[4]&0xffffff]=r[value]&255;++r[4];cycles+=4;}while(!(c.sr&1));}
 void table(){auto&r=c.r;
  r[7]=32;r[4]=w(0x8cedbe54);r[5]=r[7];r[6]=18;cycles+=4;repeat(6);
  r[5]=r[7];r[6]=34;cycles+=2;repeat(6);
  r[6]=16;r[5]=r[6];r[7]=3;cycles+=3;repeat(7);
  r[5]=r[6];r[7]=51;cycles+=2;repeat(7);
  r[5]=r[6];r[7]=67;cycles+=2;repeat(7);
  r[5]=8;r[6]=84;cycles+=2;repeat(6);
  r[5]=4;r[6]=101;cycles+=2;repeat(6);
  r[7]=2;r[5]=r[7];r[6]=118;cycles+=3;repeat(6);
  r[5]=r[7];r[6]=unsigned(-121);cycles+=2;repeat(6);
  c.pc=c.pr;cycles+=3;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab3a,"Unexpected eighth-table continuation");c.r[3]=w(0x8cedabd4);c.pr=0x8cedab40;c.pc=0x8cedab3a;cycles+=4;phase(1);}
};
