/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventDebounce.h>
 *  @brief      <Dem_EventDebounce>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 3:02:58>
 */
/*============================================================================*/

#ifndef DEM_EVENTDEBOUNCE_H_
#define DEM_EVENTDEBOUNCE_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4   wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTDEBOUNCE_H_MODULE_ID            54U
#define DEM_EVENTDEBOUNCE_H_VENDOR_ID          0U
#define DEM_EVENTDEBOUNCE_H_AR_MAJOR_VERSION   3U
#define DEM_EVENTDEBOUNCE_H_AR_MINOR_VERSION   1U
#define DEM_EVENTDEBOUNCE_H_AR_PATCH_VERSION   0U
#define DEM_EVENTDEBOUNCE_H_SW_MAJOR_VERSION   1U
#define DEM_EVENTDEBOUNCE_H_SW_MINOR_VERSION   0U
#define DEM_EVENTDEBOUNCE_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Types.h"

/********[M A C R O S]*********************************************************/

/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
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
extern FUNC(void, DEM_CODE) Dem_DebounceInit(void);
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
extern FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetInteralFDC
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
extern FUNC(sint8, DEM_CODE) Dem_GetInteralFDC(Dem_EventIdType IntId);
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
extern FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcess(
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer
    );
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
extern FUNC(void, DEM_CODE) Dem_DebounceFreeze(Dem_EventIdType IntId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
extern FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#else
#define Dem_DebounceFreeze(x)
#define Dem_DebounceTimerMain()
#endif

#endif /* DEM_DEM_EVENTDEBOUNCE_H_ */

/********[E N D   O F   F I L E]***********************************************/
