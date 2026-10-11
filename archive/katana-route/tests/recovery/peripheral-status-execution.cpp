// Original per-record connection test, aggregate eight-bit mask and caller prefix.
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
static auto read(const char* path){std::ifstream f(path,std::ios::binary);need(bool(f),"Missing peripheral fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned address){unsigned v;std::memcpy(&v,m.data()+(address&0xffffff),4);return v;}
static void put(auto& m,unsigned address,unsigned value){std::memcpy(m.data()+(address&0xffffff),&value,4);}
static void half(auto& m,unsigned address,unsigned value){std::uint16_t v=value;std::memcpy(m.data()+(address&0xffffff),&v,2);}
static void fill(auto& m,unsigned address,unsigned size,unsigned value){std::fill_n(m.begin()+(address&0xffffff),size,value);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t){
 SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}
 c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;
 c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.fpscr=0x40001;c.fpul=0xcafebabe;return c;
}
static void abi(const SH4Context& before,const SH4Context& after,unsigned result,unsigned macl,bool has_result=true){
 need(after.pc==before.pr && after.pr==before.pr,"Peripheral return PC/PR differs");
 if(has_result)need(after.r[0]==result,"Peripheral result differs");
 for(unsigned i=8;i<16;++i)need(before.r[i]==after.r[i],"Peripheral saved register differs");
 need((before.sr&~1u)==(after.sr&~1u),"Peripheral interrupt mask differs");
 need(before.mach==after.mach && after.macl==macl,"Peripheral multiply state differs");
 need(before.gbr==after.gbr && before.vbr==after.vbr && before.fpscr==after.fpscr && before.fpul==after.fpul && before.fr_bits==after.fr_bits && before.xf_bits==after.xf_bits,"Peripheral control/FPU state differs");
}
static SH4Context load_state(const char* path){
 std::ifstream f(path);need(bool(f),"Missing worker state");SH4Context c{};std::string l;
 while(std::getline(f,l)){
  auto eq=l.find('=');if(eq==std::string::npos)continue;auto key=l.substr(0,eq);unsigned v=0;
  if(key=="pc" || key=="pr" || key=="sr" || key=="fpscr" || key=="vbr" || key=="spc" || key=="gbr" || key=="mach" || key=="macl" || key=="fpul" || key[0]=='r' || key.starts_with("fr") || key.starts_with("xf"))v=std::stoul(l.substr(eq+1),nullptr,16);else continue;
  if(key=="pc")c.pc=v;else if(key=="pr")c.pr=v;else if(key=="sr")c.sr=v;else if(key=="fpscr")c.fpscr=v;else if(key=="vbr")c.vbr=v;else if(key=="spc")c.spc=v;else if(key=="gbr")c.gbr=v;else if(key=="mach")c.mach=v;else if(key=="macl")c.macl=v;else if(key=="fpul")c.fpul=v;
  else if(key.starts_with("fr"))c.fr_bits[std::stoul(key.substr(2))]=v;else if(key.starts_with("xf"))c.xf_bits[std::stoul(key.substr(2))]=v;else if(key[0]=='r')c.r[std::stoul(key.substr(1))]=v;
 }
 return c;
}
static void state_dump(const std::string& path,const SH4Context& c){
 std::ofstream f(path);f<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';
 for(unsigned i=0;i<16;++i)f<<"r"<<std::dec<<i<<'='<<std::hex<<c.r[i]<<'\n';for(unsigned i=0;i<16;++i)f<<"fr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<'\n';for(unsigned i=0;i<16;++i)f<<"xf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';need(bool(f),"Prediction state write failed");
}
int main(int argc,char**argv){try{
 need(argc==5,"usage: status-oracle RAM STATE FLASH PREDICTION_PREFIX");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024 && flash.size()==131072,"Wrong status fixtures");auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);SH4Context boot{};dc_setup_commercial_boot(boot,rt);dc_set_system_flash_image(rt,flash);const auto sound=rt.aica_ram;const unsigned manager=word(main,0x8c8ac634);unsigned direct=0,aggregate=0;
 auto exact=[&](auto expected,unsigned stack,unsigned scratch){if(scratch)std::copy_n(rt.main_ram.begin()+((stack-scratch)&0xffffff),scratch,expected.begin()+((stack-scratch)&0xffffff));need(rt.main_ram==expected && rt.system_flash==flash && rt.aica_ram==sound,"Status memory/flash/sound guards differ");};
 for(unsigned slot=0;slot<8;++slot)for(unsigned active:{0u,1u,0x7fu,0x80u,0xffu})for(unsigned flags:{0u,1u,2u,3u,0x7fffffffu,0x80000000u,0x80000002u,0xffffffffu})for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  rt.main_ram=main;const unsigned id=word(main,0x8c676234+4*slot);rt.main_ram[(0x8c897e50+24*id)&0xffffff]=active;put(rt.main_ram,manager+0x44c+120*id,flags);auto expected=rt.main_ram;auto c=context(0x8c664aec,mask,t);c.fpscr|=bank<<21;c.r[4]=slot;auto before=c;call_recompiled(c,rt,c.pc);const unsigned result=active && (flags&2)?1:0;abi(before,c,result,120*id);need(c.sr==((before.sr&~1u)|!result),"Per-record T differs");need(c.r[4]==manager+0x44c+120*id,"Per-record address differs");exact(expected,before.r[15],16);++direct;
 }
 for(unsigned reverse:{0u,1u})for(unsigned bits=0;bits<256;++bits)for(unsigned form:{0u,1u})for(unsigned mask:{0u,6u,15u})for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  rt.main_ram=main;unsigned last=0;
  for(unsigned slot=0;slot<8;++slot){const unsigned id=word(main,0x8c676234+4*(reverse?7-slot:slot));put(rt.main_ram,0x8c676234+4*slot,id);rt.main_ram[(0x8c897e50+24*id)&0xffffff]=form?0x80:((bits>>slot)&1);put(rt.main_ram,manager+0x44c+120*id,form?(((bits>>slot)&1)?0xfffffffe:0xfffffffc):0x80000002);last=id;}
  auto expected=rt.main_ram;auto c=context(0x8c64bf74,mask,t);c.fpscr|=bank<<21;auto before=c;call_recompiled(c,rt,c.pc);abi(before,c,bits,120*last);need(c.sr==((before.sr&~1u)|1u),"Mask final T differs");exact(expected,before.r[15],40);++aggregate;
 }
 // Actual caller prefix: compute connected bit0, then reject before its installed callback.
 rt.main_ram=main;auto c=load_state(argv[2]);need(c.pc==0x8c606a28 && word(main,0x8c8937e0)==0 && word(main,0x8c8937ec)!=0,"Frozen status prefix differs");auto before=c;auto expected=main;const unsigned stack=c.r[15];
 for(unsigned i=0;i<5;++i)put(expected,stack-4*(i+1),before.r[14-i]);put(expected,stack-24,0x8c606a34);put(expected,stack-28,7);put(expected,stack-32,0x8c64bf90);put(expected,stack-36,0x8c897e50+24*20);put(expected,stack-40,0x8c664b08);put(expected,stack-4,0x8c606a42);put(expected,stack,1);put(expected,0x8c8937e0,1);
 auto predicted=before;predicted.pc=0x8c606a76;predicted.pr=0x8c606a7a;predicted.sr&=~1u;predicted.macl=20*120;predicted.r[0]=1;predicted.r[1]=1;predicted.r[2]=1;predicted.r[3]=1;predicted.r[4]=0;predicted.r[8]=0x8c8937e0;predicted.r[9]=0x8c606b38;predicted.r[10]=255;predicted.r[11]=0;predicted.r[12]=8;predicted.r[13]=1;predicted.r[14]=0x8c8937e4;
 bool rejected=false;try{call_recompiled(c,rt,c.pc);}catch(const std::runtime_error&e){std::cout<<e.what()<<'\n';rejected=std::string(e.what()).find("0x8C606B38")!=std::string::npos;}need(rejected,"Frozen installed callback did not remain an explicit missing dependency");
 for(unsigned i=0;i<16;++i)if(c.r[i]!=predicted.r[i])std::cerr<<"R"<<i<<" actual="<<std::hex<<c.r[i]<<" expected="<<predicted.r[i]<<std::dec<<'\n';
 need(c.pc==predicted.pc && c.pr==predicted.pr && c.sr==predicted.sr && c.r==predicted.r && c.mach==predicted.mach && c.macl==predicted.macl && c.gbr==predicted.gbr && c.vbr==predicted.vbr && c.spc==predicted.spc && c.fpscr==predicted.fpscr && c.fpul==predicted.fpul && c.fr_bits==predicted.fr_bits && c.xf_bits==predicted.xf_bits,"Frozen callback-prefix state differs");exact(expected,stack,0);
 const std::string out=argv[4];std::ofstream ram(out+"-ram.bin",std::ios::binary);ram.write(reinterpret_cast<const char*>(expected.data()),expected.size());need(bool(ram),"Prediction RAM write failed");state_dump(out+"-state.txt",predicted);
 std::cout<<"PASS original peripheral status: "<<direct<<" per-record and "<<aggregate<<" aggregate cases; all256 masks, both identifier orders/FPU banks, byte-sign/flag branches, exact memory guards and source-defined MACL; frozen caller sets bit0 and rejects before606B38 callback with exact RAM/state prediction\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
