/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <PduR_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:	>
 *  
 *  @author     <>
 *  @date       <2015-04-01 15:26:08> 
 */
/*============================================================================*/


/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define PDUR_CFG_C_AR_MAJOR_VERSION  2
#define PDUR_CFG_C_AR_MINOR_VERSION  3
#define PDUR_CFG_C_AR_PATCH_VERSION  0
#define PDUR_CFG_C_SW_MAJOR_VERSION  1
#define PDUR_CFG_C_SW_MINOR_VERSION  0
#define PDUR_CFG_C_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "PduR.h"

#if(STD_ON == PDUR_CANIF_SUPPORT)
#include "CanIfDiag.h"
#include "CanIfDiag_Cbk.h"
#endif
#if(STD_ON == PDUR_LINIF_SUPPORT)
#include "LinIf.h"
#include "LinIf_Cbk.h"
#endif
#if(STD_ON == PDUR_COM_SUPPORT)
#include "Com.h"
#include "Com_Cbk.h"
#endif

#if(STD_ON == PDUR_IPDUM_SUPPORT)
#include "IpduM.h"
#endif

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (PDUR_CFG_C_AR_MAJOR_VERSION != PDUR_CFG_H_AR_MAJOR_VERSION)
  #error "PDUR_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_AR_MINOR_VERSION != PDUR_CFG_H_AR_MINOR_VERSION)
  #error "PDUR_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_AR_PATCH_VERSION != PDUR_CFG_H_AR_PATCH_VERSION)
  #error "PDUR_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_SW_MAJOR_VERSION != PDUR_CFG_H_SW_MAJOR_VERSION)
  #error "PDUR_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_SW_MINOR_VERSION != PDUR_CFG_H_SW_MINOR_VERSION)
  #error "PDUR_Cfg.c : Mismatch in Specification Major Version"
#endif

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define PDUR_START_SEC_CONST_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRLoIfTransmitType, CANIF_CONST) PduRLoIf_Transmit[PDUR_MAX_BUSTYPE_NUMBER] =
{
#if(STD_ON == PDUR_CANIF_SUPPORT)
    CanIf_Transmit,
#else
    NULL_PTR,
#endif
#if(STD_ON == PDUR_LINIF_SUPPORT)
    LinIf_Transmit,
#else
    NULL_PTR,
#endif
#if(STD_ON == PDUR_IPDUM_SUPPORT)
    IpduM_Transmit
#else
    NULL_PTR
#endif
};
#define PDUR_STOP_SEC_CONST_UNSPECIFIED
#include "PduR_MemMap.h"


#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(PduR_ComDestTxPduType, PDUR_CONST) PduRComDestTxPdu[PDUR_MAX_COMDESTTXPDU_NUMBER] =
{
	{
		/* DestId */
		0,
		PDUR_CAN
	},
	{
		1,
		PDUR_CAN
	},
	{
		/* DestId */
		0,
		PDUR_LIN
	},
	{
		1,
		PDUR_LIN
	},
	{
		/* DestId */
		0,
		PDUR_IPDUML
	},
	{
		1,
		PDUR_IPDUML
	}
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(PduR_IfConfPduType, PDUR_CONST) PduRIfConfPdu[PDUR_MAX_IFCONFPDU_NUMBER] =
{
	{
		/* ConfId */
		0,
		PDUR_COM
	},
	{
		1,
		PDUR_COM
	},
	{
		/* ConfId */
		2,
		PDUR_COM
	},
	{
		3,
		PDUR_COM
	},
	{
		/* DestId */
		0,
		PDUR_IPDUMU
	},
	{
		1,
		PDUR_IPDUMU
	},
	{
		/* DestId */
		0,
		PDUR_GATEWAY
	},
	{
		1,
		PDUR_GATEWAY
	}
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#if(STD_ON == PDUR_GATEWAY_OPERATION)
#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(PduR_GWRoutingTableType, PDUR_CONST) PduRGWDestTxPdu[PDUR_MAX_GWDESTTXPDU_NUMBER] =
{
	{
		/* DestId */
		0,
		/* BusType */
		PDUR_CAN,
		/* Start as PduRGWTxBuffer in Location */
		0,
		/* Depth */
		0,
		/* Length */
		8,
		/* DefaultValue */
		0
	},
	{
		/* DestId */
		2,
		/* BusType */
		PDUR_CAN,
		/* Start */
		1,
		/* Depth */
		2,
		/* Length */
		8,
		/* DefaultValue */
		0
	},
	{
		/* DestId */
		0,
		/* BusType */
		PDUR_LIN,
		/* Start */
		2,
		/* Depth */
		0,
		/* Length */
		8,
		/* DefaultValue */
		0
	}
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"


#endif

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(PduR_IfDestRxPduType, PDUR_CONST) PduRIfDestRxPdu[PDUR_MAX_IFDESTRXPDU_NUMBER] =
{
	{
		/* DestId */
		0,
		PDUR_COM
	},
	{
		1,
		PDUR_COM
	},
	{
		/* DestId */
		2,
		PDUR_COM
	},
	{
		3,
		PDUR_COM
	},
	{
		/* DestId */
		0,
		PDUR_IPDUMU
	},
	{
		1,
		PDUR_IPDUMU
	},
	{
		/* DestId */
		0,
		PDUR_GATEWAY
	},
	{
		1,
		PDUR_GATEWAY
	}
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(PduR_IfSrcRxPduType, PDUR_CONST) PduRIfSrcRxPdu[PDUR_MAX_IFSRCRXPDU_NUMBER] =
{
	{
		/* StartId */
		0,
		0
	},
	{
		1,
		1
	},
	{
		/* StartId */
		2,
		2
	},
	{
		3,
		4
	},
	{
		/* StartId */
		5,
		6
	}
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

#define PDUR_START_CONST_PBCFG
#include "PduR_MemMap.h"
CONST(uint16, PDUR_CONST) PduRIpduMRxPdu[PDUR_MAX_IPDUMRXPDU_NUMBER] =
{
	2,
	3,
	4
};
#define PDUR_STOP_CONST_PBCFG
#include "PduR_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/

