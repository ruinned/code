/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <PduR_Dcm.h>
 *  @brief      <Declear the Headfiles of PduR Module>
 *
 *  <Compiler:      MCU:  >
 *
 *  @author     <Huanyu.Zhao>
 *  @date       <01-04-2015>
 */
/*============================================================================*/
#ifndef  PDUR_DCM_H
#define  PDUR_DCM_H 

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
#define PDUR_DCM_H_AR_MAJOR_VERSION      2U
#define PDUR_DCM_H_AR_MINOR_VERSION      3U
#define PDUR_DCM_H_AR_PATCH_VERSION      0U
#define PDUR_DCM_H_SW_MAJOR_VERSION      1U
#define PDUR_DCM_H_SW_MINOR_VERSION      0U
#define PDUR_DCM_H_SW_PATCH_VERSION      0U

/*=======[I N C L U D E S]====================================================*/
#include "PduR.h"
#if(STD_ON == PDUR_DCM_SUPPORT)

#if(STD_ON == PDUR_CANTP_SUPPORT)
#include "CanTpDiag.h"
#endif
#if(STD_ON == PDUR_LINTP_SUPPORT)
#include "LinTp.h"
#endif
/*=======[M A C R O S]========================================================*/

/*==========[M A C R O  F U N C T I O N S]====================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * Brief               Requests a tranmission for the DCM module
 *
 * ServiceId           0x15
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId:ID of DCM I-PDU that has been transmitted.
 * 					   PduInfoPtr: Contains the lenght of the received I-PDU
 * 					   and a pointer to a buffer containing the I-SDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#if((STD_ON == PDUR_CANTP_SUPPORT) && (CAN_TP == PDUR_SINGLE_TP))
#define PduR_DcmTransmit CanTp_Transmit
#elif ((STD_ON ==  PDUR_LINTP_SUPPORT) && (LIN_TP == PDUR_SINGLE_TP))
#define PduR_DcmTransmit LinTp_Transmit
#endif

#endif /* #if(STD_ON == PDUR_DCM_SUPPORT) */

/*=======[I N T E R N A L   D A T A]==========================================*/



/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/



#endif  /* end of PDUR_DCM_H */

/*=======[E N D   O F   F I L E]==============================================*/

