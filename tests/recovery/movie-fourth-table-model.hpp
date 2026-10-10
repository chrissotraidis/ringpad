#pragma once
// Independent ordered stores and live-literal reads for original EDB1FC.
struct FourthTableModel : RequestModel {
 std::function<void(unsigned,const FourthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void emit(unsigned value){auto a=c.r[4];m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;c.r[4]+=2;}
 void repeat(unsigned count){c.r[5]=count;for(;;){t(c.r[5]==0);cycles+=2;if(c.sr&1)break;emit(c.r[6]);--c.r[5];cycles+=3;}}
 void table(){auto&r=c.r;
  // R6 is loaded before the first store, including when output overlaps literals.
  r[5]=sw(0x8cedb25a);r[4]=w(0x8cedb268);r[6]=sw(0x8cedb262);emit(r[5]);
  for(unsigned a:{0x8cedb25c,0x8cedb25e,0x8cedb260}){r[5]=sw(a);emit(r[5]);}
  cycles+=16;repeat(4);
  r[6]=sw(0x8cedb264);cycles+=3;repeat(8);
  r[6]=sw(0x8cedb266);cycles+=3;repeat(16);
  c.pc=c.pr;cycles+=3;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab28,"Unexpected fourth-table continuation");c.pr=0x8cedab2c;c.pc=0x8cedab28;cycles+=3;phase(1);}
};
