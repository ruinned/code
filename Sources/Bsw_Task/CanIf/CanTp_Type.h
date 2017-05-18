// CanTp_Type.h

#ifndef CANTP_TYPE_H
#define CANTP_TYPE_H

#include "GeneralTypeDef.h"
#include "CanIf_Type.h"
#include "Ecu.h"

#define MAX_CANTP_BUFFER 100

// buffer direction
typedef enum
{
    TX_BUF,
    RX_BUF
}eCanTp_TxRxBufType;

// these for CanTp buffer position
typedef enum
{
    DIAG_BUF,
    BALANCE_BUF,
    MAX_NUM_BUF
}eCanTp_UseBufType;

// error type
typedef enum
{
    CANTP_ERR_DEFAULT,
    CANTP_ERR_OK,
    CANTP_ERR_CHN,  // Can channel err
    CANTP_ERR_DIR,  // direction err
    CANTP_ERR_BS_OT,    // BS over time ,sender wait follow control frame over time
    CANTP_ERR_CR_OT,    // CR over time ,receive wait continune frame over time  
    CANTP_ERR_LEN,  // LEN over max len
    CANTP_ERR_CMD,  // invalid command
    CANTP_ERR_BUF_ST_CONFLICT        
}eCanTp_ErrType;

// buffer status
typedef enum
{
    CANTP_DEFAULT,
    CANTP_SENDING,     // indicate Tx buffer is send data now
    CANTP_SENDCPT,     // indicate Tx buffer send data complete
    CANTP_RECVING,     // indicate Rx buffer is receive data now
    CANTP_RECVCPT      // indicate RX buffer receive commande data complete
}eCanTp_BufStType;

// tx step type
typedef enum
{
    CANTP_TX_STEP_DEFAULT,
    CANTP_TX_SNGL_FRAME,
    CANTP_TX_FIRST_FRAME,
    CANTP_WAIT_FOLLOW_FRAME,
    CANTP_TX_CONTINUE_FRAME,
    CANTP_TX_LAST_FRAME
}eCanTp_TxStepType;

// rx step type
typedef enum
{
    CANTP_RX_STEP_DEFAULT,
    CANTP_RX_SNGL_FRAME,
    CANTP_RX_FIRST_FRAME,
    CANTP_RX_CONTIUNE_FRAME,
    CANTP_RX_FOLLOW_FRAME,
    CANTP_RX_LAST_FRAM
}eCanTp_RxStepType;

// Follow frame status
typedef enum
{
    CANTP_FOLLOW_FRAME_DFAUL,
    CANTP_FOLLOW_SEND,
    CANTP_FOLLOW_RECV
}eCanTp_FollowFrameType;

// CanTp control type
typedef struct
{
    uByte BufSt;
    uWord HadExecuteLen;
    uByte ExecuteStep;
    uByte ContinueFrameCouter;
    uByte FollowFrameStatus;
    uByte BSCnt;  // sender wait follow ctrol frame over time
    uByte BSCntEn;
    uByte CRCnt;  // receiver wait continue frame over time 
    uByte CRCntEn;  
    eCanTp_ErrType TpRslt; 
    uByte ContinueFrameTimer;
}sCanTp_CtrlType;

// Bufhandle 
typedef struct
{
    eCanTp_TxRxBufType TxOrRx;
    uWord MaxContinueFrameCount;
    uWord ValidLen;
    uLWord Id;
    uByte *RAMPTR DataPtr;
    uByte ContinueFrameTime;
}sCanTp_BufHandleType;

// super class
typedef struct
{
    eCanIf_ChannelType Chn;
    sCanTp_BufHandleType TxHandle;
    sCanTp_BufHandleType RxHandle;
    sCanTp_CtrlType TxCtrl;
    sCanTp_CtrlType RxCtrl;    
}sCanTp_BufType;

// CanTp MsgTyped
typedef struct
{
    uLWord Id;
    uByte  Data[MAX_CANTP_BUFFER];
    uWord  DataLen;
}sCanTp_MsgType;

#endif