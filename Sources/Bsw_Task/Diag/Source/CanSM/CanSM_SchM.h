/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM_SchM.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/
#ifndef CANSM_SCHM_H
#define CANSM_SCHM_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_SCHM_H_AR_MAJOR_VERSION  1U
#define CANSM_SCHM_H_AR_MINOR_VERSION  2U
#define CANSM_SCHM_H_AR_PATCH_VERSION  0U
#define CANSM_SCHM_H_SW_MAJOR_VERSION  1U
#define CANSM_SCHM_H_SW_MINOR_VERSION  0U
#define CANSM_SCHM_H_SW_PATCH_VERSION  0U
/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
/* @req CANSM022 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * @brief        :       Scheduled function of the CanSM.
 * Service ID    :       <SERVICE_ID_CANSM_MAINFUNCTION>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       <None>
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       SchM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(void, COM_CODE)
CanSM_MainFunction(void);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /* CANSM_SCHM_H */
/*=======[E N D   O F   F I L E]==============================================*/
