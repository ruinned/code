/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_Cbk.c
 *  @brief      Call-back function implementation
 *  
 *
 *  @author     stanley
 *  @date       2013-4-7 
 */
/*============================================================================*/


/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2013-4-7   stanley    create
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_CBK_C_AR_MAJOR_VERSION  2
#define CANTP_CBK_C_AR_MINOR_VERSION  3
#define CANTP_CBK_C_AR_PATCH_VERSION  0
#define CANTP_CBK_C_SW_MAJOR_VERSION  1
#define CANTP_CBK_C_SW_MINOR_VERSION  0
#define CANTP_CBK_C_SW_PATCH_VERSION  0

/*required CANIF version*/
#define CANTP_CBK_C_CANIF_AR_MAJOR_VERSION  3
#define CANTP_CBK_C_CANIF_AR_MINOR_VERSION  2

/*required PDUR version*/
#define CANTP_CBK_C_PDUR_AR_MAJOR_VERSION  2
#define CANTP_CBK_C_PDUR_AR_MINOR_VERSION  3

/*required DEM version*/
#define CANTP_CBK_C_DEM_AR_MAJOR_VERSION  3
#define CANTP_CBK_C_DEM_AR_MINOR_VERSION  1

/*required DET version*/
#define CANTP_CBK_C_DET_AR_MAJOR_VERSION  2
#define CANTP_CBK_C_DET_AR_MINOR_VERSION  2

/*=======[I N C L U D E S]====================================================*/
#include "CanTpDiag.h"
#include "CanTp_Cbk.h"
#include "CanTp_Internal.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/*check version information with CanTp.h*/
#if (CANTP_CBK_C_AR_MAJOR_VERSION != CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanTp.h"
#endif
#if (CANTP_CBK_C_AR_MINOR_VERSION != CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanTp.h"
#endif
#if (CANTP_CBK_C_AR_PATCH_VERSION != CANTP_H_AR_PATCH_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Patch Version with CanTp.h"
#endif
#if (CANTP_CBK_C_SW_MAJOR_VERSION != CANTP_H_SW_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Major Version with CanTp.h"
#endif
#if (CANTP_CBK_C_SW_MINOR_VERSION != CANTP_H_SW_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Minor Version with CanTp.h"
#endif

/*check version information with CanTp_Cbk.h*/
#if (CANTP_CBK_C_AR_MAJOR_VERSION != CANTP_CBK_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanTp_Cbk.h"
#endif
#if (CANTP_CBK_C_AR_MINOR_VERSION != CANTP_CBK_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanTp_Cbk.h"
#endif
#if (CANTP_CBK_C_AR_PATCH_VERSION != CANTP_CBK_H_AR_PATCH_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Patch Version with CanTp_Cbk.h"
#endif
#if (CANTP_CBK_C_SW_MAJOR_VERSION != CANTP_CBK_H_SW_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Major Version with CanTp_Cbk.h"
#endif
#if (CANTP_CBK_C_SW_MINOR_VERSION != CANTP_CBK_H_SW_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Minor Version with CanTp_Cbk.h"
#endif

/*check version information with CanTp_Internal.h*/
#if (CANTP_CBK_C_AR_MAJOR_VERSION != CANTP_INTERNAL_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanTp_Internal.h"
#endif
#if (CANTP_CBK_C_AR_MINOR_VERSION != CANTP_INTERNAL_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanTp_Internal.h"
#endif
#if (CANTP_CBK_C_AR_PATCH_VERSION != CANTP_INTERNAL_H_AR_PATCH_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Patch Version with CanTp_Internal.h"
#endif
#if (CANTP_CBK_C_SW_MAJOR_VERSION != CANTP_INTERNAL_H_SW_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Major Version with CanTp_Internal.h"
#endif
#if (CANTP_CBK_C_SW_MINOR_VERSION != CANTP_INTERNAL_H_SW_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Minor Version with CanTp_Internal.h"
#endif

/*check version information with CanTp_Types.h*/
#if (CANTP_CBK_C_AR_MAJOR_VERSION != CANTP_TYPES_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanTp_Types.h"
#endif
#if (CANTP_CBK_C_AR_MINOR_VERSION != CANTP_TYPES_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanTp_Types.h"
#endif
#if (CANTP_CBK_C_AR_PATCH_VERSION != CANTP_TYPES_H_AR_PATCH_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Patch Version with CanTp_Types.h"
#endif
#if (CANTP_CBK_C_SW_MAJOR_VERSION != CANTP_TYPES_H_SW_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
#if (CANTP_CBK_C_SW_MINOR_VERSION != CANTP_TYPES_H_SW_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Minor Version with CanTp_Types.h"
#endif

/*check version information with CanTp_Cfg.h*/
#if (CANTP_CBK_C_AR_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_CBK_C_AR_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanTp_Cfg.h"
#endif
#if (CANTP_CBK_C_AR_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Patch Version with CanTp_Cfg.h"
#endif
#if (CANTP_CBK_C_SW_MAJOR_VERSION != CANTP_CFG_H_SW_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_CBK_C_SW_MINOR_VERSION != CANTP_CFG_H_SW_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Software Implementation Minor Version with CanTp_Cfg.h"
#endif

#if 0
/*check version information with DEM*/
#if (CANTP_CBK_C_DEM_AR_MAJOR_VERSION != DEM_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with Dem.h"
#endif
#if (CANTP_CBK_C_DEM_AR_MINOR_VERSION != DEM_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with Dem.h"
#endif
#endif

#if (CANTP_DEV_ERROR_DETECT == STD_ON)
/*Check version information with DET*/
#if (CANTP_CBK_C_DET_AR_MAJOR_VERSION != DET_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with Det.h"
#endif
#if (CANTP_CBK_C_DET_AR_MINOR_VERSION != DET_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with Det.h"
#endif
#endif

/*Check version information with PDUR*/
#if (CANTP_CBK_C_PDUR_AR_MAJOR_VERSION != PDUR_CANTP_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with PduR_CanTp.h"
#endif
#if (CANTP_CBK_C_PDUR_AR_MINOR_VERSION != PDUR_CANTP_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with PduR_CanTp.h"
#endif

/*Check version information with CANIF*/
#if (CANTP_CBK_C_CANIF_AR_MAJOR_VERSION != CANIF_H_AR_MAJOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Major Version with CanIf.h"
#endif
#if (CANTP_CBK_C_CANIF_AR_MINOR_VERSION != CANIF_H_AR_MINOR_VERSION)
  #error "CanTp_Cbk.c : Mismatch in Specification Minor Version with CanIf.h"
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
 * Brief               This function is called by the CanTp_RxIndication after a successful
 *                     reception of a RX CAN L-PDU, put the PDU into pool.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId the received N-PDU ID
 *                     CanTpRxPduPtr indicator of structure with received
 *                                   L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CanTp_RxIndication
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_PoolRxPdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr);

/*************************************************************************/
/*
 * Brief               Handle Confirm event occurred to the specific channel.
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
CanTp_DispatchCfmEvent(uint8 ChannelIdx);

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               This function is called by the CAN Interface after a successful
 *                     reception of a RX CAN L-PDU.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId the received N-PDU ID
 *                     CanTpRxPduPtr indicator of structure with received
 *                                   L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface Receiving handling
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE)
CanTp_RxIndication(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr
    )
{
    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        if ((NULL_PTR == CanTpRxPduPtr)
            || (NULL_PTR == CanTpRxPduPtr->SduDataPtr))
        {
            /*INVALID pointer parameter*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_ADDRESS);
        }
        else if (0 == CanTpRxPduPtr->SduLength)
        {
            /*invalid received NPDU data space*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_RX_LENGTH);
        }
        else
        { /*pointer not null*/
        #endif
            /*put the received NPDU into pool of the channel*/
            CanTp_PoolRxPdu(CanTpRxPduId, CanTpRxPduPtr);
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        } /*END OF pointer not null*/
        #endif

    } /*END OF CANTP_ON == CanTp_RuntimeControl.ModuleState*/
    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /*CanTp module not started*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_RXINDICATION, CANTP_E_UNINIT);
    }
    #endif
}

/*************************************************************************/
/*
 * Brief               All transmitted CAN frames belonging to the CAN Transport
 *                     Layer will be confirmed by this function.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxPduId ID of CAN L-PDU that has been transmitted
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface transmitting confirmation
 */
/*************************************************************************/
FUNC(void, CANTP_APPL_CODE)
CanTp_TxConfirmation(PduIdType CanTpTxPduId)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr = NULL_PTR;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr = NULL_PTR;
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;
    volatile boolean eventConsumed = FALSE;
    uint8 channelIdx  = 0;

    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        /*first check if it is a transmitted data NPDU*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxNPdu(CanTpTxPduId);

        if (NULL_PTR != txNSduCfgPtr)
        {
            /*A transmitted data PDU confirmed*/
            /*Store the confirmation event only when the channel related with this TX SDU
             * is transmitting this SDU and it is waiting for the confirmation*/
            channelPtr = &CanTp_Channels[txNSduCfgPtr->TxChannel];

            if (((CANTP_TX_SF_V4SF_CFM == channelPtr->Substate)
                    || (CANTP_TX_LARGE_V4DATA_CFM == channelPtr->Substate))
                && (txNSduCfgPtr->TxNSduId == channelPtr->NSduId))
            {
                channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_TX_CONFIRMED;
                channelIdx = txNSduCfgPtr->TxChannel;
                eventConsumed = TRUE;
            }
        } /*END OF handling data frame confirmation*/

        if (FALSE == eventConsumed)
        {
            /*then check if it is a transmitted FC NPDU*/
            rxNSduCfgPtr = CanTp_GetRxSduCfgByTxFCNPdu(CanTpTxPduId);
            if (NULL_PTR != rxNSduCfgPtr)
            {
                /*A transmitted FC PDU confirmed*/
                /*Store the confirmation event only when the channel related with this RX SDU
                 * is receiving this SDU and it is waiting for the FC confirmation*/
            	channelPtr = &CanTp_Channels[rxNSduCfgPtr->RxChannel];

                if (((CANTP_RX_LARGE_V4FCCTS_CFM == channelPtr->Substate)
                        || (CANTP_RX_LARGE_V4FCWT_CFM == channelPtr->Substate))
                    && (rxNSduCfgPtr->RxNSduId == channelPtr->NSduId))
                {
                    channelPtr->EventFlags |= CANTP_CHANNEL_EVENT_TXFC_CONFIRMED;
                    channelIdx = rxNSduCfgPtr->RxChannel;
                    eventConsumed = TRUE;
                }
            }
        } /*END OF FC confirmation*/

        if (TRUE == eventConsumed)
        {
        	/*handle confirmation events*/
        	CanTp_DispatchCfmEvent(channelIdx);
        }
        #if (CANTP_DEV_ERROR_DETECT == STD_ON)
        else
        {
            /*invalid confirmation*/
            Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
        }
        #endif
    } /*END OF CANTP_ON == CanTp_RuntimeControl.ModuleState*/
    #if (CANTP_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /*CanTp module not started*/
        Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_UNINIT);
    }
    #endif
}

/*========[Internal STATIC Function Implementation]==================================*/
/*************************************************************************/
/*
 * Brief               This function is called by the CanTp_RxIndication after a successful
 *                     reception of a RX CAN L-PDU, put the PDU into pool.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId the received N-PDU ID
 *                     CanTpRxPduPtr indicator of structure with received
 *                                   L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CanTp_RxIndication
 */
/*************************************************************************/
STATIC FUNC(void, CANTP_CODE)
CanTp_PoolRxPdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr = NULL_PTR;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr = NULL_PTR;
    CanTp_FramePCIType sduFrameType;
    volatile boolean pduConsumed = FALSE;
    
   /****************************************/        
    /*chenms 2014-6-21*/
 #if(STD_ON == CANTP_PADDING_NEEDED)
    uint8  PciInfo = 0xFF;
    uint8  PaddingOffset = 0xFF;
    uint8  Running = TRUE; 
     
    #if(STD_ON == CANTP_EXTENDED_ADDRESSING_SUPPORT)
    PciInfo = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX] & CANTP_FTYPE_MASK;  /*extended Addressing*/
    if(CANTP_FTYPE_SF == PciInfo)  
    {
    	PaddingOffset = CANTP_SF_DATA_OFFSET_EX + CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX];
    }
    if(CANTP_FTYPE_FC == PciInfo)
    {
    	PaddingOffset = 4U;
    }
    #else
    PciInfo = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK; /*normal Addressing*/
    if(CANTP_FTYPE_SF == PciInfo)  
    {
    	PaddingOffset = CANTP_SF_DATA_OFFSET_STD + CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD];
    }
    if(CANTP_FTYPE_FC == PciInfo)
    {
    	PaddingOffset = 3U;
    }
    #endif
    
    if((CANTP_FTYPE_SF == PciInfo) || (CANTP_FTYPE_FC == PciInfo))
    {
		for(;(PaddingOffset<CANTP_CAN_FRAME_LEN_MAX)&&(TRUE ==Running);PaddingOffset++)
		{
			if(CANTP_PADDING_BYTE != CanTpRxPduPtr->SduDataPtr[PaddingOffset])
			{
			    Running = FALSE;	
			}
		}
    }
    
    if(TRUE == Running)
    {
#endif  
/******************************************/
	    /*first try to check if the received NPDU whether is CF SF or FF*/
	    rxNSduCfgPtr = CanTp_GetRxSduCfgByNpdu(CanTpRxPduId, CanTpRxPduPtr, &sduFrameType);
		/********************************************/
	    if (NULL_PTR != rxNSduCfgPtr)
	    {
	        if ((FALSE == CanTp_ChnlPools[rxNSduCfgPtr->RxChannel].Filled)
	            || (((CANTP_LARGE_RECEIVING == CanTp_Channels[rxNSduCfgPtr->RxChannel].RootState)
	                    || (CANTP_SF_RECEIVING == CanTp_Channels[rxNSduCfgPtr->RxChannel].RootState))
	                && ((CANTP_FTYPE_FF == sduFrameType)
	                    || (CANTP_FTYPE_SF == sduFrameType))))
	        {
	            CanTp_ChnlPools[rxNSduCfgPtr->RxChannel].PduInfo.SduLength = CanTpRxPduPtr->SduLength;
	            CanTp_MemoryCopy(	CanTpRxPduPtr->SduDataPtr,
	                				CanTp_ChnlPools[rxNSduCfgPtr->RxChannel].PduInfo.SduDataPtr,
	                				CanTpRxPduPtr->SduLength);
	            CanTp_ChnlPools[rxNSduCfgPtr->RxChannel].RxNPduId = CanTpRxPduId;
	            CanTp_ChnlPools[rxNSduCfgPtr->RxChannel].Filled = TRUE;

	            pduConsumed = TRUE;
	        }
	    }

	    if (FALSE == pduConsumed)
	    {
	        /*then check whether it is FC*/
	        txNSduCfgPtr = CanTp_GetTxSduCfgByFCNpdu(CanTpRxPduId, CanTpRxPduPtr);
	        if (NULL_PTR != txNSduCfgPtr)
	        {
	            if ((FALSE == CanTp_ChnlPools[txNSduCfgPtr->TxChannel].Filled)
	                && (CANTP_LARGE_TRANSMITTING == CanTp_Channels[txNSduCfgPtr->TxChannel].RootState))
	            {
	                CanTp_ChnlPools[txNSduCfgPtr->TxChannel].PduInfo.SduLength = CanTpRxPduPtr->SduLength;
	                CanTp_MemoryCopy(	CanTpRxPduPtr->SduDataPtr,
	                    				CanTp_ChnlPools[txNSduCfgPtr->TxChannel].PduInfo.SduDataPtr,
	                    				CanTpRxPduPtr->SduLength);
	                CanTp_ChnlPools[txNSduCfgPtr->TxChannel].RxNPduId = CanTpRxPduId;
	                CanTp_ChnlPools[txNSduCfgPtr->TxChannel].Filled = TRUE;
	            }
	        }
	    }
#if(STD_ON == CANTP_PADDING_NEEDED)	    
    }
#endif
}

/*************************************************************************/
/*
 * Brief               Handle Confirm event occurred to the specific channel.
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
CanTp_DispatchCfmEvent(uint8 ChannelIdx)
{
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr = NULL_PTR;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_TX_CONFIRMED) != 0)
    {
        if ((CANTP_TX_SF_V4SF_CFM == channelPtr->Substate)
            || (CANTP_TX_LARGE_V4DATA_CFM == channelPtr->Substate))
        {
            /*A data frame transmit confirmed*/
            CanTp_TxHandleTransmitCfm(ChannelIdx);
        }
        else
        {
            channelPtr->EventFlags = channelPtr->EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_TX_CONFIRMED);
        }
    }

    if ((channelPtr->EventFlags & CANTP_CHANNEL_EVENT_TXFC_CONFIRMED) != 0)
    {
        if ((CANTP_RX_LARGE_V4FCCTS_CFM == channelPtr->Substate)
            || (CANTP_RX_LARGE_V4FCWT_CFM == channelPtr->Substate))
        {
            /*A data frame transmit confirmed*/
            CanTp_RxHandleFCCfm(ChannelIdx);
        }
        else
        {
            channelPtr->EventFlags = channelPtr->EventFlags & ((uint8)~CANTP_CHANNEL_EVENT_TXFC_CONFIRMED);
        }
    }
}
#define CANTP_STOP_SEC_CODE
#include "MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
