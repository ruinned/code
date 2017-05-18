/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <>
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
#define DCMDSP_C_VENDOR_ID  0
#define DCMDSP_C_MODULE_ID  0
#define DCMDSP_C_AR_MAJOR_VERSION  3
#define DCMDSP_C_AR_MINOR_VERSION  3
#define DCMDSP_C_AR_PATCH_VERSION  0
#define DCMDSP_C_SW_MAJOR_VERSION  1
#define DCMDSP_C_SW_MINOR_VERSION  0
#define DCMDSP_C_SW_PATCH_VERSION  0
#define DCMDSP_C_VENDOR_API_INFIX  0

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Include.h"
#include "Dem.h"
#include "Dem_Dcm.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCMDSP_C_AR_MAJOR_VERSION != DCMDSP_H_AR_MAJOR_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSP_C_AR_MINOR_VERSION != DCMDSP_H_AR_MINOR_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSP_C_AR_PATCH_VERSION != DCMDSP_H_AR_PATCH_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSP_C_SW_MAJOR_VERSION != DCMDSP_H_SW_MAJOR_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSP_C_SW_MINOR_VERSION != DCMDSP_H_SW_MINOR_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif
#if (DCMDSP_C_SW_PATCH_VERSION != DCMDSP_H_SW_PATCH_VERSION)
  #error "DcmDsp.c : Mismatch in Specification Major Version"
#endif

/******************************* Define ************************************/
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
	#define DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
	#include "MemMap.h"
	VAR(Dcm_RoutineControlStateType,DCM_VAR_NOINIT)Dcm_RoutineControlState[DCM_DSP_ROUTINE_MAX_NUM];
	#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
	#include "MemMap.h"
#endif

/****************************** Implement***********************************/
/*************************************************************************/
/*
 * Brief               <The Dsp layer response to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/*************************************/
#define  DCM_START_SEC_CODE
#include "MemMap.h"
extern uint8 Fl_BootMode[]; 
FUNC(void,DCM_CODE)DspInternal_DcmConfirmation(uint8 ProtocolCtrlId)
{
    uint8   MsgCtrlId;
    uint8   RxChannelCtrlId;
    uint8   TxChannelCtrlId;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    RxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex;
    TxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    /************************************************/
    /*NRC = 0x78 respond to the confirmation process*/
    if(DCM_E_RESPONSE_PENDING==(Dcm_MsgCtrl[MsgCtrlId].NRC))
    {
        /*Nrc=0x78,response to confirmation*/
        /************************************/
        #if 1                                     /* bootloader 复位进入boot模式，回复50 02*/
        //if(0x2A == (*(uint8 *)FL_BOOT_MODE))
        if((0xA5 == Fl_BootMode[0])||(0xA5 == Fl_BootMode[1])||(0xA5 == Fl_BootMode[2])||(0xA5 == Fl_BootMode[3]))       
        {
           /*have received bootloader request,reset*/
           Mcu_PerformReset();
        }
        #endif
        /****@req DCM-FUNR-054[DCM165]****/
        SchM_Enter_Dcm(Dcm_MsgCtrl);
        Dcm_MsgCtrl[MsgCtrlId].NRC = 0xFFu;
        Dcm_MsgCtrl[MsgCtrlId].SendFlag = FALSE;
        Dcm_MsgCtrl[MsgCtrlId].RspStyle = DCM_POS_RSP;
        Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState = DCM_MSG_RECEIVED;
        Dcm_ChannelCtrl[RxChannelCtrlId].Dcm_ChannelRxState = DCM_CH_OCCUPIED;
        Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_ChannelTxState = DCM_CH_OCCUPIED;
        /*restart P2*Timer*/
        Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.PendingNum++;
        Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2CurTicks 	 = Frt_ReadOutTicks();
        Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2ExpiredTicks = Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax;
        Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2State    	 = DCM_P2TIMER_ON;
        SchM_Exit_Dcm(Dcm_MsgCtrl);        
        return;
    }
    /************************************************/
#if((STD_ON == DCM_SECURITY_FUNC_ENABLED)&&((STD_ON == DCM_UDS_SERVICE0X27_ENABLED)))
    /*securityAccess service*/
    if ((0x27 == Dcm_MsgCtrl[MsgCtrlId].SID)
           && (DCM_NEG_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
           && (DCM_E_GENERALREJECT == Dcm_MsgCtrl[MsgCtrlId].NRC))
    {
          /*confirmation to reject SecurityAccess service*/
          DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
    }
    if ((0x27u == Dcm_MsgCtrl[MsgCtrlId].SID)
           && ( (DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
        	    || (DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle) )
           && (DCM_SERVICE_KEY == Dcm_SecCtrl.Dcm_SecServiceState))
    {
        /*Compare key success*/
    	/****@req DCM-FUNR-179[DCM325]****/
        DslInternal_SetSecurityLevel(Dcm_SecCtrl.Dcm_NewSec);
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
        SchM_Enter_Dcm(Dcm_SecCtrl);
        Dcm_SecCtrl.Dcm_RunDlyFlag         = FALSE;
        Dcm_SecCtrl.Dcm_LockedFlag         = FALSE;
        Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0u;
        Dcm_SecCtrl.Dcm_FalseAcessCount    = 0u;
        Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState = DCM_SECTIMER_OFF;
        SchM_Exit_Dcm(Dcm_SecCtrl);
    }
#endif

    /************************************************/
#if((STD_ON == DCM_SESSION_FUNC_ENABLED)&&((STD_ON == DCM_UDS_SERVICE0X10_ENABLED)))
    /*session control sevice confirmation*/
    if((   (DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
				|| (DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle))
        && (0x10u == Dcm_MsgCtrl[MsgCtrlId].SID))
    {
    	/*Uds 0x10 service*/
    	/****@req DCM-FUNR-109[DCM311]****/
        DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    }
#endif

    /**********************************************/
#if (STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
    if(  ((DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
    		||(DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle) )
      && (0x11u == Dcm_MsgCtrl[MsgCtrlId].SID))
    {
        if(TRUE == gAppl_UpdataOK_ResponseFlag)
        {
        	gAppl_UpdataOK_ResponseFlag = FALSE;  /*clear app updata flag*/
        	DslInternal_SesRefresh(DCM_DEFAULT_SESSION);
        }
        else
        {
        	/*confirm to EcuReset service,call Mcu_PerformReset trigger reset*/
    		/****@req DCM-FUNR-113[DCM374]****/
        	Mcu_PerformReset();        	        	
        }
    }
#endif

    /***********************************************
     For the realization of the various sub-function,
     the function code needs to be added here.
     ***********************************************/
    (void)DslInternal_ResetResource(ProtocolCtrlId);
    return;
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"

/****************************************************************************/
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE)Dsp_InitRoutineStates(void)
{
    uint8 index;
    for (index = 0; index < DCM_DSP_ROUTINE_MAX_NUM; index++)
    {
        Dcm_RoutineControlState[index] = DCM_DSP_ROUTINE_INIT;
    }
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif


/****************************************************************************/
#if((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X85_ENABLED))
#define  DCM_START_SEC_CODE
#include "MemMap.h"
FUNC(void,DCM_CODE) Dsp_EnableAllDtcsFresh(void)
{
   /*The update of the DTC status bit information shall continue once a ControlDTCSetting request is performed
     with sub-function set to on or a session layer timeout occurs (server transitions to defaultSession. */
    (void)Dem_EnableDTCStorage(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);
}
#define  DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif


