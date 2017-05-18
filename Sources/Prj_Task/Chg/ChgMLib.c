// ChgM.c
#include "ChargeSt.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "AppRelay.h"
#include "ErrorAlarm.h"
#include "GbChgM-B.h"
#include "ChgM-B.h"

#define RELAY_OFF              0
#define RELAY_ON               1
#define CHGM_FLAG_DISABLE      0
#define CHGM_FLAG_ENABLE       1


extern void ChgM_WaitBmsReady(void);
extern void ChgM_CalcChgStartTime(void);
extern void ChgM_DetChgCanSt(void);
extern void ChgM_ChgRemainTimeCalc(uWord *RemainTime);
extern uByte ChgM_HardWareErrJudge(void);
extern uByte ChgM_IsoErrJudge(void);
extern uByte ChgM_CellvHighJudge(void);
extern uByte ChgM_SumvHighJudge(void) ;
extern uByte ChgM_CurHighJudge(void) ;
extern uByte ChgM_TemperHighJudge(void) ;
extern uByte ChgM_TemperLowJudge(void);
extern uByte ChgM_CellVLowJudge(void);
extern uByte ChgM_SumVLowJudge(void);
extern uByte ChgM_RlyErrJudge(void);
extern uByte ChgM_ErrAccept(void);


extern void ChgM_RelayCtrl(void);
extern void ChgM_GetCurByTemper(uLWord *Cur, uByte MinT, uByte MaxT, const sChg_CurTableType * ChgTable);
extern void ChgM_ConverCurUint(uWord *Cur, uWord BatCap);
extern void ChgM_CompareAndGetMinCur(uWord *Cur, uWord CurMaxT, uWord CurMinT);
extern void ChgM_ModifyCurAfterLookUpTable(uWord *Cur);
extern void ChgM_ModifyPmtChgCurByStep(uWord Cur, uWord Step);
extern void ChgM_ModifyCurByChargerOutPutCur(uWord *PmtChgCur,uWord *MinChgCur, uWord MaxOutPutCur,uWord MinOutCur);
extern void ChgM_ModifyPmtCurByActualCur(uWord *PmtChgCur, uWord Cur);
extern void ChgM_CurStepDownCalc(uWord CurStep);
extern void ChgM_StageCvToReadyEnd(void);


/* wait bms data ready after bms power on */
void ChgM_WaitBmsReady(void)
{
    /* counter overflowe, bms ready, 2.5S */
    if(ChgM_Ctrl.Cnt.WaitReady >= CHGM_CNT_WAIT_READY)
    {
        ChgM_Ctrl.Flag.BmsReady = CHGM_FLAG_ENABLE;
    }
    else
    {
        ChgM_Ctrl.Cnt.WaitReady++;
    }
}

/* current more than 2A ,the counter will start */
void ChgM_CalcChgStartTime(void)
{
    /* detect charge start time  */
    if(ChgM_Ctrl.Data.Cur < CHGM_DET_START_CHG_CUR)    /* > 2A */
    {
        ChgM_Ctrl.Cnt.ChgStart++;
        if(ChgM_Ctrl.Cnt.ChgStart >= CHGM_CNT_START_TIME)  /* 5min */
        {
            ChgM_Ctrl.Cnt.ChgStart = CHGM_CNT_START_TIME;
        }
    }
    else
    {
        ChgM_Ctrl.Cnt.ChgStart = 0;
    }
}

/* detect whether charge CAN is connect */
void ChgM_DetChgCanSt(void)
{
    if(ChgM_Ctrl.Data.Life == ChgM_Ctrl.Data.LifeBak)
    {
        /* life have no change and last 10s, charger no connect */
        ChgM_Ctrl.Cnt.ChgCanErr++;
        if(ChgM_Ctrl.Cnt.ChgCanErr > CHGM_CNT_CAN_ERR)
        {
            ChgM_Ctrl.Cnt.ChgCanErr = CHGM_CNT_CAN_ERR;
            ChgM_Ctrl.Flag.ChgRx = CHGM_FLAG_DISABLE;
        }
    }
    else
    {
        /* the charger have been connected */
        ChgM_Ctrl.Data.LifeBak = ChgM_Ctrl.Data.Life;
        ChgM_Ctrl.Cnt.ChgCanErr = 0;
        ChgM_Ctrl.Flag.ChgRx = CHGM_FLAG_ENABLE;
    }
}

/* detect charge remain time  */
void ChgM_ChgRemainTimeCalc(uWord *RemainTime)
{
    uLWord llwtmp;
    uLWord llwtmp_cc;
    uLWord llwtmp_cv;
    uWord  CvCur;

    if(CHGM_FLAG_ENABLE == Chg_GetAcLinkSt())
    {
        /* for ac charge calc remain time */
        CvCur = 80;     
    }
    else
    {
        /* for Dc charge calc remain time */
        CvCur = ChgM_Ctrl.Data.RatedCap / 10; //0.1C 
        CvCur += (OFFSET_CUR - ChgM_Ctrl.Data.Cur); 
        CvCur /= 2;
    }

    if((CHG_ST_ON == ChgM_Ctrl.Data.ChgSt) && (ChgM_Ctrl.Data.Cur < OFFSET_CUR))
    {
        if(CHG_STAGE_CC_MODE == ChgM_Ctrl.Data.ChgStage)
        {
            llwtmp =  ChgM_Ctrl.Data.RatedCap - ChgM_Ctrl.Data.RemainCap;  /* 0.1AH */
            if(llwtmp > (ChgM_Ctrl.Data.RatedCap / 50)) /* >2% */
            {
                /* calc CC stage need charge capacity and need time */
                llwtmp_cc = llwtmp - (ChgM_Ctrl.Data.RatedCap / 50); 
                llwtmp_cc /= (OFFSET_CUR - ChgM_Ctrl.Data.Cur); /*  miniute  */
                
                /* calc CV stage need charge capacity and need time */
                llwtmp_cv = (ChgM_Ctrl.Data.RatedCap / 50);
                llwtmp_cv /= CvCur;
                
                /* clac time of CC + CV */
                llwtmp = llwtmp_cc + llwtmp_cv;
                llwtmp *= 60;
            }
            else
            {
                llwtmp_cc = 0;
                llwtmp_cv = llwtmp / CvCur;
                /* clac time of CC + CV */
                llwtmp = llwtmp_cc + llwtmp_cv;
                llwtmp *= 60;
            }
        }
        else if(CHG_STAGE_CV_MODE == ChgM_Ctrl.Data.ChgStage)
        {
            llwtmp_cv = ChgM_Ctrl.Data.RatedCap  - ChgM_Ctrl.Data.RemainCap;  /* 0.1AH */
            llwtmp_cv *= 60;
            llwtmp = llwtmp_cv / CvCur;
        }
        else
        {
            llwtmp = 0;
        }

        if(llwtmp > 600)
        {
            llwtmp = 600;  /* max time is 10hour */
        }
    }
    else
    {
        llwtmp = 0;
    }

    *RemainTime  = (uWord)llwtmp;
}

uByte ChgM_HardWareErrJudge(void)
{
    uByte HdErr;
    uWord BmsHdErr;

    HdErr = 0;
    BmsHdErr = 0;

    /* charge disable when BMS has hardware error or CAN error  */
    BmsHdErr = (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_CELLV);
    BmsHdErr = (BmsHdErr << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_TEMPER);
    BmsHdErr = (BmsHdErr << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_CUR);
    BmsHdErr = (BmsHdErr << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_ISO);
    BmsHdErr = (BmsHdErr << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_CAN4);

    if(BmsHdErr != 0)
    {
        HdErr = 1;
    }

    return HdErr;
}

uByte ChgM_IsoErrJudge(void)
{
    uByte IsoErr;

    IsoErr = 0;
    if(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR))
    {
        IsoErr = 1;
    }
    return IsoErr;
}

uByte ChgM_CellvHighJudge(void)        /* Cell voltage more than 3.8V */
{
    uByte CellVHigh;
    CellVHigh = 0;
    if((CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady) && (ChgM_Ctrl.Data.MaxV < 0xFFF0))
    {
        if((ChgM_Ctrl.Data.MaxV >= CHGM_ERR_MAX_CELLV) || (Err_GetBatErrLevel(ERR_BAT_CELLV_HIGH) >= 3))
        {
            CellVHigh = 1;
        }
    }
    return CellVHigh;
}

uByte ChgM_SumvHighJudge(void)     /* Sum voltage more than 390V */
{
    uByte SumVHigh;
    SumVHigh = 0;

    if((ChgM_Ctrl.Data.SumV >= CHGM_ERR_MAX_SUMV) || (Err_GetBatErrLevel(ERR_BAT_SUMV_HIGH) >= 3))
    {
        SumVHigh = 1;
    }

    return SumVHigh;
}

uByte ChgM_CurHighJudge(void)
{
    uByte CurHigh;
    uWord wtmp;

    CurHigh = 0;
    /* charge disable when charge current > 1.5 * permit current  */
    /* only judge when permit current > Thrhd  */
    if(ChgM_Ctrl.Data.PmtChgCur > CHGM_JUDGE_CUR)
    {
        wtmp = OFFSET_CUR - (ChgM_Ctrl.Data.PmtChgCur * 3 / 2);
        if(ChgM_Ctrl.Data.Cur  < wtmp)
        {
            CurHigh = 1;
        }
    }
    else
    {
        /* Permit cur <= 30A, charge current more than 45A */
        if(ChgM_Ctrl.Data.Cur < CHGM_ERR_MAX_CUR)
        {
            CurHigh = 1;
        }
    }

    return CurHigh;
}

uByte ChgM_TemperHighJudge(void)
{
    uByte TemperHigh;
    TemperHigh = 0;

    if((CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady) && (ChgM_Ctrl.Data.MaxT < 0xF0))
    {
        /* max temperature more than config parameter */
        if((ChgM_Ctrl.Data.MaxT >= Chg_GetPermitMaxT()) || (ChgM_Ctrl.Data.MaxT >= CHGM_ERR_MAX_TEMPER))
        {
            TemperHigh = 1;
        }
    }
    return TemperHigh;
}

uByte ChgM_TemperLowJudge(void)
{
    uByte TemperLow;
    TemperLow = 0;

    if((CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady) && (ChgM_Ctrl.Data.MinT < 0xF0))
    {
        /* min temperature less than config parameter */
        if((ChgM_Ctrl.Data.MinT  <= Chg_GetPermitMinT()) || (ChgM_Ctrl.Data.MinT <= CHGM_ERR_MIN_TEMPER))
        {
            TemperLow = 1;
        }
    }

    return TemperLow;
}

uByte ChgM_CellVLowJudge(void)
{
    uByte CellVLow;
    CellVLow = 0;
    if((CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady) && (ChgM_Ctrl.Data.MinV < 0xFFF0))
    {
        /* cell voltage less than config parameter */
        if((ChgM_Ctrl.Data.MinV < Chg_GetPermitMinCellV()) || (ChgM_Ctrl.Data.MinV < CHGM_ERR_MIN_CELLV))
        {
            CellVLow = 1;
        }
    }

    return CellVLow;
}

uByte ChgM_SumVLowJudge(void)
{
    uByte SumVLow;
    SumVLow = 0;
    if(CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady)
    {
        /* Sum voltage less than 100 V */
        if(ChgM_Ctrl.Data.SumV < CHGM_ERR_MIN_SUMV)
        {
            SumVLow = 1;
        }
    }

    return SumVLow;
}

uByte ChgM_RlyErrJudge(void)
{
    uByte RlyErr;
    RlyErr = 0;

    /*Relay Err, need detect */
    if(RELAY_ERROR == BatSys_GetChgRlySt())
    {
        RlyErr = 1;
    }

    return RlyErr;
}

/*Fault Recognition */
uByte ChgM_ErrAccept(void)
{
    uByte rslt;

    rslt = CHGM_FLAG_DISABLE;
    if(ChgM_Ctrl.ChgEndReason.LWd != 0)  /* have Err and last for 2S */
    {
        ChgM_Ctrl.Cnt.ChgErr++;
    }
    else
    {
        ChgM_Ctrl.Cnt.ChgErr = 0;
    }

    if(ChgM_Ctrl.Cnt.ChgErr >= CHGM_CNT_CHG_ERR)
    {
        ChgM_Ctrl.Cnt.ChgErr = CHGM_CNT_CHG_ERR;
        rslt = CHGM_FLAG_ENABLE;

#ifdef ECU_M_ENABLE_GBCHGIF
        GbChgM_EndReasonDeal();   /* Charge End reaseon deal for Gb charge */
#endif
    }

    return rslt;
}


/* charge relay control */
void ChgM_RelayCtrl(void)
{
#ifdef ECU_M_ENABLE_GBCHGIF
    if((CHGM_FLAG_ENABLE == ChgM_Ctrl.Data.ChgEnable) && (CHGM_FLAG_ENABLE == ChgM_Ctrl.Data.LinkSt) && (GbChgM_Ctrl.ChgStep >= ChgStep3) && (GbChgM_Ctrl.ChgStep <= ChgStep5))
    {
        /* Enable to close relay */
        ChgM_Ctrl.Cnt.ChgRlyOff = 0;
        ChgM_Ctrl.Flag.ChgRlyCtrl = RELAY_ON;
    }
    else
    {
        /* Enable to open relay */
        if(ChgM_Ctrl.Cnt.ChgRlyOff >= CHGM_CNT_RLY_OFF)
        {
            ChgM_Ctrl.Cnt.ChgRlyOff = CHGM_CNT_RLY_OFF;
            ChgM_Ctrl.Flag.ChgRlyCtrl = RELAY_OFF;
        }
        else
        {
            ChgM_Ctrl.Cnt.ChgRlyOff++;
        }
    }
#else
    if( (CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.ChgRx) && (CHGM_FLAG_ENABLE == ChgM_Ctrl.Data.ChgEnable) )  /* need more condition */
    {
        /* Enable to close relay */
        ChgM_Ctrl.Cnt.ChgRlyOff = 0;
        if(ChgM_Ctrl.Cnt.ChgRlyOn >= CHGM_CNT_RLY_ON) // 1S
        {
            ChgM_Ctrl.Cnt.ChgRlyOn = CHGM_CNT_RLY_ON;
            ChgM_Ctrl.Flag.ChgRlyCtrl = RELAY_ON;
        }
        else
        {
            ChgM_Ctrl.Cnt.ChgRlyOn++;
        }
    }
    else
    {
        /* Enable to open relay */
        ChgM_Ctrl.Cnt.ChgRlyOn = 0;
        if(ChgM_Ctrl.Cnt.ChgRlyOff >= CHGM_CNT_RLY_OFF) // 1S
        {
            ChgM_Ctrl.Cnt.ChgRlyOff = CHGM_CNT_RLY_OFF;
            ChgM_Ctrl.Flag.ChgRlyCtrl = RELAY_OFF;
        }
        else
        {
            ChgM_Ctrl.Cnt.ChgRlyOff++;
        }
    }
#endif /* end of charge relay control */
}

/* Look up the temper-Cur tabel */
void ChgM_GetCurByTemper(uLWord *Cur, uByte MinT, uByte MaxT, const sChg_CurTableType * ChgTable)
{
    uByte  i;
    static uWord CurMinT;
    static uWord CurMaxT;

#ifdef CHG_CALC_CUR_MODE1
    /* calc charge current by lby_min_t  */
    for (i = 0; i < T_SEC; i++)
    {
        if (MinT < ChgTable[i].t)
        {
            CurMinT = ChgTable[i].Cur;
            break;
        }
    }
    if(T_SEC == i)
    {
        CurMinT = 0;
    }

    /* calc charge current by lby_max_t  */
    for (i = 0; i < T_SEC; i++)
    {
        if (MaxT < ChgTable[i].t)
        {
            CurMaxT = ChgTable[i].Cur;
            break;
        }
    }
    if(T_SEC == i)
    {
        CurMaxT = 0;
    }
 #endif   
    
 #ifdef CHG_CALC_CUR_MODE2
    /* init charge current by temper only once */
    if(ChgM_Ctrl.Flag.InitChgCur == CHGM_FLAG_FAIL)
    {
        for (i = 0; i < T_SEC; i++)
        {
            if (MinT < ChgTable[i].t)
            {
                CurMinT = ChgTable[i].Cur;
                break;
            }
        }
        if(T_SEC == i)
        {
            CurMinT = 0;
        }

        for (i = 0; i < T_SEC; i++)
        {
            if (MaxT < ChgTable[i].t)
            {
                CurMaxT = ChgTable[i].Cur;
                break;
            }
        }
        if(T_SEC == i)
        {
            CurMaxT = 0;
        }

        ChgM_Ctrl.Flag.InitChgCur = CHGM_FLAG_OK;
    }

    /* calc charge current by min_t */
    for(i = 0; i < (T_SEC - 1); i++)
    {
        if( (MinT > ChgTable[i].t)
                && (MinT < ChgTable[i + 1].t) )
        {
            CurMinT = ChgTable[i + 1].Cur;
            break;
        }
    }

    if( (MinT < ChgTable[0].t)
            || (MinT >= ChgTable[T_SEC - 1].t) )
    {
        CurMinT = 0;
    }

    //calc charge current by max_t
    for(i = 0; i < (T_SEC - 1); i++)
    {
        if( (MaxT > ChgTable[i].t)
                && (MaxT < ChgTable[i + 1].t) )
        {
            CurMaxT = ChgTable[i + 1].Cur;
            break;
        }
    }

    if((MaxT < ChgTable[0].t)
            || (MaxT >= ChgTable[T_SEC - 1].t) )
    {
        CurMaxT = 0;
    }

#endif 
    

    *Cur = CurMinT;  /*0.01C unit */
    *(++Cur) = CurMaxT;  /*0.01C unit */
}

/* convert current unit 0.01C to 0.1A */
void ChgM_ConverCurUint(uWord *Cur, uWord BatCap)
{
    uLWord Current;
    Current =  (uLWord)BatCap * (*Cur);
    (*Cur) = (uWord)(Current / 100);	/* 0.1A unit  */
}

/*Compare Current */
void ChgM_CompareAndGetMinCur(uWord *Cur, uWord CurMaxT, uWord CurMinT)
{
    (*Cur) = CurMaxT;
    if((*Cur) > CurMinT)
    {
        (*Cur) = CurMinT;
    }
}

/* Modify Current after lookup table */
void ChgM_ModifyCurAfterLookUpTable(uWord *Cur)
{
    if(ChgM_Ctrl.Data.MinV < MINV_DOWN_CC_CUR)
    {
        /* max pmt charge current cut half down when cell volt is very low */
        (*Cur) /= 2;
    }

    if(ChgM_Ctrl.Cnt.ChgStart < CHGM_CNT_CHG_3MIN) /* no last for 3 minute */
    {
        if(ChgM_Ctrl.Data.MaxV > MAXV_DOWN_CC_CUR)
        {
            /* max pmt charge current cut half down when cell volt is very  high */
            ChgM_Ctrl.Flag.DownCcCur = CHGM_FLAG_ENABLE;
        }
    }
    if(CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.DownCcCur)
    {
        (*Cur) /= 2;
    }

    /* min pmt charge current is 0.1C when battery T & V is ok */
    if ( (*Cur > 0) && (*Cur < (ChgM_Ctrl.Data.RatedCap / 10)) )
    {
        *Cur = ChgM_Ctrl.Data.RatedCap / 10;
    }
}

/* Smooth Current every 3S */
void ChgM_ModifyPmtChgCurByStep(uWord Cur,uWord Step)
{
    if(Cur == ChgM_Ctrl.Data.PmtChgCur)  /* pmt charge current don't need to modify */
    {
        ChgM_Ctrl.Cnt.CalcCur = 0;
    }
    else /* pmt charge current need to modify */
    {
        ChgM_Ctrl.Cnt.CalcCur++;
    }

    if(ChgM_Ctrl.Cnt.CalcCur >= CHGM_CNT_CALC_CUR)    /* wait for 3S */
    {
        /* pmt charge current need to modify and last for 3S */
        ChgM_Ctrl.Cnt.CalcCur = 0;

        /* modify pmt charge current of  charger */
        if(ChgM_Ctrl.Data.PmtChgCur < Cur)
        {
            if(ChgM_Ctrl.Data.PmtChgCur + Step < Cur)  /* step up   */
            {
                ChgM_Ctrl.Data.PmtChgCur += Step;
            }
            else
            {
                ChgM_Ctrl.Data.PmtChgCur = Cur;
            }

        }
        else if((ChgM_Ctrl.Data.PmtChgCur) > Cur)
        {
            if(ChgM_Ctrl.Data.PmtChgCur > Cur + Step)  /* step down  */
            {
                ChgM_Ctrl.Data.PmtChgCur -= Step;
            }
            else
            {
                ChgM_Ctrl.Data.PmtChgCur = Cur;
            }
        }
        else
        {

        }
    }
}

/* Modify charge current according to the charger */
void ChgM_ModifyCurByChargerOutPutCur(uWord *PmtChgCur,uWord *MinChgCur, uWord MaxOutPutCur,uWord MinOutCur)
{
    if(*PmtChgCur > MaxOutPutCur)
    {
        *PmtChgCur = MaxOutPutCur;
    }

    if(*MinChgCur < MinOutCur)
    {
        *MinChgCur = MinOutCur;
    }
}

/* modify pmt charge current by actual charge current */
void ChgM_ModifyPmtCurByActualCur(uWord *PmtChgCur, uWord Cur)
{
    uWord Current;
    Current = Cur;
    if(Current < OFFSET_CUR)
    {
        Current = OFFSET_CUR - Current;
    }
    else
    {
        Current = 0;
    }
    if(Current < (*PmtChgCur) )
    {
        *PmtChgCur = Current;
    }
}

/*  Step down Current every 2S */
void ChgM_CurStepDownCalc(uWord CurStep)
{
    if(ChgM_Ctrl.Cnt.StepCur >= CHGM_CNT_STEP_CUR)  /* wait for 2S */
    {
        ChgM_Ctrl.Cnt.StepCur = 0;

        if( ChgM_Ctrl.Cnt.ChgStart < CHGM_CNT_CHG_3MIN) /* charge time no more than 3 minute */
        {
            /* if permit current larger than 0.2C,then permit current cut down to 0.2C  */
            if( ChgM_Ctrl.Data.PmtChgCur > (ChgM_Ctrl.Data.RatedCap / 5) )
            {
                ChgM_Ctrl.Data.PmtChgCur = (ChgM_Ctrl.Data.RatedCap / 5);
            }
        }

        /* if permit current larger than 0.3C,then permit current cut down to 0.3C  */
        if( ChgM_Ctrl.Data.PmtChgCur > (ChgM_Ctrl.Data.RatedCap * 3 / 10) )
        {
            ChgM_Ctrl.Data.PmtChgCur = (ChgM_Ctrl.Data.RatedCap * 3 / 10);
        }
        else
        {
            /* Smooth Permit charge current */
            if (ChgM_Ctrl.Data.PmtChgCur > (ChgM_Ctrl.Data.MinChgCur + CurStep) )
            {
                ChgM_Ctrl.Data.PmtChgCur -= CurStep;
            }
            else
            {
                if(ChgM_Ctrl.Data.PmtChgCur >= ChgM_Ctrl.Data.MinChgCur)
                {
                    ChgM_Ctrl.Data.PmtChgCur = ChgM_Ctrl.Data.MinChgCur;
                }
            }
        }
    }
    else
    {
        ChgM_Ctrl.Cnt.StepCur++;
    }
}

/* Cv stage --> Ready to end */
void ChgM_StageCvToReadyEnd(void)
{
    if(ChgM_Ctrl.Data.PmtChgCur <= ChgM_Ctrl.Data.MinChgCur)
    {
        ChgM_Ctrl.Data.ChgStage = CHG_STAGE_READY_TO_END;

#ifdef ECU_M_ENABLE_GBCHGIF
        GbChgM_StopReasonJudge();
#endif

    }
}