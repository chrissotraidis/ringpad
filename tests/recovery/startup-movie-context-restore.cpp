// Diagnostic capture of original movie frame execution; independent verification pending.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"
#include <cfenv>

namespace dcrecomp_generated {void movie_context_entry_register(DCRuntime&);void movie_context_save_register(DCRuntime&);void movie_slot_mark_register(DCRuntime&);void movie_context_restore_register(DCRuntime&);}
using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}





#include "movie-entry/movie-entry-manifest.hpp"
#include "movie-frame-callback-manifest.hpp"
struct Prediction{SH4Context c;std::vector<std::uint8_t>ram;unsigned cycles;};
template<class F>static void rejection(F f,const char*why){try{f();}catch(const std::runtime_error&e){need(std::string(e.what()).find(why)!=std::string::npos,"Wrong module rejection");return;}throw std::runtime_error("Required activation rejection absent");}
static void save_graphics(const DCRuntime&rt,const std::string&stem){std::ofstream f(stem+"-graphics.txt");f<<std::hex;
 f<<"dmac_sar2="<<std::uint64_t(rt.dmac_sar2)<<'\n';
 f<<"dmac_dar2="<<std::uint64_t(rt.dmac_dar2)<<'\n';
 f<<"dmac_dmatcr2="<<std::uint64_t(rt.dmac_dmatcr2)<<'\n';
 f<<"dmac_chcr2="<<std::uint64_t(rt.dmac_chcr2)<<'\n';
 f<<"dmac_dmaor="<<std::uint64_t(rt.dmac_dmaor)<<'\n';
 f<<"sb_c2dstat="<<std::uint64_t(rt.sb_c2dstat)<<'\n';
 f<<"sb_c2dlen="<<std::uint64_t(rt.sb_c2dlen)<<'\n';
 f<<"sb_c2dst="<<std::uint64_t(rt.sb_c2dst)<<'\n';
 f<<"ch2_dma_runs="<<std::uint64_t(rt.ch2_dma_runs)<<'\n';
 f<<"ch2_dma_bytes="<<std::uint64_t(rt.ch2_dma_bytes)<<'\n';
 f<<"ch2_dma_ta="<<std::uint64_t(rt.ch2_dma_ta)<<'\n';
 f<<"ch2_dma_texture="<<std::uint64_t(rt.ch2_dma_texture)<<'\n';
 f<<"holly_irq_raised="<<std::uint64_t(rt.holly_irq_raised)<<'\n';
 f<<"holly_irq_taken="<<std::uint64_t(rt.holly_irq_taken)<<'\n';
 f<<"holly_last_intevt="<<std::uint64_t(rt.holly_last_intevt)<<'\n';
 f<<"holly_vblank_ack_writes="<<std::uint64_t(rt.holly_vblank_ack_writes)<<'\n';
 f<<"holly_last_ack_a_value="<<std::uint64_t(rt.holly_last_ack_a_value)<<'\n';
 f<<"holly_last_ack_a_pc="<<std::uint64_t(rt.holly_last_ack_a_pc)<<'\n';
 f<<"pvr_ta_front_state="<<std::uint64_t(rt.pvr_ta_front_state)<<'\n';
 f<<"pvr_ta_front_list="<<std::uint64_t(rt.pvr_ta_front_list)<<'\n';
 f<<"pvr_ta_capture_packets="<<std::uint64_t(rt.pvr_ta_capture_packets)<<'\n';
 f<<"pvr_ta_parse_flushes="<<std::uint64_t(rt.pvr_ta_parse_flushes)<<'\n';
 f<<"pvr_list_open="<<std::uint64_t(rt.pvr_list_open)<<'\n';
 f<<"pvr_list_init_writes="<<std::uint64_t(rt.pvr_list_init_writes)<<'\n';
 f<<"pvr_list_cont_writes="<<std::uint64_t(rt.pvr_list_cont_writes)<<'\n';
 f<<"pvr_ta_render_pass="<<std::uint64_t(rt.pvr_ta_render_pass)<<'\n';
 f<<"pvr_ta_current_pointer="<<std::uint64_t(rt.pvr_ta_current_pointer)<<'\n';
 f<<"pvr_ta_guest_pointer_advances="<<std::uint64_t(rt.pvr_ta_guest_pointer_advances)<<'\n';
 f<<"pvr_list_end_irqs="<<std::uint64_t(rt.pvr_list_end_irqs)<<'\n';
 f<<"pvr_render_done_irqs="<<std::uint64_t(rt.pvr_render_done_irqs)<<'\n';
 f<<"last_pvr_mmio_offset="<<std::uint64_t(rt.last_pvr_mmio_offset)<<'\n';
 f<<"last_pvr_mmio_value="<<std::uint64_t(rt.last_pvr_mmio_value)<<'\n';
 f<<"pvr_isp_start_writes="<<std::uint64_t(rt.pvr_isp_start_writes)<<'\n';
 f<<"ta_packets="<<std::uint64_t(rt.ta_packets)<<'\n';
 f<<"pvr_vertices="<<std::uint64_t(rt.pvr_vertices)<<'\n';
 f<<"pvr_triangles="<<std::uint64_t(rt.pvr_triangles)<<'\n';
 for(unsigned i=0;i<3;++i){f<<"holly_pending"<<i<<'='<<rt.holly_pending[i]<<'\n';f<<"holly_ack_writes"<<i<<'='<<rt.holly_ack_writes[i]<<'\n';for(unsigned j=0;j<3;++j)f<<"holly_masks"<<i<<j<<'='<<rt.holly_masks[i][j]<<'\n';}
 std::vector<std::pair<std::uint32_t,std::uint32_t>> mmio(rt.mmio32.begin(),rt.mmio32.end());std::sort(mmio.begin(),mmio.end());std::ofstream m(stem+"-mmio.txt");m<<std::hex;for(auto [a,v]:mmio)m<<a<<'='<<v<<'\n';need(bool(f)&&bool(m),"Cannot save graphics");}
int main(int argc,char**argv){try{
 need(argc==12,"usage: setup RAM STATE TIMERS SQ SQSTATE FLASH AICA MOVIE OUTPUT");const auto ram=read(argv[1]),sq=read(argv[4]),flash=read(argv[6]),sound=read(argv[7]),movie=read(argv[8]);auto initial=state(argv[2]);need(initial.pc==0x8ceb0254,"Wrong resident return boundary");const auto device=read_sq(argv[5]);const std::string out=argv[9];unsigned controls=0,activation_controls=0;const int saved=std::fegetround();struct Restore{int mode;~Restore(){std::fesetround(mode);}}restore{saved};
 const std::array<unsigned,40> addresses={0x8cecefe8,0x8cecefec,0x8ceceff0,0x8ceceff4,0x8ceceff8,0x8cedd098,0x8cedd0cc,0x8cedd0d4,0x8cedd0d8,0x8cedcf1c,0x8cedcf20,0x8cedcf24,0x8cedcf28,0x8cedcf30,0x8cedcf2c,0x8cedcf34,0x8cedd0e0,0x8cedd0dc,0x8cecf00c,0x8cecf010,0x8cecf014,0x8cecf018,0x8cecf01c,0x8cecf020,0x8cecf024,0x8cecf028,0x8cecf030,0x8cedcf38,0x8cedd08c,0x8cedd04c,0x8cedd050,0x8cedd054,0x8cedd058,0x8cedd05c,0x8cedd060,0x8cedd064,0x8cedd068,0x8cedd06c,0x8cedd078,0x8cedd088};
 const std::array<unsigned,40> values={0x8cedd114,0x1,0x5,0x2,0x2,0x1,0x1,0xd,0x1,0x1,0x5,0x2,0x2,0x2,0xea24,0x7512,0x1,0x2,0x1,0x384000,0x140,0x1e0,0x6,0x8c900000,0x2d0120,0x0,0x0,0x1,0x2d0100,0xa0,0xf0,0x140,0x1e0,0x8c9f4000,0x140,0x1e0,0x6,0x8ca64820,0x8cbb6040,0x8cbc8040};
 auto replay=movie;for(unsigned i=0;i<addresses.size();++i)put(replay,addresses[i]-0x8ceb0000,values[i]);need(std::equal(replay.begin(),replay.end(),ram.begin()+0xeb0000),"Module differs beyond audited stores");
 std::vector<CodeModuleEntry> entries(std::begin(movie_entry_entries),std::end(movie_entry_entries));for(auto e:movie_frame_callback_entries)entries.push_back(e);need(entries.size()==90,"Instance manifest differs");
 auto make=[&]{auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);need(!rt.targets.contains(0x8c5ffae0),"Context entry already bound");movie_context_entry_register(rt);need(!rt.targets.contains(0x8c5ffb70),"Context save already bound");movie_context_save_register(rt);movie_slot_mark_register(rt);need(!rt.targets.contains(0x8c5ffa1c),"Context getter already bound");movie_context_restore_register(rt);need(!rt.targets.contains(0x8c647e7c),"Next initializer inventory differs");for(unsigned target:{0x8ceb0174u,0x8cedc348u,0x8ced811au,0x8cedc394u,0x8ced7eccu})need(!rt.targets.contains(target),"Unexpected existing setup binding");rt.main_ram=ram;rt.system_flash=flash;rt.aica_ram=sound;load_timers(rt,argv[3]);restore_sq(rt,sq,device);for(auto a:addresses)put(rt.main_ram,a,word(movie,a-0x8ceb0000));need(std::equal(movie.begin(),movie.end(),rt.main_ram.begin()+0xeb0000),"Original image not restored");return owner;};
 auto activate=[&](DCRuntime&rt){dc_activate_code_module(rt,0x8ceb0000,movie.data(),movie.size(),entries.data(),entries.size());for(unsigned i=0;i<addresses.size();++i)put(rt.main_ram,addresses[i],values[i]);need(rt.main_ram==ram,"Captured RAM not restored");};
 auto owner=make();auto&rt=*owner;dc_begin_code_module(rt,0x8ceb0000,movie.size());activate(rt);auto c=initial;need(rt.targets.contains(0x8c608df0)&&rt.targets.contains(0x8c5ffa24)&&rt.targets.contains(0x8c5ffa1c)&&!rt.targets.contains(0x8cecf9f2),"Frame/movie binding inventory differs");need(c.fpscr==0x40001&&!rt.device_clock_enabled,"Discovery execution mode differs");rt.fpu_aot_mode=0;need(std::fesetround(FE_TONEAREST)==0,"Host mode setup failed");
 rt.trace_history_enabled=true;rt.pvr_ta_provenance_enabled=true;
 save_graphics(rt,out+"/initial");
 std::string boundary;try{call_recompiled(c,rt,0x8ceb0254);}catch(const std::runtime_error&e){boundary=e.what();}
 same(state(argv[11]),c);need(rt.main_ram==read(argv[10]),"Integrated callback RAM differs");
 save_graphics(rt,out+"/observed");
 save_state(c,out+"/observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/observed-timers.txt");write_observed_capture(out+"/observed-ram.bin",argv[1],rt.main_ram.data(),rt.main_ram.size());write_observed_capture(out+"/observed-aica.bin",argv[7],rt.aica_ram.data(),rt.aica_ram.size());write_observed_capture(out+"/observed-flash.bin",argv[6],rt.system_flash.data(),rt.system_flash.size());save_ram(rt.vram,out+"/observed-vram.bin");save_sq(rt.store_queues,sqfields(rt),out+"/observed");
 std::ofstream(out+"/boundary.txt")<<boundary<<'\n';std::ofstream devices(out+"/observed-devices.txt");devices<<"device_clock_enabled="<<rt.device_clock_enabled<<"\ndevice_cycles="<<rt.device_clock_sh4_cycles<<"\npvr_frames="<<rt.pvr_frames<<"\npvr_logical_frames="<<rt.pvr_logical_frames<<"\nta_packets="<<rt.pvr_ta_stream.size()<<"\narm_instructions="<<rt.aica_arm.instructions<<"\ng2_dma_runs="<<rt.g2_dma_runs<<"\nholly_irq_taken="<<rt.holly_irq_taken<<"\nasync_redirect="<<rt.sh4_async_redirect<<"\nrelocated_code_matches="<<rt.relocated_code_matches<<"\n";
 write_observed_capture(out+"/observed-ta-packets.bin",{},rt.pvr_ta_stream.data(),rt.pvr_ta_stream.size()*sizeof(rt.pvr_ta_stream[0]));
 write_observed_capture(out+"/observed-ta-sources.bin",{},rt.pvr_ta_stream_source.data(),rt.pvr_ta_stream_source.size());
 write_observed_capture(out+"/observed-ta-pcs.bin",{},rt.pvr_ta_stream_pc.data(),rt.pvr_ta_stream_pc.size()*sizeof(std::uint32_t));
 save_ram(rt.ta_partial,out+"/observed-ta-partial.bin");std::ofstream(out+"/observed-ta-partial-size.txt")<<rt.ta_partial_size<<'\n';
 std::ofstream trace(out+"/observed-calls.txt");trace<<"total="<<rt.trace_call_count<<" retained="<<rt.trace_history_size<<'\n';
 for(std::size_t i=0;i<rt.trace_history_size;++i){auto&e=rt.trace_history[(rt.trace_history_next+rt.trace_history.size()-rt.trace_history_size+i)%rt.trace_history.size()];trace<<std::hex<<e.caller<<' '<<e.target<<' '<<e.r8<<' '<<e.sp<<'\n';}
 need(rt.pvr_ta_stream.size()==rt.pvr_ta_stream_pc.size()&&rt.pvr_ta_stream.size()==rt.pvr_ta_stream_source.size(),"TA provenance incomplete");
 need(rt.relocated_code_matches==0,"Unexpected signature fallback");need(boundary.empty()&&c.pc==0x8cecf9f2,"Context transfer did not reach worker");
 std::ofstream(out+"/result.json")<<R"({"context_transfer_completed":true,"independent_state_verification_pending":false,"native_invocations":1,"new_instructions":2,"integrated_cycles":1214,"restore_cycles":89,"candidate_only":true,"G1_resolved":false})"<<'\n';std::cout<<"DISCOVERY original movie loop boundary: "<<boundary<<" pc="<<std::hex<<c.pc<<" pr="<<c.pr<<'\n';
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
