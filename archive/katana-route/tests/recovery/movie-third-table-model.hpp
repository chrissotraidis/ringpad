#pragma once
// Independent ordered state and timing model for original EDAFB6.
struct ThirdTableModel : RequestModel {
 std::function<void(unsigned,const ThirdTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void half(unsigned a,unsigned value){m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;}
 void emit(unsigned value){half(c.r[4],value);c.r[4]+=2;}
 void repeat(unsigned value,unsigned counter){auto&r=c.r;need(r[counter]<=32,"Unbounded table repeat");for(;;){t(r[counter]==0);cycles+=2;if(c.sr&1)break;emit(r[value]);--r[counter];cycles+=3;}}
 void table(){auto&r=c.r;
  push(r[14]);r[5]=8;push(r[13]);r[7]=sw(0x8cedb004);push(r[12]);r[4]=w(0x8cedb00c);r[14]=r[7];push(r[11]);push(r[10]);r[6]=r[5];cycles+=11;repeat(14,6);
  r[6]=sw(0x8cedb006);r[14]=r[7];emit(r[6]);r[6]=6;cycles+=6;repeat(14,6);
  r[6]=sw(0x8cedb008);r[14]=r[7];emit(r[6]);r[6]=r[5];cycles+=6;repeat(14,6);
  r[6]=sw(0x8cedb10e);r[5]=sw(0x8cedb10c);cycles+=2;
  do{need(cycles<10000,"Unbounded single-word table loop");r[3]=11|r[5];r[5]-=16;half(r[4],r[3]);t(std::int32_t(r[5])>=std::int32_t(r[6]));r[4]+=2;cycles+=7;}while(c.sr&1);
  r[6]=sw(0x8cedb110);r[14]=sw(0x8cedb112);r[5]=r[6];cycles+=3;
  do{need(cycles<10000,"Unbounded two-word table loop");r[2]=10|r[5];half(r[4],r[2]);r[2]=10;r[5]-=16;r[2]|=r[6];t(std::int32_t(r[5])>=std::int32_t(r[14]));r[4]+=2;half(r[4],r[2]);r[6]-=16;r[4]+=2;cycles+=12;}while(c.sr&1);
  r[13]=sw(0x8cedb11a);r[14]=sw(0x8cedb11c);r[6]=sw(0x8cedb11e);r[5]=sw(0x8cedb114);r[11]=sw(0x8cedb116);r[12]=sw(0x8cedb118);cycles+=6;
  do{need(cycles<10000,"Unbounded eight-word table loop");r[10]=8|r[5];r[3]=r[5];emit(r[10]);emit(r[10]);r[3]|=r[12];r[10]=r[5];emit(r[3]);r[3]=r[5]|r[13];emit(r[3]);r[10]|=r[11];emit(r[10]);emit(r[10]);r[10]=r[5]|r[6];emit(r[10]);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[14]));emit(r[10]);cycles+=29;}while(c.sr&1);
  r[5]=sw(0x8cedb120);r[12]=sw(0x8cedb128);r[11]=sw(0x8cedb126);r[1]=r[5];r[14]=sw(0x8cedb12c);r[1]-=16;r[13]=sw(0x8cedb12a);r[0]=sw(0x8cedb122);r[10]=sw(0x8cedb124);cycles+=9;
  do{need(cycles<10000,"Unbounded sixteen-word table loop");r[6]=7|r[5];r[2]=r[5];emit(r[6]);emit(r[6]);r[2]|=r[0];r[6]=r[5];emit(r[2]);r[2]=r[5]|r[14];emit(r[2]);r[6]|=r[10];emit(r[6]);emit(r[6]);r[6]=r[5]|r[13];emit(r[6]);emit(r[6]);r[6]=r[5]|r[11];for(unsigned i=0;i<4;++i)emit(r[6]);r[6]=r[5]|r[12];for(unsigned i=0;i<3;++i)emit(r[6]);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[1]));emit(r[6]);cycles+=49;}while(c.sr&1);
  r[4]=w(0x8cedb130);r[6]=r[7];r[5]=2;cycles+=4;repeat(6,5);
  r[6]=96;r[5]=112;cycles+=2;
  do{r[2]=5|r[5];r[5]-=16;half(r[4],r[2]);t(std::int32_t(r[5])>=std::int32_t(r[6]));r[4]+=2;cycles+=7;}while(c.sr&1);
  r[7]=80;r[5]=r[7];r[6]=r[7];r[7]=64;cycles+=4;
  do{r[3]=4|r[5];half(r[4],r[3]);r[3]=4;r[5]-=16;r[3]|=r[6];t(std::int32_t(r[5])>=std::int32_t(r[7]));r[4]+=2;half(r[4],r[3]);r[6]-=16;r[4]+=2;cycles+=12;}while(c.sr&1);
  r[7]=sw(0x8cedb24a);r[5]=48;r[6]=sw(0x8cedb24c);r[14]=32;cycles+=4;
  do{r[13]=3|r[5];r[2]=r[5];emit(r[13]);emit(r[13]);r[2]|=r[7];emit(r[2]);r[2]=r[5];r[5]-=16;r[2]|=r[6];t(std::int32_t(r[5])>=std::int32_t(r[14]));emit(r[2]);cycles+=17;}while(c.sr&1);
  r[5]=17;r[3]=sw(0x8cedb24e);emit(r[5]);emit(r[5]);emit(r[3]);r[3]=sw(0x8cedb250);r[5]=sw(0x8cedb252);emit(r[3]);emit(r[5]);emit(r[5]);r[5]=sw(0x8cedb254);emit(r[5]);emit(r[5]);r[5]=sw(0x8cedb256);for(unsigned i=0;i<4;++i)emit(r[5]);r[5]=sw(0x8cedb258);for(unsigned i=0;i<3;++i)emit(r[5]);half(r[4],r[5]);r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=45;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab24,"Unexpected third-table continuation");c.pr=0x8cedab28;c.pc=0x8cedab24;cycles+=3;phase(1);}
};
