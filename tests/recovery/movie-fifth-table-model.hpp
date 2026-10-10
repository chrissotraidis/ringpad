#pragma once
// Independent ordered stores and live-literal reads for original EDB26C.
struct FifthTableModel : RequestModel {
 std::function<void(unsigned,const FifthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void emit(unsigned value){auto a=c.r[4];m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;c.r[4]+=2;}
 void repeat(){c.r[5]=c.r[6];for(;;){t(c.r[5]==0);cycles+=2;if(c.sr&1)break;emit(c.r[7]);--c.r[5];cycles+=3;}}
 void table(){auto&r=c.r;
  r[5]=sw(0x8cedb35a);r[6]=2;r[4]=w(0x8cedb378);r[7]=sw(0x8cedb362);emit(r[5]);
  for(unsigned a:{0x8cedb35c,0x8cedb35e,0x8cedb360}){r[5]=sw(a);emit(r[5]);}
  cycles+=17;repeat();
  r[7]=sw(0x8cedb364);cycles+=3;repeat();
  // Each larger count is installed once, then reused by the paired loop.
  for(unsigned a:{0x8cedb366,0x8cedb36a,0x8cedb36e}){
   r[7]=sw(a);r[6]*=2;cycles+=4;repeat();
   r[7]=sw(a+2);cycles+=3;repeat();
  }
  c.pc=c.pr;cycles+=3;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab2c,"Unexpected fifth-table continuation");c.pr=0x8cedab30;c.pc=0x8cedab2c;cycles+=3;phase(1);}
};
