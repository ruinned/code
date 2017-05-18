// Xgate_AcCpIoc.h
// modify base on AC CP_IOC.h, jusd use for xgate

#ifndef __XGATE_AC_CP_IOC
#define __XGATE_AC_CP_IOC

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h" 
#include "Xgate_Config.h" 

#ifdef XGATE_ENABLE_ACCP

#pragma push
#pragma DATA_SEG SHARED_DATA
extern unsigned int gwd_Shard_AcCpPeriod;
extern unsigned char gby_Shard_AcCpDuty;
#pragma pop

extern void Xgate_CpuGetAcCp(unsigned int *ptr_period,unsigned char *ptr_duty);

#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE 
#pragma CONST_SEG XGATE_CONST

#pragma NO_STRING_CONSTR
//#define XGATE_SETACCP() asm NOP 

#define XGATE_XGATESETACCP(v,w) \
{__asm(LOOP1 : SSEM #XGATE_ACCP_SEMAPHORE); __asm(BCC LOOP1);}\
gwd_Shard_AcCpPeriod = v; \
gby_Shard_AcCpDuty = w; \
{__asm(CSEM #XGATE_ACCP_SEMAPHORE);}

        
#pragma pop

#endif /* ifndef __XGATE_AC_CP_IOC */

#endif // XGATE_ENABLE_ACCP switch define