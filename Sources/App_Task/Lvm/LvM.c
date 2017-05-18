// LvM.c
#include "LvM.h"
#include "BmsSysSt.h"
#include "DoIf.h"
#include "SchIf.h"
#include "EE.h"
#include "SysConf.h"
#include "RunModem.h"
#include "BatterySysSt.h"
#include "IntCan.h"
#include "CanIf_Type.h"
#include "CanIf.h"

#include "DataStore.h"
#include "ClockIf.h"

#define LVM_TM_1S              TM_MS(1000)
#define LVM_FLAG_OK            1
#define LVM_FLAG_FAIL          0
#define TM_5MIN                TM_MS(300000)
#define LVM_FLAG_ENABLE        1
#define LVM_FLAG_DISABLE       0
#define LVM_OFF_STATE_CNT      5
#define LVM_ON_STATE_CNT       5

uByte  flag_test_k15;

typedef struct
{
    uByte cnt;
    uByte pow_down_cnt;
    uWord save_time_cnt;
} sLvM_TmCntType;

typedef struct
{
    uByte Bak;
    uByte BakupFlag;
} sLvM_RunModeMType;

typedef struct
{
    uByte OnCnt;
    uByte OffCnt;
    uByte St;
}sLvM_SnglChkType;

typedef struct
{
    sLvM_SnglChkType K15;    
    sLvM_SnglChkType OffBDChg;
    sLvM_SnglChkType OnBDChg; 
    sLvM_SnglChkType Alarm;
#ifdef LVM_CAN_ACTIVE_ENABLE   
    sLvM_SnglChkType CarActive;
#endif    
}sLvM_ActSgnlChkType;

sLvM_ActSgnlChkType LvM_ActSgnlChk;

sLvM_TmCntType LvM_TmCnt;
uByte gby_KeyResetFlag;
sLvM_RunModeMType LvM_RunModeM;

uByte gby_BcuShutDownEn;
uByte gby_VcuShutDownEn;
uByte gby_SystemShutDownEn;
uWord tm_test_on = 0;


static void LvM_SaveSystemTime(void)
{
    GeneralReturnType return_rslt;

    return_rslt = BmsSys_UpdatePowerOffTime();

    (void)EE_SaveVar(BOTHEE, SYSTEMST_POWEROFF_RESET_INFO);
}

static uByte LvM_DetectPowState()
{
    uWord volt;

    uByte rslt;

    volt = BmsSys_GetK15Volt();
    if(volt < LVM_STATE_OFF_THRED)
    {
        LvM_ActSgnlChk.K15.OffCnt++;
        LvM_ActSgnlChk.K15.OnCnt = 0;
    }
    else
    {
        LvM_ActSgnlChk.K15.OffCnt = 0;
        LvM_ActSgnlChk.K15.OnCnt++;

    }
    if(LvM_ActSgnlChk.K15.OffCnt >= LVM_OFF_STATE_CNT)
    {
        LvM_ActSgnlChk.K15.OffCnt = LVM_OFF_STATE_CNT;
        LvM_ActSgnlChk.K15.St = LVM_STATE_OFF;
    }
    else if(LvM_ActSgnlChk.K15.OnCnt >= LVM_ON_STATE_CNT)
    {
        LvM_ActSgnlChk.K15.OnCnt = LVM_ON_STATE_CNT;
        LvM_ActSgnlChk.K15.St = LVM_STATE_ON;
    }
    BmsSys_UpdateK15St(LvM_ActSgnlChk.K15.St);

    volt = BmsSys_GetOffBDChgVolt();
    if(volt < LVM_STATE_OFF_THRED)
    {
        LvM_ActSgnlChk.OffBDChg.OffCnt++;
        LvM_ActSgnlChk.OffBDChg.OnCnt = 0;
    }
    else
    {
        LvM_ActSgnlChk.OffBDChg.OffCnt = 0;
        LvM_ActSgnlChk.OffBDChg.OnCnt++;
    }
    
    if (LvM_ActSgnlChk.OffBDChg.OffCnt >= LVM_OFF_STATE_CNT)
    {
        LvM_ActSgnlChk.OffBDChg.OffCnt = LVM_OFF_STATE_CNT;
        LvM_ActSgnlChk.OffBDChg.St = LVM_STATE_OFF;
    }
    else if(LvM_ActSgnlChk.OffBDChg.OnCnt >= LVM_ON_STATE_CNT)
    {
        LvM_ActSgnlChk.OffBDChg.OnCnt = LVM_ON_STATE_CNT;
        LvM_ActSgnlChk.OffBDChg.St = LVM_STATE_ON;
    }

    BmsSys_UpdateOffBDChgSt(LvM_ActSgnlChk.OffBDChg.St);

    volt = BmsSys_GetOnBDChgVolt();
    if(volt < LVM_STATE_OFF_THRED)
    {
        LvM_ActSgnlChk.OnBDChg.OffCnt++;
        LvM_ActSgnlChk.OnBDChg.OnCnt = 0;
    }
    else
    {
        LvM_ActSgnlChk.OnBDChg.OffCnt = 0;
        LvM_ActSgnlChk.OnBDChg.OnCnt++;
    }
    if (LvM_ActSgnlChk.OnBDChg.OffCnt >= LVM_OFF_STATE_CNT)
    {
        LvM_ActSgnlChk.OnBDChg.OffCnt = LVM_OFF_STATE_CNT;
        LvM_ActSgnlChk.OnBDChg.St = LVM_STATE_OFF;
    }
    else if(LvM_ActSgnlChk.OnBDChg.OnCnt >= LVM_ON_STATE_CNT)
    {
        LvM_ActSgnlChk.OnBDChg.OnCnt = LVM_ON_STATE_CNT;
        LvM_ActSgnlChk.OnBDChg.St = LVM_STATE_ON;
    }

    BmsSys_UpdateOnBDChgSt(LvM_ActSgnlChk.OnBDChg.St);

    volt = BmsSys_GetAlarmVolt();
    if(volt < LVM_STATE_OFF_THRED)
    {
        LvM_ActSgnlChk.Alarm.OffCnt++;
        LvM_ActSgnlChk.Alarm.OnCnt = 0;
    }
    else
    {
        LvM_ActSgnlChk.Alarm.OffCnt = 0;
        LvM_ActSgnlChk.Alarm.OnCnt++;
    }
    
    if (LvM_ActSgnlChk.Alarm.OffCnt >= LVM_OFF_STATE_CNT)
    {
        LvM_ActSgnlChk.Alarm.OffCnt = LVM_OFF_STATE_CNT;
        LvM_ActSgnlChk.Alarm.St = LVM_STATE_OFF;
    }
    else if(LvM_ActSgnlChk.Alarm.OnCnt >= LVM_ON_STATE_CNT)
    {
        LvM_ActSgnlChk.Alarm.OnCnt = LVM_ON_STATE_CNT;
        LvM_ActSgnlChk.Alarm.St = LVM_STATE_ON;
    }

    BmsSys_UpdateAlarmSt(LvM_ActSgnlChk.Alarm.St);
#ifdef LVM_CAN_ACTIVE_ENABLE

    volt = BmsSys_GetCanInhVolt();
    if(volt < LVM_STATE_OFF_THRED)
    {
        LvM_ActSgnlChk.CarActive.OffCnt++;
        LvM_ActSgnlChk.CarActive.OnCnt = 0;
    }
    else
    {
        LvM_ActSgnlChk.CarActive.OffCnt = 0;
        LvM_ActSgnlChk.CarActive.OnCnt++;
    }
    if (LvM_ActSgnlChk.CarActive.OffCnt >= LVM_OFF_STATE_CNT)
    {
        LvM_ActSgnlChk.CarActive.OffCnt = LVM_OFF_STATE_CNT;
        LvM_ActSgnlChk.CarActive.St = LVM_STATE_OFF;
    }
    else if(LvM_ActSgnlChk.CarActive.OnCnt >= LVM_ON_STATE_CNT)
    {
        LvM_ActSgnlChk.CarActive.OnCnt = LVM_ON_STATE_CNT;
        LvM_ActSgnlChk.CarActive.St = LVM_STATE_ON;
    }
#endif

    if( (LVM_STATE_OFF == LvM_ActSgnlChk.K15.St)
      &&(LVM_STATE_OFF == LvM_ActSgnlChk.OffBDChg.St)
      &&(LVM_STATE_OFF == LvM_ActSgnlChk.OnBDChg.St)
      &&(LVM_STATE_OFF == LvM_ActSgnlChk.Alarm.St)
#ifdef LVM_CAN_ACTIVE_ENABLE1
            &&(LVM_STATE_OFF == LvM_ActSgnlChk.CarActive.St)
#endif
      )
    {
        rslt = LVM_STATE_OFF;
    }
    else if((LVM_STATE_ON == LvM_ActSgnlChk.K15.St)
            ||(LVM_STATE_ON == LvM_ActSgnlChk.OffBDChg.St)
            ||(LVM_STATE_ON == LvM_ActSgnlChk.OnBDChg.St)
            ||(LVM_STATE_ON == LvM_ActSgnlChk.Alarm.St)
#ifdef LVM_CAN_ACTIVE_ENABLE1
            ||(LVM_STATE_ON == LvM_ActSgnlChk.CarActive.St)
#endif
           )
    {
        rslt = LVM_STATE_ON;
    }
    else
    {
        rslt = LVM_STATE_INIT;
    }

    return rslt;
}

static void LvM_LowVoltCtrl(void)
{
    GeneralReturnType return_rslt;

    if(1 == flag_test_k15)  //用于生产测试ON档延时下电
    {
        if( (LVM_STATE_OFF == LvM_DetectPowState()) || (LVM_FLAG_ENABLE == gby_VcuShutDownEn) )
        {

            tm_test_on++;
            if(tm_test_on >= 400)
            {
            
                tm_test_on = 401;
                return_rslt = POWER_OFF_BMU();
                return_rslt = POWER_OFF();
            }

        } 
        else
        {
            if(LVM_FLAG_OK == LvM_RunModeM.BakupFlag)
            {
                LvM_RunModeM.BakupFlag = LVM_FLAG_FAIL;
                (void)RunModem_SwitchModeTo((eRunMode_ModeType)LvM_RunModeM.Bak);
            }

            LvM_TmCnt.pow_down_cnt = 0;

            BmsSys_UpdateAwakeSt(LVM_STATE_ON);

            if(LVM_FLAG_OK == gby_KeyResetFlag)
            {
                return_rslt = POWER_ON_BMU();
                gby_BcuShutDownEn = LVM_FLAG_DISABLE;
                gby_KeyResetFlag = LVM_FLAG_FAIL;
                return_rslt = POWER_ON();
            }
        }
    }
    else
    {
        tm_test_on = 0;
       /* if((LVM_STATE_OFF == LvM_DetectPowState()) && (1 == LvM_ActSgnlChk.CarActive.St))
        {
            (void)CanIf_SetCanMode(CAR_CAN_CHANNEL,0);
        }
        else */if( (LVM_STATE_OFF == LvM_DetectPowState()) || (LVM_FLAG_ENABLE == gby_VcuShutDownEn) )
        {
            if(LVM_FLAG_FAIL == LvM_RunModeM.BakupFlag)
            {
                LvM_RunModeM.BakupFlag = LVM_FLAG_OK;
                LvM_RunModeM.Bak = BatSys_GetRunMode();
                (void)CanIf_SetCanMode(CAR_CAN_CHANNEL,0);
            }

            (void)RunModem_SwitchModeTo(RUNMODE_SHUT_DOWN);

            BmsSys_UpdateAwakeSt(LVM_STATE_OFF);

            if(LVM_FLAG_ENABLE == gby_SystemShutDownEn)
            {
                return_rslt = POWER_OFF_BMU();
                gby_BcuShutDownEn = LVM_FLAG_ENABLE;
                if(LvM_TmCnt.pow_down_cnt >= LVM_TM_1S)
                {
                    LvM_TmCnt.pow_down_cnt = 0;
                    return_rslt = POWER_OFF();
                }
            }
            
            else
            {
                LvM_TmCnt.pow_down_cnt = 0;
                
                gby_BcuShutDownEn =  LVM_FLAG_DISABLE;   
            }

            if(LVM_FLAG_FAIL == gby_KeyResetFlag)
            {
                DataStore_EnableEventDataStore(POWOFF_DATA);

                LvM_SaveSystemTime();
                gby_KeyResetFlag = LVM_FLAG_OK;
            }
        }
        else
        {
            if(LVM_FLAG_OK == LvM_RunModeM.BakupFlag)
            {
                LvM_RunModeM.BakupFlag = LVM_FLAG_FAIL;
                (void)RunModem_SwitchModeTo((eRunMode_ModeType)LvM_RunModeM.Bak); 
            }
            (void)CanIf_SetCanMode(CAR_CAN_CHANNEL,1);
            LvM_TmCnt.pow_down_cnt = 0;

            BmsSys_UpdateAwakeSt(LVM_STATE_ON);

            if(LVM_FLAG_OK == gby_KeyResetFlag)
            {
                return_rslt = POWER_ON_BMU();
                gby_BcuShutDownEn = LVM_FLAG_DISABLE;
                gby_KeyResetFlag = LVM_FLAG_FAIL;
                return_rslt = POWER_ON();
            }
        }
    
    }
    // prevent C5917: Removed dead assignment
    (void)return_rslt;

}

void LvM_Init(void)
{
    GeneralReturnType return_rslt;

    LvM_TmCnt.cnt = 0;
    LvM_TmCnt.pow_down_cnt = 0;
    gby_KeyResetFlag = 0;
    LvM_TmCnt.save_time_cnt = 0;
    
    LvM_ActSgnlChk.K15.St = LVM_STATE_INIT;
    LvM_ActSgnlChk.OffBDChg.St = LVM_STATE_INIT;
    LvM_ActSgnlChk.OnBDChg.St = LVM_STATE_INIT;
    LvM_ActSgnlChk.Alarm.St = LVM_STATE_INIT;

    return_rslt = POWER_ON();
    return_rslt = POWER_ON_BMU();
    gby_SystemShutDownEn = LVM_FLAG_ENABLE;
    // prevent C5917: Removed dead assignment
    (void)return_rslt;

}

void LvM_CountCbk(void)
{
    (LvM_TmCnt.cnt < 255) ? (LvM_TmCnt.cnt++) : (LvM_TmCnt.cnt = 255);
    (LvM_TmCnt.pow_down_cnt < 255) ? (LvM_TmCnt.pow_down_cnt++) : (LvM_TmCnt.pow_down_cnt = 255);
    (LvM_TmCnt.save_time_cnt < 65500) ? (LvM_TmCnt.save_time_cnt++) : (LvM_TmCnt.save_time_cnt = 65500);
}

void LvM_ProcMain(void)
{
    if(LvM_TmCnt.cnt >= TM_MS(10))
    {
        LvM_TmCnt.cnt = 0;

#ifdef LOW_VOLT_MANAGE_ENABLE
        LvM_LowVoltCtrl();
#endif
    }
    if(LvM_TmCnt.save_time_cnt >= TM_5MIN)
    {
        LvM_TmCnt.save_time_cnt = 0;
        LvM_SaveSystemTime();
    }
}

uByte LvM_GetBcuShutDownEn(void)
{
    return gby_BcuShutDownEn;
}

void LvM_UpdateSystemShutDownEn(uByte order)
{
    gby_SystemShutDownEn = order;
}

void LvM_UpdateVcuShutDownEn(uByte order)
{
    gby_VcuShutDownEn = order;
}
