/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_ProtocolManage.c>
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
#define DCMDSL_PROTOCOLMANAGE_C_VENDOR_ID  0
#define DCMDSL_PROTOCOLMANAGE_C_MODULE_ID  0
#define DCMDSL_PROTOCOLMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_PROTOCOLMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_PROTOCOLMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_PROTOCOLMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_PROTOCOLMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_PROTOCOLMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_PROTOCOLMANAGE_C_VENDOR_API_INFIX  0

/******************************* references ************************************/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_PROTOCOLMANAGE_C_AR_MAJOR_VERSION != DCMDSL_PROTOCOLMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PROTOCOLMANAGE_C_AR_MINOR_VERSION != DCMDSL_PROTOCOLMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PROTOCOLMANAGE_C_AR_PATCH_VERSION != DCMDSL_PROTOCOLMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PROTOCOLMANAGE_C_SW_MAJOR_VERSION != DCMDSL_PROTOCOLMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PROTOCOLMANAGE_C_SW_MINOR_VERSION != DCMDSL_PROTOCOLMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_PROTOCOLMANAGE_C_SW_PATCH_VERSION != DCMDSL_PROTOCOLMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_ProtocolManage.c : Mismatch in Specification Major Version"
#endif

/********************************************************
***************Resource statement************************
********************************************************/
#define DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(Dcm_ProtocolCtrlType,DCM_VAR_NOINIT)Dcm_ProtocolCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];/*Protocol control block*/
#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
/********************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetProtocolTicks(uint8  ProtocolCtrlId,
													   TickType P2ServerTicks,
													   TickType P2StarServerTicks);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


/*******************public functions******************************/
/*************************************************************************/
/*
 * Brief               <Initializing the corresponding protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitProtocolCtrl(uint8 ProtocolCtrlId)
{
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8    Index;
    boolean  Flag = FALSE;
#endif

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    Dcm_ProtocolCtrl[ProtocolCtrlId].ProtocolId = DCM_INVALID_UINT8;
    Dcm_ProtocolCtrl[ProtocolCtrlId].ProtocolPri= DCM_INVALID_UINT8;
    Dcm_ProtocolCtrl[ProtocolCtrlId].ProtocolPreemptTime = DCM_INVALID_UINT32;
    Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex        = DCM_INVALID_UINT8;
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax         = (50ul / MS_PER_TICK);   /* ms converted to tick */
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax     = (5000ul / MS_PER_TICK); /* ms converted to tick */
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMin         = 0ul;
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMin     = 0ul;
    Dcm_ProtocolCtrl[ProtocolCtrlId].S3Server            = (5000ul / MS_PER_TICK); /* ms converted to tick */
    /****@req DCM-FUNR-037[DCM027]****/
    /****@req DCM-FUNR-038[DCM143]****/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    for(Index=0;(Index<(Dcm_DspCfg.pDcm_DspSession->DcmDspSessionRow_Num))&&(FALSE==Flag);Index++)
    {
       if(DCM_DEFAULT_SESSION == (Dcm_DspCfg.pDcm_DspSession->pDcmDspSessionRow)[Index].DcmDspSessionLevel)
       {
           /*in DcmDspSessionRow,find the default session*/
           Flag = TRUE;
           Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax = (Dcm_DspCfg.pDcm_DspSession->pDcmDspSessionRow)[Index].DcmDspSessionP2ServerMax;
           Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax = (Dcm_DspCfg.pDcm_DspSession->pDcmDspSessionRow)[Index].DcmDspSessionP2StarServerMax;
       }
    }
#endif
    Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState   = DCM_PROTOCOL_INVALID;
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <protocal start>
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
FUNC(Std_ReturnType,DCM_CODE)DslInternal_ProtocolStart(uint8  ProtocolCtrlId)
{
    uint8  Flag = 0u;
    uint8  Index= 0u;
    uint8  PendingNumx = 0u;
    uint8  NotAllowedNum = 0u;
    uint8  ProtocolCfgId = 0u;
    Dcm_ProtocolType ProtocolIdx;
    Std_ReturnType ret;

    if(DCM_PROTOCOL_VALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
    {
        return E_OK;
    }
    ProtocolCfgId = ProtocolCtrlId;
    ProtocolIdx = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgId].DcmDslProtocolID;
    Flag = FALSE;
    for(Index=0;(Index<(Dcm_DslCfg.DCMCallBackDcmRequest_PortNum))&&(FALSE==Flag);Index++)
    {
        /****@req DCM-FUNR-046[DCM036]****/
        if(NULL_PTR == (Dcm_DslCfg.pDcmDslCallback_DCMRequestService)[Index].StartProtocol)
        {
            Flag = TRUE;
        }
        else
        {
            /*Notify the application, Start Protocol*/
            ret = (*((Dcm_DslCfg.pDcmDslCallback_DCMRequestService)[Index].StartProtocol))(ProtocolIdx);
            switch(ret)
            {
                case E_OK:
                     break;
                case E_PENDING:
                     PendingNumx++;
                     break;
                case E_PROTOCOL_NOT_ALLOWED:
                     NotAllowedNum++;
                     break;
                default:
                     NotAllowedNum++;
                     break;
            }
        }
    }
    if(TRUE == Flag)
    {
        (void)DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    else if(0u!=NotAllowedNum)
    {
        /*Application does not allow Start Protocol.*/
        (void)DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_PROTOCOL_NOT_ALLOWED;
    }
    else if(0u!=PendingNumx)
    {
        /*Application delay Start Protocol*/
        ret = E_PENDING;
    }
    else
    {
        SchM_Enter_Dcm(Dcm_ProtocolCtrl);
        Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState = DCM_PROTOCOL_VALID;
        SchM_Exit_Dcm(Dcm_ProtocolCtrl);
        /****@req DCM-FUNR-048[DCM145]****/
        /****@req DCM-FUNR-050[DCM147]****/
        /****@req DCM-FUNR-049[DCM146]****/
        /****@req DCM-FUNR-047[DCM144]****/
    #if(STD_ON == DCM_SESSION_FUNC_ENABLED)
        SchM_Enter_Dcm(Dcm_SesCtrl);
        Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
        SchM_Exit_Dcm(Dcm_SesCtrl);
        DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    #endif
        ret = E_OK;
    }
    return (ret);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set the the DCM module supports protocol corresponding to the link layer time>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number;
 *                      P2ServerTicks:To be set P2ServerTicks;
 *                      P2StarServerTicks:To be set P2StarServerTicks; >
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)DslInternal_SetProtocolTicks(uint8  ProtocolCtrlId,
													   TickType P2ServerTicks,
													   TickType P2StarServerTicks)
{
    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax     = P2ServerTicks;
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax = P2StarServerTicks;
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMin     = 0ul;
    Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMin = 0ul;
    Dcm_ProtocolCtrl[ProtocolCtrlId].S3Server        = (5000ul / MS_PER_TICK); /* ms converted to tick */
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol link layer parameters of time>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <P2ServerTicks:
 *                      P2StarServerTicks:>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DslInternal_SetProtocolLinkLayerTicks(TickType P2ServerTicks,TickType P2StarServerTicks)
{
   uint8    Num;
   uint8    ProtocolCtrlId;
   TickType P2ServerTicksBak;
   TickType P2StarServerTicksBak;
   TickType P2ServerTicksLimit;
   TickType P2StarServerTicksLimit;
   const P2VAR(Dcm_DslProtocolTimingRowType, AUTOMATIC, AUTOMATIC) pProtocolTimeLimit;

   P2ServerTicksBak     = P2ServerTicks;
   P2StarServerTicksBak = P2StarServerTicks;

   Num = Dcm_DslCfg.pDcmDslProtocol->DcmDslProtocolRow_Num;
   for(ProtocolCtrlId=0;ProtocolCtrlId<Num;ProtocolCtrlId++)
   {
       P2ServerTicks      = P2ServerTicksBak;
       P2StarServerTicks  = P2StarServerTicksBak;
       pProtocolTimeLimit = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCtrlId].pDcmDslProtocolTimeLimit;
       if(NULL_PTR == pProtocolTimeLimit)
       {
           /*the current protocol  Correspond to time parameters of the link layer unrestricted;*/
           DslInternal_SetProtocolTicks(ProtocolCtrlId,P2ServerTicks,P2StarServerTicks);
       }
       else
       {
           /*the current protocol  Correspond to time parameters of the link layer restricted;*/
           P2ServerTicksLimit     = pProtocolTimeLimit->DcmTimStrP2ServerMax;     /*Upper limit P2Server of the corresponding protocol*/
           P2StarServerTicksLimit = pProtocolTimeLimit->DcmTimStrP2StarServerMax; /*Upper limit P2StarServer of the corresponding protocol*/
           if(P2ServerTicks > P2ServerTicksLimit)
           {
               P2ServerTicks = P2ServerTicksLimit;
           }
           if(P2StarServerTicks > P2StarServerTicksLimit)
           {
               P2StarServerTicks = P2StarServerTicksLimit;
           }
           DslInternal_SetProtocolTicks(ProtocolCtrlId,P2ServerTicks,P2StarServerTicks);
       }
   }
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
