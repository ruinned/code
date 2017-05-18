/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM_EcuM.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/

#ifndef CANSM_ECUM_H
#define CANSM_ECUM_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_ECUM_H_AR_MAJOR_VERSION  1U
#define CANSM_ECUM_H_AR_MINOR_VERSION  2U
#define CANSM_ECUM_H_AR_PATCH_VERSION  0U
#define CANSM_ECUM_H_SW_MAJOR_VERSION  1U
#define CANSM_ECUM_H_SW_MINOR_VERSION  0U
#define CANSM_ECUM_H_SW_PATCH_VERSION  0U
/*=======[I N C L U D E S]====================================================*/
#include "CanSM_Types.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * @brief        :       This service initializes the CanSM module.
 * Service ID    :       <SERVICE_ID_CANSM_INIT>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Non Reentrant>
 * Param-Name[in]:       configPtr:Pointer to the CANSM configuration data.
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       <None>
 * CallByAPI     :       ECUM
 */
/******************************************************************************/
/* @req CANSM123 @req CANSM198 @req CANSM179 */
#if (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS)
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(void, CANSM_CODE)
CanSM_Init
(
    P2CONST(CanSM_ConfigType, CANSM_VAR, CANSM_CONST_PBCFG)ConfigPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#else /* CANSM_VARIANT_POST_BUILD != CANSM_VARIANT_CLASS */
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(void, COM_CODE)
CanSM_Init(void);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif /* CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS */
#endif /* CANSM_ECUM_H */
/*=======[E N D   O F   F I L E]==============================================*/
