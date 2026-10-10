// Source-derived movie resource preparation and open wrapper, no guest execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
static unsigned prepare(auto&expected_ram,SH4Context&c){const unsigned mask=(c.sr>>4)&15;
RequestModel m{c,expected_ram};auto&er=m.c.r;need(er[4]==0x8cedd114&&m.c.pr==0x8ceb0254,"Method arguments differ");
  m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=8;er[14]=er[4];put(expected_ram,er[15],er[5]);er[3]=m.w(er[14]+56);put(expected_ram,er[15]+4,er[3]);m.c.pr=0x8cecf372;er[4]=er[14];m.cycles=11;
  m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=8;er[14]=er[4];er[3]=m.w(er[14]+48);m.c.pr=0x8cecf460;put(expected_ram,er[15]+4,er[3]);m.cycles+=9;
  er[0]=m.c.sr;er[3]=0xffffff0f;er[0]>>=4;er[0]&=15;er[4]=er[0];er[0]=m.c.sr;er[0]&=er[3];er[0]|=240;m.c.sr=er[0]&0x700083f3;er[0]=er[4];m.cycles+=18;
  m.c.pr=0x8cecf464;put(expected_ram,er[15],er[0]);m.cycles+=3;
  er[3]=m.w(0x8cecfb8c);m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[0]=m.w(er[3]);m.t(er[0]==1);need(m.c.sr&1,"Captured drain flag differs");er[2]=m.w(0x8cecfb90);er[1]=1;put(expected_ram,er[2],er[1]);er[14]=m.w(0x8cecfb84);er[13]=m.w(0x8cecfb80);er[3]=m.w(0x8cecfb94);er[0]=m.w(er[3]);m.t(er[0]==1);need(!(m.c.sr&1),"Active drain loop unmodeled");er[2]=m.w(0x8cecfb90);er[1]=0;put(expected_ram,er[2],er[1]);m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();m.cycles+=26;
  er[13]=0;put(expected_ram,er[14]+8,er[13]);m.c.pr=0x8cecf46c;er[4]=m.w(er[15]);m.cycles+=5;
  er[3]=m.c.sr;er[0]=er[4];er[2]=0xffffff0f;er[0]&=15;er[0]<<=2;er[3]&=er[2];er[0]<<=2;er[0]|=er[3];m.c.sr=er[0]&0x700083f3;m.cycles+=16;
  er[3]=m.w(0x8cecf4e0);m.c.pr=0x8cecf472;er[4]=m.w(er[15]+4);m.c.pc=0x8cecf46c;m.cycles+=4;need(m.cycles==92&&er[3]==0x8ced4084,"Inner reset boundary differs");
  m.push(er[14]);m.push(m.c.pr);er[3]=m.w(0x8ced40cc);need(er[3]==0x8ced1baa,"Validity target differs");m.c.pr=0x8ced408e;er[14]=er[4];m.cycles+=6;
  er[3]=m.w(0x8ced1c0c);m.t(er[4]==0);put(expected_ram,er[3],er[4]);need(er[4]!=0,"Null instance");er[0]=64;er[3]=m.w(er[4]+64);m.t(er[3]==0);need(er[3]==1,"Unmodeled active instance");er[0]=0;m.cycles+=12;
  m.t(er[0]==0);er[0]=64;er[0]=m.w(er[14]+er[0]);m.t(er[0]==1);er[0]=0;m.c.pr=m.pop();er[14]=m.pop();m.cycles+=11;
  er[4]=er[0];m.t(er[4]==0);need(m.c.sr&1,"Inner reset failed");er[1]=m.w(er[14]+52);m.t(er[1]==0);need(m.c.sr&1,"Open stream branch unmodeled");er[0]=64;er[2]=m.w(er[14]+er[0]);m.t(er[2]==0);need(m.c.sr&1,"Active object branch unmodeled");m.cycles+=10;
  er[3]=m.w(er[14]+56);er[4]=er[3];put(expected_ram,er[15],er[3]);er[2]=m.w(er[3]);er[1]=m.w(er[2]+20);need(er[1]==0x8c662194,"Original resource initializer differs");er[15]+=8;m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();m.cycles+=11;
  er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=m.w(er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);put(expected_ram,er[4]+24,er[5]);m.cycles+=8;need(m.c.pr==0x8cecf372,"Resource reset did not return to method");
  er[3]=m.w(0x8cecf3ac);er[5]=m.w(er[15]+4);m.c.pr=0x8cecf37a;er[4]=m.w(er[15]);m.c.pc=0x8cecf372;m.cycles+=5;need(m.cycles==155&&er[3]==0x8ced094c,"Next open wrapper inventory differs");need(((m.c.sr>>4)&15)==mask,"IRQ mask not restored");er[3]=m.w(0x8ced09c4);need(er[3]==0x8c65fb12,"Resident open target inventory differs");m.c.pc=0x8ced094c;m.cycles+=4;need(m.cycles==159,"Connected open cycles differ");c=m.c;return m.cycles;}
int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto ram=read(argv[1]);auto c=state(argv[2]);need(c.pc==0x8ceb023c&&c.pr==0x8ceb0254,"Resource start differs");auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];const auto cycles=prepare(ram,c);
save_state(c,out+"/end-state.txt");save_ram(ram,out+"/end-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,out+"/end-timers.txt");
need(c.r[4]==0x8ceb8b84&&c.r[5]==0x8c8aa8f8&&c.pr==0x8cecf37a&&c.pc==0x8ced094c,"Resident open args differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":159,"guest_execution":false,"next_target":"8C65FB12"})"<<'\n';std::cout<<"PASS resource preparation159cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
