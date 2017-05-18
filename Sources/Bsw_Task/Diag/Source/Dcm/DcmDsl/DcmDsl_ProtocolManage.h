/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_ProtocolManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/
#ifndef DCMDSL_PROTOCOLMANAGE_H
#define DCMDSL_PROTOCOLMANAGE_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_PROTOCOLMANAGE_H_VENDOR_ID  0
#define DCMDSL_PROTOCOLMANAGE_H_MODULE_ID  0
#define DCMDSL_PROTOCOLMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_PROTOCOLMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_PROTOCOLMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_PROTOCOLMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_PROTOCOLMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_PROTOCOLMANAGE_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"
#include "Std_ExtendedTypes.h"

/****************************** declarations *********************************/
/*********************************************************
 *The operation of the protocol managment data structure
 *********************************************************/
typedef enum
{
   DCM_PROTOCOL_VALID   = 1,  /*Protocol "valid" state*/
   DCM_PROTOCOL_INVALID = 0   /*Protocol "Invalid" state*/
}Dcm_ProtocolStateType;

typedef struct
{
   Dcm_ProtocolType ProtocolId; /*The protocol ID number*/
   uint8      ProtocolPri;        /*protocol priority*/
   uint8      MsgCtrlIndex;       /*Message control block index number*/
   TickType   ProtocolPreemptTime;/*protocol preempt time*/
   TickType   P2ServerMax;        /*In the current session state P2ServerMax.*/
   TickType   P2ServerMin;        /*In the current session state P2ServerMin.*/
   TickType   P2StarServerMax;    /*In the current session state P2*ServerMax.*/
   TickType   P2StarServerMin;    /*In the current session state P2*ServerMin.*/
   TickType   S3Server;           /*S3Server value*/
   Dcm_ProtocolStateType  Dcm_ProtocolState;/*Protocol state*/
}Dcm_ProtocolCtrlType;

/********************************************************
***************Resource statement************************
********************************************************/
#define DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
extern  VAR(Dcm_ProtocolCtrlType,DCM_VAR_NOINIT)  Dcm_ProtocolCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];/*Protocol control block*/
#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Initializing the corresponding protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitProtocolCtrl(uint8 ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <protocal start>
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
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_ProtocolStart(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol link layer parameters of time>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <P2ServerTicks:
 *                      P2StarServerTicks:>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_SetProtocolLinkLayerTicks(TickType P2ServerTicks,TickType P2StarServerTicks);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* DCMDSL_PROTOCOLMANAGE_H_ */
