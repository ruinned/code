/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:57>
 */
/*============================================================================*/


#ifndef CANIF_CFG_H
#define CANIF_CFG_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_CFG_H_AR_MAJOR_VERSION    3
#define CANIF_CFG_H_AR_MINOR_VERSION    2
#define CANIF_CFG_H_AR_PATCH_VERSION    0
#define CANIF_CFG_H_SW_MAJOR_VERSION    1
#define CANIF_CFG_H_SW_MINOR_VERSION    0
#define CANIF_CFG_H_SW_PATCH_VERSION    8


/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"


/*=======[M A C R O S]========================================================*/
/* define CANIF_VARIANT TYPE */
#define CANIF_VARIANT_PC             VARIANT_PRE_COMPILE
#define CANIF_VARIANT_LT             VARIANT_LINK_TIME
#define CANIF_VARIANT_PB             VARIANT_POST_BUILD

/* define CanIf Software Filter Type */
#define CANIF_BINARY                0
#define CANIF_INDEX                 1
#define CANIF_LINEAR                2
#define CANIF_TABLE                 3      

/* define the Variant of the CanIf Module */
#define CANIF_VARIANT_CFG           CANIF_VARIANT_PC

/* CanIfPrivateConfiguration,contains the private configuration (parameters)
 * of the CAN Interface. 
 */
#define CANIF_DLC_CHECK             STD_ON

/* Define TxBuffer number*/
#define CANIF_NUMBER_OF_TXBUFFERS   12

/* Enable or disable Tx Buffer,depend on 
 * CANIF_NUMBER_OF_TXBUFFERS which configed above.
 * If bigger than 0, then configuration tool should set STD_ON
 */
#define CANIF_TX_BUFFER_USED        STD_ON

/* define software filter method 
 * CANIF_BINARY;CANIF_INDEX;CANIF_LINEAR;CANIF_TABLE
 */
#define CANIF_SOFTWARE_FILTER_TYPE  CANIF_LINEAR

/* CanIfPublicConfiguration */
#define CANIF_DEV_ERROR_DETECT                  STD_OFF
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT  STD_ON
#define CANIF_READRXPDU_DATA_API                STD_OFF
#define CANIF_READRXPDU_NOTIFY_STATUS_API       STD_OFF
#define CANIF_READTXPDU_NOTIFY_STATUS_API       STD_OFF
#define CANIF_SETDYNAMICTXID_API                STD_OFF
#define CANIF_VERSION_INFO_API                  STD_OFF

#define CANIF_WAKEUP_VALIDATION                 STD_OFF
#define CANIF_TRANSMIT_CANCELLATION             STD_OFF

/* user defined macros */
#define CANIF_DEM_ERROR_DETECT                  STD_ON
#define CANIF_CANTRANSCEIVER_SUPPORTED          STD_OFF


/* define Controller configuration number */
#define CANIF_MAX_CONTROLLER                    1



/* define Rx L-Pdu number */
#define CANIF_MAX_NUMBER_OF_CANRXPDUIDS             6

/* define Tx L-Pdu number */
#define CANIF_MAX_NUMBER_OF_CANTXPDUIDS             12

/* dynamic ID number */
#define CANIF_MAX_NUMBER_OF_DYNAMIC_CANTXPDUIDS     0

/* define RxBuffer number,config tool generate automatic when Rx Pdu config finished */
#define CANIF_MAX_NUMBER_OF_RXBUFFERS               0

#define CANIF_NUMBER_OF_HTHUSED                     3

#define CANIF_NUMBER_OF_HRHUSED                     4

#define CANIF_TXPDUID_DCM_TX         0
#define CANIF_TXPDUID_TXPDU_0         1
#define CANIF_TXPDUID_TXPDU_1         2
#define CANIF_TXPDUID_TXPDU_2         3
#define CANIF_TXPDUID_TXPDU_3         4
#define CANIF_TXPDUID_TXPDU_4         5
#define CANIF_TXPDUID_TXPDU_5         6
#define CANIF_TXPDUID_TXPDU_6         7
#define CANIF_TXPDUID_TXPDU_7         8
#define CANIF_TXPDUID_TXPDU_8         9
#define CANIF_TXPDUID_TXPDU_9         10
#define CANIF_TXPDUID_TXPDU_10         11

#define CANIF_RXPUDID_RXPDU_3         0
#define CANIF_RXPUDID_RXPDU_0         1
#define CANIF_RXPUDID_RXPDU_1         2
#define CANIF_RXPUDID_DCM_PHY         3
#define CANIF_RXPUDID_DCM_FUNC        4
#define CANIF_RXPUDID_RXPDU_2         3



#endif

/*=======[E N D   O F   F I L E]==============================================*/