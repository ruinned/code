
#ifndef TEMPERIF_TYPE_H
#define TEMPERIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))

#if (ECU_CATEGORY == M_BCU_C)
#define TEMPER_MAX_NUM  11
#endif

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
#define TEMPER_MAX_NUM  9
#endif

typedef enum 
{
   ERR_TEMPER_RANGE,
   ERR_TEMPER_FUNCTION_PTR_NULL,
   ERR_TEMPER_ID_NUM_NOTMATCH,
   ERR_TEMPER_OPERATION_PTR_NULL,
   ERR_TEMPER_PROPERTY_PTR_NULL,
   ERR_TEMPER_TYPE_NOTMATCH,
}eTemperIf_ReturnType;

typedef enum 
{
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
   TEMPERIF_NTC1_NO,
   TEMPERIF_NTC2_NO,
   TEMPERIF_NTC3_NO,
   TEMPERIF_NTC4_NO,
   TEMPERIF_NTC5_NO,
   TEMPERIF_NTC6_NO,
   TEMPERIF_NTC7_NO,
   TEMPERIF_NTC8_NO,
#if (ECU_CATEGORY == M_BCU_C)
   TEMPERIF_NTC9_NO,
   TEMPERIF_NTC10_NO,
#endif  //#if (ECU_CATEGORY == M_BCU_C)
   TEMPERIF_18B20_NO,
#endif  //#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
}eTemperIf_IdNumType; 
 

typedef enum 
{
   TEMPERIF_NTC,   
   T18B20,   
}eTemperIf_SensorTypeType;

typedef GeneralReturnType (* TemperIf_NtcGetAdcValuePtrType)(uWord *Adc_Value);
typedef bool (* TemperIf_18B20GetValPtrType)(void);
typedef void (* TemperIf_18B20PutValPtrType)(bool Val);

typedef struct 
{
    const eTemperIf_IdNumType TemperIf_IdNum;  //the property of temper number
    const eTemperIf_SensorTypeType TemperIf_SensorType;         //the property of temper type
}sTemperIf_PropertyType;  //temper property

typedef struct 
{
    TemperIf_NtcGetAdcValuePtrType  TemperIf_NtcGetAdcValuePtr; //the pointer of function to operate temper
    TemperIf_18B20GetValPtrType TemperIf_18B20GetValPtr;
    TemperIf_18B20PutValPtrType TemperIf_18B20PutValPtr;
}sTemperIf_OperationType;

typedef struct 
{
    const sTemperIf_PropertyType *TemperIf_PropertyPtr;  //the pointer of property
    const sTemperIf_OperationType *TemperIf_OperationPtr;  //the pointer of operation
}sTemperIf_HandleType;

#endif //#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
#endif 
 

