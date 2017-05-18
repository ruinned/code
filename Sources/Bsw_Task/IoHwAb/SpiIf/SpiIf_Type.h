#ifndef SPIIF_TYPE_H
#define SPIIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#define SPIIF_MAX_NUM  1

typedef enum 
{
   SPIIF_TLE8102_NO,
}eSpiIf_IdNumType;

typedef struct 
{
    const eSpiIf_IdNumType SpiIf_IdNum;
}sSpiIf_PropertyType;

typedef void (* SpiIf_IoClrValPtrType)(void);
typedef void (* SpiIf_IoSetValPtrType)(void);
typedef bool (* SpiIf_IoGetValPtrType)(void);
typedef struct 
{
    SpiIf_IoClrValPtrType  SpiIf_IoClrValPtr;
    SpiIf_IoSetValPtrType  SpiIf_IoSetValPtr;
}sSpiIf_IoOutPutOperationType; //for IO output operation

typedef struct 
{
    SpiIf_IoGetValPtrType  SpiIf_IoGetValPtr;
}sSpiIf_IoInputOperationType;//for IO input operation
 
typedef struct 
{
    /* the IO pin of CS for SPI module */
    sSpiIf_IoOutPutOperationType  SpiIf_IoOutPutOperationCs;
    /* the IO pin of SCK for SPI module */
    sSpiIf_IoOutPutOperationType  SpiIf_IoOutPutOperationSck;
    /* the IO pin of SI for SPI module */
    sSpiIf_IoOutPutOperationType  SpiIf_IoOutPutOperationSi;
    /* the IO pin of SO for SPI module */
    sSpiIf_IoInputOperationType   SpiIf_IoInPutOperationSo;
}sSpiIf_OperationType; //for SPI operation

typedef struct 
{
    const sSpiIf_PropertyType *SpiIf_PropertyPtr;      //the pointer of property
    const sSpiIf_OperationType *SpiIf_OperationPtr;   //the pointer of operation
}sSpiIf_HandleType;

#endif 
 

