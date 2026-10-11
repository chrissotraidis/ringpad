// Original language query and region fallback on declared synthetic flash.
// These cases never change the live startup or run the sound CPU.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void need(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static std::vector<std::uint8_t> read(const char* path){
    std::ifstream f(path,std::ios::binary);need(bool(f),"Missing settings fixture");
    return {std::istreambuf_iterator<char>(f),{}};
}
static unsigned word(const std::vector<std::uint8_t>& data,unsigned address){
    unsigned value;std::memcpy(&value,data.data()+(address&0xFFFFFF),4);return value;
}
static void put(std::vector<std::uint8_t>& data,unsigned address,unsigned value){
    std::memcpy(data.data()+(address&0xFFFFFF),&value,4);
}
static unsigned crc(const std::vector<std::uint8_t>& data,unsigned offset){
    unsigned value=0xFFFF;
    for(unsigned i=0;i<62;++i)for(int bit=7;bit>=0;--bit){
        const bool different=((value>>15)^((data[offset+i]>>bit)&1))&1;
        value=(value<<1)&0xFFFF;if(different)value^=0x1021;
    }
    return value^0xFFFF;
}
static SH4Context context(unsigned entry,unsigned mask,unsigned t){
    SH4Context c{};for(unsigned i=0;i<16;++i)c.r[i]=0x11110000+i;
    c.r[15]=0x8CEFF000;c.pc=entry;c.pr=0xFFFFFFFF;c.sr=0x40000000|(mask<<4)|t;
    c.gbr=0x8CD12340;c.vbr=0x8C000000;c.fpscr=0x40001;return c;
}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result){
    need(after.pc==before.pr && after.pr==before.pr && after.r[0]==result,"Settings return ABI differs");
    for(unsigned i=8;i<16;++i)need(after.r[i]==before.r[i],"Settings callee-saved register differs");
    need((after.sr&~1u)==(before.sr&~1u),"Settings interrupt mask differs");
    need(after.gbr==before.gbr && after.vbr==before.vbr && after.fpscr==before.fpscr &&
        after.fpul==before.fpul && after.mach==before.mach && after.macl==before.macl,"Settings control register differs");
}
// Test-only alternate raw READ ABI. The real copying/bounds/error service still
// executes; only a successful READ result is represented as Flycast status0.
// This comparator is never installed in live startup or the public core.
static RecompiledFunction original_flash=nullptr;
static unsigned comparator_reads=0;
static void compare_zero_status(SH4Context& c,DCRuntime& r){
 const bool read_command=c.r[7]==1;original_flash(c,r);
 if(read_command && c.r[0]!=0xFFFFFFFF){++comparator_reads;c.r[0]=0;}
}
int main(int argc,char**argv){try{
 need(argc==3,"usage: language-oracle MAIN_RAM STEREO_FLASH");const auto main=read(argv[1]),stereo=read(argv[2]);
 need(main.size()==16*1024*1024 && stereo.size()==131072,"Wrong language fixture size");
 auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);
 SH4Context initial{};dc_setup_commercial_boot(initial,rt);const auto sound=rt.aica_ram;const auto flash_hook=dc_read32(rt,0x8C0000B8);original_flash=rt.targets.at(flash_hook);
 constexpr unsigned output=0x8CD10004,stack=0x8CEFF000,cache=0x8C900000,descriptor=0x8C8937F0;
 need(word(main,0x8C677190)==descriptor && word(main,0x8C67714C)==0,"Captured language prerequisites differ");
 unsigned cases=0,comparisons=0;
 auto initialize=[&](const auto& flash){
  rt.main_ram=main;dc_set_system_flash_image(rt,flash);SH4Context c{};c.r[7]=0;c.pr=0xFFFFFFFF;
  rt.targets.at(dc_read32(rt,0x8C0000B0))(c,rt);need(c.r[0]==0 && rt.system_info_initialized,"Language SYSINFO_INIT failed");
 };
 auto run=[&](unsigned scenario,unsigned language,unsigned region,unsigned overrides,unsigned mode,unsigned mask,unsigned t,bool zero_status=false){
  auto flash=stereo;flash[0x1A003]=region;
  if(overrides){flash[0x1A05E]=overrides==1?0xFF:0x12;flash[0x1A05F]=overrides==1?0x12:0x34;}
  if(scenario<2){std::fill(flash.begin()+0x1C000,flash.end(),0xFF);if(scenario==1){const char magic[]="KATANA_FLASH____";std::copy_n(magic,16,flash.begin()+0x1C000);flash[0x1C010]=2;flash[0x1C011]=0;}}
  else{flash[0x1C047]=language;flash[0x1C048]=mode;const unsigned sum=crc(flash,0x1C040);flash[0x1C07E]=sum&255;flash[0x1C07F]=sum>>8;if(scenario==2)flash[0x1C07E]^=1;}
  const bool record=scenario>=2,valid_crc=scenario!=2;
  initialize(flash);rt.register_target(flash_hook,zero_status?compare_zero_status:original_flash);
  auto setup=context(0x8C60300C,0,0);setup.r[4]=cache;call_recompiled(setup,rt,setup.pc);need(setup.r[0]==0 && word(rt.main_ram,0x8C67714C)==cache,"Original language work-area setup failed");
  dc_write32(rt,output-4,0x12345678);dc_write32(rt,output,0xA5A5A5A5);dc_write32(rt,output+4,0x87654321);
  auto expected=rt.main_ram,expected_flash=flash;
  if(!scenario){const char magic[]="KATANA_FLASH____";std::copy_n(magic,16,expected_flash.begin()+0x1C000);expected_flash[0x1C010]=2;expected_flash[0x1C011]=0;}
  std::vector<std::uint8_t> expected_cache(0x4000,0xFF);std::copy_n(expected_flash.begin()+0x1C000,64,expected_cache.begin());std::copy_n(expected_flash.begin()+0x1FFC0,64,expected_cache.end()-64);
  if(record && valid_crc)std::copy_n(flash.begin()+0x1C040,64,expected_cache.begin()+6*64);
  std::copy(expected_cache.begin(),expected_cache.end(),expected.begin()+(cache&0xFFFFFF));
  put(expected,descriptor,2);put(expected,descriptor+4,0x1C000);put(expected,descriptor+8,0x4000);put(expected,descriptor+12,cache);
  put(expected,descriptor+16,record?1:0);put(expected,descriptor+20,254);put(expected,descriptor+24,1);put(expected,descriptor+28,0);put(expected,descriptor+32,1);put(expected,descriptor+40,5);put(expected,descriptor+44,0);put(expected,descriptor+48,stack-92);
  std::copy_n(expected_flash.begin()+(record?0x1C040:0x1C000),64,expected.begin()+((descriptor+52)&0xFFFFFF));
  const bool fallback=!record || !valid_crc || language==255;
  if(!fallback){std::copy_n(flash.begin()+0x1C042,8,expected.begin()+0x78);put(expected,output,static_cast<unsigned>(static_cast<std::int32_t>(static_cast<std::int8_t>(language))));}
  else{
   // Original fallback replays raw INIT, replacing the complete24-byte SYSINFO record.
   std::fill_n(expected.begin()+0x68,24,0);std::copy_n(flash.begin()+0x1A056,8,expected.begin()+0x68);std::copy_n(flash.begin()+0x1A000,5,expected.begin()+0x70);
   if(zero_status && overrides){expected[0x70]=flash[0x1A05E];expected[0x71]=flash[0x1A05F];}
   const auto signed_region=static_cast<std::int32_t>(static_cast<std::int8_t>(region));put(expected,output,signed_region==-1?1:static_cast<unsigned>(signed_region-48));
  }
  auto c=context(0x8C6030DC,mask,t);c.r[4]=output;const auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,0);
  std::copy_n(rt.main_ram.begin()+((stack-160)&0xFFFFFF),160,expected.begin()+((stack-160)&0xFFFFFF));
  if(rt.main_ram!=expected){for(unsigned i=0;i<expected.size();++i)if(rt.main_ram[i]!=expected[i]){std::cerr<<"zero_status="<<zero_status<<" scenario="<<scenario<<" language="<<language<<" region="<<region<<" overrides="<<overrides<<" RAM offset="<<std::hex<<i<<" got="<<unsigned(rt.main_ram[i])<<" expected="<<unsigned(expected[i])<<std::dec<<'\n';break;}throw std::runtime_error("Language exact RAM differs");}
  need(rt.system_flash==expected_flash && rt.aica_ram==sound,"Language flash/sound guards differ");if(zero_status)++comparisons;else ++cases;
 };
 for(unsigned language=0;language<256;++language)run(3,language,'1',0,0,0,0);
 for(unsigned language:{0u,1u,5u,127u,128u,254u,255u})for(unsigned mode:{0u,2u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(3,language,'1',0,mode,mask,t);
 for(unsigned scenario:{0u,1u,2u,3u})for(unsigned region:{unsigned('0'),unsigned('1'),unsigned('2'),255u,128u})for(unsigned overrides:{0u,1u,2u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(scenario,255,region,overrides,0,mask,t);
 // Compare the conflicting successful READ conventions only in isolated fallback cases.
 for(unsigned scenario:{0u,1u,2u,3u})for(unsigned region:{unsigned('0'),unsigned('1'),unsigned('2'),255u,128u})for(unsigned overrides:{0u,1u,2u})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u})run(scenario,255,region,overrides,0,mask,t,true);
 rt.register_target(flash_hook,original_flash);
 for(bool missing_output:{false,true})for(unsigned mask:{0u,5u,15u})for(unsigned t:{0u,1u}){
  initialize(stereo);dc_write32(rt,output,0xA5A5A5A5);auto expected=rt.main_ram;if(!missing_output)put(expected,output,0);
  auto c=context(0x8C6030DC,mask,t);c.r[4]=missing_output?0:output;const auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,0xFFFFFFFF);
  std::copy_n(rt.main_ram.begin()+((stack-12)&0xFFFFFF),12,expected.begin()+((stack-12)&0xFFFFFF));need(rt.main_ram==expected && rt.system_flash==stereo,"Missing language argument changed state");++cases;
 }
 need(comparator_reads>=comparisons,"Alternate READ contract did not execute");
 std::cout<<"PASS original language settings: "<<cases<<" native cases, "<<comparisons<<" zero-status comparator cases; all language bytes, signed values, region fallback, factory overrides, invalidCRC/mode, missing arguments, exact RAM/flash and preserved ABI\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
