/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem.h>
 *  @brief      <Dem>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Dec 26, 2014 - 10:17:19 AM>
 */
/*============================================================================*/

#ifndef DEM_H_
#define DEM_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Dec 26, 2014    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_MODULE_ID            54U
#define DEM_H_VENDOR_ID          0U
#define DEM_H_AR_MAJOR_VERSION   3U
#define DEM_H_AR_MINOR_VERSION   1U
#define DEM_H_AR_PATCH_VERSION   0U
#define DEM_H_SW_MAJOR_VERSION   1U
#define DEM_H_SW_MINOR_VERSION   0U
#define DEM_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_CfgTypes.h"
#include "Std_ExtendedTypes.h"

/********[Interface ECU State Manager <=> Dem]***************************/
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
extern FUNC(void, DEM_CODE)
Dem_PreInit(void);
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
extern FUNC(void, DEM_CODE)
Dem_Init(P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) ConfigPtr);
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
extern FUNC(void, DEM_CODE) Dem_Shutdown(void);
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
extern FUNC(void, DEM_CODE) Dem_ReportErrorStatus(
    Dem_EventIdType EventId,
    Dem_EventStatusType EventStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventAvailable(
    Dem_EventIdType EventId,
    boolean AvailableStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventStatus(
    Dem_EventIdType EventId,
    Dem_EventStatusType EventStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventDebounceStatus(
    Dem_EventIdType EventId,
    Dem_DebounceResetStatusType DebounceResetStatus
    );
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
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventStatus(
    Dem_EventIdType EventId
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_PrestoreFreezeFrame(
    Dem_EventIdType EventId
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPrestoredFreezeFrame(
    Dem_EventIdType EventId
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetOperationCycleState(
    uint8 OperationCycleId,
    Dem_OperationCycleStateType CycleState
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetOperationCycleState(
    uint8 OperationCycleId,
    P2VAR(Dem_OperationCycleStateType, AUTOMATIC, DEM_APPL_DATA) CycleState
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleState(
    uint8 OperationCycleId
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleCounterValue(
    uint8 CounterValue
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetWIRStatus(
    Dem_EventIdType EventId,
    boolean WIRStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetComponentFailed(
    Dem_ComponentIdType ComponentId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventStatus(
    Dem_EventIdType EventId,
    P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) EventStatusByte
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventFailed(
    Dem_EventIdType EventId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventFailed
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventTested(
    Dem_EventIdType EventId,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventTested
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDebouncingOfEvent(
    Dem_EventIdType EventId,
    P2VAR(Dem_DebouncingStateType, AUTOMATIC, DEM_APPL_DATA) DebouncingState
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCOfEvent(
    Dem_EventIdType EventId,
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTCOfEvent
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEnableCondition(
    uint8 EnableConditionID,
    boolean ConditionFulfilled
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetStorageCondition(
    uint8 StorageConditionID,
    boolean ConditionFulfilled
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetFaultDetectionCounter(
    Dem_EventIdType EventId,
    P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetIndicatorStatus(
    uint8 IndicatorId,
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetIndicatorStatus(
    uint8 IndicatorId,
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventFreezeFrameData(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    boolean ReportTotalRecord,
    uint16 DataId,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventExtendedDataRecord(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventMemoryOverflow(
    Dem_DTCOriginType DTCOrigin,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) OverflowIndication
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetNumberOfEventMemoryEntries(
    Dem_DTCOriginType DTCOrigin,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) NumberOfEventMemoryEntries
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetComponentAvailable(
    Dem_ComponentIdType ComponentId,
    boolean AvailableStatus
    );
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetDTCSuppression(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    boolean SuppressionStatus
);
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
extern FUNC(void, DEM_CODE) Dem_MainFunction(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif /* DEM_H_ */

/********[E N D   O F   F I L E]***********************************************/
