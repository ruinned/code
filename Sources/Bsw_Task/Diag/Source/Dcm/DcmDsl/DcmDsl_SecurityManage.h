/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_SecurityManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_SECURITYMANAGE_H
#define DCMDSL_SECURITYMANAGE_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_SECURITYMANAGE_H_VENDOR_ID  0
#define DCMDSL_SECURITYMANAGE_H_MODULE_ID  0
#define DCMDSL_SECURITYMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_SECURITYMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_SECURITYMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_SECURITYMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_SECURITYMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_SECURITYMANAGE_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"
#include "Std_ExtendedTypes.h"

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
/****************************** declarations *********************************/
/**********************************************************************
 *************Security level management data structure****************
 *********************************************************************/
typedef  enum
{
   DCM_SECTIMER_ON  = 1u,        /*Security level timer "on" state*/
   DCM_SECTIMER_OFF = 0u         /*Security level timer "oFF" state*/
}Dcm_SecTimerStateType;

typedef  struct
{
   TickType  Dcm_SecCurTicks;     /*Security level timer curent ticks*/
   TickType  Dcm_SecExpiredTicks; /*Security level timer timeout ticks*/
   Dcm_SecTimerStateType  Dcm_SecTimerState;
}Dcm_SecTimerCtrlType;

typedef  enum
{
   DCM_SERVICE_IDLE = 0u,       /* "Idle" */
   DCM_SERVICE_SEED = 1u,       /* After "receiving seed"*/
   DCM_SERVICE_KEY  = 2u        /* After "comparing key" */
}Dcm_SecServiceStateType;

typedef  enum
{
   DCM_SEC_LOCKED   = 0u,
   DCM_SEC_UNLOCKED = 1u
}Dcm_SecStateType;

typedef  struct
{
   boolean  Dcm_RunDlyFlag;             		/*delay accessing SecurityAcess service Flag.*/
   boolean  Dcm_LockedFlag;             		/*prohibit accessing SecurityAcess service Flag.*/
   uint8    Dcm_SubfunctionForSeed;     		/*Request seed sub-functions.*/
   uint8    Dcm_FalseAcessCount;        		/*the number of Compare key failures and consecutive Request Seed*/
   Dcm_SecLevelType  Dcm_ActiveSec;     		/*DCM module current level of security*/
   Dcm_SecLevelType  Dcm_NewSec;        		/*DCM module to be changed security level*/
   Dcm_SecServiceStateType Dcm_SecServiceState;	/*Security level change process status*/
   Dcm_SecTimerCtrlType    Dcm_OnBootDlyCtrl;  	/*Boot delay access to securityAcess services timer control block*/
   Dcm_SecTimerCtrlType    Dcm_RunDlyCtrl;     	/*When you reach the number of failure,the delay access control block*/
   Dcm_SecStateType        Dcm_SecState;
}Dcm_SecCtrlType;

/********************************************************
 ******************resource statement********************
********************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
extern   VAR(Dcm_SecCtrlType,DCM_VAR_NOINIT) Dcm_SecCtrl; /*Security level management control block*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <initial of Security level control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_InitSecCtrl(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Setting the security level>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSec:To be changed the security level>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE) DslInternal_SetSecurityLevel(Dcm_SecLevelType  NewSec);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set SecurityAccess service process ,receives seed/key status>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Status:State to be modified>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_SetSecurityAccessStatus(Dcm_SecServiceStateType Status);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif

#endif /* DCMDSL_SECURITYMANAGE_H_ */
