#pragma once
#include "movie-work-followup-model.hpp"
// Ordered byte stores; later literals are read only when their instructions run.
struct WorkTransformModel : WorkFollowupModel {
 unsigned sw2(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void table(){auto&r=c.r;r[0]=sw2(0x8cedaac4);r[6]=0;r[5]=w(0x8cedaad4);r[4]=r[0];cycles+=4;
  need(r[4]>0&&r[4]<=384,"Unbounded decrement fixture");
  do{--r[4];t(r[4]==0);wb(r[5],r[6]);++r[5];cycles+=4;}while(!(c.sr&1));
  r[7]=sw2(0x8cedaac2);r[4]=r[6];cycles+=2;unsigned iterations=0;
  do{wb(r[5],r[4]);++r[4];t(int(r[4])>=int(r[7]));++r[5];cycles+=5;need(++iterations<=256,"Unbounded ascending fixture");}while(!(c.sr&1));
  r[6]=sw2(0x8cedaac6);r[4]=r[0];cycles+=2;
  do{--r[4];t(r[4]==0);wb(r[5],r[6]);++r[5];cycles+=4;}while(!(c.sr&1));
  r[3]=w(0x8cedaad4);r[2]=w(0x8cedaadc);r[0]+=r[3];put(m,r[2],r[0]);c.pc=c.pr;cycles+=6;
 }
 void wrapper(){auto&r=c.r;push(r[14]);push(c.pr);c.pr=0x8ceda9ac;r[14]=r[4];cycles+=5;table();t(r[14]==0);cycles+=2;
  if(!(c.sr&1)){r[3]=w(0x8cedaad8);r[5]=w(0x8cedaad4);r[6]=sw2(0x8cedaac2);c.pr=0x8ceda9ba;r[4]=r[14];cycles+=6;need((r[3]&0x1fffffff)==0x0ced6708,"Unexpected copy target");words();r[2]=sw2(0x8cedaac4);r[3]=w(0x8cedaadc);r[14]+=r[2];put(m,r[3],r[14]);cycles+=4;}
  c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=4;
 }
};
