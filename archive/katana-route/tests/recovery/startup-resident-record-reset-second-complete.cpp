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



namespace dcrecomp_generated {void resident_record_reset_second_register(DCRuntime&);void resident_record_status_second_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c65f23e,"Wrong channel query input");
 const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};unsigned controls=0;
 for(unsigned high_bits:{0u,1u})for(unsigned alternate:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);auto count=rt.targets.size();need(!rt.targets.contains(0x8c66107a),"Object setup already registered");resident_record_reset_second_register(rt);need(rt.targets.size()==count+27,"Object setup entries differ");need(!rt.targets.contains(0x8c66184c),"Status already registered");resident_record_status_second_register(rt);need(rt.targets.size()==count+31&&rt.targets.contains(0x8c6607b8),"Status or retained IRQ restore entries differ");
  rt.main_ram=ram;rt.aica_ram=sound;rt.system_flash=flash;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;need(c.r[4]==0x8c8a8c90&&c.r[5]==0&&word(ram,0x8c67ff20)==1&&ram[(c.r[4]&0xffffff)+5]==0,"Captured reset state differs");if(alternate)c.r[5]=1;rt.main_ram[(c.r[4]&0xffffff)+1]=(high_bits?0x85:0)|(alternate?2:0);auto expected_ram=rt.main_ram;auto expected=c;auto&er=expected.r;
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
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,cycles);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c66107a);need(rt.relocated_code_matches==0,"Unexpected template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Channel query RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host,"Host mode leaked");
  if(!high_bits&&!alternate&&!host_zero&&guest_zero){save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==16,"Channel query coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":16,"captured_cycles":78,"already_active_cycles":81,"return_target":"8C6481CA","new_instructions":19,"status_matches_argument_only":true,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS original reset forwarding leaf:16controls,78/81cycles,returned6481CA\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
