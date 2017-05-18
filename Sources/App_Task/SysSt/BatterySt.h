// BatterySt.h
#ifndef BATTERYST_H
#define BATTERYST_H

#include "GeneralTypeDef.h"
#include "Ecu.h"


#define OFFSET_CUR 30000
#define OFFSET_PWR 30000

#define BAT_FLAG_FAIL 0
#define BAT_FLAG_OK 1

#define BAT_CNT_ACC_CHG_TIME  5         /* 1 min */
#define BAT_CNT_SOC_DIS       50        /* 10 min */
#define BAT_CNT_CUR_DIS       50        /* 10 min */
#define BAT_CNT_MAX_TEMP_DIS  50        /* 10 min */
#define BAT_CNT_MIN_TEMP_DIS  50        /* 10 min */
#define BAT_CNT_AVG_TEMP_DIS  50        /* 10 min */
#define BAT_CNT_DELTA_TEMP_DIS 50       /* 10 min */

#define BAT_CNT_TEMPER_RISE   5         /* 1 min */
#define BAT_CNT_1HOUR         300       /* 1 Hour */
#define BAT_CNT_TEMPER_RISE_RATE  50    /* 10 min */
#define BAT_CNT_POW_ON        25        /* 5 min  */
#define BAT_CNT_CHG_TIMES     25        /* 5 min  */
#define BAT_CNT_CLEAR         0        
#define BAT_CHG_START_CNT     25        /* 5min */

#define ERR_FAIL_GET  0xFF

typedef enum
{
    BATST_ERR_UPDATE  =  ERR_DATA_SYSTEMST_BASE,
    BATST_ERR_GET,
}eBat_ReturnType;


typedef enum
{
    PRIDE = 1,
    MGL,
    CALB,
    LISHEN,
    BOSTON,
    BAK,
} eBat_ManufacturerType;

typedef enum
{
    LFP = 1,
    LMO,
    NCM,
    VRLA,
    NIMH,   
    LCO, 
    PLIB,
    LTO,
    OTHER
    
} eBat_MaterialType;


typedef enum
{
    ACC_SUMV,
    INT_SUMV,
    OUT_SUMV,
    INT_SUMV_BAK,
    OUT_SUMV_BAK,
    INT_SUMV_BAK1,
    OUT_SUMV_BAK1,
} eBat_SumvType;

typedef enum
{
    INSCUR,
    AVGCUR,
} eCurrentType;

typedef struct
{
    uWord ChgBalTimeTotal[MAX_CELL_NUM * 3 / 4];
    uWord DchBalTimeTotal[MAX_CELL_NUM * 3 / 4];

#ifdef BMS_ENABLE_SINGLE_BAL_TIME
    uWord DchBalTimeSingle[MAX_CELL_NUM];
    uWord ChgBalTimeSingle[MAX_CELL_NUM];
#endif

} sCell_BalanceTimeType;

typedef union
{
    uByte  Info;
    struct
    { 
       uByte CellOtherErr :    1;
       uByte CellOpenWire :    1;
       uByte ChgBalErr    :    1;
       uByte DchBalErr    :    1;
       uByte ChgBal       :    1;
       uByte DchBal       :    1;
       uByte rsv6         :    1;
       uByte rsv7         :    1;
          
    } bit;
} sCell_InfoType;

typedef struct
{
    uWord Voltage;
#ifdef BMS_ENABLE_CALC_INTRES
    uByte Int_Res;
#endif
#ifdef   BMS_ENABLE_CALC_CELL_SOC
    uWord Soc;      /* Reserved */
#endif
    //uByte Soh;    /* Reserved */

} sBat_CellBasicType;


typedef struct
{
    sCell_InfoType Cell_Info;
} sBat_CellStType;

typedef struct
{
    uByte Temper;

} sBat_PackTemperType;

#define Bat_SOCType uWord

typedef struct
{
    uWord AccSumV;
    uWord IntSumV;

    uWord IntSumvBak;
    uWord OutSumV;
    uWord OutSumvBak;
    uWord IntSumvBak1;
    uWord OutSumvBak1;
} sBat_SumvType;

typedef struct
{
    uLWord RatedCapacity;
    uLWord ActualCapacity;
} sBat_CapacityType;

typedef struct
{
    uLWord TotalChgAh;
    uLWord TotalDchAh;
} sBat_TotalAhType;

typedef struct
{
    uWord InsCur;
    uWord AvgCur;
} sBat_CurrentType;

typedef struct
{
    /* voltage statistic */
    uWord MaxV;
    uWord MinV;
    uWord MaxVNo;
    uWord MinVNo;
    /* temperature statistic */
    uByte MaxT;
    uByte MinT;
    uWord MaxTNo;
    uWord MinTNo;
    /* Resistance statistic */
    uByte MaxR;
    uByte MinR;
    uWord MaxRNo;
    uWord MinRNo;
} sBat_PackMaxMinType;

typedef struct
{
    /* voltage statistic */
    uWord MaxV;
    uWord MinV;
    uWord MaxVTotalNo;
    uWord MinVTotalNo;
    uByte MaxVSingleNo;
    uByte MinVSingleNo;
    uByte MaxVBmuNo;
    uByte MinVBmuNo;
    /* temperature statistic */
    uByte MaxT;
    uByte MinT;
    uWord MaxTTotalNo;
    uWord MinTTotalNo;
    uByte MaxTSingleNo;
    uByte MinTSingleNo;
    uByte MaxTBmuNo;
    uByte MinTBmuNo;
    /* Resistance statistic */
    uByte MaxR;
    uByte MinR;
    uWord MaxRTotalNo;
    uWord MinRTotalNo;
    uByte MaxRSingleNo;
    uByte MinRSingleNo;
    uByte MaxRBmuNo;
    uByte MinRBmuNo;
    //uWord MaxPermitChgCur;
    uWord MaxPermitDchCur;
    uWord MaxFeedBackCur;
    uWord MaxPermitFeedBackPower;
    uWord MaxPermitDchPower;
    uWord MaxPulseDchCur;
    uWord MaxPulseFeedBackCur;
    uWord MaxPulseDchPower;
    uWord MaxPulseFeedBackPower;
} sBat_AllPackMaxMinType;

typedef struct
{
    uWord HisMaxV;
    uWord HisMinV;

    uWord HisMaxDeltaV;

    uWord HisMaxAvgV;
    uWord HisMinAvgV;

    uWord HisMaxSumV;
    uWord HisMinSumV;

} Bat_HisVoltInfoType;


typedef struct
{
    uByte HisMaxT;
    uByte HisMinT;

    uByte HisMaxDeltaT;
    uByte HisMaxAvgT;
    uByte HisMinAvgT;

} Bat_HisTemperInfoType;

typedef struct
{
    uWord HisMaxR;
    uWord HisMinR;

} Bat_HisIntResInfoType;


typedef struct
{
    uWord HisMaxChgPower;
    uWord HisMaxDchPower;
    uWord HisMaxChgCur;
    uWord HisMaxDchCur;
    

} Bat_HisPowerInfoType;


typedef struct
{
    uWord ChgTimes;
    uWord ChgFullTimes;
    uWord DchEmptyTimes;

} Bat_HisChgDchgInfoType;

typedef struct
{
    uLWord MinTDis[14];
    uLWord MaxTDis[14];

} Bat_HisTemperDisInfo1Type;

typedef struct
{
    uLWord AvgTDis[14];
    uLWord MaxDeltaTDis[14];

} Bat_HisTemperDisInfo2Type;

typedef struct
{
    Bat_HisVoltInfoType     Bat_HisVoltInfo;
    Bat_HisTemperInfoType   Bat_HisTemperInfo;
    Bat_HisPowerInfoType    Bat_HisPowerInfo;
    Bat_HisIntResInfoType   Bat_HisIntResInfo;
    Bat_HisChgDchgInfoType  Bat_HisChgDchgInfo;
    Bat_HisTemperDisInfo1Type  Bat_HisTemperDisInfo1;
    Bat_HisTemperDisInfo2Type  Bat_HisTemperDisInfo2;
    uLWord SocDis[10];
    uLWord CurDis[14];
} sBat_HisInfoType;

typedef struct
{
    uWord SerialNum;
    uByte ParallelNum;
    uByte Manufacturer;
    uByte MaterialType;
    uWord RatedEnergy;
    uLWord RatedCapacity;
    uLWord ActualCapacity;
    uWord RatedSumVolt;

    uByte PropertyId;
    uWord AppType;
    uByte BatDate[6];
    uLWord SeriesNum;
} Bat_PackParameter1Type;

typedef struct
{
    uLWord PackNo;
} Bat_PackParameter2Type;


typedef struct
{
    uLWord PackTotalChgAh;
    uLWord PackTotalDchAh;
} Bat_PackParameter3Type;


typedef struct
{
    uWord DchEmptyTimes;
    uWord ChgFullTimes;
    uWord ChgTimes;
   
} Bat_PackParameter4Type;

typedef struct
{
    Bat_PackParameter1Type  PackParameter1;
    Bat_PackParameter2Type  PackParameter2;
    Bat_PackParameter3Type  PackParameter3;
    Bat_PackParameter4Type  PackParameter4;
} sBat_PackParameterType;

typedef struct
{
    sBat_SumvType  Bat_Sumv;
    sBat_CurrentType  Bat_Cur;
    sBat_PackParameterType  AllPackParameter;
    uWord  Soc;
    uWord  Soh;
    uWord  AvgV;
    uWord  DeltaV;
    uByte  AvgT;
    uByte  DeltaT;
    uByte  TemperRise;
    uByte  MaxTRiseRate;
    uByte  TemperRisePerHour;
    uWord  AvgIntRes;
    uWord  TotalIntRes;
    uWord  BatPower;
    uLWord AccChgAh;
    uLWord AccDchAh;
    sBat_TotalAhType  Bat_TotalAh;
    sBat_CapacityType  Bat_Capacity;
    uLWord  RemainCapacity;
    /*uLWord  RemainEnergy;
    uLWord  RemainMileage;
    uWord   RemainDriveTime; */
    uByte MainBalanceSt;
    uByte BmuNum;
    uWord TemperNum;
    uWord BatNum;
    uByte ChipNum; 
} sBat_StatisticInfoType;

typedef struct
{
    uWord DeltaV;
    uByte DeltaT;
    uWord AvgV;
    uByte AvgT;
    uByte AvgIntRes;
    uByte BalSt;
    uByte TemperNum;
    uByte BatNum;
    uLWord RemainCapacity;
    uLWord AccChgAh;
    uLWord AccDchAh;
    uWord Soh;
} sBat_PackOtherInfoType;

typedef struct
{
    sBat_CellBasicType * RAMPTR CellBasicPtr;
    sBat_CellStType * RAMPTR CellStPtr;

    sBat_PackTemperType * RAMPTR PackTemperPtr;
    sBat_SumvType  PackSumv;
    sBat_CurrentType  PackCurrent;
    Bat_SOCType  PackSoc;  /* Reserved */
    sBat_PackMaxMinType  PackMaxMin;
    sBat_PackParameterType  PackParameter;
    sBat_PackOtherInfoType  PackOtherInfo;
} sBat_PackInfoType;


typedef struct
{
    sBat_PackInfoType *RAMPTR * RAMPTR PackInfoPtrRef;
    sBat_AllPackMaxMinType * RAMPTR AllPackMaxMinPtr;
    sBat_StatisticInfoType * RAMPTR StatisticInfoPtr;
    sBat_HisInfoType * RAMPTR HisInfoPtr;
} sBat_AllInfoType;


extern void Bat_StatusInit(void);
extern  void Bat_Main(void);
extern void Bat_CountCbk(void);


#ifdef BMS_ENABLE_SINGLE_BAL_TIME
  extern uWord Bat_GetCellDchBalanceTimeSingle(uWord local_cell_no);
  extern  eBat_ReturnType Bat_UpdateCellDchBalanceTimeSingle(uWord local_cell_no, uWord bal_time_single);

  extern uWord Bat_GetCellChgBalanceTimeSingle(uWord local_cell_no);
  extern  eBat_ReturnType Bat_UpdateCellChgBalanceTimeSingle(uWord local_cell_no, uWord bal_time_single);
#endif /* #ifdef BMS_ENABLE_SINGLE_BAL_TIME */


extern uWord Bat_GetCellDchBalanceTimeTotal( uWord local_cell_no);
extern eBat_ReturnType Bat_UpdateCellDchBalanceTimeTotal(uWord local_cell_no, uWord bal_time_total);

extern uWord Bat_GetCellChgBalanceTimeTotal(uWord local_cell_no);
extern eBat_ReturnType Bat_UpdateCellChgBalanceTimeTotal(uWord local_cell_no, uWord bal_time_total);


extern uByte Bat_GetCellBalanceSt(uByte bmu_no, uWord local_cell_no);
extern eBat_ReturnType Bat_UpdateCellBalanceSt(uByte bmu_no, uWord local_cell_no, uByte bal_st);

extern uWord Bat_GetCellV(uByte bmu_no, uWord local_cell_no);
extern eBat_ReturnType Bat_UpdateCellV(uByte bmu_no, uWord local_cell_no, uWord voltage) ;

#ifdef BMS_ENABLE_CALC_INTRES
  extern uByte Bat_GetCellR(uByte bmu_no, uWord local_cell_no);
  extern eBat_ReturnType Bat_UpdateCellR(uByte bmu_no, uWord local_cell_no, uByte intres) ;
#endif

extern uByte Bat_GetTemper(uByte bmu_no, uWord local_temper_no) ;
extern eBat_ReturnType Bat_UpdateTemper(uByte bmu_no, uWord local_temper_no, uByte temper) ;


/* only bcu operation below */

extern uWord Bat_GetSumV(eBat_SumvType sumv_type) ;
extern void Bat_UpdateSumV(eBat_SumvType sumv_type, uWord sumv);

extern uWord Bat_GetCurrent(eCurrentType type);
extern void Bat_UpdateCurrent(eCurrentType type, uWord current);

extern void Bat_UpdateSOC(Bat_SOCType soc);
extern Bat_SOCType Bat_GetSOC(void);

extern uWord Bat_GetMaxV(void);
extern  void Bat_UpdateMaxV(uWord max_v);

extern uWord Bat_GetMinV(void);
extern void Bat_UpdateMinV(uWord min_v);

extern uWord Bat_GetMaxVTotalNo(void);
extern void Bat_UpdateMaxVTotalNo(uWord max_v_total_no);

extern uWord Bat_GetMinVTotalNo(void);
extern void Bat_UpdateMinVTotalNo(uWord min_v_total_no);

extern uByte Bat_GetMaxVSingleNo(void);
extern void Bat_UpdateMaxVSingleNo(uByte max_v_single_no);

extern uByte Bat_GetMinVSingleNo(void);
extern  void Bat_UpdateMinVSingleNo(uByte min_v_single_no);

extern uByte Bat_GetMaxVBmuNo(void);
extern void Bat_UpdateMaxVBmuNo(uByte max_v_bmu_no);

extern uByte Bat_GetMinVBmuNo(void);
extern void Bat_UpdateMinVBmuNo(uByte min_v_bmu_no);

extern uByte Bat_GetMaxT(void);
extern  void Bat_UpdateMaxT(uByte max_t);

extern uByte Bat_GetMinT(void);
extern void Bat_UpdateMinT(uByte min_t) ;

extern uWord Bat_GetMaxTTotalNo(void);
extern void Bat_UpdateMaxTTotalNo(uWord max_t_no);

extern uWord Bat_GetMinTTotalNo(void);
extern void Bat_UpdateMinTTotalNo(uWord min_t_total_no);

extern uByte Bat_GetMaxTSingleNo(void);
extern void Bat_UpdateMaxTSingleNo(uByte max_t_single_no);

extern uByte Bat_GetMinTSingleNo(void);
extern void Bat_UpdateMinTSingleNo(uByte min_t_single_no);

extern uByte Bat_GetMaxTBmuNo(void);
extern void Bat_UpdateMaxTBmuNo(uByte max_t_bmu_no);

extern  uByte Bat_GetMinTBmuNo(void);
extern void Bat_UpdateMinTBmuNo(uByte min_t_bmu_no);

extern uByte Bat_GetMaxR(void);
extern void Bat_UpdateMaxR(uByte max_r);

extern uByte Bat_GetMinR(void);
extern void Bat_UpdateMinR(uByte min_r);

extern uWord Bat_GetMaxRTotalNo(void);
extern  void Bat_UpdateMaxRTotalNo(uWord max_t_total_no);

extern  uWord Bat_GetMinRTotalNo(void);
extern  void Bat_UpdateMinRTotalNo(uWord min_r_total_no);

extern  uByte Bat_GetMaxRSingleNo(void);
extern void Bat_UpdateMaxRSingleNo(uByte max_r_single_no);

extern uByte Bat_GetMinRSingleNo(void);
extern void Bat_UpdateMinRSingleNo(uByte min_r_single_no);

extern uByte Bat_GetMaxRBmuNo(void);
extern void Bat_UpdateMaxRBmuNo(uByte max_r_bmu_no);

extern uByte Bat_GetMinRBmuNo(void);
extern void Bat_UpdateMinRBmuNo(uByte min_r_bmu_no);

extern uWord Bat_GetMaxPermitDchCur(void);
extern void Bat_UpdateMaxPermitDchCur(uWord max_cur);

extern uWord Bat_GetMaxFeedBackCur(void);
extern void Bat_UpdateMaxFeedBackCur(uWord max_cur);

extern uWord Bat_GetMaxPermitFeedbackPower(void);
extern void Bat_UpdateMaxPermitFeedbackPower(uWord max_pwr);

extern uWord Bat_GetMaxPermitDchPower(void);
extern  void Bat_UpdateMaxPermitDchPower(uWord max_pwr);

extern uWord Bat_GetMaxPulseDchCur(void);
extern void Bat_UpdateMaxPulseDchCur(uWord max_cur);

extern uWord Bat_GetMaxPulseFeedBackCur(void);
extern void Bat_UpdateMaxPulseFeedBackCur(uWord max_cur);

extern uWord Bat_GetMaxPulseDchPower(void);
extern void Bat_UpdateMaxPulseDchPower(uWord max_power);

extern uWord Bat_GetMaxPulseFeedbackPower(void);
extern void Bat_UpdateMaxPulseFeedbackPower(uWord max_power);

extern uWord Bat_GetHisMaxV(void);
extern void Bat_UpdateHisMaxV(uWord his_max_v);

extern uWord Bat_GetHisMinV(void);
extern void Bat_UpdateHisMinV(uWord his_min_v);

extern uWord Bat_GetHisDeltaV(void);
extern void Bat_UpdateHisDeltaV(uWord his_delta_v);

extern uWord Bat_GetHisMaxAvgV(void);
extern void Bat_UpdateHisMaxAvgV(uWord his_max_avg_v);

extern uWord Bat_GetHisMinAvgV(void);
extern void Bat_UpdateHisMinAvgV(uWord his_min_avg_v);

extern uByte Bat_GetHisMaxT(void);
extern  void Bat_UpdateHisMaxT(uByte his_max_t);

extern uByte Bat_GetHisMinT(void);
extern  void Bat_UpdateHisMinT(uByte his_min_t);

extern uByte Bat_GetHisMaxDeltaT(void);
extern void Bat_UpdateHisMaxDeltaT(uByte his_delta_t);

extern uByte Bat_GetHisMaxAvgT(void);
extern void Bat_UpdateHisMaxAvgT(uByte his_max_avg_t);

extern uByte Bat_GetHisMinAvgT(void);
extern void Bat_UpdateHisMinAvgT(uByte his_min_avg_t);

extern uWord Bat_GetHisMaxR(void);
extern void Bat_UpdateHisMaxR(uWord his_max_r);

extern uWord Bat_GetHisMinR(void);
extern void Bat_UpdateHisMinR(uWord his_min_r);

extern  uWord Bat_GetHisMaxChgCur(void);
extern  void Bat_UpdateHisMaxChgCur(uWord his_max_chg_cur);

extern  uWord Bat_GetHisMaxDchCur(void);
extern  void Bat_UpdateHisMaxDchCur(uWord his_max_dch_cur);

extern  uWord Bat_GetHisMaxChgPwr(void);
extern  void Bat_UpdateHisMaxChgPwr(uWord his_max_chg_pwr);

extern  uWord Bat_GetHisMaxDchPwr(void);
extern  void Bat_UpdateHisMaxDchPwr(uWord his_max_dch_pwr);

extern  uWord Bat_GetHisMaxSumV(void);
extern  void Bat_UpdateHisMaxSumV(uWord his_max_sum_v);

extern  uWord Bat_GetHisMinSumV(void);
extern  void Bat_UpdateHisMinSumV(uWord his_min_sum_v);

extern  uLWord Bat_GetMaxTDistribution(uByte area);
extern  void Bat_UpdateMaxTDistribution(uByte area, uLWord max_t_dis);

extern  uLWord Bat_GetMinTDistribution(uByte area);
extern  void Bat_UpdateMinTDistribution(uByte area, uLWord min_t_dis);

extern  uLWord Bat_GetMaxDeltaTDistribution(uByte area);
extern  void Bat_UpdateMaxDeltaTDistribution(uByte area, uLWord delta_t_dis);

extern uLWord Bat_GetAvgTDistribution(uByte area);
extern void Bat_UpdateAvgTDistribution(uByte area, uLWord avg_t_dis);

extern  uLWord Bat_GetCurDistribution(uByte area);
extern  void Bat_UpdateCurDistribution(uByte area, uLWord cur_dis);

extern uLWord Bat_GetSocDistribution(uByte area);
extern void Bat_UpdateSocDistribution(uByte area, uLWord soc_dis);

extern uWord Bat_GetHisChgTimes(void);
extern void Bat_UpdateHisChgTimes(uWord chg_times);

extern uWord Bat_GetHisChgFullTimes(void);
extern void Bat_UpdateHisChgFullTimes(uWord chg_full_times);

extern uWord Bat_GetDchEmptyTimes(void);
extern  void Bat_UpdateDchEmptyTimes(uWord dch_empty_times);

extern uWord Bat_GetSerialNum(void);
extern void Bat_UpdateSerialNum(uWord serial_num);

extern uByte Bat_GetParallelNum(void);
extern void Bat_UpdateParallelNum(uByte parallel_num);

extern eBat_ManufacturerType Bat_GetManufacturer(void);
extern void Bat_UpdateManufacturer(eBat_ManufacturerType manufacturer) ;

extern eBat_MaterialType Bat_GetMaterialType(void);
extern void Bat_UpdateMaterialType(eBat_MaterialType type);

extern uWord Bat_GetRatedEnergy(void);
extern void Bat_UpdateRatedEnergy(uWord energy);

extern uLWord Bat_GetRatedCapacity(void);
extern void Bat_UpdateRatedCapacity(uLWord cap);

extern uLWord Bat_GetActualCapacity(void);
extern void Bat_UpdateActualCapacity(uLWord cap);

extern uWord Bat_GetRatedSumVolt(void);
extern void Bat_UpdateRatedSumVolt(uWord sum_v);

extern uByte Bat_GetPropertyId(void);
extern void Bat_UpdatePropertyId(uByte id) ;

extern uWord Bat_GetAppType(void);
extern void Bat_UpdateAppType(uWord type) ;

extern void Bat_GetBatDate(uByte *date) ;
extern void Bat_UpdateBatDate(uByte * date);

extern uLWord Bat_GetSeriesNum(void);
extern void Bat_UpdateSeriesNum(uLWord num) ;

extern uLWord Bat_GetAllPackNo(void) ;
extern void Bat_UpdateAllPackNo(uLWord no) ;

extern uLWord Bat_GetChgTotalAh(void) ;
extern void Bat_UpdateChgTotalAh(uLWord total_ah) ;

extern uLWord Bat_GetDchgTotalAh(void) ;
extern void Bat_UpdateDchgTotalAh(uLWord total_ah);

extern uWord Bat_GetChgTimes(void);
extern void Bat_UpdateChgTimes(uWord times);

extern uWord Bat_GetChgFullTimes(void);
extern void Bat_UpdateChgFullTimes(uWord times) ;

extern uWord Bat_GetDchgEmptyTimes(void);
extern void Bat_UpdateDchgEmptyTimes(uWord times);

extern  uWord Bat_GetSoh(void);
extern  void Bat_UpdateSoh(uWord soh);

extern uWord Bat_GetAvgV(void);
extern void Bat_UpdateAvgV(uWord avg_v);

extern uWord Bat_GetDeltaV(void);
extern void Bat_UpdateDeltaV(uWord delta_v);

extern uByte Bat_GetAvgT(void);
extern  void Bat_UpdateAvgT(uByte avg_t);

extern  uByte Bat_GetDeltaT(void);
extern  void Bat_UpdateDeltaT(uByte delta_t);

extern  uByte Bat_GetBatteryTemperRise(void);
extern   void Bat_UpdateBatteryTemperRise(uByte temper_rise);

extern uByte Bat_GetTemperRisePerHour(void);
extern void Bat_UpdateTemperRisePerHour(uByte temper_rise);

extern uByte Bat_GetMaxTRiseRate(void);
extern void Bat_UpdateMaxTRiseRate(uByte max_t_rise_rate);

extern  uWord Bat_GetPwr(void);
extern  void Bat_UpdatePwr(uWord bat_pwr);

extern  uLWord Bat_GetAccumulateChgAh(void);
extern  void Bat_UpdateAccumulateChgAh(uLWord acc_chg_ah);

extern  uLWord Bat_GetAccumulateDchgAh(void);
extern  void Bat_UpdateAccumulateDchgAh(uLWord acc_dch_ah);

extern  uLWord Bat_GetTotalChgAh(void);
extern void Bat_UpdateTotalChgAh(uLWord total_chg_ah);

extern  uLWord Bat_GetTotalDchAh(void);
extern  void Bat_UpdateTotalDChAh(uLWord total_dch_ah);

extern  uLWord Bat_GetRatedCapacityCalc(void);
extern  void Bat_UpdateRatedCapacityCalc(uLWord rate_cap);

extern  uLWord Bat_GetActualCapacityCalc(void);
extern  void Bat_UpdateActualCapacityCalc(uLWord act_cap);

extern  uLWord Bat_GetRemainCapacity(void);
extern  void Bat_UpdateRemainCapacity(uLWord remain_cap);

/*extern  uWord Bat_GetRemainEnergy(void);
extern void Bat_UpdateRemainEnergy(uWord energy);

extern uWord Bat_GetRemainDriveMileage(void);
extern void Bat_UpdateRemainDriveMileage(uWord mileage);

extern  uWord Bat_GetRemainDriveTime(void);
extern  void Bat_UpdateRemainDriveTime(uWord time);*/

extern uByte Bat_GetMainBalanceSt(void);
extern  void Bat_UpdateMainBalanceSt(uByte bal_st);

extern uByte Bat_GetBmuNum(void);
extern void Bat_UpdateBmuNum(uByte bmu_num);

extern uByte Bat_GetChipNum(void);
extern void Bat_UpdateChipNum(uByte chip_num);

extern uWord Bat_GetTemperNum(void);
extern void Bat_UpdateTemperNum(uWord temper_num);

extern uWord Bat_GetBatNum(void);
extern void Bat_UpdateBatNum(uWord bat_num);

extern uWord Bat_GetTotalIntRes(void);
extern void Bat_UpdateTotalIntRes(uWord total_int_res);

extern uWord Bat_GetAvgIntRes(void);
extern void Bat_UpdateAvgIntRes(uWord avg_int_res);

/* end of only bcu operation */


/* bcu and bmu operation below */
extern uWord Bat_GetPackSumV(uByte bmu_no, eBat_SumvType sumv_v_type) ;
extern eBat_ReturnType Bat_UpdatePackSumV(uByte bmu_no, eBat_SumvType sumv_v_type, uWord sumv);

extern uWord Bat_GetPackMaxV(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMaxV(uByte bmu_no, uWord max_v);

extern uWord Bat_GetPackMinV(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMinV(uByte bmu_no, uWord min_v);

extern uWord Bat_GetPackMaxVNo(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMaxVNo(uByte bmu_no, uWord max_v_no);

extern uWord Bat_GetPackMinVNo(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackMinVNo(uByte bmu_no, uWord min_v_no);

extern uByte Bat_GetPackMaxT(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMaxT(uByte bmu_no,uByte max_t);

extern uWord Bat_GetPackMaxTNo(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMaxTNo(uByte bmu_no, uWord max_t_no);

extern uByte Bat_GetPackMinT(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMinT(uByte bmu_no,uByte min_t);

extern  uWord Bat_GetPackMinTNo(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMinTNo(uByte bmu_no, uWord min_t_no);

extern uByte Bat_GetPackMaxR(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMaxR(uByte bmu_no, uByte max_r);

extern uByte Bat_GetPackMinR(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackMinR(uByte bmu_no,uByte min_r);

extern uWord Bat_GetPackMaxRNo(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackMaxRNo(uByte bmu_no, uWord min_r_no);

extern uWord Bat_GetPackMinRNo(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackMinRNo(uByte bmu_no, uWord min_r_no);


extern uByte Bat_GetPackTemperNum(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackTemperNum(uByte bmu_no, uByte temper_num) ;

extern uByte Bat_GetPackBatNum(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackBatNum(uByte bmu_no, uByte bat_num);

extern uByte Bat_GetPackBalSt(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackBalSt(uByte bmu_no, uByte bal_st);

extern uLWord Bat_GetPackRemainCapacity(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackRemainCapacity(uByte bmu_no, uLWord cap);

extern uWord Bat_GetPackSerialNum(uByte bmu_no) ;
extern eBat_ReturnType Bat_UpdatePackSerialNum(uByte bmu_no, uWord serial_num) ;

extern  uByte Bat_GetPackParallelNum(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackParallelNum(uByte bmu_no, uByte parallel_num);

extern  eBat_ManufacturerType Bat_GetPackManufacturer(uByte bmu_no) ;
extern  eBat_ReturnType Bat_UpdatePackManufacturer(uByte bmu_no, eBat_ManufacturerType type)  ;

extern  eBat_MaterialType Bat_GetPackMaterialType(uByte bmu_no) ;
extern  eBat_ReturnType Bat_UpdatePackMaterialType(uByte bmu_no, eBat_MaterialType type);

extern  uWord Bat_GetPackRatedEnergy(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackRatedEnergy(uByte bmu_no, uWord energy) ;

extern  uLWord Bat_GetPackRatedCapacity(uByte bmu_no);
extern  eBat_ReturnType Bat_UpdatePackRatedCapacity(uByte bmu_no, uLWord capacity) ;

extern uLWord Bat_GetPackActualCapacity(uByte bmu_no) ;
extern eBat_ReturnType Bat_UpdatePackActualCapacity(uByte bmu_no, uLWord capacity) ;

extern  uWord Bat_GetPackRatedSumVolt(uByte bmu_no)  ;
extern  eBat_ReturnType Bat_UpdatePackRatedSumVolt(uByte bmu_no, uWord volt);

extern uWord Bat_GetPackCurrent(uByte bmu_no, eCurrentType type);
extern eBat_ReturnType Bat_UpdatePackCurrent(uByte bmu_no, eCurrentType type, uWord current);

extern  eBat_ReturnType Bat_GetPackBatDate(uByte bmu_no, uByte * date);
extern  eBat_ReturnType Bat_UpdatePackBatDate(uByte bmu_no, uByte * date);

extern uLWord Bat_GetPackNo(uByte bmu_no) ;
extern eBat_ReturnType Bat_UpdatePackNo(uByte bmu_no, uLWord no) ;


extern uByte Bat_GetPackProperty(uByte bmu_no) ;
extern  eBat_ReturnType Bat_UpdatePackProperty(uByte bmu_no, uByte property_id);

extern uWord Bat_GetPackAppType(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAppType(uByte bmu_no, uWord type) ;

extern uLWord Bat_GetPackSeriesNum(uByte bmu_no) ;
extern eBat_ReturnType Bat_UpdatePackSeriesNum(uByte bmu_no,uLWord series_num) ;

extern uWord Bat_GetPackDeltaV(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackDeltaV(uByte bmu_no, uWord delta_v) ;

extern uByte Bat_GetPackDeltaT(uByte bmu_no) ;
extern eBat_ReturnType Bat_UpdatePackDeltaT(uByte bmu_no, uByte delta_t);

extern uWord Bat_GetPackAvgV(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAvgV(uByte bmu_no, uWord avg_v);

extern uByte Bat_GetPackAvgT(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAvgT(uByte bmu_no, uByte avg_t);

extern uByte Bat_GetPackAvgIntRes(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAvgIntRes(uByte bmu_no, uByte avg_intres);

extern uLWord Bat_GetPackTotalChgAh(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackTotalChgAh(uByte bmu_no,uLWord total_chg_ah);

extern uLWord Bat_GetPackTotalDchAh(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackTotalDchAh(uByte bmu_no,uLWord total_dch_ah);

extern uWord Bat_GetPackSoc(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackSoc(uByte bmu_no, uWord soc);

extern uWord Bat_GetPackSoh(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackSoh(uByte bmu_no, uWord soh);

extern uLWord Bat_GetPackAccumulateChgAh(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAccumulateChgAh(uByte bmu_no, uLWord acc_dch_ah);

extern uLWord Bat_GetPackAccumulateDchAh(uByte bmu_no);
extern eBat_ReturnType Bat_UpdatePackAccumulateDchAh(uByte bmu_no, uLWord acc_dch_ah);

/* end of bcu and bmu operation */


#endif  /* BATTERY_H */