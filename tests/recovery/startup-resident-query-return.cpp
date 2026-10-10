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



struct BufferQueryModel:RequestModel{
 void query(){auto&r=c.r;push(r[14]);push(r[13]);r[3]=w(0x8c65e6f0);push(r[12]);push(r[11]);push(r[10]);push(r[9]);r[9]=0;r[13]=w(r[3]);t(r[13]==0);r[12]=r[9];need(!(c.sr&1),"Captured query global absent");cycles+=12;
 t(r[6]==0);r[0]=c.sr&1;t(r[7]==0);r[1]=c.sr&1;r[0]|=r[1];t(r[0]==0);need(c.sr&1,"Query output absent");cycles+=7;
 r[14]=static_cast<int8_t>(r[5]);t(static_cast<int32_t>(r[14])>=0);cycles+=3;bool invalid=!(c.sr&1);
 if(!invalid){r[1]=w(0x8c65e6fc);r[11]=static_cast<int8_t>(r[5]);r[0]=r[4];r[11]<<=2;t(r[11]>>31);r[11]<<=1;t(r[0]==r[1]);r[10]=16;cycles+=8;need(!(c.sr&1),"Only reached SPRS type modeled");
 for(unsigned literal:{0x8c65e700u,0x8c65e704u,0x8c65e708u}){r[1]=w(literal);t(r[0]==r[1]);cycles+=3;need(bool(c.sr&1)==(literal==0x8c65e708),"Unexpected query type");}
 t(static_cast<int32_t>(r[14])>=static_cast<int32_t>(r[10]));cycles+=2;invalid=c.sr&1;
 if(!invalid){r[4]=static_cast<int16_t>(w(0x8c65e6ea));cycles+=1;r[4]+=r[13];r[4]+=r[11];cycles+=3;t(r[4]==0);need(!(c.sr&1),"Query table absent");cycles+=2;r[3]=w(r[4]);put(m,r[6],r[3]);r[2]=w(r[4]+4);put(m,r[7],r[2]);cycles+=5;}}
 if(invalid){r[12]=w(0x8c65e768);cycles+=3;}
 r[9]=pop();r[0]=r[12];r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;
 }
};
struct QueryReturnModel:BufferQueryModel{
 void adapter_prefix(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=16;r[14]=r[4];r[3]=r[14]+16;r[2]=w(0x8c66c438);wb(r[15],r[5]);put(m,r[15]+4,r[6]);put(m,r[15]+8,r[7]);r[4]=sb(r[15]);r[4]<<=2;put(m,r[15]+12,r[4]);r[4]+=r[3];c.pr=0x8c66c39e;r[4]=w(r[4]);need(r[2]==0x8c66e930,"Original getter target differs");cycles+=18;
 r[0]=w(r[4]+4);c.pc=c.pr;cycles+=3;
 r[4]=w(0x8c66c43c);r[3]=w(0x8c66c440);put(m,r[15],r[0]);r[6]=w(r[15]+4);r[7]=w(r[15]+8);c.pr=0x8c66c3ac;r[5]=w(r[15]);c.pc=0x8c66c39e;cycles+=8;need(r[3]==0x8c65e640&&r[4]==0x52535053,"Original query target differs");
 }
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
 SH4Context adapter_return{};
 void adapter_tail(){auto&r=c.r;r[2]=r[14];r[1]=w(r[15]+4);r[3]=w(r[15]+12);r[2]+=48;r[1]=w(r[1]);r[13]=r[0];r[2]+=r[3];put(m,r[2],r[1]);t(r[13]==0);r[3]=w(r[15]+8);r[2]=w(r[3]);put(m,r[14]+56,r[2]);need(c.sr&1,"Query failed");cycles+=13;r[0]=r[13];r[15]+=16;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;need(c.pc==0x8c660a2e,"Query adapter return differs");}
 void loop_tail(){auto&r=c.r;r[2]=r[14];r[3]=r[12];r[1]=w(r[15]);r[2]+=40;r[3]<<=2;++r[12];r[3]+=r[2];t(static_cast<int32_t>(r[12])>=static_cast<int32_t>(r[13]));put(m,r[3],r[1]);cycles+=10;}
 void complete(){auto&r=c.r;adapter_tail();adapter_return=c;loop_tail();need(!(c.sr&1)&&r[12]==1,"Second query not required");
 r[7]=r[15];r[5]=r[12];r[0]=64;r[6]=r[15];r[7]+=4;c.pr=0x8c660a2e;r[4]=w(r[14]+64);cycles+=8;adapter_prefix();query();adapter_tail();loop_tail();need((c.sr&1)&&r[12]==2,"Queries incomplete");
 r[2]=w(0x8c660aac);r[1]=w(r[15]+4);c.pr=0x8c660a4a;r[0]=w(r[14]+8);need(r[2]==0x8c10e67c&&r[0]==16&&r[1]==0x4000,"Original divide inputs differ");cycles+=5;divide();need(r[0]==0x400,"Record quotient differs");
 r[0]<<=2;r[3]=1;t(r[0]>>31);r[0]<<=1;put(m,r[14]+48,r[0]);wb(r[14],r[3]);cycles+=5;r[4]=r[15]+8;c.pr=0x8c660a5a;cycles+=4;
 r[0]=w(r[4]);r[2]=c.sr;r[1]=static_cast<int16_t>(w(0x8c660896));r[0]&=15;r[0]<<=2;r[2]&=r[1];r[0]<<=2;r[0]|=r[2];c.sr=r[0];c.pc=c.pr;cycles+=16;
 r[0]=r[14];r[15]+=12;c.pr=pop();for(unsigned i=10;i<=13;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();cycles+=10;need(c.pc==0x8c647f84&&r[0]==0x8c8a8c90&&cycles==278,"Outer constructor return differs");
 }
};
namespace dcrecomp_generated {void resident_buffer_query_register(DCRuntime&);void resident_channel_query_register(DCRuntime&);void resident_resource_id_register(DCRuntime&);void resident_second_constructor_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66c3ac,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);resident_buffer_query_register(rt);resident_channel_query_register(rt);resident_resource_id_register(rt);resident_second_constructor_register(rt);need(rt.targets.contains(0x8c10e67c)&&rt.targets.contains(0x8c6607b8),"Retained divide/IRQ restore missing");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;auto expected_ram=ram;QueryReturnModel m{{{c,expected_ram}}};m.complete();auto expected=m.c;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c66c3ac);same(m.adapter_return,c);call_recompiled(c,rt,0x8c660a2e);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==4,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":4,"cycles":278,"return_pc":"8C647F84","outer_constructor_returned":true,"new_instructions":0,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original query chain and outer constructor return:4controls,278cycles,to647F84\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
