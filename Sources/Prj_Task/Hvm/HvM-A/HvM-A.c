// HvM.c

#include "HvM-A.h"
#include "ErrorAlarm.h"
#include "AppRelay.h"
#include "BatterySt.h"
#include "ChargeSt.h"
#include "BmsSysSt.h"
#include "BatterySysSt.h"
#include "SensorDi.h"
#include "SchIf.h"
#include "LvM.h"
#include "DataStore.h"

#ifdef USE_HVM_A


typedef struct
{
    uByte StartPreChg;
    uByte HvPowerOnEnd;
    uByte FirstBoot;
    uByte ReadMinVolt;
    uByte ReadSumVolt;
    uByte ForbidPowerOn;
    uByte PreFirstEnd;
    uByte NegRelaySt;
    uByte PosRelaySt;
    uByte PreRelaySt;

} sHvM_FlagType;

typedef struct
{
    uWord StartPre ;
    uWord EndPre;
    uWord ReadSumVolt;
    uWord ReadMinVolt;
    uWord OpenRelay;
    uWord WaitVcuCmd;
    uWord Step;
    uWord fault;
    uWord TriggerOff;
} sHvM_CntType;

typedef struct
{
    uByte   Step;
    uByte   BcuCmd;
    uByte   NegNeedChk;
    uByte   PosNeedChk;
    uWord   BcuSumvOutBak;

} sHvM_CtrlHvType;

typedef struct
{
    uByte   IsoErr;
    uWord   SumvOut;
    uWord   SumvIn;
    uWord   Current;
} sHvM_BatInf;

typedef struct
{
    byte cnt;
} sHvM_TmCntType;


#define CHECK_PRE

#define PWR_ON_INIT       0

#define F_PWR_ON_N_CHK    1
#define F_WAIT_PWR_ON     2
#define F_PWR_ON          3
#define F_WAIT_PWR_OFF    4
#define F_PWR_OFF_FST_VCU 5
#define F_PWR_OFF_FST_BCU 6
#define F_PWR_OFF_P_CHK   7
#define F_PWR_OFF_SND     8
#define F_PWR_OFF_N_CHK   9
#define F_PWR_OFF_END     10

#define MAX_PRE_CUR   20
#define SUMV_50V      500
#define SUMV_100V     1000
#define SUMV_10V      100
#define CUR_1A        10
#define CUR_10A       100

#define VCU_CTL_PWR_ON     1
#define VCU_CTL_PWR_OFF    2
#define BCU_CTL_P_OFF     1
#define BCU_CTL_N_OFF     2
#define HVM_FLAG_OK   1
#define HVM_FLAG_FAIL 0
#define HVM_FLAG_ENABLE   1
#define HVM_FLAG_DISABLE  0
#define HVM_HIGH	1
#define HVM_LOW  0
#define HVM_TRIGST_OFF 0
#define HVM_TRIGST_ON  1

#define HVM_CNT_30MS  6
#define HVM_CNT_50MS  10
#define HVM_CNT_120MS 24
#define HVM_CNT_150MS 30
#define HVM_CNT_500MS 100
#define HVM_CNT_600MS 120
#define HVM_CNT_1S    200
#define HVM_CNT_2S    400
#define HVM_CNT_20S   4000

#define RLY_STA_OPEN     0x00
#define RLY_STA_CLOSE	   0x01
#define RLY_STA_PASTE	   0x02
#define RLY_STA_INVALID	 0x04

#define PRE_CHG_OVERTIME  0x01
#define PRE_CHG_LOAD_OPEN  0x02
#define PRE_CHG_LOAD_SHORT 0x04
#define PRE_CHG_CIRCURT_OPEN 0x08

sHvM_FlagType    HvM_Flag;
sHvM_CntType     HvM_Cnt;
sHvM_CtrlHvType  HvM_CtrlHv;
sHvM_ErrType     HvM_Err;
sHvM_BatInf      HvM_BatInf;
sActor_CtrlRelayCmdType HvM_CtrlRelayCmd;
sHvM_TmCntType HvM_TmCnt;
sHvM_VcuInfoType HvM_VcuInfo;

static void HvM_ModuleInit(void);
static void HvM_VoltChk(sHvM_FlagType *p_flg);
static uByte HvM_RlyInvalidChk(uByte di_id);
static uByte HvM_RlyPasteChk(uByte di_id);

static void HvM_PowOffChk(sHvM_CtrlHvType *p_ctrl, sHvM_FlagType *p_flg, sHvM_CntType *p_cnt)
{
    if(HVM_TRIGST_OFF != BmsSys_GetAwakeSt())
    {
        p_cnt->TriggerOff = 0;
    }
    
    if( (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_CELLV_LOW))
     || (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_T_HIGH))
     || (ERR_LEVEL_ONE <= Err_GetHwErrLevel(ERR_HW_CAN4))
     || (ERR_LEVEL_THREE <= Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR))
     || (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_SHORT_CIRCUIT)))
    {
        //p_cnt->fault++;
    }
    else
    {
        p_cnt->fault = 0;
    }
    
    if( (p_cnt->TriggerOff >= HVM_CNT_50MS) || (p_cnt->fault >= HVM_CNT_2S))
    {
        p_cnt->TriggerOff = 0;
        p_cnt->fault = 0;
        p_flg->ForbidPowerOn = HVM_FLAG_OK;

        if(F_WAIT_PWR_ON == p_ctrl->Step)
        {
            p_ctrl->Step = F_PWR_OFF_SND;
        }
        
        if(F_PWR_ON == p_ctrl->Step)
        {
            p_ctrl->Step = F_WAIT_PWR_OFF;
            p_ctrl->BcuCmd = BCU_CTL_P_OFF;  /* bcu request power off */
        }
        
        if(F_WAIT_PWR_OFF == p_ctrl->Step)
        {
            p_ctrl->BcuCmd = BCU_CTL_P_OFF;  /* bcu request power off */
        }
    }
}

static void HvM_FDBKHvCtrl(sHvM_CtrlHvType *p_ctrl, sHvM_FlagType *p_flg, sHvM_CntType *p_cnt, sHvM_ErrType  *p_err)
{
    uWord wtmp;
    uLWord llwtmp;
    uWord lwd_sumv_int;
    uWord lwd_sumv_out;
    uByte lby_iso_err;
    uWord lwd_cur;
    GeneralReturnType return_rslt = ERR_OK;
    HvM_CtrlRelayCmd.CmdType = IMMED_RELAY_CMD;


    if(HVM_FLAG_FAIL == p_flg->FirstBoot)   /* initialization using for the first time */
    {
        HvM_ModuleInit();
        p_flg->FirstBoot = HVM_FLAG_OK;
    }

    HvM_PowOffChk(&HvM_CtrlHv, &HvM_Flag, &HvM_Cnt);

    switch(p_ctrl->Step)
    {
    case PWR_ON_INIT:
    {
        HvM_VoltChk(&HvM_Flag);
        
        /*detect relay error */
        p_flg->NegRelaySt = HvM_RlyPasteChk(CPU_DI1_ID); /*whether the N rly is paste? */
        BatSys_UpdateNegRlySt(p_flg->NegRelaySt);
        
        p_flg->PosRelaySt = HvM_RlyPasteChk(CPU_DI2_ID); /*whether the P rly is paste? */
        BatSys_UpdatePosRlySt(p_flg->PosRelaySt);
        
        if(RLY_STA_PASTE == (p_flg->NegRelaySt))
        {
            (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_NEG_ADJOIN, ERR_LEVEL_THREE);
            p_err->code.bit.NegPaste = 1;
            p_flg->ForbidPowerOn = HVM_FLAG_OK;
            p_ctrl->Step = F_PWR_OFF_END;
        }
        else
        {
            p_err->code.bit.NegPaste = 0;
        }
        
        if(RLY_STA_PASTE == (p_flg->PosRelaySt))
        {
            (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_ADJOIN, ERR_LEVEL_THREE);
            p_err->code.bit.PosPaste = 1;
            p_flg->ForbidPowerOn = HVM_FLAG_OK;
            p_ctrl->Step = F_PWR_OFF_END;
        }
        else
        {
            p_err->code.bit.PosPaste = 0;
        }
        
        /* is the sumv valid*/
        if(HVM_FLAG_OK == p_flg->ReadSumVolt)
        {
            p_err->code.bit.SumVolt = 0;
            lwd_sumv_out = Bat_GetSumV(OUT_SUMV);
            p_ctrl->BcuSumvOutBak = lwd_sumv_out;
            p_cnt->ReadSumVolt = 0;
        }
        else
        {
            if(p_cnt->ReadSumVolt >= HVM_CNT_500MS)
            {
                p_cnt->ReadSumVolt = HVM_CNT_500MS;
                p_flg->ReadSumVolt = HVM_FLAG_FAIL;
                p_err->code.bit.SumVolt = 1;
                p_flg->ForbidPowerOn = HVM_FLAG_OK;
                p_ctrl->Step = F_PWR_OFF_END;
            }
        }
        
        /* is the minv valid*/
        if(HVM_FLAG_OK == p_flg->ReadMinVolt)
        {
            p_err->code.bit.MinVolt = 0;
            p_cnt->ReadMinVolt = 0;
        }
        else
        {
            if(p_cnt->ReadMinVolt >= HVM_CNT_500MS)
            {
                p_cnt->ReadMinVolt = HVM_CNT_500MS;
                p_flg->ReadMinVolt = HVM_FLAG_FAIL;
                p_err->code.bit.MinVolt = 1;
                p_flg->ForbidPowerOn = HVM_FLAG_OK;
                p_ctrl->Step = F_PWR_OFF_END;
            }
        }

        lby_iso_err = (uByte)(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR));
        if(0 != lby_iso_err)
        {
            p_err->code.bit.IsoBat = 1;
            p_flg->ForbidPowerOn = HVM_FLAG_OK;
            p_ctrl->Step = F_PWR_OFF_END;
        }
        
        if( (0 == lby_iso_err)
         && (HVM_FLAG_OK == p_flg->ReadSumVolt)
         && (HVM_FLAG_OK == p_flg->ReadSumVolt)
         && (p_flg->PosRelaySt != RLY_STA_PASTE)
         && (p_flg->NegRelaySt != RLY_STA_PASTE) )
        {
            HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
            return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
            
            p_ctrl->Step = F_PWR_ON_N_CHK; /* wait power on */
            p_cnt->Step = 0;
            
            LvM_UpdateSystemShutDownEn(HVM_FLAG_DISABLE);// control low power off
        }
        break;
    }
    case F_PWR_ON_N_CHK:
    {
        if(p_cnt->Step >= HVM_CNT_30MS) /* whether the N rly is invalid,and the pre is paste? */
        {
            p_ctrl->Step = F_WAIT_PWR_ON;
        }
        break;
    }

    case F_WAIT_PWR_ON:
    {
        p_cnt->Step = 0;
        lby_iso_err = (uByte)(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR));
        
        p_flg->NegRelaySt = HvM_RlyInvalidChk(CPU_DI1_ID); /* whether the N rly is invalid?  */
        BatSys_UpdateNegRlySt(p_flg->NegRelaySt);
        
        if( (HvM_VcuInfo.VcuSumV > (SUMV_50V + p_ctrl->BcuSumvOutBak))
         || (Bat_GetSumV(OUT_SUMV) > (SUMV_50V + p_ctrl->BcuSumvOutBak)) )
        {
            p_flg->PreRelaySt = RLY_STA_PASTE;
            BatSys_UpdatePreRlySt(p_flg->PreRelaySt);
        }
        else
        {
            p_flg->PreRelaySt = RLY_STA_OPEN;
            BatSys_UpdatePreRlySt(p_flg->PreRelaySt);
        }

        if( (0 == lby_iso_err)
         && (p_flg->NegRelaySt == RLY_STA_CLOSE)
         && (p_flg->PreRelaySt != RLY_STA_PASTE))
        {
            if(VCU_CTL_PWR_ON == (HvM_VcuInfo.Cmd & VCU_CTL_PWR_ON))  /* vcu request power on */
            {
                if( (0 == Chg_GetAcLinkSt())
                 && (0 == Chg_GetDcLinkSt())
                 && ((uByte)Err_GetBatErrLevel(ERR_BAT_CELLV_LOW) < (uByte)ERR_LEVEL_TWO) )
                {
                    p_ctrl->Step = F_PWR_ON;
                }
                else
                {
                    if((0 != Chg_GetAcLinkSt()) || (0 != Chg_GetDcLinkSt()))
                    {
                        p_err->code.bit.ChgLink = 1;
                    }
                    
                    if((uByte)Err_GetBatErrLevel(ERR_BAT_CELLV_LOW) >= (uByte)ERR_LEVEL_TWO)
                    {
                        p_err->code.bit.MinVolt = 1;
                    }

                    p_ctrl->Step = F_PWR_OFF_SND;	  /*open neg relay*/
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                }
            }
        }
        else
        {
            if(0 != lby_iso_err)
            {
                p_err->code.bit.IsoNeg = 1;
                p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
            }
            else
            {
                p_err->code.bit.IsoNeg = 0;
            }
            
            if(RLY_STA_INVALID == p_flg->NegRelaySt)
            {
                (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_NEG_NOT_CLOSED, ERR_LEVEL_THREE);
                p_err->code.bit.NegInvalid = 1;
                p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                p_ctrl->NegNeedChk = HVM_FLAG_FAIL; /* do not check neg status*/
            }
            else
            {
                p_err->code.bit.NegInvalid = 0;
            }
            
            if(RLY_STA_PASTE == p_flg->PreRelaySt)
            {
                (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_ADJOIN, ERR_LEVEL_THREE);
                p_err->code.bit.PrePaste = 1;
                p_ctrl->PosNeedChk = HVM_FLAG_FAIL;
            }
            else
            {
                p_err->code.bit.PrePaste = 0;
            }
            
            p_ctrl->Step = F_PWR_OFF_SND ; /* forbid power on again*/
            p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
        }

        break;
    }

    case F_PWR_ON:
    {
        if(HVM_FLAG_FAIL == p_flg->StartPreChg)
        {
            p_flg->StartPreChg = HVM_FLAG_OK;
            HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
            return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
            p_cnt->StartPre = 0;
        }
        
        if(p_cnt->StartPre < HVM_CNT_600MS)		// pre time
        {
#ifdef CHECK_PRE
            if((p_cnt->StartPre > HVM_CNT_120MS) && (p_cnt->StartPre < HVM_CNT_150MS))
            {
                lwd_cur = Bat_GetCurrent(INSCUR);
                lwd_sumv_out = Bat_GetSumV(OUT_SUMV);
                lwd_sumv_int = Bat_GetSumV(INT_SUMV);
                if(lwd_cur > (OFFSET_CUR + MAX_PRE_CUR) )
                {
                    p_err->code.bit.LoadShort = 1;	 /*the load is short*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt(Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = F_PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_LOAD_SHORT);
                }
                else if( lwd_sumv_out < (SUMV_50V + p_ctrl->BcuSumvOutBak) )
                {
                    p_err->code.bit.PreCircuitOff = 1;  /*pre circuit is off*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt(Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = F_PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_CIRCURT_OPEN);
                }
                else if(lwd_sumv_int < (lwd_sumv_out + SUMV_10V))
                {
                    p_err->code.bit.LoadOff = 1;  /*load circuit is off*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt(Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = F_PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_LOAD_OPEN);
                }
                else
                {
                    p_err->code.bit.LoadShort = 0;
                    p_err->code.bit.LoadOff = 0;
                    p_err->code.bit.PreCircuitOff = 0;
                }
            }
#endif
        }
        else
        {
            /* precharge delay above 600 ms
            and precharge voltage is above 95% sumv*/
            /* 95% of voltage inside relay */
            lby_iso_err = (uByte)(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR));
            lwd_sumv_int = Bat_GetSumV(INT_SUMV);
            lwd_sumv_out = Bat_GetSumV(OUT_SUMV);
            llwtmp = lwd_sumv_int;
            llwtmp *= 95;
            llwtmp /= 100;
            wtmp = (uWord)(llwtmp);

            if( ((lwd_sumv_out > wtmp) || (HvM_VcuInfo.VcuSumV > wtmp)) /* the sumv is detectde by vcu */
                    && ( ((lwd_sumv_out  <= lwd_sumv_int) && (lwd_sumv_int < lwd_sumv_out + SUMV_20V))/* delt_v < 10V*/
                         ||( (HvM_VcuInfo.VcuSumV <= lwd_sumv_int) && (lwd_sumv_int < HvM_VcuInfo.VcuSumV + SUMV_20V)))
                    && (0 == lby_iso_err) )         /*need modify,like ext_sumv 960V  sumv 1000V*/
            {
                if(HVM_FLAG_FAIL == p_flg->PreFirstEnd)
                {
                    p_flg->PreFirstEnd = HVM_FLAG_OK; /* forbid close pos again*/

                    HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
                    return_rslt = Actor_CtrlRelay(POS_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                    p_cnt->EndPre = 0;
                }
                
                if(p_cnt->EndPre >= HVM_CNT_50MS) /* 50 ms after positive relay close */
                {
                    p_cnt->EndPre = HVM_CNT_50MS;
                    p_flg->PosRelaySt = HvM_RlyInvalidChk(CPU_DI2_ID);
                    BatSys_UpdatePosRlySt(p_flg->PosRelaySt);
                    
                    if(p_flg->PosRelaySt == RLY_STA_INVALID)
                    {
                        p_err->code.bit.PosInvalid = 1;
                        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                        return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                        BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_NOT_CLOSED, ERR_LEVEL_THREE);
                        
                        p_ctrl->Step = F_PWR_OFF_FST_VCU;
                        p_ctrl->PosNeedChk = HVM_FLAG_FAIL;
                        p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    }
                    else
                    {
                        p_err->code.bit.PosInvalid = 0;
                        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                        return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                        BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                        
                        p_flg->HvPowerOnEnd = HVM_FLAG_OK;  /* power on sucess*/
                        p_ctrl->Step = F_WAIT_PWR_OFF; /* wait power off */
                    }
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                }

            }
            else
            {
                /* power on failed */
                if(HVM_FLAG_OK != p_flg->HvPowerOnEnd)
                {
                    if(p_cnt->StartPre > HVM_CNT_2S)
                    {
                        p_cnt->StartPre = 0 ;
                        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                        return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                        BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                        
                        p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                        p_ctrl->Step = F_PWR_OFF_SND; /* open neg relay */
                        p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                        (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_OVERTIME);

                        if(0 != lby_iso_err)
                        {
                            p_err->code.bit.IsoPos = 1;
                        }
                        else
                        {
                            p_err->code.bit.IsoPos = 0;
                        }
                        /* update relay life time */
                        (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    }
                }
            }
        }
        
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == VCU_CTL_PWR_OFF)	/* vcu request power off */
        {
            p_ctrl->Step = F_PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        
        break;
    }

    case F_WAIT_PWR_OFF:
    {
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == VCU_CTL_PWR_OFF)	/* vcu request power off */
        {
            p_ctrl->Step = F_PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        else if(BCU_CTL_P_OFF == p_ctrl->BcuCmd)  	/* bcu request power off */
        {
            p_ctrl->Step = F_PWR_OFF_FST_BCU;
            p_cnt->WaitVcuCmd = 0;
        }
        else
        {
            p_ctrl->Step = F_WAIT_PWR_OFF;
        }
        break;
    }

    case F_PWR_OFF_FST_VCU:
    {
        // check pre-relay must open
        if(RLY_STA_OPEN != BatSys_GetPreRlySt())
        {
            HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
            (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
            /* update relay life time */
            (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));    
        }
        
        if(RLY_STA_OPEN != BatSys_GetPosRlySt())
        {
            /* the cur is safety*/
            lwd_cur = Bat_GetCurrent(INSCUR);
            if( (lwd_cur< (OFFSET_CUR + CUR_10A)) && (lwd_cur > (OFFSET_CUR - CUR_10A)))
            {
                HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                return_rslt = Actor_CtrlRelay(POS_RELAY_ID, HvM_CtrlRelayCmd);
                BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                
                /* update relay life time */ 
                (void)BatSys_RlyLifeCalc(POS_RELAY_ID, Bat_GetCurrent(INSCUR));
                
                p_ctrl->PosNeedChk = HVM_FLAG_OK;
                p_ctrl->Step = F_PWR_OFF_P_CHK;
                p_cnt->Step = 0;
            }
            else
            {
                if(p_cnt->OpenRelay > HVM_CNT_1S)
                {
                    p_cnt->OpenRelay = 0;
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    return_rslt = Actor_CtrlRelay(POS_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                    
                    /* update relay life time */ 
                    (void)BatSys_RlyLifeCalc(POS_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_OK;
                    p_ctrl->Step = F_PWR_OFF_P_CHK;
                    p_cnt->Step = 0;
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_OPEN_CUR, ERR_LEVEL_ONE);
                    DataStore_EnableEventDataStore(RLY_ERR_OPEN);
                }
            }
        }
        else
        {
            p_ctrl->Step = F_PWR_OFF_P_CHK;
            p_cnt->Step = 0;
        }

        break;
    }

    case F_PWR_OFF_FST_BCU:
    {
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == VCU_CTL_PWR_OFF) /* vcu  agree open neg relay */
        {
            p_ctrl->Step = F_PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        else
        {
            if(p_cnt->WaitVcuCmd > HVM_CNT_2S) /* after 2S open neg relay*/
            {
                p_cnt->WaitVcuCmd = 0;
                p_ctrl->Step = F_PWR_OFF_FST_VCU;
                p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
            }
        }

        break;
    }

    case F_PWR_OFF_P_CHK:
    {
        if(HVM_FLAG_OK == p_ctrl->PosNeedChk)
        {
            if(p_cnt->Step >= HVM_CNT_30MS)
            {
                p_flg->PosRelaySt = HvM_RlyPasteChk(CPU_DI2_ID); /* P rly is open? */
                BatSys_UpdatePosRlySt(p_flg->PosRelaySt);
                
                if(RLY_STA_PASTE == p_flg->PosRelaySt)
                {
                    p_err->code.bit.PosPaste = 1;
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_ADJOIN, ERR_LEVEL_THREE);
                }
                else
                {
                    p_err->code.bit.PosPaste = 0;
                }
                
                p_ctrl->Step = F_PWR_OFF_SND ;
            }
        }
        else
        {
            p_ctrl->Step = F_PWR_OFF_SND ;
        }

        break;
    }

    case F_PWR_OFF_SND :
    {
        p_cnt->Step = 0;

        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
        return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
        BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
        /* update relay life time */
        (void)BatSys_RlyLifeCalc(NEG_RELAY_ID, Bat_GetCurrent(INSCUR));
        p_ctrl->Step = F_PWR_OFF_N_CHK ;

        break;
    }
    case F_PWR_OFF_N_CHK:
    {
        if(HVM_FLAG_OK == p_ctrl->NegNeedChk)
        {
            if(p_cnt->Step >= 3)
            {
                p_flg->NegRelaySt = HvM_RlyPasteChk(CPU_DI1_ID);
                BatSys_UpdateNegRlySt(p_flg->NegRelaySt);

                if(	p_flg->NegRelaySt == RLY_STA_PASTE)
                {
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_NEG_ADJOIN, ERR_LEVEL_THREE);
                    p_err->code.bit.NegPaste = 1;
                }
                else
                {
                    p_err->code.bit.NegPaste = 0;
                }
            
                p_ctrl->Step = F_PWR_OFF_END ;
            }
        }
        else
        {
            p_ctrl->Step = F_PWR_OFF_END ;
        }
        break;
    }
    case F_PWR_OFF_END :
    {
        p_cnt->Step = 0;
        LvM_UpdateSystemShutDownEn(HVM_FLAG_ENABLE);// control low power off
        
        if(((VCU_CTL_PWR_ON == (HvM_VcuInfo.Cmd & VCU_CTL_PWR_ON))) && (HVM_FLAG_FAIL == p_flg->ForbidPowerOn) )/* restart */
        {
            p_ctrl->Step = PWR_ON_INIT;
            p_flg->StartPreChg = HVM_FLAG_FAIL;
            p_flg->PreFirstEnd = HVM_FLAG_FAIL;
        }

        break;
    }
    default :
        break;
    }
    
    (void)return_rslt;
}

static uByte HvM_RlyInvalidChk(uByte di_id)
{
    uByte flg;
    eSensor_DiStatusType di_value;

    flg = 0xFF;
    di_value = Sensor_GetDiSt(di_id);
    
    if(HVM_HIGH == (uByte)di_value)
    {
        flg = RLY_STA_CLOSE;
    }
    else
    {
        flg = RLY_STA_INVALID;
    }

    return flg;
}

static uByte HvM_RlyPasteChk(uByte di_id)
{
    uByte flg;
    eSensor_DiStatusType di_value;
    
    flg = 0xFF;
    di_value = Sensor_GetDiSt(di_id);
    
    if(HVM_LOW == (uByte)di_value)
    {
        flg = RLY_STA_OPEN;
    }
    else
    {
        flg = RLY_STA_PASTE;
    }

    return flg;
}

static void HvM_VoltChk(sHvM_FlagType *p_flg)
{
    uWord lwd_sumv_int;
    uWord lwd_sumv_acc;
    uWord lwd_maxv;
    uWord lwd_minv;

    /* is the sumv valid*/

    if(HVM_FLAG_FAIL == p_flg->ReadSumVolt)
    {
        lwd_sumv_acc = Bat_GetSumV(ACC_SUMV);
        lwd_sumv_int = Bat_GetSumV(INT_SUMV);
        if((lwd_sumv_acc > SUMV_100V) || (lwd_sumv_int > SUMV_100V))
        {
            p_flg->ReadSumVolt = HVM_FLAG_OK;
        }
        else
        {
            p_flg->ReadSumVolt = HVM_FLAG_FAIL;

        }
    }

    /* is the minv valid*/
    if(HVM_FLAG_FAIL == p_flg->ReadMinVolt)
    {
        lwd_maxv = Bat_GetMaxV();
        lwd_minv = Bat_GetMinV();

        if((lwd_maxv >0) && (lwd_minv > 0) && (lwd_minv != 0xFFFF))
        {
            p_flg->ReadMinVolt = HVM_FLAG_OK;
        }
        else
        {
            p_flg->ReadMinVolt = HVM_FLAG_FAIL;

        }
    }
}

static void HvM_ModuleInit(void)
{

    uByte *p_byte;
    uWord i;

    p_byte = (uByte *)(&HvM_Flag);
    for(i = 0; i < sizeof(HvM_Flag); i++)
    {
        *(p_byte++) = 0;
    }
    p_byte = (uByte *)(&HvM_Cnt);
    for(i = 0; i < sizeof(HvM_Cnt); i++)
    {
        *(p_byte++) = 0;
    }
    p_byte = (uByte *)(&HvM_CtrlHv);
    for(i = 0; i < sizeof(HvM_CtrlHv); i++)
    {
        *(p_byte++) = 0;
    }
    p_byte = (uByte *)(&HvM_Err);
    for(i = 0; i < sizeof(HvM_Err); i++)
    {
        *(p_byte++) = 0;
    }
    p_byte = (uByte *)(&HvM_CtrlRelayCmd);
    for(i = 0; i < sizeof(HvM_CtrlRelayCmd); i++)
    {
        *(p_byte++) = 0;
    }
    
    HvM_CtrlHv.PosNeedChk = HVM_FLAG_OK;
    HvM_CtrlHv.NegNeedChk = HVM_FLAG_OK;
    HvM_CtrlHv.Step = PWR_ON_INIT;

    HvM_TmCnt.cnt = 0;
#ifdef __DEBUG_TEST_HVM

   // HvM_Flag.ReadMinVolt = 1;
   // HvM_Flag.ReadSumVolt = 1;
    HvM_VcuInfo.Cmd = 1;
    Bat_UpdateSumV(INT_SUMV, 4000);
    Bat_UpdateMaxV(3000);
    Bat_UpdateMinV(3000);
    BmsSys_UpdateAwakeSt(0);

#endif

}

void HvM_UpdateVcuInfo(sHvM_VcuInfoType *p_vcu)
{
    HvM_VcuInfo.Cmd = p_vcu->Cmd;
    HvM_VcuInfo.Life = p_vcu->Life;
    HvM_VcuInfo.VcuSumV = p_vcu->VcuSumV;
}

void HvM_ModuleCountCbk(void)
{
    (HvM_TmCnt.cnt < 255) ? (HvM_TmCnt.cnt++) : (HvM_TmCnt.cnt = 255);
    
    (HvM_Cnt.StartPre < 65535) ? (HvM_Cnt.StartPre++) : (HvM_Cnt.StartPre = 65535);
    (HvM_Cnt.EndPre < 65535) ? (HvM_Cnt.EndPre++) : (HvM_Cnt.EndPre = 65535);
    (HvM_Cnt.ReadSumVolt < 65535) ? (HvM_Cnt.ReadSumVolt++) : (HvM_Cnt.ReadSumVolt = 65535);
    (HvM_Cnt.ReadMinVolt < 65535) ? (HvM_Cnt.ReadMinVolt++) : (HvM_Cnt.ReadMinVolt = 65535);
    (HvM_Cnt.OpenRelay < 65535) ? (HvM_Cnt.OpenRelay++) : (HvM_Cnt.OpenRelay = 65535);
    (HvM_Cnt.WaitVcuCmd < 65535) ? (HvM_Cnt.WaitVcuCmd++) : (HvM_Cnt.WaitVcuCmd = 65535);
    (HvM_Cnt.Step < 65535) ? (HvM_Cnt.Step++) : (HvM_Cnt.Step = 65535);
    (HvM_Cnt.fault < 65535) ? (HvM_Cnt.fault++) : (HvM_Cnt.fault = 65535);
    (HvM_Cnt.TriggerOff < 65535) ? (HvM_Cnt.TriggerOff++) : (HvM_Cnt.TriggerOff = 65535);
}

void HvM_ModuleCbk(void)
{
#ifdef __DEBUG_TEST_HVM
    HvM_TmCnt.cnt  = 3;
#endif

    if(HvM_TmCnt.cnt >= TM_MS(10))
    {
        HvM_TmCnt.cnt = 0;
        HvM_FDBKHvCtrl(&HvM_CtrlHv, &HvM_Flag, &HvM_Cnt, &HvM_Err);
    }
}

#endif // #ifdef USE_HVM_A

