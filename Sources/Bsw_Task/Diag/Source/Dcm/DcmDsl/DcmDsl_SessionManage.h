/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_SessionManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_SESSIONMANAGE_H
#define DCMDSL_SESSIONMANAGE_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_SESSIONMANAGE_H_VENDOR_ID  0
#define DCMDSL_SESSIONMANAGE_H_MODULE_ID  0
#define DCMDSL_SESSIONMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_SESSIONMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_SESSIONMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_SESSIONMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_SESSIONMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_SESSIONMANAGE_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"
#include "Std_ExtendedTypes.h"

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
/****************************** declarations *********************************/
/*****************************************************************
 **********Session Management sub-function data structure*******
 *****************************************************************/
typedef enum
{
   DCM_S3TIMER_ON  = 0,
   DCM_S3TIMER_OFF = 1
}Dcm_S3StateType;

typedef struct
{
   TickType  Dcm_S3CurTicks;
   TickType  Dcm_S3ExpiredTicks;   /*S3Timer,timeout ticks*/
   Dcm_S3StateType  Dcm_S3State;   /*S3timer status */
}Dcm_S3CtrlType;

typedef enum
{
   DCM_SESSION_DEFAULT   = 0,     /*default session*/
   DCM_SESSION_UNDEFAULT = 1      /*undefault session*/
}Dcm_SesStateType;

typedef struct
{
   Dcm_SesType      Dcm_ActiveSes;        /*Module current session-state values;*/
   Dcm_SesType      Dcm_NewSes;           /*The session control value of the module to be changed;*/
   Dcm_S3CtrlType   Dcm_S3Ctrl;           /*S3Timer control block*/
   Dcm_SesStateType Dcm_SessionState;     /*DCM module session state*/
}Dcm_SesCtrlType;

/********************************************************
 *******************resource statement*******************
********************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
extern   VAR(Dcm_SesCtrlType,DCM_VAR_NOINIT) Dcm_SesCtrl; /*Session management control block*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
 **************Session Management sub-function function declaration*************
 *******************************************************************************/
/*************************************************************************/
/*
 * Brief               <The initialization of session management control block>
 * ServiceId           <None>
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
extern   FUNC(void,DCM_CODE) DslInternal_InitSesCtrl(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <start S3Timer>
 * ServiceId           <None>
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
extern   FUNC(void,DCM_CODE) DslInternal_S3ServerStart(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <stop S3Timer>
 * ServiceId           <None>
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
extern   FUNC(void,DCM_CODE)DslInternal_S3ServerStop(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <timeout of S3Timer>
 * ServiceId           <None>
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
extern   FUNC(void,DCM_CODE)DslInternal_S3ServerTimeout(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Refresh session>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:Session control state to be refreshed>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_SesRefresh(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif

#endif /* DCMDSL_SESSIONMANAGE_H_ */
