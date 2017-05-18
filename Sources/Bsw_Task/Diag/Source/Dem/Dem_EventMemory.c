/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventMemory.c>
 *  @brief      <Dem_EventMemory>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 3:01:14>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTMEMORY_C_AR_MAJOR_VERSION   3U
#define DEM_EVENTMEMORY_C_AR_MINOR_VERSION   1U
#define DEM_EVENTMEMORY_C_AR_PATCH_VERSION   0U
#define DEM_EVENTMEMORY_C_SW_MAJOR_VERSION   1U
#define DEM_EVENTMEMORY_C_SW_MINOR_VERSION   0U
#define DEM_EVENTMEMORY_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/

#include "ExternFlashIf.h"
#include "NvmIf.h"
#include "DataStore.h"

#include "Dem_CfgTypes.h"
#include "Dem_EventMemory.h"
#include "Dem_ExtendedData.h"
#include "Dem_FreezeFrame.h"

#include "Std_ExtendedTypes.h"
/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_EVENTMEMORY_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif

#if (DEM_EVENTMEMORY_C_AR_MAJOR_VERSION != DEM_EVENTMEMORY_H_AR_MAJOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_AR_MINOR_VERSION != DEM_EVENTMEMORY_H_AR_MINOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_AR_PATCH_VERSION != DEM_EVENTMEMORY_H_AR_PATCH_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_SW_MAJOR_VERSION != DEM_EVENTMEMORY_H_SW_MAJOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTMEMORY_C_SW_MINOR_VERSION != DEM_EVENTMEMORY_H_SW_MINOR_VERSION)
  #error "Dem_EventMemory.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
#define DEM_MEM_DEST_MAX_NUM 2

/********[I N T E R N A L   D A T A]*******************************************/

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemSetClearDtcFlag = FALSE;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemUpdateNvmFlag[DEM_EVENT_PARAMETER_NUM];;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_MemDestInfoType,AUTOMATIC) DemMemDestInfo[DEM_MEM_DEST_TOTAL_NUM];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_EventInfoType,AUTOMATIC) DemEventInfo[DEM_EVENT_PARAMETER_NUM];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_INDICATOR_NUM > 0)
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_IndicatorStatusType,AUTOMATIC) DemWIRState[DEM_INDICATOR_NUM];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(uint8,AUTOMATIC) DemAgingCycle = DEM_OPERATION_CYCLE_INVALID;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
#define DEM_AGING_CNT_UNAVAILABLE 0xFF
/* SWS_Dem_00640 */
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(uint8,AUTOMATIC) DemExternAgingCnt = DEM_AGING_CNT_UNAVAILABLE;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
#endif
/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

#if(DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint8, DEM_CODE) Dem_EventPriorityGet(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_EventDisplacement(Dem_EventIdType InternalId,
    uint8 MemDest);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_EventAgingProcess(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
uint8 OpId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(((DEM_FREEZE_FRAME_CLASS_NUM > 0)\
    &&(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))\
    ||(DEM_EXTENDED_DATA_CLASS_NUM > 0))

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_EventDataStorageTrigger(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
uint8 Trigger);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#else
#define Dem_EventDataStorageTrigger(a,b,c)
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_MemEntryAllocate(Dem_EventIdType IntId,
    uint8 MemDest);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_INDICATOR_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(boolean, DEM_CODE) Dem_CheckWIROn(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(boolean, DEM_CODE) Dem_CheckWIROff(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_OperationCycleCounterProcess(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <Dem_EventMemInit>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventMemInit(void)
{
    uint8 iloop = DEM_MEM_DEST_TOTAL_NUM;
    P2VAR(Dem_MemDestInfoType, AUTOMATIC, DEM_VAR) pMem = DemMemDestInfo;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = DemEventInfo;
    P2CONST(Dem_MemDestConfigType, AUTOMATIC, DEM_CONST) pMemCfg = DemMemDestCfg;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = DemEventParameter;
    uint8 DemDebounceBehavior;
#if(DEM_NVRAM_BLOCKID_NUM == 0)
    uint8 EntryNum;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
#endif
    while (iloop > 0)
    {
        iloop--;
        pMem->OverFlow = FALSE;
        pMem->RecordNum = 0;
#if(DEM_NVRAM_BLOCKID_NUM == 0)
        EntryNum = pMemCfg->EntryNum;
        pEntry = pMemCfg->EntryList;
        while (EntryNum > 0)
        {
            Dem_MemEntryDelete(pEntry);
            pEntry++;
            EntryNum--;
        }
#endif
        Dem_MemSet(pMem->DTCRecordUpdateStatus, 0x00, DEM_DTC_NUM_BYTE);
        pMemCfg++;
        pMem++;
    }
#if(DEM_INDICATOR_NUM > 0)
    Dem_MemSet(DemWIRState, DEM_INDICATOR_OFF, DEM_INDICATOR_NUM);
#endif

    iloop = 0;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        Dem_MemSet((uint8*UDS_FAR)pEvent, 0x00, sizeof(Dem_EventInfoType));
        DemUpdateNvmFlag[iloop] = FALSE;
        pEvent->IntId = iloop;
        DEM_FLAGS_SET(pEvent->UdsStatus,
            DEM_UDS_STATUS_TNCSLC|DEM_UDS_STATUS_TNCTOC);
#if(DEM_CONFIG_VARIANT != DEM_CONFIG_POST_BUILD)
        if (pEventCfg->DemEventAvailable == TRUE)
#else
        if (DemPbCfgPtr->DemEventAvailable[iloop] == TRUE)
        #endif
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
        }

        if (DemOperationCycle[pEventCfg->DemOperationCycleRef]
            .DemOperationCycleAutostart == TRUE)
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_OPERATION_CYCLE);
        }
        switch (pEventCfg->AlgorithmType)
        {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
            case DEM_DEBOUNCE_COUNTER_BASED:
                DemDebounceBehavior = DemDebounceCounterBasedClass[pEventCfg
                    ->AlgorithmRef].DemDebounceBehavior;
                break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
            case DEM_DEBOUNCE_TIME_BASE:
                DemDebounceBehavior = DemDebounceTimeBaseClass[pEventCfg
                    ->AlgorithmRef].DemDebounceBehavior;
                break;
#endif
            default:
                DemDebounceBehavior = DEM_DEBOUNCE_FREEZE;
                break;
        }
        if (DemDebounceBehavior == DEM_DEBOUNCE_RESET)
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_RESET);
        }

#if(DEM_ENABLE_CONDITION_NUM > 0)
        if(pEventCfg->DemEnableConditionGroupRef == DEM_ENABLE_CONDITION_GROUP_INVALID)
#endif
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
        }
#if(DEM_STORAGE_CONDITION_NUM > 0)
        if(pEventCfg->DemStorageConditionGroupRef == DEM_STORAGE_CONDITION_GROUP_INVALID)
#endif
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
        }

        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);

        pEvent++;
        pEventCfg++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckEventMemEntryExistsAlready>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_CheckEventMemEntryExistsAlready(Dem_EventIdType IntId)
{
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    uint8 iloop = DEM_MEM_DEST_MAX_NUM_OF_DTC;
    uint16 TempRef = DemEventParameter[IntId].DemDTCRef;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

    if (TempRef == DEM_DTC_INVALID)
    {
        return NULL_PTR;
    }
    pMemDest = DemDTCAttributes[DemDTC[TempRef].DemDTCAttributesRef]
    .DemMemoryDestinationRef;
    while (iloop > 0)
    {
        if (*pMemDest != DEM_MEM_DEST_INVALID)
        {
            pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(IntId), *pMemDest);
            if (NULL_PTR != pEntry)
            {
                return pEntry;
            }
        }
        pMemDest++;
        iloop--;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetEventInfo>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_EventInfoType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_GetEventInfo(Dem_EventIdType IntId)
{
    return &DemEventInfo[IntId];
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventTestFailed>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventTestFailed(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[IntId];
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = &DemEventInfo[IntId];
    boolean TFBitChange = FALSE;

    if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    {
        Dem_SetOccurrenceEvent(IntId, DEM_UDS_STATUS_TF);
        TFBitChange = TRUE;
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
        /* SWS_Dem_00641 */
        pEvent->AgingCounter = DemExternAgingCnt;
#endif
    }
    DEM_FLAGS_SET(pEvent->UdsStatus,
    DEM_UDS_STATUS_TF|DEM_UDS_STATUS_TFTOC|DEM_UDS_STATUS_TFSLC);
    DEM_FLAGS_CLR(pEvent->UdsStatus,
    DEM_UDS_STATUS_TNCTOC|DEM_UDS_STATUS_TNCSLC);

#if(DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_TF)
    if (pEvent->OccurrenceCounter < 0xFF)
    {
        pEvent->OccurrenceCounter++;
    }
#endif

#if(DEM_STORAGE_CONDITION_NUM > 0)
    if (0x00
    == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))
    {
        return;
    }
#endif

    /* @req SWS_Dem_00783 */
    if (TFBitChange == TRUE)
    {
#if(DEM_TRIGGER_ON_TEST_FAILED == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
        Dem_EventRetention(IntId, pEventBuffer);
#else
        Dem_EventDataStorageTrigger(IntId,pEventBuffer,DEM_TRIGGER_ON_TEST_FAILED);
#endif
    }
    /* Check PendingDTC bit */
    if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
    {
        /* @req SWS_Dem_00784 */
        DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);
#if(DEM_TRIGGER_ON_PENDING == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
        Dem_EventRetention(IntId,pEventBuffer);
        
#else
        Dem_EventDataStorageTrigger(IntId, pEventBuffer,
        DEM_TRIGGER_ON_PENDING);
#endif
    }
#if(DEM_TRIGGER_ON_PENDING == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
    else
    {
        /* @req SWS_Dem_00922 */
        if(TFBitChange == TRUE)
        {
            Dem_EventDataStorageTrigger(IntId,pEventBuffer,DEM_TRIGGER_ON_PENDING);
        }
    }
#endif

    /* confirmedLevel reached? */
    if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
    {
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
        if(pEvent->FailureCounter >= DemPbCfgPtr->DemEventFailureCycleCounterThreshold[IntId])
#else
        if (pEvent->FailureCounter
        >= pEventCfg->DemEventFailureCycleCounterThreshold)
#endif
        {
            Dem_SetOccurrenceEvent(IntId, DEM_UDS_STATUS_CDTC);
            DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC);
            pEvent->FailureCounter = 0;
#if(DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_CDTC)
            if(pEvent->OccurrenceCounter < 0xFF)
            {
                pEvent->OccurrenceCounter++;
            }
#endif
#if(DEM_INDICATOR_NUM > 0)
            /* Check WarningIndicatorOnCondition */
            if (TRUE == Dem_CheckWIROn(pEvent, pEventCfg))
            {
                DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
            }
#endif
            /* @req SWS_Dem_00785 */
#if(DEM_TRIGGER_ON_CONFIRMED == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
            Dem_EventRetention(IntId,pEventBuffer);
#else
            Dem_EventDataStorageTrigger(IntId, pEventBuffer,
            DEM_TRIGGER_ON_CONFIRMED);
#endif
        }
    }
    else
    {
#if((DEM_TRIGGER_ON_CONFIRMED == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER) \
    && (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF))
        /* @req SWS_Dem_00923 */
        if(TFBitChange == TRUE)
        {
            Dem_EventRetention(IntId,pEventBuffer);
        }
#endif
#if(DEM_INDICATOR_NUM > 0)
            /* Check WarningIndicatorOnCondition */
            if (TRUE == Dem_CheckWIROn(pEvent, pEventCfg))
            {
                DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
            }
#endif
    }
    
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventTestPassed>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventTestPassed(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = &DemEventInfo[IntId];
    uint16 TempRef = DemEventParameter[IntId].DemDTCRef;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg;

#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
    boolean TFClear = FALSE;

    if(0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus,DEM_UDS_STATUS_TF))
    {
        TFClear = TRUE;
    }
#endif
    DEM_FLAGS_CLR(pEvent->UdsStatus,
    DEM_UDS_STATUS_TF|DEM_UDS_STATUS_TNCTOC|DEM_UDS_STATUS_TNCSLC);

#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    Dem_EventDataStorageTrigger(IntId, pEventBuffer, DEM_TRIGGER_ON_PASSED);
#endif
    if (TempRef == DEM_DTC_INVALID)
    {
        return;
    }

    /* @req SWS_Dem_00698 */
    pDTCAttrCfg = &DemDTCAttributes[DemDTC[TempRef].DemDTCAttributesRef];
    if ((pDTCAttrCfg->DemAgingAllowed == FALSE)
    && (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC)))
    {
        return;
    }
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
    if(TFClear == TRUE)
    {
        /* SWS_Dem_00642 */
        pEvent->AgingCounter = (DemExternAgingCnt + pDTCAttrCfg->DemAgingCycleCounterThreshold)%254 + 1;
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventRetention>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern uint8 DemOccurrenceCounter;
extern uint8 DemAgingCounter;
FUNC(void, DEM_CODE) Dem_EventRetention(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
{
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    uint8 iloop = DEM_MEM_DEST_MAX_NUM_OF_DTC;
    uint16 TempRef = DemEventParameter[IntId].DemDTCRef;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

    if (TempRef == DEM_DTC_INVALID)
    {
        return;
    }
    pMemDest = DemDTCAttributes[DemDTC[TempRef].DemDTCAttributesRef]
    .DemMemoryDestinationRef;
    while (iloop > 0)
    {
        iloop--;
        if (*pMemDest != DEM_MEM_DEST_INVALID)
        {
            pEntry = Dem_MemEntryAllocate(IntId, *pMemDest);
            if ((pEntry != NULL_PTR)
            && (0x00 == DEM_BITS_ISSET(DemMemDestInfo[*pMemDest].DTCRecordUpdateStatus, TempRef)))
            {
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
                Dem_FreezeFrameStorage(IntId, pEntry, pEventBuffer,
                DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
                DemOccurrenceCounter = DemEventInfo[IntId].OccurrenceCounter;
                DemAgingCounter = DemEventInfo[IntId].AgingCounter;
                
                Dem_ExtendedDataStorage(IntId, pEntry, pEventBuffer,
                DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER);
#endif         
            }
        }
        pMemDest++;
    }

    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_OperationCycleStart>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OpId && IsRestart>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_OperationCycleStart(uint8 OpId,
    boolean IsRestart)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = DemEventParameter;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = DemEventInfo;
    uint16 iloop = DEM_EVENT_PARAMETER_NUM;

    while (iloop > 0)
    {
        iloop--;
        //Dem_EventAgingProcess(pEvent, OpId);
        if (pEventCfg->DemOperationCycleRef == OpId)
        {
            /* @req SWS_Dem_00679 Monitor re-initialization  */
            Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_RESTART);

            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_OPERATION_CYCLE);
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_TESTED);
            /* bit0 DO Nothing */
            /* bit2 1=0 ? */
            if(IsRestart == TRUE)
            {
                Dem_OperationCycleCounterProcess(pEvent);
                if(0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus,DEM_UDS_STATUS_TFTOC))
                {
                   if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                   {
                       /* @req SWS_Dem_00390 */
                       DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);
                       /* @req SWS_Dem_00391 */
                       pEvent->FailureCounter = 0;
                   }
                }
                else
                {
                    pEvent->FailureCounter++;
                }
            }

            /* bit1 1 => 0 @req SWS_Dem_00389*/
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC);

            /* bit6 1 => 0 @req SWS_Dem_00394*/
            DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC);
        }
        pEventCfg++;
        pEvent++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_OperationCycleEnd>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OpId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_OperationCycleEnd(uint8 OpId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = DemEventParameter;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = DemEventInfo;
    uint16 iloop = DEM_EVENT_PARAMETER_NUM;

    while (iloop > 0)
    {
        Dem_EventAgingProcess(pEvent, OpId);
        if (pEventCfg->DemOperationCycleRef == OpId)
        {
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_OPERATION_CYCLE);
            /* Process Operation Cycle Counters */
            Dem_OperationCycleCounterProcess(pEvent);
            if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
            {
                /* TestFailedThisOperationCycle = 0 */
                if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                {
                    /* @req SWS_Dem_00390 */
                    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);
                    /* @req SWS_Dem_00391 */
                    pEvent->FailureCounter = 0;
                }
            }
            else
            {
                /* TestFailedThisOperationCycle += 1 */
                pEvent->FailureCounter++;
            }
        }
        pEventCfg++;
        pEvent++;
        iloop--;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId && MemDestIndex>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_MemEntryGet(Dem_EventIdType ExtId,
    uint8 MemDest)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[MemDest].EntryList;
    uint8 iloop = DemMemDestCfg[MemDest].EntryNum;

    while (iloop > 0)
    {
        iloop--;
        if (pEntry->EventId == ExtId)
        {
            return pEntry;
        }
        pEntry++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryGetByDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCIndex && MemDest>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <Dem_EventMemEntryType*>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_MemEntryGetByDTC(uint32 DTCIndex,
    uint8 MemDest)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[MemDest].EntryList;
    uint8 iloop = DemMemDestCfg[MemDest].EntryNum;

    while (iloop > 0)
    {
        iloop--;
        if((pEntry->EventId != 0x00) &&
            (DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef == DTCIndex))
        {
            return pEntry;
        }
        pEntry++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/
/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryAllocate>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && MemDestIndex>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_MemEntryAllocate(Dem_EventIdType IntId,
    uint8 MemDest)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_MemDestInfoType, AUTOMATIC, DEM_VAR) pMem=&DemMemDestInfo[MemDest];
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(IntId), MemDest);
    if (pEntry == NULL_PTR)
    {
        pEntry = Dem_MemEntryGet(0x00, MemDest);
        if (pEntry == NULL_PTR)
        {
            pMem->OverFlow = TRUE;
#if(DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
            /* SWS_Dem_00401 SWS_Dem_00402 */
            pEntry = Dem_EventDisplacement(IntId, MemDest);
#endif
        }
        if (pEntry != NULL_PTR)
        {
            pEntry->AbsTime = pMem->RecordNum++;
            pEntry->EventId = Dem_GetEventExternalId(IntId);
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
            pEntry->FFNum = 0;
            pEntry->LatestFF = 0;
#endif
            pEvent = &DemEventInfo[IntId];
            if(pEvent->OperationCycleCntStarted == FALSE)
            {
                pEvent->OperationCycleCntStarted = TRUE;
                pEvent->CyclesSinceFirstFailed = 0;
                pEvent->CyclesSinceLastFailed = 0;
                pEvent->FailedCycles = 0;
            }
        }
    }
#if(DEM_NVRAM_BLOCKID_NUM > 0)
    DemUpdateNvmFlag[IntId] = TRUE;
#endif
    return pEntry;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryDelete>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEntry>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MemEntryDelete(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    if((pEntry->EventId > 0) && (pEntry->EventId <= DEM_EVENT_PARAMETER_NUM))
    {
        pEvent = &DemEventInfo[Dem_GetEventInternalId(pEntry->EventId)];
        /* Process Cycle Counters */
        pEvent->OperationCycleCntStarted = FALSE;
        pEvent->CyclesSinceFirstFailed = 0;
        pEvent->CyclesSinceLastFailed = 0;
        pEvent->FailedCycles = 0;

#if(DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
        /* SWS_Dem_00409 */
        /* Reset bit2 bit3 to 0 */
        DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC|DEM_UDS_STATUS_CDTC);
    #if(DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT)
        DEM_FLAGS_CLR(pEvent->UdsStatus,DEM_UDS_STATUS_TFSLC);
    #endif
#endif
    }


    pEntry->AbsTime = 0x00;
    pEntry->EventId = 0x00;
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    pEntry->FFNum = 0x00;
    pEntry->LatestFF = 0x00;
    Dem_MemSet((uint8*UDS_FAR)pEntry->FFList, 0xFF,
    sizeof(Dem_FreezeFrameInfoType) * DEM_MAX_NUMBER_FF_RECORDS);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    Dem_MemSet(pEntry->ExtData, 0xFF, DEM_EXTENDED_DATA_MAX_LEN);
    Dem_MemSet(pEntry->ExtStatus, 0x00, DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE);
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(((DEM_FREEZE_FRAME_CLASS_NUM > 0)\
    &&(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))\
    ||(DEM_EXTENDED_DATA_CLASS_NUM > 0))
/*************************************************************************/
/*
 * @Brief               <Dem_EventDataStorageTrigger>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer && Trigger>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_EventDataStorageTrigger(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
uint8 Trigger)
{
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    uint8 iloop = DEM_MEM_DEST_MAX_NUM_OF_DTC;
    uint16 TempRef = DemEventParameter[IntId].DemDTCRef;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    boolean FFStorage = FALSE;
    boolean ExtDataStorage = FALSE;

    if (TempRef == DEM_DTC_INVALID)
    {
        return;
    }
#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) \
    &&(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))
    FFStorage = Dem_CheckFreezeFrameIsStorage(IntId, Trigger);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    ExtDataStorage = Dem_CheckExtendedDataIsStorage(IntId, Trigger);
#endif

    if((FFStorage == FALSE) && (ExtDataStorage == FALSE))
    {
        return;
    }

    pMemDest = DemDTCAttributes[DemDTC[TempRef].DemDTCAttributesRef]
    .DemMemoryDestinationRef;
    while (iloop > 0)
    {
        iloop--;
        if ((*pMemDest != DEM_MEM_DEST_INVALID)
        && (0x00
            == DEM_BITS_ISSET(
                DemMemDestInfo[*pMemDest].DTCRecordUpdateStatus, TempRef)))
        {
            pEntry = Dem_MemEntryAllocate(IntId, *pMemDest);
            if (pEntry != NULL_PTR)
            {
#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) \
    &&(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))
                if(FFStorage == TRUE)
                {
                    Dem_FreezeFrameStorage(IntId,pEntry,pEventBuffer,Trigger);
                }
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
                if(ExtDataStorage == TRUE)
                {
                    Dem_ExtendedDataStorage(IntId, pEntry, pEventBuffer, Trigger);
                }
#endif
            }
        }
        pMemDest++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#if(DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * @Brief               <Dem_EventPriorityGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <uint8>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint8, DEM_CODE) Dem_EventPriorityGet(Dem_EventIdType IntId)
{
    uint8 res = 0xFF;
    uint16 ref = DemEventParameter[IntId].DemDTCRef;

    if (ref != DEM_DTC_INVALID)
    {
        res = DemDTCAttributes[DemDTC[ref].DemDTCAttributesRef].DemDTCPriority;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventDisplacement>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <InternalId && MemIndex>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE) Dem_EventDisplacement(Dem_EventIdType InternalId,
    uint8 MemDest)
{
    /* Search entries with lowest priority */
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pResEntry = NULL_PTR;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[MemDest].EntryList;
    uint8 iloop = DemMemDestCfg[MemDest].EntryNum;
    uint8 EventPrority = Dem_EventPriorityGet(InternalId);
    uint8 EntryPrority;
#if(DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
    boolean PassiveFound = FALSE;
#endif
    boolean LowProrityFound = FALSE;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    uint16 TempEventIntId;

    while (iloop > 0)
    {
        iloop--;
        TempEventIntId = Dem_GetEventInternalId(pEntry->EventId);
        /* Check CurrentDTC Disable Record Update */
        if (0x00 != DEM_BITS_ISSET(DemMemDestInfo[MemDest].DTCRecordUpdateStatus,
                DemEventParameter[TempEventIntId].DemDTCRef))
        {
            pEntry++;
            continue;
        }
        /* Search Priority of reported event >= found entries */
        EntryPrority = Dem_EventPriorityGet(TempEventIntId);
        pEvent = &DemEventInfo[TempEventIntId];
        /* SWS_Dem_00695 TODO: */
#if(DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
        /* SWS_Dem_00404 */
        if (EventPrority <= EntryPrority)
        {
            /* Check PassiveMode */
            if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
            {
                /* In Passive Mode */
                if (PassiveFound == FALSE)
                {
                    /* The First Passive Mode Entry Found */
                    pResEntry = pEntry;
                    PassiveFound = TRUE;

                }
                else
                {
                    /* Already found */
                    if (pResEntry->AbsTime > pEntry->AbsTime)
                    {
                        /* Get The Oldest One */
                        pResEntry = pEntry;
                    }
                }
            }
            else
            {
                /* In Active Mode */
                if (PassiveFound == FALSE)
                {
#endif
                    /* SWS_Dem_00405  Check Prority */
                    if (EventPrority < EntryPrority)
                    {
                        if (LowProrityFound == FALSE)
                        {
                            LowProrityFound = TRUE;
                            pResEntry = pEntry;
                        }
                        else
                        {
                            /* Already found */
                            if (pResEntry->AbsTime > pEntry->AbsTime)
                            {
                                /* Get The Oldest One */
                                pResEntry = pEntry;
                            }
                        }
                    }
                    else
                    {
                        if (LowProrityFound == FALSE)
                        {
                            /* SWS_Dem_00692 Check TNCTOC bit */
                            if (0x00
                                != DEM_FLAGS_ISSET(pEvent->UdsStatus,
                                    DEM_UDS_STATUS_TNCTOC))
                            {
                                if (pResEntry == NULL_PTR)
                                {
                                    pResEntry = pEntry;
                                }
                                else
                                {
                                    /* Already found */
                                    if (pResEntry->AbsTime > pEntry->AbsTime)
                                    {
                                        /* Get The Oldest One */
                                        pResEntry = pEntry;
                                    }
                                }
                            }
                        }
                    }
#if(DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
                }
            }
        }
#endif
        pEntry++;
    }

    if (pResEntry != NULL_PTR)
    {
        Dem_MemEntryDelete(pResEntry);
    }
    return pResEntry;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_INDICATOR_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_CheckWIROn>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEvent && pEventCfg>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <boolean>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(boolean, DEM_CODE) Dem_CheckWIROn(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    boolean res = FALSE;
    uint8 iloop = pEventCfg->AttrNum;
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_CONST) pWIRAttr =
    &DemIndicatorAttribute[pEventCfg->AttrStartIndex];

    while (iloop > 0)
    {
        iloop--;
        if (pWIRAttr->DemIndicatorFailureCycleCounterThreshold
        <= pEvent->FailedCycles)
        {
            res = TRUE;
#if(DEM_CONFIG_VARIANT != DEM_CONFIG_POST_BUILD)
            DemWIRState[pWIRAttr->DemIndicatorRef] = pWIRAttr
            ->DemIndicatorBehaviour;
#else
            DemWIRState[pWIRAttr->DemIndicatorRef] = DemPbCfgPtr->DemIndicatorBehaviour[pWIRAttr->DemIndicatorRef];
#endif
        }
        pWIRAttr++;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckWIROff>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEvent && pEventCfg>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <boolean>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(boolean, DEM_CODE) Dem_CheckWIROff(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    boolean res = FALSE;
    uint8 iloop = pEventCfg->AttrNum;
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_CONST) pWIRAttr =
    &DemIndicatorAttribute[pEventCfg->AttrStartIndex];

    while (iloop > 0)
    {
        iloop--;
        if (pWIRAttr->DemIndicatorHealingCycleCounterThreshold
        <= pEvent->HealingCounter)
        {
            DemWIRState[pWIRAttr->DemIndicatorRef] = DEM_INDICATOR_OFF;
        }
        else
        {
            res = TRUE;
        }
        pWIRAttr++;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * @Brief               <Dem_EventAgingProcess>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OpId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEvent>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_EventAgingProcess(
P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent,
uint8 OpId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[pEvent->IntId];
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    uint8 iloop = DEM_MEM_DEST_MAX_NUM_OF_DTC;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

    if (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
    {
        if(0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
        {
            return;
        }
#if((DEM_INDICATOR_NUM > 0))
        /* Process Healing Counter */
        if ((pEventCfg->DemOperationCycleRef == OpId)
        && (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
        && (pEvent->HealingCounter < 0xFF))
        {
            pEvent->HealingCounter++;
            /* Process Healing */
            if (FALSE != Dem_CheckWIROff(pEvent, pEventCfg))
            {
                return;
            }
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_INTERN)
            pEvent->AgingCounter = 0;
#endif
        }
#endif

        if (FALSE == Dem_CheckEventMemEntryExistsAlready(pEvent->IntId))
        {
            return;
        }
        pDTCAttrCfg = &DemDTCAttributes[DemDTC[pEventCfg->DemDTCRef]
        .DemDTCAttributesRef];
        if (pDTCAttrCfg->DemAgingAllowed == FALSE)
        {
            return;
        }
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_INTERN)
        if (((pEventCfg->DemOperationCycleRef == OpId)
            || (pDTCAttrCfg->DemAgingCycleRef == OpId))
        && (pEvent->AgingCounter < 0xFF)
#if(DEM_AGING_REQUIERES_TESTED_CYCLE == STD_ON)
        && (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
#endif
        )
        {
            pEvent->AgingCounter++;
        }
#endif

#if(DEM_STATUS_BIT_AGING_AND_DISPLACEMENT == DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR)
        if(pEvent->AgingCounter >= pDTCAttrCfg->DemAgingCycleCounterThresholdForTFSLC)
        {
            /* SWS_Dem_01054 */
            DEM_FLAGS_CLR(pEvent->UdsStatus,DEM_UDS_STATUS_TFSLC);
        }
#endif
        /* Process Aging */
        if (pEvent->AgingCounter >= pDTCAttrCfg->DemAgingCycleCounterThreshold)
        {
            /* SWS_Dem_00498 */
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC);
            pEvent->AgingCounter = 0x00;
            pMemDest = pDTCAttrCfg->DemMemoryDestinationRef;
            while (iloop > 0)
            {
                iloop--;
                if (*pMemDest != DEM_MEM_DEST_INVALID)
                {
                    pEntry = Dem_MemEntryGet(
                    Dem_GetEventExternalId(pEvent->IntId), *pMemDest);
                    if (pEntry != NULL_PTR)
                    {
                        DemUpdateNvmFlag[pEvent->IntId] = TRUE;
                        Dem_MemEntryDelete(pEntry);
                    }
                }
                pMemDest++;
            }
        }
    }
    else
    {
        /* Process Healing Counter */
        if (pEventCfg->DemOperationCycleRef == OpId)
        {
            pEvent->HealingCounter=0;
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_INTERN)
            pEvent->AgingCounter=0;
#endif
        }

    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_OperationCycleCounterProcess>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEvent>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_OperationCycleCounterProcess(Dem_EventInfoType *pEvent)
{
    if (pEvent->OperationCycleCntStarted == TRUE)
    {
        if (pEvent->CyclesSinceFirstFailed < 0xFF)
        {
            pEvent->CyclesSinceFirstFailed++;
        }

        if(0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
        {
            /* Test Failed This Operation Cycle */
            pEvent->CyclesSinceLastFailed = 0;
            if(pEvent->FailedCycles < 0xFF)
            {
                pEvent->FailedCycles++;
            }
        }
        if (pEvent->CyclesSinceLastFailed < 0xFF)
        {
            pEvent->CyclesSinceLastFailed++;
        }
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_NVRAM_BLOCKID_NUM > 0)

/*************************************************************************/
/*
 * @Brief               <Dem_ReadNvRAM>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEvent>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ReadNvRAM()
{
    Std_ReturnType retVal = E_OK;	  
    uint16 iloop = 0;
    uint16 tempEventId;
    uint32 tempAddr1 = DEM_STORAGE_BASE_ADDR1;  
    uint32 tempAddr2 = DEM_STORAGE_BASE_ADDR2;  
    while (iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY)
    {
        DataStore_UdsRead(tempAddr1, (uint8*UDS_FAR)&DemPrimaryMemory[iloop], ENTRY_LEN, 0); //ExternFlashIf_ReadData(tempAddr1, (uint8*UDS_FAR)&DemPrimaryMemory[iloop], ENTRY_LEN);                  
        tempEventId = DemPrimaryMemory[iloop].EventId;       
        if((tempEventId > 0) && (tempEventId <= DEM_EVENT_PARAMETER_NUM))
        {
            tempEventId = Dem_GetEventInternalId(tempEventId);
            DataStore_UdsRead(tempAddr2, (uint8*UDS_FAR)&DemEventInfo[tempEventId], STATUS_LEN, 1); //ExternFlashIf_ReadData(tempAddr2, (uint8*UDS_FAR)&DemEventInfo[tempEventId], STATUS_LEN);
        } 
        else 
        {
            Dem_MemEntryDelete(&DemPrimaryMemory[iloop]);
            Dem_ResetUdsStatus(tempEventId);
        }
        tempAddr1 += ENTRY_LEN; 
        tempAddr2 += STATUS_LEN; 
        iloop++;
    }
    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/*************************************************************************/
/*
 * @Brief               <Dem_EntryInit>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EntryInit(void)
{
    uint8 iloop = DEM_MEM_DEST_TOTAL_NUM;
    P2CONST(Dem_MemDestConfigType, AUTOMATIC, DEM_CONST) pMemCfg = DemMemDestCfg;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    uint8 EntryNum;
    while (iloop > 0)
    {
        iloop--;
        EntryNum = pMemCfg->EntryNum;
        pEntry = pMemCfg->EntryList;
        while (EntryNum > 0)
        {
            Dem_MemEntryDelete(pEntry);
            pEntry++;
            EntryNum--;
        }
        pMemCfg++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * @Brief               <Dem_ResetUdsStatus>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ResetUdsStatus(uint16 EventId)
{
    uint16 tempEventId = 0;
    if((EventId > 0) && (EventId <= DEM_EVENT_PARAMETER_NUM)) 
    {
        tempEventId = Dem_GetEventInternalId(EventId);
        DemEventInfo[tempEventId].IntId = tempEventId;
        DemEventInfo[tempEventId].Status = 0x1f;
        DemEventInfo[tempEventId].UdsStatus = 0x50;
        DemEventInfo[tempEventId].OccurrenceCounter = 0;  
        DemEventInfo[tempEventId].FailureCounter = 0;
        DemEventInfo[tempEventId].HealingCounter = 0;
        DemEventInfo[tempEventId].AgingCounter = 0;
        DemEventInfo[tempEventId].OperationCycleCntStarted = FALSE;
        DemEventInfo[tempEventId].CyclesSinceFirstFailed = 0;
        DemEventInfo[tempEventId].CyclesSinceLastFailed = 0;
        DemEventInfo[tempEventId].FailedCycles = 0;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_UpdateEntry>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_UpdateEntry()
{
    uint16 iloop = 0;
    uint16 iiloop = 0;
    uint16 tempEventId = 0;
    uint32 tempAddr1 = DEM_STORAGE_BASE_ADDR1; 
    Dem_EventMemEntryType TempMemEntry;
    uint8 writeflag = 0;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {         
        if(DemUpdateNvmFlag[iloop] == TRUE) /* the related event is tested failed */
        {
            tempEventId = Dem_GetEventExternalId(iloop);
            while (iiloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY)
            {
                DataStore_UdsRead(tempAddr1, (uint8*UDS_FAR)&TempMemEntry, ENTRY_LEN, 0); //ExternFlashIf_ReadData(tempAddr1, (uint8*UDS_FAR)&TempMemEntry, ENTRY_LEN);        
                if((DemPrimaryMemory[iiloop].EventId == tempEventId) 
                           || (TempMemEntry.EventId != DemPrimaryMemory[iiloop].EventId)) 
                {                         
                    writeflag = 1; //ExternFlashIf_WriteData(tempAddr1, (uint8*UDS_FAR)&DemPrimaryMemory[iiloop], ENTRY_LEN);
                }      
                tempAddr1 += ENTRY_LEN;               
                iiloop++;
            } 
            DemUpdateNvmFlag[iloop] = FALSE;
        }  
        iloop++;
    } 
    if(writeflag) 
    {
       DataStore_UdsWrite((uint8*UDS_FAR)&DemEventInfo[0], 0);
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_UpdateStatus>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_UpdateStatus()
{
    uint16 iloop = 0;
    uint16 iiloop = 0;
    uint16 tempEventId = 0;
    uint32 tempAddr2 = DEM_STORAGE_BASE_ADDR2; 
    uint8 writeflag = 0;
    while (iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY)
    {
        tempEventId = DemPrimaryMemory[iloop].EventId;   
        
        if((tempEventId > 0) && (tempEventId <= DEM_EVENT_PARAMETER_NUM))
        {
            tempEventId = Dem_GetEventInternalId(tempEventId);                         
            writeflag = 1; //ExternFlashIf_WriteData(tempAddr2, (uint8*UDS_FAR)&DemEventInfo[tempEventId], STATUS_LEN);
        }
        tempAddr2 += STATUS_LEN;                          
        iloop++;  
    }  
    if(writeflag) 
    {
       DataStore_UdsWrite((uint8*UDS_FAR)&DemEventInfo[0], 1);
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ClearAllInfo>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ClearAllInfo()
{
    uint16 iloop = 0;
    uint16 tempEventId = 0; 
    uint32 tempAddr1 = DEM_STORAGE_BASE_ADDR1; 
    uint32 tempAddr2 = DEM_STORAGE_BASE_ADDR2; 
    if(DemSetClearDtcFlag == TRUE) 
    {
        while (iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY)
        {
            tempEventId = DemPrimaryMemory[iloop].EventId;   
            tempEventId = Dem_GetEventInternalId(iloop);                                    
            //ExternFlashIf_WriteData(tempAddr1, (uint8*UDS_FAR)&DemPrimaryMemory[iloop], ENTRY_LEN); 
            //ExternFlashIf_WriteData(tempAddr2, (uint8*UDS_FAR)&DemEventInfo[tempEventId], STATUS_LEN);     
            tempAddr1 += ENTRY_LEN; 
            tempAddr2 += STATUS_LEN;              
            iloop++;
        } 
        DemSetClearDtcFlag = FALSE;
    }    
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/********[E N D   O F   F I L E]***********************************************/
