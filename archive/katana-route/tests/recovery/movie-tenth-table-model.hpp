#pragma once
// Independent ordered word stores and live literal reads for original EDBEA2.
struct TenthTableModel : RequestModel {
 std::function<void(unsigned,const TenthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void emit(unsigned value){put(m,c.r[4],value);c.r[4]+=4;}
 void repeat(unsigned value,unsigned count){auto&r=c.r;need(r[count]<=32,"Unbounded word-table loop");for(;;){t(r[count]==0);cycles+=2;if(c.sr&1)break;emit(r[value]);--r[count];cycles+=3;}}
 void table(){auto&r=c.r;
  r[4]=w(0x8cedbfe4);r[5]=4;r[7]=0;r[6]=r[5];cycles+=5;repeat(7,6);
  r[7]=w(0x8cedbfe8);r[6]=r[5];cycles+=3;repeat(7,6);
  r[0]=w(0x8cedbfec);r[6]=2;r[7]=r[6];cycles+=4;repeat(0,7);
  for(unsigned a:{0x8cedbff0,0x8cedbff4,0x8cedbff8}){r[0]=w(a);r[7]=r[6];cycles+=3;repeat(0,7);}
  for(unsigned a:{0x8cedbffc,0x8cedc000,0x8cedc004,0x8cedc008}){r[7]=w(a);r[6]=r[5];cycles+=3;repeat(7,6);}
  // R7 is fetched before any individual store: preserve this order under overlap.
  r[5]=w(0x8cedc00c);r[7]=w(0x8cedc028);emit(r[5]);
  r[5]=w(0x8cedc010);emit(r[5]);r[5]=w(0x8cedc014);emit(r[5]);--r[5];emit(r[5]);
  for(unsigned a:{0x8cedc018,0x8cedc01c,0x8cedc020,0x8cedc024}){r[5]=w(a);emit(r[5]);}
  r[5]=8;r[6]=r[5];cycles+=28;repeat(7,6);
  for(unsigned a:{0x8cedc02c,0x8cedc030}){r[7]=w(a);r[6]=r[5];cycles+=3;repeat(7,6);}
  r[7]=w(0x8cedc034);r[5]=16;r[6]=r[5];cycles+=4;repeat(7,6);
  r[7]=w(0x8cedc038);r[6]=r[5];cycles+=3;repeat(7,6);
  r[6]=w(0x8cedc03c);r[5]=32;cycles+=3;repeat(6,5);
  c.pc=c.pr;cycles+=3;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab46,"Unexpected tenth-table continuation");c.r[2]=w(0x8cedabdc);c.pr=0x8cedab4c;c.pc=0x8cedab46;cycles+=4;phase(1);}
};
