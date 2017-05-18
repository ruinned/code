// Xgate_RealTime.C

#include "GeneralTypeDef.h"
#include "Xgate_Config.h"
#include "Xgate_StartIf.h"
#include "Xgate_RealTime.h"

#ifdef XGATE_ENABLE_REALTIME

#pragma push
#pragma DATA_SEG SHARED_DATA /* allocate the following variables in the segment SHARED_DATA */
sRealTimeType Shared_RealTime;
uByte gby_Shared_RealTimeRdEn;
GeneralReturnType gwd_Shared_RealTimeRdRslt;

#pragma pop

void Xgate_CpuGetRealTime(sRealTimeType *time, uWord *rslt)
{
    do
    {
        SET_SEM(XGATE_REALTIME_SEMAPHORE);
    }
    while(!TST_SEM(XGATE_REALTIME_SEMAPHORE)); 
    time->second = Shared_RealTime.second;
    time->minute = Shared_RealTime.minute;
    time->hour = Shared_RealTime.hour;
    time->day = Shared_RealTime.day;
    time->month = Shared_RealTime.month;
    time->year = Shared_RealTime.year;
    *rslt = gwd_Shared_RealTimeRdRslt;
    REL_SEM(XGATE_REALTIME_SEMAPHORE);  
}

void Xgate_CpuSetRealTimeRdEn(uByte en)
{
    do
    {
        SET_SEM(XGATE_REALTIME_SEMAPHORE);
    }
    while(!TST_SEM(XGATE_REALTIME_SEMAPHORE));
    gby_Shared_RealTimeRdEn = en;
    REL_SEM(XGATE_REALTIME_SEMAPHORE);     
}


#endif

