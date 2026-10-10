#pragma once
#include "startup-hardware-capture-20261009.hpp"
static void capture_startup_pvr_irq(const DCRuntime& rt,const std::string& prefix){
 capture_startup_hardware(rt,prefix);
 std::ofstream pvr(prefix+"-pvr-irq.txt");require(bool(pvr),"PVR IRQ capture unavailable");
 pvr<<"pvr_ta_current_pointer="<<+rt.pvr_ta_current_pointer<<"\n";
 pvr<<"pvr_list_init_writes="<<+rt.pvr_list_init_writes<<"\n";
 pvr<<"pvr_list_cont_writes="<<+rt.pvr_list_cont_writes<<"\n";
 pvr<<"pvr_ta_guest_pointer_advances="<<+rt.pvr_ta_guest_pointer_advances<<"\n";
 pvr<<"pvr_ta_front_state="<<+rt.pvr_ta_front_state<<"\n";
 pvr<<"pvr_ta_front_list="<<+rt.pvr_ta_front_list<<"\n";
 pvr<<"dmac_sar2="<<+rt.dmac_sar2<<"\n";
 pvr<<"dmac_dar2="<<+rt.dmac_dar2<<"\n";
 pvr<<"dmac_dmatcr2="<<+rt.dmac_dmatcr2<<"\n";
 pvr<<"dmac_chcr2="<<+rt.dmac_chcr2<<"\n";
}
