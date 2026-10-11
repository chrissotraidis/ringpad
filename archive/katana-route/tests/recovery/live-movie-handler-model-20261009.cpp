// Retained source-derived MOVIE setters, allocation and reset prefix. No guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
struct HandlerPoolModel:RequestModel{
 bool full=false;unsigned slot=0;
 void pool(){auto&r=c.r;push(r[14]);r[0]=64;push(c.pr);r[14]=w(0x8c6621e4);r[7]=0;cycles+=5;
 for(;;){r[3]=r[7];t(r[3]>>31);r[3]<<=1;r[2]=r[7];r[3]+=r[2];r[3]<<=4;r[3]+=r[14];r[1]=w(r[3]+4);t(r[1]==0);cycles+=10;if(c.sr&1)break;++r[7];t(static_cast<int32_t>(r[7])>=static_cast<int32_t>(r[0]));cycles+=3;if(c.sr&1)break;}
 slot=r[7];r[0]=r[7];t(r[0]==64);cycles+=4;full=c.sr&1;
 if(full){c.pr=pop();r[0]=0;c.pc=c.pr;r[14]=pop();cycles+=5;return;}
 r[3]=r[7];t(r[7]>>31);r[7]<<=1;r[7]+=r[3];r[3]=w(0x8c6621ec);r[7]<<=4;r[2]=1;r[14]+=r[7];put(m,r[14]+4,r[2]);put(m,r[14],r[3]);r[3]=w(0x8c6621f4);r[2]=w(0x8c6621f0);put(m,r[14]+28,r[4]);put(m,r[14]+32,r[5]);put(m,r[14]+36,r[6]);put(m,r[14]+8,r[2]);put(m,r[14]+40,r[3]);put(m,r[14]+44,r[14]);c.pr=0x8c662162;r[4]=r[14];c.pc=0x8c66213a;cycles+=21;
 }
};

static unsigned rate(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
put(expected_ram,er[4]+56,er[5]);expected.pc=expected.pr;const auto setter_return=expected;er[2]=word(ram,0x8ced14b8);er[5]=0;expected.pr=0x8ced146c;er[4]=er[14];expected.pc=0x8ced1464;need(er[2]==0x8c648412,"Original next target differs");return 8;}

static unsigned flag(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
er[0]=109;expected_ram[(er[4]+er[0])&0xffffff]=std::uint8_t(er[5]);expected.pc=expected.pr;const auto setter_return=expected;er[0]=er[14];er[15]+=4;expected.pr=word(ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(ram,er[15]);er[15]+=4;need(expected.pc==0x8ced136a,"Original wrapper return differs");return 10;}

static unsigned next(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
er[13]=er[0];need(er[13]==0x8c6774f0,"Wrapper handle differs");expected.sr&=~1u;er[2]=word(ram,0x8ced1494);er[5]=word(ram,er[11]);er[6]=word(ram,er[11]+4);expected.pr=0x8ced13b2;er[4]=word(ram,er[11]+8);expected.pc=0x8ced13a8;need(er[2]==0x8c662106,"Original next target differs");return 9;}

static unsigned init(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=word(expected_ram,er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);expected.pc=expected.pr;put(expected_ram,er[4]+24,er[5]);auto init_return=expected;need(expected.pc==0x8c662162,"Initializer return differs");er[0]=er[14];expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pc==0x8ced13b2&&er[0]==0x8c8aa8c8,"Pool return differs");
  return 14;}

static unsigned bind(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
er[5]=er[0];need(er[5]==0x8c8aa8c8,"Pool handle differs");expected.sr&=~1u;er[3]=static_cast<int16_t>(word(ram,0x8ced1492));er[1]=word(ram,0x8ced14a0);er[4]=er[3]-124;er[4]+=er[14];put(expected_ram,er[12],er[13]);er[4]+=er[3];put(expected_ram,er[12]+4,er[5]);er[2]=word(ram,er[13]+12);er[4]+=8;put(expected_ram,er[4],er[2]);expected.pr=0x8ced13e4;er[4]=er[13];expected.pc=0x8ced13ca;need(er[1]==0x8c64816a,"Original setup target differs");return 17;}

static unsigned start(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
er[15]-=4;put(expected_ram,er[15],er[14]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;put(expected_ram,er[15],er[5]);er[14]=er[4];expected.pr=0x8c648178;er[4]=er[14];expected.pc=0x8c64816a;
  return 8;}

static unsigned reset(auto&expected_ram,SH4Context&expected){auto&er=expected.r;const auto&ram=expected_ram;
const unsigned nesting=word(expected_ram,0x8c67ff20);need(nesting==0,"Live reset nesting changed");
for(unsigned i=14;i>=11;--i){er[15]-=4;put(expected_ram,er[15],er[i]);}er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;er[3]=word(expected_ram,0x8c648360);expected.pr=0x8c6481ba;er[14]=er[4];need(er[3]==0x8c65eaa8,"Critical entry target differs");
  er[4]=word(expected_ram,0x8c65eaf8);er[3]=word(expected_ram,er[4]);expected.sr=(expected.sr&~1u)|unsigned(er[3]==0);if(!nesting){er[0]=expected.sr;er[3]=word(expected_ram,0x8c65eafc);er[2]=static_cast<int16_t>(word(expected_ram,0x8c65eaf4));er[0]=(er[0]>>4)&15;put(expected_ram,er[3],er[0]);er[0]=(expected.sr&er[2])|240;expected.sr=er[0];}er[3]=word(expected_ram,er[4])+1;put(expected_ram,er[4],er[3]);expected.pc=expected.pr;
  er[2]=word(expected_ram,0x8c648368);er[5]=0;expected.pr=0x8c6481c2;er[4]=word(expected_ram,er[14]+12);expected.pc=0x8c6481ba;need(er[2]==0x8c65f238,"Reset child target differs");const unsigned cycles=nesting?24:40;
  return cycles;}

int main(int argc,char**argv){try{
 need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8ced145c,"Wrong initial caller");unsigned cycles=0;
 auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
 cycles+=rate(m,c);save("rate");cycles+=flag(m,c);save("flag");cycles+=next(m,c);save("next");
 HandlerPoolModel pool{{c,m}};pool.pool();need(pool.cycles==79&&pool.slot==3&&!pool.full,"Live pool occupancy differs");c=pool.c;cycles+=pool.cycles;save("pool");
 cycles+=init(m,c);save("init");cycles+=bind(m,c);save("bind");cycles+=start(m,c);save("start");cycles+=reset(m,c);save("end");need(cycles==185,"Total cycles differ");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":185,"guest_execution":false,"next_target":"8C65F238"})"<<'\n';std::cout<<"PASS source model185cycles reaches65F238\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
