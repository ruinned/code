/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_Types.h
 *  @brief      Public type definitions for CanTp module
 *  
 *  
 *  @author     stanley
 *  @date       2013-4-7
 */
/*============================================================================*/

#ifndef CANTP_TYPES_H 
#define CANTP_TYPES_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2013-4-7   stanley    create
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_TYPES_H_AR_MAJOR_VERSION  2
#define CANTP_TYPES_H_AR_MINOR_VERSION  3
#define CANTP_TYPES_H_AR_PATCH_VERSION  0
#define CANTP_TYPES_H_SW_MAJOR_VERSION  1
#define CANTP_TYPES_H_SW_MINOR_VERSION  0
#define CANTP_TYPES_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Os.h"
#include "Std_ExtendedTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
#if (CANTP_DEV_ERROR_DETECT == STD_ON)

    #define CANTP_SERVICEID_INIT      0x01

    #define CANTP_SERVICEID_SHUTDOWN  0x02

    #define CANTP_SERVICEID_TRANSMIT  0x03

    #define CANTP_SERVICEID_CANCELTRANSMITREQUEST 0x03

    #define CANTP_SERVICEID_RXINDICATION   0x04

    #define CANTP_SERVICEID_TXCONFIRMATION 0x05

    #define CANTP_SERVICEID_MAINFUNCTION   0x06

    #define CANTP_SERVICEID_GETVERSIONINFO 0x07

    /* Development error code for CanTp */
    #define CANTP_E_PARAM_CONFIG             0x01

    #define CANTP_E_PARAM_ID                 0x02

    #define CANTP_E_PARAM_ADDRESS            0x04

    #define CANTP_E_UNINIT                   0x20

    #define CANTP_E_INVALID_TX_ID            0x30

    #define CANTP_E_INVALID_RX_ID            0x40

    #define CANTP_E_INVALID_TX_BUFFER        0x50

    #define CANTP_E_INVALID_RX_BUFFER        0x60

    #define CANTP_E_INVALID_TX_LENGTH        0x70

    #define CANTP_E_INVALID_RX_LENGTH        0x80

    #define CANTP_E_INVALID_TATYPE           0x90
#endif /*End of MACROs for development error*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Addressing mode is supported for RX or TX N-SDU */
typedef enum CanTp_AddressingFormat_E
{
    CANTP_STANDARD, /* Standard addressing format */
    CANTP_EXTENDED /* Extended addressing format */
} CanTp_AddressingFormatType;

/* The communication type of RX or TX N-SDU */
typedef enum CanTp_TaType_E
{
    CANTP_FUNCTIONAL, /* Functional request type */
    CANTP_PHYSICAL /* physical request type */
} CanTp_TaTypeType;

/* Parameters needs to be configured for each CAN N-SDU that the CanTp module shall receive. */
typedef struct CanTp_RxNSdu_T
{
    /* communication addressing mode for this RX N-SDU */
    CanTp_AddressingFormatType AddressingFormat;

    /* Maximum number of N=PDUs the CanTp receiver allows the sender to send,
     * before waiting for an authorization to continue transmission of the following N-PDUs. */
    uint8 Bs;

    /* Value of the N-Ar timeout. Unit:the number of OS counter value. */
    TickType Nar;

    /* Value of the performance requirement for (N_Br+N_Ar). N_Br is the elapsed time between the receiving
     * indication of a FF or CF or the transmit confirmation of a FC, until the transmit request of the next FC. */
    TickType Nbr;

    /* Value of the N_Cr timeout. N_Cr is the time until the reception of the next Consecutive Frame N-PDU. */
    TickType Ncr;

    /* Link to the RX connection channel, which has to be used for receiving this N-PDU. */
    uint8 RxChannel;

    /* Data Length Code of this RxNsdu. In case of variable message length, this value indicates the minimum data length. */
    uint16 RxDl;

    /* PduId for referenced RX NPDU in the COM stack */
    PduIdType RxNPduId;

    /* PduId of the referenced PDU in the COM_Stack. */
    PduIdType RxNSduId;

    #if (CANTP_PADDING_NEEDED == STD_ON)
    /* Defines if the receive frame uses padding or not. */
    boolean RxPaddingActivation;
	#endif

    /* Declares the communication type of this Rx N-SDU. */
    CanTp_TaTypeType RxTaType;

    /* Indicates how many Flow Control wait N-PDUs can be consecutively transmitted by the receiver. */
    uint8 RxWftMax;

    /* Duration of the minimum time the CanTp Sender shall wait between the transmissions of two CF N-PDUs. */
    uint8 STmin;
    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    /* SA for RxNSdu with addressing format set to EXTENDED. */
    uint8 Sa;

    /* TA for RxNSdu with addressing format set to EXTENDED. */
    uint8 Ta;
	#endif

    /* PduId for used TX NPDU in COM stack, which is used as the FC-NPDU for this RxNSdu. */
    PduIdType TxFcNPduId;
} CanTp_RxNSduType;

/* Parameters needs to be configured for each CAN N_SDU that the CanTp module shall transmit. */
typedef struct CanTp_TxNSdu_T
{
    /* Declares which communication addressing format is supported for this TXNSdu. */
    CanTp_AddressingFormatType AddressingMode;

    /* Value of the N_As timeout. N_As is the time for transmission of a CAN frame on the part of the sender. */
    TickType Nas;

    /* Value of the N_Bs timeout. N_Bs is the time of transmission until reception of the next Flow Control N_PDU. */
    TickType Nbs;

    /* Value of the performation requirement of (N_Cs + N_As). N_Cs is the time which elapses
     *  between the transmit request of a CF N_PDU until the transmit request of the next CF N-PDU. */
    TickType Ncs;

    /* PduId of referenced RX FC N-PDU in COM stack. */
    PduIdType RxFcNPduId;

    #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    /* SA of TxNSdu with addressing mode set to EXTENDED. */
    uint8 Sa;

    /* TA of TxNSdu with addressing mode set to EXTENDED. */
    uint8 Ta;
	#endif

    /* Link to the connection channel which has to be used for transmission of this N-PDU. */
    uint8 TxChannel;

    /* Data Length Code of this TxNSdu. In  case of variable length message, this value indicates the minimum data length. */
    uint16 TxDl;

    /* PduId of referenced TxNPdu in COM stack.  */
    PduIdType TxNPduId;

    /* PduId of the referened Tx N_SDU in COM stack. */
    PduIdType TxNSduId;

    #if (CANTP_PADDING_NEEDED == STD_ON)
    /* Defines if the transmit frame use padding or not. */
    boolean TxPaddingActivation;
	#endif

    /* Communication type of this TxNSdu. */
    CanTp_TaTypeType TxTaType;
} CanTp_TxNSduType;

/* Configuration of the CanTp module */
typedef struct CanTp_Config_T
{
    P2CONST(CanTp_RxNSduType, TYPEDEF, CANTP_CONST) RxNSdus; /* link to RxNSdus */
    P2CONST(CanTp_TxNSduType, TYPEDEF, CANTP_CONST) TxNSdus; /* link to TxNSdus */
} CanTp_ConfigType;

/*Cancel Reason for CancelTransmitRequest*/
typedef enum CanTp_CancelReason_T
{
    CANTP_CNLDO, /*Cancel Transfer because data are outdated*/
    CANTP_CNLNB, /*Cancel Transfer because no further buffer can be provided*/
    CANTP_CNLOR  /*Cancel Transfer because of another reason*/
} CanTp_CancelReasonType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"
extern CONST(CanTp_ConfigType, CANTP_CONST) CanTp_CfgData;
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/
