// Original callback installation reuses the retained runtime helper. Null callbacks restore original defaults through retained644EF0.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void movie_callback_install_register(DCRuntime&);}

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}







struct InstallModel : RequestModel {
 unsigned sw(unsigned a){return unsigned(std::int32_t(std::int16_t(m[a&0xffffff]|(unsigned(m[(a+1)&0xffffff])<<8))));}
 void prefix(){auto&r=c.r;push(c.pr);r[15]-=8;r[3]=w(0x8c645400);r[7]=r[5];put(m,r[15],r[4]);put(m,r[15]+4,r[5]);r[5]=7;r[6]=w(r[15]);c.pr=0x8c6453b6;r[4]=w(r[3]);c.pc=0x8c645002;cycles+=11;}
 void removal(){auto&r=c.r;const auto index=r[5];r[0]=index;bool matched=false;cycles+=1;
  for(unsigned i=0;i<9;++i){t(r[0]==i);cycles+=2;if(c.sr&1){matched=true;break;}}
  if(matched){if(index>=5)cycles+=2;
   const unsigned offset_literal=index<5?0x8c644fc0+index*2:0x8c645040+(index-5)*2;
   const unsigned callback_literal=index<5?0x8c644fcc+index*4:0x8c64504c+(index-5)*4;
   r[0]=sw(offset_literal);const unsigned reg=index==0?3:(index&1)?1:2;r[reg]=w(callback_literal);
   put(m,r[4]+r[0],r[reg]);cycles+=index==0?4:index==7?4:index==8?3:(index&1)?6:5;
  }else cycles+=2;
  c.pc=c.pr;cycles+=3;
 }
 bool helper(){auto&r=c.r;push(c.pr);r[15]-=8;r[0]=c.sr;r[3]=sw(0x8c645048);r[0]>>=2;r[0]>>=2;r[0]&=15;put(m,r[15],r[0]);r[0]=c.sr;r[0]&=r[3];r[0]|=240;c.sr=r[0]&0x700083f3;t(r[6]==0);cycles+=19;
  if(c.sr&1){c.pr=0x8c645064;c.pc=0x8c645060;cycles+=3;removal();}else{
  r[3]=sw(0x8c64504a);r[5]<<=2;t((r[5]>>31)!=0);r[5]<<=1;r[2]=sw(0x8c64504a);r[3]+=r[4];put(m,r[15]+4,r[5]);r[5]+=r[3];r[2]+=r[4];put(m,r[5],r[6]);r[3]=w(r[15]+4);r[2]+=r[3];put(m,r[2]+4,r[7]);cycles+=13;}
  r[0]=w(r[15]);r[3]=c.sr;r[2]=sw(0x8c645100);r[0]&=15;r[0]<<=2;r[3]&=r[2];r[0]<<=2;r[0]|=r[3];c.sr=r[0]&0x700083f3;r[15]+=8;c.pr=pop();c.pc=c.pr;cycles+=18;return true;
 }
 void finish(){c.r[15]+=8;c.pr=pop();c.pc=c.pr;c.r[0]=0;cycles+=5;}
};
struct InstallPrediction {SH4Context c;std::vector<std::uint8_t> ram;unsigned cycles;};
static thread_local std::function<void(bool)> install_observer;
static thread_local RecompiledFunction real_install_helper=nullptr;
static void observed_install_helper(SH4Context&c,DCRuntime&rt){if(install_observer)install_observer(false);real_install_helper(c,rt);if(install_observer)install_observer(true);}
int main(int argc,char**argv){try{
 need(argc==9,"usage: callback-install RAM STATE TIMERS SQ SQSTATE FLASH AICA OUTPUT");const std::string out=argv[8];unsigned parent_controls=0,helper_controls=0,null_returns=0;
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto device=read_sq(argv[5]);const auto captured=state(argv[2]);
 auto check=[&](bool parent,unsigned index,unsigned callback,unsigned mask,unsigned alias,unsigned bank,bool capture){auto c=captured;auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);movie_callback_install_register(rt);need(rt.targets.count(0x8c644ef0)&&rt.targets.count(0x8c645002),"Retained callback helpers must be registered");
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);
  if(!capture){unsigned base=alias?0xac000000:0x8c000000,table=base+0xedc000,fn=callback==0?0:callback==1?base+0xecfa68:base+0x112234,arg=(mask&1)?0xa5a55a5a:0;
   c.r[15]=base+0xefe000;c.pr=base+0x112238;c.r[4]=parent?fn:table;c.r[5]=parent?arg:index;c.r[6]=fn;c.r[7]=arg;put(rt.main_ram,0x8c67fd44,table);
   std::fill(rt.main_ram.begin()+0xedc000,rt.main_ram.begin()+0xedd000,0xa5);c.sr=(c.sr&~0x200000f1u)|(mask<<4)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}
  }
  const auto before=c;const bool null_case=parent?c.r[4]==0:c.r[6]==0;auto expected=rt.main_ram;InstallModel m{{c,expected}};std::vector<InstallPrediction> phases;auto remember=[&]{phases.push_back({m.c,expected,m.cycles});};
  if(parent){m.prefix();if(capture)remember();}const bool returned=m.helper();if(capture)remember();if(parent&&returned){m.finish();if(capture)remember();}
  const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  const unsigned removal_cycles[]={10,14,15,18,19,24,25,26,27};const unsigned expected_cycles=null_case?(parent?82:40+(index<9?removal_cycles[index]:24)):(parent?66:50);need(m.cycles==expected_cycles,"Independent callback timing differs");
  if(capture){need(returned&&phases.size()==3,"Captured callback must complete");for(unsigned i=0;i<phases.size();++i){const auto&p=phases[i];auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}}
  unsigned seen=0;auto compare_phase=[&](unsigned i){need(i==seen&&i<phases.size(),"Callback phase order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Callback boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt)&&rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram,"Callback boundary devices differ");++seen;};
  if(capture){install_observer=[&](bool after){compare_phase(after?1:0);};real_install_helper=rt.targets.at(0x8c645002);rt.register_target(0x8c645002,observed_install_helper);}
  call_recompiled(c,rt,parent?(alias?0xac6453a2:0x8c6453a2):(alias?0xac645002:0x8c645002));if(null_case)++null_returns;
  if(capture){compare_phase(2);need(seen==3,"Missing callback phases");install_observer={};}
  same(m.c,c);need(expected==rt.main_ram,"Complete callback RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Callback timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Callback SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected callback device effects");
  if(returned){need(c.r[15]==before.r[15]&&c.pr==before.pr&&c.pc==before.pr,"Callback caller state not restored");need((c.sr&0xf0)==(before.sr&0xf0),"Callback interrupt mask not restored");}
  if(capture){need(phases[0].cycles==11&&phases[1].cycles==61&&phases[2].cycles==66&&c.r[0]==0,"Captured callback boundaries differ");need(word(rt.main_ram,0x8c8a723c)==0x8cecfa68&&word(rt.main_ram,0x8c8a7240)==0,"Captured callback installation differs");auto stem=out+"/install-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else if(parent)++parent_controls;else ++helper_controls;
 };
 for(unsigned index:{0u,1u,2u,3u,4u,5u,6u,7u,8u,9u,15u,0x20000007u})for(unsigned callback:{0u,1u})for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(false,index,callback,mask,alias,bank,false);
 for(unsigned callback:{0u,1u,2u})for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(true,7,callback,mask,alias,bank,false);
 check(true,7,1,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"helper_controls\":"<<helper_controls<<",\"parent_controls\":"<<parent_controls<<",\"null_returns\":"<<null_returns<<",\"nonnull_helper_cycles\":50,\"captured_connected_cycles\":[11,61,66],\"null_slot7_helper_cycles\":66,\"null_parent_cycles\":82,\"interrupt_masks\":16,\"new_original_bodies\":1,\"new_original_instructions\":14,\"complete_nonnull_install_verified\":true,\"null_default_restoration_verified\":true,\"retained_helpers\":[\"0x8C645002\",\"0x8C644EF0\"],\"original_SR_instructions_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original callback installer: "<<helper_controls<<" helper,"<<parent_controls<<" parent controls;"<<null_returns<<" complete null default restorations;11/61/66cycle captured boundaries;all16 masks restored on both paths\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
