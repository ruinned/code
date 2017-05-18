// ErrorAlarm.c
#include "ErrorAlarm.h"
#include "ErrorMacro.h"
#include "BatterySt.h"
#include "BmsSysSt.h"
#include "SysConf.h"
#include "EE.h"
#include "IntCan.h"
#include "DataStore.h"
#include "ChargeSt.h"
#include "CarCan.h"



// ErrorAlarm Module use PAGED_RAM
#if(ECU_RAM_USE_MODE)
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM
#endif
#else
#pragma DATA_SEG DEFAULT
#endif

// timer for error alarm task
typedef struct
{
    uByte Cnt;
    uByte CntCheckErr;
} sErr_TmCntType;

sErr_TmCntType Err_TmCnt;

uByte gby_ReadErrThrhd = 0;

#define ERR_FLAG_OK    1U
#define ERR_FLAG_FAIL  0U

#define ERR_NUM_BAT    32U
#define ERR_NUM_BATSYS 64U
#define ERR_NUM_HW     32U
#define ERR_NUM_HW_DETAIL 32U

#define ERR_LEVEL_V1   0U
#define ERR_LEVEL_V2   1U
#define ERR_LEVEL_V3   2U
#define ERR_LEVEL_NUM  3U


typedef struct
{
    uWord Error;
    uWord Normal;
} sErr_HystrsType;

sErr_HystrsType Err_HystrsThrhd[ERR_LEVEL_NUM];


typedef struct
{
    sErr_HystrsType SumVHigh[ERR_LEVEL_NUM];
    sErr_HystrsType SumVLow[ERR_LEVEL_NUM];
    sErr_HystrsType THigh[ERR_LEVEL_NUM];
    sErr_HystrsType TLow[ERR_LEVEL_NUM];
    sErr_HystrsType DeltTAllPack[ERR_LEVEL_NUM];
    sErr_HystrsType DeltVSinglPack[ERR_LEVEL_NUM];
    sErr_HystrsType DeltVAllPack[ERR_LEVEL_NUM];
    sErr_HystrsType CellVHigh[ERR_LEVEL_NUM];
    sErr_HystrsType CellVLow[ERR_LEVEL_NUM];
    sErr_HystrsType CurHighChg[ERR_LEVEL_NUM];
    sErr_HystrsType CurHighFd[ERR_LEVEL_NUM];
    sErr_HystrsType CurHighDch[ERR_LEVEL_NUM];
    sErr_HystrsType DeltTSinglPack[ERR_LEVEL_NUM];
    sErr_HystrsType SocHigh[ERR_LEVEL_NUM];
    sErr_HystrsType SocLow[ERR_LEVEL_NUM];
    sErr_HystrsType IntResHighCell[ERR_LEVEL_NUM];
    sErr_HystrsType THighPole[ERR_LEVEL_NUM];//BatSysErr
    sErr_HystrsType SohLow[ERR_LEVEL_NUM];

    sErr_HystrsType SupplyVHigh[ERR_LEVEL_NUM];
    sErr_HystrsType SupplyVLow[ERR_LEVEL_NUM];
} sErr_HystrsCntType;

sErr_HystrsCntType Err_HystrsCnt;

typedef struct
{
    uWord SumVHigh;
    uWord SumVLow;
    uByte THigh;
    uByte TLow;
    uByte DeltTAllPack;
    uWord DeltVSinglPack;
    uWord DeltVAllPack;
    uWord CellVHigh;
    uWord CellVLow;
    uWord CurHighChg;
    uWord CurHighDch;
    uByte DeltTSinglPack;
    uWord SocHigh;
    uWord SocLow;
    uByte IntResHighCell;
    uByte THighPole;//BatSysErr
    uWord SohLow;
} sErr_ThrhdType;

sErr_ThrhdType Err_Thrhd[ERR_LEVEL_NUM];

typedef union
{
    sErr_ByteNType ByteN;
    struct
    {
        uByte SumVHigh        : 2;
        uByte SumVLow         : 2;
        uByte CellVHigh       : 2;
        uByte CellVLow        : 2;
        uByte DeltVSinglPack  : 2;
        uByte DeltVAllPack    : 2;
        uByte DeltTSinglPack  : 2;
        uByte DeltTAllPack    : 2;
        uByte THigh           : 2;
        uByte TLow            : 2;
        uByte CurHighChg      : 2;
        uByte CurHighDch      : 2;
        uByte CurHighFdbck    : 2;
        uByte RsvFor3b2       : 2;
        uByte SocHigh         : 2;
        uByte SocLow          : 2;
        uByte SocHighCell     : 2;
        uByte SocLowCell      : 2;
        uByte SocDiffCell     : 2;
        uByte AvrgVDiffAllPack: 2;
        uByte GroupNoDiff     : 2;
        uByte IntResHighCell  : 2;
        uByte IntResDiffCell  : 2;
        uByte IntResHigh      : 2;
        uByte SocDiffAllPack  : 2;
        uByte ManuDiff        : 2;
        uByte TypeDiff        : 2;
        uByte ShortCircuit    : 2;
        uByte SohLow          : 2;
        uByte SohLowCell      : 2;
        uByte SohDiffCell     : 2;
        uByte RsvFor7b6       : 2;
    } sBit;
} uErr_BatErrCodeType;

uErr_BatErrCodeType Err_BatErrCode;

uErr_BatErrCodeType Err_BatErrCodeBak;

typedef union
{
    sErr_ByteNType ByteN[2];
    struct
    {
        // page 0
        uByte PosAdjoin       : 2;
        uByte PosNotClosed    : 2;
        uByte PosOpenCur      : 2;
        uByte NegOpenCur      : 2;
        uByte NegAdjoin       : 2;
        uByte NegNotClosed    : 2;
        uByte PreAdjoin       : 2;
        uByte PreNotClosed    : 2;
        uByte DcDcAdjoin      : 2;
        uByte DcDcNotClosed   : 2;
        uByte HvilFail        : 2;
        uByte HvilAbnrml      : 2;
        uByte ChgAdjoin       : 2;
        uByte ChgNotClosed    : 2;
        uByte ChgOpenCur      : 2;
        uByte RsvFor3b6       : 2;
        uByte PreChg          : 2;
        uByte RsvFor4b2       : 2;
        uByte RsvFor4b4       : 2;
        uByte RsvFor4b6       : 2;
        uByte FanOff          : 2;
        uByte FanOn           : 2;
        uByte HeaterNotOn     : 2;
        uByte HeaterNotOff    : 2;
        uByte THighHeater     : 2;
        uByte THighInlet      : 2;
        uByte TLowInlet       : 2;
        uByte DeltTInOutlet   : 2;
        uByte THighOutlet     : 2;
        uByte TLowOutlet      : 2;
        uByte THighPole       : 2;
        uByte RsvFor7b6       : 2;
        // page 1
        uByte ShutDown        : 2;
        uByte SupplyVHigh     : 2;
        uByte SupplyVLow      : 2;
        uByte RsvFor8b6       : 2;
        uByte FanAdjoin       : 2;
        uByte FanNotClosed    : 2;
        uByte RsvFor9b4       : 2;
        uByte LiqudSensorAlrm : 2;
        uByte PressSensorAlrm : 2;
        uByte CllsnSensorAlrm : 2;
        uByte HumdtSensorAlrm : 2;
        uByte SmokeSensorAlrm : 2;
        uByte THighFuse       : 2;
        uByte FuseBroken      : 2;
        uByte RsvFor11b4      : 2;
        uByte RsvFor11b6      : 2;
        uByte IsoPos          : 2;
        uByte IsoNeg          : 2;
        uByte IsoMid          : 2;
        uByte IsoErr          : 2;
        uByte RsvFor13b0      : 2;
        uByte RsvFor13b2      : 2;
        uByte RsvFor13b4      : 2;
        uByte RsvFor13b6      : 2;
        uByte RsvFor14b0      : 2;
        uByte RsvFor14b2      : 2;
        uByte RsvFor14b4      : 2;
        uByte RsvFor14b6      : 2;
        uByte RsvFor15b0      : 2;
        uByte RsvFor15b2      : 2;
        uByte RsvFor15b4      : 2;
        uByte RsvFor15b6      : 2;
    } sBit;
} uErr_BatSysErrCodeType;

uErr_BatSysErrCodeType Err_BatSysErrCode;
uErr_BatSysErrCodeType Err_BatSysErrCodeBak;


typedef union
{
    sErr_ByteNType ByteN;
    struct
    {
        uByte DetectCellV     : 2;
        uByte DetectTemper    : 2;
        uByte DetectCur       : 2;
        uByte DetectIso       : 2;
        uByte DetectSumVIn    : 2;
        uByte DetectSumVOut   : 2;
        uByte ClockExt        : 2;
        uByte EepromCpu       : 2;
        uByte EepromExt       : 2;
        uByte FlashExt        : 2;
        uByte FlashCpu        : 2;
        uByte CpuGeneral      : 2;
        uByte Can4            : 2;
        uByte Can0            : 2;
        uByte Can1            : 2;
        uByte Can2            : 2;
        uByte Can3            : 2;
        uByte Ethernet        : 2;
        uByte Rs232           : 2;
        uByte Rs485           : 2;
        uByte BalChg          : 2;
        uByte BalDch          : 2;
        uByte DetectWh        : 2;
        uByte Software        : 2;
        uByte RsvFor6b0       : 2;
        uByte RsvFor6b2       : 2;
        uByte Ai              : 2;
        uByte Ao              : 2;
        uByte Di              : 2;
        uByte Do              : 2;
        uByte PreChg          : 2;
        uByte RelayDriver     : 2;
    } sBit;
} uErr_HwErrCodeType;


uErr_HwErrCodeType Err_HwErrCodeBmu[MAX_PACK_NUM + 1];

Err_HwErrDetailType Err_HwErrDetailBmu[MAX_PACK_NUM + 1][ERR_NUM_HW_DETAIL];


uErr_HwErrCodeType Err_HwErrCode;

uErr_HwErrCodeType Err_HwErrCodeBak;

Err_HwErrDetailType Err_HwErrDetail[ERR_NUM_HW_DETAIL];
Err_HwErrDetailType Err_HwErrDetailBak[ERR_NUM_HW_DETAIL];

uByte gby_BatErrJudgeStep;

// internal function declaration
static uByte Err_CheckErrChanged(void);
static void Err_JudgeHighThrhd(uWord Value, sErr_HystrsType * RAMPTR ErrHystrsThrhdPtr, sErr_HystrsType * RAMPTR ErrHystrsCntPtr,
                               uWord * MaxCntValue, uByte * ErrLevelPtr);
static void Err_JudgeLowThrhd(uWord Value, sErr_HystrsType * RAMPTR ErrHystrsThrhdPtr, sErr_HystrsType * RAMPTR ErrHystrsCntPtr,
                              uWord * MaxCntValue, uByte * ErrLevelPtr);
static GeneralReturnType Err_JudgeBatErrLevel(void);

static GeneralReturnType Err_JudgeBatSysErrLevel(void);
static void Err_DealHwErrLevel(void);

void Updata_DTCStatus(void);
// input function interface

GeneralReturnType Err_UpdateBatSysErrLevel(eErr_BatSysErrIndexType BatSysErrIndex, eErr_LevelType ErrLevel)
{
    GeneralReturnType err_code = ERR_OK;
    uByte page, byte_no, bit_no;

    if(BatSysErrIndex <= ERR_BATSYS_RSV_FOR_7_6)
    {
        page = 0;
    }
    else if(BatSysErrIndex <= ERR_BATSYS_RSV_FOR_15_6)
    {
        page = 1;
        BatSysErrIndex -= ERR_BATSYS_SHUT_DOWN;
    }
    else
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    if(ErrLevel > ERR_LEVEL_THREE)
    {
        err_code = (GeneralReturnType)ERR_CODE_LEVEL_OVER;
        return err_code;
    }

    byte_no = (uByte)((uWord)BatSysErrIndex / 4);
    bit_no = (uByte)((uWord)BatSysErrIndex % 4);

    Err_BatSysErrCode.ByteN[page].By[byte_no] &= ~((uByte)(0x03 << (bit_no * 2)));
    Err_BatSysErrCode.ByteN[page].By[byte_no] |= ((uByte)ErrLevel << (bit_no * 2));

    return err_code;
}

// update BMU data and save to data[BmuNo]
GeneralReturnType Err_UpdateHwErrLevelFromCan(uByte BmuNo, sErr_ByteNType * HwErrByteNPtr)
{
    GeneralReturnType err_code = ERR_OK;
    uByte i;

    if(0 == BmuNo)
    {
        err_code = (GeneralReturnType)ERR_CODE_BMU_NO_ZERO;
        return err_code;
    }
    if(BmuNo > MAX_BMU_NUM)
    {
        err_code = (GeneralReturnType)ERR_CODE_BMU_NO_OVER;
        return err_code;
    }

    for(i=0; i<8; i++)
    {
        Err_HwErrCodeBmu[BmuNo].ByteN.By[i] = HwErrByteNPtr->By[i];
    }

    return err_code;
}

// update BMU data and save to data[BmuNo]
GeneralReturnType Err_UpdateHwErrDetailFromCan(uByte BmuNo, eErr_HwErrIndexType HwErrIndex, Err_HwErrDetailType HwErrDetail)
{
    GeneralReturnType err_code = ERR_OK;

    if(0 == BmuNo)
    {
        err_code = (GeneralReturnType)ERR_CODE_BMU_NO_ZERO;
        return err_code;
    }
    if(BmuNo > MAX_BMU_NUM)
    {
        err_code = (GeneralReturnType)ERR_CODE_BMU_NO_OVER;
        return err_code;
    }

    if(HwErrIndex > ERR_HW_RELAY_DRIVER)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    Err_HwErrDetailBmu[BmuNo][HwErrIndex] = HwErrDetail;

    return err_code;
}


// update BCU_C/BCU_D data and save to data[0]
// or update BMU/BMU_R data and save to data
GeneralReturnType Err_UpdateHwErrLevel(eErr_HwErrIndexType HwErrIndex, eErr_LevelType ErrLevel)
{
    GeneralReturnType err_code = ERR_OK;
    uByte byte_no, bit_no;

    if(HwErrIndex > ERR_HW_RELAY_DRIVER)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    if(ErrLevel > ERR_LEVEL_THREE)
    {
        err_code = (GeneralReturnType)ERR_CODE_LEVEL_OVER;
        return err_code;
    }

    byte_no = (uByte)((uWord)HwErrIndex / 4);
    bit_no = (uByte)((uWord)HwErrIndex % 4);

    Err_HwErrCodeBmu[0].ByteN.By[byte_no] &= ~((uByte)(0x03 << (bit_no * 2)));
    Err_HwErrCodeBmu[0].ByteN.By[byte_no] |= ((uByte)ErrLevel << (bit_no * 2));

    return err_code;
}

// update BCU_C/BCU_D data and save to data[0]
// or update BMU/BMU_R data and save to data
GeneralReturnType Err_UpdateHwErrDetail(eErr_HwErrIndexType HwErrIndex, Err_HwErrDetailType HwErrDetail)
{
    GeneralReturnType err_code = ERR_OK;

    if(HwErrIndex > ERR_HW_RELAY_DRIVER)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    Err_HwErrDetailBmu[0][HwErrIndex] = HwErrDetail;

    return err_code;
}
// end of input function interface




// output function interface
eErr_LevelType Err_GetBatErrLevel(eErr_BatErrIndexType BatErrIndex)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD;
    uByte byte_no, bit_no;

    if(BatErrIndex > ERR_BAT_RSV_FOR_7_6)
    {
        return err_level;
    }

    byte_no = (uByte)((uWord)BatErrIndex / 4);
    bit_no = (uByte)((uWord)BatErrIndex % 4);

    if(byte_no < 8)
    {
        err_level = (eErr_LevelType)((Err_BatErrCode.ByteN.By[byte_no] >> (bit_no * 2)) & 0x03);
    }
    return err_level;
}

sErr_ByteNType Err_GetBatAllErrLevel(void)
{
    sErr_ByteNType err_byte_n;
    uByte i;

    for(i=0; i<8; i++)
    {
        err_byte_n.By[i] = Err_BatErrCode.ByteN.By[i];
    }

    return err_byte_n;
}


eErr_LevelType Err_GetBatSysErrLevel(eErr_BatSysErrIndexType BatSysErrIndex)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD;
    uByte page, byte_no, bit_no;

    if(BatSysErrIndex <= ERR_BATSYS_RSV_FOR_7_6)
    {
        page = 0;
    }
    else if(BatSysErrIndex <= ERR_BATSYS_RSV_FOR_15_6)
    {
        page = 1;
        BatSysErrIndex -= ERR_BATSYS_SHUT_DOWN;
    }
    else
    {
        return err_level;
    }

    byte_no = (uByte)((uWord)BatSysErrIndex / 4);
    bit_no = (uByte)((uWord)BatSysErrIndex % 4);

    err_level = (eErr_LevelType)((Err_BatSysErrCode.ByteN[page].By[byte_no] >> (bit_no * 2)) & 0x03);

    return err_level;
}

sErr_ByteNType Err_GetBatSysAllErrLevel(uByte Page)
{
    sErr_ByteNType err_byte_n;
    uByte i;

    if(Page > 1)
    {
        for(i=0; i<8; i++)
        {
            err_byte_n.By[i] = (uByte)ERR_LEVEL_INVALD;
        }
        return err_byte_n;
    }

    for(i=0; i<8; i++)
    {
        err_byte_n.By[i] = Err_BatSysErrCode.ByteN[Page].By[i];
    }

    return err_byte_n;
}


eErr_LevelType Err_GetHwErrLevel(eErr_HwErrIndexType HwErrIndex)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD;
    uByte byte_no, bit_no;

    if(HwErrIndex > ERR_HW_RELAY_DRIVER)
    {
        return err_level;
    }

    byte_no = (uByte)((uWord)HwErrIndex / 4);
    bit_no = (uByte)((uWord)HwErrIndex % 4);

    err_level = (eErr_LevelType)((Err_HwErrCode.ByteN.By[byte_no] >> (bit_no * 2)) & 0x03);

    return err_level;
}

sErr_ByteNType Err_GetHwAllErrLevel(void)
{
    sErr_ByteNType err_byte_n;
    uByte i;

    for(i=0; i<8; i++)
    {
        err_byte_n.By[i] = Err_HwErrCode.ByteN.By[i];
    }

    return err_byte_n;
}

Err_HwErrDetailType Err_GetHwErrDetail(eErr_HwErrIndexType HwErrIndex)
{
    if(HwErrIndex > ERR_HW_RELAY_DRIVER)
    {
        return ERR_HW_DETAIL_INVALID;
    }
    return Err_HwErrDetail[HwErrIndex];
}

eErr_LevelType Err_GetErrLevel(eErr_TypeType ErrType)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD, err_level_tmp;
    uByte i;

    if(ErrType > ERR_ALL)
    {
        return err_level;
    }

    switch(ErrType)
    {
    case ERR_BAT:
    {
        err_level = ERR_LEVEL_NORMAL;
        for(i=0; i<ERR_NUM_BAT; i++)
        {
            err_level_tmp = Err_GetBatErrLevel((eErr_BatErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        break;
    }

    case ERR_BATSYS:
    {
        err_level = ERR_LEVEL_NORMAL;
        for(i=0; i<ERR_NUM_BATSYS; i++)
        {
            err_level_tmp = Err_GetBatSysErrLevel((eErr_BatSysErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        break;
    }

    case ERR_HW:
    {
        err_level = ERR_LEVEL_NORMAL;
        for(i=0; i<ERR_NUM_HW; i++)
        {
            err_level_tmp = Err_GetHwErrLevel((eErr_HwErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        break;
    }
    case ERR_ALL:
    {
        err_level = ERR_LEVEL_NORMAL;
        for(i=0; i<ERR_NUM_BAT; i++)
        {
            err_level_tmp = Err_GetBatErrLevel((eErr_BatErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }

        for(i=0; i<ERR_NUM_BATSYS; i++)
        {
            err_level_tmp = Err_GetBatSysErrLevel((eErr_BatSysErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        for(i=0; i<ERR_NUM_HW; i++)
        {
            err_level_tmp = Err_GetHwErrLevel((eErr_HwErrIndexType)i);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        break;
    }
    default:
        break;
    }

    return err_level;
}
// end of output function interface




// callback function interface
void Err_Init(void)
{
    uByte * RAMPTR byte_ptr;
    uWord i;
    GeneralReturnType err_code = ERR_OK;

    byte_ptr = (uByte * RAMPTR)Err_Thrhd;
    for(i=0; i<sizeof(Err_Thrhd); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)Err_HystrsThrhd;
    for(i=0; i<sizeof(Err_HystrsThrhd); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_HystrsCnt);
    for(i=0; i<sizeof(Err_HystrsCnt); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_BatErrCode);
    for(i=0; i<sizeof(Err_BatErrCode); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_BatErrCodeBak);
    for(i=0; i<sizeof(Err_BatErrCodeBak); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_BatSysErrCode);
    for(i=0; i<sizeof(Err_BatSysErrCode); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_BatSysErrCodeBak);
    for(i=0; i<sizeof(Err_BatSysErrCodeBak); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)Err_HwErrCodeBmu;
    for(i=0; i<sizeof(Err_HwErrCodeBmu); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)Err_HwErrDetailBmu;
    for(i=0; i<sizeof(Err_HwErrDetailBmu); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_HwErrCode);
    for(i=0; i<sizeof(Err_HwErrCode); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)(&Err_HwErrCodeBak);
    for(i=0; i<sizeof(Err_HwErrCodeBak); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)Err_HwErrDetail;
    for(i=0; i<sizeof(Err_HwErrDetail); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte * RAMPTR)Err_HwErrDetailBak;
    for(i=0; i<sizeof(Err_HwErrDetailBak); i++)
    {
        *(byte_ptr++) = 0;
    }

    err_code = SysConf_InitParaTable(ERRORALARM_THRHD_LVV1, (uLWord)(&Err_Thrhd[ERR_LEVEL_V1]), sizeof(sErr_ThrhdType));
    err_code = SysConf_InitParaTable(ERRORALARM_THRHD_LVV2, (uLWord)(&Err_Thrhd[ERR_LEVEL_V2]), sizeof(sErr_ThrhdType));
    err_code = SysConf_InitParaTable(ERRORALARM_THRHD_LVV3, (uLWord)(&Err_Thrhd[ERR_LEVEL_V3]), sizeof(sErr_ThrhdType));

    err_code = EE_LoadVar(ERRORALARM_THRHD_LVV1);
    err_code |= EE_LoadVar(ERRORALARM_THRHD_LVV2);
    err_code |= EE_LoadVar(ERRORALARM_THRHD_LVV3);

    if(ERR_OK != err_code)
    {
        err_code = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        err_code = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        err_code = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        err_code = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }

    Err_TmCnt.Cnt = 0;
    Err_TmCnt.CntCheckErr = 0;


    (void)err_code;
}

void Err_Count(void)
{
    (Err_TmCnt.Cnt < 255) ? (Err_TmCnt.Cnt++) : (Err_TmCnt.Cnt = 255);

    (Err_TmCnt.CntCheckErr < 255) ? (Err_TmCnt.CntCheckErr++) : (Err_TmCnt.CntCheckErr = 255);
}

void Err_Main(void)
{
    GeneralReturnType err_code = ERR_OK;

    // how to process returned error code
    if(Err_TmCnt.Cnt >= TM_10MS)
    {
        Err_TmCnt.Cnt = 0;
        err_code = Err_JudgeBatErrLevel();

        err_code = Err_JudgeBatSysErrLevel();
        Err_DealHwErrLevel();

    }

    if(Err_TmCnt.CntCheckErr >= TM_10MS)
    {
        Err_TmCnt.CntCheckErr = 0;
        if(ERR_FLAG_OK == Err_CheckErrChanged())
        {
            //save error data to Flash
            DataStore_EnableErrorDataStore(ERROR_DATA);
            Updata_DTCStatus();
        }
    }

}

uWord Err_GetAlarmThrhdVal(uByte Err_type, uByte Err_level)
{
    uWord wtmp;
    switch(Err_type)
    {
    case ERR_BAT_SUMV_HIGH:
    {
        wtmp = Err_Thrhd[Err_level].SumVHigh;
        break;
    }
    case ERR_BAT_SUMV_LOW:
    {
        wtmp = Err_Thrhd[Err_level].SumVLow;
        break;
    }
    case ERR_BAT_DELTT_ALLPACK:
    {
        wtmp = Err_Thrhd[Err_level].DeltTAllPack;
        break;
    }
    case ERR_BAT_DELTV_SINGLPACK:
    {
        wtmp = Err_Thrhd[Err_level].DeltVSinglPack;
        break;
    }
    case ERR_BAT_DELTV_ALLPACK:
    {
        wtmp = Err_Thrhd[Err_level].DeltVAllPack;
        break;
    }
    case ERR_BAT_DELTT_SINGLPACK:
    {
        wtmp = Err_Thrhd[Err_level].DeltTSinglPack;
        break;
    }
    case ERR_BAT_SOC_HIGH:
    {
        wtmp = Err_Thrhd[Err_level].SocHigh;
        break;
    }
    case ERR_BAT_INTRES_HIGH_CELL:
    {
        wtmp = Err_Thrhd[Err_level].IntResHighCell;
        break;
    }
    case ERR_BAT_POLE_T_HIGH:
    {
        wtmp = Err_Thrhd[Err_level].THighPole;
        break;
    }
    case ERR_BAT_SOH_LOW:
    {
        wtmp = Err_Thrhd[Err_level].SohLow;
        break;
    }

    case ERR_BAT_T_HIGH:
    {
        wtmp = Err_Thrhd[Err_level].THigh;
        break;
    }
    case ERR_BAT_T_LOW:
    {
        wtmp = Err_Thrhd[Err_level].TLow;
        break;
    }
    case ERR_BAT_CELLV_HIGH:
    {
        wtmp = Err_Thrhd[Err_level].CellVHigh;
        break;
    }
    case ERR_BAT_CELLV_LOW:
    {
        wtmp = Err_Thrhd[Err_level].CellVLow;
        break;
    }
    case ERR_BAT_CUR_HIGH_CHG:
    {
        wtmp = Err_Thrhd[Err_level].CurHighChg;
        break;
    }
    case ERR_BAT_CUR_HIGH_DCH:
    {
        wtmp = Err_Thrhd[Err_level].CurHighDch;
        break;
    }
    case ERR_BAT_SOC_LOW:
    {
        wtmp = Err_Thrhd[Err_level].SocLow;
        break;
    }
    default:
        wtmp = 0;
        break;
    }
    return wtmp;

}
// end of callback function interface

// internal function interface

uByte Err_CheckErrChanged(void)
{
    uByte i, flag = ERR_FLAG_FAIL;

    for(i=0; i<8; i++)
    {
        if(Err_BatErrCodeBak.ByteN.By[i] != Err_BatErrCode.ByteN.By[i])
        {
            Err_BatErrCodeBak.ByteN.By[i] = Err_BatErrCode.ByteN.By[i];
            flag = ERR_FLAG_OK;
        }

        if(Err_BatSysErrCodeBak.ByteN[0].By[i] != Err_BatSysErrCode.ByteN[0].By[i])
        {
            Err_BatSysErrCodeBak.ByteN[0].By[i] = Err_BatSysErrCode.ByteN[0].By[i];
            flag = ERR_FLAG_OK;
        }
        if(Err_BatSysErrCodeBak.ByteN[1].By[i] != Err_BatSysErrCode.ByteN[1].By[i])
        {
            Err_BatSysErrCodeBak.ByteN[1].By[i] = Err_BatSysErrCode.ByteN[1].By[i];
            flag = ERR_FLAG_OK;
        }

        if(Err_HwErrCodeBak.ByteN.By[i] != Err_HwErrCode.ByteN.By[i])
        {
            Err_HwErrCodeBak.ByteN.By[i] = Err_HwErrCode.ByteN.By[i];
            flag = ERR_FLAG_OK;
        }
    }

    // check harware error detail infos here or not
    /*for(i=0; i<32; i++)
    {
        if(Err_HwErrDetailBak[i] != Err_HwErrDetail[i])
        {
            Err_HwErrDetailBak[i] = Err_HwErrDetail[i];
         //   flag = ERR_FLAG_OK;
        }
    }*/
    // don't care harware error detail for data save

    return flag;
}


void Err_JudgeHighThrhd(uWord Value, sErr_HystrsType * RAMPTR ErrHystrsThrhdPtr, sErr_HystrsType * RAMPTR ErrHystrsCntPtr,
                        uWord * MaxCntValue, uByte * ErrLevelPtr)
{
    // error set
    //ERR_LEVEL_THREE
    if(Value > (ErrHystrsThrhdPtr +2)->Error)
    {
        if((ErrHystrsCntPtr +2)->Error < *(MaxCntValue +4))
        {
            (ErrHystrsCntPtr +2)->Error++;
        }
        else
        {
            (ErrHystrsCntPtr +2)->Error = *(MaxCntValue +4);
            if(*ErrLevelPtr <= 3)
            {
                *ErrLevelPtr = 3;
            }
        }
    }
    else
    {
        (ErrHystrsCntPtr +2)->Error = 0;
    }
    //ERR_LEVEL_TWO
    if(Value > (ErrHystrsThrhdPtr +1)->Error)
    {
        if((ErrHystrsCntPtr +1)->Error < *(MaxCntValue +2))
        {
            (ErrHystrsCntPtr +1)->Error++;
        }
        else
        {
            (ErrHystrsCntPtr +1)->Error = *(MaxCntValue +2);
            if(*ErrLevelPtr <= 2)
            {
                *ErrLevelPtr = 2;
            }
        }
    }
    else
    {
        (ErrHystrsCntPtr +1)->Error = 0;
    }
    //ERR_LEVEL_ONE
    if(Value > ErrHystrsThrhdPtr->Error)
    {
        if(ErrHystrsCntPtr->Error < *MaxCntValue)
        {
            ErrHystrsCntPtr->Error++;
        }
        else
        {
            ErrHystrsCntPtr->Error = *MaxCntValue;
            if(*ErrLevelPtr <= 1)
            {
                *ErrLevelPtr = 1;
            }
        }
    }
    else
    {
        ErrHystrsCntPtr->Error = 0;
    }
    // error clear
    if(*ErrLevelPtr >= 1)
    {
        //ERR_LEVEL_NORMAL
        if((Value < ErrHystrsThrhdPtr->Normal) && (*ErrLevelPtr == 1))
        {
            if(ErrHystrsCntPtr->Normal < *(MaxCntValue +1))
            {
                ErrHystrsCntPtr->Normal++;
            }
            else
            {
                ErrHystrsCntPtr->Normal = *(MaxCntValue +1);
                *ErrLevelPtr = 0;
            }
        }
        else
        {
            ErrHystrsCntPtr->Normal = 0;
        }
        //ERR_LEVEL_ONE
        if((Value < (ErrHystrsThrhdPtr +1)->Normal) && (*ErrLevelPtr == 2))
        {
            if((ErrHystrsCntPtr +1)->Normal < *(MaxCntValue +3))
            {
                (ErrHystrsCntPtr +1)->Normal++;
            }
            else
            {
                (ErrHystrsCntPtr +1)->Normal = *(MaxCntValue +3);
                if(Value > ErrHystrsThrhdPtr->Error)
                {
                    *ErrLevelPtr = 1;
                }
                else
                {
                    *ErrLevelPtr = 0;
                }
            }
        }
        else
        {
            (ErrHystrsCntPtr +1)->Normal = 0;
        }
        //ERR_LEVEL_TWO
        if((Value < (ErrHystrsThrhdPtr +2)->Normal) && (*ErrLevelPtr == 3))
        {
            if((ErrHystrsCntPtr +2)->Normal < *(MaxCntValue +5))
            {
                (ErrHystrsCntPtr +2)->Normal++;
            }
            else
            {
                (ErrHystrsCntPtr +2)->Normal = *(MaxCntValue +5);
                if(Value > (ErrHystrsThrhdPtr +1)->Error)
                {
                    *ErrLevelPtr = 2;
                }
                else if(Value > ErrHystrsThrhdPtr->Error)
                {
                    *ErrLevelPtr = 1;
                }
                else
                {
                    *ErrLevelPtr = 0;
                }
            }
        }
        else
        {
            (ErrHystrsCntPtr +2)->Normal = 0;
        }
    }
    else
    {
        ErrHystrsCntPtr->Normal = 0;
        (ErrHystrsCntPtr + 1)->Normal = 0;
        (ErrHystrsCntPtr + 2)->Normal = 0;
    }
}

void Err_JudgeLowThrhd(uWord Value, sErr_HystrsType * RAMPTR ErrHystrsThrhdPtr, sErr_HystrsType * RAMPTR ErrHystrsCntPtr,
                       uWord * MaxCntValue, uByte * ErrLevelPtr)
{
    // error set
    //ERR_LEVEL_THREE
    if(Value < (ErrHystrsThrhdPtr +2)->Error)
    {
        if((ErrHystrsCntPtr +2)->Error < *(MaxCntValue +4))
        {
            (ErrHystrsCntPtr +2)->Error++;
        }
        else
        {
            (ErrHystrsCntPtr +2)->Error = *(MaxCntValue +4);
            if(*ErrLevelPtr <= 3)
            {
                *ErrLevelPtr = 3;
            }
        }
    }
    else
    {
        (ErrHystrsCntPtr +2)->Error = 0;
    }
    //ERR_LEVEL_TWO
    if(Value < (ErrHystrsThrhdPtr +1)->Error)
    {
        if((ErrHystrsCntPtr +1)->Error < *(MaxCntValue +2))
        {
            (ErrHystrsCntPtr +1)->Error++;
        }
        else
        {
            (ErrHystrsCntPtr +1)->Error = *(MaxCntValue +2);
            if(*ErrLevelPtr <= 2)
            {
                *ErrLevelPtr = 2;
            }
        }
    }
    else
    {
        (ErrHystrsCntPtr +1)->Error = 0;
    }
    //ERR_LEVEL_ONE
    if(Value < ErrHystrsThrhdPtr->Error)
    {
        if(ErrHystrsCntPtr->Error < *MaxCntValue)
        {
            ErrHystrsCntPtr->Error++;
        }
        else
        {
            ErrHystrsCntPtr->Error = *MaxCntValue;
            if(*ErrLevelPtr <= 1)
            {
                *ErrLevelPtr = 1;
            }
        }
    }
    else
    {
        ErrHystrsCntPtr->Error = 0;
    }
    // error clear
    if(*ErrLevelPtr >= 1)
    {
        //ERR_LEVEL_NORMAL
        if((Value > ErrHystrsThrhdPtr->Normal) && (*ErrLevelPtr == 1))
        {
            if(ErrHystrsCntPtr->Normal < *(MaxCntValue +1))
            {
                ErrHystrsCntPtr->Normal++;
            }
            else
            {
                ErrHystrsCntPtr->Normal = *(MaxCntValue +1);
                *ErrLevelPtr = 0;
            }
        }
        else
        {
            ErrHystrsCntPtr->Normal = 0;
        }
        //ERR_LEVEL_ONE
        if((Value > (ErrHystrsThrhdPtr +1)->Normal) && (*ErrLevelPtr == 2))
        {
            if((ErrHystrsCntPtr +1)->Normal < *(MaxCntValue +3))
            {
                (ErrHystrsCntPtr +1)->Normal++;
            }
            else
            {
                (ErrHystrsCntPtr +1)->Normal = *(MaxCntValue +3);
                if(Value < ErrHystrsThrhdPtr->Error)
                {
                    *ErrLevelPtr = 1;
                }
                else
                {
                    *ErrLevelPtr = 0;
                }
            }
        }
        else
        {
            (ErrHystrsCntPtr +1)->Normal = 0;
        }
        //ERR_LEVEL_TWO
        if((Value > (ErrHystrsThrhdPtr +2)->Normal) && (*ErrLevelPtr == 3))
        {
            if((ErrHystrsCntPtr +2)->Normal < *(MaxCntValue +5))
            {
                (ErrHystrsCntPtr +2)->Normal++;
            }
            else
            {
                (ErrHystrsCntPtr +2)->Normal = *(MaxCntValue +5);
                if(Value < (ErrHystrsThrhdPtr +1)->Error)
                {
                    *ErrLevelPtr = 2;
                }
                else if(Value < ErrHystrsThrhdPtr->Error)
                {
                    *ErrLevelPtr = 1;
                }
                else
                {
                    *ErrLevelPtr = 0;
                }
            }
        }
        else
        {
            (ErrHystrsCntPtr +2)->Normal = 0;
        }
    }
    else
    {
        ErrHystrsCntPtr->Normal = 0;
        (ErrHystrsCntPtr + 1)->Normal = 0;
        (ErrHystrsCntPtr + 2)->Normal = 0;
    }
}

// need check
GeneralReturnType Err_JudgeBatErrLevel(void)
{
    GeneralReturnType err_code = ERR_OK;
    uWord max_cnt_value[6];
    uByte err_level = 0;
    uLWord llwtmp;

    uByte max_delta_temp, i, btmp;
    uLWord group_no;
    eBat_ManufacturerType bat_manu;
    eBat_MaterialType bat_type;
    uWord max_delta_volt, current, wtmp;

    if(0 == gby_BatErrJudgeStep )
    {
        gby_BatErrJudgeStep++;
        /* sumv high */
        max_cnt_value[0] = ERR_CNT_SET_SUMV_HIGH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SUMV_HIGH_L1;
        max_cnt_value[2] = ERR_CNT_SET_SUMV_HIGH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SUMV_HIGH_L2;
        max_cnt_value[4] = ERR_CNT_SET_SUMV_HIGH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SUMV_HIGH_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SumVHigh;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SumVHigh - ERR_THRHD_HYSTRS_SUMV_HIGH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SumVHigh;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SumVHigh - ERR_THRHD_HYSTRS_SUMV_HIGH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SumVHigh;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SumVHigh - ERR_THRHD_HYSTRS_SUMV_HIGH_L3;

        err_level = Err_BatErrCode.sBit.SumVHigh;
        Err_JudgeHighThrhd(Bat_GetSumV(INT_SUMV), Err_HystrsThrhd, Err_HystrsCnt.SumVHigh,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SumVHigh = err_level;

        /* sumv low */
        max_cnt_value[0] = ERR_CNT_SET_SUMV_LOW_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SUMV_LOW_L1;
        max_cnt_value[2] = ERR_CNT_SET_SUMV_LOW_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SUMV_LOW_L2;
        max_cnt_value[4] = ERR_CNT_SET_SUMV_LOW_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SUMV_LOW_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SumVLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SumVLow + ERR_THRHD_HYSTRS_SUMV_LOW_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SumVLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SumVLow + ERR_THRHD_HYSTRS_SUMV_LOW_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SumVLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SumVLow + ERR_THRHD_HYSTRS_SUMV_LOW_L3;

        err_level = Err_BatErrCode.sBit.SumVLow;
        Err_JudgeLowThrhd(Bat_GetSumV(INT_SUMV), Err_HystrsThrhd, Err_HystrsCnt.SumVLow,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SumVLow = err_level;

        /* cellv high */
        max_cnt_value[0] = ERR_CNT_SET_CELLV_HIGH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_CELLV_HIGH_L1;
        max_cnt_value[2] = ERR_CNT_SET_CELLV_HIGH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_CELLV_HIGH_L2;
        max_cnt_value[4] = ERR_CNT_SET_CELLV_HIGH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_CELLV_HIGH_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].CellVHigh;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].CellVHigh - ERR_THRHD_HYSTRS_CELLV_HIGH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].CellVHigh;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].CellVHigh - ERR_THRHD_HYSTRS_CELLV_HIGH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].CellVHigh;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].CellVHigh - ERR_THRHD_HYSTRS_CELLV_HIGH_L3;

        err_level = Err_BatErrCode.sBit.CellVHigh;
        Err_JudgeHighThrhd(Bat_GetMaxV(), Err_HystrsThrhd, Err_HystrsCnt.CellVHigh,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.CellVHigh = err_level;

        /* cellv low */
        max_cnt_value[0] = ERR_CNT_SET_CELLV_LOW_L1;
        max_cnt_value[1] = ERR_CNT_CLR_CELLV_LOW_L1;
        max_cnt_value[2] = ERR_CNT_SET_CELLV_LOW_L2;
        max_cnt_value[3] = ERR_CNT_CLR_CELLV_LOW_L2;
        max_cnt_value[4] = ERR_CNT_SET_CELLV_LOW_L3;
        max_cnt_value[5] = ERR_CNT_CLR_CELLV_LOW_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].CellVLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].CellVLow + ERR_THRHD_HYSTRS_CELLV_LOW_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].CellVLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].CellVLow + ERR_THRHD_HYSTRS_CELLV_LOW_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].CellVLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].CellVLow + ERR_THRHD_HYSTRS_CELLV_LOW_L3;

        err_level = Err_BatErrCode.sBit.CellVLow;
        Err_JudgeLowThrhd(Bat_GetMinV(), Err_HystrsThrhd, Err_HystrsCnt.CellVLow,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.CellVLow = err_level;

        /* delt cellv singlepack high */
        max_cnt_value[0] = ERR_CNT_SET_DELTV_L1;
        max_cnt_value[1] = ERR_CNT_CLR_DELTV_L1;
        max_cnt_value[2] = ERR_CNT_SET_DELTV_L2;
        max_cnt_value[3] = ERR_CNT_CLR_DELTV_L2;
        max_cnt_value[4] = ERR_CNT_SET_DELTV_L3;
        max_cnt_value[5] = ERR_CNT_CLR_DELTV_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].DeltVSinglPack;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].DeltVSinglPack - ERR_THRHD_HYSTRS_DELTV_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].DeltVSinglPack;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].DeltVSinglPack - ERR_THRHD_HYSTRS_DELTV_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].DeltVSinglPack;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].DeltVSinglPack - ERR_THRHD_HYSTRS_DELTV_L3;

        err_level = Err_BatErrCode.sBit.DeltVSinglPack;
        max_delta_volt = 0;

        for(i=1; i<=MAX_PACK_NUM; i++)
        {
            wtmp = Bat_GetPackDeltaV(i);
            if (max_delta_volt < wtmp)
            {
                max_delta_volt = wtmp;
            }
        }
        Err_JudgeHighThrhd(max_delta_volt, Err_HystrsThrhd, Err_HystrsCnt.DeltVSinglPack,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.DeltVSinglPack = err_level;

        /* delt cellv allpack high */
        max_cnt_value[0] = ERR_CNT_SET_DELTV_L1;
        max_cnt_value[1] = ERR_CNT_CLR_DELTV_L1;
        max_cnt_value[2] = ERR_CNT_SET_DELTV_L2;
        max_cnt_value[3] = ERR_CNT_CLR_DELTV_L2;
        max_cnt_value[4] = ERR_CNT_SET_DELTV_L3;
        max_cnt_value[5] = ERR_CNT_CLR_DELTV_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].DeltVAllPack;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].DeltVAllPack - ERR_THRHD_HYSTRS_DELTV_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].DeltVAllPack;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].DeltVAllPack - ERR_THRHD_HYSTRS_DELTV_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].DeltVAllPack;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].DeltVAllPack - ERR_THRHD_HYSTRS_DELTV_L3;

        err_level = Err_BatErrCode.sBit.DeltVAllPack;
        Err_JudgeHighThrhd(Bat_GetDeltaV(), Err_HystrsThrhd, Err_HystrsCnt.DeltVAllPack,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.DeltVAllPack = err_level;

        /* delt temper singlepack high */
        max_cnt_value[0] = ERR_CNT_SET_DELTT_L1;
        max_cnt_value[1] = ERR_CNT_CLR_DELTT_L1;
        max_cnt_value[2] = ERR_CNT_SET_DELTT_L2;
        max_cnt_value[3] = ERR_CNT_CLR_DELTT_L2;
        max_cnt_value[4] = ERR_CNT_SET_DELTT_L3;
        max_cnt_value[5] = ERR_CNT_CLR_DELTT_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].DeltTSinglPack;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].DeltTSinglPack - ERR_THRHD_HYSTRS_DELTT_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].DeltTSinglPack;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].DeltTSinglPack - ERR_THRHD_HYSTRS_DELTT_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].DeltTSinglPack;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].DeltTSinglPack - ERR_THRHD_HYSTRS_DELTT_L3;

        err_level = Err_BatErrCode.sBit.DeltTSinglPack;
        max_delta_temp = 0;
        for(i=1; i<=MAX_PACK_NUM; i++)
        {
            btmp = Bat_GetPackDeltaT(i);
            if (max_delta_temp < btmp)
            {
                max_delta_temp = btmp;
            }
        }
        Err_JudgeHighThrhd(max_delta_temp, Err_HystrsThrhd, Err_HystrsCnt.DeltTSinglPack,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.DeltTSinglPack = err_level;

        /* delt temper allpack high */
        max_cnt_value[0] = ERR_CNT_SET_DELTT_L1;
        max_cnt_value[1] = ERR_CNT_CLR_DELTT_L1;
        max_cnt_value[2] = ERR_CNT_SET_DELTT_L2;
        max_cnt_value[3] = ERR_CNT_CLR_DELTT_L2;
        max_cnt_value[4] = ERR_CNT_SET_DELTT_L3;
        max_cnt_value[5] = ERR_CNT_CLR_DELTT_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].DeltTAllPack;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].DeltTAllPack - ERR_THRHD_HYSTRS_DELTT_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].DeltTAllPack;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].DeltTAllPack - ERR_THRHD_HYSTRS_DELTT_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].DeltTAllPack;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].DeltTAllPack - ERR_THRHD_HYSTRS_DELTT_L3;

        err_level = Err_BatErrCode.sBit.DeltTAllPack;
        Err_JudgeHighThrhd(Bat_GetDeltaT(), Err_HystrsThrhd, Err_HystrsCnt.DeltTAllPack,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.DeltTAllPack = err_level;

        /* temper high */
        max_cnt_value[0] = ERR_CNT_SET_TEMPER_HIGH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_TEMPER_HIGH_L1;
        max_cnt_value[2] = ERR_CNT_SET_TEMPER_HIGH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_TEMPER_HIGH_L2;
        max_cnt_value[4] = ERR_CNT_SET_TEMPER_HIGH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_TEMPER_HIGH_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].THigh;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].THigh - ERR_THRHD_HYSTRS_TEMPER_HIGH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].THigh;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].THigh - ERR_THRHD_HYSTRS_TEMPER_HIGH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].THigh;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].THigh - ERR_THRHD_HYSTRS_TEMPER_HIGH_L3;

        err_level = Err_BatErrCode.sBit.THigh;
        if(Bat_GetMaxT() < 0xf0)
        {
            btmp = Bat_GetMaxT();
        }
        else
        {
            btmp = 40;
        }
        Err_JudgeHighThrhd(btmp, Err_HystrsThrhd, Err_HystrsCnt.THigh,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.THigh = err_level;

        /* temper low */
        max_cnt_value[0] = ERR_CNT_SET_TEMPER_LOW_L1;
        max_cnt_value[1] = ERR_CNT_CLR_TEMPER_LOW_L1;
        max_cnt_value[2] = ERR_CNT_SET_TEMPER_LOW_L2;
        max_cnt_value[3] = ERR_CNT_CLR_TEMPER_LOW_L2;
        max_cnt_value[4] = ERR_CNT_SET_TEMPER_LOW_L3;
        max_cnt_value[5] = ERR_CNT_CLR_TEMPER_LOW_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].TLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].TLow + ERR_THRHD_HYSTRS_TEMPER_LOW_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].TLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].TLow + ERR_THRHD_HYSTRS_TEMPER_LOW_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].TLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].TLow + ERR_THRHD_HYSTRS_TEMPER_LOW_L3;

        err_level = Err_BatErrCode.sBit.TLow;
        Err_JudgeLowThrhd(Bat_GetMinT(), Err_HystrsThrhd, Err_HystrsCnt.TLow,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.TLow = err_level;
    }
    else if(1 == gby_BatErrJudgeStep)
    {
        gby_BatErrJudgeStep = 0;
        /* charge current high */
        max_cnt_value[0] = ERR_CNT_SET_CURCHG_L1;
        max_cnt_value[1] = ERR_CNT_CLR_CURCHG_L1;
        max_cnt_value[2] = ERR_CNT_SET_CURCHG_L2;
        max_cnt_value[3] = ERR_CNT_CLR_CURCHG_L2;
        max_cnt_value[4] = ERR_CNT_SET_CURCHG_L3;
        max_cnt_value[5] = ERR_CNT_CLR_CURCHG_L3;
#ifdef ERR_CHG_CUR_TABLE
        if(Chg_GetAcLinkSt())
        {
            llwtmp = Chg_GetPermitChgCurOn();
        }
        else
        {
            llwtmp = Chg_GetPermitChgCurExt();
        }

        llwtmp *= ERR_CHG_CUR_GAIN_L1;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[0].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURCHG_L1)
        {
            Err_HystrsThrhd[0].Normal = llwtmp - ERR_THRHD_HYSTRS_CURCHG_L1;
        }
        else
        {
            Err_HystrsThrhd[0].Normal = llwtmp;
        }

        if(Chg_GetAcLinkSt())
        {
            llwtmp = Chg_GetPermitChgCurOn();
        }
        else
        {
            llwtmp = Chg_GetPermitChgCurExt();
        }

        llwtmp *= ERR_CHG_CUR_GAIN_L2;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[1].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURCHG_L2)
        {
            Err_HystrsThrhd[1].Normal = llwtmp - ERR_THRHD_HYSTRS_CURCHG_L2;
        }
        else
        {
            Err_HystrsThrhd[1].Normal = llwtmp;
        }

        if(Chg_GetAcLinkSt())
        {
            llwtmp = Chg_GetPermitChgCurOn();
        }
        else
        {
            llwtmp = Chg_GetPermitChgCurExt();
        }

        llwtmp *= ERR_CHG_CUR_GAIN_L3;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[2].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURCHG_L3)
        {
            Err_HystrsThrhd[2].Normal = llwtmp - ERR_THRHD_HYSTRS_CURCHG_L3;
        }
        else
        {
            Err_HystrsThrhd[2].Normal = llwtmp;
        }
#else
        Err_HystrsThrhd[0].Error = Err_Thrhd[0].CurHighChg;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].CurHighChg - ERR_THRHD_HYSTRS_CURCHG_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].CurHighChg;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].CurHighChg - ERR_THRHD_HYSTRS_CURCHG_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].CurHighChg;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].CurHighChg - ERR_THRHD_HYSTRS_CURCHG_L3;
#endif
        if( (CHG_ST_OFF != Chg_GetChgSt())
                || (ERR_FLAG_OK == Chg_GetAcLinkSt())
                || (ERR_FLAG_OK == Chg_GetDcLinkSt()) )
        {
            current = Bat_GetCurrent(INSCUR);
            if(current < OFFSET_CUR)
            {
                current = OFFSET_CUR - current;
            }
            else
            {
                current = 0;
            }
        }
        else
        {
            current = 0;
        }

        err_level = Err_BatErrCode.sBit.CurHighChg;
        Err_JudgeHighThrhd(current, Err_HystrsThrhd, Err_HystrsCnt.CurHighChg,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.CurHighChg = err_level;

        /* discharge current high */
        max_cnt_value[0] = ERR_CNT_SET_CURDCH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_CURDCH_L1;
        max_cnt_value[2] = ERR_CNT_SET_CURDCH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_CURDCH_L2;
        max_cnt_value[4] = ERR_CNT_SET_CURDCH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_CURDCH_L3;

#ifdef ERR_DIS_CUR_TABLE
        llwtmp = Bat_GetMaxPermitDchCur();
        llwtmp *= ERR_DIS_CUR_GAIN_L1;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[0].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURDCH_L1)
        {
            Err_HystrsThrhd[0].Normal = llwtmp - ERR_THRHD_HYSTRS_CURDCH_L1;
        }
        else
        {
            Err_HystrsThrhd[0].Normal = llwtmp;
        }

        llwtmp = Bat_GetMaxPermitDchCur();
        llwtmp *= ERR_DIS_CUR_GAIN_L2;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[1].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURDCH_L2)
        {
            Err_HystrsThrhd[1].Normal = llwtmp - ERR_THRHD_HYSTRS_CURDCH_L2;
        }
        else
        {
            Err_HystrsThrhd[1].Normal = llwtmp;
        }

        llwtmp = Bat_GetMaxPermitDchCur();
        llwtmp *= ERR_DIS_CUR_GAIN_L3;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[2].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURDCH_L3)
        {
            Err_HystrsThrhd[2].Normal = llwtmp - ERR_THRHD_HYSTRS_CURDCH_L3;
        }
        else
        {
            Err_HystrsThrhd[2].Normal = llwtmp;
        }
#else
        Err_HystrsThrhd[0].Error = Err_Thrhd[0].CurHighDch;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].CurHighDch - ERR_THRHD_HYSTRS_CURDCH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].CurHighDch;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].CurHighDch - ERR_THRHD_HYSTRS_CURDCH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].CurHighDch;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].CurHighDch - ERR_THRHD_HYSTRS_CURDCH_L3;
#endif
        current = Bat_GetCurrent(INSCUR);
        if(current > OFFSET_CUR)
        {
            current -= OFFSET_CUR;
        }
        else
        {
            current = 0;
        }

        err_level = Err_BatErrCode.sBit.CurHighDch;
        Err_JudgeHighThrhd(current, Err_HystrsThrhd, Err_HystrsCnt.CurHighDch,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.CurHighDch = err_level;

        /* feedback current high */
        max_cnt_value[0] = ERR_CNT_SET_CURFEEDBAK_L1;
        max_cnt_value[1] = ERR_CNT_CLR_CURFEEDBAK_L1;
        max_cnt_value[2] = ERR_CNT_SET_CURFEEDBAK_L2;
        max_cnt_value[3] = ERR_CNT_CLR_CURFEEDBAK_L2;
        max_cnt_value[4] = ERR_CNT_SET_CURFEEDBAK_L3;
        max_cnt_value[5] = ERR_CNT_CLR_CURFEEDBAK_L3;

#ifdef ERR_FDB_CUR_TABLE
        llwtmp = Bat_GetMaxFeedBackCur();
        llwtmp *= ERR_FDB_CUR_GAIN_L1;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[0].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURFEEDBAK_L1)
        {
            Err_HystrsThrhd[0].Normal = llwtmp - ERR_THRHD_HYSTRS_CURFEEDBAK_L1;
        }
        else
        {
            Err_HystrsThrhd[0].Normal = llwtmp;
        }

        llwtmp = Bat_GetMaxFeedBackCur();
        llwtmp *= ERR_FDB_CUR_GAIN_L2;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[1].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURFEEDBAK_L2)
        {
            Err_HystrsThrhd[1].Normal = llwtmp - ERR_THRHD_HYSTRS_CURFEEDBAK_L2;
        }
        else
        {
            Err_HystrsThrhd[1].Normal = llwtmp;
        }

        llwtmp *= ERR_FDB_CUR_GAIN_L3;
        llwtmp /= 10;
        if(0 == llwtmp)
        {
            llwtmp = 10;
        }
        Err_HystrsThrhd[2].Error = llwtmp;

        if(llwtmp > ERR_THRHD_HYSTRS_CURFEEDBAK_L3)
        {
            Err_HystrsThrhd[2].Normal = llwtmp - ERR_THRHD_HYSTRS_CURFEEDBAK_L3;
        }
        else
        {
            Err_HystrsThrhd[2].Normal = llwtmp;
        }
#else
        Err_HystrsThrhd[0].Error = Err_Thrhd[0].CurHighChg;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].CurHighChg - ERR_THRHD_HYSTRS_CURFEEDBAK_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].CurHighChg;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].CurHighChg - ERR_THRHD_HYSTRS_CURFEEDBAK_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].CurHighChg;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].CurHighChg - ERR_THRHD_HYSTRS_CURFEEDBAK_L3;
#endif
        if( (CHG_ST_OFF == Chg_GetChgSt())
                || ((CHG_ST_ON != Chg_GetChgSt()) && (ERR_FLAG_FAIL == Chg_GetAcLinkSt()) && (ERR_FLAG_FAIL == Chg_GetDcLinkSt())) )
        {

            current = Bat_GetCurrent(INSCUR);
            if(current < OFFSET_CUR)
            {
                current = OFFSET_CUR - current;
            }
            else
            {
                current = 0;
            }
        }
        else
        {
            current = 0;
        }

        err_level = Err_BatErrCode.sBit.CurHighFdbck;              //////////
        Err_JudgeHighThrhd(current, Err_HystrsThrhd, Err_HystrsCnt.CurHighFd,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.CurHighFdbck = err_level;
        /* rsv for 3b2 */

        /* soc high */
        max_cnt_value[0] = ERR_CNT_SET_SOC_HIGH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SOC_HIGH_L1;
        max_cnt_value[2] = ERR_CNT_SET_SOC_HIGH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SOC_HIGH_L2;
        max_cnt_value[4] = ERR_CNT_SET_SOC_HIGH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SOC_HIGH_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SocHigh;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SocHigh - ERR_THRHD_HYSTRS_SOC_HIGH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SocHigh;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SocHigh - ERR_THRHD_HYSTRS_SOC_HIGH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SocHigh;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SocHigh - ERR_THRHD_HYSTRS_SOC_HIGH_L3;

        err_level = Err_BatErrCode.sBit.SocHigh;
        Err_JudgeHighThrhd(Bat_GetSOC(), Err_HystrsThrhd, Err_HystrsCnt.SocHigh,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SocHigh = err_level;

        /* soc low */
        max_cnt_value[0] = ERR_CNT_SET_SOC_LOW_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SOC_LOW_L1;
        max_cnt_value[2] = ERR_CNT_SET_SOC_LOW_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SOC_LOW_L2;
        max_cnt_value[4] = ERR_CNT_SET_SOC_LOW_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SOC_LOW_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SocLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SocLow + ERR_THRHD_HYSTRS_SOC_LOW_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SocLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SocLow + ERR_THRHD_HYSTRS_SOC_LOW_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SocLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SocLow + ERR_THRHD_HYSTRS_SOC_LOW_L3;

        err_level = Err_BatErrCode.sBit.SocLow;
        Err_JudgeLowThrhd(Bat_GetSOC(), Err_HystrsThrhd, Err_HystrsCnt.SocLow,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SocLow = err_level;

        /* cell soc high */
        /*Err_HystrsThrhd[0].Error = Err_Thrhd[0].SocHigh;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SocHigh + ERR_THRHD_HYSTRS_SOC_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SocHigh;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SocHigh + ERR_THRHD_HYSTRS_SOC_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SocHigh;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SocHigh + ERR_THRHD_HYSTRS_SOC_L3;

        Err_JudgeLowThrhd(Bat_GetCellSOC(), Err_HystrsThrhd, Err_HystrsCnt.SocHigh,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SocHighCell = err_level;*/

        /* cell soc low */
        /*max_cnt_value[0] = ERR_CNT_SET_SOC_LOW_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SOC_LOW_L1;
        max_cnt_value[2] = ERR_CNT_SET_SOC_LOW_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SOC_LOW_L2;
        max_cnt_value[4] = ERR_CNT_SET_SOC_LOW_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SOC_LOW_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SocLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SocLow - ERR_THRHD_HYSTRS_SOC_LOW_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SocLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SocLow - ERR_THRHD_HYSTRS_SOC_LOW_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SocLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SocLow - ERR_THRHD_HYSTRS_SOC_LOW_L3;

        Err_JudgeLowThrhd(Bat_GetCellSOC(), Err_HystrsThrhd, Err_HystrsCnt.SocHigh,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SocLowCell = err_level;*/

        /* cell soc diff */

        /* avrg voltage allpack diff */

        /* battery group number diff */

        group_no = Bat_GetPackNo(1);
        for(i=2; i<=MAX_PACK_NUM; i++)
        {
            if(group_no != Bat_GetPackNo(i))
            {
                Err_BatErrCode.sBit.GroupNoDiff = (uByte)ERR_LEVEL_TWO;
            }
        }

        /* cell int_res high */
        max_cnt_value[0] = ERR_CNT_SET_INTRES_L1;
        max_cnt_value[1] = ERR_CNT_CLR_INTRES_L1;
        max_cnt_value[2] = ERR_CNT_SET_INTRES_L2;
        max_cnt_value[3] = ERR_CNT_CLR_INTRES_L2;
        max_cnt_value[4] = ERR_CNT_SET_INTRES_L3;
        max_cnt_value[5] = ERR_CNT_CLR_INTRES_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].IntResHighCell;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].IntResHighCell + ERR_THRHD_HYSTRS_INTRES_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].IntResHighCell;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].IntResHighCell + ERR_THRHD_HYSTRS_INTRES_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].IntResHighCell;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].IntResHighCell + ERR_THRHD_HYSTRS_INTRES_L3;

        err_level = Err_BatErrCode.sBit.IntResHighCell;
        Err_JudgeHighThrhd(Bat_GetMaxR(), Err_HystrsThrhd, Err_HystrsCnt.IntResHighCell,
                           max_cnt_value, &err_level);
        Err_BatErrCode.sBit.IntResHighCell = err_level;

        /* cell int_res diff */

        /* battery int_res high */

        /* soc singlpack diff */

        /* battery manu diff */

        bat_manu = Bat_GetPackManufacturer(1);
        for(i=2; i<=MAX_PACK_NUM; i++)
        {
            if(bat_manu != Bat_GetPackManufacturer(i))
            {
                Err_BatErrCode.sBit.ManuDiff = (uByte)ERR_LEVEL_TWO;
            }
        }

        /* battery type diff */

        bat_type = Bat_GetPackMaterialType(1);
        for(i=2; i<=MAX_PACK_NUM; i++)
        {
            if(bat_type != Bat_GetPackMaterialType(i))
            {
                Err_BatErrCode.sBit.TypeDiff = (uByte)ERR_LEVEL_TWO;
            }
        }

        /* battery short circuit */

        /* soh or cap low */
        max_cnt_value[0] = ERR_CNT_SET_SOH_L1;
        max_cnt_value[1] = ERR_CNT_CLR_SOH_L1;
        max_cnt_value[2] = ERR_CNT_SET_SOH_L2;
        max_cnt_value[3] = ERR_CNT_CLR_SOH_L2;
        max_cnt_value[4] = ERR_CNT_SET_SOH_L3;
        max_cnt_value[5] = ERR_CNT_CLR_SOH_L3;

        Err_HystrsThrhd[0].Error = Err_Thrhd[0].SohLow;
        Err_HystrsThrhd[0].Normal = Err_Thrhd[0].SohLow + ERR_THRHD_HYSTRS_SOH_L1;
        Err_HystrsThrhd[1].Error = Err_Thrhd[1].SohLow;
        Err_HystrsThrhd[1].Normal = Err_Thrhd[1].SohLow + ERR_THRHD_HYSTRS_SOH_L2;
        Err_HystrsThrhd[2].Error = Err_Thrhd[2].SohLow;
        Err_HystrsThrhd[2].Normal = Err_Thrhd[2].SohLow + ERR_THRHD_HYSTRS_SOH_L3;

        err_level = Err_BatErrCode.sBit.SohLow;
        Err_JudgeLowThrhd(Bat_GetSoh(), Err_HystrsThrhd, Err_HystrsCnt.SohLow,
                          max_cnt_value, &err_level);
        Err_BatErrCode.sBit.SohLow = err_level;
    }
    /* cell soh or cap low */

    /* cell soh or cap diff */

    /* rsv for 7b6 */

    return err_code;
}


// need check
GeneralReturnType Err_JudgeBatSysErrLevel(void)
{
    GeneralReturnType err_code = ERR_OK;
    uWord max_cnt_value[6];
    uByte err_level = 0;

    /* cell pole temper high */
    max_cnt_value[0] = ERR_CNT_SET_TEMPER_HIGH_L1;
    max_cnt_value[1] = ERR_CNT_CLR_TEMPER_HIGH_L1;
    max_cnt_value[2] = ERR_CNT_SET_TEMPER_HIGH_L2;
    max_cnt_value[3] = ERR_CNT_CLR_TEMPER_HIGH_L2;
    max_cnt_value[4] = ERR_CNT_SET_TEMPER_HIGH_L3;
    max_cnt_value[5] = ERR_CNT_CLR_TEMPER_HIGH_L3;

    Err_HystrsThrhd[0].Error = Err_Thrhd[0].THighPole;
    Err_HystrsThrhd[0].Normal = Err_Thrhd[0].THighPole - ERR_THRHD_HYSTRS_TEMPER_HIGH_L1;
    Err_HystrsThrhd[1].Error = Err_Thrhd[1].THighPole;
    Err_HystrsThrhd[1].Normal = Err_Thrhd[1].THighPole - ERR_THRHD_HYSTRS_TEMPER_HIGH_L2;
    Err_HystrsThrhd[2].Error = Err_Thrhd[2].THighPole;
    Err_HystrsThrhd[2].Normal = Err_Thrhd[2].THighPole - ERR_THRHD_HYSTRS_TEMPER_HIGH_L3;

    err_level = Err_BatSysErrCode.sBit.THighPole;
    //Err_JudgeHighThrhd(Bat_GetPoleT(), Err_HystrsThrhd, Err_HystrsCnt.THighPole,
    //                   max_cnt_value, &err_level);
    Err_BatSysErrCode.sBit.THighPole = err_level;

    /* inlet temper high */

    /* inlet temper low */

    /* delt temper inoutlet low */

    /* outlet temper high */

    /* outlet temper low */

    /* supply voltage high */
    max_cnt_value[0] = ERR_CNT_SET_POWVOLT_HIGH_L1;
    max_cnt_value[1] = ERR_CNT_CLR_POWVOLT_HIGH_L1;
    max_cnt_value[2] = ERR_CNT_SET_POWVOLT_HIGH_L2;
    max_cnt_value[3] = ERR_CNT_CLR_POWVOLT_HIGH_L2;
    max_cnt_value[4] = ERR_CNT_SET_POWVOLT_HIGH_L3;
    max_cnt_value[5] = ERR_CNT_CLR_POWVOLT_HIGH_L3;

    Err_HystrsThrhd[0].Error = ERR_THRHD_POWVOLT_HIGH_L1;// 0.1V
    Err_HystrsThrhd[0].Normal = ERR_THRHD_POWVOLT_HIGH_L1 - ERR_THRHD_HYSTRS_POWVOLT_HIGH_L1;
    Err_HystrsThrhd[1].Error = ERR_THRHD_POWVOLT_HIGH_L2;
    Err_HystrsThrhd[1].Normal = ERR_THRHD_POWVOLT_HIGH_L2 - ERR_THRHD_HYSTRS_POWVOLT_HIGH_L2;
    Err_HystrsThrhd[2].Error = ERR_THRHD_POWVOLT_HIGH_L3;
    Err_HystrsThrhd[2].Normal = ERR_THRHD_POWVOLT_HIGH_L3 - ERR_THRHD_HYSTRS_POWVOLT_HIGH_L3;

    err_level = Err_BatSysErrCode.sBit.SupplyVHigh;
    Err_JudgeHighThrhd(BmsSys_GetK30Volt(), Err_HystrsThrhd, Err_HystrsCnt.SupplyVHigh,
                       max_cnt_value, &err_level);
    Err_BatSysErrCode.sBit.SupplyVHigh = err_level;

    /* supply voltage low */
    max_cnt_value[0] = ERR_CNT_SET_POWVOLT_LOW_L1;
    max_cnt_value[1] = ERR_CNT_CLR_POWVOLT_LOW_L1;
    max_cnt_value[2] = ERR_CNT_SET_POWVOLT_LOW_L2;
    max_cnt_value[3] = ERR_CNT_CLR_POWVOLT_LOW_L2;
    max_cnt_value[4] = ERR_CNT_SET_POWVOLT_LOW_L3;
    max_cnt_value[5] = ERR_CNT_CLR_POWVOLT_LOW_L3;

    Err_HystrsThrhd[0].Error = ERR_THRHD_POWVOLT_LOW_L1;// 0.1V
    Err_HystrsThrhd[0].Normal = ERR_THRHD_POWVOLT_LOW_L1 - ERR_THRHD_HYSTRS_POWVOLT_LOW_L1;
    Err_HystrsThrhd[1].Error = ERR_THRHD_POWVOLT_LOW_L2;
    Err_HystrsThrhd[1].Normal = ERR_THRHD_POWVOLT_LOW_L2 - ERR_THRHD_HYSTRS_POWVOLT_LOW_L2;
    Err_HystrsThrhd[2].Error = ERR_THRHD_POWVOLT_LOW_L3;
    Err_HystrsThrhd[2].Normal = ERR_THRHD_POWVOLT_LOW_L3 - ERR_THRHD_HYSTRS_POWVOLT_LOW_L3;

    err_level = Err_BatSysErrCode.sBit.SupplyVLow;
    Err_JudgeLowThrhd(BmsSys_GetK30Volt(), Err_HystrsThrhd, Err_HystrsCnt.SupplyVLow,
                      max_cnt_value, &err_level);
    Err_BatSysErrCode.sBit.SupplyVLow = err_level;

    /* fuse temper high */

    return err_code;
}

void Err_DealHwErrLevel(void)
{
    uByte i;
    uWord wtmp;

    for(i=0; i<8; i++)
    {
        Err_HwErrCode.ByteN.By[i] = 0;
    }

    // check can4 error
    wtmp = 0;//IntCan_GetBmuMaxOutLineCnt();
    wtmp /= 2;

    if(wtmp >= ERR_CNT_CAN4_L3)
    {
        Err_HwErrCode.sBit.Can4 = ERR_LEVEL_THREE;
    }
    else if(wtmp >= ERR_CNT_CAN4_L2)
    {
        Err_HwErrCode.sBit.Can4 = ERR_LEVEL_TWO;
    }
    else if(wtmp >= ERR_CNT_CAN4_L1)
    {
        Err_HwErrCode.sBit.Can4 = ERR_LEVEL_ONE;
    }
    else
    {
        if((ERR_LEVEL_THREE == Err_HwErrCode.sBit.Can4) && (ERR_CLR_CAN4_L3_EN))
        {
            Err_HwErrCode.sBit.Can4 = ERR_LEVEL_NORMAL;
        }
        else if((ERR_LEVEL_TWO == Err_HwErrCode.sBit.Can4) && (ERR_CLR_CAN4_L2_EN))
        {
            Err_HwErrCode.sBit.Can4 = ERR_LEVEL_NORMAL;
        }
        else if((ERR_LEVEL_TWO == Err_HwErrCode.sBit.Can4) && (ERR_CLR_CAN4_L1_EN))
        {
            Err_HwErrCode.sBit.Can4 = ERR_LEVEL_NORMAL;
        }
    }
    
    // check sofware error
    if(__GET_ASSERT_ERROR_CNT() >= 1)
    {
        Err_HwErrCode.sBit.Software = ERR_LEVEL_TWO;
    }

    for(i=0; i <= Bat_GetBmuNum(); i++)
    {
        if(Err_HwErrCode.sBit.DetectCellV < Err_HwErrCodeBmu[i].sBit.DetectCellV)
        {
            Err_HwErrCode.sBit.DetectCellV = Err_HwErrCodeBmu[i].sBit.DetectCellV;
        }

        if(Err_HwErrCode.sBit.DetectTemper < Err_HwErrCodeBmu[i].sBit.DetectTemper)
        {
            Err_HwErrCode.sBit.DetectTemper = Err_HwErrCodeBmu[i].sBit.DetectTemper;
        }

        if(Err_HwErrCode.sBit.DetectCur < Err_HwErrCodeBmu[i].sBit.DetectCur)
        {
            Err_HwErrCode.sBit.DetectCur = Err_HwErrCodeBmu[i].sBit.DetectCur;
        }

        if(Err_HwErrCode.sBit.DetectIso < Err_HwErrCodeBmu[i].sBit.DetectIso)
        {
            Err_HwErrCode.sBit.DetectIso = Err_HwErrCodeBmu[i].sBit.DetectIso;
        }

        if(Err_HwErrCode.sBit.DetectSumVIn < Err_HwErrCodeBmu[i].sBit.DetectSumVIn)
        {
            Err_HwErrCode.sBit.DetectSumVIn = Err_HwErrCodeBmu[i].sBit.DetectSumVIn;
        }

        if(Err_HwErrCode.sBit.DetectSumVOut < Err_HwErrCodeBmu[i].sBit.DetectSumVOut)
        {
            Err_HwErrCode.sBit.DetectSumVOut = Err_HwErrCodeBmu[i].sBit.DetectSumVOut;
        }

        if(Err_HwErrCode.sBit.ClockExt < Err_HwErrCodeBmu[i].sBit.ClockExt)
        {
            Err_HwErrCode.sBit.ClockExt = Err_HwErrCodeBmu[i].sBit.ClockExt;
        }

        if(Err_HwErrCode.sBit.EepromCpu < Err_HwErrCodeBmu[i].sBit.EepromCpu)
        {
            Err_HwErrCode.sBit.EepromCpu = Err_HwErrCodeBmu[i].sBit.EepromCpu;
        }

        if(Err_HwErrCode.sBit.EepromExt < Err_HwErrCodeBmu[i].sBit.EepromExt)
        {
            Err_HwErrCode.sBit.EepromExt = Err_HwErrCodeBmu[i].sBit.EepromExt;
        }

        if(Err_HwErrCode.sBit.FlashExt < Err_HwErrCodeBmu[i].sBit.FlashExt)
        {
            Err_HwErrCode.sBit.FlashExt = Err_HwErrCodeBmu[i].sBit.FlashExt;
        }

        if(Err_HwErrCode.sBit.FlashCpu < Err_HwErrCodeBmu[i].sBit.FlashCpu)
        {
            Err_HwErrCode.sBit.FlashCpu = Err_HwErrCodeBmu[i].sBit.FlashCpu;
        }

        if(Err_HwErrCode.sBit.CpuGeneral < Err_HwErrCodeBmu[i].sBit.CpuGeneral)
        {
            Err_HwErrCode.sBit.CpuGeneral = Err_HwErrCodeBmu[i].sBit.CpuGeneral;
        }

        if(Err_HwErrCode.sBit.Can4 < Err_HwErrCodeBmu[i].sBit.Can4)
        {
            Err_HwErrCode.sBit.Can4 = Err_HwErrCodeBmu[i].sBit.Can4;
        }

        if(Err_HwErrCode.sBit.Can0 < Err_HwErrCodeBmu[i].sBit.Can0)
        {
            Err_HwErrCode.sBit.Can0 = Err_HwErrCodeBmu[i].sBit.Can0;
        }

        if(Err_HwErrCode.sBit.Can1 < Err_HwErrCodeBmu[i].sBit.Can1)
        {
            Err_HwErrCode.sBit.Can1 = Err_HwErrCodeBmu[i].sBit.Can1;
        }

        if(Err_HwErrCode.sBit.Can2 < Err_HwErrCodeBmu[i].sBit.Can2)
        {
            Err_HwErrCode.sBit.Can2 = Err_HwErrCodeBmu[i].sBit.Can2;
        }

        if(Err_HwErrCode.sBit.Can3 < Err_HwErrCodeBmu[i].sBit.Can3)
        {
            Err_HwErrCode.sBit.Can3 = Err_HwErrCodeBmu[i].sBit.Can3;
        }

        if(Err_HwErrCode.sBit.Ethernet < Err_HwErrCodeBmu[i].sBit.Ethernet)
        {
            Err_HwErrCode.sBit.Ethernet = Err_HwErrCodeBmu[i].sBit.Ethernet;
        }

        if(Err_HwErrCode.sBit.Rs232 < Err_HwErrCodeBmu[i].sBit.Rs232)
        {
            Err_HwErrCode.sBit.Rs232 = Err_HwErrCodeBmu[i].sBit.Rs232;
        }

        if(Err_HwErrCode.sBit.Rs485 < Err_HwErrCodeBmu[i].sBit.Rs485)
        {
            Err_HwErrCode.sBit.Rs485 = Err_HwErrCodeBmu[i].sBit.Rs485;
        }

        if(Err_HwErrCode.sBit.BalChg < Err_HwErrCodeBmu[i].sBit.BalChg)
        {
            Err_HwErrCode.sBit.BalChg = Err_HwErrCodeBmu[i].sBit.BalChg;
        }

        if(Err_HwErrCode.sBit.BalDch < Err_HwErrCodeBmu[i].sBit.BalDch)
        {
            Err_HwErrCode.sBit.BalDch = Err_HwErrCodeBmu[i].sBit.BalDch;
        }

        if(Err_HwErrCode.sBit.DetectWh < Err_HwErrCodeBmu[i].sBit.DetectWh)
        {
            Err_HwErrCode.sBit.DetectWh = Err_HwErrCodeBmu[i].sBit.DetectWh;
        }

        if(Err_HwErrCode.sBit.Software < Err_HwErrCodeBmu[i].sBit.Software)
        {
            Err_HwErrCode.sBit.Software = Err_HwErrCodeBmu[i].sBit.Software;
        }

        if(Err_HwErrCode.sBit.RsvFor6b0 < Err_HwErrCodeBmu[i].sBit.RsvFor6b0)
        {
            Err_HwErrCode.sBit.RsvFor6b0 = Err_HwErrCodeBmu[i].sBit.RsvFor6b0;
        }

        if(Err_HwErrCode.sBit.RsvFor6b2 < Err_HwErrCodeBmu[i].sBit.RsvFor6b2)
        {
            Err_HwErrCode.sBit.RsvFor6b2 = Err_HwErrCodeBmu[i].sBit.RsvFor6b2;
        }

        if(Err_HwErrCode.sBit.Ai < Err_HwErrCodeBmu[i].sBit.Ai)
        {
            Err_HwErrCode.sBit.Ai = Err_HwErrCodeBmu[i].sBit.Ai;
        }

        if(Err_HwErrCode.sBit.Ao < Err_HwErrCodeBmu[i].sBit.Ao)
        {
            Err_HwErrCode.sBit.Ao = Err_HwErrCodeBmu[i].sBit.Ao;
        }

        if(Err_HwErrCode.sBit.Di < Err_HwErrCodeBmu[i].sBit.Di)
        {
            Err_HwErrCode.sBit.Di = Err_HwErrCodeBmu[i].sBit.Di;
        }

        if(Err_HwErrCode.sBit.Do < Err_HwErrCodeBmu[i].sBit.Do)
        {
            Err_HwErrCode.sBit.Do = Err_HwErrCodeBmu[i].sBit.Do;
        }

        if(Err_HwErrCode.sBit.PreChg < Err_HwErrCodeBmu[i].sBit.PreChg)
        {
            Err_HwErrCode.sBit.PreChg = Err_HwErrCodeBmu[i].sBit.PreChg;
        }

        if(Err_HwErrCode.sBit.RelayDriver < Err_HwErrCodeBmu[i].sBit.RelayDriver)
        {
            Err_HwErrCode.sBit.RelayDriver = Err_HwErrCodeBmu[i].sBit.RelayDriver;
        }
    }
}

/*void Err_DealHwErrLevel(void)
{
    GeneralReturnType err_code = ERR_OK;
    eErr_LevelType err_level = ERR_LEVEL_INVALD, err_level_tmp;
    uByte i, j;

    for(j=0; j<ERR_NUM_HW; j++)
    {
        err_level = ERR_LEVEL_NORMAL;
        for(i=0; i<MAX_PACK_NUM; i++)
        {
            err_level_tmp = Err_GetHwErrLevelSinglePack(i, (eErr_HwErrIndexType)j);
            if(err_level < err_level_tmp)
            {
                err_level = err_level_tmp;
            }
        }
        err_code = Err_UpdateHwErrLevel((eErr_HwErrIndexType)j, (eErr_LevelType)err_level);
    }
}*/

void Err_DealHwErrDetail(void)
{
    uByte i, j;

    for(j=0; j<ERR_NUM_HW_DETAIL; j++)
    {
        Err_HwErrDetail[j] = 0;

        for(i=0; i<=MAX_PACK_NUM; i++)
        {
            Err_HwErrDetail[j] |= Err_HwErrDetailBmu[i][j];
        }
    }
}


uWord Err_GetSumVHighThrd1(void)
{
    return Err_Thrhd[0].SumVHigh;
}

uWord Err_GetSumVLowThrd1(void)
{
    return Err_Thrhd[0].SumVLow;
}

uWord Err_GetCellVHighThrd1(void)
{
    return Err_Thrhd[0].CellVHigh;
}

uWord Err_GetCellVLowThrd1(void)
{
    return Err_Thrhd[0].CellVLow;
}

uWord Err_GetDisChgCurHighThrd1(void)
{
    return Err_Thrhd[0].CurHighDch;
}

uWord Err_GetChgCurHighThrd1(void)
{
    return Err_Thrhd[0].CurHighChg;
}

uWord Err_GetSocHighThrd1(void)
{
    return Err_Thrhd[0].SocHigh;
}

uWord Err_GetSocLowThrd1(void)
{
    return Err_Thrhd[0].SocLow;
}

uWord Err_GetTHighThrd1(void)
{
    return Err_Thrhd[0].THigh;
}

uWord Err_GetTLowThrd1(void)
{
    return Err_Thrhd[0].TLow;
}
// end of inner function interface





// debug interface function
//#define __DEBUG_TEST_ERRORALARM
#ifdef __DEBUG_TEST_ERRORALARM

// only use for debug
GeneralReturnType Err_UpdateBatErrLevel(eErr_BatErrIndexType BatErrIndex, eErr_LevelType ErrLevel)
{
    GeneralReturnType err_code = ERR_OK;
    uByte byte_no, bit_no;

    if(BatErrIndex > ERR_BAT_RSV_FOR_7_6)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    if(ErrLevel > ERR_LEVEL_THREE)
    {
        err_code = (GeneralReturnType)ERR_CODE_LEVEL_OVER;
        return err_code;
    }

    byte_no = (uByte)((uWord)BatErrIndex / 4);
    bit_no = (uByte)((uWord)BatErrIndex % 4);

    Err_BatErrCode.ByteN.By[byte_no] &= ~((uByte)(0x03 << (bit_no * 2)));
    Err_BatErrCode.ByteN.By[byte_no] |= ((uByte)ErrLevel << (bit_no * 2));

    return err_code;
}


// only use for debug
eErr_LevelType Err_GetHwErrLevelSinglePack(uByte BmuNo, eErr_HwErrIndexType HwErrIndex)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD;
    uByte byte_no, bit_no;

    if(BmuNo > MAX_BMU_NUM)
    {
        return err_level;
    }
    if(HwErrIndex > ERR_HW_RSV_FOR_7_6)
    {
        return err_level;
    }

    byte_no = (uByte)((uWord)HwErrIndex / 4);
    bit_no = (uByte)((uWord)HwErrIndex % 4);

    err_level = (eErr_LevelType)((Err_HwErrCodeBmu[BmuNo].ByteN.By[byte_no] >> (bit_no * 2)) & 0x03);

    return err_level;
}

// only use for debug
sErr_ByteNType Err_GetHwAllErrLevelSinglePack(uByte BmuNo)
{
    sErr_ByteNType err_byte_n;
    uByte i;

    if(BmuNo > MAX_BMU_NUM)
    {
        for(i=0; i<8; i++)
        {
            err_byte_n.By[i] = (uByte)ERR_LEVEL_INVALD;
        }
        return err_byte_n;
    }

    for(i=0; i<8; i++)
    {
        err_byte_n.By[i] = Err_HwErrCodeBmu[BmuNo].ByteN.By[i];
    }

    return err_byte_n;
}

// only use for debug
Err_HwErrDetailType Err_GetHwErrDetailSinglePack(uByte BmuNo, eErr_HwErrIndexType HwErrIndex)
{
    if(BmuNo > MAX_BMU_NUM)
    {
        return ERR_HW_DETAIL_INVALID;
    }
    if(HwErrIndex > ERR_HW_RSV_FOR_7_6)
    {
        return ERR_HW_DETAIL_INVALID;
    }
    return Err_HwErrDetailBmu[BmuNo][HwErrIndex];
}

// only use for debug
GeneralReturnType Err_UpdateHwErrLevelAllPack(eErr_HwErrIndexType HwErrIndex, eErr_LevelType ErrLevel)
{
    GeneralReturnType err_code = ERR_OK;
    uByte byte_no, bit_no;

    if(HwErrIndex > ERR_HW_RSV_FOR_7_6)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    if(ErrLevel > ERR_LEVEL_THREE)
    {
        err_code = (GeneralReturnType)ERR_CODE_LEVEL_OVER;
        return err_code;
    }

    byte_no = (uByte)((uWord)HwErrIndex / 4);
    bit_no = (uByte)((uWord)HwErrIndex % 4);

    Err_HwErrCode.ByteN.By[byte_no] &= ~((uByte)(0x03 << (bit_no * 2)));
    Err_HwErrCode.ByteN.By[byte_no] |= ((uByte)ErrLevel << (bit_no * 2));

    return err_code;
}

// only use for debug
void Err_UpdateHwAllErrLevelAllPack(sErr_ByteNType * HwErrByteNPtr)
{
    uByte i;

    for(i=0; i<8; i++)
    {
        Err_HwErrCode.ByteN.By[i] = HwErrByteNPtr->By[i];
    }
}

// only use for debug
GeneralReturnType Err_UpdateHwErrDetailAllPack(eErr_HwErrIndexType HwErrIndex, Err_HwErrDetailType HwErrDetail)
{
    GeneralReturnType err_code = ERR_OK;

    if(HwErrIndex > ERR_HW_RSV_FOR_7_6)
    {
        err_code = (GeneralReturnType)ERR_CODE_INDEX_OVER;
        return err_code;
    }

    Err_HwErrDetail[HwErrIndex] = HwErrDetail;

    return err_code;
}


#define __DEBUG_ERR_EXT_FUNC
//#define __DEBUG_ERR_INT_FUNC
void Err_InitDebug(void)
{
    volatile GeneralReturnType err_code = ERR_OK;
    sErr_ByteNType data;
    uByte i, j;

    for(i=0; i<8; i++)
    {
        data.By[i] = 0xE4;//level 3210
    }

    Err_Init();

#ifdef __DEBUG_ERR_EXT_FUNC

    for(i=0; i<ERR_NUM_BAT; i++)
    {
        err_code = Err_UpdateBatErrLevel((eErr_BatErrIndexType)i, (eErr_LevelType)(i%4));//level 0123
        __ASSERT(ERR_OK == err_code)
    }
    err_code = Err_UpdateBatErrLevel(ERR_BAT_RSV_FOR_7_6, (eErr_LevelType)4);
    __ASSERT((GeneralReturnType)ERR_CODE_LEVEL_OVER == err_code)
    err_code = Err_UpdateBatErrLevel((eErr_BatErrIndexType)ERR_NUM_BAT, ERR_LEVEL_NORMAL);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)

    for(i=0; i<ERR_NUM_BATSYS; i++)
    {
        err_code = Err_UpdateBatSysErrLevel((eErr_BatSysErrIndexType)i, (eErr_LevelType)(i%4));//level 0123
        __ASSERT(ERR_OK == err_code)
    }
    err_code = Err_UpdateBatSysErrLevel(ERR_BATSYS_RSV_FOR_15_6, (eErr_LevelType)4);
    __ASSERT((GeneralReturnType)ERR_CODE_LEVEL_OVER == err_code)
    err_code = Err_UpdateBatSysErrLevel((eErr_BatSysErrIndexType)ERR_NUM_BATSYS, ERR_LEVEL_NORMAL);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)



#endif//#ifdef __DEBUG_ERR_EXT_FUNC

    err_code = Err_UpdateHwErrLevelFromCan(0, &data);
    __ASSERT((GeneralReturnType)ERR_CODE_BMU_NO_ZERO == err_code)
    for(i=1; i<MAX_PACK_NUM; i++)
    {
        err_code = Err_UpdateHwErrLevelFromCan(i, &data);
        __ASSERT(ERR_OK == err_code)
    }
    err_code = Err_UpdateHwErrLevelFromCan(MAX_PACK_NUM, &data);
    __ASSERT((GeneralReturnType)ERR_CODE_BMU_NO_OVER == err_code)

    err_code = Err_UpdateHwErrDetailFromCan(0, ERR_HW_DETECT_CELLV, 0x5A5A);
    __ASSERT((GeneralReturnType)ERR_CODE_BMU_NO_ZERO == err_code)
    for (i=1; i<MAX_PACK_NUM; i++)
    {
        for (j=0; j<ERR_NUM_HW_DETAIL; j++)
        {
            err_code = Err_UpdateHwErrDetailFromCan(i, (eErr_HwErrIndexType)j, 0x5A5A);
            __ASSERT(ERR_OK == err_code)
        }
        err_code = Err_UpdateHwErrDetailFromCan(i, (eErr_HwErrIndexType)ERR_NUM_HW_DETAIL, 0x5A5A);
        __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)
    }
    err_code = Err_UpdateHwErrDetailFromCan(MAX_PACK_NUM, ERR_HW_DETECT_CELLV, 0x5A5A);
    __ASSERT((GeneralReturnType)ERR_CODE_BMU_NO_OVER == err_code)

    for(i=0; i<ERR_NUM_HW; i++)
    {
        err_code = Err_UpdateHwErrLevel((eErr_HwErrIndexType)i, (eErr_LevelType)(i%4));//level 0123
        __ASSERT(ERR_OK == err_code)
        err_code = Err_UpdateHwErrDetail((eErr_HwErrIndexType)i, 0x5A5A);
        __ASSERT(ERR_OK == err_code)
    }
    err_code = Err_UpdateHwErrLevel(ERR_HW_RSV_FOR_7_6, (eErr_LevelType)4);
    __ASSERT((GeneralReturnType)ERR_CODE_LEVEL_OVER == err_code)
    err_code = Err_UpdateHwErrLevel((eErr_HwErrIndexType)ERR_NUM_HW, ERR_LEVEL_NORMAL);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)
    err_code = Err_UpdateHwErrDetail(ERR_HW_RSV_FOR_7_6, 0x5A5A);
    __ASSERT(ERR_OK == err_code)
    err_code = Err_UpdateHwErrDetail((eErr_HwErrIndexType)ERR_NUM_HW, 0x5A5A);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)

#ifdef __DEBUG_ERR_EXT_FUNC

    for(i=0; i<ERR_NUM_HW; i++)
    {
        err_code = Err_UpdateHwErrLevelAllPack((eErr_HwErrIndexType)i, (eErr_LevelType)(3 - i%4));//level 0123
        __ASSERT(ERR_OK == err_code)
        err_code = Err_UpdateHwErrDetailAllPack((eErr_HwErrIndexType)i, 0x5A5A);
        __ASSERT(ERR_OK == err_code)
    }
    err_code = Err_UpdateHwErrLevelAllPack(ERR_HW_RSV_FOR_7_6, (eErr_LevelType)4);
    __ASSERT((GeneralReturnType)ERR_CODE_LEVEL_OVER == err_code)
    err_code = Err_UpdateHwErrLevelAllPack((eErr_HwErrIndexType)ERR_NUM_HW, ERR_LEVEL_NORMAL);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)
    err_code = Err_UpdateHwErrDetailAllPack(ERR_HW_RSV_FOR_7_6, 0x5A5A);
    __ASSERT(ERR_OK == err_code)
    err_code = Err_UpdateHwErrDetailAllPack((eErr_HwErrIndexType)ERR_NUM_HW, 0x5A5A);
    __ASSERT((GeneralReturnType)ERR_CODE_INDEX_OVER == err_code)

    Err_UpdateHwAllErrLevelAllPack(&data);

#endif//#ifdef __DEBUG_ERR_EXT_FUNC
}

void Err_MainDebug(void)
{
    eErr_LevelType err_level = ERR_LEVEL_INVALD;
    sErr_ByteNType data;
    Err_HwErrDetailType detail;
    uByte i, j;

#ifdef __DEBUG_ERR_INT_FUNC

    Err_Main();

#endif

#ifdef __DEBUG_ERR_EXT_FUNC

    for(i=0; i<ERR_NUM_BAT; i++)
    {
        err_level = Err_GetBatErrLevel((eErr_BatErrIndexType)i);
        __ASSERT(err_level == (eErr_LevelType)(i%4))
    }
    err_level = Err_GetBatErrLevel((eErr_BatErrIndexType)ERR_NUM_BAT);
    __ASSERT(ERR_LEVEL_INVALD == err_level)
    data = Err_GetBatAllErrLevel();
    for(i=0; i<8; i++)
    {
        __ASSERT(0xE4 == (data.By[i]))
        data.By[i] = 0;
    }

    for(i=0; i<ERR_NUM_BATSYS; i++)
    {
        err_level = Err_GetBatSysErrLevel((eErr_BatSysErrIndexType)i);
        __ASSERT(err_level == (eErr_LevelType)(i%4))
    }
    err_level = Err_GetBatSysErrLevel((eErr_BatSysErrIndexType)ERR_NUM_BATSYS);
    __ASSERT(ERR_LEVEL_INVALD == err_level)
    data = Err_GetBatSysAllErrLevel(0);
    for(i=0; i<8; i++)
    {
        __ASSERT(0xE4 == data.By[i])
        data.By[i] = 0;
    }
    data = Err_GetBatSysAllErrLevel(1);
    for(i=0; i<8; i++)
    {
        __ASSERT(0xE4 == data.By[i])
        data.By[i] = 0;
    }
    data = Err_GetBatSysAllErrLevel(2);
    for(i=0; i<8; i++)
    {
        __ASSERT(ERR_LEVEL_INVALD == (eErr_LevelType)(data.By[i]))
        data.By[i] = 0;
    }

    for(i=0; i<MAX_PACK_NUM; i++)
    {
        for(j=0; j<ERR_NUM_HW; j++)
        {
            err_level = Err_GetHwErrLevelSinglePack(i, (eErr_HwErrIndexType)j);
            __ASSERT(err_level == (eErr_LevelType)(j%4))
            err_level = Err_GetHwErrLevelSinglePack(i, (eErr_HwErrIndexType)ERR_NUM_HW);
            __ASSERT(ERR_LEVEL_INVALD == err_level)

            detail = Err_GetHwErrDetailSinglePack(i, (eErr_HwErrIndexType)j);
            __ASSERT(detail == 0x5A5A)
            detail = Err_GetHwErrDetailSinglePack(i, (eErr_HwErrIndexType)ERR_NUM_HW_DETAIL);
            __ASSERT(ERR_HW_DETAIL_INVALID == detail)
        }
        data = Err_GetHwAllErrLevelSinglePack(i);
        for(j=0; j<8; j++)
        {
            __ASSERT(0xE4 == data.By[j])
            data.By[j] = 0;
        }
    }

#endif//#ifdef __DEBUG_ERR_EXT_FUNC

    for(i=0; i<ERR_NUM_HW; i++)
    {
        err_level = Err_GetHwErrLevel((eErr_HwErrIndexType)i);
        __ASSERT(err_level == (eErr_LevelType)(i%4))

        detail = Err_GetHwErrDetail((eErr_HwErrIndexType)i);
        __ASSERT(detail == 0x5A5A)
    }
    err_level = Err_GetHwErrLevel((eErr_HwErrIndexType)ERR_NUM_HW);
    __ASSERT(ERR_LEVEL_INVALD == err_level)
    detail = Err_GetHwErrDetail((eErr_HwErrIndexType)ERR_NUM_HW_DETAIL);
    __ASSERT(ERR_HW_DETAIL_INVALID == detail)
    data = Err_GetHwAllErrLevel();
    for(i=0; i<8; i++)
    {
        __ASSERT(0xE4 == data.By[i])
    }

#ifdef __DEBUG_ERR_EXT_FUNC

    err_level = Err_GetErrLevel(ERR_BAT);
    __ASSERT(ERR_LEVEL_THREE == err_level)
    err_level = Err_GetErrLevel(ERR_BATSYS);
    __ASSERT(ERR_LEVEL_THREE == err_level)
    err_level = Err_GetErrLevel(ERR_HW);
    __ASSERT(ERR_LEVEL_THREE == err_level)
    err_level = Err_GetErrLevel(ERR_ALL);
    __ASSERT(ERR_LEVEL_THREE == err_level)

#endif//#ifdef __DEBUG_ERR_EXT_FUNC
}

#endif//#ifdef __DEBUG_TEST_ERRORALARM
// end of debug interface function


#include "Dem.h"
#include "Dem_IntErrId.h"
#include "Dem_Types.h"
void Updata_DTCStatus(void) 
{
    #if 1   
    if(Err_BatErrCode.sBit.SocLow)     /* ÓÐSOC¹ÊÕÏ */
    {
      Dem_SetEventStatus(soc_too_low_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else          /* Ã»ÓÐSOC¹ÊÕÏ */
    {
      Dem_SetEventStatus(soc_too_low_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CurHighChg == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(charge_current_too_large_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(charge_current_too_large_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CurHighChg == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(charge_current_too_large_level_2_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(charge_current_too_large_level_2_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CurHighDch == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(discharge_current_too_large_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(discharge_current_too_large_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CurHighDch == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(discharge_current_too_large_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(discharge_current_too_large_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVHigh == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVHigh == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_2_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_2_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVHigh == ERR_LEVEL_THREE)
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_3_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_high_level_3_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVLow == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVLow == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_2_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_2_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.CellVLow == ERR_LEVEL_THREE)
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_3_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(cell_voltage_too_low_level_3_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.SumVHigh == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(pack_voltage_too_high_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(pack_voltage_too_high_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.SumVHigh == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(pack_voltage_too_high_level_2_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(pack_voltage_too_high_level_2_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.SumVLow == ERR_LEVEL_ONE)
    {
      Dem_SetEventStatus(pack_voltage_too_low_level_1_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(pack_voltage_too_low_level_1_ID,DEM_EVENT_STATUS_PASSED);
    }
    
    if(Err_BatErrCode.sBit.SumVLow == ERR_LEVEL_TWO)
    {
      Dem_SetEventStatus(pack_voltage_too_low_level_2_ID,DEM_EVENT_STATUS_FAILED);
    } 
    else
    {
      Dem_SetEventStatus(pack_voltage_too_low_level_2_ID,DEM_EVENT_STATUS_PASSED);
    }
    #endif
    
}

#pragma DATA_SEG DEFAULT// end of PAGED_RAM