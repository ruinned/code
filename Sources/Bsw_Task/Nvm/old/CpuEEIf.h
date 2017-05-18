#ifndef CPUEEIF_H
#define CPUEEIF_H

#include "GeneralTypeDef.h"


// errorcode define
typedef enum
{
    CPUEEIF_ERR_OK,
    CPUEEIF_ERR_GETBYTE = ERR_BSW_NVM_BASE,
    CPUEEIF_ERR_SETBYTE,
    CPUEEIF_ERR_RDARRAY,
    CPUEEIF_ERR_WRARRAY,
}eCpuEEIf_ErrCodeType;

extern void CpuEEIf_Init(void);
extern void CpuEEIf_WriteEnable(void);
extern void CpuEEIf_WriteDisable(void);
extern uWord CpuEEIf_SetByte(uWord relative_addr, uByte data);
extern uWord CpuEEIf_SetWord(uWord relative_addr, uWord data);
extern uWord CpuEEIf_SetLong(uWord relative_addr, uLWord data);
extern uWord CpuEEIf_GetByte(uWord relative_addr, uByte * p_data);
extern uWord CpuEEIf_ReadArray(uByte* p_data, uWord relative_addr, uByte dlen);
extern uWord CpuEEIf_WriteArray(uByte* p_data, uWord relative_addr, uByte dlen);

#endif /* CPUEEIF_H */