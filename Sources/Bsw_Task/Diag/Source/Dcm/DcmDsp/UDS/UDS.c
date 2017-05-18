/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <UDS.c>
 *  @brief      <functions of UDS services>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <Guan Shengyong>
 *  @date       <2013-03-27>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]==================================================*/
/*  <VERSION>    <DATE>        <AUTHOR>           <REVISION LOG>
 *   V0.1       20130327    Guan Shengyong       Initial version
 *
 *
 *
 *==============================================================================*/
/******************************* references ************************************/
#include "Dcm_Include.h"
#if (STD_ON == DCM_UDS_FUNC_ENABLED)
#include "Dem.h"
#include "UDS.h"
#include "Dem_Dcm.h"

/***********************************************************************
 ***********************************************************************
 ***********************************************************************/
	/**********************************/
#if(STD_ON == DCM_UDS_SERVICE0X10_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_Uds0x10SessionPermissionCheck(
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)NotAllowNum,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)ForceRespNm,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)PendignNum,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)DefaultNum,
									uint8 Subfunction);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X11_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x11ServiceConditionCheck(
									uint8 ProtocolCtrlId,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x11SubfunctionConditionCheck(
									uint8 ProtocolCtrlId,
									uint8 resetTypeIdx);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_Uds0x11SubfunctionPermissionCheck(
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)notAllowCnt,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)pendingCnt,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)otherRetCnt,
									uint8  resetType,
									P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X27_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceConditionCheck(uint8 ProtocolCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceSubfunctionCheck(
									uint8 ProtocolCtrlId,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlID,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)ReqSecLevel,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)SecCfgIndex,
									P2VAR(uint8, AUTOMATIC, AUTOMATIC)SubFunction);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_Uds0x27ServiceReqSeedResp(
									uint8   ProtocolCtrlId,
									uint8   MsgCtrlID,
									uint8   SubFunction,
									uint16  Offset,
									uint8   SeedSize,
									Std_ReturnType ret);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_Uds0x27ServiceCompareKeyResp(
									uint8 	 ProtocolCtrlId,
									uint8    MsgCtrlID,
									uint8    SubFunction,
									uint16   Offset,
									uint8    ReqSecLevel,
									uint8    SecCfgIndex,
									Std_ReturnType ret);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceActiveSecLevelCheck(
									uint8   ProtocolCtrlId,
									uint8   MsgCtrlID,
									uint8   ReqSecLevel,
									uint16  Offset,
									uint8   SeedSize,
									uint8   SubFunction,
									uint8   SeccfgIndex);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceStatusCheck(
									uint8 ProtocolCtrlId,
									uint8 SubFunction);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceMsgLenCheck(
									uint8  ProtocolCtrlId,
									uint8  SubFunction,
									uint8  ReqLen,
									uint8  AdrSize,
									uint8  KeySize);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_ENABLED)
	#if(STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED)
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x01(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED)
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x02(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED)
        #define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x03(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED)
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x04(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED)
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x06(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
	#endif

	#if(STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED)
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x0A(uint8 ProtocolCtrlId, uint8 MsgCtrlId);
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
    #endif
    
		#define DCM_START_SEC_CODE
		#include "MemMap.h"
		STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x19SubfunctionAnalyse( uint8 ProtocolCtrlId,
															  				uint8 MsgCtrlId,
															  				uint8 Subfunction );
		#define DCM_STOP_SEC_CODE
		#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X22_ENABLED)
	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidNumbercheck(uint8 numberOfDid);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidCheck(uint16 receiveDid,
																	 P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT)pIndex);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidSessionCheck(uint8  DidInfoCfgIndex,
																			P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidSecurityCheck(uint8  DidInfoCfgIndex,
																			 P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidConditionCheck(
																		uint8  ProtocolCtrlId,
																		uint8  DidCfgIndex,
																		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_ReadDidDataLength(
																		uint8 DidCfgIndex,
																		uint8 DidInfoCfgIndex,
																		P2VAR(uint16, AUTOMATIC, AUTOMATIC)pDidSize,
																		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x22ServiceConditonCheck(
																uint8 ProtocolCtrlId,
																P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define  DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsAssembleResponse(	uint8    ProtocolCtrlId,
																uint8    DidCfgIndex,
																P2VAR(uint16, AUTOMATIC, AUTOMATIC)ResOffset,
																uint16   RecDid,
																uint16   DidSize	);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X2E_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidCheck(	uint16 RecDid,
														P2VAR(uint8, AUTOMATIC, AUTOMATIC)pDidCfgIndex,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidSesCheck(uint8 DidInfoCfgIndex,
														  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidSecCheck(uint8 DidInfoCfgIndex,
														  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidWriteConditionCheck(uint8 ProtocolCtrlId,
																	 uint8 DidCfgIndex,
																	 P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidWriteDataLength(uint8   ProtocolCtrlId,
																 uint8   DidCfgIndex,
																 uint8   DidInfoCfgIndex,
																 P2VAR(uint16, AUTOMATIC, AUTOMATIC)pDidSize,
																 P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC Std_ReturnType  DspInternalUDS0x2E_DidWrite( uint8 ProtocolCtrlId,
														uint8 MsgCtrlId,
														uint8 DidCfgIndex,
														uint16 DidSize,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x2EServiceConditionCheck(	uint8 ProtocolCtrlId,
																   			P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsWriteDidSessionAndSecurityCheck(uint8   ProtocolCtrlId,
															   uint8   DidInfoCfgIndex,
															   P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X2F_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsConditionCheck(uint8 ProtocolCtrlId,
															   P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsIODidSessionAndSecutityCheck(	uint8 ProtocolCtrlId,
																  			P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_UdsReturnControlToEcu(	uint8   MsgCtrlId,
															uint8   DidCfgIndex,
															uint16  Offset,
															P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
															P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode,
															P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_UdsResetToDefult(uint8   MsgCtrlId,
													 uint8   DidCfgIndex,
													 uint16  Offset,
													 P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
													 P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
													 P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_UdsFreezeCurrentState( uint8   MsgCtrlId,
													uint8   DidCfgIndex,
													uint16  Offset,
													P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
													P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
													P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_UdsShortTermAdjustment(uint8   MsgCtrlId,
													uint8   DidCfgIndex,
													uint16  Offset,
													P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
													P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
													P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_UdsIOControlSendResponse(	uint8 	ProtocolCtrlId,
																uint8   MsgCtrlId,
																uint16  RecDid,
																uint16  Offset,
																uint8   ControlStatusRecordSize,
																uint8   InputOutControlParameter,
																Dcm_NegativeResponseCodeType ErrorCode,
																Std_ReturnType  Ret);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_RoutineControlServiceConditonCheck(uint8   ProtocolCtrlId,
															 					P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_RoutineIdSessionAndSecurityCheck(uint8 ProtocolCtrlId,
																 			  P2CONST(Dcm_DspRoutineAuthorizationType, AUTOMATIC, DCM_APPL_CONST)pDspRoutineAuthorization);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_StartRoutine(  	uint8  MsgCtrlId,
																	uint8  RoutineCfgId,
																	uint8  RoutineInfoCfgId,
																	uint16 Offset,
																	P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
																	P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_StopRoutine(	uint8  MsgCtrlId,
													 			uint8  RoutineCfgId,
													 			uint8  RoutineInfoCfgId,
													 			uint16 Offset,
													 			P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
													 			P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_RequestRoutinrResult(uint8   MsgCtrlId,
														  uint8   RoutineCfgId,
														  uint8   RoutineInfoCfgId,
														  uint16  Offset,
														  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
														  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dcm_RoutineControlServiceSendResponse(uint8   ProtocolCtrlId,
												uint8   MsgCtrlId,
												uint8   ResLen,
												uint16  routineId,
												uint16  Offset,
												uint8   SubFunction,
												Std_ReturnType  Ret,
												Dcm_NegativeResponseCodeType  ErrorCode);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif

	/***********************************/
#if (STD_ON == DCM_UDS_SERVICE0X85_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsCDSConditionCheck(uint8  ProtocolCtrlId,
																  P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif


	/***********************************/
#if(STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(void, DCM_CODE)Dsp_InitLinkCtrolCtrl(void);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsLCCondtionCheck(uint8  ProtocolCtrlId,
																P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"

	#define DCM_START_SEC_CODE
	#include "MemMap.h"
	STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsLCVBTWFB(uint8  ProtocolCtrlId,
														 uint8  MsgCtrlId,
														 P2VAR(uint8, AUTOMATIC, AUTOMATIC)baudrateIdentifier);
	#define DCM_STOP_SEC_CODE
	#include "MemMap.h"
#endif
/***********************************************************************
 ***********************************************************************
 ***********************************************************************/
/*control block for link control service*/
#if(STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
	#define DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
	#include "MemMap.h"
	STATIC VAR(Dcm_LinkControlCtrlType, DCM_VAR) Dcm_LinkControlCtrl = {LINK_CONTROL_IDLE,0,0};
	#define DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
	#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsCommunicaitonControl(uint8  controlType,uint8 communicationType);
#endif


/****************************************************************
	 UDS:DiagnosticSessionControl (10 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X10_ENABLED)
   /*******************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_Uds0x10SessionPermissionCheck( P2VAR(uint8, AUTOMATIC, AUTOMATIC)NotAllowNum,
													  P2VAR(uint8, AUTOMATIC, AUTOMATIC)ForceRespNm,
   													  P2VAR(uint8, AUTOMATIC, AUTOMATIC)PendignNum,
   													  P2VAR(uint8, AUTOMATIC, AUTOMATIC)DefaultNum,
   													  uint8 Subfunction )
{
    uint8 SesIndex;
   	Std_ReturnType  ret = E_OK;

    /**********************************/
   	/*check get session permission*/
   	for(SesIndex = 0;SesIndex < (Dcm_DslCfg.DcmDslSessionControl_PortNum);SesIndex++)
   	{
   		ret = (*(Dcm_DslCfg.pDcmDslSessionControl[SesIndex].GetSesChgPermission))(Dcm_SesCtrl.Dcm_ActiveSes,Subfunction);
   		switch(ret)
   		{
   			case  E_OK:
   				break;
   			case  E_SESSION_NOT_ALLOWED:
   				*NotAllowNum = *NotAllowNum + 1;
   				break;
   			#if(STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
   			case  E_FORCE_RCRRP:
   				*ForceRespNm = *ForceRespNm + 1;
   				break;
   			#endif
   			case  E_PENDING:
   				*PendignNum = *PendignNum + 1;
   				break;
   			default:
   				*DefaultNum = *DefaultNum + 1;
   				break;
   		}
   	}
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif
   
/*******************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x10(uint8  ProtocolCtrlId)
{
   	uint8   MsgCtrlId;
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
   	#if(STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
   	uint8   ForceRespNm = 0;
   	#endif
   	uint8   SesIndex;
   	uint8   Subfunction;
   	uint8   DspSessionRowNum;
   	uint8   DefaultNum = 0;
   	uint8   PendignNum = 0;
   	uint8   NotAllowNum = 0;
   	uint8   TxChannelCtrlIndex;
   	uint8   TxChannelCfgIndex;
   	uint16  Offset;
   	boolean  Flag;
    P2CONST(Dcm_DspSessionRowType ,AUTOMATIC ,DCM_APPL_CONST) pDspSessionRow;
#endif
#if((STD_ON==DCM_SESSION_FUNC_ENABLED)||(STD_ON == DCM_SECURITY_FUNC_ENABLED) )
    	Std_ReturnType  ret = E_OK;
#endif

   	/**************************************/
   #if(STD_ON == DCM_SESSION_FUNC_ENABLED)
   	/*session check,check whether the current session supports the request service*/
   	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_DIAGNOSTIC_SESSION_CONTROL);
   	if(E_NOT_OK == ret)
   	{
   		/****@req DCM-FUNR-073[DCM211]****/
   		/*the current session does not support the request service,send NRC = 0x7F*/
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_NOT_OK;
   	}
   #endif
   
   	/**************************************/
   #if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
   	/*security check,check whether the current security supports the request service*/
   	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_DIAGNOSTIC_SESSION_CONTROL);
   	if (E_NOT_OK == ret)
   	{
   		/****@req DCM-FUNR-074[DCM217]****/
   		/*the current security does not support the request service,send NRC = 0x33*/
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_NOT_OK;
   	}
   #endif
   
   	/*************************************/
   	/*if the required protocol is configured,get the index of runtime datum*/
   	MsgCtrlId =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
   
   	/*check the message length*/
   	if (DCM_UDS0X10_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
   	{
   		/*the length of massage is not correct,send NRC 0x13*/
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_NOT_OK;
   	}
   	
   #if(STD_OFF == DCM_SESSION_FUNC_ENABLED)
   	/*if the session function is disabled,send NRC 0x31*/
   	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
   	DsdInternal_ProcessingDone(ProtocolCtrlId);
   	return E_NOT_OK;
   	
   #else
   	Subfunction = (Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData)[1];
   	DspSessionRowNum = Dcm_DspCfg.pDcm_DspSession->DcmDspSessionRow_Num;
   	pDspSessionRow   = Dcm_DspCfg.pDcm_DspSession->pDcmDspSessionRow;
   	Flag = FALSE;
   
   	/*************************************************/
   	/*check whether the required session is configured*/
   	for(SesIndex=0;(SesIndex<DspSessionRowNum)&&(FALSE==Flag);SesIndex++)
   	{
   		if(Subfunction == (pDspSessionRow[SesIndex].DcmDspSessionLevel))
   		{
   			Flag = TRUE;
   		}
   	}
   
   	/*if not configured,send NRC 0x12*/
   	if(FALSE == Flag)
   	{
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_NOT_OK;
   	}
   
   	/*************************************************/
   	/*check get session permission*/
   	Dcm_Uds0x10SessionPermissionCheck(	&NotAllowNum,
   							  			&ForceRespNm,
   							  			&PendignNum,
   							  			&DefaultNum,
   							  			Subfunction	);
   
   	/*if not allowed or other unexpected return is present,send NRC 0x22*/
   	if ((NotAllowNum > 0) || (DefaultNum > 0))
   	{
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_SESSION_NOT_ALLOWED;
   	}
   
   #if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
   	/*deal with 'force response pending'*/
   	else if(ForceRespNm>0)
   	{
   		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_RESPONSE_PENDING);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return E_FORCE_RCRRP;
   	}
   #endif
   	else if(PendignNum>0)
   	{
   		/*deal with pending*/
   		return E_PENDING;
   	}
   	else
   	{
   		/*assemble and send positive response*/
   		SchM_Enter_Dcm(Dcm_SesCtrl);
   		Dcm_SesCtrl.Dcm_NewSes = Subfunction;
   		SchM_Exit_Dcm(Dcm_SesCtrl);
   		SchM_Enter_Dcm(Dcm_MsgCtrl);
   		SchM_Enter_Dcm(Dcm_Channel);
   		TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
   		TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
   		Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
   		Dcm_Channel[Offset] = 0x50;
   		Dcm_Channel[Offset + 1] = Subfunction;
   		
			Dcm_Channel[Offset + 2] = (uint8)(pDspSessionRow[SesIndex-1].DcmDspSessionP2ServerMax >>8u);
		  Dcm_Channel[Offset + 3] = (uint8)(pDspSessionRow[SesIndex-1].DcmDspSessionP2ServerMax);
	  	Dcm_Channel[Offset + 4] = (uint8)(((pDspSessionRow[SesIndex-1].DcmDspSessionP2StarServerMax) / 10u) >>8u);
		  Dcm_Channel[Offset + 5] = (uint8)((pDspSessionRow[SesIndex-1].DcmDspSessionP2StarServerMax / 10u));
   		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 6u;
   		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen 	= 6u;	
   		
   		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
   		SchM_Exit_Dcm(Dcm_Channel);
   		SchM_Exit_Dcm(Dcm_MsgCtrl);
   		DsdInternal_ProcessingDone(ProtocolCtrlId);
   		return  E_OK;
   	}
   #endif
   }
   #define DCM_STOP_SEC_CODE
   #include "MemMap.h"
 #endif


/****************************************************************
	 			UDS:ECUReset(0x11) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X11_ENABLED)
/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x11ServiceConditionCheck( uint8 ProtocolCtrlId,
																	P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType ret = E_OK;
     
	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (DCM_UDS0X11_REQ_DATA_LENGTH != Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	/*************************************************/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_ECU_RESET);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

    /*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId, SID_ECU_RESET);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x11SubfunctionConditionCheck(uint8 ProtocolCtrlId, uint8 resetTypeIdx)
{
	Std_ReturnType ret = E_OK;
	#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) || (STD_ON == DCM_SECURITY_FUNC_ENABLED))
	uint8  index = 0;
	boolean conditionFlag = FALSE;
	#endif

	/*if the sub-function is supported,check session and security*/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	conditionFlag = FALSE;
	for (index = 0; (index < Dcm_DspCfg.pDcmDspEcuReset[resetTypeIdx].DcmDspEcuResetSessionRefNum) && (FALSE == conditionFlag); index++)
	{
		if (Dcm_SesCtrl.Dcm_ActiveSes == Dcm_DspCfg.pDcmDspEcuReset[resetTypeIdx].pDcmDspEcuResetSessionRow[index])
		{
			conditionFlag = TRUE;
		}
	}
	/*if the current session does not support reset,send NRC 0x7E*/
	if (FALSE == conditionFlag)
	{
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
   /*check the current security level*/
	#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
	conditionFlag = FALSE;
	for (index = 0; (index < Dcm_DspCfg.pDcmDspEcuReset[resetTypeIdx].DcmDspEcuResetSecurityLevelRefNum) && (FALSE == conditionFlag); index++)
	{
		if (Dcm_SecCtrl.Dcm_ActiveSec == Dcm_DspCfg.pDcmDspEcuReset[resetTypeIdx].pDcmDspEcuResetSecurityLevelRow[index])
		{
			conditionFlag = TRUE;
		}
	}
	/*if the current security does not support reset,send NRC 0x33*/
	if (FALSE == conditionFlag)
	{
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#if(STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_Uds0x11SubfunctionPermissionCheck(
														P2VAR(uint8, AUTOMATIC, AUTOMATIC)notAllowCnt,
														P2VAR(uint8, AUTOMATIC, AUTOMATIC)pendingCnt,
														P2VAR(uint8, AUTOMATIC, AUTOMATIC)otherRetCnt,
														uint8  resetType,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode)
{
	uint8  index = 0;
	Std_ReturnType ret = E_OK;

	/*invoke all the callouts for reset permission*/
	for (index = 0;index < Dcm_DspCfg.DcmEcuResetPortNum; index++)
	{
		ret = Dcm_DspCfg.pDcmEcuResetPort[index](resetType, ErrorCode);
		switch (ret)
		{
			case E_OK:
				break;
			case E_NOT_OK:
				*notAllowCnt = *notAllowCnt + 1;
				break;
			case E_PENDING:
				*pendingCnt = *pendingCnt + 1;
				break;
			default:
				/*some unexpected return value*/
				*otherRetCnt = *otherRetCnt + 1;
				break;
		}
	}
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x11(uint8  ProtocolCtrlId)
{
#if(STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
	uint8  index = 0;
	uint8  MsgCtrlId = 0;
	uint8  resetType = 0;
	uint8  notAllowCnt = 0;
	uint8  pendingCnt = 0;
	uint8  otherRetCnt = 0;
	uint8  resetTypeIdx = DCM_INVALID_UINT8;
	uint8  TxChannelCtrlIndex = 0;
	uint8  TxChannelCfgIndex = 0;
	uint16 Offset = 0;
	Dcm_NegativeResponseCodeType ErrorCode;
	Std_ReturnType ret = E_OK;

  
	ret = Dcm_Uds0x11ServiceConditionCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}
	/*************************************************/
	/*get the reset type*/
	resetType = (Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData)[1];

	/*check the required reset type is supported*/
	for (index = 0; (index < Dcm_DspCfg.DcmDspEcuResetNum) && (DCM_INVALID_UINT8 == resetTypeIdx); index++)
	{
		if (Dcm_DspCfg.pDcmDspEcuReset[index].DcmEcuResetType == resetType)
		{
			resetTypeIdx = index;
		}
	}
	if (DCM_INVALID_UINT8 == resetTypeIdx)
	{
		/*if the required reset type is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*************************************************/
 	 /*if the sub-function is supported,check session and security*/
	ret = Dcm_Uds0x11SubfunctionConditionCheck(ProtocolCtrlId, resetTypeIdx);
	if(E_OK != ret)
	{
		return ret;
	}

	/*************************************************/
 	/*invoke all the callouts for reset permission*/
	Dcm_Uds0x11SubfunctionPermissionCheck(	&notAllowCnt, 
	                               			&pendingCnt, 
	                               			&otherRetCnt, 
	                               			resetType, 
	                               			&ErrorCode);

	if (notAllowCnt > 0)
	{
		/*if not allowed is present,make ErrorCode as the negative response*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, ErrorCode);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (otherRetCnt > 0)
	{
		/*if the unexpected return value is present,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (pendingCnt > 0)
	{
		/*deal with pending*/
		return E_PENDING;
	}
	else
	{
		/*if all return values are OK,assemble and send positive response*/
		TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
		TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
		Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
		/*assemble positive response*/
		SchM_Enter_Dcm(Dcm_Channel);
		Dcm_Channel[Offset]   = 0x51;/*response SID*/
		Dcm_Channel[Offset+1] = resetType;/*an echo of bits 6 - 0 of the sub-function parameter*/
		SchM_Exit_Dcm(Dcm_Channel);
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 2u;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen  = 2u;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData  = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}
#else
	(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_REQUESTOUTOFRANGE);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	 UDS:SecurityAccess(27 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X27_ENABLED)
/*******************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceConditionCheck(uint8 ProtocolCtrlId)
{
	Std_ReturnType ret = E_OK;
    uint8  MsgCtrlIndex = 0xFF;
    uint16 MsgLength = 0;
	/*************************************************/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId, SID_SECURITY_ACCESS);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

    /*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId, SID_SECURITY_ACCESS);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif
    
    /*************************************************/
    MsgCtrlIndex =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    MsgLength = Dcm_MsgCtrl[MsgCtrlIndex].MsgContext.ReqDataLen;
    if(MsgLength < DCM_UDS0X27_REQ_DATA_MINLENGTH)
    {
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }
	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceSubfunctionCheck(	uint8 ProtocolCtrlId,
																			P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlID,
																			P2VAR(uint8, AUTOMATIC, AUTOMATIC)ReqSecLevel,
																			P2VAR(uint8, AUTOMATIC, AUTOMATIC)SecCfgIndex,
																			P2VAR(uint8, AUTOMATIC, AUTOMATIC)SubFunction)
{
	uint8   	Index;
	uint8   	SecurityRowNum;
	boolean     Flag;
	uint8       SecCfgLevel;
#if(STD_ON == DELAY_TIME_IN_BOOT_ENABLED)
	TickType    OldTicks;
	TickType    Ticks;
#endif
	Std_ReturnType ret = E_OK;

	/********************************/
	/*try to find the corresponding security level in configuration*/
	*MsgCtrlID   = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
	*SubFunction = (Dcm_MsgCtrl[*MsgCtrlID].MsgContext.pReqData)[1];
	*ReqSecLevel = (*SubFunction + 1u)/2u;
	SecurityRowNum = Dcm_DspCfg.pDcm_DspSecurity->DcmDspSecurityRow_Num;
	/*********************************************/
	if(*ReqSecLevel == 0x00)
	{
	    /*if not found in DcmDspSecurityRow,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;

	}
	/*********************************************/
	Flag = FALSE;
	for(Index=0;(Index<SecurityRowNum)&&(FALSE==Flag);Index++)
	{
		SecCfgLevel = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[Index].DcmDspSecurityLevel;
		if(*ReqSecLevel == SecCfgLevel)
		{   
		    *SecCfgIndex = Index;
			Flag = TRUE;
		}
	}
	if(FALSE == Flag)
	{
		/*if not found in DcmDspSecurityRow,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}

	/*************************************************/
	/*deal with delay on boot*/
	*SecCfgIndex = Index - (uint8)1;
	
	#if(STD_ON == DELAY_TIME_IN_BOOT_ENABLED)
	if (  (0u != (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[*SecCfgIndex].DcmDspSecurityDelayTimeOnBoot)
	&&(DCM_SECTIMER_ON == Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecTimerState))
	{
		/*yes,need to deal with delay on boot*/
		OldTicks = Dcm_SecCtrl.Dcm_OnBootDlyCtrl.Dcm_SecCurTicks;
		Ticks = Frt_CalculateElapsedTicks(OldTicks);
		if ( Ticks < (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[*SecCfgIndex].DcmDspSecurityDelayTimeOnBoot)
		{
			/*if still in delay on boot,prohibit this request and reset resource*/
			(void)DslInternal_ResetResource(ProtocolCtrlId);
			return E_NOT_OK;
		}
	}
	#endif 
	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif
/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_Uds0x27ServiceReqSeedResp(	uint8   ProtocolCtrlId,
															uint8   MsgCtrlID,
															uint8   SubFunction,
															uint16  Offset,
															uint8   SeedSize,
															Std_ReturnType ret)
{
	switch(ret)
	{
		case E_OK:
			SchM_Enter_Dcm(Dcm_MsgCtrl);
			Dcm_Channel[Offset] = 0x67;
			Dcm_Channel[Offset + 1] = SubFunction;
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResMaxDataLen =((uint32)SeedSize+2);
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResDataLen    =((uint32)SeedSize+2);
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.pResData      =&Dcm_Channel[Offset];
			SchM_Exit_Dcm(Dcm_MsgCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			SchM_Enter_Dcm(Dcm_SecCtrl);
			/*seed has been sent,set the status to DCM_SERVICE_SEED*/
			Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_SEED;
			Dcm_SecCtrl.Dcm_SubfunctionForSeed = SubFunction;
			SchM_Exit_Dcm(Dcm_SecCtrl);
			break;

		case E_NOT_OK:
			/*if fail to get seed,send NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;

		case E_PENDING:
			break;

		default:
			/*unexpected return value,send NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_Uds0x27ServiceCompareKeyResp(
			uint8 	 ProtocolCtrlId,
			uint8    MsgCtrlID,
			uint8    SubFunction,
			uint16   Offset,
			uint8    ReqSecLevel,
			uint8    SecCfgIndex,
			Std_ReturnType ret)
{
	switch(ret)
	{
		case E_OK:
			Dcm_Channel[Offset] = 0x67;
			Dcm_Channel[Offset + 1] = SubFunction;
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResMaxDataLen = 2u;
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResDataLen    = 2u;
			Dcm_MsgCtrl[MsgCtrlID].MsgContext.pResData      = &Dcm_Channel[Offset];
			SchM_Enter_Dcm(Dcm_SecCtrl);
			Dcm_SecCtrl.Dcm_NewSec = ReqSecLevel;
			Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_KEY;
			SchM_Exit_Dcm(Dcm_SecCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;

		case E_NOT_OK:
			/*ret = E_NOT_OK,sent NRC 0x22*/
			DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;

		case E_PENDING:
			break;

		case E_COMPARE_KEY_FAILED:
			/*'compare key' is failed,increase the false count*/
			SchM_Enter_Dcm(Dcm_SecCtrl);
			Dcm_SecCtrl.Dcm_FalseAcessCount = Dcm_SecCtrl.Dcm_FalseAcessCount + (uint8)1;
			
			if(Dcm_SecCtrl.Dcm_FalseAcessCount >= (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].DcmDspSecurityNumAttDelay)
			{
				Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTicks     = Frt_ReadOutTicks();
				Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTicks = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].DcmDspSecurityDelayTime;
				Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState   = DCM_SECTIMER_ON;
				SchM_Exit_Dcm(Dcm_SecCtrl);
				
				DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
				(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_EXCEEDEDNUMBEROFATTEMPTS);/*NRC=0x36*/
				DsdInternal_ProcessingDone(ProtocolCtrlId);
			}
			else
			{
				SchM_Exit_Dcm(Dcm_SecCtrl);
				DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
				(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INVALIDKEY);/*NRC=0x35*/
				DsdInternal_ProcessingDone(ProtocolCtrlId);
			}
			break;
		default:
			/*unexpected return value is present,send NRC 0x22*/
			DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceActiveSecLevelCheck(
				uint8   ProtocolCtrlId,
				uint8   MsgCtrlID,
				uint8   ReqSecLevel,
				uint16  Offset,
				uint8   SeedSize,
				uint8   SubFunction,
				uint8   SeccfgIndex)
{
	uint8       Index;
	Std_ReturnType ret = E_OK;

	/*check whether the security service state is idle or seed.*/
    if((DCM_SERVICE_IDLE != Dcm_SecCtrl.Dcm_SecServiceState)
    	&& (DCM_SERVICE_SEED != Dcm_SecCtrl.Dcm_SecServiceState)) 	 
	{
		  /*if not,send NRC 0x24:request sequence error*/
		  DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
		  (void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTSEQUENCEERROR);
		  DsdInternal_ProcessingDone(ProtocolCtrlId);
		  return  E_NOT_OK;
	}
	if (ReqSecLevel == Dcm_SecCtrl.Dcm_ActiveSec)
	{
		/*if the server is on the request security level,set seed to zero*/
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_Channel[Offset] = 0x67;
		Dcm_Channel[Offset + 1] = SubFunction;
		for (Index = 0; Index < SeedSize; Index++)
		{
			Dcm_Channel[Offset + Index+ 2] = 0x00;
		}
		Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResMaxDataLen = ((uint32)SeedSize + 2u);
		Dcm_MsgCtrl[MsgCtrlID].MsgContext.ResDataLen    = ((uint32)SeedSize + 2u);
		Dcm_MsgCtrl[MsgCtrlID].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceStatusCheck(
			uint8 ProtocolCtrlId,
			uint8 SubFunction)
{
	Std_ReturnType ret = E_OK;

	/*check the status*/
	if (DCM_SERVICE_SEED != Dcm_SecCtrl.Dcm_SecServiceState)
	{
		/*if the server is not on the status DCM_SERVICE_SEED,send NRC 0x24:request sequence error*/
		DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTSEQUENCEERROR);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	if ((SubFunction <= Dcm_SecCtrl.Dcm_SubfunctionForSeed)
		|| (1u != (SubFunction-(Dcm_SecCtrl.Dcm_SubfunctionForSeed))))
	{
		/*'requestSeed' and 'seedKey' don't  match*/
		DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x27ServiceMsgLenCheck( uint8  ProtocolCtrlId,
																	uint8  SubFunction,
																	uint8  ReqLen,
																	uint8  AdrSize,
																	uint8  KeySize)
{
	uint8  ReqCfgLen;
	Std_ReturnType ret = E_OK;

	if (1u == (SubFunction & 0x01u))
	{
		ReqCfgLen = (uint8)(AdrSize + 2u);
	}
	else
	{
		ReqCfgLen = (uint8)(KeySize + 2u);
	}
	if (ReqLen != ReqCfgLen)
	{
		/*if the request message length is not correct,send NRC 0x13*/
		DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x27(uint8  ProtocolCtrlId)
{
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	uint8       Index;
	uint8       ReqSecLevel;
	uint8       SubFunction;
	uint8       MsgCtrlID;
	uint8       SecCfgIndex = 0u;
	uint8       ReqLen;
	uint8       AdrSize;
	uint8       SeedSize;
	uint8       KeySize;
	uint8       TxChannelCtrlIndex;
	uint8       TxChannelCfgIndex;
	uint16      Offset;
	#if (DCM_SECURITY_ADR_MAXSIZE > 0)
	uint8   Dcm_Adr[DCM_SECURITY_ADR_MAXSIZE] = {0};
	#endif
	Dcm_NegativeResponseCodeType  NegativeResponseCode;
	P2FUNC(Std_ReturnType ,DCM_APPL_CODE ,ComparedKey)(uint8 *UDS_FAR key);
	P2FUNC(Std_ReturnType ,DCM_APPL_CODE ,GottSeed)(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);
#endif
	Std_ReturnType ret;

	/*************************************************/
	ret = Dcm_Uds0x27ServiceConditionCheck(ProtocolCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}

	/*************************************************/
#if(STD_OFF == DCM_SECURITY_FUNC_ENABLED)
	DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  E_NOT_OK;
#else

	/*************************************************/
	/*if the delay timer is active and a request is transmitted,send NRC 0x37*/
	if(DCM_SECTIMER_ON == Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState)
	{
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUIREDTIMEDELAYNOTEXPIRED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	
	/*************************************************/
	ret = Dcm_Uds0x27ServiceSubfunctionCheck(	ProtocolCtrlId,
	                      						&MsgCtrlID,
	                      						&ReqSecLevel,
	                      						&SecCfgIndex,
	                      						&SubFunction);
	if(E_OK != ret)
	{
		return ret;
	}

	/*************************************************/
	/*check whether the corresponding callouts are configured*/
	ComparedKey = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].Dcm_SecurityAccess.CompareKey;
	GottSeed    = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].Dcm_SecurityAccess.GetSeed;
	if ((NULL_PTR == ComparedKey) || (NULL_PTR == GottSeed))
	{
		/*if not configured,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	/*************************************************/
	/*check the request message length*/
	AdrSize  = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].DcmDspSecurityADRSize;
	SeedSize = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].DcmDspSecuritySeedSize;
	KeySize  = (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].DcmDspSecurityKeySize;
	ReqLen = (uint8)(Dcm_MsgCtrl[MsgCtrlID].MsgContext.ReqDataLen);

	ret = Dcm_Uds0x27ServiceMsgLenCheck(ProtocolCtrlId,
		                       			SubFunction,
		                       			ReqLen,
		                       			AdrSize,
		                       			KeySize	);
	if(E_OK != ret)
	{
		return ret;
	}

	/*************************************************/
	/*get Tx buffer*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlID].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	/*deal with 'request seed'*/
	if (0x01 == (SubFunction & 0x01u))
	{
		/*check whether the security service state is idle*/
		ret = Dcm_Uds0x27ServiceActiveSecLevelCheck(ProtocolCtrlId,
													MsgCtrlID,
													ReqSecLevel,
													Offset,
													SeedSize,
													SubFunction,
													SecCfgIndex);
		if(E_OK != ret)
		{
			return ret;
		}
		/*if the server is on the other security level,check the size of security access data record*/
		#if (DCM_SECURITY_ADR_MAXSIZE > 0)
		if(0u == AdrSize)
		{
		#endif
			ret = (* GottSeed)(NULL_PTR,&Dcm_Channel[Offset + 2],&NegativeResponseCode);
		#if (DCM_SECURITY_ADR_MAXSIZE > 0)
		}
		else
		{   /*read-out ADR  from request message*/
			for(Index=0;Index<AdrSize;Index++)
			{
				Dcm_Adr[Index] = (Dcm_MsgCtrl[MsgCtrlID].MsgContext.pReqData)[Index + 2u];
			}
			ret = (* GottSeed)(&Dcm_Adr[0],&Dcm_Channel[Offset + 2],&NegativeResponseCode);
		}
		#endif

		/* send message */
		Dcm_Uds0x27ServiceReqSeedResp(	ProtocolCtrlId,
										MsgCtrlID,
										SubFunction,
										Offset,
										SeedSize,
										ret	);

	} /*ENDOF if(0x01 == (SubFunction & 0x01u))*/
	/*deal with 'send key'*/
	else
	{
		/*check the status*/
		ret = Dcm_Uds0x27ServiceStatusCheck(ProtocolCtrlId, SubFunction);
		if(E_OK != ret)
		{
			return ret;
		}

		/*if 'requestSeed' and 'seedKey' match,compare the key*/
#if (STD_ON == DCM_FACTORY_MODE_ENABLED)	
		if(gFactory_ActiveFlag == TRUE) 
		{
		   /*do nothing*/
		} 
		else 
#endif
		{
		   ret = (* ComparedKey)(&(Dcm_MsgCtrl[MsgCtrlID].MsgContext.pReqData[2]));
		}
		
		
		/* send message*/
		Dcm_Uds0x27ServiceCompareKeyResp(	ProtocolCtrlId,
											MsgCtrlID,
											SubFunction,
											Offset,
											ReqSecLevel,
											SecCfgIndex,
											ret);
	}
	return (ret);
#endif
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	     UDS:ClearDiagnosticInformation (14 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X14_ENABLED)
/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x14(uint8  ProtocolCtrlId)
{
	uint8   MsgCtrlId = 0;
	uint8   TxChannelCtrlIndex = 0;
	uint8   TxChannelCfgIndex = 0;
	uint16  Offset = 0;
	uint32  groupOfDTC = 0;
	Dem_ReturnClearDTCType returnClearDTC = DEM_CLEAR_OK;
	Std_ReturnType ret = E_OK;

	/*************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_CLEAR_DIAGNOSTIC_INFORMATION);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_CLEAR_DIAGNOSTIC_INFORMATION);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
	/*if the required protocol is configuted,get the index of runtime datum*/
	MsgCtrlId =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (DCM_UDS0X14_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*************************************************/
	/*get and assemble DTC, high byte first*/
	groupOfDTC = (((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1]))<< 16)\
	           	      |(((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2]))<<8)\
	                    |((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3]));

	/*invoke the corresponding API provided by DEM*/
	returnClearDTC = Dem_ClearDTC(	groupOfDTC, 
	                             	DEM_DTC_KIND_ALL_DTCS, 
	                             	DEM_DTC_ORIGIN_PRIMARY_MEMORY);

	switch (returnClearDTC)
	{
		case DEM_CLEAR_OK:
			/*clear is successful,assemble and send the positive response*/
			TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
			TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
			Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
			SchM_Enter_Dcm(Dcm_Channel);
			Dcm_Channel[Offset] = 0x54;   /*response SID*/
			SchM_Exit_Dcm(Dcm_Channel);
			SchM_Enter_Dcm(Dcm_MsgCtrl);
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x01;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 0x01;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
			SchM_Exit_Dcm(Dcm_MsgCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
		case DEM_CLEAR_WRONG_DTC:
			/*Server does not support the required groupOfDTC,send NRC 0x31*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_REQUESTOUTOFRANGE);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
		case DEM_CLEAR_WRONG_DTCORIGIN:
		case DEM_CLEAR_WRONG_DTCKIND:
		case DEM_CLEAR_FAILED:
			/*internal conditions within the server prevent the 
			   clearing of DTC related information stored in the server,sent NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
		case DEM_CLEAR_PENDING:
			ret = E_PENDING;
			break;
		default:
			/*the unexpected return value is present,send NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
	}
	
	return (ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	     UDS:ReadDTCInformation (19 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_ENABLED)

#if(STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x01(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8    TxChannelCtrlIndex = 0;
	uint8    TxChannelCfgIndex = 0;
	uint8    DtcStatusMask = 0;
	uint16   DtcCount = 0;
	uint16   Offset = 0;
	boolean  NotAllowedFlag = FALSE;
	boolean  PendingFlag = FALSE;
	Std_ReturnType  ret = E_OK;
	Dcm_MsgLenType  MsgLen = 0;
	Dem_ReturnSetDTCFilterType    ReturnSetDTCFilter;
	Dem_ReturnGetNumberOfFilteredDTCType  ReturnGetNumberOfFilteredDTC;

    /**************************************************/
	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X01_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

    /**************************************************/
	/*Sub-function:0x01*/
	MsgLen = 2u;
	PendingFlag    = FALSE;
	NotAllowedFlag = FALSE;
	DtcStatusMask  = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];

	/*obtain DTCStatusAvailabilityMask*/
	ret = Dem_GetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen]);
	if (E_OK == ret)
	{
		/*one byte for the storage of DTC status availability mask*/
		MsgLen = MsgLen + 1u;

		/*get and store DTCFormatIdentifier*/
		Dcm_Channel[Offset + MsgLen] = Dem_GetTranslationType();
		MsgLen = MsgLen + 1u;

		/*set DTC filter*/
		ReturnSetDTCFilter = Dem_SetDTCFilter(	DtcStatusMask,
												DEM_DTC_KIND_ALL_DTCS,
												DEM_DTC_ORIGIN_PRIMARY_MEMORY,
												DEM_FILTER_WITH_SEVERITY_NO,
												DEM_SEVERITY_NO_SEVERITY,
												DEM_FILTER_FOR_FDC_NO	);
		if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
		{
			/*get the number of required DTC*/
			ReturnGetNumberOfFilteredDTC = Dem_GetNumberOfFilteredDTC(&DtcCount);
			switch(ReturnGetNumberOfFilteredDTC)
			{
				case DEM_NUMBER_OK:
					Dcm_Channel[Offset + MsgLen] = (uint8)(DtcCount>>8);
					MsgLen = MsgLen + 1u;
					Dcm_Channel[Offset + MsgLen] = (uint8)DtcCount;
					MsgLen = MsgLen + 1u;
					break;
				case DEM_NUMBER_FAILED:
					NotAllowedFlag = TRUE;
					break;
				case DEM_NUMBER_PENDING:
					PendingFlag = TRUE;
					break;
				default:
					NotAllowedFlag = TRUE;
					break;
			}
		}
		else
		{
			/*if fail to set DTC filter*/
			NotAllowedFlag  = TRUE;
		}
	}
	else
	{
		NotAllowedFlag = TRUE;
	}
    /**************************************************/
	if (TRUE == NotAllowedFlag)
	{
		/*if fail to get any request information,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (TRUE == PendingFlag)
	{
		/*if some request info is not available yet,enter pending and wait for the next cycle*/
		return E_PENDING;
	}
	else
	{
		/*assemble positive response*/
		Dcm_Channel[Offset] = 0x59;
		Dcm_Channel[Offset + 1u] = 0x01;
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}
	
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x02(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint8     DtcStatus;
	uint8     DtcStatusMask;
	uint16    Offset;
	uint16    Index;
	uint16    DtcCount;
	uint32    Dtc;
	boolean   NotAllowedFlag = FALSE;
	boolean   PendingFlag = FALSE;
	boolean   Flag = FALSE;
	Std_ReturnType  ret = E_OK;
	Dcm_MsgLenType  MsgLen;
	Dem_ReturnSetDTCFilterType         ReturnSetDTCFilter;
	Dem_ReturnGetNextFilteredDTCType   ReturnGetNextFilteredDTC = DEM_FILTERED_OK;
	Dem_ReturnGetNumberOfFilteredDTCType  ReturnGetNumberOfFilteredDTC;

    /**************************************************/
	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X02_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

    /**************************************************/
	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	MsgLen = 2u;	/*keep two bytes to store response SID and echo of sub-function*/
	DtcStatusMask  = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];

	/*get DTCStatusAvailabilityMask*/
	ret = Dem_GetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen]);
	if (E_OK == ret)
	{
		/*if DTCStatusAvailabilityMask = 0,no DTC*/
        if (0u == Dcm_Channel[Offset + MsgLen])
        {
        	MsgLen = MsgLen + 1u;
        }
		/*if DTCStatusAvailabilityMask !=0,have DTC*/
        else
		{
        	MsgLen = MsgLen + 1u;
			/*set DTC filter*/
			ReturnSetDTCFilter = Dem_SetDTCFilter(	DtcStatusMask,
												    DEM_DTC_KIND_ALL_DTCS,
												    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
												    DEM_FILTER_WITH_SEVERITY_NO,
												    DEM_SEVERITY_NO_SEVERITY,
												    DEM_FILTER_FOR_FDC_NO	);
			if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
			{
				ReturnGetNumberOfFilteredDTC = Dem_GetNumberOfFilteredDTC(&DtcCount);
				switch(ReturnGetNumberOfFilteredDTC)
				{
					case DEM_NUMBER_OK:
						 Flag = FALSE;
						 for(Index=0; (Index<DtcCount)&&(FALSE==Flag); Index++)
						 {
							ReturnGetNextFilteredDTC = Dem_GetNextFilteredDTC(&Dtc,&DtcStatus);
							switch(ReturnGetNextFilteredDTC)
							{
								case  DEM_FILTERED_OK:
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = DtcStatus;
									  MsgLen  = MsgLen + 1u;
									  break;
								case  DEM_FILTERED_NO_MATCHING_DTC:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
								case  DEM_FILTERED_WRONG_DTCKIND:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
								case  DEM_FILTERED_PENDING:
									  PendingFlag = TRUE;
									  Flag = TRUE;
									  break;
								default:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
							}
						}
						break;
					case DEM_NUMBER_FAILED:
						NotAllowedFlag = TRUE;
						break;
					case DEM_NUMBER_PENDING:
						PendingFlag = TRUE;
						break;
					default:
						NotAllowedFlag = TRUE;
						break;
				}
			}
			else
			{
				/*fail to set DTC Filter*/
				NotAllowedFlag  = TRUE;
			}
		}
	}
	else
	{
		NotAllowedFlag = TRUE;
	}

	/************************************************/
	if (TRUE == NotAllowedFlag)
	{
		/*if fail to get any request information,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (TRUE == PendingFlag)
	{
		/*if some request info is not available yet,enter pending and wait for the next cycle*/
		return E_PENDING;
	}
	else
	{
		/*assemble positive response*/
		Dcm_Channel[Offset] = 0x59;
		Dcm_Channel[Offset + 1u] = 0x02;
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x03(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint16    Offset;
	uint16    Index;
	uint16    DtcCount;
	uint8     SnapshotRecNum;
	uint32    Dtc;
	boolean   NotAllowedFlag = FALSE;
	boolean   PendingFlag = FALSE;
	boolean   Flag = FALSE;
	Std_ReturnType  ret = E_OK;
	Dcm_MsgLenType  MsgLen;
	Dem_ReturnSetDTCFilterType         ReturnSetDTCFilter;
	Dem_ReturnGetNextFilteredDTCType   ReturnGetNextFilteredRecord = DEM_FILTERED_OK;

  /**************************************************/
	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X03_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

  /**************************************************/
	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset      = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	MsgLen = 2u;	/*keep two bytes to store response SID and echo of sub-function*/

	if(E_OK == Dem_DisableDTCRecordUpdate()) 
	{
	  /* set DTC filter for record */
  	ReturnSetDTCFilter = Dem_SetDTCFilterForRecords(&DtcCount);
  	
  	if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
  	{
  		 Flag = FALSE;
  		 for(Index=0; (Index<DtcCount)&&(FALSE==Flag); Index++)
  		 {
  			ReturnGetNextFilteredRecord = Dem_GetNextFilteredRecord(&Dtc,&SnapshotRecNum);
  			switch(ReturnGetNextFilteredRecord)
  			{
  				case  DEM_FILTERED_OK:
  					  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16);
  					  MsgLen  = MsgLen + 1u;
  					  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8);
  					  MsgLen  = MsgLen + 1u;
  					  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc);
  					  MsgLen  = MsgLen + 1u;
  					  Dcm_Channel[Offset + MsgLen] = SnapshotRecNum;
  					  MsgLen  = MsgLen + 1u;
  					  break;
  				case  DEM_FILTERED_NO_MATCHING_DTC:
  					  NotAllowedFlag = TRUE;
  					  Flag = TRUE;
  					  break;
  				case  DEM_FILTERED_WRONG_DTCKIND:
  					  NotAllowedFlag = TRUE;
  					  Flag = TRUE;
  					  break;
  				case  DEM_FILTERED_PENDING:
  					  PendingFlag = TRUE;
  					  Flag = TRUE;
  					  break;
  				default:
  					  NotAllowedFlag = TRUE;
  					  Flag = TRUE;
  					  break;
  			}
  		}

  	}
  	else
  	{
  		/*fail to set DTC Filter*/
  		NotAllowedFlag  = TRUE;
  	}
		if(E_OK != Dem_EnableDTCRecordUpdate())
		{
			PendingFlag = TRUE;
		}
	} 
	else 
	{
	   PendingFlag = TRUE;
	}
	/************************************************/
	if (TRUE == NotAllowedFlag)
	{
		/*if fail to get any request information,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (TRUE == PendingFlag)
	{
		/*if some request info is not available yet,enter pending and wait for the next cycle*/
		return E_PENDING;
	}
	else
	{
		/*assemble positive response*/
		Dcm_Channel[Offset] = 0x59;
		Dcm_Channel[Offset + 1u] = 0x03;
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x04(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint8     CurBufferSize;
	uint8     DtcStatus;
	uint8     SnapshotRecord;
	uint8     Index;
	uint16    ChannelSize;
	uint16    Offset;
	uint32    Dtc;
	boolean   NotAllowedFlag = FALSE;
	boolean   PendingFlag = FALSE;
	Dcm_MsgLenType  MsgLen = 0;
	Dem_ReturnGetFreezeFrameDataByDTCType retGetFreezeFrameDataByDTC;
	Dem_ReturnGetStatusOfDTCType   ReturnGetStatusOfDTC;

    /**************************************************/
	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X04_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

    /**************************************************/
	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	ChannelSize = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].Dcm_DslBufferSize;
	Offset      = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	/*get DTC and reoord number from request message*/
	Dtc = (((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2])<< 16)
	 	 |(((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3])<< 8)
	 	 |(((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]));
	SnapshotRecord = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[5];

	/*read-out the correspoding DtcStatus of required DTC*/
	ReturnGetStatusOfDTC = Dem_GetStatusOfDTC(Dtc,
											  DEM_DTC_KIND_ALL_DTCS,
											  DEM_DTC_ORIGIN_PRIMARY_MEMORY,
											  &DtcStatus);
	if(DEM_STATUS_OK == ReturnGetStatusOfDTC)
	{		
		MsgLen = 2u;
		Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc>>16);
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc>>8);
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc);
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = DtcStatus;
		MsgLen = MsgLen + 1u;
		if(0xFF == SnapshotRecord)
		{
			/*read-out all required snapshot datum*/
			if(E_OK == Dem_DisableDTCRecordUpdate())
			{
				for(Index = 1u; Index<0xEF; Index++)
				{																	
					/*reserve 1byte to store record number*/	
					CurBufferSize = (uint8)(ChannelSize - MsgLen - 1);
					retGetFreezeFrameDataByDTC = Dem_GetFreezeFrameDataByDTC(Dtc,
					                     DEM_DTC_KIND_ALL_DTCS,
					                     DEM_DTC_ORIGIN_PRIMARY_MEMORY,
					                     Index,
					                     &Dcm_Channel[Offset + MsgLen + 1],
					                     &CurBufferSize);
					if(DEM_GET_FFDATABYDTC_OK == retGetFreezeFrameDataByDTC)
					{
						Dcm_Channel[Offset + MsgLen] = Index;
						MsgLen = MsgLen + CurBufferSize + 1;
					}
					else if(DEM_GET_FFDATABYDTC_PENDING == retGetFreezeFrameDataByDTC)
					{
						PendingFlag = TRUE;
						break;
					}
					else if(DEM_GET_FFDATABYDTC_WRONG_DTC == retGetFreezeFrameDataByDTC)
					{
						NotAllowedFlag = TRUE;	
					}
					else
					{
						/*nothing to do,Keep the format consistent*/
					}
				}
				if(E_OK != Dem_EnableDTCRecordUpdate())
				{
					PendingFlag = TRUE;
				}
			}
			else
			{
				PendingFlag = TRUE;
			}
		}
		else
		{
			/*read-out the correspoding snapshot by SnapshotRecord*/
			if(E_OK == Dem_DisableDTCRecordUpdate())
			{
				/*reserve 1byte to store record number*/																	
				CurBufferSize = (uint8)(ChannelSize - MsgLen - 1);
				retGetFreezeFrameDataByDTC = Dem_GetFreezeFrameDataByDTC(Dtc,
				                     DEM_DTC_KIND_ALL_DTCS,
				                     DEM_DTC_ORIGIN_PRIMARY_MEMORY,
				                     SnapshotRecord,
				                     &Dcm_Channel[Offset + MsgLen+1],
				                     &CurBufferSize);
				if(DEM_GET_FFDATABYDTC_OK == retGetFreezeFrameDataByDTC)
				{
					Dcm_Channel[Offset + MsgLen] = SnapshotRecord;
					MsgLen = MsgLen + CurBufferSize + 1u;
				}
				else if(DEM_GET_FFDATABYDTC_PENDING == retGetFreezeFrameDataByDTC)
				{
					PendingFlag = TRUE;
				}
				else if(DEM_GET_FFDATABYDTC_WRONG_DTC == retGetFreezeFrameDataByDTC)
				{
					NotAllowedFlag = TRUE;	
				}
				else
				{
					/*nothing to do,Keep the format consistent*/
				}
																
				if(E_OK != Dem_EnableDTCRecordUpdate())
				{
					PendingFlag = TRUE;
				}
			}
			else
			{
				PendingFlag = TRUE;
			}			
		}
	}
	else
	{
		NotAllowedFlag = TRUE;
	}

	if(TRUE == NotAllowedFlag)
	{
		/*if fail to get any request information,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if(TRUE == PendingFlag)
	{
		/*if some request info is not available yet,enter pending and wait for the next cycle*/
		return E_PENDING;
	}
	else
	{
		/*assemble positive response*/
		Dcm_Channel[Offset] = 0x59;
		Dcm_Channel[Offset + 1u] = 0x04;
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/******************************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x06(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint8     CurChannelSize;
	uint8     Index;
	uint8     DtcStatus;
	uint8     DtcExtendedDataRecordNumber;
	uint16    ChannelSize;
	uint16    Offset;
	uint32    Dtc;
	boolean   NotAllowedFlag = FALSE;
	boolean   PendingFlag = FALSE;
	Dcm_MsgLenType  MsgLen;
	Dem_ReturnGetStatusOfDTCType   ReturnGetStatusOfDTC;
	Dem_ReturnGetExtendedDataRecordByDTCType  ReturnGetExtendedDataRecord;

	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X06_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	ChannelSize = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].Dcm_DslBufferSize;
	Offset      = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	/*get DTC and record number from request message*/
	Dtc = (((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2]) << 16)
		 |(((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3]) << 8)
		 |(((uint32)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]));
	DtcExtendedDataRecordNumber = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[5];
	/*read-out the corresponding DtcStatus of required DTC*/
	ReturnGetStatusOfDTC = Dem_GetStatusOfDTC(	Dtc,
	                                      		DEM_DTC_KIND_ALL_DTCS,
	                                      		DEM_DTC_ORIGIN_PRIMARY_MEMORY,
	                                      		&DtcStatus	);
	if (DEM_STATUS_OK == ReturnGetStatusOfDTC)
	{
		MsgLen = 2u;
		Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16);
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8);
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = (uint8) Dtc;
		MsgLen = MsgLen + 1u;
		Dcm_Channel[Offset + MsgLen] = DtcStatus;
		MsgLen = MsgLen + 1u;
		/*read-out the corresponding extended data of DtcExtendedDataRecordNumber*/
		/*disable DTC record update*/
		if (E_OK == Dem_DisableDTCRecordUpdate())
		{
			if ((0xFE == DtcExtendedDataRecordNumber) || (0xFF == DtcExtendedDataRecordNumber))
			{
				/*DTC + (equal to 0xFF or 0xFE)*/
				/*read-out the corresponding extended data of required DTC*/
				if (0xFE == DtcExtendedDataRecordNumber)
				{
					/*read-out OBD relative extended data of required DTC*/
					Index = 0x90;
				}
				else
				{
					Index = 0x01;
				}

				for (; Index <= 0xEF; Index++)
				{
					/*calculate the remained size of Tx buffer,keep one byte to store record number*/
					CurChannelSize = (uint8)(ChannelSize - MsgLen - 1u);
					ReturnGetExtendedDataRecord = Dem_GetExtendedDataRecordByDTC(
													Dtc,
													DEM_DTC_KIND_ALL_DTCS,
													DEM_DTC_ORIGIN_PRIMARY_MEMORY,
													Index,
													&Dcm_Channel[Offset + MsgLen + 1u],
													&CurChannelSize);
					if (DEM_RECORD_OK == ReturnGetExtendedDataRecord)
					{
						/*save DTCExtendDataRecordNumber to Tx buffer*/
						Dcm_Channel[Offset + MsgLen] = Index;
						MsgLen = MsgLen + 1u;
						/*successfully store extended data,increase MsgLen*/
						MsgLen = MsgLen + CurChannelSize;
					}
					else if (DEM_RECORD_PENDING == ReturnGetExtendedDataRecord)
					{
						PendingFlag = TRUE;
						break;
					}
					else if(DEM_RECORD_WRONG_DTC == ReturnGetExtendedDataRecord)
					{
						NotAllowedFlag = TRUE;	
					}
					else if(DEM_RECORD_WRONG_NUMBER == ReturnGetExtendedDataRecord)
					{
						NotAllowedFlag = TRUE;	
					}
					else
					{
						/*nothing to do,just keep the format consistent*/
					}
				}
			}
			/*DTC + (not 0xFF nor 0xFE)*/
			else
			{
				/*calculate the remained size of Tx buffer,keep one byte to store record number*/
				CurChannelSize = (uint8)(ChannelSize - MsgLen - 1u);
				ReturnGetExtendedDataRecord = Dem_GetExtendedDataRecordByDTC(
											Dtc,
											DEM_DTC_KIND_ALL_DTCS,
											DEM_DTC_ORIGIN_PRIMARY_MEMORY,
											DtcExtendedDataRecordNumber,
											&Dcm_Channel[Offset + MsgLen + 1u],
											&CurChannelSize);
				if (DEM_RECORD_OK == ReturnGetExtendedDataRecord)
				{
					/*store extended data record number*/
					Dcm_Channel[Offset + MsgLen] = DtcExtendedDataRecordNumber;
					MsgLen = MsgLen + 1u;
					/*successfully store extended data,increase MsgLen*/
					MsgLen = MsgLen + CurChannelSize;
				}
				else if (DEM_RECORD_PENDING == ReturnGetExtendedDataRecord)
				{
					PendingFlag = TRUE;
				}
				else if(DEM_RECORD_WRONG_DTC == ReturnGetExtendedDataRecord)
				{
					NotAllowedFlag = TRUE;	
				}
				else if(DEM_RECORD_WRONG_NUMBER == ReturnGetExtendedDataRecord)
				{
					NotAllowedFlag = TRUE;	
				}
				else
				{
					/*nothing to do,just keep the format consistent*/
				}
			}
			/*enable DTC record update*/
			if (E_NOT_OK == Dem_EnableDTCRecordUpdate())
			{
				PendingFlag = TRUE;
			}

		}
		else
		{
			PendingFlag = TRUE;
		}
	}
	else
	{
		NotAllowedFlag = TRUE;
	}

	if(TRUE == NotAllowedFlag)
	{
		/*if fail to get any request information,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	else if (TRUE == PendingFlag)
	{
		/*if some request info is not available yet,enter pending and wait for the next cycle*/
		return E_PENDING;
	}
	else
	{
		/*assemble positive response*/
		Dcm_Channel[Offset] = 0x59;
		Dcm_Channel[Offset + 1u] = 0x06;
		SchM_Enter_Dcm(Dcm_MsgCtrl);
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
		Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
		SchM_Exit_Dcm(Dcm_MsgCtrl);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_OK;
	}
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/*******************************************/
#if(STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)ReadDtcInfoSub_0x0A(uint8 ProtocolCtrlId, uint8 MsgCtrlId)
{
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint8     DtcStatus;
	uint16    Offset;
	uint16    DtcCount;
    uint16    Index;
	uint32    Dtc;
	boolean   NotAllowedFlag = FALSE;
	boolean   PendingFlag    = FALSE;
	boolean   Flag 			 = FALSE;
	Std_ReturnType  ret      = E_OK;
	Dcm_MsgLenType  MsgLen   = 2u;/*store response SID and echo of sub-function*/
	Dem_ReturnSetDTCFilterType    ReturnSetDTCFilter;
	Dem_ReturnGetNextFilteredDTCType      ReturnGetNextFilteredDTC;
	Dem_ReturnGetNumberOfFilteredDTCType  ReturnGetNumberOfFilteredDTC;

	/*check the massage length*/
	if (DCM_UDS0X19_SUBFUNC0X0A_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*find Tx buffer and offset*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset      = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	/*get DTCStatusAvailabilityMask*/
	ret = Dem_GetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen]);
	if (E_OK == ret)
	{
        if(0u == Dcm_Channel[Offset + MsgLen])
        {
        	MsgLen = MsgLen + 1u;
        }
        else
        {
    		/*one byte for the storage of DTC status availability mask*/
    		MsgLen = MsgLen + 1u;
    		/*set DTC filter*/
    		ReturnSetDTCFilter = Dem_SetDTCFilter(ALL_SUPPORTED_DTC,
    						   	   	   	   	   	  DEM_DTC_KIND_ALL_DTCS,
    						   	   	   	   	   	  DEM_DTC_ORIGIN_PRIMARY_MEMORY,
    						   	   	   	   	   	  DEM_FILTER_WITH_SEVERITY_NO,
    						   	   	   	   	   	  DEM_SEVERITY_NO_SEVERITY,
    						   	   	   	   	   	  DEM_FILTER_FOR_FDC_NO);
			if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
			{
				ReturnGetNumberOfFilteredDTC = Dem_GetNumberOfFilteredDTC(&DtcCount);
				switch(ReturnGetNumberOfFilteredDTC)
				{
					case DEM_NUMBER_OK:
						 Flag = FALSE;
						 for(Index=0; (Index<DtcCount)&&(FALSE==Flag); Index++)
						 {
							ReturnGetNextFilteredDTC = Dem_GetNextFilteredDTC(&Dtc,&DtcStatus);
							switch(ReturnGetNextFilteredDTC)
							{
								case  DEM_FILTERED_OK:
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc);
									  MsgLen  = MsgLen + 1u;
									  Dcm_Channel[Offset + MsgLen] = DtcStatus;
									  MsgLen  = MsgLen + 1u;
									  break;
								case  DEM_FILTERED_NO_MATCHING_DTC:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
								case  DEM_FILTERED_WRONG_DTCKIND:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
								case  DEM_FILTERED_PENDING:
									  PendingFlag = TRUE;
									  Flag = TRUE;
									  break;
								default:
									  NotAllowedFlag = TRUE;
									  Flag = TRUE;
									  break;
							}
						}
						break;
					case DEM_NUMBER_FAILED:
						NotAllowedFlag = TRUE;
						break;
					case DEM_NUMBER_PENDING:
						PendingFlag = TRUE;
						break;
					default:
						NotAllowedFlag = TRUE;
						break;
				}
			}
        }
	}
	/************************************************/
    if (TRUE == NotAllowedFlag)
    {
    	/*if fail to get any request information,send NRC 0x22*/
    	(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
    	DsdInternal_ProcessingDone(ProtocolCtrlId);
    	return E_NOT_OK;
    }
    if (TRUE == PendingFlag)
    {
    	/*if some request info is not available yet,enter pending and wait for the next cycle*/
    	return E_PENDING;
    }
	/*assemble positive response*/
	Dcm_Channel[Offset] = 0x59;
	Dcm_Channel[Offset + 1u] = 0x0A;
	SchM_Enter_Dcm(Dcm_MsgCtrl);
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  E_OK;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x19SubfunctionAnalyse(	uint8 ProtocolCtrlId,
																	uint8 MsgCtrlId,
																	uint8 Subfunction)
{
	Std_ReturnType  ret = E_OK;

	/*check sub-function entrance*/
	switch (Subfunction)
	{
	#if(STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED)
		case DCM_REPORTNUMBEROFDTCBYSTATUSMASK:
			/*Sub-function:0x01*/
			ret = ReadDtcInfoSub_0x01(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif

	#if(STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED)
		case DCM_REPORTDTCBYSTATUSMASK:
			/*Sub-function:0x02*/
			ret = ReadDtcInfoSub_0x02(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED)
		case DCM_REPORTREPORTDTCSNAPSHOTIDENTIFICATION:
			/*Sub-function:0x03*/
			ret = ReadDtcInfoSub_0x03(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif
			
	#if(STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED)
		case DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER:
			/*Sub-function:0x04*/
			ret = ReadDtcInfoSub_0x04(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED)
		case DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER:
			/*Sub-function:0x06*/
			ret = ReadDtcInfoSub_0x06(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif
	
	#if(STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED)
		case DCM_REPORTSUPPORTEDDTC:
			/*Sub-function:0x0A*/
			ret = ReadDtcInfoSub_0x0A(ProtocolCtrlId, MsgCtrlId);
			break;
	#endif
	
		default:
			/*if the sub-function is not supported,reset 'control resource' and send NRC 0x12*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SUBFUNCTIONNOTSUPPORTED);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
	}

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/*****************************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x19(uint8  ProtocolCtrlId)
{
	uint8     MsgCtrlId;
    #if (STD_ON == DCM_DSP_READ_DTC_FUNC_ENABLED)
	uint8     Index;
	uint8     Subfunction;
	uint8     SubFunctionCfgIndex;
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	uint8     RefSecNum;
	#endif
	P2CONST(Dcm_DspReadDTCRowType, AUTOMATIC, DCM_APPL_CONST) pDspReadDTCRow;
	boolean   Flag;
	Std_ReturnType  ret;
    #endif

	/*************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_READ_DTC_INFORMATION);
	if (E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_READ_DTC_INFORMATION);
	if (E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
	/*if the required protocol is configuted,get the index of runtime datum*/
	MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X19_REQ_DATA_MINLENGTH)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

    /**************************************************/
	/*check whether the required sub-function is configured*/
	Subfunction = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
	Flag  =  FALSE;
	for (Index = 0; (Index < (Dcm_DspCfg.pDcmDspReadDTC->DcmDspReadDTCRowNum)) && (FALSE == Flag); Index++)
	{
		if (  (Subfunction == (Dcm_DspCfg.pDcmDspReadDTC->pDcmDspReadDTCRow)[Index].DcmDspDTCInfoSubFunc)
		    &&(TRUE == (Dcm_DspCfg.pDcmDspReadDTC->pDcmDspReadDTCRow)[Index].DcmDspDTCInfoSubFuncSupp))
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if Sub-function is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	
    /**************************************************/
	/*get sub-function configuration index*/
	SubFunctionCfgIndex = (uint8)(Index - 1);
	pDspReadDTCRow   = Dcm_DspCfg.pDcmDspReadDTC->pDcmDspReadDTCRow;

	/*check whether the sub-function is supported in current security level */
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
	RefSecNum = pDspReadDTCRow[SubFunctionCfgIndex].DcmDspDTCInfoSecLevelRefNum;
	Flag  =  FALSE;
	for(Index=0;(Index<RefSecNum)&&(FALSE==Flag);Index++)
	{
		if (Dcm_MkCtrl.Dcm_ActiveSec == pDspReadDTCRow[SubFunctionCfgIndex].pDcmDspDTCSecurityLevelRow[Index])
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if Security Access denied,send NRC 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

    /**************************************************/
	/*check sub-function entrance*/
	ret = Dcm_Uds0x19SubfunctionAnalyse(ProtocolCtrlId, MsgCtrlId, Subfunction);

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	     UDS:ReadDataByIdentifier (22 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X22_ENABLED)
/************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidNumbercheck(uint8 numberOfDid)
{
	Std_ReturnType ret = E_OK;

	if (0u != (Dcm_DspCfg.DcmDspMaxDidToRead))
	{
		/*check the limitation of number of required DID*/
		if (numberOfDid > (Dcm_DspCfg.DcmDspMaxDidToRead))
		{
			ret = E_NOT_OK;
		}
	}
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidCheck(uint16 receiveDid,
		                                                         P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT)pDidCfgIndex)
{
    uint8     Index;
	boolean   Flag = FALSE;
    Std_ReturnType  ret = E_NOT_OK;

	for (Index = 0;(Index < Dcm_DspCfg.DcmDspDidNum) && (FALSE == Flag); Index++)
	{
		if (receiveDid == Dcm_DspCfg.pDcmDspDid[Index].DcmDspDidId)
		{
			Flag = TRUE;
			(*pDidCfgIndex) = Index;
			ret = E_OK;
		}
	}
    return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidSessionCheck(uint8  DidInfoCfgIndex,
																		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
    uint8   Index = 0;
	boolean Flag = FALSE;
	P2CONST(Dcm_DspDidReadType, AUTOMATIC, DCM_APPL_CONST) pDspDidRead;
	Std_ReturnType   ret = E_OK;

	/***************************************
	 * pDcmDspDidRead is NULL_PTR
	 ***************************************/
	pDspDidRead  = Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidAccess->pDcmDspDidRead;
	if (NULL_PTR == pDspDidRead)
	{
		/*Dcm433:if the DcmDspDidRead of required Did is not configured,send NRC 0x31*/
        (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
	}
	else
	{
		for (Index = 0; (Index < (pDspDidRead->DcmDspDidReadSessionRefNum)) && (FALSE == Flag); Index++)
		{
			if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidRead->pDcmDspDidReadSessionRow[Index])
			{
				Flag = TRUE;
			}
		}
		if (FALSE == Flag)
		{
			/*Dcm434:the reading processing is not supported in current session,send NRC 0x7F*/
			(*pNrc) = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
			ret = E_NOT_OK;
		}
	}
    return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidSecurityCheck(	uint8  DidInfoCfgIndex,
																			P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
    uint8    Index = 0;
    boolean  Flag = FALSE;
	P2CONST(Dcm_DspDidReadType, AUTOMATIC, DCM_APPL_CONST) pDspDidRead;
	Std_ReturnType   ret = E_OK;

	/***************************************
	 * pDcmDspDidRead is NULL_PTR
	 ***************************************/
	pDspDidRead  = Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidAccess->pDcmDspDidRead;
	if (NULL_PTR == pDspDidRead)
	{
		/*Dcm433:if the DcmDspDidRead of required Did is not configured,send NRC 0x31*/
        (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
	}
	else
	{
		for (Index = 0; (Index < (pDspDidRead->DcmDspDidReadSecurityLevelRefNum)) && (FALSE == Flag); Index++)
		{
			if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidRead->pDcmDspDidReadSecurityLevelRow[Index])
			{
				Flag = TRUE;
			}
		}
		if (FALSE == Flag)
		{
			/*Dcm435:the reading processing is not supported in current security level,send NRC 0x33*/
           (*pNrc) = DCM_E_SECURITYACCESSDENIED;
			ret = E_NOT_OK;
		}
	}
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_DidConditionCheck(	uint8  ProtocolCtrlId,
																			uint8  DidCfgIndex,
																			P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
	Std_ReturnType ret = E_OK;


    if(NULL_PTR != Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidConditionCheckReadFnc)
    {
    	  ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidConditionCheckReadFnc))(pNrc);
    	  if(E_NOT_OK == ret)
    	  {
			 (void)DsdInternal_SetNrc(ProtocolCtrlId,*pNrc);
			  DsdInternal_ProcessingDone(ProtocolCtrlId);
    	  }
    }
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)DspInternalUDS0x22_ReadDidDataLength( uint8 DidCfgIndex,
																			uint8 DidInfoCfgIndex,
																			P2VAR(uint16, AUTOMATIC, AUTOMATIC)pDidSize,
															                P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
	Std_ReturnType  ret ;

    /*check the DID length*/
	if(TRUE == Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].DcmDspDidFixedLength)
	{
		/*DidDataSize is fixed*/
		(*pDidSize) = Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidSize;
		ret = E_OK;
	}
	else
	{
		/*DidDataSize is dynamic*/
		if (NULL_PTR == Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataLengthFnc)
		{
			/*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
            (*pNrc) = DCM_E_CONDITIONSNOTCORRECT;
			ret = E_NOT_OK;
		}
		else
		{
			ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataLengthFnc))(pDidSize);
		}
	}
    return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x22ServiceConditonCheck(	uint8 ProtocolCtrlId,
																		P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType ret = E_OK;


	/*************************************************/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_READ_DATA_BY_IDENTIFIER);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_READ_DATA_BY_IDENTIFIER);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId  = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if ((0u == (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen & 1u))\
		||(Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X22_REQ_DATA_MINLENGTH))
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsAssembleResponse(	uint8    ProtocolCtrlId,
																uint8    DidCfgIndex,
																P2VAR(uint16, AUTOMATIC, AUTOMATIC)ResOffset,
																uint16   RecDid,
																uint16   DidSize	)
{
	Std_ReturnType ret = E_OK;

	/*set the response message*/
	/**Dcm437**/
	Dcm_Channel[*ResOffset] = (uint8)((RecDid) >> 8); /*echo of DID MSB*/
	*ResOffset = *ResOffset + 1;
	Dcm_Channel[*ResOffset] = (uint8)(RecDid);
	*ResOffset = *ResOffset + 1;
	if (NULL_PTR == Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataFnc)
	{
		/*DcmDspDidReadDataFnc is NULL,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataFnc))(&Dcm_Channel[*ResOffset]);
	*ResOffset = *ResOffset + DidSize;
	if(E_NOT_OK == ret)
	{
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT); /*NRC=0x22*/
		DsdInternal_ProcessingDone(ProtocolCtrlId);
	}
	return ret;

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x22(uint8  ProtocolCtrlId)
{
	uint8     MsgCtrlId;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
	uint8     DidNum;
	uint8     Index0;
	uint8     DidCfgIndex;
	uint8     DidInfoCfgIndex;
	uint8     ReqOffset;
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint16    RecDid;
	uint16    DidSize;
	uint16    ResOffset;
	uint16    Offset;
	Dcm_NegativeResponseCodeType   ErrorCode;
#endif
	Std_ReturnType ret = E_OK;


	ret = Dcm_Uds0x22ServiceConditonCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}

#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
	/*NRC 0x31:request out of range*/
	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return E_NOT_OK;

#else

    /**************************************************/
	/*calculate the number of required DID*/
	DidNum = (uint8)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen >> 1u);
	ret = DspInternalUDS0x22_DidNumbercheck(DidNum);
    if(E_NOT_OK == ret)
    {   
        /*NRC = 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);  
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }

    /************************************************/
	/*get the runtime datum index*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	Dcm_Channel[Offset] = 0x62; /*response SID*/
	ResOffset  = (uint16)(Offset + 1u);
	ReqOffset  = (uint8)1;

	for (Index0 = 0; Index0 < DidNum; Index0++)
	{
		RecDid = (((uint16)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[ReqOffset]) << 8) \
			    |((uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[ReqOffset + 1u]));
		        		 
		ReqOffset = (uint8)(ReqOffset + 2);
        /*************************************/
		/*find the required DID in configuration*/
		ret = DspInternalUDS0x22_DidCheck(RecDid,&DidCfgIndex);
		if (E_NOT_OK == ret)
		{
			/*Dcm438:if the required DID is not found in DcmDspDid,send NRC 0x31*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			return E_NOT_OK;
		}
        /**************************************/
		/*if the required DID is found,check whether the pDcmDspDidRead container is configured*/
		DidInfoCfgIndex = Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;

	    /**************************************/
		/*check the current session*/
		#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
        ret = DspInternalUDS0x22_DidSessionCheck(DidInfoCfgIndex,&ErrorCode);
        if(E_NOT_OK == ret)
        {
			(void)DsdInternal_SetNrc(ProtocolCtrlId,ErrorCode);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			return  E_NOT_OK;
        }
		#endif

	    /***************************************/
		/*check the current security level*/
		#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
        ret = DspInternalUDS0x22_DidSecurityCheck(DidInfoCfgIndex,&ErrorCode);
        if(E_NOT_OK == ret)
        {
			(void)DsdInternal_SetNrc(ProtocolCtrlId,ErrorCode);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			return  E_NOT_OK;
        }
		#endif

	    /***************************************/
        /*check the current DID condition is OK*/
        ret = DspInternalUDS0x22_DidConditionCheck(ProtocolCtrlId, DidCfgIndex,&ErrorCode);
        if(E_OK != ret)
        {
        	return ret;
        }

	    /***************************************/
        /*Read the current length of the data in DID*/
        ret = DspInternalUDS0x22_ReadDidDataLength(	DidCfgIndex,
        		                                   	DidInfoCfgIndex,
                                                   	&DidSize,
        		                                   	&ErrorCode	);

       if(E_NOT_OK == ret)
       {
		  (void)DsdInternal_SetNrc(ProtocolCtrlId,ErrorCode);
		  DsdInternal_ProcessingDone(ProtocolCtrlId);
		  return  E_NOT_OK;
       }
		/*set the response message*/
		/**Dcm437**/
       ret = Dcm_UdsAssembleResponse(	ProtocolCtrlId, 
                                 		DidCfgIndex, 
                                 		&ResOffset, 
                                 		RecDid, 
                                 		DidSize	);
       if(E_OK != ret)
       {
    	   return ret;
       }

	}

	SchM_Enter_Dcm(Dcm_MsgCtrl);
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = ResOffset - Offset;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = ResOffset - Offset;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  E_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	     UDS:WriteDataByIdentifier (2E hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X2E_ENABLED)
/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidCheck(	uint16 RecDid,
		                                    		P2VAR(uint8, AUTOMATIC, AUTOMATIC)pDidCfgIndex,
		                                    		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
	uint8     Index;
	boolean   Flag = FALSE;
	Std_ReturnType  ret = E_NOT_OK;

	/*find the corresponding DID in configuration*/
	for (Index = 0; (Index < Dcm_DspCfg.DcmDspDidNum) && (FALSE == Flag); Index++)
	{
		if (RecDid == Dcm_DspCfg.pDcmDspDid[Index].DcmDspDidId)
		{
		 	Flag = TRUE;
			(*pDidCfgIndex) = Index;
			ret = E_OK;
		}
	}
	if (FALSE == Flag)
	{
		/*if not found,send NRC 0x31*/
		(*pNrc) = DCM_E_REQUESTOUTOFRANGE;
		ret = E_NOT_OK;
	}
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidSesCheck(uint8 DidInfoCfgIndex,
		                                             P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc)
{
	uint8    Index;
	boolean  Flag =FALSE;
	Std_ReturnType  ret = E_NOT_OK;
	P2CONST(Dcm_DspDidWriteType, AUTOMATIC, DCM_APPL_CONST) pDspDidWrite;

	/************************/
	pDspDidWrite = Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidAccess->pDcmDspDidWrite;
	if ((NULL_PTR == pDspDidWrite))
	{
		/*Judge DID the configuration items pDcmDspDidWrite whether NULL_PTR,
		 *if not supported,send Nrc 0x31*/
        (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
         ret = E_NOT_OK;
	}
	else
	{
		for(Index = 0; (Index < (pDspDidWrite->DcmDspDidWriteSessionRefNum)) && (FALSE == Flag); Index++)
		{
			if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidWrite->pDcmDspDidWriteSessionRow[Index])
			{
				Flag = TRUE;
				ret  = E_OK;
			}
		}
		if (FALSE == Flag)
		{
			/*if the current session is not supported,send NRC 0x7F*/
			(*pNrc) = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
			ret = E_NOT_OK;
		}
	}
    return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidSecCheck(uint8 DidInfoCfgIndex,
		                                              P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
	uint8    Index;
	boolean  Flag =FALSE;
	Std_ReturnType  ret = E_NOT_OK;
	P2CONST(Dcm_DspDidWriteType, AUTOMATIC, DCM_APPL_CONST) pDspDidWrite;

	/************************/
	pDspDidWrite = Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidAccess->pDcmDspDidWrite;
	if ((NULL_PTR == pDspDidWrite))
	{
		/*Judge DID the configuration items pDcmDspDidWrite whether NULL_PTR,
		 *if not supported,send Nrc 0x31*/
        (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
		 ret = E_NOT_OK;
	}
	else
	{
		for (Index = 0; (Index < (pDspDidWrite->DcmDspDidWriteSecurityLevelRefNum)) && (FALSE == Flag); Index++)
		{
			#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
			if (gFactory_ActiveFlag||(Dcm_MkCtrl.Dcm_ActiveSec == pDspDidWrite->pDcmDspDidWriteSecurityLevelRow[Index]))/*yb*/
			#else
			if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidWrite->pDcmDspDidWriteSecurityLevelRow[Index])
			#endif
			{
				Flag = TRUE;
				ret = E_OK;
			}
		}
		if (FALSE == Flag)
		{
			/*if the current security is not supported,send NRC 0x33*/
			(*pNrc) = DCM_E_SECURITYACCESSDENIED;
			ret = E_NOT_OK;
		}
	}
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidWriteConditionCheck(	uint8 ProtocolCtrlId,
																	uint8 DidCfgIndex,
																	P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc)

{
	Std_ReturnType ret = E_OK;
    
    if(NULL_PTR != Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidConditionCheckWriteFnc)
    {
    	ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidConditionCheckWriteFnc))(pNrc);
    	if(E_NOT_OK == ret)
    	{
			(void)DsdInternal_SetNrc(ProtocolCtrlId, *pNrc);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
    	}
    }
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/**************************************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidWriteDataLength(	uint8   ProtocolCtrlId,
																uint8   DidCfgIndex,
																uint8   DidInfoCfgIndex,
																P2VAR(uint16, AUTOMATIC, AUTOMATIC)pDidSize,
																P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)pNrc)
{
	Std_ReturnType  ret;

	if (TRUE == Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].DcmDspDidFixedLength)
	{
		/*DidDataSize is fixed*/
		(*pDidSize) = Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidSize;
		return E_OK;
	}
	
	/*DidDataSize is dynamic*/
	if (NULL_PTR == Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataLengthFnc)
	{
		/*if DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
        return E_NOT_OK;
	}
	
	ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidReadDataLengthFnc))(pDidSize);
    
    switch(ret)
    {
    	case E_OK:
    	     break;
    	case E_PENDING:
    	     break;
    	case E_NOT_OK:
			 (void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
			 DsdInternal_ProcessingDone(ProtocolCtrlId);
    	     break;
       default:
    	     break;
    }    
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC Std_ReturnType  DspInternalUDS0x2E_DidWrite(	uint8 ProtocolCtrlId,
													uint8 MsgCtrlId,
													uint8 DidCfgIndex,
													uint16 DidSize,
													P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc)
{
	Std_ReturnType  ret;


	/*check DcmDspDidWriteFnc*/
	if(NULL_PTR == Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidWriteFnc)
	{
		/*if DcmDspDidWriteFnc is NULL,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return(E_NOT_OK);
	}
	ret = (*(Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidWriteFnc))( &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3],
																	 DidSize,
																	 pNrc);
    switch(ret)
    {
    	case E_OK:
    	     break;
    	case E_PENDING:
    	     break;
    	case E_NOT_OK:
			 (void)DsdInternal_SetNrc(ProtocolCtrlId, *pNrc);
			 DsdInternal_ProcessingDone(ProtocolCtrlId);
    	     break;
    	default:
 			 (void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
			 DsdInternal_ProcessingDone(ProtocolCtrlId);
   	     	 break;
    }
	return(ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/***************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x2EServiceConditionCheck(	uint8 ProtocolCtrlId,
																		P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType ret = E_OK;

	/*************************************************/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_WRITE_DATA_BY_IDENTIFIER);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_WRITE_DATA_BY_IDENTIFIER);
	if (E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED); /*NRC = 0x33*/
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X2E_REQ_DATA_MINLENGTH)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/***************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsWriteDidSessionAndSecurityCheck(uint8   ProtocolCtrlId,
																			uint8   DidInfoCfgIndex,
																			P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode)			
{
	Std_ReturnType ret = E_OK;

	/*check the current session*/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	ret = DspInternalUDS0x2E_DidSesCheck(DidInfoCfgIndex, ErrorCode);
	if(E_NOT_OK == ret)
	{
		/*if the current session is not support the Did,send NRC*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, *ErrorCode);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;

	}
	#endif
    /**************************************************/
	/*check the current security level*/
	#if(STD_ON==DCM_SECURITY_FUNC_ENABLED)
	ret = DspInternalUDS0x2E_DidSecCheck(DidInfoCfgIndex, ErrorCode);
	if(E_NOT_OK == ret)
	{
		/*if the current security is not support the DID,send NRC*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, *ErrorCode);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	#endif

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/***************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x2E(uint8  ProtocolCtrlId)
{
	uint8     MsgCtrlId;
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
	uint8     DidCfgIndex;
	uint8     DidInfoCfgIndex;
	uint8     TxChannelCtrlIndex;
	uint8     TxChannelCfgIndex;
	uint16    RecDid;
	uint16    DidSize;
	uint16    Offset;
	Dcm_NegativeResponseCodeType   ErrorCode;
#endif
	Std_ReturnType ret = E_OK;

	/*************************************************/
	ret = Dcm_Uds0x2EServiceConditionCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}

#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
	/*NRC 0x31:request out of range*/
	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return E_NOT_OK;

#else
    /**************************************************/
	/*get the required DID from request message*/
	RecDid = (((uint16)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1]) << 8)\
		    |((uint16) Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2]);

	/*Determine if the DID is configured*/
	ret = DspInternalUDS0x2E_DidCheck(RecDid,
	                                 &DidCfgIndex,
	                                 &ErrorCode);
	if(E_NOT_OK == ret)
	{
		/*if not found,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,ErrorCode);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	
    /**************************************************/
	/*check whether the Did can be configured*/
	DidInfoCfgIndex = Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;

	/*************************************************/
	if (TRUE == (Dcm_DspCfg.pDcmDspDidInfo[DidInfoCfgIndex].DcmDspDidDynamicallyDefined))
	{
		/*if the Did can be dynamically defined,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}

    /**************************************************/
	ret = Dcm_UdsWriteDidSessionAndSecurityCheck(	ProtocolCtrlId, 
													DidInfoCfgIndex, 
													&ErrorCode	);
	if(E_OK != ret)
	{
		return ret;
	}

    /**************************************************/
	/*check DcmDspDidConditionCheckWriteFnc*/
    ret = DspInternalUDS0x2E_DidWriteConditionCheck(ProtocolCtrlId, 
    												DidCfgIndex, 
    												&ErrorCode);
    if(E_OK != ret)
    {
		return ret;
    }

    /**************************************************/
    /*read the length of DID*/
    ret = DspInternalUDS0x2E_DidWriteDataLength(ProtocolCtrlId,
    											DidCfgIndex,
											    DidInfoCfgIndex,
											    &DidSize,
											    &ErrorCode);
    if(E_OK != ret)
    {
		return ret;
    }

   /**************************************************/
   /*check message length*/
   if( (3u+DidSize )!=Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
   {
		/*if the message length is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
   }
   /**************************************************/
	ret = DspInternalUDS0x2E_DidWrite( ProtocolCtrlId,
									   MsgCtrlId,
									   DidCfgIndex,
		                               DidSize,
   		                               &ErrorCode);
	if(E_OK != ret)
	{
		return ret;
	}

	/*assemble and send positive response*/
	SchM_Enter_Dcm(Dcm_MsgCtrl);
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	Dcm_Channel[Offset] = 0x6E;
	Dcm_Channel[Offset + 1] = (uint8)(RecDid>>8);
	Dcm_Channel[Offset + 2] = (uint8)RecDid;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 3u;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 3u;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
#endif
	return  E_OK;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
	 UDS:InputOutputControlByIdentifier(2F hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X2F_ENABLED)

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsConditionCheck(	uint8 ProtocolCtrlId,
															P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType  Ret = E_OK;

	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	Ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER);
	if (E_NOT_OK == Ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	Ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER);
	if (E_NOT_OK == Ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X2F_REQ_DATA_MINLENGTH)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	return Ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsIODidSessionAndSecutityCheck(uint8 ProtocolCtrlId,
															  			P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl)
{
#if( (STD_ON==DCM_SESSION_FUNC_ENABLED)||(STD_ON == DCM_SECURITY_FUNC_ENABLED) )
	boolean Flag;
	uint8   Index;
#endif
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	uint8   SesRefNum;
#endif
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	uint8   SecRefNum;
#endif
	Std_ReturnType  Ret = E_OK;

    /*****************************************/
	/*check the current session*/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check*/
	Flag = FALSE;
	SesRefNum = pDspDidControl->DcmDspDidControlSessionRefNum;
	for (Index = 0; (Index < SesRefNum) && (FALSE == Flag); Index++)
	{
		if (Dcm_MkCtrl.Dcm_ActiveSes == (pDspDidControl->pDcmDspDidControlSessionRow)[Index])
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if the processing is not supported in current session,send NRC 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
#endif

	/*****************************************/
	/*check the current security level*/
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
	Flag = FALSE;
	SecRefNum = pDspDidControl->DcmDspDidControlSecurityLevelRefNum;
	for (Index = 0; (Index < SecRefNum) && (FALSE == Flag); Index++)
	{
		#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
		if(gFactory_ActiveFlag||(Dcm_MkCtrl.Dcm_ActiveSec == (pDspDidControl->pDcmDspDidControlSecurityLevelRow)[Index]))/*yb*/
		#else
		if(Dcm_MkCtrl.Dcm_ActiveSec == (pDspDidControl->pDcmDspDidControlSecurityLevelRow)[Index])/*yb*/
		#endif
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if the processing is not supported in current security level,send NRC 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
#endif

	return Ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_UdsReturnControlToEcu(	uint8   MsgCtrlId,
														uint8   DidCfgIndex,
														uint16  Offset,
														P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode,
														P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl)
{
	uint8   ControlOptionRecordSize = 0u;
	uint8   ContrlEnableMaskRecordSize = 0u;
	uint8   ControlStatusRecordSize = 0u;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlEnableMaskRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlOptionRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlStatusRecord = NULL_PTR;

	if (NULL_PTR == ((Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidReturnControlToECUFnc))
	{
		/*DcmDspDidReturnControlToECUFnc is NULL,send NRC 0x31*/
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_REQUESTOUTOFRANGE;
		return ControlStatusRecordSize;
	}
	if (NULL_PTR == pDspDidControl->pDcmDspDidReturnControlToEcu)
	{
		/*pDcmDspDidReturnControlToEcu is NULL*/
		(*Ret) = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidReturnControlToECUFnc(	NULL_PTR,
																						NULL_PTR,
																						NULL_PTR,
																						ErrorCode);
		return ControlStatusRecordSize;
	}
	/*check the request message length*/
	ControlOptionRecordSize    = pDspDidControl->pDcmDspDidReturnControlToEcu->DcmDspDidControlOptionRecordSize;
	ContrlEnableMaskRecordSize = pDspDidControl->pDcmDspDidReturnControlToEcu->DcmDspDidControlEnableMaskRecordSize;
	ControlStatusRecordSize    = pDspDidControl->pDcmDspDidReturnControlToEcu->DcmDspDidControlStatusRecordSize;
	if ((4u+ControlOptionRecordSize+ContrlEnableMaskRecordSize)!=Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*if the request message length is not correct,send NRC 0x13*/
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT; /*Nrc=0x13*/
		return ControlStatusRecordSize;
	}
	if (0u != ControlOptionRecordSize)
	{
		pControlOptionRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]);
	}
	if (0u != ContrlEnableMaskRecordSize)
	{
		pControlEnableMaskRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4u + ControlOptionRecordSize]);
	}
	if (0u != ControlStatusRecordSize)
	{
		pControlStatusRecord = &Dcm_Channel[Offset + 4u];
	}
	/*进行ReturnControlToEcu操作*/
	SchM_Enter_Dcm(Dcm_Channel);
	(*Ret) = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidReturnControlToECUFnc( 	pControlOptionRecord,
																			    	pControlEnableMaskRecord,
																			    	pControlStatusRecord,
																			    	ErrorCode);
	SchM_Exit_Dcm(Dcm_Channel);
	return ControlStatusRecordSize;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_UdsResetToDefult(	uint8   MsgCtrlId,
													uint8   DidCfgIndex,
													uint16  Offset,
													P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
													P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
													P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl)
{
	uint8   ControlOptionRecordSize =0u;
	uint8   ContrlEnableMaskRecordSize = 0u;
	uint8   ControlStatusRecordSize = 0u;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlEnableMaskRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlOptionRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlStatusRecord = NULL_PTR;

	if (NULL_PTR == ((Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidResetToDefaultFnc))
	{
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_REQUESTOUTOFRANGE; /*Nrc=0x31*/
		return ControlStatusRecordSize;
	}
	if (NULL_PTR == pDspDidControl->pDcmDspDidResetToDefault)
	{
		/*pDcmDspDidResetToDefault is invalid*/
		(*Ret) = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidResetToDefaultFnc(	NULL_PTR,
																				NULL_PTR,
																				NULL_PTR,
																				ErrorCode);
		return ControlStatusRecordSize;
	}
	/*check the request message length*/
	ControlOptionRecordSize    = pDspDidControl->pDcmDspDidResetToDefault->DcmDspDidControlOptionRecordSize;
	ContrlEnableMaskRecordSize = pDspDidControl->pDcmDspDidResetToDefault->DcmDspDidControlEnableMaskRecordSize;
	ControlStatusRecordSize    = pDspDidControl->pDcmDspDidResetToDefault->DcmDspDidControlStatusRecordSize;
	if ((4u+ControlOptionRecordSize+ContrlEnableMaskRecordSize) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*if the request message length is not correct,send NRC 0x13*/
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
		return ControlStatusRecordSize;
	}
	if (0u != ControlOptionRecordSize)
	{
		pControlOptionRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]);
	}
	if (0u != ContrlEnableMaskRecordSize)
	{
		pControlEnableMaskRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4u + ControlOptionRecordSize]);
	}
	if (0u != ControlStatusRecordSize)
	{
		pControlStatusRecord = &Dcm_Channel[Offset + 4u];
	}
    /*进行ResetToDefault操作*/
	SchM_Enter_Dcm(Dcm_Channel);
	(*Ret) = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidResetToDefaultFnc(	pControlOptionRecord,
									   											pControlEnableMaskRecord,
									   											pControlStatusRecord,
									   											ErrorCode);
	SchM_Exit_Dcm(Dcm_Channel);
	return ControlStatusRecordSize;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_UdsFreezeCurrentState(	uint8   MsgCtrlId,
														uint8   DidCfgIndex,
														uint16  Offset,
														P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
														P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl)
{
	uint8   ControlOptionRecordSize = 0u;
	uint8   ContrlEnableMaskRecordSize = 0u;
	uint8   ControlStatusRecordSize = 0u;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlEnableMaskRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlOptionRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlStatusRecord = NULL_PTR;

	if (NULL_PTR == ((Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidFreezeCurrentStateFnc))
	{
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
		return ControlStatusRecordSize;
	}
	if (NULL_PTR == pDspDidControl->pDcmDspDidFreezeCurrentState)
	{
		/*pDcmDspDidFreezeCurrentState is invalid*/
		(*Ret) = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidFreezeCurrentStateFnc(	NULL_PTR,
																						NULL_PTR,
																						NULL_PTR,
																						ErrorCode);
		return ControlStatusRecordSize;
	}
    /********************************/
  	ControlOptionRecordSize    = pDspDidControl->pDcmDspDidFreezeCurrentState->DcmDspDidControlOptionRecordSize;
	ContrlEnableMaskRecordSize = pDspDidControl->pDcmDspDidFreezeCurrentState->DcmDspDidControlEnableMaskRecordSize;
	ControlStatusRecordSize    = pDspDidControl->pDcmDspDidFreezeCurrentState->DcmDspDidControlStatusRecordSize;
	if ((4u + ControlOptionRecordSize + ContrlEnableMaskRecordSize)!=Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT; /*NRC=0x13*/
		return ControlStatusRecordSize;
	}
	if (0u != ControlOptionRecordSize)
	{
		pControlOptionRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]);
	}
	if (0u != ContrlEnableMaskRecordSize)
	{
		pControlEnableMaskRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4u + ControlOptionRecordSize]);
	}
	if (0u != ControlStatusRecordSize)
	{
		pControlStatusRecord = &Dcm_Channel[Offset + 4u];
	}
	SchM_Enter_Dcm(Dcm_Channel);
	*Ret = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidFreezeCurrentStateFnc(	pControlOptionRecord,
																			  	pControlEnableMaskRecord,
																			  	pControlStatusRecord,
																			 	ErrorCode);
	SchM_Exit_Dcm(Dcm_Channel);
	return ControlStatusRecordSize;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_UdsShortTermAdjustment(	uint8   MsgCtrlId,
														uint8   DidCfgIndex,
														uint16  Offset,
														P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
														P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode,
														P2CONST(Dcm_DspDidControlType, AUTOMATIC, DCM_APPL_CONST)pDspDidControl)
{
	uint8   ControlOptionRecordSize = 0u;
	uint8   ContrlEnableMaskRecordSize = 0u;
	uint8   ControlStatusRecordSize = 0u;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlEnableMaskRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlOptionRecord = NULL_PTR;
	P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) pControlStatusRecord = NULL_PTR;

	if (NULL_PTR == ((Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidShortTermAdjustmentFnc))
	{
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
		return ControlStatusRecordSize;
	}
	if (NULL_PTR == pDspDidControl->pDcmDspDidShortTermAdjustement)
	{
		(Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidShortTermAdjustmentFnc(NULL_PTR,NULL_PTR,NULL_PTR,ErrorCode);
		return ControlStatusRecordSize;
	}
	
    /**********************/
	ControlOptionRecordSize    = pDspDidControl->pDcmDspDidShortTermAdjustement->DcmDspDidControlOptionRecordSize;
	ContrlEnableMaskRecordSize = pDspDidControl->pDcmDspDidShortTermAdjustement->DcmDspDidControlEnableMaskRecordSize;
	ControlStatusRecordSize    = pDspDidControl->pDcmDspDidShortTermAdjustement->DcmDspDidControlStatusRecordSize;
	if ((4u + ControlOptionRecordSize + ContrlEnableMaskRecordSize) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		(*Ret) = E_NOT_OK;
		(*ErrorCode) = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT; /*Nrc=0x13*/
		return ControlStatusRecordSize;
	}
	if (0u != ControlOptionRecordSize)
	{
		pControlOptionRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]);
	}
	if (0u != ContrlEnableMaskRecordSize)
	{
		pControlEnableMaskRecord = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4 + ControlOptionRecordSize]);
	}
	if (0u != ControlStatusRecordSize)
	{
		pControlStatusRecord = &Dcm_Channel[Offset + 4u];
	}
	SchM_Enter_Dcm(Dcm_Channel);
	*Ret = (Dcm_DspCfg.pDcmDspDid)[DidCfgIndex].DcmDspDidShortTermAdjustmentFnc(	pControlOptionRecord,
																			     	pControlEnableMaskRecord,
																			     	pControlStatusRecord,
																			   		ErrorCode);
	SchM_Exit_Dcm(Dcm_Channel);

	return ControlStatusRecordSize;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_UdsIOControlSendResponse(	uint8 	ProtocolCtrlId,
															uint8   MsgCtrlId,
															uint16  RecDid,
															uint16  Offset,
															uint8   ControlStatusRecordSize,
															uint8   InputOutControlParameter,
															Dcm_NegativeResponseCodeType ErrorCode,
															Std_ReturnType  Ret)
{
	switch(Ret)
	{
		case  E_OK:
			SchM_Enter_Dcm(Dcm_Channel);
			Dcm_Channel[Offset] = 0x6F;  /*response SID*/
			Dcm_Channel[Offset + 1] = (uint8)(RecDid >> 8); /*response Did MSB*/
			Dcm_Channel[Offset + 2] = (uint8)RecDid;     /*response Did LSB*/
			Dcm_Channel[Offset + 3] = InputOutControlParameter; /*InputOutControlParameter*/
			SchM_Exit_Dcm(Dcm_Channel);
			SchM_Enter_Dcm(Dcm_MsgCtrl);
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 4u + (uint32)ControlStatusRecordSize;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 4u + (uint32)ControlStatusRecordSize;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
			SchM_Exit_Dcm(Dcm_MsgCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
		case  E_NOT_OK:
			(void)DsdInternal_SetNrc(ProtocolCtrlId, ErrorCode);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
		case  E_PENDING:
			break;
		default:
			/*NRC=0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
	}
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x2F(uint8  ProtocolCtrlId)
{
	uint8   MsgCtrlId;
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
	uint8   Index;
	uint8   TxChannelCtrlIndex;
	uint8   TxChannelCfgIndex;
	uint8   DidCfgIndex;
	uint8   DidInfoCfgIndex;
	uint8   ControlStatusRecordSize;
	uint8   InputOutControlParameter;
	uint16  RecDid;
	uint16  Offset;
	boolean Flag;
	P2CONST(Dcm_DspDidControlType ,AUTOMATIC ,DCM_APPL_CONST) pDspDidControl;
	Dcm_NegativeResponseCodeType ErrorCode;
#endif
	Std_ReturnType  Ret;

	/*************************************************/
	Ret = Dcm_UdsConditionCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != Ret)
	{
		return Ret;
	}

	/*****************************************/
	/*find the required DID in configuration*/
	RecDid = (((uint16)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1]) << 8)
		    |((uint16) Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2]);
	Flag = FALSE;
	for (Index = 0; (Index < Dcm_DspCfg.DcmDspDidNum) && (FALSE == Flag); Index++)
	{
		if (RecDid == Dcm_DspCfg.pDcmDspDid[Index].DcmDspDidId)
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if not found,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*****************************************/
	/*check pDcmDspDidControl*/
	DidCfgIndex = (uint8)(Index - 1);
	DidInfoCfgIndex = Dcm_DspCfg.pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;
	pDspDidControl = (Dcm_DspCfg.pDcmDspDidInfo)[DidInfoCfgIndex].pDcmDspDidAccess->pDcmDspDidControl;
	if (NULL_PTR == pDspDidControl)
	{
		/*if pDcmDspDidControl is NULL,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/******************************************/
	Ret = Dcm_UdsIODidSessionAndSecutityCheck(ProtocolCtrlId, pDspDidControl);
	if(E_OK != Ret)
	{
		return Ret;
	}

	/*****************************************/
	/*find the TX buffer*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

	/*check whether the required InputOutControlParameter is valid*/
	InputOutControlParameter = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3];
	switch (InputOutControlParameter)
	{
		case DCM_UDS0X2F_RETURNCONTROLTOECU:/*ReturnControlToEcu*/
			ControlStatusRecordSize = Dcm_UdsReturnControlToEcu(	MsgCtrlId, 
			                                              			DidCfgIndex, 
			                                              			Offset, 
			                                              			&Ret, 
			                                              			&ErrorCode, 
			                                              			pDspDidControl	);
			break;
		case DCM_UDS0X2F_RESETTODEFAULT: /*ResetToDefault*/
			ControlStatusRecordSize = Dcm_UdsResetToDefult(		MsgCtrlId, 
																DidCfgIndex, 
																Offset, 
																&Ret, 
																&ErrorCode, 
																pDspDidControl);
			break;
		case DCM_UDS0X2F_FREEZECURRENTSTATE: /*FreezeCurrentState*/
			ControlStatusRecordSize = Dcm_UdsFreezeCurrentState(	MsgCtrlId, 
																	DidCfgIndex, 
																	Offset, 
																	&Ret, 
																	&ErrorCode, 
																	pDspDidControl);
			break;
		case DCM_UDS0X2F_SHORTTERMADJUSTMENT:/*ShortTermAdjustment*/
			ControlStatusRecordSize = Dcm_UdsShortTermAdjustment(	MsgCtrlId, 
																	DidCfgIndex, 
																	Offset, 
																	&Ret, 
																	&ErrorCode, 
																	pDspDidControl	);
			break;
		default:
			Ret = E_NOT_OK;
			ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
			break;
	}
	/* send response message */
	Dcm_UdsIOControlSendResponse(	ProtocolCtrlId,
	                				MsgCtrlId,
	                 				RecDid,
	                 				Offset,
									ControlStatusRecordSize,
									InputOutControlParameter,
									ErrorCode,
									Ret);

	return(Ret);
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
                 UDS:RoutineControl (31 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_RoutineControlServiceConditonCheck(
		uint8   ProtocolCtrlId,
		P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType  Ret = E_OK;

	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	Ret = DsdInternal_SesCheck(ProtocolCtrlId, SID_ROUTINE_CONTROL);
	if(E_NOT_OK == Ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	Ret = DsdInternal_SecurityCheck(ProtocolCtrlId, SID_ROUTINE_CONTROL);
	if (E_NOT_OK == Ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X31_REQ_DATA_MINLENGTH)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	return Ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_RoutineIdSessionAndSecurityCheck(	uint8 ProtocolCtrlId,
																			P2CONST(Dcm_DspRoutineAuthorizationType, AUTOMATIC, DCM_APPL_CONST)pDspRoutineAuthorization)
{
#if((STD_ON==DCM_SESSION_FUNC_ENABLED) ||(STD_ON == DCM_SECURITY_FUNC_ENABLED) )
	uint8   Index;
	boolean Flag;
#endif

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	uint8   RoutineSesNum;
#endif

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	uint8   RoutineSecNum;
#endif
	Std_ReturnType  Ret = E_OK;

	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	Flag = FALSE;
	RoutineSesNum = pDspRoutineAuthorization->DcmDspRoutineAuthorizationSessionRefNum;
	for (Index=0; (Index < RoutineSesNum) && (FALSE == Flag); Index++)
	{
		if (Dcm_MkCtrl.Dcm_ActiveSes == pDspRoutineAuthorization->pDcmDspRoutineAuthorizationSessionRow[Index])
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*the processing is not supported in current session,send NRC 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*check security*/
	#if(STD_ON==DCM_SECURITY_FUNC_ENABLED)
	Flag = FALSE;
	RoutineSecNum = pDspRoutineAuthorization->DcmDspRoutineAuthorizationSecLevelRefNum;
	for (Index = 0; (Index < RoutineSecNum) && (FALSE == Flag); Index++)
	{
#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
		if (gFactory_ActiveFlag||(Dcm_MkCtrl.Dcm_ActiveSec == pDspRoutineAuthorization->pDcmDspRoutineAuthorizationSecLevelRow[Index]))/*yb*/
#else
		if (Dcm_MkCtrl.Dcm_ActiveSec == pDspRoutineAuthorization->pDcmDspRoutineAuthorizationSecLevelRow[Index])
#endif
		{
			Flag = TRUE;
		}
	}
	if(FALSE == Flag)
	{
		/*the processing is not supported in current security level,send NRC 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return  E_NOT_OK;
	}
	#endif

	return Ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_StartRoutine(uint8  MsgCtrlId,
															uint8  RoutineCfgId,
															uint8  RoutineInfoCfgId,
															uint16 Offset,
															P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
															P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC)ErrorCode)
{
	uint8   ReqLen = 0;
	uint8   ResLen = 0;
	Dcm_StartRoutineFncType   StartRoutineFnc;
	P2VAR(uint8 ,AUTOMATIC ,DCM_VAR_NOINIT) pInbuffer;
	P2VAR(uint8 ,AUTOMATIC ,DCM_VAR_NOINIT) pOutBuffer;

	StartRoutineFnc = Dcm_DspCfg.pDcmDspRoutine[RoutineCfgId].DcmDspStartRoutineFnc;
	if (NULL_PTR == StartRoutineFnc)
	{
		/*StartRoutineFnc is NULL,send NRC 0x31*/
		*ErrorCode  = DCM_E_REQUESTOUTOFRANGE;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	/*check the request message length*/
	ReqLen = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspStartRoutine->DcmDspStartRoutineCtrlOptRecSize;
	ResLen = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspStartRoutine->DcmDspStartRoutineStsOptRecSize;
	if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the request message length is not correct,send NRC 0x13*/
		*ErrorCode  = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	pInbuffer  = NULL_PTR;
	if (0u != ReqLen)
	{
		pInbuffer = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];
	}
	pOutBuffer = NULL_PTR;
	if (0u != ResLen)
	{
		pOutBuffer = &Dcm_Channel[Offset + 4u];
	}
	*Ret = (*StartRoutineFnc)(pInbuffer, pOutBuffer, ErrorCode);

	if (E_OK == *Ret)
	{
		Dcm_RoutineControlState[RoutineCfgId] = DCM_DSP_ROUTINE_ON;
	}

	return ResLen;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_StopRoutine(	uint8  MsgCtrlId,
															uint8  RoutineCfgId,
															uint8  RoutineInfoCfgId,
															uint16 Offset,
															P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
															P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode)
{
	uint8   ReqLen = 0;
	uint8   ResLen = 0;
	Dcm_StopRoutineFncType   StopRoutineFnc;
	P2VAR(uint8 ,AUTOMATIC ,DCM_VAR_NOINIT) pInbuffer;
	P2VAR(uint8 ,AUTOMATIC ,DCM_VAR_NOINIT) pOutBuffer;

    /*********************/
	StopRoutineFnc = Dcm_DspCfg.pDcmDspRoutine[RoutineCfgId].DcmDspRoutineStopFnc;
	if (NULL_PTR == StopRoutineFnc)
	{
		/*StopRoutineFnc is NULL,send NRC 0x31*/
		*ErrorCode  = DCM_E_SUBFUNCTIONNOTSUPPORTED;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	
	if (NULL_PTR != Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineStop)
	{
		ReqLen = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineStop->DcmDspStopRoutineCtrlOptRecSize;
		ResLen = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineStop->DcmDspStopRoutineStsOptRecSize;
	    if ((ReqLen + DCM_UDS0X31_REQ_DATA_MINLENGTH) != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	    {
		   	/*the request message length is not correct,send NRC 0x13*/
			*ErrorCode  = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
			*Ret = E_NOT_OK;
			return ResLen;
		}

	}	
    /*********************/
	if (DCM_DSP_ROUTINE_ON != Dcm_RoutineControlState[RoutineCfgId])
	{
		/*the request sequence error,send NRC 0x24*/
		*ErrorCode  = DCM_E_REQUESTSEQUENCEERROR;
		*Ret = E_NOT_OK;
		return ResLen;
	}

	if (NULL_PTR == Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineStop)
	{
		/*pDcmDspRoutineStop is NULL*/
		ResLen = 0u;
		*Ret = (*StopRoutineFnc)(NULL_PTR,NULL_PTR,ErrorCode);
	}
	else
	{
		pInbuffer  = NULL_PTR;
		if(0u != ReqLen)
		{
			pInbuffer  = &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4];  /*routineControlOptionRecord*/
		}
		pOutBuffer = NULL_PTR;
		if(0u != ResLen)
		{
			pOutBuffer = &Dcm_Channel[Offset + 4u];	 /*routineStatusRecord*/
		}
		*Ret = (*StopRoutineFnc)(pInbuffer,pOutBuffer,ErrorCode);
	}
	if (E_OK == *Ret)
	{
		Dcm_RoutineControlState[RoutineCfgId] = DCM_DSP_ROUTINE_OFF;
	}

	return ResLen;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(uint8, DCM_CODE)Dcm_RoutineControl_RequestRoutinrResult(
		uint8   MsgCtrlId,
		uint8   RoutineCfgId,
		uint8   RoutineInfoCfgId,
		uint16  Offset,
		P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) Ret,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) ErrorCode)
{
	uint8   ResLen = 0;
	Dcm_RequestResultsRoutineFncType  RequestResultsFnc;
	P2VAR(uint8 ,AUTOMATIC ,DCM_VAR_NOINIT) pOutBuffer;
    
    /**********************/
	if (DCM_UDS0X31_REQ_DATA_MINLENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the request message length is not correct,send NRC 0x13*/
		*ErrorCode  = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	
	if (DCM_DSP_ROUTINE_INIT == Dcm_RoutineControlState[RoutineCfgId])
	{
		/*the request sequence error,send NRC 0x24*/
		*ErrorCode  = DCM_E_REQUESTSEQUENCEERROR;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	
	RequestResultsFnc = Dcm_DspCfg.pDcmDspRoutine[RoutineCfgId].DcmDspRequestResultsRoutineFnc;
	if (NULL_PTR == RequestResultsFnc)
	{
		/*RequestResultsFnc is NULL,send NRC 0x31*/
		*ErrorCode  = DCM_E_REQUESTOUTOFRANGE;
		*Ret = E_NOT_OK;
		return ResLen;
	}
	if (NULL_PTR == Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineRequestRes)
	{
		ResLen = 0u;
		*Ret = (*RequestResultsFnc)(NULL_PTR,ErrorCode);
	}
	else
	{
		ResLen = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineRequestRes->DcmDspReqResRtnCtrlOptRecSize;
		pOutBuffer = NULL_PTR;
		if (0u != ResLen)
		{
			pOutBuffer = &Dcm_Channel[Offset + 4u];
		}
		*Ret = (*RequestResultsFnc)(pOutBuffer, ErrorCode);		
	}
	return ResLen;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dcm_RoutineControlServiceSendResponse(
		uint8   ProtocolCtrlId,
		uint8   MsgCtrlId,
		uint8   ResLen,
		uint16  routineId,
		uint16  Offset,
		uint8   SubFunction,
		Std_ReturnType  Ret,
		Dcm_NegativeResponseCodeType  ErrorCode)
{
	/*check the return value*/
	switch (Ret)
	{
		case  E_OK:
			SchM_Enter_Dcm(Dcm_MsgCtrl);
			Dcm_Channel[Offset] = 0x71;
			Dcm_Channel[Offset + 1u] = SubFunction;
			Dcm_Channel[Offset + 2u] = (uint8)(routineId >> 8);
			Dcm_Channel[Offset + 3u] = (uint8)(routineId);
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = (uint32)ResLen + 4u;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = (uint32)ResLen + 4u;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
			SchM_Exit_Dcm(Dcm_MsgCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
		case  E_NOT_OK:
			/*NRC=ErrorCode*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId, ErrorCode);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			break;
		case  E_PENDING:
			break;
		default:
			/*unexpected return value,send NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
		break;
	}

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x31(uint8  ProtocolCtrlId)
{
	uint8   MsgCtrlId;
#if((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
	uint8   Index;
	uint8   ResLen = 0;
	uint8   SubFunction;
	uint8   RoutineCfgId;
	uint8   RoutineInfoCfgId;
	uint8   TxChannelCtrlIndex;
	uint8   TxChannelCfgIndex;
	uint16  routineId;
	uint16  Offset;
	boolean Flag;
	Dcm_NegativeResponseCodeType  ErrorCode;
	P2CONST(Dcm_DspRoutineAuthorizationType, AUTOMATIC, DCM_APPL_CONST)  pDspRoutineAuthorization;
#endif
	Std_ReturnType  Ret = E_OK;

	/*************************************************/
	Ret = Dcm_RoutineControlServiceConditonCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != Ret)
	{
		return Ret;
	}

#if ((STD_OFF == DCM_DSP_ROUTINE_FUNC_ENABLED) || (DCM_DSP_ROUTINE_MAX_NUM == 0))
	/*NRC 0x31:request out of range*/
	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return E_NOT_OK;

#else
	/*************************************************/
	/*check whether the sub-function is supported*/
	SubFunction = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
	if(  (SubFunction != DCM_UDS0X31_STARTROUTINE)
			&& (SubFunction != DCM_UDS0X31_STOPROUTINE)
			&& (SubFunction != DCM_UDS0X31_REQUESTROUTINERESULTS) )
	{
		/*if the required sub-function is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*************************************************/
	/*find the required RoutineID in configuration*/
	routineId =  (((uint16)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2]) << 8)\
			   | ((uint16) Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3]);
	Flag = FALSE;
	for (Index=0;(Index<Dcm_DspCfg.DcmDspRoutineNum)&&(FALSE == Flag);Index++)
	{
		if (routineId == Dcm_DspCfg.pDcmDspRoutine[Index].RoutineId)
		{
			Flag = TRUE;
		}
	}
	if (FALSE == Flag)
	{
		/*if not found,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*************************************************/
	/*check session and security*/
	RoutineCfgId     = (uint8)(Index - 1);
	RoutineInfoCfgId = Dcm_DspCfg.pDcmDspRoutine[RoutineCfgId].DcmDspRoutineInfoID;
	pDspRoutineAuthorization = Dcm_DspCfg.pDcmDspRoutineInfo[RoutineInfoCfgId].pDcmDspRoutineAuthorization;
	Ret = Dcm_RoutineIdSessionAndSecurityCheck(ProtocolCtrlId, pDspRoutineAuthorization);
	if(E_OK != Ret)
	{
		return Ret;
	}

	/*************************************************/
	/*get the Tx buffer*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	switch (SubFunction)
	{
		case  DCM_UDS0X31_STARTROUTINE:
			ResLen = Dcm_RoutineControl_StartRoutine(MsgCtrlId, RoutineCfgId, RoutineInfoCfgId, Offset, &Ret, &ErrorCode);
			break;
		case  DCM_UDS0X31_STOPROUTINE:
			ResLen = Dcm_RoutineControl_StopRoutine(MsgCtrlId, RoutineCfgId, RoutineInfoCfgId, Offset, &Ret, &ErrorCode);
			break;
		case  DCM_UDS0X31_REQUESTROUTINERESULTS:
			ResLen = Dcm_RoutineControl_RequestRoutinrResult(MsgCtrlId, RoutineCfgId, RoutineInfoCfgId, Offset, &Ret, &ErrorCode);
			break;
		default:
			ErrorCode  = DCM_E_CONDITIONSNOTCORRECT;
			Ret = E_NOT_OK;
			break;
	}
	/*check the return value*/
	Dcm_RoutineControlServiceSendResponse(ProtocolCtrlId, MsgCtrlId, ResLen, routineId, Offset, SubFunction, Ret, ErrorCode);

	return (Ret);
#endif
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
                 UDS:TesterPresent (3E hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X3E_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x3E(uint8  ProtocolCtrlId)
{
	uint8  MsgCtrlId;
	uint8  TxChannelCtrlIndex;
	uint8  TxChannelCfgIndex;
	uint16 Offset;
#if((STD_ON == DCM_SESSION_FUNC_ENABLED) || (STD_ON == DCM_SECURITY_FUNC_ENABLED))
	Std_ReturnType ret;
#endif

	/*************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_TESTER_PRESENT);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_TESTER_PRESENT);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
	/*if the required protocol is configuted,get the index of runtime datum*/
	MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (DCM_UDS0X3E_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	if (DCM_UDS0X3E_ZERO_SUBFUNCTION != Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1])
	{
		/*the required sub-function is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*assemble and send positive response*/
	SchM_Enter_Dcm(Dcm_MsgCtrl);
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	Dcm_Channel[Offset] = 0x7E;
	Dcm_Channel[Offset + 1] =(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData)[1];
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 2u;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 2u;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  E_OK;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
                 UDS:ControlDTCSetting (85 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X85_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0X85ServiceConditionCheck(	uint8  ProtocolCtrlId,
																		P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType ret = E_OK;

	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId, SID_CONTROL_DTC_SETTING);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId, SID_CONTROL_DTC_SETTING);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

    if (DCM_UDS0X85_REQ_DATA_MINLENGTH > (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen))
    {
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }

    if (  (DCM_UDS0X85_REQ_DATA_MINLENGTH != (Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen))
    	&&(0u != ((Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen - DCM_UDS0X85_REQ_DATA_MINLENGTH) % 3)))
    {
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }

    return ret;

}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"


#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x85(uint8  ProtocolCtrlId)
{
	uint8  MsgCtrlId;
	uint8  TxChannelCtrlIndex;
	uint8  TxChannelCfgIndex;
	uint8  subFunc;
	uint16 Offset;
	uint32 groupOfDTC;
	uint32 index = 0;
	Std_ReturnType ret = E_OK;
	Dem_ReturnControlDTCStorageType returnCtrlDtcStorage = DEM_CONTROL_DTC_STORAGE_OK;
	uint32 numOfDTC = 0;

	/************************************/
	ret = Dcm_Uds0X85ServiceConditionCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}
    
    /************************************/
	/*get the sub-function*/
	subFunc = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
	if (  (DCM_UDS0X85_ON  != subFunc)
		&&(DCM_UDS0X85_OFF != subFunc) )
	{
		/*the required sub-function is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

    /************************************/
	/*the request message does not contain DTCSettingControlOptionRecord*/
    if (DCM_UDS0X85_REQ_DATA_MINLENGTH == Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*check the sub-function*/
		if (DCM_UDS0X85_ON == subFunc)
		{
			/*invoke the corresponding API provided by DEM*/
			returnCtrlDtcStorage = Dem_EnableDTCStorage(DEM_DTC_GROUP_ALL_DTCS, 
			                                            DEM_DTC_KIND_ALL_DTCS);
		}
		if (DCM_UDS0X85_OFF == subFunc)
		{
			/*invoke the corresponding API provided by DEM*/
			returnCtrlDtcStorage = Dem_DisableDTCStorage(DEM_DTC_GROUP_ALL_DTCS, 
			                                             DEM_DTC_KIND_ALL_DTCS);
		}
	}

	/*the request message contains DTCSettingControlOptionRecord*/
	else if(0u == ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - DCM_UDS0X85_REQ_DATA_MINLENGTH) % 3))
	{
		numOfDTC = (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 2) / 3;
		/*check the sub-function*/
		if (DCM_UDS0X85_ON == subFunc)
		{
			/*get and assemble DTC, high byte first*/
			for (index = 0;(index < numOfDTC)&&(DEM_CONTROL_DTC_STORAGE_OK == returnCtrlDtcStorage);index++)
			{
				groupOfDTC = (((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2 + (3 * index)])) << 16)
							|(((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3 + (3 * index)])) << 8)
						    |((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4 + (3 * index)]));
				/*invoke the corresponding API provided by DEM*/
				returnCtrlDtcStorage = Dem_EnableDTCStorage(groupOfDTC, DEM_DTC_KIND_ALL_DTCS);
			}
		}
		if (DCM_UDS0X85_OFF == subFunc)
		{
			/*get and assemble DTC, high byte first*/
			for (index = 0;(index < numOfDTC)&&(DEM_CONTROL_DTC_STORAGE_OK == returnCtrlDtcStorage);index++)
			{
				groupOfDTC = (((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2 + (3 * index)])) << 16)
							|(((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3 + (3 * index)])) << 8)
						    |((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4 + (3 * index)]));
				/*invoke the corresponding API provided by DEM*/
				returnCtrlDtcStorage = Dem_DisableDTCStorage(groupOfDTC, DEM_DTC_KIND_ALL_DTCS);
			}
		}
	}
	else
	{
		/*Nothing to do */
	}
	switch (returnCtrlDtcStorage)
	{
		case DEM_CONTROL_DTC_STORAGE_OK:
			/*the processing is successful,assemble positive response*/
			TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
			TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
			Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
			SchM_Enter_Dcm(Dcm_Channel);
			Dcm_Channel[Offset] = 0xC5;   /*response SID*/
			Dcm_Channel[Offset + 1] = subFunc;  /*echo of 0 - 6 bits of sub-function*/
			SchM_Exit_Dcm(Dcm_Channel);
			SchM_Enter_Dcm(Dcm_MsgCtrl);
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x02;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 0x02;
			Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
			SchM_Exit_Dcm(Dcm_MsgCtrl);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_OK;
			break;
		case DEM_CONTROL_DTC_STORAGE_N_OK:
			/*the processing is not successful,send NRC 0x22*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
		case DEM_CONTROL_DTC_WRONG_DTCGROUP:
			/*DTCSettingControlOptionRecord error,send NRC 0x31*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
		default:
			/*DTCSettingControlOptionRecord error,send NRC 0x31*/
			(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
			DsdInternal_ProcessingDone(ProtocolCtrlId);
			ret = E_NOT_OK;
			break;
	}

	return  ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
             UDS:CommunicationControl (28 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
/********************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x28(uint8  ProtocolCtrlId)
{
	uint8  MsgCtrlId = 0;
	uint8  TxChannelCtrlIndex = 0;
	uint8  TxChannelCfgIndex = 0;
	uint8  subFunc = 0;
	uint8  comType = 0;
	uint16 Offset = 0;
	Std_ReturnType ret = E_OK;

	/*************************************************/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_COMMUNICATION_CONTROL);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif

	/*************************************************/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_COMMUNICATION_CONTROL);
	if(E_NOT_OK == ret)
	{
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
#endif
	
	/*************************************************/
	/*if the required protocol is configuted,get the index of runtime datum*/
	MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (DCM_UDS0X28_REQ_DATA_MAXLENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	/*get the sub-function and communicationType*/
	subFunc =Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
	comType =Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];

	if ((subFunc != DCM_UDS0X28_ENABLE_RX_AND_TX)
	     &&(subFunc != DCM_UDS0X28_DISABLE_RX_AND_TX))    /*just support 00,03*/
	{
		/*the required sub-function is not supported,send NRC 0x12*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
    if ((comType < COMTYPE_NORM) || (comType > COMTYPE_NORM_AND_NM))
    {
		/*communicationType is not supported,send NRC 0x31*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }

    /*******************************************************
     *callback function make communicaiton control
     *******************************************************/
    ret = Dcm_UdsCommunicaitonControl(subFunc,comType);
    if (E_NOT_OK == ret)
    {
		/*the processing is not successful,send NRC 0x22*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_CONDITIONSNOTCORRECT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
    }
    /**********************************************************
     * assemble positive response
     *********************************************************/
	/*the processing is successful,assemble positive response*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	SchM_Enter_Dcm(Dcm_Channel);
	Dcm_Channel[Offset] = 0x68;   		/*response SID*/
	Dcm_Channel[Offset + 1] = subFunc;  /*echo of 0 - 6 bits of sub-function*/
	SchM_Exit_Dcm(Dcm_Channel);
	SchM_Enter_Dcm(Dcm_MsgCtrl);
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x02;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 0x02;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  ret;
}

STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsCommunicaitonControl(uint8  controlType,uint8 communicationType)
{
	/*todo:real action for communication control, implementation specific*/
	return E_OK;
}

#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

/****************************************************************
           UDS:LinkControl (87 hex) service
 ***************************************************************/
#if(STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
/************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(void, DCM_CODE)Dsp_InitLinkCtrolCtrl(void)
{
	Dcm_LinkControlCtrl.fixedBaudrate   = 0u;
	Dcm_LinkControlCtrl.specialBaudrate = 0u;
	Dcm_LinkControlCtrl.linkCtrlStatus  = LINK_CONTROL_IDLE;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsLCCondtionCheck(uint8  ProtocolCtrlId,
															P2VAR(uint8, AUTOMATIC, AUTOMATIC)MsgCtrlId)
{
	Std_ReturnType ret = E_OK;

	/*************************************************/
	#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
	/*session check,check whether the current session supports the request service*/
	ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_LINK_CONTROL);
	if(E_NOT_OK == ret)
	{
		Dsp_InitLinkCtrolCtrl();
		/****@req DCM-FUNR-073[DCM211]****/
		/*the current session does not support the request service,send NRC = 0x7F*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

		/*************************************************/
	#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	/*security check,check whether the current security supports the request service*/
	ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_LINK_CONTROL);
	if(E_NOT_OK == ret)
	{
		Dsp_InitLinkCtrolCtrl();
		/****@req DCM-FUNR-074[DCM217]****/
		/*the current security does not support the request service,send NRC = 0x33*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SECURITYACCESSDENIED);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	#endif

	/*************************************************/
	/*if the required protocol is configured,get the index of runtime datum*/
	*MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

	/*check the massage length*/
	if (DCM_UDS0X87_REQ_DATA_MINLENGTH > Dcm_MsgCtrl[*MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*the length of massage is not correct,send NRC 0x13*/
		Dsp_InitLinkCtrolCtrl();
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}

	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
STATIC FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsLCVBTWFB(uint8  ProtocolCtrlId,
													 uint8  MsgCtrlId,
													 P2VAR(uint8, AUTOMATIC, AUTOMATIC)baudrateIdentifier)
{
	Std_ReturnType ret = E_OK;

	if (DCM_UDS0X87_SUBFUNC0X01_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
	{
		/*reset Link control block*/
		Dsp_InitLinkCtrolCtrl();
		/*the length of massage is not correct,send NRC 0x13*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	if(LINK_CONTROL_IDLE != Dcm_LinkControlCtrl.linkCtrlStatus)
	{
		/*reset Link control block*/
		Dsp_InitLinkCtrolCtrl();
		/*request sequence error, send NRC 0x24*/
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTSEQUENCEERROR);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	*baudrateIdentifier = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
	if(  (0u == *baudrateIdentifier)
	   ||((*baudrateIdentifier >=6)&&(*baudrateIdentifier <= 0x0F))
	   ||(*baudrateIdentifier >= 0x14))
	{
		/*reset Link control block*/
		Dsp_InitLinkCtrolCtrl();
		/* baudrateIdentifier is invalid,send NRC 0x31 */
		(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTOUTOFRANGE);
		DsdInternal_ProcessingDone(ProtocolCtrlId);
		return E_NOT_OK;
	}
	return ret;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"

/************************/
#define DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)DspInternal_UDS0x87(uint8  ProtocolCtrlId)
{
	uint8  MsgCtrlId = 0u;
	uint8  TxChannelCtrlIndex = 0u;
	uint8  TxChannelCfgIndex = 0u;
	uint8  subFunc = 0u;
	uint8  baudrateIdentifier = 0;
	uint16 Offset = 0u;
	uint32 linkBaudrateRecord = 0;
	Std_ReturnType ret = E_OK;

	/*************************************************/
	ret = Dcm_UdsLCCondtionCheck(ProtocolCtrlId, &MsgCtrlId);
	if(E_OK != ret)
	{
		return ret;
	}
	/*************************************************/
	/*get the sub-function*/
	subFunc = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
    switch(subFunc)
    {
		case DCM_UDS0X87_VERIFY_BAUDRATE_TRANSITION_WITH_FIXED_BAUDRATE:
			ret = Dcm_UdsLCVBTWFB(ProtocolCtrlId,MsgCtrlId,&baudrateIdentifier);
			if(E_OK != ret)
			{
				return ret;
			}
            /******************************************************
             * verifies if the transition can be performed
             *****************************************************/
            ret = Rte_VerifyBaudrateTransitionWithFixedBaudrate(baudrateIdentifier);
            if (E_NOT_OK == ret)
            {
            	/*reset Link control block*/
            	Dsp_InitLinkCtrolCtrl();
                /*condition is error ,send NRC=0x22*/
            	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
            	DsdInternal_ProcessingDone(ProtocolCtrlId);
            	return E_NOT_OK;
            }
            SchM_Enter_Dcm(Dcm_LinkControlCtrl);
            Dcm_LinkControlCtrl.fixedBaudrate = baudrateIdentifier;
            Dcm_LinkControlCtrl.linkCtrlStatus = LINK_CONTROL_FBR_VERIFICATION;
            SchM_Exit_Dcm(Dcm_LinkControlCtrl);
			break;

		case DCM_UDS0X87_VERIFY_BAUDRATE_TRANSITION_WITH_SPECIFIC_BAUDRATE:
			if (DCM_UDS0X87_SUBFUNC0X02_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
			{
				/*reset Link control block*/
				Dsp_InitLinkCtrolCtrl();
				/*the length of massage is not correct,send NRC 0x13*/
				(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
				DsdInternal_ProcessingDone(ProtocolCtrlId);
				return E_NOT_OK;
			}
            if (LINK_CONTROL_IDLE != Dcm_LinkControlCtrl.linkCtrlStatus)
            {
            	/*reset Link control block*/
            	Dsp_InitLinkCtrolCtrl();
				/*request sequence error, send NRC 0x24*/
            	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTSEQUENCEERROR);
            	DsdInternal_ProcessingDone(ProtocolCtrlId);
            	return E_NOT_OK;
            }
			linkBaudrateRecord |= ((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2])) << 16;
			linkBaudrateRecord |= ((uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3])) << 8;
			linkBaudrateRecord |= (uint32)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4]);
           /****************************************************************
             * callback function verifies if the transition can be performed
             ***************************************************************/
            ret = Rte_VerifyBaudrateTransitionWithSpecificBaudrate(linkBaudrateRecord);
            if(E_NOT_OK == ret)
            {
            	/*reset Link control block*/
            	Dsp_InitLinkCtrolCtrl();
                /*condition is error ,send NRC=0x22*/
            	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_CONDITIONSNOTCORRECT);
            	DsdInternal_ProcessingDone(ProtocolCtrlId);
            	return E_NOT_OK;
            }
            SchM_Enter_Dcm(Dcm_LinkControlCtrl);
            Dcm_LinkControlCtrl.specialBaudrate  = linkBaudrateRecord;
            Dcm_LinkControlCtrl.linkCtrlStatus   = LINK_CONTROL_SBR_VERIFICATION;
            SchM_Exit_Dcm(Dcm_LinkControlCtrl);
			break;

		case DCM_UDS0X87_TRANSITION_BAUDRATE:
			if(DCM_UDS0X87_SUBFUNC0X03_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
			{
				/*reset Link control block*/
				Dsp_InitLinkCtrolCtrl();
				/*the length of massage is not correct,send NRC 0x13*/
				(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
				DsdInternal_ProcessingDone(ProtocolCtrlId);
				return E_NOT_OK;
			}
			if(	  (LINK_CONTROL_FBR_VERIFICATION != Dcm_LinkControlCtrl.linkCtrlStatus)
			   && (LINK_CONTROL_SBR_VERIFICATION != Dcm_LinkControlCtrl.linkCtrlStatus ))
			{
            	/*reset Link control block*/
            	Dsp_InitLinkCtrolCtrl();
				/*request sequence error, send NRC 0x24*/
            	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_REQUESTSEQUENCEERROR);
            	DsdInternal_ProcessingDone(ProtocolCtrlId);
            	return E_NOT_OK;
			}
            /****************************************************
             *callback function make baud rate transition
             ****************************************************/
            if(LINK_CONTROL_FBR_VERIFICATION == Dcm_LinkControlCtrl.linkCtrlStatus)
            {
            	Rte_transitionFixedBaudrate(Dcm_LinkControlCtrl.fixedBaudrate);
            }
            if(LINK_CONTROL_SBR_VERIFICATION == Dcm_LinkControlCtrl.linkCtrlStatus)
            {
            	Rte_transitionSpecificBaudrate(Dcm_LinkControlCtrl.specialBaudrate);
            }
            Dsp_InitLinkCtrolCtrl();
			break;
		default:
			Dsp_InitLinkCtrolCtrl();
        	(void)DsdInternal_SetNrc(ProtocolCtrlId,DCM_E_SUBFUNCTIONNOTSUPPORTED);
        	DsdInternal_ProcessingDone(ProtocolCtrlId);
        	return E_NOT_OK;
			break;
    }
	/*the processing is successful,assemble positive response*/
	TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
	TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
	Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
	SchM_Enter_Dcm(Dcm_Channel);
	Dcm_Channel[Offset] = 0xC7;   /*response SID*/
	Dcm_Channel[Offset + 1] = subFunc;  /*echo of 0 - 6 bits of sub-function*/
	SchM_Exit_Dcm(Dcm_Channel);
	SchM_Enter_Dcm(Dcm_MsgCtrl);
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x02;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 0x02;
	Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
	SchM_Exit_Dcm(Dcm_MsgCtrl);
	DsdInternal_ProcessingDone(ProtocolCtrlId);
	return  E_OK;
}
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

 
#endif
 
 
 
 
 
 
 
 
 
 
 
