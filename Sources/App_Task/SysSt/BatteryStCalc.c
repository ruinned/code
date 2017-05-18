// BatterySt.c
#include "BatterySt.h"
#include "SysConf.h"
#include "EE.h"
#include "SchIf.h"
#include "ChargeSt.h"
#include "ErrorAlarm.h"
#include "WdgIf.h"
#include "ChgM-B.h"

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

#define HIS_INFO_DATA_LEN 12

const uWord  Bat_SocDis[10] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
const uWord  Bat_CurDis[14]= {25000, 27000, 28000, 28500, 29000, 29500, 30000, 30500, 31000, 31500, 32000, 33000, 34000, 35000};
const uByte  Bat_TemperDis[14] = {0, 10, 20, 30, 35, 40, 50, 55,60, 65, 70, 80, 90, 100 };
const uByte  Bat_DeltaTemperDis[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
const eSysConf_ParameterNOType  Bat_DataBuf[HIS_INFO_DATA_LEN] = 
{
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G1,  
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G2,
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G3,  
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G4,
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, 
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G6,
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G7,  
     SYSTEMST_HSTY_TEMPER_AREA_INFO_G8,
     SYSTEMST_HSTY_CURRENT_AREA_INFO_G1,  
     SYSTEMST_HSTY_CURRENT_AREA_INFO_G2,
     SYSTEMST_HSTY_SOC_AREA_INFO_G1,      
     SYSTEMST_HSTY_SOC_AREA_INFO_G2
};

typedef struct
{
    uByte StaticInfoCnt;
    uByte DisInfoCnt;
    uByte LoadDataCnt;
    uWord MaxTempDis;
    uWord MinTempDis;
    uWord AvgTempDis;
    uWord DeltaTempDis;
    uWord ChgData;
    uWord TempRise;
    uWord RiseRate;
    uWord TempRisePerHour;
    uWord SocDis;
    uWord CurDis;
    uByte ChgAccTime;
} sBat_TmCntType;

typedef struct
{
  uByte MaxTBakFlg    : 1;
  uByte PerHourFlg    : 1;
  uByte ChgTimesFlg   : 1;
  uByte ChgFullFlg    : 1;
  uByte TemperRiseRateFlg  : 1;
  uByte LoadDataFlg   : 1;
  uByte Rsv           : 2;
 
  
  uByte MaxTBak;
  uByte InitMaxT; 
  uWord PwrOnTimeCalc;
  uByte MaxTBakPerHour;
  uByte Cnt;
  uByte LoadDisInfo;
   
}sBat_StCalcType;
                    

sBat_TmCntType  Bat_TmCnt;
sBat_StCalcType Bat_StCalc;

extern sBat_AllInfoType  Bat_AllInfo;
extern sCell_BalanceTimeType Bat_LocalPackBalTime;

extern void Bat_DataProcessInit(void);

static void Bat_CalcAllMaxMin(void);

/*
** Data process
*/

void Bat_DataProcessInit(void)
{
    uByte * RAMPTR pbyte;
    uByte bcnt;
    GeneralReturnType rslt;
    
    pbyte = (uByte *RAMPTR)(&Bat_TmCnt);
    for(bcnt = 0; bcnt < sizeof(Bat_TmCnt); bcnt++)
    {
        *(pbyte++) = 0;
    } 
     
    pbyte = (uByte *RAMPTR)(&Bat_StCalc);
    for(bcnt = 0; bcnt < sizeof(Bat_StCalc); bcnt++)
    {
        *(pbyte++) = 0;
    }
    Bat_TmCnt.StaticInfoCnt = TM_MS(10);
    /* Read history Data From EE */
    (void)SysConf_InitParaTable(SYSTEMST_BAT_PACK_INFO_G1, (uLWord)(&(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1)), sizeof(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1));
    (void)SysConf_InitParaTable(SYSTEMST_BAT_PACK_INFO_G2, (uLWord)(&(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter2)), sizeof(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter2));
    (void)SysConf_InitParaTable(SYSTEMST_BAT_PACK_INFO_G3, (uLWord)(&(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3)), sizeof(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3));
    (void)SysConf_InitParaTable(SYSTEMST_BAT_PACK_INFO_G4, (uLWord)(&(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4)), sizeof(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4));

    rslt = 0;
    rslt = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G1);
    rslt |= EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G2);
    if(ERR_OK != rslt)  
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
    }
    rslt = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G3);
    rslt = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G4);

    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_INFO, (uLWord)(&(Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo)), sizeof(Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo));
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_CELLV_INFO, (uLWord)(&(Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo)), sizeof(Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo));
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_INRES_INFO, (uLWord)((&(Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo))), sizeof(Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo));
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_CHG_DCH_INFO, (uLWord)(&(Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo)),sizeof(Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo));
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_POWER_INFO, (uLWord)(&(Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo)),sizeof(Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo));

    rslt = EE_LoadVar(SYSTEMST_HSTY_TEMPER_INFO);
    if(rslt)
    {
        Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMinT = 250; 
    }
    rslt = EE_LoadVar(SYSTEMST_HSTY_CELLV_INFO);
    if(rslt)
    {
        Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinV = 5000; 
    }
    rslt = EE_LoadVar(SYSTEMST_HSTY_INRES_INFO);
    if(rslt)
    {
        Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo.HisMinR = 60000;
    }
    rslt = EE_LoadVar(SYSTEMST_HSTY_CHG_DCH_INFO);   
    rslt = EE_LoadVar(SYSTEMST_HSTY_POWER_INFO);
    if(rslt)
    {
        Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgPower = 30000;        
        Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgCur = 30000;
    }
 
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G1, ((uLWord )((Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MinTDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G2, ((uLWord )(&(Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MinTDis[7])) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G3, ((uLWord )((Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MaxTDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G4, ((uLWord )(&(Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MaxTDis[7])) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, ((uLWord )((Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.AvgTDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G6, ((uLWord )(&(Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.AvgTDis[7])) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G7, ((uLWord )((Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.MaxDeltaTDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_TEMPER_AREA_INFO_G8, ((uLWord )(&(Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.MaxDeltaTDis[7])) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_CURRENT_AREA_INFO_G1, ((uLWord)((Bat_AllInfo.HisInfoPtr->CurDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_CURRENT_AREA_INFO_G2, ((uLWord)(&(Bat_AllInfo.HisInfoPtr->CurDis[7])) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_SOC_AREA_INFO_G1, ((uLWord)((Bat_AllInfo.HisInfoPtr->SocDis)) ), 28);
    (void)SysConf_InitParaTable(SYSTEMST_HSTY_SOC_AREA_INFO_G2, ((uLWord)(&(Bat_AllInfo.HisInfoPtr->SocDis[7])) ), 12);

    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G1, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));
    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G2, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));
    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G3, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));
    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G4, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));
    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G5, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));
    (void)SysConf_InitParaTable(SYSTEMST_REMAIN_CAPACITY_BAK_G6, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->RemainCapacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->RemainCapacity));

    /* need read SYSTEMST_REMAIN_AH_CNT_BAK  and SOC*/
    rslt = EE_LoadVar(SYSTEMST_REMAIN_CAPACITY_BAK_G1);

    (void)SysConf_InitParaTable(SYSTEMST_RATED_ACTUAL_CAPACITY, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->Bat_Capacity)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->Bat_Capacity));
    rslt = EE_LoadVar(SYSTEMST_RATED_ACTUAL_CAPACITY);
    if(ERR_OK != rslt) 
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
    }


    (void)SysConf_InitParaTable(SYSTEMST_SOH, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->Soh)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->Soh));
    //rslt = EE_LoadVar(SYSTEMST_SOH); // no use now

    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_DCH_AH, ((uLWord)(&(Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh)) ), sizeof(Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh));
    //rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH); // load in soh module

    
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_1_20, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[0]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_21_40, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[15]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_41_60, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[30]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_61_80, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[45]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_81_100, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[60]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_101_120, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[75]))), 30);
    
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_1_20, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[0]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_21_40, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[15]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_41_60, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[30]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_61_80, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[45]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_81_100, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[60]))), 30);
    (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_101_120, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[75]))), 30);
 
    #if(MAX_CELL_NUM > 120)
    {
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_121_140, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[90]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_141_160, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[105]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_161_180, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[120]))), 30);
       
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_121_140, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[90]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_141_160, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[105]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_161_180, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[120]))), 30);
    }
    #endif
    #if(MAX_CELL_NUM > 180)
    {
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_181_200, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[135]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_201_220, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[150]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_221_240, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[165]))), 30);
    
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_181_200, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[135]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_201_220, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[150]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_221_240, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[165]))), 30);
    }
    #endif
    #if(MAX_CELL_NUM > 240)
    {
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_241_260, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[180]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_261_280, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[195]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_CHG_BAL_TIME_281_300, ((uLWord)(&(Bat_LocalPackBalTime.ChgBalTimeTotal[210]))), 30);
   
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_241_260, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[180]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_261_280, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[195]))), 30);
        (void)SysConf_InitParaTable(SYSTEMST_TOTAL_DCH_BAL_TIME_281_300, ((uLWord)(&(Bat_LocalPackBalTime.DchBalTimeTotal[210]))), 30);
    }  
    #endif

}

static void Bat_HisMaxMinDataCalc(void)
{
    uWord his_max;
    uWord his_min;
    uWord actual_max;
    uWord actual_min;
    uWord his_data;
    uWord actual_data;
    uByte flg_ee_save;

    flg_ee_save = 0;

#ifdef __DEBUG_HISMAXMIN_TEST
      Bat_UpdateHisMinT(80);
      Bat_UpdateHisMaxDeltaT(5);
      Bat_UpdateHisMaxAvgT(50);
      Bat_UpdateHisMinAvgT(70);
      Bat_UpdateHisMaxT(50);
      Bat_UpdateHisMinV(5000);
      Bat_UpdateHisMaxV(1000);
      Bat_UpdateHisDeltaV(10);
      Bat_UpdateHisMaxAvgV(2000);
      Bat_UpdateHisMinAvgV(5000);
      Bat_UpdateHisMaxT(50);
      Bat_UpdateHisMaxSumV(100);
      Bat_UpdateHisMinSumV(3000);
      Bat_UpdateHisMaxR(5);
      Bat_UpdateHisMinR(500);
      Bat_UpdateHisMaxChgCur(30000);
      Bat_UpdateHisMaxDchCur(30000);
      Bat_UpdateHisMaxChgPwr(30000);
      Bat_UpdateHisMaxDchPwr(30000);
      
      EE_SaveVar(BOTHEE,SYSTEMST_HSTY_TEMPER_INFO);
      EE_SaveVar(BOTHEE,SYSTEMST_HSTY_CELLV_INFO);
      EE_SaveVar(BOTHEE,SYSTEMST_HSTY_INRES_INFO);
      EE_SaveVar(BOTHEE,SYSTEMST_HSTY_CHG_DCH_INFO);   
      EE_SaveVar(BOTHEE,SYSTEMST_HSTY_POWER_INFO);
    

      
      
#endif
    /* history mint */
    actual_min = Bat_GetMinT();
    his_min = Bat_GetHisMinT();

    if((his_min > actual_min) && (Bat_GetMinT() < 0xF0))
    {
        Bat_UpdateHisMinT((uByte) actual_min);
        flg_ee_save = 1;
    }

    /* history deltat */
    actual_data =  Bat_GetDeltaT();
    his_data =  Bat_GetHisMaxDeltaT();
    if(his_data > actual_data)
    {
        Bat_UpdateHisMaxDeltaT((uByte) actual_data);
        flg_ee_save = 1;
    }

    /* history max avg temper */
    actual_max =  Bat_GetAvgT();
    his_max = Bat_GetHisMaxAvgT();

    if((his_max < actual_max) && (Bat_GetAvgT() < 0xF0))
    {
        Bat_UpdateHisMaxAvgT((uByte)actual_max);
        flg_ee_save = 1;
    }

    /* history min avg temper */
    actual_min =  Bat_GetAvgT();
    his_min = Bat_GetHisMinAvgT();

    if((his_min > actual_min) && (Bat_GetAvgT() < 0xF0))
    {
        Bat_UpdateHisMinAvgT((uByte)actual_min);
        flg_ee_save = 1;
    }

    /*  history max_t */
    actual_max = Bat_GetMaxT();
    his_max = Bat_GetHisMaxT();

    if((his_max < actual_max) && (Bat_GetMaxT() < 0xF0))
    {
        Bat_UpdateHisMaxT((uByte)actual_max);
        flg_ee_save = 1;
    }

    if(flg_ee_save != 0)
    {
        (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_INFO);
        flg_ee_save = 0;
    }

    /* history min_v */
    actual_min = Bat_GetMinV();
    his_min = Bat_GetHisMinV();

    if((his_min > actual_min) && (Bat_GetMinV() < 0xFFF0))
    {
        Bat_UpdateHisMinV((uWord)actual_min);
        flg_ee_save = 1;
    }

    /* history max_v */
    actual_max = Bat_GetMaxV();
    his_max = Bat_GetHisMaxV();

    if((his_max < actual_max) && (Bat_GetMaxV() < 0xFFF0))
    {
        Bat_UpdateHisMaxV((uWord)actual_max);
        flg_ee_save = 1;
    }

    /* history deltav */
    actual_data =  Bat_GetDeltaV();
    his_data =  Bat_GetHisDeltaV();

    if(his_data < actual_data)
    {
        Bat_UpdateHisDeltaV((uWord)actual_data);
        flg_ee_save = 1;
    }

    /* history max avgv */
    actual_max =  Bat_GetAvgV();
    his_max = Bat_GetHisMaxAvgV();

    if((his_max < actual_max) && (Bat_GetAvgV() < 0xFFF0))
    {
        Bat_UpdateHisMaxAvgV((uWord)actual_max);
        flg_ee_save = 1;
    }

    /* history min avgv */
    actual_min =  Bat_GetAvgV();
    his_min = Bat_GetHisMinAvgV();

    if((his_min > actual_min) && (Bat_GetAvgV() < 0xFFF0))
    {
        Bat_UpdateHisMinAvgV((uWord)actual_min);
        flg_ee_save = 1;
    }

    /* history max sumv */
    actual_max =  Bat_GetSumV(INT_SUMV);
    his_max = Bat_GetHisMaxSumV();

    if(his_max < actual_max)
    {
        Bat_UpdateHisMaxSumV((uWord)actual_max);
        flg_ee_save = 1;
    }

    /* history min sumv */
    actual_min =  Bat_GetSumV(INT_SUMV);
    his_min = Bat_GetHisMinSumV();

    if(his_min > actual_min)
    {
        Bat_UpdateHisMinSumV((uWord)his_min);
        flg_ee_save = 1;
    }

    if(flg_ee_save != 0)
    {
        (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_CELLV_INFO);
        flg_ee_save = 0;
    }

    /* history max res */
    actual_max =  Bat_GetMaxR();
    his_max = Bat_GetHisMaxR();

    if((his_max < actual_max) && (Bat_GetMaxR() < 0xF0))
    {
        Bat_UpdateHisMaxR((uWord)actual_max);
        flg_ee_save = 1;
    }

    /* history min res */
    actual_min =  Bat_GetMinR();
    his_min = Bat_GetHisMinR();

    if((his_min > actual_min)  && (Bat_GetMinR() < 0xF0))
    {
        Bat_UpdateHisMinR((uWord)actual_min);
        flg_ee_save = 1;
    }
    if(flg_ee_save != 0)
    {
        (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_INRES_INFO);
        flg_ee_save = 0;
    }

    /* history max chg cur */
    actual_max =  Bat_GetCurrent(AVGCUR);
    if(actual_max < OFFSET_CUR)
    {
        his_max = Bat_GetHisMaxChgCur();

        if(his_max > actual_max)
        {
            Bat_UpdateHisMaxChgCur((uWord)actual_max);
            flg_ee_save = 1;
        }
    }

    /* history max dch cur */
    actual_max =  Bat_GetCurrent(AVGCUR);
    if(actual_max > OFFSET_CUR)
    {
        his_max = Bat_GetHisMaxDchCur();
        if(his_max < actual_max)
        {
            Bat_UpdateHisMaxDchCur((uWord) actual_max);
            flg_ee_save = 1;
        }
    }

    /* history max chg power */
    actual_max =  Bat_GetPwr();  
    if(actual_max < OFFSET_PWR)
    {
        his_max = Bat_GetHisMaxChgPwr();
        if(his_max > actual_max)
        {
            Bat_UpdateHisMaxChgPwr((uWord)actual_max);
            flg_ee_save = 1;
        }
    }


    /* history max dch power */
    actual_max =  Bat_GetPwr();
    if(actual_max > OFFSET_PWR)
    { 
        his_max = Bat_GetHisMaxDchPwr();
        if(his_max < actual_max)
        {
            Bat_UpdateHisMaxDchPwr((uWord)actual_max);
            flg_ee_save = 1;
        }
    }

    if(flg_ee_save != 0)
    {
        (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_POWER_INFO);
        flg_ee_save = 0;
    }

    // prevent C5917
    (void)flg_ee_save;
    /* history distribution */
}

static void Bat_HisChgDataCalc(void)
{
    /* chg times*/
    uByte chg_st;
    uWord chg_times,chg_full_times;
    uWord dch_empty_times;
    uByte flg_ee_save;
#ifdef __DEBUG_BATST_TEST
     Chg_UpdateChgSt(CHG_ST_ON);
     Chg_UpdateChgStage(CHG_STAGE_END);
     
#endif
    flg_ee_save = 0;

    chg_st = Chg_GetChgSt();
    chg_times = Bat_GetHisChgTimes();

    if(BAT_FLAG_FAIL == Bat_StCalc.ChgTimesFlg)
    {
        if(CHG_ST_ON == chg_st)
        {
            if(Bat_TmCnt.ChgData < BAT_CNT_CHG_TIMES)
            {
                Bat_TmCnt.ChgData++;
            }
            else
            {
                Bat_StCalc.ChgTimesFlg = BAT_FLAG_OK;
                Bat_UpdateHisChgTimes(++chg_times);
                Bat_TmCnt.ChgData = BAT_CNT_CLEAR;

                flg_ee_save = 1;
            }
        }
        
    }
    
    /* chg full times */
    chg_full_times = Bat_GetHisChgFullTimes();
    if(BAT_FLAG_FAIL == Bat_StCalc.ChgFullFlg)
    {
        if((CHG_STAGE_END == Chg_GetChgStage()) && (ChgM_GetChgStartCnt() >= BAT_CHG_START_CNT) )
        {
            Bat_StCalc.ChgFullFlg = BAT_FLAG_OK;
            Bat_UpdateHisChgFullTimes(++chg_full_times);
            flg_ee_save = 1;
        }
    }

    /* dch empty times */
    Bat_StCalc.PwrOnTimeCalc++;
    dch_empty_times = Bat_GetDchEmptyTimes();
    if((ERR_LEVEL_TWO == Err_GetBatErrLevel(ERR_BAT_CELLV_LOW)) && (Bat_StCalc.PwrOnTimeCalc >= BAT_CNT_POW_ON))
    {
          Bat_StCalc.PwrOnTimeCalc = BAT_CNT_CLEAR;
          Bat_UpdateDchEmptyTimes(++dch_empty_times);
          flg_ee_save = 1;
       
    }

    if(flg_ee_save != 0)
    {
        (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_CHG_DCH_INFO);
        flg_ee_save = 0;
    }
    
     // prevent C5917
    (void)flg_ee_save;
}

static void  Bat_CalcChgAccTime(void)
 {
    if((CHG_ST_ON == Chg_GetChgSt()) && (Bat_GetCurrent(INSCUR) < (OFFSET_CUR - 20)))
    {
       if(Bat_TmCnt.ChgAccTime < BAT_CNT_ACC_CHG_TIME)
       {
          Bat_TmCnt.ChgAccTime++;
       }
       else
       {
           Bat_TmCnt.ChgAccTime = 0;
           Chg_UpdateAccumulatedChgTime(Chg_GetAccumulatedChgTime() + 1);
       }
    }
   
 }

   
static void Bat_TemperRiseCalc(void)          /*from power on */
{
    uByte temp_rise;
    
    if(Bat_GetMaxT() >= 0xF0)
    {
      return;
    }
   
    if(BAT_FLAG_FAIL == Bat_StCalc.MaxTBakFlg)
    { 
        Bat_StCalc.MaxTBakFlg = BAT_FLAG_OK; 
        Bat_StCalc.MaxTBak = Bat_GetMaxT(); 
    }
    if(Bat_TmCnt.TempRise < BAT_CNT_TEMPER_RISE)  /* 1 min */
    {
        Bat_TmCnt.TempRise++;
    }
    else
    {
       (Bat_GetMaxT() > Bat_StCalc.MaxTBak) ? (temp_rise = Bat_GetMaxT() - Bat_StCalc.MaxTBak) : (temp_rise = 0);

        Bat_TmCnt.TempRise = BAT_CNT_CLEAR;
        Bat_UpdateBatteryTemperRise(temp_rise);
    }
}


static void Bat_TemperRisePerHourCalc(void)
{
    uByte temp_rise;
    
    if(Bat_GetMaxT() >= 0xF0)
    {
      return;
    }
    
    if(BAT_FLAG_FAIL == Bat_StCalc.PerHourFlg) 
    {
       
        Bat_StCalc.PerHourFlg = BAT_FLAG_OK; 
        Bat_StCalc.MaxTBakPerHour = Bat_GetMaxT(); 
    }
    if(Bat_TmCnt.TempRisePerHour < BAT_CNT_1HOUR)
    {
        Bat_TmCnt.TempRisePerHour++;
    }
    else
    {
       
        (Bat_GetMaxT() > Bat_StCalc.MaxTBakPerHour) ? (temp_rise = Bat_GetMaxT() - Bat_StCalc.MaxTBakPerHour) : (temp_rise = 0);

        Bat_TmCnt.TempRisePerHour = BAT_CNT_CLEAR;
        Bat_StCalc.PerHourFlg = BAT_FLAG_FAIL; 
        Bat_UpdateTemperRisePerHour(temp_rise);
    }
}


static void Bat_TemperMaxTRiseRateCalc(void)
{
    uByte temp_rise;
    uWord temp_rise_rate;

    if(Bat_GetMaxT() >= 0xF0)
    {
      return;
    }
    
    if(BAT_FLAG_OK != Bat_StCalc.TemperRiseRateFlg)
    {
        Bat_StCalc.InitMaxT = Bat_GetMaxT();
        Bat_StCalc.TemperRiseRateFlg = BAT_FLAG_OK;
    }
    if(Bat_TmCnt.RiseRate < BAT_CNT_TEMPER_RISE_RATE)
    {
        Bat_TmCnt.RiseRate++;
    }
    else
    {
        (Bat_GetMaxT() > Bat_StCalc.InitMaxT) ? (temp_rise = Bat_GetMaxT()  - Bat_StCalc.InitMaxT) : (temp_rise = 0);

        temp_rise_rate = temp_rise;
        temp_rise_rate = (temp_rise_rate * 10) / (BAT_CNT_TEMPER_RISE_RATE / 5);   

        Bat_TmCnt.RiseRate = BAT_CNT_CLEAR;
        Bat_StCalc.TemperRiseRateFlg = BAT_FLAG_FAIL;
        Bat_UpdateMaxTRiseRate((uByte)temp_rise_rate);
    }
}

static void Bat_SocDistribution(void)
{
    Bat_SOCType soc;
    uByte i;
    uLWord soc_dis[10];
    uByte cnt;

    soc = Bat_GetSOC();

    if(Bat_TmCnt.SocDis < BAT_CNT_SOC_DIS)  
    {
        Bat_TmCnt.SocDis++;
    }
    else
    {
        cnt = sizeof(soc_dis) / sizeof(soc_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            soc_dis[i] = Bat_GetSocDistribution(i);
            if(soc <= Bat_SocDis[i])
            {
                Bat_UpdateSocDistribution(i,++soc_dis[i]);

                if(i < 7)
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_SOC_AREA_INFO_G1);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_SOC_AREA_INFO_G2);
                }
                
                break;
            }
        }
        Bat_TmCnt.SocDis = BAT_CNT_CLEAR;
    }
}


static void Bat_MinTempDistribution(void)
{
    
    uLWord temp_dis[14];
    uByte min_t,i;
    uByte cnt;
    
    if(Bat_GetMinT() >= 0xF0)
    {
       return;
    }
    
    if(Bat_TmCnt.MinTempDis < BAT_CNT_MIN_TEMP_DIS)  //config
    {
        Bat_TmCnt.MinTempDis++;
    }
    else
    {
        min_t = Bat_GetMinT();
        cnt =  sizeof(temp_dis) / sizeof(temp_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            temp_dis[i] = Bat_GetMinTDistribution(i);
           
            if(min_t <= Bat_TemperDis[i])
            {
                Bat_UpdateMinTDistribution(i, ++temp_dis[i]);
                if(i < 7)
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G1);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G2);
                }
                
                break;
            }
        }
        Bat_TmCnt.MinTempDis = BAT_CNT_CLEAR;
       
    }
  
}


static void Bat_MaxTempDistribution(void)
{
    
    uLWord temp_dis[14];
    uByte i,cnt;
    uByte max_t;
    
    if(Bat_GetMaxT() >= 0xF0)
    {
       return;
    }
   
    if(Bat_TmCnt.MaxTempDis < BAT_CNT_MAX_TEMP_DIS)  //config
    {
        Bat_TmCnt.MaxTempDis++;
    }
    else
    {
        max_t = Bat_GetMaxT();
        cnt = sizeof(temp_dis) / sizeof(temp_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            temp_dis[i] = Bat_GetMaxTDistribution(i);
           
            if(max_t <= Bat_TemperDis[i])
            {
                Bat_UpdateMaxTDistribution(i, ++temp_dis[i]);
                if(i < 7)
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G3);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G4);
                }
               
                break;
            }
        }
        Bat_TmCnt.MaxTempDis = BAT_CNT_CLEAR;
       
    }
  
}



static void Bat_AvgTempDistribution(void)
{
    
    uLWord temp_dis[14];
    uByte i, cnt,avg_t;
    
    if(Bat_GetAvgT() >= 0xF0)
    {
       return;
    }
    
    if(Bat_TmCnt.AvgTempDis < BAT_CNT_AVG_TEMP_DIS)  //config
    {
        Bat_TmCnt.AvgTempDis++;
    }
    else
    {
        avg_t = Bat_GetAvgT();
        cnt = sizeof(temp_dis) / sizeof(temp_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            temp_dis[i] = Bat_GetAvgTDistribution(i);
            if(avg_t <= Bat_TemperDis[i])
            {
                Bat_UpdateAvgTDistribution(i, ++temp_dis[i]);
                if(i < 7)
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G5);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G6);
                }
                break;
            }
            
        }
        Bat_TmCnt.AvgTempDis = BAT_CNT_CLEAR;
       
    }
  
}


static void Bat_DeltaTempDistribution(void)
{
    
    uLWord temp_dis[14];
    uByte i, cnt,delta_t;
    
#ifdef __DEBUG_BATST_TEST
    Bat_UpdateDeltaT(7);
#endif
    
    if(Bat_TmCnt.DeltaTempDis < BAT_CNT_DELTA_TEMP_DIS)  //config
    {
        Bat_TmCnt.DeltaTempDis++;
    }
    else
    {
        delta_t = Bat_GetDeltaT();
        cnt = sizeof(temp_dis) / sizeof(temp_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            temp_dis[i] = Bat_GetMaxDeltaTDistribution(i);
            if(delta_t <= Bat_DeltaTemperDis[i])
            {
                Bat_UpdateMaxDeltaTDistribution(i, ++temp_dis[i]);
                if(i < 7)
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G7);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_TEMPER_AREA_INFO_G8);
                }
                
                break;
            }
            
        }
        Bat_TmCnt.DeltaTempDis = BAT_CNT_CLEAR;
       
    }
  
}

static void Bat_CurDistribution(void)
{
    uByte i,cnt;
    uLWord cur_dis[14];
    uWord cur;

#ifdef __DEBUG_BATST_TEST
    Bat_UpdateCurrent(AVGCUR, 32000);
#endif    
   
    if(Bat_TmCnt.CurDis < BAT_CNT_CUR_DIS)  //config
    {
        Bat_TmCnt.CurDis++;
    }
    else
    {
        cur = Bat_GetCurrent(AVGCUR);
        cnt = sizeof(cur_dis) / sizeof(cur_dis[0]);
        for(i = 0; i < cnt; i++)
        {
            cur_dis[i] = Bat_GetCurDistribution(i);

            if( cur <= Bat_CurDis[i] )
            {
                Bat_UpdateCurDistribution(i, ++cur_dis[i]);
                if(i < 7)
                {                                  
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_CURRENT_AREA_INFO_G1);
                }
                else
                {
                    (void)EE_SaveVar(BOTHEE, SYSTEMST_HSTY_CURRENT_AREA_INFO_G2);
                }
                
                break;
            }
        }
        Bat_TmCnt.CurDis = BAT_CNT_CLEAR;
    }
}


static void Bat_LoadDisInfo(void)
{
    if(Bat_TmCnt.LoadDataCnt >= TM_MS(60))
    {
       Bat_TmCnt.LoadDataCnt = 0;
       (void)EE_LoadVar(Bat_DataBuf[Bat_StCalc.Cnt++]); 
    }
    if(Bat_StCalc.Cnt >= HIS_INFO_DATA_LEN)
    {
       Bat_StCalc.LoadDataFlg = BAT_FLAG_OK; 
    }
    
}


//uWord total_temper;
//uWord total_serial_num, total_temper_num;
//uByte test_temper[50];
/* calc all pack max and min infor */
void Bat_CalcAllMaxMin(void)
{
    uByte i;
    uWord wtmp;
    uByte bmu_no;
    uByte start_chip_no;
    uByte total_chip_num;
    uWord local_temper_no, local_cell_no;
    uWord total_serial_num, total_temper_num;
    
    uByte temper, max_t, min_t, avg_t, delta_t;
#ifdef BMS_ENABLE_CALC_INTRES    
    uByte cell_r, min_r, max_r;
    uByte max_r_bmu_no, min_r_bmu_no;
    uWord pack_max_r_no, pack_min_r_no;
    uWord max_r_total_no, min_r_total_no;
#endif    
    uWord cell_v, min_v, max_v, avg_v, delta_v;
    
    uByte max_v_bmu_no, min_v_bmu_no;
    uByte min_t_bmu_no, max_t_bmu_no;
    
    uWord max_v_total_no, min_v_total_no;
    uWord max_t_total_no, min_t_total_no;
 
    uLWord total_v;
    uWord total_temper;
    
    uWord pack_max_v_no, pack_min_v_no;
    uWord pack_max_t_no, pack_min_t_no;

    start_chip_no = 1;
    pack_max_v_no = 1;
    pack_min_v_no = 1;
    pack_max_t_no = 1;
    pack_min_t_no = 1;
#ifdef BMS_ENABLE_CALC_INTRES  
    pack_max_r_no = 1;
    pack_min_r_no = 1;
    max_r_bmu_no = 1;
    min_r_bmu_no = 1;
#endif /* #ifdef BMS_ENABLE_CALC_INTRES  */
    max_v_bmu_no = 1;
    min_v_bmu_no = 1;
    max_t_bmu_no = 1;
    min_t_bmu_no = 1;

     max_v = 0;
     max_t = 0;
     min_t = 0xff;
     min_v = 0xffff; 
     total_chip_num = Bat_GetChipNum() + 1;
#ifdef BMS_ENABLE_CALC_INTRES  
     max_r = 0;
     min_r = 0xff;
#endif  /* #ifdef BMS_ENABLE_CALC_INTRES */    
     total_v = 0;
     total_temper =0;
     total_serial_num = 0;
     total_temper_num = 0;
     max_v_total_no = 0;
     min_v_total_no  = 0;
     max_t_total_no = 0;
     min_t_total_no  = 0;
     
#ifdef __DEBUG_BATST_TEST_BCU_D
    for(bmu_no = start_chip_no ; bmu_no < total_chip_num; bmu_no++)
    {
        Bat_UpdatePackMinV(bmu_no, 3000 + bmu_no * 10);
        Bat_UpdatePackMaxV(bmu_no, 3500 + bmu_no * 10);
        Bat_UpdatePackMaxT(bmu_no, 50 + bmu_no);
        Bat_UpdatePackMinT(bmu_no, 40 + bmu_no);

        Bat_UpdatePackMaxVNo(bmu_no, bmu_no);
        Bat_UpdatePackMinVNo(bmu_no, bmu_no);
        Bat_UpdatePackMaxTNo(bmu_no, bmu_no);
        Bat_UpdatePackMinTNo(bmu_no, bmu_no);

        Bat_UpdatePackSerialNum(bmu_no, bmu_no * 5);
        Bat_UpdatePackTemperNum(bmu_no, bmu_no * 1);

        Bat_UpdatePackSumV(bmu_no, ACC_SUMV, 500);
    }

    for(bmu_no = start_chip_no; bmu_no < total_chip_num; bmu_no++)
    {
        for(i = 1; i < Bat_GetPackTemperNum(bmu_no) + 1; i++)
        {
            Bat_UpdateTemper(bmu_no, i,40 + i);
        }

        for(i = 1; i < Bat_GetPackSerialNum(bmu_no) + 1; i ++)
        {
            Bat_UpdateCellR(bmu_no, i, 100 + i * 5);
        }
    }
#endif /* #if __DEBUG_BATST_TEST_BCU_D */

    for(bmu_no = start_chip_no; bmu_no < total_chip_num; bmu_no++)     //Bat_UpdatePackSerialNum
    {
        
        total_serial_num += Bat_GetPackSerialNum(bmu_no);      
        total_temper_num += Bat_GetPackTemperNum(bmu_no);
        for(local_cell_no = 1; local_cell_no < (Bat_GetPackSerialNum(bmu_no) + 1); local_cell_no++)
        {
            if(0xFFF0 <= Bat_GetCellV(bmu_no, local_cell_no))
            {
               total_serial_num -= 1;
            }
            else
            {
                cell_v = (uLWord)(Bat_GetCellV(bmu_no, local_cell_no));
                  
                total_v += cell_v; 
                if(0xFFF0 > cell_v)
                {
                  if(min_v > cell_v)
                  {
                      min_v = cell_v;
                      min_v_bmu_no = bmu_no;
                      (void)Bat_UpdatePackMinVNo(bmu_no, local_cell_no);
                  }
                }
               
                if(0xFFF0 > cell_v)
                {
                  if(max_v < cell_v)
                  {
                      max_v = cell_v;
                      max_v_bmu_no = bmu_no;
                      (void)Bat_UpdatePackMaxVNo(bmu_no, local_cell_no);
                  }
                }
            }
        }
        for(local_temper_no = 1; local_temper_no < (Bat_GetPackTemperNum(bmu_no) + 1); local_temper_no++)
        {
            if(0xF0 > Bat_GetTemper(bmu_no, local_temper_no))
            {
                temper =  Bat_GetTemper(bmu_no, local_temper_no);
                total_temper += temper;
                if(0xF0 > temper)
                {
                    if(min_t > temper)
                    {
                        min_t = temper;
                        min_t_bmu_no = bmu_no;
                        (void)Bat_UpdatePackMinTNo(bmu_no, local_temper_no);
                    }
                    
                    if(max_t < temper)
                    {
                        max_t = temper;
                        max_t_bmu_no = bmu_no;
                        (void)Bat_UpdatePackMaxTNo(bmu_no, local_temper_no);
                    }
                }
            }
            else
            {
                total_temper_num -= 1;
            }
            local_temper_no += 2;
        }
 
      /*  
        cell_v = Bat_GetPackMinV(bmu_no);
        temper = Bat_GetPackMinT(bmu_no);

        if(0xFFF0 > cell_v)
        {
          if(min_v > cell_v)
          {
              min_v = cell_v;
              min_v_bmu_no = bmu_no;
          }
        }

        if(0xF0 > temper)
        {
          if(min_t > temper)
          {
              min_t = temper;
              min_t_bmu_no = bmu_no;
          }
        }

        cell_v = Bat_GetPackMaxV(bmu_no);
        temper = Bat_GetPackMaxT(bmu_no);

        if(0xFFF0 > cell_v)
        {
          if(max_v < cell_v)
          {
              max_v = cell_v;
              max_v_bmu_no = bmu_no;
          }
        }
       
        if(0xF0 > temper)
        {
          if(max_t < temper)
          {
              max_t = temper;
              max_t_bmu_no = bmu_no;
          }
        }
        
        if((0xFFF0 > Bat_GetPackMaxV(bmu_no)) && (0xFFF0 > Bat_GetPackMinV(bmu_no)))
        {
          (Bat_GetPackMaxV(bmu_no) > Bat_GetPackMinV(bmu_no)) ? (delta_v = Bat_GetPackMaxV(bmu_no) - Bat_GetPackMinV(bmu_no)) : (delta_v = 0);
           (void)Bat_UpdatePackDeltaV(bmu_no, delta_v);
        }
        else
        {
           (void)Bat_UpdatePackDeltaV(bmu_no, 0);
        }
        if((0xF0 > Bat_GetPackMaxT(bmu_no)) && (0xF0 > Bat_GetPackMinT(bmu_no)))
        {
          (Bat_GetPackMaxT(bmu_no) > Bat_GetPackMinT(bmu_no)) ? (delta_t = Bat_GetPackMaxT(bmu_no) - Bat_GetPackMinT(bmu_no)) : (delta_t = 0);
           (void)Bat_UpdatePackDeltaT(bmu_no, delta_t);
        }
        else
        {
            (void)Bat_UpdatePackDeltaT(bmu_no, 0);
        }
        
      //  total_v += Bat_GetPackSumV(bmu_no, ACC_SUMV); 
        total_serial_num += Bat_GetPackSerialNum(bmu_no);      
        total_temper_num += Bat_GetPackTemperNum(bmu_no);

        for(local_cell_no = 1; local_cell_no < (Bat_GetPackSerialNum(bmu_no) + 1); local_cell_no++)
        {
            if(0xFFF0 <= Bat_GetCellV(bmu_no, local_cell_no))
            {
               total_serial_num -= 1;
            }
            else
            {
               total_v += (uLWord)(Bat_GetCellV(bmu_no, local_cell_no)); 
            }
        }
        for(local_temper_no = 1; local_temper_no < (Bat_GetPackTemperNum(bmu_no) + 1); local_temper_no++)
        {
            if(0xF0 > Bat_GetTemper(bmu_no, local_temper_no))
            {
                total_temper += Bat_GetTemper(bmu_no, local_temper_no);
                //test_temper[(bmu_no-1)*10 + (local_temper_no - 1) ] =  Bat_GetTemper(bmu_no, local_temper_no);
            }
            else
            {
                total_temper_num -= 1;
            }
        }
        
      */

#ifdef BMS_ENABLE_CALC_INTRES
        for(local_cell_no = 1; local_cell_no < (Bat_GetPackSerialNum(bmu_no) + 1); local_cell_no++)
        {
            cell_r = Bat_GetCellR(bmu_no, local_cell_no);

            if(0xFFF0 > cell_r)
            {
                if(cell_r < min_r)
                {
                    min_r = cell_r;
                    min_r_bmu_no = bmu_no;
                    (void)Bat_UpdatePackMinRNo(bmu_no, local_cell_no);
                }

                if(cell_r > max_r)
                {
                    max_r = cell_r;
                    max_r_bmu_no = bmu_no;
                    (void)Bat_UpdatePackMaxRNo(bmu_no, local_cell_no);
                }
            }
        }
#endif /* #ifdef BMS_ENABLE_CALC_INTRES */
    }

     

    (max_v > min_v) ? (delta_v = max_v - min_v) : (delta_v = 0);
    (max_t > min_t) ? (delta_t = max_t - min_t) : (delta_t = 0);

    avg_v = (uWord)(total_v  / total_serial_num);
    if((avg_v > max_v) || (avg_v < min_v) || (total_serial_num != Bat_GetBatNum()))
    {
        avg_v = (uWord)((uLWord)Bat_GetSumV(INT_SUMV) * 100 / Bat_GetBatNum());
    }
    if((avg_v > max_v) || (avg_v < min_v))
    {
        avg_v = (max_v + min_v) / 2 ;
    }
    
    if((total_serial_num == 0) || (min_v >= 0xFFF0))
    {
       min_v = 0xFFFF; 
    }
    if((total_serial_num == 0) || (max_v >= 0xFFF0))
    {       
       max_v = 0xFFFF;       
    }
    if((total_serial_num == 0) || (avg_v >= 0xFFF0))
    {
        avg_v = 0xFFFF;
    }

   
    
    avg_t = (uByte)(total_temper / total_temper_num);

    if((total_temper_num == 0) || (min_t >= 0xF0))
    {
       min_t = 0xFF;
    }
    if((total_temper_num == 0) || (max_t >= 0xF0))
    {   
       max_t = 0xFF;  
    }
    if((total_temper_num == 0) || (avg_t >= 0xF0))
    {
       avg_t = 0xFF;
    }
   

    pack_max_v_no = Bat_GetPackMaxVNo(max_v_bmu_no);
    pack_min_v_no = Bat_GetPackMinVNo(min_v_bmu_no);
    pack_max_t_no = Bat_GetPackMaxTNo(max_t_bmu_no);
    pack_min_t_no = Bat_GetPackMinTNo(min_t_bmu_no); 

#ifdef BMS_ENABLE_CALC_INTRES
    pack_max_r_no = Bat_GetPackMaxRNo(max_r_bmu_no);
    pack_min_r_no = Bat_GetPackMinRNo(min_r_bmu_no);
#endif  /* #ifdef BMS_ENABLE_CALC_INTRES */


    wtmp = 0;  
    for(i = start_chip_no; i < max_v_bmu_no ; i++)
    {
        wtmp += Bat_GetPackSerialNum(i);
    }
    max_v_total_no =  pack_max_v_no + wtmp;

    wtmp = 0;
    for(i = start_chip_no; i < min_v_bmu_no; i++)
    {
        wtmp +=  Bat_GetPackSerialNum(i);
    }
    min_v_total_no = pack_min_v_no + wtmp;

    wtmp = 0;
    for(i = start_chip_no; i < max_t_bmu_no; i++)
    {
        wtmp +=  1;//Bat_GetPackTemperNum(i);
    }
    max_t_total_no = pack_max_t_no + wtmp;

   

    wtmp = 0;
    for(i = start_chip_no; i < min_t_bmu_no; i++)
    {
        wtmp +=  1;//Bat_GetPackTemperNum(i);
    }
    min_t_total_no = pack_min_t_no + wtmp;

    
 #ifdef BMS_ENABLE_CALC_INTRES   
    wtmp = 0;
    for(i = start_chip_no; i < max_r_bmu_no ; i++)
    {
        wtmp +=  Bat_GetPackSerialNum(i);
    }
    max_r_total_no =  pack_max_r_no + wtmp;


    wtmp = 0;   
    for(i = start_chip_no; i < min_r_bmu_no; i++)
    {
        wtmp +=  Bat_GetPackSerialNum(i);
    }
    min_r_total_no = pack_min_r_no + wtmp;

 #endif /* #ifdef BMS_ENABLE_CALC_INTRES */


   
    Bat_UpdateDeltaT(delta_t);
    Bat_UpdateDeltaV(delta_v);

    Bat_UpdateAvgV(avg_v);
    Bat_UpdateAvgT(avg_t);

    Bat_UpdateMaxV(max_v);
    Bat_UpdateMaxT(max_t);
    Bat_UpdateMinV(min_v);
    Bat_UpdateMinT(min_t);

    Bat_UpdateMaxVTotalNo(max_v_total_no);
    Bat_UpdateMinVTotalNo(min_v_total_no);
    Bat_UpdateMaxTTotalNo(max_t_total_no);
    Bat_UpdateMinTTotalNo(min_t_total_no);

    Bat_UpdateMaxVBmuNo(max_v_bmu_no);
    Bat_UpdateMinVBmuNo(min_v_bmu_no);
    Bat_UpdateMaxTBmuNo(max_t_bmu_no);
    Bat_UpdateMinTBmuNo(min_t_bmu_no);
                             
    Bat_UpdateMaxVSingleNo((uByte)pack_max_v_no);
    Bat_UpdateMinVSingleNo((uByte)pack_min_v_no);
    Bat_UpdateMaxTSingleNo((uByte)pack_max_t_no);
    Bat_UpdateMinTSingleNo((uByte)pack_min_t_no);
    
           
    wtmp = (uWord)(total_v / 100);  //0.1v/bit
    Bat_UpdateSumV(ACC_SUMV, wtmp);

    #ifdef BMS_ENABLE_CALC_INTRES
        Bat_UpdateMaxR(max_r);
        Bat_UpdateMinR(min_r);
        Bat_UpdateMaxRTotalNo(max_r_total_no);
        Bat_UpdateMinRTotalNo(min_r_total_no);
        Bat_UpdateMaxRBmuNo(max_r_bmu_no);
        Bat_UpdateMinRBmuNo(min_r_bmu_no);
        Bat_UpdateMaxRSingleNo(pack_max_r_no);
        Bat_UpdateMinRSingleNo(pack_min_r_no);
    #endif /* #ifdef BMS_ENABLE_CALC_INTRES */
}
/* end of calc all pack max and min infor */

void Bat_CountCbk(void)
{
    (Bat_TmCnt.StaticInfoCnt < 255) ? (Bat_TmCnt.StaticInfoCnt++) : (Bat_TmCnt.StaticInfoCnt = 255);
    (Bat_TmCnt.DisInfoCnt < 255) ? (Bat_TmCnt.DisInfoCnt++) : (Bat_TmCnt.DisInfoCnt = 255);
    (Bat_TmCnt.LoadDataCnt < 255) ? (Bat_TmCnt.LoadDataCnt++) : (Bat_TmCnt.LoadDataCnt = 255);
   
}


void Bat_Main(void)
{
    /* deal System Information, such as max_v,max_t */
    if(Bat_TmCnt.StaticInfoCnt >= TM_MS(10))
    {
        Bat_TmCnt.StaticInfoCnt = 0;

        Bat_CalcAllMaxMin(); 
    }
 
    if(BAT_FLAG_FAIL == Bat_StCalc.LoadDataFlg)
    {
         Bat_LoadDisInfo();
    }
    if(Bat_TmCnt.DisInfoCnt >= TM_MS(1000))
    {
        Bat_TmCnt.DisInfoCnt = 0;

        if(0 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_SocDistribution();
        }
        else if(1 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_CurDistribution();
        }
        else if(2 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_MaxTempDistribution();
        }
        else if(3 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_MinTempDistribution();
        }
        else if(4 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_AvgTempDistribution();
        }
        else if(5 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_DeltaTempDistribution();
        }
        else if(6 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_CalcChgAccTime();
        }
        else if(7 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_HisChgDataCalc();
        }
        else if(8 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_TemperRiseCalc();
        }   
        else if(9 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_HisMaxMinDataCalc();
        } 
        else if(10 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo++;
            Bat_TemperRisePerHourCalc();
        } 
        else if(11 == Bat_StCalc.LoadDisInfo)
        {
            Bat_StCalc.LoadDisInfo = 0;
            Bat_TemperMaxTRiseRateCalc();  
        } 

    }
}

#pragma DATA_SEG DEFAULT
/*
** End Of Data Process
*/
