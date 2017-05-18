#include "GbChgM-B.h"
#include "ChgM-B.h"
#include "GbChgCan.h"
#include "BatterySt.h"
#include "ChargeSt.h"
#include "ChgCan.h"
#include "AiIf.h"
#include "GbChg_IfDet.h"
#include "ErrorAlarm.h"
#include "DoIf.h"
#include "Elock.h"
#include "CarCan.h"

#define GAC_TEST 


#define GB_RELAY_ON  1U
#define GB_RELAY_OFF 0U
#define GB_FLAG_ENABLE  1U
#define GB_FLAG_DISABLE 0U

typedef struct
{
    uWord RxChgMsg;
    uByte CbkCnt;
} sGbChgM_TmCntType;

sGbChgM_TmCntType GbChgM_TmCnt;
sGbChgM_CtrlType GbChgM_Ctrl;

uWord gwd_CntChgLinkFail;
uWord gwd_CntAcLinkFail;
uWord gwd_AcPmtChgCur;

uWord CCS_OUT_TIME;

uByte GbChg_Mode = MODE_2011;
uWord gwd_CrmDelayTime;
uWord gwd_CroDelayTime;
uByte gby_BemTimeCnt;

extern uByte GbChgCan_GetChargerRxBcsFinish(void);
extern void GbChgCan_UpdateChargerRxBcsFinish(uByte Flg); 
extern void GbChgCan_UpdateRxFlg(uWord flg);
extern uWord GbChgCan_GetRxFlg(void);
extern uByte GbChgCan_GetCrmRxBms(void);
extern uByte GbChgCan_GetCroChgReady(void);
extern void GbChgCan_UpdateCntBst(uByte cnt);

void GbChgM_LinkModeJudge(void);
static void  GbChgM_FlowProc(void);
static void GbChgM_AcChg(void);


void GbChgM_UpdateAcPmtChgCur(uWord cur)
{
    gwd_AcPmtChgCur = cur;
}

uWord GbChgM_GetAcPmtChgCur(void)
{
    return gwd_AcPmtChgCur;
}


void GbChgM_ModuleInit(void)
{
    uByte i, *p_byte;

    p_byte = (uByte*)(&GbChgM_Ctrl);
    for(i = 0; i < sizeof(GbChgM_Ctrl); i++)
    {
        *(p_byte++) = 0;
    }

    p_byte = (uByte *)(&GbChgM_TmCnt);
    for(i = 0; i < sizeof(GbChgM_TmCnt); i++)
    {
        *(p_byte++) = 0;
    }
    gwd_CrmDelayTime = TM_MS(5000);
    gwd_CroDelayTime = TM_MS(60000);
    gby_BemTimeCnt = 0;
}

void GbChgM_ModuleCountCbk(void)
{
    (GbChgM_TmCnt.RxChgMsg < 65535) ? (GbChgM_TmCnt.RxChgMsg++) : (GbChgM_TmCnt.RxChgMsg = 65535);
    (GbChgM_TmCnt.CbkCnt < 255) ? (GbChgM_TmCnt.CbkCnt++) : (GbChgM_TmCnt.CbkCnt = 255);    
}

/******************************************************************************/
/* FUNCTION: deal gb charge control                                           */
/*                                                                            */
/* DESCRIPTION: should call this funtion every 10ms                           */
/******************************************************************************/
void GbChgM_ModuleCbk(void)
{
    /* deal GB protocol  */
    if(GbChgM_TmCnt.CbkCnt >= TM_MS(15))
    {
       GbChgM_TmCnt.CbkCnt = 0;
       GbChgM_LinkModeJudge();  
       
       GbChgM_FlowProc();
       GbChgM_AcChg();
    }
}

void GbChgM_EndReasonDeal(void)
{
    uLWord lwd_end_reason;

    lwd_end_reason = ChgM_Ctrl.ChgEndReason.LWd;

    if((lwd_end_reason & CHG_ISO_ERR) == CHG_ISO_ERR)
    {
        GbChgM_Ctrl.ChgEndReason2 |= ISO_ERR;
    }

    if((lwd_end_reason & CHG_TEMP_HIGH) == CHG_TEMP_HIGH)
    {
        GbChgM_Ctrl.ChgEndReason3 |= BAT_OVER_T;
    }

    if(((lwd_end_reason & CHG_HD_ERR) == CHG_HD_ERR)
            || ((lwd_end_reason & CHG_TEMP_LOW) == CHG_TEMP_LOW)
            || ((lwd_end_reason & CHG_RLY_ERR) == CHG_RLY_ERR))
    {
        if(GbChg_Mode == MODE_2015)
        {
          
            GbChgM_Ctrl.ChgEndReason3 |= 0x40;//OTHER_ERR;
        }
        else
        {
            GbChgM_Ctrl.ChgEndReason3 |= 0x04;
        }
    }

    if((lwd_end_reason & CHG_CUR_HIGH) == CHG_CUR_HIGH)
    {
        GbChgM_Ctrl.ChgEndReason4 |= OVER_CUR;
    }

    if( ((lwd_end_reason & CHG_CELLV_HIGH) == CHG_CELLV_HIGH)
            || ((lwd_end_reason & CHG_SUMV_HIGH) == CHG_SUMV_HIGH))
            //|| ((lwd_end_reason & CHG_SUMV_LOW) == CHG_SUMV_LOW))
    {
        GbChgM_Ctrl.ChgEndReason4 |= OVER_VOLT;
    }
}

uWord GbChgM_ACPmtCurCalc(void)
{
    uWord lwd_cur;
    uWord lwd_ac_cur;

    lwd_cur = Chg_GetPermitChgCurOn();
    lwd_ac_cur = GbChgM_GetAcPmtChgCur();
    if(lwd_cur > lwd_ac_cur)
    {
        lwd_cur = lwd_ac_cur;
    }

    return lwd_cur;
}

void GbChgM_StopReasonJudge(void)
{
    uWord lwd_sum_v;
    uWord lwd_max_v;
    uWord lwd_max_pmt_sum_v_thrhd;
    uWord lwd_max_pmt_cell_v_thrhd;
    uByte wtmp;

    lwd_sum_v = Bat_GetSumV(INT_SUMV);
    lwd_max_pmt_sum_v_thrhd = Chg_GetPermitMaxSumV();
    lwd_max_v = Bat_GetMaxV();
    lwd_max_pmt_cell_v_thrhd = Chg_GetPermitMaxCellV();

    if(lwd_sum_v >= lwd_max_pmt_sum_v_thrhd)
    {
        GbChgM_Ctrl.ChgEndReason1 |= REACH_SUMV;
    }
    if(lwd_max_v >= lwd_max_pmt_cell_v_thrhd)
    {
        GbChgM_Ctrl.ChgEndReason1 |= REACH_CELLV;
    }
    
    wtmp = Chg_GetChargerFault();
    if(wtmp != 0)
    {
        GbChgM_Ctrl.ChgEndReason1 |= RX_CST;     //GB27930-2015    
    }
}

/* AC mode or DC mode */
static void GbChg_ModeJudge(uByte *p_gb_chg_mode,  uWord *link_fail_cnt)
{
    uWord lwd_cc_volt;
    uWord lwd_cc2_volt;
    uByte lby_cp_signal;
    static uByte lby_cp_flg = 0;
  
    GbChgDet_Ai(AIIF_CPUAI_DC_P2_NO, GAIN_VOLT, &lwd_cc2_volt);
    GbChgDet_Ai(AIIF_CPUAI_AC_P3_NO, GAIN_VOLT, &lwd_cc_volt);
    GbChgDet_DutyCalc(&lby_cp_signal);
   
    Chg_UpdateCCVolt(lwd_cc_volt);
    Chg_UpdateCc2Volt(lwd_cc2_volt);
    Chg_UpdateCpDutyCycle(lby_cp_signal);

 
   if( ( ((lwd_cc_volt > CC_V_L_32AMODE) && (lwd_cc_volt < CC_V_H_32AMODE)) 
            || ((lwd_cc_volt > CC_V_L_16AMODE) && (lwd_cc_volt < CC_V_H_16AMODE))
            || ((lwd_cc_volt > CC_V_L_10AMODE) && (lwd_cc_volt < CC_V_H_10AMODE))
            || ((lwd_cc_volt > CC_V_L_63AMODE) && (lwd_cc_volt < CC_V_H_63AMODE))) 
            && (0 != lby_cp_signal)  && (lwd_cc2_volt > CON_CONFLICT_V) )        
    {
         lby_cp_flg = 1;
         *p_gb_chg_mode = AC_CHG_MODE2_B;
    }
    else if ( ( ((lwd_cc_volt > CC_V_L_32AMODE) && (lwd_cc_volt < CC_V_H_32AMODE)) // 32A
            || ((lwd_cc_volt > CC_V_L_16AMODE) && (lwd_cc_volt < CC_V_H_16AMODE)) // 16A
            || ((lwd_cc_volt > CC_V_L_10AMODE) && (lwd_cc_volt < CC_V_H_10AMODE)) //10A
            || ((lwd_cc_volt > CC_V_L_63AMODE) && (lwd_cc_volt < CC_V_H_63AMODE))) // 63A
            && (0 == lby_cp_signal)  && (lwd_cc2_volt > CON_CONFLICT_V) && (lby_cp_flg == 0) ) 
    {
         *p_gb_chg_mode = AC_CHG_MODE1_B; 
    }
    else if(( (lwd_cc2_volt > CC2_V_L) && (lwd_cc2_volt < CC2_V_H) ) && (lwd_cc_volt > CON_CONFLICT_V) )
    {
         *p_gb_chg_mode = DC_CHG_MODE;
    }
    else if((((lwd_cc_volt > CC_V_L_16AMODE) && (lwd_cc_volt < CC_V_H_16AMODE)) // 16A
             || ((lwd_cc_volt > CC_V_L_32AMODE) && (lwd_cc_volt < CC_V_H_32AMODE)) // 32A
             || ((lwd_cc_volt > CC_V_L_10AMODE) && (lwd_cc_volt < CC_V_H_10AMODE)) //10A
             || ((lwd_cc_volt > CC_V_L_63AMODE) && (lwd_cc_volt < CC_V_H_63AMODE))  //63A
             || (0 != lby_cp_signal) )  && ( (lwd_cc2_volt > CC2_V_L) && (lwd_cc2_volt < CC2_V_H) ))
    {
         *p_gb_chg_mode = CHG_MODE_CONFLICT;
    }
    else
    {
        if((*link_fail_cnt) < CNT_DET_LINK)
        {
            (*link_fail_cnt)++;                                                                                                                                          
        }
        else
        {
            *p_gb_chg_mode = NO_CHG_CON;
            (*link_fail_cnt) = CNT_DET_LINK;
        }
    }
}

void GbChgM_LinkModeJudge(void)
{
    uWord lwd_cp_cur = 0;
    uWord lwd_cc_cur = 0;
    uByte lby_cp_duty = 0;
    uWord lwd_cc_volt = 0;

    uByte lby_gb_chg_mode = 0;
    uByte lby_dc_link_st = 0;
    uByte lby_ac_link_st = 0;
    uWord lwd_ac_chg_pmt_cur = 0;

    GbChg_ModeJudge(&lby_gb_chg_mode, &gwd_CntChgLinkFail);
    lwd_cc_volt = Chg_GetCcVolt();
    lby_cp_duty = Chg_GetCpDutyCycle();
    
    switch(lby_gb_chg_mode)
    {
      case DC_CHG_MODE:
      {
          lby_dc_link_st = GB_FLAG_ENABLE;
          lby_ac_link_st = GB_FLAG_DISABLE;
          lwd_cc_cur = 0;
          lwd_ac_chg_pmt_cur = 0;
          break;
      }

      case AC_CHG_MODE1_B:          /* family use  */
      case AC_CHG_MODE3_A:
      {  
          if((lwd_cc_volt > CC_V_L_16AMODE) && (lwd_cc_volt < CC_V_H_16AMODE))  /* cap 16A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 160;
              
          }
          else if((lwd_cc_volt > CC_V_L_32AMODE) && (lwd_cc_volt < CC_V_H_32AMODE) ) /* cap 32A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 320;
              
          }
          else if((lwd_cc_volt > CC_V_L_63AMODE) && (lwd_cc_volt < CC_V_H_63AMODE) ) /* cap 63A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 630;
              
          }
          else if((lwd_cc_volt > CC_V_L_10AMODE) && (lwd_cc_volt < CC_V_H_10AMODE) ) /* cap 10A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 100;
             
          }
          else
          {
              lby_ac_link_st = GB_FLAG_DISABLE;
              lwd_cc_cur = 0;
          }
          lby_dc_link_st = GB_FLAG_DISABLE;
          lwd_cp_cur = 0;
          lwd_ac_chg_pmt_cur = lwd_cc_cur;

          break;
      }

      case AC_CHG_MODE2_B:		    /*the same as AC_CHG_MODE3_C */
      case AC_CHG_MODE3_B:				/*the same as AC_CHG_MODE3_C */
      case AC_CHG_MODE3_C:
      {
          if((lwd_cc_volt > CC_V_L_16AMODE) && (lwd_cc_volt < CC_V_H_16AMODE))   /* cap 16A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 160;
          }
          else if((lwd_cc_volt > CC_V_L_32AMODE) && (lwd_cc_volt < CC_V_H_32AMODE))   /* cap 32A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 320;
          }
          else if((lwd_cc_volt > CC_V_L_63AMODE) && (lwd_cc_volt < CC_V_H_63AMODE) ) /* cap 63A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 630;
          }
           else if((lwd_cc_volt > CC_V_L_10AMODE) && (lwd_cc_volt < CC_V_H_10AMODE) ) /* cap 10A */
          {
              lby_ac_link_st = GB_FLAG_ENABLE;
              lwd_cc_cur = 100;
          }
          else
          {
              lby_ac_link_st = GB_FLAG_DISABLE;
              lwd_cc_cur = 0;
          }

          /* judge cp_signal current   AC_curent */
          if((lby_cp_duty <= 100) && (lby_cp_duty > 0))
          {
              lwd_cp_cur = (uWord)lby_cp_duty * 8;   
          }
          else
          {
              lby_ac_link_st = GB_FLAG_DISABLE;
              lwd_cp_cur = 0;
          }
          
          if(lwd_cc_cur > lwd_cp_cur)
          {
              lwd_ac_chg_pmt_cur = lwd_cp_cur;
          }
          else
          {
              lwd_ac_chg_pmt_cur = lwd_cc_cur;
          }
          lby_dc_link_st = GB_FLAG_DISABLE;   
          break;
      }

      case AC_CHG_CON_ERR:							  /*the same as  CHG_MODE_CONFLICT */
      case AC_CHG_LOCK_ERR:								/*the same as  CHG_MODE_CONFLICT */
      case CHG_MODE_CONFLICT:
      {
          lby_dc_link_st = GB_FLAG_DISABLE;
          lby_ac_link_st = GB_FLAG_DISABLE;
          lwd_cc_cur = 0;
          lwd_ac_chg_pmt_cur = 0;
          break;
      }

      default:
      {
          lby_dc_link_st = GB_FLAG_DISABLE;
          lby_ac_link_st = GB_FLAG_DISABLE;
          lwd_cc_cur = 0;
          lwd_ac_chg_pmt_cur = 0;
          break;
      }
    }  
 #ifdef GAC_TEST
   // lby_dc_link_st = GB_FLAG_ENABLE;
   //lby_ac_link_st = GB_FLAG_ENABLE;
 #endif 
    Chg_UpdateDcLinkSt(lby_dc_link_st);
    if(1 == CarCan_GetVcuAcChargeCCSt())
    {
        lby_ac_link_st = GB_FLAG_ENABLE;
    }
    else
    {
        lby_ac_link_st = GB_FLAG_DISABLE;
    }
    
    Chg_UpdateAcLinkSt(lby_ac_link_st);
    if(1 == CarCan_GetVcuChargerCapaSt())
    {
        lwd_cc_cur = 100;
    }
    else if(2 == CarCan_GetVcuChargerCapaSt())
    {
        lwd_cc_cur = 160;
    }
    else if(3 == CarCan_GetVcuChargerCapaSt())
    {
        lwd_cc_cur = 320;
    }
    else if(4 == CarCan_GetVcuChargerCapaSt())
    {
        lwd_cc_cur = 640;
    }
    else
    {
        lwd_cc_cur = 0;
    }
    lwd_ac_chg_pmt_cur = lwd_cc_cur;
    Chg_UpdateCableCap(lwd_cc_cur); 
    Chg_UpdateCpPmtCur(lwd_cp_cur);
    GbChgM_UpdateAcPmtChgCur(lwd_ac_chg_pmt_cur);
}

void GbChgM_FlowProc(void)
{
    uByte lby_dc_link_st;
    uByte lby_chg_enable_ext;
    uWord lwd_rx_flg;
    uWord DC_ChgMaxOut,DC_ChgMinOut,wtmp;

    sGbChgM_CtrlType *pGbChgCtrl;
    pGbChgCtrl = &GbChgM_Ctrl;

    lby_dc_link_st = Chg_GetDcLinkSt();
    lby_chg_enable_ext = Chg_GetPermitChgExtSt();
    lwd_rx_flg = GbChgCan_GetRxFlg();
    

#ifdef __DEBUG_GB_CHG_TEST
    lby_dc_link_st = 1;
    lby_chg_enable_ext = 1;

#endif
    switch (pGbChgCtrl->ChgStep)
    {
    case ChgStep0:
    {
        if(GB_FLAG_ENABLE == lby_dc_link_st)
        {
            CarCan_UpdateDcChargerComErr(0);
            if(CHM_RX == (lwd_rx_flg & CHM_RX))  // GB27930-2015
            {
                GbChgM_TmCnt.RxChgMsg = 0;
                gwd_CrmDelayTime = TM_MS(60000);
                lwd_rx_flg = 0;
                if(CarCan_GetBmsMode() >= 2)
                {  
                    pGbChgCtrl->Flg.Bhm = GB_FLAG_ENABLE;	 /* send Bhm enable */
                }
                //pGbChgCtrl->ChgStep = ChgStep0; //2016-8-10

                GbChgCan_UpdateChargerRxBcsFinish(0);
                /* need clear error information when re-handshake successful  */
                pGbChgCtrl->Flg.Bem = GB_FLAG_DISABLE;  /* send Bem disable  */
                pGbChgCtrl->HandShakeErr = 0;
                pGbChgCtrl->ConfigErr = 0;
                pGbChgCtrl->ChargingErr = 0;
                pGbChgCtrl->ChargeEndErr = 0;    
            } 
            if( ((CRM_RX == (lwd_rx_flg & CRM_RX)))
                    && (CHG_NOT_RX_BMS == GbChgCan_GetCrmRxBms() ) )/* charger has Not recognize BMS */
            {
                lwd_rx_flg = 0;
                pGbChgCtrl->Flg.Bhm = GB_FLAG_DISABLE;	 /* send Bhm disable */
                pGbChgCtrl->Flg.Brm = GB_FLAG_ENABLE;	 /* send Brm enable */
                pGbChgCtrl->ChgStep = ChgStep1;

                GbChgCan_UpdateChargerRxBcsFinish(0);
                /* need clear error information when re-handshake successful  */
                pGbChgCtrl->Flg.Bem = GB_FLAG_DISABLE;  /* send Bem disable  */
                pGbChgCtrl->HandShakeErr = 0;
                pGbChgCtrl->ConfigErr = 0;
                pGbChgCtrl->ChargingErr = 0;
                pGbChgCtrl->ChargeEndErr = 0;
            }
            else
            {
                if(GbChgM_TmCnt.RxChgMsg < gwd_CrmDelayTime)
                {
                    pGbChgCtrl->ChgStep = ChgStep0;
                }
                else
                {
                    pGbChgCtrl->HandShakeErr |= RX_CRM0_ERR;
                    pGbChgCtrl->ChgStep = ChgStep9;
                }  
            }
        }

        break;
    }

    case ChgStep1:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }

        if( ((CRM_RX == (lwd_rx_flg & CRM_RX)))
                && (CHG_RX_BMS == GbChgCan_GetCrmRxBms()))//GbChgCan_RxData.Crm.RxBms) )/* charger recognize BMS */
        {
            lwd_rx_flg = 0;
            pGbChgCtrl->Flg.Brm = GB_FLAG_DISABLE;	 /* send Brm disable */
            pGbChgCtrl->ChgStep = ChgStep2;
        }
        else
        {
            if(GbChgM_TmCnt.RxChgMsg < TM_MS(5000))
            {
                pGbChgCtrl->Flg.Brm = GB_FLAG_ENABLE;	 /* send Brm enable */
                pGbChgCtrl->ChgStep = ChgStep1;
            }
            else
            {
                pGbChgCtrl->Flg.Brm = GB_FLAG_DISABLE;	 /* send Brm disable  */
                pGbChgCtrl->HandShakeErr |= RX_CRM1_ERR;
                pGbChgCtrl->ChgStep = ChgStep9;
            }
        }

        break;
    }

    case ChgStep2:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }

        pGbChgCtrl->Flg.Bcp = GB_FLAG_ENABLE;	 /* send Bcp enable */

        /* receive charger time and max output ability */
        if ( (CTS_RX == (lwd_rx_flg & CTS_RX))
                && (CML_RX == (lwd_rx_flg & CML_RX)))
        {
            lwd_rx_flg &= ~CTS_RX;
            lwd_rx_flg &= ~CML_RX;

            pGbChgCtrl->Flg.Bcp = GB_FLAG_DISABLE;	 /* send Bcp disable */
            pGbChgCtrl->ChgStep = ChgStep3;
        }
        else
        {
            if(GbChgM_TmCnt.RxChgMsg < TM_MS(5000))
            {
                pGbChgCtrl->Flg.Bcp = GB_FLAG_ENABLE;	 /* send Bcp enable */
                pGbChgCtrl->ChgStep = ChgStep2;
            }
            else
            {
                pGbChgCtrl->Flg.Bcp = GB_FLAG_DISABLE;	 /* send Bcp disable */
                pGbChgCtrl->ConfigErr |= RX_CTS_CML_ERR;
                pGbChgCtrl->ChgStep = ChgStep9;
            }
        }

        break;
    }

    case ChgStep3:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }

        // need modify
        if(1 == Chg_GetDcLinkSt())
        {
            DC_ChgMaxOut= Chg_GetMaxOutPutVoltage();
            wtmp= Chg_GetPermitMaxSumV();
            DC_ChgMinOut = Chg_GetMinOutPutVoltage();
            if((wtmp >= DC_ChgMaxOut) || (wtmp <= DC_ChgMinOut))
            {
                CarCan_UpdateDcChargeConfig(0);
                pGbChgCtrl->ChgStep = ChgStep9;
            } 
            else
            {
                CarCan_UpdateDcChargeConfig(1);
            }
        }
        
        if((GB_FLAG_ENABLE == lby_chg_enable_ext) && (1 == CarCan_GetDcChargeConfig())) /* bms Ready */
        {
            if(GB_RELAY_ON == ChgM_Ctrl.Flag.ChgRlyCtrl)
            {
                pGbChgCtrl->Flg.Bro = GB_FLAG_ENABLE;	 /* send Bro enable */
                pGbChgCtrl->ChgStep = ChgStep4;
            }
            else
            {
                break;
            }
        }
        else                                   /* bms Not Ready */
        {
            pGbChgCtrl->Flg.Bro = GB_FLAG_ENABLE;	 /* send Bro enable */

            pGbChgCtrl->ChgStep = ChgStep3;
        }

        break;
    }

    case ChgStep4:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }
        if(GB_RELAY_ON == ChgM_Ctrl.Flag.ChgRlyCtrl)
        {
            gwd_CroDelayTime = TM_MS(5000);
        }
        if( (CRO_RX == (lwd_rx_flg & CRO_RX))
                && (CHG_READY == GbChgCan_GetCroChgReady()))/* charger Ready */
        {
            lwd_rx_flg &= ~CRO_RX;
            pGbChgCtrl->Flg.Bro = GB_FLAG_DISABLE;	 /* send Bro disable */
            pGbChgCtrl->ChgStep = ChgStep5;
        }
        else
        {
            
            if(GbChgM_TmCnt.RxChgMsg < gwd_CroDelayTime)
            {
                pGbChgCtrl->Flg.Bro = GB_FLAG_ENABLE;	 /* send Bro enable */
                pGbChgCtrl->ChgStep = ChgStep4;
            }
            else
            {
                pGbChgCtrl->Flg.Bro = GB_FLAG_DISABLE;	 /* send Bro disable */
                pGbChgCtrl->ConfigErr |= RX_CRO_ERR;
                pGbChgCtrl->ChgStep = ChgStep9;
                CarCan_UpdateDcChargeConfig(0);
            }
        }

        break;
    }

    case ChgStep5:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep) 
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }
        if(GB_FLAG_DISABLE == GbChgCan_GetChargerRxBcsFinish())
        {
            CCS_OUT_TIME = TM_MS(5000);
        }
        else
        {
            CCS_OUT_TIME = TM_MS(1000);
        }

        pGbChgCtrl->Flg.Bcl = GB_FLAG_ENABLE;	 /* send Bcl enable */
        pGbChgCtrl->Flg.Bcs = GB_FLAG_ENABLE;	 /* send Bcs enable */

        GbChgCan_UpdateCntBst(0);
        // GbChgCan_TmCnt.Bst = 0; /* don't send Bst in this step */

        if( CCS_RX == (lwd_rx_flg & CCS_RX) ) /* charger status */
        {
            lwd_rx_flg &= ~CCS_RX;
            GbChgM_TmCnt.RxChgMsg = 0;

            pGbChgCtrl->Flg.Bsm = GB_FLAG_ENABLE;	 /* send Bsm enable */
            pGbChgCtrl->Flg.Bmv = GB_FLAG_ENABLE;	 /* send Bmv enable */
            pGbChgCtrl->Flg.Bmt = GB_FLAG_ENABLE;	 /* send Bmt enable */
            //pGbChgCtrl->Flg.Bsp = GB_FLAG_ENABLE;	 /* send Bsp enable */

            pGbChgCtrl->ChgStep = ChgStep5;
        }
        else
        {
            if(GbChgM_TmCnt.RxChgMsg < CCS_OUT_TIME)
            {
                pGbChgCtrl->ChgStep = ChgStep5;
            }
            else
            {
                pGbChgCtrl->Flg.Bcl = GB_FLAG_DISABLE;	 /* send Bcl disable */
                pGbChgCtrl->Flg.Bcs = GB_FLAG_DISABLE;	 /* send Bcs disable */
                pGbChgCtrl->Flg.Bsm = GB_FLAG_DISABLE;	 /* send Bsm disable */
                pGbChgCtrl->Flg.Bmv = GB_FLAG_DISABLE;	 /* send Bmv disable */
                pGbChgCtrl->Flg.Bmt = GB_FLAG_DISABLE;	 /* send Bmt disable */
                //pGbChgCtrl->Flg.Bsp = GB_FLAG_DISABLE;	 /* send Bsp disable */

                //pGbChgCtrl->ChgEndReason3 |= OTHER_ERR; /* other error*/
                if(GbChg_Mode == MODE_2015)
                {
                  
                    GbChgM_Ctrl.ChgEndReason3 |= 0x40;//OTHER_ERR;
                }
                else
                {
                    GbChgM_Ctrl.ChgEndReason3 |= 0x04;
                }
                
                pGbChgCtrl->Flg.Bst = GB_FLAG_ENABLE;	 /* send Bst enable */
                pGbChgCtrl->ChargingErr |= RX_CCS_ERR;
                pGbChgCtrl->ChgStep = ChgStep9;
            }
        }

        if( (GB_FLAG_DISABLE == lby_chg_enable_ext)		 /* BMS charge end */
                || (CST_RX == (lwd_rx_flg & CST_RX))	 /* charger charge end */
                || (GB_FLAG_DISABLE == lby_dc_link_st) 
                || (0 == CarCan_GetVcuDcChargeCom())) /* charge link open */
        {
            if(CST_RX == (lwd_rx_flg & CST_RX))
            {
                GbChgM_Ctrl.ChgEndReason1 |= RX_CST;
            }
            
            lwd_rx_flg &= ~CST_RX;
            pGbChgCtrl->ChgStep = ChgStep6;
            pGbChgCtrl->Flg.Bst = GB_FLAG_ENABLE;	 /* send Bst enable */

            if(GB_FLAG_DISABLE == lby_dc_link_st)
            {
                pGbChgCtrl->ChgEndReason2 |= CHG_CON_ERR; /* charge connector error */
                pGbChgCtrl->ChgEndReason3 |= CC2VOLT_ERR; 
            }
            CarCan_UpdateBmsMode(8);
        }

        break;
    }

    case ChgStep6:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }

        pGbChgCtrl->Flg.Bst = GB_FLAG_ENABLE;	 /* send Bst enable */

        pGbChgCtrl->Flg.Bcl = GB_FLAG_DISABLE;	 /* send Bcl disable */
        pGbChgCtrl->Flg.Bcs = GB_FLAG_DISABLE;	 /* send Bcs disable */
        pGbChgCtrl->Flg.Bsm = GB_FLAG_DISABLE;	 /* send Bsm disable */
        pGbChgCtrl->Flg.Bmv = GB_FLAG_DISABLE;	 /* send Bmv disable */
        pGbChgCtrl->Flg.Bmt = GB_FLAG_DISABLE;	 /* send Bmt disable */
        //pGbChgCtrl->Flg.Bsp = GB_FLAG_DISABLE;	 /* send Bsp disable */

        if( (CST_RX == (lwd_rx_flg & CST_RX)) ) /* charger stop msg */
        {
            lwd_rx_flg &= ~CST_RX;

            pGbChgCtrl->ChgStep = ChgStep7;
        }
        else
        {
            if(GbChgM_TmCnt.RxChgMsg < TM_MS(5000))
            {
                pGbChgCtrl->ChgStep = ChgStep6;
            }
            else
            {
                pGbChgCtrl->ChargingErr |= RX_CST_ERR;
                pGbChgCtrl->Flg.Bst = GB_FLAG_DISABLE;	 /* send Bst disable */
                pGbChgCtrl->ChgStep = ChgStep9;
            }
        }

        break;
    }

    case ChgStep7:
    {
        if(pGbChgCtrl->ChgStepBak != pGbChgCtrl->ChgStep)
        {
            pGbChgCtrl->ChgStepBak = pGbChgCtrl->ChgStep;
            GbChgM_TmCnt.RxChgMsg = 0;
        }

        pGbChgCtrl->Flg.Bst = GB_FLAG_DISABLE; /* send Bst disable */
        pGbChgCtrl->Flg.Bsd = GB_FLAG_ENABLE;	 /* send Bsd enable */

        if( CSD_RX == (lwd_rx_flg & CSD_RX) )
        {
            lwd_rx_flg &= ~CSD_RX;
            pGbChgCtrl->ChgStep = ChgStep8;
        }
        else
        {
            if(GbChgM_TmCnt.RxChgMsg < TM_MS(5000))
            {
                pGbChgCtrl->ChgStep = ChgStep7;
            }
            else
            {
                pGbChgCtrl->ChargeEndErr |= RX_CSD_ERR;
                pGbChgCtrl->Flg.Bsd = GB_FLAG_DISABLE;	 // send Bsd disable
                pGbChgCtrl->ChgStep = ChgStep9;
            }
        }

        break;
    }

    case ChgStep8:		//charge Nomal end
    {
        GbChgM_TmCnt.RxChgMsg = 0;
        pGbChgCtrl->Flg.Bsd = GB_FLAG_ENABLE;	 /* send Bsd enable */

        if((CRM_RX == (lwd_rx_flg & CRM_RX)) || (CHM_RX == (lwd_rx_flg & CHM_RX)))
        {
            lwd_rx_flg = 0;
            pGbChgCtrl->Flg.Bsd = GB_FLAG_DISABLE; /* send Bsd disable */
            pGbChgCtrl->ChgStep = ChgStep0;   /* wait handshake again */
            pGbChgCtrl->ChgEndReason1 = 0;
            pGbChgCtrl->ChgEndReason2 = 0;
            pGbChgCtrl->ChgEndReason3 = 0;
            pGbChgCtrl->ChgEndReason4 = 0;
            
            //pGbChgCtrl->Flg.Bem = GB_FLAG_DISABLE;  /* send Bem disable  */
            pGbChgCtrl->HandShakeErr = 0;
            pGbChgCtrl->ConfigErr = 0;
            pGbChgCtrl->ChargingErr = 0;
            pGbChgCtrl->ChargeEndErr = 0;
        }

        break;
    }

    case ChgStep9:  /* charge error end */
    {
        GbChgM_TmCnt.RxChgMsg = 0;
        pGbChgCtrl->Flg.Bem = GB_FLAG_ENABLE;	 /* send Bem enable */
        CarCan_UpdateDcChargerComErr(1);
        gby_BemTimeCnt++;
        if(gby_BemTimeCnt > 3)
        {
            gby_BemTimeCnt = 3;
            pGbChgCtrl->Flg.Bem = GB_FLAG_DISABLE;	 /* send Bem enable */
            pGbChgCtrl->ChgStep = ChgStep6;
            CarCan_UpdateDcChErrLvl(1);
        }

        if((CRM_RX == (lwd_rx_flg & CRM_RX) && (CHG_NOT_RX_BMS == GbChgCan_GetCrmRxBms())) || (CHM_RX == (lwd_rx_flg & CHM_RX)))
        {
            lwd_rx_flg = 0;
            pGbChgCtrl->Flg.Bst = GB_FLAG_DISABLE; /* send Bst disable */
            pGbChgCtrl->ChgStep = ChgStep0;   /* wait handshake again */
            pGbChgCtrl->ChgEndReason1 = 0;
            pGbChgCtrl->ChgEndReason2 = 0;
            pGbChgCtrl->ChgEndReason3 = 0;
            pGbChgCtrl->ChgEndReason4 = 0;
            
            pGbChgCtrl->Flg.Bem = GB_FLAG_DISABLE;  /* send Bem disable  */
            pGbChgCtrl->HandShakeErr = 0;
            pGbChgCtrl->ConfigErr = 0;
            pGbChgCtrl->ChargingErr = 0;
            pGbChgCtrl->ChargeEndErr = 0;
        }
        

        break;
    }

    default:
    {
        break;
    }
    }

    GbChgCan_UpdateRxFlg(lwd_rx_flg);

}


static void GbChgM_AcChg(void)
{
    uByte lby_ac_link_st;
    uByte lby_chg_enable_en;
    uWord lwd_ac_chg_pmt_cur;
    uWord rslt;

    lby_ac_link_st = Chg_GetAcLinkSt();
    lby_chg_enable_en = Chg_GetPermitChgOnSt();

    if(GB_FLAG_ENABLE == lby_ac_link_st)
    {
        gwd_CntAcLinkFail = 0;
        if(GB_FLAG_ENABLE == lby_chg_enable_en)
        {
            // CHG_S2 = ON;
            rslt = DoIf_Ctrl((uByte)DOIF_CPU_CHG_S_NO, (bool)CHG_S_ON);
        }
        else
        {
            //  CHG_S2 = OFF;
            rslt = DoIf_Ctrl((uByte)DOIF_CPU_CHG_S_NO, (bool)CHG_S_OFF);
        }
    }
    else
    {
        if(gwd_CntAcLinkFail < LINK_FAIL_300MS)
        {
            gwd_CntAcLinkFail++;
        }
        else
        {
            gwd_CntAcLinkFail = 0;
            lby_chg_enable_en = GB_FLAG_DISABLE;
            lwd_ac_chg_pmt_cur = 0;
            Chg_UpdatePermitChgOnSt(lby_chg_enable_en);
            GbChgM_UpdateAcPmtChgCur(lwd_ac_chg_pmt_cur);
            //  CHG_S2 = OFF;
            rslt = DoIf_Ctrl((uByte)DOIF_CPU_CHG_S_NO, (bool)CHG_S_OFF);
        }
    }
}
