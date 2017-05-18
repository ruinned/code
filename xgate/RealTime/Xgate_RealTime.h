#ifndef XGATE_REALTIME_H
#define XGATE_REALTIME_H

/* MODULE ExternTimer_Scl. */

 /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "Xgate_Config.h"

#ifdef XGATE_ENABLE_REALTIME

#pragma push

#pragma DATA_SEG SHARED_DATA
typedef struct
{
    uByte year;
    uByte month;
    uByte day;
    uByte hour;
    uByte minute;
    uByte second;
}sRealTimeType;
extern sRealTimeType Shared_RealTime;
extern unsigned char gby_Shared_RealTimeRdEn;
extern unsigned int gwd_Shared_RealTimeRdRslt;
#pragma pop


#pragma push
#pragma DATA_SEG XGATE_DATA
#pragma CODE_SEG XGATE_CODE
#pragma CONST_SEG XGATE_CONST
#pragma NO_STRING_CONSTR 

/* Macro for xgate get shared data */    
#define XGATE_XGATESET_REALTIME(a,b,c,d,e,f,g) \
{__asm(LOOP4 : SSEM #XGATE_REALTIME_SEMAPHORE);__asm(BCC LOOP4);}\
Shared_RealTime.second = a;  \
Shared_RealTime.minute = b;  \
Shared_RealTime.hour = c;  \
Shared_RealTime.day = d; \
Shared_RealTime.month = e;  \
Shared_RealTime.year = f;  \
gwd_Shared_RealTimeRdRslt = g;\
{__asm(CSEM #XGATE_REALTIME_SEMAPHORE);}

/* Macro for xgate set shared data */
#define XGATE_XGATEGET_REALTIME(a)\
{__asm(LOOP5 : SSEM #XGATE_REALTIME_SEMAPHORE);__asm(BCC LOOP5);}\
a = gby_Shared_RealTimeRdEn; \
{__asm(CSEM #XGATE_REALTIME_SEMAPHORE);}
#pragma pop

extern void Xgate_CpuGetRealTime(sRealTimeType *time, uWord *rslt);
extern void Xgate_CpuSetRealTimeRdEn(uByte en);    

#endif /* #ifndef XGATE_REALTIME_H */

#endif // XGATE_ENABLE_REALTIME switch define