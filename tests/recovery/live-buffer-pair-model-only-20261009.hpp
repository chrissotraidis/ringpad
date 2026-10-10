// Exact prefix of sealed startup-resident-buffer-pair.cpp, ending before guest driver declarations.
// Original create prefix builder and retained parent, stopped at the real next helper.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
#include "capture-storage.hpp"
#include <cfenv>

using SQFields=std::array<std::uint64_t,12>;
static const char* sqkeys[]={"qacr0","qacr1","qacr_writes","pref_calls","pref_sq_calls","sq_commits","pref_ta_commits","pref_texture_commits","pref_other_commits","last_pref_address","last_pref_target","last_pref_source_pc"};
static SQFields sqfields(const DCRuntime&r){return{r.qacr[0],r.qacr[1],r.qacr_writes,r.pref_calls,r.pref_sq_calls,r.sq_commits,r.pref_ta_commits,r.pref_texture_commits,r.pref_other_commits,r.last_pref_address,r.last_pref_target,r.last_pref_source_pc};}
static SQFields read_sq(const char*path){std::ifstream f(path);need(bool(f),"Missing SQ state");SQFields a{};std::string line;for(unsigned i=0;i<12;++i){need(bool(std::getline(f,line)),"Incomplete SQ state");auto eq=line.find('=');need(line.substr(0,eq)==sqkeys[i],"SQ field order differs");a[i]=std::stoull(line.substr(eq+1),nullptr,16);}need(!std::getline(f,line),"Extra SQ state");return a;}
static void restore_sq(DCRuntime&r,const auto&bytes,const SQFields&a){need(bytes.size()==64,"SQ fixture size differs");std::copy(bytes.begin(),bytes.end(),r.store_queues.begin());r.qacr={unsigned(a[0]),unsigned(a[1])};r.qacr_writes=a[2];r.pref_calls=a[3];r.pref_sq_calls=a[4];r.sq_commits=a[5];r.pref_ta_commits=a[6];r.pref_texture_commits=a[7];r.pref_other_commits=a[8];r.last_pref_address=a[9];r.last_pref_target=a[10];r.last_pref_source_pc=a[11];}
static void save_sq(const auto&bytes,const SQFields&a,const std::string&stem){save_ram(bytes,stem+"-store-queues.bin");std::ofstream f(stem+"-store-queue-state.txt");for(unsigned i=0;i<12;++i)f<<sqkeys[i]<<'='<<std::hex<<a[i]<<'\n';need(bool(f),"Cannot save SQ expectation");}

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
static void original_word_write(auto&m,auto&sound,SH4Context&c){
 const auto original=c;const unsigned sp=c.r[15],address=c.r[4],value=c.r[5],table=word(m,0x8c5ff84c),mask=(c.sr>>4)&15;
 need(address<0x200000&&word(m,table+0xd4)==1,"Only reached idle G2 word path modeled");
 for(unsigned i=0;i<4;++i){unsigned channel=word(m,table+4+52*i);need((word(m,table)+32*channel)==0xa05f7800+32*i,"Unexpected G2 register layout");}
 put(m,sp-4,c.pr);put(m,sp-8,value);put(m,sp-12,address);put(m,sp-16,0);put(m,sp-20,0);
 const unsigned writesp=sp-20;for(unsigned i=0;i<7;++i)put(m,writesp-4-4*i,c.r[14-i]);put(m,writesp-32,0x8c66af68);
 const unsigned local=writesp-48;put(m,local,0xa0800000+address);put(m,local+4,4);put(m,local+12,mask);
 guard_stack(m,table,local,0x8c10f40c,c.r[14],1,c.r[12],c.r[11],true);
 fifo_stack(m,table,local,0x8c10f412,c.r[14],1,false);
 put(m,local,sp-8);put(m,local+4,0xa0800000+address);put(m,local+8,0);
 fifo_stack(m,table,local,0x8c10f4ce,7,1,false);fifo_stack(m,table,local,0x8c10f4d2,7,1,true);
 put(sound,address,value);guard_stack(m,table,local,0x8c10f4f4,7,1,0x8c110b00,0x8c1107cc,false);
 c.pc=c.pr;c.r[0]=0;c.r[1]=0xd4;c.sr|=1;c.r[2]=c.sr&0xffffff0f;c.r[3]=0xffffff0f;c.r[4]=0xa05f7860;c.r[5]=local-32;c.r[6]=4;c.r[7]=1;
}
static void buffer_model(auto&expected_ram,auto&expected_sound,SH4Context&expected){
  auto&r=expected.r;
  for(unsigned i=14;i>=11;--i){r[15]-=4;put(expected_ram,r[15],r[i]);}r[15]-=4;put(expected_ram,r[15],expected.pr);r[15]-=8;
  r[3]=word(expected_ram,0x8c65e51c);put(expected_ram,r[15],r[6]);put(expected_ram,r[15]+4,r[7]);r[6]=word(expected_ram,r[3]);need(r[6]!=0,"Captured global uninitialized");r[12]=0;r[7]=static_cast<unsigned>(static_cast<int>(static_cast<int8_t>(r[5])));need(r[7]<16&&r[4]==0x52535053,"Only reached SPRS path modeled");
  r[11]=r[7]*8;r[13]=1;r[14]=16;r[0]=r[4];r[1]=word(expected_ram,0x8c65e530);need(r[1]==r[0],"SPRS literal differs");expected.sr&=~1u;
  auto half=[&](unsigned a){return static_cast<unsigned>(static_cast<int>(static_cast<int16_t>(word(expected_ram,a))));};
  r[0]=half(0x8c65e5e4);r[3]=half(0x8c65e5ea);r[13]=word(expected_ram,r[6]+r[0]);r[14]=half(0x8c65e5ec);r[13]+=r[3]+r[11];r[14]+=r[6]+r[11];r[3]=word(expected_ram,0x8c65e604);r[5]=word(expected_ram,r[15]);r[4]=r[13];expected.pr=0x8c65e5c2;expected.pc=0x8c65e5ba;need(r[3]==0x8c66af40,"Next original write helper differs");
  original_word_write(expected_ram,expected_sound,expected);
  r[3]=word(expected_ram,0x8c65e604);r[4]=r[13]+4;r[5]=word(expected_ram,r[15]+4);expected.pr=0x8c65e5cc;original_word_write(expected_ram,expected_sound,expected);
  r[2]=word(expected_ram,r[15]);put(expected_ram,r[14],r[2]);r[3]=word(expected_ram,r[15]+4);put(expected_ram,r[14]+4,r[3]);r[0]=r[12];r[15]+=8;expected.pr=word(expected_ram,r[15]);r[15]+=4;for(unsigned i=11;i<=14;++i){r[i]=word(expected_ram,r[15]);r[15]+=4;}expected.pc=expected.pr;
}

static void logger_model(auto&m,SH4Context&c){
 auto&r=c.r;need(word(m,0x8c67ff1c)==0,"Reached logging disabled path changed");r[2]=0x8c67ff1c;r[15]-=4;put(m,r[15],c.pr);r[3]=0;c.sr|=1;c.pr=word(m,r[15]);r[15]+=4;r[0]=0;c.pc=c.pr;
}
static unsigned allocate_model(auto&m,SH4Context&c){
 auto&r=c.r;for(unsigned i=14;i>=8;--i){r[15]-=4;put(m,r[15],r[i]);}r[15]-=4;put(m,r[15],c.pr);r[15]-=4;
 r[3]=0x8c67ff10;r[9]=0;put(m,r[15],r[4]);r[11]=r[9];r[4]=word(m,r[3]);need(r[4]!=0,"Allocator global absent");r[0]=0x17c;c.sr&=~1u;r[14]=word(m,r[4]+r[0]);
 r[2]=0x8c670b04;r[12]=0x8c65e03c;c.pr=0x8c647252;r[15]-=4;put(m,r[15],r[2]);logger_model(m,c);
 r[15]+=4;r[8]=0x8c670b10;r[10]=64;r[13]=0;unsigned occupied=0;
 for(;;){
  r[15]-=4;put(m,r[15],r[13]);c.pr=0x8c647260;r[15]-=4;put(m,r[15],r[8]);logger_model(m,c);
  r[0]=word(m,r[14]);c.sr=(c.sr&~1u)|(r[0]==1);r[15]+=8;if(r[0]!=1)break;
  ++r[13];c.sr=(c.sr&~1u)|(r[13]>=r[10]);r[14]+=32;++occupied;need(occupied<64,"Only reached successful allocation modeled");
 }
 r[2]=0x8c670b20;r[15]-=4;put(m,r[15],r[14]);c.pr=0x8c64728a;r[15]-=4;put(m,r[15],r[2]);logger_model(m,c);r[15]+=8;
 r[3]=0x8c10e958;r[1]=r[14];r[2]=0x8c6774bc;c.pr=0x8c647296;r[0]=32;
 r[15]-=4;put(m,r[15],r[3]);for(unsigned i=0;i<32;i+=4)put(m,r[1]+i,word(m,r[2]+i));r[0]=word(m,r[2]+4);r[3]=word(m,r[15]);r[15]+=4;
 r[1]=word(m,r[15]);r[0]=0x184;put(m,r[1],r[14]);r[1]=0x8c67ff10;r[3]=word(m,r[1]);r[2]=word(m,r[3]+r[0])+1;put(m,r[3]+r[0],r[2]);c.sr|=1;
 r[0]=r[11];r[15]+=4;c.pr=word(m,r[15]);r[15]+=4;for(unsigned i=8;i<=14;++i){r[i]=word(m,r[15]);r[15]+=4;}c.pc=c.pr;
 return 120+21*occupied;
}
static unsigned caller_model(auto&m,auto&sound,SH4Context&c){
 auto&r=c.r;unsigned cycles=0;
 auto check_buffer=[&](){need(r[0]==0,"Buffer return failure");r[4]=r[0];c.sr|=1;++r[9];c.sr=(c.sr&~1u)|(r[9]>=r[11]);cycles+=6;};
 check_buffer();need(r[9]==1&&r[11]==2,"Only original two-buffer path modeled");
 r[5]=word(m,r[15]);r[2]=word(m,0x8c66bab8);r[5]+=4;r[4]=word(m,0x8c66babc);put(m,r[15],r[5]);r[5]-=4;r[5]=word(m,r[5]);r[6]=-r[5]+16;r[3]=r[6];r[6]=(r[6]<<8)+r[3];r[3]=word(m,0x8c66bac0);r[6]=-((r[6]<<2)<<2<<2)+r[2];c.pr=0x8c66ba16;r[7]=r[8];c.pc=0x8c66b9ee;cycles+=21;
 need(r[5]==14&&r[6]==0x1f7f80&&r[7]==0x4000,"Second buffer arguments differ");buffer_model(m,sound,c);cycles+=1331;check_buffer();
 put(m,r[15],r[13]);cycles+=3;
 auto loop_head=[&](){r[3]=word(m,r[15]);r[9]=0x8c676c04;r[8]=0x8c6472fc;c.sr=(c.sr&~1u)|(r[3]>=r[11]);cycles+=5;};
 loop_head();
 for(unsigned i=0;i<2;++i){
  r[2]=0x8c647220;r[4]=r[15]+16;c.pr=0x8c66ba34;cycles+=5;cycles+=allocate_model(m,c);
  need(r[0]==0,"Pool allocation failed");c.sr|=1;put(m,r[15]+4,r[0]);cycles+=4;
  r[3]=word(m,r[15]);r[2]=r[14];r[1]=word(m,r[15]+16);r[2]+=24;r[3]=(r[3]<<2)+r[2];put(m,r[3],r[1]);r[3]=word(m,r[15])+1;put(m,r[15],r[3]);cycles+=10;loop_head();
 }
 put(m,r[15],r[13]);cycles+=3;
 auto second_head=[&](){r[2]=word(m,r[15])<<2;put(m,r[15]+8,r[2]);r[3]=word(m,r[15]);c.sr=(c.sr&~1u)|(r[3]>=r[11]);cycles+=6;};
 second_head();
 for(unsigned i=0;i<2;++i){
  r[2]=0x8c647220;r[4]=r[15]+16;c.pr=0x8c66bae0;cycles+=5;cycles+=allocate_model(m,c);
  need(r[0]==0,"Pool allocation failed");c.sr|=1;put(m,r[15]+4,r[0]);cycles+=3;
  r[2]=r[14];r[3]=word(m,r[15]+8);r[2]+=32;r[1]=word(m,r[15]+16);r[2]+=r[3];put(m,r[2],r[1]);r[3]=word(m,r[15])+1;put(m,r[15],r[3]);cycles+=9;second_head();
 }
 r[2]=r[15];r[0]=r[11];r[2]+=24;c.sr=(c.sr&~1u)|(r[0]==1);put(m,r[15]+4,r[2]);r[5]=r[11];cycles+=7;
 r[2]=word(m,r[15]+28);r[2]=static_cast<int8_t>(r[2]);r[15]-=4;put(m,r[15],r[2]);r[3]=word(m,r[15]+8);r[2]=word(m,r[3]);r[3]=word(m,0x8c66bd04);r[2]=static_cast<int8_t>(r[2]);r[15]-=4;put(m,r[15],r[2]);r[15]-=4;put(m,r[15],r[5]);r[4]=r[15]+32;c.pr=0x8c66bbb8;c.pc=0x8c66bba0;cycles+=13;
 need(r[3]==0x8c66cfc4,"Next target changed");return cycles;
}
