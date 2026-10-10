// Original DMA callback and mailbox byte publication; frozen devices, exact stack.
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
// Independently specified call frames for the selected idle-channel word path.
// These predict every scratch byte; no runtime bytes are copied into expectations.
static void guard_stack(auto& m,unsigned table,unsigned sp,unsigned ret,unsigned r14,unsigned r13,unsigned r12,unsigned r11,bool acquire){
 put(m,sp-4,r14);put(m,sp-8,r13);put(m,sp-12,r12);put(m,sp-16,r11);put(m,sp-20,ret);
 if(acquire){put(m,sp-24,156);put(m,sp-28,0x8c110ad6);put(m,sp-32,0);}
 else{put(m,sp-24,0x8c110a5a);put(m,sp-28,table+160);put(m,sp-32,0);}
 put(m,0x8c7b0b40,acquire?1:0);
}
static void fifo_stack(auto& m,unsigned table,unsigned sp,unsigned ret,unsigned r14,unsigned r13,bool write_query){
 put(m,sp-4,r14);put(m,sp-8,r13);put(m,sp-12,ret);put(m,sp-16,1);
 unsigned counter=table+(write_query?0x110:0x10c);put(m,counter,word(m,counter)+1);
}
static void publish(auto& m,auto& sound,const SH4Context& c,unsigned offset,unsigned value){
 unsigned sp=c.r[15],table=word(m,0x8c5ff84c);
 put(m,sp-4,c.r[14]);put(m,sp-8,c.r[13]);put(m,sp-12,c.pr);unsigned local=sp-24;m[local&0xffffff]=value;
 if(offset>=0x200000)return;
 unsigned aligned=offset&~3u,temp=local+8,lane=offset&3,mask=(c.sr>>4)&15;
 put(m,local+4,lane);
 // Read the aligned sound word through original66CE4C/10F080.
 put(m,local-4,0x8c66ceac);put(m,local-8,aligned);put(m,local-12,0);put(m,local-16,0);
 unsigned readsp=local-16;put(m,readsp-4,aligned);put(m,readsp-8,temp);
 for(unsigned i=0;i<4;++i)put(m,readsp-12-4*i,c.r[12-i]);put(m,readsp-28,0x8c66ce6e);
 unsigned readlocal=readsp-36;put(m,readlocal,4);put(m,readlocal+4,mask);
 guard_stack(m,table,readlocal,0x8c10f0fc,1,0,0,0,true);
 fifo_stack(m,table,readlocal,0x8c10f102,1,0,false);
 put(m,temp,word(sound,aligned));guard_stack(m,table,readlocal,0x8c10f188,1,0,0,0,false);
 m[(temp+lane)&0xffffff]=value;
 // Write the modified word through original66E97C/10F390.
 unsigned modified=word(m,temp);put(m,local-4,0x8c66ceba);put(m,local-8,modified);put(m,local-12,aligned);put(m,local-16,0);put(m,local-20,0);
 unsigned writesp=local-20;put(m,writesp-4,aligned);put(m,writesp-8,temp);
 for(unsigned i=0;i<5;++i)put(m,writesp-12-4*i,c.r[12-i]);put(m,writesp-32,0x8c66e9a4);
 unsigned writelocal=writesp-48;put(m,writelocal,0xa0800000+aligned);put(m,writelocal+4,4);put(m,writelocal+12,mask);
 guard_stack(m,table,writelocal,0x8c10f40c,aligned,1,c.r[12],c.r[11],true);
 fifo_stack(m,table,writelocal,0x8c10f412,aligned,1,false);
 put(m,writelocal,local-8);put(m,writelocal+4,0xa0800000+aligned);put(m,writelocal+8,0);
 fifo_stack(m,table,writelocal,0x8c10f4ce,7,1,false);fifo_stack(m,table,writelocal,0x8c10f4d2,7,1,true);
 put(sound,aligned,modified);guard_stack(m,table,writelocal,0x8c10f4f4,7,1,0x8c110b00,0x8c1107cc,false);
}
static SH4Context published(SH4Context c){unsigned offset=c.r[4];c.pc=c.pr;c.r[0]=0;c.sr|=1;
 if(offset>=0x200000){c.r[3]=0x200000;return c;}
 c.r[1]=0xd4;c.r[2]=(c.sr&0xffffff0f);c.r[3]=0xffffff0f;c.r[4]=0xa05f7860;c.r[5]=c.r[15]-124;c.r[6]=4;c.r[7]=1;return c;
}
static void callback_memory(auto& m,auto& sound,const SH4Context& c){unsigned sp=c.r[15],manager=word(m,0x8c67ff10);put(m,sp-4,c.r[14]);put(m,sp-8,c.pr);auto writer=c;writer.r[14]=0x8c67ff10;writer.pr=0x8c66a822;writer.r[15]-=8;publish(m,sound,writer,word(m,manager+0x1bc),1);
 unsigned count=word(m,manager+0x15c),packets=word(m,manager+0x158);
 if(static_cast<int>(count)>0)for(unsigned i=0;i<count;++i)m[(packets+16*i)&0xffffff]=0;
 put(m,manager+0x15c,0);put(m,manager+0x160,0);
}
static SH4Context callback_state(SH4Context c,const auto& m){unsigned manager=word(m,0x8c67ff10),count=word(m,manager+0x15c);bool valid=word(m,manager+0x1bc)<0x200000;c.pc=c.pr;c.sr|=1;c.r[0]=0x160;c.r[1]=manager;c.r[2]=count;c.r[3]=manager;c.r[4]=static_cast<int>(count)>0?count:0;c.r[5]=0;if(valid){c.r[6]=4;c.r[7]=1;}return c;}
static void save_state(const std::string& path,const SH4Context& c){std::ofstream f(path);f<<std::hex<<"pc="<<c.pc<<"\npr="<<c.pr<<"\nsr="<<c.sr<<"\nfpscr="<<c.fpscr<<"\nvbr="<<c.vbr<<"\nspc="<<c.spc<<"\ngbr="<<c.gbr<<"\nmach="<<c.mach<<"\nmacl="<<c.macl<<"\nfpul="<<c.fpul<<'\n';for(unsigned i=0;i<16;++i)f<<'r'<<std::dec<<i<<'='<<std::hex<<c.r[i]<<"\nfr"<<std::dec<<i<<'='<<std::hex<<c.fr_bits[i]<<"\nxf"<<std::dec<<i<<'='<<std::hex<<c.xf_bits[i]<<'\n';need(bool(f),"Cannot save expected callback fields");}
static void save_bytes(const std::string& path,const auto& bytes){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());need(bool(f),"Cannot save expected callback bytes");}
int main(int argc,char**argv){try{
 need(argc==6,"usage: sound-completion RAM STATE AICA FLASH OUTPUT");const auto main=read(argv[1]),sound=read(argv[3]),flash=read(argv[4]);need(main.size()==16777216&&sound.size()==2097152&&flash.size()==131072,"Wrong completion fixtures");unsigned manager=word(main,0x8c67ff10),table=word(main,0x8c5ff84c);need(word(main,table+0xd4)==1&&word(main,manager+0x15c)==32&&word(main,manager+0x160)==1,"Frozen callback manager/critical mode differs");
 auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);register_recompiled_program(rt);auto reset=[&]{rt.main_ram=main;rt.aica_ram=sound;rt.system_flash=flash;};
 auto exact=[&](const auto& m,const auto& a){if(rt.main_ram!=m){auto i=std::mismatch(m.begin(),m.end(),rt.main_ram.begin()).first-m.begin();std::cerr<<"Completion RAM mismatch "<<std::hex<<(0x8c000000+i)<<" expected="<<unsigned(m[i])<<" actual="<<unsigned(rt.main_ram[i])<<'\n';throw std::runtime_error("Completion RAM/stack/guards differ");}if(rt.aica_ram!=a){auto i=std::mismatch(a.begin(),a.end(),rt.aica_ram.begin()).first-a.begin();std::cerr<<"Completion AICA mismatch "<<std::hex<<i<<" expected="<<unsigned(a[i])<<" actual="<<unsigned(rt.aica_ram[i])<<'\n';throw std::runtime_error("Completion AICA word/neighbor differs");}need(rt.system_flash==flash&&rt.g2_dma_runs==0&&rt.aica_arm.instructions==0,"Completion changes frozen devices");};unsigned writer_cases=0,callback_cases=0;
 for(unsigned offset:{0u,1u,2u,3u,0x13400u,0x13401u,0x13402u,0x13403u,0x1ffffcu,0x1ffffdu,0x1ffffeu,0x1fffffu,0x200000u,0x200001u,0x7fffffffu})for(unsigned value:{0u,1u,0x80u,0xffffffffu})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();auto c=context(0x8c66ce84,mask,t,bank);c.r[4]=offset;c.r[5]=value;auto expected=rt.main_ram,expected_sound=rt.aica_ram;
  if(offset<0x200000)for(unsigned j=0;j<4;++j)rt.aica_ram[(offset&~3u)+j]=expected_sound[(offset&~3u)+j]=0xa1+j*23;
  publish(expected,expected_sound,c,offset,value);auto wanted=published(c);call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected,expected_sound);++writer_cases;
 }
 for(unsigned count:{0u,1u,2u,31u,32u,33u,0xffffffffu,0x80000000u})for(unsigned offset:{0x13400u,0x13401u,0x13402u,0x13403u,0x200000u,0x200001u})for(unsigned mask=0;mask<16;++mask)for(unsigned t:{0u,1u})for(unsigned bank:{0u,1u}){
  reset();put(rt.main_ram,manager+0x15c,count);put(rt.main_ram,manager+0x1bc,offset);auto c=context(0x8c66a810,mask,t,bank);auto expected=rt.main_ram,expected_sound=rt.aica_ram;auto wanted=callback_state(c,expected);callback_memory(expected,expected_sound,c);call_recompiled(c,rt,c.pc);same(wanted,c);exact(expected,expected_sound);++callback_cases;
 }
 reset();auto c=state(argv[2]);need(c.pc==0x8c65e27e&&c.pr==0x8c65e284,"Actual frozen DMA callback invocation differs");auto expected=main,expected_sound=sound;auto wanted=callback_state(c,expected);callback_memory(expected,expected_sound,c);
 // Save independent expectations before actual frozen execution or a new startup.
 std::string out=argv[5];save_bytes(out+"/expected-callback-return-ram.bin",expected);save_bytes(out+"/expected-callback-return-aica.bin",expected_sound);save_state(out+"/expected-callback-return-state.txt",wanted);
 call_recompiled(c,rt,0x8c66a810);same(wanted,c);exact(expected,expected_sound);
 std::cout<<"PASS original sound completion: "<<writer_cases<<" aligned-byte writer and "<<callback_cases<<" callback cases; all four byte lanes, nonnegative bounds, signed batch counts, all16 interrupt masks/both T values/both FPU banks; exact entire RAM/AICA/stack and all58 fields, no excluded bytes; actual frozen callback publishes mailbox1, clears32 packet flags/count/busy and returns65E284; idle-G2/ARM7-off fixture, no live firmware consumption, playback/hardware/peripheral completion/gameplay/G1 claim\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
