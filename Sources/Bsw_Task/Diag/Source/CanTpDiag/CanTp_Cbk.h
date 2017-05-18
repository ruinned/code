/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_Cbk.h
 *  @brief      Call-back functions declarations
 *  
 *  @author     stanley
 *  @date       2013-4-7
 */
/*============================================================================*/


#ifndef CANTP_CBK_H 
#define CANTP_CBK_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2013-4-7   stanley     create
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_CBK_H_AR_MAJOR_VERSION  2
#define CANTP_CBK_H_AR_MINOR_VERSION  3
#define CANTP_CBK_H_AR_PATCH_VERSION  0
#define CANTP_CBK_H_SW_MAJOR_VERSION  1
#define CANTP_CBK_H_SW_MINOR_VERSION  0
#define CANTP_CBK_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTpDiag_Types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_RxIndication(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr
    );

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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_TxConfirmation(PduIdType CanTpTxPduId);

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_CBK_H */

/*=======[E N D   O F   F I L E]==============================================*/
