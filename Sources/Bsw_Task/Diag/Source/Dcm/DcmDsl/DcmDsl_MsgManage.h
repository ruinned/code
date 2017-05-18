/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl_MsgManage.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_MSGMANAGE_H
#define DCMDSL_MSGMANAGE_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCMDSL_MSGMANAGE_H_VENDOR_ID  0
#define DCMDSL_MSGMANAGE_H_MODULE_ID  0
#define DCMDSL_MSGMANAGE_H_AR_MAJOR_VERSION  3
#define DCMDSL_MSGMANAGE_H_AR_MINOR_VERSION  3
#define DCMDSL_MSGMANAGE_H_AR_PATCH_VERSION  0
#define DCMDSL_MSGMANAGE_H_SW_MAJOR_VERSION  1
#define DCMDSL_MSGMANAGE_H_SW_MINOR_VERSION  0
#define DCMDSL_MSGMANAGE_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Types.h"
#include "Std_ExtendedTypes.h"

/****************************** definitions *********************************/
#define  DCM_MSG_NUM    DCM_DSLPROTOCOLROW_NUM_MAX /*number of Message control block*/

/**************************************************************************
 **************Data transceiver sub-function data structure***************
 *************************************************************************/
/******************************************
 ***P2Timer runtime type / control type****
*******************************************/
typedef enum
{
   DCM_P2TIMER_ON  = 0,
   DCM_P2TIMER_OFF = 1
}Dcm_P2StateType;

typedef  struct
{
	TickType    Dcm_P2CurTicks;
	TickType    Dcm_P2ExpiredTicks;
    uint8     	PendingNum;
    Dcm_P2StateType  Dcm_P2State;
}Dcm_P2CtrlType;

/********************************************
 Channel operation status type / control type
 *******************************************/
typedef enum
{
   DCM_CH_IDLE = 0,       /*Channel "idle" state*/
   DCM_CH_OCCUPIED = 1    /*Channel "occupied" state*/
}Dcm_ChannelStateType;

typedef struct
{
    uint8    Dcm_ChannelCfgIndex;    		 /*Static configuration channel index number*/
    Dcm_ChannelStateType Dcm_ChannelRxState; /*Static configuration channel receiving state*/
    Dcm_ChannelStateType Dcm_ChannelTxState; /*Static configuration channel transmission status*/
}Dcm_ChannelCtrlType;

/************************************
 Message Run-time data structures
 ************************************/
typedef  uint8   Dcm_MsgItemType;
typedef  P2VAR(Dcm_MsgItemType, AUTOMATIC, AUTOMATIC) Dcm_MsgType;
typedef  uint32  Dcm_MsgLenType;
typedef  uint8   Dcm_IdContextType;

typedef  struct
{
    Dcm_DslProtocolRxAddrType ReqType; /*=FALSE:physical Addressing,=TRUE:Functional Addressing*/
    boolean    SuppressPosResponse;    /*:=FALSE:Allow positive response;=TRUE:Suppress positive response*/
    boolean    CancelOperation;        /*=FALSE:Not cancel Pending, =TRUE:Cancel Pending*/
}Dcm_MsgAddInfoType;

typedef  struct
{
    Dcm_MsgType          pReqData;     /*Point to request data*/
    Dcm_MsgLenType       ReqDataLen;   /*Request data length*/
    Dcm_MsgType          pResData;     /*Point to the response data (including the SID data)*/
    Dcm_MsgLenType       ResDataLen;   /*Response packet data length(including the SID data)*/
    Dcm_MsgAddInfoType   MsgAddInfo;   /*Additional information for service requests and responses*/
    Dcm_MsgLenType       ResMaxDataLen;/*The maximum number of bytes of response data*/
    Dcm_IdContextType    IdContext;
    PduIdType            DcmRxPduId;   /*Request message identifier*/
}Dcm_MsgContextType;

typedef enum
{
    DCM_MSG_WAIT         = 0,
    DCM_MSG_RECEIVED     = 1,
    DCM_MSG_PROCESSED    = 2,
    DCM_MSG_TRANSMISSION = 3,
    DCM_MSG_CONFIRMATION = 4
}Dcm_MsgStateType;

typedef enum
{
    DCM_POS_RSP = 0, 		  /*Positive response*/
    DCM_NEG_RSP = 1, 		  /*Negative response*/
    DCM_POS_RSP_SUPPRESS = 2  /*Positive response Suppress*/
}Dcm_RspType;

typedef struct
{
    uint8   SID;         			/*Service ID*/
    uint8   Subfunction;      /*Service Subfunction */
    uint8   NRC;         			/*Negative response code, default = 0xFF*/
    boolean SendFlag;    			/*=TRUE: being sent;=FALSE: not sent*/
    PduIdType   DcmTxPduId;
    uint8   Dcm_RxCtrlChannelIndex; /*Message receiver channel control block index number*/
    uint8   Dcm_TxCtrlChannelIndex; /*Message Transmit channel control block index number*/
    Dcm_RspType RspStyle;           /*Type of response*/
    Dcm_MsgContextType  MsgContext; /*Request/response message content*/
    Dcm_P2CtrlType      Dcm_P2Ctrl; /*P2Timer control block*/
    Dcm_MsgStateType    Dcm_MsgState;/*Message status*/
}Dcm_MsgCtrlType;

/********************************************
      function addressing buffer
 ********************************************/
typedef  struct
{
    uint8   Length;
    uint8   Buffer[8];
}Dcm_FunctionMessageType;

/********************************************************
*********************Resource statement*****************
********************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
extern   VAR(Dcm_ChannelCtrlType,DCM_VAR_NOINIT) Dcm_ChannelCtrl[DCM_CHANNEL_NUM]; /*Channel control block*/
extern   VAR(Dcm_MsgCtrlType,DCM_VAR_NOINIT) Dcm_MsgCtrl[DCM_MSG_NUM];         	   /*Message control block*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

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
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitChannelCtrl(uint8 ChannelCtrlId);
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
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_InitMsgCtrl(uint8  MsgCtrlId);
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
extern 	 FUNC(void,DCM_CODE)DslInternal_InitMkCtrl(void);
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
extern   FUNC(Std_ReturnType,DCM_CODE)DslInternal_ResetResource(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

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
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_ProcessingDone(uint8  ProtocolCtrlId);
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
extern   FUNC(void,DCM_CODE)DslInternal_RxIndication(uint8  ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* DCMDSL_MSGMANAGE_H_ */
