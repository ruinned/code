/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanSM_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100> 
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58>
 */
/*============================================================================*/


/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_CFG_C_AR_MAJOR_VERSION  1
#define CANSM_CFG_C_AR_MINOR_VERSION  2
#define CANSM_CFG_C_AR_PATCH_VERSION  0
#define CANSM_CFG_C_SW_MAJOR_VERSION  1
#define CANSM_CFG_C_SW_MINOR_VERSION  0
#define CANSM_CFG_C_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "CanSM_Cfg.h"
#include "CanSM.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if(CANSM_CFG_C_AR_MAJOR_VERSION != CANSM_CFG_H_AR_MAJOR_VERSION)
    #error "CanSM_cfg.c:Mismatch in Specification Major Version"
#endif

#if(CANSM_CFG_C_AR_MINOR_VERSION != CANSM_CFG_H_AR_MINOR_VERSION)
    #error "CanSM_cfg.c:Mismatch in Specification Minor Version"
#endif

#if(CANSM_CFG_C_AR_PATCH_VERSION != CANSM_CFG_H_AR_PATCH_VERSION)
    #error "CanSM_cfg.c:Mismatch in Specification Patch Version"
#endif

#if(CANSM_CFG_C_SW_MAJOR_VERSION != CANSM_CFG_H_SW_MAJOR_VERSION)
    #error "CanSM_cfg.c:Mismatch in Specification Major Version"
#endif

#if(CANSM_CFG_C_SW_MINOR_VERSION != CANSM_CFG_H_SW_MINOR_VERSION)
    #error "CanSM_cfg.c:Mismatch in Specification Minor Version"
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
CONST(CanSM_PCNetworkType, CANSM_CONST) CanSM_PCNetwork[CANSM_NETWORK_NUM] =
{
		{
            #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING)
            /*
             * whether disables the deadline monitoring of RX PDU
             * Groups during bus-off recovery to avoid timeouts,PC
             */
			FALSE,
            #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */
            #if (STD_ON == CANSM_BOR_TX_CONFIRMATION_POLLING)
		    /*
		     * whether the CanSM polls the CanIf_GetTxConfigurationState
		     * API to decide the bus-off state to be recovered instead of
		     * using the CanSMBorTimeTxEnsured parameter for this decision. PC
		     */
			TRUE,
            #endif /* STD_ON == CANSM_BOR_TX_CONFIRMATION_POLLING */
		    /* the initialize parameter for the configured RX PDU group */
			TRUE,
		    /* the initialize parameter for the configured TX PDU group */
			TRUE
		},
};
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"

#define CANSM_START_CONST_PBCFG
#include "CanSM_MemMap.h"
STATIC CONST(uint8, CANSM_CONST_PBCFG) Controller[CANSM_CONTROLLER_NUM] =
{
	0
};
#define CANSM_STOP_CONST_PBCFG
#include "CanSM_MemMap.h"

#define CANSM_START_CONST_PBCFG
#include "CanSM_MemMap.h"
CONST(CanSM_ControllerType, CANSM_CONST_PBCFG)  CanSM_Controller[CANSM_NETWORK_NUM] =
{
		{
		        /* controller  pointer */
				(uint8*)&Controller[0],
				/* controller  count */
				1
		},
};
#define CANSM_STOP_CONST_PBCFG
#include "CanSM_MemMap.h"

#define CANSM_START_CONST_PBCFG
#include "CanSM_MemMap.h"
CONST(CanSM_NetworkType, CANSM_CONST_PBCFG) CanSM_Network[CANSM_NETWORK_NUM] =
{
		{
           /*
            * bus-off-counter value the bus-off recovery state machine
            * switches from level 1 to level 2,PC,PB,LT
            */
           10,
           /*
            * bus-off-counter value the bus-off recovery state machine
            * shall report a failed production error state to the DEM.
            */
           10,
           /* Bus-off  recovery  level 1 time,PC,PB,LT */
           8,
           /* Bus-off  recovery  level 2 time,PC,PB,LT */
           99,
           /* Bus-off  event  check duration  time,PC,PB,LT */
           0,
           /* Can  network  handle */
           0,
           #if (STD_ON == CANSM_USE_OSEKNM)
           /* NM notification */
           FALSE,
           #endif /* STD_ON == CANSM_USE_OSEKNM */
           #if (STD_ON == CANSM_USE_TRANSCEIVER)
           /* Transceiver id */
           0xFFFF
           #endif /* STD_ON == CANSM_USE_TRANSCEIVER */
		},
};
#define CANSM_STOP_CONST_PBCFG
#include "CanSM_MemMap.h"

/* TX PDU group id */
#if (STD_ON == CANSM_USE_COM)
#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
CONST(Com_PduGroupIdType, CANSM_CONST) CanSM_TxPduGroupId[CANSM_NETWORK_NUM] = 
{
	0
};
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"

/* RX PDU group id */
#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
CONST(Com_PduGroupIdType, CANSM_CONST) CanSM_RxPduGroupId[CANSM_NETWORK_NUM] = 
{
	1
};
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
#endif


/*=======[E N D   O F   F I L E]==============================================*/
