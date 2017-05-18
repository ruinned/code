/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/
#ifndef DCMDSD_H
#define DCMDSD_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSD_H_VENDOR_ID  0
#define DCMDSD_H_MODULE_ID  0
#define DCMDSD_H_AR_MAJOR_VERSION  3
#define DCMDSD_H_AR_MINOR_VERSION  3
#define DCMDSD_H_AR_PATCH_VERSION  0
#define DCMDSD_H_SW_MAJOR_VERSION  1
#define DCMDSD_H_SW_MINOR_VERSION  0
#define DCMDSD_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"


/****************************** declarations *********************************/
/*************************************************************************/
/*
 * Brief               <Find SID corresponding service configuration table index
 *                      and the service index in corresponding service configuration table.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId: The corresponding protocol control block ID number>
 * Param-Name[out]     <(*pSidTabIndex):Request packet index number in the service configuration table;
 *                      (*pSidTabServieCfgIndex): the service index in corresponding service configuration table>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE) DsdInternal_SearchSidTabServiceIndex(uint8   Sid,
																		    uint8   ProtocolCtrlId,
																		    P2VAR(uint16,AUTOMATIC, DCM_VAR)pSidTabIndex,
																		    P2VAR(uint16,AUTOMATIC, DCM_VAR)pSidTabServieCfgIndex);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set the negative response code NRC;>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Nrc: the negative response code;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SetNrc(uint8 ProtocolCtrlId,
														uint8 Nrc);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set corresponding message processing state>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      MsgState: message processing state>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SetMsgState(uint8 ProtocolCtrlId,
															  Dcm_MsgStateType MsgState);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <The diagnostic request processing in DSD layer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DsdInternal_RxIndication(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Response to a diagnosis request processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DsdInternal_ProcessingDone(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Notify the DSD layer, respond to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DsdInternal_TxConfirmation(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Determine the diagnostic request packet is allowed in the current session state.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SesCheck(uint8 ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               <determine the diagnostic request packet is allowed in the current security level>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SecurityCheck(uint8 ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************** definitions *********************************/

#endif /* DCMDSD_H_ */
