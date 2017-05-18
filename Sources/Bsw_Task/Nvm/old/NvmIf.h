#ifndef NVMIF_H
#define NVMIF_H

#include "GeneralTypeDef.h"

typedef struct
{
    void (* Init)(void);
    void (* WriteEnable)(void);
    void (* WriteDisable)(void);
    uWord (* SetByte)(uWord addr, uByte data);
    uWord (* SetWord)(uWord addr, uWord data);
    uWord (* SetLong)(uWord addr, uLWord data);
    uWord (* GetByte)(uWord addr, uByte * p_data);
    uWord (* WriteArray)(uByte* p_data, uWord addr, uByte dlen);
    uWord (* ReadArray)(uByte* p_data, uWord addr, uByte dlen);
} sNvmIf_OperationType;

typedef struct
{
    uByte (* Init)(void);
    uByte (* ReadStatusReg)(void);

    void (* WriteBuf)(uByte buf_no, uWord buf_addr, uByte * RAMPTR p_data, uWord dlen);
    void (* ReadBuf)(uWord buf_no, uByte *RAMPTR  p_data, uWord dlen);

    void (* WritePage)(uWord page, uByte * RAMPTR p_data, uWord dlen);
    void (* ReadPage)(uWord page, uByte * RAMPTR p_data, uWord dlen);
    uWord (* CheckPage)(uWord page, uByte * RAMPTR p_data, uWord dlen);

    void (* ChipErase)(void);
    void (* PageErase)(uWord page);
    void (* BlockErase)(uWord block);
} sNvmIf_FlashOperationType;

extern const sNvmIf_OperationType NvmIf_Operation[];
extern const sNvmIf_FlashOperationType NvmIf_FlashOperation;

#endif /* NVMIF_H */