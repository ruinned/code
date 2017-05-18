// SumvDet.c
#include "SumvDet.h"
#include "EE.h"
#include "ErrorAlarm.h"
#include "SchIf.h"
#include "BatterySt.h"
#include "ErrorAlarm.h"

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
typedef struct
{
    uWord gain;
    uLWord offset;
} sSumvDet_ConfigType;

sSumvDet_ConfigType RAMPTR SumvDet_Config[SUMVDET_NUM];
#pragma DATA_SEG DEFAULT


typedef struct
{
    uByte cnt;
} sSumvDet_TmCntType;

sSumvDet_TmCntType SumvDet_TmCnt;

uWord gwd_SumvAdValue[SUMVDET_NUM];
uWord gwd_SumvAdValueBak[SUMVDET_NUM];
uWord gwd_SumvAdValUnCngCnt[SUMVDET_NUM];

uByte gby_SumvDetStep;

static uByte gby_Sumv_ErrCnt[SUMVDET_NUM];
static uWord gwd_Sumv_Backup[SUMVDET_NUM];

static void SumvDet_InitSumvDet(void);
static void SumvDet_ProcSumvDet(void);

void SumvDet_Init(void)
{
    SumvDet_TmCnt.cnt = 0;
    SumvDet_InitSumvDet();
}

void SumvDet_MainCountCbk(void)
{
    (SumvDet_TmCnt.cnt < 255) ? (SumvDet_TmCnt.cnt++) : (SumvDet_TmCnt.cnt = 255);
}

void SumvDet_MainCbk(void)
{
    if(SumvDet_TmCnt.cnt >= TM_MS(10))
    {
        SumvDet_TmCnt.cnt = 0;
        SumvDet_ProcSumvDet();
    }
}


static void SumvDet_InitSumvDet(void)
{
    uByte i;
    uWord para_no;
    GeneralReturnType rslt[SUMVDET_NUM];
    Err_HwErrDetailType hderr_detail[SUMVDET_NUM];
    gby_SumvDetStep = 0;

    for(i = 0; i < SUMVDET_NUM; i++)
    {
        gby_Sumv_ErrCnt[i] = 0;
        hderr_detail[i] = 0;
        rslt[i] = 0;

        if(i < 2)
        {
            para_no = (uWord)SUMVDET_SUMVIN_CALIB_PARA + i;
        }
        else if(i < 4)
        {
            para_no = (uWord)SUMVDET_SUMVINBAK_CALIB_PARA + (i % 2);
        }
        else
        {
            para_no = (uWord)SUMVDET_SUMVINBAK1_CALIB_PARA + (i % 2);
        }
        rslt[i] = SysConf_InitParaTable((eSysConf_ParameterNOType)para_no, (uLWord)(&SumvDet_Config[i]), sizeof(SumvDet_Config[i]));

        rslt[i] |= EE_LoadVar((eSysConf_ParameterNOType)para_no);
        if((uWord)SUMVIF_ERR_OK != rslt[i])
        {
            rslt[i] = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
            rslt[i] = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
            if((uWord)SUMV_INNER_ID == (i % 2))
            {
                SumvDet_Config[i].gain = INNERSUMV_DEFAULT_GAIN;
                SumvDet_Config[i].offset = INNERSUMV_DEFAULT_OFFSET;
                hderr_detail[i] |= SUMVDET_HDERR_NOCALIB;
            }
            else
            {
                SumvDet_Config[i].gain = EXTERN_INNERBAK_SUMV_DEFAULT_GAIN;
                SumvDet_Config[i].offset = EXTERN_INNERBAK_SUMV_DEFAULT_OFFSET;
                hderr_detail[i] |= SUMVDET_HDERR_NOCALIB;
            }
        }

        if(i < 2)
        {   
            rslt[i] = SumvIf_Init(SUMV_INNER_ID + i);
            if((uWord)SUMVIF_ERR_OK != rslt[i])
            {
                hderr_detail[i] |= SUMVDET_HDERR_INIT;
            }

            rslt[i] = SumvIf_StartConvert(SUMV_INNER_ID + i);
            if((uWord)SUMVIF_ERR_OK != rslt[i])
            {
                hderr_detail[i] |= SUMVDET_HDERR_COM;
            }
        }

        if(0 != hderr_detail[i])
        {
            // hardware error
            if((uWord)SUMV_INNER_ID == (i % 2))
            {
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_IN, ERR_LEVEL_TWO);
                rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_IN, hderr_detail[i]);
            }
            else 
            {
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_OUT, ERR_LEVEL_TWO);
                rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_OUT, hderr_detail[i]);
            }
        }
    }
}

static void SumvDet_ProcSumvDet(void)
{
    uByte i;
    uLWord sumv_real_value[SUMVDET_NUM];
    GeneralReturnType rslt[SUMVDET_NUM];
    Err_HwErrDetailType hderr_detail_orin[3];
    Err_HwErrDetailType hderr_detail[3];
    uByte data[2];
    uLWord llwtmp;

    hderr_detail_orin[0] = Err_GetHwErrDetail(ERR_HW_DETECT_SUMV_IN);
    hderr_detail_orin[1] = Err_GetHwErrDetail(ERR_HW_DETECT_SUMV_OUT);
    hderr_detail_orin[2] = 0; // need add

    i = gby_SumvDetStep;
    gby_SumvDetStep++;
    if(gby_SumvDetStep >= SUMVDET_NUM)
    {
        gby_SumvDetStep = 0;
    }

    hderr_detail[i] = 0;

    rslt[i] = SumvIf_RdAd((eSumv_IdNoType)i, &gwd_SumvAdValue[i]);

    if(((0 == i % 2) || (1 == i % 2))&& (i < 4))
    {
        (void)SumvIf_StartConvert(i + 2);
    }
    else if(4 == i)
    {
        (void)SumvIf_StartConvert(SUMV_INNER_ID);
    }
    else 
    {
        (void)SumvIf_StartConvert(SUMV_EXTER_ID);
    }
    
         
    if(gwd_SumvAdValue[i] != gwd_SumvAdValueBak[i])
    {
        gwd_SumvAdValueBak[i] = gwd_SumvAdValue[i];
        gwd_SumvAdValUnCngCnt[i]  = 0;    
    }   
    else
    {
        gwd_SumvAdValUnCngCnt[i]++;
        if(gwd_SumvAdValUnCngCnt[i] >= 10) // 100mS
        {
            gwd_SumvAdValUnCngCnt[i] = 0;          
        }
 
    }
    
    if((uWord)SUMVIF_ERR_OK != rslt[i])
    {
        if((uWord)SUMVIF_ERR_OUTRANGE == rslt[i])
        {
            hderr_detail[i] |= SUMVDET_HDERR_OUTRANG;
        }
        else if((uWord)SUMVIF_ERR_ADCONVERT == rslt[i])
        {
            hderr_detail[i] |= SUMVDET_HDERR_ADCONVERT;
        }
        else if((uWord)SUMVIF_ERR_NEG == rslt[i])
        {
//          hderr_detail[i] |= SUMVDET_HDERR_NEGVOLT;
        }
        else if((uWord)SUMVIF_ERROR_PTR_NULL == rslt[i])
        {
            hderr_detail[i] |= SUMVDET_HDERR_PTR_NULL;
        }
        else
        {
            hderr_detail[i] |= SUMVDET_HDERR_IDNO_OUTRANGE;
        }

        sumv_real_value[i] = 0;
    }
    else
    {
        sumv_real_value[i] = gwd_SumvAdValue[i];
        sumv_real_value[i] *= SumvDet_Config[i].gain;
        
        if(SumvDet_Config[i].offset & 0x80000000) // is minus?
        {
            llwtmp = 0xFFFFFFFF - SumvDet_Config[i].offset + 1; 
            if(sumv_real_value[i] >= llwtmp)
            { 
                sumv_real_value[i] -= llwtmp;
            }
        }
        else
        {
            sumv_real_value[i] += SumvDet_Config[i].offset;  
        }

        /* if(sumv_real_value[i] & 0x80000000)
         {
             sumv_real_value[i] = 0;
             hderr_detail[i] |= SUMVDET_HDERR_NEGVOLT;
         }
         else
         {  */
        sumv_real_value[i] /= SUMVOLT_GAIN;
        // }

        if(sumv_real_value[i] > SUMV_1000V)
        {
            hderr_detail[i] |= SUMVDET_HDERR_OUTRANG;
        }
    }

    if(0 != hderr_detail[i])
    {
        sumv_real_value[i] = gwd_Sumv_Backup[i];

        gby_Sumv_ErrCnt[i]++;
        if(gby_Sumv_ErrCnt[i] > SUMV_ERR_CNT)
        {
            gby_Sumv_ErrCnt[i] = SUMV_ERR_CNT;

            // update hardware error level when reach error count
            if((uWord)SUMV_INNER_ID == (i % 2))
            {
                // update hardware error level, except no calibation
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_IN, ERR_LEVEL_TWO);
            }
            else if((uWord)SUMV_EXTER_ID == (i % 2))
            {
                // update hardware error level, except no calibation
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_OUT, ERR_LEVEL_TWO);
            }
        }

        // re-init sumv module
        rslt[i] = SumvIf_Init((eSumv_IdNoType)i);
        if((uWord)SUMVIF_ERR_OK != rslt[i])
        {
            hderr_detail[i] |= SUMVDET_HDERR_INIT;
        }

        hderr_detail[i] |= hderr_detail_orin[i];

        // update hardware error detail when error appear
        if((uWord)SUMV_INNER_ID == (i % 2))
        {
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_IN, hderr_detail[i]);
        }
        else if((uWord)SUMV_EXTER_ID == (i % 2))
        {
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_OUT, hderr_detail[i]);
        }
    }
    else
    {
        gwd_Sumv_Backup[i] = (uWord)sumv_real_value[i];

        gby_Sumv_ErrCnt[i] = 0;

        // clear hardware error
        if(0 == (hderr_detail_orin[i] & SUMVDET_HDERR_NOCALIB))
        {
            if((uWord)SUMV_INNER_ID == (i % 2))
            {
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_IN, ERR_LEVEL_NORMAL);
            }
            else if((uWord)SUMV_EXTER_ID == (i % 2))
            {
                rslt[i] = Err_UpdateHwErrLevel(ERR_HW_DETECT_SUMV_OUT, ERR_LEVEL_NORMAL);
            }
        }

        // clear hardware error detail
        hderr_detail[i] |= hderr_detail_orin[i] & SUMVDET_HDERR_NOCALIB;

        // update hardware error detail
        if((uWord)SUMV_INNER_ID == (i % 2))
        {
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_IN, hderr_detail[i]);
        }
        else if((uWord)SUMV_EXTER_ID == (i % 2))
        {
            rslt[i] = Err_UpdateHwErrDetail(ERR_HW_DETECT_SUMV_OUT, hderr_detail[i]);
        }
    }

    Bat_UpdateSumV((eBat_SumvType)(i + 1), (uWord)sumv_real_value[i]);

}

/*
 *  interface for Calibration
 */
GeneralReturnType SumvDet_GetSumvAd(eSumv_IdNoType Sumv_IdNo, uWord * Sumv_Ad)
{
    GeneralReturnType rslt;

    if((uWord)Sumv_IdNo < SUMVDET_NUM)
    {
        *Sumv_Ad = gwd_SumvAdValue[Sumv_IdNo];
        rslt = ERR_OK;
    }
    else
    {
        *Sumv_Ad = 0;
        rslt = ERR_SUMVDET_GET_SUMVAD;
    }

    return rslt;
}

GeneralReturnType SumvDet_UpdateGainAndOffset(eSumv_IdNoType Sumv_IdNo,uWord Gain, uLWord Offset)
{
    GeneralReturnType rslt;
    eSumv_IdNoType Sumv_IdNo1;
    uWord para_no;
    
    Sumv_IdNo1 = Sumv_IdNo;
    Sumv_IdNo %= 2;
    
    switch(Sumv_IdNo)
    {
    case SUMV_INNER_ID:
    {
        if(0 == Sumv_IdNo1)
        {
            para_no = (uWord)SUMVDET_SUMVIN_CALIB_PARA;
        }
        else if(2 == Sumv_IdNo1)
        {
            para_no = (uWord)SUMVDET_SUMVINBAK_CALIB_PARA;
        }
        else
        {
            para_no = (uWord)SUMVDET_SUMVINBAK1_CALIB_PARA;
        }
        SumvDet_Config[Sumv_IdNo1].gain = Gain;
        SumvDet_Config[Sumv_IdNo1].offset = Offset;
        rslt = EE_SaveVarForCalib(BOTHEE,para_no);
        break;
    }
    case SUMV_EXTER_ID:
    {
        if(1 == Sumv_IdNo1)
        {
            para_no = (uWord)SUMVDET_SUMVOUT_CALIB_PARA;
        }
        else if(3 == Sumv_IdNo1)
        {
            para_no = (uWord)SUMVDET_SUMVOUTBAK_CALIB_PARA;
        }
        else
        {
            para_no = (uWord)SUMVDET_SUMVOUTBAK1_CALIB_PARA;
        }
        SumvDet_Config[Sumv_IdNo1].gain = Gain;
        SumvDet_Config[Sumv_IdNo1].offset = Offset;
        rslt = EE_SaveVarForCalib(BOTHEE, para_no);
        break;
    }
    default:
    {
        rslt = ERR_SUMVDET_PARA_UPDATE;
        break;
    }
    }

    return rslt;
}

void SumvDet_UpdateRunStep(eSumv_IdNoType Sumv_IdNo) 
{
    gby_SumvDetStep = (uByte)Sumv_IdNo;
}

