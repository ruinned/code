/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_PendingManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_PENDINGMANAGE_H
#define DCMDSL_PENDINGMANAGE_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_PENDINGMANAGE_H_VENDOR_ID  0
#define DCMDSL_PENDINGMANAGE_H_MODULE_ID  0
#define DCMDSL_PENDINGMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_PENDINGMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_PENDINGMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_PENDINGMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_PENDINGMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_PENDINGMANAGE_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"

/****************************** declarations *********************************/
/*************************************************************************/
/*
 * Brief               <Start P2Timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerStart(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <stop P2timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerStop(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <P2Timer timeout processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerTimeout(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Judgment P2Timer run time is more than P2ServerMin>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_IsLargerThanP2ServerMin(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* DCMDSL_PENDINGMANAGE_H_ */
