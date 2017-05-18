// Xgate_AcCpIoc.C

#include "GeneralTypeDef.h"
#include "Xgate_Config.h"
#include "IO_Map.h"
#include "Xgate_StartIf.h"

#ifdef XGATE_ENABLE_ACCP

#pragma push
#pragma DATA_SEG SHARED_DATA /* allocate the following variables in the segment SHARED_DATA */
uWord gwd_Shard_AcCpPeriod;
uByte gby_Shard_AcCpDuty;
#pragma pop

void Xgate_CpuGetAcCp(uWord *ptr_period,uByte *ptr_duty)
{
    do
    {
        SET_SEM(XGATE_ACCP_SEMAPHORE);
    }
    while(!TST_SEM(XGATE_ACCP_SEMAPHORE)); 
    *ptr_period = gwd_Shard_AcCpPeriod;
    *ptr_duty = gby_Shard_AcCpDuty;
    
    REL_SEM(XGATE_ACCP_SEMAPHORE);  
}

#endif 