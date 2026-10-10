// Diagnostic: distinguish stale code selection from stale literal data.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace dcrecomp_generated;
static void require(bool value,const char* why){if(!value)throw std::runtime_error(why);}
static auto make(){auto r=std::make_unique<DCRuntime>();load_embedded_elf_image(*r);register_recompiled_program(*r);return r;}
static unsigned run(DCRuntime& r,unsigned pc){SH4Context c{};c.pc=pc;c.pr=0xFFFFFFFF;c.r[15]=0x8CFFF000;call_recompiled(c,r,pc);require(c.pc==c.pr && c.r[15]==0x8CFFF000,"Return/stack mismatch");return c.r[0];}
int main(){
 try{
    auto code=make();require(run(*code,0x8C900000)==11 && run(*code,0x8C900040)==22,"Code controls differ");
    std::array<std::uint8_t,32> replacement{};
    for(unsigned n=0;n<32;++n)replacement[n]=dc_read8(*code,0x8C900040+n);
    dc_load_bytes(*code,0xAC900000,replacement.data(),replacement.size());
    require(dc_read16(*code,0x8C900000)==0xE016,"Replacement code absent");
    const bool stale_code=run(*code,0x8C900000)!=22;
    code->dispatch_cache={};
    const bool stale_without_cache=run(*code,0x0C900000)!=22;
    auto bulk=make();require(run(*bulk,0x8C900080)==7,"Literal control differs");
    const std::array<std::uint8_t,4> changed{19,0,0,0};
    dc_load_bytes(*bulk,0xAC900088,changed.data(),changed.size());
    const bool stale_bulk=run(*bulk,0x8C900080)!=19;
    auto zero=make();dc_zero_bytes(*zero,0x0C900088,4);
    const bool stale_zero=run(*zero,0x8C900080)!=0;
    auto scalar=make();dc_write32(*scalar,0xAC900088,23);
    require(run(*scalar,0x8C900080)==23,"Scalar invalidation failed");
    auto boundary=make();
    constexpr unsigned split=0x8C923FFE;
    const std::array<std::uint8_t,4> crossing{1,2,3,4};
    dc_load_bytes(*boundary,split|0x20000000,crossing.data(),crossing.size());
    require(dc_load_pc_literal16(*boundary,0x8C900080,split,0)==0x0201 &&
            dc_load_pc_literal16(*boundary,0x8C900080,split+2,0)==0x0403,"Bulk write missed a dirty page");
    // Start a separate untouched pair of pages with valid nonzero baked values.
    auto clearing=make();
    const unsigned off=(split&0x1FFFFFFF)-DCRuntime::kMainRamPhysicalBase;
    for(unsigned n=0;n<4;++n)clearing->main_ram[off+n]=crossing[n];
    dc_zero_bytes(*clearing,split&0x1FFFFFFF,4);
    require(dc_load_pc_literal16(*clearing,0x8C900080,split,0x0201)==0 &&
            dc_load_pc_literal16(*clearing,0x8C900080,split+2,0x0403)==0,"Clear missed a dirty page");
    const auto dirty_before=clearing->main_ram_literal_dirty;
    dc_load_bytes(*clearing,split,nullptr,0);dc_zero_bytes(*clearing,split,0);
    require(clearing->main_ram_literal_dirty==dirty_before,"Empty write dirtied memory");
    require(!stale_bulk && !stale_zero,"Bulk literal regression remains");
    std::cout<<"{\"stale_code_after_replacement\":"<<(stale_code?"true":"false")
             <<",\"stale_code_with_empty_cache\":"<<(stale_without_cache?"true":"false")
             <<",\"stale_bulk_literal\":"<<(stale_bulk?"true":"false")
             <<",\"stale_zero_literal\":"<<(stale_zero?"true":"false")
             <<",\"scalar_literal_correct\":true,\"cross_page_and_empty_controls\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
