/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <App_Com.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-02-25 10:48:10>
 */
/*============================================================================*/


/****************************** references *********************************/
#include "ComStack_Types.h"
#include "App_Com.h" 
#include "CanIf_Cfg.h" 
#include "CanIfDiag.h" 

#include "CanIf_type.h"
#include "Can_cbk.h"

uint8 test_app_data[8] = {0};
PduIdType   App_RxPduId;
PduInfoType App_Rx_Pdu;
uint8 App_Send[8];

void App_RxIndication(
    PduIdType AppRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) AppRxPduPtr
    ) 
{
    PduIdType i; 
    PduIdType pduIdIndex;
    PduInfoType CanIfPdu;
    uint32 CanId;
    uint8 CanLen, data[8];
    
    
    for (i = 0; i < CANIF_MAX_NUMBER_OF_CANRXPDUIDS; i++)
    {
        if (AppRxPduId == CanIf_RxPduConfigData[i].CanIfCanRxPduId)
        {
            CanId = CanIf_RxPduConfigData[i].CanIfCanRxPduCanId;
            break;
        }
    }
    
    CanLen = AppRxPduPtr->SduLength;
    for (i = 0; i < 8; i++) 
    {
        data[i] = AppRxPduPtr->SduDataPtr[i];
    }
    
    CanIf_AppRx(CanId, CanLen, data);
    
    /*App_RxPduId = AppRxPduId;
    App_Rx_Pdu.SduDataPtr = test_app_data;
    App_Rx_Pdu.SduDataPtr[0] =  AppRxPduPtr->SduDataPtr[0];
    App_Rx_Pdu.SduLength =  AppRxPduPtr->SduLength; */
	  //return E_OK;
}
void App_TxConfirmation() 
{
	  //return E_OK;
}

void App_TxCanData(const sCanIf_MsgType  msg_ptr) 
{
    PduIdType i; 
    PduIdType pduIdIndex;
    PduInfoType CanIfPdu;
    
    for (i = 0; i < CANIF_MAX_NUMBER_OF_CANTXPDUIDS; i++)
    {
        if (msg_ptr.id == CanIf_TxPduConfigData[i].CanIfCanTxPduIdCanId)
        {
            pduIdIndex = CanIf_TxPduConfigData[i].CanIfCanTxPduId;
            break;
        }
    }
    
    if(i >= CANIF_MAX_NUMBER_OF_CANTXPDUIDS)
      pduIdIndex = CanIf_TxPduConfigData[i-1].CanIfCanTxPduId;
    CanIfPdu.SduLength = msg_ptr.length;
    for(i = 0; i < 8; i++)
    {
        App_Send[i] = msg_ptr.data[i];
    }
    CanIfPdu.SduDataPtr = App_Send;
    (void)CanIf_Transmit(pduIdIndex, &CanIfPdu);
}


