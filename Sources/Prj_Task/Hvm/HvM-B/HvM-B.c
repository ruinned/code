// HvM.c

#include "HvM-B.h"
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
#include "CarCan.h"


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
    uByte ChgRelaySt;
    uByte HeatRelaySt;
} sHvM_FlagType;

typedef struct
{
    uWord   StartPre ;
    uWord   EndPre;
    uWord   ReadSumVolt;
    uWord   ReadMinVolt;
    uWord   OpenRelay;
    uWord   WaitVcuCmd;
    uWord   Step;
    uWord   fault;
    uWord   TriggerOff;
    uWord   NegRelayFilter;
    uWord   PRelayFilter;
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
#define N_CHK    	        1
#define P_CHK			        2
#define N_ON							3
#define WAIT_PWR_ON				4
#define PWR_ON						5
#define PWR_ON_P_CHK  		6
#define WAIT_PWR_OFF			7
#define PWR_OFF_FST_VCU		8
#define PWR_OFF_FST_BCU		9
#define PWR_OFF_SND				10
#define PWR_OFF_END				11
#define DC_CHK            12


#define MAX_PRE_CUR   20
#define SUMV_20V      200
#define SUMV_50V      500
#define SUMV_100V     1000
#define SUMV_150V     1500
#define SUMV_10V      100
#define CUR_1A        10
#define CUR_10A       100

#define VCU_CTL_PWR_ON     1
#define VCU_CTL_PWR_OFF    1
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
#define HVM_CNT_100MS 20
#define HVM_CNT_120MS 24
#define HVM_CNT_150MS 30
#define HVM_CNT_200MS 40
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

#define  POS_RELAY_PASE_SUMV   500


sHvM_FlagType    HvM_Flag;
sHvM_CntType     HvM_Cnt;
sHvM_CtrlHvType  HvM_CtrlHv;
sHvM_ErrType     HvM_Err;
sHvM_BatInf      HvM_BatInf;
sActor_CtrlRelayCmdType HvM_CtrlRelayCmd;
sHvM_TmCntType HvM_TmCnt;
sHvM_VcuInfoType HvM_VcuInfo;

uByte gby_PosPaseCnt;

static void HvM_ModuleInit(void);
static void HvM_VoltChk(sHvM_FlagType *p_flg);

static void HvM_PowOffChk(sHvM_CtrlHvType *p_ctrl, sHvM_FlagType *p_flg, sHvM_CntType *p_cnt)
{
    if(HVM_TRIGST_OFF != BmsSys_GetAwakeSt())
    {
        p_cnt->TriggerOff = 0;
    }
    p_cnt->TriggerOff = 0;
    /*if( (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_CELLV_LOW))
     || (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_T_HIGH))
     || (ERR_LEVEL_ONE <= Err_GetHwErrLevel(ERR_HW_CAN4))
     || (ERR_LEVEL_THREE <= Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR))
     || (ERR_LEVEL_THREE == Err_GetBatErrLevel(ERR_BAT_SHORT_CIRCUIT)))*/

    if(4 != CarCan_GetBmsErrLvl())
    {
        p_cnt->fault = 0;
    }
    
    if( (p_cnt->TriggerOff >= HVM_CNT_50MS) || (p_cnt->fault >= HVM_CNT_1S))
    {
        p_cnt->TriggerOff = 0;
        p_cnt->fault = 0;
        p_flg->ForbidPowerOn = HVM_FLAG_OK;

        if(WAIT_PWR_ON == p_ctrl->Step)
        {
            p_ctrl->Step = PWR_OFF_SND;
        }
        
        if(PWR_ON == p_ctrl->Step)
        {
            p_ctrl->Step = WAIT_PWR_OFF;
        }
        
        if(WAIT_PWR_OFF == p_ctrl->Step)
        {
            p_ctrl->BcuCmd = BCU_CTL_P_OFF;  /* bcu request power off */
        }        
    }
}

static void HvM_NFDBKHvCtrl(sHvM_CtrlHvType *p_ctrl, sHvM_FlagType *p_flg, sHvM_CntType *p_cnt, sHvM_ErrType  *p_err)
{
    uWord wtmp,wtmp1,wtmp2,wtmp_bak,wtmp_bak_bak;
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
        CarCan_UpdateBmsMode(1);
        HvM_VoltChk(&HvM_Flag);
        
        /* is the sumv valid*/
        if(HVM_FLAG_OK == p_flg->ReadSumVolt)
        {
            p_err->code.bit.SumVolt = 0;
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
                p_ctrl->Step = PWR_OFF_SND;
                p_cnt->Step = 0;
            }
            break;
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
                p_ctrl->Step = PWR_OFF_SND;
                p_cnt->Step = 0;
            }
            break;
        }
        
        p_ctrl->BcuSumvOutBak = Bat_GetSumV(OUT_SUMV);
            
        /* close pre relay, check neg relay whether paste */
        HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
        return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
        BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID)); 
        
        LvM_UpdateSystemShutDownEn(HVM_FLAG_DISABLE);// control low power off
        
        p_ctrl->Step = N_CHK;
        p_cnt->Step = 0;
        
        break;
    }

    case N_CHK:
    {
        /* wait 70ms for pre-relay colse */
        if(p_cnt->Step >= 7)		
        {
            /* check neg relay whether paste */
            if(Bat_GetSumV(OUT_SUMV) > (SUMV_50V + p_ctrl->BcuSumvOutBak))
            {
                if(p_cnt->NegRelayFilter >= HVM_CNT_50MS)
                {
                    BatSys_UpdateNegRlySt(RLY_STA_PASTE); 
                    p_flg->NegRelaySt = RLY_STA_PASTE;
                    p_err->code.bit.NegPaste = 1;
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_NEG_ADJOIN, ERR_LEVEL_THREE);
                    
                    /* forbid pow on again */
                    p_cnt->Step = 0;
                    p_flg->ForbidPowerOn = HVM_FLAG_OK;
                    p_ctrl->Step = PWR_OFF_SND;	 
                }
                else
                {
                    break;
                }
            }
            else
            {
                p_cnt->NegRelayFilter = 0;
                /* neg relay is open */
                BatSys_UpdateNegRlySt(RLY_STA_OPEN);  
                p_flg->NegRelaySt = RLY_STA_OPEN;
                p_err->code.bit.NegPaste = 0;
               
                p_cnt->Step = 0;
                p_ctrl->Step = P_CHK;
            }
                            
            /* open pre-relay */
            
            HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
            return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID)); 
            /* update relay life time */
            (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
        }
        else
        {
            p_cnt->NegRelayFilter = 0;
        }
        break;
    }
    case P_CHK:
    {
        if(Bat_GetSumV(INT_SUMV) > 0)
        {
            if(p_cnt->Step < 2)		
            {
                p_cnt->PRelayFilter = 0;
                break;
            }
            wtmp1 = Bat_GetSumV(INT_SUMV);
            wtmp2 = wtmp1;
            wtmp_bak = Bat_GetSumV(INT_SUMV_BAK);
            wtmp_bak_bak = Bat_GetSumV(INT_SUMV_BAK1);
            if(wtmp1 >= wtmp_bak)
            {
                wtmp1 -= wtmp_bak;
            }
            else
            {
                wtmp1 = wtmp_bak - wtmp1;
            }
            
            if(wtmp2 >= wtmp_bak_bak)
            {
                wtmp2 -= wtmp_bak_bak;
            }
            else
            {
                wtmp2 = wtmp_bak_bak - wtmp2;
            }
            if((wtmp1 < SUMV_50V) || ((wtmp2 < SUMV_50V)))
            {
                if(p_cnt->PRelayFilter >= HVM_CNT_50MS)
                {
                    if((wtmp1 < SUMV_50V) && (wtmp2 < SUMV_50V))
                    {
                        BatSys_UpdatePosRlySt(RLY_STA_PASTE); 
                        p_flg->PosRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.PosPaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_ADJOIN, ERR_LEVEL_THREE);
                        
                        BatSys_UpdatePreRlySt(RLY_STA_PASTE); 
                        p_flg->PreRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.PrePaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_ADJOIN, ERR_LEVEL_THREE);
                        
                        BatSys_UpdateChgRlySt(RLY_STA_PASTE); 
                        p_flg->ChgRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.ChgPaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_CHG_ADJOIN, ERR_LEVEL_THREE);
                    }
                    else if(wtmp1 < SUMV_50V)
                    {
                        BatSys_UpdatePosRlySt(RLY_STA_PASTE); 
                        p_flg->PosRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.PosPaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_ADJOIN, ERR_LEVEL_THREE);
                        
                        BatSys_UpdatePreRlySt(RLY_STA_PASTE); 
                        p_flg->PreRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.PrePaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_ADJOIN, ERR_LEVEL_THREE);
                    }
                    else
                    {
                        BatSys_UpdateChgRlySt(RLY_STA_PASTE); 
                        p_flg->ChgRelaySt = RLY_STA_PASTE;
                        p_err->code.bit.ChgPaste = 1;
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_CHG_ADJOIN, ERR_LEVEL_THREE);
                    }
                    /* forbid pow on again */
                    p_cnt->Step = 0;
                    p_flg->ForbidPowerOn = HVM_FLAG_OK;
                    p_ctrl->Step = PWR_OFF_SND;	 
                }
                else
                {
                    break;
                }
            }
            else
            {
                p_cnt->PRelayFilter = 0;
                /* neg relay is open */
                BatSys_UpdatePosRlySt(RLY_STA_OPEN);  
                p_flg->PosRelaySt = RLY_STA_OPEN;
                p_err->code.bit.PosPaste = 0;
                
                BatSys_UpdatePreRlySt(RLY_STA_OPEN);  
                p_flg->PreRelaySt = RLY_STA_OPEN;
                p_err->code.bit.PrePaste = 0;
               
                p_cnt->Step = 0;
                p_ctrl->Step = DC_CHK; 
            }
        }
        else 
        {
            p_cnt->Step = 0;
            p_flg->ForbidPowerOn = HVM_FLAG_OK;
            p_ctrl->Step = PWR_OFF_SND;
        }
        break;
    }
    case DC_CHK:
    {
        if(Bat_GetSumV(INT_SUMV) > 0)
        {
            if(1 != Chg_GetDcLinkSt())
            {
                HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
                return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
                BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
                if(p_cnt->Step < 2)		
                {
                    break;
                }
                wtmp1 = Bat_GetSumV(INT_SUMV);
                wtmp_bak = Bat_GetSumV(OUT_SUMV_BAK);
                if(wtmp1 >= wtmp_bak)
                {
                    wtmp1 -= wtmp_bak;
                }
                else
                {
                    wtmp1 = wtmp_bak - wtmp1;
                }
                
                if(wtmp1 < SUMV_50V)
                {
                    BatSys_UpdateAuxBSt(RLY_STA_PASTE); 
                    //p_flg->HeatRelaySt = RLY_STA_PASTE;
                    //p_err->code.bit.HeatPaste = 1;
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_HEATER_PASE, ERR_LEVEL_THREE);
                }
                else
                {
                    BatSys_UpdateAuxBSt(RLY_STA_OPEN);  
                    //p_flg->HeatRelaySt = RLY_STA_OPEN;
                    //p_err->code.bit.HeatPaste = 0;   
                }
                HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
                BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
            }
            p_cnt->Step = 0;
            p_ctrl->Step = WAIT_PWR_ON;
        }
        else 
        {
            p_cnt->Step = 0;
            p_flg->ForbidPowerOn = HVM_FLAG_OK;
            p_ctrl->Step = PWR_OFF_SND;
        }
        break;
    }
    case WAIT_PWR_ON:
    {
        /* vcu request power on */
        LvM_UpdateSystemShutDownEn(HVM_FLAG_ENABLE);// control low power off
        if(CarCan_GetBmsMode() < 2)
        {  
           CarCan_UpdateBmsMode(2);
        }
        if(VCU_CTL_PWR_ON == (HvM_VcuInfo.Cmd & VCU_CTL_PWR_ON))  
        {
            p_ctrl->Step = PWR_ON;
            HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
            return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
            LvM_UpdateSystemShutDownEn(HVM_FLAG_DISABLE);// control low power off
            p_flg->StartPreChg = HVM_FLAG_FAIL;
        }
        break;
    }

    case PWR_ON:
    {
        /* close pre-relay */
        if(HVM_FLAG_FAIL == p_flg->StartPreChg)
        {
            CarCan_UpdateBmsMode(3);
            p_flg->StartPreChg = HVM_FLAG_OK;

            HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
            return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
            BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
            p_cnt->StartPre = 0;
            
        }
        /* begin pre charge */
        if(p_cnt->StartPre < HVM_CNT_200MS)		
        {
#ifdef CHECK_PRE
            if((p_cnt->StartPre > HVM_CNT_120MS) && (p_cnt->StartPre < HVM_CNT_500MS))
            {
                lwd_cur = Bat_GetCurrent(INSCUR);
                lwd_sumv_out = Bat_GetSumV(OUT_SUMV);
                lwd_sumv_int = Bat_GetSumV(INT_SUMV);
                
                if(lwd_cur > (OFFSET_CUR + MAX_PRE_CUR))
                {
                    p_err->code.bit.LoadShort = 1;	 /*the load is short*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_LOAD_SHORT);
                    CarCan_UpdateBmsMode(7);
                }
                else if( lwd_sumv_out < (SUMV_50V + p_ctrl->BcuSumvOutBak) )
                {
                    p_err->code.bit.PreCircuitOff = 1;  /*pre circuit is off*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                    
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_CIRCURT_OPEN);
                    CarCan_UpdateBmsMode(7);
                }
                else if(lwd_sumv_int < (lwd_sumv_out + SUMV_10V))
                {
                    p_err->code.bit.LoadOff = 1;  /*load is off*/
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    (void)Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                     
                    p_ctrl->PosNeedChk = HVM_FLAG_FAIL; /* do not check pos status*/
                    p_ctrl->Step = PWR_OFF_SND; /* open neg relay */
                    p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                    (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_LOAD_OPEN);
                    CarCan_UpdateBmsMode(7);
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
                    p_flg->PreFirstEnd = HVM_FLAG_OK;	 /* forbid close pos again*/
                    
                    HvM_CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
                    return_rslt = Actor_CtrlRelay(POS_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                    p_cnt->EndPre = 0;
                }
                if(p_cnt->EndPre >= HVM_CNT_50MS)
                {
                    p_cnt->EndPre = HVM_CNT_50MS;
                    HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                    return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                    BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                    
                    p_ctrl->Step = PWR_ON_P_CHK;/* check pos whether is invalid */
                    p_cnt->Step = 0;
                    p_flg->HvPowerOnEnd = HVM_FLAG_OK;
                    /* update relay life time */
                    (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                }

            }
            else
            {
               #if(1)
                /* power on failed */
                if(HVM_FLAG_FAIL == p_flg->HvPowerOnEnd)
                {
                    if(p_cnt->StartPre > HVM_CNT_500MS)
                    {
                        p_cnt->StartPre = 0 ;
           
                        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                        return_rslt = Actor_CtrlRelay(PRE_RELAY_ID, HvM_CtrlRelayCmd);
                        BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));

                        lby_iso_err = (uByte)(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR));
                        if(0 != lby_iso_err)
                        {
                            p_err->code.bit.IsoPos = 1;
                        }
                        else
                        {
                            p_err->code.bit.PreCircuitOff = 1;
                        }
                        
                        // maybe need add other reason when power fail
                        (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_PRE_CHG, ERR_LEVEL_THREE);
                        (void)Err_UpdateHwErrDetail(ERR_HW_PRE_CHG, PRE_CHG_OVERTIME);
                        
                        /* precharge error */
                        p_ctrl->Step = PWR_OFF_SND;	  /*open neg relay*/
                        p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
                        /* update relay life time */
                        (void)BatSys_RlyLifeCalc(PRE_RELAY_ID, Bat_GetCurrent(INSCUR));
                        CarCan_UpdateBmsMode(7);
                    }
                }
               #endif
            }
        }
            
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == 0)	/*vcu request power off */
        {
            p_ctrl->Step = PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }

        break;
    }
    
    case PWR_ON_P_CHK:
    {       
        /* wait 50ms for pre-relay open */
        if(p_cnt->Step >= 4)
        {
            lwd_sumv_out = Bat_GetSumV(OUT_SUMV);
            lwd_sumv_int = Bat_GetSumV(INT_SUMV);
            if((lwd_sumv_out + SUMV_20V) < lwd_sumv_int)
            {
                BatSys_UpdatePosRlySt(RLY_STA_INVALID);
                p_flg->PosRelaySt = RLY_STA_INVALID;// need check 
         		    p_err->code.bit.PosInvalid = 1;
         		    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_NOT_CLOSED, ERR_LEVEL_THREE);
         		    
         		    /* forbid power on again*/
         		    p_ctrl->Step = PWR_OFF_SND;	
                p_flg->ForbidPowerOn = HVM_FLAG_OK;
                CarCan_UpdateBmsMode(7); 
            }
            else
            {
                BatSys_UpdatePosRlySt(RLY_STA_CLOSE);
                p_flg->PosRelaySt = RLY_STA_CLOSE; 
         		    p_err->code.bit.PosInvalid = 0;
         		    
         		    p_ctrl->Step = WAIT_PWR_OFF;
         		    CarCan_UpdateBmsMode(4);	
            }
        }
        
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == 0)	/*vcu request power off */
        {
            p_ctrl->Step = PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        
        break;
    }
    
    case WAIT_PWR_OFF:
    {
        if(CarCan_GetBmsMode() < 4)
        {
            CarCan_UpdateBmsMode(4);
        }
        if((CarCan_GetBmsMode() >= 4) && (CarCan_GetBmsMode() < 7))
        {
            if(0 == CarCan_GetBmsErrLvl())
            {
                CarCan_UpdateBmsMode(4);
            }
            else if(1 == CarCan_GetBmsErrLvl())
            {
                CarCan_UpdateBmsMode(5);
            }
            else if(2 == CarCan_GetBmsErrLvl())
            {
                CarCan_UpdateBmsMode(6);
            }
            else if(3 == CarCan_GetBmsErrLvl())
            {
                CarCan_UpdateBmsMode(6);
            }
        }
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == 0)	/*vcu request power off */
        {
            p_ctrl->Step = PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        else if(BCU_CTL_P_OFF == p_ctrl->BcuCmd)  /*  bcu request power off */
        {
            p_ctrl->Step = PWR_OFF_FST_BCU;
            p_cnt->WaitVcuCmd = 0;
        }
        else
        {
            p_ctrl->Step = WAIT_PWR_OFF;
        }
        break;
    }

    case PWR_OFF_FST_VCU:
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
            /* current is safety */
            lwd_cur = Bat_GetCurrent(INSCUR);
            if( (lwd_cur < (OFFSET_CUR + CUR_10A)) && (lwd_cur > (OFFSET_CUR - CUR_10A)))
            {
                HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                return_rslt = Actor_CtrlRelay(POS_RELAY_ID, HvM_CtrlRelayCmd);
                BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                
                /* update relay life time */
                (void)BatSys_RlyLifeCalc(POS_RELAY_ID, Bat_GetCurrent(INSCUR));
                
                p_ctrl->Step = PWR_OFF_SND;
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
                    
                    p_ctrl->Step = PWR_OFF_SND;
                    DataStore_EnableEventDataStore(RLY_ERR_OPEN);
                    (void)Err_UpdateBatSysErrLevel(ERR_BATSYS_POS_OPEN_CUR, ERR_LEVEL_ONE);
                }
            }
        }
        else
        {
            p_ctrl->Step = PWR_OFF_SND;
        }
        
        break;
    }

    case PWR_OFF_FST_BCU:
    {
        if((HvM_VcuInfo.Cmd & VCU_CTL_PWR_OFF) == VCU_CTL_PWR_OFF) /* bcu request open N relay */
        {
            p_ctrl->Step = PWR_OFF_FST_VCU;
            p_cnt->OpenRelay = 0;
        }
        else
        {
            if(p_cnt->WaitVcuCmd > HVM_CNT_1S)
            {
                p_cnt->WaitVcuCmd = 0;
                p_cnt->OpenRelay = 0;
                p_ctrl->Step = PWR_OFF_FST_VCU;
                p_flg->ForbidPowerOn = HVM_FLAG_OK; /* forbid power on again*/
            }
        }

        break;
    }
    case PWR_OFF_SND:
    {
        HvM_CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
        return_rslt = Actor_CtrlRelay(NEG_RELAY_ID, HvM_CtrlRelayCmd);
        BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
        /* update relay life time */
        (void)BatSys_RlyLifeCalc(NEG_RELAY_ID, Bat_GetCurrent(INSCUR));

        p_ctrl->Step = PWR_OFF_END;
        CarCan_UpdateBmsMode(9);
        break;
    }
    case PWR_OFF_END:
    {
        LvM_UpdateSystemShutDownEn(HVM_FLAG_ENABLE);// control low power off
        /* restart */
        if((VCU_CTL_PWR_ON == (HvM_VcuInfo.Cmd & VCU_CTL_PWR_ON)) && (HVM_FLAG_FAIL == p_flg->ForbidPowerOn) )
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
    
    gby_PosPaseCnt = 0;

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
    (HvM_Cnt.NegRelayFilter < 65535) ? (HvM_Cnt.NegRelayFilter++) : (HvM_Cnt.NegRelayFilter = 65535);
    (HvM_Cnt.PRelayFilter < 65535) ? (HvM_Cnt.PRelayFilter++) : (HvM_Cnt.PRelayFilter = 65535);
}

void HvM_ModuleCbk(void)
{
#ifdef __DEBUG_TEST_HVM
    HvM_TmCnt.cnt  = 3;
#endif

    if(HvM_TmCnt.cnt >= TM_MS(10))
    {
        HvM_TmCnt.cnt = 0;
        HvM_NFDBKHvCtrl(&HvM_CtrlHv, &HvM_Flag, &HvM_Cnt, &HvM_Err);
    }
}


