/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_RX.c
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
#define CANTP_RX_C_AR_MAJOR_VERSION  2
#define CANTP_RX_C_AR_MINOR_VERSION  3
#define CANTP_RX_C_AR_PATCH_VERSION  0
#define CANTP_RX_C_SW_MAJOR_VERSION  1
#define CANTP_RX_C_SW_MINOR_VERSION  0
#define CANTP_RX_C_SW_PATCH_VERSION  0

/*required CANIF version*/
#define CANTP_RX_C_CANIF_AR_MAJOR_VERSION  3
#define CANTP_RX_C_CANIF_AR_MINOR_VERSION  2

/*required PDUR version*/
#define CANTP_RX_C_PDUR_AR_MAJOR_VERSION  2
#define CANTP_RX_C_PDUR_AR_MINOR_VERSION  3

/*required DEM version*/
#define CANTP_RX_C_DEM_AR_MAJOR_VERSION  3
#define CANTP_RX_C_DEM_AR_MINOR_VERSION  1

/*required DET version*/
#define CANTP_RX_C_DET_AR_MAJOR_VERSION  2
#define CANTP_RX_C_DET_AR_MINOR_VERSION  2

/*=======[I N C L U D E S]====================================================*/
#include "CanTpDiag.h"
#include "CanTp_Internal.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/*Check version information with CanTp.h*/
#if (CANTP_RX_C_AR_MAJOR_VERSION != CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with CanTp.h"
#endif
#if (CANTP_RX_C_AR_MINOR_VERSION != CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with CanTp.h"
#endif
#if (CANTP_RX_C_AR_PATCH_VERSION != CANTP_H_AR_PATCH_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Patch Version with CanTp.h"
#endif
#if (CANTP_RX_C_SW_MAJOR_VERSION != CANTP_H_SW_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Major Version with CanTp.h"
#endif
#if (CANTP_RX_C_SW_MINOR_VERSION != CANTP_H_SW_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Minor Version with CanTp.h"
#endif

/*Check version information with CanTp_Internal.h*/
#if (CANTP_RX_C_AR_MAJOR_VERSION != CANTP_INTERNAL_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with CanTp_Internal.h"
#endif
#if (CANTP_RX_C_AR_MINOR_VERSION != CANTP_INTERNAL_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with CanTp_Internal.h"
#endif
#if (CANTP_RX_C_AR_PATCH_VERSION != CANTP_INTERNAL_H_AR_PATCH_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Patch Version with CanTp_Internal.h"
#endif
#if (CANTP_RX_C_SW_MAJOR_VERSION != CANTP_INTERNAL_H_SW_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Major Version with CanTp_Internal.h"
#endif
#if (CANTP_RX_C_SW_MINOR_VERSION != CANTP_INTERNAL_H_SW_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Minor Version with CanTp_Internal.h"
#endif

/*check version information with CanTp_Types.h*/
#if (CANTP_RX_C_AR_MAJOR_VERSION != CANTP_TYPES_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with CanTp_Types.h"
#endif
#if (CANTP_RX_C_AR_MINOR_VERSION != CANTP_TYPES_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with CanTp_Types.h"
#endif
#if (CANTP_RX_C_AR_PATCH_VERSION != CANTP_TYPES_H_AR_PATCH_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Patch Version with CanTp_Types.h"
#endif
#if (CANTP_RX_C_SW_MAJOR_VERSION != CANTP_TYPES_H_SW_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
#if (CANTP_RX_C_SW_MINOR_VERSION != CANTP_TYPES_H_SW_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Minor Version with CanTp_Types.h"
#endif

/*check version information with CanTp_Cfg.h*/
#if (CANTP_RX_C_AR_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_RX_C_AR_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with CanTp_Cfg.h"
#endif
#if (CANTP_RX_C_AR_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Patch Version with CanTp_Cfg.h"
#endif
#if (CANTP_RX_C_SW_MAJOR_VERSION != CANTP_CFG_H_SW_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_RX_C_SW_MINOR_VERSION != CANTP_CFG_H_SW_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Software Implementation Minor Version with CanTp_Cfg.h"
#endif
#if 0
/*check version information with DEM*/
#if (CANTP_RX_C_DEM_AR_MAJOR_VERSION != DEM_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with Dem.h"
#endif
#if (CANTP_RX_C_DEM_AR_MINOR_VERSION != DEM_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with Dem.h"
#endif
#endif
#if (CANTP_DEV_ERROR_DETECT == STD_ON)
/*Check version information with DET*/
#if (CANTP_RX_C_DET_AR_MAJOR_VERSION != DET_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with Det.h"
#endif
#if (CANTP_RX_C_DET_AR_MINOR_VERSION != DET_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with Det.h"
#endif
#endif

/*Check version information with PDUR*/
#if (CANTP_RX_C_PDUR_AR_MAJOR_VERSION != PDUR_CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with PduR_CanTp.h"
#endif
#if (CANTP_RX_C_PDUR_AR_MINOR_VERSION != PDUR_CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with PduR_CanTp.h"
#endif

/*Check version information with CANIF*/
#if (CANTP_RX_C_CANIF_AR_MAJOR_VERSION != CANIF_H_AR_MAJOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Major Version with CanIf.h"
#endif
#if (CANTP_RX_C_CANIF_AR_MINOR_VERSION != CANIF_H_AR_MINOR_VERSION)
  #error "CanTp_RX.c : Mismatch in Specification Minor Version with CanIf.h"
#endif
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*************************************************************************/
/*
 * Brief               Construct Flow Control frame according to the parameter.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 *                     FsValue FC FS value
 *                     BSValue BS value for FC
 *                     STminValue STMin for FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              the length of constructed FC
 * PreCondition        Module initialized, a FC transmitting needed.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(uint8, CANTP_CODE)
CanTp_ConstructFCPci(
    uint8 ChannelIdx,
    uint8 FsValue,
    uint8 BSValue,
    uint8 STminValue);

/*************************************************************************/
/*
 * Brief               Calculate the BS value according to the buffer length and remain data length.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a FC transmitting needed to start a new block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_CalcBS(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Start a block by sending FC_CTS if needed.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to start a new block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_RxBlockStart(uint8 ChannelIdx);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"

/*************************************************************************/
/*
 * Brief               Get RX-SDU configuration by received NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of the received NPDU
 *                     CanTpRxPduPtr pointer to the received NPDU data
 *                     SduFrameType pointer to store the frame type
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType
    )
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    uint8 rxNSduIdx;
    CanTp_FramePCIType frameType;

    for (rxNSduIdx = 0; (NULL_PTR == result) && (rxNSduIdx < CANTP_RXNSDU_NUMBER); rxNSduIdx++)
    {
		#if (CANTP_PADDING_NEEDED == STD_ON)
        /*check CAN frame DLC first according to the padding information*/
        if ((FALSE == CanTp_CfgData.RxNSdus[rxNSduIdx].RxPaddingActivation)
            || (CanTpRxPduPtr->SduLength == CANTP_CAN_FRAME_LEN_MAX))
        {
		#endif

			#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            if (CANTP_STANDARD == CanTp_CfgData.RxNSdus[rxNSduIdx].AddressingFormat)
            {
			#endif
                /*check this RX-SDU, which is configured as standard addressing format*/
                /*check received frame type, only CF SF FF accepted*/
                frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK;
                if ((CanTp_CfgData.RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
                    && ((CANTP_FTYPE_CF == frameType)
                        || (CANTP_FTYPE_FF == frameType)
                        || (CANTP_FTYPE_SF == frameType)))
                {
                    result = &CanTp_CfgData.RxNSdus[rxNSduIdx];
                    *SduFrameType = frameType;
                }
			#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            } /*END OF Standard checking*/
            else if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_EX)
            {
                /*check this RX-SDU, which is configured as extended addressing format, and the CAN data at least includes
                 TA and PCIType*/
                /*check its TA and frame type*/
                frameType  = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX] & CANTP_FTYPE_MASK;

                if ((CanTp_CfgData.RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
                    && (CanTp_CfgData.RxNSdus[rxNSduIdx].Ta == CanTpRxPduPtr->SduDataPtr[CANTP_TA_OFFSET_EX])
                    && ((CANTP_FTYPE_CF == frameType)
                        || (CANTP_FTYPE_FF == frameType)
                        || (CANTP_FTYPE_SF == frameType)))
                {
                    result = &CanTp_CfgData.RxNSdus[rxNSduIdx];
                    *SduFrameType = frameType;
                }
            } /*END OF extended checking*/
            else
            {
                /*configured as extended addressing format, but the received NPDU not has enough data space*/
            }
			#endif
						
		#if (CANTP_PADDING_NEEDED == STD_ON)
        } /*END OF padding checking*/
		#endif
    } /*END OF for loop*/
    return result;
}

/*************************************************************************/
/*
 * Brief               Get Rx-SDU configuration by Tx-FC NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpTxPduId ID of the NPDU ID of TX FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    uint8 rxNSduIdx;

    for (rxNSduIdx = 0; (NULL_PTR == result) && (rxNSduIdx < CANTP_RXNSDU_NUMBER); rxNSduIdx++)
    {
        /*only physical Rx NSDU need configure TxFCNPduId*/
        if ((CANTP_PHYSICAL == CanTp_CfgData.RxNSdus[rxNSduIdx].RxTaType)
            && (CanTp_CfgData.RxNSdus[rxNSduIdx].TxFcNPduId == CanTpTxPduId))
        {
            result = &CanTp_CfgData.RxNSdus[rxNSduIdx];
        }
    }
    return result;

}

/*************************************************************************/
/*
 * Brief               Handle received CF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for CF,
 *                     and CF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleCFReception(uint8 ChannelIdx)
{
    uint8 cfSN;
    PduIdType nSduId;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    PduLengthType dataSize;
	boolean finished = FALSE;

    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId);

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat)
    {
	#endif
        /*standard SDU*/
        cfSN = CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_STD] & CANTP_CF_SN_MASK;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_CF_DATA_OFFSET_STD;
        CanTp_Channels[ChannelIdx].LocalBufLen--;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*extended SDU*/
        cfSN = CanTp_Channels[ChannelIdx].LocalBuf[CANTP_PCI_OFFSET_EX] & CANTP_CF_SN_MASK;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_CF_DATA_OFFSET_EX;
        CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - CANTP_CF_DATA_OFFSET_EX;
    }
	#endif

    /*check SN value*/
    if (cfSN != CanTp_Channels[ChannelIdx].CurrentCfSn)
    {
        /*wrong SN*/
        /*release resource of channel*/
        CanTp_InitChannel(ChannelIdx);
        PduR_CanTpRxIndication(nSduId, NTFRSLT_E_WRONG_SN);
    }
    else
    {
        /*It is the expected CF*/
        CanTp_Channels[ChannelIdx].HandleType = CANTP_FTYPE_CF;
        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
        CanTp_Channels[ChannelIdx].HandledCfCount++;

        /*copy data*/
        dataSize = CanTp_Channels[ChannelIdx].LocalBufLen;
        if (dataSize > CanTp_Channels[ChannelIdx].PduRBufRemaining)
        {
            dataSize = CanTp_Channels[ChannelIdx].PduRBufRemaining;
        }
				
		if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
        {
            dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
        }
				
        CanTp_MemoryCopy(
            &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
            &CanTp_Channels[ChannelIdx].PduRBufContentPointer[CanTp_Channels[ChannelIdx].PduRBufPosition],
            dataSize);
        CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - dataSize;
        CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
        CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;
        CanTp_Channels[ChannelIdx].PduRBufPosition = CanTp_Channels[ChannelIdx].PduRBufPosition + dataSize;
        CanTp_Channels[ChannelIdx].PduRBufRemaining = CanTp_Channels[ChannelIdx].PduRBufRemaining - dataSize;

        if (0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
        {
            /*the whole SDU reception finished*/
            /*release resource of channel*/
            CanTp_InitChannel(ChannelIdx);
            PduR_CanTpRxIndication(nSduId, NTFRSLT_OK);
			finished = TRUE;
        }
        else /*SDU whole reception not finished*/
        {
            if (0 != CanTp_Channels[ChannelIdx].LocalBufLen)
            {
                /*the provide RX buffer is not enough to store the frame data, so need to get more buffer*/
                CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_START;
                CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
                CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxNSduCfgPtr->Nbr;
                (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
            }
            else
            {
                /*current frame reception finished*/
                if (CanTp_Channels[ChannelIdx].CurrentBs == CanTp_Channels[ChannelIdx].HandledCfCount)
                {
                    /*current block finished, start a new block*/
                    CanTp_RxBlockStart(ChannelIdx);
                }
                else
                {
                    /*continue to wait another CF*/
                    CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4CF;
                    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
                    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxNSduCfgPtr->Ncr;
                    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
                }
                /*update SN*/
                CanTp_Channels[ChannelIdx].CurrentCfSn = (uint8)(CanTp_Channels[ChannelIdx].CurrentCfSn + 1) & CANTP_CF_SN_MASK;
            }
        } /*END OF SDU whole reception not finished*/
    } /*END OF handling expected CF*/

    /*clear the event*/
	if (FALSE == finished)
	{
        CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_CF_RECEIVED); 
    }
}

/*************************************************************************/
/*
 * Brief               Handle received FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is allowed to receive FF,
 *                     and FF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleFFReception(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    PduLengthType ffDl;

    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].RcvdNSduId);

    if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RootState)
        || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RootState))
    {
        /*Another SF received when the channel is receiving a SDU*/
        /*Notify error for old SDU reception*/
        PduR_CanTpRxIndication(CanTp_Channels[ChannelIdx].NSduId, NTFRSLT_E_NOT_OK);
    }

    ffDl = CanTp_RxGetFFDl(CanTp_Channels[ChannelIdx].LocalBuf, rxNSduCfgPtr);

    /*update channel runtime information*/
    CanTp_Channels[ChannelIdx].RootState = CANTP_LARGE_RECEIVING;
    CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_START;
    CanTp_Channels[ChannelIdx].NSduId = CanTp_Channels[ChannelIdx].RcvdNSduId;
    CanTp_Channels[ChannelIdx].RcvdNSduId = 0;
    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxNSduCfgPtr->Nbr;
    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
    CanTp_Channels[ChannelIdx].HandleType = CANTP_FTYPE_FF;

    /*initialize SDU information*/
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat)
    {
	#endif
        /*Data start from the third byte*/
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_FF_DATA_OFFSET_STD;
        CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - CANTP_FF_DATA_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*Data start from the fourth byte*/
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_FF_DATA_OFFSET_EX;
        CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - CANTP_FF_DATA_OFFSET_EX;
    }
	#endif

    CanTp_Channels[ChannelIdx].SduDataRemaining = ffDl;
    CanTp_Channels[ChannelIdx].SduDataTotalCount = ffDl;

    /*initialize buffer information*/
    CanTp_Channels[ChannelIdx].PduRBufContentPointer = NULL_PTR;
    CanTp_Channels[ChannelIdx].PduRBufPosition = 0;
    CanTp_Channels[ChannelIdx].PduRBufRemaining = 0;

    /*initialize Flow control information*/
    CanTp_Channels[ChannelIdx].CurrentBs = 0;
    CanTp_Channels[ChannelIdx].CurrentCfSn = 0;
    CanTp_Channels[ChannelIdx].HandledCfCount = 0;
    CanTp_Channels[ChannelIdx].SentWftCount = 0;

    /*clear the event*/
    CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_FF_RECEIVED);
}

/*************************************************************************/
/*
 * Brief               Handle received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is allowed to receive SF,
 *                     and SF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleSFReception(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr = NULL_PTR;
    PduLengthType sfDl;

    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].RcvdNSduId);

    if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RootState)
        || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RootState))
    {
        /*Another SF received when the channel is receiving a SDU*/
        /*Notify error for old SDU reception*/
        PduR_CanTpRxIndication(CanTp_Channels[ChannelIdx].NSduId, NTFRSLT_E_NOT_OK);
    }

    sfDl = CanTp_RxGetSFDl(CanTp_Channels[ChannelIdx].LocalBuf, rxNSduCfgPtr);

    /*update channel runtime information*/
    CanTp_Channels[ChannelIdx].RootState = CANTP_SF_RECEIVING;
    CanTp_Channels[ChannelIdx].Substate = CANTP_RX_SF_START;
    CanTp_Channels[ChannelIdx].NSduId = CanTp_Channels[ChannelIdx].RcvdNSduId;
    CanTp_Channels[ChannelIdx].RcvdNSduId = 0;
    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxNSduCfgPtr->Nbr;
    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
    CanTp_Channels[ChannelIdx].HandleType = CANTP_FTYPE_SF;

    /*initialize SDU information*/
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat)
    {
	#endif
        /*Data start from the second byte*/
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_SF_DATA_OFFSET_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*Data start from the third byte*/
        CanTp_Channels[ChannelIdx].LocalBufPosition = CANTP_SF_DATA_OFFSET_EX;
    }
	#endif

    CanTp_Channels[ChannelIdx].LocalBufLen = sfDl;
    CanTp_Channels[ChannelIdx].SduDataRemaining = sfDl;
    CanTp_Channels[ChannelIdx].SduDataTotalCount = sfDl;

    /*initialize buffer information*/
    CanTp_Channels[ChannelIdx].PduRBufContentPointer = NULL_PTR;
    CanTp_Channels[ChannelIdx].PduRBufPosition = 0;
    CanTp_Channels[ChannelIdx].PduRBufRemaining = 0;

    /*clear the event*/
    CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_SF_RECEIVED);
}

/*************************************************************************/
/*
 * Brief               Handle confirmation for FC.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for the confirmation
 *                     of FC frame, and a FC frame confirmed.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleFCCfm(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].NSduId);

    if (CANTP_RX_LARGE_V4FCWT_CFM == CanTp_Channels[ChannelIdx].Substate)
    {
        /*transit to RX_LARGE_START, which will try to get buffer again*/
        CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_START;
        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NB;
        CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxSduCfgPtr->Nbr;
        (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
    }
    else if (CANTP_RX_LARGE_V4FCCTS_CFM == CanTp_Channels[ChannelIdx].Substate)
    {
        /*FC_CTS sent, start to waiting for CFs*/
        CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4CF;
        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
        CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxSduCfgPtr->Ncr;
        (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
    }
    else
    {
        /*will not happen with handling of filtering when indicated by CanIf*/
    }

    /*clear the event*/
    CanTp_Channels[ChannelIdx].EventFlags = CanTp_Channels[ChannelIdx].EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_TXFC_CONFIRMED);
}

/*************************************************************************/
/*
 * Brief               Back ground handling for receiving SF,
 *                     in this state the channel is trying to get buffer to store
 *                     received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get buffer to store
 *                     received SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleSFStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    BufReq_ReturnType bufRslt = BUFREQ_E_NOT_OK;
    PduLengthType dataSize;
    PduIdType nSduId;
    
    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = CanTp_Channels[ChannelIdx].SduDataTotalCount;
    bufRslt = PduR_CanTpProvideRxBuffer(CanTp_Channels[ChannelIdx].NSduId, pduInfo.SduLength, &pduInfoPtr);

    /*handle according the result of request RX buffer*/
    switch (bufRslt)
    {
        case BUFREQ_OK:
        {
            CanTp_Channels[ChannelIdx].PduRBufContentPointer = pduInfoPtr->SduDataPtr;
            CanTp_Channels[ChannelIdx].PduRBufPosition = 0;
            CanTp_Channels[ChannelIdx].PduRBufRemaining = pduInfoPtr->SduLength;
            dataSize = CanTp_Channels[ChannelIdx].LocalBufLen;
            /*use the minimum length for data copying, among the two buffers and the remain SDU data*/
            if (dataSize > pduInfoPtr->SduLength)
            {
                dataSize = pduInfoPtr->SduLength;
            }

            if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
            }

            /*copy data*/
            CanTp_MemoryCopy(
                &CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                CanTp_Channels[ChannelIdx].PduRBufContentPointer,
                dataSize);
            /*update local buffer length information*/
            CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;

            if (0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                /*data copy finished*/

                /*Release the resource of the channel*/
                CanTp_InitChannel(ChannelIdx);

                /*Notify the upper*/
                PduR_CanTpRxIndication(nSduId, NTFRSLT_OK);
            }
            else
            {
                /*update channel information, and ready for data copy at next round*/
                CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - dataSize;
                CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
            }

            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK:
            /*Handle the same with the two values:BUFREQ_E_NOT_OK,BUFREQ_E_OVFL*/
        case BUFREQ_E_OVFL:
        {
            /*Release the resource of the channel*/
            CanTp_InitChannel(ChannelIdx);

            /*Notify the upper for error*/
            PduR_CanTpRxIndication(nSduId, NTFRSLT_E_NO_BUFFER);
            break;
        }
        case BUFREQ_E_BUSY:
            /*Upper is busy, request RX buffer till next round*/
            break;
        default:
            /*invalid return value*/
            break;
    } /*END OF switch (bufRslt)*/
}

/*************************************************************************/
/*
 * Brief               Back ground handling for large SDU receiving, trying to get RX buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get buffer to store
 *                     received FF or CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_RxHandleLargeStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    BufReq_ReturnType bufRslt = BUFREQ_E_NOT_OK;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    PduLengthType dataSize;
    PduIdType nSduId;

    nSduId = CanTp_Channels[ChannelIdx].NSduId;
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId);
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = CanTp_Channels[ChannelIdx].SduDataTotalCount;
    bufRslt = PduR_CanTpProvideRxBuffer(CanTp_Channels[ChannelIdx].NSduId, pduInfo.SduLength, &pduInfoPtr);

    /*handle according to the result of requesting buffer*/
    switch(bufRslt)
    {
        case BUFREQ_OK:
        {
            /*get buffer successful, copy to store the received data*/
            CanTp_Channels[ChannelIdx].PduRBufContentPointer = pduInfoPtr->SduDataPtr;
            CanTp_Channels[ChannelIdx].PduRBufPosition = 0;
            CanTp_Channels[ChannelIdx].PduRBufRemaining = pduInfoPtr->SduLength;
            dataSize = CanTp_Channels[ChannelIdx].LocalBufLen;
            /*use the minimum length for data copying, among the two buffers and the remain SDU data*/
            if (dataSize > pduInfoPtr->SduLength)
            {
                dataSize = pduInfoPtr->SduLength;
            }
            if (dataSize > CanTp_Channels[ChannelIdx].SduDataRemaining)
            {
                dataSize = CanTp_Channels[ChannelIdx].SduDataRemaining;
            }

            /*copy data*/
            CanTp_MemoryCopy(	&CanTp_Channels[ChannelIdx].LocalBuf[CanTp_Channels[ChannelIdx].LocalBufPosition],
                				CanTp_Channels[ChannelIdx].PduRBufContentPointer,
                				dataSize);

            /*update local buffer and SDU information in channel*/
            CanTp_Channels[ChannelIdx].LocalBufLen = CanTp_Channels[ChannelIdx].LocalBufLen - dataSize;
            CanTp_Channels[ChannelIdx].LocalBufPosition = CanTp_Channels[ChannelIdx].LocalBufPosition + dataSize;
            CanTp_Channels[ChannelIdx].SduDataRemaining = CanTp_Channels[ChannelIdx].SduDataRemaining - dataSize;
            if ((0 == CanTp_Channels[ChannelIdx].LocalBufLen)
                || (0 == CanTp_Channels[ChannelIdx].SduDataRemaining))
            {
                /*current frame reception finished*/
                if (0 == CanTp_Channels[ChannelIdx].SduDataRemaining)
                {
                    /*the whole SDU reception finished*/
                    /*release resource of the channel*/
                    CanTp_InitChannel(ChannelIdx);
                    PduR_CanTpRxIndication(nSduId, NTFRSLT_OK);
                }
                else
                {
                    /*FF reception finished or current BS reception finished, send FC_CTS to start a block*/
                    CanTp_Channels[ChannelIdx].PduRBufRemaining = CanTp_Channels[ChannelIdx].PduRBufRemaining - dataSize;
                    CanTp_Channels[ChannelIdx].PduRBufPosition = dataSize;
                    if ((0 != rxSduCfgPtr->Bs)
                        || (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType))
                    {
                        CanTp_RxBlockStart(ChannelIdx);
                    }
                    else
                    {
                        /*the whole SDU will be send in one single block, continue to wait another CF*/
                        CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4CF;
                        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NC;
                        CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxSduCfgPtr->Ncr;
                        (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
                    }

                    /*update expected next SN*/
                    CanTp_Channels[ChannelIdx].CurrentCfSn = (uint8)(CanTp_Channels[ChannelIdx].CurrentCfSn + 1) & CANTP_CF_SN_MASK;
                }
            } /*END OF current frame reception finished*/
            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK:
            /*Handle the same with the two values:BUFREQ_E_NOT_OK,BUFREQ_E_OVFL*/
        case BUFREQ_E_OVFL:
        {
            /*when return value is NOT_OK or OVFL, handle this result only when handling FF.*/
            if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType)
            {
                /*when handling FF, send FC_OVFL*/
                pduInfo.SduLength = CanTp_ConstructFCPci(ChannelIdx, CANTP_FC_FS_OVFLW, 0, 0);
                pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].FcBuf;
                (void)CanIf_Transmit(rxSduCfgPtr->TxFcNPduId, pduInfoPtr);

                /*release resource of the channel*/
                CanTp_InitChannel(ChannelIdx);
                PduR_CanTpRxIndication(nSduId, NTFRSLT_E_NO_BUFFER);
            }
            break;
        }
        case BUFREQ_E_BUSY:
        {
            /*The upper is busy, send FC_WAIT, if needed.*/
            /*If WFTMax is zero for RX SDU, no FC_WAIT will be sent*/
            if (0 != rxSduCfgPtr->RxWftMax)
            {
                /*If the whole SDU will be send in more than one single block(I.E. BS is not zero) and it the end of current block
                 * but not the last block, or if currently it is handling First Frame, it is allowed to send FC_WAIT*/
                if ((CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].HandleType)
                    || ((CanTp_Channels[ChannelIdx].CurrentBs == CanTp_Channels[ChannelIdx].HandledCfCount)
                        && (CanTp_Channels[ChannelIdx].SduDataRemaining > CanTp_Channels[ChannelIdx].LocalBufLen)))
                {
                    /*Try to send FC_WAIT*/
                    if (rxSduCfgPtr->RxWftMax <= CanTp_Channels[ChannelIdx].SentWftCount)
                    {
                        /*Can not send any more FC_WAIT. Abort this reception*/
                        /*release resource of this channel*/
                        CanTp_InitChannel(ChannelIdx);
                        PduR_CanTpRxIndication(nSduId, NTFRSLT_E_WFT_OVRN);
                    }
                    else
                    {
                        /*Send FC_WAIT and then wait for the confirmation*/
                        pduInfo.SduLength = CanTp_ConstructFCPci(ChannelIdx, CANTP_FC_FS_WT, 0, 0);
                        pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].FcBuf;
                        
                        CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4FCWT_CFM;   /*chenms 2013-9-26 19:32*/
                        (void)CanIf_Transmit(rxSduCfgPtr->TxFcNPduId, pduInfoPtr);
                        CanTp_Channels[ChannelIdx].SentWftCount++;
                        /*transit to waiting for confirmation of FC_WAIT*/
                        CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4FCWT_CFM;
                        CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
                        CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxSduCfgPtr->Nar;
                        (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);
                    }
                } /*END OF try to send FC_WAIT*/
            }
            break;
        } /*ENDOF case BUFREQ_E_BUSY*/
        default:
            /*invalid result*/
            break;
    } /*END OF switch(bufRslt)*/
}

/*************************************************************************/
/*
 * Brief               Get SDU configuration by N-SDU ID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxSduId ID of the SDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId)
{
    uint8 rxNSduIdx;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;

    for (rxNSduIdx = 0; (NULL_PTR == result) && (rxNSduIdx < CANTP_RXNSDU_NUMBER); rxNSduIdx++)
    {
        if (CanTp_CfgData.RxNSdus[rxNSduIdx].RxNSduId == CanTpRxSduId)
        {
            result = &CanTp_CfgData.RxNSdus[rxNSduIdx];
        }
    }
    return result;
}

/*************************************************************************/
/*
 * Brief               Get FF_DL information from the data content of FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduPtr pointer to data
 *                     RxNSduCfgPtr pointer to configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              FF_DL
 * PreCondition        Module initialized, the channel received a FF.
 * CallByAPI           CanTp_RxIndication and background handling
 */
/*************************************************************************/
FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
{
    uint16 ffDl = 0;

    /*check FF DL*/
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
    {
	#endif
        /*PCI starts at byte[0]*/
        ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_STD] & CANTP_SF_DL_MASK;
        ffDl <<= CANTP_OFFSET_FFDL_HIGNT_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 1];
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*PCI starts at byte[1]*/
        ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_EX] & CANTP_SF_DL_MASK;
        ffDl <<= CANTP_OFFSET_FFDL_HIGNT_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 1];
    }
	#endif

    return ffDl;
}

/*************************************************************************/
/*
 * Brief               Get SFF_DL information from the data content of SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduPtr pointer to data
 *                     RxNSduCfgPtr pointer to configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              FF_DL
 * PreCondition        Module initialized, the channel received a SF.
 * CallByAPI           CanTp_RxIndication and background handling
 */
/*************************************************************************/
FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetSFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
{
    uint8 sfDl;

    /*check SF DL*/
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
    {
	#endif
        /*PCI is byte[0]*/
        sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_STD] & CANTP_SF_DL_MASK;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*PCI is byte[1]*/
        sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_EX] & CANTP_SF_DL_MASK;
    }
	#endif

    return sfDl;
}

/*============Internal Static Function Implementations========================*/
/*************************************************************************/
/*
 * Brief               Construct Flow Control frame according to the parameter.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 *                     FsValue FC FS value
 *                     BSValue BS value for FC
 *                     STminValue STMin for FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              the length of constructed FC
 * PreCondition        Module initialized, a FC transmitting needed.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(uint8, CANTP_CODE)
CanTp_ConstructFCPci(
    uint8 ChannelIdx,
    uint8 FsValue,
    uint8 BSValue,
    uint8 STminValue)
{
    #if ((CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_PADDING_NEEDED == STD_ON))
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    #endif
    uint8 fcDataLen;

    #if ((CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_PADDING_NEEDED == STD_ON))
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].NSduId);
    #endif

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    if (CANTP_STANDARD == rxSduCfgPtr->AddressingFormat)
    {
	#endif
        /*STANDARD SDU*/
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_PCI_OFFSET_STD] = CANTP_FTYPE_FC | FsValue;
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_FC_OFFSET_BS_STD] = BSValue;
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_FC_OFFSET_STMIN_STD] = STminValue;
        fcDataLen = CANTP_FC_LEN_STD;
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    }
    else
    {
        /*EXTENDED SDU*/
        /*Set TA in FC frame. The TA of FC is the SA of the SDU*/
        CanTp_Channels[ChannelIdx].FcBuf[0] = rxSduCfgPtr->Sa;
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_FC | FsValue;
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_FC_OFFSET_BS_EX] = BSValue;
        CanTp_Channels[ChannelIdx].FcBuf[CANTP_FC_OFFSET_STMIN_EX] = STminValue;
        fcDataLen = CANTP_FC_LEN_EX;
    }
	#endif

    #if (CANTP_PADDING_NEEDED == STD_ON)
    /*Padding if needed*/
    if (TRUE == rxSduCfgPtr->RxPaddingActivation)
    {
        CanTp_MemorySet(
            &CanTp_Channels[ChannelIdx].FcBuf[fcDataLen],
            CANTP_PADDING_BYTE,
            CANTP_CAN_FRAME_LEN_MAX - fcDataLen);
        fcDataLen = CANTP_CAN_FRAME_LEN_MAX;
    }
	#endif
    /*return the FC frame length*/
    return fcDataLen;
}

/*************************************************************************/
/*
 * Brief               Calculate the BS value according to the buffer length and remain data length.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a FC transmitting needed to start a new block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_CalcBS(uint8 ChannelIdx)
{
    PduLengthType blockDataLen;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].NSduId);

    if(0 == rxSduCfgPtr->Bs)
    {
        /*BS is 0, no more FC need from now on*/
        CanTp_Channels[ChannelIdx].CurrentBs = 0;
    }
    else
    {
        /*get the data length of this block using the minimum value between PDUR buffer length and
         * SDU remain data length. If the minimum value is zero, I.E. the PDUR remain buffer is 0, use 1
         * as the BS*/
        blockDataLen = CanTp_Channels[ChannelIdx].SduDataRemaining;
        if (blockDataLen > CanTp_Channels[ChannelIdx].PduRBufRemaining)
        {
            blockDataLen = CanTp_Channels[ChannelIdx].PduRBufRemaining;
        }

        if (0 == blockDataLen)
        {
            /*no more upper buffer space left */
            CanTp_Channels[ChannelIdx].CurrentBs = 1U;
        }
        else
        {
			#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            if (CANTP_STANDARD == rxSduCfgPtr->AddressingFormat)
            {
			#endif
                /*Standard SDU*/
                CanTp_Channels[ChannelIdx].CurrentBs = (uint8)((blockDataLen + CANTP_CF_DATALEN_STD - 1) / CANTP_CF_DATALEN_STD);
			#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            }
            else
            {
                /*extended SDU*/
                CanTp_Channels[ChannelIdx].CurrentBs = (uint8)((blockDataLen + CANTP_CF_DATALEN_EX - 1) / CANTP_CF_DATALEN_EX);
            }
			#endif

            /*use the minimum value between calculated and the configured maximum value*/
            if (CanTp_Channels[ChannelIdx].CurrentBs > rxSduCfgPtr->Bs)
            {
                CanTp_Channels[ChannelIdx].CurrentBs = rxSduCfgPtr->Bs;
            }
        }
    } /*END OF BS needed*/
}

/*************************************************************************/
/*
 * Brief               Start a block by sending FC_CTS if needed.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, need to start a new block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_RxBlockStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTp_Channels[ChannelIdx].NSduId);

    /*send FC_CTS*/
    /*calculate the BS value*/
    CanTp_CalcBS(ChannelIdx);
    pduInfo.SduLength = CanTp_ConstructFCPci(
        ChannelIdx, CANTP_FC_FS_CTS,
        CanTp_Channels[ChannelIdx].CurrentBs,
        rxSduCfgPtr->STmin);
    pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].FcBuf;
      
      
    CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4FCCTS_CFM;  /*chenms 2013-9-26 19:32*/ 
    (void)CanIf_Transmit(rxSduCfgPtr->TxFcNPduId,  &pduInfo);
    /*transit to waiting for confirmation of FC_CTS*/
    CanTp_Channels[ChannelIdx].Substate = CANTP_RX_LARGE_V4FCCTS_CFM;
    CanTp_Channels[ChannelIdx].ChannelTimer.EnabledTimer = CANTP_NA;
    CanTp_Channels[ChannelIdx].ChannelTimer.RemainTime = rxSduCfgPtr->Nar;
    (void)GetCounterValue(CANTP_OS_COUNTER_ID, &CanTp_Channels[ChannelIdx].ChannelTimer.StartTime);

    CanTp_Channels[ChannelIdx].SentWftCount = 0;
    CanTp_Channels[ChannelIdx].HandledCfCount = 0;
}
#define CANTP_STOP_SEC_CODE
#include "MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
