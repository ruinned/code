// Xgate_StartIf.h

#ifndef XGATE_STARTIF_H
#define XGATE_STARTIF_H

#include "IO_Map.h"

/* define Semaphore operation in hc12*/
#define SET_SEM(x)  (XGSEM = 0x0101 << (x))   // lock semaphore x, x = 0~7
#define TST_SEM(x)  (XGSEM & (0x0001 << (x))) // Test semaphore x,x = 0~7
#define REL_SEM(x)  (XGSEM = 0x0100 << (x))   // unlock semaphor x,x = 0~7  

extern void Xgate_SetupAndStart(void);
#endif /* XGATE_STARTIF_H */