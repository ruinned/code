/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <App_Com.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-02-25 10:48:10>
 */
/*============================================================================*/


#ifndef APP_COM_H
#define APP_COM_H  

#include "Std_Types.h"
#include "CanIf_type.h"

/*********************************/

extern void App_RxIndication
(PduIdType AppRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) AppRxPduPtr);
    
extern void App_TxConfirmation();
extern void App_TxCanData(const sCanIf_MsgType  msg_ptr);


#endif /* APP_COM_H */
