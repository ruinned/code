/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM_ComM.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/
#ifndef CANSM_COMM_H
#define CANSM_COMM_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_COMM_H_AR_MAJOR_VERSION  1U
#define CANSM_COMM_H_AR_MINOR_VERSION  2U
#define CANSM_COMM_H_AR_PATCH_VERSION  0U
#define CANSM_COMM_H_SW_MAJOR_VERSION  1U
#define CANSM_COMM_H_SW_MINOR_VERSION  0U
#define CANSM_COMM_H_SW_PATCH_VERSION  0U
/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* @req CANSM009 @req CANSM037 */
/******************************************************************************/
/*
 * @brief        :       This service shall change the communication mode of a CAN
 *                       network to the requested one.
 * Service ID    :       <SERVICE_ID_CANSM_REQUESTCOMMODE>
 * Sync/Async    :       <Asynchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       NetworkHandle: Handle of destinated communication network for request
 *                       ComM_Mode: Requested communication mode
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       E_OK: Service accepted
 *                       E_NOT_OK: Service denied
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       ComM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(Std_ReturnType, CANSM_CODE)
CanSM_RequestComMode
(
    NetworkHandleType NetworkHandle,
    ComM_ModeType ComM_Mode
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
/* @req CANSM090 */
/******************************************************************************/
/*
 * @brief        :       This service shall put out the current communication mode
 *                       of a CAN network.
 * Service ID    :       <SERVICE_ID_CANSM_GETCURRENTCOMMODE>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       NetworkHandle: whose current communication mode shall be put out
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       E_OK: Service accepted
 *                       E_NOT_OK: Service denied
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       ComM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(Std_ReturnType, CANSM_CODE)
CanSM_GetCurrentComMode
(
    NetworkHandleType NetworkHandle,
    P2VAR(ComM_ModeType, AUTOMATIC, CANSM_APPL_DATA) ComM_ModePtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /* CANSM_COMM_H */
/*=======[E N D   O F   F I L E]==============================================*/
