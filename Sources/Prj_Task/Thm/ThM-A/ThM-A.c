// ThM.c
#include "ThM-A.h"
#include "EE.h"
#include "ErrorAlarm.h"
#include "SchIf.h"
#include "AppRelay.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "IntCan.h"

#ifdef USE_THM_A

#define FAN_CTRL(relay_id, relay_cmd)  Actor_CtrlRelay(relay_id, relay_cmd)

typedef struct
{
    uByte cnt;
} sThM_TmCntType;

sThM_TmCntType ThM_TmCnt;

typedef struct
{
    uByte fan_on_temper;
    uByte fan_off_temper;
    uByte fan_on_dt;
    uByte fan_off_dt;
}sFan_ThrhdType;

typedef struct
{
    uByte heat_on_temper;
    uByte heat_off_temper;
    uByte heat_off_dt;
}sHeat_ThrhdType;

typedef struct
{
    sFan_ThrhdType  Fan_Thrhd;
    sHeat_ThrhdType Heat_Thrhd;
}SThM_ThrhdType;

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

SThM_ThrhdType ThM_Thrhd;

#pragma DATA_SEG DEFAULT

//uByte gby_fan_st;
//uWord gwd_fan_speed;
uByte gby_LoadVar_Flag;

#if(MAX_BMU_NUM)
uByte gby_InformBmuFlag[MAX_BMU_NUM];
#endif

uByte gby_OpenFanFlag;
uByte gby_ReceiveBcuCmdCnt;
uByte gby_BmuSelfCtrlFlag;

#define THM_CNT_5S    10

#define THM_FLAG_OK   1
#define THM_FLAG_FAIL 0

#define TEMPER_60C  100
#define TEMPER_50C  90
#define TEMPER_40C  80
#define TEMPER_30C  70
#define TEMPER_20C  60
#define TEMPER_10C  50
#define TEMPER_0C   40

// temper error code
#define ALL_TEMPER_LOSE  0x01

static void ThM_ProcThM(void);


void ThM_ModuleInit(void)
{
    uWord rslt;
    
    ThM_TmCnt.cnt = 0;
    gby_LoadVar_Flag = THM_FLAG_FAIL;
    
    rslt = SysConf_InitParaTable(THM_HEAT_MANAGE_PARA, (uLWord)(&ThM_Thrhd), sizeof(ThM_Thrhd));
    // load Thm control parameter
    rslt = EE_LoadVar(THM_HEAT_MANAGE_PARA);
    (void)rslt;
}

void ThM_ModuleCountCbk(void)
{
    (ThM_TmCnt.cnt < 255) ? (ThM_TmCnt.cnt++) : (ThM_TmCnt.cnt = 255);
}

void ThM_ModuleCbk(void)
{
    if(ThM_TmCnt.cnt >= TM_500MS)
    {
        ThM_TmCnt.cnt = 0;
        ThM_ProcThM();
    }
}

static void ThM_ProcThM(void)
{ 
    uByte lby_bat_max_t;
    uByte lby_bat_min_t;
    uByte lby_bat_dt;
    uByte lby_bat_t_err;
    uWord fan_speed_ctrl;
    uWord rslt;
    uByte bmu_no;
    uByte total_bmu_num;
    sActor_CtrlRelayCmdType relay_cmd;
    sIntCan_BcuCtrlBmuRlyType bcu_relay_ctrl;   
#if(MAX_BMU_NUM)   
    uByte max_tmper[MAX_BMU_NUM];
    uByte min_tmper[MAX_BMU_NUM];
    uByte delta_tmper[MAX_BMU_NUM];
    total_bmu_num = Bat_GetBmuNum();
#endif       
    
    if( (THM_FUN_ENABLE == THM_BCU_CTRL_BCU)
     || (THM_FLAG_OK == gby_BmuSelfCtrlFlag) )
    {
        // get battery status
        lby_bat_max_t = Bat_GetMaxT();
        lby_bat_min_t = Bat_GetMinT();
        lby_bat_dt = Bat_GetDeltaT();
        
        lby_bat_t_err = (uByte)Err_GetHwErrLevel(ERR_HW_DETECT_TEMPER);
        
        // control fan on
        if( (lby_bat_max_t >= ThM_Thrhd.Fan_Thrhd.fan_on_temper)
         || ((lby_bat_dt >= ThM_Thrhd.Fan_Thrhd.fan_on_dt) && (lby_bat_min_t >= FAN_ON_MIN_T)) 
         //|| ((uByte)ERR_LEVEL_TWO == lby_bat_t_err) )
         && (THM_FLAG_FAIL == gby_OpenFanFlag)
         && (lby_bat_max_t < 0xf0) )
        {
            // calc fan speed
            if(lby_bat_max_t > TEMPER_40C)
            {
                fan_speed_ctrl = FAN_HIGH_SPEED;
            }
            else
            {
                fan_speed_ctrl = FAN_LOW_SPEED;
            }
            
            // send fan control command
            relay_cmd.Bcmd = RELAY_CLOSE_CMD;
            relay_cmd.CmdType = IMMED_RELAY_CMD;
            relay_cmd.Dute = (uByte)(fan_speed_ctrl / 10);
            rslt = FAN_CTRL(FAN_RELAY_ID, relay_cmd);
            BatSys_UpdateFanSt((uByte)Actor_GetRelaySt(FAN_RELAY_ID));
            gby_OpenFanFlag = THM_FLAG_OK;
        }
        
        // control fan off
        if( (lby_bat_max_t <= ThM_Thrhd.Fan_Thrhd.fan_off_temper)
         && ((lby_bat_dt <= ThM_Thrhd.Fan_Thrhd.fan_off_dt) || (lby_bat_min_t <= FAN_OFF_MIN_T))
         && (THM_FLAG_OK == gby_OpenFanFlag))
        {
            gby_OpenFanFlag = THM_FLAG_FAIL;
            fan_speed_ctrl = 0;
            
            // send fan control command
            relay_cmd.Bcmd = RELAY_OPEN_CMD;
            relay_cmd.CmdType = IMMED_RELAY_CMD;
            relay_cmd.Dute = (uByte)(fan_speed_ctrl / 10);
            rslt = FAN_CTRL(FAN_RELAY_ID, relay_cmd);
            BatSys_UpdateFanSt((uByte)Actor_GetRelaySt(FAN_RELAY_ID));
        }
     }
         
#if(MAX_BMU_NUM)    
     if(THM_FUN_ENABLE == THM_BCU_CTRL_BMU)
     {
        for(bmu_no = 1; bmu_no <= total_bmu_num; bmu_no++)
        {
           max_tmper[bmu_no - 1] = Bat_GetPackMaxT(bmu_no);
           min_tmper[bmu_no - 1] = Bat_GetPackMinT(bmu_no);
           delta_tmper[bmu_no - 1] = Bat_GetPackDeltaT(bmu_no);
                 
           // control fan on
           if( (max_tmper[bmu_no - 1] >= ThM_Thrhd.Fan_Thrhd.fan_on_temper)
            || ((delta_tmper[bmu_no - 1] >= ThM_Thrhd.Fan_Thrhd.fan_on_dt) && (min_tmper[bmu_no - 1] >= FAN_ON_MIN_T))
            && (THM_FLAG_FAIL == gby_OpenFanFlag)
            && (max_tmper[bmu_no - 1] < 0xf0))
           {
              gby_InformBmuFlag[bmu_no - 1] = THM_FLAG_OK;
              gby_OpenFanFlag = THM_FLAG_OK;
           }
           
           // control fan off
           if( (max_tmper[bmu_no - 1] <= ThM_Thrhd.Fan_Thrhd.fan_off_temper)
             && (delta_tmper[bmu_no - 1] <= ThM_Thrhd.Fan_Thrhd.fan_off_dt)
             && (THM_FLAG_OK == gby_OpenFanFlag))
           {
              gby_OpenFanFlag = THM_FLAG_FAIL;
              gby_InformBmuFlag[bmu_no - 1] = THM_FLAG_FAIL;
           }
           
            if(THM_FLAG_FAIL == gby_InformBmuFlag[bmu_no - 1])
            {
               IntCan_CtrlSlaveRelay(bmu_no, FAN_RELAY_ID, RELAY_OPEN);
               IntCan_CtrlSlaveRelay(bmu_no, 2, THM_FLAG_OK);
            }
            else
            {
               IntCan_CtrlSlaveRelay(bmu_no, FAN_RELAY_ID, RELAY_CLOSE);
               IntCan_CtrlSlaveRelay(bmu_no, 2, THM_FLAG_OK);
            }
        }
     }
#endif     

}

static void ThM_GetFanSt()
{
#ifdef FANST_FEED_BAK
// use fan feedback singal to judge fan status

#else
// use mosfet status to judge fan status

#endif
}


void ThM_TestFan(uByte fan_on_off, uWord fan_speed_ctrl)
{
    uWord rslt;
    sActor_CtrlRelayCmdType relay_cmd;
    
    // send fan control command
    relay_cmd.Bcmd = fan_on_off;
    relay_cmd.CmdType = IMMED_RELAY_CMD;
    relay_cmd.Dute = (uByte)(fan_speed_ctrl / 10);
    rslt = FAN_CTRL(FAN_RELAY_ID, relay_cmd);
}

#endif // #ifdef USE_THM_A
