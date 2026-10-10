// Original SDK setup and command10 payload. No lock, issue or response stubs.
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
static void need(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static auto read(const char* p){std::ifstream f(p,std::ios::binary);need(bool(f),"Missing transfer fixture");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};}
static unsigned word(const auto& m,unsigned a){unsigned v;std::memcpy(&v,m.data()+(a&0xffffff),4);return v;}
static void put(auto& m,unsigned a,unsigned v){std::memcpy(m.data()+(a&0xffffff),&v,4);}
static SH4Context context(unsigned entry,unsigned mask,unsigned t,unsigned bank){SH4Context c{};for(unsigned i=0;i<16;++i){c.r[i]=0x12340000+i*17;c.fr_bits[i]=0x7fc01000+i;c.xf_bits[i]=0x80000000+i;}c.pc=entry;c.pr=0xffffffff;c.r[15]=0x8ceff000;c.sr=0x40000000|(mask<<4)|t;c.mach=0x12345678;c.macl=0x89abcdef;c.gbr=0x8cd12340;c.vbr=0x8c000000;c.spc=0x8c010024;c.fpscr=0x40001|(bank<<21);c.fpul=0xcafebabe;return c;}
static void control(const SH4Context& b,const SH4Context& c,unsigned macl){need((b.sr&~1u)==(c.sr&~1u)&&b.mach==c.mach&&c.macl==macl&&b.gbr==c.gbr&&b.vbr==c.vbr&&b.spc==c.spc&&b.fpscr==c.fpscr&&b.fpul==c.fpul&&b.fr_bits==c.fr_bits&&b.xf_bits==c.xf_bits,"Transfer control/FPU differs");}
static void returned(const SH4Context& b,const SH4Context& c,unsigned result,unsigned macl){control(b,c,macl);need(c.pc==b.pr&&c.pr==b.pr&&c.r[0]==result,"Transfer return differs");for(unsigned i=8;i<16;++i)need(b.r[i]==c.r[i],"Transfer saved register differs");}
static bool reject(SH4Context& c,DCRuntime& rt,unsigned entry,const char* target){try{call_recompiled(c,rt,entry);}catch(const std::runtime_error& e){if(std::string(e.what()).find(target)!=std::string::npos)return true;throw;}return false;}
static SH4Context state(const char* path){std::ifstream f(path);need(bool(f),"Missing transfer state");SH4Context c{};std::string l;while(std::getline(f,l)){auto eq=l.find('=');if(eq==std::string::npos)continue;auto k=l.substr(0,eq);if(!(k=="pc"||k=="pr"||k=="sr"||k=="fpscr"||k=="vbr"||k=="spc"||k=="gbr"||k=="mach"||k=="macl"||k=="fpul"||k[0]=='r'||k.starts_with("fr")||k.starts_with("xf")))continue;unsigned v=std::stoul(l.substr(eq+1),nullptr,16);if(k=="pc")c.pc=v;else if(k=="pr")c.pr=v;else if(k=="sr")c.sr=v;else if(k=="fpscr")c.fpscr=v;else if(k=="vbr")c.vbr=v;else if(k=="spc")c.spc=v;else if(k=="gbr")c.gbr=v;else if(k=="mach")c.mach=v;else if(k=="macl")c.macl=v;else if(k=="fpul")c.fpul=v;else if(k.starts_with("fr"))c.fr_bits[std::stoul(k.substr(2))]=v;else if(k.starts_with("xf"))c.xf_bits[std::stoul(k.substr(2))]=v;else if(k[0]=='r')c.r[std::stoul(k.substr(1))]=v;}return c;}
static void same(const SH4Context& a,const SH4Context& b){for(unsigned i=0;i<16;++i)if(a.r[i]!=b.r[i])std::cerr<<"Frozen register R"<<std::dec<<i<<" expected="<<std::hex<<a.r[i]<<" actual="<<b.r[i]<<"\n";if(a.pc!=b.pc||a.pr!=b.pr||a.sr!=b.sr||a.macl!=b.macl)std::cerr<<"Frozen PC/PR/SR/MACL expected="<<std::hex<<a.pc<<"/"<<a.pr<<"/"<<a.sr<<"/"<<a.macl<<" actual="<<b.pc<<"/"<<b.pr<<"/"<<b.sr<<"/"<<b.macl<<"\n";need(a.r==b.r&&a.pc==b.pc&&a.pr==b.pr&&a.sr==b.sr&&a.gbr==b.gbr&&a.vbr==b.vbr&&a.spc==b.spc&&a.mach==b.mach&&a.macl==b.macl&&a.fpul==b.fpul&&a.fpscr==b.fpscr&&a.fr_bits==b.fr_bits&&a.xf_bits==b.xf_bits,"Frozen transfer full state differs");}
static void packet_stack(auto& m,const SH4Context& c){unsigned s=c.r[15];put(m,s-4,c.r[14]);put(m,s-8,c.pr);put(m,s-16,c.r[5]);put(m,s-12,c.r[6]<<24);put(m,s-20,0);put(m,s-24,0);}
static SH4Context packet_boundary(SH4Context c){unsigned s=c.r[15];c.pc=0x8c664200;c.pr=0x8c66421e;c.r[3]=0;c.r[5]=10;c.r[6]=s-16;c.r[7]=2;c.r[14]=s-16;c.r[15]=s-24;return c;}
int main(int argc,char**argv){try{
 need(argc==4,"usage: issue-oracle RAM STATE FLASH");const auto main=read(argv[1]),flash=read(argv[3]);need(main.size()==16*1024*1024&&flash.size()==131072,"Wrong issue fixtures");auto owner=std::make_unique<DCRuntime>();auto& rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);const auto sound=rt.aica_ram;auto reset=[&]{rt.main_ram=main;rt.system_flash=flash;};auto exact=[&](const auto& wanted){if(rt.main_ram!=wanted){auto i=std::mismatch(wanted.begin(),wanted.end(),rt.main_ram.begin()).first-wanted.begin();std::cerr<<"Issue RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(wanted[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Issue RAM/stack/guard differs");}need(rt.aica_ram==sound&&rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Issue changes frozen devices");};unsigned setup_cases=0,packet_cases=0,lock_cases=0;const unsigned sdk_root=word(main,0x8c8ac634),lock=word(main,0x8c664534);
 for(unsigned id=0;id<48;++id)for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();unsigned record=sdk_root+40+44*id;rt.main_ram[(record+13)&0xffffff]=0xa5;auto c=context(0x8c66315e,mask,t,bank);c.r[4]=id;auto expected=rt.main_ram;put(expected,c.r[15]-4,c.pr);expected[(record+13)&0xffffff]=1;auto wanted=c;wanted.pc=c.pr;wanted.r[0]=1;wanted.r[2]=44;wanted.r[3]=0x8c8ac634;wanted.r[4]=record;wanted.macl=44*id;call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected);++setup_cases;
 }
 for(unsigned id:{0u,1u,7u,47u})for(unsigned value:{0u,1u,0x80u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c664200,mask,1,bank);c.r[4]=id;c.r[5]=value^0x12345678;c.r[6]=value;auto expected=rt.main_ram;packet_stack(expected,c);auto wanted=packet_boundary(c);need(reject(c,rt,c.pc,"0x8C663550"),"Packet issue boundary did not reject");same(wanted,c);exact(expected);++packet_cases;
 }
 for(unsigned id:{0u,1u,7u,47u})for(unsigned held:{0u,1u,0x7fu,0x80u,0xffu})for(unsigned mask=0;mask<16;++mask)for(unsigned bank:{0u,1u}){
  reset();rt.main_ram[lock&0xffffff]=held;auto c=context(0x8c64a6c2,mask,0,bank);c.r[4]=id;c.r[5]=2;c.r[6]=0;auto expected=rt.main_ram;unsigned s=c.r[15];put(expected,s-4,c.pr);put(expected,s-12,0x8c64a6cc);expected[lock&0xffffff]=held|0x80;auto wanted=c;wanted.r[0]=lock;wanted.r[3]=0x8c664534;wanted.r[15]=s-12;wanted.pr=0x8c6641f2;
  if(!held){wanted.sr|=1;packet_stack(expected,wanted);wanted=packet_boundary(wanted);need(reject(c,rt,c.pc,"0x8C663550"),"Free lock issue boundary did not reject");}
  else{put(expected,s-16,0x8c6761dc);wanted.pc=0x8c6641de;wanted.pr=0x8c6641e6;wanted.r[1]=0x8c6761dc;wanted.r[2]=0x8c663088;wanted.r[15]=s-16;need(reject(c,rt,c.pc,"0x8C663088"),"Held lock logging boundary did not reject");}
  same(wanted,c);exact(expected);++lock_cases;
 }
 // Captured startup -> real setup -> saved caller -> command10 construction.
 reset();auto c=state(argv[2]);need(c.pc==0x8c64a87c&&c.pr==0x8c66462e&&c.r[4]==1&&rt.main_ram[lock&0xffffff]==0,"Frozen setup invocation/lock differs");auto expected=main;unsigned s=c.r[15],record=sdk_root+84;put(expected,s-4,c.pr);expected[(record+13)&0xffffff]=1;auto wanted=c;wanted.pc=c.pr;wanted.r[0]=1;wanted.r[2]=44;wanted.r[3]=0x8c8ac634;wanted.r[4]=record;wanted.macl=44;call_recompiled(c,rt,0x8c66315e);same(wanted,c);exact(expected);
 put(expected,s-4,0x8c66463c);put(expected,s-12,0x8c64a6cc);expected[lock&0xffffff]=0x80;auto packet=c;packet.r[0]=lock;packet.r[2]=0x8c897e50;packet.r[3]=0x8c664534;packet.r[4]=1;packet.r[5]=2;packet.r[6]=0;packet.r[12]=0x8c64a6c2;packet.r[15]=s-12;packet.pr=0x8c6641f2;packet.sr|=1;packet_stack(expected,packet);wanted=packet_boundary(packet);need(reject(c,rt,0x8c66462e,"0x8C663550"),"Frozen command10 boundary did not reject");same(wanted,c);exact(expected);
 std::cout<<"PASS original peripheral issue: "<<setup_cases<<" setup, "<<packet_cases<<" packet and "<<lock_cases<<" wrapper/free-or-held lock cases; all48 SDK records/all16 interrupt masks/both FPU banks, both setup T values; entire RAM/stack and all58 fields exact; actual frozen caller builds command10 words[2,0], sets setup1 and lock80, rejects663550; contention rejects663088; no queued request/responses/completion or hardware/input/gameplay claim\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
