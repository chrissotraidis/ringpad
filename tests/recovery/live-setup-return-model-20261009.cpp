// Source-derived setup return and MOVIE state update; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned resident_handler_setter(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;put(expected_ram,expected.r[4]+8,expected.r[5]);expected.pc=expected.pr;need(expected.pc==0x8c648186,"Handler setter return differs");expected.r[2]=word(expected_ram,expected.r[15]);expected.r[3]=word(expected_ram,0x8c648350);put(expected_ram,expected.r[14]+20,expected.r[2]);expected.pr=0x8c648190;expected.r[4]=word(expected_ram,expected.r[14]+4);expected.pc=0x8c648186;need(expected.r[3]==0x8c65f4b8,"Next channel start differs");
  return 9;}
static unsigned resident_channel_enable(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;
  er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;expected.pr=0x8c65f4c0;put(expected_ram,er[15],er[4]);
  for(unsigned o:{44u,48u,52u,64u,68u})put(expected_ram,er[4]+o,0);er[3]=word(expected_ram,0x8c65f450);need(er[3]==0x7fffffff,"Initializer sentinel differs");put(expected_ram,er[4]+56,er[3]);put(expected_ram,er[4]+60,0xffffffff);expected_ram[(er[4]+3)&0xffffff]=0;er[0]=0;er[5]=0;er[2]=0xffffffff;
  er[2]=word(expected_ram,er[15]);er[0]=1;expected_ram[(er[2]+1)&0xffffff]=1;er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;const auto channel_return=expected;need(expected.pc==0x8c648190,"Channel start return differs");
  er[0]=1;expected_ram[(er[14]+1)&0xffffff]=1;er[0]=3;expected_ram[(er[14]+2)&0xffffff]=3;er[0]=76;er[3]=0;put(expected_ram,er[14]+76,0);er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[2]=word(expected_ram,0x8c648364);need(er[2]==0x8c65eace,"Critical exit target differs");er[14]=word(expected_ram,er[15]);er[15]+=4;
  er[4]=word(expected_ram,0x8c65eaf8);need(word(expected_ram,er[4])==1,"Captured setup nesting differs");er[3]=0;er[2]=0;expected.sr|=1u;put(expected_ram,er[4],0);er[2]=word(expected_ram,0x8c65eafc);er[1]=static_cast<int16_t>(word(expected_ram,0x8c65eaf4));er[0]=word(expected_ram,er[2]);er[2]=expected.sr;er[0]=((er[0]&15)<<4)|(er[2]&er[1]);er[2]&=er[1];expected.sr=er[0];expected.pc=expected.pr;need(expected.pc==0x8ced13e4,"MOVIE setup return differs");
  return 66;}
static unsigned movie_after_setup(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[5]=1;expected.pr=0x8ced13ea;expected.r[4]=expected.r[14];expected.pc=0x8ced13e4;return 4;}
static unsigned movie_status_toggle(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;const unsigned route=expected.r[5]?0:(word(expected_ram,expected.r[4]+72)==1?1:2);need(route==0,"Live status route differs");auto&er=expected.r;er[15]-=4;er[6]=static_cast<int16_t>(word(expected_ram,0x8ced16b8));expected.sr=(expected.sr&~1u)|unsigned(er[5]==0);er[3]=static_cast<int16_t>(word(expected_ram,0x8ced16b6));er[6]+=er[4]+er[3]+8;er[2]=word(expected_ram,er[6]);put(expected_ram,er[15],er[2]);put(expected_ram,er[6]+44,er[5]);unsigned cycles=11;
  if(route){er[0]=word(expected_ram,er[4]+72);expected.sr=(expected.sr&~1u)|unsigned(er[0]==1);cycles+=4;}
  if(route==1){expected.pc=expected.pr;er[15]+=4;cycles+=3;}else{er[4]=word(expected_ram,er[15]);er[15]+=4;er[3]=word(expected_ram,0x8ced16c0);need(er[3]==0x8c648548,"Toggle forwarding target differs");expected.pc=0x8ced1666;cycles+=7;}
  return cycles;}

int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c64817e,"Wrong setup entry");unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=resident_handler_setter(m,c);save("resident-handler-setter");
cycles+=resident_channel_enable(m,c);save("resident-channel-enable");
cycles+=movie_after_setup(m,c);save("movie-after-setup");
cycles+=movie_status_toggle(m,c);save("end");
need(cycles==97,"Setup return cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":97,"guest_execution":false,"next_target":"8C648548"})"<<'\n';std::cout<<"PASS setup return97cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
