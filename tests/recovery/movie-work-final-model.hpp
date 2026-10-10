#pragma once
// Two ordered pushes precede BSR; its delay slot copies the argument to R14.
struct WorkFinalModel : RequestModel {
 void prefix(){c.pc=0x8ceda9a4;push(c.r[14]);push(c.pr);c.pr=0x8ceda9ac;c.r[14]=c.r[4];cycles+=5;}
};
