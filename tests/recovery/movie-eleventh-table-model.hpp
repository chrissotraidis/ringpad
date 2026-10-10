#pragma once
// Independent pipeline: load the next pointer pair before storing the previous.
struct EleventhTableModel : RequestModel {
 std::function<void(unsigned,const EleventhTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 void table(){auto&r=c.r;
  r[2]=w(0x8cedc0dc);r[3]=w(0x8cedc0d8);
  for(unsigned i=1;i<20;++i){unsigned dst=i%2?0:2,val=dst+1,prev=i%2?2:0;
   r[dst]=w(0x8cedc0dc+8*i);r[val]=w(0x8cedc0d8+8*i);
   put(m,r[prev],r[prev+1]);
  }
  c.pc=c.pr;put(m,r[0],r[1]);cycles+=62;
 }
 bool connected(){table();phase(0);auto&r=c.r;need(c.pc==0x8cedab4c,"Unexpected pointer-installer continuation");r[3]=w(r[15]);t(r[3]==0);cycles+=3;
  if(c.sr&1){r[15]+=4;c.pr=pop();c.pc=c.pr;cycles+=5;phase(1);return true;}
  r[4]=w(r[15]);r[15]+=4;r[3]=w(0x8cedabe0);c.pr=pop();c.pc=0x8cedab52;cycles+=6;phase(1);return false;
 }
};
