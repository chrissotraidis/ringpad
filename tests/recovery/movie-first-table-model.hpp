#pragma once
// Independent ordered SH-4 state model for EDAB72 and the EDAB14 first-call prefix.
struct FirstTableModel : RequestModel {
 std::function<void(unsigned,const FirstTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void half(unsigned a,unsigned value){m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;}
 void repeat(unsigned value,unsigned counter){auto&r=c.r;need(r[counter]<=64,"Unbounded inner table loop");for(;;){t(r[counter]==0);cycles+=2;if(c.sr&1)break;half(r[4],r[value]);--r[counter];r[4]+=2;cycles+=3;}}
 void table(){auto&r=c.r;
  push(r[14]);r[6]=16;push(r[12]);push(r[11]);r[11]=sw(0x8cedabca);r[4]=w(0x8cedabe4);push(r[10]);r[7]=r[11];push(r[9]);r[5]=r[6];cycles+=11;repeat(7,5);
  r[14]=sw(0x8cedabcc);r[5]=2;r[7]=r[5];cycles+=4;repeat(14,7);
  r[14]=r[11];r[7]=12;cycles+=3;repeat(14,7);
  r[14]=sw(0x8cedabce);r[7]=r[5];cycles+=3;repeat(14,7);
  r[14]=r[11];r[7]=r[6];cycles+=3;repeat(14,7);
  r[14]=sw(0x8cedacc2);r[12]=sw(0x8cedacc6);r[1]=sw(0x8cedacc8);r[7]=r[14];r[10]=sw(0x8cedacc4);cycles+=5;
  do{need(cycles<20000,"Unbounded table pairs");r[9]=r[7]|r[10];half(r[4],r[9]);r[9]=r[14]|r[12];r[4]+=2;r[7]-=16;half(r[4],r[9]);t(std::int32_t(r[7])>=std::int32_t(r[1]));r[4]+=2;r[14]-=16;cycles+=12;}while(c.sr&1);
  r[14]=sw(0x8cedacca);r[9]=sw(0x8cedacce);r[12]=sw(0x8cedacd0);r[7]=r[14];r[0]=sw(0x8cedaccc);cycles+=5;
  do{need(cycles<20000,"Unbounded two-word table loop");r[10]=r[7]|r[9];r[1]=r[5];cycles+=4;repeat(10,1);r[10]=r[14]|r[12];r[1]=r[5];cycles+=4;repeat(10,1);r[7]-=16;t(std::int32_t(r[7])>=std::int32_t(r[0]));r[14]-=16;cycles+=4;}while(c.sr&1);
  r[12]=sw(0x8cedacd2);r[14]=8;r[10]=sw(0x8cedacd6);r[0]=sw(0x8cedacd4);r[7]=r[12];cycles+=5;
  do{need(cycles<20000,"Unbounded eight-word table loop");r[9]=r[7]|r[0];r[1]=r[14];cycles+=4;repeat(9,1);r[9]=r[12]|r[10];r[1]=r[14];cycles+=4;repeat(9,1);r[2]=sw(0x8cedacd8);r[7]-=16;t(std::int32_t(r[7])>=std::int32_t(r[2]));r[12]-=16;cycles+=5;}while(c.sr&1);
  r[14]=sw(0x8cedacda);r[9]=sw(0x8cedacde);r[12]=sw(0x8cedace0);r[7]=r[14];r[0]=sw(0x8cedacdc);cycles+=5;
  do{need(cycles<20000,"Unbounded sixteen-word table loop");r[10]=r[7]|r[9];r[1]=r[6];cycles+=4;repeat(10,1);r[10]=r[14]|r[12];r[1]=r[6];cycles+=4;repeat(10,1);r[7]-=16;t(std::int32_t(r[7])>=std::int32_t(r[0]));r[14]-=16;cycles+=4;}while(c.sr&1);
  r[4]=w(0x8cedace4);r[7]=4;r[1]=r[11];r[14]=r[7];cycles+=5;repeat(1,14);
  r[10]=sw(0x8cedadd2);r[1]=112;r[11]=sw(0x8cedadd4);r[12]=96;r[14]=r[1];cycles+=5;
  do{r[9]=r[14]|r[11];half(r[4],r[9]);r[9]=r[1]|r[10];r[4]+=2;r[14]-=16;half(r[4],r[9]);t(std::int32_t(r[14])>=std::int32_t(r[12]));r[4]+=2;r[1]-=16;cycles+=12;}while(c.sr&1);
  r[0]=sw(0x8cedadd6);r[12]=80;r[10]=sw(0x8cedadd8);r[9]=64;r[14]=r[12];cycles+=5;
  do{r[11]=r[14]|r[10];r[1]=r[5];cycles+=4;repeat(11,1);r[11]=r[12]|r[0];r[1]=r[5];cycles+=4;repeat(11,1);r[14]-=16;t(std::int32_t(r[14])>=std::int32_t(r[9]));r[12]-=16;cycles+=4;}while(c.sr&1);
  r[10]=sw(0x8cedadda);r[1]=48;r[11]=sw(0x8cedaddc);r[5]=r[1];r[9]=32;cycles+=5;
  do{r[12]=r[5]|r[10];r[14]=r[7];cycles+=4;repeat(12,14);r[12]=r[1]|r[11];r[14]=r[7];cycles+=4;repeat(12,14);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[9]));r[1]-=16;cycles+=4;}while(c.sr&1);
  r[7]=sw(0x8cedadde);r[5]=r[6];cycles+=3;repeat(7,5);
  r[7]=sw(0x8cedade0);r[5]=r[6];cycles+=3;repeat(7,5);
  r[9]=pop();r[10]=pop();r[11]=pop();r[12]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
 }
 void connected(){push(c.pr);c.r[15]-=4;c.pr=0x8cedab1c;put(m,c.r[15],c.r[4]);c.pc=0x8cedab72;cycles+=5;phase(0);table();phase(1);c.pr=0x8cedab20;c.pc=0x8cedab1c;cycles+=3;phase(2);}
};
