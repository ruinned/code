/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:58> 
 */
/*============================================================================*/


/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/

#define CANIF_CFG_C_AR_MAJOR_VERSION  3
#define CANIF_CFG_C_AR_MINOR_VERSION  2
#define CANIF_CFG_C_AR_PATCH_VERSION  0
#define CANIF_CFG_C_SW_MAJOR_VERSION  1
#define CANIF_CFG_C_SW_MINOR_VERSION  0
#define CANIF_CFG_C_SW_PATCH_VERSION  8

/*=======[I N C L U D E S]====================================================*/
#include "CanIfDiag.h"
#include "Can.h"

#include "CanSM_Cbk.h"
#include "CanTp_Cbk.h"
#include "App_Com.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if(CANIF_CFG_C_AR_MAJOR_VERSION != CANIF_CFG_H_AR_MAJOR_VERSION)
    #error "CanIfDiag.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_CFG_C_AR_MINOR_VERSION != CANIF_CFG_H_AR_MINOR_VERSION)
    #error "CanIfDiag.c:Mismatch in Specification Minor Version"
#endif

#if(CANIF_CFG_C_AR_PATCH_VERSION != CANIF_CFG_H_AR_PATCH_VERSION)
    #error "CanIfDiag.c:Mismatch in Specification Patch Version"
#endif

#if(CANIF_CFG_C_SW_MAJOR_VERSION != CANIF_CFG_H_SW_MAJOR_VERSION)
    #error "CanIfDiag.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_CFG_C_SW_MINOR_VERSION != CANIF_CFG_H_SW_MINOR_VERSION)
    #error "CanIfDiag.c:Mismatch in Specification Minor Version"
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
P2VAR(CanIf_RxPduBufferType,AUTOMATIC,CANIF_VAR) CanIf_RxBuffers;
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfUserRxFct,CANIF_CONST) CanIf_UserRxIndication[CANIF_USER_MAX_COUNT] =
{
	NULL_PTR,/*&CanNm_RxIndication,Enable only when CanNm include*/
    &CanTp_RxIndication,/*&CanTp_RxIndication,Enable only when CanTp include*/
    NULL_PTR,/*&PduR_CanIfRxIndication,Enable only when PduR include*/
	NULL_PTR,/**<&Xcp_CanIfRxIndication,XCP */
    NULL_PTR, /**<&OsekNm_RxIndication,OskeNm*/
    &App_RxIndication      /**<&App_RxIndication, APP*/

};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfUserTxFct,CANIF_CONST) CanIf_UserTxConfirmation[CANIF_USER_MAX_COUNT] =
{
    NULL_PTR,/*&CanNm_TxConfirmation,Enable only when CanNm include*/
    &CanTp_TxConfirmation,/*&CanTp_TxConfirmation,Enable only when CanTp include*/
    NULL_PTR,/*&PduR_CanIfTxConfirmation,Enable only when PduR include*/
    NULL_PTR,/**<&Xcp_CanIfTxConfirmation,XCP */
    NULL_PTR, /**<&OsekNm_TxConfirmation,OsekNm*/
    &App_TxConfirmation       /**<&App_TxConfirmation, APP*/
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/* contains the references  to the configuration setup of each underlying CAN driver.*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_ControllerConfigType,CANIF_CONST) CanIf_ControllerConfiguration[CANIF_MAX_CONTROLLER] =
{
    {
        /* Enables wakeup support and defines the source device of a wakeup event. */
        CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
        /* Logical handle of the underlying CAN controller  */
        CAN_CONTROLLER_0,
	#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
        /* Logical handle of the underlying CAN Tranceiver,add by isoft */ 
        0xFF,
    #endif
	#if (STD_ON == CANIF_WAKEUP_VALIDATION)
        /* CanIf WakeUp source,add by isoft */
        0x0
	#endif
	  
	},
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/* Callout functions with respect to the upper layers */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_DispatchConfigType,CANIF_CONST) CanIf_DispatchConfigData = 
{
    /* Name of target BusOff notification services  */
	&CanSM_ControllerBusOff,/*&CanSM_ControllerBusOff,Enable only when CanSM include*/
	
    #if (STD_ON == CANIF_WAKEUP_VALIDATION)
	/*  Name of target wakeup notification services */
	NULL_PTR,/*&EcuM_SetWakeupEvent, Enable only when EcuM include */

	/* Name of target wakeup validation notification services */
	NULL_PTR /*&EcuM_ValidateWakeupEvent, Enable only when EcuM include */
    #endif 
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/* Configuration parameters for all the underlying CAN drivers*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_DriverConfigType,CANIF_CONST) CanIf_DriverConfiguration =
{
    TRUE, /* CanIfBusoffNotification */
    TRUE, /* CanIfReceiveIndication */
    TRUE, /* CanIfTxConfirmation */
    FALSE  /* CanIfWakeupNotification */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/* contains configuration parameters for each hardware receive object */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_HrhConfigType,CANIF_CONST) CanIf_HrhConfigData[CANIF_NUMBER_OF_HRHUSED] =
{
	{
	    /* Define s the HRH type i.e, whether its a BasicCan or FullCan */
	    CANIF_BASIC_CAN,

		/* perform software filtering */
		TRUE,
		
		/* Reference to controller Id to which the HRH belongs to */
		CAN_CONTROLLER_0
	},
	{
	    /* Define s the HRH type i.e, whether its a BasicCan or FullCan */
	    CANIF_BASIC_CAN,

		/* perform software filtering */
		TRUE,
		
		/* Reference to controller Id to which the HRH belongs to */
		CAN_CONTROLLER_0
	},
		{
	    /* Define s the HRH type i.e, whether its a BasicCan or FullCan */
	    CANIF_FULL_CAN,

		/* perform software filtering */
		FALSE,
		
		/* Reference to controller Id to which the HRH belongs to */
		CAN_CONTROLLER_0
	},
	{
	    /* Define s the HRH type i.e, whether its a BasicCan or FullCan */
	    CANIF_FULL_CAN,

		/* perform software filtering */
		FALSE,
		
		/* Reference to controller Id to which the HRH belongs to */
		CAN_CONTROLLER_0
	},
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/* contains parameters related to each HTH */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_HthConfigType,CANIF_CONST) CanIf_HthConfigData[CANIF_NUMBER_OF_HTHUSED] =
{
    {
        /*Transmission method of the corresponding HTH */
        CANIF_BASIC_CAN,

        /* Reference to controller Id to which the HTH belongs to */
        CAN_CONTROLLER_0,

        /* refers to a particular HTH object in the CAN Driver Module configuration*/
        4
    },
    {
        /*Transmission method of the corresponding HTH */
        CANIF_FULL_CAN,

        /* Reference to controller Id to which the HTH belongs to */
        CAN_CONTROLLER_0,

        /* refers to a particular HTH object in the CAN Driver Module configuration*/
        5
    },
    {
        /*Transmission method of the corresponding HTH */
        CANIF_FULL_CAN,

        /* Reference to controller Id to which the HTH belongs to */
        CAN_CONTROLLER_0,

        /* refers to a particular HTH object in the CAN Driver Module configuration*/
        6
    },
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_TxNotifStatus[CANIF_MAX_NUMBER_OF_CANTXPDUIDS];
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_RxNotifStatus[CANIF_MAX_NUMBER_OF_CANRXPDUIDS];
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"

/* define dynamic tx pdu Canid */
#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
P2VAR(Can_IdType,AUTOMATIC,CANIF_VAR) CanIf_DynamicTxPduCanIds;
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"


/* references to the configuration setup of each underlying CAN Driver */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_InitHohConfigType, CANIF_CONST_PBCFG) CanIf_InitHohConfig =
{  
    /* Selects the CAN Interface specific configuration setup */
    &Can_ControllerConfigData[0u]
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"


/* contains the configuration (parameters) of each transmit CAN L-PDU */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_TxPduConfigType, CANIF_CONST_PBCFG) CanIf_TxPduConfigData[CANIF_MAX_NUMBER_OF_CANTXPDUIDS] =
{
	{  /* Dcm_Tx */
		0x7e1,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		0,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_CAN_TP, /* CanIfTxUserType*/
		1        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_0 */
		0x2a6,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		1,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		2        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_1 */
		0x37f,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		2,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_2 */
		0x3bf,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		3,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_3 */
		0x36f,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		4,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_4 */
		0x377,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		5,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_5 */
		0x3a9,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		6,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_6 */
		0x3be,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		7,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_7 */
		0x38d,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		8,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_8 */
		0x382,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		9,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_9 */
		0x6c2,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		10,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
	{  /* TxPdu_10 */
		0x372,      /* CanIfCanTxPduIdCanId*/
		0,          /* CanIfCanTxPduIdDlc*/
		11,			/* CanIfCanTxPduIndex*/
		CANIF_PDU_TYPE_STATIC,  /* CanIfCanTxPduType*/
		0xFFFFu,    /* CanIfDynamicTxPduCanIdIndex*/
		FALSE,       /* CanIfReadTxPduNotifyStatus*/
    0xFF,       /* CanIfTxNotifyIndex*/
    CANIF_STANDARD_CAN,     /* CanIfTxPduIdCanIdType */
		CANIF_USER_TYPE_APP, /* CanIfTxUserType*/
		0        /* CanIfCanTxPduHthRef*/
  },
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"


/* contains the configuration (parameters) of each receive CAN L-PDU */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_RxPduConfigType, CANIF_CONST_PBCFG)CanIf_RxPduConfigData[CANIF_MAX_NUMBER_OF_CANRXPDUIDS] =
{
	{  /* RxPdu_3 */
		0x1ca,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		0,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_APP   /* CanIfRxUserType*/
	
    },
	{  /* RxPdu_0 */
		0x39c,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		1,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_APP   /* CanIfRxUserType*/
	
    },
	{  /* RxPdu_1 */
		0x39e,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		2,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_APP   /* CanIfRxUserType*/
	
    },
	{  /* Dcm_Phy */
	  0x7d2,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		3,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_CAN_TP   /* CanIfRxUserType*/
	
    },
	{  /* Dcm_Func */
	0x7df,//	0x7d1,//0x7df,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		4,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_CAN_TP   /* CanIfRxUserType*/
	
    },
	{  /* RxPdu_2 */
		0x2a1,      /* CanIfCanRxPduCanId*/
		8,         /* CanIfCanRxPduDlc*/
		3,         /* CanIfCanRxPduId*/
		FALSE,          /* CanIfReadRxPduData*/
    0xFF,             /* CanIfRxBufferIndex */
		FALSE,         /* CanIfReadRxPduNotifyStatus*/	
		0xFF,             /* CanIfRxNotifyIndex */
		CANIF_STANDARD_CAN,      /* CanIfRxPduIdCanIdType*/
		CANIF_USER_TYPE_APP   /* CanIfRxUserType*/
	
    },
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

/* define hth index ,every Hth used as BASIC CAN have a index cfg 
 * The Max size determined at link time, equal to CanIf_HthConfigData size.
 */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_HohIndexType, CANIF_CONST_PBCFG) CanIf_HthIndexCfg[CANIF_NUMBER_OF_HTHUSED] = 
{
    {
        2,/* start index */
        11 /* stop index */
    },
    {
        0,/* start index */
        0 /* stop index */
    },
    {
        1,/* start index */
        1 /* stop index */
    },
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"


/* every Hrh used as BASIC CAN and 'CanIfSoftwareFilterHrh' Configured TRUE have a RangeMask */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_HrhRangeMaskType, CANIF_CONST_PBCFG) CanIf_HrhRangeMaskConfig[CANIF_NUMBER_OF_HRHUSED] =
{
    {
        0x7d0, 
        0x7d0      
    },
    {
        0x390, 
        0x390      
    }, 
    {
        0x7ff, 
        0x2a1      
    }, 
    {
        0x7ff, 
        0x1ca      
    },
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"


#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_HohIndexType, CANIF_CONST_PBCFG) CanIf_HrhPduIndex[CANIF_NUMBER_OF_HRHUSED] = 
{
    {
        3,/* start index */
        4 /* stop index */
    },
    {
        1,/* start index */
        2 /* stop index */
    },
    {
        5,/* start index */
        5 /* stop index */
    },
    {
        0,/* start index */
        0 /* stop index */
    },
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"


/* each CanIfHrhConfig have a corresponding HrhIdConfig which contains the HRH */
#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(uint8, CANIF_CONST_PBCFG) CanIf_HrhIdConfig[CANIF_NUMBER_OF_HRHUSED] =
{
    0, 
    1, 
    2, 
    3,
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_CONST_PBCFG
#include "CanIf_MemMap.h"
CONST(CanIf_HrhFilterConfigType, CANIF_CONST_PBCFG) CanIf_HrhFilterRefCfg =
{
    &CanIf_HrhIdConfig[0u],
    &CanIf_HrhRangeMaskConfig[0u],
    &CanIf_HrhPduIndex[0u]
};
#define CANIF_STOP_CONST_PBCFG
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(uint16,CANIF_VAR)CanIf_TxBufferUsedCount[CANIF_NUMBER_OF_HTHUSED];
#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "CanIf_MemMap.h"


/*=======[E N D   O F   F I L E]==============================================*/