#include "SOH-A.h"
#include "EE.h"
#include "BatterySt.h"
#include "BmsSysSt.h"
#include "ChargeSt.h"
#include "SchIf.h"
#include "SOC.h"
#include "DataStore.h"

#ifdef USE_SOH_A

//#define SOH_A_DEBUG

typedef struct
{
    uByte cnt;
} sSOH_TmCntType;

sSOH_TmCntType SOH_TmCnt;

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
    uByte  SohCalcStep;
    uLWord TotalChgAhBackup;
    uLWord TotalDchAhBackup;
    uLWord BatActualAhBackup[2];
} sSOH_CtrlParaType;

sSOH_CtrlParaType SOH_CtrlPara;

#pragma DATA_SEG DEFAULT

static uByte gby_flag_judge_bat_static;

#define SOH_MAX_T_POINT 1
#define SOH_MAX_SOH_POINT 21

#define SOH_CALC_STEP_NO_START   0
#define SOH_CALC_STEP_BACKUP_AH  1
#define SOH_CALC_STEP_CALC_SOH   2


#define SOH_TIME_1HOUR   3600			 //unit:S
#define SOH_TIME_5DAY    432000    //unit:S

#define SOH_CALC_20AH  20000

#define SOH_NO_JUDGE_BAT_STATIC   0
#define SOH_HAS_JUDGE_BAT_STATIC  1

static void SOH_Calc_Proc(void);

/*****************************************************************************/
/* FUNCTION: initial soh calc                                                */
/*                                                                           */
/* DESCRIPTION:                                                              */
/*****************************************************************************/
void SOH_ModuleInit(void)
{
    GeneralReturnType rslt[3];
    uWord soh;
    uLWord bat_total_chg_ah;
    uLWord bat_total_dch_ah;
    uLWord bat_rated_ah;
    uLWord bat_actual_ah;

    SOH_TmCnt.cnt = 0;

    rslt[0] = SysConf_InitParaTable(SYSTEMST_SOH_CTRL_PARA, (uLWord)(&SOH_CtrlPara), sizeof(SOH_CtrlPara));

    rslt[0] = EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH);
    rslt[1] = EE_LoadVar(SYSTEMST_SOH_CTRL_PARA);
    rslt[2] = EE_LoadVar(SYSTEMST_SOH);

    bat_total_chg_ah = Bat_GetTotalChgAh();
    bat_total_dch_ah = Bat_GetTotalDchAh();
    bat_actual_ah = Bat_GetActualCapacityCalc();
    bat_rated_ah = Bat_GetRatedCapacityCalc();

    if(ERR_OK != rslt[0])
    {
        bat_total_chg_ah = 0;
        bat_total_dch_ah = 0;
        Bat_UpdateTotalChgAh(bat_total_chg_ah);
        Bat_UpdateTotalDChAh(bat_total_dch_ah);

        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_TOTAL_CHG_DCH_AH);
    }

    if(ERR_OK != rslt[1])
    {
        SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_NO_START;
        SOH_CtrlPara.TotalChgAhBackup = 0;
        SOH_CtrlPara.TotalDchAhBackup = 0;
        SOH_CtrlPara.BatActualAhBackup[0] = 0;
        SOH_CtrlPara.BatActualAhBackup[1] = 0;

        rslt[1] = EE_SaveVar(BOTHEE, SYSTEMST_SOH_CTRL_PARA);
    }

    if( (bat_actual_ah > bat_rated_ah)
     || (0 == bat_actual_ah)
     || (0xffffffff == bat_actual_ah) )
    {
        bat_actual_ah = bat_rated_ah;
        Bat_UpdateActualCapacityCalc(bat_actual_ah);
        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_RATED_ACTUAL_CAPACITY);
    }

    //soh = (uWord)((bat_actual_ah * 1000) / bat_rated_ah);
    soh = Bat_GetSoh();
    if((soh > 1000) || (soh < 600))
    {
        Bat_UpdateSoh(1000);
    }
}

void SOH_ModuleCountCbk(void)
{
    (SOH_TmCnt.cnt < 255) ? (SOH_TmCnt.cnt++) : (SOH_TmCnt.cnt = 255);
}

void SOH_ModuleCbk(void)
{
    if(SOH_TmCnt.cnt >= TM_MS(200))
    {
        SOH_TmCnt.cnt = 0;
        SOH_Calc_Proc();
    }
}
/*****************************************************************************/
/* FUNCTION: process of soh calc                                             */
/*                                                                           */
/* DESCRIPTION: should call this funtion every 100ms                         */
/*****************************************************************************/
static void SOH_Calc_Proc(void)
{
    uByte i;
    GeneralReturnType rslt = ERR_OK;  
    uByte soh_calc_step_backup;
    uWord minv;
    uWord maxv;
    uWord soh;
    uWord target_soc;
    uLWord llwtmp;
    uLWord wtmp_ocv;
    uLWord static_time;
    uLWord bat_total_chg_ah;
    uLWord bat_total_dch_ah;
    uLWord bat_rated_ah;
    uLWord bat_actual_ah;
    uLWord remain_ah;
    uLWord actual_ah;
    uLWord dch_ah;
    uLWord chg_ah;
    uWord k_temp;
    
    const sSOC_SocToVoltType *SohOcvPtr;
    SohOcvPtr = &SocToOcvArray[0][0];
#ifdef SOH_A_DEBUG
    Bat_UpdateTotalChgAh(1055);
    Bat_UpdateTotalDChAh(2110);
    Bat_UpdateActualCapacityCalc(96);
    Bat_UpdateRatedCapacityCalc(100);
    Bat_UpdateMinV(3000);
    Bat_UpdateMaxV(3100);
//    Chg_UpdateChgStage(CHG_STAGE_END);
#endif
    minv = Bat_GetMinV();
    maxv = Bat_GetMaxV();
    bat_total_chg_ah = Bat_GetTotalChgAh();
    bat_total_dch_ah = Bat_GetTotalDchAh();
    bat_actual_ah = Bat_GetActualCapacityCalc();
    bat_rated_ah = Bat_GetRatedCapacityCalc();
    target_soc = 0;
    soh = Bat_GetSoh();
    
    // calc temper K
    k_temp = (uWord)((bat_rated_ah * soh) / bat_actual_ah);

    soh_calc_step_backup = SOH_CtrlPara.SohCalcStep;

    // judge whether charge full end
    if(CHG_STAGE_END == Chg_GetChgStage())
    {
        if(SOH_CALC_STEP_NO_START == SOH_CtrlPara.SohCalcStep)
        {
            // backup total charge ah and total discharge ah when charge full
            SOH_CtrlPara.TotalChgAhBackup = bat_total_chg_ah;
            SOH_CtrlPara.TotalDchAhBackup = bat_total_dch_ah;
            SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_BACKUP_AH;
        }
    }
    // if charge ah > 33% Rated AH, then stop calc SOH
    if( (SOH_CALC_STEP_BACKUP_AH == SOH_CtrlPara.SohCalcStep)
     && (bat_total_chg_ah >= (SOH_CtrlPara.TotalChgAhBackup + (bat_rated_ah / 3))) )
    {
        SOH_CtrlPara.TotalChgAhBackup = 0;
        SOH_CtrlPara.TotalDchAhBackup = 0;
        SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_NO_START;
    }

    // judge whether battery is static
    if(SOH_NO_JUDGE_BAT_STATIC == gby_flag_judge_bat_static)
    {
        // begin to check battery static time
        rslt = BmsSys_CalcStandbyTime(&static_time);
#ifdef SOH_A_DEBUG
         rslt = ERR_OK; 
         static_time = 200;
#endif
        if(ERR_OK == rslt) // judge whether clock is ok
        {
            // check whther battery static time > 1H and < 5 day
            if(static_time > SOH_TIME_1HOUR) 
            {
                if(static_time < SOH_TIME_5DAY)   // >1h and <5d
                {                  
                    if(SOH_CALC_STEP_BACKUP_AH == SOH_CtrlPara.SohCalcStep)
                    {
                        // enable start calc soh
                        SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_CALC_SOH;
                    }
                }
                else// battery static time >= 5 day
                {
                    // battery static time too long, stop cal soh
                    SOH_CtrlPara.TotalChgAhBackup = 0;
                    SOH_CtrlPara.TotalDchAhBackup = 0;
                    SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_NO_START;
                }
            }
        }
        else
        {
            // time hardware error
            SOH_CtrlPara.TotalChgAhBackup = 0;
            SOH_CtrlPara.TotalDchAhBackup = 0;
            SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_NO_START;
        }
        gby_flag_judge_bat_static = SOH_HAS_JUDGE_BAT_STATIC;
    }

    // start to calc soh
    if (SOH_CALC_STEP_CALC_SOH == SOH_CtrlPara.SohCalcStep)
    {
        SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_BACKUP_AH;

        // judge condition, soc of minv-bat <= 35%, dV <= 400mV and minv >= 2V
        if( (minv <= SOH_ConfigPara.Max_Ocv_Calc_Soh) && (minv >= SOH_ConfigPara.Min_Ocv_Calc_Soh)
         && ((maxv - minv) <= SOH_ConfigPara.DeltaV_Calc_Soh) )
        {
            wtmp_ocv = minv;
            SOH_CtrlPara.SohCalcStep = SOH_CALC_STEP_NO_START;
            
            // get soc by ocv
            if(wtmp_ocv <= SohOcvPtr->Volt)
            {
                target_soc = 0;
            }
            else if (wtmp_ocv >= (SohOcvPtr + SOC_Macro.Soc_Max_Soc_Point - 1)->Volt)
            {
                target_soc = 1000;
            }
            else
            {
                for (i = 1; i < SOC_Macro.Soc_Max_Soc_Point; i++)
                {
                    if(wtmp_ocv == (SohOcvPtr + i)->Volt)
                    {
                        target_soc = (SohOcvPtr + i)->Soc;
                        break;
                    }
                    else
                    {
                        if (wtmp_ocv < (SohOcvPtr + i)->Volt)
                        {
                            llwtmp = (SohOcvPtr + i)->Volt - (SohOcvPtr + i - 1)->Volt;
                            llwtmp *= 10;
                            llwtmp /= ((SohOcvPtr + i)->Soc - (SohOcvPtr + i - 1)->Soc);

                             target_soc = (uWord)((SohOcvPtr + i - 1)->Soc
                                      + (wtmp_ocv - (SohOcvPtr + i - 1)->Volt) *10 / llwtmp);
                            break ;
                        }
                    }
                }
            }          

            llwtmp = target_soc;

            // calc remain capacity
            if(llwtmp <= 1000)
            {
                remain_ah = (bat_rated_ah * llwtmp) / 1000; 
            }
            else
            {
                remain_ah = 0;
            }
            
            // actual capacity = remain_capacity + discharge_capacity - charge_capacity
            if(bat_total_dch_ah > SOH_CtrlPara.TotalDchAhBackup)
            {
                dch_ah = bat_total_dch_ah - SOH_CtrlPara.TotalDchAhBackup;//discharge capacity calc
                actual_ah = dch_ah + remain_ah;
            }
            else
            {
                actual_ah = 0 + remain_ah;
            }

            if(bat_total_chg_ah > SOH_CtrlPara.TotalChgAhBackup)
            {
                chg_ah = bat_total_chg_ah - SOH_CtrlPara.TotalChgAhBackup;	 //charge capacity calc
                if(actual_ah > chg_ah)
                {
                    actual_ah -= chg_ah;
                }
            }
            // end of calc actual capacity

            // judge whether actual capacity is in a reasonable range
            if( (actual_ah < ((bat_rated_ah * 13) / 10))			// < 1.3 bat_rated_ah
             && (actual_ah > ((bat_rated_ah * 7) / 10) ) )   	// > 0.7 bat_rated_ah
            {
                // calc average value
                if( (SOH_CtrlPara.BatActualAhBackup[0] < ((bat_rated_ah * 13) / 10) )		
                 && (SOH_CtrlPara.BatActualAhBackup[0] > ((bat_rated_ah * 7) / 10) ) )
                {
                    actual_ah += SOH_CtrlPara.BatActualAhBackup[0];
                    actual_ah /= 2;
                }
                
                if( (SOH_CtrlPara.BatActualAhBackup[1] < ((bat_rated_ah * 13) / 10) )		
                 && (SOH_CtrlPara.BatActualAhBackup[1] > ((bat_rated_ah * 7) / 10) ) )
                {
                    actual_ah += SOH_CtrlPara.BatActualAhBackup[1];
                    actual_ah /= 2;
                }
                
                SOH_CtrlPara.BatActualAhBackup[1] = SOH_CtrlPara.BatActualAhBackup[0];
                SOH_CtrlPara.BatActualAhBackup[0] = actual_ah;
                            
                // calc soh value
                if(actual_ah <= bat_rated_ah)
                {
                    soh = (uWord)((actual_ah * 1000) / bat_rated_ah);	// 0.1% unit
                }
                else
                {
                    soh = 1000; // 100%
                }
                
                // re-calc battery actual ah
                bat_actual_ah = bat_rated_ah * soh / k_temp;
                
                Bat_UpdateSoh(soh);
                Bat_UpdateActualCapacityCalc(bat_actual_ah);
                rslt = EE_SaveVar(BOTHEE, SYSTEMST_SOH);
                rslt = EE_SaveVar(BOTHEE, SYSTEMST_RATED_ACTUAL_CAPACITY);
                
                DataStore_EnableEventDataStore(MFY_SOH);
            }           
        }
    }
    
    if(soh_calc_step_backup != SOH_CtrlPara.SohCalcStep)
    {
        rslt = EE_SaveVar(BOTHEE, SYSTEMST_SOH_CTRL_PARA);
    }
    
    // prevent C5917: Removed dead assignment
    (void)rslt;
}

#endif // #ifdef USE_SOH_A