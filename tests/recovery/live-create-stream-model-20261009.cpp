// Source-derived remaining creation tail and original stream-size computation.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
struct SizeModel : RequestModel {
 std::function<void(unsigned,const SizeModel&)> observe;
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

 void phase(unsigned i){if(observe)observe(i,*this);}
 void shar(unsigned n){const auto v=c.r[n];t(v&1);c.r[n]=(v>>1)|(v&0x80000000);}
 void stream(){auto&r=c.r;push(r[14]);r[14]=r[5];push(c.pr);t(!(r[14]>>31));cycles+=5;if(!(c.sr&1)){r[14]+=7;++cycles;}
  shar(14);r[0]=r[4];shar(14);shar(14);t(r[0]==2);cycles+=6;
  if(c.sr&1){r[4]=0;put(m,r[6],r[14]);put(m,r[7],r[4]);r[3]=w(r[15]+8);put(m,r[3],r[14]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=11;}
  else{r[4]=0x800;r[1]=r[14];r[3]=w(0x8ced0320);r[1]+=r[4];c.pr=0x8ced01f6;r[0]=r[4];c.pc=r[3];cycles+=7;phase(1);divide();phase(2);
   r[2]=w(0x8ced0324);r[0]<<=8;r[4]=w(0x8ced032c);r[0]<<=2;t(r[0]>>31);r[0]<<=1;put(m,r[6],r[0]);put(m,r[7],r[2]);r[1]=w(r[6]);r[2]=w(0x8ced0328);r[3]=w(r[15]+8);r[1]+=r[2];put(m,r[3],r[1]);r[3]=w(r[15]+12);put(m,r[3],r[4]);r[3]=w(r[15]+16);put(m,r[3],r[4]);cycles+=16;}
  r[3]=w(r[15]+20);r[2]=w(0x8ced0330);put(m,r[3],r[2]);c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
 }
};

static unsigned create_tail(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;
need(expected.pc==0x8ced1b12&&expected.pr==0x8ced1b16&&er[4]==0x8cee2210,"Creation tail input differs");
er[3]=word(ram,0x8ced1c0c);put(expected_ram,er[3],er[4]);er[0]=64;er[3]=word(expected_ram,er[4]+64);need(er[3]==1,"Expected valid instance");er[0]=0;expected.sr|=1u;er[4]=static_cast<int16_t>(word(ram,0x8ced1bfa));er[5]=word(expected_ram,er[15]);er[6]=word(expected_ram,er[15]+4);er[4]+=er[14];expected.pr=0x8ced1b54;put(expected_ram,er[4],er[5]);put(expected_ram,er[4]+4,er[6]);er[0]=0;er[15]+=8;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pr==0x8ced05e0,"Bind saved return differs");er[4]=er[0];expected.sr|=1u;er[0]=er[14];er[15]+=100;expected.pr=word(expected_ram,er[15]);er[15]+=4;for(unsigned reg:{10u,11u,12u,13u,14u}){er[reg]=word(expected_ram,er[15]);er[15]+=4;}expected.pc=expected.pr;need(expected.pc==0x8cecf1b0&&er[0]==0x8cee2210,"Creation return differs");return 44;}
static unsigned outer_stream(auto&expected_ram,SH4Context&c){
SizeModel m{{c,expected_ram}};auto&er=m.c.r;er[15]+=16;er[10]=er[0];er[2]=er[10];m.t(er[2]==0);need(er[2]==0x8cee2210,"Real instance missing");put(expected_ram,er[14]+48,er[0]);for(unsigned reg:{2u,3u,2u,3u}){er[reg]=er[15]+20;m.push(er[reg]);}er[6]=er[15];er[5]=m.w(er[12]+4);er[7]=er[15];er[2]=m.w(0x8cecf2bc);er[7]+=20;er[6]+=16;m.c.pr=0x8cecf1e4;er[4]=m.w(er[12]);m.cycles=27;need(er[2]==0x8ced01bc,"Stream-size target differs");m.stream();need(m.cycles==148&&m.c.pc==0x8cecf1e4,"Stream return differs");er[15]+=16;er[2]=m.w(0x8cecf2c4);er[5]=er[12];er[3]=m.w(0x8cecf2c0);er[1]=m.w(0x8cecf2c8);er[6]=m.w(er[2]);er[7]=m.w(er[3]);m.c.pr=0x8cecf1f6;er[4]=er[14];m.c.pc=0x8cecf1e4 /* Retained emitter records block entry at unresolved dynamic call. */;m.cycles+=10;need(er[1]==0x8ced0620,"Next output setup target differs");c=m.c;return m.cycles;}
int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=create_tail(m,c);save("creation-return");cycles+=outer_stream(m,c);save("end");need(cycles==202,"Creation/stream cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":202,"guest_execution":false,"next_target":"8CED0620"})"<<'\n';std::cout<<"PASS creation tail44cycles plus outer stream158cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
