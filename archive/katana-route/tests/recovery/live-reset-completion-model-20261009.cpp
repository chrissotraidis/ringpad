// Source-derived reset completion model; no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned reset_second(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[3]=word(ram,0x8c65f2e4);expected.pc=0x8c65f23e;need(expected.r[3]==0x8c66107a,"Original forwarding target differs");
  return 4;}
static unsigned record_reset_second_complete(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;need(expected.r[4]==0x8c8a8c90&&expected.r[5]==0&&expected_ram[(expected.r[4]&0xffffff)+5]==0,"Second reset idle precondition differs");const bool alternate=expected.r[5]!=0;auto&er=expected.r;
  er[15]-=4;put(expected_ram,er[15],er[14]);er[15]-=4;put(expected_ram,er[15],er[13]);er[15]-=4;put(expected_ram,er[15],expected.pr);er[15]-=8;er[14]=er[4];put(expected_ram,er[15]+4,er[5]);expected.sr&=~1u;er[13]=0;
  er[4]=er[15];expected.pr=0x8c66109c;
  // Original retained6607A0 stores the incoming IRQ mask and keeps IRQs masked.
  er[0]=(expected.sr>>4)&15;er[3]=static_cast<int16_t>(word(expected_ram,0x8c660896));put(expected_ram,er[4],er[0]);er[0]=(expected.sr&er[3])|240;expected.sr=er[0];
  er[2]=word(expected_ram,er[15]+4);expected.sr=(expected.sr&~1u)|unsigned(er[2]==0);
  if(alternate){er[0]=word(expected_ram,er[15]+4);expected.sr|=1u;expected.pr=0x8c6610e0;expected.pc=0x8c6610dc;}else{expected.pr=0x8c6610a6;expected.pc=0x8c6610a2;}er[4]=er[14];unsigned cycles=alternate?39:36;
  // Original66184C returns bit1 of byte1; SHAR also clears T for even 0/2.
  er[15]-=4;put(expected_ram,er[15],expected.pr);expected.sr&=~1u;er[0]=expected_ram[(er[4]&0xffffff)+1];er[4]=(er[0]&2)>>1;er[0]=er[4];expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;cycles+=13;
  need(er[0]==alternate,"Status/argument pair differs");expected.sr|=1u;cycles+=2;
  expected.pr=0x8c66111c;er[4]=er[15];cycles+=3;
  // Existing IRQ restore preserves the outer critical section's saved mask.
  er[0]=word(expected_ram,er[4]);er[2]=expected.sr;er[1]=static_cast<int16_t>(word(expected_ram,0x8c660896));er[0]=((er[0]&15)<<4)|(er[2]&er[1]);er[2]&=er[1];expected.sr=er[0];cycles+=16;
  need(er[13]==0,"Unexpected error path");expected.sr|=1u;cycles+=2;
  er[15]+=8;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[13]=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;cycles+=6;
  return cycles;}
static unsigned reset_second_return(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[2]=word(ram,0x8c648370);expected.r[4]=word(ram,expected.r[14]+4);expected.pr=0x8c6481d0;expected.pc=0x8c6481ca;need(expected.r[2]==0x8c65f4ce,"Next reset target differs");
  return 4;}
static unsigned channel_reset(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[15]-=4;put(expected_ram,expected.r[15],expected.pr);expected.r[15]-=4;expected.r[3]=word(ram,0x8c65f574);put(expected_ram,expected.r[15],expected.r[4]);expected.r[4]=word(ram,expected.r[4]+4);expected.pr=0x8c65f4da;expected.pc=0x8c65f4ce;need(expected.r[3]==0x8c66b4bc,"Channel reset target differs");
  return 7;}
static unsigned stream_reset(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;expected.r[15]-=4;put(expected_ram,expected.r[15],expected.pr);expected.r[15]-=4;expected.r[3]=word(ram,0x8c66b510);put(expected_ram,expected.r[15],expected.r[4]);expected.r[4]=word(ram,expected.r[4]+8);expected.pr=0x8c66b4c8;expected.pc=0x8c66b4bc;need(expected.r[3]==0x8c6489a8,"Channel reset target differs");
  return 7;}
static unsigned buffer_reset(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;
  er[2]=0;er[3]=er[4];er[15]-=4;put(expected_ram,er[15],er[4]);er[6]=8;put(expected_ram,er[3]+12,er[2]);er[5]=er[2];er[4]=word(expected_ram,er[15]);er[3]=word(expected_ram,0x8c6489e0);need(er[3]==0x8c10ef3c,"Retained clear target differs");er[4]+=40;er[15]+=4;
  // Actual retained forward byte clear: five setup, eight five-cycle iterations, RTS.
  er[7]=0;er[3]=0;er[0]=er[4];for(unsigned i=0;i<8;++i){++er[7];expected_ram[er[0]&0xffffff]=er[5]&255;expected.sr=(expected.sr&~1u)|unsigned(er[7]>=er[6]);++er[0];}er[0]=er[4];expected.pc=expected.pr;
  // Both original caller tails return naturally in the connected test.
  er[2]=word(expected_ram,er[15]);er[3]=0;put(expected_ram,er[2]+4,er[3]);er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;
  er[2]=word(expected_ram,er[15]);er[0]=0;expected_ram[(er[2]&0xffffff)+1]=0;er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;
  return 76;}
static unsigned outer_reset_return(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;const unsigned nesting=word(expected_ram,0x8c67ff20);need(nesting==1,"Outer reset nesting differs");auto&er=expected.r;
  er[3]=word(expected_ram,er[14]+8);need(er[3]==0,"Nonempty auxiliary object outside captured path");expected.sr|=1u;er[12]=0;er[0]=expected_ram[(er[14]&0xffffff)+2];need(er[0]==0,"Captured object type differs");expected.sr&=~1u;put(expected_ram,er[14]+20,er[12]);er[13]=er[12];unsigned cycles=10;
  while(true){er[0]=static_cast<int8_t>(expected_ram[(er[14]&0xffffff)+3]);expected.sr=(expected.sr&~1u)|unsigned(static_cast<int32_t>(er[13])>=static_cast<int32_t>(er[0]));cycles+=3;if(expected.sr&1)break;need(er[13]<2,"Unexpected child count");er[3]=er[14]+24;er[11]=(er[13]<<2)+er[3];er[11]=word(expected_ram,er[11]);er[2]=word(expected_ram,er[11]);er[3]=word(expected_ram,er[2]+20);need(er[3]==0x8c662194,"Unexpected child initializer");expected.pr=0x8c648224;er[4]=er[11];cycles+=11;
   er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=word(expected_ram,er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);put(expected_ram,er[4]+24,er[5]);cycles+=8;++er[13];cycles+=1;
  }
  er[0]=er[12];expected_ram[(er[14]&0xffffff)+1]=er[0];er[15]+=4;expected.pr=word(expected_ram,er[15]);er[15]+=4;er[3]=word(expected_ram,0x8c648364);need(er[3]==0x8c65eace,"Critical exit target differs");for(unsigned i=11;i<=14;++i){er[i]=word(expected_ram,er[15]);er[15]+=4;}cycles+=11;
  er[4]=word(expected_ram,0x8c65eaf8);er[3]=word(expected_ram,er[4])-1;er[2]=er[3];expected.sr=(expected.sr&~1u)|unsigned(er[2]==0);put(expected_ram,er[4],er[3]);cycles+=7;
  if(er[2]==0){er[2]=word(expected_ram,0x8c65eafc);er[1]=static_cast<int16_t>(word(expected_ram,0x8c65eaf4));er[0]=word(expected_ram,er[2]);er[2]=expected.sr;er[0]=((er[0]&15)<<4)|(er[2]&er[1]);er[2]&=er[1];expected.sr=er[0];cycles+=14;}expected.pc=expected.pr;cycles+=3;need(cycles==(nesting==1?94:80),"Reset tail cycle breakdown differs");
  return cycles;}
static unsigned post_reset(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;auto&er=expected.r;er[2]=word(expected_ram,0x8c648360);need(er[2]==0x8c65eaa8,"Critical entry target differs");expected.pr=0x8c64817e;
  er[4]=word(expected_ram,0x8c65eaf8);er[3]=word(expected_ram,er[4]);need(er[3]==0,"Outer reset did not release critical count");expected.sr|=1u;er[0]=expected.sr;er[3]=word(expected_ram,0x8c65eafc);er[2]=static_cast<int16_t>(word(expected_ram,0x8c65eaf4));er[0]=(er[0]>>4)&15;put(expected_ram,er[3],er[0]);er[0]=(expected.sr&er[2])|240;expected.sr=er[0];er[3]=word(expected_ram,er[4])+1;put(expected_ram,er[4],er[3]);
  er[3]=word(expected_ram,0x8c64834c);er[5]=word(expected_ram,er[15]);expected.pr=0x8c648186;er[4]=word(expected_ram,er[14]+4);expected.pc=0x8c64817e;need(er[3]==0x8c65f4a4,"Post-reset target differs");
  return 34;}

int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8c6481c2,"Wrong reset entry");unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=reset_second(m,c);save("reset-second");
cycles+=record_reset_second_complete(m,c);save("record-reset-second-complete");
cycles+=reset_second_return(m,c);save("reset-second-return");
cycles+=channel_reset(m,c);save("channel-reset");
cycles+=stream_reset(m,c);save("stream-reset");
cycles+=buffer_reset(m,c);save("buffer-reset");
cycles+=outer_reset_return(m,c);save("outer-reset-return");
cycles+=post_reset(m,c);save("end");
need(cycles==304,"Reset completion cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":304,"guest_execution":false,"next_target":"8C65F4A4"})"<<'\n';std::cout<<"PASS reset completion304cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
