// Original09D14C quad construction; the original drawing callee is delegated.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void frame_quad_register(DCRuntime&);}
struct FrameQuad : RequestModel {
 bool prefix(){auto&r=c.r;auto&f=(c.fpscr&(1u<<21))?c.xf_bits:c.fr_bits;need(!(c.fpscr&(1u<<20)),"Model requires single-width FMOV");
  push(c.pr);r[15]-=72;r[2]=w(0x8c09d280);r[5]=255;r[3]=w(r[2]);r[4]=r[5]-r[3];t(int(r[4])>0);cycles+=10;
  if(!(c.sr&1)){r[15]+=72;c.pr=pop();c.pc=c.pr;cycles+=5;return false;}
  t(int(r[4])>=int(r[5]));cycles+=2;if(c.sr&1){r[4]=r[5];++cycles;}
  r[5]=r[15]+24;r[6]=r[5];r[0]=0x8c09d284;r[3]=r[6]+24;f[4]=0;r[2]=r[6]+16;r[7]=r[5]+8;r[4]<<=16;
  put(m,r[7],f[4]);put(m,r[6],f[4]);r[4]<<=8;put(m,r[15]+4,r[2]);put(m,r[15],r[3]);f[3]=w(r[0]);r[0]=4;put(m,r[3],f[3]);put(m,r[2],f[3]);r[3]=w(r[15]);put(m,r[0]+r[3],f[4]);put(m,r[0]+r[6],f[4]);
  r[0]=0x8c09d288;f[3]=w(r[0]);r[6]=r[15];r[3]=w(r[15]+4);r[0]=4;r[6]+=56;put(m,r[0]+r[3],f[3]);put(m,r[0]+r[7],f[3]);r[3]=0;
  put(m,r[6]+12,r[4]);r[0]=0x8c09d28c;put(m,r[6]+8,r[4]);r[2]=4;put(m,r[6]+4,r[4]);put(m,r[6],r[4]);r[4]=r[15];put(m,r[15]+8,r[5]);r[5]=r[2];put(m,r[15]+12,r[6]);r[6]=96;put(m,r[15]+16,r[3]);r[3]=w(0x8c09d290);put(m,r[15]+20,r[2]);f[4]=w(r[0]);c.pr=0x8c09d1ca;r[4]+=8;c.pc=r[3];need(c.pc==0x8c642908,"Unreviewed draw target");cycles+=51;return true;
 }
};
static RecompiledFunction real_draw;
static std::function<void(SH4Context&,DCRuntime&)> observe_draw;
static void watched_draw(SH4Context&c,DCRuntime&rt){observe_draw(c,rt);real_draw(c,rt);}
int main(int argc,char**argv){try{
 need(argc==5,"usage: frame-quad RAM STATE FLASH OUTPUT");const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");const std::string out=argv[4];unsigned controls=0,skips=0,draws=0;
 auto check=[&](unsigned value,unsigned bank,unsigned alias,bool capture){
  auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);frame_quad_register(rt);rt.main_ram=ram;rt.system_flash=flash;load_timers(rt,argv[2]);auto c=initial;
  if(!capture){put(rt.main_ram,0x8c7a4d00,value);c.fpscr=(c.fpscr&~(1u<<21))|(bank<<21);c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.r[15]=alias?0xacefe000:0x8cefe000;c.pr=alias?0xac112234:0x8c112234;}
  auto expected=rt.main_ram;FrameQuad model{{c,expected}};const bool drawing=model.prefix();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  auto exact=[&](const SH4Context&actual){same(model.c,actual);need(expected==rt.main_ram,"Quad prefix whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&!rt.sh4_tick_pending_cycles&&rt.sh4_tick_batch_cycles==1,"Quad timer controls differ");};
  if(capture){need(drawing&&model.cycles==63,"Captured quad prefix timing differs");save_ram(expected,out+"/phase0-expected-ram.bin");save_state(model.c,out+"/phase0-expected-state.txt");save_timers(timers,start,tocr,out+"/phase0-expected-timers.txt");}
  unsigned seen=0;bool verified=false;real_draw=rt.targets.at(0x8c642908);observe_draw=[&](SH4Context&actual,DCRuntime&){need(drawing&&seen++==0,"Unexpected draw entry");exact(actual);const unsigned packet=actual.r[4],xy=word(rt.main_ram,packet),colors=word(rt.main_ram,packet+4);need(actual.r[5]==4&&actual.r[6]==96&&word(rt.main_ram,packet+8)==0&&word(rt.main_ram,packet+12)==4,"Quad descriptor differs");const unsigned width=word(ram,0x8c09d284),height=word(ram,0x8c09d288);const unsigned coords[]={0,0,0,height,width,height,width,0};for(unsigned i=0;i<8;++i)need(word(rt.main_ram,xy+4*i)==coords[i],"Quad vertex differs");const auto raw=255u-value;const unsigned color=(int(raw)>=255?255u:raw)<<24;for(unsigned i=0;i<4;++i)need(word(rt.main_ram,colors+4*i)==color,"Quad color differs");verified=true;};rt.register_target(0x8c642908,watched_draw);
  std::string terminal;try{call_recompiled(c,rt,0x8c09d14c);terminal="returned";}catch(const std::runtime_error&e){terminal=e.what();}observe_draw={};rt.register_target(0x8c642908,real_draw);
  if(drawing){need(seen==1&&verified,"Quad entry comparison incomplete");need(terminal=="returned"||terminal.find("No recompiled/native target registered for Dreamcast address")!=std::string::npos,"Unexpected original draw outcome");++draws;}else{need(!seen&&terminal=="returned","Skipped quad did not return");exact(c);++skips;}
  if(capture){save_ram(rt.main_ram,out+"/quad-observed-ram.bin");save_state(c,out+"/quad-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/quad-observed-timers.txt");std::ofstream(out+"/terminal.txt")<<terminal<<'\n';std::cout<<"Actual delegated draw outcome: "<<terminal<<'\n';}else ++controls;
 };
 for(unsigned value:{0u,8u,254u,255u,256u,0xffffffffu,0x7fffffffu,0x80000000u,0x80000100u})for(unsigned bank=0;bank<2;++bank)for(unsigned alias=0;alias<2;++alias)check(value,bank,alias,false);
 check(word(ram,0x8c7a4d00),0,0,true);
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"draw_entries_including_capture\":"<<draws<<",\"skip_returns\":"<<skips<<",\"captured_prefix_cycles\":63,\"whole_RAM_CPU_TMU_verified\":true,\"quad_vertices_and_colors_verified\":true,\"draw_complete_return_verified\":false,\"quad_complete_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original frame quad: "<<controls<<" alpha/bank/alias controls;63cycle captured whole RAM CPU TMU and640x480 vertices/f7000000 colors;real draw delegated, later state observed only\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
