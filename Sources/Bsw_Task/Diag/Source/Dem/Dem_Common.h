/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Common.h>
 *  @brief      <Dem_Common>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4- PM2:38:18>
 */
/*============================================================================*/

#ifndef DEM_COMMON_H_
#define DEM_COMMON_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_COMMON_H_MODULE_ID            54U
#define DEM_COMMON_H_VENDOR_ID          0U
#define DEM_COMMON_H_AR_MAJOR_VERSION   3U
#define DEM_COMMON_H_AR_MINOR_VERSION   1U
#define DEM_COMMON_H_AR_PATCH_VERSION   0U
#define DEM_COMMON_H_SW_MAJOR_VERSION   1U
#define DEM_COMMON_H_SW_MINOR_VERSION   0U
#define DEM_COMMON_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Std_Types.h"
#include "Dem_Types.h"
#include "Dem_CfgTypes.h"
#if(DEM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include "Std_ExtendedTypes.h"

/********[M A C R O S]*********************************************************/
/* Dem InitState Define  */
#define DEM_STATE_UNINIT 0x02
#define DEM_STATE_PRE_INIT 0x01
#define DEM_STATE_INIT 0x00
#define DEM_STATE_SHUTDOWN 0x03

/* Check Event Lock Status */
#define DEM_CHECK_ELOCK_ALL 0xFF
#define DEM_CHECK_ELOCK_OPCYC 0x01
#define DEM_CHECK_ELOCK_ENABLECOND 0x02
#define DEM_CHECK_ELOCK_DTCSETTING 0x04
#define DEM_CHECK_ELOCK_AVAILABLE 0x08
#define DEM_CHECK_ELOCK_COMPONENT 0x10

#define DEM_CHECK_ELOCK_EXP_OPCYC 0xFE
#define DEM_CHECK_ELOCK_EXP_ENABLECOND  0xFD
#define DEM_CHECK_ELOCK_EXP_OPCYC_ENABLECOND 0xFC

#define Dem_GetEventInternalId(ExtId) (ExtId-1)
#define Dem_GetEventExternalId(IntId) (IntId+1)

/* FLAGS*/
#define DEM_FLAGS_SET(_status,_val) (_status) |= (_val)
#define DEM_FLAGS_CLR(_status,_val) (_status) &= ~(_val)
#define DEM_FLAGS_ISSET(_status,_val) ((_status)&(_val))

/* Status Bits Functions */
/* _p = &uint8_array[0] */
#define DEM_BITS_SET(_p,_n) ((_p)[(_n)>>3] |= (1<<((_n)&7)))
#define DEM_BITS_CLR(_p,_n) ((_p)[(_n)>>3] &= ~(1<<((_n)&7)))
#define DEM_BITS_ISSET(_p,_n) ((_p)[(_n)>>3] & (1<<((_n)&7)))

/* Det Check */
#if(DEM_DEV_ERROR_DETECT == STD_ON)
#define DEM_DEV_CHECK(_cond, _apiid,_errid) \
    do{ \
        if(_cond) \
        { \
            Det_ReportError(54,0,(_apiid),(_errid)); \
            return; \
        } \
    }while(0)

#define DEM_DEV_CHECK_R(_cond, _apiid, _errid) \
    do{ \
        if(_cond) \
        { \
            Det_ReportError(54,0,(_apiid),(_errid)); \
            return (E_NOT_OK); \
        } \
    }while(0)
#define DEM_DEV_CHECK_RETURN(_cond, _apiid,_errid,_res) \
    do{ \
        if(_cond) \
        { \
            Det_ReportError(54,0,(_apiid),(_errid)); \
            return (_res); \
        } \
    }while(0)
#else
#define DEM_DEV_CHECK(_cond, _apiid,_errid)
#define DEM_DEV_CHECK_R(_cond, _apiid,_errid)
#define DEM_DEV_CHECK_RETURN(_cond, _apiid,_errid,_res)
#endif

/* InitMonitorForEvent */
#define Dem_InitMonitorForEvent(_IntId, _Reason) \
    do{ \
        if(DemEventParameter[(_IntId)].DemCallbackInitMForE != NULL_PTR) \
        { \
            (DemEventParameter[(_IntId)].DemCallbackInitMForE)(_Reason); \
        } \
    }while(0)

typedef struct
{
    /* EventId, Status: Dem_ReportErrorStatus/Dem_SetEventStatus()*/
    Dem_EventIdType ExtId;
    Dem_EventStatusType Status;
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 FFData[DEM_FREEZE_FRAME_MAX_LEN];
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    uint8 ExtData[DEM_EXTENDED_DATA_MAX_LEN];
#endif
#endif
} Dem_EventBufferType;

/********[E X T E R N A L   D A T A]*******************************************/
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) DemPbCfgPtr;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#endif
#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(uint8,AUTOMATIC) DemInitState;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
/* OperationCycleStatus */
extern VAR(uint8,AUTOMATIC) DemOperationCycleStatus[DEM_OPERATION_CYCLE_NUM_BYTE];
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(boolean,AUTOMATIC) DemDTCStatusChangedInd;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(boolean,AUTOMATIC) DemClearDTCSet;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(uint8,AUTOMATIC) DemAgingCycle;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_AGING_CYCLE_COUNTER_PROCESSING == DEM_PROCESS_AGINGCTR_EXTERN)
#define DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
extern VAR(uint8,AUTOMATIC) DemExternAgingCnt;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#endif

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
/*************************************************************************/
/*
 * @Brief               <MemSet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Val && Size>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <Dest>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_MemSet(
    P2VAR(uint8, AUTOMATIC, DEM_VAR) Dest,
    uint8 Val,
    uint32 Size);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <MemCopy>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Src && Size>
 * @Param-Name[out]     <Dest>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_MemCopy(
    P2VAR(uint8, AUTOMATIC, DEM_VAR) Dest,
    P2CONST(uint8, AUTOMATIC, DEM_VAR) Src,
    uint32 Size);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <CheckCondictionFulfilled>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <cond && group && len>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <boolean>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(boolean, DEM_CODE) Dem_CheckCondictionFulfilled(
    P2CONST(uint8, AUTOMATIC, DEM_VAR) cond,
    P2CONST(uint8, AUTOMATIC, DEM_VAR) group,
    uint8 len);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DTC_ATTRIBUTES_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Get EventDTCAttributesCfg>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <const Dem_DTCAttributesType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST), DEM_CODE)
    Dem_EventDTCAttributesCfg(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define Dem_EventDTCAttributesCfg(x) NULL_PTR
#endif
/*************************************************************************/
/*
 * @Brief               <GetInternalMemDest>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCOrigin>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <uint8>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE) Dem_GetInternalMemDest(Dem_DTCOriginType DTCOrigin);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetEventIdByDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTC>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventIdType>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_EventIdType, DEM_CODE) Dem_GetEventIdByDTC(uint32 DTC);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_SetOccurrenceEvent>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && Status>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_SetOccurrenceEvent(Dem_EventIdType IntId,
    uint8 Status);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ClearDTCProcess>
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
extern FUNC(void, DEM_CODE) Dem_ClearDTCProcess(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_DcmInit>
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
extern FUNC(void, DEM_CODE) Dem_DcmInit(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
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
extern FUNC(void, DEM_CODE) Dem_TriggerOnEventStatus(Dem_EventIdType IntId,
    Dem_UdsStatusByteType OldStatus,
    Dem_UdsStatusByteType NewStatus);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetEventDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && DTCFormat>
 * @Param-Name[out]     <DTC>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_GetEventDTC(Dem_EventIdType IntId,
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint32, AUTOMATIC, DEM_VAR) DTC
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif /* DEM_DEM_COMMON_H_ */

/********[E N D   O F   F I L E]***********************************************/
