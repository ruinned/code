/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf.h>
 *  @brief      <CanIf>
 *  
 *  <Compiler: CodeWarrior2.8 MCU:MPC5634>
 *  
 *  @author     <Tommy>
 *  @date       <2013-06-26>
 */
/*============================================================================*/

#ifndef CANIFDIAG_H 
#define CANIFDIAG_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130425   liujn      Initial version
 *  V1.0.1       20130510   Tommy      reconstruct version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_MODULE_ID            60U
#define CANIF_H_VENDOR_ID          62U
#define CANIF_H_AR_MAJOR_VERSION   3U
#define CANIF_H_AR_MINOR_VERSION   2U
#define CANIF_H_AR_PATCH_VERSION   0U
#define CANIF_H_SW_MAJOR_VERSION   1U
#define CANIF_H_SW_MINOR_VERSION   0U
#define CANIF_H_SW_PATCH_VERSION   8U

/*=======[I N C L U D E S]====================================================*/
#include "CanIf_Cfg.h"
#include "CanIfDiag_Types.h"

#if (CANIF_VARIANT_CFG == CANIF_VARIANT_LT) || (CANIF_VARIANT_CFG == CANIF_VARIANT_PB)
#include "CanIf_Lcfg.h"
#endif

#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
#include "CanIf_Pbcfg.h"
#endif

#if STD_ON == CANIF_WAKEUP_VALIDATION
#include "EcuM.h"
#endif

#include "SchM_CanIf.h"

/*=======[M A C R O S]========================================================*/
#define CANIF_INSTANCE_ID                   (0U)

#if(STD_ON == CANIF_DEV_ERROR_DETECT) /** @req CANIF018 @req CANIF246 */
#define CANIF_INIT_ID                       ((uint8)0x01U)
#define CANIF_INIT_CONTROLLER_ID            ((uint8)0x02U)
#define CANIF_SET_CONTROLLER_MODE_ID        ((uint8)0x03U)
#define CANIF_GET_CONTROLLER_MODE_ID        ((uint8)0x04U)
#define CANIF_TRANSMIT_ID                   ((uint8)0x05U)
#define CANIF_READRXPDUDATA_ID              ((uint8)0x06U)
#define CANIF_READTXNOTIFSTATUS_ID          ((uint8)0x07U)
#define CANIF_READRXNOTIFSTATUS_ID          ((uint8)0x08U)
#define CANIF_SETPDUMODE_ID                 ((uint8)0x09U)
#define CANIF_GETPDUMODE_ID                 ((uint8)0x0AU)
#define CANIF_GETVERSION_ID                 ((uint8)0x0BU)
#define CANIF_SETDYNAMICTX_ID               ((uint8)0x0CU)
#define CANIF_SET_TRANSCEIVERMODE_ID        ((uint8)0x0DU)
#define CANIF_GET_TRANSCEIVERMODE_ID        ((uint8)0x0EU)
#define CANIF_GET_TRCVMODEREASON_ID         ((uint8)0x0FU)
#define CANIF_SET_TRANSCEIVERWAKEMODE_ID    ((uint8)0x10U)
#define CANIF_CHECKWAKEUP_ID                ((uint8)0x11U)
#define CANIF_CHECKVALIDATION_ID            ((uint8)0x12U)
#define CANIF_TXCONFIRMATION_ID             ((uint8)0x13U)
#define CANIF_RXINDICATION_ID               ((uint8)0x14U)
#define CANIF_CANCELTXCONFIRMATION_ID       ((uint8)0x15U)
#define CANIF_CONTROLLER_BUSOFF_ID          ((uint8)0x16U)
#define CANIF_GETTXCONFIRMATIONSTATE        ((uint8)0x19U)

/* Error detection */
/** @req CANIF017 @req CANIF154 @req CANIF120  @req CANIF207 @req CANIF119 */
#define CANIF_E_PARAM_CANID                 ((uint8)0x10U)
#define CANIF_E_PARAM_DLC                   ((uint8)0x11U)
#define CANIF_E_PARAM_HRH                   ((uint8)0x12U)
#define CANIF_E_PARAM_CHANNEL               ((uint8)0x13U)
#define CANIF_E_PARAM_CONTROLLER            ((uint8)0x14U)
#define CANIF_E_PARAM_WAKEUPSOURCE          ((uint8)0x15U)
#define CANIF_E_PARAM_LPDU                  ((uint8)0x16U)
#define CANIF_E_PARAM_CONTROLLERSTATUS      ((uint8)0x17U)
#define CANIF_E_PARAM_POINTER               ((uint8)0x20U)                                                          
#define CANIF_E_UNINIT                      ((uint8)0x30U)
#define CANIF_E_NOK_NOSUPPORT               ((uint8)0x40U)    
#define CANIF_TRCV_E_TRCV_NOT_STANDBY       ((uint8)0x60U)
#define CANIF_TRCV_E_TRCV_NOT_NORMAL        ((uint8)0x70U)
#define CANIF_E_INVALID_TXPDUID             ((uint8)0x80U)
#define CANIF_E_INVALID_RXPDUID             ((uint8)0x90U)
#endif


/* mask for getting extended can id */
#define CANIF_EXTENDED_CANID_MAX            (0x1FFFFFFFU)

/* mask for getting extended can id */
#define CANIF_STANDARD_CANID_MAX            (0x7FFU)

#define CANIF_CANID_EXTEND_BIT              (0x80000000U)

#define CANIF_CANID_EXTEND_ALL_BIT          (0x9FFFF800U)

/*=======[E X T E R N A L   D A T A]==========================================*/

/* pointer to UserRxIndiaction array*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CONST(CanIfUserRxFct,CANIF_CONST) CanIf_UserRxIndication[CANIF_USER_MAX_COUNT];
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/* Name of target confirmation services to target upper layers */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CONST(CanIfUserTxFct,CANIF_CONST) CanIf_UserTxConfirmation[CANIF_USER_MAX_COUNT];/** @req CNAIF048 */ 
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#if CANIF_VARIANT_CFG == CANIF_VARIANT_PC
/* PC LT supported */

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_RXBUFFERS > 0u
extern VAR(CanIf_RxPduBufferType, CANIF_VAR) CanIf_RxBuffers[CANIF_MAX_NUMBER_OF_RXBUFFERS];
#else
extern P2VAR(CanIf_RxPduBufferType,AUTOMATIC,CANIF_VAR) CanIf_RxBuffers;
#endif
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

/* contains the configuration (parameters) of all addressed CAN controllers */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_MAX_CONTROLLER > 0u
extern CONST(CanIf_ControllerConfigType, CANIF_CONST) CanIf_ControllerConfiguration[CANIF_MAX_CONTROLLER];
#else
extern P2CONST(CanIf_ControllerConfigType,AUTOMATIC,CANIF_CONST) CanIf_ControllerConfiguration;
#endif 
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/* Callout functions with respect to the upper layers. */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CONST(CanIf_DispatchConfigType, CANIF_CONST) CanIf_DispatchConfigData;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CONST(CanIf_DriverConfigType, CANIF_CONST) CanIf_DriverConfiguration;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HRHUSED > 0u
extern CONST(CanIf_HrhConfigType, CANIF_CONST) CanIf_HrhConfigData[CANIF_NUMBER_OF_HRHUSED];
#else
extern P2CONST(CanIf_HrhConfigType,AUTOMATIC,CANIF_CONST) CanIf_HrhConfigData;
#endif
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HTHUSED > 0u
extern CONST(CanIf_HthConfigType, CANIF_CONST) CanIf_HthConfigData[CANIF_NUMBER_OF_HTHUSED];
#else
extern P2CONST(CanIf_HthConfigType,AUTOMATIC,CANIF_CONST) CanIf_HthConfigData;
#endif
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if (STD_ON == CANIF_TX_BUFFER_USED) && (CANIF_NUMBER_OF_HTHUSED > 0u)
extern VAR(uint16, CANIF_VAR) CanIf_TxBufferUsedCount[CANIF_NUMBER_OF_HTHUSED];
#endif 
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_TRANSCEIVER_TYPE > 0u
extern CONST(CanIfTrcvFctType,CANIF_CONST) CanIf_TrcvFctData[CANIF_TRANSCEIVER_TYPE];
#else
extern P2CONST(CanIfTrcvFctType,CANIF_CONST) CanIf_TrcvFctData;
#endif

#if CANIF_MAX_TRANSCEIVER > 0u
extern CONST(CanIfTransceiverDrvConfigType,CANIF_CONST) CanIf_TransceiverDrvConfigData[CANIF_MAX_TRANSCEIVER];
#else
extern P2CONST(CanIfTransceiverDrvConfigType,CANIF_CONST) CanIf_TransceiverDrvConfigData;
#endif
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif /* (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED) */

/* PC LT PB supported */

#if (STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API)
/* if CANIF_MAX_NUMBER_OF_CANTXPDUIDS is 0,define CanIf_TxNotifStatus as a pionter */
#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_CANTXPDUIDS > 0u
extern VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_TxNotifStatus[CANIF_MAX_NUMBER_OF_CANTXPDUIDS];
#else
extern P2VAR(CanIf_NotifStatusType,AUTOMATIC,CANIF_VAR) CanIf_TxNotifStatus;
#endif
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif

#if(STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)
#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_CANRXPDUIDS > 0u
extern VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_RxNotifStatus[CANIF_MAX_NUMBER_OF_CANRXPDUIDS];
#else
extern P2VAR(CanIf_NotifStatusType,AUTOMATIC,CANIF_VAR) CanIf_RxNotifStatus;
#endif
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif

/* define dynamic tx pdu Canid */
#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_DYNAMIC_CANTXPDUIDS > 0u
extern VAR(Can_IdType, CANIF_VAR) CanIf_DynamicTxPduCanIds[CANIF_MAX_NUMBER_OF_DYNAMIC_CANTXPDUIDS];
#else
extern P2VAR(Can_IdType,AUTOMATIC,CANIF_VAR) CanIf_DynamicTxPduCanIds;
#endif
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
extern CONST(CanIf_InitHohConfigType, CANIF_CONST_PBCFG) CanIf_InitHohConfig;
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_CANRXPDUIDS > 0u 
extern CONST(CanIf_RxPduConfigType, CANIF_CONST_PBCFG) CanIf_RxPduConfigData[CANIF_MAX_NUMBER_OF_CANRXPDUIDS];
#else
extern P2CONST(CanIf_RxPduConfigType,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_RxPduConfigData;
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HRHUSED > 0u
extern CONST(uint8, CANIF_CONST_PBCFG) CanIf_HrhIdConfig[CANIF_NUMBER_OF_HRHUSED];
#else
extern P2CONST(uint8,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_HrhIdConfig;
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

/** @req CANIF237 */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HRHUSED > 0u
extern CONST(CanIf_HrhRangeMaskType, CANIF_CONST_PBCFG) CanIf_HrhRangeMaskConfig[CANIF_NUMBER_OF_HRHUSED]; 
#else
extern P2CONST(CanIf_HrhRangeMaskType,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_HrhRangeMaskConfig; 
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HRHUSED > 0u
extern CONST(CanIf_HohIndexType, CANIF_CONST_PBCFG) CanIf_HrhPduIndex[CANIF_NUMBER_OF_HRHUSED];
#else
extern P2CONST(CanIf_HohIndexType,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_HrhPduIndex;
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
extern CONST(CanIf_HrhFilterConfigType, CANIF_CONST_PBCFG) CanIf_HrhFilterRefCfg;
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_MAX_NUMBER_OF_CANTXPDUIDS > 0u
extern CONST(CanIf_TxPduConfigType, CANIF_CONST_PBCFG) CanIf_TxPduConfigData[CANIF_MAX_NUMBER_OF_CANTXPDUIDS];
#else
extern P2CONST(CanIf_TxPduConfigType,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_TxPduConfigData;
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
#if CANIF_NUMBER_OF_HTHUSED > 0u
extern CONST(CanIf_HohIndexType, CANIF_CONST_PBCFG) CanIf_HthIndexCfg[CANIF_NUMBER_OF_HTHUSED];
#else
extern P2CONST(CanIf_HohIndexType,AUTOMATIC,CANIF_CONST_PBCFG) CanIf_HthIndexCfg;
#endif
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#endif /* end of #if CANIF_VARIANT_CFG == CANIF_VARIANT_PC */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*************************************************************************/
/*
 * Brief               Init CANIF module, assign global variable
 * ServiceId           0x1 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: point to static configuration table
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIfDiag_Init(P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_CONST_PBCFG) ConfigPtr);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Can interface controller initialization
 * ServiceId           0x02 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller:Can controller
 *                     ConfigurationIndex:Index number of configuration 
 *                     set
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIf_InitController(uint8 Controller, uint8 ConfigurationIndex);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Call Can driver service to change corresponding Can
 *                     controller mode, it execute initialization,to change
 *                     a Can controller to required mode
 * ServiceId           0x03 
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller:Controller to be changed 
 *                     ControllerMode:required mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType 
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllerMode(uint8 Controller, CanIf_ControllerModeType ControllerMode);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Get the mode of can controller 
 * ServiceId           0x15 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller:requested Can controller
 * Param-Name[out]     ControllerModePtr:controller mode pointer
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE)
CanIf_GetControllerMode
(
    uint8 Controller,
    P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA)ControllerModePtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Initiates a request for transmission of the CAN 
 *                     L-PDU, The corresponding CAN controller and HTH have
 *                     to be resolved by the CanTxPduId 
 * ServiceId           0x5 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId: handle of CAN L-PDU to be transmitted
 *                     PduInfo: Pointer to a structure with CAN L-PDU
 *                     related data and DLC
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE)
CanIf_Transmit
(
    PduIdType CanTxPduId, 
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_CONST) PduInfoPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (STD_ON == CANIF_READRXPDU_DATA_API)
/*************************************************************************/
/*
 * Brief               This service provides the CAN  DLC and the received
 *                     data of the
 * ServiceId           0x06 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanRxPduId: Receive L-PDU handle of CAN L-PDU
 * Param-Name[out]     *PduInfoPtr:  Pointer to a structure with CAN L-PDU 
 *                     related data DLC and pointer to CAN L-SDU buffer
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_ReadRxPduData
(
    PduIdType CanRxPduId, 
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if (STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               This service provides the status  of the static or
 *                     dynamic CAN Tx L-PDU requested by CanTxPduId
 * ServiceId           0x07 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanPduId:L-PDU handle of CAN L-PDU to be 
 *                     transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadTxNotifStatus(PduIdType CanTxPduId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if (STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief              This service provides the status of the CAN Rx L-PDU 
 *                    requested by CanRxPduId.
 * ServiceId           0x08 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanRxPduId:Receive L-PDU handle of CAN L-PDU
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadRxNotifStatus(PduIdType CanRxPduId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               This service provides the CAN  DLC and the received
 *                     data of the
 * ServiceId           0x06 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanRxPduId:Receive L-PDU handle of CAN L-PDU
 * Param-Name[out]     *PduInfoPtr:Pointer to a structure with CAN L-PDU 
 *                     related data DLC and pointer to CAN L-SDU buffer
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetPduMode(uint8 Controller, CanIf_ChannelSetModeType PduModeRequest);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               This service reports the current  mode of the 
 *                     requested Pdu channel
 * ServiceId           0x0A 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller:All PDUs of the own ECU con nected to 
 *                     the corresponding physical CAN controller are 
 *                     addressed
 * Param-Name[out]     PduModePtr:Pointer to a memory location, where 
 *                     the current mode of the logical PDU channel will be
 *                     stored
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetPduMode
(
    uint8 Controller, 
    P2VAR(CanIf_ChannelGetModeType, AUTOMATIC, CANIF_APPL_DATA) PduModePtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if ( STD_ON == CANIF_SETDYNAMICTXID_API )
/*************************************************************************/
/*
 * Brief               This service reconfigures the  corresponding CAN
 *                     identifier of the requested CAN L-PDU
 * ServiceId           0x0C
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId:L-PDU handle of CAN L-PDU for 
 *                     transmission
 *                     CanId:Standard/Extended CAN ID of CAN L-PDU that
 *                     shall be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetTransceiverMode
(    
    uint8 Transceiver,
    CanIf_TransceiverModeType TransceiverMode
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetTransceiverMode
(
    uint8 Transceiver,
    P2VAR(CanIf_TransceiverModeType, AUTOMATIC, CANIF_APPL_DATA) TransceiverModePtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetTrcvWakeupReason
(
    uint8 Transceiver,
    P2VAR(CanIf_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA)TrcvWuReasonPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetTransceiverWakeupMode
(
    uint8 Transceiver,
    CanIf_TrcvWakeupModeType TrcvWakeupMode
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif



#if (STD_ON == CANIF_WAKEUP_VALIDATION)
/*************************************************************************/
/*
 * Brief               This Service checks, whether an underlying CAN 
 *                     driver or CAN Transceiver driver already signals 
 *                     an wakeup event by the CAN network
 * ServiceId           0x11 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeUpSource:Source device, who initia ted the 
 *                     wakeup event:CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               This service is performed to validate a previous 
 *                     wakeup event. This service is called by the ECU 
 *                     Firmware
 * ServiceId           0x12 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeUpSource:Source device, who initia ted the 
 *                     wakeup event:CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif /* end of STD_ON == CANIF_WAKEUP_VALIDATION */


#if (STD_ON == CANIF_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               This service returns the version information of 
 *                     this module
 * ServiceId           0x0B 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Versioninfo: Pointer to where to store the version
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF158 @req CANIF021 */ 
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if (STD_ON == CANIF_DEV_ERROR_DETECT)
#define CanIf_GetVersionInfo(VersionInfo) \
    do{\
        if (NULL_PTR == (VersionInfo))\
        { \
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETVERSION_ID, CANIF_E_PARAM_POINTER);\
        }\
        else\
        {\
            (VersionInfo)->vendorID = CANIF_H_VENDOR_ID; \
            (VersionInfo)->moduleID = CANIF_MODULE_ID; \
            (VersionInfo)->instanceID = 0u; \
            (VersionInfo)->sw_major_version = CANIF_H_SW_MAJOR_VERSION; \
            (VersionInfo)->sw_minor_version = CANIF_H_SW_MINOR_VERSION; \
            (VersionInfo)->sw_patch_version = CANIF_H_SW_PATCH_VERSION; \
        }\
    }while(0)  
#else
#define CanIf_GetVersionInfo(VersionInfo) \
    do{\
          (VersionInfo)->vendorID = CANIF_H_VENDOR_ID; \
          (VersionInfo)->moduleID = CANIF_MODULE_ID; \
          (VersionInfo)->instanceID = 0u; \
          (VersionInfo)->sw_major_version = CANIF_H_SW_MAJOR_VERSION; \
          (VersionInfo)->sw_minor_version = CANIF_H_SW_MINOR_VERSION; \
          (VersionInfo)->sw_patch_version = CANIF_H_SW_PATCH_VERSION; \
    }while(0)  
#endif 
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif/* STD_ON == CANIF_VERSION_INFO_API */


#if (STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT) 
/*************************************************************************/
/*
 * Brief               reports if any TX confirmation has been done for the 
 *                     whole CAN controller since the last CAN controller start.
 * ServiceId           0x19 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanController:Abstracted CanIf ControllerId which
 *                                    is assigned to a CAN controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType , CANIF_CODE)
CanIf_GetTxConfirmationState(uint8 CanController);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#endif/* #ifndef _CANIF_H */



/*=======[E N D   O F   F I L E]==============================================*/

