/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventQueue.c>
 *  @brief      <Dem_EventQueue>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 2:01:43>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTQUEUE_C_AR_MAJOR_VERSION   3U
#define DEM_EVENTQUEUE_C_AR_MINOR_VERSION   1U
#define DEM_EVENTQUEUE_C_AR_PATCH_VERSION   0U
#define DEM_EVENTQUEUE_C_SW_MAJOR_VERSION   1U
#define DEM_EVENTQUEUE_C_SW_MINOR_VERSION   0U
#define DEM_EVENTQUEUE_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_EventQueue.h"
#include "Dem_Common.h"
#include "Dem_EventDebounce.h"
#include "Dem_EventMemory.h"
#include "Dem_FreezeFrame.h"
#include "Dem_ExtendedData.h"
#include "Std_ExtendedTypes.h"

/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_EVENTQUEUE_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif

#if (DEM_EVENTQUEUE_C_AR_MAJOR_VERSION != DEM_EVENTMEMORY_H_AR_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_MINOR_VERSION != DEM_EVENTMEMORY_H_AR_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_PATCH_VERSION != DEM_EVENTMEMORY_H_AR_PATCH_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MAJOR_VERSION != DEM_EVENTMEMORY_H_SW_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MINOR_VERSION != DEM_EVENTMEMORY_H_SW_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif

#if (DEM_EVENTQUEUE_C_AR_MAJOR_VERSION != DEM_EVENTQUEUE_H_AR_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_MINOR_VERSION != DEM_EVENTQUEUE_H_AR_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_PATCH_VERSION != DEM_EVENTQUEUE_H_AR_PATCH_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MAJOR_VERSION != DEM_EVENTQUEUE_H_SW_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MINOR_VERSION != DEM_EVENTQUEUE_H_SW_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
typedef struct
{
    Dem_EventBufferType Queue[DEM_EVENT_QUEUE_SIZE];
    uint8 ReadIndex;
    uint8 WriteIndex;
} Dem_EventQueueType;

/********[I N T E R N A L   D A T A]*******************************************/
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_EventQueueType,AUTOMATIC) DemEventQueue;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <Dem_EventQueueInit>
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
FUNC(void, DEM_CODE) Dem_EventQueueInit(void)
{
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pBuffer = pQueue->Queue;
    uint8 iloop = DEM_EVENT_QUEUE_SIZE;

    pQueue->ReadIndex = 0x00;
    pQueue->WriteIndex = 0x00;

    while(iloop > 0)
    {
        iloop--;
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
        Dem_MemSet(pBuffer->FFData, 0xFF,DEM_FREEZE_FRAME_MAX_LEN);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
        Dem_MemSet(pBuffer->ExtData, 0xFF,DEM_EXTENDED_DATA_MAX_LEN);
#endif
#endif
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventQueueAdd>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId && Status>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_EventQueueAdd(Dem_EventIdType ExtId,
    Dem_EventStatusType Status)
{
    Std_ReturnType res = E_NOT_OK;
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pBuffer = &(pQueue->Queue[pQueue->WriteIndex]);
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(ExtId));

    if (0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_TESTED))
    {
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_TESTED);
    }

    if (pBuffer->ExtId == 0x00)
    {
        pBuffer->ExtId = ExtId;
        pBuffer->Status = Status;
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
        Dem_FreezeFrameGet(Dem_GetEventInternalId(ExtId), pBuffer->FFData);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
        Dem_ExtendedDataGet(Dem_GetEventInternalId(ExtId), pBuffer->ExtData);
#endif
#endif
        pQueue->WriteIndex++;
        if (pQueue->WriteIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->WriteIndex = 0x00;
        }
        res = E_OK;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventQueueProcess>
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
FUNC(void, DEM_CODE) Dem_EventQueueProcess(void)
{
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pBuffer = &(pQueue->Queue[pQueue->ReadIndex]);
    Dem_EventIdType IntId;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    Dem_UdsStatusByteType oldStatus;

    /* Proccess Event */
    while(pBuffer->ExtId != 0x00)
    {
        IntId = Dem_GetEventInternalId(pBuffer->ExtId);
        pEvent = Dem_GetEventInfo(IntId);
        oldStatus = pEvent->UdsStatus;
        /* Debounce */
        switch(Dem_DebounceProcess(pBuffer))
        {
            case DEM_EVENT_STATUS_PASSED:
            Dem_EventTestPassed(IntId, pBuffer);
            break;
            case DEM_EVENT_STATUS_FAILED:
            Dem_EventTestFailed(IntId, pBuffer);
            break;
            default:
            break;
        }

        if(oldStatus != pEvent->UdsStatus)
        {
            Dem_TriggerOnEventStatus(IntId,oldStatus,pEvent->UdsStatus);
        }
        /* Clear Buffer */
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;

        /* Move ReadIndex */
        pQueue->ReadIndex++;
        if(pQueue->ReadIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->ReadIndex = 0x00;
            pBuffer = pQueue->Queue;
        }
        else
        {
            pBuffer++;
        }
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/

/********[E N D   O F   F I L E]***********************************************/
