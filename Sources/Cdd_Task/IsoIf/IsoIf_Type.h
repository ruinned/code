#ifndef ISOIF_TYPE_H
#define ISOIF_TYPE_H

#include "GeneralTypeDef.h"



typedef void (* Iso_DoCtrlPtrType)(bool Val);

typedef GeneralReturnType (* Iso_GetAdValPtrType)(uWord *Ai_AdcValue);

typedef enum 
{
    ISO1,
}eIso_IdNumType;

typedef enum 
{
   ERR_ISO_RANGE,                /* the ISO number out of range */
   ERR_ISO_VALUE_PTR_NULL,
   ERR_ISO_PROPERTY_PTR_NULL,  /*  the pointer of property is null */
   ERR_ISO_OPERATION_PTR_NULL, /*  the pointer of operation is null */
   ERR_ISO_ID_NUM_NOTMATCH,    /*  the id number is not match */
   ERR_ISO_FUNCTION_PTR_NULL,     /*  the pointer of putval is null */ 
}eIso_ReturnType;

typedef struct
  {
     eIso_IdNumType  Iso_IdNum;
  }sIso_PropertyType;

typedef struct
  {
    Iso_DoCtrlPtrType Iso_DoCtrlPosPtr;
    Iso_DoCtrlPtrType Iso_DoCtrlNegPtr; 
    Iso_DoCtrlPtrType Ios_DoCtrlPowPtr;
    Iso_GetAdValPtrType  Iso_GetAdValPosPtr;
    Iso_GetAdValPtrType  Iso_GetAdValNegPtr;
    
  }sIso_OperationType;
  

typedef struct 
  {
     const sIso_PropertyType *Iso_PropertyPtr;
     const sIso_OperationType *Iso_OperationPtr;
  }sIso_HandleType;
  
  typedef struct
{
    uByte gain;
    uByte offset;
}
sIso_InitConfigType;
  
#endif   /* ISOIF_TYPE_H */
    



