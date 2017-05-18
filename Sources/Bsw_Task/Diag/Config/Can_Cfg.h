/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Can_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:57>
 */
/*============================================================================*/

	
/*============================================================================*/
#ifndef CAN_CFG_H
#define CAN_CFG_H
 
/*=======[F I L E  V E R S I O N   I N F O R M A T I O N]===============================*/
#define CAN_CFG_H_AR_MAJOR_VERSION     2U 
#define CAN_CFG_H_AR_MINOR_VERSION     4U 
#define CAN_CFG_H_AR_PATCH_VERSION     0U 
#define CAN_CFG_H_SW_MAJOR_VERSION     1U
#define CAN_CFG_H_SW_MINOR_VERSION     0U
#define CAN_CFG_H_SW_PATCH_VERSION     0U 

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
 
/*=======[R E L E A S E   V E R S I O N   I N F O R M A T I O N]===============================*/
#define CAN_VENDOR_ID                 62U         /* Replace with a symbolic name once available */
#define CAN_MODULE_ID                 80U         /* from AUTOSAR_BasicSoftwareModules.pdf */
#define CAN_INSTANCE                  0U
#define CAN_AR_MAJOR_VERSION          2U 
#define CAN_AR_MINOR_VERSION          4U 
#define CAN_AR_PATCH_VERSION          0U 
#define CAN_SW_MAJOR_VERSION          1U
#define CAN_SW_MINOR_VERSION          0U
#define CAN_SW_PATCH_VERSION          0U 

/*=======[M A C R O S]========================================================*/
/*
 * Variants Class (PC, PB)
 */
#define CAN_VARIANT_PC                VARIANT_PRE_COMPILE
#define CAN_VARIANT_PB                VARIANT_POST_BUILD
#define CAN_VARIANT_CFG               CAN_VARIANT_PC

/*
 * CanGeneral
 */
#define CAN_DEV_ERROR_DETECT            STD_OFF    /* development error detection */
#define CAN_DEM_ERROR_DETECT            STD_ON   /* public error detection */

#define CAN_HW_TRANSMIT_CANCELLATION    STD_OFF    /* transmision cancel */
#define CAN_MULTIPLEXED_TRANSMISSION    STD_OFF    /* multiplexed transmision */
#define CAN_WAKEUP_SUPPORT              STD_OFF   /* wakeup */
#define CAN_VERSION_INFO_API            STD_ON    /* version info get API */

/* max. number of tight SW loops to execute while waiting for a state change */
#define CAN_TIMEOUT_DURATION            10000

#define CAN_MAINFUNCTION_BUSOFFPERIOD   100U
#define CAN_MAINFUCTION_WRITEPERIOD     100U
#define CAN_MAINFUNCTION_READPERIOD     100U
#define CAN_MAINFUNCTION_WAKEUPPERIOD   100U

/* Instance Id of the module instance */
#define CAN_INDEX                       0U

/* 
 * CanController
 */
/* Can controller id */
#define CAN_CONTROLLER_0                0

#define CAN_CONTROLLER_1                1

#define CAN_CONTROLLER_2                2

#define CAN_CONTROLLER_3                3

#define CAN_CONTROLLER_4                4

#define CAN_MAX_CONTROLLERS             1

#define CAN_CONTROLLER0_BUSOFF_INTERRUPT      STD_ON
#define CAN_CONTROLLER0_RX_INTERRUPT          STD_ON
#define CAN_CONTROLLER0_TX_INTERRUPT          STD_ON
#define CAN_CONTROLLER0_WAKEUP_INTERRUPT      STD_ON

#define CAN_CONTROLLER1_BUSOFF_INTERRUPT      STD_OFF
#define CAN_CONTROLLER1_RX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER1_TX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER1_WAKEUP_INTERRUPT      STD_OFF

#define CAN_CONTROLLER2_BUSOFF_INTERRUPT      STD_OFF
#define CAN_CONTROLLER2_RX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER2_TX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER2_WAKEUP_INTERRUPT      STD_OFF

#define CAN_CONTROLLER3_BUSOFF_INTERRUPT      STD_OFF
#define CAN_CONTROLLER3_RX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER3_TX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER3_WAKEUP_INTERRUPT      STD_OFF

#define CAN_CONTROLLER4_BUSOFF_INTERRUPT      STD_OFF
#define CAN_CONTROLLER4_RX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER4_TX_INTERRUPT          STD_OFF
#define CAN_CONTROLLER4_WAKEUP_INTERRUPT      STD_OFF

#define CAN_BUSOFF_POLLING                   STD_OFF
#define CAN_RX_POLLING                       STD_OFF
#define CAN_TX_POLLING                       STD_OFF
#define CAN_WAKEUP_POLLING                   STD_OFF

/*
 * Additional Configure
 */
#define CAN_MAX_HARDWAREOBJECTS              7U
#define CAN_MAX_HOHS                         7U

#endif /* #define CAN_CFG_H */

/*=======[E N D   O F   F I L E]==============================================*/