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

static void guard_stack(auto& m,unsigned table,unsigned sp,unsigned ret,unsigned r14,unsigned r13,unsigned r12,unsigned r11,bool acquire){
 put(m,sp-4,r14);put(m,sp-8,r13);put(m,sp-12,r12);put(m,sp-16,r11);put(m,sp-20,ret);
 if(acquire){put(m,sp-24,156);put(m,sp-28,0x8c110ad6);put(m,sp-32,0);}
 else{put(m,sp-24,0x8c110a5a);put(m,sp-28,table+160);put(m,sp-32,0);}
 put(m,0x8c7b0b40,acquire?1:0);
}
static void fifo_stack(auto& m,unsigned table,unsigned sp,unsigned ret,unsigned r14,unsigned r13,bool write_query){
 put(m,sp-4,r14);put(m,sp-8,r13);put(m,sp-12,ret);put(m,sp-16,1);
 unsigned counter=table+(write_query?0x110:0x10c);put(m,counter,word(m,counter)+1);
}
static void original_word_write(auto&m,auto&sound,SH4Context&c){
 const auto original=c;const unsigned sp=c.r[15],address=c.r[4],value=c.r[5],table=word(m,0x8c5ff84c),mask=(c.sr>>4)&15;
 need(address<0x200000&&word(m,table+0xd4)==1,"Only reached idle G2 word path modeled");
 for(unsigned i=0;i<4;++i){unsigned channel=word(m,table+4+52*i);need((word(m,table)+32*channel)==0xa05f7800+32*i,"Unexpected G2 register layout");}
 put(m,sp-4,c.pr);put(m,sp-8,value);put(m,sp-12,address);put(m,sp-16,0);put(m,sp-20,0);
 const unsigned writesp=sp-20;for(unsigned i=0;i<7;++i)put(m,writesp-4-4*i,c.r[14-i]);put(m,writesp-32,0x8c66af68);
 const unsigned local=writesp-48;put(m,local,0xa0800000+address);put(m,local+4,4);put(m,local+12,mask);
 guard_stack(m,table,local,0x8c10f40c,c.r[14],1,c.r[12],c.r[11],true);
 fifo_stack(m,table,local,0x8c10f412,c.r[14],1,false);
 put(m,local,sp-8);put(m,local+4,0xa0800000+address);put(m,local+8,0);
 fifo_stack(m,table,local,0x8c10f4ce,7,1,false);fifo_stack(m,table,local,0x8c10f4d2,7,1,true);
 put(sound,address,value);guard_stack(m,table,local,0x8c10f4f4,7,1,0x8c110b00,0x8c1107cc,false);
 c.pc=c.pr;c.r[0]=0;c.r[1]=0xd4;c.sr|=1;c.r[2]=c.sr&0xffffff0f;c.r[3]=0xffffff0f;c.r[4]=0xa05f7860;c.r[5]=local-32;c.r[6]=4;c.r[7]=1;
}
namespace dcrecomp_generated {void resident_buffer_bind_register(DCRuntime&);void resident_sound_word_register(DCRuntime&);}
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto initial=state(argv[2]);const auto device=read_sq(argv[5]);const std::string out=argv[9];need(initial.pc==0x8c66b9ee,"Wrong actual return boundary");
 unsigned controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 for(unsigned alternate:{0u,1u})for(unsigned host_zero:{0u,1u})for(unsigned guest_zero:{0u,1u}){
  const int host_mode=host_zero?FE_TOWARDZERO:FE_TONEAREST;need(std::fesetround(host_mode)==0,"Host setup failed");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);
  need(!rt.targets.contains(0x8c65e470),"Work continuation already registered");auto count=rt.targets.size();resident_buffer_bind_register(rt);need(rt.targets.size()==count+39,"Work entries differ");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);auto c=initial;c.fpscr=0x40000|guest_zero;
  need(!rt.targets.contains(0x8c66af40)&&rt.targets.contains(0x8c66e97c),"Original/template inventory differs");auto extra=rt.targets.size();resident_sound_word_register(rt);need(rt.targets.size()==extra+4,"Original word-writer entries differ");
  if(alternate){c.r[5]=14;c.r[6]=0x1f7f80;}
  auto expected_ram=rt.main_ram;auto expected=c;auto&r=expected.r;
  for(unsigned i=14;i>=11;--i){r[15]-=4;put(expected_ram,r[15],r[i]);}r[15]-=4;put(expected_ram,r[15],expected.pr);r[15]-=8;
  r[3]=word(expected_ram,0x8c65e51c);put(expected_ram,r[15],r[6]);put(expected_ram,r[15]+4,r[7]);r[6]=word(expected_ram,r[3]);need(r[6]!=0,"Captured global uninitialized");r[12]=0;r[7]=static_cast<unsigned>(static_cast<int>(static_cast<int8_t>(r[5])));need(r[7]<16&&r[4]==0x52535053,"Only reached SPRS path modeled");
  r[11]=r[7]*8;r[13]=1;r[14]=16;r[0]=r[4];r[1]=word(expected_ram,0x8c65e530);need(r[1]==r[0],"SPRS literal differs");expected.sr&=~1u;
  auto half=[&](unsigned a){return static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(word(expected_ram,a))));};
  r[0]=half(0x8c65e5e4);r[3]=half(0x8c65e5ea);r[13]=word(expected_ram,r[6]+r[0]);r[14]=half(0x8c65e5ec);r[13]+=r[3]+r[11];r[14]+=r[6]+r[11];r[3]=word(expected_ram,0x8c65e604);r[5]=word(expected_ram,r[15]);r[4]=r[13];expected.pr=0x8c65e5c2;expected.pc=0x8c65e5ba;need(r[3]==0x8c66af40,"Next original write helper differs");
  auto expected_sound=sound;original_word_write(expected_ram,expected_sound,expected);
  r[3]=word(expected_ram,0x8c65e604);r[4]=r[13]+4;r[5]=word(expected_ram,r[15]+4);expected.pr=0x8c65e5cc;original_word_write(expected_ram,expected_sound,expected);
  r[2]=word(expected_ram,r[15]);put(expected_ram,r[14],r[2]);r[3]=word(expected_ram,r[15]+4);put(expected_ram,r[14]+4,r[3]);r[0]=r[12];r[15]+=8;expected.pr=word(expected_ram,r[15]);r[15]+=4;for(unsigned i=11;i<=14;++i){r[i]=word(expected_ram,r[15]);r[15]+=4;}expected.pc=expected.pr;
  auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,1331);const auto vram=rt.vram;
  call_recompiled(c,rt,0x8c65e470);need(rt.relocated_code_matches==0,"Original writer unexpectedly used template fallback");same(expected,c);need(expected_ram==rt.main_ram,"Work return RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");
  need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"SQ differs");need(rt.aica_ram==expected_sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&!rt.g2_dma_runs&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected device effects");need(std::fegetround()==host_mode,"Host mode leaked");
  if(!alternate&&!host_zero&&guest_zero){save_ram(rt.aica_ram,out+"/observed-aica.bin");save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());}++controls;
 }
 need(controls==8,"Complete buffer binding coverage differs");std::ofstream(out+"/result.json")<<R"({"passed":true,"controls":8,"cycles":1331,"return_pc":"8C66BA16","result":0,"audio_word_stores":2,"whole_original_buffer_return":true,"relocation_matches":0,"new_original_writer_instructions":25,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"PASS complete original SPRS buffer binding:8controls,1331cycles,two AICA words,return66BA16\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
