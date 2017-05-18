// SOC.c
#include "SOC-B.h"
#include "ChargeSt.h"
#include "BmsSysSt.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "ErrorAlarm.h"
#include "DataStore.h"
#include "AppRelay.h"
#include "SysConf.h"
#include "SchIf.h"
#include "EE.h"
#include "LvM.h"

#ifdef USE_SOC_B


// task schedule counter
typedef struct
{
    uByte CntEstmat;
    uByte CntAhIntgl;
} sSOC_TmCntType;

sSOC_TmCntType SOC_TmCnt;


// error code define
typedef enum
{
    SOC_ERRCODE_CELLV_LOW = ERR_DATA_SOC_BASE,
    SOC_ERRCODE_CELLV_HIGH,
    SOC_ERRCODE_T_LOW,
    SOC_ERRCODE_T_HIGH,
    SOC_ERRCODE_CUR_LOW,
    SOC_ERRCODE_CUR_HIGH,
    SOC_ERRCODE_AVGV_USED,
    SOC_ERRCODE_AVGV_LOW,
    SOC_ERRCODE_CLOCK_EXT,

    SOC_ERRCODE_DETECT_CUR,
    SOC_ERRCODE_AH_ABNORMAL
    //must less than 32
} eSOC_ErrCodeType;

typedef struct
{
    uWord Cur;
    uWord AvgCur;    // 5s
    uWord MaxV;
    uWord MinV;
    uWord AvgV;
    uByte MaxT;
    uByte MinT;

    uWord CurBak[10]; // CurBak[1] --- last Cur, CurBak[0] --- current Cur
    uWord MinVBak[20];// MinVBak[1] --- last MinV, MinVBak[0] --- current MinV

    uByte CntCurSave;
} sSOC_EstmatDataInType;

sSOC_EstmatDataInType SOC_EstmatDataIn;


typedef struct
{
    uWord CntSysStart;
    uWord CntSaveTotalAh;

    uByte FlagOcvEstmatFinish;
    uByte FlagTemperEstmatFinish;
    uByte FlagSocToOcvArrayUsed;
    uByte FlagDynamicEstmatEnable;
    uByte FlagDeltaMinVLess50mV;

    uWord CntBatStatic;
    uWord CntBatDynamic;
    uWord CntLmtCellVHigh;
    uWord CntLmtCellVLow;
} sSOC_EstmatDataCalcType;

sSOC_EstmatDataCalcType SOC_EstmatDataCalc;


typedef struct
{
    uByte FlagValid;
    uByte FlagDir;
    uByte FlagOcvOnce;
    uLWord DeltaValue;
    uLWord TargetValue;
    uByte EstmatType;
    uWord TimeInterval;
} sSOC_EstmatDataOutType;

sSOC_EstmatDataOutType SOC_EstmatDataOut;

// define estimate type and priority
#define ESTMAT_TYPE_NULL     0U
#define ESTMAT_TYPE_TEMPER   1U
#define ESTMAT_TYPE_DYN_LOW  2U
#define ESTMAT_TYPE_OCV_STEP 3U
#define ESTMAT_TYPE_OCV_INIT 4U
#define ESTMAT_TYPE_LMT_LOW  5U
#define ESTMAT_TYPE_CHG_PRE  6U
#define ESTMAT_TYPE_CHG_END  7U
#define ESTMAT_TYPE_LMT_HIGH 8U
#define ESTMAT_TYPE_USER     9U

//#ifdef SOC_SMOOTH_EN
typedef struct
{
    uByte FlagValid;
    uByte FlagDir;
    uLWord DeltaValue;   // delta ah need to smooth

    uLWord AhIntglBak;   // for current and smooth dir NOT same
    uByte FlagAhIntglBak;// for current and smooth dir NOT same

    uWord TimeInterval;  // time interval to change 0.1%SOC
    uByte Cnt;
} sSOC_SmoothType;

sSOC_SmoothType SOC_Smooth;
//#endif


typedef struct
{
    uLWord TotalInAh;	 //unit:1mAh
    uLWord TotalInAhBak;
    uLWord TotalOutAh;
    uLWord TotalOutAhBak;

    uWord Soc;			 //unit:0.1%
    uWord SocBak;
    uLWord CurAh;		 // current ah
    uLWord MaxAh;		 // rated OR actual ah
    uByte FlagSaveAh;
} sSOC_AhIntglType;

sSOC_AhIntglType SOC_AhIntgl;


#define SOC_FLAG_OK      1U
#define SOC_FLAG_FAIL    0U
#define SOC_FLAG_ENABLE  1U
#define SOC_FLAG_DISABLE 0U
#define SOC_DIR_PLUS     1U
#define SOC_DIR_MINUS    0U

#define SOC_3_MIN        180U// unit:1s
#define SOC_6_MIN        360U
#define SOC_10_MIN       600U
#define SOC_30_MIN       1800U
#define SOC_ONE_HOUR     3600U
#define SOC_TWO_HOUR     7200U
#define SOC_120_HOUR     432000U // 5 day ,unit 1s

#define WCNT_100MS 1
#define WCNT_200MS 2
#define WCNT_500MS 5
#define WCNT_600MS 6
#define WCNT_1S    10
#define WCNT_2S    (2 * WCNT_1S)
#define WCNT_5S    (5 * WCNT_1S)
#define WCNT_10S   (10 * WCNT_1S)
#define WCNT_15S   (15 * WCNT_1S)
#define WCNT_20S   (20 * WCNT_1S)
#define WCNT_30S   (30 * WCNT_1S)
#define WCNT_60S   (60 * WCNT_1S)
#define WCNT_5MIN  (300 * WCNT_1S)

#define SOC_ESTMAT_CNT_LMT WCNT_60S

#define AH_ABNORMAL_VALUE         300U // unit:1mAh
#define CELLV_ABNORMAL_VALUE_MAX  4500 // unit:1mV
#define CELLV_ABNORMAL_VALUE_MIN  500  // unit:1mV
#define T_ABNORMAL_VALUE_MAX      100  // 60'C
#define T_ABNORMAL_VALUE_MIN	    10	 // -30'C

GeneralReturnType SOC_AhIntglProcess(void)
{
    GeneralReturnType err_code = ERR_OK;
    uLWord llwd_delta_ah;

    // detect current error
    //if(Err_GetHwErrLevel(ERR_HW_DETECT_CUR))
    //{
    //    err_code = (GeneralReturnType)SOC_ERRCODE_DETECT_CUR;
    //     return err_code;
    //}

    // get new ah
    SOC_AhIntgl.TotalInAh = Bat_GetAccumulateChgAh();
    SOC_AhIntgl.TotalOutAh = Bat_GetAccumulateDchgAh();


    // if no new ah
    if((SOC_AhIntgl.TotalInAhBak == SOC_AhIntgl.TotalInAh)
            && (SOC_AhIntgl.TotalOutAhBak == SOC_AhIntgl.TotalOutAh))
    {
        return err_code;
    }

    // calculate ah and soc
    if(SOC_AhIntgl.MaxAh > 0)
    {
        // ah calculation
        if(SOC_AhIntgl.TotalInAh > SOC_AhIntgl.TotalInAhBak)
        {
            llwd_delta_ah = SOC_AhIntgl.TotalInAh - SOC_AhIntgl.TotalInAhBak;

            if(llwd_delta_ah >= AH_ABNORMAL_VALUE)
            {
                err_code = (GeneralReturnType)SOC_ERRCODE_AH_ABNORMAL;
                return err_code;
            }

            Bat_UpdateTotalChgAh(Bat_GetTotalChgAh() + llwd_delta_ah);

            SOC_AhIntgl.FlagSaveAh = SOC_FLAG_OK;

//#ifdef SOC_SMOOTH_EN
            if(SOC_FUN_ENABLE == SOC_MacroSwitch.Soc_Smooth)
            {
                if((SOC_FLAG_OK == SOC_Smooth.FlagAhIntglBak)
                        && (SOC_DIR_MINUS == SOC_Smooth.FlagDir))// dec 0.1%SOC
                {
                    SOC_Smooth.AhIntglBak += llwd_delta_ah;

                    //ah changed more than 0.2%SOC
                    if(SOC_Smooth.AhIntglBak >= SOC_AhIntgl.MaxAh /500)
                    {
                        SOC_Smooth.FlagAhIntglBak = SOC_FLAG_FAIL;
                        SOC_Smooth.DeltaValue -= SOC_AhIntgl.MaxAh /1000;

                        llwd_delta_ah = SOC_Smooth.AhIntglBak - SOC_AhIntgl.MaxAh /1000;
                    }
                    else
                    {
                        llwd_delta_ah = 0;
                    }
                }
            }
//#endif

            SOC_AhIntgl.CurAh += llwd_delta_ah;

            // prevent soc excess 100%
            if(SOC_AhIntgl.CurAh > SOC_AhIntgl.MaxAh)
            {
                SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh;
            }

        }

        if(SOC_AhIntgl.TotalOutAh > SOC_AhIntgl.TotalOutAhBak)
        {
            llwd_delta_ah = SOC_AhIntgl.TotalOutAh - SOC_AhIntgl.TotalOutAhBak;

            if(llwd_delta_ah >= AH_ABNORMAL_VALUE)
            {
                err_code = (GeneralReturnType)SOC_ERRCODE_AH_ABNORMAL;
                return err_code;
            }

            Bat_UpdateTotalDChAh(Bat_GetTotalDchAh() + llwd_delta_ah);

            SOC_AhIntgl.FlagSaveAh = SOC_FLAG_OK;

//#ifdef SOC_SMOOTH_EN
            if(SOC_FUN_ENABLE == SOC_MacroSwitch.Soc_Smooth)
            {
                if((SOC_FLAG_OK == SOC_Smooth.FlagAhIntglBak)
                        && (SOC_DIR_PLUS == SOC_Smooth.FlagDir))// add 0.1%SOC
                {
                    SOC_Smooth.AhIntglBak += llwd_delta_ah;

                    // ah changed more than 0.2%SOC
                    if(SOC_Smooth.AhIntglBak >= SOC_AhIntgl.MaxAh /500)
                    {
                        SOC_Smooth.FlagAhIntglBak = SOC_FLAG_FAIL;
                        SOC_Smooth.DeltaValue -= SOC_AhIntgl.MaxAh /1000;

                        llwd_delta_ah = SOC_Smooth.AhIntglBak - SOC_AhIntgl.MaxAh /1000;
                    }
                    else
                    {
                        llwd_delta_ah = 0;
                    }
                }
            }
//#endif

            // prevent soc lower than 0
            if(SOC_AhIntgl.CurAh < llwd_delta_ah)
            {
                SOC_AhIntgl.CurAh = 0;
            }
            else
            {
                SOC_AhIntgl.CurAh -= llwd_delta_ah;;
            }

        }

        // soc calculation
        SOC_AhIntgl.Soc = (uWord)(SOC_AhIntgl.CurAh * 1000 / SOC_AhIntgl.MaxAh);

        if(CHG_ST_ON == Chg_GetChgSt())// charging
        {
            // both CC and CV mode valid
            if(SOC_EstmatDataIn.MaxV < (Chg_GetPermitMaxCellV() - SOC_ConfigPara.DeltaV_Soc_Less_985))// unit:1mV
            {
                if((SOC_AhIntgl.SocBak <= 985) && (SOC_AhIntgl.Soc > 985))
                {
                    SOC_AhIntgl.Soc = 985;
                    SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh * SOC_AhIntgl.Soc / 1000;
                }
            }
            if(SOC_EstmatDataIn.MaxV < (Chg_GetPermitMaxCellV() - SOC_ConfigPara.DeltaV_Soc_Less_995)) // unit:1mV
            {
                if((SOC_AhIntgl.SocBak  <= 995) && (SOC_AhIntgl.Soc > 995))
                {
                    SOC_AhIntgl.Soc = 995;
                    SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh * SOC_AhIntgl.Soc / 1000;
                }
            }
        }
    }

    // backup ah data for next calculation
    SOC_AhIntgl.TotalInAhBak = SOC_AhIntgl.TotalInAh;
    SOC_AhIntgl.TotalOutAhBak = SOC_AhIntgl.TotalOutAh;

    return err_code;
}

GeneralReturnType SOC_GetBatSt(void)
{
    GeneralReturnType err_code = ERR_OK;
    uByte i;
    uLWord sum_cur;

    SOC_EstmatDataIn.Cur = Bat_GetCurrent(INSCUR);
    SOC_EstmatDataIn.MaxV = Bat_GetMaxV();
    SOC_EstmatDataIn.MinV = Bat_GetMinV();
    SOC_EstmatDataIn.AvgV = Bat_GetAvgV();
    SOC_EstmatDataIn.MaxT = Bat_GetMaxT();
    SOC_EstmatDataIn.MinT = Bat_GetMinT();


    SOC_EstmatDataIn.CntCurSave %= 5;
    if(0 == SOC_EstmatDataIn.CntCurSave)// every 500ms save a current value
    {
        for(i = 9; i > 0; i--)
        {
            SOC_EstmatDataIn.CurBak[i] = SOC_EstmatDataIn.CurBak[i-1];
        }
    }
    SOC_EstmatDataIn.CurBak[0] = SOC_EstmatDataIn.Cur;
    if(0 == SOC_EstmatDataIn.CntCurSave)// every 500ms calculate a 5s avg_cur
    {
        sum_cur = 0;
        for(i = 0; i < 10; i++)
        {
            sum_cur += SOC_EstmatDataIn.CurBak[i];
        }
        // use this data after power on 10s
        SOC_EstmatDataIn.AvgCur = (uWord)(sum_cur /10);
    }
    SOC_EstmatDataIn.CntCurSave++;

    for(i = 19; i > 0; i--)// save 20 min_v
    {
        SOC_EstmatDataIn.MinVBak[i] = SOC_EstmatDataIn.MinVBak[i-1];
    }
    SOC_EstmatDataIn.MinVBak[0] = SOC_EstmatDataIn.MinV;

    if(SOC_EstmatDataIn.MaxV > CELLV_ABNORMAL_VALUE_MAX)
    {
        err_code = (GeneralReturnType)SOC_ERRCODE_CELLV_HIGH;
    }
    if(SOC_EstmatDataIn.MinV < CELLV_ABNORMAL_VALUE_MIN)
    {
        err_code = (GeneralReturnType)SOC_ERRCODE_CELLV_LOW;
    }
    if(SOC_EstmatDataIn.MaxT > T_ABNORMAL_VALUE_MAX)
    {
        err_code = (GeneralReturnType)SOC_ERRCODE_T_HIGH;
    }
    if(SOC_EstmatDataIn.MinT < T_ABNORMAL_VALUE_MIN)
    {
        err_code = (GeneralReturnType)SOC_ERRCODE_T_LOW;
    }

    return err_code;
}

GeneralReturnType SOC_EstmatOCV(void)
{
    GeneralReturnType err_code = ERR_OK;
    uByte i, t=0;
    uByte mode;
    uByte flg_estmat;
    uWord avg_v;
    uWord soc_tmp = 0;
    uWord soc_target = 0;
    uLWord llwd = 0;
    
    const sSOC_SocToVoltType  *SocOcvPtr;   
    SocOcvPtr = &SocToOcvArray[0][0];
    
    if(SOC_EstmatDataIn.MinT >= 55) // 15-60'C
    {
        t = 0;
    }
    else if(SOC_EstmatDataIn.MinT >= 40) // 0-15'C
    {
        t = 1;
    }
    else if(SOC_EstmatDataIn.MinT >= 30)// -10-0'C
    {
        t = 2;
    }
    else
    {
        t = 3;
    }

    err_code = BmsSys_CalcStandbyTime(&llwd);

    if(llwd >= SOC_ONE_HOUR)
    {
        mode = 1;

        if(SOC_120_HOUR <= llwd)
        {
            SOC_EstmatDataOut.FlagOcvOnce = SOC_FLAG_ENABLE;
        }
    }
    else if(llwd >= SOC_30_MIN)
    {
        mode = 2;
    }
    else if(llwd >= SOC_10_MIN)
    {
        mode = 3;
    }
    else if(llwd >= SOC_6_MIN)
    {
        mode = 4;
    }
    else if(llwd >= SOC_3_MIN)
    {
        mode = 5;
    }
    else
    {
        mode = 0;
    }

    avg_v = SOC_EstmatDataIn.AvgV;

    if ((avg_v > 0) && (avg_v <= 4500) && (0 != mode))
    {
//#ifdef OCV_INIT_ESTMAT_EN
        if(SOC_FUN_ENABLE == SOC_MacroSwitch.Ocv_Init_Estmat)
        {
            // new soc calc by lookup table
            if( ((avg_v >= SOC_ConfigPara.Ocv_Linear_Range_L1) && (avg_v <= SOC_ConfigPara.Ocv_Linear_Range_H1))
                    || ((avg_v >= SOC_ConfigPara.Ocv_Linear_Range_L2) && (avg_v <= SOC_ConfigPara.Ocv_Linear_Range_H2)) )
            {
                if(avg_v <= (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) ) -> Volt)
                {
                     soc_tmp = (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point))->Soc;
                }
                else if(avg_v >= (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + (SOC_Macro.Soc_Max_Soc_Point - 1))->Volt)
                {
                     soc_tmp = (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + (SOC_Macro.Soc_Max_Soc_Point - 1))->Soc;
                }
                else
                {
                    for (i = 1; i < SOC_Macro.Soc_Max_Soc_Point; i++)
                    {
                        if(avg_v == (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i)->Volt)
                        {
                            soc_tmp = (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i) ->Soc;
                            break;
                        }
                        else
                        {
                            if(avg_v < (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i)->Volt)
                            {
                                llwd = (uLWord)((SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i)->Volt - (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i - 1)->Volt) *100
                                       /((SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i) ->Soc - (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i - 1) ->Soc);

                                soc_tmp = (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i - 1)->Soc
                                          + (uWord)((uLWord)(avg_v - (SocOcvPtr + (t * SOC_Macro.Soc_Max_Soc_Point) + i - 1)->Volt) *100 / llwd);
                                break ;
                            }
                        }
                    }
                }

                SOC_EstmatDataCalc.FlagSocToOcvArrayUsed = SOC_FLAG_OK;
                SOC_EstmatDataCalc.FlagDynamicEstmatEnable = SOC_FLAG_ENABLE;

                flg_estmat = SOC_FLAG_FAIL;
                if(1 == mode)
                {
                    if((soc_tmp >= (SOC_AhIntgl.Soc + 20)) || ((soc_tmp + 20) <= SOC_AhIntgl.Soc))
                    {
                        flg_estmat = SOC_FLAG_OK;
                        soc_target = soc_tmp;
                    }
                }
                else if(2 == mode)
                {
                    if((soc_tmp >= (SOC_AhIntgl.Soc + 50)) || ((soc_tmp + 50) <= SOC_AhIntgl.Soc))
                    {
                        flg_estmat = SOC_FLAG_OK;
                        soc_target = (soc_tmp * 9 + SOC_AhIntgl.Soc) / 10;
                    }
                }
                else if(3 == mode)
                {
                    if((soc_tmp >= (SOC_AhIntgl.Soc + 50)) || ((soc_tmp + 50) <= SOC_AhIntgl.Soc))
                    {
                        flg_estmat = SOC_FLAG_OK;
                        soc_target = (soc_tmp * 8 + SOC_AhIntgl.Soc * 2) / 10;
                    }
                }
                else if(4 == mode)
                {
                    if((soc_tmp >= (SOC_AhIntgl.Soc + 50)) || ((soc_tmp + 50) <= SOC_AhIntgl.Soc))
                    {
                        flg_estmat = SOC_FLAG_OK;
                        soc_target = (soc_tmp * 7 + SOC_AhIntgl.Soc * 3) / 10;
                    }
                }
                else if(5 == mode)
                {
                    if( ((soc_tmp >= (SOC_AhIntgl.Soc + 50)) || ((soc_tmp + 50) <= SOC_AhIntgl.Soc))
                            && ((soc_tmp >= 700) || (soc_tmp <= 100)) )
                    {
                        flg_estmat = SOC_FLAG_OK;
                        soc_target = (soc_tmp * 7 + SOC_AhIntgl.Soc * 3) / 10;
                    }
                }
                else
                {
                    flg_estmat = SOC_FLAG_FAIL;
                }

                if(SOC_FLAG_OK == flg_estmat)
                {
                    SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;

                    if(soc_target > SOC_AhIntgl.Soc)
                    {
                        SOC_EstmatDataOut.DeltaValue = soc_target - SOC_AhIntgl.Soc;
                        SOC_EstmatDataOut.FlagDir = SOC_DIR_PLUS;
                    }
                    else
                    {
                        SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - soc_target;
                        SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;
                    }

                    SOC_EstmatDataOut.TargetValue = soc_target;
                    SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_OCV_INIT;
                }
            }
        }

//#endif// #ifdef OCV_INIT_ESTMAT_EN

        SOC_EstmatDataCalc.FlagOcvEstmatFinish = SOC_FLAG_OK;
    }
    else
    {
        SOC_EstmatDataCalc.FlagSocToOcvArrayUsed = SOC_FLAG_FAIL;
        SOC_EstmatDataCalc.FlagDynamicEstmatEnable = SOC_FLAG_ENABLE;
    }

    return err_code;
}


void SOC_EstmatDchDyn(void)
{
    uByte i, t = 0;
    uWord soc_tmp = 0, min_v;
    uWord min_v_max, min_v_min;
    uLWord llwd_tmp = 0;
    const sSOC_SocToVoltType *SocDynPtr;
    const sSOC_SocToVoltType *SocOcvPtr;
   
    SocDynPtr = &SocToVoltArrayDchDynamic[0][0]; 
    SocOcvPtr = &SocToOcvArray[0][0] ;

    // current limitation conditons cal
    if( (SOC_EstmatDataIn.Cur < (OFFSET_CUR + SOC_AhIntgl.MaxAh / 770))// <0.13c
            && (SOC_EstmatDataIn.Cur > (OFFSET_CUR - SOC_AhIntgl.MaxAh / 770)))// cur 0.1A ah 0.001Ah
    {
        if(SOC_EstmatDataCalc.CntBatStatic < 65000)
        {
            SOC_EstmatDataCalc.CntBatStatic++;
        }
    }
    else
    {
        SOC_EstmatDataCalc.CntBatStatic = 0;
    }

    if(SOC_EstmatDataIn.Cur > (OFFSET_CUR + SOC_AhIntgl.MaxAh /100))// >1c
    {
        SOC_EstmatDataCalc.CntBatDynamic = 0;
    }
    else
    {
        if(SOC_EstmatDataCalc.CntBatDynamic < 65000)
        {
            SOC_EstmatDataCalc.CntBatDynamic++;
        }
    }

    // cell voltage limitation conditons cal
    if((SOC_EstmatDataCalc.CntSysStart >= WCNT_10S)
            && (SOC_EstmatDataIn.MinV > 0) && (SOC_EstmatDataIn.MinV <= SOC_ConfigPara.Min_CellV_Low)
            && (SOC_EstmatDataIn.AvgV > 0) && (SOC_EstmatDataIn.AvgV <= SOC_ConfigPara.Avg_CellV_Low) )
    {
        min_v_max = 0;
        min_v_min = 0xFFFF;
        for(i = 0; i < 20; i++)
        {
            if(SOC_EstmatDataIn.MinVBak[i] < min_v_min)
            {
                min_v_min = SOC_EstmatDataIn.MinVBak[i];
            }
            if(SOC_EstmatDataIn.MinVBak[i] > min_v_max)
            {
                min_v_max = SOC_EstmatDataIn.MinVBak[i];
            }
        }
        if( ((min_v_max - min_v_min) <= 50)
                && (SOC_EstmatDataCalc.CntBatStatic >= WCNT_15S))
        {
            SOC_EstmatDataCalc.FlagDeltaMinVLess50mV = SOC_FLAG_OK;	// 2s min_v del <= 50mV
        }
        else
        {
            SOC_EstmatDataCalc.FlagDeltaMinVLess50mV = SOC_FLAG_FAIL;
        }
    }
    else
    {
        SOC_EstmatDataCalc.FlagDeltaMinVLess50mV = SOC_FLAG_FAIL;
    }

//#ifdef DYN_LOW_ESTMAT_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.Dyn_Low_Estmat)
    {
        if(SOC_FLAG_ENABLE == SOC_EstmatDataCalc.FlagDynamicEstmatEnable)
        {
            if(//(SOC_FLAG_FAIL == SOC_EstmatDataCalc.FlagSocToOcvArrayUsed) && // no soc_ocv modify
                (SOC_EstmatDataCalc.CntSysStart >=  WCNT_5MIN) // power on time >5min
                && (SOC_EstmatDataCalc.CntBatStatic >= WCNT_15S)  // static time >15s cur <0.1C
                //&& ((SOC_EstmatDataCalc.CntBatDynamic >= WCNT_15S)
                //    && (SOC_EstmatDataCalc.CntBatDynamic < WCNT_30S))// cur <1C
                // make sure cur >1C happened before soc dynamic estimate
                && (SOC_FLAG_OK == SOC_EstmatDataCalc.FlagDeltaMinVLess50mV))
            {
                min_v = SOC_EstmatDataIn.MinV;  // use min_v to modify soc

                // min_v < 3.269V
                if ((min_v > 0) && (min_v < SOC_ConfigPara.Min_CellV_Low)
                 && (SOC_EstmatDataIn.AvgV <= SOC_ConfigPara.Avg_CellV_Low))
                {
                    if(SOC_EstmatDataIn.MinT >= 55) // 15-60'C
                    {
                        t = 0;
                    }
                    else if(SOC_EstmatDataIn.MinT >= 40) // 0-15'C
                    {
                        t = 1;
                    }
                    else if(SOC_EstmatDataIn.MinT >= 30)// -10-0'C
                    {
                        t = 2;
                    }
                    else
                    {
                        t = 3;
                    }

                    if (min_v <= (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point)->Volt)
                    {
                        soc_tmp = (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point)->Soc;
                    }
                    else if (min_v >= (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + SOC_Macro.Soc_Max_Dch_Point - 1)->Volt)
                    {
                         soc_tmp = (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + SOC_Macro.Soc_Max_Dch_Point - 1)->Soc;
                    }
                    else
                    {
                        for (i = 1; i < SOC_Macro.Soc_Max_Dch_Point; i++)
                        {
                            if(min_v == (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i)->Volt)
                            {
                                soc_tmp = (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i)->Soc;
                                break;
                            }
                            else
                            {
                                if (min_v < (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i)->Volt)
                                {
                                    llwd_tmp = (uLWord)((SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i)->Volt - (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i - 1)->Volt) *100
                                               /((SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i)->Soc - (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i - 1)->Soc);
                                    soc_tmp = (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i - 1)->Soc
                                              + (uWord)((uLWord)(min_v - (SocDynPtr + t * SOC_Macro.Soc_Max_Dch_Point + i - 1)->Volt) *100 / llwd_tmp);
                                    
                                    break ;
                                }
                            }
                        }
                    }

                    // >5%, need to dynamic modify soc
                    //if ((((SOC_AhIntgl.Soc + 50) <= soc_tmp) || ((soc_tmp + 50) <= SOC_AhIntgl.Soc))
                    if( ((soc_tmp + 50) <= SOC_AhIntgl.Soc) && (soc_tmp <= SOC_ConfigPara.Max_Soc_Low_Estmat)
                            && (SOC_EstmatDataOut.EstmatType < ESTMAT_TYPE_DYN_LOW) )
                    {
                        SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;
                        if(soc_tmp > SOC_AhIntgl.Soc)
                        {
                            SOC_EstmatDataOut.DeltaValue = soc_tmp - SOC_AhIntgl.Soc;
                            SOC_EstmatDataOut.FlagDir = SOC_DIR_PLUS;
                        }
                        else
                        {
                            SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - soc_tmp;
                            SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;
                        }

                        SOC_EstmatDataOut.TargetValue = soc_tmp;
                        SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_DYN_LOW;

                        //only once dynamic modify
                        //SOC_EstmatDataCalc.FlagDynamicEstmatEnable = SOC_FLAG_DISABLE;
                    }
                }
            }
        }
    }
//#endif

//#ifdef SDYN_LOW_ESTMAT_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.Static_Low_Estmat)
    {
        if(SOC_FLAG_ENABLE == SOC_EstmatDataCalc.FlagDynamicEstmatEnable)
        {
            if( (SOC_EstmatDataOut.EstmatType < ESTMAT_TYPE_DYN_LOW)
                    && (SOC_EstmatDataCalc.CntSysStart >=  WCNT_5MIN) // power on time >5min
                    && (SOC_EstmatDataCalc.CntBatStatic >= WCNT_15S)  // static time >15s cur <0.1C
                    && (SOC_FLAG_OK == SOC_EstmatDataCalc.FlagDeltaMinVLess50mV)
                    && (SOC_EstmatDataIn.MinT >= 50) )
            {
                min_v = SOC_EstmatDataIn.MinV;  // use min_v to modify soc

                // min_v < 3.269V
                if ((min_v > 0) && (min_v < SOC_ConfigPara.Min_CellV_Low)
                 && (SOC_EstmatDataIn.AvgV <= SOC_ConfigPara.Avg_CellV_Low))
                {
                    min_v += 30;
                    t = 0;

                    if(min_v <= (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point)->Volt)
                    {
                       soc_tmp = (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point)->Soc;
                    }
                    else if(min_v >= (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + SOC_Macro.Soc_Max_Soc_Point - 1)->Volt)
                    {
                        soc_tmp = (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + SOC_Macro.Soc_Max_Soc_Point - 1)->Soc;
                    }
                    else
                    {
                        for (i = 1; i < SOC_Macro.Soc_Max_Soc_Point; i++)
                        {
                            if(min_v == (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i)->Volt)
                            {
                                soc_tmp = (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i)->Soc;
                                break;
                            }
                            else
                            {
                                if(min_v < (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i)->Volt)
                                {
                                    llwd_tmp = (uLWord)((SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i)->Volt - (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i - 1)->Volt) *100
                                                 /((SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i)->Soc - (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i - 1)->Soc);

                                    soc_tmp = (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i - 1)->Soc
                                              + (uWord)((uLWord)(min_v - (SocOcvPtr + t * SOC_Macro.Soc_Max_Soc_Point + i - 1)->Volt) *100 / llwd_tmp);

                                    break ;
                                }
                            }
                        }
                    }

                    // >5%, need to dynamic modify soc
                    if (((soc_tmp + 50) <= SOC_AhIntgl.Soc) && (soc_tmp <= SOC_ConfigPara.Max_Soc_Low_Estmat))
                    {
                        SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;

                        SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - soc_tmp;
                        SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;

                        SOC_EstmatDataOut.TargetValue = soc_tmp;
                        SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_DYN_LOW;

                        //only once dynamic modify
                        //SOC_EstmatDataCalc.FlagDynamicEstmatEnable = SOC_FLAG_DISABLE;
                    }
                }
            }
        }
    }
//#endif
    if(SOC_EstmatDataCalc.CntBatStatic >= WCNT_15S)
    {
        SOC_EstmatDataCalc.CntBatStatic = 0; // clear for next time to judge low modify
    }
}

void SOC_EstmatLimit(void)
{
    uWord soc_bak, soc_tmp;

    soc_bak = SOC_AhIntgl.Soc;
    soc_tmp = soc_bak;

//#ifdef LMT_HIGH_ESTMAT_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.Lmt_High_Estmat)
    {
        if((SOC_EstmatDataIn.MaxV > SOC_ConfigPara.Max_CellV_Lmt) && (SOC_EstmatDataIn.MaxV < 45000))
        {
            if(SOC_EstmatDataCalc.CntLmtCellVHigh < SOC_ESTMAT_CNT_LMT)
            {
                SOC_EstmatDataCalc.CntLmtCellVHigh++;
            }
            else
            {
                if(SOC_AhIntgl.Soc <= 950)
                {
                    soc_tmp = 1000;
                }
            }
        }
        else
        {
            SOC_EstmatDataCalc.CntLmtCellVHigh = 0;
        }
    }
//#endif// #ifdef LMT_HIGH_ESTMAT_EN

//#ifdef LMT_LOW_ESTMAT_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.Lmt_Low_Estmat)
    {
        if((SOC_EstmatDataIn.MinV < SOC_ConfigPara.Min_CellV_Lmt) && (SOC_EstmatDataIn.MinV > 0))
        {
            if(SOC_EstmatDataCalc.CntLmtCellVLow < SOC_ESTMAT_CNT_LMT)
            {
                SOC_EstmatDataCalc.CntLmtCellVLow++;
            }
            else
            {
                if(SOC_AhIntgl.Soc >= 50)
                {
                    soc_tmp = 0;
                }
            }
        }
        else
        {
            SOC_EstmatDataCalc.CntLmtCellVLow = 0;
        }
    }
//#endif// #ifdef LMT_LOW_ESTMAT_EN

    // output estimate value
    if(soc_tmp != soc_bak)
    {
        if((soc_tmp > SOC_AhIntgl.Soc) && (SOC_EstmatDataOut.EstmatType != ESTMAT_TYPE_LMT_HIGH))
        {
            SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;
            SOC_EstmatDataOut.DeltaValue = soc_tmp - SOC_AhIntgl.Soc;
            SOC_EstmatDataOut.FlagDir = SOC_DIR_PLUS;
            SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_LMT_HIGH;
            SOC_EstmatDataOut.TargetValue = soc_tmp;
        }

        if((soc_tmp < SOC_AhIntgl.Soc) && (SOC_EstmatDataOut.EstmatType != ESTMAT_TYPE_LMT_LOW))
        {
            SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;
            SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - soc_tmp;
            SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;
            SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_LMT_LOW;
            SOC_EstmatDataOut.TargetValue = soc_tmp;
        }
    }
}

void SOC_EstmatUser(uWord soc_target, uWord TimeInterval)
{
    if((soc_target != SOC_AhIntgl.Soc) && (ESTMAT_TYPE_USER != SOC_EstmatDataOut.EstmatType))
    {
        SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;
        SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_USER;
        SOC_EstmatDataOut.TargetValue = soc_target;

        if(SOC_EstmatDataOut.TargetValue > SOC_AhIntgl.Soc)
        {
            SOC_EstmatDataOut.DeltaValue = SOC_EstmatDataOut.TargetValue - SOC_AhIntgl.Soc;
            SOC_EstmatDataOut.FlagDir = SOC_DIR_PLUS;
        }
        else
        {
            SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - SOC_EstmatDataOut.TargetValue;
            SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;
        }

        if(0 != TimeInterval)
        {
            SOC_EstmatDataOut.TimeInterval = TimeInterval;// 0.1% / Time
        }
        else
        {
            SOC_EstmatDataOut.TimeInterval = WCNT_100MS;// not use
            SOC_EstmatDataOut.FlagOcvOnce = SOC_FLAG_ENABLE;
        }
    }
}

void SOC_EstmatActualAH(void)
{
    uByte i;
    uByte min_t;
    uWord wtmp = 0;
    uWord soh;
    uWord soc_tmp;
    uLWord llwd;
    uLWord standby_time = 0;
    uLWord bat_actual_ah, bat_actual_ah_bak;
    uLWord bat_rated_ah;
    const sSOC_TemperToAhType *SocTempPtr;
    
    SocTempPtr = TemperToAhArray;
    // modify actual AH by temper
//#ifdef AAH_TEMP_ESTMAT_EN
    (void)BmsSys_CalcStandbyTime(&standby_time);
    if( (SOC_FUN_ENABLE == SOC_MacroSwitch.ActualAh_Temper_Estmat)
            && (SOC_ONE_HOUR <= standby_time)
            && ((SOC_EstmatDataIn.MaxT - SOC_EstmatDataIn.MinT) <= 10)
            //&& (Err_GetHwErrLevel(ERR_HW_DETECT_TEMPER))
            && (ESTMAT_TYPE_NULL == SOC_EstmatDataOut.EstmatType) )
    {
        min_t = SOC_EstmatDataIn.MinT;

        if(min_t <= SocTempPtr->Temper)
        {
           // wtmp = TemperToAhArray[0].k_Ah;
            wtmp = SocTempPtr->k_Ah;
        }
        else if(min_t >= (SocTempPtr + SOC_Macro.Soc_Max_T_Ah_Point - 1)-> Temper)
        {
           wtmp = (SocTempPtr + SOC_Macro.Soc_Max_T_Ah_Point - 1)->k_Ah;
        }
        else
        {
            for (i = 1; i < SOC_Macro.Soc_Max_T_Ah_Point; i++)
            {
                if(min_t == (SocTempPtr + i)->Temper)
                {
                    wtmp = (SocTempPtr + i)->k_Ah;
                    break;
                }
                else
                {
                    if(min_t < (SocTempPtr + i)->Temper) 
                    {
                        llwd = (uLWord)((SocTempPtr + i)->Temper - (SocTempPtr + i -1)->Temper) * 100
                               / (uLWord)((SocTempPtr + i)->k_Ah - (SocTempPtr + i -1)->k_Ah);

                        wtmp = (SocTempPtr + i - 1)->k_Ah
                              + (uWord)((uLWord)(min_t - (SocTempPtr + i - 1)->Temper) *100 / llwd); 
                        break ;
                    }
                }
            }
        }

        soh = Bat_GetSoh();
        bat_rated_ah = Bat_GetRatedCapacityCalc();
        bat_actual_ah_bak = Bat_GetActualCapacityCalc();
        llwd = bat_rated_ah * soh / 1000;
        bat_actual_ah = llwd * wtmp / 100;

        Bat_UpdateActualCapacityCalc(bat_actual_ah);
        SOC_AhIntgl.MaxAh = bat_actual_ah;

        // calc SOC by new capacity
        llwd = bat_actual_ah_bak * (1000 - SOC_AhIntgl.Soc) / 1000;

        if(bat_actual_ah > llwd)
        {
            soc_tmp = (uWord)((bat_actual_ah - llwd) * 1000 / bat_actual_ah);
        }
        else
        {
            soc_tmp = 0;
        }

        // prepare for smooth soc
        llwd = SOC_AhIntgl.Soc * bat_actual_ah / 1000;
        Bat_UpdateRemainCapacity(llwd);
        SOC_AhIntgl.CurAh = llwd;

        (void)EE_SaveVar(BOTHEE, SYSTEMST_REMAIN_CAPACITY_BAK_G1);
        (void)EE_SaveVar(BOTHEE, SYSTEMST_RATED_ACTUAL_CAPACITY);
        /* end of calc battery capacity by min temper */

        // if delta SOC >2%
        if((soc_tmp >= (SOC_AhIntgl.Soc + 20)) || ((soc_tmp + 20) <= SOC_AhIntgl.Soc))
        {
            SOC_EstmatDataOut.FlagValid = SOC_FLAG_OK;

            if(soc_tmp > SOC_AhIntgl.Soc)
            {
                SOC_EstmatDataOut.DeltaValue = soc_tmp - SOC_AhIntgl.Soc;
                SOC_EstmatDataOut.FlagDir = SOC_DIR_PLUS;
            }
            else
            {
                SOC_EstmatDataOut.DeltaValue = SOC_AhIntgl.Soc - soc_tmp;
                SOC_EstmatDataOut.FlagDir = SOC_DIR_MINUS;
            }
            SOC_EstmatDataOut.TargetValue = soc_tmp;
            SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_TEMPER;
        }
    }
//#endif
    SOC_EstmatDataCalc.FlagTemperEstmatFinish = SOC_FLAG_OK;
}

//#ifdef SOC_SMOOTH_EN
void SOC_CurAhSmooth(void)
{
    if (SOC_Smooth.Cnt < WCNT_20S)
    {
        SOC_Smooth.Cnt++;
    }
    else
    {
        SOC_Smooth.Cnt = WCNT_20S;
    }

    if((SOC_Smooth.DeltaValue != 0)
            && (SOC_Smooth.Cnt >= SOC_Smooth.TimeInterval))
    {
        if(ESTMAT_TYPE_CHG_END != SOC_EstmatDataOut.EstmatType)
        {
            if((SOC_EstmatDataIn.Cur >= (OFFSET_CUR + SOC_AhIntgl.MaxAh /1000)) // >0.1C
                    || (SOC_EstmatDataIn.Cur <= (OFFSET_CUR - 30)))// < -3A
            {
                if(SOC_EstmatDataIn.Cur <= (OFFSET_CUR - 30))
                {
                    if(SOC_DIR_PLUS == SOC_Smooth.FlagDir)
                    {
                        if (SOC_AhIntgl.CurAh < SOC_AhIntgl.MaxAh)
                        {
                            SOC_AhIntgl.CurAh += SOC_AhIntgl.MaxAh /1000;
                            if(SOC_AhIntgl.CurAh > SOC_AhIntgl.MaxAh)
                            {
                                SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh;
                            }

                            if(SOC_Smooth.DeltaValue >= (SOC_AhIntgl.MaxAh /1000))
                            {
                                SOC_Smooth.DeltaValue -= SOC_AhIntgl.MaxAh /1000;
                            }
                            else
                            {
                                SOC_Smooth.DeltaValue = 0;
                            }
                        }
                        else
                        {
                            SOC_Smooth.DeltaValue = 0;
                        }
                    }
                    else // dec 0.1%SOC
                    {
                        if(SOC_FLAG_FAIL == SOC_Smooth.FlagAhIntglBak)
                        {
                            SOC_Smooth.FlagAhIntglBak = SOC_FLAG_OK;
                            SOC_Smooth.AhIntglBak = 0;
                        }
                    }
                }

                if((SOC_EstmatDataIn.Cur >= (OFFSET_CUR + SOC_AhIntgl.MaxAh /1000)))
                {
                    if (SOC_DIR_PLUS == SOC_Smooth.FlagDir)
                    {
                        if(SOC_FLAG_FAIL == SOC_Smooth.FlagAhIntglBak)
                        {
                            SOC_Smooth.FlagAhIntglBak = SOC_FLAG_OK;
                            SOC_Smooth.AhIntglBak = 0;
                        }
                    }
                    else // dec 0.1%SOC
                    {
                        if (SOC_AhIntgl.CurAh >= SOC_AhIntgl.MaxAh /1000)
                        {
                            SOC_AhIntgl.CurAh -= SOC_AhIntgl.MaxAh /1000;

                            if(SOC_Smooth.DeltaValue >= (SOC_AhIntgl.MaxAh /1000))
                            {
                                SOC_Smooth.DeltaValue -= SOC_AhIntgl.MaxAh /1000;
                            }
                            else
                            {
                                SOC_Smooth.DeltaValue = 0;
                            }
                        }
                        else
                        {
                            SOC_Smooth.DeltaValue = 0;
                        }
                    }
                }
            }
        }
        else// ESTMAT_TYPE_CHG_END no current limitation
        {
            if (SOC_DIR_PLUS == SOC_Smooth.FlagDir)// add 0.1%SOC
            {
                if (SOC_AhIntgl.CurAh < SOC_AhIntgl.MaxAh)
                {
                    SOC_AhIntgl.CurAh += SOC_AhIntgl.MaxAh /1000;
                    if(SOC_AhIntgl.CurAh > SOC_AhIntgl.MaxAh)
                    {
                        SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh;
                    }

                    if(SOC_Smooth.DeltaValue >= (SOC_AhIntgl.MaxAh /1000))
                    {
                        SOC_Smooth.DeltaValue -= SOC_AhIntgl.MaxAh /1000;
                    }
                    else
                    {
                        SOC_Smooth.DeltaValue = 0;
                    }
                }
                else
                {
                    SOC_Smooth.DeltaValue = 0;
                }
            }
        }

        SOC_Smooth.Cnt = 0;
    }

    if(0 == SOC_Smooth.DeltaValue)
    {
        SOC_Smooth.FlagValid = SOC_FLAG_FAIL;
        SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_NULL;
    }
}
//#endif// #ifdef SOC_SMOOTH_EN

GeneralReturnType SOC_EstmatProcess(void)
{
    GeneralReturnType err_code = ERR_OK;

    SOC_EstmatDataCalc.CntSysStart++;
    if(SOC_EstmatDataCalc.CntSysStart >= WCNT_5MIN)
    {
        SOC_EstmatDataCalc.CntSysStart = WCNT_5MIN;
    }

    SOC_EstmatDataCalc.CntSaveTotalAh++;
    if( (SOC_EstmatDataCalc.CntSaveTotalAh >= WCNT_5MIN)
            || (LVM_STATE_OFF == BmsSys_GetAwakeSt()) )
    {
        SOC_EstmatDataCalc.CntSaveTotalAh = 0;
        if(SOC_FLAG_OK == SOC_AhIntgl.FlagSaveAh)
        {
            SOC_AhIntgl.FlagSaveAh = SOC_FLAG_FAIL;
            err_code = EE_SaveVar(BOTHEE, SYSTEMST_TOTAL_CHG_DCH_AH);

        }
    }

    err_code = SOC_GetBatSt();

    if((SOC_FLAG_FAIL == SOC_EstmatDataCalc.FlagOcvEstmatFinish)
            && ((uByte)RELAY_CLOSED != BatSys_GetPosRlySt())
            && ((uByte)RELAY_CLOSED != BatSys_GetPreRlySt())
            // may need add bmu[N] conditions
            && (SOC_EstmatDataIn.MinV > 0)
            && (SOC_EstmatDataCalc.CntSysStart >= WCNT_200MS)
            && (SOC_EstmatDataCalc.CntSysStart < WCNT_600MS))// time may need modify
    {
        if(ERR_LEVEL_NORMAL == Err_GetHwErrLevel(ERR_HW_CLOCK_EXT))
        {
            err_code = SOC_EstmatOCV();// only once OCV estimate
        }
        else
        {
            err_code = (GeneralReturnType)SOC_ERRCODE_CLOCK_EXT;
        }
    }

    SOC_EstmatDchDyn();
    SOC_EstmatLimit();

    if(SOC_FLAG_OK == SOC_EstmatDataOut.FlagValid)// for all modify
    {
        SOC_EstmatDataOut.FlagValid = SOC_FLAG_FAIL;

//#ifdef SOC_SMOOTH_EN
        if( (SOC_FUN_ENABLE == SOC_MacroSwitch.Soc_Smooth)
                && (SOC_FLAG_DISABLE == SOC_EstmatDataOut.FlagOcvOnce) )
        {
            SOC_Smooth.FlagValid = SOC_FLAG_OK;
            SOC_Smooth.FlagDir = SOC_EstmatDataOut.FlagDir;
            SOC_Smooth.DeltaValue = SOC_EstmatDataOut.DeltaValue * SOC_AhIntgl.MaxAh /1000;

            if( (ESTMAT_TYPE_DYN_LOW == SOC_EstmatDataOut.EstmatType)
                    && (SOC_EstmatDataOut.TargetValue <= 300) )
            {
                // speed up smooth for ESTMAT_TYPE_DYN_LOW
                SOC_Smooth.TimeInterval = WCNT_2S;// 0.1%/2S equal to 0.5%/10S and 3%/1min
            }
            else if( (ESTMAT_TYPE_OCV_INIT == SOC_EstmatDataOut.EstmatType)
                     && (SOC_EstmatDataOut.TargetValue <= 300))
            {
                // speed up smooth for ESTMAT_TYPE_DYN_LOW
                SOC_Smooth.TimeInterval = WCNT_2S;// 0.1%/2S equal to 0.5%/10S and 3%/1min
            }
            else if( (ESTMAT_TYPE_LMT_LOW == SOC_EstmatDataOut.EstmatType)
                     || (ESTMAT_TYPE_LMT_HIGH == SOC_EstmatDataOut.EstmatType) )
            {
                SOC_Smooth.TimeInterval = WCNT_2S;// 0.1%/2S equal to 0.5%/10S and 3%/1min
            }
            else if(ESTMAT_TYPE_USER == SOC_EstmatDataOut.EstmatType)
            {
                SOC_Smooth.TimeInterval = SOC_EstmatDataOut.TimeInterval;
            }
            else
            {
                SOC_Smooth.TimeInterval = WCNT_10S;// 0.1%/10S equal to 0.6%/1min
            }
        }
//#else
        else
        {
            SOC_EstmatDataOut.FlagOcvOnce = SOC_FLAG_DISABLE;
            // direct update current Ah
            SOC_EstmatDataOut.DeltaValue *= SOC_AhIntgl.MaxAh /1000;
            if(SOC_DIR_PLUS == SOC_EstmatDataOut.FlagDir)
            {
                if((SOC_AhIntgl.CurAh + SOC_EstmatDataOut.DeltaValue) < SOC_AhIntgl.MaxAh)
                {
                    SOC_AhIntgl.CurAh += SOC_EstmatDataOut.DeltaValue;
                }
                else
                {
                    SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh;
                }
            }
            else
            {
                if(SOC_AhIntgl.CurAh > SOC_EstmatDataOut.DeltaValue)
                {
                    SOC_AhIntgl.CurAh -= SOC_EstmatDataOut.DeltaValue;
                }
                else
                {
                    SOC_AhIntgl.CurAh = 0;
                }
            }
            SOC_EstmatDataOut.EstmatType = ESTMAT_TYPE_NULL;
        }
//#endif// #ifdef SOC_SMOOTH_EN

        //DataStore_UpdateRsvData((uByte*)(&(SOC_EstmatDataOut.EstmatType)), sizeof(SOC_EstmatDataOut.EstmatType));
        DataStore_EnableEventDataStore(MFY_SOC);
    }

//#ifdef SOC_SMOOTH_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.Soc_Smooth)
    {
        if(SOC_FLAG_OK == SOC_Smooth.FlagValid)// for all estimate
        {
            SOC_CurAhSmooth();
        }
    }
//#endif

    return err_code;
}


void SOC_ModuleInit(void)
{
    uByte * byte_ptr;
    uWord i;
    uWord soc;

    byte_ptr = (uByte *)(&SOC_EstmatDataIn);
    for(i=0; i<sizeof(SOC_EstmatDataIn); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte *)(&SOC_EstmatDataCalc);
    for(i=0; i<sizeof(SOC_EstmatDataCalc); i++)
    {
        *(byte_ptr++) = 0;
    }

    byte_ptr = (uByte *)(&SOC_EstmatDataOut);
    for(i=0; i<sizeof(SOC_EstmatDataOut); i++)
    {
        *(byte_ptr++) = 0;
    }

//#ifdef SOC_SMOOTH_EN
    byte_ptr = (uByte *)(&SOC_Smooth);
    for(i=0; i<sizeof(SOC_Smooth); i++)
    {
        *(byte_ptr++) = 0;
    }
//#endif

    byte_ptr = (uByte *)(&SOC_AhIntgl);
    for(i=0; i<sizeof(SOC_AhIntgl); i++)
    {
        *(byte_ptr++) = 0;
    }

    SOC_AhIntgl.CurAh = Bat_GetRemainCapacity();

//#ifdef AAH_SOH_ESTMAT_EN
    if(SOC_FUN_ENABLE == SOC_MacroSwitch.ActualAh_Soh_Estmat)
    {
        SOC_AhIntgl.MaxAh = Bat_GetActualCapacityCalc();
    }
//#else
    else
    {
        SOC_AhIntgl.MaxAh = Bat_GetRatedCapacityCalc();
    }
//#endif

    soc = (uWord)(SOC_AhIntgl.CurAh * 1000 / SOC_AhIntgl.MaxAh);
    Bat_UpdateSOC(soc);

    SOC_AhIntgl.Soc = soc;
    SOC_AhIntgl.SocBak = soc;

    SOC_TmCnt.CntEstmat = 0;
    SOC_TmCnt.CntAhIntgl = 0;

    SOC_EstmatDataCalc.FlagDynamicEstmatEnable = SOC_FLAG_ENABLE;
}

void SOC_ModuleCountCbk(void)
{
    (SOC_TmCnt.CntEstmat < 255) ? (SOC_TmCnt.CntEstmat ++) : (SOC_TmCnt.CntEstmat = 255);
    (SOC_TmCnt.CntAhIntgl < 255) ? (SOC_TmCnt.CntAhIntgl ++) : (SOC_TmCnt.CntAhIntgl = 255);
}

void SOC_ModuleCbk(void)
{
    GeneralReturnType err_code = ERR_OK;

    SOC_AhIntgl.CurAh = Bat_GetRemainCapacity();

    if(SOC_FUN_ENABLE == SOC_MacroSwitch.ActualAh_Soh_Estmat)
    {
        SOC_AhIntgl.MaxAh = Bat_GetActualCapacityCalc();
    }
    else
    {
        SOC_AhIntgl.MaxAh = Bat_GetRatedCapacityCalc();
    }


    if(SOC_TmCnt.CntAhIntgl >= TM_500MS)
    {
        SOC_TmCnt.CntAhIntgl = 0;
        err_code = SOC_AhIntglProcess();
    }

    if(SOC_TmCnt.CntEstmat >= TM_100MS)
    {
        SOC_TmCnt.CntEstmat = 0;
        err_code = SOC_EstmatProcess();
    }

    // prevent soc excess 100%
    if(SOC_AhIntgl.CurAh > SOC_AhIntgl.MaxAh)
    {
        SOC_AhIntgl.CurAh = SOC_AhIntgl.MaxAh;
    }

    // update battery status data
    Bat_UpdateRemainCapacity(SOC_AhIntgl.CurAh);
    // soc calculation
    SOC_AhIntgl.Soc = (uWord)(SOC_AhIntgl.CurAh * 1000 / SOC_AhIntgl.MaxAh);

    // update battery status data
    Bat_UpdateSOC(SOC_AhIntgl.Soc);


    if ((SOC_AhIntgl.SocBak >= (SOC_AhIntgl.Soc + 2))
            || ((SOC_AhIntgl.SocBak + 2) <= SOC_AhIntgl.Soc)
            || ((0 != SOC_AhIntgl.SocBak) && (0 == SOC_AhIntgl.Soc))
            || ((1000 != SOC_AhIntgl.SocBak) && (1000 == SOC_AhIntgl.Soc))
            || ((SOC_AhIntgl.SocBak != SOC_AhIntgl.Soc) && (LVM_STATE_OFF == BmsSys_GetAwakeSt())))
    {
        // save current ah, need save ah when power off
        err_code = EE_SaveVar(BOTHEE, SYSTEMST_REMAIN_CAPACITY_BAK_G1);
        SOC_AhIntgl.SocBak = SOC_AhIntgl.Soc;
    }

    // only for debug
    /*if(ERR_OK != err_code)
    {
        // save err_code
        DataStore_UpdateRsvData((uByte*)(&err_code), sizeof(err_code));
        DataStore_EnableEventDataStore(MFY_SOC);
    } */

    // prevent C5917: Removed dead assignment
    (void)err_code;
}

#endif // #ifdef USE_SOC_B