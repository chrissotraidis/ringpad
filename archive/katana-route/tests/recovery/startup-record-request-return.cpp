// Source-derived original request/worker return contracts; no guest substitution.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <sstream>
using namespace dcrecomp_generated;
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing transfer fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xffffff),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}c.pc=entry;c.ssr=0x40008001;c.sgr=0x8ce34560;c.dbr=0xa1234567;for(unsigned i=0;i<8;++i)c.r_bank[i]=0xdead0010+i;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;}
static SH4Context state(const char* path){std::ifstream f(path);need(bool(f),"Missing transfer state");SH4Context c{};std::string l;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!(k=="pc"||k=="pr"||k=="sr"||k=="fpscr"||k=="vbr"||k=="spc"||k=="gbr"||k=="mach"||k=="macl"||k=="fpul"||k=="ssr"||k=="sgr"||k=="dbr"||k.starts_with("bank_r")||k[0]=='r'||k.starts_with("fr")||k.starts_with("xf")))continue;unsigned v=std::stoul(l.substr(eq+1),nullptr,16);if(k=="ssr")c.ssr=v;else if(k=="sgr")c.sgr=v;else if(k=="dbr")c.dbr=v;else if(k.starts_with("bank_r"))c.r_bank[std::stoul(k.substr(6))]=v;else if(k=="pc")c.pc=v;else if(k=="pr")c.pr=v;else if(k=="sr")c.sr=v;else if(k=="fpscr")c.fpscr=v;else if(k=="vbr")c.vbr=v;else if(k=="spc")c.spc=v;else if(k=="gbr")c.gbr=v;else if(k=="mach")c.mach=v;else if(k=="macl")c.macl=v;else if(k=="fpul")c.fpul=v;else if(k.starts_with("fr"))c.fr_bits[std::stoul(k.substr(2))]=v;else if(k.starts_with("xf"))c.xf_bits[std::stoul(k.substr(2))]=v;else if(k[0]=='r')c.r[std::stoul(k.substr(1))]=v;}return c;}
static void same(const SH4Context& a,const SH4Context& b){for(unsigned i=0;i<16;++i)if(a.r[i]!=b.r[i])std::cerr<<"Frozen register R"<<std::dec<<i<<" expected="<<std::hex<<a.r[i]<<" actual="<<b.r[i]<<"\n";if(a.pc!=b.pc||a.pr!=b.pr||a.sr!=b.sr||a.macl!=b.macl)std::cerr<<"Frozen PC/PR/SR/MACL expected="<<std::hex<<a.pc<<"/"<<a.pr<<"/"<<a.sr<<"/"<<a.macl<<" actual="<<b.pc<<"/"<<b.pr<<"/"<<b.sr<<"/"<<b.macl<<"\n";need(a.ssr==b.ssr&&a.sgr==b.sgr&&a.dbr==b.dbr&&a.r_bank==b.r_bank&&a.r==b.r&&a.pc==b.pc&&a.pr==b.pr&&a.sr==b.sr&&a.gbr==b.gbr&&a.vbr==b.vbr&&a.spc==b.spc&&a.mach==b.mach&&a.macl==b.macl&&a.fpul==b.fpul&&a.fpscr==b.fpscr&&a.fr_bits==b.fr_bits&&a.xf_bits==b.xf_bits,"Frozen transfer full state differs");}
static void save_state(const SH4Context& c,const std::string& path){std::ofstream s(path);need(bool(s),"Cannot save expected module state");s<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)s<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)s<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';s<<"dbr="<<std::hex<<c.dbr<<'\n';s<<"ssr="<<std::hex<<c.ssr<<"\nsgr="<<c.sgr<<'\n';for(unsigned i=0;i<8;++i)s<<"bank_r"<<std::dec<<i<<'='<<std::hex<<c.r_bank[i]<<'\n';need(bool(s),"Expected module state write failed");}
static void save_ram(const auto& m,const std::string& path){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(m.data()),m.size());need(bool(f),"Cannot save prospective RAM");}
static void timer_same(const SH4TMUChannel&a,const SH4TMUChannel&b){need(a.tcor==b.tcor&&a.tcnt==b.tcnt&&a.tcr==b.tcr&&a.cycle_accumulator==b.cycle_accumulator&&a.lazy_sh4_cycles==b.lazy_sh4_cycles&&a.ticks==b.ticks&&a.underflows==b.underflows,"Timer channel differs");}
static void tick(auto& timers,unsigned start,unsigned cycles){for(unsigned i=0;i<3;++i)if(start&(1u<<i)){auto& ch=timers[i];need(ch.lazy_sh4_cycles==0&&(ch.tcr&7)<=4,"Unsupported synthetic timer initial state");const unsigned div=16u<<(2*(ch.tcr&7));const auto total=ch.cycle_accumulator+cycles;const auto n=total/div;need(n<=ch.tcnt,"Synthetic timer unexpectedly underflows");ch.cycle_accumulator=total%div;ch.tcnt-=n;ch.ticks+=n;}}
static void load_timers(DCRuntime& rt,const char* path){std::ifstream f(path);need(bool(f),"Missing observed timer state");std::string l;unsigned count=0;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!k.starts_with("tmu")&&!k.starts_with("sh4_tick_"))continue;auto v=std::stoull(l.substr(eq+1),nullptr,16);++count;if(k=="tmu_tocr")rt.tmu_tocr=v;else if(k=="tmu_tstr")rt.tmu_tstr=v;else if(k=="sh4_tick_pending_cycles")rt.sh4_tick_pending_cycles=v;else if(k=="sh4_tick_batch_cycles")rt.sh4_tick_batch_cycles=v;else{need(k.size()>5&&k[3]>='0'&&k[3]<='2'&&k[4]=='_',"Invalid observed timer key");auto& c=rt.tmu[k[3]-'0'];auto field=k.substr(5);if(field=="tcor")c.tcor=v;else if(field=="tcnt")c.tcnt=v;else if(field=="tcr")c.tcr=v;else if(field=="cycle_accumulator")c.cycle_accumulator=v;else if(field=="lazy_sh4_cycles")c.lazy_sh4_cycles=v;else if(field=="ticks")c.ticks=v;else if(field=="underflows")c.underflows=v;else throw std::runtime_error("Unknown timer field");}}
 need(count==25&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1&&(rt.tmu_tstr&~7u)==0,"Observed timer/batch fixture differs");}
static void save_timers(const auto&timers,unsigned start,unsigned tocr,const std::string& path){std::ofstream f(path);f<<std::hex<<"tmu_tocr="<<tocr<<"\ntmu_tstr="<<start<<"\nsh4_tick_pending_cycles=0\nsh4_tick_batch_cycles=1\n";for(unsigned i=0;i<3;++i){const auto& c=timers[i];auto out=[&](const char* key,auto v){f<<"tmu"<<std::dec<<i<<'_'<<key<<'='<<std::hex<<v<<'\n';};out("tcor",c.tcor);out("tcnt",c.tcnt);out("tcr",c.tcr);out("cycle_accumulator",c.cycle_accumulator);out("lazy_sh4_cycles",c.lazy_sh4_cycles);out("ticks",c.ticks);out("underflows",c.underflows);}need(bool(f),"Cannot save expected timers");}


namespace dcrecomp_generated {void callback_register(DCRuntime&);void parent_register(DCRuntime&);void record_helpers_register(DCRuntime&);void helper_register(DCRuntime&);void serializer_register(DCRuntime&);void transport_register(DCRuntime&);void command_register(DCRuntime&);void submission_register(DCRuntime&);void worker_register(DCRuntime&);}

static void registers(DCRuntime&rt){register_recompiled_program(rt);callback_register(rt);parent_register(rt);record_helpers_register(rt);helper_register(rt);serializer_register(rt);transport_register(rt);command_register(rt);submission_register(rt);worker_register(rt);for(unsigned a:{0x8c10b992u,0x8c10b906u,0x8c10a326u,0x8c10a2dau,0x8c6067aau,0x8c64a734u,0x8c664286u,0x8c6642c0u,0x8c663550u})need(rt.targets.contains(a),"Missing real binding");}

// Expectations are derived from the SH-4 source, including ordered frame writes.
// This models queue insertion, not device processing or response delivery.
struct RequestModel {
 SH4Context c; std::vector<std::uint8_t>& m; unsigned cycles=0;
 auto& r(){return c.r;}
 unsigned w(unsigned a){return word(m,a);}
 unsigned sb(unsigned a){return unsigned(int(std::int8_t(m[a&0xffffff])));}
 void wb(unsigned a,unsigned v){m[a&0xffffff]=std::uint8_t(v);}
 void push(unsigned v){c.r[15]-=4;put(m,c.r[15],v);}
 unsigned pop(){auto v=w(c.r[15]);c.r[15]+=4;return v;}
 void t(bool v){c.sr=(c.sr&~1u)|unsigned(v);}
 void range(unsigned a,unsigned b,unsigned extra=0){cycles+=(b-a)/2+1+extra;}
 void finish_request(){
  c.r[15]+=28;c.pr=pop();for(unsigned i=8;i<14;++i)c.r[i]=pop();c.pc=c.pr;c.r[14]=pop();range(0x7a6,0x7b8,1);
 }
 void request(){
  auto& r=c.r;
  for(unsigned i=14;i>=8;--i)push(r[i]);push(c.pr);r[15]-=28;
  r[10]=44;r[9]=r[4];c.macl=r[10]*r[9];r[14]=w(0x8c663590);
  r[0]=r[5];wb(r[15]+4,r[0]);r[0]=64;put(m,r[15]+24,r[6]);
  r[4]=w(r[14]);r[10]=c.macl;r[3]=r[4]+40;r[12]=w(r[15]+r[0]);r[3]+=r[10];
  r[0]=sb(r[3]+6);t(r[0]==0);r[11]=r[7];range(0x550,0x584,1);
  if(!(c.sr&1)){r[0]=0xffffffff;range(0x586,0x588);finish_request();return;}
  r[0]=sb(r[15]+4);r[2]=r[15]+16;r[5]=0xc0;wb(r[3]+6,r[0]);r[8]=r[11];
  r[0]=w(0x8c6636a0);r[8]+=r[12];r[4]=r[8]+3;put(m,r[15]+8,r[4]);r[4]=3;
  r[3]=sb(r[0]+r[9]);r[0]=0x10cc;r[5]&=r[3];wb(r[2],r[3]);r[5]&=255;
  put(m,r[15]+12,r[5]);r[2]=unsigned(-6);r[5]=unsigned(int(r[5])>>6)&r[4];put(m,r[15],r[5]);
  r[13]=w(r[14]);r[2]=r[13];r[3]=w(r[2]+r[0]);t(int(r[3])>0);range(0x594,0x5c8);
  if(c.sr&1){
   r[0]=0xfd8;r[1]=w(r[13]+r[0]);t(int(r[1])>0);range(0x5ca,0x5d0);
   if(c.sr&1){
    r[0]=0xfcc;r[2]=w(r[13]+r[0]);r[3]=unsigned(int(std::int16_t(w(r[2])>>16)));
    r[4]&=r[3];r[3]=w(r[15]);t(r[4]==r[3]);range(0x5d2,0x5e2);
    need(!(c.sr&1),"Same-port padding path requires separate source model");
   }
  }
  r[0]=0xfcc;r[2]=w(r[14]);r[1]=w(r[2]+r[0]+12);r[3]=w(r[2]+r[0]);
  r[1]<<=2;r[3]+=r[1];put(m,r[2]+r[0],r[3]);r[1]=r[8]&255;r[2]=w(r[15]);
  r[13]=w(r[14]);r[2]<<=16;r[13]=w(r[13]+r[0]);r[2]|=r[1];put(m,r[15]+20,r[1]);
  r[1]=0x10b4;put(m,r[13],r[2]);r[13]+=4;push(r[13]);r[13]+=4;
  r[0]=w(r[14]);r[3]=r[0];r[1]+=r[3];r[3]=w(r[1]);r[2]=r[3];
  t(r[3]>>31);r[3]<<=1;r[3]+=r[2];r[3]<<=4;r[2]=0xfe8;r[4]=r[9];
  t(r[3]>>31);r[3]<<=1;r[0]+=r[2];r[4]<<=2;r[0]+=r[3];c.pr=0x8c66363a;r[4]=w(r[0]+r[4]);
  range(0x5ea,0x638,1);
  // Real6644FE converts the queue's response destination to a physical address.
  r[0]=w(0x8c66452c)&r[4];c.pc=c.pr;cycles+=4;
  r[2]=pop();r[1]=0x10c4;put(m,r[2],r[0]);r[0]=sb(r[15]+4);r[6]=w(0x8c6636a4);
  r[0]&=255;r[7]=w(r[15]+24);put(m,r[15],r[0]);r[0]=16;r[9]=sb(r[15]+r[0]);
  r[0]=w(r[14]);r[4]=w(0x8c6636ac);r[0]=w(r[0]+r[1]);r[5]=w(0x8c6636a8);t(r[0]==1);r[9]&=255;
  range(0x63a,0x65a);
  if(c.sr&1){
   r[3]=w(r[15]);r[9]<<=16;r[0]=w(r[15]+12);r[9]&=r[4];r[3]<<=16;t(int(r[11])>0);
   r[3]<<=8;r[0]<<=8;r[3]&=r[5];r[0]&=r[6];r[3]|=r[9]|r[0];r[0]=w(r[15]+20);
   r[3]|=r[0];put(m,r[13],r[3]);r[13]+=4;r[9]=0;range(0x65c,0x67e);
   if(c.sr&1){do{r[3]=w(r[7]);r[7]+=4;++r[9];t(int(r[9])>=int(r[11]));put(m,r[13],r[3]);r[13]+=4;range(0x680,0x68a);}while(!(c.sr&1));range(0x68c,0x68e);}
  }else{
   r[3]=w(r[15]+12);r[9]<<=8;r[8]<<=16;r[0]=w(r[15]);r[3]<<=16;t(int(r[11])>0);r[8]<<=8;
   r[3]&=r[4];r[9]&=r[6];r[8]&=r[5];r[3]|=r[9]|r[8]|r[0];put(m,r[13],r[3]);r[13]+=4;r[8]=0;range(0x6b0,0x6d0);
   if(c.sr&1){do{
    r[9]=w(r[7]);r[7]+=4;++r[8];t(int(r[8])>=int(r[11]));
    r[2]=(r[9]<<24)&r[5];r[3]=(r[9]<<8)&r[4];r[2]|=r[3];r[3]=unsigned(-8);
    r[1]=unsigned(int(r[9])>>8)&r[6];r[2]|=r[1];r[1]=(r[9]>>24)&255;r[2]|=r[1];put(m,r[13],r[2]);r[13]+=4;
    range(0x6d2,0x702);
   }while(!(c.sr&1));}
  }
  r[1]=0x10c4;r[0]=w(r[14]);r[0]=w(r[0]+r[1]);t(r[0]==1);range(0x704,0x70c);
  if(c.sr&1){
   r[7]=w(r[15]+60);t(int(r[12])>0);r[11]=0;range(0x70e,0x714);
   if(c.sr&1){do{
    r[0]=sb(r[7]+1);++r[11];r[1]=sb(r[7]);t(int(r[11])>=int(r[12]));r[0]=(r[0]<<16)&r[4];r[3]=r[0];
    r[0]=sb(r[7]+2);r[1]<<=16;r[0]=(r[0]<<8)&r[6];r[3]|=r[0];r[0]=sb(r[7]+3);
    r[1]=(r[1]<<8)&r[5];r[0]&=255;r[3]|=r[1]|r[0];put(m,r[13],r[3]);r[13]+=4;r[7]+=4;range(0x716,0x740);
   }while(!(c.sr&1));range(0x742,0x744);}
  }else{
   r[5]=w(r[15]+60);t(int(r[12])>0);r[4]=0;range(0x746,0x74c);
   if(c.sr&1){do{r[3]=w(r[5]);r[5]+=4;++r[4];t(int(r[4])>=int(r[12]));put(m,r[13],r[3]);r[13]+=4;range(0x74e,0x758);}while(!(c.sr&1));}
  }
  r[0]=0xfd8;r[2]=w(r[15]+8);r[3]=w(r[14]);put(m,r[3]+r[0],r[2]);r[0]+=4;r[3]=w(r[14]);
  r[2]=w(r[15]+8);r[1]=w(r[3]+r[0]);r[2]<<=2;r[1]+=r[2];put(m,r[3]+r[0],r[1]);r[0]+=4;
  r[3]=w(r[14]);r[2]=w(r[14]);r[1]=w(r[3]+r[0]);r[2]+=40;++r[1];put(m,r[3]+r[0],r[1]);
  --r[1];r[2]+=r[10];put(m,r[2]+24,r[1]);r[3]=w(r[14]);r[2]=w(0x8c6637c0);r[1]=w(r[3]+r[0]);
  r[1]&=r[2];put(m,r[3]+r[0],r[1]);r[3]=w(r[14]);r[1]=w(r[3]+r[0]);t(r[1]==0);range(0x75a,0x794);
  if(c.sr&1){r[0]=0xfe0;r[3]=1;r[1]=w(r[14]);put(m,r[1]+r[0],r[3]);range(0x796,0x79c);}
  r[0]=w(r[14])+40+r[10];r[0]=w(r[0]+24);range(0x79e,0x7a4);finish_request();
 }
 void worker(){
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[6]<<=16;r[14]=r[15];r[6]<<=8;put(m,r[14],r[5]);
  r[7]<<=16;r[3]=w(r[15]+16);r[6]|=r[7]|r[3];put(m,r[14]+4,r[6]);r[7]=2;r[3]=w(r[15]+24);
  r[6]=r[14];push(r[3]);r[2]=w(r[15]+24);push(r[2]);c.pr=0x8c6642e8;r[5]=12;c.pc=0x8c663550;cycles+=21;
  request();r[15]+=16;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=5;
 }
};

static unsigned check_return(DCRuntime&rt,SH4Context&c,bool worker,const std::string&stem={}){
 auto m=rt.main_ram;RequestModel model{c,m};if(worker)model.worker();else model.request();
 auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
 const auto flash=rt.system_flash,sound=rt.aica_ram;
 if(!stem.empty()){save_ram(m,stem+"-expected-ram.bin");save_state(model.c,stem+"-expected-state.txt");save_timers(timers,start,tocr,stem+"-expected-timers.txt");}
 call_recompiled(c,rt,worker?0x8c6642c0:0x8c663550);
 if(!stem.empty()){save_state(c,stem+"-actual-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"-actual-timers.txt");}
 same(model.c,c);
 if(m!=rt.main_ram){unsigned n=0;for(unsigned i=0;i<m.size()&&n<12;++i)if(m[i]!=rt.main_ram[i]){std::cerr<<"RAM @"<<std::hex<<i<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';++n;}}
 need(m==rt.main_ram,"Full request return RAM differs");
 for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
 need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Request timer controls differ");
 need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");
 return model.cycles;
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: request-return RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
 load_embedded_elf_image(rt);registers(rt);const auto ram=read(argv[1]),flash=read(argv[3]);rt.system_flash=flash;
 need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");const auto actual=state(argv[2]);const std::string stem=argv[4];
 // Patterned payloads ensure this tests packing, byte order and counter wrap,
 // rather than relying on the zero payload in the captured startup route.
 unsigned controls=0;
 for(unsigned endian:{0u,1u})for(unsigned count:{0u,1u,48u})for(unsigned wrap:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[2]);auto c=actual;c.pc=0x8c6642c0;
  put(rt.main_ram,c.r[15]+8,count);const auto root=word(ram,0x8c8ac634);put(rt.main_ram,root+0x10c4,endian);put(rt.main_ram,root+0xfe0,wrap?0x7fffffff:56);
  for(unsigned i=0;i<count;++i)put(rt.main_ram,word(ram,c.r[15]+4)+i*4,0x81fe00a5u^(i*0x1020301u));
  c.r[5]=0x12345678;c.r[6]=0x89;c.r[7]=0xab;put(rt.main_ram,c.r[15],0xcdef);
  check_return(rt,c,true);++controls;
 }
 for(unsigned busy:{1u,12u,128u,255u}){rt.main_ram=ram;load_timers(rt,argv[2]);auto c=actual;const auto root=word(ram,0x8c8ac634);rt.main_ram[(root+40+44+6)&0xffffff]=busy;check_return(rt,c,true);need(c.r[0]==0xffffffff,"Busy request did not reject");++controls;}
 rt.main_ram=ram;load_timers(rt,argv[2]);auto c=actual;const auto cycles=check_return(rt,c,true,stem+"/worker-return");
 const auto root=word(ram,0x8c8ac634),packet=word(ram,root+0xfcc),record=root+40+44;
 // Independent queue contract, separate from the register model above.
 need(cycles==556&&c.r[0]==56&&word(rt.main_ram,root+0xfd8)==53&&word(rt.main_ram,root+0xfdc)==212&&word(rt.main_ram,root+0xfe0)==57,"Actual request size/sequence differs");
 need(rt.main_ram[(record+6)&0xffffff]==12&&word(rt.main_ram,record+24)==56&&word(rt.main_ram,packet)==50&&word(rt.main_ram,packet+8)==0x3200010c&&word(rt.main_ram,packet+12)==0x04000000&&word(rt.main_ram,packet+16)==0,"Actual command/header differs");
 const auto response=word(ram,root+0xfe8+96*word(ram,root+0x10b4)+4)&0x0fffffff;need(word(rt.main_ram,packet+4)==response,"Queue response destination differs");
 for(unsigned i=0;i<48;++i)need(word(rt.main_ram,packet+20+i*4)==0,"Actual payload differs");
 need(rt.main_ram[0x664538]==0x80,"Worker released caller-owned lock");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"worker_cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"queue_words\":53,\"request_id\":56,\"delivery_verified\":false,\"device_clocks_enabled\":false,\"parent_full_return_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original request/worker return: "<<controls<<" byte-order/payload/sequence-wrap/busy controls; actual556cycles whole16MiB RAM69CPU25TMU, 53-word queued command12/request56, caller lock80 retained; delivery/parent return/normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
