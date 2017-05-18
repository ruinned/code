/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <ComM_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100> 
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58>
 */
/*============================================================================*/

	

/*=======[V E R S I O N  I N F O R M A T I O N]===================================================*/
#define COMM_CFG_C_AR_MAJOR_VERSION  2
#define COMM_CFG_C_AR_MINOR_VERSION  1
#define COMM_CFG_C_AR_PATCH_VERSION  0
#define COMM_CFG_C_SW_MAJOR_VERSION  1
#define COMM_CFG_C_SW_MINOR_VERSION  0
#define COMM_CFG_C_SW_PATCH_VERSION  0

/*================================[I N C L U D E S]===============================================*/
#include "ComM_Cfg.h"
#include "ComM_CfgType.h"
#if (STD_ON == COMM_RTE_NOTIFY_USED)
#include "Rte.h"
#endif
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===============================================================*/
#if (COMM_CFG_C_AR_MAJOR_VERSION != COMM_CFG_H_AR_MAJOR_VERSION)
    #error "ComM_Cfg.c:Mismatch in Specification Major Version"
#endif /*COMM_CFG_C_AR_MAJOR_VERSION != COMM_CFG_H_AR_MAJOR_VERSION */
#if (COMM_CFG_C_AR_MINOR_VERSION != COMM_CFG_H_AR_MINOR_VERSION)
    #error "ComM_Cfg.c:Mismatch in Specification Minor Version"
#endif /*COMM_CFG_C_AR_MINOR_VERSION != COMM_CFG_H_AR_MINOR_VERSION */
#if (COMM_CFG_C_AR_PATCH_VERSION != COMM_CFG_H_AR_PATCH_VERSION)
    #error "ComM_Cfg.c:Mismatch in Specification Patch Version"
#endif /*COMM_CFG_C_AR_PATCH_VERSION != COMM_CFG_H_AR_PATCH_VERSION */
#if (COMM_CFG_C_SW_MAJOR_VERSION != COMM_CFG_H_SW_MAJOR_VERSION)
    #error "ComM_Cfg.c:Mismatch in Specification Major Version"
#endif /*COMM_CFG_C_SW_MAJOR_VERSION != COMM_CFG_H_SW_MAJOR_VERSION */
#if (COMM_CFG_C_SW_MINOR_VERSION != COMM_CFG_H_SW_MINOR_VERSION)
    #error "ComM_Cfg.c:Mismatch in Specification Minor Version"
#endif /*COMM_CFG_C_SW_MINOR_VERSION != COMM_CFG_H_SW_MINOR_VERSION */

/*===========================[E X T E R N A L   D A T A]==========================================*/
#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

CONST(ComM_ChannelCfgType, COMM_CONST) ComM_ChannelConfig[1] =
{
    {
        COMM_BUS_TYPE_CAN,
        10,
        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
        FALSE,
        #endif
        #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
        FALSE,
        #endif
        #if (STD_ON == COMM_NVM_USED)
        0x0,
        #endif
        0,
        COMM_NONE
    },
};

CONST(uint8, COMM_CONST) ComM_ChannelMapConfig[1][1] = 
{
    {0x01},
};

CONST(ComM_UserCfgType, COMM_CONST) ComM_UserConfig[1] = 
{
    {
        &ComM_ChannelMapConfig[0][0],
        #if (STD_ON == COMM_RTE_NOTIFY_USED)
        NULL_PTR,
        0
        #endif
    },
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
