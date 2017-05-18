/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_PendingManage.c>
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
#define DCMDSL_PENDINGMANAGE_C_VENDOR_ID  0
#define DCMDSL_PENDINGMANAGE_C_MODULE_ID  0
#define DCMDSL_PENDINGMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_PENDINGMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_PENDINGMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_PENDINGMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_PENDINGMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_PENDINGMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_PENDINGMANAGE_C_VENDOR_API_INFIX  0

/******************************* references ************************************/
#include "Dcm_Include.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_PENDINGMANAGE_C_AR_MAJOR_VERSION != DCMDSL_PENDINGMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PENDINGMANAGE_C_AR_MINOR_VERSION != DCMDSL_PENDINGMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PENDINGMANAGE_C_AR_PATCH_VERSION != DCMDSL_PENDINGMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PENDINGMANAGE_C_SW_MAJOR_VERSION != DCMDSL_PENDINGMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PENDINGMANAGE_C_SW_MINOR_VERSION != DCMDSL_PENDINGMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PENDINGMANAGE_C_SW_PATCH_VERSION != DCMDSL_PENDINGMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_PendingManage.c : Mismatch in Specification Major Version"
#endif

/********************************public functions******************************/
/*************************************************************************/
/*
 * Brief               <Start P2Timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerStart(uint8  ProtocolCtrlId)
{
    uint8  MsgCtrlIndexx;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if(DCM_P2TIMER_ON == Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2State)
    {
        return  E_OK;
    }
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.PendingNum         = 0u;
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2CurTicks     = Frt_ReadOutTicks();
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2ExpiredTicks = Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax;
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2State        = DCM_P2TIMER_ON;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <stop P2timer>
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
FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerStop(uint8  ProtocolCtrlId)
{
    uint8  MsgCtrlIndexx;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if(DCM_P2TIMER_OFF == Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2State)
    {
        return  E_OK;
    }
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2State = DCM_P2TIMER_OFF;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <P2Timer timeout processing>
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
FUNC(Std_ReturnType,DCM_CODE)DslInternal_P2ServerTimeout(uint8  ProtocolCtrlId)
{
    uint8  PendingNumx;
    uint8  MsgCtrlId;
    uint8  PendingMaxTimes;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    MsgCtrlId        =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    PendingMaxTimes  =  Dcm_DslCfg.pDcmDslDiagRespCfg->DcmDslDiagRespMaxNumRespPend;
    PendingNumx      =  Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.PendingNum;
    if( PendingNumx < PendingMaxTimes)
    {
        /*Send Nrc=0x78*/
        /****@req DCM-FUNR-010[DCM024]****/
        Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_PROCESSED;
        (void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_RESPONSE_PENDING);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    else
    {
        /*Send Nrc=0x10*/
        /****@req DCM-FUNR-012[DCM120]****/
        Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_PROCESSED;
        (void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_GENERALREJECT);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Judgment P2Timer run time is more than P2ServerMin>
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
FUNC(Std_ReturnType,DCM_CODE)DslInternal_IsLargerThanP2ServerMin(uint8  ProtocolCtrlId)
{
    uint8    MsgCtrlIndexx;
    TickType P2ServerMinx;
    TickType OldTicks;
    TickType Ticks;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    OldTicks     = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2CurTicks;
    Ticks        = Frt_CalculateElapsedTicks(OldTicks);
    P2ServerMinx  = Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMin;
    if(Ticks < P2ServerMinx)
    {
        return  E_NOT_OK;
    }
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
