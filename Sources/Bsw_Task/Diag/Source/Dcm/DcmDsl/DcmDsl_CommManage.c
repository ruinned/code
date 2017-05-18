/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_CommManage.c>
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
#define DCMDSL_COMMANAGE_C_VENDOR_ID  0
#define DCMDSL_COMMANAGE_C_MODULE_ID  0
#define DCMDSL_COMMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_COMMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_COMMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_COMMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_COMMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_COMMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_COMMANAGE_C_VENDOR_API_INFIX  0

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_COMMANAGE_C_AR_MAJOR_VERSION != DCMDSL_COMMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_COMMANAGE_C_AR_MINOR_VERSION != DCMDSL_COMMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_COMMANAGE_C_AR_PATCH_VERSION != DCMDSL_COMMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_COMMANAGE_C_SW_MAJOR_VERSION != DCMDSL_COMMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_COMMANAGE_C_SW_MINOR_VERSION != DCMDSL_COMMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_COMMANAGE_C_SW_PATCH_VERSION != DCMDSL_COMMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_CommManage.c : Mismatch in Specification Major Version"
#endif

/*******************************************************
 *********************Resource define*******************
********************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
STATIC  VAR(Dcm_CommCtrlType,DCM_VAR_NOINIT)Dcm_CommCtrl;  /*Diagnostic Communication Control*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

/****************************** implementations ********************************/
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
FUNC(void,DCM_CODE)DslInternal_InitComMCtrl(void)
{
    SchM_Enter_Dcm(Dcm_CommCtrl);
    Dcm_CommCtrl.Dcm_CommState = DCM_COMM_NO_COMMUNICATION;
    SchM_Exit_Dcm(Dcm_CommCtrl);
}
#define DCM_STOP_SEC_CODE
#include  "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_NO_COMMUNICATION>
 * ServiceId           <0x21>
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
/****@req DCM_APIR_029[DCM356]****/
/****@req DCM-APIR-030[DCM148]****/
/****@req DCM-APIR-031[DCM149]****/
/****@req DCM-APIR-032[DCM150]****/
/****@req DCM-APIR-033[DCM151]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_ComM_NoComModeEntered(uint8 channel)
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl);
    if(DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_COMM_NOCOMMODEENTERED,
                        DCM_E_UNINIT);
        SchM_Exit_Dcm(Dcm_MkCtrl);
        return;
    }
    SchM_Exit_Dcm(Dcm_MkCtrl);
#endif
    SchM_Enter_Dcm(Dcm_CommCtrl);
    Dcm_CommCtrl.Dcm_CommState = DCM_COMM_NO_COMMUNICATION;
    SchM_Exit_Dcm(Dcm_CommCtrl);
}
#define DCM_STOP_SEC_CODE
#include  "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_SILENT_COMMUNICATION>
 * ServiceId           <0x22>
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
/****@req Dcm_APIR_035[DCM358]****/
/****@req DCM-APIR-036[DCM153]****/
/****@req DCM-APIR-037[DCM154]****/
/****@req DCM-APIR-038[DCM155]****/
/****@req DCM-APIR-039[DCM156]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_ComM_SilentComModeEntered(uint8 channel)
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl);
    if(DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_COMM_SILENTCOMMODEENTERED,
                        DCM_E_UNINIT);
        SchM_Exit_Dcm(Dcm_MkCtrl);
        return;
    }
    SchM_Exit_Dcm(Dcm_MkCtrl);
#endif
    SchM_Enter_Dcm(Dcm_CommCtrl);
    Dcm_CommCtrl.Dcm_CommState = DCM_COMM_SILENT_COMMUNICATION;
    SchM_Exit_Dcm(Dcm_CommCtrl);
}
#define DCM_STOP_SEC_CODE
#include  "MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules, network communication mode is DCM_COMM_FULL_COMMUNICATION>
 * ServiceId           <0x23>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM_APIR_040[DCM360]****/
/****@req DCM-APIR-041[DCM157]****/
/****@req DCM-APIR-042[DCM159]****/
/****@req DCM-APIR-043[DCM160]****/
/****@req DCM-APIR-044[DCM161]****/
/****@req DCM-APIR-045[DCM162]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_ComM_FullComModeEntered(uint8 channel)
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl);
    if(DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_COMM_FULLCOMMODEENTERED,
                        DCM_E_UNINIT);
        SchM_Exit_Dcm(Dcm_MkCtrl);
        return;
    }
    SchM_Exit_Dcm(Dcm_MkCtrl);
#endif
    SchM_Enter_Dcm(Dcm_CommCtrl);
    Dcm_CommCtrl.Dcm_CommState = DCM_COMM_FULL_COMMUNICATION;
    SchM_Exit_Dcm(Dcm_CommCtrl);
}
#define DCM_STOP_SEC_CODE
#include  "MemMap.h"

/************************internal functions*****************************/
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
FUNC(void,DCM_CODE)Dcm_ComM_ReadCurComState(P2VAR(Dcm_CommStateType,AUTOMATIC,DCM_VAR)pState)
{
    if(NULL_PTR == pState)
    {
       return;
    }
    (*pState) = Dcm_CommCtrl.Dcm_CommState;
}
#define DCM_STOP_SEC_CODE
#include  "MemMap.h"



