/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp.c
 *  @brief      Public functions implementation
 *  
 *  
 *  @author     stanley
 *  @date       2013-4-7 
 */
/*============================================================================*/


/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2013-4-7  stanley    create
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_C_AR_MAJOR_VERSION  2
#define CANTP_C_AR_MINOR_VERSION  3
#define CANTP_C_AR_PATCH_VERSION  0
#define CANTP_C_SW_MAJOR_VERSION  1
#define CANTP_C_SW_MINOR_VERSION  0
#define CANTP_C_SW_PATCH_VERSION  0

/*required CANIF version*/
#define CANTP_C_CANIF_AR_MAJOR_VERSION  3
#define CANTP_C_CANIF_AR_MINOR_VERSION  2

/*required PDUR version*/
#define CANTP_C_PDUR_AR_MAJOR_VERSION  2
#define CANTP_C_PDUR_AR_MINOR_VERSION  3

/*required DEM version*/
#define CANTP_C_DEM_AR_MAJOR_VERSION  3
#define CANTP_C_DEM_AR_MINOR_VERSION  1

/*required DET version*/
#define CANTP_C_DET_AR_MAJOR_VERSION  2
#define CANTP_C_DET_AR_MINOR_VERSION  2

/*=======[I N C L U D E S]====================================================*/
#include "CanTpDiag.h"
#include "CanTp_Internal.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/*check version information with CanTp.h*/
#if (CANTP_C_AR_MAJOR_VERSION != CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with CanTp.h"
#endif
#if (CANTP_C_AR_MINOR_VERSION != CANTP_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with CanTp.h"
#endif
#if (CANTP_C_AR_PATCH_VERSION != CANTP_H_AR_PATCH_VERSION)
  #error "CanTp.c : Mismatch in Specification Patch Version with CanTp.h"
#endif
#if (CANTP_C_SW_MAJOR_VERSION != CANTP_H_SW_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Major Version with CanTp.h"
#endif
#if (CANTP_C_SW_MINOR_VERSION != CANTP_H_SW_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Minor Version with CanTp.h"
#endif

/*check version information with CanTp_Internal.h*/
#if (CANTP_C_AR_MAJOR_VERSION != CANTP_INTERNAL_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with CanTp_Internal.h"
#endif
#if (CANTP_C_AR_MINOR_VERSION != CANTP_INTERNAL_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with CanTp_Internal.h"
#endif
#if (CANTP_C_AR_PATCH_VERSION != CANTP_INTERNAL_H_AR_PATCH_VERSION)
  #error "CanTp.c : Mismatch in Specification Patch Version with CanTp_Internal.h"
#endif
#if (CANTP_C_SW_MAJOR_VERSION != CANTP_INTERNAL_H_SW_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Major Version with CanTp_Internal.h"
#endif
#if (CANTP_C_SW_MINOR_VERSION != CANTP_INTERNAL_H_SW_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Minor Version with CanTp_Internal.h"
#endif

/*check version information with CanTp_Types.h*/
#if (CANTP_C_AR_MAJOR_VERSION != CANTP_TYPES_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with CanTp_Types.h"
#endif
#if (CANTP_C_AR_MINOR_VERSION != CANTP_TYPES_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with CanTp_Types.h"
#endif
#if (CANTP_C_AR_PATCH_VERSION != CANTP_TYPES_H_AR_PATCH_VERSION)
  #error "CanTp.c : Mismatch in Specification Patch Version with CanTp_Types.h"
#endif
#if (CANTP_C_SW_MAJOR_VERSION != CANTP_TYPES_H_SW_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
#if (CANTP_C_SW_MINOR_VERSION != CANTP_TYPES_H_SW_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Minor Version with CanTp_Types.h"
#endif

/*check version information with CanTp_Cfg.h*/
#if (CANTP_C_AR_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_C_AR_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with CanTp_Cfg.h"
#endif
#if (CANTP_C_AR_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION)
  #error "CanTp.c : Mismatch in Specification Patch Version with CanTp_Cfg.h"
#endif
#if (CANTP_C_SW_MAJOR_VERSION != CANTP_CFG_H_SW_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_C_SW_MINOR_VERSION != CANTP_CFG_H_SW_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Software Implementation Minor Version with CanTp_Cfg.h"
#endif
#if 0
/*check version information with DEM*/
#if (CANTP_C_DEM_AR_MAJOR_VERSION != DEM_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with Dem.h"
#endif
#if (CANTP_C_DEM_AR_MINOR_VERSION != DEM_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with Dem.h"
#endif

#endif

#if (CANTP_DEV_ERROR_DETECT == STD_ON)
/*Check version information with DET*/
#if (CANTP_C_DET_AR_MAJOR_VERSION != DET_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with Det.h"
#endif
#if (CANTP_C_DET_AR_MINOR_VERSION != DET_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with Det.h"
#endif
#endif

/*Check version information with PDUR*/
#if (CANTP_C_PDUR_AR_MAJOR_VERSION != PDUR_CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with PduR_CanTp.h"
#endif
#if (CANTP_C_PDUR_AR_MINOR_VERSION != PDUR_CANTP_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with PduR_CanTp.h"
#endif

/*Check version information with CANIF*/
#if (CANTP_C_CANIF_AR_MAJOR_VERSION != CANIF_H_AR_MAJOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Major Version with CanIf.h"
#endif
#if (CANTP_C_CANIF_AR_MINOR_VERSION != CANIF_H_AR_MINOR_VERSION)
  #error "CanTp.c : Mismatch in Specification Minor Version with CanIf.h"
#endif
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
/*CANTP global variable for module state:CANTP_ON, CANTP_OFF*/
volatile VAR(CanTp_StateType, CANTP_VAR_POWER_ON_INIT) CanTp_ModuleState = CANTP_OFF;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

#define CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
/*CANTP global variable for runtime channel controlling information*/
VAR(CanTp_ChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];

/*Rx Pool for Channels*/
VAR(CanTp_RxPoolType, CANTP_VAR_NOINIT) CanTp_ChnlPools[CANTP_CHANNEL_NUMBER];

#define CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               Store the received RXNPDU when indicated.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of received NPDU
 *                     CanTpRxPduPtr pointer to data of received NPDU
 *                     RxNSduCfgPtr pointer to configuration structure of
 *                     this SDU matched with this received NPDU
 *                     FrameType frame type
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK the received NPDU is expected
 *                     E_NOT_OK not expected NPDU
 * PreCondition        Module initialized, a NPDU received.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvt(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_FramePCIType FrameType
    );

/*************************************************************************/
/*
 * Brief               Store the received FC NPDU when indicated.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of received NPDU
 *                     CanTpRxPduPtr pointer to data of received NPDU
 *                     TxNSduCfgPtr pointer to configuration structure of
 *                     this SDU matched with this received FC NPDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK the received NPDU is expected
 *                     E_NOT_OK not expected NPDU
 * PreCondition        Module initialized, a FC NPDU received.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxFCNPduEvt(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr
    );

/*************************************************************************/
/*
 * Brief               Try to accept the transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxNSduCfgPtr pointer to configuration of the requested SDU
 *                     CanTpTxInfoPtr pointer to structure of information
 *                     of SDU to transmit
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Return E_OK, if accept; return E_NOT_OK, if reject.
 * PreCondition        Module initialized, the channel can accept transmit request.
 * CallByAPI           CanTp_Transmit
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_AcceptTransmitRequest(
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    );

/*************************************************************************/
/*
 * Brief               Handle TX event occurred to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_DispatchTxEvent(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle back ground states to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleBackGroudStates(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle RX event occurred to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_DispatchRxEvent(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle the timer for specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleTimers(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle the timer for RX channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleRxTimer(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle the timer for TX channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleTxTimer(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Update ABC timer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_UpdateABCTimer(uint8 ChannelIdx);

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               This function initializes the CanTp module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE) CanTp_Init(void)
{
    uint8 channelIdx = 0;

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
    /*if the module is working, turn off it, then it can not receive or
     transmit new N-SDUs*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        CanTp_ModuleState = CANTP_OFF;
    }
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
    /*Initialize all the channels to stop all the working channels*/
    for (channelIdx = 0; channelIdx < CANTP_CHANNEL_NUMBER; channelIdx++)
    {
        CanTp_InitChannel(channelIdx);
    }

    /*Initialize channel pools*/
    for (channelIdx = 0; channelIdx < CANTP_CHANNEL_NUMBER; channelIdx++)
    {
        CanTp_ChnlPools[channelIdx].Filled = FALSE;
        CanTp_ChnlPools[channelIdx].RxNPduId = 0xFF;
        CanTp_ChnlPools[channelIdx].PduInfo.SduDataPtr = CanTp_ChnlPools[channelIdx].DataBuf;
    }
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    /*turn the switch of module state on*/
    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
    /*CanTp start to work*/
    CanTp_ModuleState = CANTP_ON;
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/
}

/*************************************************************************/
/*
 * Brief               This function to shutdown the CanTp module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE) CanTp_Shutdown(void)
{
    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
    /*CanTp stops working*/
    CanTp_ModuleState = CANTP_OFF;
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/
}

/*************************************************************************/
/*
 * Brief               This service is used to request the transfer of segmented data.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be transmitted.
 *                     CanTpTxInfoPtr indicator of a structure with CAN N-SDU related
 *                                    data:indicator of a CAN N-SDU buffer and the length
 *                                    of this buffer.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:request can be started successfully
 *                     E_NOT_OK:request cannot be started
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_Transmit(	PduIdType CanTpTxSduId,
    													P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;

    /*check module state, handle the request only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        if (NULL_PTR == CanTpTxInfoPtr)
        {
            /*invalid transmit request information pointer*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TRANSMIT, CANTP_E_PARAM_ADDRESS);
        }
        else
        { /*information pointer OK*/
        #endif
            txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTpTxSduId);
            if (NULL_PTR != txNSduCfgPtr)
            {
                
                /*A configured TX SDU matched. Try to accepted the transmit request*/
                result = CanTp_AcceptTransmitRequest(txNSduCfgPtr, CanTpTxInfoPtr);
            } /*END OF if (NULL_PTR != txNSduCfgPtr)*/
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
            else
            {
                /*No configured TX SDU matched with this TxSduId*/
                Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TX_ID);
            }
        } /*END OF information pointer OK*/
        #endif
    } /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /*CanTp module not started*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TRANSMIT, CANTP_E_UNINIT);
    }
    #endif
    return result;
}

#if (CANTP_TC == STD_ON)
/*************************************************************************/
/*
 * Brief               This service is used to cancel the transfer of pending
 *                     CAN N-SDUs. The connection is identified by CanTpTxPduId.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be canceled.
 *                     CanTpCancelReason The reason for cancelation.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelTransmitRequest(
    PduIdType CanTpTxPduId,
    CanTp_CancelReasonType CanTpCacelReason
    )
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;

    /*check module state, handle the request only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        /*Get configuration of the specified TX-NSDU*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTpTxPduId);

        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        if (NULL_PTR == txNSduCfgPtr)
        {
            /*Invalid TxNSduId, no configuration matched*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_INVALID_TX_ID);
        }
        else
        { /*A matched configuration matched*/
        #endif
            SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/

            if ((CanTp_Channels[txNSduCfgPtr->TxChannel].NSduId == CanTpTxPduId)
                && ((CANTP_LARGE_TRANSMITTING == CanTp_Channels[txNSduCfgPtr->TxChannel].RootState)
                    || (CANTP_SF_TRANSIMITTING == CanTp_Channels[txNSduCfgPtr->TxChannel].RootState)))
            {
                /*The TX-NSDU is being transmitted*/
                CanTp_Channels[txNSduCfgPtr->TxChannel].EventFlags = CANTP_CHANNEL_EVENT_TXCANCELLED;
                /*turn off the sub-state to reject the following events for this channel transmitting*/
                CanTp_Channels[txNSduCfgPtr->TxChannel].Substate = CANTP_SUBSTATE_NONE;
                result = E_OK;
            }

            SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        } /*END OF A matched configuration matched*/
        #endif
    } /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /*CanTp module not started*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_UNINIT);
    }
    #endif
    return result;
}
#endif

/*************************************************************************/
/*
 * Brief               The main function for scheduling the CANTP.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp module initialized
 * CallByAPI           SchM
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE) CanTp_MainFunction(void)
{
    uint8 channelIdx;
    /*check module state, handle only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        for (channelIdx = 0; channelIdx < CANTP_CHANNEL_NUMBER; channelIdx++)
        {
            SchM_Enter_CanTp(CANTP_MODULE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
            /*handle channel timers*/
            CanTp_HandleTimers(channelIdx);

            /*handle events for channel*/
            if (TRUE == CanTp_ChnlPools[channelIdx].Filled)
            {
                /*receive the received PDU to channel from pool, if OK*/
                CanTp_FilterReceivedPdu(channelIdx);
                CanTp_ChnlPools[channelIdx].Filled = FALSE;
            }

            /*handle receive events*/
            CanTp_DispatchRxEvent(channelIdx);

            /*handle transmit events*/
            CanTp_DispatchTxEvent(channelIdx);

            /*handle back ground states*/
            CanTp_HandleBackGroudStates(channelIdx);
            SchM_Exit_CanTp(CANTP_MODULE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/
        }
    }
}

#if (CANTP_VERSION_INFO_API == STD_ON)
/*************************************************************************/
/*
 * Brief               This function return the version information of the CANTP module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo indicator as to where to store the version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE) CanTp_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo
    )
{
    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == versioninfo)
    {
        /*report development error*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_GETVERSIONINFO, CANTP_E_PARAM_ADDRESS);
    }
    else
    { /*NULL_PTR != versioninfo*/
    #endif /*end of #if (CANTP_DEV_ERROR_DETECT == STD_ON)*/

        versioninfo->moduleID = CANTP_MODULE_ID;
        versioninfo->instanceID = CANTP_INSTANCE_ID;
        versioninfo->sw_major_version = CANTP_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTP_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTP_C_SW_PATCH_VERSION;
        versioninfo->vendorID = CANTP_VENDOR_ID;

    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    }
    #endif
}
#endif

/*=============Module Internal Function Implementation====================*/
/*************************************************************************/
/*
 * Brief               Initialize the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of the channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module Initialization phase, or receiving/transmitting
 *                     success or failed
 * CallByAPI           CanTp_Init, CanTp Internal
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_InitChannel(uint8 ChannelIdx)
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

    /*initialize channel state information*/
    channelPtr->RootState = CANTP_IDLE;
    channelPtr->Substate = CANTP_SUBSTATE_NONE;
    channelPtr->HandleType = CANTP_FTYPE_RESEVED;

    /*initialize channel ABC timer*/
    channelPtr->ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
    channelPtr->ChannelTimer.RemainTime = 0;
    channelPtr->ChannelTimer.StartTime = 0;

    /*initialize channel flow control parameter*/
    channelPtr->CurrentBs = 0;
    channelPtr->CurrentCfSn = 0;
    channelPtr->HandledCfCount = 0;
    channelPtr->SentWftCount = 0;
    channelPtr->STminTimer.Started = FALSE;
    channelPtr->STminTimer.RemainTime = 0;
    channelPtr->STminTimer.StartTime = 0;
    channelPtr->STminTimer.FcSTMin = 0;

    /*initialize SDU information*/
    channelPtr->NSduId = 0;
    channelPtr->RcvdNPduId = 0;
    channelPtr->RcvdNSduId = 0;
    channelPtr->SduDataRemaining = 0;
    channelPtr->SduDataTotalCount = 0;

    /*initialize event flags*/
    channelPtr->EventFlags = 0;

    /*initialize buffer information*/
    channelPtr->LocalBufLen = 0;
    channelPtr->LocalBufPosition = 0;
    channelPtr->PduRBufContentPointer = NULL_PTR;
    channelPtr->PduRBufPosition = 0;
    channelPtr->PduRBufRemaining = 0;
}

/*************************************************************************/
/*
 * Brief               Utility function: Set memory with specific value.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Address memory address to set
 *                     Value value to set
 *                     Length memory length to set
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_MemorySet(
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address,
    uint8 Value,
    uint16 Length)
{
    while (Length > 0)
    {
        Length--;
        Address[Length] = Value;
    }
}

/*************************************************************************/
/*
 * Brief               Utility function:copy memory data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Source memory address to copy from
 *                     Dest memory address to copy to
 *                     Length memory length to copy
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_MemoryCopy(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) Source,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Dest,
    uint16 Length)
{
    while (Length > 0)
    {
        Length--;
        Dest[Length] = Source[Length];
    }
}

/*************************************************************************/
/*
 * Brief               Filter the Reception indication event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of the channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a NPDU received.
 * CallByAPI           CanTp_Mainfunctioin
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_FilterReceivedPdu(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr = NULL_PTR;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr = NULL_PTR;
    CanTp_FramePCIType frameType;
    volatile Std_ReturnType eventConsumed = E_NOT_OK;

    /*first try to check if the received NPDU whether is CF SF or FF*/
    rxNSduCfgPtr = CanTp_GetRxSduCfgByNpdu(
        CanTp_ChnlPools[ChannelIdx].RxNPduId,
        &(CanTp_ChnlPools[ChannelIdx].PduInfo),
        &frameType);

    if (NULL_PTR != rxNSduCfgPtr)
    {
        /*A CF or FF or SF received*/
        eventConsumed = CanTp_StoreRxNPduEvt(
            CanTp_ChnlPools[ChannelIdx].RxNPduId,
            &(CanTp_ChnlPools[ChannelIdx].PduInfo),
            rxNSduCfgPtr,
            frameType);

    } /*END OF handling receiving CF SF FF*/

    if (E_NOT_OK == eventConsumed)
    {
        /*Not SF CF FF, then check whether it is FC*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByFCNpdu(
            CanTp_ChnlPools[ChannelIdx].RxNPduId,
            &(CanTp_ChnlPools[ChannelIdx].PduInfo));
        if (NULL_PTR != txNSduCfgPtr)
        {
            /*A FC received*/
            eventConsumed = CanTp_StoreRxFCNPduEvt(
                CanTp_ChnlPools[ChannelIdx].RxNPduId,
                &(CanTp_ChnlPools[ChannelIdx].PduInfo),
                txNSduCfgPtr);
        } /*END OF receiving FC*/
    }

    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    if (E_NOT_OK == eventConsumed)
    {
        /*an unexpected or invalid received NPDU*/
        /*Not expected NPDU*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_MAINFUNCTION, CANTP_E_INVALID_RX_ID);
    }
    #endif
}

/*=================STATIC Function Implementation==============================*/
/*************************************************************************/
/*
 * Brief               Try to accept the transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxNSduCfgPtr pointer to configuration of the requested SDU
 *                     CanTpTxInfoPtr pointer to structure of information
 *                     of SDU to transmit
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Return E_OK, if accept; return E_NOT_OK, if reject.
 * PreCondition        Module initialized, the channel can accept transmit request.
 * CallByAPI           CanTp_Transmit
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_AcceptTransmitRequest(
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    )
{
    Std_ReturnType result = E_NOT_OK;
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;

    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    /*check data length of the transmit request*/
    if ((TxNSduCfgPtr->TxDl > CanTpTxInfoPtr->SduLength)
        || (CANTP_NSDU_LENGTH_LIMIT < CanTpTxInfoPtr->SduLength))
    {
        /*Length invalid*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TX_LENGTH);
    }
    else if ((CANTP_FUNCTIONAL == TxNSduCfgPtr->TxTaType)
    && (((CANTP_STANDARD == TxNSduCfgPtr->AddressingMode)
            && (CANTP_SF_LEN_MAX_STD < CanTpTxInfoPtr->SduLength)) /*SDU uses standard addressing mode*/
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
        || ((CANTP_EXTENDED == TxNSduCfgPtr->AddressingMode)
            && (CANTP_SF_LEN_MAX_EX < CanTpTxInfoPtr->SduLength)) /*SDU uses extended addressing mode*/
		#endif
			  ))
    {
        /*check TA type, invalid*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TATYPE);
    }
    else /*All request parameters are valid*/
    {
    #endif
        /*check if the channel is free*/
        channelPtr = &CanTp_Channels[TxNSduCfgPtr->TxChannel];

        SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
        if ((CANTP_IDLE == channelPtr->RootState)
            && (0 == channelPtr->EventFlags))
        {
            /*channel is free, and no event happened to it*/
            channelPtr->EventFlags = CANTP_CHANNEL_EVENT_TRANSMITREQUEST;
            channelPtr->NSduId = TxNSduCfgPtr->TxNSduId;
            channelPtr->SduDataRemaining = CanTpTxInfoPtr->SduLength;
            channelPtr->SduDataTotalCount = CanTpTxInfoPtr->SduLength;
			/*set channel as sender to avoid unexpected PDU reception*/
			channelPtr->RootState = CANTP_SF_TRANSIMITTING;

            result = E_OK;
        }
        SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    }
    #endif
    return result;
}

/*************************************************************************/
/*
 * Brief               Handle back ground states to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleBackGroudStates(uint8 ChannelIdx)
{
    /*Handle background operation for channel ,if needed*/
    switch (CanTp_Channels[ChannelIdx].Substate)
    {
        case CANTP_RX_SF_START:
        {
            /*receiving SF*/
            CanTp_RxHandleSFStart(ChannelIdx);
            break;
        }
        case CANTP_RX_LARGE_START:
        {
            /*receiving large SDU, trying to get buffer*/
            CanTp_RxHandleLargeStart(ChannelIdx);
            break;
        }
        case CANTP_TX_SF_START:
        {
            /*transmitting SF, trying to get buffer*/
            CanTp_TxHandleSFStart(ChannelIdx);
            break;
        }
        case CANTP_TX_LARGE_START:
        {
            /*transmitting large SDU, trying to get buffer*/
            CanTp_TxHandleLargeStart(ChannelIdx);
            break;
        }
        case CANTP_TX_LARGE_SENDCF:
        {
            /*transmitting CFs*/
            CanTp_TxHandleLargeSendCF(ChannelIdx);
            break;
        }
        default:
            /*No special handling needed*/
            break;
    } /*END OF switch (CanTp_Channels[ChannelIdx].Substate)*/
}

/*************************************************************************/
/*
 * Brief               Handle TX event occurred to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_DispatchTxEvent(uint8 ChannelIdx)
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_TXCANCELLED) != 0)
    {
        /*A transmit request canceled*/
        CanTp_TxHandleTransmitCancel(ChannelIdx);
    }

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_TRANSMITREQUEST) != 0)
    {
        /*A transmit request accepted*/
        CanTp_TxHandleTransmitReq(ChannelIdx);
    }
}

/*************************************************************************/
/*
 * Brief               Handle RX event occurred to the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_DispatchRxEvent(uint8 ChannelIdx)
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_CF_RECEIVED) != 0)
    {
        /*A CF frame received*/
        if ((CANTP_LARGE_RECEIVING == channelPtr->RootState)
            && (CANTP_RX_LARGE_V4CF == channelPtr->Substate))
        {
            /*A CF frame received*/
            CanTp_RxHandleCFReception(ChannelIdx);
        }
        else
        {
            channelPtr->EventFlags = channelPtr->EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_CF_RECEIVED);
        }
    }

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_FC_RECEIVED) != 0)
    {
        if ((CANTP_LARGE_TRANSMITTING == channelPtr->RootState)
            && (CANTP_TX_LARGE_V4FC == channelPtr->Substate))
        {
            /*A FC frame received*/
            CanTp_TxHandleFCReception(ChannelIdx);
        }
        else
        {
            channelPtr->EventFlags = channelPtr->EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_FC_RECEIVED);
        }
    }

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_FF_RECEIVED) != 0)
    {
        /*A FF frame received*/
        CanTp_RxHandleFFReception(ChannelIdx);
    }

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_SF_RECEIVED) != 0)
    {
        /*A SF frame received*/
        CanTp_RxHandleSFReception(ChannelIdx);
    }
}
/*************************************************************************/
/*
 * Brief               Handle the timer for specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleTimers(uint8 ChannelIdx)
{
    if (CANTP_IDLE != CanTp_Channels[ChannelIdx].RootState)
    {
        if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RootState)
            || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RootState))
        {
            /*channel is receiving SDU*/
            CanTp_HandleRxTimer(ChannelIdx);
        }
        else
        {
            /*channel is transmitting SDU*/
            CanTp_HandleTxTimer(ChannelIdx);
        }

    } /*ENDOF if (CANTP_IDLE != CanTp_Channels[ChannelIdx].RootState)*/
}

/*************************************************************************/
/*
 * Brief               Handle the timer for RX channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleRxTimer(uint8 ChannelIdx)
{
    NotifResultType ntfRslt = NTFRSLT_E_NOT_OK;
    PduIdType nSduId;

    /*Handle ABC timer*/
    /*update the time information*/
    CanTp_UpdateABCTimer(ChannelIdx);

    if (0 == CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime)
    {
        /*Channel timer timeout occurred*/
        nSduId = CanTp_Channels[ChannelIdx].NSduId;

        if (CANTP_NA == CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
        {
            /*Nar timeout*/
            ntfRslt = NTFRSLT_E_TIMEOUT_A;
        }
        else if (CANTP_NC == CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
        {
            /*Ncr timeout*/
            ntfRslt = NTFRSLT_E_TIMEOUT_CR;
        }
        else
        {
            /*other timer timeout, result is NTFRSLT_E_NOT_OK*/
        }

        /*release resource of channel*/
        CanTp_InitChannel(ChannelIdx);

        /*notify upper*/
        /*receiving failed*/
        PduR_CanTpRxIndication(nSduId, ntfRslt);
#if 0
        /*Report product error to DEM*/
        Dem_ReportErrorStatus(CANTP_E_COM, DEM_EVENT_STATUS_PREFAILED);
#endif
    } /*ENDOF channel timer timeout*/
}

/*************************************************************************/
/*
 * Brief               Handle the timer for TX channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_HandleTxTimer(uint8 ChannelIdx)
{
    TickType elapsedTick;
    NotifResultType ntfRslt = NTFRSLT_E_NOT_OK;
    PduIdType nSduId;

    /*Handle ABC timer*/
	if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
	{
        /*update the time information*/
        CanTp_UpdateABCTimer(ChannelIdx);

        if (0 == CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime)
        {
            /*Channel timer timeout occurred*/
            nSduId = CanTp_Channels[ChannelIdx].NSduId;

            if (CANTP_NA == CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
            {
                /*Nas timeout*/
                ntfRslt = NTFRSLT_E_TIMEOUT_A;
            }
            else if (CANTP_NB == CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
            {
                /*Nbs timeout*/
                ntfRslt = NTFRSLT_E_TIMEOUT_BS;
            }
            else
            {
                /*other timer timeout, result is NTFRSLT_E_NOT_OK*/
            }

            /*release resource of channel*/
            CanTp_InitChannel(ChannelIdx);

            /*notify upper*/
            /*transmitting failed*/
            PduR_CanTpTxConfirmation(nSduId, ntfRslt);
 #if 0
            /*Report product error to DEM*/
            Dem_ReportErrorStatus(CANTP_E_COM, DEM_EVENT_STATUS_PREFAILED);
#endif
        } /*ENDOF channel timer timeout*/
	}

    /*handle STmin timer for transmitter if needed*/
    if ((CANTP_LARGE_TRANSMITTING == CanTp_Channels[ChannelIdx].RootState)
        && (TRUE == CanTp_Channels[ChannelIdx].STminTimer.Started))
    {
        (void)GetElapsedCounterValue(
            CANTP_OS_COUNTER_ID,
            &CanTp_Channels[ChannelIdx].STminTimer.StartTime,
            &elapsedTick);
            
        if (CanTp_Channels[ChannelIdx].STminTimer.RemainTime <= elapsedTick)
        {
            CanTp_Channels[ChannelIdx].STminTimer.RemainTime = 0;
        }
        else
        {
            CanTp_Channels[ChannelIdx].STminTimer.RemainTime = CanTp_Channels[ChannelIdx].STminTimer.RemainTime -elapsedTick;
        }
    } /*END OF STMin timer handling*/
}

/*************************************************************************/
/*
 * Brief               Update ABC timer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A.
 * PreCondition        Module initialized.
 * CallByAPI           CanTp_MainFuncation
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_UpdateABCTimer(uint8 ChannelIdx)
{
    TickType elapsedTick;

    /*update the time information*/
    if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer)
    {
        (void)GetElapsedCounterValue(	CANTP_OS_COUNTER_ID,
            							&CanTp_Channels[ChannelIdx].ChannelTimer.StartTime,
            							&elapsedTick);
        
        if (CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime <= elapsedTick)
        {
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = 0;

        }
        else
        {
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime - elapsedTick;
        }
    } /*ENDOF update timer information*/
}

/*************************************************************************/
/*
 * Brief               Store the received RXNPDU when indicated.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of received NPDU
 *                     CanTpRxPduPtr pointer to data of received NPDU
 *                     RxNSduCfgPtr pointer to configuration structure of
 *                     this SDU matched with this received NPDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK the received NPDU is expected
 *                     E_NOT_OK not expected NPDU
 * PreCondition        Module initialized, a NPDU received.
 * CallByAPI           CanTp_Mainfunctioin
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvt(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_FramePCIType FrameType
    )
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;
    PduIdType sduId = 0;
    Std_ReturnType result = E_NOT_OK;
    PduLengthType frameDl = 0;
    PduLengthType frameDlLimit = 0;

    sduId = RxNSduCfgPtr->RxNSduId;

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
    channelPtr = &CanTp_Channels[RxNSduCfgPtr->RxChannel];

    switch (FrameType)
    {
        case CANTP_FTYPE_CF:
        {
            /*only accept it when the channel is receiving a large SDU related with this CF,
             * and it is waiting a CF.*/
            if (CANTP_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
            {
                /*invalid TAType with CF frame, wrong configuration*/
                #if (CANTP_DEV_ERROR_DETECT == STD_ON)
                Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_CONFIG);
                #endif
            }
            else if ((sduId == channelPtr->NSduId)
                && (CANTP_LARGE_RECEIVING == channelPtr->RootState))
            {
                channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_CF_RECEIVED;
                channelPtr->LocalBufLen = CanTpRxPduPtr->SduLength;
                CanTp_MemoryCopy(CanTpRxPduPtr->SduDataPtr, channelPtr->LocalBuf, CanTpRxPduPtr->SduLength);
                result = E_OK;
            }
            else
            {
                /*CF not expected*/
            }
            break;
        } /*ENDOF case CANTP_FTYPE_CF*/
        case CANTP_FTYPE_FF:
        {
            /*Only accept FF  when the channel is IDLE or it is handling SDU receiving*/
            if (CANTP_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
            {
                /*invalid TAType with FF frame, wrong configuration*/
                #if (CANTP_DEV_ERROR_DETECT == STD_ON)
                Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_CONFIG);
                #endif
            }
            else if ((CANTP_IDLE == channelPtr->RootState)
                || (CANTP_SF_RECEIVING == channelPtr->RootState)
                || (CANTP_LARGE_RECEIVING == channelPtr->RootState))
            {
                /*check data length of the FF, ignore the frame if invalid length*/
                frameDl = CanTp_RxGetFFDl(CanTpRxPduPtr->SduDataPtr, RxNSduCfgPtr);

                #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
                { /*normal addressing mode*/
                #endif
                    frameDlLimit = CANTP_SF_LEN_MAX_STD;
                #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                }
                else
                { /*extended addressing mode*/
                    frameDlLimit = CANTP_SF_LEN_MAX_EX;
                }
                #endif

                if ((frameDl <= CANTP_NSDU_LENGTH_LIMIT)
                    && (frameDl >= RxNSduCfgPtr->RxDl)
                    && (frameDl > frameDlLimit))
                {

                    /*transit the sub-state in order to ignore the following events related with the last reception*/
                    channelPtr->Substate = CANTP_SUBSTATE_NONE;
                    /*store event information*/
                    channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_FF_RECEIVED;
                    channelPtr->RcvdNPduId = CanTpRxPduId;
                    channelPtr->RcvdNSduId = sduId;
                    channelPtr->LocalBufLen = CanTpRxPduPtr->SduLength;
                    CanTp_MemoryCopy(CanTpRxPduPtr->SduDataPtr, channelPtr->LocalBuf, CanTpRxPduPtr->SduLength);
                    result = E_OK;
                } /*END OF data length checking*/
            }
            else
            {
                /*FF not expected*/
            }
            break;
        } /*ENDOF case CANTP_FTYPE_FF*/
        case CANTP_FTYPE_SF:
        {
            /*Handle FF and SF with the same strategy*/
            /*Only accept SF when the channel is IDLE or it is handling SDU receiving*/
            if ((CANTP_IDLE == channelPtr->RootState)
                || (CANTP_SF_RECEIVING == channelPtr->RootState)
                || (CANTP_LARGE_RECEIVING == channelPtr->RootState))
            {
                /*check data length of the SF, ignore the frame if invalid length*/
                frameDl = CanTp_RxGetSFDl(CanTpRxPduPtr->SduDataPtr, RxNSduCfgPtr);

                #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
                { /*normal addressing mode*/
                #endif
                    frameDlLimit = CANTP_SF_LEN_MAX_STD;
                #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                }
                else
                { /*extended addressing mode*/
                    frameDlLimit = CANTP_SF_LEN_MAX_EX;
                }
                #endif

                if ((frameDl <= frameDlLimit)
                    && (frameDl >= RxNSduCfgPtr->RxDl))
                {
                    /*transit the sub-state in order to ignore the following events related with the last reception*/
                    channelPtr->Substate = CANTP_SUBSTATE_NONE;
                    /*store event information*/
                    channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_SF_RECEIVED;
                    channelPtr->RcvdNPduId = CanTpRxPduId;
                    channelPtr->RcvdNSduId = sduId;
                    channelPtr->LocalBufLen = CanTpRxPduPtr->SduLength;
                    CanTp_MemoryCopy(CanTpRxPduPtr->SduDataPtr, channelPtr->LocalBuf, CanTpRxPduPtr->SduLength);
                    result = E_OK;
                } /*END OF length checking*/
            }
            break;
        } /*ENDOF case CANTP_FTYPE_SF*/
        default:
            /*this will never happen*/
            break;
    } /*END OF switch (frameType) */
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    return result;
}

/*************************************************************************/
/*
 * Brief               Store the received FC NPDU when indicated.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of received NPDU
 *                     CanTpRxPduPtr pointer to data of received NPDU
 *                     TxNSduCfgPtr pointer to configuration structure of
 *                     this SDU matched with this received FC NPDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK the received NPDU is expected
 *                     E_NOT_OK not expected NPDU
 * PreCondition        Module initialized, a FC NPDU received.
 * CallByAPI           CanTp_Mainfunctioin
 */
/*************************************************************************/
STATIC FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxFCNPduEvt(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr
    )
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;
    PduIdType SduId = 0;
    Std_ReturnType result = E_NOT_OK;

    SduId = TxNSduCfgPtr->TxNSduId;

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
    channelPtr = &CanTp_Channels[TxNSduCfgPtr->TxChannel];
    /*only accepted the received FC when the channel is transmitting a large SDU related with the FC NPDU, and it is waiting for a FC.
     * Otherwise ignore the frame*/
    if ((SduId == channelPtr->NSduId)
        && (CANTP_LARGE_TRANSMITTING == channelPtr->RootState))
    {
        channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_FC_RECEIVED;
        CanTp_MemoryCopy(CanTpRxPduPtr->SduDataPtr, channelPtr->FcBuf, CanTpRxPduPtr->SduLength);
        result = E_OK;
    }
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    return result;
}

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
