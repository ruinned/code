/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsd.c>
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
#define DCMDSD_C_VENDOR_ID  0
#define DCMDSD_C_MODULE_ID  0
#define DCMDSD_C_AR_MAJOR_VERSION  3
#define DCMDSD_C_AR_MINOR_VERSION  3
#define DCMDSD_C_AR_PATCH_VERSION  0
#define DCMDSD_C_SW_MAJOR_VERSION  1
#define DCMDSD_C_SW_MINOR_VERSION  0
#define DCMDSD_C_SW_PATCH_VERSION  0
#define DCMDSD_C_VENDOR_API_INFIX  0
/******************************* references ************************************/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSD_C_AR_MAJOR_VERSION != DCMDSD_H_AR_MAJOR_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSD_C_AR_MINOR_VERSION != DCMDSD_H_AR_MINOR_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSD_C_AR_PATCH_VERSION != DCMDSD_H_AR_PATCH_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSD_C_SW_MAJOR_VERSION != DCMDSD_H_SW_MAJOR_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSD_C_SW_MINOR_VERSION != DCMDSD_H_SW_MINOR_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSD_C_SW_PATCH_VERSION != DCMDSD_H_SW_PATCH_VERSION)
  #error "DcmDsd.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************/
/*Defined NRC = 0x78 channel*/
#define DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
STATIC  VAR(uint8,DCM_VAR_POWER_ON_INIT)Dcm_Nrc78Channel[8] = {0};  /*Send NRC = 0x78 channel (Note: The channel length range: 8 ~ 4095)*/
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

/******************************* references ************************************/
/*****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SuppressPosRsp(uint8 ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SidCheck(uint8  ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_AddressingFormatCheck(uint8  ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#if(STD_ON == DCM_REQUEST_INDICATION_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_IndicationCheck(uint8 ProtocolCtrlId,uint8 Sid);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*******************internal functions****************************/
/*************************************************************************/
/*
 * Brief               <Find SID corresponding service configuration table index
 *                      and the service index in corresponding service configuration table.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId: The corresponding protocol control block ID number>
 * Param-Name[out]     <(*pSidTabIndex):Request packet index number in the service configuration table;
 *                      (*pSidTabServieCfgIndex): the service index in corresponding service configuration table>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-062[DCM192]****/
/****@req DCM-FUNR-063[DCM193]****/
/****@req DCM-FUNR-065[DCM196]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE) DsdInternal_SearchSidTabServiceIndex(uint8   Sid,
																   uint8   ProtocolCtrlId,
																   P2VAR(uint16,AUTOMATIC, DCM_VAR)pSidTabIndex,
																   P2VAR(uint16,AUTOMATIC, DCM_VAR)pSidTabServieCfgIndex)
{
    uint8  SidTabId;
    uint8  ProtocolCfgIndex;
    uint8  Service_Num;
    uint16 SidTabIndex;
    uint16 ServiceIndex;
    boolean  Flag;
    P2CONST(Dcm_DsdServiceCfgType, AUTOMATIC,DCM_CONST)pDsdSidTab;

    if(  (NULL_PTR == pSidTabIndex)
       ||(NULL_PTR == pSidTabServieCfgIndex) )
    {
        return E_NOT_OK;
    }
    ProtocolCfgIndex = ProtocolCtrlId;
    /*Read the protocol includes the service table ID */
    SidTabId  = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].DcmDslServiceTableID;
    /*find SidTabId configuration position in the service configuration table*/
    Flag = FALSE;
    for(SidTabIndex=0;(SidTabIndex<Dcm_DsdCfg.DcmDsdServiceTable_Num)&&(FALSE==Flag);SidTabIndex++)
    {
        if(SidTabId == ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabIndex].DcmDsdSidTabId))
        {
            Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
        /*Did not find the service table ID,in the configuration table*/
        return  E_NOT_OK;
    }
    (*pSidTabIndex) = SidTabIndex - (uint16)1;

    /*Read the first address of the corresponding service table*/
    pDsdSidTab  = (Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabIndex-1].pDcmDsdService;

    /*Read the number of services,in the corresponding service table*/
    Service_Num = (Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabIndex-1].DcmDsdSidTab_ServiceNum;
    Flag = FALSE;
    for(ServiceIndex=0;(ServiceIndex<Service_Num)&&(FALSE==Flag);ServiceIndex++)
    {
        if(Sid == (pDsdSidTab[ServiceIndex].DcmDsdSidTabServiceId))
        {
           Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
      /*Did not find the service ID,in the corresponding service table*/
      return  E_NOT_OK;
    }
    (*pSidTabServieCfgIndex) = ServiceIndex - (uint16)1;
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Save a positive response prohibit information>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-068[DCM200]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SuppressPosRsp(uint8 ProtocolCtrlId,
		                                                        uint8 Sid)
{
    uint8  MsgCtrlIndexx;
    uint8  SubFunction;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    boolean  SubFuncAvial;
    Std_ReturnType  ret;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid,
                                               ProtocolCtrlId,
                                               &SidTabCfgIndex,
                                               &SidTabServiceCfgIndex);
    if(E_NOT_OK == ret)
    {
       return  E_NOT_OK;
    }

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    /****@req DCM-FUNR-072[DCM204]****/
    SubFuncAvial = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].DcmDsdSidTabSubfuncAvial;
    if(( TRUE == SubFuncAvial ) && ((0x10u == Sid) || (0x11u == Sid)|| (0x27u == Sid) ||(0x28u == Sid) || 
               (0x3Eu == Sid) || (0x85u == Sid) || (0x22u == Sid)|| (0x2Eu == Sid)|| 
               (0x14u == Sid) || (0x19u == Sid)|| (0x2Fu == Sid)|| (0x31u == Sid)))
    {
        MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        SubFunction  = (Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData)[1];
        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.SuppressPosResponse  = FALSE;
        if(0x80u == (SubFunction & 0x80u))
        {
          Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.SuppressPosResponse = TRUE;
          /*Set response type "DCM_POS_RSP_SUPPRESS"*/
          /****@req DCM-FUNR-068[DCM200]****/
          Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle = DCM_POS_RSP_SUPPRESS;
        }
        /*Clear positive response Prohibition information*/
        /****@req DCM-FUNR-069[DCM201]****/
        (Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData)[1] = (SubFunction & 0x7Fu);
    }
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    return E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Check request packet whether the SID is configured in the service table>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SidCheck(uint8  ProtocolCtrlId,
		                                                  uint8  Sid)
{
    uint16  SidTabCfgIndex;
    uint16  SidTabServiceCfgIndex;
    Std_ReturnType  ret;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid,
                                               ProtocolCtrlId,
                                               &SidTabCfgIndex,
                                               &SidTabServiceCfgIndex);
    return(ret);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Check request packet addressing type whether is correct.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_AddressingFormatCheck(uint8  ProtocolCtrlId,
		                                                               uint8  Sid)
{
	uint8  MsgCtrlIndexx;
	uint16 SidTabCfgIndex;
	uint16 SidTabServiceCfgIndex;
	Dcm_DslProtocolRxAddrType  RxServiceAddressingFormat;
	Dcm_DsdServiceAddressingFormatType  CfgServiceAddressingFormat;
    Std_ReturnType  ret;

    /*******************************************************/
	ret = DsdInternal_SearchSidTabServiceIndex(Sid,
											   ProtocolCtrlId,
											   &SidTabCfgIndex,
											   &SidTabServiceCfgIndex);
    if(E_NOT_OK == ret)
    {
    	return(E_NOT_OK);
    }
    /*Read the service configuration support addressing*/
    CfgServiceAddressingFormat = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].AddressingFormat;
    if(DCM_ADDRESSING_PHYANDFUNC == CfgServiceAddressingFormat)
    {
    	/*The service supports functional addressing and physical addressing*/
    	return(E_OK);
    }
    /*Read request packet addressing type*/
	MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
	RxServiceAddressingFormat = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
    if(CfgServiceAddressingFormat != RxServiceAddressingFormat)
    {
        /*Configure addressing type is different from the actual addressing type*/
    	return(E_NOT_OK);
    }
    return(E_OK);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


#if(STD_ON == DCM_REQUEST_INDICATION_ENABLED)
/*************************************************************************/
/*
 * Brief               <Diagnostic request packet is whether allowed in the current application environment>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant/Non Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)DsdInternal_IndicationCheck(uint8 ProtocolCtrlId,
		                                                         uint8 Sid)
{
    uint8  MsgCtrlIndexx;
    uint8  PortNum;
    uint8  PortIndex;
    uint8  NotAcceptNum = 0u;
    uint8  Env_NokNum = 0u;
    uint8  PendingNumx = 0u;
    uint16 ReqDatalen;
    boolean  Flag;
    Std_ReturnType  ret = E_NOT_OK;
    P2VAR(uint8, AUTOMATIC, DCM_VAR)pReqDatax;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);

    MsgCtrlIndexx =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pReqDatax     =  &Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData[1];
    ReqDatalen   =   (uint16)(Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen - 1u);

    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);

    /****@req DCM-FUNR-075[DCM218]****/
    Flag = FALSE;
    PortNum = Dcm_DslCfg.DcmServiceRequestIndication_PortNum;
    for(PortIndex=0;(PortIndex<PortNum)&&(FALSE==Flag);PortIndex++)
    {
        if(NULL_PTR == ((Dcm_DslCfg.pDcmServiceRequestIndication)[PortIndex].Indication))
        {
            Flag = TRUE;
        }
        else
        {
            ret = (*((Dcm_DslCfg.pDcmServiceRequestIndication)[PortIndex].Indication))(Sid,pReqDatax,ReqDatalen);
            switch(ret)
            {
                case E_OK:
                     break;
                case E_REQUEST_NOT_ACCEPTED:
                     NotAcceptNum++;
                     break;
                case E_REQUEST_ENV_NOK:
                     Env_NokNum++;
                     break;
                case E_PENDING:
                     PendingNumx++;
                     break;
                default:
                     NotAcceptNum++;
                     break;
            }
        }
    }
    if(TRUE == Flag)
    {
        ret = E_NOT_OK;
    }
    else if(0u!=NotAcceptNum)
    {
        /*the request is not accepted*/
        ret = E_REQUEST_NOT_ACCEPTED;
    }
    else if(0u!=Env_NokNum)
    {
        /*the request is not allowed in the current Environment.*/
        ret = E_REQUEST_ENV_NOK;
    }
    else if(0u!=PendingNumx)
    {
        ret = E_PENDING;
    }
    else
    {
        /*nothing to do*/
    }
    return  ret;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               <Determine the diagnostic request packet is allowed in the current session state.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SesCheck(uint8 ProtocolCtrlId,uint8 Sid)
{
    uint8   ActiveSes;
    uint8   SesRefNum;
    uint8   SesCfgIndex;
    uint16  SidTabCfgIndex;
    uint16  SidTabServiceCfgIndex;    
    boolean  Flag;
    Std_ReturnType  ret;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid,
                                               ProtocolCtrlId,
                                               &SidTabCfgIndex,
                                               &SidTabServiceCfgIndex);
    if(E_NOT_OK == ret)
    {
       /*Did not find the corresponding service in the configuration table*/
       return  E_NOT_OK;
    }
    /******************************/
    SesRefNum = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].DcmDsdSidTabSessionLevel_Num;
    if(0u == SesRefNum)
    {
       /*No session state constraints*/
       return  E_OK;
    }
    /******************************/
    /*get Currently valid session state*/
    SchM_Enter_Dcm(Dcm_MkCtrl);
    ActiveSes = Dcm_MkCtrl.Dcm_ActiveSes;
    SchM_Exit_Dcm(Dcm_MkCtrl);

    Flag = FALSE;
    for(SesCfgIndex=0;(SesCfgIndex<SesRefNum)&&(FALSE==Flag);SesCfgIndex++)
    {
        if(ActiveSes == ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].pDcmDsdSidTabSessionLevelRef[SesCfgIndex])
        {
            Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
        /*this service is not supported in the current valid session status*/
        return  E_NOT_OK;
    }
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               <determine the diagnostic request packet is allowed in the current security level>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SecurityCheck(uint8 ProtocolCtrlId,uint8 Sid)
{
    uint8   ActiveSec;
    uint8   SecRefNum;
    uint8   SecCfgIndex;
    uint16  SidTabCfgIndex;
    uint16  SidTabServiceCfgIndex;
    P2CONST(uint8,AUTOMATIC,DCM_CONST)pSecCfg;
    boolean Flag;
    Std_ReturnType  ret;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid,
                                               ProtocolCtrlId,
                                               &SidTabCfgIndex,
                                               &SidTabServiceCfgIndex);
    if(E_NOT_OK == ret)
    {
    	/*Did not find the corresponding service in the configuration table*/
        return  E_NOT_OK;
    }

    /******************************/
    SecRefNum = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].DcmDsdSidTabSecurityLevel_Num;
    if(0u == SecRefNum)
    {
        /*No session state constraints*/
        return  E_OK;
    }
    /******************************/
    /*get Currently valid security level*/
    SchM_Enter_Dcm(Dcm_MkCtrl);
    ActiveSec = Dcm_MkCtrl.Dcm_ActiveSec;
    SchM_Exit_Dcm(Dcm_MkCtrl);

    Flag = FALSE;
    pSecCfg = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].pDcmDsdSidTabSecurityLevelRef;
    for(SecCfgIndex=0;(SecCfgIndex<SecRefNum)&&(FALSE==Flag);SecCfgIndex++)
    {
#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
        if(gFactory_ActiveFlag || (ActiveSec == pSecCfg[SecCfgIndex]))/*yb*/
#else
        if(ActiveSec == pSecCfg[SecCfgIndex])
        #endif
        {
            Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
    	/*this service is not supported in the current valid security level*/
        return  E_NOT_OK;
    }
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*******************public functions******************************/
/*************************************************************************/
/*
 * Brief               <Set the negative response code NRC;>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Nrc: the negative response code;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SetNrc(uint8 ProtocolCtrlId,
		                                        uint8 Nrc)
{
    uint8  MsgCtrlIndexx;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_MsgCtrl[MsgCtrlIndexx].NRC 		= Nrc;
    Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle 	= DCM_NEG_RSP;

    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <Set corresponding message processing state>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      MsgState: message processing state>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DsdInternal_SetMsgState(uint8 ProtocolCtrlId,
		                                             Dcm_MsgStateType MsgState)
{
    uint8  MsgCtrlIndexx;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = MsgState;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <The diagnostic request processing in DSD layer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-060[DCM178]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DsdInternal_RxIndication(uint8  ProtocolCtrlId)
{
    uint8  Sid;
    uint8  MsgCtrlId;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    Dcm_MsgStateType MsgState;
    Std_ReturnType ret;
    P2FUNC(Std_ReturnType, AUTOMATIC, Functionx)(uint8  ProtocolCtrlId);

    /*************************************************/
    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    MsgState  = Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState;
    Sid       = Dcm_MsgCtrl[MsgCtrlId].SID;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);

    /*************************************************/
    if(DCM_MSG_RECEIVED != MsgState)
    {
        /*The error message status, reset message processing*/
        (void)DslInternal_ResetResource(ProtocolCtrlId);
        return  E_NOT_OK;
    }
    /*************************************************/
    /*SID range checks*/
    /****@req DCM-FUNR-061[DCM084]****/
#if(STD_OFF == DCM_RESPOND_ALL_REQUEST)
    if(  ((Sid>=0x40u)&&(Sid<=0x7Fu))
       ||(Sid>=0xC0u))
    {
        /*(0x40=<SID<=0x7F)»ò(0xC0=<SID<=0xFF)*/
        DslInternal_ResetResource(ProtocolCtrlId);
        return  E_NOT_OK;
    }
#endif
    /*************************************************/
    /*Check whether the SID is configured in the corresponding service table*/
    ret = DsdInternal_SidCheck(ProtocolCtrlId,Sid);
    if(E_NOT_OK == ret)
    {
        /****@req DCM-FUNR-066[DCM197]****/
        /*Set message status "DCM_MSG_PROCESSED"*/
        (void)DsdInternal_SetMsgState(ProtocolCtrlId,DCM_MSG_PROCESSED);
        /*Set Nrc=0x11(service not supported )*/
        (void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTED);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        return E_NOT_OK;
    }
    /*************************************************/
    /*Check whether received packets addressing modes support*/
    ret = DsdInternal_AddressingFormatCheck(ProtocolCtrlId,Sid);
    if(E_NOT_OK == ret)
    {
    	/*Set message status "DCM_MSG_PROCESSED"*/
    	(void)DsdInternal_SetMsgState(ProtocolCtrlId,DCM_MSG_PROCESSED);
    	/*Set Nrc=0x11(service not supported )*/
    	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTED);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        return E_NOT_OK;
    }
    /*************************************************/
    /*positive response  Suppress bit processing*/
    ret = DsdInternal_SuppressPosRsp(ProtocolCtrlId,Sid);
    if(E_NOT_OK == ret)
    {
    	(void)DslInternal_ResetResource(ProtocolCtrlId);
        return E_NOT_OK;
    }
    /*************************************************/
    /*Application environment and permissions verification*/
#if(STD_ON == DCM_REQUEST_INDICATION_ENABLED)
    ret = DsdInternal_IndicationCheck(ProtocolCtrlId,Sid);
    switch(ret)
    {
        case  E_OK:
              break;
        case  E_NOT_OK:
        	  (void)DslInternal_ResetResource(ProtocolCtrlId);
              break;
        case  E_REQUEST_NOT_ACCEPTED:
              /****@req DCM-FUNR-076[DCM462]****/
        	  (void)DslInternal_ResetResource(ProtocolCtrlId);
              break;
        case  E_REQUEST_ENV_NOK:
              /****@req DCM-FUNR-077[DCM463]****/
        	  (void)DsdInternal_SetMsgState(ProtocolCtrlId,DCM_MSG_PROCESSED);
              /*Set Nrc=0x22(condition not correct)*/
        	  (void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
              DsdInternal_ProcessingDone(ProtocolCtrlId);
              break;
        case  E_PENDING:
              break;
        default:
        	  (void)DslInternal_ResetResource(ProtocolCtrlId);
              break;
    }
#endif
    if(E_OK == ret)
    {
        SchM_Enter_Dcm(Dcm_ProtocolCtrl);
        SchM_Enter_Dcm(Dcm_MsgCtrl);
        MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_PROCESSED;

        SchM_Exit_Dcm(Dcm_MsgCtrl);
        SchM_Exit_Dcm(Dcm_ProtocolCtrl);
        (void)DsdInternal_SearchSidTabServiceIndex(Sid,
                                                   ProtocolCtrlId,
                                                   &SidTabCfgIndex,
                                                   &SidTabServiceCfgIndex);
        /****@req DCM-FUNR-070[DCM202]****/
        /****@req DCM-FUNR-078[DCM221]****/
        Functionx = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].Function;
        /*Enter the specific service handler*/
        ret = (*Functionx)(ProtocolCtrlId);
        return ret;
    }
    return E_NOT_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Response to a diagnosis request processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-079[DCM222]****/
/****@req DCM-FUNR-080[DCM223]****/
/****@req DCM-FUNR-081[DCM224]****/
/****@req DCM-FUNR-082[DCM225]****/
/****@req DCM-FUNR-083[DCM228]****/
/****@req DCM-FUNR-084[DCM231]****/
/****@req DCM-FUNR-091[DCM240]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DsdInternal_ProcessingDone(uint8  ProtocolCtrlId)
{
    uint8   Nrc;
    uint8   MsgCtrlIndexx;
    uint8   TxChannelCtrlIndex;
    uint8   TxChannelCfgIndex;
    uint16  Offset;
    Dcm_RspType   RspStylex;
    Dcm_MsgStateType  MsgState;
    Dcm_DslProtocolRxAddrType ReqTypex;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    MsgState     = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState;
    if(DCM_MSG_PROCESSED != MsgState)
    {
        /*Message processing error*/
    	(void)DslInternal_ResetResource(ProtocolCtrlId);
        SchM_Exit_Dcm(Dcm_MsgCtrl);
        SchM_Exit_Dcm(Dcm_ProtocolCtrl);
        return;
    }
    /*Set message status "DCM_MSG_TRANSMISSION "*/
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = DCM_MSG_TRANSMISSION;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex;

    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);

    RspStylex =  Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle;
    switch(RspStylex)
    {
        case  DCM_POS_RSP:  /*--------------positive response---------------*/
              TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
              Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
              /*Store the response data to the corresponding transmit channel*/
              SchM_Enter_Dcm(Dcm_MsgCtrl);
              Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData = &Dcm_Channel[Offset];
              SchM_Exit_Dcm(Dcm_MsgCtrl);
              /****@req DCM-FUNR-006[DCM114]****/
              DslInternal_ProcessingDone(ProtocolCtrlId);
              break;

        case  DCM_NEG_RSP: /*---------------Negative response---------------*/
              SchM_Enter_Dcm(Dcm_MsgCtrl);
              ReqTypex  = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;/* Diagnosis  request type */
              Nrc      = Dcm_MsgCtrl[MsgCtrlIndexx].NRC;
              if((DCM_FUNCTIONAL == ReqTypex)
                 &&( (DCM_E_SERVICENOTSUPPORTED == Nrc)
                     ||(DCM_E_SUBFUNCTIONNOTSUPPORTED == Nrc)
                     ||(DCM_E_REQUESTOUTOFRANGE == Nrc) ) )
              {
                  /****@req DCM-FUNR-085[DCM001]****/
            	  (void)DslInternal_ResetResource(ProtocolCtrlId);
                  SchM_Exit_Dcm(Dcm_MsgCtrl);
              }
              else if(DCM_E_RESPONSE_PENDING == Nrc)
              {
                  /*NRC=0x78*/
                  /****@req DCM-FUNR-011[DCM119]****/
                  /****@req DCM-FUNR-071[DCM203]****/
                  /****@req DCM-FUNR-095[DCM038]****/
                  Dcm_Nrc78Channel[0] = 0x7F;
                  Dcm_Nrc78Channel[1] = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
                  Dcm_Nrc78Channel[2] = DCM_E_RESPONSE_PENDING;
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData      = &Dcm_Nrc78Channel[0];
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResDataLen    = 3u;
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen = 3u;
                  SchM_Exit_Dcm(Dcm_MsgCtrl);
                  /****@req DCM-FUNR-006[DCM114]****/
                  DslInternal_ProcessingDone(ProtocolCtrlId);
              }
              else
              {
                  /*Other Nrc code*/
                  /****@req DCM-FUNR-095[DCM038]****/
                  TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
                  Offset =(Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

                  SchM_Enter_Dcm(Dcm_Channel);
                  Dcm_Channel[Offset]   = 0x7F;
                  Dcm_Channel[Offset+1] = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
                  Dcm_Channel[Offset+2] = Dcm_MsgCtrl[MsgCtrlIndexx].NRC;
                  SchM_Exit_Dcm(Dcm_Channel);
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pResData      = &Dcm_Channel[Offset];
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResDataLen    = 3u;
                  Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen = 3u;
                  SchM_Exit_Dcm(Dcm_MsgCtrl);
                  /****@req DCM-FUNR-006[DCM114]****/
                  DslInternal_ProcessingDone(ProtocolCtrlId);
              }
              break;

        case  DCM_POS_RSP_SUPPRESS: /*---------------positive response suppress---------------*/
              /****@req DCM-FUNR-035[DCM141]****/
              /****@req DCM-FUNR-090[DCM238]****/
              DspInternal_DcmConfirmation(ProtocolCtrlId);
              break;
        default:
        	  (void)DslInternal_ResetResource(ProtocolCtrlId);
              break;
    }
    return;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Notify the DSD layer, respond to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DsdInternal_TxConfirmation(uint8  ProtocolCtrlId)
{
    /****@req DCM-FUNR-089[DCM236]****/
    DspInternal_DcmConfirmation(ProtocolCtrlId);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"










 
 
