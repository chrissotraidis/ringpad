#pragma once
// Ordered stack write precedes the signed halfword literal load.
struct TableTailModel : RequestModel {
 void prefix(){c.pc=0x8cedc0ba;push(c.pr);c.r[3]=unsigned(int(std::int16_t(w(0x8cedc0d4)&0xffff)));c.r[4]+=c.r[3];c.pr=0x8cedc0c2;cycles+=5;}
};
