/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <PduR_CanTP.h>
 *  @brief      <Declear the Headfiles of PduR Module>
 *
 *  <Compiler:      MCU:  >
 *
 *  @author     <Huanyu.Zhao>
 *  @date       <01-04-2015>
 */
/*============================================================================*/
#ifndef  PDUR_CANTP_H 
#define  PDUR_CANTP_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0     20150401  Huanyu.Zhao Initial version
 *
 *
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define PDUR_VENDOR_ID               62U
#define PDUR_MODULE_ID               51U
#define PDUR_CANTP_H_AR_MAJOR_VERSION      2U
#define PDUR_CANTP_H_AR_MINOR_VERSION      3U
#define PDUR_CANTP_H_AR_PATCH_VERSION      0U
#define PDUR_CANTP_H_SW_MAJOR_VERSION      1U
#define PDUR_CANTP_H_SW_MINOR_VERSION      0U
#define PDUR_CANTP_H_SW_PATCH_VERSION      0U

/*=======[I N C L U D E S]====================================================*/
#include "PduR.h"

#if(STD_ON == PDUR_CANTP_SUPPORT)

#if(STD_ON == PDUR_DCM_SUPPORT)
#include "Dcm_Cbk.h"
#endif

/*=======[M A C R O S]========================================================*/


/*==========[M A C R O  F U N C T I O N S]====================================*/



/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/



/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if((STD_ON == PDUR_DCM_SUPPORT)  && (CAN_TP == PDUR_SINGLE_TP))

/******************************************************************************/
/*
 * Brief               Provides Rx buffer for the CAN TP
 *
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId:ID of CAN N-PDU that shall be received.
 * 					   TpSduLength:This length Idenfifies the overall number of
 * 					   bytes to be received.
 *
 * Param-Name[out]     PduInfoPtr:Pointer to pointer to PduInfoStructure containing
 * 						SDU data pointer and SDU length of a receive buffer.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType:BUFREQ_OK:Buffer request accomplished successful
 * 						BUFREQ_E_BUSY:Currently no buffer available
 * 						BUFREQ_E_OVFL:Receiver is not able to receive number of TpSduLength
 * 						Byters;
 * 						BUFREQ_E_NOT_OK:Buffer request not successful, no buffer rovided.
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PduR_CanTpProvideRxBuffer Dcm_ProvideRxBuffer

/******************************************************************************/
/*
 * Brief               Rx indicator for the CAN TP
 *
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId:ID of CAN L-PDU that has been Transmitted.
 *						Result:NTFRSLT_OK:
 *						NTFRSLT_E_NOT_OK,NTFRSLT_E_CANCELLATION_NOT_OK,
 *						NTFRSLT_E_TIMEOUT_A,NTFRSLT_E_TIMEOUT_Cr,
 *						NTFRSLT_E_WRONG_SN,NTFRSLT_E_UNEXP_PDU,NTFRSLT_E_NO_BUFFER
 *						in case TP reception did not complete successfully;
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PduR_CanTpRxIndication Dcm_RxIndication

/******************************************************************************/
/*
 * Brief               Rx indicator for the CAN TP
 *
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId:ID of CAN L-PDU that has been Transmitted.
 *						Result:NTFRSLT_OK:
 *						NTFRSLT_E_NOT_OK,NTFRSLT_E_CANCELLATION_NOT_OK,
 *						NTFRSLT_E_TIMEOUT_A,NTFRSLT_E_TIMEOUT_Cr,
 *						NTFRSLT_E_WRONG_SN,NTFRSLT_E_UNEXP_PDU,NTFRSLT_E_NO_BUFFER
 *						in case TP reception did not complete successfully;
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PduR_CanTpProvideTxBuffer Dcm_ProvideTxBuffer


/******************************************************************************/
/*
 * Brief               Tx confirmation for the CAN TP
 *
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxPduId:ID of CAN N-PDU that has been Transmitted.
 *					    Result:NTFRSLT_OK:
 *						NTFRSLT_E_NOT_OK,NTFRSLT_E_CANCELLATION_NOT_OK,
 *						NTFRSLT_E_TIMEOUT_A,NTFRSLT_E_TIMEOUT_Cr,
 *						NTFRSLT_E_WRONG_SN,NTFRSLT_E_UNEXP_PDU,NTFRSLT_E_NO_BUFFER
 *						in case TP reception did not complete successfully;
 * Param-Name[out]     None
 *
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PduR_CanTpTxConfirmation Dcm_TxConfirmation

#endif /* #if((STD_ON == PDUR_DCM_SUPPORT)  && (CAN_TP == PDUR_SINGLE_TP)) */

#endif /* #if(STD_ON == PDUR_DCM_SUPPORT) */
/*=======[I N T E R N A L   D A T A]==========================================*/



/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/




#endif  /* end of PDUR_CANTP_H */

/*=======[E N D   O F   F I L E]==============================================*/

