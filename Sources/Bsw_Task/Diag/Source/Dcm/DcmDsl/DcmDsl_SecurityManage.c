/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_SecurityManage.c>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms    Initial version                         */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_SECURITYMANAGE_C_VENDOR_ID  0
#define DCMDSL_SECURITYMANAGE_C_MODULE_ID  0
#define DCMDSL_SECURITYMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_SECURITYMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_SECURITYMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_SECURITYMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_SECURITYMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_SECURITYMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_SECURITYMANAGE_C_VENDOR_API_INFIX  0

/******************************* references ************************************/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_SECURITYMANAGE_C_AR_MAJOR_VERSION != DCMDSL_SECURITYMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SECURITYMANAGE_C_AR_MINOR_VERSION != DCMDSL_SECURITYMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SECURITYMANAGE_C_AR_PATCH_VERSION != DCMDSL_SECURITYMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SECURITYMANAGE_C_SW_MAJOR_VERSION != DCMDSL_SECURITYMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SECURITYMANAGE_C_SW_MINOR_VERSION != DCMDSL_SECURITYMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SECURITYMANAGE_C_SW_PATCH_VERSION != DCMDSL_SECURITYMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_SecurityManage.c : Mismatch in Specification Major Version"
#endif

/********************************************************
 *******************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(Dcm_SecCtrlType,DCM_VAR_NOINIT) Dcm_SecCtrl;  /*Security level management control block*/
#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
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
FUNC(void,DCM_CODE)DslInternal_InitSecCtrl(void)
{
    uint8   Index;
    boolean Flag = FALSE;
    P2CONST(Dcm_DspSecurityRowType,AUTOMATIC,DCM_CONST)pSecurityRow;

    SchM_Enter_Dcm(Dcm_SecCtrl);
    Dcm_SecCtrl.Dcm_RunDlyFlag         = FALSE;
    Dcm_SecCtrl.Dcm_LockedFlag         = FALSE;
    Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0u;
    Dcm_SecCtrl.Dcm_FalseAcessCount    = 0u;
    /****@req DCM-FUNR-030[DCM033]****/
    Dcm_SecCtrl.Dcm_ActiveSec          = DCM_SEC_LEV_LOCKED;
    Dcm_SecCtrl.Dcm_NewSec             = DCM_SEC_LEV_LOCKED;
    Dcm_SecCtrl.Dcm_SecServiceState    = DCM_SERVICE_IDLE;
    Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecCurTicks     = DCM_INVALID_UINT32;
    Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecExpiredTicks = DCM_INVALID_UINT32;
    Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecTimerState   = DCM_SECTIMER_OFF;
    Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTicks        = DCM_INVALID_UINT32;
    Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTicks    = DCM_INVALID_UINT32;

    Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTicks        = Frt_ReadOutTicks();
  	Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTicks    = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[0].DcmDspSecurityDelayTime;
  	Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState      = DCM_SECTIMER_ON;
	
	#if(STD_ON == DELAY_TIME_IN_BOOT_ENABLED)
    pSecurityRow = Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow;
    for(Index=0u;(Index<(Dcm_DspCfg.pDcm_DspSecurity->DcmDspSecurityRow_Num))&&(FALSE == Flag);Index++)
    {
       if(0u!=(pSecurityRow[Index].DcmDspSecurityDelayTimeOnBoot))
       {
           /*If at least one security level configuration items DcmDspSecurityDelayTimeOnBoot! = 0, you need to start the power-on delay.*/
           Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecCurTicks     = Frt_ReadOutTicks();
           Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecTimerState   = DCM_SECTIMER_ON;
           Flag = TRUE;
       }
    }
    #endif 
    /******************************************/
    /*Set security level equal to "Lock"*/
    Dcm_SecCtrl.Dcm_SecState = DCM_SEC_LOCKED;
    SchM_Exit_Dcm(Dcm_SecCtrl);
}
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
/****@req DCM-FUNR-029[DCM020]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE) DslInternal_SetSecurityLevel(Dcm_SecLevelType  NewSec)
{
    SchM_Enter_Dcm(Dcm_SecCtrl);
    Dcm_SecCtrl.Dcm_ActiveSec = NewSec;
    SchM_Exit_Dcm(Dcm_SecCtrl);

    SchM_Enter_Dcm(Dcm_MkCtrl);
    Dcm_MkCtrl.Dcm_ActiveSec  = NewSec;
    SchM_Exit_Dcm(Dcm_MkCtrl);
}
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
FUNC(void,DCM_CODE)DslInternal_SetSecurityAccessStatus(Dcm_SecServiceStateType Status)
{
    SchM_Enter_Dcm(Dcm_SecCtrl);
    Dcm_SecCtrl.Dcm_SecServiceState = Status;
    SchM_Exit_Dcm(Dcm_SecCtrl);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif

