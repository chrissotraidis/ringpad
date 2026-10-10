#pragma once
#include "movie-work-followup-model.hpp"
struct FinalSetupModel : WorkFollowupModel {
 void helper(){auto&r=c.r;push(r[14]);t(r[4]==0);push(r[13]);push(c.pr);r[14]=w(0x8ceda864);r[13]=w(0x8ceda860);cycles+=7;
  if(c.sr&1){r[4]=w(0x8ceda868);r[3]=0xffffffe0;r[4]&=r[3];cycles+=3;}
  put(m,r[14],r[4]);r[4]+=64;r[5]=w(0x8ceda86c);r[6]=16;r[3]=w(0x8ceda870);put(m,r[13],r[4]);c.pr=0x8ceda7e8;r[4]=w(r[14]);cycles+=9;need((r[3]&0x1fffffff)==0x0ced6708,"Unexpected first copy target");words();
  c.pr=pop();r[4]=w(r[13]);r[6]=16;r[3]=w(0x8ceda870);r[13]=pop();r[5]=w(0x8ceda874);r[14]=pop();cycles+=9;need((r[3]&0x1fffffff)==0x0ced6708,"Unexpected second copy target");words();
 }
 void parent_tail(){auto&r=c.r;r[0]=0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;}
};
