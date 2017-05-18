/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/
#ifndef CANSM_H
#define CANSM_H

/* @req CANSM008 */
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_MODULE_ID           140U
#define CANSM_VENDOR_ID           62U
#define CANSM_H_AR_MAJOR_VERSION  1U
#define CANSM_H_AR_MINOR_VERSION  2U
#define CANSM_H_AR_PATCH_VERSION  0U
#define CANSM_H_SW_MAJOR_VERSION  1U
#define CANSM_H_SW_MINOR_VERSION  0U
#define CANSM_H_SW_PATCH_VERSION  0U
/*=======[I N C L U D E S]====================================================*/
/* @req CANSM238 */
#include "ComStack_Types.h"
/* @req CANSM239 */
#include "CanSM_Cfg.h"
#if((CANSM_VARIANT_LINK_TIME == CANSM_VARIANT_CLASS)||(CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS))
#include "CanSM_LCfg.h"
#endif
#include "CanSM_EcuM.h"
#include "CanSM_Types.h"
#if (STD_ON == CANSM_USE_COM)
#include "Com.h"
#endif
#include "Std_ExtendedTypes.h"

/*=======[M A C R O S]========================================================*/
#define    CANSM_INSTANCE_ID                    (0U)
#if (STD_ON == CANSM_CONFIGURATION_USE_DET)
/* error code from specification */
/* @req CANSM069 */
#define    CANSM_E_UNINIT                       0x01
#define    CANSM_E_PARAM_POINTER                0x02
#define    CANSM_E_INVALID_NETWORK_HANDLE       0x03
#define    CANSM_E_INVALID_NETWORK_MODE         0x04
#define    CANSM_E_INVALID_RETURNVALUE          0x05
#define    CANSM_E_PARAM_CONTROLLER             0x06

/* Service IDs */
#define    SERVICE_ID_CANSM_INIT                0x00
#define    SERVICE_ID_CANSM_REQUESTCOMMODE      0x02
#define    SERVICE_ID_CANSM_GETCURRENTCOMMODE   0x03
#define    SERVICE_ID_CANSM_CONTROLLERBUSOFF    0x04
#define    SERVICE_ID_CANSM_MAINFUNCTION        0x05
#define    SERVICE_ID_CANSM_GETVERSIONINFO      0x01
#endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*************************************************************************/
/*
 * Brief               This service returns the version information of
 *                     this module
 * ServiceId           <SERVICE_ID_CANSM_GETVERSIONINFO>
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Versioninfo: Pointer to where to store the version
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* @req CANSM024 @req CANSM180 */
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
#if (STD_ON == CANSM_CONFIGURATION_USE_DET)
#define CanSM_GetVersionInfo(VersionInfo) \
    do{\
        if (NULL_PTR == (VersionInfo))\
        { \
     	    Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_GETVERSIONINFO, CANSM_E_PARAM_POINTER);\
        }\
        else\
    	{\
        	(VersionInfo)->vendorID = CANSM_VENDOR_ID; \
        	(VersionInfo)->moduleID = CANSM_MODULE_ID; \
        	(VersionInfo)->instanceID = 0u; \
        	(VersionInfo)->sw_major_version = CANSM_H_SW_MAJOR_VERSION; \
        	(VersionInfo)->sw_minor_version = CANSM_H_SW_MINOR_VERSION; \
        	(VersionInfo)->sw_patch_version = CANSM_H_SW_PATCH_VERSION; \
    	}\
    }while(0)
#else /* STD_OFF == CANSM_CONFIGURATION_USE_DET */
#define CanSM_GetVersionInfo(VersionInfo) \
    do{\
        	(VersionInfo)->vendorID = CANSM_VENDOR_ID; \
        	(VersionInfo)->moduleID = CANSM_MODULE_ID; \
        	(VersionInfo)->instanceID = 0u; \
        	(VersionInfo)->sw_major_version = CANSM_H_SW_MAJOR_VERSION; \
        	(VersionInfo)->sw_minor_version = CANSM_H_SW_MINOR_VERSION; \
        	(VersionInfo)->sw_patch_version = CANSM_H_SW_PATCH_VERSION; \
    }while(0)
#endif /* STD_ON == CANSM_VERSION_INFO_API */
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/* pre-compile-only network configuration array */
#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
extern CONST(CanSM_PCNetworkType, CANSM_CONST)CanSM_PCNetwork[CANSM_NETWORK_NUM];
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"

#if (CANSM_VARIANT_CLASS == CANSM_VARIANT_PRE_COMPILE)
/* pre-compile network controller configuration array */
#define CANSM_START_CONST_PBCFG
#include "CanSM_MemMap.h"
extern CONST(CanSM_ControllerType, CANSM_CONST_PBCFG) CanSM_Controller[CANSM_NETWORK_NUM];
#define CANSM_STOP_CONST_PBCFG
#include "CanSM_MemMap.h"

/* pre-compile network configuration array */
#define CANSM_START_CONST_PBCFG
#include "CanSM_MemMap.h"
extern CONST(CanSM_NetworkType, CANSM_CONST_PBCFG) CanSM_Network[CANSM_NETWORK_NUM];
#define CANSM_STOP_CONST_PBCFG
#include "CanSM_MemMap.h"

#if (STD_ON == CANSM_USE_COM)
#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
/* TX PDU group id */
extern CONST(Com_PduGroupIdType, CANSM_CONST) CanSM_TxPduGroupId[CANSM_NETWORK_NUM];
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
/* RX PDU group id */
extern CONST(Com_PduGroupIdType, CANSM_CONST) CanSM_RxPduGroupId[CANSM_NETWORK_NUM];
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
#endif
#endif /* CANSM_VARIANT_CLASS == CANSM_VARIANT_PRE_COMPILE */

#endif /* CANSM_H */
/*=======[E N D   O F   F I L E]==============================================*/
