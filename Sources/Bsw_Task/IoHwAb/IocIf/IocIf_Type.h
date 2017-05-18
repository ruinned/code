

#ifndef IOCIF_TYPE_H
#define IOCIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))

#define  IOCIF_MAX_NUM  1

typedef enum 
{
   IOCIF_AC_CP_NO,
}eIocIf_IdNumType; 


typedef uByte (* IocIf_EnablePtrType)(void);
typedef uByte (* IocIf_DisablePtrType)(void);
typedef uByte (* IocIf_EnableEventPtrType)(void);
typedef uByte (* IocIf_DisableEventPtrType)(void);
typedef uByte (* IocIf_ResetPtrType)(void);
typedef uByte (* IocIf_GetCaptureValuePtrType)(uWord *ValuePtr);
typedef void (* IocIf_RisingTriggerPtrType)(void);
typedef void (* IocIf_FallingTriggerPtrType)(void);

typedef enum 
{
  IOCIF_NORMAL_LOGIC_TYPE,
  IOCIF_ABNORMAL_LOGIC_TYPE
}eIocIf_LogicTypeType;

typedef struct 
{
    const eIocIf_IdNumType IocIf_IdNum;  //the property of do number
    const eIocIf_LogicTypeType IocIf_Logic;
}sIocIf_PropertyType;  //do property

typedef struct 
{
    IocIf_EnablePtrType  IocIf_EnablePtr; //the pointer of function to operate DO
    IocIf_DisablePtrType IocIf_DisablePtr;
    IocIf_EnableEventPtrType IocIf_EnableEventPtr;
    IocIf_DisableEventPtrType IocIf_DisableEventPtr;
    IocIf_ResetPtrType IocIf_ResetPtr;
    IocIf_GetCaptureValuePtrType IocIf_GetCaptureValuePtr;
    IocIf_RisingTriggerPtrType IocIf_RisingTriggerPtr;
    IocIf_FallingTriggerPtrType IocIf_FallingTriggerPtr;
}sIocIf_OperationType;

typedef struct 
{
    const sIocIf_PropertyType *IocIf_PropertyPtr;  //the pointer of property
    const sIocIf_OperationType *IocIf_OperationPtr;  //the pointer of operation
}sIocIf_HandleType;
#endif

#endif 
 

