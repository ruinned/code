/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_CommManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_COMMMANAGE_H
#define DCMDSL_COMMMANAGE_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_COMMANAGE_H_VENDOR_ID  0
#define DCMDSL_COMMANAGE_H_MODULE_ID  0
#define DCMDSL_COMMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_COMMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_COMMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_COMMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_COMMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_COMMANAGE_H_SW_PATCH_VERSION  0

/**************************************************************/
#include "Dcm_Types.h"
/* Using for the channel number for ComM */
#define DCM_COMM_CHANNEL_NO          0

/***************************************************************************************
 *************Communication Management sub-function data structure*****
 **************************************************************************************/
typedef  enum
{
   DCM_COMM_NO_COMMUNICATION      = 0, /* conmmunication is "NO Communication"status */
   DCM_COMM_SILENT_COMMUNICATION  = 1, /* conmmunication is "Silent Communication"status */
   DCM_COMM_FULL_COMMUNICATION    = 2  /* conmmunication is "Full Communication"status*/
}Dcm_CommStateType;

typedef struct
{
   Dcm_CommStateType  Dcm_CommState;
}Dcm_CommCtrlType;


/*********************************************************************************
 *****************Communication Management function declarations******************
 ********************************************************************************/
/*************************************************************************/
/*
 * Brief               <initialization of Comm Submodule>
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
extern   FUNC(void,DCM_CODE)DslInternal_InitComMCtrl(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Read the current network communication status >
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <pState:Pointer to current network communication status>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_ReadCurComState(P2VAR(Dcm_CommStateType,AUTOMATIC,DCM_VAR)pState);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* DCMDSL_COMMMANAGE_H_ */
