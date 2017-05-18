/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_CfgTypes.h>
 *  @brief      <Dem_CfgTypes>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.23  - AM 10:31:27>
 */
/*============================================================================*/

#ifndef DEM_CFGTYPES_H_
#define DEM_CFGTYPES_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.23     wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_CFGTYPES_H_MODULE_ID            54U
#define DEM_CFGTYPES_H_VENDOR_ID          0U
#define DEM_CFGTYPES_H_AR_MAJOR_VERSION   3U
#define DEM_CFGTYPES_H_AR_MINOR_VERSION   1U
#define DEM_CFGTYPES_H_AR_PATCH_VERSION   0U
#define DEM_CFGTYPES_H_SW_MAJOR_VERSION   1U
#define DEM_CFGTYPES_H_SW_MINOR_VERSION   0U
#define DEM_CFGTYPES_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Types.h"
#include "Std_ExtendedTypes.h"

/*******************************************************************************
*                          General Configuration
*******************************************************************************/
typedef Std_ReturnType (*Dem_ReadDataFncType)(uint8*UDS_FAR Buffer, uint16 BufSize);

/*******************************************************************************
*                          Condition Configuration
*******************************************************************************/
/* DemGeneral/DemOperationCycle 1-256 */
typedef struct
{
    /* DemOperationCycleAutomaticEnd Range: true or false */
    const boolean DemOperationCycleAutomaticEnd;

    /* DemOperationCycleAutostart Range: true or false */
    const boolean DemOperationCycleAutostart;

    /* DemOperationCycleType type:
    Enum{
    DEM_OPCYC_IGNITION
    DEM_OPCYC_OBD_DCY
    DEM_OPCYC_OTHER
    DEM_OPCYC_POWER
    DEM_OPCYC_TIME
    DEM_OPCYC_WARMUP
    } */
    const uint8 DemOperationCycleType;
}Dem_OperationCycleType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_OperationCycleType,AUTOMATIC) DemOperationCycle[DEM_OPERATION_CYCLE_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


#if(DEM_ENABLE_CONDITION_NUM > 0)
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemEnableCondition 0-255 */
extern CONST(uint8,AUTOMATIC) DemEnableCondition[DEM_ENABLE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_ENABLE_CONDITION_GROUP_NUM > 0)
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemEnableConditionGroup 0-255 */
extern CONST(uint8,AUTOMATIC) DemEnableConditionGroup[DEM_ENABLE_CONDITION_GROUP_NUM][DEM_ENABLE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif


#if(DEM_STORAGE_CONDITION_NUM > 0)
/* DemGeneral/DemStorageCondition 0-255 */    
typedef struct
{
    /* DemStorageConditionReplacementEventRef Reference: DemEventParameter MULTI:0-1*/
    const uint16 DemStorageConditionReplacementEventRef;
    
    /* DemStorageConditionStatus Range: true or false */
    const boolean DemStorageConditionStatus;    
}Dem_StorageConditionType;    

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
const Dem_StorageConditionType DemStorageCondition[DEM_STORAGE_CONDITION_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_STORAGE_CONDITION_GROUP_NUM > 0)

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemStorageConditionGroup 0-255 */
extern CONST(uint8,AUTOMATIC) DemStorageConditionGroup[DEM_STORAGE_CONDITION_GROUP_NUM][DEM_STORAGE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

/*******************************************************************************
*                          FreezeFrame Configuration
*******************************************************************************/
#if(DEM_DID_CLASS_NUM > 0)
/* DemGeneral/DemDidClass 0-0xFFFF */
typedef struct
{
    /* DemDidIdentifier Range:0~65535 */
    const uint16 DemDidIdentifier;

    /*DemDidDataElementClassRef Reference: DemDataElementClass MULTI:1-255*/
    const uint16 DataSize;
    const Dem_ReadDataFncType ReadDataFnc;
} Dem_DidClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_DidClassType,AUTOMATIC) DemDidClass[DEM_DID_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
/* DemGeneral/DemFreezeFrameClass 0-65535 */
typedef struct
{
    /* DemDidClassRef 1-255 */
    /* SUM(DemDidClassRef/DataSize) */
    const uint16 DataSize;
    
    /* OffSet Of Array DemDidClassRef */
    const uint16 StartIndex;
    
    /* COUNT(DemDidClassRef) */
    const uint8 RefNum;  
    
}Dem_FreezeFrameClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(uint16,AUTOMATIC) DemDidClassRef[DEM_DID_CLASS_REF_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_FreezeFrameClassType,AUTOMATIC) DemFreezeFrameClass[DEM_FREEZE_FRAME_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)
/* DemGeneral/DemFreezeFrameRecordClass 0-255 */
typedef struct
{
    /* DemFreezeFrameRecordNumber Range:0~254 */
    const uint8 DemFreezeFrameRecordNumber;

    /* DemFreezeFrameRecordTrigger type:
    Enum{
    DEM_TRIGGER_ON_CONFIRMED
    DEM_TRIGGER_ON_FDC_THRESHOLD
    DEM_TRIGGER_ON_PENDING
    DEM_TRIGGER_ON_TEST_FAILED
    } */
    const uint8 DemFreezeFrameRecordTrigger;

    /* DemFreezeFrameRecordUpdate type:
    Enum{
    DEM_UPDATE_RECORD_NO
    DEM_UPDATE_RECORD_YES
    } */
    const uint8 DemFreezeFrameRecordUpdate;
}Dem_FreezeFrameRecordClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_FreezeFrameRecordClassType,AUTOMATIC) DemFreezeFrameRecordClass[DEM_FREEZE_FRAME_RECORD_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM > 0)
/* DemGeneral/DemFreezeFrameRecNumClass 0-255 */
typedef struct
{
    /* DemFreezeFrameRecordClassRef 1-254 */
    const uint16 StartIndex;
    const uint8 RefNum;    
}Dem_FreezeFrameRecNumClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(uint8,AUTOMATIC) DemFreezeFrameRecordClassRef[DEM_FREEZE_FRAME_RECORD_CLASS_REF_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


/* DemFreezeFrameRecNumClass 0-255 */
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_FreezeFrameRecNumClassType,AUTOMATIC) DemFreezeFrameRecNumClass[DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

/*******************************************************************************
*                          ExtendedData Configuration
*******************************************************************************/

#if(DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
/* DemGeneral/DemExtendedDataRecordClass 0-253 */
typedef struct
{
    /* DemExtendedDataRecordNumber Range:1~239 */
    const uint8 DemExtendedDataRecordNumber;

    /* DemExtendedDataRecordTrigger type:
     Enum{
     DEM_TRIGGER_ON_CONFIRMED
     DEM_TRIGGER_ON_FDC_THRESHOLD
     DEM_TRIGGER_ON_MIRROR
     DEM_TRIGGER_ON_PASSED
     DEM_TRIGGER_ON_PENDING
     DEM_TRIGGER_ON_TEST_FAILED
     } */
    const uint8 DemExtendedDataRecordTrigger;

    /* DemExtendedDataRecordUpdate type:
     Enum{
     DEM_UPDATE_RECORD_NO
     DEM_UPDATE_RECORD_YES
     } */
    const uint8 DemExtendedDataRecordUpdate;

    /* DemDataElementClassRef Reference: DemDataElementClass MULTI:1-255*/
    const uint8 DataSize;
    const Dem_ReadDataFncType ReadDataFnc;
} Dem_ExtendedDataRecordClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_ExtendedDataRecordClassType,AUTOMATIC) DemExtendedDataRecordClass[DEM_EXTENDED_DATA_RECORD_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
/* DemGeneral/DemExtendedDataClass 0-* */
typedef struct
{
    /* SUM(DemExtendedDataRecordClassRef/DataSize) */
    const uint16 DataSize;
    /* DemExtendedDataRecordClassRef 1-253 */
    const uint16 StartIndex;
    const uint8 RefNum;    
}Dem_ExtendedDataClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(uint8,AUTOMATIC) DemExtendedDataRecordClassRef[DEM_EXTENDED_DATA_RECORD_CLASS_REF_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_ExtendedDataClassType,AUTOMATIC) DemExtendedDataClass[DEM_EXTENDED_DATA_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

/*******************************************************************************
*                          DTC Configuration
*******************************************************************************/
#if(DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0)
/* DemGeneral/DemCallbackDTCStatusChanged 0..* */
typedef Std_ReturnType (*Dem_TriggerOnDTCStatusType)(
    uint32 DTC,
    Dem_UdsStatusByteType DTCStatusOld,
    Dem_UdsStatusByteType DTCStatusNew);

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_TriggerOnDTCStatusType,AUTOMATIC) DemCallbackDTCStatusChanged[DEM_CALLBACK_DTC_STATUS_CHANGED_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_GROUP_OF_DTC_NUM > 0)
/* DemGeneral/DemGroupOfDTC 0-255 */
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(uint32,AUTOMATIC) DemGroupOfDTC[DEM_GROUP_OF_DTC_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_DTC_NUM > 0)
/* DemConfigSet/DemDTC 0-65535 */
typedef struct
{
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_PRE_COMPILE)
    /* DemDtcValue Range:1~16777214 */
    const uint32 DemDtcValue;
#endif

    /* DemDTCAttributesRef Reference: DemDTCAttributes MULTI:1-1*/
    const uint16 DemDTCAttributesRef;

    /* DemDTCFunctionalUnit Range:0~255 */
    const uint8 DemDTCFunctionalUnit;

    /* DemDTCSeverity type:
     Enum{
     DEM_SEVERITY_CHECK_AT_NEXT_HALT
     DEM_SEVERITY_CHECK_IMMEDIATELY
     DEM_SEVERITY_MAINTENANCE_ONLY
     DEM_SEVERITY_NO_SEVERITY
     } */
    const uint8 DemDTCSeverity;
    
#if(DEM_GROUP_OF_DTC_NUM > 0)
    /* DemDTCGroupRef */
    const uint8 GroupRef; /* DTC GroupIndex */
#endif
} Dem_DTCType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_DTCType,AUTOMATIC) DemDTC[DEM_DTC_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_DTC_ATTRIBUTES_NUM > 0)
/* DemConfigSet/DemDTCAttributes 0-65535 */
typedef struct
{
    /* DemAgingAllowed Range: true or false */
    const boolean DemAgingAllowed;

    /* DemAgingCycleRef Reference: DemOperationCycle MULTI:0-1*/
    const uint8 DemAgingCycleRef;

    /* DemAgingCycleCounterThreshold Range:1~256 */
    const uint8 DemAgingCycleCounterThreshold;

    /* DemAgingCycleCounterThresholdForTFSLC Range:1~256 */
    const uint8 DemAgingCycleCounterThresholdForTFSLC;

    /* DemDTCPriority Range:1~256 */
    const uint8 DemDTCPriority;

    /* DemEventMemoryEntryFdcThresholdStorageValue Range:1~126 */
    const uint8 DemEventMemoryEntryFdcThresholdStorageValue;

#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    /* DemFreezeFrameRecNumClassRef Reference: DemFreezeFrameRecNumClass MULTI:0-1
    */
    const uint8 DemFreezeFrameRecNumClassRef;
#else
    /* DemMaxNumberFreezeFrameRecords Range:0~255 */
    const uint8 DemMaxNumberFreezeFrameRecords;
#endif

#if(DEM_NVM_ENABLE == STD_ON)
    /* DemImmediateNvStorage Range: true or false */
    const boolean DemImmediateNvStorage;
#endif
    /* DemDTCSignificance type:
    Enum{
    DEM_EVENT_SIGNIFICANCE_FAULT
    DEM_EVENT_SIGNIFICANCE_OCCURRENCE
    } */
    const uint8 DemDTCSignificance;

    /* DemExtendedDataClassRef Reference: DemExtendedDataClass MULTI:0-1*/
    const uint16 DemExtendedDataClassRef;

    /* DemFreezeFrameClassRef Reference: DemFreezeFrameClass MULTI:0-1*/
    const uint16 DemFreezeFrameClassRef;

    /* DemMemoryDestinationRef ChoiceReference: TODO: 2*/
    const uint8 DemMemoryDestinationRef[DEM_MEM_DEST_MAX_NUM_OF_DTC];
}Dem_DTCAttributesType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_DTCAttributesType,AUTOMATIC) DemDTCAttributes[DEM_DTC_ATTRIBUTES_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


#endif
/*******************************************************************************
*                          Indicator Configuration
*******************************************************************************/
#if(DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
/* DemConfigSet/DemEventParameter/DemIndicatorAttribute */
typedef struct
{
    /* DemIndicatorFailureCycleCounterThreshold Range:0~255 */
    const uint8 DemIndicatorFailureCycleCounterThreshold;

    /* DemIndicatorHealingCycleCounterThreshold Range:0~255 */
    const uint8 DemIndicatorHealingCycleCounterThreshold;
    
    /* DemIndicatorRef Reference: DemIndicator MULTI:1-1*/
    const uint8 DemIndicatorRef;

    #if(DEM_CONFIG_VARIANT != DEM_CONFIG_POST_BUILD)
    /* DemIndicatorBehaviour type:
    Enum{
    DEM_INDICATOR_BLINKING
    DEM_INDICATOR_BLINK_CONT
    DEM_INDICATOR_CONTINUOUS
    DEM_INDICATOR_FAST_FLASH
    DEM_INDICATOR_SLOW_FLASH
    } */
    const uint8 DemIndicatorBehaviour;
    #endif    
}Dem_IndicatorAttributeType;  

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_IndicatorAttributeType,AUTOMATIC) DemIndicatorAttribute[DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM];  
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

/*******************************************************************************
*                          Debounce Configuration
*******************************************************************************/
#if((DEM_DEBOUNCE_COUNTER_BASED_SUPPORT == STD_ON) \
    && (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0))
/* DemConfigSet/DemDebounceCounterBasedClass 0-65535 */
typedef struct
{
    /* DemDebounceCounterDecrementStepSize Range:1~32768 */
    const sint16 DemDebounceCounterDecrementStepSize;

    /* DemDebounceCounterIncrementStepSize Range:1~32767 */
    const sint16 DemDebounceCounterIncrementStepSize;

    /* DemDebounceCounterJumpDownValue Range:-32768~32767 */
    const sint16 DemDebounceCounterJumpDownValue;

    /* DemDebounceCounterJumpUpValue Range:-32768~32767 */
    const sint16 DemDebounceCounterJumpUpValue;

    /* DemDebounceCounterFailedThreshold Range:1~32767 */
    const sint16 DemDebounceCounterFailedThreshold;

    /* DemDebounceCounterPassedThreshold Range:-32768~-1 */
    const sint16 DemDebounceCounterPassedThreshold;

    /* DemDebounceCounterJumpDown Range: true or false */
    const boolean DemDebounceCounterJumpDown;

    /* DemDebounceCounterJumpUp Range: true or false */
    const boolean DemDebounceCounterJumpUp;

    /* DemDebounceCounterStorage Range: true or false */
    const boolean DemDebounceCounterStorage;

    /* DemDebounceBehavior type:
    Enum{
    DEM_DEBOUNCE_FREEZE
    DEM_DEBOUNCE_RESET
    } */
    const uint8 DemDebounceBehavior;
}Dem_DebounceCounterBasedClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_DebounceCounterBasedClassType,AUTOMATIC) DemDebounceCounterBasedClass[DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if((DEM_DEBOUNCE_TIME_BASED_SUPPORT == STD_ON)\
    &&(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0))
/* DemConfigSet/DemDebounceTimeBaseClass 0-65535 */
typedef struct
{
    /* DemDebounceTimeFailedThreshold Range:0.001~3600 */
    const uint32 DemDebounceTimeFailedThreshold;

    /* DemDebounceTimePassedThreshold Range:0.001~3600 */
    const uint32 DemDebounceTimePassedThreshold;

    /* DemDebounceBehavior type:
     Enum{
     DEM_DEBOUNCE_FREEZE
     DEM_DEBOUNCE_RESET
     } */
    const uint8 DemDebounceBehavior;
} Dem_DebounceTimeBaseClassType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_DebounceTimeBaseClassType,AUTOMATIC) DemDebounceTimeBaseClass[DEM_DEBOUNCE_TIME_BASE_CLASS_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
/* DemConfigSet/DemEventParameter/DemDebounceAlgorithmClass/DemDebounceMonitorInternal */
typedef Std_ReturnType (*Dem_GetFDCFncType)(sint8* FDC);

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_GetFDCFncType,AUTOMATIC) DemGetFDCFncs[DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif
/*******************************************************************************
*                          Event Configuration
*******************************************************************************/
/* DemConfigSet/DemEventParameter/DemCallbackInitMForE */
typedef Std_ReturnType (*Dem_CallbackInitMForEType)(Dem_InitMonitorReasonType InitMonitorReason);

/* DemConfigSet/DemEventParameter/DemCallbackEventDataChanged */
typedef void (*Dem_CallbackEventDataChangedType)(Dem_EventIdType EventId);

#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
/* DemConfigSet/DemEventParameter/DemCallbackEventStatusChanged */
typedef void (*Dem_TriggerOnEventStatusType)( \
    Dem_EventIdType EventId, \
    Dem_UdsStatusByteType EventStatusByteOld, \
    Dem_UdsStatusByteType EventStatusByteNew \
    );
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_TriggerOnEventStatusType,AUTOMATIC) DemCallbackEventStatusChanged[DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM > 0)
/* DemConfigSet/DemEventParameter/DemCallbackClearEventAllowed */
typedef struct
{
    /* DemCallbackClearEventAllowedFnc type: FunctionName */
    const Std_ReturnType (*ClearEventAllowed)(boolean *Allowed);
    /* DemClearEventAllowedBehavior type:
    Enum{
    DEM_NO_STATUS_BYTE_CHANGE
    DEM_ONLY_THIS_CYCLE_AND_READINESS
    } */
    const uint8 DemClearEventAllowedBehavior;
}Dem_CallbackClearEventAllowedType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_CallbackClearEventAllowedType,AUTOMATIC) DemCallbackClearEventAllowed[DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#endif
/* DemConfigSet/DemEventParameter 0-65535 */
typedef struct
{
    /* DemCallbackInitMForE 0-1 */
    const Dem_CallbackInitMForEType DemCallbackInitMForE;
    
    /* DemCallbackEventDataChanged 0-1 */
    const Dem_CallbackEventDataChangedType DemCallbackEventDataChanged;

#if(DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM > 0)
    /* DemCallbackClearEventAllowed 0-1 */
    const uint16 DemCallbackClearEventAllowed;
#endif
 
#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0) 
    /* DemCallbackEventStatusChanged 0-* */    
    const uint16 StatusChangedCbkStartIndex;
    const uint8 StatusChangedCbkNum;
#endif  
    
    /* DemDTCRef Reference: DemDTC MULTI:0-1*/
    const uint16 DemDTCRef;

    
    /* DemDebounceAlgorithmClass 1-1 -CHOICES */    
    const uint16 AlgorithmRef; /* Index Of AlgirithmClass */
    const uint16 AlgorithmIndex;
    const uint8 AlgorithmType;
    
#if(DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
    /* DemIndicatorAttribute 0-255 */
    const uint16 AttrStartIndex;
    const uint8 AttrNum;    
#endif

#if(DEM_CONFIG_VARIANT != DEM_CONFIG_POST_BUILD)
    /* DemEventFailureCycleCounterThreshold Range:0~255 */
    const uint8 DemEventFailureCycleCounterThreshold;
    /* DemEventAvailable Range: true or false */
    const boolean DemEventAvailable;
#endif

    /* DemFFPrestorageSupported Range: true or false */
    const boolean DemFFPrestorageSupported;

    /* DemEventKind type:
    Enum{
    DEM_EVENT_KIND_BSW
    DEM_EVENT_KIND_SWC
    } */
    const uint8 DemEventKind;

    /* DemReportBehavior type:
    Enum{
    REPORT_AFTER_INIT
    REPORT_BEFORE_INIT
    } */
    const uint8 DemReportBehavior;

    /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
    const uint8 DemOperationCycleRef;

    /* DemEnableConditionGroupRef Reference: DemEnableConditionGroup MULTI:0-1*/
    const uint8 DemEnableConditionGroupRef;

    /* DemStorageConditionGroupRef Reference: DemStorageConditionGroup MULTI:0-1*/
    const uint8 DemStorageConditionGroupRef;

}Dem_EventParameterType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_EventParameterType,AUTOMATIC) DemEventParameter[DEM_EVENT_PARAMETER_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


/*******************************************************************************
*                          Memory Configuration
*******************************************************************************/
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
/* FreezeFrameInfo Define */
typedef struct
{
    uint8 RecordNum;
    uint8 Data[DEM_FREEZE_FRAME_MAX_LEN];
}Dem_FreezeFrameInfoType;  
#endif

/* EventMemoryEntry Define */
typedef struct
{
    uint32 AbsTime;
    Dem_EventIdType EventId;
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 FFNum;
    uint8 LatestFF;
    Dem_FreezeFrameInfoType FFList[DEM_MAX_NUMBER_FF_RECORDS];
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    uint8 ExtStatus[DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE];
    uint8 ExtData[DEM_EXTENDED_DATA_MAX_LEN];
#endif
    uint32 Crc;
}Dem_EventMemEntryType;

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemPrimaryMemory 1-1 */
extern VAR(Dem_EventMemEntryType,AUTOMATIC) DemPrimaryMemory[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

/* EventMemoryDest Define */
typedef struct
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, AUTOMATIC) EntryList;
    const uint8 EntryNum;
    const Dem_DTCOriginType ExtId;
}Dem_MemDestConfigType;

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(Dem_MemDestConfigType,AUTOMATIC) DemMemDestCfg[DEM_MEM_DEST_TOTAL_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


/* Non-volatile Memory Type */
typedef struct
{
#if(DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0)
    sint16 InternalDebounceCounter[DEM_DEBOUNCE_COUNTER_STORAGE_NUM];
#endif
    uint8 UdsDtcStatus[DEM_EVENT_PARAMETER_NUM];

#if(DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON)
    uint8 OpCycStatus[DEM_OPERATION_CYCLE_NUM_BYTE];
#endif
    uint32 Crc;
}Dem_NonVolatileMemInfoType;
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(Dem_NonVolatileMemInfoType,AUTOMATIC) DemNonVolatileMemInfo;
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_NVRAM_BLOCKID_NUM > 0)
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
extern CONST(uint16,AUTOMATIC) DemNvRamBlockId[DEM_NVRAM_BLOCKID_NUM];
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

/*******************************************************************************
*                          PostBuild Configuration
*******************************************************************************/

#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
typedef struct
{
    /* DemDTC.DemDtcValue 0-1,{1~16777214} */
    P2CONST(uint32, AUTOMATIC, DEM_CONST_PBCFG) DemDtcValue;

    /* DemEventParameter.DemEventFailureCycleCounterThreshold 1-1 {0-255} */
    P2CONST(uint8, AUTOMATIC, DEM_CONST_PBCFG) DemEventFailureCycleCounterThreshold;
    /* DemEventParameter.DemEventAvailable 1-1 {true, flase} */
    P2CONST(boolean, AUTOMATIC, DEM_CONST_PBCFG) DemEventAvailable;
#if(DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
    /* DemEventParameter.DemIndicatorAttribute.DemIndicatorBehaviour  1-1{enum} */
    P2CONST(uint8, AUTOMATIC, DEM_CONST_PBCFG) DemIndicatorBehaviour;
#endif
}Dem_ConfigType;
#else
typedef void Dem_ConfigType;
#endif

#endif /* DEM_CFGNEW_DEM_CFGTYPES_H_ */

/********[E N D   O F   F I L E]***********************************************/
