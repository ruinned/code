// SOF.c
#include "SOF-B.h"
#include "ErrorAlarm.h"
#include "BatterySt.h"
#include "SchIf.h"

#ifdef USE_SOF_B


#define SOF_MAX_DCHCUR_BAK 55

#define SOF_SELFAD_100MS   5  // 20ms * 5 = 100ms

#define SOF_MIN_DCHCUR     50


#define SOF_ERR_LEVEL_ZERO 0  // normal
#define SOF_ERR_LEVEL_ONE  1  // warning
#define SOF_ERR_LEVEL_TWO  2  // error

#define SOF_MIN_CUR 30050  

#define SOF_FLAG_OK        1
#define SOF_FLAG_FAIL      0   
    
#define ARRAY_ELEMENT_NUM(array)  (sizeof(array)/sizeof(array[0]))   

#define SOF_CONST_DCHCUR       0
#define SOF_CONST_FEEDBACKCUR  1
#define SOF_PULSE_DCHCUR       2
#define SOF_PULSE_FEEDBACKCUR  3

#define SOF_CONST_DCHPOW       4
#define SOF_CONST_FEEDBACKPOW  5
#define SOF_PULSE_DCHPOW       6
#define SOF_PULSE_FEEDBACKPOW  7

typedef struct
{
    uByte cnt;
    uByte selfad_cnt;
} sSOF_TmCntType;

typedef struct 
{
    const sByte  *TemperPtr; // point to t array
    const uWord  *SocPtr; // point to soc array           
    const uWord  *TablePtr; // ponit to table
}sSOF_PowerCurveType;

typedef struct 
{
    uWord Feedback;
    uWord Discharge;
}sSOF_PowerType;

typedef struct
{ 
    sSOF_PowerType SOF_PulseCur;
    sSOF_PowerType SOF_CnstCur; 
    sSOF_PowerType SOF_PulsePow;
    sSOF_PowerType SOF_CnstPow;    
}sSOF_PowerLmtType;

typedef struct
{
    uWord MaxDchCur;
    sSOF_PowerLmtType   PowLmt;
    
    /*   err */
    uByte CellvLowErr;  // error of cellv low
    uByte CellvHighErr; // error of cellv high
    uByte SumvHighErr;  //error of sumv high
    uByte SumvLowErr;   //error of sumv low
    uByte TempHighErr;  //error of temperature high
    uByte TempLowErr;   //error of temperature low
    uByte DetVolErr;	  //error of det_v_all
    uByte DetTemErr;	  //error of det t all
    uByte ChrCurErr;		//error of chr cur
    uByte DchCurErr;	  //error of dch cur
    uByte CanComErr;		//error of can commun
    
    /* bat infomation*/
    uLWord BatCap;      // 0.001AH unit
    uByte MaxT;  
    uByte MinT;  
    uByte AvgT;
    uWord Current;
    uWord MinV;
    uWord MaxV;
    uWord SOC; 
}sSOF_CalcValueType;


sSOF_TmCntType SOF_TmCnt;
sSOF_CalcValueType SOF_CalcValue;

uWord gwd_DchCurrentBak[SOF_MAX_DCHCUR_BAK];
uByte gby_LowVoltCnt;
uByte gby_LowPowerCtrlFlag;
uByte gby_PulseDisCurCnt;
uByte gby_PulseFeedCurCnt;
uByte gby_FirstCalCurFlag;
uByte gby_FirstCalPowFlag;

uByte gby_SofModifyRatio[8];
uWord gwd_SofTargetValue[8];

sSOF_PowerCurveType SOF_CnstDisCurCurves =
{
    DchTemperArray,
    SocArray,
    &CnstDisCurArray[0][0]
};

sSOF_PowerCurveType SOF_CnstFeedbackCurCurves =
{
    FbTemperArray,
    SocArray,
    &CnstFeedbackCurArray[0][0]
};

sSOF_PowerCurveType SOF_PulseDisCurCurves =
{
    DchTemperArray,
    SocArray,
    &PulseDisCurArray[0][0]
};

sSOF_PowerCurveType SOF_PulseFeedbackCurCurves =
{
    FbTemperArray,
    SocArray,
    &PulseFeedbackCurArray[0][0]
};



sSOF_PowerCurveType SOF_CnstDisPowCurves =
{
    DchTemperArray,
    SocArray,
    &CnstDisPowArray[0][0]
};

sSOF_PowerCurveType SOF_CnstFeedbackPowCurves =
{
    FbTemperArray,
    SocArray,
    &CnstFeedbackPowArray[0][0]
};

sSOF_PowerCurveType SOF_PulseDisPowCurves =
{
    DchTemperArray,
    SocArray,
    &PulseDisPowArray[0][0]
};

sSOF_PowerCurveType SOF_PulseFeedbackPowCurves =
{
    FbTemperArray,
    SocArray,
    &PulseFeedbackPowArray[0][0]
};

static void SOF_UpdateBatSt(void)
{
    SOF_CalcValue.MaxT  = Bat_GetMaxT();
    SOF_CalcValue.MinT = Bat_GetMinT();
    SOF_CalcValue.AvgT = Bat_GetAvgT();
    SOF_CalcValue.MaxV = Bat_GetMaxV();
    SOF_CalcValue.MinV = Bat_GetMinV();
    SOF_CalcValue.Current = Bat_GetCurrent(INSCUR);
    SOF_CalcValue.SOC =  Bat_GetSOC();
    SOF_CalcValue.BatCap = Bat_GetRatedCapacityCalc();
    
    SOF_CalcValue.CellvLowErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_CELLV_LOW));
    SOF_CalcValue.CellvHighErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_CELLV_HIGH));
    SOF_CalcValue.SumvLowErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_SUMV_LOW));
    SOF_CalcValue.SumvHighErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_SUMV_HIGH));
    SOF_CalcValue.TempLowErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_T_LOW));
    SOF_CalcValue.TempHighErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_T_HIGH));
    SOF_CalcValue.DetVolErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_DELTV_ALLPACK));
    SOF_CalcValue.DetTemErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_DELTT_ALLPACK));
    SOF_CalcValue.ChrCurErr= (uByte)(Err_GetBatErrLevel(ERR_BAT_CUR_HIGH_FDBCK));
    SOF_CalcValue.DchCurErr = (uByte)(Err_GetBatErrLevel(ERR_BAT_CUR_HIGH_DCH));
    SOF_CalcValue.CanComErr = (uByte)(Err_GetHwErrLevel(ERR_HW_CAN4));

#ifdef __DEBUG_TEST_SOF
    SOF_CalcValue.MinT= 35;
    SOF_CalcValue.MaxT= 35;
    SOF_CalcValue.SOC = 900;
#endif

}

static void SOF_SelfAdaptionCalcMaxDchCur(void)
{
    uByte i;
    uWord max_dch_current;
    uWord current;
    uWord minv;
    
    minv = Bat_GetMinV();
    current = Bat_GetCurrent(INSCUR);

    if(current > SOF_MIN_CUR)
    {
        for(i = SOF_MAX_DCHCUR_BAK-1; i > 0 ; i--)
        {
            gwd_DchCurrentBak[i] = gwd_DchCurrentBak[i-1];
        }
        gwd_DchCurrentBak[0] = current;
    }
    else
    {
        for(i = 0; i < SOF_MAX_DCHCUR_BAK; i++)
        {
            gwd_DchCurrentBak[i] = 0;
        }
    }

    if( (minv > 0) && (minv < SOF_ConfigPara.Sof_Minv_Self_Calc_Cur) && (current > SOF_MIN_CUR) )
    {
        gby_LowPowerCtrlFlag = 1;
    }

    if(1 == gby_LowPowerCtrlFlag)
    {
        if(gby_LowVoltCnt < SOF_SELFAD_100MS)
        {
            gby_LowVoltCnt++;
        }
        else
        {
            gby_LowVoltCnt = SOF_SELFAD_100MS;	
        }
        
        max_dch_current = 0;
        if(gby_LowVoltCnt >= SOF_SELFAD_100MS)
        {
            gby_LowVoltCnt = 0;
            gby_LowPowerCtrlFlag = 0;

            for(i = 0; i < SOF_MAX_DCHCUR_BAK; i++)
            {
                if(max_dch_current < gwd_DchCurrentBak[i])
                {
                    max_dch_current = gwd_DchCurrentBak[i];
                }
                if(gwd_DchCurrentBak[i] <= SOF_MIN_CUR)
                {
                    max_dch_current = 0;
                    break;
                }
            }
        }
        if(max_dch_current > SOF_MIN_CUR)
        {
            max_dch_current -= OFFSET_CUR;

            if( (0 == SOF_CalcValue.MaxDchCur) || (max_dch_current < SOF_CalcValue.MaxDchCur) )
            {
                SOF_CalcValue.MaxDchCur = max_dch_current;
            }
        }
    }
    else
    {
       gby_LowVoltCnt = 0;
    }
}

/*
 * Input: t --- control by temperature  offset is -40
 *        soc --- used 0.1% / bit
 *        curve_ptr --- point to curve
 *        soc_len --- number of soc,or soc arrary length
 *        t_len --- number of temperature, ro temperature array length
 * Return cur --- unit is 0.1
 * NOTICE: curve value is using signed int
 */
static uWord SOF_GetCurveCur(uWord soc, uByte t, sSOF_PowerCurveType * curve_ptr, uByte soc_len, uByte t_len)
{
    uByte i, j, lby_soc_len, lby_t_len;
    sByte lby_t, lby_tmp_t;
    uWord lwd_soc;
    sLWord sllwtmp[2], sllwtmp_calc[2];
    uLWord llwd_k, llwd_cur;
    uWord wtmp;

#define SOC_MAX 1000 // 100%     
#define CUR_GAIN 10000;

    // prevent temperature over max value
    llwd_cur = 0; // initialate current
    lby_tmp_t = t;
    lby_tmp_t -= 40;
    // prevent soc error value
    (soc > SOC_MAX) ? (lwd_soc = SOC_MAX) : (lwd_soc = (uWord)soc);

    if(lby_tmp_t > *(curve_ptr->TemperPtr + SOF_Macro.Sof_Max_T_Point - 1))
    {
        // if t over max value of table return direct
        return (uWord)llwd_cur;
    }
    else if(lby_tmp_t < *(curve_ptr->TemperPtr) )
    {
        // if t over min value of table return direct
        return (uWord)llwd_cur;
    }
    else
    {
        lby_t = (sByte)t;
        lby_t -= 40;
    }

    // prevent length overflow max length
    (soc_len > SOF_Macro.Sof_Max_Soc_Point) ? (lby_soc_len = SOF_Macro.Sof_Max_Soc_Point) : (lby_soc_len = soc_len);
    (t_len > SOF_Macro.Sof_Max_T_Point) ? (lby_t_len = SOF_Macro.Sof_Max_T_Point) : (lby_t_len = t_len);

    // calculate max cur from curve
    for(i = 0; i < lby_t_len; i++)
    {
        if(lby_t == *(curve_ptr->TemperPtr + i) )
        {
            // on the table point
            for(j = 0; j < lby_soc_len; j++)
            {
                if(lwd_soc == *(curve_ptr->SocPtr + j))
                {
                    // on the table point
                    llwd_cur = *(curve_ptr->TablePtr + i + j * lby_t_len);

                    break;
                }
                else
                {
                    if(lwd_soc < *(curve_ptr->SocPtr + j + 1))
                    {
                        // soc between the table point

                        sllwtmp[0] = *(curve_ptr->TablePtr + i + j * lby_t_len);
                        sllwtmp[0] *= CUR_GAIN;
                        sllwtmp[1] = *(curve_ptr->TablePtr + i + (j + 1) * lby_t_len);
                        sllwtmp[1] *= CUR_GAIN;

                        // calculate derivative
                        llwd_k = (sllwtmp[1] - sllwtmp[0])
                                 /(*(curve_ptr->SocPtr + j + 1) - *(curve_ptr->SocPtr + j));
                        llwd_cur = llwd_k * (lwd_soc - *(curve_ptr->SocPtr + j) ) + sllwtmp[0];
                        llwd_cur /= CUR_GAIN;

                        break;
                    }
                }
            }

            break;
        }
        else
        {
            if(lby_t < *(curve_ptr->TemperPtr + i + 1))
            {
                // t between the table point
                for(j = 0; j < lby_soc_len; j++)
                {
                    if(lwd_soc == *(curve_ptr->SocPtr + j))
                    {
                        // on the table point

                        sllwtmp[0] = *(curve_ptr->TablePtr + i + j * lby_t_len);
                        sllwtmp[0] *= CUR_GAIN;
                        sllwtmp[1] = *(curve_ptr->TablePtr + (i + 1) + j * lby_t_len);
                        sllwtmp[1] *= CUR_GAIN;

                        llwd_k = (sllwtmp[1] - sllwtmp[0])
                                 /(*(curve_ptr->TemperPtr + i + 1) - *(curve_ptr->TemperPtr + i));
                        llwd_cur = llwd_k * (lby_t - *(curve_ptr->TemperPtr + i) ) + sllwtmp[0];
                        llwd_cur /= CUR_GAIN;

                        break;
                    }
                    else
                    {
                        if(lwd_soc < *(curve_ptr->SocPtr + j + 1))
                        {
                            // soc between the table point
                            sllwtmp[0] = *(curve_ptr->TablePtr + i + j * lby_t_len);
                            sllwtmp[0] *= CUR_GAIN;
                            sllwtmp[1] = *(curve_ptr->TablePtr + i + (j + 1) * lby_t_len);
                            sllwtmp[1] *= CUR_GAIN;
                            // calculate derivative
                            llwd_k = (sllwtmp[1] - sllwtmp[0])
                                     /(*(curve_ptr->SocPtr + j + 1) - *(curve_ptr->SocPtr + j));
                            sllwtmp_calc[0] = llwd_k * (lwd_soc - *(curve_ptr->SocPtr + j) ) + sllwtmp[0];

                            sllwtmp[0] = *(curve_ptr->TablePtr + (i + 1) + j * lby_t_len);
                            sllwtmp[0] *= CUR_GAIN;
                            sllwtmp[1] = *(curve_ptr->TablePtr + (i + 1) + (j + 1) * lby_t_len);
                            sllwtmp[1] *= CUR_GAIN;

                            // calculate derivative
                            llwd_k = (sllwtmp[1] - sllwtmp[0])
                                     /(*(curve_ptr->SocPtr + j + 1) - *(curve_ptr->SocPtr + j));
                            sllwtmp_calc[1] = llwd_k * (lwd_soc - *(curve_ptr->SocPtr + j) ) + sllwtmp[0];


                            llwd_k = (sllwtmp_calc[1] - sllwtmp_calc[0])
                                     /(*(curve_ptr->TemperPtr + i + 1) - *(curve_ptr->TemperPtr + i));
                            llwd_cur = llwd_k * (lby_t - *(curve_ptr->TemperPtr + i) ) + sllwtmp_calc[0];
                            llwd_cur /= CUR_GAIN;

                            break;
                        } // end of if(lwd_soc > *(curve_ptr->p_soc + j))
                    }
                } // end of for(j = 0; j < lby_soc_len; j++)

                break;
            }// end of if(lby_t > *(curve_ptr->p_t + i))
        }
    } // end of for(i = 0; i < lby_t_len; i++)

    wtmp = (uWord)(llwd_cur);

    return wtmp;
}

static void SOF_CalcRealValue( uWord * cur_ptr, uWord target, uWord unit)
{
    if(*cur_ptr > target)
    {
        if(*cur_ptr > target + unit)
        {
            *cur_ptr -= unit;
        }
        else
        {
            *cur_ptr = target;
        }
    }
    else if(*cur_ptr < target)
    {
        if(*cur_ptr + unit < target)
        {
            *cur_ptr += unit;
        }
        else
        {
            *cur_ptr = target;
        }
    }
    else
    {
        // keep old
    }
}

static void SOF_CalcPmtCur(sSOF_CalcValueType *Calc_ptr)
{
    sSOF_CalcValueType SOF_PowLmt;
    uLWord lwtmp;
    uWord w_unit;
    uByte i;
    uByte k_discharge[7];
    uByte k_feedback[7];

    /* get permit current value from array below */
    SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_CnstDisCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_CnstDisCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    if(SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge > lwtmp)
    {
        SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = (uWord)lwtmp;
    }
    
    SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_CnstFeedbackCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_CnstFeedbackCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    if(SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback > lwtmp)
    {
        SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback = (uWord)lwtmp;
    }
    
    SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_PulseFeedbackCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_PulseFeedbackCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    if(SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback > lwtmp)
    {
        SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = (uWord)lwtmp;
    }
    
    SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_PulseDisCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_PulseDisCurCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
    if(SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge > lwtmp)
    {
        SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = (uWord)lwtmp;
    }
    
    /* get permit power value from array below */
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
    {
        SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_CnstDisPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_CnstDisPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        if(SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge > lwtmp)
        {
            SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge = (uWord)lwtmp;
        }
        
        SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_CnstFeedbackPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_CnstFeedbackPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        if(SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback > lwtmp)
        {
            SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback = (uWord)lwtmp;
        }
        
        SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_PulseDisPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_PulseDisPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        if(SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge > lwtmp)
        {
            SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge = (uWord)lwtmp;
        }
        
        SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MinT, &SOF_PulseFeedbackPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        lwtmp = SOF_GetCurveCur(Calc_ptr->SOC, Calc_ptr->MaxT, &SOF_PulseFeedbackPowCurves, SOF_Macro.Sof_Max_Soc_Point, SOF_Macro.Sof_Max_T_Point);
        if(SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback > lwtmp)
        {
            SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback = (uWord)lwtmp;
        }
    }
    /* end of get value from array */
    
    /* user operation */
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_User_Fun)
    {
        /* user set target value */
        if(0xffff != gwd_SofTargetValue[SOF_CONST_DCHCUR])
        {
            SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = gwd_SofTargetValue[SOF_CONST_DCHCUR];
            gwd_SofTargetValue[SOF_CONST_DCHCUR] = 0xffff;
        }
        
        if(0xffff != gwd_SofTargetValue[SOF_CONST_FEEDBACKCUR])
        {
            SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback = gwd_SofTargetValue[SOF_CONST_FEEDBACKCUR];
            gwd_SofTargetValue[SOF_CONST_FEEDBACKCUR] = 0xffff;
        }
        
        if(0xffff != gwd_SofTargetValue[SOF_PULSE_DCHCUR])
        {
            SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = gwd_SofTargetValue[SOF_PULSE_DCHCUR];
            gwd_SofTargetValue[SOF_PULSE_DCHCUR] = 0xffff;
        }
        
        if(0xffff != gwd_SofTargetValue[SOF_PULSE_FEEDBACKCUR])
        {
            SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = gwd_SofTargetValue[SOF_PULSE_FEEDBACKCUR];
            gwd_SofTargetValue[SOF_PULSE_FEEDBACKCUR] = 0xffff;
        }
    
        if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
        {
            if(0xffff != gwd_SofTargetValue[SOF_CONST_DCHPOW])
            {
                SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge = gwd_SofTargetValue[SOF_CONST_DCHPOW];
                gwd_SofTargetValue[SOF_CONST_DCHPOW] = 0xffff;
            }
            
            if(0xffff != gwd_SofTargetValue[SOF_CONST_FEEDBACKPOW])
            {
                SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback = gwd_SofTargetValue[SOF_CONST_FEEDBACKPOW];
                gwd_SofTargetValue[SOF_CONST_FEEDBACKPOW] = 0xffff;
            }
            
            if(0xffff != gwd_SofTargetValue[SOF_PULSE_DCHPOW])
            {
                SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge = gwd_SofTargetValue[SOF_PULSE_DCHPOW];
                gwd_SofTargetValue[SOF_PULSE_DCHPOW] = 0xffff;
            }
            
            if(0xffff != gwd_SofTargetValue[SOF_PULSE_FEEDBACKPOW])
            {
                SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback = gwd_SofTargetValue[SOF_PULSE_FEEDBACKPOW];
                gwd_SofTargetValue[SOF_PULSE_FEEDBACKPOW] = 0xffff;
            }
        }
        /* end of user set target value */
        
        /* user modify value */
        if(100 != gby_SofModifyRatio[SOF_CONST_DCHCUR])
        {
            lwtmp = SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge;
            SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = (uWord)(lwtmp * gby_SofModifyRatio[SOF_CONST_DCHCUR] / 100);
            gby_SofModifyRatio[SOF_CONST_DCHCUR] = 100;
        }
        
        if(100 != gby_SofModifyRatio[SOF_CONST_FEEDBACKCUR])
        {
            lwtmp = SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback;
            SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback = (uWord)(lwtmp * gby_SofModifyRatio[SOF_CONST_FEEDBACKCUR] / 100);
            gby_SofModifyRatio[SOF_CONST_FEEDBACKCUR] = 100;
        }
        
        if(100 != gby_SofModifyRatio[SOF_PULSE_DCHCUR])
        {
            lwtmp = SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge; 
            SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = (uWord)(lwtmp * gby_SofModifyRatio[SOF_PULSE_DCHCUR] / 100);
            gby_SofModifyRatio[SOF_PULSE_DCHCUR] = 100;
        }
        
        if(100 != gby_SofModifyRatio[SOF_PULSE_FEEDBACKCUR])
        {
            lwtmp = SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback;
            SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = (uWord)(lwtmp * gby_SofModifyRatio[SOF_PULSE_FEEDBACKCUR] / 100);
            gby_SofModifyRatio[SOF_PULSE_FEEDBACKCUR] = 100;
        }
    
        if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
        {
            if(100 != gby_SofModifyRatio[SOF_CONST_DCHPOW])
            {
                lwtmp = SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge;
                SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge = (uWord)(lwtmp * gby_SofModifyRatio[SOF_CONST_DCHPOW] / 100);
                gby_SofModifyRatio[SOF_CONST_DCHPOW] = 100;
            }
            
            if(100 != gby_SofModifyRatio[SOF_CONST_FEEDBACKPOW])
            {
                lwtmp = SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback;
                SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback = (uWord)(lwtmp * gby_SofModifyRatio[SOF_CONST_FEEDBACKPOW] / 100);
                gby_SofModifyRatio[SOF_CONST_FEEDBACKPOW] = 100;
            }
            
            if(100 != gby_SofModifyRatio[SOF_PULSE_DCHPOW])
            {
                lwtmp = SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge;
                SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge = (uWord)(lwtmp * gby_SofModifyRatio[SOF_PULSE_DCHPOW] / 100);
                gby_SofModifyRatio[SOF_PULSE_DCHPOW] = 100;
            }
            
            if(100 != gby_SofModifyRatio[SOF_PULSE_FEEDBACKPOW])
            {
                lwtmp = SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback;
                SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback = (uWord)(lwtmp * gby_SofModifyRatio[SOF_PULSE_FEEDBACKPOW] / 100);
                gby_SofModifyRatio[SOF_PULSE_FEEDBACKPOW] = 100;
            }
        }
        /* end of user modify value */
    }    
    /* end of user operation */
    
    /* modify pulse current by current last time */
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Pulse_Cur_Modify)
    {
        lwtmp = Calc_ptr->Current;
        if(lwtmp > OFFSET_CUR)
        {
            gby_PulseFeedCurCnt = 0;
            
            lwtmp -= OFFSET_CUR;
            if(lwtmp > SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge)
            {
                gby_PulseDisCurCnt++; 
            }
            else
            {
                gby_PulseDisCurCnt = 0;
            }
            
            if(gby_PulseDisCurCnt >= SOF_ConfigPara.Sof_Pulse_Discharge_Cur_Time)
            {
                gby_PulseDisCurCnt = SOF_ConfigPara.Sof_Pulse_Discharge_Cur_Time;
                SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge;
            }
        }
        else if(lwtmp < OFFSET_CUR)
        {
            gby_PulseDisCurCnt = 0;
            
            lwtmp = OFFSET_CUR - lwtmp;
            if(lwtmp > SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback)
            {
                gby_PulseFeedCurCnt++; 
            }
            else
            {
                gby_PulseFeedCurCnt = 0;
            }
            
            if(gby_PulseFeedCurCnt >= SOF_ConfigPara.Sof_Pulse_FeedBack_Cur_Time)
            {
                gby_PulseFeedCurCnt = SOF_ConfigPara.Sof_Pulse_FeedBack_Cur_Time;
                SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback;
            }
        }
        else
        {
            gby_PulseDisCurCnt = 0;
            gby_PulseFeedCurCnt = 0;
        }
    }
    
    /* modify permit current by error and volt below */
    for(i = 0; i < 7; i++)
    {
        k_discharge[i] = 100;
        k_feedback[i] = 100;
    }
 
    // modify permit discharge current by error
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Dch_Cur_Err_Modify)
    {
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->CellvLowErr)
        {
            k_discharge[0] = SOF_ConfigPara.Sof_Modify_CellvLow_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->CellvLowErr)
        {
            k_discharge[0] = SOF_ConfigPara.Sof_Modify_CellvLow_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->SumvLowErr)
        {
            k_discharge[1] = SOF_ConfigPara.Sof_Modify_SumvLow_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->SumvLowErr)
        {
            k_discharge[1] = SOF_ConfigPara.Sof_Modify_SumvLow_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->DetVolErr)
        {
            k_discharge[2] = SOF_ConfigPara.Sof_Modify_DetVolt_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->DetVolErr)
        {
            k_discharge[2] = SOF_ConfigPara.Sof_Modify_DetVolt_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->DetTemErr)
        {
            k_discharge[3] = SOF_ConfigPara.Sof_Modify_DetTemp_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->DetTemErr)
        {
            k_discharge[3] = SOF_ConfigPara.Sof_Modify_DetTemp_L1;
        }
        
        if(SOF_ERR_LEVEL_ZERO != Calc_ptr->CanComErr)
        {
            k_discharge[4] = SOF_ConfigPara.Sof_Modify_IntCanErr;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->DchCurErr)
        {
            k_discharge[5] = SOF_ConfigPara.Sof_Modify_DchCur_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->DchCurErr)
        {
            k_discharge[5] = SOF_ConfigPara.Sof_Modify_DchCur_L1;
        }
    }
    
    // modify permit discharge current by min volt
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Dch_Cur_Volt_Modify)
    {
        for(i = 0; i < SOF_Macro.Sof_Max_Volt_Point; i++)
        {
            if(Calc_ptr->MinV < VoltLowArray[i].volt)
            {
                k_discharge[6] = VoltLowArray[i].k;
                break;
            }
        }
    }
    
    // modify permit feedback current by error
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Fd_Cur_Err_Modify)
    {
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->CellvHighErr)
        {
            k_feedback[0] = SOF_ConfigPara.Sof_Modify_CellvHigh_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->CellvHighErr)
        {
            k_feedback[0] = SOF_ConfigPara.Sof_Modify_CellvHigh_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->SumvHighErr)
        {
            k_feedback[1] = SOF_ConfigPara.Sof_Modify_SumvHigh_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->SumvHighErr)
        {
            k_feedback[1] = SOF_ConfigPara.Sof_Modify_SumvHigh_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->DetVolErr)
        {
            k_feedback[2] = SOF_ConfigPara.Sof_Modify_DetVolt_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->DetVolErr)
        {
            k_feedback[2] = SOF_ConfigPara.Sof_Modify_DetVolt_L1;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->DetTemErr)
        {
            k_feedback[3] = SOF_ConfigPara.Sof_Modify_DetTemp_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->DetTemErr)
        {
            k_feedback[3] = SOF_ConfigPara.Sof_Modify_DetTemp_L1;
        }
        
        if(SOF_ERR_LEVEL_ZERO != Calc_ptr->CanComErr)
        {
            k_feedback[4] = SOF_ConfigPara.Sof_Modify_IntCanErr;
        }
        
        if(SOF_ERR_LEVEL_TWO <= Calc_ptr->ChrCurErr)
        {
            k_feedback[5] = SOF_ConfigPara.Sof_Modify_FdCur_L2;
        }
        else if(SOF_ERR_LEVEL_ONE == Calc_ptr->ChrCurErr)
        {
            k_feedback[5] = SOF_ConfigPara.Sof_Modify_FdCur_L1;
        }
    }
    
    // modify permit feedback current by max volt
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Fd_Cur_Volt_Modify)
    {
        for(i = 0; i < SOF_Macro.Sof_Max_Volt_Point; i++)
        {
            if(Calc_ptr->MaxV > VoltHighArray[i].volt)
            {
                k_feedback[6] = VoltHighArray[i].k;
                break;
            }
        }
     }
    
    // modify by k
    lwtmp = k_discharge[0];
    for(i = 0; i < 7; i++)
    {
        if(lwtmp > k_discharge[i])
        {
            lwtmp = k_discharge[i];
        }
    }
    
    if(lwtmp != 100)
    {
        SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge) / 100);
        SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge) / 100);
    
        if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
        {
            SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge) / 100);
            SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge) / 100);
        }
    }
    
    lwtmp = k_feedback[0];
    for(i = 0; i < 7; i++)
    {
        if(lwtmp > k_feedback[i])
        {
            lwtmp = k_feedback[i];
        }
    }
    
    if(lwtmp != 100)
    {
        SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback) / 100);
        SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback) / 100);
        
        if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
        {
            SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback) / 100);
            SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback = (uWord)((lwtmp * SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback) / 100);
        }
    }
    /* end of modify permit current */  

    /* use self adaptation to modify permit current */
    if(Calc_ptr->MaxDchCur > SOF_MIN_DCHCUR)
    {
        if(Calc_ptr->MaxDchCur < SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge )
        {
            SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge = Calc_ptr->MaxDchCur;
        }
    }
    /* end of use self adaptation to modify permit current */

    /* smooth current below */    
    if((SOF_FLAG_FAIL == gby_FirstCalCurFlag)
      && ((SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge > 0)
         || (SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback > 0)
         || (SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge > 0)
         || (SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback > 0))) 
    {
        gby_FirstCalCurFlag = SOF_FLAG_OK;
        Calc_ptr->PowLmt.SOF_CnstCur.Discharge = SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge;
        Calc_ptr->PowLmt.SOF_CnstCur.Feedback = SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback;
        Calc_ptr->PowLmt.SOF_PulseCur.Discharge = SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge;
        Calc_ptr->PowLmt.SOF_PulseCur.Feedback = SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback;
    }
    else
    {
        w_unit = SOF_ConfigPara.Sof_Const_DchCur_Smooth_Unit;
        SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_CnstCur.Discharge), SOF_PowLmt.PowLmt.SOF_CnstCur.Discharge, w_unit);
        w_unit = SOF_ConfigPara.Sof_Const_FeedbackCur_Smooth_Unit;
        SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_CnstCur.Feedback), SOF_PowLmt.PowLmt.SOF_CnstCur.Feedback, w_unit);
        w_unit = SOF_ConfigPara.Sof_Pulse_DchCur_Smooth_Unit;
        SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_PulseCur.Discharge), SOF_PowLmt.PowLmt.SOF_PulseCur.Discharge, w_unit);
        w_unit = SOF_ConfigPara.Sof_Pulse_FeedbackCur_Smooth_Unit;
        SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_PulseCur.Feedback), SOF_PowLmt.PowLmt.SOF_PulseCur.Feedback, w_unit);
    }
    
    if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
    {        
        if((SOF_FLAG_FAIL == gby_FirstCalPowFlag)
          && ((SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge > 0)
             || (SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback > 0)
             || (SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge > 0)
             || (SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback > 0)))
        {
            gby_FirstCalPowFlag = SOF_FLAG_OK;
            Calc_ptr->PowLmt.SOF_CnstPow.Discharge = SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge;
            Calc_ptr->PowLmt.SOF_CnstPow.Feedback = SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback;
            Calc_ptr->PowLmt.SOF_PulsePow.Discharge = SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge;
            Calc_ptr->PowLmt.SOF_PulsePow.Feedback = SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback;
        }
        else
        {
            w_unit = SOF_ConfigPara.Sof_Const_DchPow_Smooth_Unit;
            SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_CnstPow.Discharge), SOF_PowLmt.PowLmt.SOF_CnstPow.Discharge, w_unit);
            w_unit = SOF_ConfigPara.Sof_Const_FeedbackPow_Smooth_Unit;
            SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_CnstPow.Feedback), SOF_PowLmt.PowLmt.SOF_CnstPow.Feedback, w_unit);
            w_unit = SOF_ConfigPara.Sof_Pulse_DchPow_Smooth_Unit;
            SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_PulsePow.Discharge), SOF_PowLmt.PowLmt.SOF_PulsePow.Discharge, w_unit);
            w_unit = SOF_ConfigPara.Sof_Pulse_FeedbackPow_Smooth_Unit;
            SOF_CalcRealValue(&(Calc_ptr->PowLmt.SOF_PulsePow.Feedback), SOF_PowLmt.PowLmt.SOF_PulsePow.Feedback, w_unit);
        }
    }
    /* end of smooth current */
    
    // prevent C5917: Removed dead assignment
    (void)lwtmp;
}

void SOF_ModifyUser(uByte type, uByte ratio)
{
    if(type < 8)
    {
        gby_SofModifyRatio[type] = ratio;
    }
}

void SOF_SetTargetUser(uByte type, uWord value)
{
    if(type < 8)
    {
        gwd_SofTargetValue[type] = value;
    }
}

void SOF_ModuleInit(void)
{
    uByte i;
    SOF_TmCnt.cnt = 0;
    SOF_TmCnt.selfad_cnt = 0;
    
    for(i = 0; i < 8; i++)
    {
        gby_SofModifyRatio[i] = 100;
        gwd_SofTargetValue[i] = 0xffff;
    }
   
    gby_FirstCalCurFlag = SOF_FLAG_FAIL;
    gby_FirstCalPowFlag = SOF_FLAG_FAIL;
}
void SOF_ModuleCountCbk(void)
{
    (SOF_TmCnt.cnt < 255) ? (SOF_TmCnt.cnt++) : (SOF_TmCnt.cnt = 255);
    (SOF_TmCnt.selfad_cnt < 255) ? (SOF_TmCnt.selfad_cnt++) : (SOF_TmCnt.selfad_cnt = 255);

}

void SOF_ModuleCbk(void)
{
    if(SOF_TmCnt.cnt >= TM_MS(10))
    {
        SOF_TmCnt.cnt = 0;
        
        SOF_UpdateBatSt();
        SOF_CalcPmtCur(&SOF_CalcValue);
        
        Bat_UpdateMaxPermitDchCur(SOF_CalcValue.PowLmt.SOF_CnstCur.Discharge);
        Bat_UpdateMaxFeedBackCur(SOF_CalcValue.PowLmt.SOF_CnstCur.Feedback);
        Bat_UpdateMaxPulseDchCur(SOF_CalcValue.PowLmt.SOF_PulseCur.Discharge);
        Bat_UpdateMaxPulseFeedBackCur(SOF_CalcValue.PowLmt.SOF_PulseCur.Feedback);
        
        if(SOF_FUN_ENABLE == SOF_Macro.Sof_Calc_Max_Power)
        {
            Bat_UpdateMaxPermitDchPower(SOF_CalcValue.PowLmt.SOF_CnstPow.Discharge);
            Bat_UpdateMaxPermitFeedbackPower(SOF_CalcValue.PowLmt.SOF_CnstPow.Feedback);
            Bat_UpdateMaxPulseDchPower(SOF_CalcValue.PowLmt.SOF_PulsePow.Discharge);
            Bat_UpdateMaxPulseFeedbackPower(SOF_CalcValue.PowLmt.SOF_PulsePow.Feedback);
        }
    }
    
    if(SOF_TmCnt.selfad_cnt >= TM_MS(20))
    {
        SOF_TmCnt.selfad_cnt = 0;
        SOF_SelfAdaptionCalcMaxDchCur();
    }
}

#endif // #ifdef USE_SOF_B

