#ifndef EXTERNEEIF_H
#define EXTERNEEIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

// errorcode define
typedef enum
{
    EXTERNEEIF_ERR_OK,
    EXTERNEEIF_ERR_GETBYTE = ERR_BSW_NVM_BASE + 10U,
    EXTERNEEIF_ERR_SETBYTE,
    EXTERNEEIF_ERR_RDARRAY,
    EXTERNEEIF_ERR_WRARRAY,
}eExternEEIf_ErrCodeType;

extern void ExternEEIf_Init(void);
extern void ExternEEIf_WriteEnable(void);
extern void ExternEEIf_WriteDisable(void);
extern uWord ExternEEIf_SetByte(uWord addr, uByte data);
extern uWord ExternEEIf_SetWord(uWord addr, uWord data);
extern uWord ExternEEIf_SetLong(uWord addr, uLWord data);
extern uWord ExternEEIf_GetByte(uWord addr, uByte * p_data);
extern uWord ExternEEIf_WriteArray(uByte* p_data, uWord addr, uByte dlen);
extern uWord ExternEEIf_ReadArray(uByte* p_data, uWord addr, uByte dlen);


#endif /* EXTERNEEIF_H */
