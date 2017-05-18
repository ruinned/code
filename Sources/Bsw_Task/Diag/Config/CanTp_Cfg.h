/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanTp_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58>
 */
/*============================================================================*/


#ifndef CANTP_CFG_H
#define CANTP_CFG_H
/*========[V E R S I O N  I N F O R M A T I O N]=========*/
#define CANTP_CFG_H_AR_MAJOR_VERSION  2
#define CANTP_CFG_H_AR_MINOR_VERSION  3
#define CANTP_CFG_H_AR_PATCH_VERSION   0
#define CANTP_CFG_H_SW_MAJOR_VERSION  1
#define CANTP_CFG_H_SW_MINOR_VERSION  0
#define CANTP_CFG_H_SW_PATCH_VERSION   0
/*======== [I N C L U D E S]========================*/
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*========[M A C R O S] ==========================*/
#define CANTP_INSTANCE_ID               0x0
/* Total number of channel used in CanTp module. */
#define CANTP_CHANNEL_NUMBER 		   0x2
/* Switches the Development Error Detection and Notification ON or OFF */
#define CANTP_DEV_ERROR_DETECT 		STD_OFF
/* The time for MainFunction,expressed as the value with the unit for os counter */
#define CANTP_MAIN_FUNCTION_PERIOD 	0xA
/*****************************************************/
/*configuration MACRO extended for cutting*/
/*switch for if the ECU need padding for TP frames*/
#define CANTP_PADDING_NEEDED 		    STD_ON

/* Used for the initialization of unused bytes with a certain value */
#define CANTP_PADDING_BYTE 			0x00 //0x55
/**********************************************/
/* Total number of RX-NSdu. */
#define CANTP_RXNSDU_NUMBER 		0x2

/* Total number of TX-NSdu in CanTp module. */
#define CANTP_TXNSDU_NUMBER 		0x1

/**********************************************/
/* Preprocessor switch for enabling Transmit Cancellation */
#define CANTP_TC 					    STD_OFF
#define CANTP_VERSION_INFO_API 		STD_OFF
/*the counter ID from OS used for CANTP timers*/
#define CANTP_OS_COUNTER_ID 		    0x0
/*switch for if the ECU needed to support extended addressing model*/
#define CANTP_EXTENDED_ADDRESSING_SUPPORT STD_OFF

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif /* CANTP_CFG_H */

