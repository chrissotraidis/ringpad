#pragma once
#include "dc_runtime.hpp"
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace dcrecomp_generated;
static void require(bool value, const char* why) { if (!value) throw std::runtime_error(why); }
static void check_registration(DCRuntime& rt, unsigned size, bool head, bool success, bool managed = false) {
    constexpr std::uint32_t heap = 0x8CF00000, task = 0x8CE00000, free_head = 0x8C7A32BC;
    for (unsigned n = 0; n < 288; ++n) dc_write8(rt, heap - 16 + n, 0xA5);
    for (unsigned n = 0; n < 96; ++n) dc_write8(rt, task - 16 + n, 0xA5);
    dc_write32(rt, free_head, head ? heap : 0);
    // Empty free-list links and physical-next/previous block links.
    dc_write32(rt, heap, 0); dc_write32(rt, heap + 4, 0);
    dc_write32(rt, heap + 8, heap + size); dc_write32(rt, heap + 12, 0);
    dc_write32(rt, heap + size, 0); dc_write32(rt, heap + size + 4, 0);
    dc_write32(rt, heap + size + 8, 0); dc_write32(rt, heap + size + 12, heap);
    std::array<std::uint8_t, 288> expected_heap{};
    std::array<std::uint8_t, 96> expected_task{};
    for (unsigned n=0;n<expected_heap.size();++n) expected_heap[n]=dc_read8(rt,heap-16+n);
    for (unsigned n=0;n<expected_task.size();++n) expected_task[n]=dc_read8(rt,task-16+n);
    auto put = [](auto& bytes, unsigned offset, std::uint32_t value) {
        for (unsigned n=0;n<4;++n) bytes.at(offset+n)=static_cast<std::uint8_t>(value>>(8*n));
    };
    if (success) {
        put(expected_task, 16+16, 0x0C903148); put(expected_task, 16+20, 0);
        put(expected_task, 16+24, 0); put(expected_task, 16+36, heap+16);
        expected_heap.at(16+16)=0; // Original init callback initializes the requested byte.
        if (size>32) {
            put(expected_heap,16+8,heap+32);
            put(expected_heap,16+32,0); put(expected_heap,16+36,0);
            put(expected_heap,16+40,heap+size); put(expected_heap,16+44,heap);
            put(expected_heap,16+size+12,heap+32);
        }
    } else put(expected_task,16+16,0x8C09A2FC); // Deferred cleanup callback, not executed here.
    std::array<std::uint8_t,24> metadata{};
    for(unsigned n=0;n<metadata.size();++n) metadata[n]=dc_read8(rt,0x8C900188+n);
    SH4Context ctx{};
    for(unsigned n=0;n<15;++n) ctx.r[n]=0x12340000+n;
    ctx.r[4]=task; ctx.r[15]=0x8CE3F000; ctx.pc=0x8C900040; ctx.pr=0xFFFFFFFF;
    const auto before=ctx;
    call_recompiled(ctx,rt,ctx.pc);
    require(ctx.r[0]==(success?task:0),"Wrong task return");
    require(ctx.pc==before.pr && ctx.pr==before.pr && ctx.r[15]==before.r[15],"Return/stack changed");
    for(unsigned n=8;n<15;++n) require(ctx.r[n]==before.r[n],"Preserved register changed");
    const auto expected_head=success?(size>32?heap+32:0):(head?heap:0);
    require(dc_read32(rt,free_head)==expected_head,"Wrong free-list head");
    for(unsigned n=0;n<expected_heap.size();++n)
        require(dc_read8(rt,heap-16+n)==expected_heap[n],"Heap/guard mutation mismatch");
    for(unsigned n=0;n<expected_task.size();++n)
        require(dc_read8(rt,task-16+n)==expected_task[n],"Task/guard mutation mismatch");
    for(unsigned n=0;n<metadata.size();++n)
        require(dc_read8(rt,0x8C900188+n)==metadata[n],"Original metadata changed");
    // Stored execution/cleanup callbacks are outside this five-routine slice.
    bool rejected=false;
    try { call_recompiled(ctx,rt,dc_read32(rt,task+16)); }
    catch(const std::runtime_error& e) {
        rejected=std::string(e.what()).find(managed && success ? "Inactive or unregistered code module target" : "No recompiled/native target registered")!=std::string::npos;
    }
    require(rejected,"Unimplemented continuation did not fail explicitly");
    std::cout<<"PASS size="<<size<<" head="<<head<<" success="<<success
             <<" heap/task/guards/metadata/preserved-state; continuation rejected\n";
}
