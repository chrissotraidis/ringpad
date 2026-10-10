// Retained source-derived query and resident return model; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
struct BufferQueryModel:RequestModel{
 void query(){auto&r=c.r;push(r[14]);push(r[13]);r[3]=w(0x8c65e6f0);push(r[12]);push(r[11]);push(r[10]);push(r[9]);r[9]=0;r[13]=w(r[3]);t(r[13]==0);r[12]=r[9];need(!(c.sr&1),"Captured query global absent");cycles+=12;
 t(r[6]==0);r[0]=c.sr&1;t(r[7]==0);r[1]=c.sr&1;r[0]|=r[1];t(r[0]==0);need(c.sr&1,"Query output absent");cycles+=7;
 r[14]=static_cast<int8_t>(r[5]);t(static_cast<int32_t>(r[14])>=0);cycles+=3;bool invalid=!(c.sr&1);
 if(!invalid){r[1]=w(0x8c65e6fc);r[11]=static_cast<int8_t>(r[5]);r[0]=r[4];r[11]<<=2;t(r[11]>>31);r[11]<<=1;t(r[0]==r[1]);r[10]=16;cycles+=8;need(!(c.sr&1),"Only reached SPRS type modeled");
 for(unsigned literal:{0x8c65e700u,0x8c65e704u,0x8c65e708u}){r[1]=w(literal);t(r[0]==r[1]);cycles+=3;need(bool(c.sr&1)==(literal==0x8c65e708),"Unexpected query type");}
 t(static_cast<int32_t>(r[14])>=static_cast<int32_t>(r[10]));cycles+=2;invalid=c.sr&1;
 if(!invalid){r[4]=static_cast<int16_t>(w(0x8c65e6ea));cycles+=1;r[4]+=r[13];r[4]+=r[11];cycles+=3;t(r[4]==0);need(!(c.sr&1),"Query table absent");cycles+=2;r[3]=w(r[4]);put(m,r[6],r[3]);r[2]=w(r[4]+4);put(m,r[7],r[2]);cycles+=5;}}
 if(invalid){r[12]=w(0x8c65e768);cycles+=3;}
 r[9]=pop();r[0]=r[12];r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }
};
struct QueryReturnModel:BufferQueryModel{
 void adapter_prefix(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=16;r[14]=r[4];r[3]=r[14]+16;r[2]=w(0x8c66c438);wb(r[15],r[5]);put(m,r[15]+4,r[6]);put(m,r[15]+8,r[7]);r[4]=sb(r[15]);r[4]<<=2;put(m,r[15]+12,r[4]);r[4]+=r[3];c.pr=0x8c66c39e;r[4]=w(r[4]);need(r[2]==0x8c66e930,"Original getter target differs");cycles+=18;
 r[0]=w(r[4]+4);c.pc=c.pr;cycles+=3;
 r[4]=w(0x8c66c43c);r[3]=w(0x8c66c440);put(m,r[15],r[0]);r[6]=w(r[15]+4);r[7]=w(r[15]+8);c.pr=0x8c66c3ac;r[5]=w(r[15]);c.pc=0x8c66c39e;cycles+=8;need(r[3]==0x8c65e640&&r[4]==0x52535053,"Original query target differs");
 }
 void rot(unsigned n){const unsigned before=c.r[n],bit=c.sr&1;c.r[n]=(before<<1)|bit;t(before>>31);}
 void div0s(unsigned rm,unsigned rn){const unsigned q=c.r[rn]>>31,sign=c.r[rm]>>31;c.sr=(c.sr&~0x301u)|(q<<8)|(sign<<9)|(q^sign);}
 // Hitachi SH-4 Programming Manual rev5, section9.19: use a widened
 // add/subtract to derive carry/borrow independently of emitted DIV1 code.
 void step(){const bool q=c.sr&0x100,sign=c.sr&0x200,top=c.r[3]>>31;const unsigned shifted=(c.r[3]<<1)|(c.sr&1);
  const std::int64_t wide=std::int64_t(shifted)+(q==sign?-std::int64_t(c.r[0]):std::int64_t(c.r[0]));
  const bool carry=wide<0||wide>0xffffffffll,newq=top^carry^sign;c.r[3]=unsigned(wide);c.sr=(c.sr&~0x101u)|(unsigned(newq)<<8)|unsigned(newq==sign);
 }
 void divide(){auto&r=c.r;t(r[0]==0);push(r[2]);
  if(c.sr&1){r[1]=w(0x8c10e728);r[2]=w(0x8c10e72c);r[0]=0;put(m,r[1],r[2]);c.pc=c.pr;r[2]=pop();cycles+=10;return;}
  push(r[3]);r[2]=0;div0s(2,1);const unsigned borrow=c.sr&1;r[3]=0u-borrow;t(borrow);const std::uint64_t sub=std::uint64_t(r[2])+(c.sr&1);const unsigned old=r[1];r[1]=unsigned(std::uint64_t(old)-sub);t(std::uint64_t(old)<sub);div0s(0,3);
  for(unsigned i=0;i<32;++i){rot(1);step();}rot(1);const std::uint64_t sum=std::uint64_t(r[1])+r[2]+(c.sr&1);r[1]=unsigned(sum);t(sum>>32);r[0]=r[1];r[3]=pop();c.pc=c.pr;r[2]=pop();cycles+=80;
 }
 SH4Context adapter_return{};
 void adapter_tail(){auto&r=c.r;r[2]=r[14];r[1]=w(r[15]+4);r[3]=w(r[15]+12);r[2]+=48;r[1]=w(r[1]);r[13]=r[0];r[2]+=r[3];put(m,r[2],r[1]);t(r[13]==0);r[3]=w(r[15]+8);r[2]=w(r[3]);put(m,r[14]+56,r[2]);need(c.sr&1,"Query failed");cycles+=13;r[0]=r[13];r[15]+=16;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;need(c.pc==0x8c660a2e,"Query adapter return differs");}
 void loop_tail(){auto&r=c.r;r[2]=r[14];r[3]=r[12];r[1]=w(r[15]);r[2]+=40;r[3]<<=2;++r[12];r[3]+=r[2];t(static_cast<int32_t>(r[12])>=static_cast<int32_t>(r[13]));put(m,r[3],r[1]);cycles+=10;}
 void complete(){auto&r=c.r;adapter_tail();adapter_return=c;loop_tail();need(!(c.sr&1)&&r[12]==1,"Second query not required");
 r[7]=r[15];r[5]=r[12];r[0]=64;r[6]=r[15];r[7]+=4;c.pr=0x8c660a2e;r[4]=w(r[14]+64);cycles+=8;adapter_prefix();query();adapter_tail();loop_tail();need((c.sr&1)&&r[12]==2,"Queries incomplete");
 r[2]=w(0x8c660aac);r[1]=w(r[15]+4);c.pr=0x8c660a4a;r[0]=w(r[14]+8);need(r[2]==0x8c10e67c&&r[0]==16&&r[1]==0x4000,"Original divide inputs differ");cycles+=5;divide();need(r[0]==0x400,"Record quotient differs");
 r[0]<<=2;r[3]=1;t(r[0]>>31);r[0]<<=1;put(m,r[14]+48,r[0]);wb(r[14],r[3]);cycles+=5;r[4]=r[15]+8;c.pr=0x8c660a5a;cycles+=4;
 r[0]=w(r[4]);r[2]=c.sr;r[1]=static_cast<int16_t>(w(0x8c660896));r[0]&=15;r[0]<<=2;r[2]&=r[1];r[0]<<=2;r[0]|=r[2];c.sr=r[0];c.pc=c.pr;cycles+=16;
 r[0]=r[14];r[15]+=12;c.pr=pop();for(unsigned i=10;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;need(c.pc==0x8c647f84&&r[0]==0x8c8a8c90&&cycles==278,"Outer constructor return differs");
 }
};
struct ResidentReturnModel:RequestModel{
 void half(unsigned a,unsigned v){wb(a,v);wb(a+1,v>>8);}
 void finish(){auto&r=c.r;t(r[0]==0);put(m,r[14]+12,r[0]);need(!(c.sr&1),"Outer constructor returned null");cycles+=3;r[3]=60;put(m,r[14]+56,r[3]);r[2]=w(r[14]+36);t(static_cast<int32_t>(r[2])>=0);cycles+=5;
 if(!(c.sr&1)){r[3]=static_cast<int16_t>(w(0x8c6480dc));r[2]+=r[3];cycles+=2;}
 r[3]=0xfffffff5;r[4]=r[14];r[0]=60;t(static_cast<int32_t>(r[12])>0);r[2]=unsigned(static_cast<int32_t>(r[2])>>11);r[5]=r[13];half(r[14]+r[0],r[2]);r[3]=static_cast<int16_t>(w(r[14]+r[0]));r[0]=0x8c6480e0;c.fr_bits[2]=w(r[0]);r[0]=62;c.fpul=r[3];need(r[3]==0xffffffff||r[3]==0||r[3]==1,"Only exact unit/zero scalar path modeled");need(c.fr_bits[2]==0x3f599999,"Original coefficient differs");c.fr_bits[3]=r[3]==0?0:r[3]==1?0x3f599999:0xbf599999;c.fpul=0;r[3]=c.fpul;half(r[14]+r[0],r[3]);r[0]=64;half(r[14]+r[0],r[13]);r[4]+=66;cycles+=21;
 need((c.sr&1)&&r[12]==2&&r[13]==0,"Captured stereo loop differs");r[6]=0xffffff80;++cycles;for(unsigned i=0;i<2;++i){++r[5];half(r[4],r[6]);t(static_cast<int32_t>(r[5])>=static_cast<int32_t>(r[12]));r[4]+=2;cycles+=5;}
 r[0]=108;r[4]=1;wb(r[14]+r[0],r[4]);for(unsigned offset:{84u,88u,92u}){r[0]=offset;put(m,r[14]+r[0],r[13]);}r[0]=96;half(r[14]+r[0],r[13]);r[0]=100;put(m,r[14]+r[0],r[13]);for(unsigned offset:{104u,106u}){r[0]=offset;half(r[14]+r[0],r[13]);}r[0]=109;wb(r[14]+r[0],r[4]);r[0]=114;wb(r[14]+r[0],r[13]);r[0]=r[14];wb(r[14],r[4]);cycles+=23;
 r[15]+=4;c.pr=pop();for(unsigned i=8;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=11;need(c.pc==0x8ced144c&&r[0]==0x8c6774f0,"Resident constructor return differs");
 }
};

int main(int argc,char**argv){try{
 need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c660a20,"Wrong initial caller");unsigned cycles=0;
 auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
 QueryReturnModel prefix{{{c,m}}};prefix.adapter_prefix();need(prefix.cycles==29,"Query prefix cycles differ");c=prefix.c;cycles+=prefix.cycles;save("prefix");
 BufferQueryModel query{{c,m}};query.query();need(query.cycles==61,"Buffer query cycles differ");c=query.c;cycles+=query.cycles;save("query");
 QueryReturnModel complete{{{c,m}}};complete.complete();need(complete.cycles==278,"Query return cycles differ");c=complete.c;cycles+=complete.cycles;save("outer-return");
 ResidentReturnModel finish{{c,m}};finish.finish();need(finish.cycles==76,"Resident return cycles differ");c=finish.c;cycles+=finish.cycles;save("end");need(cycles==444,"Total cycles differ");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":444,"guest_execution":false,"return_pc":"8CED144C"})"<<'\n';std::cout<<"PASS source model444cycles returns ED144C\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
