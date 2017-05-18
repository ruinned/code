/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       CanTp_Internal.h
 *  @brief      Internal header file of CanTp module.
 *
 *  @author     stanley
 *  @date       2013-4-9
 */
/*============================================================================*/

#ifndef CANTP_INTERNAL_H 
#define CANTP_INTERNAL_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       2013-4-9   stanley     create
 * 
 *  
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANTP_INTERNAL_H_AR_MAJOR_VERSION  2
#define CANTP_INTERNAL_H_AR_MINOR_VERSION  3
#define CANTP_INTERNAL_H_AR_PATCH_VERSION  0
#define CANTP_INTERNAL_H_SW_MAJOR_VERSION  1
#define CANTP_INTERNAL_H_SW_MINOR_VERSION  0
#define CANTP_INTERNAL_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTpDiag_Types.h"
#if (CANTP_DEV_ERROR_DETECT == STD_ON)
    #include "Det.h"
#endif
//#include "Dem.h"
#include "PduR_CanTp.h"
#include "CanIfDiag.h"
#include "SchM_CanTp.h"
#include "Std_ExtendedTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
#define CANTP_CAN_FRAME_LEN_MAX (PduLengthType)0x08
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* State of the CanTp module. Initialized or not. */
typedef enum CanTp_State_E
{
    CANTP_OFF, /* CanTp module not initialized */
    CANTP_ON   /* CanTp module initialized */
} CanTp_StateType;

/* current enabled ABC timer for receiver or sender  */
typedef enum CanTp_ABCTimer_E
{
    CANTP_TIMER_NONE, /* No timer started */
    CANTP_NA,   /* Nar or Nas started */
    CANTP_NB,   /* Nbs or Nbr started */
    CANTP_NC    /* Ncs or Ncr started */
} CanTp_ABCTimerType;

/* running control of the ABC timer for receiver and sender */
typedef struct CanTp_ChannelTimer_T
{
    CanTp_ABCTimerType EnabledTimer;
    TickType StartTime;
    TickType RemainTime;
} CanTp_ChannelTimerType;

/* running control for STmin for sender */
typedef struct CanTp_STmin_T
{
    boolean Started;
    TickType RemainTime;
    TickType StartTime;
    TickType FcSTMin;
} CanTp_STminType;

/*root states of communication channel*/
typedef enum CanTp_ChannelRootState_E
{
    CANTP_IDLE, /*the channel is idle*/
    CANTP_SF_RECEIVING, /*the channel is handling SF receiving*/
    CANTP_SF_TRANSIMITTING, /*the channel is handling SF transmitting*/
    CANTP_LARGE_RECEIVING, /*the channel is handling large SDU receiving*/
    CANTP_LARGE_TRANSMITTING /*the channel is handling large SDU transmitting*/
} CanTp_ChannelRootStateType;

/*Sub-states of communication channel*/
typedef enum CanTp_ChannelSubstate_E
{
    CANTP_SUBSTATE_NONE,

    CANTP_RX_SF_START, /*SF Receiving*/

    CANTP_RX_LARGE_START, /*Large receiving start*/
    CANTP_RX_LARGE_V4FCWT_CFM, /*wait for confirmation of FC_WAIT*/
    CANTP_RX_LARGE_V4FCCTS_CFM, /*wait for confirmation of FC_CTS*/
    CANTP_RX_LARGE_V4CF, /*wait for the following CFs*/
    CANTP_RX_LARGE_RXCF, /*receiving received CF*/

    CANTP_TX_SF_START, /*SF transmitting start*/
    CANTP_TX_SF_V4SF_CFM, /*wait for confirmation of SF transmitting*/

    CANTP_TX_LARGE_START, /*large transmitting start*/
    CANTP_TX_LARGE_V4DATA_CFM, /*wait for confirmation of transmitted frame*/
    CANTP_TX_LARGE_V4FC, /*wait for FC*/
    CANTP_TX_LARGE_SENDCF /*sending CFs*/
} CanTp_ChannelSubstateType;

/*type of frame types*/
typedef uint8 CanTp_FramePCIType;

#define CANTP_FTYPE_MASK (uint8)0xF0

#define CANTP_FTYPE_SF 0x00

#define CANTP_FTYPE_FF (uint8)0x10

#define CANTP_FTYPE_CF (uint8)0x20

#define CANTP_FTYPE_FC (uint8)0x30

#define CANTP_FTYPE_RESEVED (uint8)0xF0

/*mask of the SF_DL*/
#define CANTP_SF_DL_MASK (uint8)0x0F

/*mask of the upper nibble for FF DL*/
#define CANTP_FF_DL_MASK (uint8)0x0F

/*bit offset of the length information in the low part of first PCI byte*/
#define CANTP_OFFSET_FFDL_HIGNT_PART 8

/*Offset of FF_DL low nibble for standard addressing*/
#define CANTP_FF_FFDL_LOW_OFFSET_STD 1

/*Offset of FF_DL low nibble for extended addressing*/
#define CANTP_FF_FFDL_LOW_OFFSET_EX 2

/*Offset of FF_DL high nibble for standard addressing*/
#define CANTP_FF_FFDL_HIGH_OFFSET_STD 0

/*Offset of FF_DL high nibble for extended addressing*/
#define CANTP_FF_FFDL_HIGH_OFFSET_EX 1

/*mask of the SN for CF*/
#define CANTP_CF_SN_MASK (uint8)0x0F

/*mask of the FS for FC*/
#define CANTP_FC_FS_MASK (uint8)0x0F

/*CONST values for FS of FC*/
/*continue to send*/
#define CANTP_FC_FS_CTS  0x00

/*FC_WAIT*/
#define CANTP_FC_FS_WT (uint8)0x01

/*overflow*/
#define CANTP_FC_FS_OVFLW (uint8)0x02

/*reserved*/
#define CANTP_FC_FS_RESERVED (uint8)0x0F

/*offset of FC_BS for standard addressing*/
#define CANTP_FC_OFFSET_BS_STD  1

/*offset of FC_BS for extended addressing*/
#define CANTP_FC_OFFSET_BS_EX  2

/*FC STmin limits*/
/*ms upper limit*/
#define CANTP_FC_STMIN_MS_LIMIT (uint8)0x7F

/*us lower limit*/
#define CANTP_FC_STMIN_US_LOW (uint8)0xF1

/*us upper limit*/
#define CANTP_FC_STMIN_US_UPPER (uint8)0xF9

/*unit for STmin from 0-7f in us*/
#define CANTP_FC_STMIN_MS_UNIT (uint32)1000

/*unit for STmin from F1-F9 in us*/
#define CANTP_FC_STMIN_US_UNIT (uint32)100

/*offset of FC_STMin for standard addressing*/
#define CANTP_FC_OFFSET_STMIN_STD  2

/*offset of FC_STmin for extended addressing*/
#define CANTP_FC_OFFSET_STMIN_EX  3

/*frame length of FC for standard addressing*/
#define CANTP_FC_LEN_STD  (PduLengthType)0x03

/*frame length of FC for extended addressing*/
#define CANTP_FC_LEN_EX  (PduLengthType)0x04

/*maximum data length of N-SDU*/
#define CANTP_NSDU_LENGTH_LIMIT (PduLengthType)4095

/*maximum length of single frame data, if the addressing format is standard*/
#define CANTP_SF_LEN_MAX_STD (PduLengthType)0x07

/*maximum length of single frame data, if the addressing format is extended*/
#define CANTP_SF_LEN_MAX_EX (PduLengthType)0x06

/*maximum length of the CF data for standard SDU*/
#define CANTP_CF_DATALEN_STD (PduLengthType)0x07

/*maximum length of the CF data for extended SDU*/
#define CANTP_CF_DATALEN_EX (PduLengthType)0x06

/*CF data offset for CF of standard addressing*/
#define CANTP_CF_DATA_OFFSET_STD (PduLengthType)0x01

/*CF data offset for CF of extended addressing*/
#define CANTP_CF_DATA_OFFSET_EX (PduLengthType)0x02

/*data offset of FF for standard addressing*/
#define CANTP_FF_DATA_OFFSET_STD (PduLengthType)0x02

/*data offset of FF for extended addressing*/
#define CANTP_FF_DATA_OFFSET_EX (PduLengthType)0x03

/*data offset of SF for standard addressing*/
#define CANTP_SF_DATA_OFFSET_STD (PduLengthType)0x01

/*data offset of SF for extended addressing*/
#define CANTP_SF_DATA_OFFSET_EX (PduLengthType)0x02

/*offset of PCI for standard addressing*/
#define CANTP_PCI_OFFSET_STD (PduLengthType)0x00

/*offset of PCI for extended addressing*/
#define CANTP_PCI_OFFSET_EX (PduLengthType)0x01

/*offset of TA in extended addressing frame*/
#define CANTP_TA_OFFSET_EX (PduLengthType)0x00

/*A pool for PDU received from lower module, don't care whatever it is*/
typedef struct CanTp_ReceivePool_T
{
    boolean Filled;
    PduIdType RxNPduId;
    PduInfoType PduInfo;
    uint8 DataBuf[CANTP_CAN_FRAME_LEN_MAX];
} CanTp_RxPoolType;

/* Channel parameters during CanTp module running. Channel is a logic connection between sender and receiver. */
typedef struct CanTp_Channel_T
{
    /*runtime timer for channel*/
    CanTp_ChannelTimerType ChannelTimer;

    /*STmin timer runtime information*/
    CanTp_STminType STminTimer;

    /*root state of channel*/
    CanTp_ChannelRootStateType RootState;

    /*sub-state of channel*/
    CanTp_ChannelSubstateType Substate;

    /*handled frame type in current phase*/
    CanTp_FramePCIType HandleType;

    /*identifier of N-SDU which is using this channel*/
    PduIdType NSduId;

    /*newly received N-PDU identifier*/
    PduIdType RcvdNPduId;

    /*newly received N-SDU identifier*/
    PduIdType RcvdNSduId;

    /*length of valid data in local buffer*/
    PduLengthType LocalBufLen;

    /*offset of the valid data in local buffer*/
    PduLengthType LocalBufPosition;

    /*local buffer of channel*/
    uint8 LocalBuf[CANTP_CAN_FRAME_LEN_MAX];

    /*FC content received or transmit by this channel*/
    uint8 FcBuf[CANTP_CAN_FRAME_LEN_MAX];

    /*data in bytes remaining in PDUR buffer*/
    PduLengthType PduRBufRemaining;

    /*offset of data to deal with in PDUR buffer*/
    PduLengthType PduRBufPosition;

    /*pointer to buffer of PDUR*/
    P2VAR(uint8, TYPEDEF, CANTP_APPL_DATA) PduRBufContentPointer;

    /*data in bytes need to deal with of this N-SDU*/
    PduLengthType SduDataRemaining;

    /*total count of N-SDU data in bytes*/
    PduLengthType SduDataTotalCount;

    /*current BS value*/
    uint8 CurrentBs;

    /*count of handled CF*/
    uint8 HandledCfCount;

    /*current CF serial number*/
    uint8 CurrentCfSn;

    /*number of already sent FCwait consecutively*/
    uint8 SentWftCount;

    /*Event Flags*/
    uint8 EventFlags;
} CanTp_ChannelType;

/*structure for FC frame information*/
typedef struct CanTp_FCInfo_T
{
    uint8 FcFs;
    uint8 FcBS;
    TickType FcSTMin; /*STmin in tick unit*/
} CanTp_FCInfoType;

/*Event flags*/
#define CANTP_CHANNEL_EVENT_SF_RECEIVED (uint8)0x01

#define CANTP_CHANNEL_EVENT_FF_RECEIVED (uint8)0x02

#define CANTP_CHANNEL_EVENT_CF_RECEIVED (uint8)0x04

#define CANTP_CHANNEL_EVENT_FC_RECEIVED (uint8)0x08

#define CANTP_CHANNEL_EVENT_TX_CONFIRMED (uint8)0x10

#define CANTP_CHANNEL_EVENT_TXFC_CONFIRMED (uint8)0x20

#define CANTP_CHANNEL_EVENT_TXCANCELLED (uint8)0x40

#define CANTP_CHANNEL_EVENT_TRANSMITREQUEST (uint8)0x80

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"
/*CANTP global variable for module state:CANTP_ON, CANTP_OFF*/
extern volatile VAR(CanTp_StateType, CANTP_VAR_POWER_ON_INIT) CanTp_ModuleState;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

#define CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"
/*CANTP global variable for runtime channel controlling information*/
extern VAR(CanTp_ChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];

/*Rx Pool for Channels*/
extern VAR(CanTp_RxPoolType, CANTP_VAR_NOINIT) CanTp_ChnlPools[CANTP_CHANNEL_NUMBER];

#define CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "MemMap.h"
/*************************************************************************/
/*
 * Brief               Initialize the specific channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of the channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module Initialization phase, or receiving/transmitting
 *                     success or failed
 * CallByAPI           CanTp_Init, CanTp Internal
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_InitChannel(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Get TX-SDU configuration by TX-NSduId.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TxSduId ID of the TX SDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched with TxSduId
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId);

/*************************************************************************/
/*
 * Brief               Get RX-SDU configuration by received NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of the received NPDU
 *                     CanTpRxPduPtr pointer to the received NPDU data
 *                     SduFrameType pointer to store the frame type
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType
    );

/*************************************************************************/
/*
 * Brief               Get Rx-SDU configuration by Tx-FC NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpTxPduId ID of the NPDU ID of TX FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId);

/*************************************************************************/
/*
 * Brief               Get TX-SDU configuration by received FC NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduId ID of the NPDU ID of received FC
 *                     CanTpRxPduPtr pointer to the data of received FC
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByFCNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr
    );

/*************************************************************************/
/*
 * Brief               Get SDU configuration by N-SDU ID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxSduId ID of the SDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId);

/*************************************************************************/
/*
 * Brief               Get Tx-SDU configuration by Tx NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpTxPduId ID of the data NPDU
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              pointer to the configuration structure of this SDU
 *                     NULL_PTR if the no SDU matched
 * PreCondition        Module initialized
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId);

/*************************************************************************/
/*
 * Brief               Handle received CF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for CF,
 *                     and CF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleCFReception(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle received FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is allowed to receive FF,
 *                     and FF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleFFReception(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is allowed to receive SF,
 *                     and SF received
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleSFReception(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle confirmation for FC.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for the confirmation
 *                     of FC frame, and a FC frame confirmed.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleFCCfm(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Back ground handling for receiving SF,
 *                     in this state the channel is trying to get buffer to store
 *                     received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get buffer to store
 *                     received SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleSFStart(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Back ground handling for large SDU receiving, trying to get RX buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get buffer to store
 *                     received FF or CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleLargeStart(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Get FF_DL information from the data content of FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduPtr pointer to data
 *                     RxNSduCfgPtr pointer to configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              FF_DL
 * PreCondition        Module initialized, the channel received a FF.
 * CallByAPI           CanTp_RxIndication and background handling
 */
/*************************************************************************/
extern FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr);

/*************************************************************************/
/*
 * Brief               Get SFF_DL information from the data content of SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpRxPduPtr pointer to data
 *                     RxNSduCfgPtr pointer to configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              FF_DL
 * PreCondition        Module initialized, the channel received a SF.
 * CallByAPI           CanTp_RxIndication and background handling
 */
/*************************************************************************/
extern FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetSFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr);

/*************************************************************************/
/*
 * Brief               Handle received FC.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for a FC from
 *                     the receiver, and FC received.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleFCReception(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel has accepted a transmitting
 *                     request.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitReq(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle transmit cancel request event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel has accepted a cancel transmitting
 *                     request.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCancel(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Handle confirmation for data transmit.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is waiting for a confirmation
 *                     of transmitted data.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCfm(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Back ground handling for SF transmitting.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get TX buffer
 *                     so that can transmit the SF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleSFStart(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Back ground handling for large SDU transmitting, trying to get TX buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is trying to get TX buffer
 *                     so that can transmit the FF or CF.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeStart(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Back ground handling for sending CFs.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx channel index
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, the channel is in the state of sending the
 *                     remain CF of current block.
 * CallByAPI           CanTp_MainFunction
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeSendCF(uint8 ChannelIdx);

/*************************************************************************/
/*
 * Brief               Utility function: Set memory with specific value.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Address memory address to set
 *                     Value value to set
 *                     Length memory length to set
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_MemorySet(
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address,
    uint8 Value,
    uint16 Length);

/*************************************************************************/
/*
 * Brief               Utility function:copy memory data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Source memory address to copy from
 *                     Dest memory address to copy to
 *                     Length memory length to copy
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
extern FUNC(void, CANTP_CODE)
CanTp_MemoryCopy(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) Source,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Dest,
    uint16 Length);

/*************************************************************************/
/*
 * Brief               Filter the Reception indication event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ChannelIdx index of the channel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        Module initialized, a NPDU received.
 * CallByAPI           CanTp_Mainfunctioin
 */
/*************************************************************************/
FUNC(void, CANTP_CODE)
CanTp_FilterReceivedPdu(uint8 ChannelIdx);

#define CANTP_STOP_SEC_CODE
#include "MemMap.h"

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_INTERNAL_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
