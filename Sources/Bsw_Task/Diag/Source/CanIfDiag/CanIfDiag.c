/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf.c>
 *  @brief      <CanIf>
 *  
 *  <Compiler: CodeWarrior2.8 MCU:MPC5634>
 *  
 *  @author     <Tommy>
 *  @date       <2013-09-27> 
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130425   liujn      Initial version
 
 *  V1.0.1       20130627   Tommy      these functions not realized in this version:
                                       1.multiple Can Drivers not supported
                                       2.Can transceiver Driver not supported  
                                       
 *  V1.0.2       20130927   Tommy      when CANIF_DLC_CHECK disabled,use the current 
                                       received DLC value,do not use the expected dlc
                                       which set during congfiguration.CANIF296,CANIF262 
                                       
 *  V1.0.3       20131010   Tommy      CANIF add a wakeup source in CanIf_ControllerConfigType,
                                       useful for wakeup source check and independent from CAN  
                                       Driver. update CanIf_CheckWakeup & CanIf_CheckValidation 
                                       
 *  V1.0.4       20140106   Tommy      CanIf add SchM_Enter_CanIf and SchM_Exit_CanIf when doing 
                                       Mode set or change. It's important because we find a bug
                                       in ChangAn S401NM Project which not do critical protect.
                                       
 *  V1.0.5       20140117   Tommy      CanIf_TxPduConfigType add CanIfCanTxPduId,this item indi-
                                       cates upper layer txPdu handle,that's upper layer config 
                                       index.It saves search time,useful for Txconfirmation.
                                       
 *  V1.0.6       20140424   Tommy      Add CanIf_FirstCallRxInd for CanIf_CheckValidation used.
                                       update CanIf_ReadTxNotifStatus,CanIf_ReadRxNotifStatus. 

 *  V1.0.7       20140811   Tommy      Add CanIf UpperLayer - Applicaiton according to FAW reqs.
                                       Add Can transceiver Driver support.
                                       Add Binary Search Algorithm for Hrh Search and RxPdu search.

 *  V1.0.8       20141225   Tommy      Add CanIf_TxBufferUsedCount for Txbuffer Management in order
                                       to reduce the process time in CanIf_TxConfirmation. If this
                                       count is zero,then no need to polling txbuffer.
 */

/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * Msg(4:0491) Array subscripting applied to an object of pointer type.
 */
/* MISRA-C:2004 Rule 19.7,
 * Msg(4:3453) A function could probably be used instead of this function-like macro.
 */

/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_C_AR_MAJOR_VERSION   3U
#define CANIF_C_AR_MINOR_VERSION   2U
#define CANIF_C_AR_PATCH_VERSION   0U
#define CANIF_C_SW_MAJOR_VERSION   1U
#define CANIF_C_SW_MINOR_VERSION   0U
#define CANIF_C_SW_PATCH_VERSION   8U


/*=======[I N C L U D E S]====================================================*/
#include "CanIfDiag.h" /** @req CANIF122 */

#if STD_ON == CANIF_DEM_ERROR_DETECT
#include "Dem.h" /** @req CANIF150 */
#endif

#if STD_ON == CANIF_DEV_ERROR_DETECT
#include "Det.h" /** @req CANIF279 */
#endif

#include "CanIfDiag_Cbk.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if(CANIF_C_AR_MAJOR_VERSION != CANIF_H_AR_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_C_AR_MINOR_VERSION != CANIF_H_AR_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif

#if(CANIF_C_AR_PATCH_VERSION != CANIF_H_AR_PATCH_VERSION)
    #error "CanIf.c:Mismatch in Specification Patch Version"
#endif

#if(CANIF_C_SW_MAJOR_VERSION != CANIF_H_SW_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_C_SW_MINOR_VERSION != CANIF_H_SW_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif

#if STD_ON == CANIF_DEM_ERROR_DETECT
#if( 3u != DEM_H_AR_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if( 1u != DEM_H_AR_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif
#endif /* end of STD_ON == CANIF_DEM_ERROR_DETECT */

#if STD_ON == CANIF_DEV_ERROR_DETECT
#if( 2u != DET_H_AR_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if( 2u != DET_H_AR_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif
#endif /* end of STD_ON == CANIF_DEV_ERROR_DETECT */

#if( 3u != CANIF_CBK_H_AR_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if( 2u != CANIF_CBK_H_AR_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif


/*=======[M A C R O S]========================================================*/
#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
#define CANIF_NUMBER_OF_CANRXPDUIDS         CanIf_ConfigStd->CanIfNumberOfCanRxPduIds
#define CANIF_NUMBER_OF_CANTXPDUIDS         CanIf_ConfigStd->CanIfNumberOfCanTXPduIds
#define CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS CanIf_ConfigStd->CanIfNumberOfDynamicCanTXPduIds
#define CANIF_NUMBER_OF_HTH_CFG             CanIf_NumberOfHthUsed
#define CANIF_NUMBER_OF_HRH_CFG             CanIf_NumberOfHrhUsed

#elif CANIF_VARIANT_CFG == CANIF_VARIANT_LT
#define CANIF_NUMBER_OF_CANRXPDUIDS         CanIf_NumberOfCanRxPduIds
#define CANIF_NUMBER_OF_CANTXPDUIDS         CanIf_NumberOfCanTXPduIds
#define CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS CanIf_NumberOfDynamicCanTXPduIds
#define CANIF_NUMBER_OF_HTH_CFG             CanIf_NumberOfHthUsed
#define CANIF_NUMBER_OF_HRH_CFG             CanIf_NumberOfHrhUsed

#elif CANIF_VARIANT_CFG == CANIF_VARIANT_PC
#define CANIF_NUMBER_OF_CANRXPDUIDS         CANIF_MAX_NUMBER_OF_CANRXPDUIDS
#define CANIF_NUMBER_OF_CANTXPDUIDS         CANIF_MAX_NUMBER_OF_CANTXPDUIDS
#define CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS CANIF_MAX_NUMBER_OF_DYNAMIC_CANTXPDUIDS
#define CANIF_NUMBER_OF_HTH_CFG             CANIF_NUMBER_OF_HTHUSED
#define CANIF_NUMBER_OF_HRH_CFG             CANIF_NUMBER_OF_HRHUSED
#endif

/* pointer to Can Controller Config Table */
#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
#define CANIF_CONTROLLER_REF_CONFIGSET(controller)  CanIf_ConfigStd->CanIfRefConfigSet->CanIfRefConfigSetRef[controller]
#else 
#define CANIF_CONTROLLER_REF_CONFIGSET(controller)  CanIf_InitHohConfig.CanIfRefConfigSetRef[controller]
#endif


#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
#define CANIF_CANRXPDUID_CANID(rxPduId)         CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfCanRxPduCanId
#define CANIF_CANRXPDUID_DLC(rxPduId)           CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfCanRxPduDlc
#define CANIF_CANRXPDUID(rxPduId)               CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfCanRxPduId
#define CANIF_READRXPDU_DATA(rxPduId)           CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfReadRxPduData
#define CANIF_RXBUFFERINDEX(rxPduId)            CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfRxBufferIndex
#define CANIF_READRXPDU_NOTIFY_STATUS(rxPduId)  CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfReadRxPduNotifyStatus
#define CANIF_RXNOTIFYINDEX(rxPduId)            CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfRxNotifyIndex
#define CANIF_CANRXPDUID_CANIDTYPE(rxPduId)     CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfRxPduIdCanIdType
#define CANIF_RX_USER_TYPE(rxPduId)             CanIf_ConfigStd->CanIfRxPduConfigRef[rxPduId].CanIfRxUserType

#define CANIF_RXRANGEMASK(hrhIndex)             CanIf_ConfigStd->CanIf_HrhFilterRef->CanIfHrhRangeMask[hrhIndex].hrhRangeMask
#define CanIf_CANIDMASK(hrhIndex)               CanIf_ConfigStd->CanIf_HrhFilterRef->CanIfHrhRangeMask[hrhIndex].canIdMask
#define CANIF_HRHIDSYMREF(hrhIndex)             CanIf_ConfigStd->CanIf_HrhFilterRef->CanIfHrhIdSymRef[hrhIndex]
#define CANIF_HRH_PDUID_STARTINDEX(hrhIndex)    CanIf_ConfigStd->CanIf_HrhFilterRef->CanIfHrhPduIndexRef[hrhIndex].startIndex
#define CANIF_HRH_PDUID_STOPINDEX(hrhIndex)     CanIf_ConfigStd->CanIf_HrhFilterRef->CanIfHrhPduIndexRef[hrhIndex].stopIndex

#else
#define CANIF_CANRXPDUID_CANID(rxPduId)         CanIf_RxPduConfigData[rxPduId].CanIfCanRxPduCanId
#define CANIF_CANRXPDUID_DLC(rxPduId)           CanIf_RxPduConfigData[rxPduId].CanIfCanRxPduDlc
/* the target rxpdu handle */
#define CANIF_CANRXPDUID(rxPduId)               CanIf_RxPduConfigData[rxPduId].CanIfCanRxPduId
#define CANIF_READRXPDU_DATA(rxPduId)           CanIf_RxPduConfigData[rxPduId].CanIfReadRxPduData
#define CANIF_RXBUFFERINDEX(rxPduId)            CanIf_RxPduConfigData[rxPduId].CanIfRxBufferIndex
#define CANIF_READRXPDU_NOTIFY_STATUS(rxPduId)  CanIf_RxPduConfigData[rxPduId].CanIfReadRxPduNotifyStatus
#define CANIF_RXNOTIFYINDEX(rxPduId)            CanIf_RxPduConfigData[rxPduId].CanIfRxNotifyIndex
#define CANIF_CANRXPDUID_CANIDTYPE(rxPduId)     CanIf_RxPduConfigData[rxPduId].CanIfRxPduIdCanIdType
#define CANIF_RX_USER_TYPE(rxPduId)             CanIf_RxPduConfigData[rxPduId].CanIfRxUserType

#define CANIF_RXRANGEMASK(hrhIndex)             CanIf_HrhRangeMaskConfig[hrhIndex].hrhRangeMask
#define CanIf_CANIDMASK(hrhIndex)               CanIf_HrhRangeMaskConfig[hrhIndex].canIdMask
#define CANIF_HRHIDSYMREF(hrhIndex)             CanIf_HrhIdConfig[hrhIndex]
#define CANIF_HRH_PDUID_STARTINDEX(hrhIndex)    CanIf_HrhPduIndex[hrhIndex].startIndex
#define CANIF_HRH_PDUID_STOPINDEX(hrhIndex)     CanIf_HrhPduIndex[hrhIndex].stopIndex

#endif

#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
#define CANIF_CANTXPDUID_CANID(txPduId)         CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfCanTxPduIdCanId
#define CANIF_CANTXPDUID_DLC(txPduId)           CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfCanTxPduIdDlc
/* the target txpdu handle */
#define CANIF_CANTXPDUID(txPduId)               CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfCanTxPduId
#define CANIF_CANTXPDUID_TYPE(txPduId)          CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfCanTxPduType
#define CANIF_DYNAMICTXPDUCANIDINDEX(txPduId)   CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfDynamicTxPduCanIdIndex
#define CANIF_READTXPDU_NOTIFY_STATUS(txPduId)  CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfReadTxPduNotifyStatus
#define CANIF_TXNOTIFYINDEX(txPduId)            CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfTxNotifyIndex
#define CANIF_TXPDUIDCANIDTYPE(txPduId)         CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfTxPduIdCanIdType
#define CANIF_TX_USER_TYPE(txPduId)             CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfTxUserType
#define CANIF_HTH_REF_ID(txPduId)               CanIf_ConfigStd->CanIfTxPduConfigRef[txPduId].CanIfCanTxPduHthRef
#define CANIF_HTHSTARTINDEX(hthConfigIndex)     CanIf_ConfigStd->CanIfHthIndexConfigRef[hthConfigIndex].startIndex
#define CANIF_HTHSTOPINDEX(hthConfigIndex)      CanIf_ConfigStd->CanIfHthIndexConfigRef[hthConfigIndex].stopIndex
#else
#define CANIF_CANTXPDUID_CANID(txPduId)         CanIf_TxPduConfigData[txPduId].CanIfCanTxPduIdCanId
#define CANIF_CANTXPDUID_DLC(txPduId)           CanIf_TxPduConfigData[txPduId].CanIfCanTxPduIdDlc
#define CANIF_CANTXPDUID(txPduId)               CanIf_TxPduConfigData[txPduId].CanIfCanTxPduId
#define CANIF_CANTXPDUID_TYPE(txPduId)          CanIf_TxPduConfigData[txPduId].CanIfCanTxPduType
#define CANIF_DYNAMICTXPDUCANIDINDEX(txPduId)   CanIf_TxPduConfigData[txPduId].CanIfDynamicTxPduCanIdIndex
#define CANIF_READTXPDU_NOTIFY_STATUS(txPduId)  CanIf_TxPduConfigData[txPduId].CanIfReadTxPduNotifyStatus
#define CANIF_TXNOTIFYINDEX(txPduId)            CanIf_TxPduConfigData[txPduId].CanIfTxNotifyIndex
#define CANIF_TXPDUIDCANIDTYPE(txPduId)         CanIf_TxPduConfigData[txPduId].CanIfTxPduIdCanIdType
#define CANIF_TX_USER_TYPE(txPduId)             CanIf_TxPduConfigData[txPduId].CanIfTxUserType
#define CANIF_HTH_REF_ID(txPduId)               CanIf_TxPduConfigData[txPduId].CanIfCanTxPduHthRef
#define CANIF_HTHSTARTINDEX(hthConfigIndex)     CanIf_HthIndexCfg[hthConfigIndex].startIndex
#define CANIF_HTHSTOPINDEX(hthConfigIndex)      CanIf_HthIndexCfg[hthConfigIndex].stopIndex
#endif


#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
#define CANIF_TRCV_WAKEUP_NOTIFICATION(canTrcvIndex)  CanIf_TransceiverDrvConfigData[canTrcvIndex].CanIfTrcvWakeupNotification
#define CANIF_TRCV_ID(canTrcvIndex)                   CanIf_TransceiverDrvConfigData[canTrcvIndex].CanIfTrcvIdRef
#define CANIF_TRCV_FCT_INDEX(canTrcvIndex)            CanIf_TransceiverDrvConfigData[canTrcvIndex].CanIfTrcvFctIndex
#endif

/*=======[I N T E R N A L   D A T A]==========================================*/
#define CANIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
/* Canif init status, at first define it as CANIF_UNINIT */
STATIC VAR(CanIf_InitStatusType, CANIF_VAR_POWER_ON_INIT) CanIf_InitStatus = CANIF_UNINIT;
#define CANIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
/* define Tx Buffers */
#if (STD_ON == CANIF_TX_BUFFER_USED) 
STATIC VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer[CANIF_NUMBER_OF_TXBUFFERS];
#endif 
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
STATIC VAR(CanIf_ControllerModeType, CANIF_VAR) CanIf_ControllerMode[CANIF_MAX_CONTROLLER];
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
STATIC VAR(CanIf_ChannelGetModeType, CANIF_VAR) CanIf_PduMode[CANIF_MAX_CONTROLLER];
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
#if (STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT) 
STATIC VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_TxConfirmationState[CANIF_MAX_CONTROLLER];
#endif 
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#if CANIF_VARIANT_CFG == CANIF_VARIANT_PB
STATIC P2CONST(CanIf_ConfigType, CANIF_CONST, CANIF_CONST_PBCFG) CanIf_ConfigStd;
#endif
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_BOOLEAN
#include "CanIf_MemMap.h"
#if STD_ON == CANIF_WAKEUP_VALIDATION
STATIC VAR(boolean,CANIF_VAR) CanIf_FirstCallRxInd[CANIF_MAX_CONTROLLER];
#endif
#define CANIF_STOP_SEC_VAR_BOOLEAN
#include "CanIf_MemMap.h"


/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllStarted(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllSleep(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllStopped(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeBufInStopped(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetRxOffline
(
    CanIf_ChannelGetModeType oldPduMode, 
    uint8 controller
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetRxOnline
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOffline
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOnline
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOfflineActive
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"



#if (STD_ON == CANIF_TX_BUFFER_USED)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Functions declared for Txbuffer */
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeControllerTxBuffer(uint8 controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_OverWritePduInTxBuffer
( 
    PduIdType txBufIndex,
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Can_ReturnType, CANIF_CODE)
CanIf_SendTxBufferPdu
(
    PduIdType txBufIndex,
    uint8 hth
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* each txPdu has a txBuffer */
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SavePduToTxMaxBuffer
(  
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_FindHighestPrioPduInTxMaxBuffer
(
    PduIdType  txPduId, 
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) bufferIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_TxMaxBufferBusyHandle
(
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_TRANSMIT_CANCELLATION)
#if (CANIF_NUMBER_OF_TXBUFFERS >= CANIF_NUMBER_OF_CANTXPDUIDS)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_TxMaxBufferCancelHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SavePduToTxBuffer
(  
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_FindHighestPrioPduInTxBuffer
(
    PduIdType  txPduId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) bufferIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_TxBusyHandle
(
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (CANIF_NUMBER_OF_TXBUFFERS < CANIF_NUMBER_OF_CANTXPDUIDS)
#if (STD_ON == CANIF_TRANSMIT_CANCELLATION)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_TxBufferCancelHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
#endif

#endif /* STD_ON == CANIF_TX_BUFFER_USED */ 


#if (STD_ON == CANIF_READRXPDU_DATA_API)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Functions declared for Rxbuffer */
STATIC FUNC(void, CANIF_CODE)
CanIf_InitRxBuffer(uint8 controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if(STD_ON == CANIF_SETDYNAMICTXID_API)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Functions declared for CanIf_DynamicTxPduCanIds */
STATIC FUNC(void, CANIF_CODE)
CanIf_InitDynamicTxPduCanIds(void);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Memcpy and Memset for CanIf Mode used */
STATIC FUNC(void, CANIF_CODE)
CanIf_Memcpy
(
    P2VAR(uint8, AUTOMATIC, CANIF_APPL_DATA) dest,
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) source,
    uint32 length   
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_READRXPDU_DATA_API)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_Memset
(
    P2VAR(uint8, AUTOMATIC, CANIF_APPL_DATA) dest,
    const uint8 source,
    uint32 length
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_CanIdRangCheck
(
    uint8 hrhId, 
    Can_IdType canId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) pduIdIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_PduPack
(
    PduIdType CanTxPduId, 
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
    P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) canPdu
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_RxPduHrhSearch
(
    uint8 hrhIndex,
    Can_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_BINARY) 
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_BinSearch_PduIndex
(
    uint8 lowValue,
    uint8 highValue,
    Can_IdType key_CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC)pduIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_BinSearch_HrhIndex
(
    uint8 lowValue,
    uint8 highValue,
    uint8 key_HrhId,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC)hrhIndex
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_RxIndicationHandle
(
    PduIdType pduIdIndex,
    uint8 pduLength,
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) CanSduPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_WAKEUP_VALIDATION)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeupAction
(
    uint8 controller,
    EcuM_WakeupSourceType WakeupSource
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeupValidationAction
(
    uint8 canController,
    EcuM_WakeupSourceType WakeupSource
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* end of STD_ON == CANIF_WAKEUP_VALIDATION */

#if((STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)||(STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeRxTxNotifyStatus(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if(STD_ON == CANIF_READRXPDU_DATA_API)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_ReadRxPduDataHandle
(
    uint8 hrhIndex,
    PduIdType rxCfgId,
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
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
/** @req CANIF001 @req CANIF041 @reqCANIF032 @req CANIF 085 @req CANIF032 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIfDiag_Init(P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_CONST_PBCFG) ConfigPtr)
{
    uint8   canLoop;
    boolean funGoFlag = TRUE;

    #if (CANIF_VARIANT_CFG == CANIF_VARIANT_PB)
    #if (STD_ON == CANIF_DEV_ERROR_DETECT) /** @req CANIF019 */

    /* for post build,report error if NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        /** @req CANIF156 */
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_PARAM_POINTER);
        funGoFlag = FALSE;
    }
    else
    #endif
    {
        /* save the configuration to global variable */
        CanIf_ConfigStd = ConfigPtr;
    }
   
    if (TRUE == funGoFlag)
    #endif
    {   
        /* CanIf Module has not been initialized */
        if (CANIF_INITED != CanIf_InitStatus)
        {
            /* Set CanIf_Init Status to CANIF_INITED*/
            CanIf_InitStatus = CANIF_INITED;

            /* set controller mode to STOPPED */ 
            /** @req CANIF086 @req CANIF092 */
            for (canLoop = 0u; canLoop < CANIF_MAX_CONTROLLER; canLoop++)
            {
                CanIf_ControllerMode[canLoop] = CANIF_CS_STOPPED;
            }
        }      
        else /**< CanIf Module has been initialized */
        {            
            for (canLoop = 0u; canLoop < CANIF_MAX_CONTROLLER; canLoop++)
            {
                /* check the status */
                if (CANIF_CS_STOPPED != CanIf_ControllerMode[canLoop])
                {  
                    funGoFlag = FALSE; 
                }
            }  
        }       
        if(TRUE == funGoFlag)
        { 
            /* Call CanIf_InitController to init controller */
            for (canLoop = 0u; canLoop < CANIF_MAX_CONTROLLER; canLoop++)
            {
                #if STD_ON == CANIF_WAKEUP_VALIDATION
                /* Init the first call rxindication event flag */
                CanIf_FirstCallRxInd[canLoop] = FALSE;
                #endif
                
                /* only define one cofig item per controller,so configindex is 0 */
                CanIf_InitController(canLoop, 0u);
            }
        }
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Can interface controller initialization
 * ServiceId           0x02 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: Can controller
 *                     ConfigurationIndex: Index number of configuration 
 *                     set
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can_Init
 */
/*************************************************************************/
/** @req CANIF002 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_InitController(uint8 Controller, uint8 ConfigurationIndex)
{
    
    /** @req CANIF022 */
    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;

    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_CONTROLLER_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    #if (CANIF_VARIANT_CFG == CANIF_VARIANT_PB)
    /* check whether ConfigurationIndex is over range, configIndex is 0 when no post build */
    if ((TRUE == detNoErr) && (ConfigurationIndex > 0u))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_CONTROLLER_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    /*check wheter Controller ID is right */
    if ((TRUE == detNoErr) &&(Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_CONTROLLER_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;
    }
    #endif
    /** @req CANIF092 */
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        uint8 controllerId;    
        /* define Can controller configuration variable */
        P2CONST(Can_ControllerConfigType, AUTOMATIC, CANIF_APPL_CONST) canConfig;

        /* get Can controller Id */
        controllerId = CanIf_ControllerConfiguration[Controller].CanIfControllerIdRef;

        /* get Can driver configuration from CanIf configuration structure */
        canConfig = &CANIF_CONTROLLER_REF_CONFIGSET(controllerId);

        if (CANIF_CS_STARTED == CanIf_ControllerMode[Controller])
        {
            /* If initialization is performed in STARTED mode, the CAN Interface will 
             * perform the transition to STOPPED mode.
             */
            if (E_OK == CanIf_SetControllerMode(Controller,CANIF_CS_STOPPED))
            {
                /* Call Can Driver InitController Function */
                Can_InitController(controllerId, canConfig);
            }
        } 
        else if (CANIF_CS_STOPPED == CanIf_ControllerMode[Controller])
        {          
            /* Re-init L-PDU buffers of corresponding controller */
            /** @req CANIF293 @req CANIF085*/
            CanIf_FreeBufInStopped(Controller);

            /* Set Pdu Mode to OffLine */
            CanIf_PduMode[Controller] = CANIF_GET_OFFLINE;

            /* Call Can Driver InitController Function */
            Can_InitController(controllerId, canConfig);
        }
        else
        {
            /* Neither CANIF_CS_STARTED nor CANIF_CS_STOPPED Mode */
        }
    }    
    return;
}
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
 * Param-Name[in]      Controller: Controller to be changed 
 *                     ControllerMode: required mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType 
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF003 @req CANIF049 @req CANIF059 @req CANIF089 @req CANIF090 @req CANIF170 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllerMode(uint8 Controller, CanIf_ControllerModeType ControllerMode)
{
    Std_ReturnType result = E_NOT_OK;

    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    #if (CANIF_VARIANT_CFG == CANIF_VARIANT_PB)
    if ((TRUE == detNoErr) && (Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;    
    }
    #endif /* (CANIF_VARIANT_CFG == CANIF_VARIANT_PB) */
    if(TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* check the ControllerMode to be set*/
        /** @req CANIF140 @req CANIF105 @req CANIF169 @req CANIF081 */
        switch (ControllerMode)
        {
            case CANIF_CS_STARTED: /** @req CANIF215 */
                result = CanIf_SetControllStarted(Controller);
                break;
                
            case CANIF_CS_SLEEP: /** @req CANIF216 @req CANIF079 @req CANIF227 */
                result = CanIf_SetControllSleep(Controller);
                break;
                
            case CANIF_CS_STOPPED: /** @req CANIF214 */
                result = CanIf_SetControllStopped(Controller);
                break;
                
            /* invalid controller mode */
            default:
                result = E_NOT_OK;
                break;
        }
    }    
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Get the mode of can controller 
 * ServiceId           0x4 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: requested Can controller
 * Param-Name[out]     ControllerModePtr: controller mode pointer
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF229 @req CANIF093 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetControllerMode
(
    uint8 Controller,
    P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA)ControllerModePtr
)
{
    Std_ReturnType result = E_NOT_OK;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_CONTROLLER_MODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }

    /* check whether controller is over range */
    if((TRUE == detNoErr) &&(Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;    
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* get current mode */
        *ControllerModePtr = CanIf_ControllerMode[Controller];
        result = E_OK;
    }
    return result;
}
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
/** @req CANIF005 @req CANIF160 @req CANIF077 @req CANIF024*/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_Transmit
(
    PduIdType CanTxPduId, 
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
)
{
    Can_ReturnType retVal;    
    Std_ReturnType result = E_NOT_OK;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */ 
    /** @req CANIF239 */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /* check NULL_PTR */
    if ((TRUE == detNoErr) && ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr)))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (CanTxPduId >= CANIF_NUMBER_OF_CANTXPDUIDS))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TRANSMIT_ID, CANIF_E_INVALID_TXPDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        uint8 hthConfigIndex;
        uint8 canControllerId; 
        
        /* Get the hth config Index */
        hthConfigIndex = CANIF_HTH_REF_ID(CanTxPduId);

        /* get controller ID */
        canControllerId = CanIf_HthConfigData[hthConfigIndex].CanIfCanControllerIdRef;

        /* check the controller mode */
        /** @req CANIF080 */
        if (CANIF_CS_STARTED == CanIf_ControllerMode[canControllerId])
        {
            if ((CANIF_GET_TX_ONLINE == CanIf_PduMode[canControllerId]) 
                 || (CANIF_GET_ONLINE == CanIf_PduMode[canControllerId]))
            {
                /**< cotroller and pdu mode are all allowed*/
                Can_PduType canPdu;
                
                CanIf_PduPack(CanTxPduId,PduInfoPtr,&canPdu);
                /** @req CANIF023 @req CANIF043 @req CANIF044 @req CANIF292 @req CANIF161 */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                /* call lower layer CAN driver API,and get return value */
                retVal = Can_Write(CanIf_HthConfigData[hthConfigIndex].CanIfHthIdSymRef, &canPdu);
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                
                if (CAN_OK == retVal)
                {
                    result = E_OK;
                }
                /** @req CANIF163 @req CANIF082 @req CANIF103 @req CANIF113 @req CANIF175 */
                else if (CAN_BUSY == retVal)
                {
                    /* if CAN_BUSY and buffer enabled, do */
                    #if (STD_ON == CANIF_TX_BUFFER_USED)
                    if (CANIF_NUMBER_OF_TXBUFFERS < CANIF_NUMBER_OF_CANTXPDUIDS)
                    {
                        result = CanIf_TxBusyHandle(&canPdu);
                    }
                    else
                    {
                        result = CanIf_TxMaxBufferBusyHandle(&canPdu);
                    }
                    #endif
                }
                else
                {
                    result = E_NOT_OK;
                }
            }
            else/**< Pdu Mode not Tx Online */
            {
                if ((CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE == CanIf_PduMode[canControllerId])
                    || (CANIF_GET_OFFLINE_ACTIVE == CanIf_PduMode[canControllerId]))
                {  
                    /* check notify enable */
                    #if (STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API)
                    if (TRUE == CANIF_READTXPDU_NOTIFY_STATUS(CanTxPduId))
                    {
                        /* change status of the coressponding CanIf_TxNotifStatus */
                        CanIf_TxNotifStatus[CANIF_TXNOTIFYINDEX(CanTxPduId)] = CANIF_TX_RX_NOTIFICATION;                        
                    }
                    #endif
                    if ((CANIF_TX_USER_TYPE(CanTxPduId) < CANIF_USER_MAX_COUNT) 
                        &&(NULL_PTR != CanIf_UserTxConfirmation[CANIF_TX_USER_TYPE(CanTxPduId)])
                       )    
                    {                   
                        /* call call-back function,feedback to upper layer */
                        CanIf_UserTxConfirmation[CANIF_TX_USER_TYPE(CanTxPduId)](CanTxPduId);
                        result = E_OK;
                    }
                }
            }
        }
        else /**< Not CANIF_CS_STARTED mode */
        {
            if (CANIF_CS_STOPPED == CanIf_ControllerMode[canControllerId])
                {
                    #if STD_ON == CANIF_DEM_ERROR_DETECT
                    /** @req CANIF020 @req CANIF223 @req CANIF161 */
                    Dem_ReportErrorStatus(CANIF_E_STOPPED, DEM_EVENT_STATUS_FAILED);
                    #endif
                }
        }
           
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if(STD_ON == CANIF_READRXPDU_DATA_API)
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
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_ReadRxPduData
(
    PduIdType CanRxPduId, 
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
)
{
    Std_ReturnType result = E_NOT_OK;
    
    /** @req CANIF194*/
    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;

    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXPDUDATA_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /* check NULL PTR*/
    if ((TRUE == detNoErr) && ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr)))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXPDUDATA_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;    
    }
    if ((TRUE == detNoErr) && (CanRxPduId >= CANIF_NUMBER_OF_CANRXPDUIDS))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXPDUDATA_ID, CANIF_E_INVALID_RXPDUID);
        detNoErr = FALSE;    
    }
    if (TRUE == detNoErr)
    #endif
    {
        uint8 hrhIndex;
        uint8 hrhIdOfPdu;
        boolean hrhFind = FALSE;
        
        /* find the hrhIndex by a valid rxCfgIdx */
        for (hrhIndex = 0u; 
            (hrhIndex < CANIF_NUMBER_OF_HRH_CFG) && (FALSE == hrhFind);
            hrhIndex++)
        {
            if ((CanRxPduId >= CANIF_HRH_PDUID_STARTINDEX(hrhIndex))
               && (CanRxPduId <= CANIF_HRH_PDUID_STOPINDEX(hrhIndex)))
            {
                /* get the hrhId of this pdu used */
                hrhIdOfPdu = hrhIndex;
                hrhFind = TRUE;  
            }
        }
        if (TRUE == hrhFind)
        {
            result = CanIf_ReadRxPduDataHandle(hrhIdOfPdu,CanRxPduId,PduInfoPtr);   
        }                        
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_READRXPDU_DATA_API */


/** @req CANIF203 */
#if(STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               This service provides the status  of the static or
 *                     dynamic CAN Tx L-PDU requested by CanTxPduId
 * ServiceId           0x07 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanPduId: L-PDU handle of CAN L-PDU to be 
 *                     transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF202 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadTxNotifStatus(PduIdType CanTxPduId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READTXNOTIFSTATUS_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (CanTxPduId >= CANIF_NUMBER_OF_CANTXPDUIDS))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READTXNOTIFSTATUS_ID, CANIF_E_INVALID_TXPDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    #endif      
    {
        /** @req CANIF200 */
        if (TRUE == CANIF_READTXPDU_NOTIFY_STATUS(CanTxPduId))
        {
            result = CanIf_TxNotifStatus[CANIF_TXNOTIFYINDEX(CanTxPduId)]; 
            
            /* clean STATUS */ 
            /** @req CANIF205 */
            CanIf_TxNotifStatus[CANIF_TXNOTIFYINDEX(CanTxPduId)] = CANIF_NO_NOTIFICATION;
        }
        else
        {
            #if(STD_ON == CANIF_DEV_ERROR_DETECT)
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READTXNOTIFSTATUS_ID, CANIF_E_INVALID_TXPDUID);
            #endif
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API */


/*************************************************************************/
/*
 * Brief               This service provides the status of the CAN Rx L-PDU
 *                     requested by CanRxPduId
 * ServiceId           0x08 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId: L-PDU handle of CAN L-PDU to be received
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF230 */
#if(STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadRxNotifStatus(PduIdType CanRxPduId)
{    
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    #if (STD_OFF == CANIF_READRXPDU_DATA_API)
    if (TRUE == detNoErr) 
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        detNoErr = FALSE;
    }
    #endif
    if ((TRUE == detNoErr) && (CanRxPduId >= CANIF_NUMBER_OF_CANRXPDUIDS))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        detNoErr = FALSE;    
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {       
        if (TRUE == CANIF_READRXPDU_NOTIFY_STATUS(CanRxPduId))
        {
            result = CanIf_RxNotifStatus[CANIF_RXNOTIFYINDEX(CanRxPduId)];

            /* clean STATUS */
            CanIf_RxNotifStatus[CANIF_RXNOTIFYINDEX(CanRxPduId)] = CANIF_NO_NOTIFICATION;
        }
        else
        {
            #if(STD_ON == CANIF_DEV_ERROR_DETECT)
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
            #endif
        }

    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API */


/*************************************************************************/
/*
 * Brief               This service sets the requested mo de at all L-PDUs
 *                     of the predefined logical PDU channel
 * ServiceId           0x09 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: All PDUs of the own ECU connected to the
 *                     orresponding physical CAN controller are addressed
 *                     PduModeRequest: Requested PDU mode change
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer 
 */
/*************************************************************************/
/** @req CANIF008 @req CANIF047 @req CANIF060 @req CANIF088 
    @req CANIF217 @req CANIF027 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetPduMode(uint8 Controller, CanIf_ChannelSetModeType PduModeRequest)
{
    Std_ReturnType result = E_NOT_OK;
    CanIf_ChannelGetModeType oldPduMode;
    
    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETPDUMODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETPDUMODE_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;    
    }    
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* @req CANIF027 */
        if (CANIF_CS_STARTED == CanIf_ControllerMode[Controller])
        {
            oldPduMode = CanIf_PduMode[Controller];

            switch(PduModeRequest)
            {
                case CANIF_SET_OFFLINE: /** @req CANIF073 @req CANIF118 */
                    /* set mode to CANIF_GET_OFFLINE */
                    CanIf_PduMode[Controller] = CANIF_GET_OFFLINE;
                    result = E_OK;
                    break;

                case CANIF_SET_RX_OFFLINE:
                    CanIf_SetRxOffline(oldPduMode, Controller);
                    result = E_OK;
                    break;

                case CANIF_SET_RX_ONLINE: /** @req CANIF096 */
                    CanIf_SetRxOnline(oldPduMode, Controller);
                    result = E_OK;
                    break;

                case CANIF_SET_TX_OFFLINE:
                    CanIf_SetTxOffline(oldPduMode, Controller);
                    result = E_OK;
                    break;

                case CANIF_SET_TX_ONLINE:
                    CanIf_SetTxOnline(oldPduMode, Controller);
                    result = E_OK;
                    break;

                case CANIF_SET_ONLINE: /** @req CANIF074 @req CANIF075 */
                    /* set pdu mode to CANIF_GET_ONLINE */
                    CanIf_PduMode[Controller] = CANIF_GET_ONLINE;
                    result = E_OK;
                    break;

                case CANIF_SET_TX_OFFLINE_ACTIVE: /** @req CANIF072 */
                    CanIf_SetTxOfflineActive(oldPduMode, Controller);
                    result = E_OK;
                    break;

                default:
                    /* invalid pdu mode*/
                    result = E_NOT_OK;
                    break;
            }/* end of switch(PduModeRequest)*/
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               This service reports the current  mode of the 
 *                     requested Pdu channel
 * ServiceId           0x0A 
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: All PDUs of the own ECU con nected to 
 *                     the corresponding physical CAN controller are 
 *                     addressed
 * Param-Name[out]     PduModePtr: Pointer to a memory location, where 
 *                     the current mode of the logical PDU channel will be
 *                     stored
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF009 @req CANIF095 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetPduMode
(
    uint8 Controller, 
    P2VAR(CanIf_ChannelGetModeType, AUTOMATIC, CANIF_APPL_DATA) PduModePtr
)
{
    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    #endif
    Std_ReturnType result = E_NOT_OK;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETPDUMODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)&&(Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETPDUMODE_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;    
    }
        /* check NULL PTR*/
        if (NULL_PTR == PduModePtr)
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETPDUMODE_ID, CANIF_E_PARAM_POINTER);
            detNoErr = FALSE;
        }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
        {
            /* get current pdu mode */
            *PduModePtr = CanIf_PduMode[Controller];
            result = E_OK;
        }
        return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if(STD_ON == CANIF_SETDYNAMICTXID_API)
/*************************************************************************/
/*
 * Brief               This service reconfigures the  corresponding CAN
 *                     identifier of the requested CAN L-PDU
 * ServiceId           0x0C
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId: L-PDU handle of CAN L-PDU for 
 *                     transmission
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that
 *                     shall be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF189 @req CANIF185 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId)
{
    PduIdType dynamicTxIndex;
    
    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    boolean canIdValid = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETDYNAMICTX_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
        /* check CanTxPduId valid */
    if ((TRUE == detNoErr)
         && ((CanTxPduId >= CANIF_NUMBER_OF_CANTXPDUIDS)
         ||(CANIF_PDU_TYPE_DYNAMIC != CANIF_CANTXPDUID_TYPE(CanTxPduId)))
       )
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETDYNAMICTX_ID, CANIF_E_INVALID_TXPDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    {
        /*  dynamic Txpdu CanId Extended */
        if (CANIF_EXTENDED_CAN == CANIF_TXPDUIDCANIDTYPE(CanTxPduId))
        {
            if (CanId > CANIF_EXTENDED_CANID_MAX)
            {
               canIdValid = FALSE; 
            }
        }
        else 
        {   
            /* Standard Can Id, check wether valid */
            if (CanId > CANIF_STANDARD_CANID_MAX)
            {
               canIdValid = FALSE; 
            }
        }
        if (FALSE == canIdValid)
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETDYNAMICTX_ID,CANIF_E_PARAM_CANID);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /** @req CANIF186 */
        /* Find The Index of DynamicTxPduCanIds */
        dynamicTxIndex = CANIF_DYNAMICTXPDUCANIDINDEX(CanTxPduId);

        /* judge dynamicTxIndex valid */
        if (dynamicTxIndex < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)
        {
            CanIf_DynamicTxPduCanIds[dynamicTxIndex] = CanId; /** @req CANIF188 */
            if (CANIF_EXTENDED_CAN == CANIF_TXPDUIDCANIDTYPE(CanTxPduId))
            {  
                /** @req CANIF188 */
                CanIf_DynamicTxPduCanIds[dynamicTxIndex] |= CANIF_CANID_EXTEND_BIT;
            }
        }
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_SETDYNAMICTXID_API */


#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetTransceiverMode
(    
    uint8 Transceiver,
    CanIf_TransceiverModeType TransceiverMode
)
{
    Std_ReturnType result = E_NOT_OK;

    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERMODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Transceiver >= CANIF_MAX_TRANSCEIVER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERMODE_ID, CANIF_TRCV_E_TRANSCEIVER);
        detNoErr = FALSE;    
    }  
    if (TRUE == detNoErr)
    {
        CanIf_TransceiverModeType TrcvCurrentMode;
        CanIf_GetTransceiverMode(Transceiver,&TrcvCurrentMode);

        if ((CANIF_TRCV_MODE_NORMAL == TrcvCurrentMode) && (CANIF_TRCV_MODE_SLEEP == TransceiverMode))
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERMODE_ID, CANIF_TRCV_E_TRCV_NOT_STANDBY);
            detNoErr = FALSE;    
        }
        if ((TRUE == detNoErr)
         && ((CANIF_TRCV_MODE_SLEEP == TrcvCurrentMode) && (CANIF_TRCV_MODE_STANDBY == TransceiverMode))
           )
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERMODE_ID, CANIF_TRCV_E_TRCV_NOT_NORMAL);
            detNoErr = FALSE; 
        }      
    }
    if(TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {    
        result = CanIf_TrcvFctData[CANIF_TRCV_FCT_INDEX(Transceiver)].CanTrcvSetOpModeFctPtr(CANIF_TRCV_ID(Transceiver),TransceiverMode);
    }    
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetTransceiverMode
(
    uint8 Transceiver,
    P2VAR(CanIf_TransceiverModeType, AUTOMATIC, CANIF_APPL_DATA) TransceiverModePtr
)
{
    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    #endif
    Std_ReturnType result = E_NOT_OK;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRANSCEIVERMODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Transceiver >= CANIF_MAX_TRANSCEIVER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRANSCEIVERMODE_ID, CANIF_TRCV_E_TRANSCEIVER);
        detNoErr = FALSE;    
    }  
    /* check NULL PTR*/
    if (NULL_PTR == TransceiverModePtr)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRANSCEIVERMODE_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* get current mode */     
         result = CanIf_TrcvFctData[CANIF_TRCV_FCT_INDEX(Transceiver)].CanTrcvGetOpModeFctPtr(CANIF_TRCV_ID(Transceiver),TransceiverModePtr);
    }
    return result;

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_GetTrcvWakeupReason
(
    uint8 Transceiver,
    P2VAR(CanIf_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA)TrcvWuReasonPtr
)
{
#if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    #endif
    Std_ReturnType result = E_NOT_OK;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRCVMODEREASON_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Transceiver >= CANIF_MAX_TRANSCEIVER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRCVMODEREASON_ID, CANIF_TRCV_E_TRANSCEIVER);
        detNoErr = FALSE;    
    }  
    /* check NULL PTR*/
    if (NULL_PTR == TrcvWuReasonPtr)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GET_TRCVMODEREASON_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* get current mode */      
        result = CanIf_TrcvFctData[CANIF_TRCV_FCT_INDEX(Transceiver)].CanTrcvGetBusWuReasonFctPtr(CANIF_TRCV_ID(Transceiver),TrcvWuReasonPtr);
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType , CANIF_CODE)
CanIf_SetTransceiverWakeupMode
(
    uint8 Transceiver,
    CanIf_TrcvWakeupModeType TrcvWakeupMode
)
{
    Std_ReturnType result = E_NOT_OK;

    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERWAKEMODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Transceiver >= CANIF_MAX_TRANSCEIVER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_TRANSCEIVERWAKEMODE_ID, CANIF_TRCV_E_TRANSCEIVER);
        detNoErr = FALSE;    
    }  
    if(TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        result = CanIf_TrcvFctData[CANIF_TRCV_FCT_INDEX(Transceiver)].CanTrcv_SetWakeupModeFctPtr(CANIF_TRCV_ID(Transceiver),TrcvWakeupMode);
    }    
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif /* STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED */


#if (STD_ON == CANIF_WAKEUP_VALIDATION)
/*************************************************************************/
/*
 * Brief               This Service checks, whether an underlying CAN 
 *                     driver or CAN Transceiver driver already signals 
 *                     an wakeup event by the CAN network
 * ServiceId           0x11 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeUpSource: Source device, who initia ted the 
 *                     wakeup event: CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/** @req CANIF219 @req CANIF181 @req CANIF231 @req CANIF286 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;
    
    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    
    /* CanIf not initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKWAKEUP_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)    
    #endif /* (STD_ON == CANIF_DEV_ERROR_DETECT) */
    {   
        uint8 controllerIndex;
        
        /*traversal all can controllers to find the wake-up source*/
        for (controllerIndex = 0u; 
             (controllerIndex < CANIF_MAX_CONTROLLER) && (result == E_NOT_OK); 
             controllerIndex++)
        {
            EcuM_WakeupSourceType wakeupSourceCfg;
            wakeupSourceCfg = CanIf_ControllerConfiguration[controllerIndex].CanIf_WakeUpSource;
            if (0u != (WakeupSource & wakeupSourceCfg))
            {
                /* do check up action , shall call Can_Cbk_CheckWakeup */
                result = CanIf_CheckWakeupAction(controllerIndex,WakeupSource);
            } 
        }

    }
    return result;
}
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
 * Param-Name[in]      WakeUpSource: Source device, who initia ted the 
 *                     wakeup event: CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;

    #if (STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;

    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
       Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_UNINIT);
       detNoErr =  FALSE;
    }
    if (TRUE == detNoErr)
    #endif
    {
        uint8 controllerIndex;

        /*traversal all can controllers to find the wake-up source*/
        for (controllerIndex = 0u; 
            (controllerIndex < CANIF_MAX_CONTROLLER) 
             && (E_NOT_OK == result); 
             controllerIndex++)
        {
                EcuM_WakeupSourceType wakeupSourceCfg;
                wakeupSourceCfg = CanIf_ControllerConfiguration[controllerIndex].CanIf_WakeUpSource;
                
                /* check wake-up source */
                if (0u != (WakeupSource & wakeupSourceCfg))
                {
                    /* do check up Validation action */
                    result = CanIf_CheckWakeupValidationAction(controllerIndex,WakeupSource);
                }                 
        }        
    }    
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif /* end of STD_ON == CANIF_WAKEUP_VALIDATION */


/** @req CANIF738 */ 
#if(STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT) 
/*************************************************************************/
/*
 * Brief               reports if any TX confirmation has been done for the 
 *                     whole CAN controller since the last CAN controller start.
 * ServiceId           0x19 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanController: Abstracted CanIf ControllerId which
 *                                    is assigned to a CAN controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        None
 * CallByAPI           Up layer(CanSM)
 */
/*************************************************************************/
/** @req CANIF734 @req CANIF737 */ 
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_GetTxConfirmationState(uint8 CanController)
{
    boolean detNoErr = TRUE;
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
        
    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    /* check initiation */
    /** @req CANIF735 */ 
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETTXCONFIRMATIONSTATE, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /** @req CANIF736 */
    if((TRUE == detNoErr) && (CanController >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_GETTXCONFIRMATIONSTATE, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;
    }
    #endif

    if (TRUE == detNoErr)
    {
        /* get the  TX confirmation state of the corresponding controller */
        result = CanIf_TxConfirmationState[CanController];  

        /* clear the state */
        CanIf_TxConfirmationState[CanController] = CANIF_NO_NOTIFICATION;
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


/*************************************************************************/
/*
 * Brief               Within this service, the CAN Driver passes back the
 *                     CanTxPduId  to the CAN Interface, which it got from
 *                     Can_Write(Hth, *PduInfo).
 * ServiceId           0x13 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId: L-PDU handle of CAN L-PDU  successfully
 *                     transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
/** @req CANIF007 @req CANIF042 @req CANIF187 @req CANIF162
    @req CANIF064 @req CANIF065 @req CANIF029 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_TxConfirmation(PduIdType CanTxPduId)
{
    uint8 canControllerId;
    uint8 hthConfigIndex;

    #if STD_ON == CANIF_TX_BUFFER_USED
    uint8 hthId;
    PduIdType txBufIndex;
    Std_ReturnType result;
    Can_ReturnType canRetValue;
    #endif

    if (TRUE == CanIf_DriverConfiguration.CanIfTxConfirmation)
    {
        #if(STD_ON == CANIF_DEV_ERROR_DETECT)
        boolean detNoErr = TRUE;
        /** @req CANIF007 */ 
        /* check initiation */
        if (CANIF_INITED != CanIf_InitStatus)
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TXCONFIRMATION_ID, CANIF_E_UNINIT);
            detNoErr = FALSE;
        }
        /** @req CANIF007 */
        /* check parameter valid */
        if((TRUE == detNoErr) && (CanTxPduId >= CANIF_NUMBER_OF_CANTXPDUIDS))
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_TXCONFIRMATION_ID, CANIF_E_PARAM_LPDU);
            detNoErr = FALSE;
        }
        if (TRUE == detNoErr)        
        #endif
        {
            /* Get the hth config Index */
            hthConfigIndex = CANIF_HTH_REF_ID(CanTxPduId);

            /* get controller ID */
            canControllerId = CanIf_HthConfigData[hthConfigIndex].CanIfCanControllerIdRef;

            #if(STD_ON == CANIF_TX_BUFFER_USED)  
            
            /* if txbuffer really used, (count > 0),then find priority */
            if (CanIf_TxBufferUsedCount[hthConfigIndex] > 0u)
            {
                if (CANIF_NUMBER_OF_TXBUFFERS < CANIF_NUMBER_OF_CANTXPDUIDS)
                {
                    /* not each txpdu assign a txbuffer */
                    result = CanIf_FindHighestPrioPduInTxBuffer(CanTxPduId, &txBufIndex);
                }
                else 
                {
                    /* each txpdu assign a txbuffer */
                    result = CanIf_FindHighestPrioPduInTxMaxBuffer(CanTxPduId, &txBufIndex);
                }

                if (E_OK == result)
                {
                    /* get the hthId */
                    hthId = CanIf_HthConfigData[hthConfigIndex].CanIfHthIdSymRef;

                    /* if tx buffer exist,check buffer first,then send */
                    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
                    canRetValue = CanIf_SendTxBufferPdu(txBufIndex,hthId);
                    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
                    
                    if (CAN_OK == canRetValue)
                    {
                        /* Free this buffer because pdu has been sucessfully transmit */
                        CanIf_TxPduBuffer[txBufIndex].used = FALSE; 

                        /* one buffer free, so count-- */
                        CanIf_TxBufferUsedCount[hthConfigIndex]--;
                    }
                }
            }
            #endif

            /* check PDU mode */
            if ((CANIF_GET_TX_ONLINE == CanIf_PduMode[canControllerId]) 
                || (CANIF_GET_ONLINE == CanIf_PduMode[canControllerId])
                || (CANIF_GET_OFFLINE_ACTIVE == CanIf_PduMode[canControllerId])
                || (CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE == CanIf_PduMode[canControllerId]))
            {
                /* enable notify status */
                #if(STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API)
                if(TRUE == CANIF_READTXPDU_NOTIFY_STATUS(CanTxPduId))
                {
                    /* change to notify status */ 
                    /** @req CANIF204 */
                    CanIf_TxNotifStatus[CANIF_TXNOTIFYINDEX(CanTxPduId)] = CANIF_TX_RX_NOTIFICATION;
                }
                #endif

                /** @req CANIF740 */
                #if (STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT)
                if (CANIF_CS_STARTED == CanIf_ControllerMode[canControllerId])
                {
                    CanIf_TxConfirmationState[canControllerId] = CANIF_TX_RX_NOTIFICATION;
                }  
                #endif
                if ((CANIF_TX_USER_TYPE(CanTxPduId) < CANIF_USER_MAX_COUNT) 
                    &&(NULL_PTR != CanIf_UserTxConfirmation[CANIF_TX_USER_TYPE(CanTxPduId)])
                    )
                {
                    /* call upper layer function,feedback to upper layer*/
                    /** @req CANIF053 @req CANIF109 @req CANIF011 */
                    CanIf_UserTxConfirmation[CANIF_TX_USER_TYPE(CanTxPduId)](CANIF_CANTXPDUID(CanTxPduId));
                }
            }
        }/* if (TRUE == detNoErr) */
    }/*if (TRUE == CanIf_DriverConfiguration.CanIfTxConfirmation)*/
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               This service is implemented in the CAN Interface 
 *                     and called by the CAN Driver after a CAN L-PDU has
 *                     been received. Within this service, the CAN 
 *                     Interface translates the  CanId  into the configured
 *                     target PDU ID and routes this indication to the
 *                     configured upper layer target service(s)
 * ServiceId           0x14 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Hrh: ID of the corresponding hardware object
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that 
 *                     has been successfully received
 *                     CanDlc: Data length code (length of CAN L-PDU 
 *                     payload)
 *                     CanSduPtr: Pointer to received L-SDU (payload)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
/** @req CANIF006 @reqCANIF098 @CANIF112 @CANIF135 @req CANIF055 @req CANIF058 @req CANIF129
    @req CANIF135 @req CANIF147 */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) 
CanIfDiag_RxIndication
(
    uint8 Hrh, 
    Can_IdType CanId, 
    uint8 CanDlc, 
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) CanSduPtr
)
{
    if (TRUE == CanIf_DriverConfiguration.CanIfReceiveIndication)
    {
        #if(STD_ON == CANIF_DEV_ERROR_DETECT)
        boolean detNoErr = TRUE;
        
        /* @req CANIF006 check initiation */
        if (CANIF_INITED != CanIf_InitStatus)
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RXINDICATION_ID, CANIF_E_UNINIT);
            detNoErr = FALSE;
        }
        /* @req CANIF006 check NULL PTR */
        if ((TRUE == detNoErr) && (NULL_PTR == CanSduPtr))
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RXINDICATION_ID, CANIF_E_PARAM_POINTER);
            detNoErr = FALSE;
        }
        /* @req CANIF006 check DLC valid */
        if ((TRUE == detNoErr) && (CanDlc > 8u))
        {
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RXINDICATION_ID, CANIF_E_PARAM_DLC);
            detNoErr = FALSE;
        }
        if (TRUE == detNoErr)
        #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
        {
            uint8 hrhIndex = 0;
			#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_LINEAR)
            uint8 hrhCount;
			#endif
            boolean hrhFindOut = FALSE;

            #if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_LINEAR)
            for (hrhCount = 0u; 
                ((hrhCount < CANIF_NUMBER_OF_HRH_CFG) && (FALSE == hrhFindOut));
                 hrhCount++)
            {
                if (Hrh == CANIF_HRHIDSYMREF(hrhCount))
                {
                   hrhIndex = hrhCount;
                   hrhFindOut = TRUE;  
                }
            }
            #elif (CANIF_SOFTWARE_FILTER_TYPE == CANIF_BINARY)
            if (E_OK == CanIf_BinSearch_HrhIndex(0,
                                                 (CANIF_NUMBER_OF_HRH_CFG - 1u),
                                                 Hrh,
                                                 &hrhIndex
                                                 ))
            {
                hrhFindOut = TRUE; 
            }
            #endif
           
            if (FALSE == hrhFindOut)
            {
                #if(STD_ON == CANIF_DEV_ERROR_DETECT)
                Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RXINDICATION_ID, CANIF_E_PARAM_HRH);
                #endif             
            }
            else
            {
                PduIdType pduIdIndex;
                uint8 canControllerId;
                canControllerId = CanIf_HrhConfigData[hrhIndex].CanIfCanControllerHrhIdRef;

                #if STD_ON == CANIF_WAKEUP_VALIDATION
                /* set first call rxindication event flag */
                CanIf_FirstCallRxInd[canControllerId] = TRUE;
                #endif

                if (E_OK == CanIf_CanIdRangCheck(hrhIndex,CanId,&pduIdIndex))
                {
                   /* check in right mode */
                    if ((CANIF_GET_OFFLINE != CanIf_PduMode[canControllerId])
                    && (CANIF_GET_TX_ONLINE != CanIf_PduMode[canControllerId])
                    && (CANIF_GET_OFFLINE_ACTIVE != CanIf_PduMode[canControllerId]))
                    {
                        /* DLC check enabled */ 
                        /** @req CANIF026 @req CANIF296 @req CANIF031 */
                        #if (STD_ON == CANIF_DLC_CHECK)
                        if (CanDlc < CANIF_CANRXPDUID_DLC(pduIdIndex))
                        {
                            #if STD_ON == CANIF_DEM_ERROR_DETECT
                            /** @req CANIF168 */
                            Dem_ReportErrorStatus(CANIF_E_INVALID_DLC, DEM_EVENT_STATUS_FAILED);
                            #endif
                        }
                        else                
                        #endif
                        {
                            #if (STD_ON == CANIF_DLC_CHECK)
                            CanIf_RxIndicationHandle(pduIdIndex,CANIF_CANRXPDUID_DLC(pduIdIndex),CanSduPtr);
                            #else
                            CanIf_RxIndicationHandle(pduIdIndex,CanDlc,CanSduPtr);
                            #endif                            
                        }
                    } 
                }
                else/**< canId Invalid  */
                {
                    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
                    Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RXINDICATION_ID, CANIF_E_PARAM_CANID);
                    #endif  
                }
            }
        }
    }/* if (TRUE == CanIf_DriverConfiguration.CanIfReceiveIndication)*/
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/** @req CANIF177 */
#if (STD_ON == CANIF_TRANSMIT_CANCELLATION)
/*************************************************************************/
/*
 * Brief               This service is implemented in the CAN Interface 
 *                     and called by the CAN Driver after a previous 
 *                     request for cancellation of a pending L-PDU transmit
 *                     request was successfully performed.
 * ServiceId           0x15 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      PduInfoPtr: Pointer to a structure with CAN L-PDU 
 *                     related data: L-PDU handle of the successfully 
 *                     aborted CAN L-PDU, CAN identifier, DLC and pointer
 *                     to CAN L-SDU buffer.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_CancelTxConfirmation(P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_CONST) PduInfoPtr)
{
    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* @req CANIF101 check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CANCELTXCONFIRMATION_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /* @req CANIF101 check parameter validation*/
    if ((TRUE == detNoErr) && ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->sdu)))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CANCELTXCONFIRMATION_ID, CANIF_E_PARAM_LPDU);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
    #endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        #if (STD_ON == CANIF_TX_BUFFER_USED)
        PduIdType canTxPduIdx;
        PduIdType txBufIndex;
        Std_ReturnType result;
        uint8 hthId;
        uint8 hthConfigIndex;
        
        canTxPduIdx = PduInfoPtr->swPduHandle;
        
        hthConfigIndex = CANIF_HTH_REF_ID(canTxPduIdx);
        
        /* get the HthId */
        hthId = CanIf_HthConfigData[hthConfigIndex].CanIfHthIdSymRef;
        
        #if (CANIF_NUMBER_OF_TXBUFFERS < CANIF_NUMBER_OF_CANTXPDUIDS)
        {
            /* do cancel handle */
            CanIf_TxBufferCancelHandle(PduInfoPtr);
            
            result = CanIf_FindHighestPrioPduInTxBuffer(canTxPduIdx,&txBufIndex);
        }
        #else
        {
            /* do cancel handle */
            CanIf_TxMaxBufferCancelHandle(PduInfoPtr);
            
            result = CanIf_FindHighestPrioPduInTxMaxBuffer(canTxPduIdx,&txBufIndex);
        }
		#endif
        if (E_OK == result)
        {
            Can_ReturnType canWriteRet;

            SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
            canWriteRet = CanIf_SendTxBufferPdu(txBufIndex,hthId);
            SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
            if (CAN_OK == canWriteRet)
            {
                /* Free this buffer because pdu has been sucessfully transmit */
                CanIf_TxPduBuffer[txBufIndex].used = FALSE; 

                /* one txbuffer free, count -- */
                CanIf_TxBufferUsedCount[hthConfigIndex]--;
            }
        }  
        #endif /* end of STD_ON == CANIF_TX_BUFFER_USED */
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_TRANSMIT_CANCELLATION */


/*************************************************************************/
/*
 * Brief               This service indicates a CAN controller BusOff 
 *                     event referring to the corresponding CAN controller.
 *                     This call-out service is called by the  CAN Driver 
 *                     and implemented in the CAN Interface. It is called 
 *                     in case of a m ode change notification of the CAN 
 *                     Driver
 * ServiceId           0x16 
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      PduInfoPtr: Pointer to a structure with CAN L-PDU 
 *                     related data: L-PDU handle of the successfully 
 *                     aborted CAN L-PDU, CAN identifier, DLC and pointer
 *                     to CAN L-SDU buffer.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
/** @req CANIF218 */ 
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_ControllerBusOff(uint8 Controller)
{
    boolean detNoErr = TRUE;

    #if(STD_ON == CANIF_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CONTROLLER_BUSOFF_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (Controller >= CANIF_MAX_CONTROLLER))
    {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CONTROLLER_BUSOFF_ID, CANIF_E_PARAM_CONTROLLER);
        detNoErr = FALSE;    
    }
    #endif

    if (TRUE == detNoErr)
    {
        /* set controller to STOP status */ 
        /** @req CANIF298 */
        if (E_NOT_OK == CanIf_SetControllerMode(Controller, CANIF_CS_STOPPED))
        {
            detNoErr = FALSE;
        }

        if ((TRUE == detNoErr)&&(CANIF_CS_STOPPED == CanIf_ControllerMode[Controller]))
        {
            /** @req CANIF739 */
            #if (STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT)
            CanIf_TxConfirmationState[Controller] = CANIF_NO_NOTIFICATION;
            #endif
            
            if ((TRUE == CanIf_DriverConfiguration.CanIfBusoffNotification)
              &&(NULL_PTR != CanIf_DispatchConfigData.CanIfBusOffNotification))
              {
                  /** @req CANIF272 @req CANIF015 */
                  CanIf_DispatchConfigData.CanIfBusOffNotification(Controller);
              }
            
        }
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set controller state to start
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Controller: Number of controller 
 *                     Index: Index of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllStarted(uint8 Controller)
{
    uint8 controllerId;
    Std_ReturnType result = E_NOT_OK;
    Can_ReturnType retval;

    /* get Can controller Id */
    controllerId = CanIf_ControllerConfiguration[Controller].CanIfControllerIdRef;

    if (CANIF_CS_STARTED == CanIf_ControllerMode[Controller])
    {
        result = E_OK;
    }
    else 
    {
        /* current mode is SLEEP */
        if (CANIF_CS_SLEEP == CanIf_ControllerMode[Controller])
        {
            /* wake up the controller */
            SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            retval = Can_SetControllerMode(controllerId, CAN_T_WAKEUP);
            SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            if (CAN_OK == retval)
            {
                /* set controller mode STOP */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                retval = Can_SetControllerMode(controllerId, CAN_T_STOP);
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                if (CAN_OK == retval)
                {
                    /* save current mode */
                    CanIf_ControllerMode[Controller] = CANIF_CS_STOPPED;
                    CanIf_FreeBufInStopped(Controller);
                    result = E_OK;
                }
            }
        }
        if (CANIF_CS_STOPPED == CanIf_ControllerMode[Controller])
        {

            /* set controller mode START */
            SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            retval = Can_SetControllerMode(controllerId, CAN_T_START);
            SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            if (CAN_NOT_OK == retval)
            {
                result = E_NOT_OK;
            }
            else
            {
                /* set controller mode to CANIF_CS_STARTED */
                CanIf_ControllerMode[Controller] = CANIF_CS_STARTED;
                result = E_OK;
            }
        }
    }

    if (E_OK == result)
    {
        /* set PDU mode ONLINE */
        result = CanIf_SetPduMode(Controller,CANIF_SET_ONLINE);

    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set controller state to sleep
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Controller: Number of controller 
 *                     Index: Index of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllSleep(uint8 Controller)
{
    uint8 controllerId;
    Std_ReturnType result = E_NOT_OK;
    Can_ReturnType retval;
    
    /* get Can controller Id */
    controllerId = CanIf_ControllerConfiguration[Controller].CanIfControllerIdRef;

    if (CANIF_CS_SLEEP == CanIf_ControllerMode[Controller])
    {
        result = E_OK;
    }
    else 
    {
        /* current mode is START */
        if (CANIF_CS_STARTED == CanIf_ControllerMode[Controller])
        {
            /* set PDU mode OFFLINE */
            if (E_OK == CanIf_SetPduMode(Controller,CANIF_SET_OFFLINE))
            {
                /* set controller mode STOP */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                retval = Can_SetControllerMode(controllerId, CAN_T_STOP);
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                if (CAN_OK == retval)
                {
                    /* save current mode */
                    CanIf_ControllerMode[Controller] = CANIF_CS_STOPPED;
                    CanIf_FreeBufInStopped(Controller);
                }
            }            
        }
        if (CANIF_CS_STOPPED == CanIf_ControllerMode[Controller])
        {
            /* set controller mode SLEEP */
            SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            retval = Can_SetControllerMode(controllerId, CAN_T_SLEEP);
            SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            if(CAN_OK == retval)
            {
                #if STD_ON == CANIF_WAKEUP_VALIDATION
                /* Clear first call rxindication event flag While Sleep */
                CanIf_FirstCallRxInd[Controller] = FALSE;
                #endif
                
                /* set controller mode to CANIF_CS_SLEEP*/
                CanIf_ControllerMode[Controller] = CANIF_CS_SLEEP;
                result = E_OK;
            }
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set controller state to stop
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Controller: Number of controller 
 *                     Index: Index of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetControllStopped(uint8 Controller)
{
    uint8 controllerId;
    Std_ReturnType result = E_NOT_OK;
    Can_ReturnType retval;
    
    /* get Can controller Id */
    controllerId = CanIf_ControllerConfiguration[Controller].CanIfControllerIdRef;
    if (CANIF_CS_STOPPED == CanIf_ControllerMode[Controller])
    {
        result = E_OK;
    }
    else 
    {
        /* current mode is SLEEP */
        if (CANIF_CS_SLEEP == CanIf_ControllerMode[Controller])
        {
            /* wake up the controller */
            SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            retval = Can_SetControllerMode(controllerId, CAN_T_WAKEUP);
            SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
            if (CAN_OK == retval)
            {
                /* set controller mode STOP */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                retval = Can_SetControllerMode(controllerId, CAN_T_STOP);
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                if (CAN_OK == retval)
                {
                    /* save current mode */
                    CanIf_ControllerMode[Controller] = CANIF_CS_STOPPED;
                    CanIf_FreeBufInStopped(Controller);
                    result = E_OK;
                }
            }
        }
        else if (CANIF_CS_STARTED == CanIf_ControllerMode[Controller])
        {
            /* set PDU mode OFFLINE */
            if (E_OK == CanIf_SetPduMode(Controller,CANIF_SET_OFFLINE))
            {
                /* set controller mode STOP */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                retval = Can_SetControllerMode(controllerId, CAN_T_STOP);
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
                if (CAN_OK == retval)
                {
                    /* save current mode */
                    CanIf_ControllerMode[Controller] = CANIF_CS_STOPPED;
                    CanIf_FreeBufInStopped(Controller);
                    result = E_OK;
                }            
            }            
        }
        else
        {
            /* nerither CANIF_CS_SLEEP nor CANIF_CS_STARTED Mode */
        }
         
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set PDU mode to CANIF_SET_RX_OFFLINE
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldPduMode: current PDU mode
 *                     Index: Index of controller 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetPduMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetRxOffline
(
    CanIf_ChannelGetModeType oldPduMode, 
    uint8 controller
)
{
    
    /* switch current pdu mode */
    switch (oldPduMode)
    {
        /* set mode according to current mode */
        case CANIF_GET_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE;
            break;

        case CANIF_GET_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_TX_ONLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE;
            break;

        /* other modes */
        default:
            break;
    }
    return ;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set PDU mode to CANIF_SET_RX_ONLINE
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldPduMode: current PDU mode
 *                     Index: Index of controller 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetPduMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetRxOnline
(
    CanIf_ChannelGetModeType oldPduMode, 
    uint8 controller
)
{
    /* switch current mode */
    switch (oldPduMode)
    {
        /* set mode according to current mode */
        case CANIF_GET_OFFLINE:
            CanIf_PduMode[controller] = CANIF_GET_RX_ONLINE;
            break;

        case CANIF_GET_TX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_ONLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
            break;

        /* other modes */
        default:
            break;
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Set PDU mode to CANIF_SET_TX_OFFLINE
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldPduMode: current PDU mode
 *                     Index: Index of controller 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetPduMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOffline
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
)
{
    /* switch current mode */
    switch (oldPduMode)
    {
        /* set mode according to current mode */
        case CANIF_GET_TX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE;
            break;

        case CANIF_GET_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_RX_ONLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_RX_ONLINE;
            break;

        /* other modes */
        default:
            break;
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set PDU mode to CANIF_SET_TX_ONLINE
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldPduMode: current PDU mode
 *                     Index: Index of controller 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetPduMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOnline
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
)
{ 
    /* switch current mode */
    switch (oldPduMode)
    {
        /* set mode according to current mode */
        case CANIF_GET_OFFLINE:
            CanIf_PduMode[controller] = CANIF_GET_TX_ONLINE;
            break;

        case CANIF_GET_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_ONLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE:
            CanIf_PduMode[controller] = CANIF_GET_TX_ONLINE;
            break;

        case CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_ONLINE;
            break;

        /* other modes */
        default:
            break;
  }
  return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               Set PDU mode to CANIF_SET_TX_OFFLINE_ACTIVEX_OFFLINE
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldPduMode: current PDU mode
 *                     Index: Index of controller 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetPduMode
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_SetTxOfflineActive
(
    CanIf_ChannelGetModeType oldPduMode,
    uint8 controller
)
{
    /* switch current mode */
    switch (oldPduMode)
    {
        /* set mode according to current mode */
        case CANIF_GET_OFFLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE;
            break;

        case CANIF_GET_RX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
            break;

        case CANIF_GET_TX_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE;
            break;

        case CANIF_GET_ONLINE:
            CanIf_PduMode[controller] = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
            break;

        /* other modes */
        default:
            break;
  }
  return;
} 
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Check Id data and length of reviecd data 
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Hrh: ID of the corresponding hardware object
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that 
 *                     has been successfully received
 *                     CanHrhConfigData: Configured Data 
 *                     RxPduIdx: the index of Rx Pdu
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication 
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_CanIdRangCheck
(
    uint8 hrhId, 
    Can_IdType canId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) pduIdIndex
)
{
    PduIdType rxPduIndex;
    Std_ReturnType result = E_NOT_OK;

    /* if the hrh defined as Basic Can and sofware filter enabled */
    if ((CANIF_BASIC_CAN == CanIf_HrhConfigData[hrhId].CanIfHrhType)
       &&(TRUE == CanIf_HrhConfigData[hrhId].CanIfSoftwareFilterHrh))
    {                     
        /* do software filter*/
        if (CanIf_CANIDMASK(hrhId) ==
           (canId & CANIF_RXRANGEMASK(hrhId)))
        {
            /* pass the Mask filter */
            result = CanIf_RxPduHrhSearch(hrhId,canId,pduIdIndex);
        }
    }
    else /**< FullCan */
    {
        /* find the rxpduId due to the hrhId,Full Can,Start and stop indexs are the same */
        rxPduIndex =  CANIF_HRH_PDUID_STARTINDEX(hrhId);

        if (canId == CANIF_CANRXPDUID_CANID(rxPduIndex))
        {
            *pduIdIndex = rxPduIndex;
            result = E_OK;
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (STD_ON == CANIF_TX_BUFFER_USED)
/*************************************************************************/
/*
 * Brief               free the txbuffer of the corresponding controller
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      controller: Can Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_Init
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeControllerTxBuffer(uint8 controller)
{
    PduIdType txBufferIndex;
    uint16 txPduId;
    uint8 hthConfigIndex;
    
    for (txBufferIndex = 0u; txBufferIndex < CANIF_NUMBER_OF_TXBUFFERS; txBufferIndex++)
    {
        /* polling txbuffer, find the one already be used */
        if (TRUE == CanIf_TxPduBuffer[txBufferIndex].used)
        {
            /* get the txPduId */
            txPduId = CanIf_TxPduBuffer[txBufferIndex].swPduHandle;

            /* check this txbuffer whether used for this controller */
            if (controller == CanIf_HthConfigData[CANIF_HTH_REF_ID(txPduId)].CanIfCanControllerIdRef)
            {
                /* set the used flag False so that this buffer can save new pdu */
                CanIf_TxPduBuffer[txBufferIndex].used = FALSE;

                /* get the hthId */
                hthConfigIndex = CANIF_HTH_REF_ID(txPduId);

                /* Set the buffer used count to 0 */
                CanIf_TxBufferUsedCount[hthConfigIndex] = 0;
            }             
        }
    }
    return;    
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               overwrite the pdu in txBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txBufIndex: index of txBuffer
 *                     pduPtr:New Can pdu Info
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_TxBusyHandle,CanIf_TxMaxBufferBusyHandle
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_OverWritePduInTxBuffer
( 
    PduIdType txBufIndex,
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{
 
    CanIf_TxPduBuffer[txBufIndex].length = pduPtr->length;
    CanIf_TxPduBuffer[txBufIndex].swPduHandle = pduPtr->swPduHandle;
    CanIf_TxPduBuffer[txBufIndex].canId = pduPtr->id;
    CanIf_Memcpy(CanIf_TxPduBuffer[txBufIndex].data,pduPtr->sdu,pduPtr->length);
    
    CanIf_TxPduBuffer[txBufIndex].used = TRUE;

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               send the pdu in txBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txBufIndex: index of txBuffer
 *                     hth:hardware transmit object handle
 *                     controller: CAN controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CAN_OK:transmit successful
 *                     CAN_NOT_OK:transmit failure
 *                     CAN_BUSY:Can transmit busy
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_TxConfirmation,CanIf_CancelTxConfirmation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Can_ReturnType, CANIF_CODE)
CanIf_SendTxBufferPdu
(
    PduIdType txBufIndex,
    uint8 hth
)
{
    Can_PduType canPduInfo;
    Can_ReturnType result;
        
    canPduInfo.id = CanIf_TxPduBuffer[txBufIndex].canId;
    canPduInfo.length = CanIf_TxPduBuffer[txBufIndex].length;
    canPduInfo.swPduHandle = CanIf_TxPduBuffer[txBufIndex].swPduHandle;
    canPduInfo.sdu = CanIf_TxPduBuffer[txBufIndex].data;

    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);
    /* call lower layer CAN driver API,and get return value */
    result = Can_Write(hth, &canPduInfo);
    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_CAN);

    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               save the pdu to txBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      pduPtr: New Can pdu need saved
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:transmit successful
 *                     E_NOT_OK:transmit failure
 * PreCondition        txbuffer used 
 * CallByAPI           CanIf_TxBusyHandle,CanIf_TxBufferCancelHandle
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SavePduToTxBuffer
(  
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_CONST) pduPtr
)
{
    uint8 canDataIndex;
    PduIdType txBufIndex;
    boolean saveOk = FALSE;
    Std_ReturnType result = E_OK;

    for (txBufIndex = 0u; 
        (txBufIndex < CANIF_NUMBER_OF_TXBUFFERS)&&(FALSE == saveOk); 
        txBufIndex++)
    {
        if (FALSE == CanIf_TxPduBuffer[txBufIndex].used)
        {
            uint8 hthConfigIndex;
            
            /* get the hth config Index */
            hthConfigIndex = CANIF_HTH_REF_ID(pduPtr->swPduHandle);
    
            CanIf_TxPduBuffer[txBufIndex].length = pduPtr->length;
            CanIf_TxPduBuffer[txBufIndex].canId = pduPtr->id;
            CanIf_TxPduBuffer[txBufIndex].swPduHandle = pduPtr->swPduHandle;
            for (canDataIndex = 0u; canDataIndex < (pduPtr->length); canDataIndex++)
            {
                CanIf_TxPduBuffer[txBufIndex].data[canDataIndex] = pduPtr->sdu[canDataIndex];
            }
            CanIf_TxPduBuffer[txBufIndex].used = TRUE;

            /* one buffer used for saving,count++ */
            CanIf_TxBufferUsedCount[hthConfigIndex]++;
            
            saveOk = TRUE;         
        }  
    }

    /* All buffers full,can't save */
    if (FALSE == saveOk)
    {
        #if STD_ON == CANIF_DEM_ERROR_DETECT
        Dem_ReportErrorStatus(CANIF_E_FULL_TX_BUFFER, DEM_EVENT_STATUS_FAILED);
        #endif
        result = E_NOT_OK;        
    }

    return result;    
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Find the highest Priority Pdu in txbuffer which 
 *                     same HTH.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txPduId: txPduId which pdu has been transmitted or 
 *                     cancelled.
 * Param-Name[out]     bufferIndex: the highest priority pdu in txbuffer.
 * Param-Name[in/out]  None
 * Return              E_OK:find successful
 *                     E_NOT_OK:find failure
 * PreCondition        txbuffer used 
 * CallByAPI           CanIf_TxConfirmation,CanIf_CancelTxConfirmation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_FindHighestPrioPduInTxBuffer
(
    PduIdType  txPduId, 
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) bufferIndex
)
{
    PduIdType txBufIndex ;
    PduIdType txBufferPduId;
    boolean findFlag = FALSE;
    Can_IdType canIdMin = 0x9FFFFFFFu;
    uint8 hthConfigIndex;
    uint8 hthId;
    Std_ReturnType result = E_OK;

    /* get the Hth config array index */
    hthConfigIndex = CANIF_HTH_REF_ID(txPduId);

    /* get the HthId */
    hthId = CanIf_HthConfigData[hthConfigIndex].CanIfHthIdSymRef;

    for (txBufIndex = 0u; txBufIndex < CANIF_NUMBER_OF_TXBUFFERS; txBufIndex++)
    {
        if (TRUE == CanIf_TxPduBuffer[txBufIndex].used)
        {
            /* find the txPduId of the pdu stored in CanIf_TxPduBuffer */
            txBufferPduId = CanIf_TxPduBuffer[txBufIndex].swPduHandle;
            
            /* find the pdu in CanIf_TxPduBuffer which using the same Hth */
            if (hthId == CanIf_HthConfigData[CANIF_HTH_REF_ID(txBufferPduId)].CanIfHthIdSymRef)
            {
                if (((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_STANDARD_CANID_MAX)
                	< (canIdMin & CANIF_STANDARD_CANID_MAX))
                	|| (((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_STANDARD_CANID_MAX)
                        == (canIdMin & CANIF_STANDARD_CANID_MAX))
                		&& ((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_CANID_EXTEND_ALL_BIT)
                        <= (canIdMin & CANIF_CANID_EXTEND_ALL_BIT))))
                {
                    canIdMin = CanIf_TxPduBuffer[txBufIndex].canId;               
                    *bufferIndex = txBufIndex;
                    findFlag = TRUE;
                }
            } 
        }
    }
    /* Do not find pdu of the same Hth */
    if (FALSE == findFlag)
    {
        result = E_NOT_OK;           
    }
    return result;       
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               CanIf call can_write which return CAN_BSUY
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pduPtr: CAN L-PDU to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:Busy Handle,pdu saved successful
 *                     E_NOT_OK:Busy Handle,pdu saved failure
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_Transmit
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_TxBusyHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{   
    PduIdType txBufIndex;
    boolean findSamePduId = FALSE;
    Std_ReturnType result = E_OK;
    

    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    
    for (txBufIndex = 0u; 
        (txBufIndex < CANIF_NUMBER_OF_TXBUFFERS) && (FALSE == findSamePduId);
         txBufIndex++)
    {
        if ((TRUE == CanIf_TxPduBuffer[txBufIndex].used)
           && (CanIf_TxPduBuffer[txBufIndex].swPduHandle == pduPtr->swPduHandle))
        {
            /* same pduid in txbuffer found, overwrite*/
            CanIf_OverWritePduInTxBuffer(txBufIndex, pduPtr);
            
            /* same pduid in txbuffer found */
            findSamePduId = TRUE;
        }
    }/* end of for */
    
    if (FALSE == findSamePduId) 
    {
        /* do not find the same pduid in txbuffer, so save to txbuffer */
        result = CanIf_SavePduToTxBuffer(pduPtr);
    }

    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);

    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (CANIF_NUMBER_OF_TXBUFFERS < CANIF_NUMBER_OF_CANTXPDUIDS)
#if (STD_ON == CANIF_TRANSMIT_CANCELLATION)
/*************************************************************************/
/*
 * Brief               processing the pdu which had been cancelled 
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pduPtr: CAN L-PDU to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_CancelTxConfirmation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_TxBufferCancelHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{
    PduIdType txBufIndex;
    boolean findSamePduId = FALSE;

    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    for (txBufIndex = 0u; 
        (txBufIndex < CANIF_NUMBER_OF_TXBUFFERS) && (FALSE == findSamePduId);
         txBufIndex++)
    {
        if ((TRUE == CanIf_TxPduBuffer[txBufIndex].used)
           && (CanIf_TxPduBuffer[txBufIndex].swPduHandle == pduPtr->swPduHandle))
        {
            /* same pduid in txbuffer found, discard the canceled pdu */
            findSamePduId = TRUE;
        }
    }
    if (FALSE == findSamePduId) 
    {
        /* do not find the same pduid in txbuffer, so save to txbuffer */
        (void)CanIf_SavePduToTxBuffer(pduPtr);
    }
    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
#endif

/*************************************************************************/
/*
 * Brief               save the pdu to txBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      pduPtr: New Can pdu need saved
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:transmit successful
 *                     E_NOT_OK:transmit failure
 * PreCondition        txbuffer used 
 * CallByAPI           CanIf_TxBusyHandle,CanIf_TxBufferCancelHandle
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SavePduToTxMaxBuffer
(
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{
    uint8 canDataIndex;
    PduIdType txBufIndex;   
    uint8 hthConfigIndex;
    txBufIndex = pduPtr->swPduHandle;  
    
    /* get the hth config Index */
    hthConfigIndex = CANIF_HTH_REF_ID(txBufIndex);

    CanIf_TxPduBuffer[txBufIndex].length = pduPtr->length;
    CanIf_TxPduBuffer[txBufIndex].swPduHandle = pduPtr->swPduHandle;
    CanIf_TxPduBuffer[txBufIndex].canId = pduPtr->id;

    for (canDataIndex = 0u; canDataIndex < (pduPtr->length); canDataIndex++)
    {
        CanIf_TxPduBuffer[txBufIndex].data[canDataIndex] = pduPtr->sdu[canDataIndex];
    }
    CanIf_TxPduBuffer[txBufIndex].used = TRUE;
    
    /* one buffer used for saving,count++ */
    CanIf_TxBufferUsedCount[hthConfigIndex]++;

    return E_OK;    
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Find the highest Priority Pdu in txbuffer which 
 *                     same HTH.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      txPduId: txPduId which pdu has been transmitted or 
 *                     cancelled.
 * Param-Name[out]     bufferIndex: the highest priority pdu in txbuffer.
 * Param-Name[in/out]  None
 * Return              E_OK:find successful
 *                     E_NOT_OK:find failure
 * PreCondition        txbuffer used 
 * CallByAPI           CanIf_TxConfirmation,CanIf_CancelTxConfirmation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_FindHighestPrioPduInTxMaxBuffer
(
    PduIdType  txPduId, 
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) bufferIndex
)
{
    PduIdType txBufIndex;
    boolean findFlag = FALSE;
    Can_IdType canIdMin = 0x9FFFFFFF;
    uint8 hthConfigIndex;
    Std_ReturnType result = E_OK;

    /* get the Hth config array index */
    hthConfigIndex = CANIF_HTH_REF_ID(txPduId);

    /* Loop from start to stop index of this hth */
    for (txBufIndex = CANIF_HTHSTARTINDEX(hthConfigIndex); 
        txBufIndex <= CANIF_HTHSTOPINDEX(hthConfigIndex); 
        txBufIndex++)
    {
        if (TRUE == CanIf_TxPduBuffer[txBufIndex].used)
        {
            /* Static and Dynamic Tx Pdu do same process */
            if (((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_STANDARD_CANID_MAX)
            	< (canIdMin & CANIF_STANDARD_CANID_MAX))
            	|| (((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_STANDARD_CANID_MAX)
                    == (canIdMin & CANIF_STANDARD_CANID_MAX))
            		&& ((CanIf_TxPduBuffer[txBufIndex].canId & CANIF_CANID_EXTEND_ALL_BIT)
                    <= (canIdMin & CANIF_CANID_EXTEND_ALL_BIT))))
            {
                canIdMin = CanIf_TxPduBuffer[txBufIndex].canId;               
                *bufferIndex = txBufIndex;
                findFlag = TRUE;
            }           
        }
    }
    /* Do not find pdu of the same Hth */
    if (FALSE == findFlag)
    {
        result = E_NOT_OK;           
    }
    return result;       
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/*************************************************************************/
/*
 * Brief               CanIf call can_write which return CAN_BSUY
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pduPtr: CAN L-PDU to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:Busy Handle,pdu saved successful
 *                     E_NOT_OK:Busy Handle,pdu saved failure
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_Transmit
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_TxMaxBufferBusyHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{   
#if 0
    CanIf_CanTxPduType txPduType;
#endif
    PduIdType currentPduId;
    Std_ReturnType result = E_OK;
  
    currentPduId = pduPtr->swPduHandle;
	#if 0
    txPduType = CANIF_CANTXPDUID_TYPE(currentPduId);
	#endif

    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    if ((TRUE == CanIf_TxPduBuffer[currentPduId].used)
       && (CanIf_TxPduBuffer[currentPduId].swPduHandle == pduPtr->swPduHandle))
    {
        /* same pduid in txbuffer found, saved,overwrite old */
        CanIf_OverWritePduInTxBuffer(currentPduId,pduPtr);
    }
    else /**< buffer not used, so saved */
    {   
        result = CanIf_SavePduToTxMaxBuffer(pduPtr);
    }
    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);

    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_TRANSMIT_CANCELLATION)
#if (CANIF_NUMBER_OF_TXBUFFERS >= CANIF_NUMBER_OF_CANTXPDUIDS)
/*************************************************************************/
/*
 * Brief               processing the pdu which had been cancelled 
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pduPtr: CAN L-PDU to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        txbuffer used
 * CallByAPI           CanIf_CancelTxConfirmation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_TxMaxBufferCancelHandle
( 
    P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr
)
{
    PduIdType canTxPduIdx;    
    
    canTxPduIdx = pduPtr->swPduHandle;
    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    /* the corresponding TxPduBuffer used */
    if (TRUE == CanIf_TxPduBuffer[canTxPduIdx].used)
    {
        /* Another new data arrived,discard the canceled pdu */
    }
    else
    {
        /* buffer empty,saved */
        (void)CanIf_SavePduToTxMaxBuffer(pduPtr);
    }
    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);

    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
#endif

#endif /*(STD_ON == CANIF_TX_BUFFER_USED)*/



#if (STD_ON == CANIF_READRXPDU_DATA_API)
/*************************************************************************/
/*
 * Brief               init the Receive Buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        CANIF_READRXPDU_DATA_API STD_ON
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_InitRxBuffer(uint8 controller)
{
    uint8 hrhIndex;
    PduIdType rxPduIndex;

    for (hrhIndex = 0u; hrhIndex < CANIF_NUMBER_OF_HRH_CFG; hrhIndex++)
    {
        /* check which controller belongs to */
        if (controller ==  CanIf_HrhConfigData[hrhIndex].CanIfCanControllerHrhIdRef)
        {
            /**/
            for (rxPduIndex = CANIF_HRH_PDUID_STARTINDEX(hrhIndex); 
                (rxPduIndex <= CANIF_HRH_PDUID_STOPINDEX(hrhIndex));
                 rxPduIndex++)
            {
                /* ReadRxPduData Api Enabled,a rxbuffer already assigned  when configuration */
                if (TRUE == CANIF_READRXPDU_DATA(rxPduIndex))
                {    
                    PduIdType rxPduBufIndex;

                    /* get the rxPduBufIndex */
                    rxPduBufIndex = CANIF_RXBUFFERINDEX(rxPduIndex);

                    /* judge the rxBuffer Index */
                    if (rxPduBufIndex < CANIF_MAX_NUMBER_OF_RXBUFFERS)
                    {
                        /* init the RxBuf, set length to 0. */
                        CanIf_RxBuffers[rxPduBufIndex].length = 0u;

                        /* init the Data */
                        CanIf_Memset(CanIf_RxBuffers[rxPduBufIndex].data,0u,8u);
                    }
                }
            }
        }
        
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif /* STD_ON == CANIF_READRXPDU_DATA_API */


#if(STD_ON == CANIF_SETDYNAMICTXID_API)
/*************************************************************************/
/*
 * Brief               init the Dynamic Transmit Buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_Init
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_InitDynamicTxPduCanIds(void)
{
    PduIdType txBufIndex;
    PduIdType dynamicPduIndex;
    
    for(txBufIndex = 0u; txBufIndex < CANIF_NUMBER_OF_CANTXPDUIDS; txBufIndex++) /** @req CANIF238 */
    {
        if (CANIF_PDU_TYPE_DYNAMIC == CANIF_CANTXPDUID_TYPE(txBufIndex))
        {
            /* get the Dynamic txpduId */
            dynamicPduIndex = CANIF_DYNAMICTXPDUCANIDINDEX(txBufIndex);
            
            /* init the DynamicTxPduCanId with the configured canId */
            CanIf_DynamicTxPduCanIds[dynamicPduIndex] = CANIF_CANTXPDUID_CANID(txBufIndex);
        }
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif
/*************************************************************************/
/*
 * Brief               MISRA C 2004 forbid to use memcpy() lib,only used  to 
 *                     copy data buffer of indirect address
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      *source:pointer to the source 
 *                     length:copy how much length
 * Param-Name[out]     None
 * Param-Name[in/out]  *dest:pointer to the dest 
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_ReadRxPduData
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_Memcpy
(
    P2VAR(uint8, AUTOMATIC, CANIF_APPL_DATA) dest,
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) source,
    uint32 length   
)
{
    while (length > 0u)
    {
        /* MISRA-C:2004 Rule 17.4
         * Msg(4:0489) The integer value 1 is being added or subtracted from a pointer.
         */
        if ((dest != NULL_PTR) && (source != NULL_PTR))
        {
            *dest = *source;
            dest++;
            source++;
        }
        else
        {
            break;
        }
        
        length--;
    }
    
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_READRXPDU_DATA_API)
/*************************************************************************/
/*
 * Brief               MISRA C 2004 forbid to use memset() lib,only used  to  
 *                     set data buffer ofindirect address
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      source:the source which want to be set 
 *                     length:copy how much length
 * Param-Name[out]     None
 * Param-Name[in/out]  *dest:pointer to the dest 
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_InitRxBuffer
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_Memset
(   P2VAR(uint8, AUTOMATIC, CANIF_APPL_DATA) dest,
    const uint8 source,
    uint32 length   
)
{
    while (length > 0u)
    {
       /* MISRA-C:2004 Rule 17.4
        * Msg(4:0489) The integer value 1 is being added or subtracted from a pointer.
        */
        if (dest != NULL_PTR)
        {
            *dest = source;
            dest++;
        }
        else
        {
            break;
        }
        
        length--;
    }
    
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
/*************************************************************************/
/*
 * Brief               Can pdu pack for transmit
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_Init
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_PduPack
(
    PduIdType CanTxPduId, 
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
    P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) canPdu
)
{
#if(STD_ON == CANIF_SETDYNAMICTXID_API)
    if (CANIF_PDU_TYPE_DYNAMIC == CANIF_CANTXPDUID_TYPE(CanTxPduId))
    {
        /* dynamic LPDU,get the canId in CanIf_DynamicTxPduCanIds */
        canPdu->id = CanIf_DynamicTxPduCanIds[CANIF_DYNAMICTXPDUCANIDINDEX(CanTxPduId)];
    }
    else
#endif
    {
        /* not dynamic Lpdu */
        canPdu->id = CANIF_CANTXPDUID_CANID(CanTxPduId);
    }

    /* get PDU length */
    canPdu->length = (uint8)PduInfoPtr->SduLength;

    /* get SDU */
    canPdu->sdu= PduInfoPtr->SduDataPtr;

    /* get PDU handle */
    canPdu->swPduHandle = CanTxPduId;

    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               direct switch to Stopped Mode,and do buffer clear,
 *                     and controller mode setting.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: switch to Stopped mode successful
 *                     E_NOT_OK:switch to Stopped mode failure
 * PreCondition        None
 * CallByAPI           CanIf_SetControllStarted,CanIf_SetControllSleep,
 *                     CanIf_SetControllStopped
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeBufInStopped(uint8 Controller)
{
   
    /* free tx buffer if enabled */
    #if(STD_ON == CANIF_TX_BUFFER_USED) 
    SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    CanIf_FreeControllerTxBuffer(Controller);
    SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_TXBUFFER);
    #endif

    /* (re-)init the rx Buffer */
    #if(STD_ON == CANIF_READRXPDU_DATA_API)
    /* Init CanIf RxBuffer */
    CanIf_InitRxBuffer(Controller);
    #endif       

	#if((STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)||(STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API))
    /* set the rx and tx Notify status buffer Of this controller */
    CanIf_FreeRxTxNotifyStatus(Controller);
	#endif

    #if STD_ON == CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
    CanIf_TxConfirmationState[Controller] = CANIF_NO_NOTIFICATION;
    #endif

	#if(STD_ON == CANIF_SETDYNAMICTXID_API)
    /* init dynamic CAN ID array */
    CanIf_InitDynamicTxPduCanIds();
	#endif

    return;        
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Search the index of RxPdu Config due to the Hrh and
 *                     CanId,
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: find the Hrh,and canId pass the filter 
 *                     E_NOT_OK:Hrh not find or canId haven't pass the filter
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication
 *                     
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_RxPduHrhSearch
(
    uint8 hrhIndex,
    Can_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex
)
{
#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_LINEAR)
    PduIdType pduIdIndex;
#endif
    Std_ReturnType result = E_NOT_OK;

    /* MISRA-C:2004 Rule 19.11
     * Msg(4:3332) The macro 'CANIF_LINEAR' used in this '#if' or '#elif' expression is not defined.
     */    
    /* Check the filter type */                
    #if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_LINEAR) 
    {
        /*Linear search from start index to stop index */
        for (pduIdIndex = CANIF_HRH_PDUID_STARTINDEX(hrhIndex); 
            ((pduIdIndex <= CANIF_HRH_PDUID_STOPINDEX(hrhIndex)) && (E_NOT_OK == result));
             pduIdIndex++)
        {
            if (CanId == CANIF_CANRXPDUID_CANID(pduIdIndex))
            {
                *rxPduIndex = pduIdIndex;
                result = E_OK;
            }
        }/* end of for */
    } 
    #elif (CANIF_SOFTWARE_FILTER_TYPE == CANIF_BINARY) 
    {
        result = CanIf_BinSearch_PduIndex(CANIF_HRH_PDUID_STARTINDEX(hrhIndex),
                                          CANIF_HRH_PDUID_STOPINDEX(hrhIndex),
                                          CanId,
                                          rxPduIndex
                                         );
    }
    #endif
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_BINARY) 
/*************************************************************************/
/*
 * Brief               Binary Search the index of RxPdu Config due to the Hrh and
 *                     CanId,
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: find the Pdu
 *                     E_NOT_OK:Pdu not find 
 * PreCondition        None
 * CallByAPI           CanIf_RxPduHrhSearch
 *                     
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_BinSearch_PduIndex
(
    uint8 lowValue,
    uint8 highValue,
    Can_IdType key_CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC)pduIndex
)
{
    uint8 midValue;
    Std_ReturnType result = E_NOT_OK;

    while ((lowValue <= highValue) && (E_NOT_OK == result))
    {
        midValue = (lowValue + highValue)/2 ;

        if (key_CanId == CANIF_CANRXPDUID_CANID(midValue))
        {
            *pduIndex = midValue;
            result =  E_OK;
        }

        else if (key_CanId > CANIF_CANRXPDUID_CANID(midValue))
        {
            lowValue = midValue + 1u;
        }
        else /*(key_CanId < CANIF_CANRXPDUID_CANID(midValue))*/
        {
            highValue = midValue - 1u;
        }
    }

    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               Binary Search the index of hrh Config due to the Hrh                   
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: find the Hrh
 *                     E_NOT_OK:Hrh not find 
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication
 *                     
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_BinSearch_HrhIndex
(
    uint8 lowValue,
    uint8 highValue,
    uint8 key_HrhId,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC)hrhIndex
)
{
    uint8 midValue;
    Std_ReturnType result = E_NOT_OK;

    while ((lowValue <= highValue) && (E_NOT_OK == result))
    {
        midValue = (lowValue + highValue)/2 ;

        if (key_HrhId == CANIF_HRHIDSYMREF(midValue))
        {
            *hrhIndex = midValue;
            result =  E_OK;
        }

        else if (key_HrhId > CANIF_HRHIDSYMREF(midValue))
        {
            lowValue = midValue + 1u;
        }
        else /*(key_HrhId < CANIF_HRHIDSYMREF(midValue))*/
        {
            highValue = midValue - 1u;
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif



/*************************************************************************/
/*
 * Brief               do RxIndication Handle with the correct rxPuIndex
 *                     which get by hrh and CanId through CanIf_RxPduHrhSearch
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Void
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication
 *                     
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_RxIndicationHandle
(
    PduIdType pduIdIndex,
    uint8 pduLength,
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) CanSduPtr
)
{
    /** @req CANIF297 @req CANIF166 */
    /* rx status notify enabled */
    #if (STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)
    /* set rx status to CANIF_TX_RX_NOTIFICATION */
    CanIf_RxNotifStatus[CANIF_RXNOTIFYINDEX(pduIdIndex)] = CANIF_TX_RX_NOTIFICATION;
    #endif

    /** @req CANIF057 @req CANIF212 @req CANIF056 @req CANIF110 @req CANIF195 */
    #if (STD_ON == CANIF_READRXPDU_DATA_API)
    if (TRUE == CANIF_READRXPDU_DATA(pduIdIndex))
    {
        SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_RXBUFFER);    
        CanIf_RxBuffers[CANIF_RXBUFFERINDEX(pduIdIndex)].length = pduLength; /** @req CANIF262 */
        /* save sdu */
        CanIf_Memcpy(CanIf_RxBuffers[CANIF_RXBUFFERINDEX(pduIdIndex)].data,CanSduPtr,pduLength);
        SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_RXBUFFER);
    }
    #endif /* STD_ON == CANIF_READRXPDU_DATA_API */

    /* inform upper layer */
    if ((CANIF_RX_USER_TYPE(pduIdIndex) < CANIF_USER_MAX_COUNT)
        &&(NULL_PTR != CanIf_UserRxIndication[CANIF_RX_USER_TYPE(pduIdIndex)])
        )

    {
        PduInfoType PduInfo;
        PduIdType upLayerPduId;
        upLayerPduId = CANIF_CANRXPDUID(pduIdIndex);
        /** @req CANIF012 @req CANIF221 */
        PduInfo.SduLength = pduLength;
        
        /*MISRA-C:2004 Rule 11.5
         *Msg(4:0311) Dangerous pointer cast results 
         *in loss of const qualification.
         */
        PduInfo.SduDataPtr = (uint8 *)CanSduPtr;
        CanIf_UserRxIndication[CANIF_RX_USER_TYPE(pduIdIndex)](upLayerPduId, &PduInfo);
    }
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (STD_ON == CANIF_WAKEUP_VALIDATION) 

/*************************************************************************/
/*
 * Brief               CanIf Check Wakeup call Can_Cbk_CheckWakeup
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      canController
 *                     WakeupSource
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:Wake up check process success
 *                     E_NOT_OK:Wake up check process failure
 * PreCondition        None
 * CallByAPI           CanIf_CheckWakeup
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeupAction
(
    uint8 controller,
    EcuM_WakeupSourceType WakeupSource
)
{

    uint8 index;
    Std_ReturnType result = E_NOT_OK;

    CanIf_WakeupSupportType wakeupSupport;   
    wakeupSupport = CanIf_ControllerConfiguration[controller].CanIfWakeupSupport;

    /* @req CANIF059 */
    /* check wakeupSource is a CAN controller or a CAN transceiver. */
    if (CANIF_WAKEUP_SUPPORT_CONTROLLER == wakeupSupport)
    {   
        index = CanIf_ControllerConfiguration[controller].CanIfControllerIdRef;

        /* check whether wake up or not */
        if (E_OK == Can_Cbk_CheckWakeup(index))
        {
            /* set controller mode to CANIF_CS_STOPPED */           
            if (E_OK == CanIf_SetControllerMode(index, CANIF_CS_STOPPED))   
            {
                if ((TRUE == CanIf_DriverConfiguration.CanIfWakeupNotification)
                    &&(NULL_PTR != CanIf_DispatchConfigData.CanIfWakeupNotification))
                {
                    /* call upper layer function */ 
                    /** @req CANIF180 @req CANIF013 */
                    CanIf_DispatchConfigData.CanIfWakeupNotification(WakeupSource);
                    result = E_OK;
                }
            }
        }
    }
    else if (CANIF_WAKEUP_SUPPORT_TRANSCEIVER == wakeupSupport)
    {
        #if STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED 
        index = CanIf_ControllerConfiguration[controller].CanIfCanTrcvIndex;
        
        /* shall call CanTrcv_CB_WakeupByBus, not supported yet */
        result = CanIf_TrcvFctData[CANIF_TRCV_FCT_INDEX(index)].CanTrcvCheckWakeupFctPtr(CANIF_TRCV_ID(index));

        if (E_OK == result)
        {
            if ((TRUE == CANIF_TRCV_WAKEUP_NOTIFICATION(index))
                &&(NULL_PTR != CanIf_DispatchConfigData.CanIfWakeupNotification))
            {
                /* call upper layer function */ 
                /** @req CANIF180 @req CANIF013 */
                CanIf_DispatchConfigData.CanIfWakeupNotification(WakeupSource);
            }
        }
        #endif
    }
    else
    {
        /* wake up not supported */
    } 
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/*************************************************************************/
/*
 * Brief               CanIf Check Wakeup Validation
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      canController
 *                     WakeupSource
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK:Wake up Validation process success
 *                     E_NOT_OK:Wake up Validation process failure
 * PreCondition        None
 * CallByAPI           CanIf_CheckValidation
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType , CANIF_CODE)
CanIf_CheckWakeupValidationAction
(
    uint8 canController,
    EcuM_WakeupSourceType WakeupSource
)
{
    Std_ReturnType result = E_NOT_OK;
    
    if ((CANIF_CS_STARTED == CanIf_ControllerMode[canController])
       && (TRUE == CanIf_FirstCallRxInd[canController]))
    {   
        /** @req CANIF179 @req CANIF182 */
        if (NULL_PTR != CanIf_DispatchConfigData.CanIfWakeupValidNotification)
        {
            /* call upper layer function */
            CanIf_DispatchConfigData.CanIfWakeupValidNotification(WakeupSource);
            result = E_OK;
        }

    } 
    return result;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif  /* end of STD_ON == CANIF_WAKEUP_VALIDATION */


#if((STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)||(STD_ON == CANIF_READTXPDU_NOTIFY_STATUS_API))
/*************************************************************************/
/*
 * Brief               Re-init rxNotifyStatus and txNotifyStatus
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(void, CANIF_CODE)
CanIf_FreeRxTxNotifyStatus(uint8 Controller)
{
    PduIdType rxPduId;
    PduIdType txPduId;
    uint8 hrhIndex;

    /* re-init the txNotifStatus Of this controller */
    for (txPduId = 0u; txPduId < CANIF_MAX_NUMBER_OF_CANTXPDUIDS; txPduId++)
    {
        uint8 hthCfgIndex;
        hthCfgIndex = CANIF_HTH_REF_ID(txPduId);

        if (Controller == CanIf_HthConfigData[hthCfgIndex].CanIfCanControllerIdRef)
        {
            if (TRUE == CANIF_READTXPDU_NOTIFY_STATUS(txPduId))
            {
                CanIf_TxNotifStatus[CANIF_TXNOTIFYINDEX(txPduId)] = CANIF_NO_NOTIFICATION;
            }
        }
    }

	#if(STD_ON == CANIF_READRXPDU_NOTIFY_STATUS_API)
    /* re-init the rxNotifStatus Of this controller */
    for (hrhIndex = 0u; hrhIndex < CANIF_NUMBER_OF_HRH_CFG; hrhIndex++)
    {
        if (Controller == CanIf_HrhConfigData[hrhIndex].CanIfCanControllerHrhIdRef)
        {
            for (rxPduId = CANIF_HRH_PDUID_STARTINDEX(hrhIndex);
                 rxPduId <= CANIF_HRH_PDUID_STOPINDEX(hrhIndex);
                 rxPduId++
                )
            {
                if (TRUE == CANIF_READRXPDU_NOTIFY_STATUS(rxPduId))
                {
                    CanIf_RxNotifStatus[CANIF_RXNOTIFYINDEX(rxPduId)] = CANIF_NO_NOTIFICATION;
                }

                
            }
        }
    }
	#endif
    return;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

#if(STD_ON == CANIF_READRXPDU_DATA_API)
/*************************************************************************/
/*
 * Brief               read rx pdu data processing
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      hrhIndex,rxCfgId,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_ReadRxPduData
 */
/*************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
STATIC FUNC(Std_ReturnType, CANIF_CODE)
CanIf_ReadRxPduDataHandle
(
    uint8 hrhIndex,
    PduIdType rxCfgId,
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
)
{
    uint8 rxBufIndex;
    uint8 canControllerId; 
    Std_ReturnType result = E_NOT_OK;

    /* get controller ID */
    canControllerId = CanIf_HrhConfigData[hrhIndex].CanIfCanControllerHrhIdRef;

    if (TRUE == CANIF_READRXPDU_DATA(rxCfgId))
    {
        /* check wether in START mode */
        if (CANIF_CS_STARTED == CanIf_ControllerMode[canControllerId])
        {
            /* check Pdu Mode Rx whether Online */
            if ((CANIF_GET_RX_ONLINE == CanIf_PduMode[canControllerId]) 
                || (CANIF_GET_ONLINE == CanIf_PduMode[canControllerId]) 
                || (CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE == CanIf_PduMode[canControllerId]))
            {
                rxBufIndex = CANIF_RXBUFFERINDEX(rxCfgId);
                
                /* if not received data */
                SchM_Enter_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_RXBUFFER);  
                if ((CanIf_RxBuffers[rxBufIndex].length > 0u)
                   &&(CanIf_RxBuffers[rxBufIndex].length <= 8u))
                {
                    PduInfoPtr->SduLength = CanIf_RxBuffers[rxBufIndex].length;
                    /** @req CANIF197 @req CANIF198 */
                    /*save SDU*/
                    CanIf_Memcpy(PduInfoPtr->SduDataPtr,CanIf_RxBuffers[rxBufIndex].data,PduInfoPtr->SduLength);

                    result = E_OK;
                }
                SchM_Exit_CanIf(CANIF_INSTANCE_ID, CANIF_AREA_RXBUFFER);
            }
        }
    }
    return result;  
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
