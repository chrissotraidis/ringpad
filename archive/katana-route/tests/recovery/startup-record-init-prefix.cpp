// Verify original010E9E's provider-entry state, then delegate the real provider.
// The provider's resulting state is evidence to investigate, not an oracle.
#define main request_contract_standalone_main
#include "startup-record-request-return.cpp"
#undef main
#include <functional>
namespace dcrecomp_generated {void record_init_register(DCRuntime&);}
static RecompiledFunction actual_provider;
static std::function<void(bool)> observe_provider;
static void watched_provider(SH4Context& c,DCRuntime& rt) {
    observe_provider(false);
    actual_provider(c,rt);
    observe_provider(true);
}
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
int main(int argc,char** argv){try{
    need(argc==5,"usage: record-init-prefix RAM STATE FLASH OUTPUT");
    auto owner=std::make_unique<DCRuntime>();auto&rt=*owner;
    load_embedded_elf_image(rt);registers(rt);record_init_register(rt);
    const auto ram=read(argv[1]),flash=read(argv[3]);const auto initial=state(argv[2]);
    need(ram.size()==16777216&&flash.size()==131072,"Fixture sizes differ");
    rt.system_flash=flash;const auto sound=rt.aica_ram;const std::string out=argv[4];
    unsigned controls=0,busy_controls=0;
    for(unsigned index:{0u,1u,7u})for(unsigned value:{1u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
        rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;
        c.r[4]=index;c.r[5]=1;c.r[6]=value;c.r[15]=alias?0xacefe000:0x8cefe000;
        c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;
        auto expected=rt.main_ram;InitPrefix model{{c,expected}};model.skip_return();
        auto timers=rt.tmu;tick(timers,rt.tmu_tstr,model.cycles);
        need(model.cycles==22,"Skip prediction cycles");call_recompiled(c,rt,0x8c010e9e);
        same(model.c,c);need(expected==rt.main_ram,"Skip whole RAM differs");
        for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);++controls;
    }
    for(unsigned index:{0u,1u,7u})for(unsigned status:{1u,0xffffffffu})for(unsigned bank:{0u,1u})for(unsigned alias:{0u,1u}){
        rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;
        c.r[4]=index;c.r[5]=1;c.r[6]=0;c.r[15]=alias?0xacefe000:0x8cefe000;
        c.sr=(c.sr&~0x20000001u)|(bank?0x20000001:0);c.fpscr^=bank<<21;
        put(rt.main_ram,word(ram,0x8c64ba50)+index*128+4,status);
        auto expected=rt.main_ram;InitPrefix model{{c,expected}};model.busy_return();
        auto timers=rt.tmu;tick(timers,rt.tmu_tstr,model.cycles);
        need(model.cycles==58,"Busy prediction cycles");call_recompiled(c,rt,0x8c010e9e);
        same(model.c,c);need(expected==rt.main_ram,"Busy whole RAM differs");
        for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);++busy_controls;
    }
    rt.main_ram=ram;load_timers(rt,argv[2]);auto c=initial;auto expected=ram;
    InitPrefix model{{c,expected}};model.provider_entry();need(model.cycles==22,"Entry prediction cycles");
    auto timers=rt.tmu;const auto start=rt.tmu_tstr,tocr=rt.tmu_tocr;tick(timers,start,22);
    save_ram(expected,out+"/init-entry-expected-ram.bin");save_state(model.c,out+"/init-entry-expected-state.txt");
    save_timers(timers,start,tocr,out+"/init-entry-expected-timers.txt");
    actual_provider=rt.targets.at(0x8c606c9c);unsigned entered=0,returned=0;
    observe_provider=[&](bool after){
        if(!after){++entered;same(model.c,c);need(expected==rt.main_ram,"Provider entry RAM differs");
            for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
        }else{++returned;save_ram(rt.main_ram,out+"/provider-observed-ram.bin");save_state(c,out+"/provider-observed-state.txt");save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/provider-observed-timers.txt");}
    };
    rt.register_target(0x8c606c9c,watched_provider);rt.trace_calls=true;
    bool rejected=false;
    try {call_recompiled(c,rt,0x8c010e9e);}
    catch(const std::runtime_error& e){
        if(std::string(e.what()).find("No recompiled/native target registered for Dreamcast address 0x8C64C766")==std::string::npos)throw;
        rejected=true;
    }
    rt.register_target(0x8c606c9c,actual_provider);observe_provider={};
    need(entered==1&&returned==0&&rejected,"Expected real provider rejection at missing tail");
    need(!model.provider()&&model.cycles==48,"Actual provider-tail prediction differs");
    tick(timers,start,26);same(model.c,c);need(expected==rt.main_ram,"Provider tail whole RAM differs");
    for(unsigned i=0;i<3;++i)timer_same(timers[i],rt.tmu[i]);
    save_ram(expected,out+"/init-tail-expected-ram.bin");save_state(model.c,out+"/init-tail-expected-state.txt");
    save_timers(timers,start,tocr,out+"/init-tail-expected-timers.txt");
    save_ram(rt.main_ram,out+"/init-observed-ram.bin");save_state(c,out+"/init-observed-state.txt");
    save_timers(rt.tmu,rt.tmu_tstr,rt.tmu_tocr,out+"/init-observed-timers.txt");
    need(rt.system_flash==flash&&rt.aica_ram==sound&&!rt.device_clock_enabled&&rt.aica_arm.instructions==0&&!rt.sh4_async_redirect&&rt.holly_irq_taken==0,"Unexpected device effects");
    std::ofstream(out+"/result.json")<<"{\"passed\":true,\"skip_controls\":"<<controls<<",\"busy_controls\":"<<busy_controls<<",\"entry_cycles\":22,\"missing_tail_cycles\":48,\"entry_and_tail_RAM_CPU_TMU_verified\":true,\"missing_target\":\"0x8C64C766\",\"actual_parent_full_return_verified\":false,\"normal_boot_verified\":false,\"G1_resolved\":false}\n";
    std::cout<<"PASS original record init prefix: "<<controls<<" skip and "<<busy_controls<<" busy controls;22cycle provider entry and48cycle missing tail match RAM CPU TMU;active parent return unproved\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
