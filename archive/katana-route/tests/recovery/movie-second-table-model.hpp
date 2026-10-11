#pragma once
// Independent ordered state and timing model for original EDADAC.
struct SecondTableModel : RequestModel {
 std::function<void(unsigned,const SecondTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void half(unsigned a,unsigned value){m[a&0xffffff]=value&255;m[(a+1)&0xffffff]=(value>>8)&255;}
 void emit(unsigned value){half(c.r[4],value);c.r[4]+=2;}
 void repeat(unsigned value,unsigned counter){auto&r=c.r;need(r[counter]<=32,"Unbounded table repeat");for(;;){t(r[counter]==0);cycles+=2;if(c.sr&1)break;emit(r[value]);--r[counter];cycles+=3;}}
 void table(){auto&r=c.r;
  r[7]=sw(0x8cedade2);r[5]=8;r[4]=w(0x8cedade8);push(r[14]);r[14]=r[7];push(r[13]);r[6]=r[5];cycles+=8;repeat(14,6);
  r[6]=sw(0x8cedade4);r[14]=r[7];emit(r[6]);r[6]=6;cycles+=6;repeat(14,6);
  r[6]=sw(0x8cedaee2);r[14]=r[7];emit(r[6]);r[6]=r[5];cycles+=6;repeat(14,6);
  r[6]=sw(0x8cedaee6);r[5]=sw(0x8cedaee4);cycles+=2;
  do{need(cycles<10000,"Unbounded single-word table loop");r[3]=11|r[5];r[5]-=16;half(r[4],r[3]);t(std::int32_t(r[5])>=std::int32_t(r[6]));r[4]+=2;cycles+=7;}while(c.sr&1);
  r[6]=sw(0x8cedaeec);r[14]=sw(0x8cedaeea);r[5]=sw(0x8cedaee8);cycles+=3;
  do{need(cycles<10000,"Unbounded two-word table loop");r[2]=10|r[5];half(r[4],r[2]);r[2]=r[5];r[5]-=16;r[2]|=r[14];t(std::int32_t(r[5])>=std::int32_t(r[6]));r[4]+=2;emit(r[2]);cycles+=11;}while(c.sr&1);
  r[13]=sw(0x8cedaef4);r[14]=sw(0x8cedaef6);r[0]=sw(0x8cedaef2);r[5]=sw(0x8cedaeee);r[1]=sw(0x8cedaef0);cycles+=5;
  do{need(cycles<10000,"Unbounded eight-word table loop");r[3]=8|r[5];r[6]=r[5];emit(r[3]);r[3]=r[5]|r[13];emit(r[3]);r[6]|=r[1];emit(r[6]);emit(r[6]);r[6]=r[5]|r[0];for(unsigned i=0;i<3;++i)emit(r[6]);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[14]));emit(r[6]);cycles+=27;}while(c.sr&1);
  r[13]=sw(0x8cedaefe);r[14]=sw(0x8cedaf00);r[0]=sw(0x8cedaefc);r[5]=sw(0x8cedaef8);r[1]=sw(0x8cedaefa);cycles+=5;
  do{need(cycles<10000,"Unbounded sixteen-word table loop");r[6]=7|r[5];emit(r[6]);emit(r[6]);r[6]=r[5]|r[13];emit(r[6]);emit(r[6]);r[6]=r[5]|r[1];for(unsigned i=0;i<4;++i)emit(r[6]);r[6]=r[5]|r[0];for(unsigned i=0;i<7;++i)emit(r[6]);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[14]));emit(r[6]);cycles+=43;}while(c.sr&1);
  r[4]=w(0x8cedaf04);r[6]=r[7];r[5]=2;cycles+=4;repeat(6,5);
  r[6]=96;r[5]=112;cycles+=2;
  do{r[2]=5|r[5];r[5]-=16;half(r[4],r[2]);t(std::int32_t(r[5])>=std::int32_t(r[6]));r[4]+=2;cycles+=7;}while(c.sr&1);
  r[6]=sw(0x8cedaff8);r[7]=64;r[5]=80;cycles+=3;
  do{r[3]=4|r[5];half(r[4],r[3]);r[3]=r[5];r[5]-=16;r[3]|=r[6];t(std::int32_t(r[5])>=std::int32_t(r[7]));r[4]+=2;emit(r[3]);cycles+=11;}while(c.sr&1);
  r[7]=sw(0x8cedaffc);r[5]=48;r[13]=sw(0x8cedaffa);r[14]=32;cycles+=4;
  do{r[2]=3|r[5];r[6]=r[5];emit(r[2]);r[2]=r[5]|r[7];emit(r[2]);r[6]|=r[13];emit(r[6]);r[5]-=16;t(std::int32_t(r[5])>=std::int32_t(r[14]));emit(r[6]);cycles+=17;}while(c.sr&1);
  r[5]=17;emit(r[5]);emit(r[5]);r[5]=sw(0x8cedaffe);emit(r[5]);emit(r[5]);r[5]=sw(0x8cedb000);for(unsigned i=0;i<4;++i)emit(r[5]);r[5]=sw(0x8cedb002);for(unsigned i=0;i<7;++i)emit(r[5]);half(r[4],r[5]);r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=39;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab20,"Unexpected second-table continuation");c.pr=0x8cedab24;c.pc=0x8cedab20;cycles+=3;phase(1);}
};
