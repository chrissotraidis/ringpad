// Original movie allocation prefix and interrupt-mask enter/leave contracts.
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
int main(int argc,char**argv){try{
 need(argc==16,"usage: slots SEARCH_RAM STATE TIMERS SQ SQSTATE FLASH AICA PARENT_RAM STATE TIMERS SQ SQSTATE FLASH AICA OUTPUT");
 const std::string out=argv[15];unsigned search_controls=0,parent_controls=0;
 auto check=[&](bool parent,unsigned pattern,unsigned mask,unsigned alias,unsigned bank,bool capture){const unsigned a=parent?8:1;const auto ram=read(argv[a]),sq=read(argv[a+3]),flash=read(argv[a+5]),sound=read(argv[a+6]);auto c=state(argv[a+1]);const auto device=read_sq(argv[a+4]);auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
  load_embedded_elf_image(rt);registers(rt);movie_allocation_register(rt);movie_lock_enter_register(rt);movie_lock_leave_register(rt);movie_resident_count_register(rt);movie_resident_buffer_register(rt);movie_slot_search_register(rt);movie_slot_status_register(rt);movie_slot_mark_register(rt);movie_slot_argument_register(rt);
  rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[a+2]);restore_sq(rt,sq,device);
  if(!capture){const unsigned buffer=alias?0xacedf000:0x8cedf000;put(rt.main_ram,0x8c892ba0,buffer);const unsigned n=pattern==0?0:pattern==1?0xffffffffu:5;put(rt.main_ram,0x8c677140,n);put(rt.main_ram,0x8c677144,pattern==7?1:0xffffffffu);
   for(unsigned i=0;i<5;++i){bool free=(pattern==2&&i==0)||(pattern==3&&i==2)||(pattern==4&&i==4)||(pattern==6&&(i==1||i==3))||pattern==7;put(rt.main_ram,buffer+i*8,free?0:0x100u+i);put(rt.main_ram,buffer+i*8+4,0xcafebe00u+i);}
   c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.r[4]=(mask&1)?0xa5a55a5a:0;c.r[13]=0xdead000d;c.r[14]=0xdead000e;c.sr=(c.sr&~0x200000f1u)|(mask<<4)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);for(unsigned i=0;i<16;++i){c.fr_bits[i]=0x7fc01234u^(i<<16);c.xf_bits[i]=0x80000000u+i;}}
  const auto before=c;auto expected=rt.main_ram;SlotModel m{{c,expected}};if(parent)m.parent();else m.search();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,m.cycles);const auto vram=rt.vram;
  const unsigned free_index=pattern==2?0:pattern==3?2:pattern==4?4:pattern==6?1:0;const unsigned predicted=capture?(parent?131:42):parent?(pattern==7?15:pattern<2?81:pattern==5?201:131+24*free_index):(pattern<2?20:pattern==5?140:42+24*free_index);need(m.cycles==predicted,"Independent slot timing formula differs");
  if(capture){auto stem=out+(parent?"/phase1-expected":"/phase0-expected");save_ram(expected,stem+"-ram.bin");save_state(m.c,stem+"-state.txt");save_timers(timers,start,tocr,stem+"-timers.txt");save_sq(sq,device,stem);}
  call_recompiled(c,rt,parent?(alias?0xac5ff932:0x8c5ff932):(alias?0xac5ff980:0x8c5ff980));same(m.c,c);need(expected==rt.main_ram,"Complete slot RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Slot timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Slot SQ/device differs");need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty()&&!rt.code_module_dispatch_depth,"Unexpected slot device effects");
  need(c.r[15]==before.r[15]&&c.pr==before.pr&&c.pc==before.pr&&c.r[13]==before.r[13]&&c.r[14]==before.r[14],"Slot caller state not restored");need((c.sr&0xf0)==(before.sr&0xf0),"Allocation interrupt mask not restored");
  if(capture){auto stem=out+(parent?"/allocation-observed":"/search-observed");save_ram(rt.main_ram,stem+"-ram.bin");save_state(c,stem+"-state.txt");save_timers(rt.tmu,start,tocr,stem+"-timers.txt");save_sq(rt.store_queues,sqfields(rt),stem);}else if(parent)++parent_controls;else ++search_controls;
 };
 for(unsigned pattern=0;pattern<7;++pattern)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(false,pattern,15,alias,bank,false);
 for(unsigned pattern=0;pattern<8;++pattern)for(unsigned mask=0;mask<16;++mask)for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(true,pattern,mask,alias,bank,false);
 check(false,0,0,0,0,true);check(true,0,0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"search_controls\":"<<search_controls<<",\"parent_controls\":"<<parent_controls<<",\"captured_search_cycles\":42,\"captured_parent_cycles\":131,\"parent_exhausted_cycles\":201,\"parent_nonpositive_cycles\":81,\"parent_busy_cycles\":15,\"interrupt_masks\":16,\"new_original_bodies\":4,\"new_original_instructions\":33,\"complete_search_return_verified\":true,\"complete_allocation_return_verified\":true,\"real_helpers_and_locks_executed\":true,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original slot search/allocation: "<<search_controls<<" search,"<<parent_controls<<" parent controls;42/131cycle captured returns;all16 masks restored;real search/getters/setters/locks\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
