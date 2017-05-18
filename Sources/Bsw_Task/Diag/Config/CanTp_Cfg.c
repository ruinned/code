/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanTp_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58> 
 */
/*============================================================================*/

/*=====[R E V I S I O N   H I S T O R Y]====================*/

/*====================================================*/
/*=======[V E R S I O N  I N F O R M A T I O N]===============*/
#define CANTP_CFG_C_AR_MAJOR_VERSION   2
#define CANTP_CFG_C_AR_MINOR_VERSION   3
#define CANTP_CFG_C_AR_PATCH_VERSION   0
#define CANTP_CFG_C_SW_MAJOR_VERSION   1
#define CANTP_CFG_C_SW_MINOR_VERSION   0
#define CANTP_CFG_C_SW_PATCH_VERSION   0

/*=======[I N C L U D E S]================================*/
#include "CanTp_Cfg.h"
#include "CanTpDiag_Types.h"
#include "Std_ExtendedTypes.h"
/*=======[V E R S I O N  C H E C K]============================*/
/*check version information with CanTp_Types.h*/
#if(CANTP_CFG_C_AR_MAJOR_VERSION != CANTP_TYPES_H_AR_MAJOR_VERSION)
	#error "CanTp_Cfg.c : Mismatch in Specification Major Version with CanTp_Types.h"
#endif
#if(CANTP_CFG_C_AR_MINOR_VERSION != CANTP_TYPES_H_AR_MINOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Specification Minor Version with CanTp_Types.h"
#endif
#if(CANTP_CFG_C_AR_PATCH_VERSION != CANTP_TYPES_H_AR_PATCH_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Specification Patch Version with CanTp_Types.h"
#endif
#if(CANTP_CFG_C_SW_MAJOR_VERSION != CANTP_TYPES_H_SW_MAJOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
#if(CANTP_CFG_C_SW_MINOR_VERSION != CANTP_TYPES_H_SW_MINOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Software Implementation Major Version with CanTp_Types.h"
#endif
/*check version information with CanTp_Cfg.h*/
#if(CANTP_CFG_C_AR_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Specification Major Version with CanTp_Cfg.h"
#endif
#if(CANTP_CFG_C_AR_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Specification Minor Version with CanTp_Cfg.h"
#endif
#if(CANTP_CFG_C_AR_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Specification Patch Version with CanTp_Cfg.h"
#endif
#if(CANTP_CFG_C_SW_MAJOR_VERSION != CANTP_CFG_H_SW_MAJOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
#if (CANTP_CFG_C_SW_MINOR_VERSION != CANTP_CFG_H_SW_MINOR_VERSION)
  #error "CanTp_Cfg.c : Mismatch in Software Implementation Major Version with CanTp_Cfg.h"
#endif
/*=======[I N T E R N A L   D A T A]=======================*/
#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"
STATIC CONST(CanTp_RxNSduType, CANTP_CONST) CanTp_RxNSdus[CANTP_RXNSDU_NUMBER] =
{	
	/*CanTpRxNSdu_Fnc*/
    {
      /* communication addressing mode for this RX N-SDU */
          CANTP_STANDARD,
      /* BS, Maximum number of N=PDUs the CanTp receiver allows the sender to send,before waiting for an authorization to continue transmission of the following N-PDUs. */
          8,
      /* Value of the N-Ar timeout. Unit:the number of OS counter value. */
          70,
      /* Value of the performance requirement for (N_Br+N_Ar). N_Br is the elapsed time between the receiving，indication of a FF or CF or the transmit confirmation of a FC, until the transmit request of the next FC. */
          70,
      /* Value of the N_Cr timeout. N_Cr is the time until the reception of the next Consecutive Frame N-PDU. */
          150,
      /* Link to the RX connection channel, which has to be used for receiving this N-PDU. */
          0,
	  /* Data Length Code of this RxNsdu. In case of variable message length, this value indicates the minimum data length. */
          1,
      /* PduId for referenced RX NPDU in the COM stack */
          0x4,	
      /* PduId of the referenced PDU in the COM_Stack. */
          0x4,
      #if (CANTP_PADDING_NEEDED == STD_ON)
      /* Defines if the receive frame uses padding or not. */
        TRUE,
		#endif
      /* Declares the communication type of this Rx N-SDU. */
        CANTP_FUNCTIONAL,
      /* Indicates how many Flow Control wait N-PDUs can be consecutively transmitted by the receiver. */
        0,
      /* Duration of the minimum time the CanTp Sender shall wait between the transmissions of two CF N-PDUs. */
        20,
      #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
      /* SA for RxNSdu with addressing format set to EXTENDED. */
        0xff,
      /* TA for RxNSdu with addressing format set to EXTENDED. */
        0xff,
	 #endif
      /* PduId for used TX NPDU in COM stack, which is used as the FC-NPDU for this RxNSdu. */
        0
    },
	/*CanTpRxNSdu_Phy*/
    {
      /* communication addressing mode for this RX N-SDU */
          CANTP_STANDARD,
      /* BS, Maximum number of N=PDUs the CanTp receiver allows the sender to send,before waiting for an authorization to continue transmission of the following N-PDUs. */
          0,
      /* Value of the N-Ar timeout. Unit:the number of OS counter value. */
          70,
      /* Value of the performance requirement for (N_Br+N_Ar). N_Br is the elapsed time between the receiving，indication of a FF or CF or the transmit confirmation of a FC, until the transmit request of the next FC. */
          70,
      /* Value of the N_Cr timeout. N_Cr is the time until the reception of the next Consecutive Frame N-PDU. */
          150,
      /* Link to the RX connection channel, which has to be used for receiving this N-PDU. */
          1,
	  /* Data Length Code of this RxNsdu. In case of variable message length, this value indicates the minimum data length. */
          1,
      /* PduId for referenced RX NPDU in the COM stack */
          0x3,	
      /* PduId of the referenced PDU in the COM_Stack. */
          0x3,
      #if (CANTP_PADDING_NEEDED == STD_ON)
      /* Defines if the receive frame uses padding or not. */
        TRUE,
		#endif
      /* Declares the communication type of this Rx N-SDU. */
        CANTP_PHYSICAL,
      /* Indicates how many Flow Control wait N-PDUs can be consecutively transmitted by the receiver. */
        0,
      /* Duration of the minimum time the CanTp Sender shall wait between the transmissions of two CF N-PDUs. */
        10,
      #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
      /* SA for RxNSdu with addressing format set to EXTENDED. */
        0xff,
      /* TA for RxNSdu with addressing format set to EXTENDED. */
        0xff,
	 #endif
      /* PduId for used TX NPDU in COM stack, which is used as the FC-NPDU for this RxNSdu. */
        0
    }
};

STATIC CONST(CanTp_TxNSduType, CANTP_CONST) CanTp_TxNSdus[CANTP_TXNSDU_NUMBER] =
{
	/*CanTpTxNSdu_1*/
    {
      /* Declares which communication addressing format is supported for this TXNSdu. */
        CANTP_STANDARD,
/* Value of the N_As timeout. N_As is the time for transmission of a CAN frame on the part of the sender. */
        70,
/* Value of the N_Bs timeout. N_Bs is the time of transmission until receptionof  the next Flow Control N_PDU. */
        150,

     /* Value of the performation requirement of (N_Cs + N_As). N_Cs is the time which elapses  between the transmit request of a CF N_PDU until the transmit request of the next CF N-PDU. */
        140,
    /* PduId of referenced RX FC N-PDU in COM stack. */
        3,	
     #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
        /* SA of TxNSdu with addressing mode set to EXTENDED. */
        0xff,
        /* TA of TxNSdu with addressing mode set to EXTENDED. */
        0xff,
	 #endif
/* Link to the connection channel which has to be used for transmission of this N-PDU. */
        1,
/* Data Length Code of this TxNSdu. In  case of variable length message, this value indicates the minimum data length. */
        1,
    /* PduId of referenced TxNPdu in COM stack.  */
       	0, 
   /* PduId of the referened Tx N_SDU in COM stack. */
        0,
        #if (CANTP_PADDING_NEEDED == STD_ON)
        /* Defines if the transmit frame use padding or not. */
        TRUE,
		#endif

        /* Communication type of this TxNSdu. */
        CANTP_PHYSICAL
    }
};
CONST(CanTp_ConfigType, CANTP_CONST) CanTp_CfgData =
{
    CanTp_RxNSdus,
    CanTp_TxNSdus
};
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"
/*=======[E X T E R N A L   D A T A]===========================*/
/*=======[E N D   O F   F I L E]==============================*/
