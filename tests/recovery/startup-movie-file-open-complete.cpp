// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"
#include <cfenv>

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}



struct MovieFileOpenModel:RequestModel{
 unsigned hash_matches=0,lookup_index=0,file_handle=0;
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(m[a&0xffffff]|(m[(a+1)&0xffffff]<<8))));}
 void wh(unsigned a,unsigned v){wb(a,v);wb(a+1,v>>8);}
 void upper(){auto&r=c.r;r[3]=97;r[5]=unsigned(int(std::int8_t(r[4])));t(int(r[5])>=int(r[3]));cycles+=4;if(c.sr&1){r[1]=122;t(int(r[5])>int(r[1]));cycles+=3;if(!(c.sr&1)){r[4]-=32;++cycles;}}r[0]=r[4];c.pc=c.pr;cycles+=3;}
 void hash(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[13]=sw(0x8c64928e);r[14]=r[4];cycles+=6;for(;;){r[2]=sb(r[14]);t(r[2]==0);cycles+=3;if(c.sr&1)break;c.pr=0x8c649192;r[4]=sb(r[14]++);cycles+=3;upper();r[0]&=255;r[13]+=r[0];cycles+=2;}c.pr=pop();r[13]>>=2;r[0]=r[13]&255;r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;}
 void equal(){auto&r=c.r;push(r[14]);r[14]=r[4];push(r[13]);push(r[8]);push(c.pr);r[13]=r[5];cycles+=7;bool match=true;for(;;){r[3]=sb(r[14]);t(r[3]==0);cycles+=3;if(c.sr&1)break;c.pr=0x8c649134;r[4]=sb(r[14]++);cycles+=3;upper();r[8]=unsigned(int(std::int8_t(r[0])));c.pr=0x8c64913a;r[4]=sb(r[13]++);cycles+=4;upper();r[0]=unsigned(int(std::int8_t(r[0])));t(r[8]==r[0]);cycles+=3;if(!(c.sr&1)){match=false;break;}}
  if(match){r[1]=sb(r[13]);t(r[1]==0);cycles+=3;match=c.sr&1;}if(match){r[0]=1;++cycles;}else{r[0]=0;cycles+=2;}c.pr=pop();r[8]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=6;}
 void lookup(){auto&r=c.r;for(unsigned i=14;i>=10;--i)push(r[i]);push(c.pr);r[15]-=4;r[14]=r[5]+16;r[12]=r[4];put(m,r[15],r[6]);r[11]=w(r[5]);c.pr=0x8c64932a;r[4]=r[12];cycles+=15;hash();t(int(r[11])>0);r[10]=r[0];r[13]=0;cycles+=4;need(c.sr&1,"Empty directory unmodeled");
  for(;;){r[0]=sb(r[14]+9);r[3]=r[10]&255;r[0]&=255;t(r[0]==r[3]);cycles+=5;bool matched=false;if(c.sr&1){++hash_matches;r[0]=w(r[15]);t(r[0]==1);cycles+=3;need(!(c.sr&1),"Case sensitive mode unmodeled");r[5]=r[14]+10;c.pr=0x8c64935a;r[4]=r[12];cycles+=5;equal();t(r[0]==1);cycles+=2;matched=c.sr&1;}
   if(matched){lookup_index=r[13]+1;r[0]=r[13]+1;cycles+=3;break;}++r[13];t(int(r[13])>=int(r[11]));r[14]+=44;cycles+=4;need(!(c.sr&1),"Captured filename not found");}
  r[15]+=4;c.pr=pop();for(unsigned i=10;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;}
 void init(){auto&r=c.r;r[0]=76;r[3]=sw(r[4]+r[0]);t(r[3]==0);cycles+=4;need(c.sr&1,"Occupied descriptor unmodeled");r[3]=sw(0x8c6042d4);r[2]=r[7];r[1]=0xfffffff5;put(m,r[4]+4,r[5]);r[2]+=r[3];put(m,r[4]+8,r[6]);r[5]=0;put(m,r[4]+12,r[7]);r[2]=unsigned(int(r[2])>>11);r[0]=64;put(m,r[4]+16,r[2]);for(unsigned off:{20u,24u,28u,32u,36u,44u,52u,60u})put(m,r[4]+off,r[5]);put(m,r[4]+r[0],r[5]);r[0]=68;put(m,r[4]+r[0],r[5]);r[0]=72;wh(r[4]+r[0],r[5]);r[0]=74;wh(r[4]+r[0],r[5]);r[0]=76;r[2]=1;wh(r[4]+r[0],r[2]);r[0]=78;r[2]=w(0x8c6042d8);r[1]=sw(r[2]);wh(r[4]+r[0],r[1]);r[0]=80;wh(r[4]+r[0],r[5]);r[0]=82;wh(r[4]+r[0],r[5]);r[0]=r[4];c.pc=c.pr;cycles+=41;}
 void provider(){auto&r=c.r;push(r[14]);push(r[13]);r[13]=r[5];push(r[12]);t(r[13]==0);push(c.pr);r[6]=0;cycles+=8;need(c.sr&1,"Explicit directory unmodeled");r[3]=w(0x8c6042dc);r[13]=w(r[3]);r[13]=w(r[13]+16);cycles+=3;r[2]=w(0x8c6042e0);c.pr=0x8c604230;r[5]=r[13];cycles+=4;lookup();r[5]=r[0];t(r[5]==0);cycles+=3;need(!(c.sr&1),"No such file");r[7]=r[5];r[2]=w(0x8c6042dc);--r[7];r[12]=sw(0x8c6042d6);r[3]=44;r[6]=w(r[2]);c.macl=r[3]*r[7];r[14]=w(r[6]+24);r[4]=0;r[7]=c.macl;cycles+=12;
  for(;;){r[3]=w(r[6]);t(int(r[4])>=int(r[3]));cycles+=3;need(!(c.sr&1),"Descriptor table full");r[0]=76;r[3]=sw(r[14]+r[0]);t(r[3]==0);cycles+=4;if(c.sr&1)break;r[14]+=84;++r[4];cycles+=2;}
  r[13]+=16;r[13]+=r[7];r[6]=w(r[13]);r[7]=w(r[13]+4);r[6]+=r[12];c.pr=0x8c60426a;r[4]=r[14];cycles+=8;init();c.pr=0x8c60426e;r[4]=0;cycles+=3;r[2]=w(0x8c60394c);r[3]=w(r[2]);put(m,r[3]+48,r[4]);r[0]=1;c.pc=c.pr;cycles+=6;r[0]=r[14];cycles+=2;c.pr=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=6;file_handle=r[0];}
 void open(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=8;r[3]=w(0x8c65fb90);r[13]=r[4];put(m,r[15]+4,r[5]);r[5]=0;c.pr=0x8c65fb26;r[4]=r[13];cycles+=11;
  r[3]=w(0x8c605558);push(r[14]);r[14]=w(r[3]);t(r[14]==0);cycles+=5;need(!(c.sr&1),"No provider installed");r[0]=w(r[14]+60);r[14]=pop();cycles+=4;need(r[0]==0x8c604214,"Different provider");provider();t(r[0]==0);r[14]=r[0];cycles+=3;need(!(c.sr&1),"Open failed");r[3]=w(0x8c65fb9c);r[5]=r[15];c.pr=0x8c65fb42;r[4]=r[14];cycles+=5;
  t(r[4]==0);need(!(c.sr&1),"Null file descriptor");r[0]=76;r[3]=sw(r[4]+r[0]);t(r[3]==0);need(!(c.sr&1),"Inactive file descriptor");r[3]=w(r[4]+12);r[0]=1;put(m,r[5],r[3]);c.pc=c.pr;cycles+=12;
  t(r[0]==0);need(!(c.sr&1),"File length failed");cycles+=2;r[6]=w(r[15]);r[5]=0;r[7]=w(r[15]+4);c.pr=0x8c65fb64;r[4]=r[14];c.pc=0x8c65fb5a;cycles+=6;}
};
namespace dcrecomp_generated {void resident_movie_file_open_register(DCRuntime&);}
struct CompleteMovieOpenModel:MovieFileOpenModel{
 void stream(){const unsigned before=cycles;auto&er=c.r;auto&expected_ram=m;
push(er[14]);er[14]=100;push(er[13]);er[0]=32;push(er[11]);er[11]=er[7];push(er[10]);er[10]=0;push(c.pr);er[13]=w(0x8c65fb84);er[7]=er[10];cycles+=11;
  c.macl=er[14]*er[7];er[3]=c.macl;er[3]+=er[13];er[2]=sb(er[3]);t(er[2]==0);need(c.sr&1,"Captured stream slot occupied");cycles+=7;er[0]=er[7];t(er[0]==32);need(!(c.sr&1),"Stream pool full");cycles+=3;
  c.macl=er[14]*er[7];er[3]=w(0x8c65fb8c);er[7]=c.macl;er[14]=er[7]+er[13];er[7]=1;er[0]=er[7];wb(er[14]+1,er[0]);er[0]=er[10];wb(er[14]+2,er[0]);put(expected_ram,er[14]+4,er[11]);put(expected_ram,er[14]+8,er[4]);put(expected_ram,er[14]+12,er[0]);put(expected_ram,er[14]+16,er[5]);put(expected_ram,er[14]+20,er[6]);put(expected_ram,er[14]+44,er[3]);er[2]=w(er[14]+20);er[3]=0x7ff;er[2]+=er[3];t(int(er[2])>=0);need(c.sr&1,"Negative stream size unmodeled");cycles+=22;
  er[1]=0xfffffff5;er[2]=unsigned(int(er[2])>>11);put(expected_ram,er[14]+48,er[2]);er[5]=0;wb(er[14],er[7]);er[2]=w(er[11]);er[1]=w(er[2]+36);need(er[1]==0x8c6621a2,"Stream query differs");c.pr=0x8c65faf8;er[4]=er[11];cycles+=10;
  push(er[14]);push(c.pr);er[0]=er[5];t(er[0]==1);er[14]=er[4];need(!(c.sr&1),"Query mode differs");cycles+=7;t(er[5]==0);need(c.sr&1,"Query zero mode differs");cycles+=2;c.pr=pop();er[0]=w(er[14]+16);er[14]=pop();cycles+=5;
  put(expected_ram,er[14]+24,er[0]);put(expected_ram,er[14]+28,er[0]);er[0]=er[14];c.pr=pop();er[10]=pop();er[11]=pop();er[13]=pop();c.pc=c.pr;er[14]=pop();cycles+=10;need(cycles-before==77&&c.pc==0x8c65fb64&&er[0]==0x8c8a8010,"Stream constructor return differs");
 }
 void finish(){auto&r=c.r;r[2]=w(0x8c65fba8);need(r[2]==0x8c11157c,"Name copy target differs");r[4]=r[0];r[5]=r[13];r[14]=r[0];r[6]=32;c.pr=0x8c65fb72;r[4]+=64;cycles+=8;
  r[1]=0;r[0]=r[5];r[7]=r[4];r[5]=r[1];cycles+=5;for(;;){t(r[5]>=r[6]);cycles+=2;if(c.sr&1)break;r[3]=sb(r[0]);t(r[3]==0);cycles+=3;if(c.sr&1){t(r[5]>=r[6]);cycles+=2;if(!(c.sr&1))do{++r[5];wb(r[7],r[1]);t(r[5]>=r[6]);++r[7];cycles+=5;}while(!(c.sr&1));break;}r[3]=sb(r[0]++);++r[5];wb(r[7]++,r[3]);cycles+=4;}r[0]=r[4];c.pc=c.pr;cycles+=3;
  r[0]=r[14];r[15]+=8;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;need(cycles==16845&&c.pc==0x8cecf37a&&r[0]==0x8c8a8010,"Connected movie open return differs");
 }
};
namespace dcrecomp_generated {void resident_movie_stream_register(DCRuntime&);void resident_movie_stream_query_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8ced094c,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned dirty:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c65fb12),"Movie open already registered");resident_movie_file_open_register(rt);need(rt.targets.size()==count+13,"Open entries differ");for(unsigned a:{0x8c605416u,0x8c604214u,0x8c64930eu,0x8c649182u,0x8c649088u,0x8c649122u,0x8c6041a4u,0x8c603748u,0x8c604b6cu})need(rt.targets.contains(a),"Retained filesystem binding missing");resident_movie_stream_register(rt);resident_movie_stream_query_register(rt);need(rt.targets.contains(0x8c65fa86)&&rt.targets.contains(0x8c11157c),"Missing stream/name copy binding");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;need(c.r[4]==0x8ceb8b84&&c.r[5]==0x8c8aa8f8,"Movie open inputs differ");const unsigned pool=word(ram,word(ram,0x8c677154)+24);need(pool==0x8c6994f4,"File pool differs");if(dirty){for(unsigned off:{4u,8u,12u,16u,20u,24u,28u,32u,36u,44u,52u,60u,64u,68u,72u,80u})put(rt.main_ram,pool+off,0xa5a5a5a5);rt.main_ram[(pool+78)&0xffffff]=0xa5;rt.main_ram[(pool+79)&0xffffff]=0xa5;}if(dirty)for(unsigned i=0;i<32;++i)rt.main_ram[0x8a8050+i]=0xa5;auto expected_ram=rt.main_ram;CompleteMovieOpenModel m{{{c,expected_ram}}};m.open();m.stream();m.finish();need(m.lookup_index==1738&&m.file_handle==pool,"Directory identity/open result differs");auto expected=m.c;std::cout<<"predicted cycles="<<m.cycles<<" index="<<m.lookup_index<<" hash_matches="<<m.hash_matches<<"\n";
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65fb12);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!dirty&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Movie file-open coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"directory_index":1738,"file_handle":"8C6994F4","file_bytes":3119104,"cycles":16845,"return_pc":"8CECF37A","stream_handle":"8C8A8010","new_instructions":0,"dirty_descriptor_verified":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original movie open:8controls,directory lookup and descriptor allocated,returnECF37A\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
