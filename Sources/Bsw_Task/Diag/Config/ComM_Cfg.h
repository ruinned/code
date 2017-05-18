/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <ComM_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58>
 */
/*============================================================================*/

	

#ifndef COMM_CFG_H
#define COMM_CFG_H
 
/*=======[F I L E  V E R S I O N   I N F O R M A T I O N]=========================================*/
#define COMM_CFG_H_AR_MAJOR_VERSION     2
#define COMM_CFG_H_AR_MINOR_VERSION     1
#define COMM_CFG_H_AR_PATCH_VERSION     0
#define COMM_CFG_H_SW_MAJOR_VERSION     1
#define COMM_CFG_H_SW_MINOR_VERSION     0
#define COMM_CFG_H_SW_PATCH_VERSION     0

/*=======[I N C L U D E S]========================================================================*/

/*=======[M A C R O S]============================================================================*/
/*
 * ComMGeneral
 */
/* Whether the CAN bus is supported or not.*/
#define COMM_BUS_CAN_USED                   STD_ON

/* Whether the LIN bus is supported or not.*/
#define COMM_BUS_LIN_USED                   STD_OFF

/* Whether the FlexRay bus is supported or not.*/
#define COMM_BUS_FLEXRAY_USED               STD_OFF

/* Whether the NM is supported or not.*/
#define COMM_NM_USED                        STD_OFF

/* Whether the none volatile memory is supported or not.*/
#define COMM_NVM_USED                       STD_OFF

/* Whether the DCM is supported or not.*/
#define COMM_DCM_USED                       STD_ON

/* Whether the ECUM is supported or not.*/
#define COMM_ECUM_USED                      STD_OFF

/* Whether the RTE notification is supported or not.*/
#define COMM_RTE_NOTIFY_USED                STD_OFF

/* Switches the Development Error Detection and Notification ON or OFF.*/
/**@req ComM555*/
#define COMM_DEV_ERROR_DETECT               STD_OFF

/* Switches the Porduct Error Detection ON or OFF.*/
#define COMM_DEM_ERROR_DETECT               STD_ON

/* Defines whether a mode inhibition affects the ECU or not.*/
/**@req ComM563*/
#define COMM_ECU_GROUP_CLASSIFICATION       0x3

/* True if mode limitation functionality shall be enabled. true:Enabled false: Disabled*/
/**@req ComM560*/
#define COMM_MODE_LIMITATION_ENABLED        STD_OFF

/* True if wake up inhibition functionality enabled.*/
/**@req COMM-CFG-010[ComM559]*/
#define COMM_WAKEUP_INHIBITION_ENABLED      STD_OFF

/* The ECU is not allowed to change state of the ECU to "Silent Communication" or 
 *"Full Communication".*/
/**@req COMM-CFG-005[ComM561]*/
#define COMM_NO_COM                         STD_OFF

/* ComM shall perform a reset after entering "No Communication" mode because of an active mode 
 * limitation to "No Communication" mode.*/
/**@req COMM-CFG-006[ComM558]*/
#define COMM_RESET_AFTER_FORCING_NO_COM     STD_OFF

/* Wake up of one channel shall lead to a wake up of all channels if true.*/
/**@req COMM-CFG-007[ComM695]*/
#define COMM_SYNCHRONOUS_WAKE_UP            STD_OFF

/* Minimum time duration in seconds, spent in the Full Communication mode.*/
/**@req COMM-CFG-008[ComM557]*/
#define COMM_T_MIN_FULL_COM_MODE_DURATION   100

/* Switches the possibility to read the published information with the service.*/
/**@req COMM-CFG-009[ComM622]*/
#define COMM_VERSION_INFO_API               STD_OFF

/* Reference to NvmBlockDescriptor.*/
/**@req COMM-CFG-011[ComM783]*/
#define COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR    0x0

#define COMM_NM_PASSIVE_USED                STD_OFF

/*
 * Additional Configuration
 */
/* Number of the channels */
#define COMM_NUMBER_OF_CHANNELS             1

/* Number of the user */
#define COMM_NUMBER_OF_USERS                1

#define ComM_MainFunction_00()     (ComM_MainFunction(0u))

#endif /*#ifndef COMM_CFG_H*/
