/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: XXX    MCU:XXX>
 *
 *  @author     <chenms>
 *  @date       <20-03-2013>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms    Initial version                         */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_C_VENDOR_ID  0
#define DCM_C_MODULE_ID  0
#define DCM_C_AR_MAJOR_VERSION  3
#define DCM_C_AR_MINOR_VERSION  3
#define DCM_C_AR_PATCH_VERSION  0
#define DCM_C_SW_MAJOR_VERSION  1
#define DCM_C_SW_MINOR_VERSION  0
#define DCM_C_SW_PATCH_VERSION  0
#define DCM_C_VENDOR_API_INFIX  0

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Include.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCM_C_AR_MAJOR_VERSION != DCM_H_AR_MAJOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_AR_MINOR_VERSION != DCM_H_AR_MINOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_AR_PATCH_VERSION != DCM_H_AR_PATCH_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_MAJOR_VERSION != DCM_H_SW_MAJOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_MINOR_VERSION != DCM_H_SW_MINOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_PATCH_VERSION != DCM_H_SW_PATCH_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif

/****************************************************
 ****************************************************
****************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(Dcm_MkCtrlType, DCM_VAR_NOINIT) Dcm_MkCtrl;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"


#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_PendingManage(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_P2ServerTimer(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_S3Timer(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_SecTimer(void);
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
VAR(boolean, DCM_VAR_NOINIT) gFactory_ActiveFlag = FALSE;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
#endif

/*******************public functions******************************/
/*************************************************************************/
/*
 * Brief                <initialization of DCM module.>
 * ServiceId            <0x01>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*************************************************************************/
/****@req DCM-APIR-002[DCM037]****/
/****@req DCM-APIR-003[DCM334]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_Init(void)
{
    uint8  Index;

    for(Index=0;Index<DCM_CHANNEL_NUM;Index++)
    {
        (void)DslInternal_InitChannelCtrl(Index);
    }
    for(Index=0;Index<DCM_MSG_NUM;Index++)
    {
    	(void)DslInternal_InitMsgCtrl(Index);
    }
    for(Index=0;Index<DCM_DSLPROTOCOLROW_NUM_MAX;Index++)
    {
    	(void)DslInternal_InitProtocolCtrl(Index);
    }

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    DslInternal_InitSesCtrl();
#endif

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
    DslInternal_InitSecCtrl();
#endif

#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
    Dsp_InitRoutineStates();
#endif
    DslInternal_InitComMCtrl();
    DslInternal_InitMkCtrl();
    
    /******************/
    //(*(uint8*UDS_FAR)FL_APPL_UPDATE) = 0xD5;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"


#if(STD_ON == DCM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to where to store the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req DCM-APIR-004[DCM065]****/
/****@req DCM-APIR-005[DCM335]****/
/****@req DCM-APIR-006[DCM336]****/
/****@req DCM-APIR-007[DCM337]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dcm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR)VersionInfo )
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
   SchM_Enter_Dcm(Dcm_MkCtrl)
   if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
   {
       SchM_Exit_Dcm(Dcm_MkCtrl);
       Det_ReportError(DCM_MODULE_ID,
                       DCM_INSTANCE_ID,
                       DCM_GETVERSIONINFO_ID,
                       DCM_E_UNINIT);
       return;
   }
   SchM_Exit_Dcm(Dcm_MkCtrl);
#endif

#if(STD_ON == DCM_DEV_ERROR_DETECT)
   if(NULL_PTR == VersionInfo)
   {
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETVERSIONINFO_ID,
                        DCM_E_PARAM);
       return;
   }
#endif
   VersionInfo->vendorID = DCM_VENDOR_ID;
   VersionInfo->moduleID = DCM_MODULE_ID;
   VersionInfo->instanceID = DCM_INSTANCE_ID;
   VersionInfo->sw_major_version = DCM_SW_MAJOR_VERSION;
   VersionInfo->sw_minor_version = DCM_SW_MINOR_VERSION;
   VersionInfo->sw_patch_version = DCM_SW_PATCH_VERSION;
 }
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               <This function provides the active security level value.>
 * ServiceId           <0x0d>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SecLevel:Pointer to Active Security Level value >
 * Param-Name[in/out]   <None>
 * Return               <E_OK:
 * 						E_NOT_OK:>
 * PreCondition         <...>
 * CallByAPI            <APIName>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req Dcm-APIR-008[DCM338]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSecurityLevel(P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR)SecLevel )
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETSECURITYLEVEL_ID,
                        DCM_E_UNINIT);
        return  E_NOT_OK;
    }
#endif

#if(STD_ON == DCM_DEV_ERROR_DETECT)
   if( NULL_PTR == SecLevel)
   {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETSECURITYLEVEL_ID,
                        DCM_E_PARAM);
       return E_NOT_OK;
   }
#endif
   (*SecLevel) = Dcm_MkCtrl.Dcm_ActiveSec;
   SchM_Exit_Dcm(Dcm_MkCtrl);
   return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function provides the active session control type value. >
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SesType:Active Session Control Type value>
 * Param-Name[in/out]  <None>
 * Return              <E_OK,E_NOT_OK>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req Dcm-APIR-009[DCM339]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSesCtrlType(P2VAR(Dcm_SesType, AUTOMATIC, DCM_VAR)SesType )
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETSESCTRLTYPE_ID,
                        DCM_E_UNINIT);
        return  E_NOT_OK;
    }
#endif

#if(STD_ON == DCM_DEV_ERROR_DETECT)
    if( NULL_PTR == SesType)
    {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETSESCTRLTYPE_ID,
                        DCM_E_PARAM);
        return E_NOT_OK;
    }
#endif
    (*SesType) = Dcm_MkCtrl.Dcm_ActiveSes;
    SchM_Exit_Dcm(Dcm_MkCtrl);
    return  E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function returns the active protocol name. >
 * ServiceId           <0x0f>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <description...>
 * Param-Name[out]     <ActiveProtocol:Active protocol type value >
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
/****@req DCM-FUNR-245[DCM048]****/
/****@req Dcm_APIR_010[DCM340]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetActiveProtocol(P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR)ActiveProtocol )
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
    SchM_Enter_Dcm(Dcm_MkCtrl)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETACTIVEPROTOCOL_ID,
                        DCM_E_UNINIT);
        return  E_NOT_OK;
    }
#endif

#if(STD_ON == DCM_DEV_ERROR_DETECT)
    if(NULL_PTR == ActiveProtocol)
    {
        SchM_Exit_Dcm(Dcm_MkCtrl);
        Det_ReportError(DCM_MODULE_ID,
                        DCM_INSTANCE_ID,
                        DCM_GETACTIVEPROTOCOL_ID,
                        DCM_E_PARAM);
        return E_NOT_OK;
    }
#endif
    (*ActiveProtocol)=Dcm_MkCtrl.Dcm_ActiveProtocol;
    SchM_Exit_Dcm(Dcm_MkCtrl);
    return E_OK;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <The Pending message background processing function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
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
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_PendingManage(void)
{
	uint8  ProtocolCtrlId;
	uint8  MsgCtrlId;
	uint16 SidTabCfgIndex;
	uint16 SidTabServiceCfgIndex;
	P2FUNC(Std_ReturnType,DCM_APPL_CODE,Functionx)(uint8  ProtocolCtrlId);

	/************************************/
	/*Pending manager*/
	for(ProtocolCtrlId=0;ProtocolCtrlId<DCM_DSLPROTOCOLROW_NUM_MAX;ProtocolCtrlId++)
	{
		MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
		if(DCM_INVALID_UINT8 != MsgCtrlId)
		{
			if(DCM_MSG_RECEIVED  == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
			{
				if(DCM_PROTOCOL_INVALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
				{
					/*when DCM indition Applicaition start the protocol,App return E_PENDING*/
					DslInternal_RxIndication(ProtocolCtrlId);
				}
				else
				{
					/*when DCM indication Application to receive a new request,App return E_PENDING*/
					(void)DsdInternal_RxIndication(ProtocolCtrlId);
				}
			}
			else if(DCM_MSG_PROCESSED == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
			{
				/*when Message is "DCM_MSG_PROCESSED" status,App return E_PENDING"*/
				(void)DsdInternal_SearchSidTabServiceIndex( Dcm_MsgCtrl[MsgCtrlId].SID,
															ProtocolCtrlId,
															&SidTabCfgIndex,
															&SidTabServiceCfgIndex);
				Functionx = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].Function;
	            /*entering specific service procedures*/
	            (void)(*Functionx)(ProtocolCtrlId);
			}
			else if(  (FALSE == Dcm_MsgCtrl[MsgCtrlId].SendFlag)
	                 &&(DCM_MSG_TRANSMISSION == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
			{
				DslInternal_ProcessingDone(ProtocolCtrlId);
			}
			else
			{
				/*Avoid Misra error*/
			}
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <P2Server Timer background processing function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
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
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_P2ServerTimer(void)
{
	uint8  ProtocolCtrlId;
	uint8  MsgCtrlId;
	TickType OldTicks;
	TickType ExpireTicks;
	TickType Ticks;

	/************************************************************/
	for(ProtocolCtrlId=0;ProtocolCtrlId<DCM_DSLPROTOCOLROW_NUM_MAX;ProtocolCtrlId++)
	{
		MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
		if(DCM_INVALID_UINT8 !=  MsgCtrlId)
		{
			if(DCM_P2TIMER_ON == (Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2State))
			{
				OldTicks    = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2CurTicks;
				ExpireTicks = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2ExpiredTicks;
				Ticks       = Frt_CalculateElapsedTicks(OldTicks);
				if(Ticks >= ExpireTicks)
				{/*P2Server Timer timeout*/
					(void)DslInternal_P2ServerTimeout(ProtocolCtrlId);
				}
			}
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               <S3Timer background processing function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_S3Timer(void)
{
	TickType OldTicks;
	TickType ExpireTicks;
	TickType Ticks;

	/************************************************************/
	/*S3Server timer*/
	if(DCM_S3TIMER_ON == (Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State))
	{
		OldTicks    = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTicks;
		ExpireTicks = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTicks;
		Ticks       = Frt_CalculateElapsedTicks(OldTicks);
		if(Ticks >= ExpireTicks)
		{
			/*S3Server Timer timeout*/
			DslInternal_S3ServerTimeout();
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               <SecTimer background processing function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_MainFunction_SecTimer(void)
{
	TickType OldTicks;
	TickType ExpireTicks;
	TickType Ticks;
  STATIC boolean first_PowerOn = FALSE;
	/***************************************/
	/*security timer*/
	if(DCM_SECTIMER_ON==(Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState))
	{
		OldTicks     = Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTicks;
		ExpireTicks  = Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTicks;
		Ticks        = Frt_CalculateElapsedTicks(OldTicks);
		if(Ticks >= ExpireTicks)
		{
			/*security timeout*/
			SchM_Enter_Dcm(Dcm_SecCtrl);
			Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState = DCM_SECTIMER_OFF;
			Dcm_SecCtrl.Dcm_RunDlyFlag = FALSE;
			SchM_Exit_Dcm(Dcm_SecCtrl);
			//DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
			/*********************************/
  		if(first_PowerOn == FALSE) 
  		{
  		    first_PowerOn = TRUE;
  		} 
  		else 
  		{
  	    	Dcm_SecCtrl.Dcm_FalseAcessCount --;
  		}			
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/**********************************************************************/
/*
 * Brief               
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#if 0
uint8 gAppl_UpdataOK_ResponseFlag = FALSE;

#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void,DCM_CODE)Dcm_BootloaderResponse(void)
{
   PduInfoType *UDS_FAR PduInfo;
   
   if(0xD5 == (*(uint8*UDS_FAR)FL_APPL_UPDATE))
   {
      gAppl_UpdataOK_ResponseFlag = TRUE;
      /*receive App updata flag,response 0x51 01*/
   	  (*(uint8*UDS_FAR) FL_APPL_UPDATE) = 0x00;/*clear App updata flag*/
   	  Dcm_ProvideRxBuffer(3,2,&PduInfo);
   	  PduInfo->SduDataPtr[0] = 0x10;
   	  PduInfo->SduDataPtr[1] = 0x83;
   	  Dcm_RxIndication(3,NTFRSLT_OK);
   	  
   	  Dcm_ProvideRxBuffer(3,2,&PduInfo);
   	  PduInfo->SduDataPtr[0] = 0x11;
   	  PduInfo->SduDataPtr[1] = 0x01;
   	  Dcm_RxIndication(3,NTFRSLT_OK);
   	  
   }
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

#endif
/*************************************************************************/
/*
 * Brief               <This service is used for processing the tasks of the main loop. >
 * ServiceId           <0x25>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/****@req Dcm_APIR_046[DCM053]****/
/****@req DCM-APIR-047[DCM362]****/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE) Dcm_MainFunction(void)
{
#if(STD_ON == DCM_DEV_ERROR_DETECT)
   SchM_Enter_Dcm(Dcm_MkCtrl)
   if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
   {
       SchM_Exit_Dcm(Dcm_MkCtrl);
       Det_ReportError(DCM_MODULE_ID,
                       DCM_INSTANCE_ID,
                       DCM_MAIN_FUNCTION_ID,
                       DCM_E_UNINIT);
       return;
   }
   SchM_Exit_Dcm(Dcm_MkCtrl);
#endif

#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
   	if(E_OK == Get_FactoryMode())
   	{
   		gFactory_ActiveFlag = TRUE;
   	}
   	else
   	{
   		gFactory_ActiveFlag = FALSE;
   	}
#endif

   /***************************************************/
   /*The Pending message background processing function*/
   Dcm_MainFunction_PendingManage();

   /**************************************************/
   /*P2Server Timer background processing function*/
   Dcm_MainFunction_P2ServerTimer();

   /**************************************************/
   /*S3Timer background processing function*/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
   Dcm_MainFunction_S3Timer();
#endif
   
   /*************************************************/
   /*SecTimer background processing function*/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
   Dcm_MainFunction_SecTimer();
#endif

   /*************************************************/
   #if 0
   Dcm_BootloaderResponse();  
   #endif  

}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

