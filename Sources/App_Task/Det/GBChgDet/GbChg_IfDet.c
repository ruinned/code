#include "GbChg_IfDet.h"
#include "AiIf.h"
#include "IocIf.h"
#include "ChargeSt.h"

#ifdef ECU_M_ENABLE_XGATE
#include"Xgate_AcCpIoc.h"
#endif


#define HZ_1K         1000
#define PERSENT_20    (HZ_1K * 20 / 100)
#define FILTER_PERIOD_CNT 100

typedef enum
{
    GBCHGDET_ERR_OK,
    GBCHGDET_AI_UNNORMAT,
    GBCHGDET_DUTY_UNNORMAT
} GbChgDet_ReturnType;

typedef struct
{
    uByte Duty;
    uWord Period;
    uByte FilterCnt;
}GbChgDet_CpType;

GbChgDet_CpType GbChgDet_Cp; 


uWord Gb_ADConver(eAiIf_ChannelIdNumType Ch)
{
    uWord  rslt = 0;
    uWord  result[6];
    uWord  max,min;
    uLWord temp_result;
    uByte i;

    max = 0;
    min = 0xffff;

    for(i = 0; i < 6; i++)
    {
        (void)AiIf_MeasureChannel((uByte)Ch, &result[i]);

        rslt += result[i];

        if(max < result[i])
        {
            max = result[i];
        }
        if(min > result[i])
        {
            min = result[i];
        }
    }


    rslt = rslt - max - min;
    rslt >>= 2;  // ad value;

    temp_result = (uLWord)(rslt);
    temp_result *= ADC_VOLT_CRITERION;
    rslt = (uWord)(temp_result / CPU_ADC_ACCURACY);  /* 0.001V */

    return rslt;
}



void GbChgDet_Ai(eAiIf_ChannelIdNumType Ch, uWord VoltGain, uWord *VoltPtr)
{

    uLWord llwtmp;
    uWord lwd_volt;
    uWord offset;
    uByte gain;
    (void)VoltGain;

    llwtmp = (uLWord)Gb_ADConver(Ch);

    if(llwtmp >= 732) // >9V
    {
        offset = 8210;
        gain = 111;
        llwtmp *= gain;
        llwtmp += offset;
    }
    else if(llwtmp >= 496) // >6V
    {
        offset = 3050;
        gain = 127;
        llwtmp *= gain;
        llwtmp -= offset;
    }
    else if(llwtmp >= 296) // >4.8V
    {
        offset = 2990;
        gain = 127;
        llwtmp *= gain;
        llwtmp -= offset;
    }
    else
    {
        offset = 380;
        gain = 118;
        llwtmp *= gain;
        llwtmp += offset;
    }

    llwtmp /= 1000;

    lwd_volt = (uWord)(llwtmp); // 0.1V unit

    *VoltPtr = lwd_volt;

}




void GbChgDet_DutyCalc(uByte *DutyPtr)
{
#ifdef ECU_M_ENABLE_XGATE
    uByte duty;
    Xgate_CpuGetAcCp(&GbChgDet_Cp.Period, &duty);
    *DutyPtr = duty;
#else 

    if(ERR_OK == IocIf_GetIocValue(IOCIF_AC_CP_NO, &GbChgDet_Cp.Period, &GbChgDet_Cp.Duty))
    {
        if((GbChgDet_Cp.Period < (HZ_1K + PERSENT_20)) && (GbChgDet_Cp.Period > (HZ_1K - PERSENT_20)))
        {
            GbChgDet_Cp.FilterCnt = 0;
        }
        else
        {
            GbChgDet_Cp.FilterCnt++;
        }
    }
    else
    {
        GbChgDet_Cp.FilterCnt++;
    }
    
    if(GbChgDet_Cp.FilterCnt >= FILTER_PERIOD_CNT)
    {
        GbChgDet_Cp.FilterCnt = FILTER_PERIOD_CNT;
        GbChgDet_Cp.Duty = 0;
        GbChgDet_Cp.Period = 0;
    }
    
    *DutyPtr = GbChgDet_Cp.Duty;
#endif
    Chg_UpdateCpPeriod(GbChgDet_Cp.Period);


}