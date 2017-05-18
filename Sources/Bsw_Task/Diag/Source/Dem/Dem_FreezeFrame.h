/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_FreezeFrame.h>
 *  @brief      <Dem_FreezeFrame>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 2:49:49>
 */
/*============================================================================*/
#ifndef DEM_FREEZEFRAME_H_
#define DEM_FREEZEFRAME_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_FREEZEFRAME_H_MODULE_ID          54U
#define DEM_FREEZEFRAME_H_VENDOR_ID          0U
#define DEM_FREEZEFRAME_H_AR_MAJOR_VERSION   3U
#define DEM_FREEZEFRAME_H_AR_MINOR_VERSION   1U
#define DEM_FREEZEFRAME_H_AR_PATCH_VERSION   0U
#define DEM_FREEZEFRAME_H_SW_MAJOR_VERSION   1U
#define DEM_FREEZEFRAME_H_SW_MINOR_VERSION   0U
#define DEM_FREEZEFRAME_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"

#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
/********[M A C R O S]*********************************************************/

/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <pBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_FreezeFrameGet(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckFreezeFrameIsStorage>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && Trigger>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <boolean>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(boolean, DEM_CODE) Dem_CheckFreezeFrameIsStorage(Dem_EventIdType IntId,
    uint8 Trigger);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameStorage>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer && Trigger>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEntry>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
uint8 Trigger);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGetFromEntry>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <RecordNum && Did>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEntry>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_FreezeFrameGetFromEntry(
P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
uint8 RecordNum,
uint16 Did,
P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGetByRecordNum>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEntry && RecordNum>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_FreezeFrameInfoType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_FreezeFrameGetByRecordNum(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameInit>
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
extern FUNC(void, DEM_CODE) Dem_FreezeFrameInit(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_PreStoreFF>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <ExtId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_PreStoreFF(Dem_EventIdType ExtId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ClearPreStoreFF>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPreStoreFF(Dem_EventIdType ExtId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#else
#define Dem_FreezeFrameInit()
#define Dem_PreStoreFF(x) E_NOT_OK
#define Dem_ClearPreStoreFF(x) E_OK
#endif

#else
#define Dem_FreezeFrameInit()
#define Dem_PreStoreFF(x) E_NOT_OK
#define Dem_ClearPreStoreFF(x) E_OK
#endif

#endif /* DEM_DEM_FREEZEFRAME_H_ */

/********[E N D   O F   F I L E]***********************************************/
