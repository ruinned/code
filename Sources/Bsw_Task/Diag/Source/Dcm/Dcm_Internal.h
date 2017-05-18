/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm_Internal.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMINTERNAL_H
#define DCMINTERNAL_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_INTERNAL_H_VENDOR_ID  0
#define DCM_INTERNAL_H_MODULE_ID  0
#define DCM_INTERNAL_H_AR_MAJOR_VERSION  3
#define DCM_INTERNAL_H_AR_MINOR_VERSION  3
#define DCM_INTERNAL_H_AR_PATCH_VERSION  0
#define DCM_INTERNAL_H_SW_MAJOR_VERSION  1
#define DCM_INTERNAL_H_SW_MINOR_VERSION  0
#define DCM_INTERNAL_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Types.h"
#include "Dcm_CfgType.h"
#include "Os.h"
#include "Std_ExtendedTypes.h"

#define   MS_PER_TICK    (TICK_DURATION/1000)  

#define DELAY_TIME_IN_BOOT_ENABLED STD_OFF

/************************************************************************
 ************************************************************************
 ************************************************************************/
#if (STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
typedef  enum
{
   DCM_DSP_ROUTINE_INIT = 0u,
   DCM_DSP_ROUTINE_ON   = 1u,
   DCM_DSP_ROUTINE_OFF  = 2u
}Dcm_RoutineControlStateType;
#endif
/****************************** declarations *********************************/
/****************************** Type definitions *****************************/
typedef  enum
{
  DCM_ON  = 1,
  DCM_OFF = 0
}Dcm_MkStateType;

typedef  struct
{
  Dcm_SesType       Dcm_ActiveSes;     /*DCM Activity session*/
  Dcm_SecLevelType  Dcm_ActiveSec;     /*DCM Activity security*/
  Dcm_ProtocolType  Dcm_ActiveProtocol;/*DCM Activity Protocol*/
  Dcm_MkStateType   Dcm_MkState;       /*DCM Module status*/
}Dcm_MkCtrlType;

/*DcmGeneral*/
extern    const  Dcm_GeneralCfgType     Dcm_GeneralCfg;

#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*DcmPageBuffer*/
extern    const  Dcm_PageBufferCfgType   Dcm_PageBufferCfg;
#endif

/*DcmDsp*/
extern    const  Dcm_DspCfgType         Dcm_DspCfg;
/*DcmDsd*/
extern    const  Dcm_DsdCfgType         Dcm_DsdCfg;
/*DcmDsl*/
extern    const  Dcm_DslCfgType         Dcm_DslCfg;

/***************************************************************/
#define  DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
extern    VAR(uint8,DCM_VAR_POWER_ON_INIT)Dcm_Channel[DCM_CHANNEL_LENGTH];
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"


#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
extern  VAR(Dcm_MkCtrlType, DCM_VAR_NOINIT) Dcm_MkCtrl;/*Module control resource*/
#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

extern  uint8 gAppl_UpdataOK_ResponseFlag;
#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
extern  VAR(boolean, DCM_VAR_NOINIT) gFactory_ActiveFlag;/*yb*/
#endif
#endif /* DCMINTERNAL_H_ */



