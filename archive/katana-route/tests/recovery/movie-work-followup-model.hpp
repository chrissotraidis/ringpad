#pragma once
struct WorkFollowupModel : RequestModel {
 unsigned prefs=0;
 std::function<void(unsigned,const WorkFollowupModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void words(){auto&r=c.r;const unsigned n=r[6];r[7]=15;push(r[12]);r[7]&=r[6];for(unsigned i=0;i<(n&15);++i){r[3]=w(r[5]);r[5]+=4;put(m,r[4],r[3]);r[4]+=4;}r[7]=0;r[6]>>=4;r[12]=r[6];
  for(unsigned block=0;block<(n>>4);++block){
   for(unsigned reg:{6u,7u,0u,1u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+4*i,r[std::array<unsigned,4>{6,7,0,1}[i]]);
   for(unsigned reg:{6u,7u,0u,1u}){r[reg]=w(r[5]);r[5]+=4;}put(m,r[4]+16,r[6]);r[3]=r[5]+32;put(m,r[4]+20,r[7]);put(m,r[4]+24,r[0]);put(m,r[4]+28,r[1]);++prefs;
   for(unsigned reg:{7u,0u,1u,6u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+32+4*i,r[std::array<unsigned,4>{7,0,1,6}[i]]);
   for(unsigned reg:{1u,0u,7u,6u}){r[reg]=w(r[5]);r[5]+=4;}for(unsigned i=0;i<4;++i)put(m,r[4]+48+4*i,r[std::array<unsigned,4>{1,0,7,6}[i]]);r[4]+=64;
  }t(true);c.pc=c.pr;r[12]=pop();cycles+=17+5*(n&15)+38*(n>>4);
 }
 void tail(bool stop=false){auto&r=c.r;r[4]=w(r[15]);t(r[4]==0);cycles+=3;
  if(!(c.sr&1)){r[5]=w(0x8ceda9a0);r[6]=8;r[15]+=4;r[2]=w(0x8ceda99c);c.pr=pop();cycles+=7;if(stop){c.pc=0x8ceda974;return;}need((r[2]&0x1fffffff)==0x0ced6708,"Unexpected tail copy");words();}
  else{r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=5;}
 }
 void helper(bool stop=false){auto&r=c.r;
  push(r[14]);r[7]=0;push(r[12]);r[1]=64;push(r[11]);r[5]=0;r[14]=w(0x8ceda98c);push(r[10]);r[10]=w(0x8ceda990);r[7]+=r[14];r[11]=w(0x8ceda994);r[6]=w(0x8ceda998);r[12]=w(0x8ceda988);cycles+=13;
  for(unsigned i=0;i<64;++i){r[3]=sb(r[6]);++r[6];r[0]=r[5];++r[5];r[3]+=r[14];t(int(r[5])>=int(r[1]));r[3]=sb(r[3]);wb(r[11]+r[0],r[3]);r[3]=sb(r[12]);++r[12];r[0]=sb(r[7]);++r[7];wb(r[10]+r[0],r[3]);cycles+=11;}
  r[10]=pop();r[11]=pop();r[12]=pop();r[14]=pop();cycles+=5;
  push(c.pr);r[15]-=4;put(m,r[15],r[4]);r[4]+=32;t(r[4]==0);cycles+=6;
  if(!(c.sr&1)){r[2]=w(0x8ceda99c);r[5]=w(0x8ceda994);c.pr=0x8ceda96e;r[6]=16;cycles+=5;if(stop){c.pc=0x8ceda966;return;}need((r[2]&0x1fffffff)==0x0ced6708,"Unexpected first copy");words();}
  tail(stop);
 }
 void connected(){helper();phase(0);need(c.pc==0x8ced80fc,"Unexpected followup return");c.r[3]=w(0x8ced819c);c.r[4]=sw(0x8ced817e);c.r[4]+=c.r[14];c.pr=0x8ced8104;c.pc=0x8ced80fc;cycles+=5;phase(1);}
};
