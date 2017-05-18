/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventMemory.h>
 *  @brief      <Dem_EventMemory>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4  - PM 3:00:58>
 */
/*============================================================================*/

#ifndef DEM_EVENTMEMORY_H_
#define DEM_EVENTMEMORY_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTMEMORY_H_MODULE_ID            54U
#define DEM_EVENTMEMORY_H_VENDOR_ID          0U
#define DEM_EVENTMEMORY_H_AR_MAJOR_VERSION   3U
#define DEM_EVENTMEMORY_H_AR_MINOR_VERSION   1U
#define DEM_EVENTMEMORY_H_AR_PATCH_VERSION   0U
#define DEM_EVENTMEMORY_H_SW_MAJOR_VERSION   1U
#define DEM_EVENTMEMORY_H_SW_MINOR_VERSION   0U
#define DEM_EVENTMEMORY_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"
#include "Std_ExtendedTypes.h"

#define DEM_EVENT_STATUS_DTC_SETTING 0x01
#define DEM_EVENT_STATUS_OPERATION_CYCLE 0x02
#define DEM_EVENT_STATUS_ENABLED_CONDICTION 0x04
#define DEM_EVENT_STATUS_STORAGE_CONDICTION 0x08
#define DEM_EVENT_STATUS_AVAILABLE 0x10
#define DEM_EVENT_STATUS_TESTED 0x20
#define DEM_EVENT_STATUS_RESET 0x40
#define DEM_EVENT_STATUS_PASSIVE 0x80

#define ENTRY_LEN               (sizeof(Dem_EventMemEntryType))
#define STATUS_LEN              (sizeof(Dem_EventInfoType))
#define DEM_STORAGE_BASE_ADDR1   0x0000
#define DEM_STORAGE_BASE_ADDR2   0x2000
/********[M A C R O S]*********************************************************/
typedef struct
{
    Dem_EventIdType IntId;
    uint8 Status;
    uint8 UdsStatus;
    /* Counters */
    uint8 OccurrenceCounter;
    uint8 FailureCounter;
    uint8 AgingCounter;
    uint8 HealingCounter;

    /* Operation Cycle Counter */
    boolean OperationCycleCntStarted;
    uint8 CyclesSinceFirstFailed;
    uint8 CyclesSinceLastFailed;
    uint8 FailedCycles;
} Dem_EventInfoType;

typedef struct
{
    uint32 RecordNum;
    uint8 DTCRecordUpdateStatus[DEM_DTC_NUM_BYTE]; /* 1, Disable, 0:Enable */
    boolean OverFlow;
} Dem_MemDestInfoType;

/********[E X T E R N A L   D A T A]*******************************************/
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(boolean,AUTOMATIC) DemUpdateNvmFlag[DEM_EVENT_PARAMETER_NUM];;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(boolean,AUTOMATIC) DemSetClearDtcFlag;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(Dem_MemDestInfoType,AUTOMATIC) DemMemDestInfo[DEM_MEM_DEST_TOTAL_NUM];
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_INDICATOR_NUM > 0)
#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(Dem_IndicatorStatusType,AUTOMATIC) DemWIRState[DEM_INDICATOR_NUM];
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif
/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
/*************************************************************************/
/*
 * @Brief               <Dem_EventMemInit>
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
extern FUNC(void, DEM_CODE) Dem_EventMemInit(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetEventInfo>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventInfoType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
    Dem_GetEventInfo(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventTestFailed>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_EventTestFailed(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventTestPassed>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_EventTestPassed(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventRetention>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_EventRetention(Dem_EventIdType IntId,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckEventMemEntryExistsAlready>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
    Dem_CheckEventMemEntryExistsAlready(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId && MemDest>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
    Dem_MemEntryGet(Dem_EventIdType ExtId,
    uint8 MemDest);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryGetByDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <MemDest && DTCIndex>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventMemEntryType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
    Dem_MemEntryGetByDTC(uint32 DTCIndex,
    uint8 MemDest);

/*************************************************************************/
/*
 * @Brief               <Dem_MemEntryDelete>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <pEntry>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_MemEntryDelete(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_OperationCycleStart>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OpId && IsRestart>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_OperationCycleStart(uint8 OpId,
    boolean IsRestart);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_OperationCycleEnd>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <OpId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_OperationCycleEnd(uint8 OpId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ReadNvRAM();
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
extern FUNC(void, DEM_CODE) Dem_EntryInit(void);
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
extern FUNC(void, DEM_CODE) Dem_ResetUdsStatus(uint16 EventId);
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
extern FUNC(void, DEM_CODE) Dem_UpdateEntry();
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
extern FUNC(void, DEM_CODE) Dem_UpdateStatus();
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
extern FUNC(void, DEM_CODE) Dem_ClearAllInfo();
#define DEM_STOP_SEC_CODE    
#include "Dem_MemMap.h"

#endif /* DEM_DEM_EVENTMEMORY_H_ */
/********[E N D   O F   F I L E]***********************************************/
