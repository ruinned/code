#ifndef XGATE_TIME_SRV_H
#define XGATE_TIME_SRV_H

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "Xgate_ExtFlash.h" 
#include "Xgate_Config.h"

#ifdef XGATE_ENABLE_TIME
#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE  

extern interrupt void Xgate_PTI1_ISR(void);

#pragma pop

#endif // #ifdef XGATE_ENABLE_EXTFLASH
#endif // XGATE_EXTFLASH_SRV_H 