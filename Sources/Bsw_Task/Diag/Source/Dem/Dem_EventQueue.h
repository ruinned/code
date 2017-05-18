/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_EventReport.h>
 *  @brief      <Dem_EventReport>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 2:00:53>
 */
/*============================================================================*/

#ifndef DEM_EVENTQUEUE_H_
#define DEM_EVENTQUEUE_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EVENTQUEUE_H_MODULE_ID          54U
#define DEM_EVENTQUEUE_H_VENDOR_ID          0U
#define DEM_EVENTQUEUE_H_AR_MAJOR_VERSION   3U
#define DEM_EVENTQUEUE_H_AR_MINOR_VERSION   1U
#define DEM_EVENTQUEUE_H_AR_PATCH_VERSION   0U
#define DEM_EVENTQUEUE_H_SW_MAJOR_VERSION   1U
#define DEM_EVENTQUEUE_H_SW_MINOR_VERSION   0U
#define DEM_EVENTQUEUE_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Types.h"

/********[M A C R O S]*********************************************************/
#if(DEM_BSW_ERROR_BUFFER_SIZE > DEM_MAX_NUMBER_EVENT_ENTRY_EVENT_BUFFER)
#define DEM_EVENT_QUEUE_SIZE DEM_BSW_ERROR_BUFFER_SIZE
#else
#define DEM_EVENT_QUEUE_SIZE DEM_MAX_NUMBER_EVENT_ENTRY_EVENT_BUFFER
#endif

/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
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
extern FUNC(void, DEM_CODE) Dem_EventQueueInit(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_EventQueueAdd>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
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
extern FUNC(Std_ReturnType, DEM_CODE)
Dem_EventQueueAdd(Dem_EventIdType ExtId,
    Dem_EventStatusType Status);
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
extern FUNC(void, DEM_CODE) Dem_EventQueueProcess(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif /* DEM_DEM_EVENTQUEUE_H_ */

/********[E N D   O F   F I L E]***********************************************/
