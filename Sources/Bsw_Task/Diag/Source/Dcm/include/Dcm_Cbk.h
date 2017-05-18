/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm_Cbk.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCM_CBK_H
#define DCM_CBK_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_CBK_H_VENDOR_ID  0
#define DCM_CBK_H_MODULE_ID  0
#define DCM_CBK_H_AR_MAJOR_VERSION  3
#define DCM_CBK_H_AR_MINOR_VERSION  3
#define DCM_CBK_H_AR_PATCH_VERSION  0
#define DCM_CBK_H_SW_MAJOR_VERSION  1
#define DCM_CBK_H_SW_MINOR_VERSION  0
#define DCM_CBK_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"

/****************************** declarations *********************************/
/*************************************************************************/
/*
 * Brief               <This is called by the PduR to indicate the competion of a reception>
 * ServiceId           <0x02>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 *                      TpSduLength: This length identifies the overall number of bytes to be received.>
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(BufReq_ReturnType,DCM_CODE)Dcm_ProvideRxBuffer(PduIdType DcmRxPduId,
															 PduLengthType TpSduLength,
															 P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This is called by the PduR to indicate the competion of a reception>
 * ServiceId           <0x03>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:ID of DCM I-PDU that has been received
 *                      Result: The result of the diagnostic request message received>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after call of DcmProvideRxBuffer()>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_RxIndication(PduIdType DcmRxPduId,NotifResultType Result);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <By this service the DCM module is requested to provide a buffer containing data to be transmitted via a transport protocol>
 * ServiceId           <0x04>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:Identifies the DCM data to be sent;
 *                      Length: This is the minimum length given in bytes of the buffer requested from the DCM>
 * Param-Name[out]     <PduInfoPtr: Pointer to pointer to PduInfoStructure containing data pointer and length of a transmit buffer>
 * Param-Name[in/out]  <None>
 * Return              <BufReq_ReturnType>
 * PreCondition        <this function must be called after call of Pdur_Transmit()>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(BufReq_ReturnType,DCM_CODE)Dcm_ProvideTxBuffer(PduIdType  DcmTxPduId,
													P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
													PduLengthType Length);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This is called by the PduR to confirm a Transmit >
 * ServiceId           <0x05>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM IPDU that has been transmitted.
 *                      Result:NTFRSLT_OK: the complete N-PDU has been transmitted.
 *                             NTFRSLT_E_CANCELATION_OK: the N-PDU has been successfully cancelled.
 *                             NTFRSLT_E_CANCELATION_NOT_OK: an error occurred when cancelling the N-PDU.
 *                             any other value: an error occurred during transmission >
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after call of DcmProvideTxBuffer().>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_TxConfirmation(PduIdType  DcmTxPduId, NotifResultType  Result);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_NO_COMMUNICATION>
 * ServiceId           <0x21>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_NoComModeEntered(uint8 channel);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_SILENT_COMMUNICATION>
 * ServiceId           <0x22>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
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
extern   FUNC(void,DCM_CODE)Dcm_ComM_SilentComModeEntered(uint8 channel);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_FULL_COMMUNICATION>
 * ServiceId           <0x23>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_FullComModeEntered(uint8 channel);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


/****************************** definitions *********************************/

#endif /* DCM_CBK_H_ */
