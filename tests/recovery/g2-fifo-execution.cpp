// Original seven-instruction G2 reader and public FIFO register contract.
// Controlled busy values apply only to the preserved generic-MMIO reduction.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool v,const char* why){if(!v)throw std::runtime_error(why);}
int main(int argc,char**argv){try{
 const bool control=argc==2 && std::string(argv[1])=="--generic-mmio-control";
 need(argc==1 || control,"Invalid reduction argument");
 auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
 load_embedded_elf_image(rt);register_recompiled_program(rt);
 unsigned cases=0;
 for(unsigned bits=0;bits<64;++bits)for(unsigned alias:{0x0C200000u,0x8C200000u,0xAC200000u,0x8D200000u})for(unsigned t:{0u,1u}){
  // Internal contamination is intentional in the independent register oracle;
  // original startup never receives these test values.
  rt.mmio32[0x005F688C]=bits;
  dc_write32(rt,alias-4,0x12345678);dc_write32(rt,alias,0xDEADBEEF);dc_write32(rt,alias+4,0x87654321);
  const auto ram_before=rt.main_ram;
  SH4Context ctx{};for(unsigned i=0;i<16;++i)ctx.r[i]=0x11110000+i;
  ctx.r[4]=alias;ctx.r[15]=0x8CFFF000;ctx.sr=0x40000000|t;ctx.pc=0x8C1115C8;ctx.pr=0xFFFFFFFF;
  auto expected=ctx;const unsigned ready=control?unsigned(!(bits&1)):1;
  expected.r[0]=0;expected.r[2]=0xA05F688C;expected.r[3]=ready;
  expected.sr=(ctx.sr&~1u)|ready;expected.pc=ctx.pr;
  call_recompiled(ctx,rt,ctx.pc);
  need(std::memcmp(&ctx,&expected,sizeof(ctx))==0,"Original reader register/return ABI differs");
  auto expected_ram=ram_before;std::memcpy(expected_ram.data()+(alias&0xFFFFFF),&ready,4);
  need(rt.main_ram==expected_ram,"Original reader RAM/guard differs");++cases;
 }
 if(!control){
  for(unsigned address:{0x005F688Cu,0x805F688Cu,0xA05F688Cu}){
   dc_write32(rt,address,0xFFFFFFFF);need(dc_read32(rt,address)==0,"FIFO register is not read-only/empty");
   dc_write16(rt,address,0xFFFF);need(dc_read16(rt,address)==0,"FIFO halfword write/read differs");
   dc_write8(rt,address,0xFF);need(dc_read8(rt,address)==0,"FIFO byte write/read differs");
  }
  dc_write32(rt,0xA0804000,0x31415926);
  need(dc_read32(rt,0xA0804000)==0x31415926 && dc_read32(rt,0xA05F688C)==0,"Completed synchronous sound RAM write leaves pending FIFO");
  dc_write32(rt,0xA0702C00,1);need(dc_read32(rt,0xA05F688C)==0,"Completed synchronous AICA register write leaves pending FIFO");
  auto bios_owner=std::make_unique<DCRuntime>();auto&bios=*bios_owner;SH4Context handoff{};
  std::fill(bios.aica_ram.begin(),bios.aica_ram.end(),0xA5);
  auto expected_sound=bios.aica_ram;const unsigned idle=0xEAFFFFFE;
  std::memcpy(expected_sound.data(),&idle,4);
  dc_setup_commercial_boot(handoff,bios);
  need(bios.aica_ram==expected_sound,"Post-BIOS idle word changes unrelated sound RAM");
  dc_aica_enable_arm7(bios,true,128);bios.aica_arm7_idle_fast_forward=false;
  dc_write32(bios,0xA0702C00,0);
  need(bios.aica_arm.instructions==32768 && bios.aica_arm.r[15]==0 && bios.aica_arm7_faults==0 && !bios.aica_arm.halted,"Post-BIOS idle loop release differs");
  need(dc_aica_run_arm7(bios,20000)==20000 && bios.aica_arm.instructions==52768 && bios.aica_arm.r[15]==0 && bios.aica_arm7_faults==0,"Idle firmware does not remain in its original loop");
  need(bios.aica_ram==expected_sound,"Idle firmware changes sound RAM");
  dc_write32(bios,0xA0702C00,1);
  need(bios.aica_arm7_in_reset && dc_aica_run_arm7(bios,128)==0,"Original reset assertion does not hold sound CPU");
  std::cout<<"PASS post-BIOS AICA idle: exact initial RAM;52768 executed ARM instructions atPC0; reset stops execution; no fault\n";
 }
 std::cout<<"PASS original G2 reader: "<<cases<<" exact RAM/context cases; "<<(control?"controlled busy/empty generic-MMIO branch oracle":"explicit read-only empty FIFO; synchronous sound writes")<<"\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
