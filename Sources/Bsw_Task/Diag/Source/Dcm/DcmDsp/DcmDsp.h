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

#ifndef DCMDSP_H
#define DCMDSP_H

#include "Std_ExtendedTypes.h"

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSP_H_VENDOR_ID  0
#define DCMDSP_H_MODULE_ID  0
#define DCMDSP_H_AR_MAJOR_VERSION  3
#define DCMDSP_H_AR_MINOR_VERSION  3
#define DCMDSP_H_AR_PATCH_VERSION  0
#define DCMDSP_H_SW_MAJOR_VERSION  1
#define DCMDSP_H_SW_MINOR_VERSION  0
#define DCMDSP_H_SW_PATCH_VERSION  0

/****************************** declarations *********************************/
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
	#define DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
	#include "MemMap.h"
	extern  VAR(Dcm_RoutineControlStateType,DCM_VAR_NOINIT)Dcm_RoutineControlState[DCM_DSP_ROUTINE_MAX_NUM];
	#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
	#include "MemMap.h"
#endif

/*************************************************************************/
/*
* Brief               <The Dsp layer response to the confirmation>
* ServiceId           <None>
* Sync/Async          <Synchronous>
* Reentrancy          <Reentrant>
* Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
* Param-Name[out]     <None>
* Param-Name[in/out]  <None>
* Return              <None>
* PreCondition        <None>
* CallByAPI           <APIName>
*/
/*************************************************************************/

#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DspInternal_DcmConfirmation(uint8 ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)Dsp_InitRoutineStates(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif


#if((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X85_ENABLED))
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE) Dsp_EnableAllDtcsFresh(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************** definitions *********************************/
#endif /* DCMDSP_H_ */
