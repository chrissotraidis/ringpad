// Original644492 tail transfer, stopping before registered657AA8 execution.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void sevenbyte_register(DCRuntime&);void word_copy_register(DCRuntime&);void payload_validator_register(DCRuntime&);void block_iteration_register(DCRuntime&);void linked_blocks_register(DCRuntime&);void next_block_register(DCRuntime&);void timer_callback_register(DCRuntime&);void timer_tail_register(DCRuntime&);void record_update_register(DCRuntime&);void record_query_register(DCRuntime&);void record_reset_register(DCRuntime&);void record_counter_register(DCRuntime&);void record_buffer_register(DCRuntime&);void bit_expand_register(DCRuntime&);void rectangle_copy_register(DCRuntime&);void record_init_register(DCRuntime&);void record_info_register(DCRuntime&);void info_wrapper_register(DCRuntime&);void timestamp_register(DCRuntime&);void bcd_register(DCRuntime&);void count_blocks_register(DCRuntime&);void global_reset_register(DCRuntime&);void global_setup_register(DCRuntime&);void arena_init_register(DCRuntime&);void global_setter_register(DCRuntime&);void global_word_copy_register(DCRuntime&);void frame_dispatch_register(DCRuntime&);void input_status_register(DCRuntime&);void frame_adjust_register(DCRuntime&);void frame_quad_register(DCRuntime&);void quad_state_register(DCRuntime&);void quad_header_register(DCRuntime&);void quad_strip_register(DCRuntime&);void strip_writer_register(DCRuntime&);void record_state2_register(DCRuntime&);void record_ready_register(DCRuntime&);void ready_query_register(DCRuntime&);void ready_setter_register(DCRuntime&);void frame_sprites_register(DCRuntime&);void sprite_header_register(DCRuntime&);void movie_service_register(DCRuntime&);void movie_setup_register(DCRuntime&);void movie_tail_register(DCRuntime&);void mode_loader_register(DCRuntime&);void loader_owner_register(DCRuntime&);void file_transfer_register(DCRuntime&);}
using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}



struct TailPrefix : RequestModel {
 void prefix(){c.r[3]=w(0x8c6444ec);c.r[4]=w(0x8c6444c0);need(c.r[3]==0x8c657aa8&&c.r[4]==0x8c8947e4,"Unexpected tail target or argument");c.pc=c.r[3];cycles=5;}
};
struct EntryBoundary {};
static RecompiledFunction real_first;
static bool first_seen=false;
static void watched_first(SH4Context&c,DCRuntime&rt){first_seen=true;throw EntryBoundary{};}
int main(int argc,char**argv){try{
 need(argc==8,"usage: movie-tail RAM STATE SQ SQSTATE FLASH AICA OUTPUT");
 const auto ram=read(argv[1]),sq=read(argv[3]),flash=read(argv[5]),sound=read(argv[6]);const auto initial=state(argv[2]);const auto device=read_sq(argv[4]);const std::string out=argv[7];unsigned controls=0;
 need(ram.size()==16777216&&sq.size()==64&&flash.size()==131072&&sound.size()==2097152,"Fixture sizes differ");
 auto check=[&](unsigned value,unsigned alias,unsigned bank,bool capture){auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
 load_embedded_elf_image(rt);registers(rt);sevenbyte_register(rt);word_copy_register(rt);payload_validator_register(rt);block_iteration_register(rt);linked_blocks_register(rt);next_block_register(rt);timer_callback_register(rt);timer_tail_register(rt);record_update_register(rt);record_query_register(rt);record_reset_register(rt);record_counter_register(rt);record_buffer_register(rt);bit_expand_register(rt);rectangle_copy_register(rt);record_init_register(rt);record_info_register(rt);info_wrapper_register(rt);timestamp_register(rt);bcd_register(rt);count_blocks_register(rt);global_reset_register(rt);global_setup_register(rt);arena_init_register(rt);global_setter_register(rt);global_word_copy_register(rt);frame_dispatch_register(rt);input_status_register(rt);frame_adjust_register(rt);frame_quad_register(rt);quad_state_register(rt);quad_header_register(rt);quad_strip_register(rt);strip_writer_register(rt);record_state2_register(rt);record_ready_register(rt);ready_query_register(rt);ready_setter_register(rt);frame_sprites_register(rt);sprite_header_register(rt);
 mode_loader_register(rt);loader_owner_register(rt);file_transfer_register(rt);movie_service_register(rt);movie_setup_register(rt);movie_tail_register(rt);rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[2]);restore_sq(rt,sq,device);auto c=initial;
 if(!capture){for(unsigned i=8;i<15;++i)c.r[i]=value^(i<<12);c.r[4]=value;c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001u:0u);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);}
 need(rt.targets.contains(0x8c657aa8),"Real first callee must remain registered");
 auto expected=rt.main_ram;TailPrefix model{{c,expected}};model.prefix();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);const auto vram=rt.vram;
 if(capture){save_ram(expected,out+"/phase0-expected-ram.bin");save_state(model.c,out+"/phase0-expected-state.txt");save_timers(timers,start,tocr,out+"/phase0-expected-timers.txt");save_sq(sq,device,out+"/phase0-expected");}
 first_seen=false;real_first=rt.targets.at(0x8c657aa8);rt.register_target(0x8c657aa8,watched_first);bool stopped=false;try{call_recompiled(c,rt,0x8c644492);}catch(const EntryBoundary&){stopped=true;}rt.register_target(0x8c657aa8,real_first);need(stopped&&first_seen&&rt.code_module_dispatch_depth==0,"Unexpected setup boundary");
 same(model.c,c);need(expected==rt.main_ram,"Loader prefix whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
 need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Loader prefix timer controls differ");need(std::equal(sq.begin(),sq.end(),rt.store_queues.begin())&&device==sqfields(rt),"Loader prefix SQ/device differs");
 need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.vram==vram&&!rt.device_clock_enabled&&!rt.aica_arm.instructions&&!rt.holly_irq_taken&&!rt.sh4_async_redirect&&rt.pvr_ta_stream.empty(),"Unexpected prefix side effects");
 if(capture){save_ram(rt.main_ram,out+"/tail-observed-ram.bin");save_state(c,out+"/tail-observed-state.txt");save_timers(rt.tmu,start,tocr,out+"/tail-observed-timers.txt");save_sq(rt.store_queues,sqfields(rt),out+"/tail-observed");}else ++controls;
 };
 for(unsigned value:{0u,1u,0x80000000u,0xffffffffu})for(unsigned alias:{0u,1u})for(unsigned bank:{0u,1u})check(value,alias,bank,false);
 check(0,0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"captured_prefix_cycles\":5,\"whole_RAM_CPU_TMU_SQ_device_verified\":true,\"intentional_stop_before_registered657AA8\":true,\"tail_callee_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original movie tail: "<<controls<<" controls;5cycle prefix verified;intentional stop before registered657AA8\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
