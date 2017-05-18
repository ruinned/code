/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Can_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100> 
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:57>
 */
/*============================================================================*/

	
/*=======[I N C L U D E S]====================================================*/
#include "Can.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N   I N F O R M A T I O N]===============================*/
#define CAN_CFG_C_AR_MAJOR_VERSION     2U  
#define CAN_CFG_C_AR_MINOR_VERSION     4U 
#define CAN_CFG_C_AR_PATCH_VERSION     0U 
#define CAN_CFG_C_SW_MAJOR_VERSION     1U
#define CAN_CFG_C_SW_MINOR_VERSION     0U
#define CAN_CFG_C_SW_PATCH_VERSION     0U 

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (CAN_CFG_C_AR_MAJOR_VERSION != CAN_CFG_H_AR_MAJOR_VERSION)
    #error "Can_Cfg.c:Mismatch in Specification Major Version"
#endif 
#if (CAN_CFG_C_AR_MINOR_VERSION != CAN_CFG_H_AR_MINOR_VERSION)
    #error "Can_Cfg.c:Mismatch in Specification Minor Version"
#endif
#if (CAN_CFG_C_AR_PATCH_VERSION != CAN_CFG_H_AR_PATCH_VERSION)
    #error "Can_Cfg.c:Mismatch in Specification Patch Version"
#endif
#if (CAN_CFG_C_SW_MAJOR_VERSION != CAN_CFG_H_SW_MAJOR_VERSION)
    #error "Can_Cfg.c:Mismatch in Specification Major Version"
#endif
#if (CAN_CFG_C_SW_MINOR_VERSION != CAN_CFG_H_SW_MINOR_VERSION)
    #error "Can_Cfg.c:Mismatch in Specification Minor Version"
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CAN_START_CONST_PBCFG
#include "Can_MemMap.h"
/* HRHs first,HTH next*/
/* Controller ID must be grouped together */
/* Can ID to ensure top priority IDs are first */
CONST(Can_HardwareObjectType, CAN_CONST_PBCFG) Can_HardwareObjectConfigData[CAN_MAX_HARDWAREOBJECTS] = 
{
    {
        CAN_HANDLE_TYPE_BASIC,      /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x7d0,                      /* CanIdValue */ 
        0,                          /* CanObjectId */
        CAN_OBJECT_TYPE_RECEIVE,    /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0x7d0                       /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_BASIC,      /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x390,                      /* CanIdValue */ 
        1,                          /* CanObjectId */
        CAN_OBJECT_TYPE_RECEIVE,    /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0x390                       /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_FULL,       /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x2a1,                      /* CanIdValue */ 
        2,                          /* CanObjectId */
        CAN_OBJECT_TYPE_RECEIVE,    /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0xFFFF                      /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_FULL,       /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x1ca,                      /* CanIdValue */ 
        3,                          /* CanObjectId */
        CAN_OBJECT_TYPE_RECEIVE,    /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0xFFFF                      /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_BASIC,      /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x0,                        /* CanIdValue */ 
        4,                          /* CanObjectId */
        CAN_OBJECT_TYPE_TRANSMIT,   /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0xFFFF                      /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_FULL,       /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x7e1,                      /* CanIdValue */ 
        5,                          /* CanObjectId */
        CAN_OBJECT_TYPE_TRANSMIT,   /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0xFFFFFFFF                  /* CanFilterMask */
    },
    {
        CAN_HANDLE_TYPE_FULL,       /* CanHandleType */
        CAN_ID_TYPE_STANDARD,       /* CanIdType*/
        0x2a6,                      /* CanIdValue */ 
        6,                          /* CanObjectId */
        CAN_OBJECT_TYPE_TRANSMIT,   /* CanObjectType */  
        CAN_CONTROLLER_0,           /* CanControllerRef */
        0xFFFFFFFF                  /* CanFilterMask */
    },
};

CONST(uint16, CAN_CONST_PBCFG) Can_HohConfigData[CAN_MAX_HOHS] = 
{0,1,2,3,4,5,6};

CONST(Can_ControllerConfigType, CAN_CONST_PBCFG) Can_ControllerConfigData[CAN_MAX_CONTROLLERS] =
{
    {
        0x00U,                                  /* Btr0 */
        0x3aU,                                  /* Btr1 */
        0x10,                                   /* CanFilterType */
        0,                                      /* CanRxHwObjFirst */
        4,                                      /* CanRxHwObjCount */
        4,                                      /* CanTxHwObjFirst */
        3,                                      /* CanTxHwObjCount */
    },
};
#define CAN_STOP_CONST_PBCFG
#include "Can_MemMap.h"

#define CAN_START_SEC_CONST_UNSPECIFIED
#include "Can_MemMap.h"
CONST(Can_ControllerPCConfigType, CAN_CONST) Can_ControllerPCConfigData[CAN_MAX_CONTROLLERS] = 
{
    {
        0x00000140U,                           /* CanControllerBaseAddress*/
        CAN_CONTROLLER_0,                      /* CanControllerId */
        CAN_PROCESS_TYPE_INTERRUPT,            /* CanBusOffProcessing */
        CAN_PROCESS_TYPE_INTERRUPT,            /* CanRxProcessing */
        CAN_PROCESS_TYPE_INTERRUPT,            /* CanTxProcessing */
        #if(STD_ON == CAN_WAKEUP_SUPPORT)
        CAN_PROCESS_TYPE_INTERRUPT,            /* CanWakeupProcessing */
        0x0,                                   /* CanWakeupSourceRef */
        #endif                                 /* #if(STD_ON == CAN_WAKEUP_SUPPORT) */
        8000000U,                              /* CanCpuClock */
	    	CAN_CLOCKSRC_TYPE_OSC,                 /* CanClockSource */
    },
};
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include "Can_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/