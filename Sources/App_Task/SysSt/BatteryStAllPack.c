// BatterySt.c
#include "BatterySt.h"


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

/* define for extern information, which come from bmu */
  #if (MAX_CELL_NUM - MAX_PACK_CELL_NUM) /* if have cell information come from other ecu ,
  * need statistic extern information
  */
    sBat_CellBasicType  Bat_ExternCellBasicBuf[MAX_BMU_NUM][MAX_PACK_CELL_NUM];
    sBat_CellStType  Bat_ExternCellStBuf[MAX_BMU_NUM][MAX_PACK_CELL_NUM];
    sBat_PackTemperType  Bat_ExternPackTemperBuf[MAX_BMU_NUM][MAX_PACK_TEMPER_NUM];

    sBat_PackInfoType  Bat_ExternPackInfo[MAX_BMU_NUM];
  #endif /* #if (MAX_CELL_NUM - MAX_PACK_CELL_NUM) */

  sBat_PackInfoType * RAMPTR Bat_AllPackInfo[] =
  {

      NULL,
    #if (MAX_BMU_NUM > 0)
        &(Bat_ExternPackInfo[0]),
    #endif
    #if (MAX_BMU_NUM > 1)
        &(Bat_ExternPackInfo[1]),
    #endif

    #if (MAX_BMU_NUM > 2)
        &(Bat_ExternPackInfo[2]),
    #endif

    #if (MAX_BMU_NUM > 3)
        &(Bat_ExternPackInfo[3]),
    #endif
    #if (MAX_BMU_NUM > 4)
        &(Bat_ExternPackInfo[4]),
    #endif
    #if (MAX_BMU_NUM > 5)
        &(Bat_ExternPackInfo[5]),
    #endif
    #if (MAX_BMU_NUM > 6)
        &(Bat_ExternPackInfo[6]),
    #endif
    #if (MAX_BMU_NUM > 7)
        &(Bat_ExternPackInfo[7]),
    #endif
    #if (MAX_BMU_NUM > 8)
        &(Bat_ExternPackInfo[8]),
    #endif
    #if (MAX_BMU_NUM > 9)
        &(Bat_ExternPackInfo[9]),
    #endif
    #if (MAX_BMU_NUM > 10)
        &(Bat_ExternPackInfo[10]),
    #endif
    #if (MAX_BMU_NUM > 11)
        &(Bat_ExternPackInfo[11]),
    #endif
    #if (MAX_BMU_NUM > 12)
        &(Bat_ExternPackInfo[12]),
    #endif
    #if (MAX_BMU_NUM > 13)
        &(Bat_ExternPackInfo[13]),
    #endif
    #if (MAX_BMU_NUM > 14)
        &(Bat_ExternPackInfo[14]),
    #endif
    #if (MAX_BMU_NUM > 15)
        &(Bat_ExternPackInfo[15]),
    #endif
    #if (MAX_BMU_NUM > 16)
        &(Bat_ExternPackInfo[16]),
    #endif
    #if (MAX_BMU_NUM > 17)
        &(Bat_ExternPackInfo[17]),
    #endif
    #if (MAX_BMU_NUM > 18)
        &(Bat_ExternPackInfo[18]),
    #endif
    #if (MAX_BMU_NUM > 19)
        &(Bat_ExternPackInfo[19]),
    #endif
    #if (MAX_BMU_NUM > 20)
        &(Bat_ExternPackInfo[20]),
    #endif
    #if (MAX_BMU_NUM > 21)
        &(Bat_ExternPackInfo[21]),
    #endif
    #if (MAX_BMU_NUM > 22)
        &(Bat_ExternPackInfo[22]),
    #endif
    #if (MAX_BMU_NUM > 23)
        &(Bat_ExternPackInfo[23]),
    #endif
    #if (MAX_BMU_NUM > 24)
        &(Bat_ExternPackInfo[24]),
    #endif
    #if (MAX_BMU_NUM > 25)
        &(Bat_ExternPackInfo[25]),
    #endif
    #if (MAX_BMU_NUM > 26)
        &(Bat_ExternPackInfo[26]),
    #endif
    #if (MAX_BMU_NUM > 27)
        &(Bat_ExternPackInfo[27]),
    #endif
    #if (MAX_BMU_NUM > 28)
        &(Bat_ExternPackInfo[28]),
    #endif
    #if (MAX_BMU_NUM > 29)
        &(Bat_ExternPackInfo[29]),
    #endif

  };

sBat_AllPackMaxMinType  Bat_AllPackMaxMin;
sBat_StatisticInfoType  Bat_StatisticInfo;
sBat_HisInfoType  Bat_HisInfo;

sBat_AllInfoType  Bat_AllInfo =
{
    Bat_AllPackInfo,
    &Bat_AllPackMaxMin,
    &Bat_StatisticInfo,
    &Bat_HisInfo
};


sCell_BalanceTimeType Bat_LocalPackBalTime;

extern void Bat_DataProcessInit(void);

static void BatPackInfo_Init(sBat_PackInfoType * RAMPTR PackInfoPtr, sBat_CellBasicType * RAMPTR CellBasicPtr, sBat_CellStType * RAMPTR CellStPtr, sBat_PackTemperType * RAMPTR PackTemperPtr)
{
    uWord wcnt;
    uByte * RAMPTR pbyte;

    /* initial cell and temperature pointer */
    PackInfoPtr->CellBasicPtr = CellBasicPtr;
    PackInfoPtr->CellStPtr = CellStPtr;
    PackInfoPtr->PackTemperPtr = PackTemperPtr;

    /* initial cell information */
    //PackInfoPtr = &Bat_LocalPackInfo;
    for(wcnt = 0; wcnt < MAX_PACK_CELL_NUM; wcnt++)
    {
        PackInfoPtr->CellBasicPtr[wcnt].Voltage = 0;
#ifdef BMS_ENABLE_CALC_INTRES
        PackInfoPtr->CellBasicPtr[wcnt].Int_Res = 0;
#endif
#ifdef BMS_ENABLE_CALC_CELL_SOC
        PackInfoPtr->CellBasicPtr[wcnt].Soc = 0;
#endif
        PackInfoPtr->CellStPtr[wcnt].Cell_Info.Info = 0;
    }

    /* initial pack temperature information */
    for(wcnt = 0; wcnt < MAX_PACK_TEMPER_NUM; wcnt++)
    {
        PackInfoPtr->PackTemperPtr[wcnt].Temper = 0;
    }

    PackInfoPtr->PackSoc = 0;
    PackInfoPtr->PackCurrent.InsCur = 0;
    PackInfoPtr->PackCurrent.AvgCur = 0;

    PackInfoPtr->PackSumv.AccSumV = 0;
    PackInfoPtr->PackSumv.IntSumV = 0;

    PackInfoPtr->PackSumv.IntSumvBak = 0;
    PackInfoPtr->PackSumv.OutSumV = 0;

    pbyte = (uByte *RAMPTR)&(PackInfoPtr->PackMaxMin);
    for(wcnt = 0; wcnt < sizeof(PackInfoPtr->PackMaxMin); wcnt++)
    {
        *(pbyte++) = 0;
    }

    pbyte = (uByte *RAMPTR)&(PackInfoPtr->PackParameter);
    for(wcnt = 0; wcnt < sizeof(PackInfoPtr->PackParameter); wcnt++)
    {
        *(pbyte++) = 0;
    }

    pbyte = (uByte *RAMPTR)&(PackInfoPtr->PackOtherInfo);
    for(wcnt = 0; wcnt < sizeof(PackInfoPtr->PackOtherInfo); wcnt++)
    {
        *(pbyte++) = 0;
    }
}

void Bat_StatusInit(void)
{
    uWord wcnt;

    uByte * RAMPTR pbyte;
    // volatile sBat_PackInfoType * RAMPTR PackInfoPtr;
    sBat_PackInfoType * RAMPTR PackInfoPtr;

#if (MAX_CELL_NUM - MAX_PACK_CELL_NUM)
    /* initial extern Pack information */
    PackInfoPtr = Bat_ExternPackInfo;
    for(wcnt = 0; wcnt < MAX_BMU_NUM; wcnt++)
    {
        BatPackInfo_Init((PackInfoPtr + wcnt), &(Bat_ExternCellBasicBuf[wcnt][0]), &(Bat_ExternCellStBuf[wcnt][0]),&(Bat_ExternPackTemperBuf[wcnt][0]));
    }
#endif


    /* initial system information */
    pbyte = (uByte * RAMPTR)(Bat_AllInfo.AllPackMaxMinPtr);
    for(wcnt = 0; wcnt < sizeof(*Bat_AllInfo.AllPackMaxMinPtr); wcnt++)
    {
        *(pbyte++) = 0;
    }

    /* initial Statistic information */
    pbyte = (uByte * RAMPTR)(Bat_AllInfo.StatisticInfoPtr);
    for(wcnt = 0; wcnt < sizeof(*Bat_AllInfo.StatisticInfoPtr); wcnt++)
    {
        *(pbyte++) = 0;
    }

    /* initial history information */
    pbyte = (uByte * RAMPTR)(Bat_AllInfo.HisInfoPtr);
    for(wcnt = 0; wcnt < sizeof(*Bat_AllInfo.HisInfoPtr); wcnt++)
    {
        *(pbyte++) = 0;
    }

    pbyte = (uByte * RAMPTR)(&Bat_LocalPackBalTime);
    for(wcnt = 0; wcnt < sizeof(Bat_LocalPackBalTime); wcnt++)
    {
        *(pbyte++) = 0;
    }

    Bat_DataProcessInit();
}

uWord Bat_GetCurrent(eCurrentType type)
{
    if(INSCUR == type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Cur.InsCur;
    }
    else if(AVGCUR == type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Cur.AvgCur;   /*1 Min */
    }
    else
    {
        return (uWord)BATST_ERR_GET;
    }
}

void Bat_UpdateCurrent(eCurrentType type, uWord cur)
{
    if(INSCUR == type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Cur.InsCur = cur;
    }
    else if(AVGCUR == type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Cur.AvgCur = cur;   /*1 Min */
    }
    else
    {

    }
}


uWord Bat_GetSumV(eBat_SumvType sumv_type)
{
    if(ACC_SUMV == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.AccSumV ;
    }
    else if(INT_SUMV == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumV ;
    }
    else if(OUT_SUMV == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumV ;
    }
    else if(INT_SUMV_BAK == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumvBak;
    }
    else if(OUT_SUMV_BAK == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumvBak;
    }
    else if(INT_SUMV_BAK1 == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumvBak1;
    }
    else if(OUT_SUMV_BAK1 == sumv_type)
    {
        return Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumvBak1;
    }
    else
    {
        return (uWord)BATST_ERR_GET;
    }
}

void Bat_UpdateSumV(eBat_SumvType sumv_type, uWord sum_v)
{
    if(ACC_SUMV == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.AccSumV  = sum_v;
    }
    else if(INT_SUMV == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumV = sum_v ;
    }
    else if(OUT_SUMV == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumV = sum_v;
    }
    else if(INT_SUMV_BAK == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumvBak = sum_v;
    }
    else if(OUT_SUMV_BAK == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumvBak = sum_v;
    }
    else if(INT_SUMV_BAK1 == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.IntSumvBak1 = sum_v;
    }
    else if(OUT_SUMV_BAK1 == sumv_type)
    {
        Bat_AllInfo.StatisticInfoPtr->Bat_Sumv.OutSumvBak1 = sum_v;
    }
    else
    {
    }
}

Bat_SOCType Bat_GetSOC(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Soc;
}

void Bat_UpdateSOC(Bat_SOCType soc)
{
    Bat_AllInfo.StatisticInfoPtr->Soc = soc;
}


uByte Bat_GetTemper(uByte bmu_no, uWord local_temper_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );



    __ASSERT(local_temper_no < MAX_PACK_TEMPER_NUM + 1);

    __ASSERT(local_temper_no > 0);
    if(local_temper_no > 0)
    {
        local_temper_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];

    if(PackInfoPtr != NULL)
    {
        return ((PackInfoPtr->PackTemperPtr) + local_temper_no)->Temper;
    }
    else
    {
        return ERR_FAIL_GET;   //0xff
    }
}

eBat_ReturnType Bat_UpdateTemper(uByte bmu_no, uWord local_temper_no, uByte temper)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( (bmu_no > 0) && (bmu_no < MAX_BMU_NUM + 1 ));

    __ASSERT(local_temper_no < MAX_PACK_TEMPER_NUM + 1);

    __ASSERT(local_temper_no > 0);

    if(local_temper_no > 0)
    {
        local_temper_no -= 1;
    }

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];

    if(PackInfoPtr != NULL)
    {
        ((PackInfoPtr->PackTemperPtr) + local_temper_no)->Temper = temper;
    }
    else
    {
        return BATST_ERR_UPDATE;      //0x4300
    }

    return (eBat_ReturnType)ERR_OK;           //0x4301
}


/*
**  All Pack Maximum and Minimum Information Operation
*/

uWord Bat_GetMaxV(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxV;
}

void Bat_UpdateMaxV(uWord max_v)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxV = max_v;
}


uWord Bat_GetMinV(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinV;
}

void Bat_UpdateMinV(uWord min_v)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinV = min_v;
}

uWord Bat_GetMaxVTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxVTotalNo;
}

void Bat_UpdateMaxVTotalNo(uWord max_v_total_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxVTotalNo = max_v_total_no;
}

uWord Bat_GetMinVTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinVTotalNo;
}

void Bat_UpdateMinVTotalNo(uWord min_v_total_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinVTotalNo = min_v_total_no;
}

uByte Bat_GetMaxVSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxVSingleNo;  //
}

void Bat_UpdateMaxVSingleNo(uByte max_v_single_no)       //
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxVSingleNo = max_v_single_no;
}

uByte Bat_GetMinVSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinVSingleNo;
}

void Bat_UpdateMinVSingleNo(uByte min_v_single_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinVSingleNo = min_v_single_no;
}

uByte Bat_GetMaxVBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxVBmuNo;
}

void Bat_UpdateMaxVBmuNo(uByte max_v_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxVBmuNo = max_v_bmu_no;
}

uByte Bat_GetMinVBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinVBmuNo;
}

void Bat_UpdateMinVBmuNo(uByte min_v_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinVBmuNo = min_v_bmu_no;
}

uByte Bat_GetMaxT(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxT;
}

void Bat_UpdateMaxT(uByte max_t)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxT = max_t;
}

uByte Bat_GetMinT(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinT;
}

void Bat_UpdateMinT(uByte min_t)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinT = min_t;
}

uWord Bat_GetMaxTTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxTTotalNo;
}

void Bat_UpdateMaxTTotalNo(uWord max_t_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxTTotalNo = max_t_no;
}

uWord Bat_GetMinTTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinTTotalNo;
}

void Bat_UpdateMinTTotalNo(uWord min_t_total_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinTTotalNo = min_t_total_no;
}

uByte Bat_GetMaxTSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxTSingleNo;
}

void Bat_UpdateMaxTSingleNo(uByte max_t_single_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxTSingleNo = max_t_single_no;
}

uByte Bat_GetMinTSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinTSingleNo;
}

void Bat_UpdateMinTSingleNo(uByte min_t_single_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinTSingleNo = min_t_single_no;
}

uByte Bat_GetMaxTBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxTBmuNo;
}

void Bat_UpdateMaxTBmuNo(uByte max_t_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxTBmuNo = max_t_bmu_no;
}

uByte Bat_GetMinTBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinTBmuNo;
}

void Bat_UpdateMinTBmuNo(uByte min_t_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinTBmuNo = min_t_bmu_no;
}

uByte Bat_GetMaxR(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxR;
}

void Bat_UpdateMaxR(uByte max_r)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxR = max_r;
}

uByte Bat_GetMinR(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinR;
}

void Bat_UpdateMinR(uByte min_r)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinR = min_r;
}

uWord Bat_GetMaxRTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxRTotalNo;
}

void Bat_UpdateMaxRTotalNo(uWord max_r_total_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxRTotalNo = max_r_total_no;
}

uWord Bat_GetMinRTotalNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinRTotalNo;
}

void Bat_UpdateMinRTotalNo(uWord min_r_total_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinRTotalNo = min_r_total_no;
}

uByte Bat_GetMaxRSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MaxRSingleNo;
}

void Bat_UpdateMaxRSingleNo(uByte max_r_single_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxRSingleNo = max_r_single_no;
}

uByte Bat_GetMinRSingleNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinRSingleNo;
}

void Bat_UpdateMinRSingleNo(uByte min_r_single_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinRSingleNo = min_r_single_no;
}

uByte Bat_GetMaxRBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinRBmuNo;
}

void Bat_UpdateMaxRBmuNo(uByte max_r_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxRBmuNo = max_r_bmu_no;
}

uByte Bat_GetMinRBmuNo(void)
{
    return Bat_AllInfo.AllPackMaxMinPtr->MinRBmuNo;
}

void Bat_UpdateMinRBmuNo(uByte min_r_bmu_no)
{
    Bat_AllInfo.AllPackMaxMinPtr->MinRBmuNo = min_r_bmu_no;
}

uWord Bat_GetMaxPermitDchCur(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPermitDchCur;
}

void Bat_UpdateMaxPermitDchCur(uWord max_cur)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPermitDchCur = max_cur;
}

uWord Bat_GetMaxFeedBackCur(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxFeedBackCur;
}

void Bat_UpdateMaxFeedBackCur(uWord max_cur)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxFeedBackCur = max_cur;
}

uWord Bat_GetMaxPermitFeedbackPower(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPermitFeedBackPower;
}

void Bat_UpdateMaxPermitFeedbackPower(uWord max_pwr)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPermitFeedBackPower = max_pwr;
}

uWord Bat_GetMaxPermitDchPower(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPermitDchPower;
}

void Bat_UpdateMaxPermitDchPower(uWord max_pwr)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPermitDchPower = max_pwr;
}

uWord Bat_GetMaxPulseDchCur(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPulseDchCur;
}

void Bat_UpdateMaxPulseDchCur(uWord max_cur)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPulseDchCur = max_cur;
}

uWord Bat_GetMaxPulseFeedBackCur(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPulseFeedBackCur;
}

void Bat_UpdateMaxPulseFeedBackCur(uWord max_cur)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPulseFeedBackCur = max_cur;
}

uWord Bat_GetMaxPulseDchPower(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPulseDchPower;
}

void Bat_UpdateMaxPulseDchPower(uWord max_power)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPulseDchPower = max_power;
}

uWord Bat_GetMaxPulseFeedbackPower(void)
{
    return  Bat_AllInfo.AllPackMaxMinPtr->MaxPulseFeedBackPower;
}

void Bat_UpdateMaxPulseFeedbackPower(uWord max_power)
{
    Bat_AllInfo.AllPackMaxMinPtr->MaxPulseFeedBackPower = max_power;
}

/*
** End of All Pack Maximum and Minimum Information Operation
*/

/*
 ** history Info  Operation
*/
uWord Bat_GetHisMaxV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxV;
}

void Bat_UpdateHisMaxV(uWord his_max_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxV = his_max_v;
}

uWord Bat_GetHisMinV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinV;
}

void Bat_UpdateHisMinV(uWord his_min_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinV = his_min_v;
}

uWord Bat_GetHisDeltaV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxDeltaV;
}

void Bat_UpdateHisDeltaV(uWord his_delta_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxDeltaV = his_delta_v;
}

uWord Bat_GetHisMaxAvgV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxAvgV;
}

void Bat_UpdateHisMaxAvgV(uWord his_max_avg_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxAvgV = his_max_avg_v;
}

uWord Bat_GetHisMinAvgV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinAvgV;
}

void Bat_UpdateHisMinAvgV(uWord his_min_avg_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinAvgV = his_min_avg_v;
}

uByte Bat_GetHisMaxT(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxT;
}

void Bat_UpdateHisMaxT(uByte his_max_t)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxT = his_max_t;
}

uByte Bat_GetHisMinT(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMinT;
}

void Bat_UpdateHisMinT(uByte his_min_t)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMinT = his_min_t;
}

uByte Bat_GetHisMaxDeltaT(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxDeltaT;
}

void Bat_UpdateHisMaxDeltaT(uByte his_delta_t)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxDeltaT = his_delta_t;
}

uByte Bat_GetHisMaxAvgT(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxAvgT;
}

void Bat_UpdateHisMaxAvgT(uByte his_max_avg_t)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMaxAvgT = his_max_avg_t;
}

uByte Bat_GetHisMinAvgT(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMinAvgT;
}

void Bat_UpdateHisMinAvgT(uByte his_min_avg_t)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperInfo.HisMinAvgT = his_min_avg_t;
}

uWord Bat_GetHisMaxR(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo.HisMaxR;
}

void Bat_UpdateHisMaxR(uWord his_max_r)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo.HisMaxR = his_max_r;
}

uWord Bat_GetHisMinR(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo.HisMinR;
}

void Bat_UpdateHisMinR(uWord his_min_r)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisIntResInfo.HisMinR = his_min_r;
}

uWord Bat_GetHisMaxChgCur(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgCur;
}

void Bat_UpdateHisMaxChgCur(uWord his_max_chg_cur)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgCur = his_max_chg_cur;
}

uWord Bat_GetHisMaxDchCur(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxDchCur;
}

void Bat_UpdateHisMaxDchCur(uWord his_max_dch_cur)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxDchCur = his_max_dch_cur;
}

uWord Bat_GetHisMaxChgPwr(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgPower;
}

void Bat_UpdateHisMaxChgPwr(uWord his_max_chg_pwr)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxChgPower = his_max_chg_pwr;
}

uWord Bat_GetHisMaxDchPwr(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxDchPower;
}

void Bat_UpdateHisMaxDchPwr(uWord his_max_dch_pwr)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisPowerInfo.HisMaxDchPower = his_max_dch_pwr;
}

uWord Bat_GetHisMaxSumV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxSumV;
}

void Bat_UpdateHisMaxSumV(uWord his_max_sum_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMaxSumV = his_max_sum_v;
}

uWord Bat_GetHisMinSumV(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinSumV;
}

void Bat_UpdateHisMinSumV(uWord his_min_sum_v)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisVoltInfo.HisMinSumV = his_min_sum_v;
}

uLWord Bat_GetMaxTDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MaxTDis[area];
}

void Bat_UpdateMaxTDistribution(uByte area, uLWord max_t_dis)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MaxTDis[area] = max_t_dis;
}

uLWord Bat_GetMinTDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MinTDis[area];
}

void Bat_UpdateMinTDistribution(uByte area, uLWord min_t_dis)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo1.MinTDis[area] = min_t_dis;
}

uLWord Bat_GetMaxDeltaTDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.MaxDeltaTDis[area];
}

void Bat_UpdateMaxDeltaTDistribution(uByte area, uLWord delta_t_dis)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.MaxDeltaTDis[area] = delta_t_dis;
}

uLWord Bat_GetAvgTDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.AvgTDis[area];
}

void Bat_UpdateAvgTDistribution(uByte area, uLWord avg_t_dis)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisTemperDisInfo2.AvgTDis[area] = avg_t_dis;
}

uLWord Bat_GetCurDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->CurDis[area];
}

void Bat_UpdateCurDistribution(uByte area, uLWord cur_dis)
{
    Bat_AllInfo.HisInfoPtr->CurDis[area] = cur_dis;
}

uLWord Bat_GetSocDistribution(uByte area)
{
    return Bat_AllInfo.HisInfoPtr->SocDis[area];
}

void Bat_UpdateSocDistribution(uByte area, uLWord soc_dis)
{
    Bat_AllInfo.HisInfoPtr->SocDis[area] = soc_dis;
}

uWord Bat_GetHisChgTimes(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.ChgTimes;
}

void Bat_UpdateHisChgTimes(uWord chg_times)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.ChgTimes = chg_times;
}

uWord Bat_GetHisChgFullTimes(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.ChgFullTimes;
}

void Bat_UpdateHisChgFullTimes(uWord chg_full_times)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.ChgFullTimes = chg_full_times;
}

uWord Bat_GetDchEmptyTimes(void)
{
    return Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.DchEmptyTimes;
}

void Bat_UpdateDchEmptyTimes(uWord dch_empty_times)
{
    Bat_AllInfo.HisInfoPtr->Bat_HisChgDchgInfo.DchEmptyTimes = dch_empty_times;
}

/*
* * end of history info operation
*/

/*
** statistic Information  Operation
*/
uWord Bat_GetSerialNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.SerialNum;
}

void Bat_UpdateSerialNum(uWord serial_num)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.SerialNum = serial_num;
}

uByte Bat_GetParallelNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.ParallelNum;
}

void Bat_UpdateParallelNum(uByte parallel_num)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.ParallelNum = parallel_num;
}

eBat_ManufacturerType Bat_GetManufacturer(void)
{
    return (eBat_ManufacturerType)Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.Manufacturer;
}

void Bat_UpdateManufacturer(eBat_ManufacturerType manufacturer)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.Manufacturer = (uByte)manufacturer;
}

eBat_MaterialType Bat_GetMaterialType(void)
{
    return (eBat_MaterialType)Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.MaterialType;
}

void Bat_UpdateMaterialType(eBat_MaterialType type)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.MaterialType = (uByte)type;
}

uWord Bat_GetRatedEnergy(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedEnergy;
}

void Bat_UpdateRatedEnergy(uWord energy)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedEnergy = energy;
}

uLWord Bat_GetRatedCapacity(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedCapacity;
}

void Bat_UpdateRatedCapacity(uLWord cap)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedCapacity = cap;
}

uLWord Bat_GetActualCapacity(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.ActualCapacity;
}

void Bat_UpdateActualCapacity(uLWord cap)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.ActualCapacity = cap;
}

uWord Bat_GetRatedSumVolt(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedSumVolt;
}

void Bat_UpdateRatedSumVolt(uWord sum_v)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.RatedSumVolt = sum_v;
}

uByte Bat_GetPropertyId(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.PropertyId;
}

void Bat_UpdatePropertyId(uByte id)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.PropertyId = id;
}

uWord Bat_GetAppType(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.AppType;
}

void Bat_UpdateAppType(uWord type)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.AppType = type;
}

void Bat_GetBatDate(uByte * date)
{
   uByte i;
    for(i = 0; i < sizeof(Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.BatDate); i++)
    {
       *(date++) = Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.BatDate[i];
    }
}

void Bat_UpdateBatDate(uByte * date)
{
    uByte i;
    for(i = 0; i < 6; i++)
    {
        Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.BatDate[i] = *(date++);
    }
}

uLWord Bat_GetSeriesNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.SeriesNum;
}

void Bat_UpdateSeriesNum(uLWord num)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter1.SeriesNum = num;
}

uLWord Bat_GetAllPackNo(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter2.PackNo;
}

void Bat_UpdateAllPackNo(uLWord no)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter2.PackNo = no;
}

uLWord Bat_GetChgTotalAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3.PackTotalChgAh;
}

void Bat_UpdateChgTotalAh(uLWord total_ah)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3.PackTotalChgAh = total_ah;
}

uLWord Bat_GetDchgTotalAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3.PackTotalDchAh;
}

void Bat_UpdateDchgTotalAh(uLWord total_ah)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter3.PackTotalDchAh = total_ah;
}

uWord Bat_GetChgTimes(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.ChgTimes;
}

void Bat_UpdateChgTimes(uWord times)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.ChgTimes = times;
}

uWord Bat_GetChgFullTimes(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.ChgFullTimes;
}

void Bat_UpdateChgFullTimes(uWord times)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.ChgFullTimes = times;
}

uWord Bat_GetDchgEmptyTimes(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.DchEmptyTimes;
}

void Bat_UpdateDchgEmptyTimes(uWord times)
{
    Bat_AllInfo.StatisticInfoPtr->AllPackParameter.PackParameter4.DchEmptyTimes = times;
}

uWord Bat_GetSoh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Soh;
}

void Bat_UpdateSoh(uWord soh)
{
    Bat_AllInfo.StatisticInfoPtr->Soh = soh;
}

uWord Bat_GetAvgV(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AvgV;
}

void Bat_UpdateAvgV(uWord avg_v)
{
    Bat_AllInfo.StatisticInfoPtr->AvgV = avg_v;
}

uWord Bat_GetDeltaV(void)
{
    return Bat_AllInfo.StatisticInfoPtr->DeltaV;
}

void Bat_UpdateDeltaV(uWord delta_v)
{
    Bat_AllInfo.StatisticInfoPtr->DeltaV = delta_v;
}

uByte Bat_GetAvgT(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AvgT;
}

void Bat_UpdateAvgT(uByte avg_t)
{
    Bat_AllInfo.StatisticInfoPtr->AvgT = avg_t;
}

uByte Bat_GetDeltaT(void)
{
    return Bat_AllInfo.StatisticInfoPtr->DeltaT;
}

void Bat_UpdateDeltaT(uByte delta_t)
{
    Bat_AllInfo.StatisticInfoPtr->DeltaT = delta_t;
}

uByte Bat_GetBatteryTemperRise(void)
{
    return Bat_AllInfo.StatisticInfoPtr->TemperRise;
}

void Bat_UpdateBatteryTemperRise(uByte temper_rise)
{
    Bat_AllInfo.StatisticInfoPtr->TemperRise = temper_rise;
}

uByte Bat_GetTemperRisePerHour(void)
{
    return Bat_AllInfo.StatisticInfoPtr->TemperRisePerHour;
}

void Bat_UpdateTemperRisePerHour(uByte temper_rise)
{
    Bat_AllInfo.StatisticInfoPtr->TemperRisePerHour = temper_rise;
}


uByte Bat_GetMaxTRiseRate(void)
{
    return Bat_AllInfo.StatisticInfoPtr->MaxTRiseRate;
}

void Bat_UpdateMaxTRiseRate(uByte max_t_rise_rate)
{
    Bat_AllInfo.StatisticInfoPtr->MaxTRiseRate = max_t_rise_rate;
}

uWord Bat_GetPwr(void)
{
    return Bat_AllInfo.StatisticInfoPtr->BatPower;
}

void Bat_UpdatePwr(uWord bat_pwr)
{
    Bat_AllInfo.StatisticInfoPtr->BatPower = bat_pwr;
}

uLWord Bat_GetAccumulateChgAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AccChgAh;
}

void Bat_UpdateAccumulateChgAh(uLWord acc_chg_ah)
{
    Bat_AllInfo.StatisticInfoPtr->AccChgAh = acc_chg_ah;
}

uLWord Bat_GetAccumulateDchgAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AccDchAh;
}

void Bat_UpdateAccumulateDchgAh(uLWord acc_dch_ah)
{
    Bat_AllInfo.StatisticInfoPtr->AccDchAh = acc_dch_ah;
}

uLWord Bat_GetTotalChgAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh.TotalChgAh;
}

void Bat_UpdateTotalChgAh(uLWord total_chg_ah)
{
    Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh.TotalChgAh = total_chg_ah;
}

uLWord Bat_GetTotalDchAh(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh.TotalDchAh;
}

void Bat_UpdateTotalDChAh(uLWord total_dch_ah)
{
    Bat_AllInfo.StatisticInfoPtr->Bat_TotalAh.TotalDchAh = total_dch_ah;
}

uWord Bat_GetAvgIntRes(void)
{
    return Bat_AllInfo.StatisticInfoPtr->AvgIntRes;
}

void Bat_UpdateAvgIntRes(uWord avg_int_res)
{
    Bat_AllInfo.StatisticInfoPtr->AvgIntRes = avg_int_res;
}

uWord Bat_GetTotalIntRes(void)
{
    return Bat_AllInfo.StatisticInfoPtr->TotalIntRes;
}

void Bat_UpdateTotalIntRes(uWord total_int_res)
{
    Bat_AllInfo.StatisticInfoPtr->TotalIntRes = total_int_res;
}

uLWord Bat_GetRatedCapacityCalc(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Bat_Capacity.RatedCapacity;
}

void Bat_UpdateRatedCapacityCalc(uLWord rate_cap)
{
    Bat_AllInfo.StatisticInfoPtr->Bat_Capacity.RatedCapacity = rate_cap;
}

uLWord Bat_GetActualCapacityCalc(void)
{
    return Bat_AllInfo.StatisticInfoPtr->Bat_Capacity.ActualCapacity;
}

void Bat_UpdateActualCapacityCalc(uLWord act_cap)
{
    Bat_AllInfo.StatisticInfoPtr->Bat_Capacity.ActualCapacity = act_cap;
}

uLWord Bat_GetRemainCapacity(void)
{
    return Bat_AllInfo.StatisticInfoPtr->RemainCapacity;
}

void Bat_UpdateRemainCapacity(uLWord remain_cap)
{
    Bat_AllInfo.StatisticInfoPtr->RemainCapacity = remain_cap;
}

/*uWord Bat_GetRemainEnergy(void)
{
}

void Bat_UpdateRemainEnergy(uWord energy)
{
}


uWord Bat_GetRemainDriveMileage(void)
{
}

void Bat_UpdateRemainDriveMileage(uWord mileage)
{
}

uWord Bat_GetRemainDriveTime(void)
{
}

void Bat_UpdateRemainDriveTime(uWord time)
{
} */

uByte Bat_GetMainBalanceSt(void)
{
    return Bat_AllInfo.StatisticInfoPtr->MainBalanceSt;
}

void Bat_UpdateMainBalanceSt(uByte bal_st)
{
    Bat_AllInfo.StatisticInfoPtr->MainBalanceSt = bal_st;
}

uByte Bat_GetBmuNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->BmuNum;
}

void Bat_UpdateBmuNum(uByte bmu_num)
{
    Bat_AllInfo.StatisticInfoPtr->BmuNum = bmu_num;
}


uByte Bat_GetChipNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->ChipNum;
}

void Bat_UpdateChipNum(uByte chip_num)
{
    Bat_AllInfo.StatisticInfoPtr->ChipNum = chip_num;
}


uWord Bat_GetTemperNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->TemperNum;
}

void Bat_UpdateTemperNum(uWord temper_num)
{
    Bat_AllInfo.StatisticInfoPtr->TemperNum = temper_num;
}

uWord Bat_GetBatNum(void)
{
    return Bat_AllInfo.StatisticInfoPtr->BatNum;
}

void Bat_UpdateBatNum(uWord bat_num)
{
    Bat_AllInfo.StatisticInfoPtr->BatNum = bat_num;
}

/*
** End of Statistic Information Operation
*/


#pragma DATA_SEG DEFAULT
/*
** End Of Data Process
*/
