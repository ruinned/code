/*================================================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <ComM_CfgType.h>
 *  @brief      <Communication Manager Module Configuration Header File> 
 *  @author     <pinghai.xiong>
 *  @date       <2014-02-24>
 */
/*================================================================================================*/
#ifndef COMM_CFGTYPE_H
#define COMM_CFGTYPE_H

/*=======[R E V I S I O N   H I S T O R Y]========================================================*/
/*  <VERSION>   <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0      2014.02.24  pinghai.xiong   Initial version
 * 
 */
 
/*=======[F I L E  V E R S I O N   I N F O R M A T I O N]=========================================*/
#define COMM_CFGTYPE_H_AR_MAJOR_VERSION     2u 
#define COMM_CFGTYPE_H_AR_MINOR_VERSION     1u 
#define COMM_CFGTYPE_H_AR_PATCH_VERSION     0u 
#define COMM_CFGTYPE_H_SW_MAJOR_VERSION     1u
#define COMM_CFGTYPE_H_SW_MINOR_VERSION     0u
#define COMM_CFGTYPE_H_SW_PATCH_VERSION     0u 

/*=======[I N C L U D E S]========================================================================*/
#include "ComStack_Types.h"
#include "ComM_Cfg.h"
#include "Std_ExtendedTypes.h"

/*=======[T Y P E   D E F I N I T I O N S]========================================================*/
/* Defines block ID type */
#if (STD_ON == COMM_NVM_USED)
typedef uint16 ComM_NvMBlockIdType;
#endif /*#if (STD_ON == COMM_NVM_USED)*/

/* Define the RTE's notification */
#if (STD_ON == COMM_RTE_NOTIFY_USED)
typedef void (*ComM_RteNotifyType)(uint8);
#endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/

typedef uint8 ComM_UserHandleType;

/* Defines the bus type of the channel.*/
typedef enum
{
    COMM_BUS_TYPE_CAN = 0u,
    COMM_BUS_TYPE_FR,
    COMM_BUS_TYPE_INTERNAL,
    COMM_BUS_TYPE_LIN
}ComM_BusType;

/* Defines the functionality of the networkmanagement.Shall be harmonized with NM configuration.*/
typedef enum
{
    COMM_FULL = 0u,  /*AUTOSAR NM available (default).*/
    COMM_LIGHT,      /*No AUTOSAR NM available but functionality to shut down a channel.*/
    COMM_NONE,       /*No NM available.*/
    COMM_PASSIVE     /*AUTOSAR NM running in passive mode available.*/
}ComM_NmVariantType;

/* This container contains the configuration (parameters) of the bus channel(s).
 * The channel parameters shall be harmonized within the whole communication stack.*/
typedef struct
{
    /* Identifies the bus type of the channel. */
    /**@req ComM567*/
    ComM_BusType ComMBusType;
    
    /* Specifies the period in seconds that the MainFunction has to be triggered with.4~100ms*/
    /**@req ComM556*/
    uint16_least ComMMainFunctionPeriod;
	
    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
    /* ECU is not allowed to change state of the channel to "Silent Communication" or 
     * "Full Communication".*/
    /**@req ComM571*/
    boolean ComMNoCom;
    #endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

    /* Defines if an ECU is not allowed to wake-up the channel.*/
    /**@req ComM569*/
    #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
    boolean ComMNoWakeup;
    #endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/
    
    /* Reference to NVRAM block containing the none volatile data*/
    /**@req ComM670*/
    #if (STD_ON == COMM_NVM_USED)
    ComM_NvMBlockIdType ComMNvmBlockDescriptor;
    #endif /*#if (STD_ON == COMM_NVM_USED)*/
    
    /* Defines the timeout (in seconds) after state "ready sleep" is left.*/
    /**@req ComM606*/
    uint16_least ComMNmLightTimeout;

    /* Defines the functionality of the networkmanagement.Shall be harmonized with NM configuration.*/
    /**@req ComM568*/
    ComM_NmVariantType ComMNmVariant;

}ComM_ChannelCfgType;

/* This container contains a list of identifiers that are needed to refer to
 * a user in the system which is designated to request Communication modes.*/
typedef struct
{
    /* Mapping table between channel and user. */
    CONSTP2CONST(uint8, COMM_CONST, COMM_CONST) ChannelMap;
	
    #if (STD_ON == COMM_RTE_NOTIFY_USED)
    /* Mode switches notification */
    ComM_RteNotifyType RteNotify;

    /* Number of channels in the user */
    uint8 NumChannel;
    #endif /*#if (STD_ON == COMM_NVM_USED)*/
}ComM_UserCfgType;


#endif /*#ifndef COMM_CFGTYPE_H*/

