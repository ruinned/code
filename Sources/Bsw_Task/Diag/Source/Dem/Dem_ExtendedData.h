/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_ExtendedData.h>
 *  @brief      <Dem_ExtendedData>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4  - PM 3:00:07>
 */
/*============================================================================*/

#ifndef DEM_EXTENDEDDATA_H_
#define DEM_EXTENDEDDATA_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EXTENDEDDATA_H_MODULE_ID          54U
#define DEM_EXTENDEDDATA_H_VENDOR_ID          0U
#define DEM_EXTENDEDDATA_H_AR_MAJOR_VERSION   3U
#define DEM_EXTENDEDDATA_H_AR_MINOR_VERSION   1U
#define DEM_EXTENDEDDATA_H_AR_PATCH_VERSION   0U
#define DEM_EXTENDEDDATA_H_SW_MAJOR_VERSION   1U
#define DEM_EXTENDEDDATA_H_SW_MINOR_VERSION   0U
#define DEM_EXTENDEDDATA_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"

#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
/********[M A C R O S]*********************************************************/

/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataGet>
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
extern FUNC(void, DEM_CODE) Dem_ExtendedDataGet(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataGetFromEntry>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEntry && RecordNum>
 * @Param-Name[out]     <pBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <pBuffer>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ExtendedDataGetFromEntry(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckExtendedDataIsStorage>
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
extern FUNC(boolean, DEM_CODE) Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId,uint8 Trigger);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataStorage>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer && Trigger>
 * @Param-Name[out]     <pEntry>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    uint8 Trigger);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#endif /* DEM_DEM_EXTENDEDDATA_H_ */

/********[E N D   O F   F I L E]***********************************************/
