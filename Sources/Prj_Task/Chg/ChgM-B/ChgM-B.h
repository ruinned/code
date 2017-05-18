// ChgM-B.h
#ifndef CHGM_B_H
#define CHGM_B_H

#include "GeneralTypeDef.h"
#include "ChgM_Macro.h"


typedef struct
{
    uWord WaitReady;   /* count for waiting bms data ready */
    uWord StepCur;     /* count for step down charge current */
    uWord CalcCur;     /* count for calc pmt charge current */
    uWord WaitStop;    /* count for wait charger stop charge */
    uWord ChgErr;      /* count for charge error */
    uWord ChgStart;    /* count fot calc time of charge start*/
    uWord ChgDownCur;  /* count for wait charger down current */
    uWord ChgRlyOn;    /* count for control charge relay on */
    uWord ChgRlyOff;   /* count for control charge relay off */
    uWord ChgCanErr;   /* count for detect charge CAN error */
    uWord ChgRlyCtrl ;  /* count for control relay off */

} sChgM_CntType;

typedef struct
{

    uByte BatSt;       /* battery status flag */
    uByte Life;
    uByte LifeBak;     /*  charger life bakup */

    uByte MinT;
    uByte MaxT;
    uWord MinV;
    uWord MaxV;
    uWord RatedCap;     /* 0.1AH */
    uWord RemainCap;
    uByte MaterialType;

    uWord PmtChgCur;
    uByte ChgEnable;

    uWord Cur;
    uWord SumV;

    uByte ChgSt;
    uByte ChgStage;
    uByte LinkSt;

    uWord MinChgCur;
    uWord MaxOutPutCur;
    uWord MinOutCur;

} sChgM_DataType;

typedef struct
{
    uByte BmsReady;          /* BMS data ready */
    uByte InitChgCur;        /* flag of init pmt charge current by temper */
    uByte ChgType ;          /* flag of charger type, such as onboard charger */
    uByte ChgRx;             /* flag of charger CAN communiction */
    uByte DownCcCur;         /* flag of current cut half down when voltage is very high */

    uByte ChgRlyCtrl;
    uByte ModifyCur ;
    uByte RlyErr;
} sChgM_FlagType;


typedef union
{
    uLWord LWd;
    struct
    {
        uByte Rsv           : 8;

        uByte SumVoltHigh   : 1;
        uByte CellVoltLow   : 1;
        uByte CellVoltHigh  : 1;
        uByte IsoErr  : 1;

        uByte HdErr         : 1;
        uByte ReachSoc      : 1;
        uByte ReachSumV     : 1;
        uByte ReachCellV    : 1;

        uByte OutputConFault : 1;
        uByte RlyErr         : 1;
        uByte ChgFull        : 1;
        uByte BatNotMatch    : 1;

        uByte CurHigh        : 1;
        uByte TemperLow      : 1;
        uByte TemperHigh     : 1;
        uByte SumVoltLow     : 1;

        uByte Rsv1            : 3;
        uByte OtherFault      : 1;

        uByte SmokeAlarmFault : 1;
        uByte AuxPowerFault   : 1;
        uByte RxDataOverTime  : 1;
        uByte ChgConFault     : 1;

    } Bit;

} uChgM_EndReasonType;

typedef struct
{
    sChgM_CntType       Cnt;
    sChgM_DataType      Data;
    sChgM_FlagType      Flag;
    uChgM_EndReasonType ChgEndReason;
} sChgM_CtrlType;

extern sChgM_CtrlType ChgM_Ctrl;

extern uWord ChgM_GetChgStartCnt(void);

extern void ChgM_ModuleInit(void);
extern void ChgM_ModuleCountCbk(void);
extern void ChgM_ModuleCbk(void);
extern uByte ChgM_GetRelayReady(void);

#endif /* CHGM_B_H */
