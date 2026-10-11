#pragma once
struct TableFirstModel : RequestModel {
 unsigned prefs=0;
 std::function<void(unsigned,const TableFirstModel&)> observe;
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
 void invoke(unsigned ret){need((c.r[14]&0x1fffffff)==0x0ced6708,"Unexpected live copy callee");need(c.r[6]<=129,"Unbounded first-copy count");c.pr=ret;words();}
 void helper(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[3]=sw(0x8cedc292);r[2]=r[4];r[1]=w(0x8cedc298);r[2]-=r[3];r[6]=sw(0x8cedc294);r[5]=w(0x8cedc2a0);r[14]=w(0x8cedc29c);put(m,r[15],r[2]);put(m,r[1],r[2]);r[2]=r[1];r[4]=w(r[2]);cycles+=16;invoke(0x8cedc196);
  const unsigned slots[]={0x8cedc2a4,0x8cedc2ac,0x8cedc2b4,0x8cedc2bc,0x8cedc2c4,0x8cedc2cc};
  const unsigned sources[]={0x8cedc2a8,0x8cedc2b0,0x8cedc2b8,0x8cedc2c0,0x8cedc2c8,0x8cedc2d0};
  const unsigned returns[]={0x8cedc1aa,0x8cedc1be,0x8cedc1d2,0x8cedc1e6,0x8cedc1fa,0x8cedc20e};
  for(unsigned i=0;i<6;++i){unsigned cursor=i%2?2:3,slot=i%2?3:2;r[cursor]=w(r[15]);r[6]=i?8:4;r[slot]=w(slots[i]);r[cursor]-=i?32:16;r[5]=w(sources[i]);put(m,r[15],r[cursor]);put(m,r[slot],r[cursor]);r[cursor]=r[slot];r[4]=w(r[cursor]);cycles+=11;invoke(returns[i]);}
  r[0]=w(r[15]);r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void connected(){helper();phase(0);need(c.pc==0x8cedc0c2,"Unexpected first-copy continuation");c.pr=0x8cedc0c6;c.r[4]=c.r[0];c.pc=0x8cedc0c2;cycles+=3;phase(1);}
};
