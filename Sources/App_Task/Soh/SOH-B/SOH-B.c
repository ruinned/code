#include "SOH-B.h"
#include "EE.h"
#include "BatterySt.h"
#include "BmsSysSt.h"
#include "ChargeSt.h"
#include "SchIf.h"
#include "WdgIf.h"
#include "SOC.h"
#include "IntRes-AB.h"

#ifdef USE_SOH_B

//#define SOH_B_DEBUG


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

static void SOH_Calc_Proc(void);
static uWord SOH_GetAHSOH(uLWord DchAH);
static uWord SOH_GetIntResSOH(uWord intres);
/*****************************************************************************/
/* FUNCTION: initial soh calc                                                */
/*                                                                           */
/* DESCRIPTION:                                                              */
/*****************************************************************************/
void SOH_ModuleInit(void)
{
    GeneralReturnType rslt[2];
    uWord soh, soh1, soh2, soh_bak;
    uByte intres;
    uLWord bat_total_chg_ah;
    uLWord bat_total_dch_ah;
    uLWord bat_rated_ah;
    uLWord bat_actual_ah;
    uWord k_temp;

    SOH_TmCnt.cnt = 0;
   
    rslt[0] = EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH);
    rslt[1] = EE_LoadVar(SYSTEMST_SOH);
    
    bat_total_dch_ah = Bat_GetTotalDchAh();
    soh = Bat_GetSoh();
    bat_rated_ah = Bat_GetRatedCapacityCalc();
    bat_actual_ah = Bat_GetActualCapacityCalc();

    
    if(ERR_OK != rslt[0])
    {
        bat_total_chg_ah = 0;
        bat_total_dch_ah = 0;
                
        Bat_UpdateTotalChgAh(bat_total_chg_ah);
        Bat_UpdateTotalDChAh(bat_total_dch_ah);
        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_TOTAL_CHG_DCH_AH);
       
    }

    if((ERR_OK != rslt[1]) || (soh > 1000) || (soh < 600) )
    {
        soh = 1000;
        Bat_UpdateSoh(soh);
        rslt[1] = EE_SaveVar(BOTHEE, SYSTEMST_SOH);
        
    }
    
    // calc temper K
    k_temp = (uWord)((bat_rated_ah * soh) / bat_actual_ah);
    
    soh_bak = soh;
   
    intres = (uByte)Bat_GetAvgIntRes();

   
    if( (intres > 250) || (0 == intres))
    {
         intres = 0;
    }
    
    bat_total_dch_ah /= 1000; // change unit to 1AH
    soh1 = SOH_GetAHSOH(bat_total_dch_ah);
    soh2 = SOH_GetIntResSOH(intres);
    
    soh = soh1;
    if(soh1 > soh2)
    {
        soh = soh2;
    }
    
    if((soh <= 1000) && (soh >= 600) && (soh != soh_bak))
    {           
        Bat_UpdateSoh(soh);
        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_SOH);
      
    }
    
    // re-calc actual ah
    bat_actual_ah = bat_rated_ah * soh / k_temp;
    // bat_actual_ah = bat_rated_ah * soh / 1000;
    
    if(bat_actual_ah != Bat_GetActualCapacityCalc())
    {
        Bat_UpdateActualCapacityCalc(bat_actual_ah);
        rslt[0] = EE_SaveVar(BOTHEE, SYSTEMST_RATED_ACTUAL_CAPACITY);
    }

}

void SOH_ModuleCountCbk(void)
{
    (SOH_TmCnt.cnt < 255) ? (SOH_TmCnt.cnt++) : (SOH_TmCnt.cnt = 255);
}

void SOH_ModuleCbk(void)
{
    if(SOH_TmCnt.cnt >= TM_MS(205))
    {
        SOH_TmCnt.cnt = 0;
        SOH_Calc_Proc();
    }
}
/*****************************************************************************/
/* FUNCTION: process of soh calc                                             */
/*                                                                           */
/* DESCRIPTION: should call this funtion every 500ms                         */
/*****************************************************************************/
static void SOH_Calc_Proc(void)
{
/*    GeneralReturnType rslt[1];
    uWord soh; 
    uWord soh1;
    uWord intres;
    uWord soh2;
    uLWord bat_total_dch_ah;

#ifdef SOH_B_DEBUG
    Bat_UpdateTotalDChAh(2110);
    Bat_UpdateTotalIntRes(2000);
#endif

    bat_total_dch_ah = Bat_GetTotalDchAh();
    intres = Bat_GetTotalIntRes();
    
    if( (bat_total_dch_ah > (SOH_CtrlPara.TotalDchAHBak + 10))
       ||(intres != SOH_CtrlPara.IntResBak))
    {
          if(intres != SOH_CtrlPara.IntResBak)
          {
              SOH_CtrlPara.IntResBak = intres;
          }
          if(bat_total_dch_ah > (SOH_CtrlPara.TotalDchAHBak + 10))
          {
              SOH_CtrlPara.TotalDchAHBak = bat_total_dch_ah;
          }          
          soh1 = SOH_GetAHSOH(bat_total_dch_ah);
          soh2 = SOH_GetIntResSOH(intres);
          soh = soh1;
          if(soh1 > soh2)
          {
              soh = soh2;
          }
          if((soh <= 1000) && (soh >= 600) && (soh != SOH_CtrlPara.SohBak))
          {
              SOH_CtrlPara.SohBak = soh;
              Bat_UpdateSoh(soh);
              rslt[0] = EE_SaveVarForCalib(BOTHEE, SYSTEMST_SOH);
          }
    }
*/
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static uWord SOH_GetIntResSOH(uWord intres)
{
    uByte i;
    uLWord soh;
    uLWord lwtmp;
    uLWord lwtmp2;
    const sSOH_IntResType *SohIntRPtr;
    
    SohIntRPtr = SOH_IntRes;
    
    soh = 1000;
    if(intres >= (SohIntRPtr + SOH_Macro.Soh_Max_Intres_Point - 1)->IntRes)
    {
        soh = (SohIntRPtr + SOH_Macro.Soh_Max_Intres_Point - 1)->Soh; 
    }
    else if(intres <= SohIntRPtr->IntRes)
    {
       soh = SohIntRPtr->Soh; 
    }
    else //  soh between 70%-100%
    {
        for (i = (SOH_Macro.Soh_Max_Intres_Point - 1); i > 0; i--)
        {
            if(intres == (SohIntRPtr +i - 1)->IntRes)
            {
                soh = (SohIntRPtr + i - 1)->Soh;
                break;
            }
            else
            {
                if(intres > (SohIntRPtr +i - 1)->IntRes)
                {
                    lwtmp = ((SohIntRPtr + i)->IntRes - (SohIntRPtr + i - 1)->IntRes);
                    lwtmp *= 1000;
                    lwtmp /= ((SohIntRPtr + i - 1)->Soh - (SohIntRPtr + i)->Soh);
                    
                    lwtmp2 = (SohIntRPtr + i)->IntRes - intres;
                    
                    soh = (uWord)(lwtmp2 * 1000 / lwtmp);
                    soh += (SohIntRPtr + i)->Soh;

                    break ;
                }
            }
        }
    }
    return (uWord)soh;
}
//************************************************************/
// Function:
// Trans para:
//************************************************************/
static uWord SOH_GetAHSOH(uLWord DchAH)
{
    uByte i;
    uLWord soh;
    uLWord lwtmp;
    uLWord lwtmp2;

    const sSOH_TotalDchAhType *SohDchAhPtr;
    SohDchAhPtr = SOH_TotalDchAh;
    
    soh = 1000;
    if(DchAH >= (SohDchAhPtr + SOH_Macro.Soh_Max_Ah_Point - 1)->TotalDchAh)
    {
        soh = (SohDchAhPtr + SOH_Macro.Soh_Max_Ah_Point - 1)->Soh; 
    }
    else if(DchAH <= SohDchAhPtr->TotalDchAh)
    {
        //soh = SOH_TotalDchAh[0].Soh; 
        soh = SohDchAhPtr->Soh;
    }
    else //  soh between 70%-100%
    {
        for (i = (SOH_Macro.Soh_Max_Ah_Point - 1); i > 0; i--)
        {
            if(DchAH == (SohDchAhPtr + i - 1)->TotalDchAh)
            {
                soh = (SohDchAhPtr + i - 1)->Soh;
                break;
            }
            else
            {
                if(DchAH > (SohDchAhPtr + i - 1)->TotalDchAh)
                {
                    lwtmp = ((SohDchAhPtr + i)->TotalDchAh - (SohDchAhPtr + i - 1)->TotalDchAh);
                    lwtmp *= 100;
                    lwtmp /= ((SohDchAhPtr + i - 1)->Soh - (SohDchAhPtr + i)->Soh);
                    
                   lwtmp2 = (SohDchAhPtr + i)->TotalDchAh - DchAH;
                    
                    soh = (uWord)(lwtmp2 * 100 / lwtmp);
                    soh += (SohDchAhPtr + i)->Soh;

                    break ;
                }
            }
        }
    }
    return (uWord)soh;
}
#endif // #ifdef USE_SOH_B