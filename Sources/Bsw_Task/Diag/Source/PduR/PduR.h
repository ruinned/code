/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <PduR.h>
 *  @brief      <Declear the Headfiles of PduR Module>
 *
 *  <Compiler:      MCU:  >
 *
 *  @author     <Huanyu.Zhao>
 *  @date       <01-04-2015>
 */
/*============================================================================*/
#ifndef  PDUR_H 
#define  PDUR_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0     20150401  Huanyu.Zhao Initial version
 *
 *
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define PDUR_VENDOR_ID               62U
#define PDUR_MODULE_ID               51U
#define PDUR_INSTANCE_ID             0U
#define PDUR_H_AR_MAJOR_VERSION      2U
#define PDUR_H_AR_MINOR_VERSION      3U
#define PDUR_H_AR_PATCH_VERSION      0U
#define PDUR_H_SW_MAJOR_VERSION      1U
#define PDUR_H_SW_MINOR_VERSION      0U
#define PDUR_H_SW_PATCH_VERSION      0U

/*=======[I N C L U D E S]====================================================*/
#include "PduR_Types.h"
#include "PduR_Cfg.h"

#if(PDUR_VARIANT_PB== PDUR_VARIANT_CFG)
#include "PDUR_PBcfg.h"
#endif

/*=======[M A C R O S]========================================================*/
#if(STD_ON == PDUR_DEV_ERROR_DETECT)
/* General function id */
#define PDUR_INIT_ID                       	  	((uint8)0x00U)
#define PDUR_GETVERSIONINFO_ID               	  ((uint8)0x17U)
#define PDUR_GETCONFIGURATIONID_ID              ((uint8)0x18U)
#define PDUR_CANCELTRANSMITREQUEST_ID           ((uint8)0x1cU)
#define PDUR_CHANGEPARAMETERREQUEST_ID          ((uint8)0x1DU)
/* Canif or CanTP function id */
#define PDUR_CANIFRXINDICATION_ID               ((uint8)0x01U)
#define PDUR_CANIFTXCONFIRMATION_ID             ((uint8)0x02U)
#define PDUR_CANTPPROVIDERXBUFFER_ID            ((uint8)0x03U)
#define PDUR_CANTPRXINDICATION_ID               ((uint8)0x04U)
#define PDUR_CANTPPROVIDETXBUFFER_ID            ((uint8)0x05U)
#define PDUR_CANTPTXCONFIRMATION_ID             ((uint8)0x06U)
/* Linif or LinTP function id */
#define PDUR_LINIFRXINDICATION_ID               ((uint8)0x0eU)
#define PDUR_LINIFTXCONFIRMATION_ID             ((uint8)0x0fU)
#define PDUR_LINIFTRIGGERTRANSMIT_ID            ((uint8)0x10U)
#define PDUR_LINTPPROVIDERXBUFFER_ID            ((uint8)0x11U)
#define PDUR_LINTPRXINDICATION_ID               ((uint8)0x12U)
#define PDUR_LINTPPROVIDETXBUFFER_ID            ((uint8)0x13U)
#define PDUR_LINTPTXCONFIRMATION_ID            	((uint8)0x14U)
/* Com function id */
#define PDUR_COMTRANSMIT_ID            			((uint8)0x15U)
/* Dcm function id */
#define PDUR_DCMTRANSMIT_ID            			((uint8)0x16U)
/* IPDUM function id */
#define PDUR_IPDUMTRANSMIT_ID            		((uint8)0x19U)
#define PDUR_IPDUMTXCONFIRMATION_ID            	((uint8)0x1aU)
#define PDUR_IPDUMRXINDICATION_ID            	((uint8)0x1bU)

/* Error Classification */
#define PDUR_E_CONFIG_PTR_INVALID             ((uint8)0x00U)
#define PDUR_E_INVALID_REQUEST	             ((uint8)0x01U)
#define PDUR_E_PDU_ID_INVALID             	 ((uint8)0x02U)
#define PDUR_E_TP_TX_REQ_REJECTED            ((uint8)0x03U)
#define PDUR_E_DATA_PTR_INVALID              ((uint8)0x05U)

#endif

/*==========[M A C R O  F U N C T I O N S]====================================*/
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versionInfo:Pointer to where to store the version
 * 						information of this module.
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           By RTE
 */
/******************************************************************************/
/* @PDUR234 @PDUR339 */
#if (STD_ON == PDUR_VERSION_INFO_API)
	/* @PDUR340 */
	#if (STD_ON == PDUR_DEV_ERROR_DETECT)
	#define PDUR_GetVersionInfo(VersionInfo) \
	    do{\
	        if (NULL_PTR == (VersionInfo))\
	        {\
	            Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_GETVERSIONINFO_ID, PDUR_E_CONFIG_PTR_INVALID);\
	        }\
	        else \
	        {\
	            (VersionInfo)->vendorID = PDUR_VENDOR_ID; \
	            (VersionInfo)->moduleID = PDUR_MODULE_ID; \
	            (VersionInfo)->instanceID = PDUR_INSTANCE_ID; \
	            (VersionInfo)->sw_major_version = PDUR_H_SW_MAJOR_VERSION; \
	            (VersionInfo)->sw_minor_version = PDUR_H_SW_MINOR_VERSION; \
	            (VersionInfo)->sw_patch_version = PDUR_H_SW_PATCH_VERSION; \
	        }\
	    }while(0)
	#else
	#define PDUR_GetVersionInfo(VersionInfo) \
	    do{\
	          (VersionInfo)->vendorID = PDUR_VENDOR_ID; \
	          (VersionInfo)->moduleID = PDUR_MODULE_ID; \
	          (VersionInfo)->instanceID = PDUR_INSTANCE_ID; \
	          (VersionInfo)->sw_major_version = PDUR_H_SW_MAJOR_VERSION; \
	          (VersionInfo)->sw_minor_version = PDUR_H_SW_MINOR_VERSION; \
	          (VersionInfo)->sw_patch_version = PDUR_H_SW_PATCH_VERSION; \
	    }while(0)
	#endif
#endif

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define PDUR_START_SEC_CONST_UNSPECIFIED
#include "PduR_MemMap.h"
extern CONST(PduRLoIfTransmitType, CANIF_CONST) PduRLoIf_Transmit[PDUR_MAX_BUSTYPE_NUMBER];
#define PDUR_STOP_SEC_CONST_UNSPECIFIED
#include "PduR_MemMap.h"

#if(PDUR_VARIANT_PC== PDUR_VARIANT_CFG)
#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(PduR_ComDestTxPduType, PDUR_CONST) PduRComDestTxPdu[PDUR_MAX_COMDESTTXPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(PduR_IfConfPduType, PDUR_CONST) PduRIfConfPdu[PDUR_MAX_IFCONFPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#if(STD_ON == PDUR_GATEWAY_OPERATION)
#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(PduR_GWRoutingTableType, PDUR_CONST) PduRGWDestTxPdu[PDUR_MAX_GWDESTTXPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(PduR_IfDestRxPduType, PDUR_CONST) PduRIfDestRxPdu[PDUR_MAX_IFDESTRXPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(PduR_IfSrcRxPduType, PDUR_CONST) PduRIfSrcRxPdu[PDUR_MAX_IFSRCRXPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#if(STD_ON == PDUR_IPDUM_SUPPORT)
#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
extern CONST(uint16, PDUR_CONST) PduRIpduMRxPdu[PDUR_MAX_IPDUMRXPDU_NUMBER];
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"
#endif

#endif /* #if(STD_ON == PDUR_GATEWAY_OPERATION) */

#endif /* #if(PDUR_VARIANT_PC== PDUR_VARIANT_CFG) */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if(STD_ON == PDUR_ZERO_COST_OPERATION)

/* @PDUR335 */
#define PduR_Init(ConfigPtr)

/* @PDUR342 */
#define PduR_GetConfigurationId() 0



#else
/******************************************************************************/
/*
 * Brief               Initializes the PDU Router
 *
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to Post build configuration data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern FUNC(void, PDUR_CODE)
PduR_Init(P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_CONST_PBCFG) ConfigPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"




/******************************************************************************/
/*
 * Brief               Returns the unique identifier of the Post build time
 * 						configuration of the PDU Router.
 *
 * ServiceId           0x18
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint32 Identifier of the post build time configuration.
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern FUNC(uint32, PDUR_CODE)
PduR_GetConfigurationId(void);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/******************************************************************************/
/*
 * Brief				This service primitive is used to cancel the transfer
 * 						of pending I-PDUs. This function has to be called with
 * 						the PDU-Id and the reason for cancellation.
 *
 * ServiceId           0x1c
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      PduCancelReason:The reason for cancellation
 * 					   PduId:This parameter contains the unique identifier of
 * 					   the I-PDU which transfer has to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return			   std_ReturnType :E_OK Cancellation request of the transfer
 * 						of the specified I-PDU is accepted.
 * 						E_NOT_OK:Cancellation request of the transfer of the
 * 						specified I-PDU is rejected.
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_CancelTransmitRequest(PduR_CancelReasonType PduCancelReason, PduIdType PduId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/******************************************************************************/
/*
 * Brief               This Service primitive is used to request the change of
 * 						the value of the PDUR_STMIN parameter.the new value is
 * 						given by PduParameterValue.
 *
 * ServiceId           0x1d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint32 Identifier of the post build time configuration.
 * PreCondition        None
 * CallByAPI
 */
/******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern FUNC(uint32, PDUR_CODE)
PduR_ChangeParameterRequest(PduR_ParameterValueType PduParameterValue, PduIdType PduId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#endif  /* end of PDUR_H */

/*=======[E N D   O F   F I L E]==============================================*/

