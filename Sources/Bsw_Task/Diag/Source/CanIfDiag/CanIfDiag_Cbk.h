/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf_Cbk.h>
 *  @brief      <CanIf>
 *  
 *  <Compiler: CodeWarrior2.8 MCU:MPC5634>
 *  
 *  @author     <Tommy>
 *  @date       <2013-05-10> 
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130425   liujn      Initial version

 *  V1.0.1       20130510   Tommy      reconstruct version
 * 
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/

#ifndef CANIFDIAG_CBK_H
#define CANIFDIAG_CBK_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_CBK_H_VENDOR_ID          60U
#define CANIF_CBK_H_AR_MAJOR_VERSION   3U
#define CANIF_CBK_H_AR_MINOR_VERSION   2U
#define CANIF_CBK_H_AR_PATCH_VERSION   0U
#define CANIF_CBK_H_SW_MAJOR_VERSION   1U
#define CANIF_CBK_H_SW_MINOR_VERSION   0U
#define CANIF_CBK_H_SW_PATCH_VERSION   8U

/*=======[I N C L U D E S]====================================================*/
#include "Can.h"
#include "CanIf_Cfg.h"


/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[F U N C T I O N   I M P L E M E N T A T V O N S]====================*/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIf_TxConfirmation(PduIdType CanTxPduId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIfDiag_RxIndication
( 
    uint8 Hrh, 
    Can_IdType CanId, 
    uint8 CanDlc, 
    P2CONST(uint8, AUTOMATIC, CANIF_APPL_CONST) CanSduPtr 
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (CANIF_TRANSMIT_CANCELLATION == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIf_CancelTxConfirmation(P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_CONST) PduInfoPtr );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE)
CanIf_ControllerBusOff(uint8 Controller);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#endif /* #ifndef CANIF_CBK_H */

/*=======[E N D   O F   F I L E]==============================================*/
