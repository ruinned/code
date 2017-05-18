// Xgate_ExtFlash.c 

#include "GeneralTypeDef.h"
#include "Xgate_Config.h"
#include "Xgate_StartIf.h"
#include "Xgate_ExtFlash.h"

#ifdef XGATE_ENABLE_EXTFLASH

#pragma push
#pragma DATA_SEG SHARED_DATA
unsigned char Share_ExtFlashDataBuff[264];
Xgate_ExtFlashType Shared_ExtFlashData;
#pragma pop

void Xgate_CpuSetExtFlash(Xgate_ExtFlashType ptr)
{
    uWord i;
    uByte * RAMPTR byte_ptr;
    do
    {
        SET_SEM(XGATE_EXTFLASH_SEMAPHORE);
    }
    while(!TST_SEM(XGATE_EXTFLASH_SEMAPHORE));
    Shared_ExtFlashData.DataAddr = ptr.DataAddr;
    byte_ptr = (uByte *RAMPTR)ptr.DataAddr; 
    Shared_ExtFlashData.DataLen = ptr.DataLen;
    for(i = 0; i < Shared_ExtFlashData.DataLen; i++)
    {
        Share_ExtFlashDataBuff[i] = *(byte_ptr++);
    }
    Shared_ExtFlashData.Page = ptr.Page;
    Shared_ExtFlashData.ChkOrWr= ptr.ChkOrWr;
    Shared_ExtFlashData.CpltFlag = ptr.CpltFlag; 
    Shared_ExtFlashData.Rslt = ptr.Rslt;
    
    REL_SEM(XGATE_EXTFLASH_SEMAPHORE);
}

void Xgate_CpuGetExtFlah(Xgate_ExtFlashType *ptr)
{
    do
    {
        SET_SEM(XGATE_EXTFLASH_SEMAPHORE);
    }
    while(!TST_SEM(XGATE_EXTFLASH_SEMAPHORE)); 
//    ptr->DataPtr = NULL;
//    ptr->Page = 0;
    ptr->ChkOrWr = Shared_ExtFlashData.ChkOrWr;
    ptr->CpltFlag = Shared_ExtFlashData.CpltFlag; 
    ptr->Rslt = Shared_ExtFlashData.Rslt;
    
    REL_SEM(XGATE_EXTFLASH_SEMAPHORE);    
}

void Xgate_CpuSetStart(void)
{
    // set soffware tirgger2
    XGSWT = 0x0404;
}

#endif // #ifdef XGATE_ENABLE_EXTFLASH