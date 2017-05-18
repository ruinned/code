
#include "SensorAi.h"
#include "AiIf.h"



#define  VOLT_PROPORTION  11

uWord Sensor_GetAiSt(uByte Ai_id)
{
    uWord Value = 0xffff; 
    uLWord Value_bak = 0; 
    uWord  Value_rslt = 0; 
    
    switch(Ai_id)
    {
    
        case CPU_AI1_ID:
        {
            (void)AiIf_MeasureChannel((uByte)AIIF_CPUAI1_NO,&Value);
            break;
        }
        case CPU_AI2_ID:
        {
            (void)AiIf_MeasureChannel((uByte)AIIF_CPUAI2_NO,&Value);
            break;
        }
        default:
        {
            break;
        }
    }
    if(Value <= CPU_ADC_ACCURACY) 
    {
        Value_bak =  (uLWord)Value;
        Value_rslt = (uWord)(((Value_bak * ADC_VOLT_CRITERION)/ CPU_ADC_ACCURACY) * VOLT_PROPORTION); 
    }

    return Value_rslt;
}