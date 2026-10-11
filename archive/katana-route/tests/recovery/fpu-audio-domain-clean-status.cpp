// Synthetic audio inputs through public runtime APIs; no game helper substituted.
#include "dc_runtime.hpp"
#include <array>
#include <bit>
#include <iostream>
#include <memory>
#include <stdexcept>
#pragma STDC FENV_ACCESS ON
using namespace dcrecomp_generated;
static void require(bool v,const char* s){if(!v)throw std::runtime_error(s);}
struct Environment {std::uint64_t control,status;bool operator==(const Environment&)const=default;};
static Environment env(){Environment e;asm volatile("mrs %0, fpcr":"=r"(e.control));asm volatile("mrs %0, fpsr":"=r"(e.status));return e;}
static void install(Environment e){asm volatile("msr fpcr, %0\n\tmsr fpsr, %1\n\tisb"::"r"(e.control),"r"(e.status):"memory");}
static std::array<std::uint64_t,16> gains;
static unsigned register_leaks=0,mixer_leaks=0;
static void audio(SH4Context& c,DCRuntime& rt){
 for(unsigned m=0;m<16;++m){auto before=env();dc_write32(rt,0x00702800,m);register_leaks+=!(env()==before);gains[m]=std::bit_cast<std::uint64_t>(rt.aica_master_gain);}
 auto before=env();before.status=0;install(before);
 try{dc_aica_run_arm7(rt,2048);}catch(...){mixer_leaks+=!(env()==before);throw;}
 mixer_leaks+=!(env()==before);
 dc_require_fpu_round_nearest(c,0x8c010000);
 volatile float a=std::bit_cast<float>(0x40c90fdau),b=500.0f;c.fpul=std::bit_cast<std::uint32_t>(float(a*b));c.pc=c.pr;
}
int main(int argc,char**argv){try{
 require(argc==2,"usage: EXPECT_FIXED");bool fixed=std::string(argv[1])=="1";auto original=env();struct Restore{Environment e;~Restore(){install(e);}}restore{original};
 unsigned controls=0,gain_differences=0,mix_differences=0,return_leaks=0;
 std::array<std::uint64_t,16> reference_gains{};std::vector<std::uint8_t> reference_pcm;std::uint64_t reference_left=0,reference_right=0;
 for(unsigned hr=0;hr<4;++hr)for(unsigned hd=0;hd<2;++hd)for(unsigned gr=0;gr<2;++gr)for(unsigned gd=0;gd<2;++gd)for(unsigned unwind=0;unwind<2;++unwind){
  install(original);auto rt=std::make_unique<DCRuntime>();dc_aica_enable_arm7(*rt,true);rt->aica_arm7_in_reset=false;rt->aica_arm7_idle_fast_forward=false;
  rt->aica_ram.resize(2*1024*1024);for(unsigned i=0;i<8192;i+=4){rt->aica_ram[i]=0;rt->aica_ram[i+1]=0;rt->aica_ram[i+2]=0xa0;rt->aica_ram[i+3]=0xe1;}
  for(unsigned i=0;i<32;++i){std::uint16_t s=1000+137*i;rt->aica_ram[0x10000+2*i]=s;rt->aica_ram[0x10001+2*i]=s>>8;}
  auto& slot=rt->aica_native_slots[0];slot.playing=true;slot.sample_address=0x10000;slot.loop_end=32;slot.pitch_step=1.1234567;slot.position=0.125;slot.total_level=7;slot.direct_send_level=15;slot.pan=0x10;
  rt->aica_native_active_mask=1;rt->aica_wav_path="synthetic-memory-only";rt->aica_native_capture_limit_frames=unwind?4:0;rt->targets[0x8c010000]=audio;
  SH4Context c{};c.pc=0x8c010000;c.pr=0x8c010100;c.fpscr=gr|(gd<<18);
  Environment caller{(original.control&~((3ull<<22)|(1ull<<24)))|(std::uint64_t(hr)<<22)|(std::uint64_t(hd)<<24),2};install(caller);
  bool threw=false;try{call_recompiled(c,*rt,0x8c010000);}catch(const std::runtime_error&e){require(std::string(e.what())=="AICA native capture complete","Unexpected exception");threw=true;}
  return_leaks+=!(env()==caller);require(threw==bool(unwind),"Wrong unwind path");require(rt->aica_native_frames==4&&rt->aica_native_nonzero_frames==4&&rt->aica_arm7_faults==0,"Mixer fixture failed");
  if(!unwind)require(c.fpul==(gr?0x4544597au:0x4544597bu),"Guest arithmetic not restored");
  auto l=std::bit_cast<std::uint64_t>(slot.cached_left_gain),rr=std::bit_cast<std::uint64_t>(slot.cached_right_gain);
  if(!controls){reference_gains=gains;reference_pcm=rt->aica_native_pcm;reference_left=l;reference_right=rr;require(gains[15]==0x3ff0000000000000ull,"Nearest full volume not unity");}
  gain_differences+=gains!=reference_gains;mix_differences+=rt->aica_native_pcm!=reference_pcm||l!=reference_left||rr!=reference_right;++controls;
 }
 install(original);std::cout<<"{\"controls\":"<<controls<<",\"volume_values_per_control\":16,\"mixed_frames_per_control\":4,\"gain_differences\":"<<gain_differences<<",\"mix_differences\":"<<mix_differences<<",\"register_leaks\":"<<register_leaks<<",\"mixer_leaks\":"<<mixer_leaks<<",\"return_leaks\":"<<return_leaks<<"}\n";
 require(!return_leaks,"Dispatch failed caller restoration");if(fixed)require(!gain_differences&&!mix_differences&&!register_leaks&&!mixer_leaks,"Audio domain isolation failed");else require(gain_differences&&register_leaks,"Audio domain defect not reproduced");
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
