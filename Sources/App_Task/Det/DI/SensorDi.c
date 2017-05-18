
#include "SensorDi.h"
#include "DiIf.h"



eSensor_DiStatusType Sensor_GetDiSt(uByte di_id)
{
    bool Value = 0xff; 
    
    switch(di_id)
    {
    
    case CPU_DI1_ID:
    {
        (void)DiIf_GetValue((uByte)DIIF_CPUDI1_NO,&Value);
        break;
    }
    case CPU_DI2_ID:
    {
        (void)DiIf_GetValue((uByte)DIIF_CPUDI2_NO,&Value);
        break;
    }

    case CPU_DI3_ID:
    {
        (void)DiIf_GetValue((uByte)DIIF_CPUDI3_NO,&Value);
        break;
    }
    case CPU_DI4_ID:
    {
        (void)DiIf_GetValue((uByte)DIIF_CPUDI4_NO,&Value);
        break;
    } 
    default:
    {
        break;
    }

    }
    if(DI_STATUS_LOW == Value)
    {
       return DI_STATUS_LOW;
    }
    else if(DI_STATUS_HIGH == Value)
    {
       return DI_STATUS_HIGH;
    } 
    else 
    {
       return DI_STATUS_ERROR;
    }
}