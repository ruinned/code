//
#include "IsoDetPrjConf.h"

const sIsoDet_PrjConfType IsoDet_PrjConf = 
{
    DET_EN,
    DOUBLE_END_DET_EN,
    DOUBLE_END_DET_FILTER_TIME,
    SUMV_PNTS,
    LEVEL_PNTS,
    DET_MIN_AD_VAL,
    DET_MIN_SUMV,
    ISO_DET_DELAY_TIMER
};

const uWord IsoDet_CnstSumvArray[SUMV_PNTS] =
{1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6500,7000,7500,8000};//unit 0.1v

const uByte IsoDet_CnstErrLevel[LEVEL_PNTS] = {1,2};
const uWord IsoDet_CnstThrehTable[LEVEL_PNTS][SUMV_PNTS] =
{
    {41,   30, 22,  17, 14, 12, 11, 10,  8,  7,  6,  5,  4,  4,  3}, // level1
    {201, 134, 101, 81, 72, 61, 54, 45, 36, 30, 25, 21, 18, 16, 14}  // level2

};
