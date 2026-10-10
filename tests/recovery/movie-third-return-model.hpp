#pragma once
struct ThirdReturnModel : RequestModel {
 void prefix(){auto&r=c.r;r[4]=r[0];t(r[4]==0);need(c.sr&1,"Success-return model requires R0 zero");r[2]=w(0x8ced2654);r[6]=9;r[4]=w(0x8ced2638);c.pr=0x8ced259a;r[5]=0;cycles+=9;need((r[2]&0x1fffffff)==0x0ced67f8,"Unexpected fill target");
  unsigned end=r[4]+36;for(unsigned i=0;i<9;++i)put(m,end-4*(i+1),0);r[3]=36;r[6]=r[7]=r[0]=0;t(true);c.pc=c.pr;cycles+=46;
  c.pr=pop();c.pc=c.pr;r[0]=0;cycles+=4;
 }
};
