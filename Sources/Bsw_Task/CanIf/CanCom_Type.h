// CanCom_Type.h
#ifndef CANCOM_TYPE_H
#define CANCOM_TYPE_H

#include "GeneralTypeDef.h"
#include "CanIf_Type.h"

// Pdu in stack
typedef uWord CCom_PduIdType;
typedef struct
{
    uByte *SduDataPtr;
    uWord SduLength;
} sCCOM_PduInfoType;

// define PDU
typedef enum
{
    RECEIVE,
    SEND
} eCCom_PduDirectionType;


typedef enum
{
    TX_DIRECT,
    TX_MIXED,
    TX_NONE,
    TX_PERIODIC
} eCComTxModeMode_type;

typedef enum
{
 PERIOD_CHANGE_DISABLE,
 PERIOD_CHANGE_ENABLE
} eCCom_TxModePeriodChangeEnableType;

typedef struct
{
    /** Transmission mode for this IPdu. */
    const eCComTxModeMode_type eTxModeMode;

    /** Defines the number of times this IPdu will be sent in each IPdu cycle.
     * Should be set to 0 for DIRECT transmission mode and >0 for DIRECT/N-times mode.
     */
    const uByte TxModeNumberOfRepetitionsFactor;

    /** Defines the period of the transmissions in DIRECT/N-times and MIXED transmission modes. */
    const uLWord TxModeRepetitionPeriodFactor;

    /** Time before first transmission of this IPDU. (i.e. between the ipdu group start and this IPDU is sent for the first time. */
    //const uLWord TxModeTimeOffsetFactor;  
   
    /** Period of cyclic transmission. */
    const uLWord TxModeTimePeriodFactor;
    
    /** Period of cycle change enable flag */
    const eCCom_TxModePeriodChangeEnableType TxModePeriodChangeEnable;
} sCComTxMode_type;

typedef struct
{
    const sCComTxMode_type sTxMode;
} eCCom_PduTxParameterType;

typedef struct
{
    const uByte FlgEndOfList; // 1 --- end ,0 --- other
    const CCom_PduIdType PduHandleId;
    const uByte PduSize;
    const eCCom_PduDirectionType PduDirection; // receive or send
    const eCCom_PduTxParameterType PduTxPara;
    const eCanIf_ChannelType PduCanIfChn;
    sCanIf_MsgType * const PduMsgPtr;
} sCCom_PduType;


typedef enum
{
    STOPED,   /* stoped, can do nothing */    
    STARTED,  /* started, can copy data, and can trismit */     
} eCCom_StartFlagType;

typedef struct
{
  uByte TxFirstFlag;
  uLWord TxFirstTimePeriodFactor;
}sCCom_TxPeriodIsFirstType;

typedef struct
{
    uByte TxNumberOfRepetitionsCnt;
    uLWord TxRepetitionPeriodCnt;
    uLWord TxTimePeriodCnt;
    uLWord TxChangeTimePeriodFactor;
    eCCom_StartFlagType TxStartedFlag;
    sCCom_TxPeriodIsFirstType TxIsFirst; 
    uByte TxResult; //eCanIf_FrameHandleResultType  
} sCCom_PduTxCtrlType;  

typedef struct
{
    const sCCom_PduType * CCom_PduPtr;
    sCCom_PduTxCtrlType * CCom_PduTxCtrlPtr;     
} sCCom_ConfigType;

#define GET_PDU(Pdu_Id) \
 (CCom_Config.CCom_PduPtr + Pdu_Id)

#define GET_PDUTXCTRL(Pdu_Id) \
(CCom_Config.CCom_PduTxCtrlPtr + Pdu_Id)

extern const sCCom_ConfigType CCom_Config;
#endif  /* CANCOM_TYPE_H */
