#ifndef XGATE_EXTFLASH_SRV_H
#define XGATE_EXTFLASH_SRV_H

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "Xgate_Config.h"
#include "GeneralTypeDef.h"

#ifdef XGATE_ENABLE_EXTFLASH

#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE  

extern interrupt void Xgate_ExtFlashEnableEvent(void);

#pragma pop

#endif // #ifdef XGATE_ENABLE_EXTFLASH
#endif // XGATE_EXTFLASH_SRV_H 