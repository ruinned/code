#ifndef ISODETPRJCONF_H
#define ISODETPRJCONF_H
#include "GeneralTypeDef.h"

#define FLAG_DISABLE 0
#define FLAG_ENABLE 1

#define TM_S(V) (V * 100)

#define DET_EN                      FLAG_ENABLE
#define DOUBLE_END_DET_EN           FLAG_ENABLE
#define ISO_DET_DELAY_TIMER         20   //1 = 10ms
#define DOUBLE_END_DET_FILTER_TIME  TM_S(0)      // only one side delay time 

#define DET_MIN_SUMV                1000
#define DET_MIN_AD_VAL              30              

/* Iso threhold table */
#define SUMV_PNTS       15
#define LEVEL_PNTS      2

typedef struct
{
    uByte DetEn;
    uByte DoubleEndDetEn;
    uWord DoubleEndDetFilterTime;
         
    uByte SumVNum;
    uByte ErrLevelNum;
    uWord DetMinAdVal;
    uWord DetMinSumV;
    uWord DelayTime;
}sIsoDet_PrjConfType;

extern const sIsoDet_PrjConfType IsoDet_PrjConf;
extern const uByte IsoDet_CnstErrLevel[LEVEL_PNTS];
extern const uWord IsoDet_CnstThrehTable[LEVEL_PNTS][SUMV_PNTS];
extern const uWord IsoDet_CnstSumvArray[SUMV_PNTS];
#endif