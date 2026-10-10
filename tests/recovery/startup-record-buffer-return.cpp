// Source-derived original buffer expansion/copy and parent, no callee stubs.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
namespace dcrecomp_generated {void record_buffer_register(DCRuntime&);void bit_expand_register(DCRuntime&);void rectangle_copy_register(DCRuntime&);}
struct BufferModel : RequestModel {
 unsigned sx(unsigned v){return unsigned(int(std::int16_t(v)));}
 unsigned sw(unsigned a){std::uint16_t v;std::memcpy(&v,m.data()+(a&0xffffff),2);return sx(v);}
 void wh(unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(m.data()+(a&0xffffff),&x,2);}
 void predicate(){auto&r=c.r;r[3]=1538;r[2]=w(0x8c10ba04);r[0]=w(r[2]);r[3]+=r[4];r[3]=sb(r[3]);r[0]&=7;t(r[3]==r[0]);r[0]=c.sr&1;c.pc=c.pr;cycles+=13;}
 void expand(){
  auto&r=c.r;push(r[14]);r[14]=unsigned(-128);r[14]&=255;t(int(r[6])>0);r[1]=15;r[7]=r[14];r[0]=0;range(0xd80,0xd8e);
  if(c.sr&1)do{
   r[2]=sb(r[4]);r[2]&=255;t((r[7]&r[2])==0);range(0xd90,0xd96);
   if(!(c.sr&1)){wb(r[5],r[1]);range(0xd98,0xd9a);}else{wb(r[5],r[0]);++cycles;}
   t(r[7]&1);r[7]>>=1;t(r[7]==0);++r[5];range(0xd9e,0xda4);
   if(c.sr&1){r[7]=r[14];++r[4];cycles+=2;}
   --r[6];t(int(r[6])>0);range(0xdaa,0xdae);
  }while(c.sr&1);
  c.pc=c.pr;r[14]=pop();cycles+=3;
 }
 void rectangle(){
  auto&r=c.r;for(unsigned i=14;i>=8;--i)push(r[i]);r[15]-=8;r[13]=w(r[15]+36);r[0]=sw(r[13]+2);r[13]=r[0];r[0]=sw(r[6]+2);r[2]=r[0];r[9]=r[0];r[0]=sw(r[7]+2);r[3]=sx(r[9]);r[2]+=r[0];t(int(r[3])>=int(r[2]));r[8]=1;range(0xafa,0xb20);
  if(!(c.sr&1))do{
   r[0]=r[9];r[3]=r[9];t(r[0]>>31);r[0]<<=1;r[2]=r[13];r[0]+=r[3];r[3]=r[13];t(r[3]>>31);r[3]<<=1;r[14]=sw(r[6]);r[3]+=r[2];r[12]=w(r[15]+36);r[3]<<=2;r[3]<<=2;r[12]=sw(r[12]);r[0]<<=2;r[2]=sx(r[14]);r[0]<<=2;wh(r[15]+4,r[0]);wh(r[15],r[3]);r[3]=sw(r[6]);r[1]=sw(r[7]);r[3]+=r[1];t(int(r[2])>=int(r[3]));range(0xb22,0xb4e);
   if(!(c.sr&1))do{
    r[11]=sx(r[12]);t(int(r[11])>=0);range(0xb50,0xb54);bool inside=c.sr&1;
    if(inside){r[3]=48;t(int(r[11])>=int(r[3]));range(0xb56,0xb5a);inside=!(c.sr&1);}
    if(inside){r[1]=sx(r[13]);t(int(r[1])>=0);range(0xb5c,0xb60);inside=c.sr&1;}
    if(inside){r[2]=32;r[1]=sx(r[13]);t(int(r[1])>=int(r[2]));range(0xb62,0xb68);inside=!(c.sr&1);}
    if(inside){
     r[10]=sw(r[15]);r[3]=r[4];r[0]=sw(r[15]+4);r[10]+=r[12];r[2]=w(r[15]+40);r[11]=r[0];r[10]=sx(r[10]);r[11]+=r[14];t((r[8]&r[2])==0);r[11]=sx(r[11]);r[10]+=r[3];r[11]+=r[5];range(0xb6a,0xb82);
     if(!(c.sr&1)){r[1]=sb(r[11]);r[1]=~r[1];wb(r[10],r[1]);range(0xb84,0xb8a);}else{r[2]=sb(r[11]);wb(r[10],r[2]);cycles+=2;}
    }
    r[2]=sw(r[6]);++r[14];r[1]=sw(r[7]);r[3]=sx(r[14]);r[2]+=r[1];t(int(r[3])>=int(r[2]));++r[12];range(0xb90,0xb9e);
   }while(!(c.sr&1));
   r[0]=sw(r[6]+2);++r[9];r[2]=sx(r[9]);r[3]=r[0];r[0]=sw(r[7]+2);r[3]+=r[0];t(int(r[2])>=int(r[3]));++r[13];range(0xba0,0xbb0);
  }while(!(c.sr&1));
  r[15]+=8;for(unsigned i=8;i<14;++i)r[i]=pop();c.pc=c.pr;r[14]=pop();range(0xbb2,0xbc2,1);
 }
 void parent(){
  auto&r=c.r;push(r[14]);r[0]=unsigned(-6);push(r[13]);r[0]<<=8;push(c.pr);r[0]-=12;r[15]+=r[0];r[3]=w(0x8c109f20);c.pr=0x8c109dc8;r[14]=r[4];range(0xdb4,0xdc6,1);predicate();t(r[0]==0);cycles+=2;
  if(!(c.sr&1)){
   r[6]=6;r[5]=r[15];r[6]<<=8;r[0]=r[6];r[0]+=12;r[4]=sw(r[14]+r[0]);r[0]+=4;r[3]=w(r[14]+r[0]);r[5]+=12;r[4]&=65535;r[4]<<=2;t(r[4]>>31);r[4]<<=1;r[4]+=r[3];c.pr=0x8c109dea;r[4]=w(r[4]);range(0xdcc,0xde8,1);expand();
   r[13]=0;r[0]=r[13];wh(r[15]+10,r[0]);r[0]=r[13];wh(r[15]+8,r[0]);r[0]=48;wh(r[15]+4,r[0]);r[0]=32;wh(r[15]+6,r[0]);r[0]=r[13];wh(r[15]+2,r[0]);wh(r[15],r[13]);push(r[13]);r[3]=r[15]+4;push(r[3]);r[6]=r[15];r[5]=r[15];r[7]=r[15];r[5]+=20;r[7]+=12;r[6]+=16;c.pr=0x8c109e1a;r[4]=r[14];range(0xdea,0xe18,1);rectangle();r[15]+=8;++cycles;
  }
  r[1]=6;r[1]<<=8;r[1]+=12;r[15]+=r[1];c.pr=pop();r[13]=pop();c.pc=c.pr;r[14]=pop();range(0xe1c,0xe2a,1);
 }
};
int main(int argc,char**argv){try{
 need(argc==5,"usage: record-buffer RAM STATE FLASH OUTPUT");auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;load_embedded_elf_image(rt);registers(rt);record_buffer_register(rt);bit_expand_register(rt);rectangle_copy_register(rt);
 for(unsigned a:{0x8c109db4u,0x8c109d80u,0x8c109afau,0x8c10b906u})need(rt.targets.contains(a),"Missing original buffer binding");
 const auto ram=read(argv[1]),flash=read(argv[3]);need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes");rt.system_flash=flash;const auto sound=rt.aica_ram;const auto initial=state(argv[2]);const std::string stem=argv[4];
 auto check=[&](SH4Context c,unsigned target,bool capture){auto expected=rt.main_ram;BufferModel model{{c,expected}};if(target==0x8c109d80)model.expand();else if(target==0x8c109afa)model.rectangle();else model.parent();auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,model.cycles);
  if(capture){save_ram(expected,stem+"/buffer-expected-ram.bin");save_state(model.c,stem+"/buffer-expected-state.txt");save_timers(timers,start,tocr,stem+"/buffer-expected-timers.txt");std::ofstream(stem+"/prediction.json")<<"{\"cycles\":"<<model.cycles<<"}\n";}
  call_recompiled(c,rt,target);
  if(capture){save_state(c,stem+"/buffer-actual-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,stem+"/buffer-actual-timers.txt");}
  same(model.c,c);need(expected==rt.main_ram,"Buffer whole RAM differs");for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);need(start==rt.tmu_tstr&&tocr==rt.tmu_tocr&&rt.sh4_tick_pending_cycles==0&&rt.sh4_tick_batch_cycles==1,"Buffer timers differ");
  need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&rt.holly_irq_taken==0&&!rt.sh4_async_redirect&&rt.g2_dma_runs==0,"Unexpected device effect");return model.cycles;
 };
 unsigned expand_controls=0,rectangle_controls=0,parent_controls=0;
 auto reset=[&](unsigned n){rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;c.r[15]=0x8cefe000;c.pr=0x8c112234;c.sr=(c.sr&~0x20000001u)|((n&2)?0x20000000u:0)|(n&1);c.fpscr=(c.fpscr&~(1u<<21))|((n&4)?(1u<<21):0);return c;};
 auto wh=[&](unsigned a,unsigned v){std::uint16_t x=v;std::memcpy(rt.main_ram.data()+(a&0xffffff),&x,2);};
 for(unsigned count:{0u,0xffffffffu,1u,7u,8u,9u,1536u})for(unsigned pattern:{0u,255u,170u,85u})for(int overlap:{4096,0,1,-1}){
  auto c=reset(expand_controls);c.r[4]=0x8cc01000;c.r[5]=c.r[4]+overlap;c.r[6]=count;std::fill_n(rt.main_ram.begin()+0xc01000,192,pattern);check(c,0x8c109d80,false);++expand_controls;
 }
 const int destinations[][2]={{-1,-1},{0,0},{47,31},{48,0},{0,32}};
 for(unsigned width:{0u,1u,3u,48u})for(unsigned height:{0u,1u,2u,32u})for(const auto&xy:destinations)for(unsigned inverse:{0u,1u}){
  auto c=reset(rectangle_controls);c.r[4]=0x8cd00000;c.r[5]=0x8cc00000;c.r[6]=0x8ce00000;c.r[7]=0x8ce00004;wh(c.r[6],0);wh(c.r[6]+2,0);wh(c.r[7],width);wh(c.r[7]+2,height);wh(0x8ce00008,xy[0]);wh(0x8ce0000a,xy[1]);put(rt.main_ram,c.r[15],0x8ce00008);put(rt.main_ram,c.r[15]+4,inverse);
  for(unsigned i=0;i<1536;++i)rt.main_ram[0xc00000+i]=(i*37+13)&255;auto before=rt.main_ram;check(c,0x8c109afa,false);
  for(unsigned y=0;y<height;++y)for(unsigned x=0;x<width;++x){int dx=xy[0]+int(x),dy=xy[1]+int(y);if(dx>=0&&dx<48&&dy>=0&&dy<32){unsigned v=before[0xc00000+y*48+x]^(inverse?255:0);need(rt.main_ram[0xd00000+dy*48+dx]==v,"Independent clipping/inversion result differs");}}
  ++rectangle_controls;
 }
 for(unsigned enabled:{0u,1u})for(unsigned index:{0u,1u,2u})for(unsigned pattern:{0u,255u,170u,85u})for(unsigned bank:{0u,1u}){
  auto c=reset(parent_controls+bank);const auto record=c.r[4];put(rt.main_ram,word(ram,0x8c10ba04),3);rt.main_ram[(record+1538)&0xffffff]=enabled?3:255;wh(record+1548,index);put(rt.main_ram,record+1552,0x8cc80000);put(rt.main_ram,0x8cc80000+index*8,0x8cc90000);std::fill_n(rt.main_ram.begin()+0xc90000,192,pattern);
  const auto cycles=check(c,0x8c109db4,false);need(cycles==(enabled?(pattern==0?75254u:(pattern==255?76790u:76022u)):35u),"Parent path cycles differ");
  if(enabled)for(unsigned i=0;i<1536;++i)need(rt.main_ram[(record+i)&0xffffff]==((pattern&(128u>>(i%8)))?15:0),"Independent parent pixel expansion differs");++parent_controls;
 }
 rt.main_ram=ram;load_timers(rt,argv[2]);need((word(ram,word(ram,0x8c10ba04))&7)==2&&ram[(initial.r[4]+1538)&0xffffff]==0,"Captured selector differs");const auto cycles=check(initial,0x8c109db4,true);need(cycles==35,"Captured buffer skip timing differs");
 std::ofstream(stem+"/result.json")<<"{\"passed\":true,\"expansion_controls\":"<<expand_controls<<",\"rectangle_controls\":"<<rectangle_controls<<",\"parent_controls\":"<<parent_controls<<",\"cycles\":"<<cycles<<",\"full_RAM_CPU_TMU_return_verified\":true,\"captured_predicate_selected\":false,\"enabled_parent_verified\":true,\"device_clocks_enabled\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
 std::cout<<"PASS original record buffer return: "<<cycles<<" cycles whole16MiB RAM69CPU25TMU; "<<expand_controls<<" expansion, "<<rectangle_controls<<" rectangle, "<<parent_controls<<" parent controls; real enabled helpers; normal boot/G1 unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
