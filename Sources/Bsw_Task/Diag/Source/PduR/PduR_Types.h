/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <PduR_Types.h>
 *  @brief      <Declear the Data_Types of PduR Module>
 *
 *  <Compiler:      MCU:  >
 *
 *  @author     <Huanyu.Zhao>
 *  @date       <01-04-2015>
 */
/*============================================================================*/
#ifndef  PDUR_TYPES_H 
#define  PDUR_TYPES_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0     2015041  Huanyu.Zhao Initial version
 *
 *
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define PDUR_TYPES_H_AR_MAJOR_VERSION  2
#define PDUR_TYPES_H_AR_MINOR_VERSION  3
#define PDUR_TYPES_H_AR_PATCH_VERSION  0
#define PDUR_TYPES_H_SW_MAJOR_VERSION  1
#define PDUR_TYPES_H_SW_MINOR_VERSION  0
#define PDUR_TYPES_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "PduR_Cfg.h"


/*=======[M A C R O S]========================================================*/


/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/*Enumeration Type for State of PDU Router*/
typedef enum
{
    PDUR_UNINIT = 0u, /* not initialized */

    PDUR_ONLINE, /* initialized successfully */

    PDUR_REDUCED /* initialization did not succeed */
} PduR_StateType;

/*Type for cancel reason type*/
typedef enum
{
    PDU_CNLDO = 0, /* cancel transfer because data are outdated */

    PDU_CNLNB, /* cancel transfer because no further buffer can be provided */

    PDU_CNLOR /* cancel transfer because of another reason */
} PduR_CancelReasonType;

/* Single interface module and TP module in case zero cost operation is enable */
typedef enum
{
    CAN_IF = 0,

    LIN_IF,
    
    CAN_TP,
    
    LIN_TP

} PduR_SingleIfType;

/* PduR module  Lower Layer Modules */
#define PDUR_CAN 			((uint8)0x00U)
#define PDUR_LIN 			((uint8)0x01U)
#define PDUR_IPDUML 		((uint8)0x02U)

/* PduR module Up Layer Modules */
#define PDUR_COM 			((uint8)0x00U)
#define PDUR_IPDUMU 		((uint8)0x01U)
#define PDUR_GATEWAY 		((uint8)0x02U)



/*Type for parameter value of parameter PDUR_STmin*/
typedef uint8 PduR_ParameterValueType;

/* define the function pointer for Transmit TxConfirmation TriggerTransmit Indication */
typedef P2FUNC(Std_ReturnType, PDUR_APPL_CODE, PduRLoIfTransmitType)(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/*define Com to LoIf routing table*/
typedef struct
{
	uint16	DestId; /* Tx PduId */
	uint8	BusType; /* CAN LIN IPDUM */
}PduR_ComDestTxPduType;

/*define  TxConfirmation routing table*/
typedef struct
{
	uint16	ConfId; /* Tx PduId */
	uint8	UpType; /* COM IPDUM GateWay */
}PduR_IfConfPduType;

/* define RxIndication routing table */
typedef struct
{
	uint16	DestId;   /* Rx PduId */
	uint8	UpType; /* COM IPDUM GateWay */
}PduR_IfDestRxPduType;

#if(STD_ON == PDUR_GATEWAY_OPERATION)
/* PduR Tx Buffer table  */
typedef struct
{
	uint16	DestId; /* Tx PduId */
	uint8	BusType; /* CAN LIN IPDUM */
	uint8 	Start;     /* Tx Buffer start */
	uint8 	Depth; /* 0:only support CAN; 1:singnel buffer;1<:FIFO */
	uint16 	Length;  /* Tx Buffer Length */
	uint8	DefaultValue; /* Tx Buffer default value */
}PduR_GWRoutingTableType;
#endif

/* define RxIndication 1:n routing table */
typedef struct
{
	uint16	StartId;
	uint16	StopId;
}PduR_IfSrcRxPduType;

/* PduR Global config of Post-build time  */
typedef struct
{
	uint8 	PduRConfigId;
	P2CONST(PduR_ComDestTxPduType, AUTOMATIC, PDUR_CONST_PBCFG) 	PduRComDestTxPduRef;
	P2CONST(PduR_IfConfPduType, AUTOMATIC, PDUR_CONST_PBCFG) 		PduRIfConfPduRef;
#if(STD_ON == PDUR_GATEWAY_OPERATION)
	P2CONST(PduR_GWRoutingTableType, AUTOMATIC, PDUR_CONST_PBCFG) 	PduRGWDestTxPduRef;
#endif
	P2CONST(PduR_IfDestRxPduType, AUTOMATIC, PDUR_CONST_PBCFG) 	       PduRIfDestRxPduRef;
	P2CONST(PduR_IfSrcRxPduType, AUTOMATIC, PDUR_CONST_PBCFG) 		PduRIfSrcRxPduRef;
	P2CONST(uint16, AUTOMATIC, PDUR_CONST_PBCFG) 					PduRIpduMRxPduRef;
}PduR_PBConfigType;





/*=======[E X T E R N A L   D A T A]==========================================*/




/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/




/*=======[I N T E R N A L   D A T A]==========================================*/



/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/



#endif  /* end of PDUR_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/

