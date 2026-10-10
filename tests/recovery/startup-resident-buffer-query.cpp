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
namespace dcrecomp_generated {void resident_buffer_query_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66c39e,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned id:{14u,15u,16u,255u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c65e640),"Buffer query already registered");resident_buffer_query_register(rt);need(rt.targets.size()==count+38,"Original buffer query entries differ");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;c.r[5]=id;auto expected_ram=ram;BufferQueryModel m{{c,expected_ram}};m.query();need(m.cycles==(id<16?61u:id==16?53u:34u)&&m.c.r[0]==(id<16?0u:0x08000003u),"Query status or timing differs");auto expected=m.c;
  if(id<16){need(word(expected_ram,c.r[6])==(id==15?0x1fbfc0u:0x1f7f80u)&&word(expected_ram,c.r[7])==0x4000,"Original buffer address/size differs");}else{need(word(expected_ram,c.r[6])==word(ram,c.r[6])&&word(expected_ram,c.r[7])==word(ram,c.r[7]),"Invalid index wrote outputs");}
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65e640);need(c.pc==initial.pr&&c.r[15]==initial.r[15],"Query frame did not return");need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(id==15&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"captured_cycles":61,"invalid_index_cycles":[53,34],"return_pc":"8C66C3AC","new_instructions":119,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original buffer query:16controls,61/53/34cycles,original address/size and invalid indices\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
