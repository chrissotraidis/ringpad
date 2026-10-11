// Original quad-to-strip reordering through real eight-byte copy helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void quad_strip_register(DCRuntime&);}
using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}
struct QuadStrip : RequestModel {
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(w(a)&0xffff)));}
 void shl(unsigned&v){t(v>>31);v<<=1;}
 void copy8(){auto&r=c.r;push(r[3]);r[3]=w(0x8c10e970);r[3]=w(r[3]+r[0]);need(r[3]==0x8c10e9ae,"Unreviewed eight-byte copy jump");r[0]-=4;r[0]=w(r[2]+r[0]);r[3]=w(r[2]);put(m,r[1]+4,r[0]);put(m,r[1],r[3]);c.pc=c.pr;r[3]=pop();cycles+=13;}
 void reorder(){auto&r=c.r;for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[0]=sw(0x8c64261e);r[15]+=r[0];r[11]=r[5];r[12]=r[11];r[5]=0;r[14]=sw(0x8c642620);++r[12];r[6]=r[5];t(r[12]&1);r[12]=unsigned(int(r[12])>>1);r[13]=r[11];t(int(r[6])>=int(r[12]));r[14]+=r[15];--r[13];r[7]=r[15];cycles+=20;
  if(!(c.sr&1))do{
   r[2]=r[6]<<2;r[3]=w(r[4]);r[1]=r[5]<<2;shl(r[1]);shl(r[2]);r[2]+=r[3];r[3]=w(0x8c642638);r[1]+=r[14];c.pr=0x8c6425b4;r[0]=8;need(r[3]==0x8c10e958,"Unreviewed copy helper");cycles+=13;copy8();
   r[3]=r[6];r[2]=w(r[4]+4);r[3]<<=2;r[0]=r[5];r[3]+=r[2];r[2]=r[13];r[1]=w(r[3]);r[0]<<=2;r[0]+=r[7];r[2]<<=2;put(m,r[0],r[1]);r[3]=w(r[4]);++r[5];r[1]=r[5]<<2;shl(r[1]);shl(r[2]);r[2]+=r[3];r[3]=w(0x8c642638);r[1]+=r[14];++r[6];c.pr=0x8c6425e2;r[0]=8;cycles+=24;copy8();
   r[2]=w(r[4]+4);r[3]=r[13]<<2;r[0]=r[5];r[3]+=r[2];t(int(r[6])>=int(r[12]));r[0]<<=2;r[1]=w(r[3]);++r[5];r[0]+=r[7];--r[13];put(m,r[0],r[1]);cycles+=13;
  }while(!(c.sr&1));
  r[0]=sw(0x8c642622);r[5]=r[11];r[4]=sw(0x8c642622);r[5]-=2;put(m,r[15]+r[0],r[14]);r[0]=sw(0x8c642624);put(m,r[15]+r[0],r[7]);c.pr=0x8c64260e;r[4]+=r[15];c.pc=0x8c6425fc;cycles+=10;
 }
};
int main(int argc,char**argv){try{
 need(argc==8,"usage: quad-strip RAM STATE TIMERS SQ SQSTATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);quad_strip_register(rt);const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);need(ram.size()==16777216&&flash.size()==131072&&sq.size()==64,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram,vram=rt.vram;const std::string out=argv[7];unsigned controls=0;
 auto check=[&](SH4Context c,bool capture){const auto input=rt.main_ram;const unsigned descriptor=c.r[4],count=c.r[5],positions=word(input,descriptor),colors=word(input,descriptor+4);auto expected=input;QuadStrip model{{c,expected}};model.reorder();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){need(model.cycles==182,"Captured strip timing differs");save_ram(expected,out+"/phase0-expected-ram.bin");save_state(model.c,out+"/phase0-expected-state.txt");save_timers(timers,start,tocr,out+"/phase0-expected-timers.txt");save_sq(sq,device,out+"/phase0-expected");}
  std::string terminal;try{call_recompiled(c,rt,0x8c642574);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}need(terminal.find("No recompiled/native target registered for Dreamcast address 0x8C6424A0")!=std::string::npos,"Unexpected strip writer outcome");same(model.c,c);need(expected==rt.main_ram,"Strip whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Strip timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Reordering changed SQ/device state");need(sound==rt.aica_ram&&vram==rt.vram&&flash==rt.system_flash&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.holly_irq_taken&&!rt.sh4_async_redirect,"Unexpected device effects");
  const unsigned reordered=word(rt.main_ram,c.r[4]),reordered_colors=word(rt.main_ram,c.r[4]+4);for(unsigned pair=0;pair<(count+1)/2;++pair)for(unsigned half=0;half<2;++half){const unsigned source=half?count-1-pair:pair,dest=2*pair+half;need(word(rt.main_ram,reordered+8*dest)==word(input,positions+8*source)&&word(rt.main_ram,reordered+8*dest+4)==word(input,positions+8*source+4)&&word(rt.main_ram,reordered_colors+4*dest)==word(input,colors+4*source),"Independent quad/strip index mapping differs");}
  if(capture){save_ram(rt.main_ram,out+"/quad-observed-ram.bin");save_state(c,out+"/quad-observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/quad-observed-timers.txt");save_sq(sq,device,out+"/quad-observed");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';std::cout<<terminal<<'\n';}else ++controls;
 };
 for(unsigned count:{0u,1u,2u,3u,4u,5u,8u})for(unsigned pattern:{0u,0x55u,0xffu})for(unsigned alias:{0u,1u}){
  rt.main_ram=ram;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.r[5]=count;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.fpscr^=alias<<21;const unsigned base=alias?0xacc00000:0x8cc00000;c.r[4]=base;put(rt.main_ram,base,base+0x100);put(rt.main_ram,base+4,base+0x200);for(unsigned i=0;i<8;++i){put(rt.main_ram,base+0x100+8*i,0x7fc00100u^(i*0x1020304)^pattern);put(rt.main_ram,base+0x104+8*i,0x80000100u^(i*0x1020304)^pattern);put(rt.main_ram,base+0x200+4*i,0xabcdef00u^i^pattern);}check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[3]);restore_sq(rt,sq,device);check(initial,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"captured_prefix_cycles\":182,\"copied_vertices\":4,\"actual_index_order\":[0,3,1,2],\"whole_RAM_CPU_TMU_verified\":true,\"SQ_device_state_unchanged\":true,\"strip_writer_executed\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original quad strip: "<<controls<<" count/pattern/alias controls;real copies and0,3,1,2 vertex/color reorder,182cycle captured prefix;strip writer/draw/normalboot unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
