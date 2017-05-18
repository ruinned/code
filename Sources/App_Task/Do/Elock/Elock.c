
#include "Ecu.h"
#include "SchIf.h"
#include "DoIf.h"
#include "AiIf.h"
#include "Elock.h"
#include "ErrorAlarm.h"

#define  CMD_LEFT_TRUN_UNLOCK    0
#define  CMD_RIGHT_TRUN_UNLOCK   1
#define  CMD_LEFT_TRUN_LOCK      2
#define  CMD_RIGHT_TRUN_LOCK     3

#define  ELOCK_MAIN_START        1
#define  ELOCK_MAIN_STOP         0
#define  ELOCK_MAIN_TEST         2

#define TEMPER_DET_OPEN_AD_VAL  0x3E8
#define TEMPER_DET_SHORT_AD_VAL 0x14



#define  AI_GAIN  11

#define  ELOCK_VOLT_HIGH  9000
#define  ELOCK_VOLT_LOW   2000

#define  ELOCK_LOCK_FD_SIGNAL_TYPE_HIGH  1
#define  ELOCK_LOCK_FD_SIGNAL_TYPE_LOW    0
#define  ELOCK_UNLOCK_FD_SIGNAL_TYPE_HIGH  1
#define  ELOCK_UNLOCK_FD_SIGNAL_TYPE_LOW    0
#define  ELOCK_FD_SIGNAL_TYPE_DEFAULT 2
#define  CMD_LAST_TIME_MAX_CNT    30
#define  ELOCK_TEMP_SHORT         0x02
#define  ELOCK_TEMP_OPEN          0x04
#define  ELOCK_HARD_ERR           0x01

#define  ST_ERR_CNT_MAX  10

uByte  Eclock_TimeCnt;
uWord  gwd_cmd_time_last_cnt[4];
uByte  gby_elock_hderr;
uWord  gwd_elock_volt;
uByte  gby_Elock_Main_Start;
uByte  gby_ElockTemper[4];
uWord  gwd_TempAd[4];
uByte  gby_TempId;


typedef union
{
    uByte by;
    struct
    {
        uByte Cmd: 2;
        uByte ElockSt: 2;
        uByte MotoSt: 2;
        uByte LockFdSigType: 1;
        uByte UnLockFdSigType: 1;
    } bit;
} uEclock_CmdStType;

uEclock_CmdStType  Eclock_CmdSt;

uByte gby_StErrCnt;
uByte Eclock_MainSt;
uByte gby_StCnt[2];
uByte gby_StCnt1[2];
#define  MAX_ST_CNT  3

void Elock_UpdateCmd(uByte Cmd)
{
    uByte Cmd_Bak;
    if(gby_Elock_Main_Start != ELOCK_MAIN_TEST)
    {  
        gby_Elock_Main_Start = ELOCK_MAIN_START; 
    }
    if(CMD_RIGHT_TRUN_LOCK == (Cmd & CMD_RIGHT_TRUN_LOCK))
    {
        Eclock_CmdSt.bit.Cmd = CMD_RIGHT_TRUN_LOCK;
    }
    else if(CMD_LEFT_TRUN_LOCK == (Cmd & CMD_LEFT_TRUN_LOCK))
    {
        Eclock_CmdSt.bit.Cmd = CMD_LEFT_TRUN_LOCK;
    }
    else if(CMD_RIGHT_TRUN_UNLOCK == (Cmd & CMD_RIGHT_TRUN_UNLOCK))
    {
        Eclock_CmdSt.bit.Cmd = CMD_RIGHT_TRUN_UNLOCK;
    }
    else if(CMD_LEFT_TRUN_UNLOCK == (Cmd & CMD_LEFT_TRUN_UNLOCK))
    {
        Eclock_CmdSt.bit.Cmd = CMD_LEFT_TRUN_UNLOCK;
    }
    else
    {
      
    }
    
    Cmd_Bak = ((Cmd & 0x0C) >> 2);
    
    if(ST_LOCK == Cmd_Bak)
    {
        Eclock_MainSt = ST_LOCK;
    }
    else if(ST_UNLOCK == Cmd_Bak)
    {
        Eclock_MainSt = ST_UNLOCK;
    }
    else
    {
        Eclock_MainSt = ST_ERR;
    }

}

uByte Elock_GetTemp(uByte Id)
{
    if(Id < 4)
    { 
        return  gby_ElockTemper[Id];
    }
    return 0;
}

uByte Eclock_GetElockSt(void)
{
    return Eclock_CmdSt.bit.ElockSt;
}

uByte Eclock_GetMotoSt(void)
{
    return Eclock_CmdSt.bit.MotoSt;
}

uByte Elock_GetHdErr(void)
{
    return gby_elock_hderr;
}

void Elock_Init(void)
{
    uByte i;
    Eclock_TimeCnt = 0;
    Eclock_CmdSt.bit.ElockSt = ST_DEFAULT;
    Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
    Eclock_MainSt = ST_DEFAULT;
    Eclock_CmdSt.bit.Cmd = CMD_LEFT_TRUN_UNLOCK;
    DoIf_Ctrl(DOIF_LOCK_LOCK_NO,1);
    DoIf_Ctrl(DOIF_LOCK_UNLOCK_NO,1);
    gby_StErrCnt = 0;
    gwd_cmd_time_last_cnt[0] = 0;
    gwd_cmd_time_last_cnt[1] = 0;
    gwd_cmd_time_last_cnt[2] = 0;
    gwd_cmd_time_last_cnt[3] = 0;
    gby_elock_hderr = 0;
    gwd_elock_volt = 0;
    Eclock_CmdSt.bit.LockFdSigType = ELOCK_LOCK_FD_SIGNAL_TYPE_HIGH;
    Eclock_CmdSt.bit.UnLockFdSigType = ELOCK_UNLOCK_FD_SIGNAL_TYPE_LOW;
    gby_Elock_Main_Start = ELOCK_MAIN_STOP;
    gby_TempId = 0;
    for(i = 0;i < 4;i++)
    {
        gby_ElockTemper[i] = 40;
        gwd_TempAd[i] = 0;
    }
    
}


void Elock_TimeCntCbk(void)
{
    (Eclock_TimeCnt < 255) ? (Eclock_TimeCnt++) : (Eclock_TimeCnt = 255);
}

static uWord Elock_DetTemper(uByte id)
{
    uByte  i;
    uWord temper_ad;
    uWord rslt = ERR_OK;

    // read ad value
    switch(id)
    {
      case 0:
      {
          (void)AiIf_MeasureChannel(AIIF_CPUNTC1_AD_NO,&temper_ad);
          break; 
      }
      case 1:
      {
          (void)AiIf_MeasureChannel(AIIF_CPUNTC2_AD_NO,&temper_ad);
          break; 
      }
      case 2:
      {
          (void)AiIf_MeasureChannel(AIIF_CPUNTC3_AD_NO,&temper_ad);
          break; 
      }
      case 3:
      {
          (void)AiIf_MeasureChannel(AIIF_CPUNTC4_AD_NO,&temper_ad);
          break; 
      }
      default:
      {
          return 3; 
      }
    }
    
    gwd_TempAd[id] = temper_ad;
    temper_ad /= 4;
    if(temper_ad >= TEMPER_DET_OPEN_AD_VAL) //30sps  // short circut or not connect
    {
        // ntc not connected
        gby_ElockTemper[id] = 0xFF;
        gby_elock_hderr |= ELOCK_TEMP_OPEN;
        return 1;
    }
    else if(temper_ad <= TEMPER_DET_SHORT_AD_VAL)
    {
        // NTC short curcuit
        gby_ElockTemper[id] = 0xFF;
        gby_elock_hderr |= ELOCK_TEMP_SHORT;
        return 2;        
    }
    else
    {
        if(id < 2)
        {
            for(i = 0; i < Elock_T_Point; i++)
            {
                if(temper_ad <= AD_T_10K[i])
                {
                    gby_ElockTemper[id] = Elock_T_Point - i - 1;

                    break;
                }
            }

            if(Elock_T_Point == i)
            {
                gby_ElockTemper[id] = 0;
            }
        }
        else
        {
            for(i = 0; i < Elock_T_Point1; i++)
            {
                if(temper_ad <= AD_T_10K1[i])
                {
                    gby_ElockTemper[id] = Elock_T_Point1 - i - 1;

                    break;
                }
            }

            if(Elock_T_Point1 == i)
            {
                gby_ElockTemper[id] = 0;
            }
        }
        
        return rslt;   // ok
    }
}

static void Elock_DetSt(void)
{
    uByte i;
    uWord Ad_Value,Ad_MaxValue,Ad_MinValue;
    uWord Ad_ValueBak[5];
    uLWord VoltBak,Ad_Sum;
    uWord Volt;
    
    Ad_Value = 0;
    Ad_MaxValue = 0;
    Volt = 0;
    Ad_Sum = 0;
    Ad_MinValue = 0xffff;
    
    for(i = 0;i < 5;i++)
    {
        Ad_ValueBak[i] = 0;
        (void)AiIf_MeasureChannel(AIIF_CPU_CPUAI3_NO,&Ad_ValueBak[i]);
        if(Ad_ValueBak[i] > Ad_MaxValue)
        {
            Ad_MaxValue = Ad_ValueBak[i];
        }
        
        if(Ad_ValueBak[i] < Ad_MinValue)
        {
            Ad_MinValue = Ad_ValueBak[i];
        }
        Ad_Sum += Ad_ValueBak[i];
    }
    
    Ad_Sum -= Ad_MaxValue;
    Ad_Sum -= Ad_MinValue;
    
    Ad_Value = (uWord)(Ad_Sum / 3);
    if(Ad_Value <= CPU_ADC_ACCURACY)
    {  
        VoltBak = (uLWord)Ad_Value; 
        VoltBak *= ADC_VOLT_CRITERION;
        VoltBak /= CPU_ADC_ACCURACY;
        VoltBak *= AI_GAIN;
        Volt = (uWord)VoltBak;
    }
    else
    {
        Volt = 0; 
    }
    gwd_elock_volt = Volt;
   
    if((CMD_RIGHT_TRUN_LOCK == Eclock_CmdSt.bit.Cmd) || (CMD_LEFT_TRUN_LOCK == Eclock_CmdSt.bit.Cmd))
    {
        gby_StCnt1[0] = 0; 
        gby_StCnt1[1] = 0;
        if(ELOCK_LOCK_FD_SIGNAL_TYPE_LOW == Eclock_CmdSt.bit.LockFdSigType)
        {
            if(Volt < ELOCK_VOLT_LOW)
            {
                gby_StCnt[0]++;
                if(gby_StCnt[0] > MAX_ST_CNT)
                { 
                    gby_StCnt[0] = MAX_ST_CNT; 
                    Eclock_CmdSt.bit.ElockSt = ST_LOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_LOCK;
                }
                gby_StCnt[1] = 0;
            }
            else if(Volt > ELOCK_VOLT_HIGH)
            {
                gby_StCnt[0] = 0;
                gby_StCnt[1]++;
                if(gby_StCnt[1] > MAX_ST_CNT)
                {
                    gby_StCnt[1] = MAX_ST_CNT;
                    Eclock_CmdSt.bit.ElockSt = ST_UNLOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_UNLOCK;
                }
            }
            else
            {
                gby_StCnt[0] = 0; 
                gby_StCnt[1] = 0;
                Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
            }
        }
        else if(ELOCK_LOCK_FD_SIGNAL_TYPE_HIGH == Eclock_CmdSt.bit.LockFdSigType)
        {
            if(Volt > ELOCK_VOLT_HIGH)
            {
                gby_StCnt[0]++;
                if(gby_StCnt[0] > MAX_ST_CNT)
                { 
                    gby_StCnt[0] = MAX_ST_CNT; 
                    Eclock_CmdSt.bit.ElockSt = ST_LOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_LOCK;
                }
                gby_StCnt[1] = 0;
            }
            else if(Volt < ELOCK_VOLT_LOW)
            {
                gby_StCnt[0] = 0;
                gby_StCnt[1]++;
                if(gby_StCnt[1] > MAX_ST_CNT)
                {
                    gby_StCnt[1] = MAX_ST_CNT;
                    Eclock_CmdSt.bit.ElockSt = ST_UNLOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_UNLOCK;
                }
            }
            else
            {
                gby_StCnt[0] = 0; 
                gby_StCnt[1] = 0;
                Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
            }
        }
        else
        {
            gby_StCnt[0] = 0; 
            gby_StCnt[1] = 0;
            Eclock_CmdSt.bit.ElockSt = ST_DEFAULT;
            Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
        }
    }
    else
    {
        gby_StCnt[0] = 0; 
        gby_StCnt[1] = 0;
        if(ELOCK_UNLOCK_FD_SIGNAL_TYPE_LOW == Eclock_CmdSt.bit.UnLockFdSigType)
        {
            if(Volt < ELOCK_VOLT_LOW)
            {
                gby_StCnt1[0]++;
                if(gby_StCnt1[0] > MAX_ST_CNT)
                { 
                    gby_StCnt1[0] = MAX_ST_CNT; 
                    Eclock_CmdSt.bit.ElockSt = ST_UNLOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_UNLOCK;
                }
                gby_StCnt1[1] = 0;
            }
            else if(Volt > ELOCK_VOLT_HIGH)
            {
                gby_StCnt1[0] = 0;
                gby_StCnt1[1]++;
                if(gby_StCnt1[1] > MAX_ST_CNT)
                {
                    gby_StCnt1[1] = MAX_ST_CNT;
                    Eclock_CmdSt.bit.ElockSt = ST_LOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_LOCK;
                }
            }
            else
            {
                gby_StCnt1[0] = 0; 
                gby_StCnt1[1] = 0;
                Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
            }
        }
        else if(ELOCK_UNLOCK_FD_SIGNAL_TYPE_HIGH == Eclock_CmdSt.bit.UnLockFdSigType)
        {
            if(Volt > ELOCK_VOLT_HIGH)
            {
                gby_StCnt1[0]++;
                if(gby_StCnt1[0] > MAX_ST_CNT)
                { 
                    gby_StCnt1[0] = MAX_ST_CNT; 
                    Eclock_CmdSt.bit.ElockSt = ST_UNLOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_UNLOCK;
                }
                gby_StCnt1[1] = 0;
            }
            else if(Volt < ELOCK_VOLT_LOW)
            {
                gby_StCnt1[0] = 0;
                gby_StCnt1[1]++;
                if(gby_StCnt1[1] > MAX_ST_CNT)
                {
                    gby_StCnt1[1] = MAX_ST_CNT;
                    Eclock_CmdSt.bit.ElockSt = ST_LOCK;
                    Eclock_CmdSt.bit.MotoSt = ST_LOCK;
                }
            }
            else
            {
                gby_StCnt1[0] = 0; 
                gby_StCnt1[1] = 0;
                Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
            }
        }
        else
        {
            gby_StCnt1[0] = 0; 
            gby_StCnt1[1] = 0;
            Eclock_CmdSt.bit.ElockSt = ST_DEFAULT;
            Eclock_CmdSt.bit.MotoSt = ST_DEFAULT;
        }
    }   
}

uWord Elock_GetAiVolt(void)
{
    return gwd_elock_volt;
}

void Elock_UpdateMainSartFlag(uByte flag)
{
    gby_Elock_Main_Start = flag;
}

void Elock_TurnRight(void)
{
    DoIf_Ctrl(DOIF_LOCK_LOCK_NO,0);
    DoIf_Ctrl(DOIF_LOCK_UNLOCK_NO,1);
}

void Elock_TurnLeft(void)
{
    DoIf_Ctrl(DOIF_LOCK_LOCK_NO,1);
    DoIf_Ctrl(DOIF_LOCK_UNLOCK_NO,0);
}

void Elock_TurnOff(void)
{
    DoIf_Ctrl(DOIF_LOCK_LOCK_NO,1);
    DoIf_Ctrl(DOIF_LOCK_UNLOCK_NO,1);
}

void Elock_Main(void)
{
    if(Eclock_TimeCnt >= TM_MS(100))
    {
        Eclock_TimeCnt = 0;
        if(ELOCK_MAIN_START == gby_Elock_Main_Start)
        {
            if(CMD_RIGHT_TRUN_LOCK == Eclock_CmdSt.bit.Cmd)
            {
                if((ST_LOCK != Eclock_CmdSt.bit.MotoSt) && (ST_LOCK != Eclock_CmdSt.bit.ElockSt) && (gwd_cmd_time_last_cnt[0] < CMD_LAST_TIME_MAX_CNT))
                { 
                    gwd_cmd_time_last_cnt[0]++;
                    Elock_TurnRight();   
                }
                else
                {
                    if(gwd_cmd_time_last_cnt[0] >= CMD_LAST_TIME_MAX_CNT)
                    {
                        gwd_cmd_time_last_cnt[0] = CMD_LAST_TIME_MAX_CNT;
                        gby_elock_hderr |= ELOCK_HARD_ERR;
                    }
                    else
                    {
                        gwd_cmd_time_last_cnt[0] = 0;
                    }
                    Elock_TurnOff();
                    gwd_cmd_time_last_cnt[1] = 0;
                    gwd_cmd_time_last_cnt[2] = 0;
                    gwd_cmd_time_last_cnt[3] = 0;
                    gby_Elock_Main_Start = ELOCK_MAIN_STOP;
                }
            }
            else if(CMD_LEFT_TRUN_LOCK == Eclock_CmdSt.bit.Cmd)
            {
                if((ST_LOCK != Eclock_CmdSt.bit.MotoSt) && (ST_LOCK != Eclock_CmdSt.bit.ElockSt) && (gwd_cmd_time_last_cnt[1] < CMD_LAST_TIME_MAX_CNT))
                {    
                    gwd_cmd_time_last_cnt[1]++;
                    Elock_TurnLeft();
                }
                else
                {
                    if(gwd_cmd_time_last_cnt[1] >= CMD_LAST_TIME_MAX_CNT)
                    {
                        gwd_cmd_time_last_cnt[1] = CMD_LAST_TIME_MAX_CNT;
                        gby_elock_hderr |= ELOCK_HARD_ERR;
                    }
                    else
                    {
                        gwd_cmd_time_last_cnt[1] = 0;
                    }
                    Elock_TurnOff();
                    gwd_cmd_time_last_cnt[0] = 0;
                    gwd_cmd_time_last_cnt[2] = 0;
                    gwd_cmd_time_last_cnt[3] = 0;
                    gby_Elock_Main_Start = ELOCK_MAIN_STOP;
                } 
            }
            else if(CMD_RIGHT_TRUN_UNLOCK == Eclock_CmdSt.bit.Cmd)
            {
                if((ST_UNLOCK != Eclock_CmdSt.bit.ElockSt) && (ST_UNLOCK != Eclock_CmdSt.bit.MotoSt) && (gwd_cmd_time_last_cnt[2] < CMD_LAST_TIME_MAX_CNT))
                { 
                    gwd_cmd_time_last_cnt[2]++;
                    Elock_TurnRight(); 
                }
                else
                {
                    if(gwd_cmd_time_last_cnt[2] >= CMD_LAST_TIME_MAX_CNT)
                    {
                        gwd_cmd_time_last_cnt[2] = CMD_LAST_TIME_MAX_CNT;
                        gby_elock_hderr |= ELOCK_HARD_ERR;
                    }
                    else
                    {
                        gwd_cmd_time_last_cnt[2] = 0;
                    }
                    Elock_TurnOff();
                    gwd_cmd_time_last_cnt[0] = 0;
                    gwd_cmd_time_last_cnt[1] = 0;
                    gwd_cmd_time_last_cnt[3] = 0;
                    gby_Elock_Main_Start = ELOCK_MAIN_STOP;
                }
            }
            else if(CMD_LEFT_TRUN_UNLOCK == Eclock_CmdSt.bit.Cmd)
            {
                if((ST_UNLOCK != Eclock_CmdSt.bit.ElockSt) && (ST_UNLOCK != Eclock_CmdSt.bit.MotoSt) && (gwd_cmd_time_last_cnt[3] < CMD_LAST_TIME_MAX_CNT))
                { 
                    gwd_cmd_time_last_cnt[3]++;
                    Elock_TurnLeft();
                }
                else
                {
                    if(gwd_cmd_time_last_cnt[3] >= CMD_LAST_TIME_MAX_CNT)
                    {
                        gwd_cmd_time_last_cnt[3] = CMD_LAST_TIME_MAX_CNT;
                        gby_elock_hderr |= ELOCK_HARD_ERR;
                    }
                    else
                    {
                        gwd_cmd_time_last_cnt[3] = 0;
                    }
                    Elock_TurnOff();
                    gwd_cmd_time_last_cnt[0] = 0;
                    gwd_cmd_time_last_cnt[1] = 0;
                    gwd_cmd_time_last_cnt[2] = 0;
                    gby_Elock_Main_Start = ELOCK_MAIN_STOP;
                }
            }
            else
            {
                Elock_TurnOff();
                gwd_cmd_time_last_cnt[3] = 0;
                gwd_cmd_time_last_cnt[0] = 0;
                gwd_cmd_time_last_cnt[1] = 0;
                gwd_cmd_time_last_cnt[2] = 0;
                gby_Elock_Main_Start = ELOCK_MAIN_STOP;
            }
            Elock_DetSt();
        } 
        Elock_DetTemper(gby_TempId);
        gby_TempId++;
        if(gby_TempId > 3)
        {
            gby_TempId = 0;
        }
        if(0 != gby_elock_hderr)
        {
            Err_UpdateHwErrLevel(ERR_HW_ELOCK,ERR_LEVEL_TWO);
        }
    }
}