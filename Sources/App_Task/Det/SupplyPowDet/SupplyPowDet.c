// SupplyPowDet.c

#include "SupplyPowDet.h"
#include "AiIf.h"
#include "BmsSysSt.h"
#include "SchIf.h"

#define VOLT_GAIN   110


#define REF_VOLT    500


typedef struct
{
    uByte cnt;
} sSupplyPowDet_TmCntType;

sSupplyPowDet_TmCntType SupplyPowDet_TmCnt;

uByte gby_DetStep;

void SupplyPowDet_Init(void)
{
    SupplyPowDet_TmCnt.cnt = 0;
    gby_DetStep = 0;
}

void SupplyPowDet_CountCbk(void)
{
    (SupplyPowDet_TmCnt.cnt < 255) ? (SupplyPowDet_TmCnt.cnt++) : (SupplyPowDet_TmCnt.cnt = 255);
}

void SupplyPowDet_ProcMain(void)
{
    GeneralReturnType return_rslt;
    uByte i;
    uWord max,min;
    uLWord result = 0;
    uWord adc_value[5];

    if(SupplyPowDet_TmCnt.cnt >= TM_MS(5))
    {
        SupplyPowDet_TmCnt.cnt = 0;

        if(0 == gby_DetStep)
        {
            gby_DetStep++;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_POW_VOLT_NO, &adc_value[i]);
                result += adc_value[i];
            }
            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateK30Volt((uWord)(result + 4));// D17s drop voltage 0.4V
        }
        else if(1 == gby_DetStep)
        {

            gby_DetStep++;
            result = 0;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_CPUAI_K15_NO, &adc_value[i]);
                result += adc_value[i];
            }
            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateK15Volt((uWord)result);

        }
        else if (2 == gby_DetStep)
        {

            gby_DetStep++;
            result = 0;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_CPUAI_OFFBD_CHG_P_NO, &adc_value[i]);
                result += adc_value[i];
            }
            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateOffBDChgVolt((uWord)result);

        }
        else if(3 == gby_DetStep)
        {
            gby_DetStep++;
            result = 0;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_CPUAI_ONBD_CHG_P_NO, &adc_value[i]);
                result += adc_value[i];
            }
            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateOnBDChgVolt((uWord)result);
        }
        else if(4 == gby_DetStep)
        {
            gby_DetStep++;
            result = 0;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_CPUAI_ALARM_NO, &adc_value[i]);
                result += adc_value[i];
            }
            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateAlarmVolt((uWord)result);
        }
        else if(5 == gby_DetStep)
        {
            gby_DetStep = 0;
            result = 0;
            for(i = 0; i < 5; i++)
            {
                return_rslt = AiIf_MeasureChannel(AIIF_CPUAI_CAN_INH_NO, &adc_value[i]);
                result += adc_value[i];
            }

            max = adc_value[0];
            min = adc_value[0];

            for(i = 1; i < 5; i++)
            {
                if(max < adc_value[i])
                {
                    max = adc_value[i];
                }
                if(min > adc_value[i])
                {
                    min = adc_value[i];
                }
            }
            result = result - max - min;
            result /= 3;
            result *= REF_VOLT;
            result *= VOLT_GAIN;
            result /= CPU_ADC_ACCURACY;
            result /= 100;
            BmsSys_UpdateCanInhVolt((uWord)result);
        }
        else
        {
            gby_DetStep = 0;
        }        
    }
}

