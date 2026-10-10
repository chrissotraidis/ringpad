#pragma once
// Independent ordered stores, byte extension and nested counters for EDB312.
struct SixthTableModel : RequestModel {
 std::function<void(unsigned,const SixthTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void emit(unsigned value){auto a=c.r[4];m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;c.r[4]+=2;}
 void repeat(unsigned value,unsigned counter){auto&r=c.r;need(r[counter]<=32,"Unbounded sixth-table repeat");for(;;){t(r[counter]==0);cycles+=2;if(c.sr&1)break;emit(r[value]);--r[counter];cycles+=3;}}
 void table(){auto&r=c.r;
  push(r[14]);r[6]=127;r[4]=w(0x8cedb37c);push(r[12]);push(r[11]);r[5]=24;cycles+=7;repeat(6,5);
  r[6]=sw(0x8cedb372);r[12]=16;r[7]=11;r[5]=r[12];cycles+=4;
  do{r[14]=(r[5]&255)|r[6];emit(r[14]);r[14]=(0u-r[5])&255;--r[5];r[14]|=r[6];t(std::int32_t(r[5])>=std::int32_t(r[7]));emit(r[14]);cycles+=12;}while(c.sr&1);
  r[14]=sw(0x8cedb374);r[5]=8;r[6]=2;r[7]=10;cycles+=4;
  do{r[11]=(r[7]&255)|r[14];r[1]=r[6];cycles+=4;repeat(11,1);r[11]=((0u-r[7])&255)|r[14];r[1]=r[6];cycles+=5;repeat(11,1);--r[7];t(std::int32_t(r[7])>=std::int32_t(r[5]));cycles+=3;}while(c.sr&1);
  r[1]=sw(0x8cedb44c);r[0]=5;r[7]=7;cycles+=3;
  do{r[11]=(r[7]&255)|r[1];r[14]=r[5];cycles+=4;repeat(11,14);r[11]=((0u-r[7])&255)|r[1];r[14]=r[5];cycles+=5;repeat(11,14);--r[7];t(std::int32_t(r[7])>=std::int32_t(r[0]));cycles+=3;}while(c.sr&1);
  for(unsigned a:{0x8cedb44e,0x8cedb450}){r[7]=sw(a);r[5]=r[12];cycles+=3;repeat(7,5);}
  r[4]=w(0x8cedb460);r[7]=127;r[5]=r[6];cycles+=4;repeat(7,5);
  r[5]=sw(0x8cedb452);r[7]=sw(0x8cedb456);emit(r[5]);r[5]=sw(0x8cedb454);emit(r[5]);r[5]=r[6];cycles+=9;repeat(7,5);
  r[7]=sw(0x8cedb458);r[5]=r[6];cycles+=3;repeat(7,5);
  r[6]=sw(0x8cedb45a);r[7]=4;r[5]=r[7];cycles+=4;repeat(6,5);
  r[6]=sw(0x8cedb45c);r[5]=r[7];cycles+=3;repeat(6,5);
  r[6]=sw(0x8cedb560);r[5]=r[12];cycles+=3;repeat(6,5);
  r[11]=pop();r[12]=pop();c.pc=c.pr;r[14]=pop();cycles+=5;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab30,"Unexpected sixth-table continuation");c.pr=0x8cedab34;c.pc=0x8cedab30;cycles+=3;phase(1);}
};
