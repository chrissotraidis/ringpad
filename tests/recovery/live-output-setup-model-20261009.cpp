// Source-derived output setup, scalar exact-value FPU and actual pool allocation.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
struct BufferFlagsModel:RequestModel{
 unsigned sw(unsigned a){return unsigned(int(std::int16_t(m[a&0xffffff]|(m[(a+1)&0xffffff]<<8))));}
 void validator(){auto&r=c.r;r[3]=w(0x8ced86d4);t(r[4]==0);put(m,r[3],r[4]);need(r[4]!=0,"Valid fixture requires buffer");r[0]=sw(0x8ced86ca);r[0]=w(r[4]+r[0]);t(r[0]==2);need(c.sr&1,"Valid fixture requires status2");r[0]=0;c.pc=c.pr;cycles+=12;}
 void flags(){auto&r=c.r;push(r[14]);push(c.pr);r[15]-=8;r[14]=r[4];t(r[14]==0);put(m,r[15],r[5]);put(m,r[15]+4,r[6]);c.pr=0x8ced8624;r[4]=r[14];cycles+=11;validator();t(r[0]==0);r[4]=sw(0x8ced86c8);r[4]+=r[14];r[0]=w(r[15]);r[3]=w(r[15]+4);r[0]<<=2;put(m,r[4]+r[0],r[3]);r[0]=0;r[15]+=8;c.pr=pop();c.pc=c.pr;r[14]=pop();cycles+=14;}
};
struct HandlerPoolModel:RequestModel{
 bool full=false;unsigned slot=0;
 void pool(){auto&r=c.r;push(r[14]);r[0]=64;push(c.pr);r[14]=w(0x8c6621e4);r[7]=0;cycles+=5;
 for(;;){r[3]=r[7];t(r[3]>>31);r[3]<<=1;r[2]=r[7];r[3]+=r[2];r[3]<<=4;r[3]+=r[14];r[1]=w(r[3]+4);t(r[1]==0);cycles+=10;if(c.sr&1)break;++r[7];t(static_cast<int32_t>(r[7])>=static_cast<int32_t>(r[0]));cycles+=3;if(c.sr&1)break;}
 slot=r[7];r[0]=r[7];t(r[0]==64);cycles+=4;full=c.sr&1;
 if(full){c.pr=pop();r[0]=0;c.pc=c.pr;r[14]=pop();cycles+=5;return;}
 r[3]=r[7];t(r[7]>>31);r[7]<<=1;r[7]+=r[3];r[3]=w(0x8c6621ec);r[7]<<=4;r[2]=1;r[14]+=r[7];put(m,r[14]+4,r[2]);put(m,r[14],r[3]);r[3]=w(0x8c6621f4);r[2]=w(0x8c6621f0);put(m,r[14]+28,r[4]);put(m,r[14]+32,r[5]);put(m,r[14]+36,r[6]);put(m,r[14]+8,r[2]);put(m,r[14]+40,r[3]);put(m,r[14]+44,r[14]);c.pr=0x8c662162;r[4]=r[14];c.pc=0x8c66213a;cycles+=21;
 }
};

static unsigned output_setup(auto&expected_ram,SH4Context&c){
RequestModel m{c,expected_ram};auto&er=m.c.r;auto setter=[&]{m.push(er[14]);er[14]=er[4];m.push(er[13]);m.t(er[14]==0);m.push(er[12]);er[12]=er[5];m.push(m.c.pr);er[13]=er[6];er[2]=m.w(0x8ced4444);need(er[2]==0x8ced1baa,"Validity target differs");er[4]=er[14];m.c.pr=0x8ced4390;
 er[3]=m.w(0x8ced1c0c);put(expected_ram,er[3],er[4]);er[0]=64;er[3]=m.w(er[4]+64);need(er[3]==1,"Instance invalid");er[0]=0;m.t(true);er[5]=er[12];er[6]=er[13];m.c.pr=0x8ced43ac;er[4]=er[14];
 m.push(er[14]);m.push(er[13]);m.push(m.c.pr);er[15]-=4;er[13]=er[5];er[5]=er[13];put(expected_ram,er[15],er[6]);er[14]=er[4];m.c.pr=0x8ced440c;
 m.push(m.c.pr);er[15]-=12;put(expected_ram,er[15],er[4]);put(expected_ram,er[15]+8,er[5]);put(expected_ram,er[15]+4,er[6]);er[0]=1;m.t(false);er[15]+=12;m.c.pr=m.pop();
 m.t(false);er[2]=static_cast<int16_t>(m.w(0x8ced443e));er[3]=er[13];er[1]=m.w(er[15]);er[3]<<=2;er[2]+=er[14];er[0]=er[13];er[3]+=er[2];m.t(er[0]==15);put(expected_ram,er[3],er[1]);er[15]+=4;m.c.pr=m.pop();er[13]=m.pop();er[14]=m.pop();need(m.c.pr==0x8ced43ac,"Mode-slot return differs");m.c.pr=m.pop();er[0]=0;er[12]=m.pop();er[13]=m.pop();er[14]=m.pop();m.c.pc=m.c.pr;m.cycles+=85;};
 for(unsigned reg:{14u,13u,12u})m.push(er[reg]);m.push(m.c.pr);er[15]-=8;er[13]=m.w(0x8ced06f4);er[5]=8;put(expected_ram,er[15]+4,er[6]);er[6]=0;put(expected_ram,er[15],er[7]);er[14]=m.w(er[4]+48);m.c.pr=0x8ced063a;er[4]=er[14];m.cycles=14;setter();
 auto set=[&](unsigned index,unsigned value,unsigned ret){er[5]=index;er[6]=value;er[4]=er[14];m.c.pr=ret;m.cycles+=5;setter();};set(1,1,0x8ced0642);set(0,0,0x8ced064a);set(23,4,0x8ced0652);
 er[2]=m.w(er[15]+4);er[0]=0x8ced06f8;er[3]=m.w(er[15]);er[1]=static_cast<int16_t>(m.w(0x8ced06ba));need(er[2]==2&&er[3]==2&&er[1]==1000&&m.w(er[0])==0x3f000000,"Captured FPU inputs differ");m.c.macl=4000;er[3]=4000;m.c.fpul=4000;m.c.fr_bits[2]=0x3f000000;m.c.fr_bits[3]=0x457a0000;m.c.fr_bits[4]=0x457a0800;er[4]=4000;m.t(false);er[12]=4000;m.cycles+=22;
 set(45,4000,0x8ced0688);set(44,4000,0x8ced0690);set(42,4000,0x8ced0698);set(14,1,0x8ced06a0);
 er[4]=er[14];er[5]=5;er[6]=0;er[15]+=8;m.c.pr=m.pop();er[2]=m.w(0x8ced06fc);er[12]=m.pop();er[13]=m.pop();er[14]=m.pop();m.c.pc=0x8ced06a0;m.cycles+=11;need(er[2]==0x8ced2ec0,"Output tail target differs");need(m.cycles==762,"Output prefix cycles differ");c=m.c;return m.cycles;}
static unsigned output_tail(auto&expected_ram,SH4Context&c){
BufferFlagsModel m{{c,expected_ram}};auto&er=m.c.r;m.push(er[14]);m.push(m.c.pr);er[15]-=8;er[14]=er[4];m.t(false);put(expected_ram,er[15],er[5]);put(expected_ram,er[15]+4,er[6]);er[3]=m.w(0x8ced2f38);need(er[3]==0x8ced1baa,"Validity target differs");m.c.pr=0x8ced2eda;er[4]=er[14];er[3]=m.w(0x8ced1c0c);put(expected_ram,er[3],er[4]);er[0]=64;er[3]=m.w(er[4]+64);need(er[3]==1,"Instance invalid");er[0]=0;m.t(true);
 er[4]=m.sw(0x8ced2f20);er[3]=m.sw(0x8ced2f1e);er[4]+=er[14];er[4]+=er[3];er[5]=er[4]+8;er[4]=m.w(er[5]);er[3]=m.w(0x8ced2f40);need(er[3]==0x8ced8604,"Flags helper differs");er[6]=m.w(er[15]+4);m.c.pr=0x8ced2efa;er[5]=m.w(er[15]);m.cycles=38;m.flags();er[4]=er[0];m.t(er[4]==0);er[0]=0;er[15]+=8;m.c.pr=m.pop();er[14]=m.pop();m.c.pc=m.c.pr;m.cycles+=9;need(m.cycles==84&&m.c.pc==0x8cecf1f6,"Output-tail return differs");c=m.c;return m.cycles;}
static unsigned resource_wrapper(auto&expected_ram,SH4Context&expected){const auto&ram=expected_ram;
auto&er=expected.r;er[3]=word(ram,0x8cecf2cc);need(er[3]==0x8ced0854,"Next wrapper differs");expected.pr=0x8cecf1fc;er[3]=word(ram,0x8ced08f4);er[6]=0;er[2]=word(ram,0x8ced08f8);er[1]=word(ram,0x8ced08fc);er[5]=word(ram,er[3]);er[4]=word(ram,er[2]);expected.pc=0x8ced0854;const unsigned cycles=12;need(er[1]==0x8c662106,"Allocator target differs");return cycles;}
static unsigned resource_allocation(auto&expected_ram,SH4Context&c){
HandlerPoolModel m{{c,expected_ram}};m.pool();need(m.slot==4&&m.cycles==92&&!m.full,"Next slot prediction differs");auto expected=m.c;auto&er=expected.r;er[5]=0;put(expected_ram,er[4]+12,er[5]);er[3]=word(expected_ram,er[4]+32);put(expected_ram,er[4]+16,er[3]);put(expected_ram,er[4]+20,er[5]);expected.pc=expected.pr;put(expected_ram,er[4]+24,er[5]);auto init_return=expected;need(expected.pc==0x8c662162,"Initializer return differs");er[0]=er[14];expected.pr=word(expected_ram,er[15]);er[15]+=4;expected.pc=expected.pr;er[14]=word(expected_ram,er[15]);er[15]+=4;need(expected.pc==0x8cecf1fc&&er[0]==0x8c8aa8f8,"Pool return differs");
  c=expected;return m.cycles+14;}
static unsigned outer_fields(auto&expected_ram,SH4Context&c){
RequestModel m{c,expected_ram};auto&er=m.c.r;need(er[0]==0x8c8aa8f8,"Allocated handle differs");er[2]=er[0];m.t(false);put(expected_ram,er[14]+56,er[0]);er[3]=m.w(0x8cecf2d0);er[0]=124;er[1]=er[14]+12;put(expected_ram,er[14],er[3]);er[2]=m.w(er[15]);er[3]=m.w(0x8cecf2d4);put(expected_ram,er[14]+124,er[2]);er[2]=er[12];m.c.pr=0x8cecf21e;er[0]=32;need(er[3]==0x8c10e958,"Original copy target differs");m.push(er[3]);er[3]=m.w(0x8c10e970);er[3]=m.w(er[3]+er[0]);need(er[3]==0x8c10e996,"32-byte copy dispatch differs");er[0]=m.w(er[2]+28);for(int off=24;off>=0;off-=8){er[3]=m.w(er[2]+off);put(expected_ram,er[1]+off+4,er[0]);if(off){er[0]=m.w(er[2]+off-4);put(expected_ram,er[1]+off,er[3]);}else put(expected_ram,er[1],er[3]);}er[3]=m.pop();
 er[0]=76;er[3]=static_cast<int16_t>(m.w(0x8cecf2a8));er[6]=er[14]+60;er[5]=7;put(expected_ram,er[14]+44,er[11]);er[2]=m.w(er[12]+28);put(expected_ram,er[14]+76,er[2]);for(unsigned off:{64u,72u,68u,100u}){er[0]=off;put(expected_ram,er[14]+off,er[13]);}er[0]=92;put(expected_ram,er[14]+92,er[3]);er[0]=96;er[2]=6;er[3]=m.w(0x8cecf2dc);put(expected_ram,er[14]+96,er[2]);er[0]=0x8cecf2d8;m.c.fr_bits[3]=m.w(er[0]);er[0]=112;put(expected_ram,er[14]+112,m.c.fr_bits[3]);put(expected_ram,er[14]+8,er[13]);m.c.pr=0x8cecf258;er[4]=er[10];m.c.pc=0x8cecf21e;need(er[3]==0x8ced45c8,"Next query target differs");c=m.c;return 71;}
int main(int argc,char**argv){try{
need(argc==5,"usage RAM STATE TIMERS OUTPUT");auto m=read(argv[1]);auto c=state(argv[2]);auto rt=std::make_unique<DCRuntime>();load_timers(*rt,argv[3]);std::string out=argv[4];need(c.pc==0x8cecf1e4,"Wrong output entry");unsigned cycles=0;
auto save=[&](const char*label){const std::string stem=out+"/"+label;save_state(c,stem+"-state.txt");save_ram(m,stem+"-ram.bin");auto timers=rt->tmu;tick(timers,rt->tmu_tstr,cycles);save_timers(timers,rt->tmu_tstr,rt->tmu_tocr,stem+"-timers.txt");};
cycles+=output_setup(m,c);save("settings");cycles+=output_tail(m,c);save("output-return");cycles+=resource_wrapper(m,c);save("wrapper");cycles+=resource_allocation(m,c);save("allocation");cycles+=outer_fields(m,c);save("end");need(cycles==1035,"Output/allocation cycles differ");
std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":1035,"guest_execution":false,"next_target":"8CED45C8"})"<<'\n';std::cout<<"PASS output846cycles plus wrapper/allocation/fields189cycles\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
