/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp.h
 *  @brief      Public interfaces declared by CanTp module.
 *
 *  
 *  @author     stanley
 *  @date       2013-4-7
 */
/*============================================================================*/


#ifndef CANTP_H 
#define CANTP_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
/* Can transport layer */
#define CANTP_MODULE_ID   35
#define CANTP_VENDOR_ID  62

#define CANTP_H_AR_MAJOR_VERSION  2
#define CANTP_H_AR_MINOR_VERSION  3
#define CANTP_H_AR_PATCH_VERSION  0
#define CANTP_H_SW_MAJOR_VERSION  1
#define CANTP_H_SW_MINOR_VERSION  0
#define CANTP_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTpDiag_Types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_Init(void);

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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_Shutdown(void);

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
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_Transmit(
    PduIdType CanTpTxSduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    );

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
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_CancelTransmitRequest(
    PduIdType CanTpTxPduId,
    CanTp_CancelReasonType CanTpCacelReason
    );
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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_MainFunction(void);

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
extern FUNC(void, CANTP_APPL_CODE)
CanTp_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo
    );
#endif

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_H */

/*=======[E N D   O F   F I L E]==============================================*/
