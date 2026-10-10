#pragma once
// BSR updates PR; NOP delay slot and the missing target change no other state.
struct WorkContinuationModel : RequestModel {void prefix(){c.pc=0x8ced8104;c.pr=0x8ced8108;cycles+=3;}};
