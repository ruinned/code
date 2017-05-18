// CanIf_Type.h
#ifndef CANIF_TYPE_H
#define CANIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

/*  error code define, begin from 0x100u~0x11F
 *  0x100~0x110 For CanIf
 *  0x110~0x11F For CanTp 
 */
#define ERR_BUF_INCONSISTENCY   ERR_BSW_CAN_BASE //0x100U /* software buffer inconsistency */
#define ERR_BUF_LOCKED          (ERR_BSW_CAN_BASE+1)//0x101U /* buffer is locked */
#define ERR_BUF_EMPTY           (ERR_BSW_CAN_BASE+2)//0x102U /* buffer is empty */
#define ERR_BUF_FULL            (ERR_BSW_CAN_BASE+3) //0x103* buffer is full */
#define ERR_MSG_LENGTH_OVER   (ERR_BSW_CAN_BASE+4)//0x104U /* message length over 8 bytes  */
#define ERR_CANIF_POINTER_NULL (ERR_BSW_CAN_BASE+5) //0x105 CanIf operation null pointer
#define ERR_CANIF_HANDLE_ERR   (ERR_BSW_CAN_BASE+6) //0x106 handle check error , this is default error in CanIf
#define ERR_CANIF_RSV          (ERR_BSW_CAN_BASE + 0xF) //(0x107~0x10F) reserver for CanIf
#define ERR_CANTP_OVER_BUF     (ERR_BSW_CAN_BASE+0x10) //0x110 over cantp buffer               
#define ERR_CANTP_RX_FIRST_FRAME (ERR_BSW_CAN_BASE+0x11) // 0x111 CanTp receive first frame error
#define ERR_CANTP_RX_CONTINUE_FRAME (ERR_BSW_CAN_BASE+0x12) // 0x112 CanTp receive continue frame error
#define ERR_CANTP_RX_FOLLOW_CTRL (ERR_BSW_CAN_BASE + 0x13) // 0x113 CanTp receive follow ctrl frame

typedef struct
{
    uLWord id;     /* CAN Msg id, donnot include SRR IDE RTR */
    uByte length;  /* CAN Msg data length */
    uByte data[8];  /* CAN Msg data */
} sCanIf_MsgType;

typedef enum
{
    CANIF_DATA_FRAME,
    CANIF_REMOTE_FRAME
} eCanIf_FrameTypeType;
#define IS_DATA_FRAME(len,type) \
  (((len) > 0) ? ((type) = CANIF_DATA_FRAME) : ((type) = CANIF_REMOTE_FRAME) )

typedef enum
{
    CANIF_STANDARD_FORMAT,
    CANIF_EXTENDED_FORMAT
} eCanIf_FrameFormatType;
#define IS_EXTENDED_FORMAT(id,format) \
 (((id) > 0x7FFU) ? ((format) = CANIF_EXTENDED_FORMAT) : ((format) = CANIF_STANDARD_FORMAT) )

typedef struct
{
    //uByte no;  /* frame NO. */
    eCanIf_FrameTypeType type;  /* frame type */
    eCanIf_FrameFormatType format; /* frame format */
    sCanIf_MsgType msg;         /* frame message */
} sCanIf_FrameType;

typedef enum
{
    CANIF_BUF_DEFAULT, // initial status
    CANIF_BUF_RDING,  // reading now , buf cannot be write
    CANIF_BUF_RD_COMPLT, // complete read, buf can be write
    CANIF_BUF_WRING,  // write now , buf cannot be read
    CANIF_BUF_WR_COMPLT, // complete write , buf can be write and read
    CANIF_BUF_LOCK  // latch for judge status, need recovery previous status after judge
} eCanIf_FrmameStatusType;

typedef enum
{
  CANIF_NULL,
  CANIF_WR_SWBUF_OK,
  CANIF_WR_SWBUF_FAIL,
  CANIF_WR_HWBUF_OK,
  CANIF_WR_HWBUF_FAIL,
  CANIF_SEND_OK,
  CANIF_SEND_FAIL   
}eCanIf_FrameHandleResultType;

// super class
typedef struct
{
    eCanIf_FrmameStatusType frame_status;
    sCanIf_FrameType frame;
    uWord frame_handle_result_ref;
    void *RAMPTR ptr_next;
} sCanIf_BufferType;

typedef enum
{
    CANIF_RX_BUF,
    CANIF_TX_BUF
} eCanIf_BufferUseType;

typedef enum
{
    CANIF_BUF_EMPTY,
    CANIF_BUF_HAVE_DATA,
    CANIF_BUF_FULL
} eCanIf_BufferStatusType;

//super class
typedef struct
{
    uByte length;
    eCanIf_BufferUseType use;
    eCanIf_BufferStatusType st;
    sCanIf_BufferType *RAMPTR rd_ptr;
    sCanIf_BufferType *RAMPTR wr_ptr;
} sCanIf_BufferHandleType;

typedef enum
{
    INT_CAN_CHANNEL,
    CAR_CAN_CHANNEL,
    CHG_CAN_CHANNEL
} eCanIf_ChannelType;


/*typedef enum
{
    CANIF_WR_HWBUF_OK,
    CANIF_WR_HWBUF_FAIL,
    CANIF_SEND_OK,
    CANIF_SEND_FAIL
} eCanIf_TxBufferHandResultType; */

typedef enum
{
    HARD_BUF1,
    HARD_BUF2,
    HARD_BUF3,
    HARD_BUF_NUM
} eCanIf_HardTxBufNum;


/*typedef struct
{
  uByte frame_no_backup; 
}sCanIf_TxWrBufferRecordType; */

/*typedef struct
{
    uByte frame_no_backup;
    eCanIf_HardTxBufNum hard_tx_buffer_num;
    eCanIf_TxBufferHandResultType result;
} sCanIf_TxHandleRecordType; */

// super class
typedef struct
{
    eCanIf_ChannelType chn;
    sCanIf_BufferType *RAMPTR rxbuf_ptr;
    sCanIf_BufferType *RAMPTR txbuf_ptr;
    sCanIf_BufferHandleType rxhandle;
    sCanIf_BufferHandleType txhandle;    
} sCanIf_ChannelBufType;

typedef struct
{
  uWord tx_rslt_ref[HARD_BUF_NUM]; 
}sCanIf_ChannelHandResultType;
              
typedef struct { 
	void (*RxIndication)( uByte chn,sCanIf_MsgType * msg_ptr);
	void (*ControllerBusOff)(uByte chn);
	void (*TxConfirmation)(uByte chn);
	void (*ControllerWakeup)(uByte chn);
} Can_CallbackType;

typedef struct
{
   uLWord AccCode1;
   uLWord AccCode2;
}sCanIf_AcceptanceCodeType;

typedef struct
{
   uLWord AccMask1;
   uLWord AccMask2;
}sCanIf_AcceptanceMaskType;

#define  CANIF_BAUDRATE_500K  1
#define  CANIF_BAUDRATE_250K  0

#endif /* CANIF_TYPE_H */