// Remaining original stream position/start and resource method return.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned ready(auto&expected_ram,SH4Context&c){
need(c.pc==0x8cecf3c4&&c.pr==0x8cecf3ce&&c.r[4]==0x8c8a8010&&c.r[5]==226,"Stream position input differs");
RequestModel m{c,expected_ram};auto&er=m.c.r;m.cycles=15;
m.push(m.c.pr);er[15]-=4;er[3]=m.w(0x8ced09cc);m.c.pr=0x8ced096a;put(expected_ram,er[15],er[4]);m.cycles+=6;need(er[3]==0x8c65ff48,"Limit setter differs");put(expected_ram,er[4]+44,er[5]);er[0]=1;m.cycles+=4;er[15]+=4;m.c.pr=m.pop();er[0]=0;m.cycles+=5;
  m.t(er[0]==0);er[0]=124;er[5]=m.w(er[14]+er[0]);m.t(int(er[5])>=0);er[3]=0xfffffff5;er[5]=unsigned(int(er[5])>>11);er[3]=m.w(0x8cecf4c8);m.c.pr=0x8cecf3f4;er[4]=m.w(er[14]+52);need(er[3]==0x8ced0972,"Compatibility target differs");m.cycles+=12;er[0]=0;m.cycles+=3;
  m.t(er[0]==0);er[3]=m.w(0x8cecf4d0);m.c.pr=0x8cecf40a;er[4]=m.w(er[14]+52);need(er[3]==0x8ced0976,"Start wrapper differs");m.cycles+=6;
  m.push(m.c.pr);er[3]=m.w(0x8ced09d0);m.c.pr=0x8ced097e;m.cycles+=5;need(er[3]==0x8c65fc82,"Start resident differs");m.push(er[14]);er[3]=0;m.push(m.c.pr);er[2]=m.w(0x8c65fd2c);er[14]=er[4];er[0]=2;m.wb(er[14]+1,er[0]);put(expected_ram,er[14]+52,er[3]);m.c.pr=0x8c65fc96;er[4]=m.w(er[14]+8);m.cycles+=11;need(er[2]==0x8c604c06,"Position query differs");
  m.t(er[4]==0);need(!(m.c.sr&1),"Null descriptor");er[0]=76;er[3]=unsigned(int(std::int16_t(expected_ram[(er[4]+er[0])&0xffffff]|(expected_ram[(er[4]+er[0]+1)&0xffffff]<<8))));m.t(er[3]==0);need(!(m.c.sr&1),"Inactive descriptor");er[0]=m.w(er[4]+20);need(er[0]==0,"Captured initial position differs");m.cycles+=10;
  put(expected_ram,er[14]+12,er[0]);er[0]=1;m.c.pr=m.pop();er[14]=m.pop();m.cycles+=6;m.c.pr=m.pop();er[0]=0;m.cycles+=4;
  er[4]=er[0];m.t(er[4]==0);er[2]=m.w(0x8cecf4d4);m.c.pr=0x8cecf422;er[4]=er[14];m.c.pc=0x8cecf41c;m.cycles+=7;need(m.cycles==94&&er[2]==0x8ced0700,"Prepare target differs");
  auto flag=[&](unsigned wrapper,unsigned leaf,unsigned value){
   m.push(m.c.pr);er[3]=m.w(wrapper==0x8ced0700?0x8ced0824:0x8ced082c);m.c.pr=wrapper+8;er[4]=m.w(er[4]+48);need(er[3]==leaf,"Flag wrapper target differs");m.cycles+=5;
   m.push(m.c.pr);er[15]-=4;er[3]=m.w(leaf==0x8ced3858?0x8ced3884:0x8ced40cc);m.c.pr=leaf+10;put(expected_ram,er[15],er[4]);need(er[3]==0x8ced1baa,"Validity target differs");m.cycles+=6;
   er[3]=m.w(0x8ced1c0c);m.t(er[4]==0);put(expected_ram,er[3],er[4]);need(er[4]!=0,"Null instance");er[0]=64;er[3]=m.w(er[4]+64);m.t(er[3]==0);need(er[3]!=0,"Invalid instance");er[0]=0;m.cycles+=12;
   m.t(er[0]==0);er[4]=m.w(er[15]);er[15]+=4;m.c.pr=m.pop();er[0]=68;er[3]=value;put(expected_ram,er[4]+er[0],er[3]);er[0]=0;m.cycles+=12;
   er[4]=er[0];m.t(er[4]==0);m.c.pr=m.pop();er[0]=0;m.cycles+=7;
  };flag(0x8ced0700,0x8ced3858,3);er[3]=m.w(0x8cecf4d8);m.c.pr=0x8cecf428;er[4]=er[14];need(er[3]==0x8ced071c,"Arm target differs");m.cycles+=4;flag(0x8ced071c,0x8ced4032,4);
  er[0]=64;er[2]=m.w(er[14]+er[0]);m.t(er[2]==0);need(m.c.sr&1,"Active object branch unmodeled");er[2]=1;er[0]=68;er[3]=0;put(expected_ram,er[14]+er[0],er[3]);put(expected_ram,er[14]+8,er[2]);er[15]+=8;m.c.pr=m.pop();er[13]=m.pop();m.c.pc=m.c.pr;er[14]=m.pop();m.cycles+=15;need(m.cycles==197&&m.c.pc==0x8ceb0254,"Method return differs");c=m.c;return m.cycles-15;}
int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto ram=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];const auto cycles=ready(ram,c);need(cycles==182,"Remaining method cycles differ");
save_state(c,out+"/method-return-state.txt");save_ram(ram,out+"/method-return-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,out+"/method-return-timers.txt");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":182,"guest_execution":false,"return_pc":"8CEB0254"})"<<'\n';std::cout<<"PASS remaining stream setup182cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
