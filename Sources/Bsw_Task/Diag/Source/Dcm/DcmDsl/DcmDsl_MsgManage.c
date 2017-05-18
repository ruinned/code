/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_MsgManage.c>
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
#define DCMDSL_MSGMANAGE_C_VENDOR_ID  0
#define DCMDSL_MSGMANAGE_C_MODULE_ID  0
#define DCMDSL_MSGMANAGE_C_AR_MAJOR_VERSION  3
#define DCMDSL_MSGMANAGE_C_AR_MINOR_VERSION  3
#define DCMDSL_MSGMANAGE_C_AR_PATCH_VERSION  0
#define DCMDSL_MSGMANAGE_C_SW_MAJOR_VERSION  1
#define DCMDSL_MSGMANAGE_C_SW_MINOR_VERSION  0
#define DCMDSL_MSGMANAGE_C_SW_PATCH_VERSION  0
#define DCMDSL_MSGMANAGE_C_VENDOR_API_INFIX  0

/*=======[I N C L U D E S]===================================================*/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSL_MSGMANAGE_C_AR_MAJOR_VERSION != DCMDSL_MSGMANAGE_H_AR_MAJOR_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_MSGMANAGE_C_AR_MINOR_VERSION != DCMDSL_MSGMANAGE_H_AR_MINOR_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_MSGMANAGE_C_AR_PATCH_VERSION != DCMDSL_MSGMANAGE_H_AR_PATCH_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_MSGMANAGE_C_SW_MAJOR_VERSION != DCMDSL_MSGMANAGE_H_SW_MAJOR_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_MSGMANAGE_C_SW_MINOR_VERSION != DCMDSL_MSGMANAGE_H_SW_MINOR_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSL_MSGMANAGE_C_SW_PATCH_VERSION != DCMDSL_MSGMANAGE_H_SW_PATCH_VERSION)
  #error "DcmDsl_MsgManage.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE) DslInternal_SearchProtocolCfgIndex (PduIdType DcmPduId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ProtocolCfgId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ConnectionCfgId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ProtocolRxCfgId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType,DCM_CODE)  DslInternal_SearchChannelCfgIndex (PduIdType DcmPduId,
																		 P2VAR(uint8,AUTOMATIC,DCM_VAR)RxChannelCfgId,
																		 P2VAR(uint8,AUTOMATIC,DCM_VAR)TxChannelCfgId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideRxBuffer_CheckInputParameter(PduIdType DcmRxPduId,
																			  PduLengthType TpSduLength,
																			  P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
																			  P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_CheckInputParameter(PduIdType DcmRxPduId,
																	      P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_AnalyseResultRequest(NotifResultType Result);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_IsFuncAddress(PduIdType DcmRxPduId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideTxBuffe_CheckInputParameter( PduIdType DcmTxPduId,
																			  P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
																			  PduLengthType Length,
																			  P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


/******************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideTxBuffe_CheckCtrlStatus(PduIdType DcmTxPduId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************************************
 *******************Resource definition******************
********************************************************/
/*Static channel allocation of resources*/
#define  DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
VAR(uint8,DCM_VAR_POWER_ON_INIT)Dcm_Channel[DCM_CHANNEL_LENGTH] = {0u};
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(Dcm_ChannelCtrlType,DCM_VAR_NOINIT)Dcm_ChannelCtrl[DCM_CHANNEL_NUM];/*Channel control block*/
VAR(Dcm_MsgCtrlType,DCM_VAR_NOINIT) Dcm_MsgCtrl[DCM_MSG_NUM]; 			/*Message control block*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

#if(STD_ON == DCM_UDS_FUNC_ENABLED)
#define  DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
STATIC   VAR(Dcm_FunctionMessageType,DCM_VAR_POWER_ON_INIT)Dcm_FunctionalMessage; /*Function addressing packets resources*/
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
#endif
/****************************** implementations ********************************/
/*************************************************************************/
/*
 * Brief               <Initializing the corresponding channel control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ChannelCtrlId:channel control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitChannelCtrl(uint8 ChannelCtrlId)
{
    if(ChannelCtrlId>=DCM_CHANNEL_NUM)
    {
        return E_NOT_OK;
    }
    SchM_Enter_Dcm(Dcm_ChannelCtrl);
    Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelCfgIndex = DCM_INVALID_UINT8;
    Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelRxState  = DCM_CH_IDLE;
    Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelTxState  = DCM_CH_IDLE;
    SchM_Exit_Dcm(Dcm_ChannelCtrl);
    return E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <initialization message control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <MsgCtrlId:message control block ID Index>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitMsgCtrl(uint8  MsgCtrlId)
{

    if(MsgCtrlId >= DCM_MSG_NUM)
    {
        return E_NOT_OK;
    }
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    Dcm_MsgCtrl[MsgCtrlId].SID  = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].Subfunction = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].NRC  = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].SendFlag = FALSE;
    Dcm_MsgCtrl[MsgCtrlId].RspStyle     = DCM_POS_RSP;
    Dcm_MsgCtrl[MsgCtrlId].DcmTxPduId   = DCM_INVALID_PDUID;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex   = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex   = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId    = DCM_INVALID_PDUID;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData      = NULL_PTR;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen    = DCM_INVALID_UINT32;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = NULL_PTR;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = DCM_INVALID_UINT32;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.MsgAddInfo.ReqType = DCM_PHYSICAL;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.MsgAddInfo.SuppressPosResponse = FALSE;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.MsgAddInfo.CancelOperation     = FALSE;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = DCM_INVALID_UINT32;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.IdContext     = DCM_INVALID_UINT8;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2CurTicks     = DCM_INVALID_UINT32;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2ExpiredTicks = DCM_INVALID_UINT32;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.PendingNum    = 0u;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2State   = DCM_P2TIMER_OFF;
    Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_WAIT;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <initialization module control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous/Asynchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <description...>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DslInternal_InitMkCtrl(void)
{
    SchM_Enter_Dcm(Dcm_MkCtrl);
    Dcm_MkCtrl.Dcm_ActiveSes 		= DCM_DEFAULT_SESSION;
    Dcm_MkCtrl.Dcm_ActiveSec 		= DCM_SEC_LEV_LOCKED;
    Dcm_MkCtrl.Dcm_ActiveProtocol 	= DCM_NO_PROTOCOL;
    Dcm_MkCtrl.Dcm_MkState   		= DCM_ON;
    SchM_Exit_Dcm(Dcm_MkCtrl);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Reset protocol control block members MsgCtrlIndex +
 *                      Reset corresponding message control block +
 *                      Reset corresponding receive / transmit channel control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:protocol control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)DslInternal_ResetResource(uint8  ProtocolCtrlId)
{
   uint8    MsgCtrlIndexx;
   uint8    RxChannelCtrlIndex;
   uint8    TxChannelCtrlIndex;
   uint8    Index;
   boolean  Flag;

   /**********************************************************/
   /*ComM interactive processing*/
   /****@req DCM-FUNR-059[DCM170]****/
   if(DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes)
   {
       /****@req DCM-FUNR-053[DCM164]****/
       /****@req DCM-FUNR-055[DCM166]****/
       Flag = FALSE;
       for(Index=0;(Index<DCM_MSG_NUM)&&(FALSE==Flag);Index++)
       {
          if((ProtocolCtrlId != Index)
              &&(DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState))
          {
              Flag = TRUE;
          }
       }
       if(FALSE == Flag)
       {
           /*Notice ComM exit "FULL COmmunication"*/
           ComM_DCM_InactiveDiagnostic(DCM_COMM_CHANNEL_NO);
       }
   }
   /*****************S3Serer timer****************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
   /****@req DCM-FUNR-035[DCM141]****/
   if(DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
   {
       Flag = FALSE;
       for(Index=0;(Index<DCM_MSG_NUM)&&(FALSE==Flag);Index++)
       {
          if((ProtocolCtrlId != Index)
              &&(DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState))
          {
              Flag = TRUE;
          }
       }
       if(FALSE == Flag)
       {
           /*restart s3timer */
          DslInternal_S3ServerStart();
       }
   }
#endif
   /*****************Reset Message*****************/
   SchM_Enter_Dcm(Dcm_ProtocolCtrl);
   SchM_Enter_Dcm(Dcm_MsgCtrl);

   MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
   RxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_RxCtrlChannelIndex;
   TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex;
   Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex = DCM_INVALID_UINT8;

   SchM_Exit_Dcm(Dcm_MsgCtrl);
   SchM_Exit_Dcm(Dcm_ProtocolCtrl);

   (void)DslInternal_InitMsgCtrl(MsgCtrlIndexx);
   /****@req DCM-FUNR-003[DCM241]****/
   (void)DslInternal_InitChannelCtrl(RxChannelCtrlIndex);
   (void)DslInternal_InitChannelCtrl(TxChannelCtrlIndex);
   return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <Find DcmPduId correspondence in which an Protocol, which a connection, and which an  receive configuration>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmPduId: Receive/transmit PduID>
 * Param-Name[out]     <(*ProtocolCfgId):   DcmPduId in the corresponding protocol configuration table index number.
 *                      (*ConnectionCfgId): DcmPduId in the corresponding protocol configuration table,specific connection index number
 *                      (*ProtocolRxCfgId): DcmPduId in the corresponding protocol configuration table, the specific connection index,
 *                                          receiver configuration index number>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE) DslInternal_SearchProtocolCfgIndex (PduIdType DcmPduId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ProtocolCfgId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ConnectionCfgId,
																		  P2VAR(uint8,AUTOMATIC,DCM_VAR)ProtocolRxCfgId)
{
    uint8  protocolindex;
    uint8  connectindex;
    uint8  rxpduidindex;
    uint8  protocolrownum;
    uint8  connectionnum;
    uint8  rxpduidnum;
    PduIdType  rxpduid;
    PduIdType  txpduid;
    P2CONST(Dcm_DslProtocolRowType,AUTOMATIC,DCM_CONST)pDslProtocolRow;
    P2CONST(Dcm_DslConnectionType,AUTOMATIC,DCM_CONST)pDslConnection;
    P2CONST(Dcm_DslProtocolRxType,AUTOMATIC,DCM_CONST)pDslProtocolRx;

    if(  (NULL_PTR == ProtocolCfgId)
       ||(NULL_PTR == ConnectionCfgId)
       ||(NULL_PTR == ProtocolRxCfgId))
    {
        return E_NOT_OK;
    }
    protocolrownum  = (Dcm_DslCfg.pDcmDslProtocol)->DcmDslProtocolRow_Num; /*Number of protocol configuration*/
    pDslProtocolRow = (Dcm_DslCfg.pDcmDslProtocol)->pDcmDslProtocolRow;    /*protocol configure first address*/
    for(protocolindex=0;protocolindex<protocolrownum;protocolindex++)
    {
    	/*in a protocolRow Container*/
        connectionnum =  pDslProtocolRow[protocolindex].DcmDslConnection_Num;/*Number of connections containers in a ProtocolRow container*/
        pDslConnection=  pDslProtocolRow[protocolindex].pDcmDslConnection;   /*in a protocolRow Container,connection Container First address*/
        for(connectindex=0;connectindex<connectionnum;connectindex++)
        {
        	/*in a connection Container*/
            if(NULL_PTR != pDslConnection[connectindex].pDcmDslMainConnection)
            {
                rxpduidnum = pDslConnection[connectindex].pDcmDslMainConnection->DcmDslProtocolRx_Num; /*Number of ProtocolRx containers,in a connection container*/
                pDslProtocolRx = pDslConnection[connectindex].pDcmDslMainConnection->pDcmDslProtocolRx;
                for(rxpduidindex=0;rxpduidindex<rxpduidnum;rxpduidindex++)
                {
                	/*in Single ProtocolRx Container*/
                    rxpduid = pDslProtocolRx[rxpduidindex].DcmDslProtocolRxPduId;
                    if(DcmPduId == rxpduid)
                    {
                        (*ProtocolCfgId)   = protocolindex;
                        (*ConnectionCfgId) = connectindex;
                        (*ProtocolRxCfgId) = rxpduidindex;
                        return  E_OK;
                    }
                }
                txpduid = pDslConnection[connectindex].pDcmDslMainConnection->DcmDslProtocolTxPduId;
                if(DcmPduId == txpduid)
                {
                    (*ProtocolCfgId)   = protocolindex;
                    (*ConnectionCfgId) = connectindex;
                    (*ProtocolRxCfgId) = 0u;
                    return E_OK;
                }
            }
        }
    }
    return  E_NOT_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <Find DcmPduId corresponding receive / transmit channel configuration index number>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmPduId: Receive/transmit PduID.>
 * Param-Name[out]     <(*RxChannelCtrlId):DcmPduId corresponding receiving channel index number.
 *                      (*TxChannelCtrlId):DcmPduId corresponding transmit channel index number>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType,DCM_CODE)  DslInternal_SearchChannelCfgIndex (PduIdType DcmPduId,
																		 P2VAR(uint8,AUTOMATIC,DCM_VAR)RxChannelCfgId,
																		 P2VAR(uint8,AUTOMATIC,DCM_VAR)TxChannelCfgId)
{
    uint8  ProtocolCfgId;
    uint8  ConnectionCfgId;
    uint8  ProtocolRxCfgId;
    uint8  RxBufferId;
    uint8  TxBufferId;
    uint8  ChannelIndex;
    uint8  ChannelNum;
    boolean Flag;
    Std_ReturnType  ret;

    if(  (NULL_PTR == RxChannelCfgId)
       ||(NULL_PTR == TxChannelCfgId))
    {
        return  E_NOT_OK;
    }
    ret = DslInternal_SearchProtocolCfgIndex(DcmPduId,
                                             &ProtocolCfgId,
                                             &ConnectionCfgId,
                                             &ProtocolRxCfgId);
    if( E_NOT_OK == ret)
    {
       return E_NOT_OK;
    }
    RxBufferId =((Dcm_DslCfg.pDcmDslProtocol)->pDcmDslProtocolRow)[ProtocolCfgId].DcmDslProtocolRxBufferID;/*DcmPduId corresponding protocol receives bufferID*/
    TxBufferId =((Dcm_DslCfg.pDcmDslProtocol)->pDcmDslProtocolRow)[ProtocolCfgId].DcmDslProtocolTxBufferID;/*DcmPduId corresponding protocol Transmits bufferID*/
    ChannelNum = Dcm_DslCfg.DcmChannelCfg_Num;  /*Configure the channel number*/
    /***************/
    Flag = FALSE;
    for(ChannelIndex=0;(ChannelIndex<ChannelNum)&&(FALSE==Flag);ChannelIndex++)
    {
        if(RxBufferId == (Dcm_DslCfg.pDcmChannelCfg[ChannelIndex].Dcm_DslBufferId))
        {
           (*RxChannelCfgId) = ChannelIndex;
           Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
        /*In Dslbuffer container configuration, can not find the corresponding RxBufferId*/
        return  E_NOT_OK;
    }
    /***************/
    Flag = FALSE;
    for(ChannelIndex=0;(ChannelIndex<ChannelNum)&&(FALSE==Flag);ChannelIndex++)
    {
        if(TxBufferId == (Dcm_DslCfg.pDcmChannelCfg[ChannelIndex].Dcm_DslBufferId))
        {
           (*TxChannelCfgId) = ChannelIndex;
           Flag = TRUE;
        }
    }
    if(FALSE == Flag)
    {
    	/*In Dslbuffer container configuration, can not find the corresponding TxBufferId*/
        return  E_NOT_OK;
    }
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideRxBuffer_CheckInputParameter(PduIdType DcmRxPduId,
																			  PduLengthType TpSduLength,
																			  P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
																			  P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolRxCfgIndex;
    uint8  RxChannelCfgIndex;
    uint8  TxChannelCfgIndex;
    Std_ReturnType   ret;

    /************************************************/
    /*check whether Dcm is initialized*/
    if(DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
    	/****@req DCM-FUNR-243[DCM042]****/
    	/****@req DCM-FUNR-244[DCM043]****/
    	/****@req DCM-FUNR-245[DCM048]****/
    	/****@req DCM-FUNR-246[DCM049]****/
        (*pDetErrorId) = DCM_E_UNINIT;
        return(E_NOT_OK);
    }
    /************************************************/
    /*Entry parameter validity checks*/
    if(  (NULL_PTR == PduInfoPtr)
       ||(NULL_PTR == (*PduInfoPtr))
       ||(0u == TpSduLength)  )
    {
       (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    /************************************************/
    /*find DcmRxPduId corresponding Protocol index number,in the protocol configuration table */
    ret = DslInternal_SearchProtocolCfgIndex(DcmRxPduId,
                                             &ProtocolCfgIndex,
                                             &ConnectionCfgIndex,
                                             &ProtocolRxCfgIndex);
    if( E_NOT_OK == ret )
    {
        (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    /************************************************/
    /*Find DcmRxPduId corresponding protocol reference receive/transmit channel, in DcmDslBuffer container index number*/
    ret = DslInternal_SearchChannelCfgIndex(DcmRxPduId,
                                            &RxChannelCfgIndex,
                                            &TxChannelCfgIndex);
    if( E_NOT_OK == ret)
    {
        (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    return(E_OK);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This is called by the PduR to indicate the competion of a reception>
 * ServiceId           <0x02>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 *                      TpSduLength: This length identifies the overall number of bytes to be received.>
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req Dcm_APIR_011[DCM094]****/
/****@req DCM-APIR-013[DCM342]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(BufReq_ReturnType,DCM_CODE)Dcm_ProvideRxBuffer(PduIdType DcmRxPduId,
													PduLengthType TpSduLength,
													P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolRxCfgIndex;
    uint8  RxChannelCfgIndex;
    uint8  TxChannelCfgIndex;
    uint8  ProtocolCtrlIndex;
    uint8  MsgCtrlIndexx;
    uint8  RxChannelCtrlIndex;
    uint8  TxChannelCtrlIndex;
    uint16 len;
    uint16 Offset;
    Dcm_MsgStateType  MsgState;
    Dcm_ChannelStateType  ChannelRxState;
    Dcm_ChannelStateType  ChannelTxState;
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
    Dcm_ProtocolType  ProtocolRowID;
    Dcm_DslProtocolRxAddrType DslProtocolRxAddrType;
    P2CONST(Dcm_DslConnectionType,AUTOMATIC,DCM_CONST)pDslConnection;
#endif
	uint8  DetErrorId;
    Std_ReturnType ret;

    /************************************************/
    ret = Dcm_ProvideRxBuffer_CheckInputParameter( DcmRxPduId,
												   TpSduLength,
												   PduInfoPtr,
												   &DetErrorId );
    if(E_NOT_OK == ret)
    {
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_PROVIDERXBUFFER_ID,
                        DetErrorId);
#endif
        return  BUFREQ_E_NOT_OK;
    }
    /************************************************/
    /*find DcmRxPduId corresponding Protocol index number,in the protocol configuration table */
    (void)DslInternal_SearchProtocolCfgIndex( DcmRxPduId,
											 &ProtocolCfgIndex,
											 &ConnectionCfgIndex,
											 &ProtocolRxCfgIndex );

    ProtocolCtrlIndex  = ProtocolCfgIndex;
    MsgCtrlIndexx      = ProtocolCfgIndex;

    /************************************************/
    /*Find DcmRxPduId corresponding protocol reference receive/transmit channel, in DcmDslBuffer container index number*/
    (void)DslInternal_SearchChannelCfgIndex( DcmRxPduId,
										     &RxChannelCfgIndex,
                                             &TxChannelCfgIndex);

    RxChannelCtrlIndex = RxChannelCfgIndex;
    TxChannelCtrlIndex = TxChannelCfgIndex;

    /*************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    /****@req DCM-FUNR-035[DCM141]****/
    /*Stop S3Server timer*/
    if(DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
    {
        DslInternal_S3ServerStop();
    }
#endif

    /************************************************/
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
    pDslConnection  = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].pDcmDslConnection;
    DslProtocolRxAddrType = (pDslConnection[ConnectionCfgIndex].pDcmDslMainConnection->pDcmDslProtocolRx)[ProtocolRxCfgIndex].DcmDslProtocolRxAddrType;
    ProtocolRowID = ((Dcm_DslCfg.pDcmDslProtocol)->pDcmDslProtocolRow)[ProtocolCfgIndex].DcmDslProtocolID;
    
    if(  (DCM_UDS_ON_CAN == ProtocolRowID)
       &&(DCM_FUNCTIONAL == DslProtocolRxAddrType) )
    {
       /*The UDS protocol functions addressing message, bypass treatment*/
       (*PduInfoPtr)->SduDataPtr = &(Dcm_FunctionalMessage.Buffer[0]);
       (*PduInfoPtr)->SduLength  = 8u;       
       Dcm_FunctionalMessage.Length = TpSduLength;/*save request length*/
       return BUFREQ_OK;
    }
#endif

    /************************************************/
    /*Check RxPduId referenced message control block status is "DCM_MSG_WAIT"*/
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    MsgState = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState;
    if(DCM_MSG_WAIT != MsgState)
    {
        /****@req DCM-APIR-015[DCM445]****/
        SchM_Exit_Dcm(Dcm_MsgCtrl);
        return  BUFREQ_E_BUSY;
    }
    SchM_Exit_Dcm(Dcm_MsgCtrl);

    /************************************************/
    /*Check RxPduId referenced message control block status is "DCM_MSG_WAIT"*/
    SchM_Enter_Dcm(Dcm_ChannelCtrl);
    ChannelRxState = Dcm_ChannelCtrl[RxChannelCtrlIndex].Dcm_ChannelRxState;
    ChannelTxState = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelTxState;
    if(  (DCM_CH_OCCUPIED == ChannelRxState)
       ||(DCM_CH_OCCUPIED == ChannelTxState))
    {
        SchM_Exit_Dcm(Dcm_ChannelCtrl);
        return  BUFREQ_E_BUSY;
    }
    SchM_Exit_Dcm(Dcm_ChannelCtrl);                    

    /************************************************/
    /*Compare application buffer length and configuration of the channel length*/
    len = (Dcm_DslCfg.pDcmChannelCfg)[RxChannelCfgIndex].Dcm_DslBufferSize;
    if(len < TpSduLength)
    {
        /****@req DCM-APIR-014[DCM444]****/
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_PROVIDERXBUFFER_ID,
                        DCM_E_INTERFACE_BUFFER_OVERFLOW);
#endif
        return BUFREQ_E_OVFL;/*before modifying: BUFREQ_E_OVEL*/
    }
    /****@req DCM-APIR-012[DCM443]****/
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[RxChannelCfgIndex].offset;
    (*PduInfoPtr)->SduDataPtr = &Dcm_Channel[Offset];
    (*PduInfoPtr)->SduLength  = len;
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen = TpSduLength;
    Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData   = &Dcm_Channel[Offset];
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    return  BUFREQ_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This is called by Dcm_RxIndicaiton() to Check some configration parameter>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:ID of DCM I-PDU that has been received>
 * Param-Name[out]     <E_OK/E_NOT_OK>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_CheckInputParameter(PduIdType DcmRxPduId,
																	      P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolRxCfgIndex;
    uint8  RxChannelCfgIndex;
    uint8  TxChannelCfgIndex;
	Std_ReturnType  ret;

    /*************************************************/
	/*To determine whether the DCM module initialization*/
    if(DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        /****@req DCM-FUNR-243[DCM042]****/
        /****@req DCM-FUNR-244[DCM043]****/
        /****@req DCM-FUNR-245[DCM048]****/
        /****@req DCM-FUNR-246[DCM049]****/
        (*pDetErrorId) = DCM_E_UNINIT;
        return(E_NOT_OK);
    }
    /*************************************************/
    /*To find DcmRxPduId corresponding protocol index number, in the protocol configuration table */
    ret = DslInternal_SearchProtocolCfgIndex( DcmRxPduId,
                                             &ProtocolCfgIndex,
                                             &ConnectionCfgIndex,
                                             &ProtocolRxCfgIndex);
    if(E_NOT_OK == ret)
    {
    	(*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    /************************************************/
    /*Find out the the configuration channel number of the agreement
     *in the index number of the container Dslbuffer*/
    ret = DslInternal_SearchChannelCfgIndex(DcmRxPduId,
                                            &RxChannelCfgIndex,
                                            &TxChannelCfgIndex);
    if( E_NOT_OK == ret)
    {
        (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    return(E_OK);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*********************************************************
 ********************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_AnalyseResultRequest(NotifResultType Result)
{
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8    Index;
    boolean  Flag = FALSE;
#endif
	Std_ReturnType  ret;

	if(NTFRSLT_OK == Result)
	{
		ret = E_OK;
	}
	else
	{
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
        if(DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
        {
            for(Index=0;(Index<DCM_MSG_NUM)&&(FALSE==Flag);Index++)
            {
               if(DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState)
               {
                   Flag = TRUE;
               }
            }
            if(FALSE == Flag)
            {
               DslInternal_S3ServerStart();
            }
        }
#endif
        ret = E_NOT_OK;
	}
	return(ret);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*********************************************************
 ********************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC  FUNC(Std_ReturnType,DCM_CODE)Dcm_RxIndication_IsFuncAddress(PduIdType DcmRxPduId)
{
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
    uint8  Sid;
    uint8  Index;
    uint8  SubFunction;
#endif
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8  Flag;
#endif
    uint16 Offset;
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolRxCfgIndex;
    uint8  RxChannelCfgIndex;
    uint8  TxChannelCfgIndex;
    uint8  MsgCtrlIndexx;
    uint8  RxChannelCtrlIndex;
    uint8  TxChannelCtrlIndex;
    Dcm_DslProtocolRxAddrType DslProtocolRxAddrType;
    P2CONST(Dcm_DslConnectionType,AUTOMATIC,DCM_CONST)pDslConnection;

    /*****************************************************/
    (void)DslInternal_SearchProtocolCfgIndex( DcmRxPduId,
											 &ProtocolCfgIndex,
											 &ConnectionCfgIndex,
											 &ProtocolRxCfgIndex );
    /*****************************************************/
    (void)DslInternal_SearchChannelCfgIndex( DcmRxPduId,
											&RxChannelCfgIndex,
											&TxChannelCfgIndex );

    MsgCtrlIndexx = ProtocolCfgIndex;
    RxChannelCtrlIndex = RxChannelCfgIndex;
    TxChannelCtrlIndex = TxChannelCfgIndex;

    /*****************************************************/
    pDslConnection = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].pDcmDslConnection;
    DslProtocolRxAddrType = (pDslConnection[ConnectionCfgIndex].pDcmDslMainConnection->pDcmDslProtocolRx)[ProtocolRxCfgIndex].DcmDslProtocolRxAddrType;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[RxChannelCfgIndex].offset;

    if(DCM_FUNCTIONAL == DslProtocolRxAddrType)
    {
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
    	/*"TesterPresent Request "*/
    	/****@req DCM-FUNR-004[DCM112]****/
    	/****@req DCM-FUNR-005[DCM113]****/
    	Sid = Dcm_FunctionalMessage.Buffer[0];
    	SubFunction = Dcm_FunctionalMessage.Buffer[1];
    	if( (0x3E == Sid) &&(0x80==SubFunction) &&(0x02 == Dcm_FunctionalMessage.Length))
    	{
    		/*SID=0x3E + subfunction(0x80)*/
		#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    		if(DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
    		{
    			Flag = FALSE;
    			for(Index=0;(Index<DCM_MSG_NUM)&&(FALSE==Flag);Index++)
    			{
    				if(DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState)
    				{
    					Flag = TRUE;
    				}
    			}
    			if(FALSE == Flag)
    			{
    				/*restart S3timer*/
    				DslInternal_S3ServerStart();
    			}
    		}
		  #endif
    		return(E_NOT_OK);
    	}
#endif
        /*(SID=0x3E + subfunction(0x00)) || (SID != 0x3E),Need to be submitted to Dsd layer handle*/
        if(  (DCM_MSG_WAIT != Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState)
           ||(DCM_CH_OCCUPIED == Dcm_ChannelCtrl[RxChannelCtrlIndex].Dcm_ChannelRxState)
           ||(DCM_CH_OCCUPIED == Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelTxState) )
        {
            /*The same protocol message being processed*/
            return(E_NOT_OK);
        }
        /*The main channel is idle,copy data to main channel*/
        for(Index=0u;Index<(Dcm_FunctionalMessage.Length);Index++)
        {
            Dcm_Channel[Offset + Index] = Dcm_FunctionalMessage.Buffer[Index];
        }
        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen = Dcm_FunctionalMessage.Length;
        Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData   = &Dcm_Channel[Offset];
    }
    return(E_OK);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This is called by the PduR to indicate the competion of a reception>
 * ServiceId           <0x03>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:ID of DCM I-PDU that has been received
 *                      Result: The result of the diagnostic request message received>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after call of DcmProvideRxBuffer()>
 * CallByAPI           <None>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req Dcm_APIR_016[DCM093]****/
/****@req DCM-APIR-018[DCM345]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_RxIndication(PduIdType DcmRxPduId,
		                            NotifResultType Result)
{
    uint16 Offset;
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolRxCfgIndex;
    uint8  RxChannelCfgIndex;
    uint8  TxChannelCfgIndex;
    Dcm_ChannelStateType  TxChannelTxState;
    uint8  ProtocolCtrlIndex;
    uint8  MsgCtrlIndexx;
    uint8  RxChannelCtrlIndex;
    uint8  TxChannelCtrlIndex;
    Dcm_DslProtocolRxAddrType DslProtocolRxAddrType;
    P2CONST(Dcm_DslConnectionType,AUTOMATIC,DCM_CONST)pDslConnection;
    Std_ReturnType  ret;
    uint8  DetErrorId;

    /************************************************/
    ret = Dcm_RxIndication_CheckInputParameter(DcmRxPduId,&DetErrorId);
    if(E_NOT_OK == ret)
    {
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_RXINDICATION_ID,
                        DetErrorId);
#endif
        return;
    }
    /**********************************************/
    /*Receive complete results analysis*/
	/*@req DCM-APIR-017[DCM344]*/
    ret = Dcm_RxIndication_AnalyseResultRequest(Result);
    if(E_NOT_OK == ret)
    {
         return;
    }

    /***********************************************/
    /*functional request packet processing*/
    ret = Dcm_RxIndication_IsFuncAddress(DcmRxPduId);
    if(E_NOT_OK == ret)
    {
        return;
    }
    /************************************************/
    /*To find DcmRxPduId corresponding protocol index number,
     *in the protocol configuration table */
    (void)DslInternal_SearchProtocolCfgIndex( 	DcmRxPduId,
												&ProtocolCfgIndex,
												&ConnectionCfgIndex,
												&ProtocolRxCfgIndex	);

    ProtocolCtrlIndex  	= ProtocolCfgIndex;
    MsgCtrlIndexx       = ProtocolCfgIndex;
    /************************************************/
    /*Find DcmRxPduId corresponding protocol reference receive/transmit channel index number,
     *in DcmDslBuffer container */
    (void)DslInternal_SearchChannelCfgIndex( DcmRxPduId,
											&RxChannelCfgIndex,
											&TxChannelCfgIndex);

    RxChannelCtrlIndex = RxChannelCfgIndex;
    TxChannelCtrlIndex = TxChannelCfgIndex;

    /************************************************/
    pDslConnection = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].pDcmDslConnection;
    DslProtocolRxAddrType = (pDslConnection[ConnectionCfgIndex].pDcmDslMainConnection->pDcmDslProtocolRx)[ProtocolRxCfgIndex].DcmDslProtocolRxAddrType;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[RxChannelCfgIndex].offset;

    /************************************************/
    /* Determine relationship between the static configuration channel and receive/transmit channel control block */
    /****@req DCM-FUNR-003[DCM241]****/
    SchM_Enter_Dcm(Dcm_ChannelCtrl);
    Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex = TxChannelCfgIndex;
    Dcm_ChannelCtrl[RxChannelCtrlIndex].Dcm_ChannelCfgIndex = RxChannelCfgIndex;
    Dcm_ChannelCtrl[RxChannelCtrlIndex].Dcm_ChannelRxState  = DCM_CH_OCCUPIED;
    TxChannelTxState = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelTxState;
    if(DCM_CH_OCCUPIED == TxChannelTxState)
    {
        /*Corresponding transmit channel status is not normal*/
        (void)DslInternal_ResetResource(ProtocolCtrlIndex);
        SchM_Exit_Dcm(Dcm_ChannelCtrl);
        return;
    }
    Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelTxState = DCM_CH_OCCUPIED;
    SchM_Exit_Dcm(Dcm_ChannelCtrl);
    /************************************************/
    /*Determine relationships between the receive/transmit channel control block and the corresponding message control block */
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    /****@req DCM-FUNR-067[DCM198]****/
    Dcm_MsgCtrl[MsgCtrlIndexx].SID = (Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData)[0];
    Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType = DslProtocolRxAddrType;
    Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId  = DcmRxPduId;
    Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData    = &Dcm_Channel[Offset];
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_RxCtrlChannelIndex = RxChannelCtrlIndex;
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_TxCtrlChannelIndex = TxChannelCtrlIndex;
    Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId = pDslConnection[ConnectionCfgIndex].pDcmDslMainConnection->DcmDslProtocolTxPduId;
    Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_MsgState = DCM_MSG_RECEIVED;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    /************************************************/
    /*Determine relationship between the message control block and the corresponding protocol control block */
    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    Dcm_ProtocolCtrl[ProtocolCtrlIndex].ProtocolId  = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].DcmDslProtocolID;
    Dcm_ProtocolCtrl[ProtocolCtrlIndex].ProtocolPri = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].DcmDslProtocolPriority;
    Dcm_ProtocolCtrl[ProtocolCtrlIndex].ProtocolPreemptTime = (Dcm_DslCfg.pDcmDslProtocol->pDcmDslProtocolRow)[ProtocolCfgIndex].DcmDslProtocolPreemptTime;
    Dcm_ProtocolCtrl[ProtocolCtrlIndex].MsgCtrlIndex = MsgCtrlIndexx;
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);
    /*************************************************/
    /*ComM interactive processing*/
    /****@req DCM-FUNR-058[DCM169]****/
    if(DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes)
    {
        /*The notification ComM into the "FULL Communication"*/
        /****@req DCM-FUNR-052[DCM163]****/
        ComM_DCM_ActiveDiagnostic(DCM_COMM_CHANNEL_NO);
    }
    /************************************************/
    /*Start P2Timer Timer*/
    (void)DslInternal_P2ServerStart(ProtocolCtrlIndex);

    /***********************************************/
    /*Notice the application to check for new protocol boot environment*/
    ret = DslInternal_ProtocolStart(ProtocolCtrlIndex);
    if(E_OK != ret)
    {
        /*the application environment does not allow to start The protocol or return E_PENDING*/
        return;
    }
    /***********************************************/
    /* save valid Protocol ID number*/
    SchM_Enter_Dcm(Dcm_MkCtrl);
    Dcm_MkCtrl.Dcm_ActiveProtocol = Dcm_ProtocolCtrl[ProtocolCtrlIndex].ProtocolId;
    SchM_Exit_Dcm(Dcm_MkCtrl);
    /**********************************************/
    /****Notification DSD layer, and reception processing****/
    /****@req DCM-FUNR-002[DCM111]****/
    (void)DsdInternal_RxIndication(ProtocolCtrlIndex);
    return;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideTxBuffe_CheckInputParameter( PduIdType DcmTxPduId,
																			  P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
																			  PduLengthType Length,
																			  P2VAR(uint8,AUTOMATIC,DCM_VAR)pDetErrorId)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolTxPduCfgIndex;
    Std_ReturnType  ret;

    /***********************************/
    /*The DCM module status check*/
    if(DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        /****@req DCM-FUNR-243[DCM042]****/
        /****@req DCM-FUNR-244[DCM043]****/
        /****@req DCM-FUNR-245[DCM048]****/
        /****@req DCM-FUNR-246[DCM049]****/
        (*pDetErrorId) = DCM_E_UNINIT;
        return(E_NOT_OK);
    }

    /***********************************/
    /*Parameter validation*/
    if(  (NULL_PTR == PduInfoPtr)
       ||(NULL_PTR == (*PduInfoPtr)) )
    {
        (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }

    /**********************************/
    /*To find DcmTxPduId corresponding Protocol index number,
     *in the protocol configuration table*/
    ret = DslInternal_SearchProtocolCfgIndex( DcmTxPduId,
                                              &ProtocolCfgIndex,
                                              &ConnectionCfgIndex,
                                              &ProtocolTxPduCfgIndex );
    if(E_NOT_OK == ret)
    {
        (*pDetErrorId) = DCM_E_PARAM;
        return(E_NOT_OK);
    }
    return(E_OK);
}

#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC   FUNC(Std_ReturnType,DCM_CODE)Dcm_ProvideTxBuffe_CheckCtrlStatus(PduIdType DcmTxPduId)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolTxPduCfgIndex;
    uint8  ProtocolCtrlId;
    uint8  MsgCtrlId;
    uint8  RxChannelCtrlId;
    uint8  TxChannelCtrlId;

    /*********************************************************/
    /*To find DcmTxPduId corresponding Protocol index number,
     *in the protocol configuration table*/
    (void)DslInternal_SearchProtocolCfgIndex( DcmTxPduId,
											 &ProtocolCfgIndex,
                                             &ConnectionCfgIndex,
                                             &ProtocolTxPduCfgIndex );

    ProtocolCtrlId  = ProtocolCfgIndex;
    MsgCtrlId       = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if(DCM_INVALID_UINT8 == MsgCtrlId)
    {
        return(E_NOT_OK);
    }
    RxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex;
    TxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;

    /***********************************************************/
    if(DCM_MSG_TRANSMISSION != Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
    {
        /*The state machine does not operate properly, return BUFREQ_E_NOT_OK*/
        return(E_NOT_OK);
    }

    if(DCM_CH_OCCUPIED != (Dcm_ChannelCtrl[RxChannelCtrlId].Dcm_ChannelRxState))
    {
    	/*Receiver channel status is not correct*/
        return(E_NOT_OK);
    }
    if(DCM_CH_OCCUPIED != (Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_ChannelTxState))
    {
    	/*The send channel status is incorrect*/
        return(E_NOT_OK);
    }
    return(E_OK);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <By this service the DCM module is requested to provide a buffer containing data to be transmitted via a transport protocol>
 * ServiceId           <0x04>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:Identifies the DCM data to be sent;
 *                      Length: This is the minimum length given in bytes of the buffer requested from the DCM>
 * Param-Name[out]     <PduInfoPtr: Pointer to pointer to PduInfoStructure containing data pointer and length of a transmit buffer>
 * Param-Name[in/out]  <None>
 * Return              <BufReq_ReturnType>
 * PreCondition        <this function must be called after call of Pdur_Transmit()>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req DCM_APIR_019[DCM092]****/
/****@req DCM-APIR-022[DCM348]****/
/****@req DCM-APIR-023[DCM349]****/
/****@req DCM-APIR-024[DCM350]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(BufReq_ReturnType,DCM_CODE)Dcm_ProvideTxBuffer(PduIdType  DcmTxPduId,
													P2VAR(P2VAR(PduInfoType,AUTOMATIC,DCM_VAR),AUTOMATIC,DCM_VAR)PduInfoPtr,
													PduLengthType Length)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolTxPduCfgIndex;
    uint8  ProtocolCtrlId;
    uint8  MsgCtrlId;
    uint8  RxChannelCtrlId;
    uint8  TxChannelCtrlId;
    uint8  TxChannelCfgId;
    uint16 ChannelCfgLength;
    Std_ReturnType  ret;
    uint8  DetErrorId;

    /*******************************************/
    ret = Dcm_ProvideTxBuffe_CheckInputParameter( DcmTxPduId,
												  PduInfoPtr,
												  Length,
												  &DetErrorId);
    if(E_NOT_OK == ret)
    {
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_PROVIDETXBUFFER_ID,
                        DetErrorId );
#endif
        return(BUFREQ_E_NOT_OK);
    }

    /*******************************************/
    ret = Dcm_ProvideTxBuffe_CheckCtrlStatus(DcmTxPduId);
    if(E_NOT_OK == ret)
    {
       return(BUFREQ_E_NOT_OK);
    }

    /*******************************************/
    (void)DslInternal_SearchProtocolCfgIndex( DcmTxPduId,
											 &ProtocolCfgIndex,
											 &ConnectionCfgIndex,
											 &ProtocolTxPduCfgIndex );

    ProtocolCtrlId  = ProtocolCfgIndex;
    MsgCtrlId       = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    RxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex;
    TxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgId  = Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_ChannelCfgIndex;

    /*******************************************/
    ChannelCfgLength = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgId].Dcm_DslBufferSize;
    if(DCM_E_RESPONSE_PENDING == Dcm_MsgCtrl[MsgCtrlId].NRC)
    {
        ChannelCfgLength = 8u;
    }
    if(Length > ChannelCfgLength)
    {
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_PROVIDETXBUFFER_ID,
                        DCM_E_INTERFACE_BUFFER_OVERFLOW);
#endif
        /****@req DCM-APIR-021[DCM347]****/
       return  BUFREQ_E_NOT_OK;
    }
    /****@req DCM-APIR-020[DCM346]****/
    (*PduInfoPtr)->SduLength  = ChannelCfgLength;
    (*PduInfoPtr)->SduDataPtr = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData;
     return BUFREQ_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <This is called by the PduR to confirm a Transmit>
 * ServiceId           <0x05>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 *                      Result: NTFRSLT_OK: the complete N-PDU has been transmitted.
 *                              NTFRSLT_E_CANCELATION_OK: the N-PDU has been successfully cancelled.
 *                              NTFRSLT_E_CANCELATION_NOT_OK: an error occurred when cancelling the N-PDU.
 *                              any other value: an error occurred during transmission>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after call of DcmProvideTxBuffer().>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM-FUNR-088[DCM235]****/
/****@req DCM-FUNR-245[DCM048]****/
/****@req DCM_APIR_025[DCM351]****/
/****@req DCM-APIR-026[DCM352]****/
/****@req DCM-APIR-027[DCM353]****/
/****@req DCM-APIR-028[DCM354]****/

#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_TxConfirmation(PduIdType  DcmTxPduId, NotifResultType  Result)
{
    uint8  ProtocolCfgIndex;
    uint8  ConnectionCfgIndex;
    uint8  ProtocolTxPduCfgIndex;
    uint8  ProtocolCtrlId;
    uint8  MsgCtrlId;
    Std_ReturnType  ret;

    /******************************************************************/
    /*The DCM module status check*/
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl);
    if(DCM_ON != (Dcm_MkCtrl.Dcm_MkState))
    {
        /****@req DCM-FUNR-243[DCM042]****/
        /****@req DCM-FUNR-244[DCM043]****/
        /****@req DCM-FUNR-245[DCM048]****/
        /****@req DCM-FUNR-246[DCM049]****/
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_TXCONFIRMATION_ID,
                        DCM_E_UNINIT);
        SchM_Exit_Dcm(Dcm_MkCtrl);
        return;
    }
    SchM_Exit_Dcm(Dcm_MkCtrl);
#endif

    /*****************************************************************/
    /*find DcmTxPduId corresponding protocol index number,in the protocol configuration table*/
    ret = DslInternal_SearchProtocolCfgIndex(DcmTxPduId,
                                             &ProtocolCfgIndex,
                                             &ConnectionCfgIndex,
                                             &ProtocolTxPduCfgIndex);
    if(E_NOT_OK == ret)
    {
#if(STD_ON == DCM_DEV_ERROR_DETECT)
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_TXCONFIRMATION_ID,
                        DCM_E_PARAM);
#endif
        return;
    }
    ProtocolCtrlId  = ProtocolCfgIndex;
    MsgCtrlId       = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*****************************************************************/
    switch(Result)
    {
       case  NTFRSLT_E_CANCELATION_OK:
             /*Confirmation message shows: Cancel successfully*/
    	     break;
       case  NTFRSLT_E_CANCELATION_NOT_OK:
    	     /****@req DCM-FUNR-045[DCM461]****/
             /*Confirmation message shows: not allowed to cancel, cancel failed*/
    	     break;
       case  NTFRSLT_E_NOT_OK:
    	     /*Confirmation message shows: response failed to send*/
    	     (void)DslInternal_ResetResource(ProtocolCtrlId);
    	     break;
       case  NTFRSLT_OK:
    	     /*Confirmation message shows: the response is sent to notify the DSD layer further confirmation*/
    	     /****@req DCM-FUNR-008[DCM117]****/
    	     /******************************************************************/
    	     /*Set corresponding to the message status = "DCM_MSG_CONFIRMATION*/
    	     SchM_Enter_Dcm(Dcm_MsgCtrl);
    	     Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_CONFIRMATION;
    	     SchM_Exit_Dcm(Dcm_MsgCtrl);
   	         DsdInternal_TxConfirmation(ProtocolCtrlId);
    	     break;
       default:
    	     (void)DslInternal_ResetResource(ProtocolCtrlId);
    	     break;
    }
    return;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*******************internal functions****************************/
/*************************************************************************/
/*
 * Brief               <DSL layer sends the response function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <DsdInternal_ProcessingDone>
 */
/*************************************************************************/
/****@req DCM-FUNR-086[DCM232]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DslInternal_ProcessingDone(uint8  ProtocolCtrlId)
{
    PduInfoType  PduInfo;
    PduIdType    DcmTxPduIdx;
    uint8        MsgCtrlIndexx;
    Std_ReturnType  ret;
    Dcm_CommStateType state;

    SchM_Enter_Dcm(Dcm_ProtocolCtrl);
    SchM_Enter_Dcm(Dcm_MsgCtrl);
    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    DcmTxPduIdx  = Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId;
    SchM_Exit_Dcm(Dcm_MsgCtrl);
    SchM_Exit_Dcm(Dcm_ProtocolCtrl);

    PduInfo.SduDataPtr = NULL_PTR;
    PduInfo.SduLength  = (PduLengthType)(Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ResMaxDataLen);

    /****@req DCM-APIR-034[DCM152]****/
    Dcm_ComM_ReadCurComState(&state);
    if(DCM_COMM_FULL_COMMUNICATION != state)
    {
        /*Network status are not allowed to send*/
        return;
    }
    /****@req DCM-FUNR-007[DCM115]****/
    ret = DslInternal_IsLargerThanP2ServerMin(ProtocolCtrlId);
    if(E_NOT_OK == ret)
    {
        /*Response time <P2ServerMin*/
        return;
    }
    /****@req DCM-FUNR-087[DCM237]****/
    ret = PduR_DcmTransmit(DcmTxPduIdx, &PduInfo);
    if(E_NOT_OK == ret)
    {
        /*Failed to send*/
        /****@req DCM-FUNR-009[DCM118]****/
        (void)DslInternal_ResetResource(ProtocolCtrlId);
        return;
    }
    /*****************************************/
    Dcm_MsgCtrl[MsgCtrlIndexx].SendFlag = TRUE;

    /****Close P2Server Timer****/
    (void)DslInternal_P2ServerStop(ProtocolCtrlId);
    return;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               <>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)DslInternal_RxIndication(uint8  ProtocolCtrlId)
{
    Std_ReturnType  ret;

    /***********************************************/
    /**Notify the application start the Protocol**/
    ret = DslInternal_ProtocolStart(ProtocolCtrlId);
    if(E_OK != ret)
    {
        /*The application environment does not allow or PENDING*/
        return;
    }
    /***********************************************/
    /*Save the valid protocol ID number*/
    SchM_Enter_Dcm(Dcm_MkCtrl);
    Dcm_MkCtrl.Dcm_ActiveProtocol = Dcm_ProtocolCtrl[ProtocolCtrlId].ProtocolId;
    SchM_Exit_Dcm(Dcm_MkCtrl);
    /**********************************/
    /**Notification DSD layer reception processing**/
    /****@req DCM-FUNR-002[DCM111]****/
    (void)DsdInternal_RxIndication(ProtocolCtrlId);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
