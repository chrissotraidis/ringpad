// Synthetic domain-boundary controls. No original game helper is substituted.
#include "dc_runtime.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#pragma STDC FENV_ACCESS ON
using namespace dcrecomp_generated;
static void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
struct Environment {std::uint64_t control,status;bool operator==(const Environment&)const=default;};
static Environment environment(){Environment e;asm volatile("mrs %0, fpcr":"=r"(e.control));asm volatile("mrs %0, fpsr":"=r"(e.status));return e;}
static void install(Environment e){asm volatile("msr fpcr, %0\n\tmsr fpsr, %1\n\tisb"::"r"(e.control),"r"(e.status):"memory");}
static void arithmetic(SH4Context&c,DCRuntime&){
 dc_require_fpu_round_nearest(c,0x8c010000);
 volatile float a=std::bit_cast<float>(0x40c90fdau),b=500.0f;
 c.fpul=std::bit_cast<std::uint32_t>(float(a*b));
 if(c.r[0])throw std::runtime_error("controlled unwind");
 c.pc=c.pr;
}
struct Pixels {std::vector<std::uint32_t> color;std::vector<float> depth;};
int main(int argc,char**argv){try{
 require(argc==2,"usage: EXPECT_FIXED (0 or 1)");const bool fixed=std::string(argv[1])=="1";
 const auto original=environment();struct Restore {Environment e;~Restore(){install(e);}}restore{original};
 unsigned controls=0,writer_leaks=0,return_leaks=0,render_leaks=0,render_differences=0;
 for(unsigned host_round=0;host_round<4;++host_round)for(unsigned host_dn=0;host_dn<2;++host_dn)
 for(unsigned guest_round=0;guest_round<2;++guest_round)for(unsigned guest_dn=0;guest_dn<2;++guest_dn)
 for(unsigned unwind=0;unwind<2;++unwind){
  auto rt=std::make_unique<DCRuntime>();rt->targets[0x8c010000]=arithmetic;
  SH4Context c{};c.pc=0x8c010000;c.pr=0x8c010100;c.r[0]=unwind;
  const Environment caller{(original.control&~((3ull<<22)|(1ull<<24)))|(std::uint64_t(host_round)<<22)|(std::uint64_t(host_dn)<<24),2};
  install(caller);dc_write_fpscr(c,guest_round|(guest_dn<<18));writer_leaks+=!(environment()==caller);
  install(caller);bool threw=false;
  try{call_recompiled(c,*rt,0x8c010000);}catch(const std::runtime_error&e){require(std::string(e.what())=="controlled unwind","Unexpected dispatch error");threw=true;}
  require(threw==bool(unwind),"Wrong return path");
  require(c.fpul==(guest_round?0x4544597au:0x4544597bu),"Guest arithmetic mode differs");
  return_leaks+=!(environment()==caller);++controls;
 }
 for(unsigned immediate=0;immediate<2;++immediate){
  Pixels reference;
  for(unsigned host_round=0;host_round<4;++host_round){
   auto rt=std::make_unique<DCRuntime>();rt->pvr_gpu_enabled=false;rt->pvr_mt_enabled=false;
   rt->pvr_packet_limit=immediate?1024:0;
   const Environment caller{(original.control&~((3ull<<22)|(1ull<<24)))|(std::uint64_t(host_round)<<22),2};install(caller);
   auto packet=[&](std::array<std::uint32_t,8>words){for(unsigned i=0;i<8;++i)dc_write32(*rt,0x10000000+4*i,words[i]);};
   packet({(4u<<29)|2u,7u<<29,0,0,0,0,0,0});
   constexpr std::array<std::array<float,3>,3> points={{{10.1f,10.2f,1.3f},{90.4f,10.5f,1.6f},{10.7f,90.8f,1.9f}}};
   constexpr std::array<std::uint32_t,3> colors={0xff5279b3,0xffd32857,0xff179df2};
   for(unsigned i=0;i<3;++i)packet({(7u<<29)|(i==2?0x10000000u:0),std::bit_cast<std::uint32_t>(points[i][0]),std::bit_cast<std::uint32_t>(points[i][1]),std::bit_cast<std::uint32_t>(points[i][2]),colors[i],0,colors[i],0});
   packet({});dc_write32(*rt,0x005f8014,1);
   render_leaks+=!(environment()==caller);
   require(rt->pvr_vertices==3&&rt->pvr_triangles>0,"Renderer fixture did not draw a triangle");
   Pixels pixels{rt->pvr_present_buffer,rt->pvr_depth};
   require(std::any_of(pixels.color.begin(),pixels.color.end(),[](auto p){return p!=0xff101018u;}),"Renderer fixture is blank");
   if(host_round==0)reference=pixels;
   else render_differences+=pixels.color!=reference.color||pixels.depth.size()!=reference.depth.size()||std::memcmp(pixels.depth.data(),reference.depth.data(),pixels.depth.size()*sizeof(float));
  }
 }
 std::cout<<"{\"controls\":"<<controls<<",\"writer_leaks\":"<<writer_leaks<<",\"return_leaks\":"<<return_leaks<<",\"render_leaks\":"<<render_leaks<<",\"render_differences\":"<<render_differences<<"}\n";
 if(fixed)require(!writer_leaks&&!return_leaks&&!render_leaks&&!render_differences,"FPU domain isolation failed");
 else require(writer_leaks&&return_leaks,"Unfixed domain leakage was not reproduced");
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
