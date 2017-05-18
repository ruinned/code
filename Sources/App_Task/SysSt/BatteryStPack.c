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

extern sBat_AllInfoType  Bat_AllInfo;

uWord Bat_GetPackSumV(uByte bmu_no, eBat_SumvType sumv_v_type)
{
    uWord sumvolt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( (bmu_no > 0) && (bmu_no < MAX_BMU_NUM + 1) );

    if(0 == bmu_no)
    {

    }

    if(bmu_no > 0)
    {
#if (MAX_CELL_NUM - MAX_PACK_CELL_NUM)

        if(ACC_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                sumvolt = (PackInfoPtr->PackSumv.AccSumV);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else if(INT_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                sumvolt = (PackInfoPtr->PackSumv.IntSumV);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else if(OUT_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                sumvolt = (PackInfoPtr->PackSumv.OutSumV);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else if(INT_SUMV_BAK == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                sumvolt = (PackInfoPtr->PackSumv.IntSumvBak);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else
        {
            sumvolt = 0; // 0V
        }
#endif
    }
    return sumvolt;
}

eBat_ReturnType Bat_UpdatePackSumV(uByte bmu_no, eBat_SumvType sumv_v_type, uWord sumv)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT(bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1);

    if(0 == bmu_no)
    {

    }

    if(bmu_no > 0)
    {
#if (MAX_CELL_NUM - MAX_PACK_CELL_NUM)

        if(ACC_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackSumv.AccSumV) = sumv;
            }
            else
            {
                return BATST_ERR_UPDATE;
            }
        }
        else if(INT_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackSumv.IntSumV) = sumv;
            }
            else
            {
                return BATST_ERR_UPDATE;
            }
        }
        else if(OUT_SUMV == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackSumv.OutSumV) = sumv;
            }
            else
            {
                return BATST_ERR_UPDATE;
            }
        }
        else if(INT_SUMV_BAK == sumv_v_type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackSumv.IntSumvBak) = sumv;
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else
        {
            return BATST_ERR_UPDATE;
        }
#endif
    }

    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackCurrent(uByte bmu_no, eCurrentType type)
{
    uWord current = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( (bmu_no > 0) && (bmu_no < MAX_BMU_NUM + 1) );

#if (MAX_CELL_NUM - MAX_PACK_CELL_NUM)
    if(bmu_no > 0)
    {
        if(INSCUR == type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                current = (PackInfoPtr->PackCurrent.InsCur);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else if(AVGCUR == type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                current = (PackInfoPtr->PackCurrent.AvgCur);
            }
            else
            {
                __ASSERT(PackInfoPtr != NULL);
            }
        }
        else
        {
            current = 0; // 0A
        }
    }
#endif
    return current;
}

eBat_ReturnType Bat_UpdatePackCurrent(uByte bmu_no, eCurrentType type, uWord current)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT(bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1);
    
#if (MAX_CELL_NUM - MAX_PACK_CELL_NUM)
    if(bmu_no > 0)
    {
        if(INSCUR == type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackCurrent.InsCur) = current;
            }
            else
            {
                return BATST_ERR_UPDATE;
            }
        }
        else if(AVGCUR == type)
        {
            PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
            if(PackInfoPtr != NULL)
            {
                (PackInfoPtr->PackCurrent.AvgCur) = current;
            }
            else
            {
                return BATST_ERR_UPDATE;
            }
        }
        else
        {
            return BATST_ERR_UPDATE;
        }
    }
#endif

    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackDeltaV(uByte bmu_no)
{
    uWord delta_volt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        delta_volt = PackInfoPtr->PackOtherInfo.DeltaV;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return delta_volt;
}

eBat_ReturnType Bat_UpdatePackDeltaV(uByte bmu_no, uWord delta_v)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.DeltaV = delta_v;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackDeltaT(uByte bmu_no)
{
    uByte dt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        dt = PackInfoPtr->PackOtherInfo.DeltaT;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return dt;
}

eBat_ReturnType Bat_UpdatePackDeltaT(uByte bmu_no, uByte delta_t)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.DeltaT = delta_t;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;

}

uWord Bat_GetPackAvgV(uByte bmu_no)
{
    uWord avg_volt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        avg_volt = PackInfoPtr->PackOtherInfo.AvgV;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }


    return avg_volt;
}

eBat_ReturnType Bat_UpdatePackAvgV(uByte bmu_no, uWord avg_v)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );


    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.AvgV = avg_v;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}


uByte Bat_GetPackAvgT(uByte bmu_no)
{
    uByte avg_t = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        avg_t = PackInfoPtr->PackOtherInfo.AvgT;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return avg_t;
}

eBat_ReturnType Bat_UpdatePackAvgT(uByte bmu_no, uByte avg_t)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.AvgT = avg_t;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackAvgIntRes(uByte bmu_no)
{
    uByte avg_intres = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        avg_intres = PackInfoPtr->PackOtherInfo.AvgIntRes;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return avg_intres;
}

eBat_ReturnType Bat_UpdatePackAvgIntRes(uByte bmu_no, uByte avg_intres)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.AvgIntRes = avg_intres;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

/*
** Single Pack Maximum and Minimum Information
*/
uWord Bat_GetPackMaxV(uByte bmu_no)
{
    uWord maxvolt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        maxvolt = PackInfoPtr->PackMaxMin.MaxV;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return maxvolt;
}

eBat_ReturnType Bat_UpdatePackMaxV(uByte bmu_no, uWord max_v)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxV =  max_v;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return  (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMinV(uByte bmu_no)
{
    uWord minvolt = 0;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        minvolt = PackInfoPtr->PackMaxMin.MinV;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return minvolt;
}

eBat_ReturnType Bat_UpdatePackMinV(uByte bmu_no, uWord min_v)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];

    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinV = min_v;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return  (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMaxVNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MaxVNo;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdatePackMaxVNo(uByte bmu_no, uWord max_v_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxVNo = max_v_no;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return   (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMinVNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MinVNo;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMinVNo(uByte bmu_no, uWord min_v_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinVNo = min_v_no ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackMaxT(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MaxT ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdatePackMaxT(uByte bmu_no,uByte max_t)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxT = max_t ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMaxTNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MaxTNo  ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdatePackMaxTNo(uByte bmu_no, uWord max_t_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxTNo = max_t_no ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackMinT(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MinT ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMinT(uByte bmu_no,uByte min_t)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinT = min_t;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMinTNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MinTNo ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMinTNo(uByte bmu_no, uWord min_t_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinTNo = min_t_no ;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackMaxR(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MaxR;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMaxR(uByte bmu_no, uByte max_r)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxR  = max_r;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackMinR(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MinR;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMinR(uByte bmu_no,uByte min_r)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinR  = min_r;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMaxRNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MaxRNo;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMaxRNo(uByte bmu_no, uWord max_r_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MaxRNo = max_r_no;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackMinRNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackMaxMin.MinRNo;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdatePackMinRNo(uByte bmu_no, uWord min_r_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackMaxMin.MinRNo = min_r_no;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

/*
**  End of single Pack Maximum and Minimum Information Operation
*/


/*
** Battery Pack Information
*/

uByte Bat_GetPackTemperNum(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.TemperNum;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackTemperNum(uByte bmu_no, uByte temper_num)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.TemperNum = temper_num;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}



uByte Bat_GetPackBatNum(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.BatNum;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackBatNum(uByte bmu_no, uByte bat_num)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;


    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.BatNum = bat_num;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}



uByte Bat_GetPackBalSt(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.BalSt;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackBalSt(uByte bmu_no, uByte bal_st)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.BalSt = bal_st;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackRemainCapacity(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.RemainCapacity;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackRemainCapacity(uByte bmu_no, uLWord cap)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.RemainCapacity = cap;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}


uWord Bat_GetPackSerialNum(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );


    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.SerialNum;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackSerialNum(uByte bmu_no,uWord serial_num)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.SerialNum = serial_num;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackParallelNum(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.ParallelNum;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackParallelNum(uByte bmu_no, uByte parallel_num)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.ParallelNum = parallel_num;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

eBat_ManufacturerType Bat_GetPackManufacturer(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return (eBat_ManufacturerType)PackInfoPtr->PackParameter.PackParameter1.Manufacturer;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return (eBat_ManufacturerType)0xff;
    }
}

eBat_ReturnType Bat_UpdatePackManufacturer(uByte bmu_no, eBat_ManufacturerType type)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.Manufacturer = (uByte)type;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

eBat_MaterialType Bat_GetPackMaterialType(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return (eBat_MaterialType)(PackInfoPtr->PackParameter.PackParameter1.MaterialType);
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return (eBat_MaterialType)0xff;
    }
}

eBat_ReturnType Bat_UpdatePackMaterialType(uByte bmu_no, eBat_MaterialType type)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.MaterialType = (uByte)type;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackRatedEnergy(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.RatedEnergy;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackRatedEnergy(uByte bmu_no, uWord energy)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.RatedEnergy = energy;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackRatedCapacity(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.RatedCapacity;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackRatedCapacity(uByte bmu_no, uLWord capacity)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.RatedCapacity = capacity;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackActualCapacity(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );


    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.ActualCapacity;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackActualCapacity(uByte bmu_no, uLWord capacity)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.ActualCapacity = capacity;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackRatedSumVolt(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.RatedSumVolt;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackRatedSumVolt(uByte bmu_no, uWord volt)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.RatedSumVolt = volt;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

eBat_ReturnType Bat_GetPackBatDate(uByte bmu_no, uByte * date)
{
    uByte i;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1);

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        for(i = 0; i < 6; i++)
        {
            *date++ = PackInfoPtr->PackParameter.PackParameter1.BatDate[i];
        }
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return ERR_OK;
}

eBat_ReturnType Bat_UpdatePackBatDate(uByte bmu_no, uByte * date)
{
    uByte i;
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        for(i = 0; i < 6; i++)
        {
            PackInfoPtr->PackParameter.PackParameter1.BatDate[i] = *(date++);
        }
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackNo(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter2.PackNo;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackNo(uByte bmu_no, uLWord no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter2.PackNo = no;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uByte Bat_GetPackProperty(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.PropertyId;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackProperty(uByte bmu_no, uByte property_id)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.PropertyId = property_id;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackAppType(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.AppType;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackAppType(uByte bmu_no, uWord type)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.AppType = type;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackSeriesNum(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter1.SeriesNum;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackSeriesNum(uByte bmu_no,uLWord series_num)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter1.SeriesNum = series_num;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackTotalChgAh(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter3.PackTotalChgAh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackTotalChgAh(uByte bmu_no,uLWord total_chg_ah)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1);

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter3.PackTotalChgAh = total_chg_ah;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackTotalDchAh(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackParameter.PackParameter3.PackTotalDchAh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }

}

eBat_ReturnType Bat_UpdatePackTotalDchAh(uByte bmu_no,uLWord total_dch_ah)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1);

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackParameter.PackParameter3.PackTotalDchAh = total_dch_ah;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackSoc(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackSoc;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackSoc(uByte bmu_no, uWord soc)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackSoc = soc;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uWord Bat_GetPackSoh(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.Soh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackSoh(uByte bmu_no, uWord soh)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.Soh = soh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackAccumulateChgAh(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.AccChgAh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackAccumulateChgAh(uByte bmu_no, uLWord acc_chg_ah)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );


    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.AccChgAh = acc_chg_ah;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }

    return (eBat_ReturnType)ERR_OK;
}

uLWord Bat_GetPackAccumulateDchAh(uByte bmu_no)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        return PackInfoPtr->PackOtherInfo.AccDchAh;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
        return 0xff;
    }
}

eBat_ReturnType Bat_UpdatePackAccumulateDchAh(uByte bmu_no, uLWord acc_dch_ah)
{
    sBat_PackInfoType * RAMPTR  PackInfoPtr;

    __ASSERT( bmu_no > 0 && bmu_no < MAX_BMU_NUM + 1 );

    PackInfoPtr = Bat_AllInfo.PackInfoPtrRef[bmu_no];
    if(PackInfoPtr != NULL)
    {
        PackInfoPtr->PackOtherInfo.AccDchAh = acc_dch_ah;
    }
    else
    {
        __ASSERT(PackInfoPtr != NULL);
    }
    return (eBat_ReturnType)ERR_OK;
}

/*
** End of Battery Pack Information Operation
*/

#pragma DATA_SEG DEFAULT

