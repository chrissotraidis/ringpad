#pragma once
struct BindNextModel : RequestModel {
 bool prefix(){auto&r=c.r;r[6]=r[0];t(r[6]==0);cycles+=3;
  if(c.sr&1){r[14]+=4;t(r[14]>=r[12]);r[13]+=4;cycles+=4;
   if(!(c.sr&1)){r[2]=w(r[14]);t(r[2]==0);cycles+=3;
    if(!(c.sr&1)){r[11]=w(r[13]);r[0]=r[10];r[7]=0;r[3]=w(r[11]+r[0]);r[5]=r[6]=0;c.pr=0x8ceda2e6;r[4]=0;c.pc=0x8ceda2d6;cycles+=9;return false;}
   }
  }
  c.pr=pop();r[0]=r[6];r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;return true;
 }
};
