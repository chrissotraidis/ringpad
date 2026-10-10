// Source-derived first reset segment; runs a host model, not guest code.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned forward(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;
expected.r[3]=word(ram,0x8c65f2e0);expected.pc=0x8c65f238;need(expected.r[3]==0x8c660fa6,"Original forwarding target differs");return 4;}
static unsigned reset(auto&expected_ram,SH4Context&expected){auto&er=expected.r;
need(er[4]==0x8c8a8c90 && er[5]==0 && word(expected_ram,0x8c67ff20)==1 && expected_ram[(er[4]&0xffffff)+5]==0,"Live idle reset precondition differs");
const bool alternate=er[5]!=0,flagged=(expected_ram[(er[4]&0xffffff)+1]&1)!=0;
  er[15]-=4;put(expected_ram,er[15],er[14]);er[15]-=4;put(expected_ram,er[15],er[13]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=8;er[14]=er[4];put(expected_ram,er[15]+4,er[5]);expected.sr&=~1u;er[0]=0;er[13]=0;
  er[4]=er[15];expected.pr=0x8c660ff6;
  // Original retained6607A0 stores the incoming IRQ mask and keeps IRQs masked.
  er[0]=(expected.sr>>4)&15;er[3]=static_cast<int16_t>(word(expected_ram,0x8c660896));put(expected_ram,er[4],er[0]);er[0]=(expected.sr&er[3])|240;expected.sr=er[0];
  er[2]=word(expected_ram,er[15]+4);expected.sr=(expected.sr&~1u)|unsigned(er[2]==0);
  if(alternate){er[0]=word(expected_ram,er[15]+4);expected.sr|=1u;expected.pr=0x8c661036;expected.pc=0x8c661032;}else{expected.pr=0x8c661000;expected.pc=0x8c660ffc;}er[4]=er[14];unsigned cycles=alternate?43:40;
  // Original status query: preserve PR on stack, return low bit of record byte1.
  er[15]-=4;put(expected_ram,er[15],expected.pr);expected.sr&=~1u;er[0]=expected_ram[(er[4]&0xffffff)+1];er[4]=er[0]&1;er[0]=er[4];expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;cycles+=12;
  expected.sr=(expected.sr&~1u)|unsigned(alternate?er[0]==1:er[0]==0);cycles+=2;
  if(!alternate&&flagged){er[3]=word(expected_ram,er[14]+52);put(expected_ram,er[14]+88,er[3]);put(expected_ram,er[14]+92,er[13]);put(expected_ram,er[14]+96,er[13]);er[0]=expected_ram[(er[14]&0xffffff)+1]&6;expected_ram[(er[14]&0xffffff)+1]=er[0];cycles+=11;}
  if(alternate&&!flagged){for(unsigned o:{52u,56u,60u,100u,104u,80u,84u,68u})put(expected_ram,er[14]+o,er[13]);expected_ram[(er[14]&0xffffff)+6]=0;expected_ram[(er[14]&0xffffff)+4]=0;er[3]=word(expected_ram,er[14]+52);put(expected_ram,er[14]+88,er[3]);put(expected_ram,er[14]+92,er[13]);er[0]=expected_ram[(er[14]&0xffffff)+1]|1;expected_ram[(er[14]&0xffffff)+1]=er[0];cycles+=25;}
  expected.pr=0x8c661070;er[4]=er[15];cycles+=3;
  // Original retained6607B8 restores saved IRQ mask, preserving other SR bits.
  er[0]=word(expected_ram,er[4]);er[2]=expected.sr;er[1]=static_cast<int16_t>(word(expected_ram,0x8c660896));er[0]=((er[0]&15)<<4)|(er[2]&er[1]);er[2]&=er[1];expected.sr=er[0];cycles+=16;
  er[15]+=8;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[13]=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;cycles+=6;return cycles;}
static unsigned tail(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;
expected.r[3]=word(ram,0x8c64836c);expected.r[5]=0;expected.r[4]=word(ram,expected.r[14]+12);expected.pr=0x8c6481ca;expected.pc=0x8c6481c2;need(expected.r[3]==0x8c65f23e,"Next reset target differs");return 5;}
int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c6481ba,"Wrong reset entry");unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=forward(m,c);save("forward");cycles+=reset(m,c);save("reset");cycles+=tail(m,c);save("end");need(cycles==88,"Predicted idle reset cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":88,"guest_execution":false,"next_target":"8C65F23E"})"<<'\n';std::cout<<"PASS first reset model88cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
