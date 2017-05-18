
#include "TemperIf_Type.h"

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
#include "AiIf.h"
///#include "T18B20DO.h"
//#include "T18B20DI.h"

static GeneralReturnType Temper1_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC1_NO,Adc_Value);
}
static GeneralReturnType Temper2_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC2_NO,Adc_Value);
}
static GeneralReturnType Temper3_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC3_NO,Adc_Value);
}
static GeneralReturnType Temper4_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC4_NO,Adc_Value);
}
static GeneralReturnType Temper5_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC5_NO,Adc_Value);
}
static GeneralReturnType Temper6_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC6_NO,Adc_Value);
}
static GeneralReturnType Temper7_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC7_NO,Adc_Value);
}
static GeneralReturnType Temper8_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC8_NO,Adc_Value);
}

#if (ECU_CATEGORY == M_BCU_C)
static GeneralReturnType Temper9_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC9_NO,Adc_Value);
}
static GeneralReturnType Temper10_Operation(uWord *Adc_Value) 
{
    return AiIf_MeasureChannel(AIIF_CPU_NTC10_NO,Adc_Value);
}
#endif //#if (ECU_CATEGORY == M_BCU_C)

static bool T18B20DI_GetValOperation(void) 
{
//    return T18B20DI_GetVal();
}
   
const sTemperIf_PropertyType TemperIf_Property[TEMPER_MAX_NUM] =  //initial the do property
{ 
  {
     TEMPERIF_NTC1_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC2_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC3_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC4_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC5_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC6_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC7_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC8_NO,
     TEMPERIF_NTC,
  },
#if (ECU_CATEGORY == M_BCU_C)
  {
     TEMPERIF_NTC9_NO,
     TEMPERIF_NTC,
  },
  {
     TEMPERIF_NTC10_NO,
     TEMPERIF_NTC,
  },
#endif //#if (ECU_CATEGORY == M_BCU_C)
  {
     TEMPERIF_18B20_NO,
     T18B20,
  },
};

const sTemperIf_OperationType TemperIf_Operation[TEMPER_MAX_NUM] =  //initial the do operation pointer
{ 
   {
      Temper1_Operation,
      NULL,
      NULL,
   },
   {
      Temper2_Operation,
      NULL,
      NULL,
   },
   {
      Temper3_Operation,
      NULL,
      NULL,
   },
   {
      Temper4_Operation,
      NULL,
      NULL,
   },
   {
      Temper5_Operation,
      NULL,
      NULL,
   },
   {
      Temper6_Operation,
      NULL,
      NULL,
   },
   {
      Temper7_Operation,
      NULL,
      NULL,
   },
   {
      Temper8_Operation,
      NULL,
      NULL,
   },
#if (ECU_CATEGORY == M_BCU_C)
   {
      Temper9_Operation,
      NULL,
      NULL,
   },
   {
      Temper10_Operation,
      NULL,
      NULL,
   },
#endif //#if (ECU_CATEGORY == M_BCU_C)
   {
      NULL,
      NULL,//T18B20DI_GetValOperation,
      NULL,//T18B20DO_PutVal,
   },
};

const sTemperIf_HandleType TemperIf_Handle = 
{
   TemperIf_Property,
   TemperIf_Operation,
};
#endif //#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))



