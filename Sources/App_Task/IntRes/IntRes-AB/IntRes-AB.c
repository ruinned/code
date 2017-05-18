#include "IntRes-AB.h"
#include "SchIf.h"
#include "SysConf.h"
#include "BatterySt.h"

#ifdef USE_INTRES_AB


typedef struct
{
    uByte cnt;
} sIntRes_TmCntType;

sIntRes_TmCntType IntRes_TmCnt;

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
    uByte  CalcStep;
    uByte  TimeCnt1;
    uWord  TimeCnt2;
    uWord  LowCur;
    uWord  HighCur;
    uWord  LowSumV;
    uWord  HighSumV;
    uWord  LowCellV[MAX_CELL_NUM];
    uWord  HighCellV[MAX_CELL_NUM];
} sIntRes_CalcType;

sIntRes_CalcType IntRes_Calc;

typedef struct
{
    uWord Sum_Intres_T1[5]; // 0.1 m次, t <= 10'C
    uWord Sum_Intres_T2[5]; // 0.1 m次, t > 10'C

    uByte Max_Intres_T1;   // 0.1 m次, t <= 10'C
    uByte Min_Intres_T1;   // 0.1 m次, t <= 10'C
    uByte Avg_Intres_T1;   // 0.1 m次, t <= 10'C

    uByte Max_Intres_T2;   // 0.1 m次, t > 10'C
    uByte Min_Intres_T2;   // 0.1 m次, t > 10'C
    uByte Avg_Intres_T2;   // 0.1 m次, t > 10'C
} sIntRes_InforType;

sIntRes_InforType IntRes_Infor;

#pragma DATA_SEG DEFAULT

static void IntRes_CalcProc(void);

void IntRes_ModuleInit(void)
{
    uWord i;
    uByte * RAMPTR p_data;

    IntRes_TmCnt.cnt = 0;

    p_data = (uByte* RAMPTR)(&IntRes_Calc);
    
    for(i = 0; i < sizeof(IntRes_Calc); i++)
    {
        *p_data++ = 0;
    }
}

void IntRes_ModuleCountCbk(void)
{
    (IntRes_TmCnt.cnt < 255) ? (IntRes_TmCnt.cnt++) : (IntRes_TmCnt.cnt = 255);
}

void IntRes_ModuleCbk(void)
{
    if(IntRes_TmCnt.cnt >= TM_MS(20))
    {
        IntRes_TmCnt.cnt = 0;
        IntRes_CalcProc();
    }
}

//************************************************************/
// Function: process of calc intRes
// Trans para: void
// call this function every 20ms
//************************************************************/
static void IntRes_CalcProc(void)
{
    uByte i;
    uByte segment;
    uByte bmu_num;
    uByte dlen;
    uWord j, k;
    uWord current;
    uWord sumvolt;
    uWord wtmp;
    uWord intres;
    uLWord lwtmp;
    GeneralReturnType rslt;
#ifdef INTRES_AB_DEBUG
    Bat_UpdateCurrent(INSCUR,30000);
    Bat_UpdateSumV(INT_SUMV,3120);
    wtmp = 3200;
    for(i = 1; i < 37; i++)
    {      
        Bat_UpdateCellV(1,i,wtmp);
        wtmp += 10;
    }
    
#endif
    i = 0;
    current = Bat_GetCurrent(INSCUR);
    sumvolt = Bat_GetSumV(INT_SUMV);
    
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);

    if(STEP_WAIT_LOW_CUR == IntRes_Calc.CalcStep)
    {
        // wait low cuurent, get initial value of min current and max volt
        if( (current < INTRES_DCHCUR_8A) && (current > INTRES_CHGCUR_1A) )
        {
            if(IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_5S)
            {
                IntRes_Calc.TimeCnt2++;

                if(INTRES_TIMECNT_1S == IntRes_Calc.TimeCnt2)
                {
                    IntRes_Calc.LowCur = current;
                    IntRes_Calc.HighSumV = sumvolt;

                    k = 0;
                    for(i = 1; i < (bmu_num + 1); i++)
                    {
                        wtmp = Bat_GetPackSerialNum(i);
                        for(j = 0; j < wtmp; j++)
                        {
                            IntRes_Calc.HighCellV[k++] = Bat_GetCellV(i, j + 1);
                        }
                    }
                }

                // continually update value of min current and max volt in 5S
                if(IntRes_Calc.TimeCnt2 > INTRES_TIMECNT_1S)
                {
                    if(current < IntRes_Calc.LowCur)
                    {
                        IntRes_Calc.LowCur = current;
                    }

                    if(sumvolt > IntRes_Calc.HighSumV)
                    {
                        IntRes_Calc.HighSumV = sumvolt;
                    }

                    k = 0;
                    for(i = 1; i < (bmu_num + 1); i++)
                    {
                        wtmp = Bat_GetPackSerialNum(i);
                        for(j = 0; j < wtmp; j++)
                        {
                            if(Bat_GetCellV(i, j + 1) > IntRes_Calc.HighCellV[k])
                            {
                                IntRes_Calc.HighCellV[k] = Bat_GetCellV(i, j + 1);
                            }
                            k++;
                        }
                    }
                }
            }
            else	// if appear high current in 5S, then give up this calculation
            {
                IntRes_Calc.TimeCnt2 = 0;
                IntRes_Calc.CalcStep = STEP_WAIT_HIGH_CUR;
            }
        }
        else  // do not has low current
        {
            IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
            IntRes_Calc.TimeCnt2 = 0;
        }
    }
    // wait high cuurent, get the value of max current and min volt
    else if(STEP_WAIT_HIGH_CUR == IntRes_Calc.CalcStep)
    {
        // continually update value of min current and max volt
        if( (current < INTRES_DCHCUR_8A) && (current > INTRES_CHGCUR_1A) )
        {
            IntRes_Calc.TimeCnt2 = 0;
            IntRes_Calc.TimeCnt1 = 0;

            if(current < IntRes_Calc.LowCur)
            {
                IntRes_Calc.LowCur = current;
            }

            if(sumvolt > IntRes_Calc.HighSumV)
            {
                IntRes_Calc.HighSumV = sumvolt;
            }

            k = 0;

            for(i = 1; i < (bmu_num + 1); i++)
            {
                wtmp = Bat_GetPackSerialNum(i);
                for(j = 0; j < wtmp; j++)
                {
                    if(Bat_GetCellV(i, j + 1) > IntRes_Calc.HighCellV[k])
                    {
                        IntRes_Calc.HighCellV[k] = Bat_GetCellV(i, j + 1);
                    }
                    k++;
                }
            }
        }
        else if(current > INTRES_DCHCUR_60A)
        {
            // appear high current and last for 1s, get value of max current and min volt
            if(IntRes_Calc.TimeCnt1 < INTRES_TIMECNT_1S)
            {
                IntRes_Calc.TimeCnt1++;
            }
            else
            {
                IntRes_Calc.TimeCnt2 = 0;
                IntRes_Calc.TimeCnt1 = 0;
                IntRes_Calc.CalcStep = STEP_WAIT_CALC_INTRES;

                IntRes_Calc.HighCur = current;
                IntRes_Calc.LowSumV = sumvolt;

                k = 0;

                for(i = 1; i < (bmu_num + 1); i++)
                {
                    wtmp = Bat_GetPackSerialNum(i);
                    for(j = 0; j < wtmp; j++)
                    {
                        IntRes_Calc.LowCellV[k++] = Bat_GetCellV(i, j + 1);
                    }
                }
            }
        }
        else// current not high enough
        {
            IntRes_Calc.TimeCnt1 = 0;

            // current can rise high in 5s
            if(IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_5S)
            {
                IntRes_Calc.TimeCnt2++;
            }
            else
            {
                IntRes_Calc.TimeCnt2 = 0;
                IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
            }
        }
    }
    else if(STEP_WAIT_CALC_INTRES == IntRes_Calc.CalcStep)
    {
        if(current > INTRES_DCHCUR_60A) // appear high current
        {
            // continually update value of max current and min volt in 10s
            if(IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_11S)
            {
                IntRes_Calc.TimeCnt2++;

                if(IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_10S)	// ??
                {
                    if(current > IntRes_Calc.HighCur)
                    {
                        IntRes_Calc.HighCur = current;
                    }
                    if(sumvolt < IntRes_Calc.LowSumV)
                    {
                        IntRes_Calc.LowSumV = sumvolt;
                    }

                    k = 0;

                    for(i = 1; i < (bmu_num + 1); i++)
                    {
                        wtmp = Bat_GetPackSerialNum(i);
                        for(j = 0; j < wtmp; j++)
                        {
                            if(Bat_GetCellV(i, j + 1) < IntRes_Calc.LowCellV[k])
                            {
                                IntRes_Calc.LowCellV[k] = Bat_GetCellV(i, j + 1);
                            }
                            k++;
                        }
                    }
                }
            }
            else //	high current last too long, then give up this calculation
            {
                IntRes_Calc.TimeCnt2 = 0;
                IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
            }
        }
        else if(current < INTRES_DCHCUR_8A) // current appear peak value, begin to calc intres
        {
            if((IntRes_Calc.TimeCnt2 > INTRES_TIMECNT_2S) && (IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_11S) )
            {
                // begin to calc intres
                if(Bat_GetSOC() < 1000)
                {
                    segment = (uByte)(Bat_GetSOC() / 200);
                }
                else
                {
                    segment = 4;
                }
                
                lwtmp = 0;
                if(IntRes_Calc.HighCur > IntRes_Calc.LowCur)
                {
                    if(IntRes_Calc.HighSumV > IntRes_Calc.LowSumV)
                    {
                        lwtmp = IntRes_Calc.HighSumV - IntRes_Calc.LowSumV;
                        lwtmp *= 10000;
                        
                        intres = (uWord)(lwtmp / (IntRes_Calc.HighCur - IntRes_Calc.LowCur));		//0.1m次 unit

                        if(Bat_GetAvgT() <= INTRES_T_10C)
                        {
                            IntRes_Infor.Sum_Intres_T1[segment] = intres;    
                        }
                        else
                        {
                            IntRes_Infor.Sum_Intres_T2[segment] = intres;
                        }
                        
                        // updata  sum_res, need modify
                        (void)Bat_UpdateTotalIntRes(intres);
                    }

                    
                    k = 0;

                    for(i = 1; i < (bmu_num + 1); i++)
                    {
                        wtmp = Bat_GetPackSerialNum(i);
                        for(j = 0; j < wtmp; j++)
                        {
                            if(IntRes_Calc.HighCellV[k] > IntRes_Calc.LowCellV[k])
                            {
                                lwtmp = IntRes_Calc.HighCellV[k]	-	IntRes_Calc.LowCellV[k];
                                lwtmp *= 1000;
                                intres = (uWord)(lwtmp / (IntRes_Calc.HighCur - IntRes_Calc.LowCur));		//0.01m次 unit

#ifdef BMS_ENABLE_CALC_INTRES
                                if(Bat_GetAvgT() <= BALM_T_10C)
                                {
                                    //updata cell res, need modify
                                    (void)Bat_UpdateCellR(i, (j + 1), intres);
                                }
                                else
                                {
                                    //updata
                                }
#endif                                
                            }
                            k++;
                        }
                    }                   
                }
            }

            // calc next once
            IntRes_Calc.TimeCnt2 = 0;
            IntRes_Calc.TimeCnt1 = 0;
            IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
        }
        else // current not low enough
        {
            // current can down low in 5s
            // ?
            if( (IntRes_Calc.TimeCnt2 > INTRES_TIMECNT_4S) && (IntRes_Calc.TimeCnt2 < INTRES_TIMECNT_11S) )			//>60A 4s - 12s
            {
                if(IntRes_Calc.TimeCnt1 < INTRES_TIMECNT_5S)				//<5s
                {
                    IntRes_Calc.TimeCnt1++;
                }
                else
                {
                    IntRes_Calc.TimeCnt2 = 0;
                    IntRes_Calc.TimeCnt1 = 0;
                    IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
                }
            }
            else // ?
            {
                IntRes_Calc.TimeCnt2 = 0;					//anew  calc
                IntRes_Calc.TimeCnt1 = 0;
                IntRes_Calc.CalcStep = STEP_WAIT_LOW_CUR;
            }
        }
    }
    (void)rslt;
}

#endif // #ifdef USE_INTRES_AB