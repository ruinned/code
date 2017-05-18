// HvM.h

#ifndef HVM_A_H
#define HVM_A_H

#include "GeneralTypeDef.h"
#include "HvM_Macro.h"


typedef union
{
    uWord wd;

    struct
    {
        uByte SumVolt       : 1;
        uByte BmsCanErr     : 1;
        uByte MinVolt       : 1;
        uByte IsoBat        : 1;

        uByte IsoNeg        : 1;
        uByte IsoPos        : 1;
        uByte NegPaste      : 1;
        uByte NegInvalid    : 1;

        uByte PrePaste      : 1;
        uByte PreInvalid    : 1;
        uByte PosPaste      : 1;
        uByte PosInvalid    : 1;

        uByte PreCircuitOff : 1;
        uByte LoadShort     : 1;
        uByte LoadOff       : 1;
        uByte ChgLink       : 1;
    } bit;

} uHvM_ErrCodeType;

typedef struct
{
    uHvM_ErrCodeType code;
}
sHvM_ErrType;

typedef struct
{
    uByte Cmd;
    uByte Life;
    uWord VcuSumV;
} sHvM_VcuInfoType;


//extern sHvM_VcuInfoType HvM_VcuInfo;
//extern sHvM_ErrType     HvM_Err;

extern void HvM_ModuleCbk(void);
extern void HvM_ModuleCountCbk(void);
extern void HvM_UpdateVcuInfo(sHvM_VcuInfoType *p_vcu);


#endif /* HVM_H */

