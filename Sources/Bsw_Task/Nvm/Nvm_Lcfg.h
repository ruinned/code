#ifndef NVMLCFG_H
#define NVMLCFG_H

#include "GeneralTypeDef.h"
#include "CPUEE.h"

typedef struct 
{
    void (*Init)(void);
    uByte (*SetByte)(CPUEE_TAddress addr, uByte data);
    uByte (*GetByte)(CPUEE_TAddress addr, uByte * p_data);
    uByte (*SetWord)(CPUEE_TAddress addr, uWord data);
    uByte (*SetLong)(CPUEE_TAddress addr, uLWord data);
}sCpuEE_OperationType;

typedef struct
{
    void (*SetInput)(void);
    void (*SetOutput)(void);
    void (*SetVal)(void);
    void (*ClrVal)(void);
    bool (*GetVal)(void);
}sNvm_IoOperationType;

typedef struct 
{
    sNvm_IoOperationType ExternEE_Sck;
    sNvm_IoOperationType ExternEE_Sda;
    sNvm_IoOperationType ExternEE_Wp;
}sExternEE_OperationType;

typedef struct 
{
    sNvm_IoOperationType ExternFlash_Sck;
    sNvm_IoOperationType ExternFlash_Si;
    sNvm_IoOperationType ExternFlash_Cs;
    sNvm_IoOperationType ExternFlash_So;
    sNvm_IoOperationType ExternFlash_Rst;
}sExternFlash_OperationType;

extern const sCpuEE_OperationType CpuEE_Operation;
extern const sExternEE_OperationType ExternEE_Operation;
extern const sExternFlash_OperationType ExternFlash_Operation;

#endif