/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_SessionManage.c>
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
#define DCMDSL_SESSIONMANAGE_C_VENDOR_ID  0
#define DCMDSL_SESSIONMANAGE_C_MODULE_ID  0
#define DCMDSL_SESSIONMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_SESSIONMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_SESSIONMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_SESSIONMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_SESSIONMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_SESSIONMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_SESSIONMANAGE_C_VENDOR_API_INFIX  0

/******************************* references ************************************/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"
#include "Rte_Dem.h"

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_SESSIONMANAGE_C_AR_MAJOR_VERSION != DCMDSL_SESSIONMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SESSIONMANAGE_C_AR_MINOR_VERSION != DCMDSL_SESSIONMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SESSIONMANAGE_C_AR_PATCH_VERSION != DCMDSL_SESSIONMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SESSIONMANAGE_C_SW_MAJOR_VERSION != DCMDSL_SESSIONMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SESSIONMANAGE_C_SW_MINOR_VERSION != DCMDSL_SESSIONMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_SESSIONMANAGE_C_SW_PATCH_VERSION != DCMDSL_SESSIONMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_SessionManage.c : Mismatch in Specification Major Version"
#endif

/********************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetSesCtrlType(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType,DCM_CODE)DslInternal_GetSesTimingValues(Dcm_SesType NewSes,
																   P2VAR(TickType,AUTOMATIC,DCM_VAR)P2ServerTicks,
																   P2VAR(TickType,AUTOMATIC,DCM_VAR)P2StarServerTicks);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetSesTimingValues(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_DefaultToDefault(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_DefaultToUndefault(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_UndefaultToUndefault(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_UndefaultTodefault(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SesTranslation(Dcm_SesType NewSes);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************************************
 **********************resource define*******************
********************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(Dcm_SesCtrlType,DCM_VAR_NOINIT) Dcm_SesCtrl;/*Session management control block*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

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
FUNC(void,DCM_CODE) DslInternal_InitSesCtrl(void)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    /****@req DCM-FUNR-033[DCM034]****/
    Dcm_SesCtrl.Dcm_ActiveSes = DCM_DEFAULT_SESSION;
    Dcm_SesCtrl.Dcm_NewSes    = DCM_DEFAULT_SESSION;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTicks     = DCM_INVALID_UINT32;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTicks = DCM_INVALID_UINT32;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State        = DCM_S3TIMER_OFF;
    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_DEFAULT;
    SchM_Exit_Dcm(Dcm_SesCtrl);
}
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
FUNC(void,DCM_CODE) DslInternal_S3ServerStart(void)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTicks     = Frt_ReadOutTicks();
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTicks = (5000ul / MS_PER_TICK); /* ms converted to tick */
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State        = DCM_S3TIMER_ON;
    SchM_Exit_Dcm(Dcm_SesCtrl);
}
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
FUNC(void,DCM_CODE)DslInternal_S3ServerStop(void)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State = DCM_S3TIMER_OFF;
    SchM_Exit_Dcm(Dcm_SesCtrl);
}
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
FUNC(void,DCM_CODE)DslInternal_S3ServerTimeout(void)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
    SchM_Exit_Dcm(Dcm_SesCtrl);
    /****@req DCM-FUNR-034[DCM140]****/
    DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    /***stop S3Timer***/
    DslInternal_S3ServerStop();
}
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
FUNC(void,DCM_CODE)DslInternal_SesRefresh(Dcm_SesType NewSes)      /* the receive session 20170316 */
{
    uint8    Index;
    boolean  Flag;
    Dcm_SesType  ActiveSes;
    Std_ReturnType ret;

    ActiveSes = (Dcm_SesCtrl.Dcm_ActiveSes);
    /****@req DCM-FUNR-036[DCM142]****/
    Flag = FALSE;
    for(Index=0;(Index<(Dcm_DslCfg.DcmDslSessionControl_PortNum))&&(FALSE==Flag);Index++)
    {
       /*Notify the application session state changes (including Self-transition)*/
       if(NULL_PTR == ((Dcm_DslCfg.pDcmDslSessionControl)[Index].ChangeIndication))
       {
           Flag = TRUE;
       }
       else
       {
           ret = (*((Dcm_DslCfg.pDcmDslSessionControl)[Index].ChangeIndication))(ActiveSes,NewSes);
           if(E_NOT_OK == ret)
           {
              Flag = TRUE;
           }
       }
    }
    if(FALSE == Flag)
    {
      /*All configuration interface allows the session to change.*/
      DslInternal_SesTranslation(NewSes);
    }
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/************************internal functions*****************************/
/*************************************************************************/
/*
 * Brief               <Set the session control state>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-032[DCM022]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetSesCtrlType(Dcm_SesType NewSes)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_ActiveSes = NewSes;
    SchM_Exit_Dcm(Dcm_SesCtrl);

    SchM_Enter_Dcm(Dcm_MkCtrl);
    Dcm_MkCtrl.Dcm_ActiveSes  = NewSes;
    SchM_Exit_Dcm(Dcm_MkCtrl);
}
FUNC(uint8,DCM_CODE)DslInternal_GetSesCtrlType(void)
{
	return Dcm_SesCtrl.Dcm_ActiveSes;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <The set NewSes corresponding configuration session time parameters>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <(*P2ServerTicks):P2ServerTicks which corresponding to NewSes
 *                      (*P2StarServerTicks):P2StarServerTicks which corresponding to NewSes>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType,DCM_CODE)DslInternal_GetSesTimingValues(Dcm_SesType NewSes,
																   P2VAR(TickType,AUTOMATIC,DCM_VAR)P2ServerTicks,
																   P2VAR(TickType,AUTOMATIC,DCM_VAR)P2StarServerTicks)
{
   uint8    Number;
   uint8    Index;
   boolean  Flag;
   P2CONST(Dcm_DspSessionRowType,AUTOMATIC,DCM_CONST)pDspSessionRow;

   if(  (NULL_PTR == P2ServerTicks)
      ||(NULL_PTR == P2StarServerTicks))
   {
       return E_NOT_OK;
   }

   pDspSessionRow = (Dcm_DspCfg.pDcm_DspSession)->pDcmDspSessionRow;
   Number  	= (Dcm_DspCfg.pDcm_DspSession)->DcmDspSessionRow_Num;
   Flag 	= FALSE;
   for(Index=0;(Index<Number)&&(FALSE==Flag);Index++)
   {
       if(NewSes == (pDspSessionRow[Index].DcmDspSessionLevel))
       {
           Flag = TRUE;
       }
   }
   if(FALSE == Flag)
   {
       /*do not configured DcmDspSessionRow container ,or  do not find the corresponding NewSes in DcmDspSessionRows*/
       return E_NOT_OK;
   }
   (*P2ServerTicks)     = pDspSessionRow[Index-1].DcmDspSessionP2ServerMax;
   (*P2StarServerTicks) = pDspSessionRow[Index-1].DcmDspSessionP2StarServerMax;
   return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol  link layer parameters ,which is in protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetSesTimingValues(Dcm_SesType NewSes)
{
	TickType P2ServerTicks;
	TickType P2StarServerTicks;
    Std_ReturnType  ret;

    /*Get the new session configuration corresponding to the time parameters*/
    ret = DslInternal_GetSesTimingValues(NewSes,&P2ServerTicks,&P2StarServerTicks);
    if(E_NOT_OK == ret)
    {
        return;
    }
    /*set all protocols which are configured,corresponding link layer Time parameters*/
    DslInternal_SetProtocolLinkLayerTicks(P2ServerTicks,P2StarServerTicks);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <session change from default to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_DefaultToDefault(Dcm_SesType NewSes)
{
    ;
   /************************************************************
    * The session state from the default to the default,
    * specific needs to be done temporarily unclear, to be added
    ************************************************************/
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <session change from default to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE) DslInternal_DefaultToUndefault(Dcm_SesType NewSes)
{
    SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_UNDEFAULT;
    SchM_Exit_Dcm(Dcm_SesCtrl);

    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);
    /* Notice Comm module into "Full Communication" */
    /****@req DCM-FUNR-056[DCM167]****/
    ComM_DCM_ActiveDiagnostic(DCM_COMM_CHANNEL_NO);
    /************************************************************
     * The session state from the default to the undefault,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <session change from undefault to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_UndefaultToUndefault(Dcm_SesType NewSes)
{
    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
    /****@req DCM-FUNR-031[DCM139]****/
    DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
    DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
#endif
    /************************************************************
     * The session state from the undefault to the undefault,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <session change from undefault to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_UndefaultTodefault(Dcm_SesType NewSes)
{
	SchM_Enter_Dcm(Dcm_SesCtrl);
    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_DEFAULT;
    SchM_Exit_Dcm(Dcm_SesCtrl);

    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);
    
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
    /****@req DCM-FUNR-031[DCM139]****/
    DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
    DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
#endif

#if((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X85_ENABLED))
    /*The update of the DTC status will be re-enabled*/
    Rte_EnableAllDtcsRecord();
#endif

#if((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X28_ENABLED ))
    /*00=enableRxAndTx,0x01 =normalCommunicationMessages*/
    Rte_CommunicaitonControl(00,0x01);
#endif
    /*Notice Comm Module exit "Full Communication"*/
    /****@req DCM-FUNR-057[DCM168]****/
    ComM_DCM_InactiveDiagnostic(DCM_COMM_CHANNEL_NO);
    /************************************************************
     * The session state from the undefault to the default,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <When the session is changed, the specific processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE) DslInternal_SesTranslation(Dcm_SesType NewSes)
{
    if(DCM_SESSION_DEFAULT == Dcm_SesCtrl.Dcm_SessionState)
    {
        if(DCM_DEFAULT_SESSION == NewSes)
        {
            /*from the default session to the default session */
            DslInternal_DefaultToDefault(NewSes);
        }
        else
        {
            /*from the default session to the undefault session */
            DslInternal_DefaultToUndefault(NewSes);
        }
    }
    else
    {
        if(DCM_DEFAULT_SESSION == NewSes)
        {
            /*from the undefault session to the default session */
            DslInternal_UndefaultTodefault(NewSes);
        }
        else
        {
            /*from the undefault session to the undefault session */
            DslInternal_UndefaultToUndefault(NewSes);
        }
    }
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif
