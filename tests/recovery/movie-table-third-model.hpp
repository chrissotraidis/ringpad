#pragma once
struct TableThirdModel : RequestModel {
 unsigned prefs=0;
 std::function<void(unsigned,const TableThirdModel&)> observe;
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
 void invoke(unsigned target,unsigned ret){need((target&0x1fffffff)==0x0ced6708,"Unexpected live copy callee");need(c.r[6]==64||c.r[6]==16,"Unexpected copy count");c.pr=ret;words();}
 void helper(){auto&r=c.r;
  push(c.pr);r[15]-=4;r[3]=sw(0x8cedc296);r[2]=r[4];r[1]=w(0x8cedc2e4);r[6]=64;r[2]-=r[3];r[5]=w(0x8cedc2e8);put(m,r[15],r[2]);put(m,r[1],r[2]);r[2]=r[1];r[1]=w(0x8cedc29c);r[4]=w(r[2]);cycles+=15;invoke(r[1],0x8cedc272);
  r[3]=w(r[15]);r[6]=16;r[2]=w(0x8cedc2ec);r[3]-=64;r[5]=w(0x8cedc2f0);put(m,r[15],r[3]);put(m,r[2],r[3]);r[3]=r[2];r[2]=w(0x8cedc29c);r[4]=w(r[3]);cycles+=12;invoke(r[2],0x8cedc288);
  r[0]=w(r[15]);r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=6;
 }
 void connected(){helper();phase(0);need(c.pc==0x8cedc0ca,"Unexpected third-copy continuation");c.pr=0x8cedc0ce;c.r[4]=c.r[0];c.pc=0x8cedc0ca;cycles+=3;phase(1);}
};
