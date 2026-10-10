// Original context construction and complete slot allocation; independent state model.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void movie_allocation_register(DCRuntime&);void movie_lock_enter_register(DCRuntime&);void movie_lock_leave_register(DCRuntime&);void movie_resident_count_register(DCRuntime&);void movie_resident_buffer_register(DCRuntime&);void movie_slot_search_register(DCRuntime&);void movie_slot_status_register(DCRuntime&);void movie_slot_mark_register(DCRuntime&);void movie_slot_argument_register(DCRuntime&);}

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}








struct SlotModel : RequestModel {
 void buffer(){auto&r=c.r;r[2]=w(0x8c5ffa38);r[0]=r[4];r[0]<<=2;r[3]=w(r[2]);t((r[0]>>31)!=0);r[0]<<=1;r[0]+=r[3];c.pc=c.pr;cycles+=8;}
 void count(){c.r[1]=w(0x8c5ffa3c);c.r[0]=w(c.r[1]);c.pc=c.pr;cycles+=4;}
 void status(){push(c.pr);c.pr=0x8c5ffa32;cycles+=4;buffer();c.pr=pop();c.pc=c.pr;c.r[0]=w(c.r[0]);cycles+=4;}
 void search(){auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);c.pr=0x8c5ff98a;cycles+=6;count();r[13]=r[0];t(std::int32_t(r[13])>0);r[14]=0;cycles+=4;bool found=false;
  if(c.sr&1){do{c.pr=0x8c5ff996;r[4]=r[14];cycles+=3;status();t(r[0]==0);cycles+=2;if(c.sr&1){r[0]=r[14];cycles+=2;found=true;break;}++r[14];t(std::int32_t(r[14])>=std::int32_t(r[13]));cycles+=3;}while(!(c.sr&1));}
  if(!found){r[0]=0xffffffff;cycles+=1;}c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=5;
 }
 void enter(){auto&r=c.r;r[0]=c.sr;r[3]=0xffffff0f;r[0]>>=4;r[0]&=15;put(m,r[4],r[0]);r[0]=c.sr;r[0]&=r[3];r[0]|=240;c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=18;}
 void leave(){auto&r=c.r;r[0]=w(r[4]);r[2]=c.sr;r[1]=0xffffff0f;r[0]&=15;r[0]<<=2;r[2]&=r[1];r[0]<<=2;r[0]|=r[2];c.sr=r[0]&0x700083f3;c.pc=c.pr;cycles+=16;}
 void parent(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[3]=w(0x8c5ffa44);put(m,r[15]+4,r[4]);r[0]=w(r[3]);t(r[0]==0xffffffff);r[14]=0;cycles+=9;
  if(!(c.sr&1)){r[0]=0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;return;}
  c.pr=0x8c5ff952;r[4]=r[15];cycles+=3;enter();c.pr=0x8c5ff956;cycles+=3;search();r[4]=r[0];t(std::int32_t(r[4])>=0);cycles+=3;
  if(c.sr&1){c.pr=0x8c5ff960;cycles+=3;buffer();r[5]=0xffffffff;r[14]=r[0];cycles+=2;c.pr=0x8c5ff968;r[4]=r[0];cycles+=3;put(m,r[4],r[5]);c.pc=c.pr;cycles+=3;
   r[5]=w(r[15]+4);c.pr=0x8c5ff96e;r[4]=r[14];cycles+=4;put(m,r[4]+4,r[5]);c.pc=c.pr;cycles+=3;r[2]=w(0x8c5ffa44);put(m,r[2],r[14]);cycles+=2;}
  c.pr=0x8c5ff976;r[4]=r[15];cycles+=3;leave();r[0]=r[14];r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
};
namespace dcrecomp_generated {void movie_stream_register(DCRuntime&);void movie_context_register(DCRuntime&);}
struct StreamModel : SlotModel {
 std::function<void(unsigned,const StreamModel&)> observe;
 void context(){auto&r=c.r;r[0]=r[4]+r[5];r[1]=w(0x8c5ffacc);r[2]=0;r[3]=0;cycles+=5;
  do{r[0]-=4;put(m,r[0],r[2]);++r[3];t(r[3]==r[1]);cycles+=4;}while(!(c.sr&1));
  r[1]=w(0x8c5ffad0);r[0]-=4;put(m,r[0],r[1]);push(r[0]);r[2]=r[4]+r[5];r[0]=r[2];r[1]=w(0x8c5ffad4);r[0]-=r[1];put(m,r[0],r[6]);
  r[0]=r[2];r[1]=w(0x8c5ffad8);r[0]-=r[1];put(m,r[0],r[7]);r[0]=r[2];r[1]=w(0x8c5ffadc);r[0]-=r[1];r[1]=c.sr;put(m,r[0],r[1]);r[0]=pop();c.pc=c.pr;cycles+=23;
 }
 void stream(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=12;put(m,r[15],r[5]);put(m,r[15]+8,r[7]);r[3]=w(r[15]);t(r[3]==0);r[14]=0;cycles+=9;
  bool invalid=c.sr&1;
  if(!invalid){t(r[4]==0);cycles+=2;invalid=c.sr&1;}
  if(!invalid){t(r[6]==0);cycles+=2;invalid=c.sr&1;}
  if(invalid){r[0]=0;r[15]+=12;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;return;}
  r[3]=w(0x8c5ffa54);r[7]=w(r[15]+8);c.pr=0x8c5ff8fe;r[5]=w(r[15]);cycles+=5;c.pc=0x8c5ffa90;if(observe)observe(0,*this);
  context();if(observe)observe(1,*this);
  r[4]=r[15];put(m,r[15],r[0]);c.pr=0x8c5ff906;r[4]+=4;cycles+=5;enter();c.pr=0x8c5ff90a;cycles+=3;c.pc=0x8c5ff980;if(observe)observe(2,*this);
  search();r[4]=r[0];t(std::int32_t(r[4])>=0);cycles+=3;
  if(c.sr&1){c.pr=0x8c5ff914;cycles+=3;buffer();r[5]=w(r[15]);r[14]=r[0];cycles+=2;c.pr=0x8c5ff91c;r[4]=r[14];cycles+=3;put(m,r[4],r[5]);c.pc=c.pr;cycles+=3;
   r[5]=w(r[15]+8);c.pr=0x8c5ff922;r[4]=r[14];cycles+=4;put(m,r[4]+4,r[5]);c.pc=c.pr;cycles+=3;}
  r[4]=r[15];c.pr=0x8c5ff928;r[4]+=4;cycles+=4;leave();r[0]=r[14];r[15]+=12;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;if(observe)observe(3,*this);
 }
};
struct Prediction {SH4Context c;std::vector<std::uint8_t> ram;unsigned cycles;};
static thread_local std::function<void(unsigned)> boundary;
static thread_local RecompiledFunction real_context=nullptr,real_search=nullptr;
static void observed_context(SH4Context&c,DCRuntime&rt){if(boundary)boundary(0);real_context(c,rt);if(boundary)boundary(1);}
static void observed_search(SH4Context&c,DCRuntime&rt){if(boundary)boundary(2);real_search(c,rt);}
int main(int argc,char**argv){try{
 need(argc==9,"usage: stream RAM STATE TIMERS SQ SQSTATE FLASH AICA OUTPUT");const std::string out=argv[8];unsigned parent_controls=0,helper_controls=0;
 const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]);const auto device=read_sq(argv[5]);const auto captured=state(argv[2]);
 auto check=[&](bool parent,unsigned pattern,unsigned mask,unsigned alias,unsigned bank,bool capture){auto c=captured;auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
  load_embedded_elf_image(rt);registers(rt);movie_lock_enter_register(rt);movie_lock_leave_register(rt);movie_resident_count_register(rt);movie_resident_buffer_register(rt);movie_slot_search_register(rt);movie_slot_status_register(rt);movie_slot_mark_register(rt);movie_slot_argument_register(rt);movie_stream_register(rt);movie_context_register(rt);
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);
  if(!capture){const unsigned base=alias?0xac000000:0x8c000000,slots=base+0xedf000;c.r[15]=base+0xefe000;c.pr=base+0x112234;c.r[4]=base+0xee4000;c.r[5]=pattern&1?0x104:0x4000;c.r[6]=alias?0xacecf9f2:0x8cecf9f2;c.r[7]=(mask&1)?0xa5a55a5a:0;c.r[13]=0xdead000d;c.r[14]=0xdead000e;
   c.sr=(c.sr&~0x200000f1u)|(mask<<4)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}
   std::fill(rt.main_ram.begin()+0xee4000,rt.main_ram.begin()+0xee8000,0xa5);
   if(parent){if(pattern==0)c.r[5]=0;if(pattern==1)c.r[4]=0;if(pattern==2)c.r[6]=0;}
   put(rt.main_ram,0x8c892ba0,slots);put(rt.main_ram,0x8c677140,pattern==3?0:pattern==4?0xffffffffu:5);
   for(unsigned i=0;i<5;++i){const bool free=(pattern==5&&i==0)||(pattern==6&&i==1)||(pattern==7&&i==4)||(pattern==9&&(i==1||i==3));put(rt.main_ram,slots+i*8,free?0:0x100+i);put(rt.main_ram,slots+i*8+4,0xcafebe00+i);}
  }
  const auto before=c;auto expected=rt.main_ram;StreamModel m{{{c,expected}}};std::vector<Prediction> phases;
  if(capture)m.observe=[&](unsigned i,const StreamModel&model){need(i==phases.size(),"Prediction phase order");phases.push_back({model.c,model.m,model.cycles});};
  if(parent)m.stream();else m.context();const auto initial_timers=rt.tmu;auto timers=initial_timers;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  const unsigned index=pattern==5?0:pattern==7?4:1;const unsigned predicted=capture?449:!parent?284:pattern<3?15+2*pattern:pattern<5?377:pattern==8?497:425+24*index;
  need(m.cycles==predicted,"Independent stream timing formula differs");
  if(capture)for(unsigned i=0;i<phases.size();++i){const auto&p=phases[i];auto t=initial_timers;tick(t,start,p.cycles);auto stem=out+"/phase"+std::to_string(i)+"-expected";save_ram(p.ram,stem+"-ram.bin");save_state(p.c,stem+"-state.txt");save_timers(t,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
  unsigned seen=0;
  auto compare_phase=[&](unsigned i){need(i==seen&&i<phases.size(),"Observed phase order");const auto&p=phases[i];same(p.c,c);need(p.ram==rt.main_ram,"Stream boundary RAM differs");auto t=initial_timers;tick(t,start,p.cycles);for(unsigned j=0;j<3;++j)timer_same(t[j],rt.tmu[j]);
   need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt)&&rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram,"Stream boundary devices differ");++seen;};
  if(capture){boundary=compare_phase;real_context=rt.targets.at(0x8c5ffa90);real_search=rt.targets.at(0x8c5ff980);rt.register_target(0x8c5ffa90,observed_context);rt.register_target(0x8c5ff980,observed_search);}
  call_recompiled(c,rt,parent?(alias?0xac5ff8d2:0x8c5ff8d2):(alias?0xac5ffa90:0x8c5ffa90));
  if(capture){compare_phase(3);need(seen==4,"Missing stream phases");boundary={};}
  same(m.c,c);need(expected==rt.main_ram,"Complete stream RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Stream timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Stream SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected stream device effects");
  need(c.r[15]==before.r[15]&&c.pr==before.pr&&c.pc==before.pr&&c.r[13]==before.r[13]&&c.r[14]==before.r[14],"Stream caller state not restored");need((c.sr&0xf0)==(before.sr&0xf0),"Stream interrupt mask not restored");
  if(capture){need(m.cycles==449&&phases[0].cycles==18&&phases[1].cycles==302&&phases[2].cycles==328,"Captured stream cycles differ");need(c.r[0]==0x8cede190,"Captured second slot differs");auto stem=out+"/stream-observed";save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else if(parent)++parent_controls;else ++helper_controls;
 };
 for(unsigned pattern=0;pattern<2;++pattern)for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(false,pattern,mask,alias,bank,false);
 for(unsigned pattern=0;pattern<10;++pattern)for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(true,pattern,mask,alias,bank,false);
 check(true,0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"helper_controls\":"<<helper_controls<<",\"parent_controls\":"<<parent_controls<<",\"helper_cycles\":284,\"captured_connected_cycles\":[18,302,328,449],\"parent_exhausted_cycles\":497,\"parent_nonpositive_cycles\":377,\"invalid_input_cycles\":[15,17,19],\"interrupt_masks\":16,\"new_original_bodies\":2,\"new_original_instructions\":78,\"complete_context_verified\":true,\"complete_stream_allocation_return_verified\":true,\"real_helpers_and_locks_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original context/stream allocation: "<<helper_controls<<" helper,"<<parent_controls<<" parent controls;18/302/328/449cycle captured boundaries;all16 masks restored;real context/search/getters/setters/locks\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
