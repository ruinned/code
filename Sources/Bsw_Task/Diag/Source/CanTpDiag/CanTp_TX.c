/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_TX.c
 *  @brief      <Briefly describe file(one line)>
 *  
 *  <Compiler: CANTP    MCU:CANTP>
 *  
 *  @author     stanley 
 *  @date       2013-4-7
 */
/*============================================================================*/


/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_TX_C_AR_MAJOR_VERSION  2
#define CANTP_TX_C_AR_MINOR_VERSION  3
#define CANTP_TX_C_AR_PATCH_VERSION  0
#define CANTP_TX_C_SW_MAJOR_VERSION  1
#define CANTP_TX_C_SW_MINOR_VERSION  0
#define CANTP_TX_C_SW_PATCH_VERSION  0

/*required CANIF version*/
#define CANTP_TX_C_CANIF_AR_MAJOR_VERSION  3
#define CANTP_TX_C_CANIF_AR_MINOR_VERSION  2

/*required PDUR version*/
#define CANTP_TX_C_PDUR_AR_MAJOR_VERSION  2
#define CANTP_TX_C_PDUR_AR_MINOR_VERSION  3

/*required DEM version*/
#define CANTP_TX_C_DEM_AR_MAJOR_VERSION  3
#define CANTP_TX_C_DEM_AR_MINOR_VERSION  1

/*required DET version*/
#define CANTP_TX_C_DET_AR_MAJOR_VERSION  2
#define CANTP_TX_C_DET_AR_MINOR_VERSION  2

/*=======[I N C L U D E S]====================================================*/
#include "CanTpDiag.h"
#include "CanTp_Internal.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/*check version information with CanTp.h*/
#if (CANTP_TX_C_AR_MAJOR_VERSION != CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with CanTp.h"
#endif
#if (CANTP_TX_C_AR_MINOR_VERSION != CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with CanTp.h"
#endif
#if (CANTP_TX_C_AR_PATCH_VERSION != CANTP_H_AR_PATCH_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Patch Version with CanTp.h"
#endif
#if (CANTP_TX_C_SW_MAJOR_VERSION != CANTP_H_SW_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Major Version with CanTp.h"
#endif
#if (CANTP_TX_C_SW_MINOR_VERSION != CANTP_H_SW_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Minor Version with CanTp.h"
#endif

/*check version information with CanTp_Internal.h*/
#if (CANTP_TX_C_AR_MAJOR_VERSION != CANTP_INTERNAL_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with CanTp_Internal.h"
#endif
#if (CANTP_TX_C_AR_MINOR_VERSION != CANTP_INTERNAL_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with CanTp_Internal.h"
#endif
#if (CANTP_TX_C_AR_PATCH_VERSION != CANTP_INTERNAL_H_AR_PATCH_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Patch Version with CanTp_Internal.h"
#endif
#if (CANTP_TX_C_SW_MAJOR_VERSION != CANTP_INTERNAL_H_SW_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Major Version with CanTp_Internal.h"
#endif
#if (CANTP_TX_C_SW_MINOR_VERSION != CANTP_INTERNAL_H_SW_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Minor Version with CanTp_Internal.h"
#endif

/*check version information with CanTp_Types.h*/
#if (CANTP_TX_C_AR_MAJOR_VERSION != CANTP_TYPES_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with CanTp_Types.h"
#endif
#if (CANTP_TX_C_AR_MINOR_VERSION != CANTP_TYPES_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with CanTp_Types.h"
#endif
#if (CANTP_TX_C_AR_PATCH_VERSION != CANTP_TYPES_H_AR_PATCH_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Patch Version with CanTp_Types.h"
#endif
#if (CANTP_TX_C_SW_MAJOR_VERSION != CANTP_TYPES_H_SW_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
#if (CANTP_TX_C_SW_MINOR_VERSION != CANTP_TYPES_H_SW_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Minor Version with CanTp_Types.h"
#endif

/*check version information with CanTp_Cfg.h*/
#if (CANTP_TX_C_AR_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_TX_C_AR_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with CanTp_Cfg.h"
#endif
#if (CANTP_TX_C_AR_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Patch Version with CanTp_Cfg.h"
#endif
#if (CANTP_TX_C_SW_MAJOR_VERSION != CANTP_CFG_H_SW_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_TX_C_SW_MINOR_VERSION != CANTP_CFG_H_SW_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Software Implementation Minor Version with CanTp_Cfg.h"
#endif
#if 0
/*check version information with DEM*/
#if (CANTP_TX_C_DEM_AR_MAJOR_VERSION != DEM_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with Dem.h"
#endif
#if (CANTP_TX_C_DEM_AR_MINOR_VERSION != DEM_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with Dem.h"
#endif
#endif
#if (CANTP_DEV_ERROR_DETECT == STD_ON)
/*Check version information with DET*/
#if (CANTP_TX_C_DET_AR_MAJOR_VERSION != DET_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with Det.h"
#endif
#if (CANTP_TX_C_DET_AR_MINOR_VERSION != DET_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with Det.h"
#endif
#endif

/*Check version information with PDUR*/
#if (CANTP_TX_C_PDUR_AR_MAJOR_VERSION != PDUR_CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with PduR_CanTp.h"
#endif
#if (CANTP_TX_C_PDUR_AR_MINOR_VERSION != PDUR_CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with PduR_CanTp.h"
#endif

/*Check version information with CANIF*/
#if (CANTP_TX_C_CANIF_AR_MAJOR_VERSION != CANIF_H_AR_MAJOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Major Version with CanIf.h"
#endif
#if (CANTP_TX_C_CANIF_AR_MINOR_VERSION != CANIF_H_AR_MINOR_VERSION)
  #error "CanTp_TX.c : Mismatch in Specification Minor Version with CanIf.h"
#endif
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               construct FF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct FF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructFFPci(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               construct SF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructSFPci(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               construct CF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructCFPci(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Get the information of received FC frame.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     FcInfoPtr information of the received FC
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a FC received and the channel is
 *                     waiting for it.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_GetChannelFCInfo(
    uint8 ChannelIdx,
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr);

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               Get TX-SDU configuration by TX-NSduId.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TxSduId ID of the TX SDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched with TxSduId
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    uint8 sduIdx;

    for (sduIdx = 0; (NULL_PTR == result) && (sduIdx < CANTP_TXNSDU_NUMBER); sduIdx++)
    {
        if (CanTp_CfgData.TxNSdus[sduIdx].TxNSduId == TxSduId)
        {
            /*find a matched TxNSdu configuration, break the loop*/
            result = &CanTp_CfgData.TxNSdus[sduIdx];
        }
    }

    return result;
}

/*************************************************************************/
/*
 * Brief               Get TX-SDU configuration by received FC NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of the NPDU ID of received FC
 *                     CanTpRxPduPtr pointer to the data of received FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByFCNpdu(	PduIdType CanTpRxPduId,
    						P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    uint8 txNSduIdx;
    CanTp_FramePCIType frameType;

    for (txNSduIdx = 0; (NULL_PTR == result) && (txNSduIdx < CANTP_TXNSDU_NUMBER); txNSduIdx++)
    {
        /*only Functional TX NSDU need configure RX FCNPDUID*/
        if (CANTP_PHYSICAL == CanTp_CfgData.TxNSdus[txNSduIdx].TxTaType)
        {
			#if (CANTP_PADDING_NEEDED == STD_ON)
            /*check CAN frame DLC according to the padding information*/
            if ((FALSE == CanTp_CfgData.TxNSdus[txNSduIdx].TxPaddingActivation)
                || (CanTpRxPduPtr->SduLength == CANTP_CAN_FRAME_LEN_MAX))
            {
			#endif

				#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                if (CANTP_STANDARD == CanTp_CfgData.TxNSdus[txNSduIdx].AddressingMode)
                {
				#endif
                    /*check the standard addressing TX-NSDU, only FC accepted*/
                    frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK;
                    if ((CanTp_CfgData.TxNSdus[txNSduIdx].RxFcNPduId == CanTpRxPduId)
                        && (CANTP_FTYPE_FC == frameType))
                    {
                        result = &CanTp_CfgData.TxNSdus[txNSduIdx];
                    }
				#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
                }
                else if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_EX)
                {
                    /*check the extended addressing TX-NSDU, only FC accepted*/
                    frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX] & CANTP_FTYPE_MASK;
                    if ((CanTp_CfgData.TxNSdus[txNSduIdx].RxFcNPduId == CanTpRxPduId)
                        && (CanTp_CfgData.TxNSdus[txNSduIdx].Sa ==  CanTpRxPduPtr->SduDataPtr[CANTP_TA_OFFSET_EX])
                        && (CANTP_FTYPE_FC == frameType))
                    {
                        result = &CanTp_CfgData.TxNSdus[txNSduIdx];
                    }
                }
                else
                {
                    /*configured as extended addressing format, but the received NPDU not has enough data space*/
                }
				#endif

			#if (CANTP_PADDING_NEEDED == STD_ON)
            } /*END OF padding checking*/
            #if (CANTP_DEV_ERROR_DETECT == STD_ON)
            else
            {
                /*invalid CAN frame data length*/
                Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_RX_LENGTH);
            }
			#endif

            #endif /* ENDOF (CANTP_PADDING_NEEDED == STD_ON)*/
        } /*END OF if (CANTP_PHYSICAL == CanTp_CfgData.TxNSdus[txNSduIdx].TxTaType)*/
    } /*END OF for loop*/
    return result;
}

/*************************************************************************/
/*
 * Brief               Get Tx-SDU configuration by Tx NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpTxPduId ID of the data NPDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId)
{

    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    uint8 txNSduIdx;

    for (txNSduIdx = 0; (NULL_PTR == result) && (txNSduIdx < CANTP_TXNSDU_NUMBER); txNSduIdx++)
    {
        if (CanTp_CfgData.TxNSdus[txNSduIdx].TxNPduId == CanTpTxPduId)
        {
            result = &CanTp_CfgData.TxNSdus[txNSduIdx];
        }
    }
    return result;
}

/*************************************************************************/
/*
 * Brief               Handle received FC.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for a FC from
 *                     the receiver, and FC received.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleFCReception(uint8 ChannelIdx)
{
    CanTp_FCInfoType fcInfo;
    PduIdType nSduId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;

    CanTp_GetChannelFCInfo(ChannelIdx, &fcInfo);
    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId);

    switch (fcInfo.FcFs)
    {
        case CANTP_FC_FS_CTS:
        {
            /*Clear to send. Get the channel ready to send CF, and make STmin timer timeout*/
            CanTp_ConstructCFPci(ChannelIdx);
            CanTp_Channels[ChannelIdx].CurrentBs = fcInfo.FcBS;
            CanTp_Channels[ChannelIdx].STminTimer.Started = TRUE;
            CanTp_Channels[ChannelIdx].STminTimer.RemainTime = 0;
            CanTp_Channels[ChannelIdx].STminTimer.FcSTMin = fcInfo.FcSTMin;
            /*transit to state SENDCF*/
            CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txNSduCfgPtr->Ncs;
            (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
            CanTp_Channels[ChannelIdx].HandleType = CANTP_FTYPE_CF;
            CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_SENDCF;
            break;
        }
        case CANTP_FC_FS_WT:
        {
            /*reset the BS timer*/
            CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txNSduCfgPtr->Nbs;
            (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
            break;
        }
        case CANTP_FC_FS_OVFLW:
        {
            /*No buffer in receiver*/
            /*release resource of channel*/
            CanTp_InitChannel(ChannelIdx);
            /*notify upper with NO_BUFFER*/
            PduR_CanTpTxConfirmation(nSduId, NTFRSLT_E_NO_BUFFER);
            break;
        }
        default:
        {
            /*Invalid FS value*/
            /*release resource of channel*/
            CanTp_InitChannel(ChannelIdx);
            /*notify upper with NO_BUFFER*/
            PduR_CanTpTxConfirmation(nSduId, NTFRSLT_E_INVALID_FS);
            break;
        }
    } /*ENDOF switch (fcInfo.FcFs)*/

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_FC_RECEIVED);
}

/*************************************************************************/
/*
 * Brief               Handle transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel has accepted a transmitting
 *                     request.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitReq(uint8 ChannelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;

    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);

    if (((CANTP_STANDARD == txNSduCfgPtr->AddressingMode)
            && (CANTP_SF_LEN_MAX_STD < CanTp_Channels[ChannelIdx].SduDataTotalCount))
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
        || ((CANTP_EXTENDED == txNSduCfgPtr->AddressingMode)
            && (CANTP_SF_LEN_MAX_EX < CanTp_Channels[ChannelIdx].SduDataTotalCount))
		#endif
				)
    {
        /*larger SDU transmit request*/
        CanTp_Channels[ChannelIdx].HandleType = CANTP_FTYPE_FF;
        CanTp_Channels[ChannelIdx].RootState = CANTP_LARGE_TRANSMITTING;
        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_START;
    }
    else
    {
        /*SF transmit request*/
        CanTp_Channels[ChannelIdx].HandleType  = CANTP_FTYPE_SF;
        CanTp_Channels[ChannelIdx].RootState = CANTP_SF_TRANSIMITTING;
        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_SF_START;
    }

    /*start CS timer*/
    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txNSduCfgPtr->Ncs;
    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);

    /*construct PCI information*/
    if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType)
    {
        /*construct FF PCI information*/
        CanTp_ConstructFFPci(ChannelIdx);

    }
    else
    {
        /*construct SF PCI information*/
        CanTp_ConstructSFPci(ChannelIdx);

    }

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].EventFlags = 0;
}


/*************************************************************************/
/*
 * Brief               Handle transmit cancel request event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel has accepted a cancel transmitting
 *                     request.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCancel(uint8 ChannelIdx)
{
    PduIdType sduId;

    sduId = CanTp_Channels[ChannelIdx].NSduId;

    /*Release the resource related with the channel*/
    CanTp_InitChannel(ChannelIdx);

    /*Notify the upper*/
    PduR_CanTpTxConfirmation(sduId, NTFRSLT_E_CANCELATION_OK);
}

/*************************************************************************/
/*
 * Brief               Handle confirmation for data transmit.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for a confirmation
 *                     of transmitted data.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCfm(uint8 ChannelIdx)
{
    PduIdType nSduId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;

    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId);

    if (CANTP_TX_SF_V4SF_CFM == CanTp_Channels[ChannelIdx].Substate)
    {
        /*Confirmation for SF data transmit. The end of the SF transmitting*/
        /*Release the channel resource*/
        CanTp_InitChannel(ChannelIdx);

        /*Notify the upper*/
        PduR_CanTpTxConfirmation(nSduId, NTFRSLT_OK);
    }
    else /*Confirmation for large SDU data transmit*/
    {
        /*update next SN number*/
        CanTp_Channels[ChannelIdx].CurrentCfSn = (uint8)(CanTp_Channels[ChannelIdx].CurrentCfSn + 1) & CANTP_CF_SN_MASK;

        if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType)
        {
            /*confirmation for FF, transit to state V4FC and start BS timer*/
            CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4FC;
            CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nbs;
            (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
        }
        else /*confirmation for CF*/
        {
            if (0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                /*SDU transmitting finished*/
                /*release resource of channel*/
                CanTp_InitChannel(ChannelIdx);
                /*notify the upper*/
                PduR_CanTpTxConfirmation(nSduId, NTFRSLT_OK);
            }
            else /*whole SDU transmitting not finished*/
            {
                /*update handled CF count*/
                CanTp_Channels[ChannelIdx].HandledCfCount++;

                if (CanTp_Channels[ChannelIdx].HandledCfCount == CanTp_Channels[ChannelIdx].CurrentBs)
                {
                    /*current block finished*/
                    /*wait another FC*/
                    CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4FC;
                    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
                    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nbs;
                    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
                }
                else /*current block not finished yet*/
                {
                    /*construct the next CF PCI, and transit to SENDCF, start STmin and CS*/
                    CanTp_ConstructCFPci(ChannelIdx);
                    CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_SENDCF;
                    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
                    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Ncs;
                    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);

                    CanTp_Channels[ChannelIdx].STminTimer.Started = TRUE;
                    CanTp_Channels[ChannelIdx].STminTimer.RemainTime = CanTp_Channels[ChannelIdx].STminTimer.FcSTMin;
                    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].STminTimer.StartTime);
                }
            } /*ENDOF whole SDU transmitting not finished*/
        } /*END OF confirmation for CF*/
    } /*END OF Confirmation for large SDU data transmit*/

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_TX_CONFIRMED);
}

/*************************************************************************/
/*
 * Brief               Back ground handling for SF transmitting.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get TX buffer
 *                     so that can transmit the SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleSFStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    BufReq_ReturnType bufRslt = BUFREQ_E_NOT_OK;
    PduLengthType dataSize;
    PduIdType nSduId;

    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0;
    bufRslt = PduR_CanTpProvideTxBuffer(CanTp_Channels[ChannelIdx].NSduId, &pduInfoPtr, 0);

    /*Handle according to the request buffer result*/
    switch (bufRslt)
    {
        case BUFREQ_OK:
        {
            /*get length of the data copy, use the minimum among two buffers and the SDU remain data*/
            dataSize = CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen;
            if (dataSize > pduInfoPtr->SduLength)
            {
                dataSize = pduInfoPtr->SduLength;
            }

            if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
            }

            /*copy data to local buffer*/
            CanTp_MemoryCopy(
                pduInfoPtr->SduDataPtr,
                &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                dataSize);
            CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;
            CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen + dataSize;
            CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
            if (0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                /*SF data copy finished, send it*/
                txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId);
                /*construct CANIF transmit request*/
                pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].LocalBuf;

                #if (CANTP_PADDING_NEEDED == STD_ON)
                if (TRUE == txSduCfgPtr->TxPaddingActivation)
                {
                    /*Padding with specific value*/
                    CanTp_MemorySet(
                        &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                        CANTP_PADDING_BYTE,
                        CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen);
                    /*Set transmit request length*/
                    pduInfo.SduLength = CANTP_CAN_FRAME_LEN_MAX;
                }
                else
                {
				#endif
                    /*Set transmit request length*/
                    pduInfo.SduLength = CanTp_Channels[ChannelIdx].LocalBufLen;
				#if (CANTP_PADDING_NEEDED == STD_ON)
                }
				#endif
                CanTp_Channels[ChannelIdx].Substate = CANTP_TX_SF_V4SF_CFM;  /*chenms 2013-9-26 19:32*/
                /*Request CANIF to transmit the SF frame*/
                (void)CanIf_Transmit(txSduCfgPtr->TxNPduId, pduInfoPtr);
                /*Transit channel to wait for confirmation of SF*/
                CanTp_Channels[ChannelIdx].Substate = CANTP_TX_SF_V4SF_CFM;
                CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
                CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nas;
                (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
            } /*END OF SF data copy finished*/
            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK:
        {
            /*Fail to get TX buffer*/
            /*Release resources related to the channel*/
            CanTp_InitChannel(ChannelIdx);

            /*Notify the upper*/
            PduR_CanTpTxConfirmation(nSduId, NTFRSLT_E_NOT_OK);
            #if (CANTP_DEV_ERROR_DETECT == STD_ON)
            /*report development error*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_MAINFUNCTION, CANTP_E_INVALID_TX_BUFFER);
            #endif
            break;
        }
        case BUFREQ_E_BUSY:
            /*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:
            /*invalid return value for PduR_CanTpProvideTxBuffer*/
            break;
    } /*END OF switch*/
}

/*************************************************************************/
/*
 * Brief               Back ground handling for large SDU transmitting, trying to get TX buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get TX buffer
 *                     so that can transmit the FF or CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    BufReq_ReturnType bufRslt = BUFREQ_E_NOT_OK;
    PduLengthType dataSize;
    PduIdType nSduId;

    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0;
    bufRslt = PduR_CanTpProvideTxBuffer(CanTp_Channels[ChannelIdx].NSduId, &pduInfoPtr, 0);

    switch (bufRslt)
    {
        case BUFREQ_OK:
        {
            /*get buffer successful*/
            CanTp_Channels[ChannelIdx].PduRBufContentPointer = pduInfoPtr->SduDataPtr;
            CanTp_Channels[ChannelIdx].PduRBufPosition = 0;
            CanTp_Channels[ChannelIdx].PduRBufRemaining = pduInfoPtr->SduLength;

            /*copy data to local buffer*/
            /*get length of the data copy, use the minimum among two buffers and the SDU remain data*/
            dataSize = CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen;
            if (dataSize > pduInfoPtr->SduLength)
            {
                dataSize = pduInfoPtr->SduLength;
            }

            if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
            }
            CanTp_MemoryCopy(
                CanTp_Channels[ChannelIdx].PduRBufContentPointer,
                &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                dataSize);

            /*update channel runtime information*/
            CanTp_Channels[ChannelIdx].PduRBufPosition = dataSize;
            CanTp_Channels[ChannelIdx].PduRBufRemaining = CanTp_Channels[ChannelIdx].PduRBufRemaining - dataSize;
            CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen + dataSize;
            CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
            CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;

            if ((0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
                || (CANTP_CAN_FRAME_LEN_MAX == CanTp_Channels[ChannelIdx].LocalBufLen))
            {
                /*current frame data copy finished*/
                txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId);

                if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType)
                {
                    /*send the FF immediately*/
                    pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].LocalBuf;
                    pduInfo.SduLength = CANTP_CAN_FRAME_LEN_MAX;
                    CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM;   /*chenms 2013-9-26 19:32*/
                    (void)CanIf_Transmit(txSduCfgPtr->TxNPduId, pduInfoPtr);

                    /*Transit channel to wait for confirmation of FF*/
                    CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM;
                    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
                    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nas;
                    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
                }
                else /*handle the CF transmitting data copy*/
                {
                    if ((TRUE == CanTp_Channels[ChannelIdx].STminTimer.Started)
                        && (0 == CanTp_Channels[ChannelIdx].STminTimer.RemainTime))
                    {
                        /*STmin timeout, send the CF immediately*/
						#if (CANTP_PADDING_NEEDED == STD_ON)
                        if ((0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
                            && (TRUE == txSduCfgPtr->TxPaddingActivation))
                        {
                            /*Padding with specific value, if it is the last CF and need padding*/
                            CanTp_MemorySet(
                                &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                                CANTP_PADDING_BYTE,
                                CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen);
                            /*Set transmit request length*/
                            pduInfo.SduLength = CANTP_CAN_FRAME_LEN_MAX;
                        }
                        else
                        {
						#endif
                            pduInfo.SduLength = CanTp_Channels[ChannelIdx].LocalBufLen;
						#if (CANTP_PADDING_NEEDED == STD_ON)
                        }
						#endif

                        pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].LocalBuf;

                        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM;   /*chenms 2013-9-26 19:32*/
                        (void)CanIf_Transmit(txSduCfgPtr->TxNPduId, pduInfoPtr);
                        /*Transit channel to wait for confirmation of CF*/
                        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM;
                        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
                        CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nas;
                        (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);

                        CanTp_Channels[ChannelIdx].STminTimer.Started = FALSE;
                    }
                    else /*STmin not time out*/
                    {
                        /*transit to state SENDCF, wait for sending next round*/
                        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_SENDCF;
                    }
                } /*END OF handle the CF transmitting data copy*/

            } /*END OF current frame data copy*/
            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK:
        {
            /*Fail to get TX buffer*/
            /*Release resources related to the channel*/
            CanTp_InitChannel(ChannelIdx);

            /*Notify the upper*/
            PduR_CanTpTxConfirmation(nSduId, NTFRSLT_E_NOT_OK);
            #if (CANTP_DEV_ERROR_DETECT == STD_ON)
            /*report development error*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_MAINFUNCTION, CANTP_E_INVALID_TX_BUFFER);
            #endif
            break;
        }
        case BUFREQ_E_BUSY:
            /*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:
            /*invalid return value*/
            break;
    } /*END OF switch (bufRslt)*/
}

/*************************************************************************/
/*
 * Brief               Back ground handling for sending CFs.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is in the state of sending the
 *                     remain CF of current block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeSendCF(uint8 ChannelIdx)
{
    PduLengthType dataSize;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    PduInfoType pduInfo;

    dataSize = CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen;
    if ((0 != dataSize)
        && (0 != CanTp_Channels[ChannelIdx].SduDataRemaining))
    {
        /*current frame not construct OK*/
        /*get length of the data copy, use the minimum among two buffers and the SDU remain data*/
        if (dataSize > CanTp_Channels[ChannelIdx].PduRBufRemaining)
        {
            dataSize = CanTp_Channels[ChannelIdx].PduRBufRemaining;
        }

        if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
        {
            dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
        }

        CanTp_MemoryCopy(
            &CanTp_Channels[ChannelIdx].PduRBufContentPointer[CanTp_Channels[ChannelIdx].PduRBufPosition],
            &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
            dataSize);

        /*update channel buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen + dataSize;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
        CanTp_Channels[ChannelIdx].PduRBufPosition = CanTp_Channels[ChannelIdx].PduRBufPosition + dataSize;
        CanTp_Channels[ChannelIdx].PduRBufRemaining = CanTp_Channels[ChannelIdx].PduRBufRemaining - dataSize;
        CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;
    } /*ENDOF construct current frame*/

    if ((0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
        || (CANTP_CAN_FRAME_LEN_MAX == CanTp_Channels[ChannelIdx].LocalBufLen))
    {
        /*current frame data ready*/
        txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);

        #if (CANTP_PADDING_NEEDED == STD_ON)
        if ((0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
            && (TRUE == txSduCfgPtr->TxPaddingActivation))
        {
            /*Padding with specific value, if it is the last CF and need padding*/
            CanTp_MemorySet(
                &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                CANTP_PADDING_BYTE,
                CANTP_CAN_FRAME_LEN_MAX - CanTp_Channels[ChannelIdx].LocalBufLen);
            /*Set transmit request length*/
            CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_CAN_FRAME_LEN_MAX;
        }
		#endif

        /*check if STMin timeout occurred to send the frame*/
        if ((TRUE == CanTp_Channels[ChannelIdx].STminTimer.Started)
            && (0 == CanTp_Channels[ChannelIdx].STminTimer.RemainTime))
        {
            pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].LocalBuf;
            pduInfo.SduLength = CanTp_Channels[ChannelIdx].LocalBufLen;

            CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM; /*chenms 2013-9-26 19:32*/
            (void)CanIf_Transmit(txSduCfgPtr->TxNPduId, &pduInfo);
            /*Transit channel to wait for confirmation of CF*/
            CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_V4DATA_CFM;
            CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
            CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = txSduCfgPtr->Nas;
            (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);

            CanTp_Channels[ChannelIdx].STminTimer.Started = FALSE;
        }
    }
    else /*not enough data provided by upper*/
    {
        /*transit to TX_LARGESTART in order to get more buffer from upper*/
        CanTp_Channels[ChannelIdx].Substate = CANTP_TX_LARGE_START;
    }
}

/*============Internal STATIC Function Implementation*/
/*************************************************************************/
/*
 * Brief               construct FF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct FF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructFFPci(uint8 ChannelIdx)
{
    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    #endif
    uint16 pciDl = 0;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);
    #endif
    pciDl = CanTp_Channels[ChannelIdx].SduDataTotalCount;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == txNSduCfgPtr->AddressingMode)
    {
	#endif
        /*standard FF*/
        /*SDU length low nibble*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_FF_FFDL_LOW_OFFSET_STD] = (uint8)pciDl;
        /*SDU length high nibble and Frame type*/
        pciDl >>= CANTP_OFFSET_FFDL_HIGNT_PART;
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_FF_FFDL_HIGH_OFFSET_STD] = CANTP_FTYPE_FF|(uint8)pciDl;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_FF_DATA_OFFSET_STD;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_FF_DATA_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*extended FF*/
        /*TA*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_TA_OFFSET_EX] = txNSduCfgPtr->Ta;

        /*SDU length low nibble*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_FF_FFDL_LOW_OFFSET_EX] = (uint8)pciDl;
        /*SDU length high nibble and Frame type*/
        pciDl >>= CANTP_OFFSET_FFDL_HIGNT_PART;
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_FF_FFDL_HIGH_OFFSET_EX] = CANTP_FTYPE_FF|(uint8)pciDl;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_FF_DATA_OFFSET_EX;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_FF_DATA_OFFSET_EX;
    }
	#endif
}

/*************************************************************************/
/*
 * Brief               construct SF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructSFPci(uint8 ChannelIdx)
{
    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    #endif
    uint16 pciDl = 0;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);
    #endif
    pciDl = CanTp_Channels[ChannelIdx].SduDataTotalCount;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == txNSduCfgPtr->AddressingMode)
    {
	#endif
        /*standard SF*/
        /*Frame type and SF DL*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_STD] = CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK);
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_SF_DATA_OFFSET_STD;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_SF_DATA_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*extended SF*/
        /*TA*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_TA_OFFSET_EX] = txNSduCfgPtr->Ta;

        /*Frame type and SF DL*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK);
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_SF_DATA_OFFSET_EX;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_SF_DATA_OFFSET_EX;
    }
	#endif
}

/*************************************************************************/
/*
 * Brief               construct CF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to construct CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_ConstructCFPci(uint8 ChannelIdx)
{
    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    #endif

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);
    #endif

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == txNSduCfgPtr->AddressingMode)
    {
	#endif
        /*Standard SDU*/
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_STD] = CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].CurrentCfSn;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_CF_DATA_OFFSET_STD;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_CF_DATA_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else /*Extended SDU*/
    {
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_TA_OFFSET_EX] = txNSduCfgPtr->Ta;
        CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].CurrentCfSn;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].LocalBufLen = CANTP_CF_DATA_OFFSET_EX;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_CF_DATA_OFFSET_EX;
    }
	#endif
}

/*************************************************************************/
/*
 * Brief               Get the information of received FC frame.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     FcInfoPtr information of the received FC
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a FC received and the channel is
 *                     waiting for it.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_GetChannelFCInfo(
    uint8 ChannelIdx,
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr)
{
    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    #endif
    uint8 offset;
    uint8 stMinRaw;
    TickType stMinUs;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTp_Channels[ChannelIdx].NSduId);
    #endif

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == txNSduCfgPtr->AddressingMode)
    {
	#endif
        /*standard SDU*/
        offset = CANTP_PCI_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*extended SDU*/
        offset = CANTP_PCI_OFFSET_EX;
    }
	#endif

    FcInfoPtr->FcFs = CanTp_Channels[ChannelIdx].FcBuf[offset] & CANTP_FC_FS_MASK;
    FcInfoPtr->FcBS = CanTp_Channels[ChannelIdx].FcBuf[offset + 1];

    /*calculate the STMin in tick unit. Round the value to the nearest integer, that is
     * if it is 0.49, then the integer will be 0, 0.50 will be 1.*/
    stMinRaw = CanTp_Channels[ChannelIdx].FcBuf[offset + 2];

    if (CANTP_FC_STMIN_MS_LIMIT >= stMinRaw)
    {
        /*ms per bit*/
        stMinUs = stMinRaw * CANTP_FC_STMIN_MS_UNIT;
    }
    else if ((CANTP_FC_STMIN_US_LOW <= stMinRaw)
        && (CANTP_FC_STMIN_US_UPPER >= stMinRaw))
    {
        /*F1-F9 100us per step*/
        stMinUs = (stMinRaw - 0xF0) * CANTP_FC_STMIN_US_UNIT;
    }
    else
    {
        /*invalid value, use 7F*/
        stMinUs = CANTP_FC_STMIN_MS_LIMIT * CANTP_FC_STMIN_MS_UNIT;
    }

    /*transit to value in unit tick*/
    FcInfoPtr->FcSTMin = (stMinUs + (TICK_DURATION >> 1U)) / TICK_DURATION;
}

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
