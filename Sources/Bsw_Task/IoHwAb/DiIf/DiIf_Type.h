
#ifndef DIIF_TYPE_H
#define DIIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

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

/*typedef enum 
{
   DI_DIRECTION_INPUT,
   DI_DIRECTION_OUTPUT
}eDi_DirectionType; */

typedef enum 
{
  DIIF_NORMAL_LOGIC_TYPE,
  DIIF_ABNORMAL_LOGIC_TYPE
}eDiIf_LogicTypeType;

typedef enum 
{
   DIIF_VALUE_LOW,
   DIIF_VALUE_HIGH
}eDiIf_ValueType;


typedef bool (* DiIf_GetValPtrtype)(void);

typedef struct 
{
    const eDiIf_IdNumType DiIf_IdNum;    //the property of di number
    const eDiIf_LogicTypeType DiIf_LogicType; //the logic type
    eDiIf_ValueType DiIf_Value;       //store the value of DI
}sDiIf_PropertyType; //di property

typedef struct 
{
    DiIf_GetValPtrtype     DiIf_GetValPtr;  //the pointer of function to operate DI
}sDiIf_OperationType;

typedef struct 
{
    sDiIf_PropertyType *DiIf_PropertyPtr; //the pointer of property
    const sDiIf_OperationType *DiIf_OperationPtr; //the pointer of operation
}sDiIf_HandleType;

#endif 
 

