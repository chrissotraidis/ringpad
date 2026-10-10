// Source-derived state/mode return and dispatcher continuation; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned resident_state_request(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[15]-=4;put(expected_ram,expected.r[15],expected.r[14]);expected.r[15]-=4;put(expected_ram,expected.r[15],expected.pr);expected.r[15]-=4;put(expected_ram,expected.r[15],expected.r[5]);expected.r[14]=expected.r[4];expected.pr=0x8c648556;expected.pc=0x8c648548;
  return 8;}
static unsigned resident_state_query(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;need(expected_ram[(expected.r[4]&0xffffff)+1]==1,"Live short state path differs");auto&er=expected.r;er[0]=static_cast<int8_t>(expected_ram[(er[4]&0xffffff)+1]);expected.pc=expected.pr;const auto query_return=expected;need(expected.pc==0x8c648556,"Query return differs");
  er[2]=word(expected_ram,er[15]);er[4]=er[0];er[0]=114;expected_ram[(er[14]+114)&0xffffff]=er[2]&255;er[0]=er[4];expected.sr&=~1u;er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pc==0x8ced13ea,"State request return differs");
  return 18;}
static unsigned movie_after_state(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.pr=0x8ced13ee;expected.r[4]=expected.r[14];expected.pc=0x8ced13ea;return 3;}
static unsigned movie_mode_install(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;er[3]=word(expected_ram,0x8ced14bc);er[6]=2;put(expected_ram,er[15],er[4]);expected.pr=0x8ced1484;er[5]=15;expected.pc=0x8ced1476;need(er[3]==0x8ced43f8,"Mode install target differs");return 8;}
static unsigned movie_mode_slot(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;er[15]-=4;put(expected_ram,er[15],er[14]);er[15]-=4;put(expected_ram,er[15],er[13]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=4;er[13]=er[5];er[5]=er[13];put(expected_ram,er[15],er[6]);er[14]=er[4];expected.pr=0x8ced440c;er[4]=er[14];expected.pc=0x8ced43f8;return 11;}
static unsigned movie_mode_validation(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;need(expected.r[5]==15&&expected.r[6]==2,"Live validator mode/value differs");auto&er=expected.r;
  er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=12;put(expected_ram,er[15],er[4]);put(expected_ram,er[15]+8,er[5]);put(expected_ram,er[15]+4,er[6]);er[0]=word(expected_ram,er[15]+8);expected.sr&=~1u;er[0]=1;er[15]+=12;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;const auto validation_return=expected;
  expected.sr&=~1u;er[2]=static_cast<int16_t>(word(expected_ram,0x8ced443e));er[3]=er[13];er[1]=word(expected_ram,er[15]);er[3]<<=2;er[2]+=er[14];er[0]=er[13];er[3]+=er[2];expected.sr|=1u;put(expected_ram,er[3],er[1]);er[4]=er[14];er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[2]=word(expected_ram,0x8ced445c);need(er[2]==0x8ced4a64,"Retained refresh target differs");er[13]=word(expected_ram,er[15]);er[15]+=4;er[14]=word(expected_ram,er[15]);er[15]+=4;
  auto refresh=[&]{er[5]=static_cast<int16_t>(word(expected_ram,0x8ced4b14));er[0]=static_cast<int16_t>(word(expected_ram,0x8ced4b16));er[5]+=er[4];er[4]=word(expected_ram,er[0]+er[4]);er[3]=er[5]+4;er[4]=(er[4]<<2)+er[3];er[2]=word(expected_ram,er[4]);put(expected_ram,er[5],er[2]);expected.pc=expected.pr;};refresh();const auto slot_return=expected;need(expected.pc==0x8ced1484,"Slot return differs");
  er[4]=word(expected_ram,er[15]);er[6]=2;er[5]=word(expected_ram,0x8ced14c0);er[15]+=4;er[3]=word(expected_ram,0x8ced14c4);need(er[3]==0x8ced4a54,"Retained installer differs");expected.pr=word(expected_ram,er[15]);er[15]+=4;
  er[7]=static_cast<int16_t>(word(expected_ram,0x8ced4b14));er[6]<<=2;er[7]+=er[4];er[3]=er[7]+4;er[6]+=er[3];put(expected_ram,er[6],er[5]);refresh();const auto wrapper_return=expected;need(expected.pc==0x8ced13ee,"Install wrapper return differs");
  er[0]=0;expected.pr=word(expected_ram,er[15]);er[15]+=4;for(unsigned i=11;i<=14;++i){er[i]=word(expected_ram,er[15]);er[15]+=4;}expected.pc=expected.pr;
  return 85;}
static unsigned movie_next_dispatch(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;const bool skip_extra=false;auto&er=expected.r;need(er[0]==0&&er[13]==3,"Captured dispatcher return differs");er[4]=er[0];expected.sr|=1u;unsigned cycles=3;
  do{er[2]=static_cast<int16_t>(word(expected_ram,0x8ceda69e));++er[13];er[14]+=er[2];need(er[13]<er[11],"No next handler in expected prefix");expected.sr&=~1u;er[0]=static_cast<int16_t>(word(expected_ram,0x8ceda6ac));er[5]=word(expected_ram,er[14]+er[0]);expected.sr=(expected.sr&~1u)|unsigned(er[5]==0);cycles+=9;}while(!er[5]);er[0]=er[9];er[10]=er[5];er[7]=0;er[3]=word(expected_ram,er[10]+er[0]);er[5]=er[6]=0;expected.pr=0x8ceda5da;er[4]=er[12];expected.pc=0x8ceda5ca;cycles+=9;need(er[3]==(skip_extra?0x8ced178c:0x8ced511a)&&cycles==(skip_extra?48:39),"Next slot/target differs");return cycles;}

int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8ced1666,"Wrong state entry");unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=resident_state_request(m,c);save("resident-state-request");
cycles+=resident_state_query(m,c);save("resident-state-query");
cycles+=movie_after_state(m,c);save("movie-after-state");
cycles+=movie_mode_install(m,c);save("movie-mode-install");
cycles+=movie_mode_slot(m,c);save("movie-mode-slot");
cycles+=movie_mode_validation(m,c);save("movie-mode-validation");
cycles+=movie_next_dispatch(m,c);save("end");
need(cycles==172,"Mode return/dispatcher cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":172,"guest_execution":false,"next_target":"8CED511A"})"<<'\n';std::cout<<"PASS mode return133cycles plus dispatcher39cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
