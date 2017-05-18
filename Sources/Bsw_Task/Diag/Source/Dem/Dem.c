/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem.c>
 *  @brief      <Dem>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.7  - PM 4:47:03>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.7    wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_C_AR_MAJOR_VERSION   3U
#define DEM_C_AR_MINOR_VERSION   1U
#define DEM_C_AR_PATCH_VERSION   0U
#define DEM_C_SW_MAJOR_VERSION   1U
#define DEM_C_SW_MINOR_VERSION   0U
#define DEM_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"
#include "Dem_EventMemory.h"
#include "Dem_EventQueue.h"
#include "Dem_EventDebounce.h"
#include "Dem_FreezeFrame.h"
#include "Dem_ExtendedData.h"
#include "Dem.h"
#include "Std_ExtendedTypes.h"

/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif

#if (DEM_C_AR_MAJOR_VERSION != DEM_FREEZEFRAME_H_AR_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_MINOR_VERSION != DEM_FREEZEFRAME_H_AR_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_PATCH_VERSION != DEM_FREEZEFRAME_H_AR_PATCH_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MAJOR_VERSION != DEM_FREEZEFRAME_H_SW_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MINOR_VERSION != DEM_FREEZEFRAME_H_SW_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif

#if (DEM_C_AR_MAJOR_VERSION != DEM_H_AR_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_MINOR_VERSION != DEM_H_AR_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_AR_PATCH_VERSION != DEM_H_AR_PATCH_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MAJOR_VERSION != DEM_H_SW_MAJOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif
#if (DEM_C_SW_MINOR_VERSION != DEM_H_SW_MINOR_VERSION)
  #error "Dem.c : Mismatch in Specification Major Version"
#endif

/********[M A C R O S]*********************************************************/
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) DemPbCfgPtr = NULL_PTR;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#endif
/********[I N T E R N A L   D A T A]*******************************************/
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(uint8,AUTOMATIC) DemInitState = DEM_STATE_UNINIT;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

/* OperationCycleStatus */
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(uint8,AUTOMATIC) DemOperationCycleStatus[DEM_OPERATION_CYCLE_NUM_BYTE];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_ENABLE_CONDITION_NUM > 0)
/* Enable Conditions Status Define */
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(uint8,AUTOMATIC) DemEnableConditionStatus[DEM_ENABLE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#if(DEM_STORAGE_CONDITION_NUM > 0)
/* Storage Conditions Status Define */
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(uint8,AUTOMATIC) DemStorageConditionStatus[DEM_STORAGE_CONDITION_NUM_BYTE];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(boolean,AUTOMATIC) DemChkEnableCond = FALSE;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
#if(DEM_ENABLE_CONDITION_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_SetEnableCondictionProcess(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_STORAGE_CONDITION_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_SetStorageCondictionProcess(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <Initializes the internal states necessary to
 * process events reported by BSW-modules.>
 * @ServiceId           <0x01>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <EcuM>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_PreInit(void)
{
    uint16 iloop;

    DEM_DEV_CHECK((DemInitState != DEM_STATE_UNINIT),DEM_SID_PREINIT, DEM_E_INIT_FAILED);
    /* OperationCycleStatus */
    Dem_MemSet(DemOperationCycleStatus, 0x00, DEM_OPERATION_CYCLE_NUM_BYTE);
    iloop = 0;
    while (iloop < DEM_OPERATION_CYCLE_NUM)
    {
        if (DemOperationCycle[iloop].DemOperationCycleAutostart == TRUE)
        {
            DEM_BITS_SET(DemOperationCycleStatus, iloop);
        }
        iloop++;
    }

#if(DEM_ENABLE_CONDITION_NUM > 0)
    /* Init Enable Conditions */
    Dem_MemCopy(DemEnableConditionStatus, DemEnableCondition,
        DEM_ENABLE_CONDITION_NUM_BYTE);
#endif

#if(DEM_STORAGE_CONDITION_NUM > 0)
    /* Init Storage Conditions */
    Dem_MemSet(DemStorageConditionStatus, 0x00, DEM_STORAGE_CONDITION_NUM_BYTE);
    iloop = 0;
    while (iloop < DEM_STORAGE_CONDITION_NUM)
    {
        if (DemStorageCondition[iloop].DemStorageConditionStatus == TRUE)
        {
            DEM_BITS_SET(DemStorageConditionStatus, iloop);
        }
        iloop++;
    }
#endif

    Dem_EventQueueInit();
    Dem_DebounceInit();
    Dem_EventMemInit();
    DemInitState = DEM_STATE_PRE_INIT;
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Initializes or reinitializes this module.>
 * @ServiceId           <0x02>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <ConfigPtr:Pointer to the configuration set in VARIANT-POSTBUILD.>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <EcuM>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_Init(P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) ConfigPtr)
{
    DEM_DEV_CHECK(((DemInitState != DEM_STATE_PRE_INIT)&&(DemInitState != DEM_STATE_SHUTDOWN)),DEM_SID_INIT,DEM_E_INIT_FAILED);
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
    DemPbCfgPtr = ConfigPtr;
#endif
    Dem_ReadNvRAM();
    
    Dem_FreezeFrameInit();
    Dem_DcmInit();
    DemInitState = DEM_STATE_INIT;
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Shuts down this module.>
 * @ServiceId           <0x03>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <Dem_Init>
 * @CallByAPI           <EcuM>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_Shutdown(void)
{
    DEM_DEV_CHECK((DemInitState != DEM_STATE_INIT),DEM_SID_SHUTDOWN,DEM_E_UNINIT);
    Dem_UpdateStatus();
    DemInitState = DEM_STATE_SHUTDOWN;
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[Interface BSW modules / SW-Components <=> Dem]*******************/
/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <0x0f>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Reentrant for different EventIds. Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId: Identification of an event by assigned Event ID.
 *                       EventStatus: Monitor test result>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <Dem_PreInit()>
 * @CallByAPI           <...>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ReportErrorStatus(
    Dem_EventIdType EventId,
    Dem_EventStatusType EventStatus
    )
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK((DemInitState == DEM_STATE_UNINIT),DEM_SID_REPORTERRORSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_REPORTERRORSTATUS,DEM_E_WRONG_CONFIGURATION);

    EventId = Dem_GetEventInternalId(EventId);
    pEventCfg = &DemEventParameter[EventId];

    DEM_DEV_CHECK((pEventCfg->DemEventKind != DEM_EVENT_KIND_BSW),DEM_SID_REPORTERRORSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK(((pEventCfg->AlgorithmType == DEM_DEBOUNCE_MONITOR_INTERNAL)
            && (EventStatus != DEM_EVENT_STATUS_FAILED)
            && (EventStatus != DEM_EVENT_STATUS_PASSED)),DEM_SID_REPORTERRORSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK(((DemInitState == DEM_STATE_PRE_INIT)
            &&((pEventCfg->DemReportBehavior != REPORT_BEFORE_INIT)
                || (EventStatus != DEM_EVENT_STATUS_FAILED))),DEM_SID_REPORTERRORSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK(((DemInitState == DEM_STATE_INIT)
            &&(pEventCfg->DemReportBehavior != REPORT_AFTER_INIT)),DEM_SID_REPORTERRORSTATUS,DEM_E_WRONG_CONFIGURATION);

    pEvent = Dem_GetEventInfo(EventId);
#if(DEM_ENABLE_CONDITION_NUM > 0)
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
    {
        return;
    }
#endif
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_OPERATION_CYCLE))
    {
        return;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
    {
        return;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
    {
        return;
    }
    (void)Dem_EventQueueAdd(Dem_GetEventExternalId(EventId),EventStatus);
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CONFIG_VARIANT != DEM_CONFIG_POST_BUILD)
/*************************************************************************/
/*
 * @Brief               <Set the available status of a specific Event.>
 * @ServiceId           <0x37>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <EventId : Identification of an event by assigned EventId.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                       Event shall be available (TRUE) or not (FALSE).>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful.
 *                       E_NOT_OK:change of available status not accepted>
 * @PreCondition        <DemInit>
 * @CallByAPI           <...>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* @req SWS_Dem_01106 */
FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventAvailable(
    Dem_EventIdType EventId,
    boolean AvailableStatus
    )
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState == DEM_STATE_UNINIT),DEM_SID_SETEVENTAVAILABLE,DEM_E_UNINIT);

    EventId = Dem_GetEventInternalId(EventId);
    pEvent = Dem_GetEventInfo(EventId);

    if ((NULL_PTR != Dem_CheckEventMemEntryExistsAlready(EventId))
        || DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    {
        return E_NOT_OK;
    }
    if (AvailableStatus == TRUE)
    {
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
    }
    else
    {
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * @Brief               <Processes the events reported by SW-Cs via RTE. >
 * @ServiceId           <0x04>
 * @Sync/Async          <Synchronous/Asynchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.
 *                       EventStatus: Monitor test result>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: set of event status was successful
 *                       E_NOT_OK: set of event status failed or could not be accepted>
 * @PreCondition        <Dem_Init>
 * @CallByAPI           <...>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventStatus(
    Dem_EventIdType EventId,
    Dem_EventStatusType EventStatus
    )
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_SETEVENTSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_SETEVENTSTATUS,DEM_E_WRONG_CONFIGURATION);
    
    EventId = Dem_GetEventInternalId(EventId);
    pEventCfg = &DemEventParameter[EventId];
    
    DEM_DEV_CHECK_R((pEventCfg->DemEventKind != DEM_EVENT_KIND_SWC),DEM_SID_SETEVENTSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R(((pEventCfg->AlgorithmType == DEM_DEBOUNCE_MONITOR_INTERNAL)
            && (EventStatus != DEM_EVENT_STATUS_FAILED)
            && (EventStatus != DEM_EVENT_STATUS_PASSED)),DEM_SID_SETEVENTSTATUS,DEM_E_WRONG_CONFIGURATION);

    pEvent = Dem_GetEventInfo(EventId);
#if(DEM_ENABLE_CONDITION_NUM > 0)
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
    {
        return E_NOT_OK;
    }
#endif
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_OPERATION_CYCLE))
    {
        return E_NOT_OK;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
    {
        return E_NOT_OK;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    return Dem_EventQueueAdd(Dem_GetEventExternalId(EventId),EventStatus);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Control the internal debounce counter/timer by BSW modules and SW-Cs.>
 * @ServiceId           <0x09>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.
 *                       DebounceResetStatus :  Freeze or reset the internal debounce counter
 *                       /timer of the specified event.>
 * @Param-Name[out]     <None...>
 * @Param-Name[in/out]  <None...>
 * @Return              <None...>
 * @PreCondition        <Dem_PreInit>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventDebounceStatus(
    Dem_EventIdType EventId,
    Dem_DebounceResetStatusType DebounceResetStatus
    )
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState < DEM_STATE_PRE_INIT),DEM_SID_RESETEVENTDEBOUNCESTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((EventId > DEM_EVENT_PARAMETER_NUM),DEM_SID_RESETEVENTDEBOUNCESTATUS,DEM_E_WRONG_CONFIGURATION);
    EventId = Dem_GetEventInternalId(EventId);

    pEvent = Dem_GetEventInfo(EventId);
    if(DebounceResetStatus == DEM_DEBOUNCE_STATUS_RESET)
    {
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_RESET);
    }
    else
    {
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_RESET);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Resets the event failed status. >
 * @ServiceId           <0x05>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId Identification of an event by assigned EventId.>
 * @Param-Name[out]     <None...>
 * @Param-Name[in/out]  <None...>
 * @Return              <E_OK: reset of event status was successful
 *                       E_NOT_OK: reset of event status failed or is not allowed,
 *                       because the event is already tested in this operation cycle>
 * @PreCondition        <description...>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventStatus(
    Dem_EventIdType EventId
    )
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_RESETEVENTSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((EventId > DEM_EVENT_PARAMETER_NUM),DEM_SID_RESETEVENTSTATUS,DEM_E_WRONG_CONFIGURATION);

    EventId = Dem_GetEventInternalId(EventId);

    pEvent = Dem_GetEventInfo(EventId);
    /* [SWS_Dem_00638] d The function Dem_ResetEventStatus shall return E_NOT_OK,
     * if the event was already tested this operation cycle (UDS DTC status bit 6
     * - TestNotCompletedThisOperationCycle is set to 0). c*/
    if(0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
    {
        return E_NOT_OK;
    }
    /* Figure 7.15: DTC status bit 0 TestFailed logic */
    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TF);

    Dem_DebounceReset(EventId);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Captures the freeze frame data for a specific event.>
 * @ServiceId           <0x06>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the sameEventId.>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId>
 * @Param-Name[out]     <None...>
 * @Param-Name[in/out]  <None...>
 * @Return              <None...>
 * @PreCondition        <{ecuc(Dem/DemConfigSet/DemEventParameter.DemFFPrestorageSupported)} == true>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_PrestoreFreezeFrame(
    Dem_EventIdType EventId
    )
{
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_PRESTOREFREEZEFRAME,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((EventId > DEM_EVENT_PARAMETER_NUM),DEM_SID_PRESTOREFREEZEFRAME,DEM_E_WRONG_CONFIGURATION);

    EventId = Dem_GetEventInternalId(EventId);
    pEventCfg = &DemEventParameter[EventId];

    DEM_DEV_CHECK_R((pEventCfg->DemFFPrestorageSupported == FALSE),DEM_SID_PRESTOREFREEZEFRAME,DEM_E_WRONG_CONFIGURATION);

    return Dem_PreStoreFF(Dem_GetEventExternalId(EventId));
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Clears a prestored freeze frame of a specific event.>
 * @ServiceId           <0x07>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId : Identification of an event by assigned EventId.>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPrestoredFreezeFrame(
    Dem_EventIdType EventId
    )
{
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT), DEM_SID_CLEARPRESTOREDFREEZEFRAME, DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)), DEM_SID_CLEARPRESTOREDFREEZEFRAME,
        DEM_E_WRONG_CONFIGURATION);

    EventId = Dem_GetEventInternalId(EventId);

    DEM_DEV_CHECK_R(
        (DemEventParameter[EventId].DemFFPrestorageSupported == FALSE), DEM_SID_CLEARPRESTOREDFREEZEFRAME,
        DEM_E_WRONG_CONFIGURATION);

    return Dem_ClearPreStoreFF(Dem_GetEventExternalId(EventId));
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets an operation cycle state. >
 * @ServiceId           <0x08>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OperationCycleId: Identification of operation cycle, like power cycle,
 *                                         driving cycle.
 *                       CycleState: New operation cycle state: (re-)start or end>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: set of operation cycle was accepted and will be handled asynchronously
 *                       E_NOT_OK: set of operation cycle was rejected>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetOperationCycleState(
    uint8 OperationCycleId,
    Dem_OperationCycleStateType CycleState
    )
{
    boolean IsRestart = FALSE;
    DEM_DEV_CHECK_R((DemInitState == DEM_STATE_UNINIT), DEM_SID_SETOPERATIONCYCLESTATE, DEM_E_UNINIT);
    DEM_DEV_CHECK_R((OperationCycleId >= DEM_OPERATION_CYCLE_NUM), DEM_SID_SETOPERATIONCYCLESTATE,
        DEM_E_WRONG_CONFIGURATION);

    if (CycleState == DEM_CYCLE_STATE_START)
    {
        if (0x00 != DEM_BITS_ISSET(DemOperationCycleStatus, OperationCycleId))
        {
            IsRestart = TRUE;
        }
        else
        {
            DEM_BITS_SET(DemOperationCycleStatus, OperationCycleId);
        }
        Dem_OperationCycleStart(OperationCycleId, IsRestart);
    }
    else
    {
        if (0x00 != DEM_BITS_ISSET(DemOperationCycleStatus, OperationCycleId))
        {
            Dem_OperationCycleEnd(OperationCycleId);
            DEM_BITS_CLR(DemOperationCycleStatus, OperationCycleId);
        }
    }

    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets information about the status of a specific operation cycle.>
 * @ServiceId           <0x9e>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OperationCycleId : Identification of operation cycle, like power cycle,
 *                                          driving cycle.>
 * @Param-Name[out]     <CycleState : Cycle status information>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: read out of operation cycle was successful
 *                       E_NOT_OK: read out of operation cycle failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetOperationCycleState(
    uint8 OperationCycleId,
    P2VAR(Dem_OperationCycleStateType, AUTOMATIC, DEM_APPL_DATA) CycleState
)
{
    DEM_DEV_CHECK_R((DemInitState == DEM_STATE_UNINIT),DEM_SID_GETOPERATIONCYCLESTATE,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((OperationCycleId >= DEM_OPERATION_CYCLE_NUM),DEM_SID_GETOPERATIONCYCLESTATE,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((CycleState == NULL_PTR),DEM_SID_GETOPERATIONCYCLESTATE,DEM_E_PARAM_POINTER);

    if(0x00 == DEM_BITS_ISSET(DemOperationCycleStatus,OperationCycleId))
    {
        *CycleState = DEM_CYCLE_STATE_END;
    }
    else
    {
        *CycleState = DEM_CYCLE_STATE_START;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Triggers the next aging cycle state.>
 * @ServiceId           <0x11>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OperationCycleId:Identification of aging cycle.>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: set of aging cycle was successful
 *                       E_NOT_OK: set of aging cycle failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleState(
    uint8 OperationCycleId
    )
{
    DEM_DEV_CHECK_R((DemInitState == DEM_STATE_UNINIT), DEM_SID_SETAGINGCYCLESTATE, DEM_E_UNINIT);
    DEM_DEV_CHECK_R((OperationCycleId >= DEM_OPERATION_CYCLE_NUM), DEM_SID_SETAGINGCYCLESTATE,
        DEM_E_WRONG_CONFIGURATION);

    DemAgingCycle = OperationCycleId;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Provides the value of the external aging counter.>
 * @ServiceId           <0x12>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <CounterValue: Current external aging counter value.>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: set of aging counter was successful
 *                       E_NOT_OK: set of aging counter failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleCounterValue(
    uint8 CounterValue
    )
{
#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
    DEM_DEV_CHECK_R((DemInitState == DEM_STATE_UNINIT),DEM_SID_SETAGINGCYCLECOUNTERVALUE,DEM_E_UNINIT);

    DemExternAgingCnt = CounterValue;
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets the WIR status bit via failsafe SW-Cs.>
 * @ServiceId           <0x7a>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.>
 * @Param-Name[in]      <EventId: Identification of an event by assigned EventId.
 *                       WIRStatus: Requested status of event related WIR-bit>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: The request is accepted
 *                       E_NOT_OK: not be accepted >
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetWIRStatus(
    Dem_EventIdType EventId,
    boolean WIRStatus
    )
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_SETWIRSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_SETWIRSTATUS,DEM_E_WRONG_CONFIGURATION);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_DTC_SETTING))
    {
        return E_NOT_OK;
    }
    if(WIRStatus == TRUE)
    {
        DEM_FLAGS_SET(pEvent->UdsStatus,DEM_UDS_STATUS_WIR);
    }
    else
    {
        DEM_FLAGS_CLR(pEvent->UdsStatus,DEM_UDS_STATUS_WIR);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <0x2a>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ComponentId:Identification of a DemComponent>
 * @Param-Name[out]     <ComponentFailed: TRUE: failed FALSE: not failed>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: getting "ComponentFailed" was successful
 *                       E_NOT_OK: getting "ComponentFailed" was not successful>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetComponentFailed(
    Dem_ComponentIdType ComponentId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
)
{
    /* TODO: */
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the current extended event status of an event.>
 * @ServiceId           <0x0a>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.>
 * @Param-Name[out]     <EventStatusByte:UDS DTC status byte of the requested event>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: get of event status was successful
 *                       E_NOT_OK: get of event status failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventStatus(
    Dem_EventIdType EventId,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) EventStatusByte
)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETEVENTSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((EventStatusByte == NULL_PTR),DEM_SID_GETEVENTSTATUS,DEM_E_PARAM_POINTER);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    *EventStatusByte = pEvent->UdsStatus;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the event failed status of an event.>
 * @ServiceId           <0x0b>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId: Identification of an event by assigned EventId.>
 * @Param-Name[out]     <EventFailed: TRUE - Last Failed FALSE - not Last Failed>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: get of "EventFailed" was successful
 *                       E_NOT_OK: get of "EventFailed" was not successful>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventFailed(
    Dem_EventIdType EventId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventFailed
)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTFAILED,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETEVENTFAILED,DEM_E_WRONG_CONFIGURATION);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus,DEM_UDS_STATUS_TF))
    {
        *EventFailed = FALSE;
    }
    else
    {
        *EventFailed = TRUE;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the event tested status of an event.>
 * @ServiceId           <0x0c>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.>
 * @Param-Name[out]     <EventTested: TRUE - event tested this cycle
 *                                    FALSE - event not tested this cycle>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: get of event state "tested" successful
 *                       E_NOT_OK: get of event state "tested" failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventTested(
    Dem_EventIdType EventId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventTested
)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTTESTED,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETEVENTTESTED,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((EventTested == NULL_PTR),DEM_SID_GETEVENTTESTED,DEM_E_PARAM_POINTER);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_TESTED))
    {
        *EventTested = FALSE;
    }
    else
    {
        *EventTested = TRUE;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the debouncing status of an event.>
 * @ServiceId           <0x9f>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.>
 * @Param-Name[out]     <DebouncingState>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: get of debouncing status per event state successful
 *                       E_NOT_OK: get of debouncing per event state failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetDebouncingOfEvent(
    Dem_EventIdType EventId,
    P2VAR(Dem_DebouncingStateType, AUTOMATIC, DEM_APPL_DATA) DebouncingState
)
{
    sint8 FDC = 0;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETDEBOUNCINGOFEVENT,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETDEBOUNCINGOFEVENT,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((DebouncingState == NULL_PTR),DEM_SID_GETDEBOUNCINGOFEVENT,DEM_E_PARAM_POINTER);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    FDC = Dem_GetInteralFDC(Dem_GetEventInternalId(EventId));

    *DebouncingState = 0x00;
    if(FDC == -128)
    {
        *DebouncingState |= DEM_TEST_COMPLETE;
    }
    else if(FDC < 0)
    {
        *DebouncingState |= DEM_TEMPORARILY_HEALED;
    }
    else if(FDC == 0)
    {
        ;/* Do Nothing */
    }
    else if(FDC < 127)
    {
        *DebouncingState |= DEM_TEMPORARILY_DEFECTIVE;
    }
    else
    {
        *DebouncingState |= DEM_TEST_COMPLETE;
    }
    /* TODO: Check Bit4 */
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the DTC of an event.>
 * @ServiceId           <0x0d>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId:Identification of an event by assigned EventId.
 *                       DTCFormat: Defines the output-format of the requested DTCvalue.>
 * @Param-Name[out]     <DTCOfEvent: Receives the DTC value in respective format returned
 *                                   by this function.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: get of DTC was successful
 *                       E_NOT_OK: the call was not successful
 *                       DEM_E_NO_DTC_AVAILABLE: there is no DTC configured in the requested format>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCOfEvent(
    Dem_EventIdType EventId,
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTCOfEvent
)
{
    Std_ReturnType res = E_NOT_OK;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETDTCOFEVENT,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETDTCOFEVENT,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((DTCOfEvent == NULL_PTR),DEM_SID_GETDTCOFEVENT,DEM_E_PARAM_POINTER);

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    *DTCOfEvent = 0x00;
    EventId = Dem_GetEventInternalId(EventId);
    pEventCfg = &DemEventParameter[EventId];
    if(pEventCfg->DemDTCRef != DEM_DTC_INVALID)
    {
        switch(DTCFormat)
        {
            case DEM_DTC_FORMAT_OBD:
            /* TODO: */
            break;
            case DEM_DTC_FORMAT_UDS:
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_PRE_COMPILE)
            *DTCOfEvent = DemDTC[pEventCfg->DemDTCRef].DemDtcValue;
#else
            *DTCOfEvent = DemPbCfgPtr->DemDtcValue[pEventCfg->DemDTCRef];
#endif
            break;
            case DEM_DTC_FORMAT_J1939:
            /* TODO: */
            break;
            default:
            break;
        }
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets an enable condition.>
 * @ServiceId           <0x39>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EnableConditionID:This parameter identifies the enable condition.
 *                       ConditionFulfilled: This parameter specifies whether the enable condition
 *                       assigned to the EnableConditionID is fulfilled
 *                       (TRUE) or not fulfilled (FALSE).>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <In case the enable condition could be set successfully the API call returns E_OK.
 *                      If the setting of the enable condition failed the return value of the
 *                      function is E_NOT_OK.>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetEnableCondition(
    uint8 EnableConditionID,
    boolean ConditionFulfilled
    )
{
#if(DEM_ENABLE_CONDITION_NUM > 0)
    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT), DEM_SID_SETENABLECONDITION, DEM_E_UNINIT);
    DEM_DEV_CHECK_R((EnableConditionID >= DEM_ENABLE_CONDITION_NUM), DEM_SID_SETENABLECONDITION,
        DEM_E_WRONG_CONFIGURATION);

    if (ConditionFulfilled == FALSE)
    {
        DEM_BITS_CLR(DemEnableConditionStatus, EnableConditionID);
    }
    else
    {
        DEM_BITS_SET(DemEnableConditionStatus, EnableConditionID);
    }
    DemChkEnableCond = TRUE;
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets a storage condition.>
 * @ServiceId           <0x38>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <StorageConditionID: This parameter identifies the storage condition.
 *                       ConditionFulfilled: This parameter specifies whether the storage condition
 *                       assigned to the StorageConditionID is fulfilled (TRUE) or not
 *                       fulfilled (FALSE).>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <In case the storage condition could be set successfully the API call
 *                      returns E_OK. If the setting of the storage condition failed the
 *                      return value of the function is E_NOT_OK.>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetStorageCondition(
    uint8 StorageConditionID,
    boolean ConditionFulfilled
    )
{
#if(DEM_STORAGE_CONDITION_NUM > 0)
    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT), DEM_SID_SETSTORAGECONDITION, DEM_E_UNINIT);
    DEM_DEV_CHECK_R((StorageConditionID >= DEM_STORAGE_CONDITION_NUM), DEM_SID_SETSTORAGECONDITION,
        DEM_E_WRONG_CONFIGURATION);

    if (ConditionFulfilled == FALSE)
    {
        DEM_BITS_CLR(DemStorageConditionStatus, StorageConditionID);
    }
    else
    {
        DEM_BITS_SET(DemStorageConditionStatus, StorageConditionID);
    }
    Dem_SetStorageCondictionProcess();
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the fault detection counter of an event. >
 * @ServiceId           <0x3e>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <EventId : Identification of an event by assigned EventId.>
 * @Param-Name[out]     <FaultDetectionCounter: This parameter receives the Fault Detection Counter
 *                      information of the requested EventId. If the return value of the function
 *                      call is other than E_OK this parameter does not contain valid data.
 *                      -128dec...127dec PASSED... FAILED according to ISO 14229-1>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: request was successful
 *                       E_NOT_OK: request failed
 *                       DEM_E_NO_FDC_AVAILABLE: there is no fault detection counter available
 *                       for the requested event>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetFaultDetectionCounter(
    Dem_EventIdType EventId,
    P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter
)
{
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETFAULTDETECTIONCOUNTER,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETFAULTDETECTIONCOUNTER,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((FaultDetectionCounter == NULL_PTR),DEM_SID_GETFAULTDETECTIONCOUNTER,DEM_E_PARAM_POINTER);
    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    *FaultDetectionCounter = Dem_GetInteralFDC(Dem_GetEventInternalId(EventId));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the indicator status derived from the event status.>
 * @ServiceId           <0x29>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <IndicatorId: Number of indicator>
 * @Param-Name[out]     <IndicatorStatus: Status of the indicator, like off, on, or blinking.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful E_NOT_OK: Operation failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetIndicatorStatus(
    uint8 IndicatorId,
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus
)
{
#if(DEM_INDICATOR_NUM > 0)
    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETINDICATORSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((IndicatorId >= DEM_INDICATOR_NUM),DEM_SID_GETINDICATORSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((IndicatorStatus == NULL_PTR),DEM_SID_GETINDICATORSTATUS,DEM_E_PARAM_POINTER);
    *IndicatorStatus = DemWIRState[IndicatorId];
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets the indicator status included in the event status.>
 * @ServiceId           <0xa1>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <IndicatorId : Number of indicator
 *                      IndicatorStatus Status of the indicator, like off, on, or blinking.>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetIndicatorStatus(
    uint8 IndicatorId,
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus
)
{
#if(DEM_INDICATOR_NUM > 0)
    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_SETINDICATORSTATUS,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((IndicatorId >= DEM_INDICATOR_NUM),DEM_SID_SETINDICATORSTATUS,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((IndicatorStatus == NULL_PTR),DEM_SID_SETINDICATORSTATUS,DEM_E_PARAM_POINTER);
    DemWIRState[IndicatorId] = *IndicatorStatus;
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the data of a freeze frame by event.>
 * @ServiceId           <0x31>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId: Identification of an event by assigned EventId.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                       frame record as defined in ISO15031-5 and ISO14229-1.
 *                       0xFF means most recent freeze frame record is returned.
 *                       ReportTotalRecord: This parameter is obsolete and shall be set to FALSE.
 *                       This function requests a single PID/DID.
 *                       DataId : This parameter specifies the PID (ISO15031-5 mapped in UDS range
 *                       0xF400 - 0xF4FF) or DID (ISO14229-1) that shall be copied to
 *                       the destination buffer.>
 * @Param-Name[out]     <DestBuffer : This parameter contains a byte pointer that points to the
 *                       buffer, to which the freeze frame data record shall be written to.
 *                       The format is raw hexadecimal values and contains no header-information.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently stored
 *                                              (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported
 *                                              by the event
 *                       DEM_E_WRONG_DIDNUMBER: The requested DID is not supported by the freeze frame>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventFreezeFrameData(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    boolean ReportTotalRecord, /* not used */
    uint16 DataId,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTFREEZEFRAMEDATA,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETEVENTFREEZEFRAMEDATA,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((DestBuffer == NULL_PTR),DEM_SID_GETEVENTFREEZEFRAMEDATA,DEM_E_PARAM_POINTER);
    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    pEntry = Dem_CheckEventMemEntryExistsAlready(Dem_GetEventInternalId(EventId));
    if(pEntry == NULL_PTR)
    {
        return DEM_E_NODATAAVAILABLE;
    }
    return Dem_FreezeFrameGetFromEntry(pEntry,RecordNumber,DataId,DestBuffer);
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the data of an extended data record by event.>
 * @ServiceId           <0x30>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <EventId : Identification of an event by assigned EventId.
 *                       RecordNumber: Identification of requested Extended data record.
 *                       Valid values are between 0x01 and 0xEF as defined in ISO14229-1.>
 * @Param-Name[out]     <DestBuffer: This parameter contains a byte pointer that points
 *                          to the buffer, to which the extended data shall bewritten to.
 *                          The format is raw hexadecimal values and contains no header-information.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently
 *                              stored (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported by the event>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventExtendedDataRecord(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer
)
{
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTEXTENDEDDATARECORD,DEM_E_UNINIT);
    DEM_DEV_CHECK_R(((EventId > DEM_EVENT_PARAMETER_NUM)||(EventId == 0)),DEM_SID_GETEVENTEXTENDEDDATARECORD,DEM_E_WRONG_CONFIGURATION);
    DEM_DEV_CHECK_R((DestBuffer == NULL_PTR),DEM_SID_GETEVENTEXTENDEDDATARECORD,DEM_E_PARAM_POINTER);
    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
    if(0x00 == DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_AVAILABLE))
    {
        return E_NOT_OK;
    }
    pEntry = Dem_CheckEventMemEntryExistsAlready(Dem_GetEventInternalId(EventId));
    if(pEntry == NULL_PTR)
    {
        return DEM_E_NODATAAVAILABLE;
    }
    return Dem_ExtendedDataGetFromEntry(pEntry,RecordNumber,DestBuffer);
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the event memory overflow indication status.>
 * @ServiceId           <0x32>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCOrigin: If the Dem supports more than one event memory this parameter
 *                          is used to select the source memory the overflow indication shall be read from.>
 * @Param-Name[out]     <OverflowIndication: This parameter returns TRUE if the according
 *                          event memory was overflowed, otherwise it returns FALSE.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventMemoryOverflow(
    Dem_DTCOriginType DTCOrigin,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) OverflowIndication
)
{
    uint8 MemDest;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETEVENTMEMORYOVERFLOW,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((OverflowIndication == NULL_PTR),DEM_SID_GETEVENTMEMORYOVERFLOW,DEM_E_PARAM_POINTER);
    MemDest = Dem_GetInternalMemDest(DTCOrigin);
    DEM_DEV_CHECK_R((MemDest == DEM_MEM_DEST_INVALID),DEM_SID_GETEVENTMEMORYOVERFLOW,DEM_E_WRONG_CONFIGURATION);

    *OverflowIndication = DemMemDestInfo[MemDest].OverFlow;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Returns the number of entries currently stored in the requested event memory.>
 * @ServiceId           <0x35>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCOrigin: If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the number of entries shall be read from.>
 * @Param-Name[out]     <NumberOfEventMemoryEntries: Number of entries currently stored in the
 *                          requested event memory.>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK: Operation was successful E_NOT_OK: Operation failed>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetNumberOfEventMemoryEntries(
    Dem_DTCOriginType DTCOrigin,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) NumberOfEventMemoryEntries
)
{
    uint8 MemDest;

    DEM_DEV_CHECK_R((DemInitState != DEM_STATE_INIT),DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES,DEM_E_UNINIT);
    DEM_DEV_CHECK_R((NumberOfEventMemoryEntries == NULL_PTR),DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES,DEM_E_PARAM_POINTER);
    MemDest = Dem_GetInternalMemDest(DTCOrigin);
    DEM_DEV_CHECK_R((MemDest == DEM_MEM_DEST_INVALID),DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES,DEM_E_WRONG_CONFIGURATION);

    *NumberOfEventMemoryEntries = DemMemDestCfg[MemDest].EntryNum;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Set the availability of a specific DemComponent.>
 * @ServiceId           <0x2b>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <ComponentId: Identification of a DemComponent.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                          Component shall be available (TRUE) or not (FALSE).>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <E_OK: Operation was successful>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetComponentAvailable(
    Dem_ComponentIdType ComponentId,
    boolean AvailableStatus
    )
{
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/*************************************************************************/
/*
 * @Brief               <Set the suppression status of a specific DTC.>
 * @ServiceId           <0x33>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC: Diagnostic Trouble Code
 *                       DTCFormat: Defines the input-format of the provided DTC value.
 *                       SuppressionStatus: This parameter specifies whether the respective
 *                       DTC shall be disabled (TRUE) or enabled (FALSE).>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <E_OK (Operation was successful),
 *                       E_NOT_OK (operation failed or event entry for this DTC still exists)>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDTCSuppression(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    boolean SuppressionStatus
)
{
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/********[Scheduled functions]*********/
/*************************************************************************/
/*
 * @Brief               <Processes all not event based Dem internal functions.>
 * @ServiceId           <0x55>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <Dem_Init>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MainFunction(void)
{
    if (DemInitState != DEM_STATE_INIT)
    {
        return;
    }

#if(DEM_ENABLE_CONDITION_NUM > 0)
    if (DemChkEnableCond != FALSE)
    {
        DemChkEnableCond = FALSE;
        Dem_SetEnableCondictionProcess();
    }
#endif

    if (DemClearDTCSet != FALSE)
    {
        Dem_ClearDTCProcess();
        DemClearDTCSet = FALSE;
    }
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    Dem_DebounceTimerMain();
#endif
    Dem_EventQueueProcess();
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/
#if(DEM_ENABLE_CONDITION_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
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
STATIC FUNC(void, DEM_CODE) Dem_SetEnableCondictionProcess(void)
{
    uint16 iloop = 0;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    boolean IsFulfilled;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pGroup;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        IsFulfilled = TRUE;
        pEvent = Dem_GetEventInfo(iloop);
        if (DemEventParameter[iloop].DemEnableConditionGroupRef
            != DEM_ENABLE_CONDITION_GROUP_INVALID)
        {
            pGroup = &DemEnableConditionGroup[DemEventParameter[iloop]
                .DemEnableConditionGroupRef][0];
            IsFulfilled = Dem_CheckCondictionFulfilled(DemEnableConditionStatus,
                pGroup, DEM_ENABLE_CONDITION_NUM_BYTE);
        }
        if (IsFulfilled == TRUE)
        {
            if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
                /* @req SWS_Dem_00681 Monitor re-initialization  */
                Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_REENABLED);
            }
        }
        else
        {
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
            Dem_DebounceFreeze(iloop);
#endif
        }
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_STORAGE_CONDITION_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_SetStorageCondictionProcess>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous->
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
STATIC FUNC(void, DEM_CODE) Dem_SetStorageCondictionProcess(void)
{
    uint16 iloop = 0;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    boolean IsFulfilled;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pGroup;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        IsFulfilled = TRUE;
        pEvent = Dem_GetEventInfo(iloop);
        if (DemEventParameter[iloop].DemStorageConditionGroupRef
            != DEM_STORAGE_CONDITION_GROUP_INVALID)
        {
            pGroup = &DemStorageConditionGroup[DemEventParameter[iloop]
                .DemStorageConditionGroupRef][0];
            IsFulfilled = Dem_CheckCondictionFulfilled(
                DemStorageConditionStatus, pGroup,
                DEM_STORAGE_CONDITION_NUM_BYTE);
        }
        if (IsFulfilled == TRUE)
        {
            if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
                /* @req SWS_Dem_00681 Monitor re-initialization  */
                Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_STORAGE_REENABLED);
            }
        }
        else
        {
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION);
        }
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/********[E N D   O F   F I L E]***********************************************/
