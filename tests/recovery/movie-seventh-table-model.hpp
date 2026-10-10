#pragma once
// Independent stack-local algorithm for EDB486; repeated groups are source-audited.
struct SeventhTableModel : RequestModel {
 std::function<void(unsigned,const SeventhTableModel&)> observe;
 void phase(unsigned i){if(observe)observe(i,*this);}
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void half(unsigned a,unsigned v){m[a&0xffffff]=v&255;m[(a+1)&0xffffff]=(v>>8)&255;}
 inline static const std::array<std::array<unsigned,2>,63> groups={{
  {0x8cedb562u,1u},
  {0x8cedb564u,1u},
  {0x8cedb566u,1u},
  {0x8cedb568u,1u},
  {0x8cedb56au,1u},
  {0x8cedb56cu,1u},
  {0x8cedb662u,2u},
  {0x8cedb664u,2u},
  {0x8cedb666u,2u},
  {0x8cedb668u,2u},
  {0x8cedb66au,2u},
  {0x8cedb66cu,2u},
  {0x8cedb66eu,2u},
  {0x8cedb75eu,2u},
  {0x8cedb760u,2u},
  {0x8cedb762u,2u},
  {0x8cedb764u,2u},
  {0x8cedb766u,2u},
  {0x8cedb768u,2u},
  {0x8cedb76au,2u},
  {0x8cedb85au,2u},
  {0x8cedb85cu,2u},
  {0x8cedb85eu,2u},
  {0x8cedb860u,2u},
  {0x8cedb862u,2u},
  {0x8cedb864u,2u},
  {0x8cedb866u,2u},
  {0x8cedb956u,2u},
  {0x8cedb958u,2u},
  {0x8cedb95au,2u},
  {0x8cedb95cu,2u},
  {0x8cedb95eu,2u},
  {0x8cedb960u,2u},
  {0x8cedb962u,2u},
  {0x8cedba52u,4u},
  {0x8cedba54u,4u},
  {0x8cedba56u,4u},
  {0x8cedba58u,4u},
  {0x8cedba5au,4u},
  {0x8cedba5cu,4u},
  {0x8cedba5eu,4u},
  {0x8cedbb4eu,4u},
  {0x8cedbb50u,8u},
  {0x8cedbb52u,8u},
  {0x8cedbb54u,8u},
  {0x8cedbb56u,8u},
  {0x8cedbb58u,16u},
  {0x8cedbb5au,16u},
  {0x8cedbc4au,16u},
  {0x8cedbc4cu,16u},
  {0x8cedbc4eu,16u},
  {0x8cedbc50u,16u},
  {0x8cedbc52u,16u},
  {0x8cedbc54u,16u},
  {0x8cedbc56u,16u},
  {0x8cedbd46u,16u},
  {0x8cedbd48u,16u},
  {0x8cedbd4au,16u},
  {0x8cedbd4cu,32u},
  {0x8cedbd4eu,32u},
  {0x8cedbd50u,32u},
  {0x8cedbd52u,32u},
  {0x8cedbe50u,64u},
 }};
 void body(bool save_counter){auto&r=c.r;r[1]=w(r[15]+8);r[1]+=2;put(m,r[15]+8,r[1]);r[0]=sw(r[15]+2);r[1]-=2;half(r[1],r[0]);r[3]=w(r[15]+4);++r[3];if(save_counter)put(m,r[15]+4,r[3]);cycles+=save_counter?8:7;}
 void table(){auto&r=c.r;
  r[15]-=12;r[3]=w(0x8cedb570);r[0]=0;put(m,r[15]+8,r[3]);half(r[15]+2,r[0]);r[3]=r[0];cycles+=7;
  for(;;){need(cycles<30000,"Unbounded initial stack loop");r[1]=r[3];r[2]=2;t(std::int32_t(r[1])>=std::int32_t(r[2]));put(m,r[15]+4,r[3]);cycles+=5;if(c.sr&1)break;body(false);}
  for(const auto&g:groups){r[0]=sw(g[0]);r[2]=0;half(r[15]+2,r[0]);put(m,r[15]+4,r[2]);cycles+=5;
   for(;;){need(cycles<30000,"Unbounded repeated stack loop");r[1]=w(r[15]+4);r[2]=g[1];t(std::int32_t(r[1])>=std::int32_t(r[2]));cycles+=4;if(c.sr&1)break;body(true);}
  }
  c.pc=c.pr;r[15]+=12;cycles+=3;
 }
 void connected(){table();phase(0);need(c.pc==0x8cedab34,"Unexpected seventh-table continuation");c.r[2]=w(0x8cedabd0);c.pr=0x8cedab3a;c.pc=0x8cedab34;cycles+=4;phase(1);}
};
