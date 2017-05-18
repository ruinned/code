/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventDebounce.c>
 *  @brief      <Dem_EventDebounce>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4  - PM 3:03:13>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTDEBOUNCE_C_AR_MAJOR_VERSION   3U
#define DEM_EVENTDEBOUNCE_C_AR_MINOR_VERSION   1U
#define DEM_EVENTDEBOUNCE_C_AR_PATCH_VERSION   0U
#define DEM_EVENTDEBOUNCE_C_SW_MAJOR_VERSION   1U
#define DEM_EVENTDEBOUNCE_C_SW_MINOR_VERSION   0U
#define DEM_EVENTDEBOUNCE_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"
#include "Dem_EventDebounce.h"
#include "Dem_EventMemory.h"
#include "Std_ExtendedTypes.h"
/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_EVENTDEBOUNCE_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif

#if (DEM_EVENTDEBOUNCE_C_AR_MAJOR_VERSION != DEM_EVENTDEBOUNCE_H_AR_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_MINOR_VERSION != DEM_EVENTDEBOUNCE_H_AR_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_PATCH_VERSION != DEM_EVENTDEBOUNCE_H_AR_PATCH_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MAJOR_VERSION != DEM_EVENTDEBOUNCE_H_SW_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MINOR_VERSION != DEM_EVENTDEBOUNCE_H_SW_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/* Debounce Counter Base Info Define */
typedef struct
{
    sint16 InternalDebounceCounter;
    Dem_EventStatusType CurStatus;
} Dem_DebounceCounterInfoType;

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_DebounceCounterInfoType,AUTOMATIC) DemDebounceCounterInfo[DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/* Debounce Time Base Info Define */
typedef struct
{
    uint32 Timeout;
    Dem_EventIdType IntId;
    Dem_EventStatusType CurStatus;
    boolean IsFreezing;
} Dem_DebounceTimeInfoType;
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_DebounceTimeInfoType,AUTOMATIC) DemDebounceTimerInfo[DEM_DEBOUNCE_TIME_BASED_EVENT_NUM];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessTimer(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/********[I N T E R N A L   D A T A]*******************************************/

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <Dem_DebounceInit>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceInit(void)
{
    uint16 iloop;
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter = DemDebounceCounterInfo;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer = DemDebounceTimerInfo;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = DemEventParameter;
#endif

#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    iloop = DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM;
    while (iloop > 0)
    {
        iloop--;
        pCounter->InternalDebounceCounter = 0;
        pCounter->CurStatus = DEM_EVENT_STATUS_PASSED;
        pCounter++;
    }
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    iloop = DEM_DEBOUNCE_TIME_BASED_EVENT_NUM;
    while (iloop > 0)
    {
        iloop--;
        pTimer->Timeout = 0x00;
        pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
        pTimer++;
    }
    iloop = 0;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (pEventCfg->AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
        {
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].IntId = iloop;
        }
        pEventCfg++;
        iloop++;
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_DebounceReset>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[IntId];

#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer;
#endif

    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:
        pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
        pCounter->CurStatus = DEM_EVENT_STATUS_PASSED;
        pCounter->InternalDebounceCounter = 0;
        break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
        pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
        pTimer->Timeout = 0;
        pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
        break;
#endif
        default:
        break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetInteralFDC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <sint8>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(sint8, DEM_CODE) Dem_GetInteralFDC(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[IntId];
    sint8 FDC = 0;
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter;
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer;
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg;
#endif
#if(DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
    Dem_GetFDCFncType GetFnc;
#endif
    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:
            pCfgCounter =
                &DemDebounceCounterBasedClass[pEventCfg->AlgorithmRef];
            pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter == 0)
            {
                FDC = 0;
            }
            else if (pCounter->InternalDebounceCounter > 0)
            {
                FDC = (sint8)((pCounter->InternalDebounceCounter * 127)
                    / pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                FDC = (sint8)((pCounter->InternalDebounceCounter * ((sint8)(-128)))
                    / pCfgCounter->DemDebounceCounterPassedThreshold);
            }
            break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemDebounceTimeBaseClass[pEventCfg->AlgorithmRef];
            switch (pTimer->CurStatus)
            {
                case DEM_EVENT_STATUS_PASSED:
                    FDC = -128;
                    break;
                case DEM_EVENT_STATUS_FAILED:
                    FDC = 127;
                    break;
                case DEM_EVENT_STATUS_PREFAILED:
                    FDC = (sint8)((pTimer->Timeout * 127)
                        / pTimerCfg->DemDebounceTimeFailedThreshold);
                    break;
                default:
                    FDC = ((sint8)(-1))*((sint8)((pTimer->Timeout * 128)
                        / pTimerCfg->DemDebounceTimePassedThreshold));

                    break;
            }
            pTimer->Timeout = 0;
            pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
            break;
#endif
#if(DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
        case DEM_DEBOUNCE_MONITOR_INTERNAL:
            GetFnc = DemGetFDCFncs[pEventCfg->AlgorithmRef];
            FDC = 0;
            if (GetFnc != NULL_PTR)
            {
                if (E_OK != GetFnc(&FDC))
                {
                    FDC = 0;
                }
            }
            break;
#endif
        default:

            break;
    }
    return FDC;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_DebounceProcess>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEventBuffer>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventStatusType>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcess(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg =
    &DemEventParameter[Dem_GetEventInternalId(pEventBuffer->ExtId)];
    Dem_EventStatusType Status = pEventBuffer->Status;

    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:
        Status = Dem_DebounceProcessCounter(pEventBuffer, pEventCfg);
        break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
        Status = Dem_DebounceProcessTimer(pEventBuffer, pEventCfg);
        break;
#endif
        default:
        break;
    }
    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_DebounceFreeze>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceFreeze(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[IntId];

    if (pEventCfg->AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
    {
        DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].IsFreezing = TRUE;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_DebounceTimerMain>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void)
{
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer = DemDebounceTimerInfo;
    uint16 iloop = DEM_DEBOUNCE_TIME_BASED_EVENT_NUM;

    while (iloop > 0)
    {
        iloop--;
        if ((pTimer->Timeout > 0) && (pTimer->IsFreezing == FALSE))
        {
            pTimer->Timeout--;
            if (pTimer->Timeout == 0)
            {
                if (pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)
                {
                    pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;
                    Dem_EventTestFailed(pTimer->IntId, NULL_PTR);
                }
                else
                {
                    pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
                    Dem_EventTestPassed(pTimer->IntId, NULL_PTR);
                }
            }
        }
        pTimer++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_DebounceProcessCounter>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEventBuffer && pEventCfg>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventStatusType>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcessCounter(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter =
        &DemDebounceCounterBasedClass[pEventCfg->AlgorithmRef];
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter=&DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
    Dem_EventStatusType Status = pEventBuffer->Status;

    switch (Status)
    {
        case DEM_EVENT_STATUS_PASSED:
            pCounter->InternalDebounceCounter = pCfgCounter
                ->DemDebounceCounterPassedThreshold;
            break;
        case DEM_EVENT_STATUS_FAILED:
            pCounter->InternalDebounceCounter = pCfgCounter
                ->DemDebounceCounterFailedThreshold;
            break;
        case DEM_EVENT_STATUS_PREPASSED:
            {
            switch (pCounter->CurStatus)
            {
                case DEM_EVENT_STATUS_FAILED: /* Failed => PrePassed */
                case DEM_EVENT_STATUS_PREFAILED: /* PreFailed => PrePassed */
                    if (pCfgCounter->DemDebounceCounterJumpDown == TRUE)
                    {
                        pCounter->InternalDebounceCounter = pCfgCounter
                            ->DemDebounceCounterJumpDownValue;
                    }
                case DEM_EVENT_STATUS_PREPASSED: /* PrePassed => Passed? */
                    if ((pCfgCounter->DemDebounceCounterPassedThreshold +
                        pCfgCounter->DemDebounceCounterDecrementStepSize)
                        >= pCounter->InternalDebounceCounter)
                    {
                        pCounter->InternalDebounceCounter = pCfgCounter
                            ->DemDebounceCounterPassedThreshold;
                    }
                    else
                    {
                        pCounter->InternalDebounceCounter -= pCfgCounter
                            ->DemDebounceCounterDecrementStepSize;
                    }
                    break;
                default: /* Passed do nothing */
                    break;
            }
        }
            break;
        default: /* DEM_EVENT_STATUS_PREFAILED */
        {
            switch (pCounter->CurStatus)
            {
                case DEM_EVENT_STATUS_PASSED: /* Passed => Prefailed */
                case DEM_EVENT_STATUS_PREPASSED: /* PrePassed => Prefailed */
                    if (pCfgCounter->DemDebounceCounterJumpUp == TRUE)
                    {
                        pCounter->InternalDebounceCounter = pCfgCounter
                            ->DemDebounceCounterJumpUpValue;
                    }
                case DEM_EVENT_STATUS_PREFAILED: /* Prefailed => Prefailed? */
                    if ((pCfgCounter->DemDebounceCounterFailedThreshold -
                        pCfgCounter->DemDebounceCounterIncrementStepSize)
                        <= pCounter->InternalDebounceCounter)
                    {
                        pCounter->InternalDebounceCounter = pCfgCounter
                            ->DemDebounceCounterFailedThreshold;
                    }
                    else
                    {
                        pCounter->InternalDebounceCounter += pCfgCounter
                            ->DemDebounceCounterIncrementStepSize;
                    }
                    break;
                default: /* Failed do nothing */
                    break;
            }
#if(DEM_TRIGGER_ON_FDC_THRESHOLD == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
            if((pEventCfg->DemDTCRef != DEM_DTC_INVALID)
                && (DemDTCAttributes[DemDTC[pEventCfg->DemDTCRef
                    ].DemDTCAttributesRef].DemEventMemoryEntryFdcThresholdStorageValue\
 <= pCounter->InternalDebounceCounter))
            {
                Dem_EventRetention(Dem_GetEventInternalId(pEventBuffer->ExtId),pEventBuffer);
            }
#endif
        }
            break;
    }

    if (pCounter->InternalDebounceCounter
        >= pCfgCounter->DemDebounceCounterFailedThreshold)
    {
        pCounter->InternalDebounceCounter = pCfgCounter
            ->DemDebounceCounterFailedThreshold;
        pCounter->CurStatus = DEM_EVENT_STATUS_FAILED;
        Status = DEM_EVENT_STATUS_FAILED;
    }
    else if (pCounter->InternalDebounceCounter
        <= pCfgCounter->DemDebounceCounterPassedThreshold)
    {
        pCounter->InternalDebounceCounter = pCfgCounter
            ->DemDebounceCounterPassedThreshold;
        pCounter->CurStatus = DEM_EVENT_STATUS_PASSED;
        Status = DEM_EVENT_STATUS_PASSED;
    }
    else
    {
        pCounter->CurStatus = Status;
    }
    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_DebounceProcessTimer>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEventBuffer && pEventCfg>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventStatusType>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcessTimer(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    Dem_EventStatusType Status = pEventBuffer->Status;
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer=
        &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg=
        &DemDebounceTimeBaseClass[pEventCfg->AlgorithmRef];
    uint8 chkmask = DEM_EVENT_STATUS_ENABLED_CONDICTION
        | DEM_EVENT_STATUS_DTC_SETTING;
   P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    if (pTimer->IsFreezing != FALSE)
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEventBuffer->ExtId));
        if (chkmask == DEM_FLAGS_ISSET(pEvent->Status, chkmask))
        {
            pTimer->IsFreezing = FALSE;
        }
    }
    switch (Status)
    {
        case DEM_EVENT_STATUS_PASSED:
            case DEM_EVENT_STATUS_FAILED:
            pTimer->CurStatus = Status;
            pTimer->Timeout = 0;
            break;
        case DEM_EVENT_STATUS_PREPASSED:
            if ((pTimer->CurStatus == DEM_EVENT_STATUS_FAILED)
                || (pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
                pTimer->Timeout = pTimerCfg->DemDebounceTimePassedThreshold;
            }
            break;
        default: /* Prefailed */
            if ((pTimer->CurStatus == DEM_EVENT_STATUS_PASSED)
                || (pTimer->CurStatus == DEM_EVENT_STATUS_PREPASSED))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PREFAILED;
                pTimer->Timeout = pTimerCfg->DemDebounceTimeFailedThreshold;
            }
            break;
    }
    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/********[E N D   O F   F I L E]***********************************************/
