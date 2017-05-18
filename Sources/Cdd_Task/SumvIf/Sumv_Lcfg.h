// Sumv_Lcfg.h
#ifndef SUMV_LCFG_H
#define SUMV_LCFG_H

#include "GeneralTypeDef.h"

typedef struct
{
    const void (*SetIoDirIn)(void);
    const void (*SetIoDirOut)(void);
    const void (*SetVal)(void);
    const void (*ClrVal)(void);
    const bool (*GetVal)(void);
} sSumv_IoOperationType;

typedef struct
{
    sSumv_IoOperationType Cpu_Sumv_Scl;
    sSumv_IoOperationType Cpu_Sumv_Sda;
    sSumv_IoOperationType Cpu_Sumv_Rdy;
    sSumv_IoOperationType Sumv_Pow;
} sSumv_OperationType;

extern const sSumv_OperationType Sumv_Operation[];

#define GET_SUMV_HANDLE(id) (&(Sumv_Operation[id]))

#endif