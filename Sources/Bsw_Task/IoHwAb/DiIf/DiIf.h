#ifndef DIIF_H
#define DIIF_H

//#include "Di_Lcfg.h"
#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#include "PORTA0.h"
#include "PORTA1.h"
#include "CPUDI1.h"
#include "CPUDI2.h"
#include "CPUDI3.h"
#include "CPUDI4.h"
//#include "OV_Power.h"
//#include "UV_Power.h"
#include "PORTA2.h"


#define  DIIF_MAX_NUM  9
typedef enum 
{
   DIIF_PORTA0_NO,
   DIIF_PORTA1_NO,
   DIIF_CPUDI1_NO,
   DIIF_CPUDI2_NO, 

   DIIF_CPUDI3_NO,
   DIIF_CPUDI4_NO,
   DIIF_OV_POWER_NO,
   DIIF_UV_POWER_NO,
   DIIF_PORTA2_NO,

}eDiIf_IdNumType;

typedef enum 
{
   DIIF_VALUE_LOW,
   DIIF_VALUE_HIGH
}eDiIf_ValueType;

GeneralReturnType DiIf_GetValue(uByte DiIf_Id,bool *DiIf_ValuePtr);

#endif /* DIIF_H */