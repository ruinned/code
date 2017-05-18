// ThM.c
#include "ThM-AB.h"
#include "EE.h"
#include "ErrorAlarm.h"
#include "SchIf.h"
#include "AppRelay.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "ChargeSt.h"
#include "IntCan.h"

#ifdef USE_THM_AB


#define FAN_CTRL(relay_id, relay_cmd)  Actor_CtrlRelay(relay_id, relay_cmd)
#define HEAT_CTRL(relay_id, relay_cmd)  Actor_CtrlRelay(relay_id, relay_cmd)

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
} sFan_ThrhdType;

typedef struct
{
    uByte heat_on_temper;
    uByte heat_off_temper;
    uByte heat_off_dt;
} sHeat_ThrhdType;

typedef struct
{
    sFan_ThrhdType  Fan_Thrhd;
    sHeat_ThrhdType Heat_Thrhd;
} SThM_ThrhdType;

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
uByte gby_HeatOnce_Flag;

#if(MAX_BMU_NUM)
uByte gby_InformBmuFanFlag[MAX_BMU_NUM];
uByte gby_InformBmuHeatFlag[MAX_BMU_NUM];
#endif

uByte gby_OpenFanFlag;
uByte gby_ReceiveBcuThmCmdCnt;
uByte gby_ReceiveBcuHeatCmdCnt;
uByte gby_BmuSelfCtrlThmFlag;

#define THM_FLAG_OK   1
#define THM_FLAG_FAIL 0

#define THM_ON_HEAT   1
#define THM_OFF_HEAT  0

#define THM_ON_FAN    1
#define THM_OFF_FAN   0

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
static void ThM_ProcHeat(void);


void ThM_ModuleInit(void)
{
    uWord rslt;

    ThM_TmCnt.cnt = 0;
    gby_LoadVar_Flag = THM_FLAG_FAIL;

    rslt = SysConf_InitParaTable(THM_HEAT_MANAGE_PARA, (uLWord)(&ThM_Thrhd), sizeof(ThM_Thrhd));
    // load Thm control parameter
    rslt = EE_LoadVar(THM_HEAT_MANAGE_PARA);
    // prevent C5917: Removed dead assignment
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
        ThM_ProcHeat();
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
#endif
    total_bmu_num = Bat_GetBmuNum();

    if( (THM_FUN_ENABLE == THM_BCU_CTRL_BCU)
            || (THM_FLAG_OK == gby_BmuSelfCtrlThmFlag) )
    {
        // get battery status
        lby_bat_max_t = Bat_GetMaxT();
        lby_bat_min_t = Bat_GetMinT();
        lby_bat_dt = Bat_GetDeltaT();

        lby_bat_t_err = (uByte)Err_GetHwErrLevel(ERR_HW_DETECT_TEMPER);

        // control fan on
        if( ( (lby_bat_max_t >= ThM_Thrhd.Fan_Thrhd.fan_on_temper)
                || ((lby_bat_dt >= ThM_Thrhd.Fan_Thrhd.fan_on_dt) && (lby_bat_min_t >= FAN_ON_MIN_T)))
            && (THM_FLAG_FAIL == gby_OpenFanFlag)
            && (lby_bat_max_t < 0xf0))
              //  || ((uByte)ERR_LEVEL_TWO == lby_bat_t_err) )
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
//            BatSys_UpdateFanSt((uByte)Actor_GetRelaySt(FAN_RELAY_ID));
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
//            BatSys_UpdateFanSt((uByte)Actor_GetRelaySt(FAN_RELAY_ID));
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
            if(  ((max_tmper[bmu_no - 1] >= ThM_Thrhd.Fan_Thrhd.fan_on_temper) && (max_tmper[bmu_no - 1] < 0xf0))
               ||((delta_tmper[bmu_no - 1] >= ThM_Thrhd.Fan_Thrhd.fan_on_dt) && (min_tmper[bmu_no - 1] >= FAN_ON_MIN_T)))
                
            {
                gby_InformBmuFanFlag[bmu_no - 1] = THM_ON_FAN;
            }

            // control fan off
            if(  (max_tmper[bmu_no - 1] <= ThM_Thrhd.Fan_Thrhd.fan_off_temper)
               &&((delta_tmper[bmu_no - 1] <= ThM_Thrhd.Fan_Thrhd.fan_off_dt) || (min_tmper[bmu_no - 1] <= FAN_OFF_MIN_T))) 
                        
            {
                gby_InformBmuFanFlag[bmu_no - 1] = THM_OFF_FAN;
            }

            if(   (THM_OFF_FAN == gby_InformBmuFanFlag[bmu_no - 1])
                &&(THM_ON_FAN == BatSys_GetAllPackFanSt(bmu_no)))
            {
                IntCan_CtrlSlaveRelay(bmu_no, FAN_RELAY_ID, RELAY_OFF);
                IntCan_CtrlSlaveRelay(bmu_no, 2, 1);

            }
            if(   (THM_ON_FAN == gby_InformBmuFanFlag[bmu_no - 1])
                &&(THM_OFF_FAN == BatSys_GetAllPackFanSt(bmu_no)))
            {
                IntCan_CtrlSlaveRelay(bmu_no, FAN_RELAY_ID, RELAY_ON);
                IntCan_CtrlSlaveRelay(bmu_no, 2, 1);

            }
        }
    }
#endif
}

static void ThM_ProcHeat(void)
{
    uByte lby_bat_max_t;
    uByte lby_bat_min_t;
    uByte lby_bat_dt;
    uWord rslt;
    uByte bmu_no;
    uByte total_bmu_num;
    sActor_CtrlRelayCmdType relay_cmd;
    sIntCan_BcuCtrlBmuRlyType bcu_relay_ctrl;
    
    uByte max_tmper[MAX_BMU_NUM];
    uByte min_tmper[MAX_BMU_NUM];
    uByte delta_tmper[MAX_BMU_NUM];
    total_bmu_num = Bat_GetBmuNum();

    // get battery status
    lby_bat_max_t = Bat_GetMaxT();
    lby_bat_min_t = Bat_GetMinT();
    lby_bat_dt = Bat_GetDeltaT();

    if( (CHG_ST_ON == Chg_GetChgSt())
      &&(THM_FLAG_OK == Chg_GetAcLinkSt()) )
    {
        if(THM_FUN_ENABLE == HEAT_BCU_CTRL_BCU)
        {

            if( (lby_bat_min_t <= ThM_Thrhd.Heat_Thrhd.heat_on_temper)
              && (THM_FLAG_FAIL == gby_HeatOnce_Flag) )
            {
                gby_HeatOnce_Flag = THM_FLAG_OK;
                relay_cmd.Bcmd = RELAY_CLOSE_CMD;
                relay_cmd.CmdType = IMMED_RELAY_CMD;
                rslt = HEAT_CTRL(HEAT_RELAY_ID, relay_cmd);
                BatSys_UpdateHeaterSt((uByte)Actor_GetRelaySt(HEAT_RELAY_ID));
            }
            if( ((lby_bat_min_t >= ThM_Thrhd.Heat_Thrhd.heat_off_temper)
                    || (lby_bat_dt >= ThM_Thrhd.Heat_Thrhd.heat_off_dt) )
                    && (THM_FLAG_OK == gby_HeatOnce_Flag) )
            {
                gby_HeatOnce_Flag = THM_FLAG_FAIL;
                relay_cmd.Bcmd = RELAY_OPEN_CMD;
                relay_cmd.CmdType = IMMED_RELAY_CMD;
                rslt = HEAT_CTRL(HEAT_RELAY_ID, relay_cmd);
                BatSys_UpdateHeaterSt((uByte)Actor_GetRelaySt(HEAT_RELAY_ID));
            }
        }
        if(THM_FUN_ENABLE == HEAT_BCU_CTRL_BMU)
        {
            for(bmu_no = 1; bmu_no <= total_bmu_num; bmu_no++)
            {
                max_tmper[bmu_no - 1] = Bat_GetPackMaxT(bmu_no);
                min_tmper[bmu_no - 1] = Bat_GetPackMinT(bmu_no);
                delta_tmper[bmu_no - 1] = Bat_GetPackDeltaT(bmu_no);
                if((min_tmper[bmu_no - 1] <= ThM_Thrhd.Heat_Thrhd.heat_on_temper))
                {
                    gby_InformBmuHeatFlag[bmu_no - 1] = THM_ON_HEAT;
                }
                if(  (min_tmper[bmu_no - 1] >= ThM_Thrhd.Heat_Thrhd.heat_off_temper)
                   ||(delta_tmper[bmu_no - 1] >= ThM_Thrhd.Heat_Thrhd.heat_off_dt))
                {
                    gby_InformBmuHeatFlag[bmu_no - 1] = THM_OFF_HEAT;
                }
                if(  (THM_OFF_HEAT == gby_InformBmuHeatFlag[bmu_no - 1])
                  && (THM_ON_HEAT == BatSys_GetAllPackHeaterSt(bmu_no)))
                
                {
                    IntCan_CtrlSlaveRelay(bmu_no, HEAT_RELAY_ID, RELAY_OFF);
                    IntCan_CtrlSlaveRelay(bmu_no, 3, 1);
                }
                if(   (THM_ON_HEAT == gby_InformBmuHeatFlag[bmu_no - 1])
                    && (THM_OFF_HEAT == BatSys_GetAllPackHeaterSt(bmu_no)))
                {
                    IntCan_CtrlSlaveRelay(bmu_no, HEAT_RELAY_ID, RELAY_ON);
                    IntCan_CtrlSlaveRelay(bmu_no, 3, 1);
                }
            }
        }
    }

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

    // prevent C5917: Removed dead assignment
    (void)rslt;
}

#endif // #ifdef USE_THM_AB
