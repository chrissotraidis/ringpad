// Original retained63CC0C, with synthetic identity-plus-translation inputs.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <array>
#include <bit>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
namespace dcrecomp_generated {void recomp_8C63CC0C(SH4Context&,DCRuntime&);}
static void require(bool ok,const char*why){if(!ok)throw std::runtime_error(why);}
static std::uint64_t fpcr(){std::uint64_t v;asm volatile("mrs %0, fpcr":"=r"(v));return v;}
static void set_fpcr(std::uint64_t v){asm volatile("msr fpcr, %0\n\tisb"::"r"(v):"memory");}
static void switch_then_direct(SH4Context&c,DCRuntime&rt){
 dc_require_fpu_round_nearest(c,0x8c010000); // current guest mode is toward zero
 dc_write_fpscr(c,0); // generated LDS changes to nearest before a direct call
 c.pc=0x8c63cc0c;recomp_8C63CC0C(c,rt);
}
int main(int argc,char**argv){try{
 require(argc==2,"usage EXPECT_FIXED");const bool fixed=std::string(argv[1])=="1";
 const auto original=fpcr();struct Restore{std::uint64_t v;~Restore(){set_fpcr(v);}}restore{original};
 unsigned controls=0,failures=0;std::array<unsigned,3> modes{};
 for(unsigned mode=0;mode<3;++mode)for(unsigned incoming=0;incoming<4;++incoming)for(unsigned nested=0;nested<2;++nested){
  auto rt=std::make_unique<DCRuntime>();load_embedded_elf_image(*rt);register_recompiled_program(*rt);
  require(rt->targets.contains(0x8c63cc0c),"Original transform is absent");rt->targets[0x8cf00000]=switch_then_direct;rt->fpu_aot_mode=mode;
  SH4Context c{};c.pc=0x8c63cc0c;c.pr=0x8cf10000;c.r[4]=0;c.r[5]=0x8cf20000;c.r[6]=0x8cf30000;c.r[15]=0x8cff0000;c.fpscr=nested?1:0;
  for(unsigned i=0;i<4;++i)c.xf_bits[i*5]=0x3f800000;
  c.xf_bits[12]=0x33c00000; // 3*2^-25; exact sum1.0000000894069671630859375
  for(unsigned i=0;i<3;++i)dc_write32(*rt,c.r[5]+4*i,std::bit_cast<unsigned>(float(i+1)));
  set_fpcr((original&~((3ull<<22)|(1ull<<24)))|(std::uint64_t(incoming)<<22));
  call_recompiled(c,*rt,nested?0x8cf00000:0x8c63cc0c);
  require(dc_read32(*rt,0x8cf30004)==0x40000000&&dc_read32(*rt,0x8cf30008)==0x40400000,"Unexpected other transform outputs");
  if(dc_read32(*rt,0x8cf30000)!=0x3f800001){++failures;++modes[mode];}
  ++controls;
 }
 std::cout<<"{\"controls\":"<<controls<<",\"nearest_failures\":"<<failures<<",\"failures_by_mode\":["<<modes[0]<<','<<modes[1]<<','<<modes[2]<<"]}\n";
 require(fixed?failures==0:failures>0,"Cached entry expectation differs");
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
