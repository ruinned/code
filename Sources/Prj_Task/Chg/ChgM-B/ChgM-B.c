// ChgM-B.c
#include "ChgM-B.h"
#include "GbChgM-B.h"
#include "ChargeSt.h"
#include "BatterySt.h"
#include "ErrorAlarm.h"
#include "ChgCan.h"
#include "SysConf.h"
#include "EE.h"
#include "CarCan.h"
#include "AppRelay.h"

#define GAC_TEST


#define CHGM_FLAG_ENABLE    1U
#define CHGM_FLAG_DISABLE   0U

#define RELAY_ON  1U
#define RELAY_OFF 0U

#define BAT_ST_CELLV_HIGH   0x01U    /* error of module voltage is high */
#define BAT_ST_CELLV_LOW    0x02U    /* error of module voltage is low */
#define BAT_ST_SOC_HIGH     0x04U    /* error of soc is high */
#define BAT_ST_SOC_LOW      0x08U    /* error of soc is low */
#define BAT_ST_CUR_HIGH     0x10U    /* error of charge current */
#define BAT_ST_T_HIGH       0x40U    /* error of temperature is high */

typedef struct
{
    uByte GeneralChgCnt;
} sChgM_TmCntType;

sChgM_TmCntType ChgM_TmCnt;
sChgM_CtrlType ChgM_Ctrl;

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
extern void ChgM_ModifyPmtChgCurByStep(uWord Cur,uWord Step);
extern void ChgM_ModifyCurByChargerOutPutCur(uWord *PmtChgCur,uWord *MinChgCur, uWord MaxOutPutCur,uWord MinOutCur);
extern void ChgM_ModifyPmtCurByActualCur(uWord *PmtChgCur, uWord Cur);
extern void ChgM_CurStepDownCalc(uWord CurStep);
extern void ChgM_StageCvToReadyEnd(void);

void ChgM_MainFun(void);
void ChgM_ChgCtrl(uByte ChgSt, uByte ChgStage);
void ChgM_ChgErrJudge(void);
void ChgM_GetDcMaxPmtChgCur(void);
void ChgM_GetAcMaxPmtChgCur(void);
void ChgM_GetBmsInfo(void);
void ChgM_SetBmsInfo(void);


uWord ChgM_GetChgStartCnt(void)
{
    return ChgM_Ctrl.Cnt.ChgStart;
}

/*=======================================================================================
<1> Function Name: ChgM_ModuleInit

<2> Description:
========================================================================================*/
void ChgM_ModuleInit(void)
{
    uWord i;
    uByte* pByte;
    uWord rslt;

    pByte = (uByte*)(&ChgM_Ctrl);
    for (i = 0; i < sizeof(ChgM_Ctrl); i++)
    {
        *(pByte++) = 0;
    }

    pByte = (uByte*)(&ChgM_TmCnt);
    for (i = 0; i < sizeof(ChgM_TmCnt); i++)
    {
        *(pByte++) = 0;
    }
    ChgM_Ctrl.Cnt.CalcCur = CHGM_CNT_CALC_CUR;
    /* load charge parameter from EE, the parameter must config */
    rslt = EE_LoadVar(SYSTEMST_CHG_CTRL_PARA);
    if(ERR_OK != rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);

        Chg_UpdatePermitMaxCellV(3600);    /* 3.65V */
        Chg_UpdatePermitMaxSumV(3500);     /* 350 V */
        Chg_UpdatePermitMaxCurrent(200);   /* 20A  */
        Chg_UpdatePermitMinT(20);          /* -20'C */
        Chg_UpdatePermitMinCurrent(20);    /* 2A */
        Chg_UpdateDownCurrentStep(20);     /* 2A */
        Chg_UpdatePermitMinCellV(2000);    /* 2.0V */
        Chg_UpdatePermitMaxT(90);          /* 50'C */
    }

    /* get battery material type, such as LiFePO4,LiMn2O4 */
    if(0 == (uByte)Bat_GetMaterialType())
    {
        Bat_UpdateMaterialType(LFP);
    }

}

/*=======================================================================================
<1> Function Name: ChgM_ModuleCountCbk

<2> Description: count every 5ms
========================================================================================*/
void ChgM_ModuleCountCbk(void)
{
    (ChgM_TmCnt.GeneralChgCnt < 255) ? (ChgM_TmCnt.GeneralChgCnt++) : (ChgM_TmCnt.GeneralChgCnt = 255);
}

/*=======================================================================================
<1> Function Name: ChgM_ModuleCbk

<2> Description:
========================================================================================*/
void ChgM_ModuleCbk(void)
{
    /* general charge control */
    if(ChgM_TmCnt.GeneralChgCnt >= CHGM_CYCLE)
    {
        ChgM_TmCnt.GeneralChgCnt = 0;
        ChgM_GetBmsInfo();
        ChgM_MainFun();
        ChgM_SetBmsInfo();
        
    }
}

uByte ChgM_GetRelayReady(void)
{
    return ChgM_Ctrl.Flag.ChgRlyCtrl;
}
/*=======================================================================================
<1> Function Name: ChgM_MainFun

<2> Description:
========================================================================================*/
void ChgM_MainFun(void)
{
    uWord RemainTime;
    uWord DC_ChgMaxOut,DC_ChgMinOut;
    uWord wtmp;
    
    sActor_CtrlRelayCmdType CtrlRelayCmd;

    /* Link state Detect */
    /* if ECU_M_ENABLE_GBCHGIF undefine,it will use this code for connect state detect ,
     * or  you can use your own code to make sure the AclinkSt and DclinkSt is true */
#ifndef ECU_M_ENABLE_GBCHGIF
    if(CHGM_FLAG_ENABLE == BmsSys_GetOnBDChgSt())
    {
        Chg_UpdateAcLinkSt(CHGM_FLAG_ENABLE);
    }
    else
    {
        Chg_UpdateAcLinkSt(CHGM_FLAG_DISABLE);
    }
    
    if(CHGM_FLAG_ENABLE == BmsSys_GetOffBDChgSt())
    {
        Chg_UpdateDcLinkSt(CHGM_FLAG_ENABLE);
    }
    else
    {
        Chg_UpdateDcLinkSt(CHGM_FLAG_DISABLE);
    }
#endif
    /* End of Link state Detect */
    
    
    /* Charge CAN state Detect */
#ifdef CHG_DET_CHG_CAN
    /* Use this function to detect whether Charge can have link */
    /* if you do not need this function,close CHG_DET_CHG_CAN */
    ChgM_DetChgCanSt();
#else
    /* you can put your own code here to make the flag  true */
    ChgM_Ctrl.Flag.ChgRx = CHGM_FLAG_ENABLE;
    /* Your can disable this flag and put your own code here to make it true*/
#endif
    /* end of Charge CAN state Detect */
     
    /* Error detect, and determines whether to allow charge */
    if(CHG_ST_OFF != Chg_GetChgSt())
    {
        ChgM_ChgErrJudge();
    }
    
    /* calc charge start time */
    ChgM_CalcChgStartTime(); /* Just a counter, when the current large than 2A,the counter start */

    /* Calculate charge remain time */
    ChgM_ChgRemainTimeCalc(&RemainTime);  
    Chg_UpdateRemainChgTime(RemainTime);  /* Update Charge Remain time */
    
    
    /* Charge Flow Control */
    ChgM_ChgCtrl(ChgM_Ctrl.Data.ChgSt, ChgM_Ctrl.Data.ChgStage);

    #ifdef GAC_TEST
    if(((0 == CarCan_GetVcuDcChargeCom()) && (0 == CarCan_GetVcuAcChargeCom())) 
        || (CarCan_GetBmsMode() < 4) || (CarCan_GetBmsMode() == 7) || (CarCan_GetBmsMode() == 9))
    {
       ChgM_Ctrl.Data.ChgEnable = CHGM_FLAG_DISABLE;
       ChgM_Ctrl.Data.PmtChgCur = 0; 
    }
    else if(1 == CarCan_GetVcuDcChargeCom())
    {
       CarCan_UpdateBmsMode(0x0c);
    }
 
    #endif
    
    /* charge relay control */
#ifdef CHG_RELAY_CTRL
    /*if you do not need this function,close CHGM_RELAY_CTRL, the relay control flg will aways true. */
    ChgM_RelayCtrl();
#else
    /* you can put your own code here to make the flag  true */
    ChgM_Ctrl.Flag.ChgRlyCtrl = RELAY_ON;
    /* Your can disable this flag and put your own code here to make it true*/
#endif
    
    if(RELAY_ON == ChgM_Ctrl.Flag.ChgRlyCtrl)
    {
        CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
        /* Put Your Code here To Close Relay */
    }
    else
    {
        CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
        /* Put Your Code here To Open Relay */
    }
    CtrlRelayCmd.CmdType = IMMED_RELAY_CMD;
    //
    (void)Actor_CtrlRelay(CPU_FAN_ID, CtrlRelayCmd);
        
    /* end of charge relay control */
}

/*=======================================================================================
<1> Function Name: ChgM_ChgCtrl

<2> Description:
========================================================================================*/
void ChgM_ChgCtrl(uByte ChgSt, uByte ChgStage)
{
    uWord CurStep;
    uWord wtmp;

    switch(ChgSt)
    {
    case CHG_ST_OFF:
    {
        /* wait for bms ready */
        ChgM_WaitBmsReady();   
        if(CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady)
        {
            /* Charge link detect */
#ifdef ECU_M_ENABLE_GBCHGIF
            /* Judge Charge have link, Bms have receive information from charger, and dc link is true */
            if((ChgM_Ctrl.Data.ChgSt != CHG_ST_END) && (CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.ChgRx) && (CHGM_FLAG_ENABLE == ChgM_Ctrl.Data.LinkSt))
#else
            /* Judge Charge have link, Bms have receive information from charger or  charge current more than 2A and last 1minute */
            if((ChgM_Ctrl.Data.ChgSt != CHG_ST_END) && ((ChgM_Ctrl.Cnt.ChgStart > CHGM_CNT_CHG_1MIN) || (CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.ChgRx)))
#endif
            {
                ChgM_Ctrl.Data.ChgSt = CHG_ST_ON;            /* CHG_ST_OFF --> CHG_ST_ON */
                ChgM_Ctrl.Data.ChgStage = CHG_STAGE_CC_MODE; /* go to CC stage */
            }
            else
            {
                if(ChgM_Ctrl.Data.ChgSt != CHG_ST_END)
                {
                    ChgM_Ctrl.Data.ChgSt = CHG_ST_OFF;
                }
            }
        }

        break;
    }
    case CHG_ST_ON:
    {
        switch(ChgStage)
        {
        case CHG_STAGE_PRE_HEAT:
        case CHG_STAGE_PRE_CHG:
        case CHG_STAGE_CC_MODE:
        {
            if(CHGM_FLAG_ENABLE == Chg_GetAcLinkSt())
            {
                /* Get Ac Charge Current */
                ChgM_GetAcMaxPmtChgCur();  
            }
            else if(CHGM_FLAG_ENABLE == Chg_GetDcLinkSt())
            {
                /* Get Dc Charge Current */
                ChgM_GetDcMaxPmtChgCur();  
                
            }
            else
            {
                ChgM_Ctrl.Data.PmtChgCur = 0;
            }
            
            /* Jump to Cv charge when maxv more than max permit cellv or sumv large than max permit sumv */
            if((ChgM_Ctrl.Data.MaxV >=  Chg_GetPermitMaxCellV()) || (ChgM_Ctrl.Data.SumV >= Chg_GetPermitMaxSumV()) )
            {
                
                /* Cc stage --> Cv Stage */  
                ChgM_Ctrl.Data.ChgStage = CHG_STAGE_CV_MODE;
            }

            break;
        }
        case CHG_STAGE_CV_MODE:
        {
            CarCan_UpdateBmsMode(0x0c);
            if(CHGM_FLAG_DISABLE == ChgM_Ctrl.Flag.ModifyCur)
            {
                /* Modify charge current by actual Current, only once */
                ChgM_ModifyPmtCurByActualCur(&ChgM_Ctrl.Data.PmtChgCur, ChgM_Ctrl.Data.Cur);
                ChgM_Ctrl.Flag.ModifyCur = CHGM_FLAG_ENABLE;
            }

            /* Whether to start step down*/
            if((ChgM_Ctrl.Data.MaxV >=  Chg_GetPermitMaxCellV()) || (ChgM_Ctrl.Data.SumV >= Chg_GetPermitMaxSumV()) )
            {
                CurStep = Chg_GetDownCurrentStep();
                /* step down current  */
                ChgM_CurStepDownCalc(CurStep);   
            }
            
            /* judge stage, Cv stage --> Ready to end */
            ChgM_StageCvToReadyEnd();   

            break;
        }
        case CHG_STAGE_READY_TO_END:
        {
            CarCan_UpdateBmsMode(0x0c);
            if(CHG_STAGE_READY_TO_END == ChgM_Ctrl.Data.ChgStage)
            {
                wtmp = ChgM_Ctrl.Data.MinChgCur;
                wtmp *= 15;
                wtmp /= 10;
                wtmp = OFFSET_CUR - wtmp;

                /* In charge and Charge Current less than 1.5 * min current
                           or counter have overflow, max permit current modify to 1A*/
                if( ((ChgM_Ctrl.Data.Cur < OFFSET_CUR) && (ChgM_Ctrl.Data.Cur > wtmp))
                 || (ChgM_Ctrl.Cnt.ChgDownCur >= CHGM_CNT_DOWN_CUR) )
                {
                    ChgM_Ctrl.Data.PmtChgCur = 10;
                }
                else
                {
                    ChgM_Ctrl.Cnt.ChgDownCur++;
                }
            }

            /* shut down charger after charge end */
            if( (ChgM_Ctrl.Data.PmtChgCur <= ChgM_Ctrl.Data.MinChgCur)
             && (CHG_STAGE_READY_TO_END ==  ChgM_Ctrl.Data.ChgStage))
            {
                /* wait 5s for charger send the status of "charge end" */
                if(ChgM_Ctrl.Cnt.WaitStop >= CHGM_CNT_WAIT_STOP)
                {
                    ChgM_Ctrl.Cnt.WaitStop = CHGM_CNT_WAIT_STOP;
                    ChgM_Ctrl.Data.PmtChgCur = 0;
                    ChgM_Ctrl.Data.ChgEnable = CHGM_FLAG_DISABLE;
                    ChgM_Ctrl.Data.ChgSt = CHG_ST_END;
                    ChgM_Ctrl.Data.ChgStage = CHG_STAGE_END;
                   // BmsSys_GetSystemTime(date);
                    
                    if(1 == Chg_GetAcLinkSt())
                    {
                        CarCan_UpdateAcChargeFull(1);
                    }
                    else
                    {
                        CarCan_UpdateDcChargeFull(1);
                    }       
                    CarCan_UpdateBmsMode(0x08);
                }
                else
                {
                    ChgM_Ctrl.Cnt.WaitStop++;
                }
            }

            break;
        }
        case CHG_STAGE_END:
        {
            break;
        }
        default :
        {
            break;
        }
        }

        break;
    }
    case CHG_ST_ERR:
    {
        /* Not allow Charg */
        ChgM_Ctrl.Data.PmtChgCur = 0;
        ChgM_Ctrl.Data.ChgEnable = CHGM_FLAG_DISABLE;
        break;
    }
    case CHG_ST_END:
    {
        break;
    }
    default :
    {
        break;
    }
    }
}

/*=======================================================================================
<1> Function Name: ChgM_ChgErrJudge

<2> Description: Error Judgement, if bms have no Error,it will permit charge
========================================================================================*/
void ChgM_ChgErrJudge(void)
{

    ChgM_Ctrl.ChgEndReason.Bit.HdErr = ChgM_HardWareErrJudge();  /* hard ware Error */
    ChgM_Ctrl.ChgEndReason.Bit.IsoErr = ChgM_IsoErrJudge();      /* Iso Error */
    ChgM_Ctrl.ChgEndReason.Bit.CellVoltHigh = ChgM_CellvHighJudge(); /* cell volt high Error */
    ChgM_Ctrl.ChgEndReason.Bit.SumVoltHigh = ChgM_SumvHighJudge(); /* sum volt high Error */
    ChgM_Ctrl.ChgEndReason.Bit.CurHigh = ChgM_CurHighJudge(); /* current high Error */

#ifdef CHG_DET_LIMIT_T
    ChgM_Ctrl.ChgEndReason.Bit.TemperHigh = ChgM_TemperHighJudge();
    ChgM_Ctrl.ChgEndReason.Bit.TemperLow = ChgM_TemperLowJudge();
#endif

#ifdef CHG_DET_LOW_V
    ChgM_Ctrl.ChgEndReason.Bit.CellVoltLow = ChgM_CellVLowJudge();
    ChgM_Ctrl.ChgEndReason.Bit.SumVoltLow = ChgM_SumVLowJudge();
#endif

#ifdef CHG_DET_RLY
    ChgM_Ctrl.ChgEndReason.Bit.RlyErr = ChgM_RlyErrJudge();
#endif

    if(ChgM_ErrAccept())  /* Fault, Jump to Error */
    {
        ChgM_Ctrl.Data.ChgSt = CHG_ST_ERR;    /* Go to Error Status */
        Chg_UpdateBmsStopReason(ChgM_Ctrl.ChgEndReason.LWd);
        ChgM_Ctrl.Data.ChgEnable = CHGM_FLAG_DISABLE;   /* disable charge */
    }
    else
    {
        if((CHGM_FLAG_ENABLE == Chg_GetAcLinkSt()) || (CHGM_FLAG_ENABLE == Chg_GetDcLinkSt()) )
        {
            if((CHG_ST_END != ChgM_Ctrl.Data.ChgSt) && (CHG_ST_ERR != ChgM_Ctrl.Data.ChgSt) 
            && (CHGM_FLAG_ENABLE == ChgM_Ctrl.Flag.BmsReady) && (0 == ChgM_Ctrl.ChgEndReason.LWd))
            {
                ChgM_Ctrl.Data.ChgEnable = CHGM_FLAG_ENABLE;   /* Enable charge */
            }
        }
    }
}

/*=======================================================================================
<1> Function Name: ChgM_GetDcMaxPmtChgCur

<2> Description:  Only for dc charge
========================================================================================*/
void ChgM_GetDcMaxPmtChgCur(void)
{
    uLWord CurTmp[2];
    uWord Step;
    uWord Cur;
    
    /* Lookup table and get cur */
    ChgM_GetCurByTemper(CurTmp, ChgM_Ctrl.Data.MinT, ChgM_Ctrl.Data.MaxT, DcChg_TemperCurTable); 
    
    /* Compare and Get Min Current*/
    ChgM_CompareAndGetMinCur(&Cur, (uWord)CurTmp[1], (uWord)CurTmp[0]); 
    
    /* convert current unit, 0.01C to 0.1A  */
    ChgM_ConverCurUint(&Cur, ChgM_Ctrl.Data.RatedCap);   //如果电流表是以容量为单位则需要换算

    /* Moidify Current according to cell voltage and charge time */
    ChgM_ModifyCurAfterLookUpTable(&Cur);   
    
    /* smooth permit Current, step is 0.05C*/
    Step = ChgM_Ctrl.Data.RatedCap / 20; /* 0.05C */
    ChgM_ModifyPmtChgCurByStep(Cur, Step);  


#ifdef CHG_DET_CHARGER_OUT_CUR    
    /* Modify current by charger's output current */
    ChgM_ModifyCurByChargerOutPutCur(&ChgM_Ctrl.Data.PmtChgCur, &ChgM_Ctrl.Data.MinChgCur,
                                     ChgM_Ctrl.Data.MaxOutPutCur, ChgM_Ctrl.Data.MinOutCur);
#endif

    /* pmt charge current modify to 0 when charge disable  */
    if(CHGM_FLAG_DISABLE == ChgM_Ctrl.Data.ChgEnable)
    {
        ChgM_Ctrl.Data.PmtChgCur = 0;
    }
}

/*=======================================================================================
<1> Function Name: ChgM_GetAcMaxPmtChgCur

<2> Description:   Onley for ac charge
========================================================================================*/
void ChgM_GetAcMaxPmtChgCur(void)
{
    uLWord CurTmp[2];
    uWord  Step;
    uWord  Cur;

    /* Lookup table and get cur */
    ChgM_GetCurByTemper(CurTmp, ChgM_Ctrl.Data.MinT, ChgM_Ctrl.Data.MaxT, AcChg_TemperCurTable);  
    
    /* Compare and Get Min Current*/
    ChgM_CompareAndGetMinCur(&Cur, (uWord)CurTmp[1], (uWord)CurTmp[0]);   
    
    /* Moidify Current Smooth */
    Step = 10;  /* smooth step: 1A */
    ChgM_ModifyPmtChgCurByStep(Cur,Step);   


#ifdef CHG_DET_GB_PLUG_CUR
  #ifdef ECU_M_ENABLE_GBCHGIF  
      /* Modify current by charger's output current if use Gb charge */
      if(CHGM_FLAG_ENABLE == Chg_GetAcLinkSt())
      {
          Chg_UpdatePermitChgCurOn(ChgM_Ctrl.Data.PmtChgCur);
          ChgM_Ctrl.Data.PmtChgCur = GbChgM_ACPmtCurCalc();
      }
  #endif
#endif

#ifdef CHG_DET_CHARGER_OUT_CUR      
    /* Modify current by charger's output current */
    ChgM_ModifyCurByChargerOutPutCur(&ChgM_Ctrl.Data.PmtChgCur, &ChgM_Ctrl.Data.MinChgCur,
                                     ChgM_Ctrl.Data.MaxOutPutCur, ChgM_Ctrl.Data.MinOutCur);
#endif

    /* pmt charge current modify to 0 when charge disable  */
    if(CHGM_FLAG_DISABLE == ChgM_Ctrl.Data.ChgEnable)
    {
        ChgM_Ctrl.Data.PmtChgCur = 0;
    }
}

/*=======================================================================================
<1> Function Name: ChgM_GetChgInfo

<2> Description:   Get  information for charge
========================================================================================*/
void ChgM_GetBmsInfo(void)
{
    ChgM_Ctrl.Data.Life = ChgCan_GetLife();/* charge CAN must update life value */
    ChgM_Ctrl.Data.MinT = Bat_GetMinT();
    ChgM_Ctrl.Data.MaxT = Bat_GetMaxT();
    ChgM_Ctrl.Data.MinV = Bat_GetMinV();
    ChgM_Ctrl.Data.MaxV = Bat_GetMaxV();
    ChgM_Ctrl.Data.RatedCap = (uWord)(Bat_GetRatedCapacityCalc() / 100);   /* 0.1AH */
    ChgM_Ctrl.Data.RemainCap = (uWord)(Bat_GetRemainCapacity() / 100);     /* 0.1AH */
    ChgM_Ctrl.Data.MaterialType = (uByte)(Bat_GetMaterialType());

    if(CHGM_FLAG_ENABLE == Chg_GetAcLinkSt())
    {
        ChgM_Ctrl.Data.PmtChgCur = Chg_GetPermitChgCurOn();     /* Ac charge, permint charge current */
        ChgM_Ctrl.Data.ChgEnable = Chg_GetPermitChgOnSt();      /* Ac charge, permint charge flag */
        ChgM_Ctrl.Data.LinkSt  = Chg_GetAcLinkSt();             /* Ac charge,  charge link status */
    }
    else 
    {
        ChgM_Ctrl.Data.PmtChgCur = Chg_GetPermitChgCurExt();    /* Dc charge, permint charge current */
        ChgM_Ctrl.Data.ChgEnable = Chg_GetPermitChgExtSt();     /* Dc charge, permint charge flag */
        ChgM_Ctrl.Data.LinkSt  = Chg_GetDcLinkSt();             /* Dc charge,  charge link status */
    }
    

    ChgM_Ctrl.Data.Cur = Bat_GetCurrent(INSCUR);
    ChgM_Ctrl.Data.SumV = Bat_GetSumV(INT_SUMV);
    ChgM_Ctrl.Data.ChgSt = Chg_GetChgSt();
    ChgM_Ctrl.Data.ChgStage = Chg_GetChgStage();
    ChgM_Ctrl.Data.MinChgCur = Chg_GetPermitMinCurrent();
    ChgM_Ctrl.Data.MaxOutPutCur = Chg_GetOutPutMaxCurrent();
    ChgM_Ctrl.Data.MinOutCur = Chg_GetOutPutMinCurrent();
}

/*=======================================================================================
<1> Function Name: ChgM_UpdateChgInfo

<2> Description:  Update Parameter
========================================================================================*/
void ChgM_SetBmsInfo(void)
{
    if(CHGM_FLAG_ENABLE == Chg_GetAcLinkSt())
    {
        Chg_UpdatePermitChgOnSt(ChgM_Ctrl.Data.ChgEnable);   /*Update ac permit charge flag */
        Chg_UpdatePermitChgCurOn(ChgM_Ctrl.Data.PmtChgCur);  /*Update ac permit charge current */
    }
    else if(CHGM_FLAG_ENABLE == Chg_GetDcLinkSt())
    {
        Chg_UpdatePermitChgCurExt(ChgM_Ctrl.Data.PmtChgCur);   /*Update dc permit charge current */
        Chg_UpdatePermitChgExtSt(ChgM_Ctrl.Data.ChgEnable);    /*Update dc permit charge flag  */
    }
    else
    {
        Chg_UpdatePermitChgOnSt(ChgM_Ctrl.Data.ChgEnable);   /*Update ac permit charge flag */
        Chg_UpdatePermitChgExtSt(ChgM_Ctrl.Data.ChgEnable);    /*Update dc permit charge flag  */
        Chg_UpdatePermitChgCurOn(ChgM_Ctrl.Data.PmtChgCur);  /*Update ac permit charge current */
        Chg_UpdatePermitChgCurExt(ChgM_Ctrl.Data.PmtChgCur);   /*Update dc permit charge current */
    }

    Chg_UpdateChgStage(ChgM_Ctrl.Data.ChgStage);      /*Update Charge stage */
    Chg_UpdateChgSt(ChgM_Ctrl.Data.ChgSt);            /*Update Charge status */

    /* detect battery status, for Gb Charge */
    if(Err_GetBatErrLevel(ERR_BAT_CELLV_HIGH) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_CELLV_HIGH;
    }
    else if(Err_GetBatErrLevel(ERR_BAT_CELLV_LOW) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_CELLV_LOW;
    }
    else
    {
    }
    
    /* detect battery status, for Gb Charge */
    if(Err_GetBatErrLevel(ERR_BAT_SOC_HIGH) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_SOC_HIGH;
    }
    else if(Err_GetBatErrLevel(ERR_BAT_SOC_LOW) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_SOC_LOW;
    }
    else
    {
    }
    
    /* detect battery status, for Gb Charge */
    if(Err_GetBatErrLevel(ERR_BAT_CUR_HIGH_CHG) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_CUR_HIGH;
    }
    else
    {
    }
    
    /* detect battery status, for Gb Charge */
    if(Err_GetBatErrLevel(ERR_BAT_T_HIGH) != ERR_LEVEL_NORMAL)
    {
        ChgM_Ctrl.Data.BatSt  |= BAT_ST_T_HIGH;
    }
}