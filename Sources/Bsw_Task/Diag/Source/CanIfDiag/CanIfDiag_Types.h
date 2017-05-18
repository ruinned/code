/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanIf_Types.h>
 *  @brief      <CanIf>
 *  
 *  <Compiler: CodeWarrior2.8 MCU:MPC5634>
 *  
 *  @author     <Tommy>
 *  @date       <2013-05-10> 
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130425   liujn      Initial version
 *  V1.0.1       20130510   Tommy      reconstruct version
 * 
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#ifndef CANIFDIAG_TYPES_H
#define CANIFDIAG_TYPES_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_TYPES_H_VENDOR_ID          60U
#define CANIF_TYPES_H_AR_MAJOR_VERSION   3U
#define CANIF_TYPES_H_AR_MINOR_VERSION   2U
#define CANIF_TYPES_H_AR_PATCH_VERSION   0U
#define CANIF_TYPES_H_SW_MAJOR_VERSION   1U
#define CANIF_TYPES_H_SW_MINOR_VERSION   0U
#define CANIF_TYPES_H_SW_PATCH_VERSION   8U

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Can.h" /** @req CANIF279 */
#if (STD_ON == CANIF_WAKEUP_VALIDATION)
#include "EcuM_Cbk.h"
#endif

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* Operating modes of the CAN network and CAN Driver */
typedef enum
{
    CANIF_CS_UNINIT = 0u,
    CANIF_CS_STOPPED,
    CANIF_CS_STARTED,
    CANIF_CS_SLEEP
}CanIf_ControllerModeType;/** @req CANIF136 */


/* notification status of the corresponding CAN L-PDU */
typedef enum 
{
    CANIF_NO_NOTIFICATION = 0u,
    CANIF_TX_RX_NOTIFICATION
}CanIf_NotifStatusType; /** @req CANIF201 */


/* Operating modes of the CAN Transceiver Drive */
typedef enum 
{
    CANIF_TRCV_MODE_NORMAL = 0,/* Transceiver mode NORMAL */
    CANIF_TRCV_MODE_STANDBY,/* Transceiver mode STANDBY */
    CANIF_TRCV_MODE_SLEEP /* Transceiver mode SLEEP */
}CanIf_TransceiverModeType; /** @req CANIF263 */

/* specify the wake up reason detected by the CAN transceiver in detail. */
typedef enum 
{
    CANIF_TRCV_WU_ERROR = 0,/* Due to an error wake up reason was not detected */
    CANIF_TRCV_WU_NOT_SUPPORTED,/* not support any information for the wake up reason */
    CANIF_TRCV_WU_BY_BUS,/* The transceiver has detected the wake up of the ECU */
    CANIF_TRCV_WU_INTERNALLY,/* woken up by the ECU via a request to NORMAL mode */
    CANIF_TRCV_WU_RESET,/* the "wake up" is due to an ECU reset */
    CANIF_TRCV_WU_POWER_ON /* "wake up"is due to an ECU reset after power on. */    
}CanIf_TrcvWakeupReasonType; /** @req CANIF264 */

/*  The notification for wakeup events in detail. */
typedef enum 
{
    CANIF_TRCV_WU_ENABLE = 0,/* The notification for wakeup events is enabled */
    CANIF_TRCV_WU_DISABLE,/* The notification for wakeup events is disabled */
    CANIF_TRCV_WU_CLEAR /* A stored wakeup event is cleared */    
}CanIf_TrcvWakeupModeType; /** @req CANIF275 */


/* define CANIF module initiation status */
typedef enum
{
    CANIF_INITED = 0u,
    CANIF_UNINIT
}CanIf_InitStatusType;


/* upper layer type */
typedef enum 
{
    /* CAN NM */
    CANIF_USER_TYPE_CAN_NM,
    /* CAN TP */
    CANIF_USER_TYPE_CAN_TP,
    /* PDU Router */
    CANIF_USER_TYPE_CAN_PDUR,
    /* XCP or CCP*/
    CANIF_USER_TYPE_XCP_CCP,
    /* OSEK NM */
    CANIF_USER_TYPE_OSEKNM,
    /* APP */
    CANIF_USER_TYPE_APP,
    /* used for count */
    CANIF_USER_MAX_COUNT
}CanIf_UserType;


/* define tx CAN L_PDU type */
typedef enum
{
    /* static */
    CANIF_PDU_TYPE_STATIC = 0u,
    /* dynamic */
    CANIF_PDU_TYPE_DYNAMIC
}CanIf_CanTxPduType;


/* define CAN ID type */ 
/** @req CANIF243 @req CANIF301 */
typedef enum
{
    /* extended 29 */
    CANIF_EXTENDED_CAN,
    /* standard 11 */
    CANIF_STANDARD_CAN
}CanIf_PduIdCanIdType;

/*Defines the HRH type*/
typedef enum
{
    CANIF_BASIC_CAN,
    CANIF_FULL_CAN
} CanIf_HohType;


/* define wakeup source */
typedef enum 
{
    /* support wake up by CAN controller */
    CANIF_WAKEUP_SUPPORT_CONTROLLER,
    /* no wake up support */
    CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
    /* support wake up by transceiver */
    CANIF_WAKEUP_SUPPORT_TRANSCEIVER
} CanIf_WakeupSupportType;

 

/* PDU set mode */
typedef enum
{
    CANIF_SET_OFFLINE = 0u,
    CANIF_SET_RX_OFFLINE,
    CANIF_SET_RX_ONLINE,
    CANIF_SET_TX_OFFLINE,
    CANIF_SET_TX_ONLINE,
    CANIF_SET_ONLINE,
    CANIF_SET_TX_OFFLINE_ACTIVE
}CanIf_ChannelSetModeType;/** @req CANIF137 */ 

/* PDU Get mode */
typedef enum
{
    CANIF_GET_OFFLINE = 0u,
    CANIF_GET_RX_ONLINE,
    CANIF_GET_TX_ONLINE,
    CANIF_GET_ONLINE,
    CANIF_GET_OFFLINE_ACTIVE,
    CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE
}CanIf_ChannelGetModeType; /** @req CANIF138 */

/** @req CANIF104 @req CANIF131 @req CANIF066 
    @req CANIF240 @req CANIF132 @req CANIF228 */
/* Defines the parameters required for configuraing multiple
 * CANID ranges for a given same HRH 
 */
typedef struct
{
    Can_IdType hrhRangeMask;
    Can_IdType canIdMask;
}CanIf_HrhRangeMaskType;/** @req CANIF930 @req CANIF016 */


/* USER define data structure,TxPDU structure */
typedef struct
{
    boolean used;
    uint8 length;
    PduIdType swPduHandle;
    Can_IdType canId;
    uint8 data[8u];
}CanIf_TxPduBufferType;

/* USER define data structure,RxPDU structure */
typedef struct
{
    /* length */
    PduLengthType length;
    /* data */
    uint8 data[8u];
}CanIf_RxPduBufferType;


/** @req CANIF259 */
/* contains configuration parameters for each hardware receive object  */
typedef struct
{
    /* Defines the HRH type i.e, whether its a BasicCan or FullCan */
    CanIf_HohType CanIfHrhType; /** @req CANIF123 */

    /* perform software filtering */
    boolean CanIfSoftwareFilterHrh;

    /* Reference to controller Id to which the HRH belongs to */
    uint8 CanIfCanControllerHrhIdRef;

}CanIf_HrhConfigType;

typedef struct
{
    /* start index in pduCfg of same Hoh */
    uint8 startIndex;
    
    /* stop index in pduCfg of same Hoh */
    uint8 stopIndex;
}CanIf_HohIndexType;



/** @req CANIF258 */
/* contains parameters related to each HTH */
typedef struct
{
    /*Transmission method of the corresponding HTH */
    CanIf_HohType CanIfHthType;

    /* Reference to controller Id to which the HTH belongs to */
    uint8 CanIfCanControllerIdRef;

    /* refers to a particular HTH object in the CAN Driver Module configuration*/
    uint8 CanIfHthIdSymRef;

}CanIf_HthConfigType;


#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)

typedef struct
{
   boolean CanIfTrcvWakeupNotification;
   uint8 CanIfTrcvIdRef;
   uint8 CanIfTrcvFctIndex;/* CanTransceiver Function pointer index */
}CanIfTransceiverDrvConfigType;

#endif /* end of (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED) */


/** @req CANIF257 @req CANIF048 */
/* contains the references to the configuration setup of each underlying CAN Driver. */
typedef struct
{
    /* Select s the CAN Interface specific  configuration setup */
    P2CONST(Can_ControllerConfigType, AUTOMATIC, CANIF_APPL_CONST) CanIfRefConfigSetRef;
    
}CanIf_InitHohConfigType;


/** @req CANIF248 @req CANIF045 */
/*  contains the configur ation (parameters) of each transmit CAN L-PDU */
typedef struct
{
    /* CAN Identifier of transmit CAN L-PDUs */
    Can_IdType CanIfCanTxPduIdCanId;

    /* Data length code (in bytes) of transm Driver for CAN L-PDU transmission. */
    uint32 CanIfCanTxPduIdDlc;

    /* the target Txpdu handle.*/
    uint16 CanIfCanTxPduId;
    
    /* Defines the type of each transmit CAN L-PDU */
    CanIf_CanTxPduType CanIfCanTxPduType;

    /* if CanIfCanTxPduType is Dynamic,Then mark the index of this pdu in DynamicTxPduCanId buffer*/
    PduIdType CanIfDynamicTxPduCanIdIndex;

    /* Enables and disables transmit confirmation */
    boolean CanIfReadTxPduNotifyStatus;

    /* if CanIfReadTxPduNotifyStatus is TRUE ,then this pdu has a Notify buffer,mark the buffer index */
    uint8 CanIfTxNotifyIndex;

    /* CAN Identifier of transmit CAN L-PDUs,Config tool need check the consistency of used hth*/
    CanIf_PduIdCanIdType CanIfTxPduIdCanIdType;

    /* type of the transmit confirmation call-out */
    CanIf_UserType CanIfTxUserType;

    /* The configuration index of the CanIfHthConfig */
    uint8 CanIfCanTxPduHthRef;
}CanIf_TxPduConfigType;

/** @req CANIF249 */
/* contains the configur ation (parameters) of each receive CAN L-PDU */
typedef struct
{
    /* CAN ID */
    Can_IdType CanIfCanRxPduCanId;

    /* Data Length code of received CAN L-PDUs*/
    uint8 CanIfCanRxPduDlc;

    /* the target pdu handle,start with 0 and continue without any gaps.*/
    uint16 CanIfCanRxPduId;

    /* Enables and disables the Rx buffering for reading of received L-PDU data. */
    boolean CanIfReadRxPduData;

    /* if CanIfReadRxPduData is TRUE ,then this pdu has a rx buffer,mark the buffer index */
    uint8 CanIfRxBufferIndex;

    /* Enables and disables receive indication*/
    boolean CanIfReadRxPduNotifyStatus;
    
    /* if CanIfReadRxPduNotifyStatus is TRUE ,then this pdu has a Notify buffer,mark the buffer index */
    uint8 CanIfRxNotifyIndex;

    /* CAN Identtifier of receive CAN L-PDUs */
    CanIf_PduIdCanIdType CanIfRxPduIdCanIdType;

    /*  type of the receive indication call-outs*/
    CanIf_UserType CanIfRxUserType;

}CanIf_RxPduConfigType;


typedef struct
{
    /* Enables wakeup support and defines the source device of a wakeup event. */
    const CanIf_WakeupSupportType CanIfWakeupSupport;

    /* Logical handle of the underlying CAN controller */ /** @req CANIF130 */
    const uint8 CanIfControllerIdRef;
    
    #if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
    /* Logical handle of the underlying CAN Tranceiver,add by isoft */ 
    const uint8 CanIfCanTrcvIndex;
    #endif
    
    #if (STD_ON == CANIF_WAKEUP_VALIDATION)
    /* CanIf WakeUp source,add by isoft */
    const EcuM_WakeupSourceType CanIf_WakeUpSource;
    #endif  
    
}CanIf_ControllerConfigType; /** @req CANIF145 @req CANIF929 */


/** @req CANIF250 */
/* Callout functions with respect to the upper layers */
typedef struct
{
    /* Name of target BusOff notification services  */
    P2FUNC(void, CANIF_APPL_CODE, CanIfBusOffNotification)(uint8);

    #if (STD_ON == CANIF_WAKEUP_VALIDATION)
    /* Name of target wakeup notification services */
    P2FUNC(void, CANIF_APPL_CODE, CanIfWakeupNotification)(EcuM_WakeupSourceType);

    /* Name of target wakeup validation notification services*/
    P2FUNC(void, CANIF_APPL_CODE, CanIfWakeupValidNotification)(EcuM_WakeupSourceType);
    #endif
} CanIf_DispatchConfigType;


/** @req CANIF253 */
/* Configuration parameters for all the underlying CAN drivers*/
typedef struct
{
    boolean CanIfBusoffNotification;
    boolean CanIfReceiveIndication;
    boolean CanIfTxConfirmation;
    boolean CanIfWakeupNotification;
}CanIf_DriverConfigType;

/* define Hrh and coressponding RangMask and PduIndex references */
typedef struct 
{
    /* HrhId, support post build, */
    P2CONST(uint8, AUTOMATIC, CANIF_CONST_PBCFG) CanIfHrhIdSymRef;
    
    /* RxPdu Config Mask Table Pointer */
    P2CONST(CanIf_HrhRangeMaskType , AUTOMATIC, CANIF_CONST_PBCFG) CanIfHrhRangeMask;

    /* each hrh used as BASIC CAN has a Index range in Rxpducfg */
    P2CONST(CanIf_HohIndexType, AUTOMATIC, CANIF_CONST_PBCFG) CanIfHrhPduIndexRef;

}CanIf_HrhFilterConfigType;


/* CanIf_ConfigType only available in PostBuild Time */
typedef struct
{
    /* RxPdu number */
    uint8 CanIfNumberOfCanRxPduIds;

    /* TxPdu number */
    uint8 CanIfNumberOfCanTXPduIds;

    /* Dynamic Pdu Number */
    uint8 CanIfNumberOfDynamicCanTXPduIds;

    /* References the corresponding CAN Controller configuration setup of the corresponding CAN Driver */
    P2CONST(CanIf_InitHohConfigType, AUTOMATIC, CANIF_CONST_PBCFG) CanIfRefConfigSet;

    /* configuration (parameters) of each receive CAN L-PDU */
    P2CONST(CanIf_RxPduConfigType, AUTOMATIC, CANIF_CONST_PBCFG) CanIfRxPduConfigRef;

    /* Hrh and coressponding RangMask and PduIndex references */
    P2CONST(CanIf_HrhFilterConfigType, AUTOMATIC, CANIF_CONST_PBCFG) CanIf_HrhFilterRef;    

    /* configuration (parameters) of each transmit CAN L-PDU */
    P2CONST(CanIf_TxPduConfigType, AUTOMATIC, CANIF_CONST_PBCFG) CanIfTxPduConfigRef; 
    
    /* each hth used as BASIC CAN has a Index range in Txpducfg   */
    P2CONST(CanIf_HohIndexType, AUTOMATIC, CANIF_CONST_PBCFG) CanIfHthIndexConfigRef; 
    
}CanIf_ConfigType; /** @req CANIF144 */


/* define the function pointer for RxIndication and Txconfirmation */
typedef P2FUNC(void,CANIF_APPL_CODE,CanIfUserRxFct)(PduIdType CanRxPduId, P2CONST(PduInfoType,AUTOMATIC, AUTOMATIC) PduInfoPtr);

typedef P2FUNC(void,CANIF_APPL_CODE,CanIfUserTxFct)(PduIdType CanTxPduId);


#if (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED)
typedef P2FUNC(Std_ReturnType,CANIF_APPL_CODE,CanTrcvSetOpModeFct)(uint8 Transceiver, CanIf_TransceiverModeType  TransceiverMode);

typedef P2FUNC(Std_ReturnType,CANIF_APPL_CODE,CanTrcvGetOpModeFct)(uint8 Transceiver, P2VAR(CanIf_TransceiverModeType, AUTOMATIC, AUTOMATIC) TransceiverModePtr);

typedef P2FUNC(Std_ReturnType,CANIF_APPL_CODE,CanTrcvGetBusWuReasonFct)(uint8 Transceiver, P2VAR(CanIf_TrcvWakeupReasonType, AUTOMATIC, AUTOMATIC)TrcvWuReasonPtr);

typedef P2FUNC(Std_ReturnType,CANIF_APPL_CODE,CanTrcv_SetWakeupModeFct)(uint8 Transceiver,  CanIf_TrcvWakeupModeType TrcvWakeupMode);

typedef P2FUNC(Std_ReturnType,CANIF_APPL_CODE,CanTrcvCheckWakeupFct)(uint8 Transceiver);

typedef struct
{
    CanTrcvSetOpModeFct CanTrcvSetOpModeFctPtr;

    CanTrcvGetOpModeFct CanTrcvGetOpModeFctPtr;

    CanTrcvGetBusWuReasonFct CanTrcvGetBusWuReasonFctPtr;

    CanTrcv_SetWakeupModeFct CanTrcv_SetWakeupModeFctPtr;

    CanTrcvCheckWakeupFct CanTrcvCheckWakeupFctPtr;
    
} CanIfTrcvFctType;

#endif /* (STD_ON == CANIF_CANTRANSCEIVER_SUPPORTED) */

#endif /* #ifndef CANIF_TYPES_H */


/*=======[E N D   O F   F I L E]==============================================*/
