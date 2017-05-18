/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanSM_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58>
 */
/*============================================================================*/


#ifndef CANSM_CFG_H
#define CANSM_CFG_H
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_CFG_H_AR_MAJOR_VERSION   1U
#define CANSM_CFG_H_AR_MINOR_VERSION   2U
#define CANSM_CFG_H_AR_PATCH_VERSION   0U
#define CANSM_CFG_H_SW_MAJOR_VERSION   1U
#define CANSM_CFG_H_SW_MINOR_VERSION   0U
#define CANSM_CFG_H_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
/*=======[M A C R O S]========================================================*/

/* configuration variants */
#define CANSM_VARIANT_PRE_COMPILE    VARIANT_PRE_COMPILE
#define CANSM_VARIANT_LINK_TIME      VARIANT_LINK_TIME
#define CANSM_VARIANT_POST_BUILD     VARIANT_POST_BUILD

/* version information switch */
/* @req CANSM027 */
#define CANSM_VERSION_INFO_API       STD_OFF

/* development error trace switch */
/* @req CANSM133 */
#define CANSM_CONFIGURATION_USE_DET  STD_OFF

/* production error trace switch */
#define CANSM_CONFIGURATION_USE_DEM  STD_ON

/* polling confirmation switch */
#define CANSM_BOR_TX_CONFIRMATION_POLLING   STD_ON

/* bus-off recovery disable reception deadline monitoring switch */
#define CANSM_BOR_DISABLE_RX_DL_MONITORING  STD_OFF

/* CanSM network handle number */
#define CANSM_NETWORK_NUM            1
/* CanSM controller handle number*/
#define CANSM_CONTROLLER_NUM         1

/* fixed time for periodic function */
#define CANSM_PERIOD_TIME            10

/* NM module used switch */
#define CANSM_USE_OSEKNM             STD_OFF

/* Use transceiver switch */
#define CANSM_USE_TRANSCEIVER        STD_OFF

/* communication module support switch */
#define CANSM_USE_COM                STD_OFF

/* configuration variants switch */
#define CANSM_VARIANT_CLASS          CANSM_VARIANT_PRE_COMPILE

#endif /* CANSM_CFG_H */

/*=======[E N D   O F   F I L E]==============================================*/
