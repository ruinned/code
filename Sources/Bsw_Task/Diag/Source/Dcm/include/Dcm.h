/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/
#ifndef DCM_H
#define DCM_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_H_VENDOR_ID  0
#define DCM_H_MODULE_ID  0
#define DCM_H_AR_MAJOR_VERSION  3
#define DCM_H_AR_MINOR_VERSION  3
#define DCM_H_AR_PATCH_VERSION  0
#define DCM_H_SW_MAJOR_VERSION  1
#define DCM_H_SW_MINOR_VERSION  0
#define DCM_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"

/****************************** definitions *********************************/
#define DCM_MODULE_ID    		(53)  		/* Diagnostic Communication Manager */

/****************************** definitions *********************************/
#define  DCM_INVALID_UINT8      (0xFFu)
#define  DCM_INVALID_UINT16     (0xFFFFu)
#define  DCM_INVALID_UINT32     (0xFFFFFFFFul)

/************************************************************************************
 *********************************Dcm Version information****************************
 ************************************************************************************/
#define  DCM_VENDOR_ID          (0x00u)    /*vendor*/
#define  DCM_INSTANCE_ID        (0x00u)    /*instance*/
#define  DCM_SW_MAJOR_VERSION   (0x01u)    /*Major Version*/
#define  DCM_SW_MINOR_VERSION   (0x00u)    /*Minor Version*/
#define  DCM_SW_PATCH_VERSION   (0x00u)    /*Patch version*/
#define  DCM_AR_MAJOR_VERSION   (0x03u)
#define  DCM_AR_MINOR_VERSION   (0x03u)
#define  DCM_AR_PATCH_VERSION   (0x00u)

/****************************************************************************************
 *****************************Development error values **********************************
 ****************************************************************************************/
/****@req DCM-FUNR-238[DCM012]****/
/****@req DCM-FUNR-239[DCM044]****/
/****@req DCM-FUNR-240[DCM364]****/
/****@req DCM-FUNR-241[DCM040]****/
/****@req DCM-FUNR-242[DCM041]****/
#define  DCM_E_INTERFACE_TIMEOUT             (0x01u) /*Application-Interface: Timeout*/
#define  DCM_E_INTERFACE_VALUE_OUT_OF_RANGE  (0x02u) /*Application-Interface: Return-value out of range*/
#define  DCM_E_INTERFACE_BUFFER_OVERFLOW     (0x03u) /*Application-Interface: Buffer Overflow*/
#define  DCM_E_INTERFACE_PROTOCOL_MISMATCH   (0x04u) /*Application-Interface: Protocol mismatch*/
#define  DCM_E_UNINIT                        (0x05u) /*Internal: DCM not initialized*/
#define  DCM_E_PARAM                         (0x06u) /*DCM API function with invalid input parameter*/
/****************************************************************************************
 *********************************Dcm Module API ID************************************
 ****************************************************************************************/
#define  DCM_INIT_ID                         (0x01u) /*Dcm_Init()*/
#define  DCM_PROVIDERXBUFFER_ID              (0x02u) /*Dcm_ProvideRxBuffer()*/
#define  DCM_RXINDICATION_ID                 (0x03u) /*Dcm_RxIndication()   */
#define  DCM_PROVIDETXBUFFER_ID              (0x04u) /*Dcm_ProvideTxBuffer()*/
#define  DCM_TXCONFIRMATION_ID               (0x05u) /*Dcm_TxConfirmation() */
#define  DCM_GETSESCTRLTYPE_ID               (0x06u) /*Dcm_GetSesCtrlType() */
#define  DCM_GETSECURITYLEVEL_ID             (0x0Du) /*Dcm_GetSecurityLevel()*/
#define  DCM_GETACTIVEPROTOCOL_ID            (0x0Fu) /*Dcm_GetActiveProtocol()*/
#define  DCM_COMM_NOCOMMODEENTERED           (0x21u) /*Dcm_Comm_NoComModeEntered()*/
#define  DCM_COMM_SILENTCOMMODEENTERED       (0x22u) /*Dcm_Comm_SilentComModeEntered()*/
#define  DCM_COMM_FULLCOMMODEENTERED         (0x23u) /*Dcm_Comm_FULLComModeEntered()*/
#define  DCM_GETVERSIONINFO_ID               (0x24u) /*Dcm_GetVersionInfo()*/
#define  DCM_MAIN_FUNCTION_ID                (0x25u) /*Dcm_Main_Function()*/

/*************************************************************************/
/*
 * Brief                <initialization of DCM module.>
 * ServiceId            <0x01>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_Init(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#if(STD_ON == DCM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to where to store the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_GetVersionInfo(
						   P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR)VersionInfo );
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               <This function provides the active security level value.>
 * ServiceId           <0x0d>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SecLevel:Pointer to Active Security Level value >
 * Param-Name[in/out]   <None>
 * Return               <E_OK:
 * 						E_NOT_OK:>
 * PreCondition         <...>
 * CallByAPI            <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSecurityLevel(
							 P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR)SecLevel );
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function provides the active session control type value. >
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SesType:Active Session Control Type value>
 * Param-Name[in/out]  <None>
 * Return              <E_OK,E_NOT_OK>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSesCtrlType(
							P2VAR(Dcm_SesType, AUTOMATIC, DCM_VAR)SesType );
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function returns the active protocol name. >
 * ServiceId           <0x0f>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <description...>
 * Param-Name[out]     <ActiveProtocol:Active protocol type value >
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)Dcm_GetActiveProtocol(
							P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR)ActiveProtocol );
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This service is used for processing the tasks of the main loop. >
 * ServiceId           <0x25>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(void,DCM_CODE) Dcm_MainFunction(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* DCM_H_ */
