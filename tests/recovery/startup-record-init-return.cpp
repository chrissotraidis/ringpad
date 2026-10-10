// Original64C880 storage counts and connected64C766 return; alternate missing callees reject.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void count_blocks_register(DCRuntime&);void next_block_register(DCRuntime&);void record_info_register(DCRuntime&);void timestamp_register(DCRuntime&);void bcd_register(DCRuntime&);void info_wrapper_register(DCRuntime&);void payload_validator_register(DCRuntime&);}
// Query models retained from startup-block-iteration-execution.cpp, derived from SH-4.
struct PayloadModel : RequestModel {
 void status_pointer(){ //64A596
  auto&r=c.r;r[0]=r[4];t(r[0]>>31);r[0]<<=1;r[3]=r[4];r[0]+=r[3];r[2]=w(0x8c64a5f0);r[0]<<=2;t(r[0]>>31);r[0]<<=1;c.pc=c.pr;r[0]+=r[2];cycles+=10;
 }
 void device(){ //64A5A8 ->6630E0
  auto&r=c.r;push(c.pr);r[3]=w(0x8c64a5f8);c.pr=0x8c64a5b0;cycles+=5;
  r[2]=120;r[3]=w(0x8c663118);c.macl=r[2]*r[4];r[1]=0x44c;r[0]=w(r[3]);r[4]=c.macl;r[0]+=r[1];c.pc=c.pr;r[0]+=r[4];cycles+=11;
  c.pr=pop();c.pc=c.pr;cycles+=4;
 }
 void busy(){ //64A876 ->66314E ->6630D0
  auto&r=c.r;r[3]=w(0x8c64a974);cycles+=4;push(c.pr);c.pr=0x8c663154;cycles+=4;
  r[2]=44;r[3]=w(0x8c663118);c.macl=r[2]*r[4];r[0]=w(r[3]);r[0]+=40;r[4]=c.macl;c.pc=c.pr;r[0]+=r[4];cycles+=10;
  c.pr=pop();r[4]=r[0];r[0]=sb(r[4]+12);c.pc=c.pr;r[0]&=255;cycles+=6;
 }
 void first_query(){ //664AEC
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=4;r[0]=w(0x8c664b7c);r[14]=r[4];r[14]<<=2;r[3]=w(0x8c664b80);r[14]=w(r[0]+r[14]);c.pr=0x8c664b00;r[4]=r[14];range(0xaec,0xafe,1);status_pointer();
  r[3]=w(0x8c664b84);put(m,r[15],r[0]);c.pr=0x8c664b08;r[4]=r[14];range(0xb00,0xb06,1);device();
  r[2]=w(r[15]);r[4]=r[0];r[3]=sb(r[2]);t(r[3]==0);range(0xb08,0xb10);bool ready=false;
  if(!(c.sr&1)){r[0]=w(r[4]);t((r[0]&2)==0);range(0xb12,0xb16);ready=!(c.sr&1);}
  r[0]=ready?1:0;r[15]+=4;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void second_query(){ //664B2C
  auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[0]=w(0x8c664b7c);r[14]=r[4];r[14]<<=2;r[3]=w(0x8c664b80);r[14]=w(r[0]+r[14]);c.pr=0x8c664b40;r[4]=r[14];range(0xb2c,0xb3e,1);status_pointer();
  r[3]=w(0x8c664b84);put(m,r[15]+4,r[0]);c.pr=0x8c664b48;r[4]=r[14];range(0xb40,0xb46,1);device();
  put(m,r[15],r[0]);r[3]=w(r[15]+4);r[2]=sb(r[3]);t(r[2]==0);range(0xb48,0xb50);bool ready=false;
  if(!(c.sr&1)){r[3]=w(0x8c664b88);c.pr=0x8c664b58;r[4]=r[14];range(0xb52,0xb56,1);busy();t(r[0]==0);range(0xb58,0xb5a);
   if(c.sr&1){r[0]=w(r[15]);r[0]=w(r[0]);t((r[0]&2)==0);range(0xb5c,0xb62);ready=!(c.sr&1);}
  }
  r[0]=ready?1:0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=6;
 }
 void finish(){auto&r=c.r;r[15]+=4;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();range(0x270,0x278,1);}
 void unavailable(){c.r[0]=0xffffff01;range(0x238,0x23c);finish();}
 void validate(){ //64E1F2, actual first-query success and all downstream branches
  auto&r=c.r;push(r[14]);push(r[13]);push(c.pr);r[15]-=4;r[14]=0x148;r[13]=r[4];r[3]=w(0x8c64e288);c.macl=r[14]*r[13];r[14]=c.macl;put(m,r[15],r[14]);r[14]+=r[3];r[0]=sb(r[14]+2);t(r[0]==0);range(0x1f2,0x20c,1);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64e28c);c.pr=0x8c64e214;r[4]=r[13];range(0x20e,0x212,1);first_query();t(r[0]==0);range(0x214,0x216);
  need(!(c.sr&1),"Missing alternate64C24A requires its own source reduction; never assume return");
  r[2]=w(0x8c64e294);c.pr=0x8c64e228;r[4]=r[13];range(0x222,0x226,1);second_query();t(r[0]==0);range(0x228,0x22a);
  if(c.sr&1){unavailable();return;}
  r[2]=w(r[15]);r[3]=w(0x8c64e288);r[2]+=r[3];r[0]=sb(r[2]+8);t(r[0]==0);range(0x22c,0x236);
  if(c.sr&1){unavailable();return;}
  r[5]=w(r[14]+32);r[4]=0;r[7]=16;r[6]=r[4];range(0x23e,0x244);
  do{r[0]=sb(r[5]);r[0]&=255;t(r[0]==85);range(0x246,0x24c);
   if(!(c.sr&1)){r[0]=r[4];wb(r[14]+11,r[0]);wb(r[14]+12,r[0]);range(0x24e,0x254);r[0]=0xffffff03;range(0x268,0x26c);finish();return;}
   ++r[6];t(int(r[6])>=int(r[7]));++r[5];range(0x256,0x25c);
  }while(!(c.sr&1));
  r[0]=w(0x8c64e298);r[13]<<=2;r[3]=w(r[0]+r[13]);t(r[3]==0);range(0x25e,0x266);
  if(c.sr&1){r[0]=0;++cycles;}else{r[0]=0xffffff03;range(0x268,0x26c);}finish();
 }
};

struct InfoModel : PayloadModel {
 void wh(unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(m.data()+(a&0xffffff),&x,2);}
 void bcd(){auto&r=c.r;r[5]=r[4];r[5]>>=2;r[5]>>=2;r[0]=r[5];r[3]=r[5];r[0]<<=2;r[2]=15;r[0]+=r[3];t(r[0]>>31);r[0]<<=1;r[4]&=r[2];c.pc=c.pr;r[0]+=r[4];cycles+=13;}
 void timestamp(){
  auto&r=c.r;push(r[14]);r[14]=r[5];push(r[13]);r[13]=r[4];push(r[8]);push(c.pr);r[4]=sb(r[14])&255;c.pr=0x8c64f1c4;cycles+=10;bcd();
  r[2]=100;c.macl=(r[2]&65535)*(r[0]&65535);r[0]=c.macl;r[8]=r[0];r[0]=sb(r[14]+1);r[4]=r[0]&255;c.pr=0x8c64f1d2;cycles+=9;bcd();
  r[8]+=r[0];wh(r[13],r[8]);r[0]=sb(r[14]+2);r[4]=r[0]&255;c.pr=0x8c64f1dc;cycles+=6;bcd();
  for(unsigned i=2;i<=5;++i){wb(r[13]+i,r[0]);r[0]=sb(r[14]+i+1);r[4]=r[0]&255;c.pr=0x8c64f1e4+(i-2)*8;cycles+=5;bcd();}
  wb(r[13]+6,r[0]);r[0]=sb(r[14]+7);r[1]=w(0x8c64f220);r[0]&=255;r[0]&=7;r[0]=sb(r[0]+r[1]);wb(r[13]+7,r[0]);c.pr=pop();r[8]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=13;
 }

 // Source: reviewed64C880; next() retained from the verified linked-block reduction.
 void next(){auto&r=c.r;r[15]-=8;r[3]=0x148;r[1]=w(0x8c64ef2c);c.macl=r[3]*r[4];r[2]=w(0x8c64ef28);put(m,r[15],r[5]);r[4]=c.macl;r[4]+=r[2];put(m,r[15]+4,r[4]);r[0]=w(r[1]);r[5]=w(r[15]);r[4]=w(r[4]+36);r[0]=w(r[0]+12);t(r[5]>>31);r[5]<<=1;r[15]+=8;cycles+=18;
  need(r[0]==0x8c664556,"Unreviewed next-block provider");r[4]+=r[5];std::int16_t v;std::memcpy(&v,m.data()+(r[4]&0xffffff),2);r[1]=unsigned(int(v));c.pc=c.pr;r[0]=r[1]&65535;cycles+=5;
 }
 void count_finish(){auto&r=c.r;r[15]+=8;c.pr=pop();r[10]=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=9;}
 void count_unavailable(){c.r[0]=unsigned(int(std::int16_t(w(0x8c64c910))));cycles+=3;count_finish();}
 void count_blocks(){
  auto&r=c.r;for(unsigned i=14;i>=10;--i)push(r[i]);push(c.pr);r[15]-=8;
  r[12]=unsigned(int(std::int16_t(w(0x8c64c90e))));r[14]=r[4];r[3]=w(0x8c64c918);c.macl=r[12]*r[14];put(m,r[15],r[5]);r[12]=c.macl;put(m,r[15]+4,r[12]);r[12]+=r[3];r[0]=sb(r[12]+2);t(r[0]==0);cycles+=19;
  if(c.sr&1){count_unavailable();return;}
  r[3]=w(0x8c64c91c);c.pr=0x8c64c8aa;r[4]=r[14];cycles+=4;first_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){c.pr=0x8c64c8b2;r[4]=r[14];c.pc=0x8c64c8ae;missing=0x8c64c24a;cycles+=3;return;}
  r[2]=w(0x8c64c920);c.pr=0x8c64c8bc;r[4]=r[14];cycles+=4;second_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){count_unavailable();return;}
  r[2]=w(r[15]+4);r[3]=w(0x8c64c918);r[2]+=r[3];r[0]=sb(r[2]+8);t(r[0]==0);cycles+=6;
  if(c.sr&1){count_unavailable();return;}
  r[3]=w(0x8c64c924);need(r[3]==0x8c606bdc,"Unreviewed count validator wrapper");c.pr=0x8c64c8d8;r[4]=r[14];cycles+=4;
  r[3]=w(0x8c606c90);need(r[3]==0x8c64e1f2,"Unreviewed count validator");cycles+=4;validate();t(int(r[0])>=0);cycles+=2;
  if(!(c.sr&1)){r[0]=unsigned(int(std::int16_t(w(0x8c64c912))));cycles+=3;count_finish();return;}
  r[0]=w(r[15]);r[4]=0;r[13]=w(0x8c64c928);t(r[0]==1);r[10]=r[4];cycles+=6;
  if(c.sr&1){
   r[11]=r[4];cycles+=2;
   for(;;){r[2]=w(r[12]+28);r[3]=w(r[2]+56);t(int(r[11])>=int(r[3]));cycles+=4;if(c.sr&1)break;
    r[3]=w(0x8c64c92c);need(r[3]==0x8c64ef00,"Unreviewed leading lookup");r[5]=r[11];c.pr=0x8c64c8fa;r[4]=r[14];cycles+=5;next();r[0]&=65535;t(r[0]==r[13]);cycles+=3;if(!(c.sr&1))break;++r[11];++cycles;
   }r[10]=r[11];cycles+=2;
  }else{
   r[3]=w(0x8c64ca08);r[2]=w(r[3]);t(r[2]==0);cycles+=4;const bool gated=!(c.sr&1);r[11]=r[4];cycles+=2;
   for(;;){r[3]=w(r[12]+28);r[2]=w(r[3]+48);t(int(r[11])>=int(r[2]));cycles+=4;if(c.sr&1)break;
    r[3]=w(0x8c64ca0c);need(r[3]==0x8c64ef00,"Unreviewed free lookup");r[5]=r[11];c.pr=gated?0x8c64c944:0x8c64c972;r[4]=r[14];cycles+=5;next();r[0]&=65535;t(r[0]==r[13]);cycles+=3;
    if(gated){if(c.sr&1){r[3]=w(0x8c64ca10);need(r[3]==0x8c64ee6a,"Unexpected gated helper");r[5]=r[11];c.pr=0x8c64c952;r[4]=r[14];c.pc=0x8c64c94a;missing=r[3];cycles+=5;return;}++r[11];++cycles;}
    else{++r[11];++cycles;if(c.sr&1){++r[10];++cycles;}}
   }if(gated)cycles+=2;
  }
  r[0]=r[10];++cycles;count_finish();
 }
 unsigned missing=0;
 void unavailable(){auto&r=c.r;r[0]=unsigned(int(std::int16_t(w(0x8c64c7f2))));cycles+=3;r[15]+=12;c.pr=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=8;}
 void run(){
  auto&r=c.r;for(unsigned i=14;i>=11;--i)push(r[i]);push(c.pr);r[15]-=12;
  r[13]=unsigned(int(std::int16_t(w(0x8c64c7f0))));r[12]=r[4];r[3]=w(0x8c64c7f8);c.macl=r[13]*r[12];r[13]=c.macl;put(m,r[15],r[13]);r[13]+=r[3];r[0]=sb(r[13]+2);t(r[0]==0);r[14]=r[5];range(0xc766,0xc786,1);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64c800);c.pr=0x8c64c78e;r[4]=r[12];range(0xc788,0xc78c,1);first_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){c.pr=0x8c64c796;r[4]=r[12];c.pc=0x8c64c792;missing=0x8c64c24a;cycles+=3;return;}
  r[2]=w(0x8c64c804);c.pr=0x8c64c7a0;r[4]=r[12];range(0xc79a,0xc79e,1);second_query();t(r[0]==0);cycles+=2;
  if(c.sr&1){unavailable();return;}
  r[2]=w(r[15]);r[3]=w(0x8c64c7f8);r[2]+=r[3];r[0]=sb(r[2]+8);t(r[0]==0);range(0xc7a4,0xc7ae);
  if(c.sr&1){unavailable();return;}
  r[3]=w(0x8c64c808);c.pr=0x8c64c7bc;r[4]=r[12];c.pc=0x8c64c7b6;range(0xc7b6,0xc7ba,1);need(r[3]==0x8c606bdc,"Unexpected wrapper target");
  r[3]=w(0x8c606c90);need(r[3]==0x8c64e1f2,"Unexpected validator target");cycles+=4;validate();
  t(int(r[0])>=0);cycles+=2;
  if(!(c.sr&1)){r[0]=unsigned(int(std::int16_t(w(0x8c64c7f4))));cycles+=3;r[15]+=12;c.pr=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=8;return;}
  r[4]=0;r[5]=32;cycles+=3;
  for(;;){t(int(r[4])>=int(r[5]));cycles+=2;if(c.sr&1)break;r[3]=r[4];r[0]=w(r[13]+32);r[3]+=16;r[2]=r[14];r[1]=sb(r[0]+r[3]);r[2]+=r[4];++r[4];wb(r[2],r[1]);cycles+=8;}
  r[11]=r[15];r[5]=w(r[13]+32);r[11]+=4;r[6]=r[11];r[4]=r[11];r[5]+=48;r[6]+=8;cycles+=8;
  for(;;){t(r[4]>=r[6]);cycles+=2;if(c.sr&1)break;r[3]=sb(r[5]);++r[5];wb(r[4],r[3]);++r[4];cycles+=3;}
  r[3]=w(0x8c64c914);r[4]=r[14];r[5]=r[11];c.pr=0x8c64c820;r[4]+=48;c.pc=0x8c64c816;cycles+=6;need(r[3]==0x8c64f1b2,"Unexpected conversion target");timestamp();
  r[2]=w(r[13]+28);r[0]=32;r[5]=0;r[3]=w(r[2]);++r[3];wh(r[14]+r[0],r[3]);r[0]=34;r[2]=w(r[13]+28);r[3]=w(r[2]+48);wh(r[14]+r[0],r[3]);c.pr=0x8c64c838;r[4]=r[12];c.pc=0x8c64c820;cycles+=13;count_blocks();if(missing)return;
  r[1]=38;r[1]+=r[14];wh(r[1],r[0]);r[0]=38;r[3]=unsigned(int(std::int16_t(w(r[14]+r[0]))));r[0]=36;r[5]=1;wh(r[14]+r[0],r[3]);r[0]=40;r[2]=w(r[13]+28);r[3]=w(r[2]+56);wh(r[14]+r[0],r[3]);c.pr=0x8c64c854;r[4]=r[12];cycles+=15;count_blocks();if(missing)return;
  r[1]=42;r[1]+=r[14];wh(r[1],r[0]);r[0]=44;r[3]=w(r[13]+28);r[2]=w(r[3]+44);++r[2];wh(r[14]+r[0],r[2]);r[0]=78;r[3]=w(r[13]+32);r[2]=sb(r[3]+r[0]);r[0]=46;r[2]&=255;wh(r[14]+r[0],r[2]);r[0]=0;r[15]+=12;c.pr=pop();r[11]=pop();r[12]=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=23;
 }
};

namespace dcrecomp_generated {void record_init_register(DCRuntime&);}
struct InitPrefix : RequestModel {
    void start() {
        auto& r=c.r;push(r[14]);r[14]=76;c.macl=r[14]*r[4];push(r[13]);
        r[0]=r[5];r[3]=w(0x8c010f48);t(r[0]==1);push(c.pr);r[14]=c.macl;
        r[14]+=r[3];r[13]=0;cycles+=13;
        need(c.sr&1,"Prefix only models original mode1");
        t(r[6]==0);cycles+=2;
    }
    void provider_entry() {
        start();need(c.sr&1,"Expected zero-argument provider path");auto& r=c.r;
        r[2]=w(0x8c010f4c);r[1]=1;r[5]=r[14];
        std::uint16_t one=1;std::memcpy(m.data()+(r[14]&0xffffff),&one,2);
        r[5]+=20;c.pr=0x8c010ece;c.pc=0x8c606c9c;cycles+=7;
    }
    void skip_return() {
        start();need(!(c.sr&1),"Expected nonzero-argument skip");
        put(m,c.r[14]+16,c.r[13]);c.r[0]=0;c.pr=pop();c.r[13]=pop();
        c.pc=c.pr;c.r[14]=pop();cycles+=7;
    }
    bool provider() {
        auto& r=c.r;push(c.pr);r[15]-=8;r[3]=w(0x8c606f50);
        put(m,r[15],r[4]);put(m,r[15]+4,r[5]);
        need((r[3]&0x1fffffff)==0x0c64b9e4,"Provider query target changed");
        c.pr=0x8c606caa;r[4]=w(r[15]);cycles+=8;
        r[4]<<=2;r[3]=w(0x8c64ba50);r[4]<<=2;r[4]<<=2;
        t(r[4]>>31);r[4]<<=1;r[4]+=r[3];r[0]=w(r[4]+4);c.pc=c.pr;cycles+=9;
        t(r[0]==0);cycles+=2;
        if(!(c.sr&1)){r[15]+=8;c.pr=pop();c.pc=c.pr;r[0]=0xffffffff;cycles+=5;return true;}
        r[4]=w(r[15]);r[5]=w(r[15]+4);r[15]+=8;r[3]=w(0x8c606f54);
        need(r[3]==0x8c64c766,"Provider tail target changed");
        c.pr=pop();c.pc=0x8c606cb6;cycles+=7;return false;
    }
    void busy_return() {
        provider_entry();need(provider(),"Busy provider expected to return");auto& r=c.r;
        r[4]=r[0];t(r[4]==0);cycles+=3;need(!(c.sr&1),"Busy return unexpectedly zero");
        put(m,r[14]+4,r[13]);cycles+=2;
        put(m,r[14]+16,r[13]);r[0]=0;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
    }
};

struct InitReturn : InitPrefix {
 unsigned missing=0;
 void run(){
  if(c.r[6]){skip_return();return;}
  provider_entry();const bool busy=provider();
  if(!busy){InfoModel info{{{c,m}}};info.run();c=info.c;cycles+=info.cycles;missing=info.missing;if(missing)return;}
  auto&r=c.r;r[4]=r[0];t(r[4]==0);cycles+=3;
  if(c.sr&1){r[0]=1;std::uint16_t one=1;std::memcpy(m.data()+((r[14]+2)&0xffffff),&one,2);cycles+=2;}
  put(m,r[14]+4,r[13]);cycles+=2;put(m,r[14]+16,r[13]);r[0]=0;c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();cycles+=7;
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-init-return RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
 load_embedded_elf_image(rt);registers(rt);record_init_register(rt);record_info_register(rt);info_wrapper_register(rt);payload_validator_register(rt);timestamp_register(rt);bcd_register(rt);count_blocks_register(rt);next_block_register(rt);
 const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];
 unsigned controls=0,returns=0,rejections=0;
 auto check=[&](SH4Context c,bool capture){auto expected=rt.main_ram;InitReturn model{{{c,expected}}};model.run();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(expected,out+"/init-expected-ram.bin");save_state(model.c,out+"/init-expected-state.txt");save_timers(timers,start,tocr,out+"/init-expected-timers.txt");std::ofstream(out+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<",\"missing_target_decimal\":"<<model.missing<<"}\n";}
  bool rejected=false;try{call_recompiled(c,rt,0x8c010e9e);}catch(const std::runtime_error&e){std::ostringstream target;target<<"No recompiled/native target registered for Dreamcast address 0x"<<std::uppercase<<std::hex<<model.missing;need(model.missing&&std::string(e.what()).find(target.str())!=std::string::npos,e.what());rejected=true;}
  need(rejected==bool(model.missing),"Unexpected initializer outcome");same(model.c,c);need(expected==rt.main_ram,"Initializer whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Timer controls differ");need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
  if(capture){save_ram(rt.main_ram,out+"/init-actual-ram.bin");save_state(c,out+"/init-actual-state.txt");save_timers(rt.tmu,start,tocr,out+"/init-actual-timers.txt");}else{++controls;if(rejected)++rejections;else ++returns;}
  return std::pair{model.cycles,model.missing};
 };
 auto reset=[&](unsigned index,unsigned bank,unsigned alias){rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[4]=index;c.r[5]=1;c.r[6]=0;c.r[15]=alias?0xacefe000:0x8cefe000;c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;return c;};
 for(unsigned index:{0u,1u,7u})for(unsigned value:{1u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
  auto c=reset(index,bank,alias);c.r[6]=value;auto result=check(c,false);need(result.first==22&&!result.second,"Original skip regression failed");
 }
 for(unsigned index:{0u,1u,7u})for(unsigned status:{1u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
  auto c=reset(index,bank,alias);put(rt.main_ram,word(ram,0x8c64ba50)+index*128+4,status);auto result=check(c,false);need(result.first==58&&!result.second,"Original busy regression failed");
 }
 const auto record=word(ram,0x8c64c7f8),port=word(ram,word(ram,0x8c664b7c)),status=word(ram,0x8c64a5f0)+24*port,shared=word(ram,word(ram,0x8c663118));
 for(unsigned present:{0u,1u})for(unsigned connected:{0u,1u})for(unsigned capability:{0u,2u})for(unsigned busy:{0u,1u})for(unsigned valid:{0u,1u})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u})for(unsigned corrupt:{0u,1u}){
  auto c=reset(0,bank,alias);rt.main_ram[(record+2)&0xffffff]=present;rt.main_ram[(record+8)&0xffffff]=valid;rt.main_ram[status&0xffffff]=connected;put(rt.main_ram,shared+0x44c+120*port,capability);rt.main_ram[(shared+40+44*port+12)&0xffffff]=busy;if(corrupt)rt.main_ram[word(ram,record+32)&0xffffff]=0;check(c,false);
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);auto result=check(initial,true);need(result.first==14278&&!result.second,"Captured initializer return differs from prospective prediction");
 const unsigned record_base=word(ram,0x8c010f48);need(word(rt.main_ram,record_base)==0x00010001&&word(rt.main_ram,record_base+4)==0&&word(rt.main_ram,record_base+16)==0,"Independent initialized flags differ");
 std::ofstream(out+"/result.json")<<"{\"passed\":true,\"controls\":"<<controls<<",\"returns\":"<<returns<<",\"rejections\":"<<rejections<<",\"captured_cycles\":"<<result.first<<",\"whole_RAM_CPU_TMU_verified\":true,\"mode1_parent_full_return_verified\":true,\"other_modes_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record init return: "<<controls<<" controls;"<<returns<<" returns,"<<rejections<<" genuine missing-callee rejections;captured14278cycle complete mode1 return;whole RAM CPU TMU;normal boot unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
