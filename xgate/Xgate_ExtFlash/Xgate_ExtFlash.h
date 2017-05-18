// Xgate_ExtFlash.h

#ifndef XGATE_EXTFLASH_H
#define XGATE_EXTFLASH_H

#include "GeneralTypeDef.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "Ecu.h"
#include "Xgate_Config.h"

#ifdef XGATE_ENABLE_EXTFLASH

#pragma push

#pragma DATA_SEG SHARED_DATA
typedef struct
{
    uLWord DataAddr;
    uWord DataLen; 
    uWord Page;
    uByte ChkOrWr;
    uByte CpltFlag;
    uByte Rslt;
    uByte Rsv;    
}Xgate_ExtFlashType;

extern uByte Share_ExtFlashDataBuff[264];
extern Xgate_ExtFlashType Shared_ExtFlashData;
#pragma pop

#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE 
#pragma CONST_SEG XGATE_CONST

#pragma NO_STRING_CONSTR 

/* Macro for xgate get shared data */    
#define XGATE_XGATEGET_EXTFLASH(i,a,b,c,d) \
{__asm(LOOP3 : SSEM #XGATE_EXTFLASH_SEMAPHORE);__asm(BCC LOOP3);}\
for(i = 0; i < 264; i++)\
{ \
    a[i] = Share_ExtFlashDataBuff[i];\
} \
b = Shared_ExtFlashData.DataLen;\
c = Shared_ExtFlashData.Page; \
d = Shared_ExtFlashData.ChkOrWr;\
{__asm(CSEM #XGATE_EXTFLASH_SEMAPHORE);}

/* Macro for xgate update shared data */
#define XGATE_XGATESET_EXTFLASH(v,w) \
{__asm(LOOP2 : SSEM #XGATE_EXTFLASH_SEMAPHORE);__asm(BCC LOOP2);}\
Shared_ExtFlashData.CpltFlag = v; \
Shared_ExtFlashData.Rslt = w; \
{__asm(CSEM #XGATE_EXTFLASH_SEMAPHORE);} 
      
        
#pragma pop

void Xgate_CpuSetExtFlash(Xgate_ExtFlashType ptr);
void Xgate_CpuGetExtFlah(Xgate_ExtFlashType *ptr);
void Xgate_CpuSetStart(void);

#endif  // XGATE_ENABLE_EXTFLASH
#endif  // XGATE_EXTFLASH_H