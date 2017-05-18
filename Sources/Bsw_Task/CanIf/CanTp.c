// CanTp.c
#include "GeneralTypeDef.h"
#include "CanTp_Type.h"
#include "Can_Lcfg.h"
#include "Can_Cbk.h"
#include "CanIf.h"
#include "Ecu.h"
#include "SchIf.h"

#define FLAG_ENABLE  1
#define FLAG_DISABLE 0
#define CR_OVER_TIME_VAL TM_MS(200)
#define BS_OVER_TIME_VAL TM_MS(500)

#define CANTP_OVER_TIME


#if(ECU_RAM_USE_MODE)
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM
#endif
#else
#pragma DATA_SEG DEFAULT
#endif

uByte CanTp_IntCanRxBuffer[MAX_NUM_BUF][MAX_CANTP_BUFFER];
uByte CanTp_IntCanTxBuffer[MAX_NUM_BUF][MAX_CANTP_BUFFER];

static void CanTp_OverTimeProcess(void);

sCanTp_BufType CanTp_Buf[] =
{
    // use for int_can diag
    {
        INT_CAN_CHANNEL,
        {
            TX_BUF,
            0, //MaxContinueFrameCount
            0, // ValidLen
            0, // Id
            &(CanTp_IntCanTxBuffer[DIAG_BUF][0]),
            0 //ContinueFrameTime
        },
        {
            RX_BUF,
            0,   //MaxContinueFrameCount
            0,     // ValidLen
            0,     // Id
            &(CanTp_IntCanRxBuffer[DIAG_BUF][0]),
            0 //ContinueFrameTime
        },
        {
            CANTP_DEFAULT, // BufSt
            0,             // HadExecuteLen
            CANTP_TX_STEP_DEFAULT,
            1,             // ContiuneFrameCounter
            0,             // FollowFrameStatus
            0,             // BsCnt
            0,             // CrCnt
            CANTP_ERR_DEFAULT,      // rslt
            0 //ContinueFrameTimer
        },
        {
            CANTP_DEFAULT, // BufSt
            0,             // HadExecuteLen
            CANTP_TX_STEP_DEFAULT,
            1,             // ContiuneFrameCounter
            0,             // FollowFrameStatus
            0,             // BsCnt
            0,             // CrCnt
            CANTP_ERR_DEFAULT,      // rslt
            0 //ContinueFrameTimer
        }
    },

    // use for int_can balance
    {
        INT_CAN_CHANNEL,
        {
            TX_BUF,
            0, //MaxContinueFrameCount
            0, // ValidLen
            0, // Id
            &(CanTp_IntCanTxBuffer[BALANCE_BUF][0]),
            0 //ContinueFrameTime
        },
        {
            RX_BUF,
            0,  // MaxContinueFrameCount
            0,
            0,
            &(CanTp_IntCanRxBuffer[BALANCE_BUF][0]),
            0 //ContinueFrameTime
        },
        {
            CANTP_DEFAULT, // BufSt
            0,             // HadExecuteLen
            CANTP_TX_STEP_DEFAULT,
            1,             // ContiuneFrameCounter
            0,             // FollowFrameStatus
            0,             // BsCnt
            0,             // CrCnt
            CANTP_ERR_DEFAULT,      // rslt
            0 //ContinueFrameTimer
        },
        {
            CANTP_DEFAULT, // BufSt
            0,             // HadExecuteLen
            CANTP_TX_STEP_DEFAULT,
            1,             // ContiuneFrameCounter
            0,             // FollowFrameStatus
            0,             // BsCnt
            0,             // Crcnt
            CANTP_ERR_DEFAULT,      // rslt
            0 //ContinueFrameTimer
        }
    }
};

void Init_CanTp(void)
{
    uWord lwd_i;
    uByte *RAMPTR p_byte;

    p_byte = (uByte *RAMPTR)(&CanTp_IntCanRxBuffer[0][0]);
    for(lwd_i = 0; lwd_i < sizeof(CanTp_IntCanRxBuffer); lwd_i++)
    {
        *(p_byte++) = 0;
    }

    p_byte = (uByte *RAMPTR)(&CanTp_IntCanTxBuffer[0][0]);
    for(lwd_i = 0; lwd_i < sizeof(CanTp_IntCanTxBuffer); lwd_i++)
    {
        *(p_byte++) = 0;
    }
}

// call by 5ms task
void CanTp_MainTx(void)
{
    uByte lby_i;
    uWord lwd_i;
    uLWord llwd;
    uWord rslt = 0;
    sCanIf_MsgType MsgPtr;
    sCanTp_BufType *RAMPTR BufPtr;

    for(lby_i = 0; lby_i < MAX_NUM_BUF; lby_i++)
    {

        BufPtr = &(CanTp_Buf[lby_i]);
        // Tx follow control frame
        if((CANTP_FOLLOW_SEND == BufPtr->RxCtrl.FollowFrameStatus) && (BufPtr->TxCtrl.BufSt == CANTP_SENDING))
        {
            MsgPtr.id = BufPtr->TxHandle.Id;
            MsgPtr.length = 8; // fixed 8
            MsgPtr.data[0] = *(BufPtr->TxHandle.DataPtr++);
            MsgPtr.data[1] = *(BufPtr->TxHandle.DataPtr++);
            MsgPtr.data[2] = *(BufPtr->TxHandle.DataPtr++);
            MsgPtr.data[3] = 0;
            MsgPtr.data[4] = 0;
            MsgPtr.data[5] = 0;
            MsgPtr.data[6] = 0;
            MsgPtr.data[7] = 0;

            // send data
            if(ERR_OK == CanIf_RequestSend(BufPtr->Chn, &MsgPtr, rslt))
            {
                BufPtr->RxCtrl.FollowFrameStatus = CANTP_FOLLOW_FRAME_DFAUL;
                BufPtr->TxCtrl.BufSt = CANTP_SENDCPT;

                // set CRCnt timer start
                BufPtr->RxCtrl.CRCntEn = FLAG_ENABLE;
                BufPtr->RxCtrl.CRCnt = 0;
            }
        }

        // Tx long data
        //comfir status
        if(BufPtr->TxCtrl.BufSt == CANTP_SENDING)
        {
            if(CANTP_TX_STEP_DEFAULT == BufPtr->TxCtrl.ExecuteStep)
            {
                if(BufPtr->TxHandle.ValidLen <= 7)
                {
                    BufPtr->TxCtrl.ExecuteStep = CANTP_TX_SNGL_FRAME;   // sigle frame
                }
                else
                {
                    BufPtr->TxCtrl.ExecuteStep = CANTP_TX_FIRST_FRAME; //
                }
            }

            switch(BufPtr->TxCtrl.ExecuteStep)
            {
            case CANTP_TX_SNGL_FRAME:
            {
                MsgPtr.id = BufPtr->TxHandle.Id;
                MsgPtr.length = 8; // fixed 8
                MsgPtr.data[0] = 0;
                MsgPtr.data[0] += BufPtr->TxHandle.ValidLen;
                MsgPtr.data[0] &= 0x0F;  // sigle frame format
                for(lwd_i = 1; lwd_i < 8; lwd_i++)
                {
                    MsgPtr.data[lwd_i] = *((BufPtr->TxHandle.DataPtr)++);
                }

                // send data

                if(ERR_OK == CanIf_RequestSend(INT_CAN_CHANNEL,&MsgPtr, rslt))
                {
                    BufPtr->TxCtrl.BufSt = CANTP_SENDCPT; // send OK
                }

                break;
            }
            case CANTP_TX_FIRST_FRAME:
            {
                MsgPtr.id = BufPtr->TxHandle.Id;
                MsgPtr.length = 8; // fixed 8
                MsgPtr.data[0] = 0;
                MsgPtr.data[1] = (uByte)(BufPtr->TxHandle.ValidLen);
                MsgPtr.data[0] += (uByte)((BufPtr->TxHandle.ValidLen & 0x0F00) >> 8);
                MsgPtr.data[0] &= 0x0F;
                MsgPtr.data[0] |= 0x10;  // first frame format

                for(lwd_i = 2; lwd_i < 8; lwd_i++)
                {
                    MsgPtr.data[lwd_i] = *(BufPtr->TxHandle.DataPtr++);
                }

                if(ERR_OK == CanIf_RequestSend(BufPtr->Chn, &MsgPtr, rslt))
                {
                    BufPtr->TxCtrl.HadExecuteLen = 6;
                    BufPtr->TxCtrl.ContinueFrameCouter = 1;
                    BufPtr->TxCtrl.ExecuteStep = CANTP_WAIT_FOLLOW_FRAME;
                }
                else
                {
                    BufPtr->TxHandle.DataPtr -= 6;
                }
                BufPtr->TxCtrl.BufSt = CANTP_SENDING;
                break;
            }
            case CANTP_WAIT_FOLLOW_FRAME:
            {
                // set BSCnt Timer start
                BufPtr->TxCtrl.BSCntEn = FLAG_ENABLE;

                // broadcase frame ,do not wait follow control frame
                if((BufPtr->TxHandle.Id & 0x0000FF00 ) == 0x00003F00)
                {
                    BufPtr->TxCtrl.ExecuteStep = CANTP_TX_CONTINUE_FRAME;
                }

                if(CANTP_FOLLOW_RECV == BufPtr->TxCtrl.FollowFrameStatus)
                {

                    BufPtr->TxCtrl.FollowFrameStatus = CANTP_FOLLOW_FRAME_DFAUL;
                    BufPtr->TxCtrl.ExecuteStep = CANTP_TX_CONTINUE_FRAME;

                    // set continue frame starte counter
                    BufPtr->TxCtrl.ContinueFrameCouter = 1;

                    // clear over time cnt
                    BufPtr->TxCtrl.BSCnt = 0;
                    // set BSCnt Timer stop
                    BufPtr->TxCtrl.BSCntEn = FLAG_DISABLE;
                }
                BufPtr->TxCtrl.ContinueFrameTimer = 0;
                break;
            }
            case CANTP_TX_CONTINUE_FRAME:
            {
                BufPtr->TxCtrl.ContinueFrameTimer++;

                if(BufPtr->TxCtrl.ContinueFrameTimer >= BufPtr->TxHandle.ContinueFrameTime )
                {
                    BufPtr->TxCtrl.ContinueFrameTimer = 0;

                    if((BufPtr->TxCtrl.HadExecuteLen + 7) < BufPtr->TxHandle.ValidLen)
                    {
                        MsgPtr.id = BufPtr->TxHandle.Id;
                        MsgPtr.length = 8; // fixed 8
                        MsgPtr.data[0] = 0;
                        MsgPtr.data[0] |= 0x20;  // continue frame format

                        MsgPtr.data[0] |= (BufPtr->TxCtrl.ContinueFrameCouter++);
                        if(BufPtr->TxCtrl.ContinueFrameCouter >= 15)
                        {
                            BufPtr->TxCtrl.ContinueFrameCouter = 0;
                        }

                        for(lwd_i = 1; lwd_i < 8; lwd_i++)
                        {
                            MsgPtr.data[lwd_i] = *((BufPtr->TxHandle.DataPtr)++);
                        }

                        if(ERR_OK == CanIf_RequestSend(BufPtr->Chn, &MsgPtr, rslt))
                        {
                            BufPtr->TxCtrl.HadExecuteLen += 7;
                        }
                        else
                        {
                            BufPtr->TxHandle.DataPtr -= 7;
                        }

                        BufPtr->TxCtrl.BufSt = CANTP_SENDING;
                    }
                    else
                    {
                        MsgPtr.id = BufPtr->TxHandle.Id;
                        MsgPtr.length = 8; // fixed 8
                        MsgPtr.data[0] = 0;
                        MsgPtr.data[0] |= 0x20;  // continue frame format

                        MsgPtr.data[0] |= (BufPtr->TxCtrl.ContinueFrameCouter++);
                        if(BufPtr->TxCtrl.ContinueFrameCouter >= 15)
                        {
                            BufPtr->TxCtrl.ContinueFrameCouter = 0;
                        }

                        for(lwd_i = 1; lwd_i <= (BufPtr->TxHandle.ValidLen - BufPtr->TxCtrl.HadExecuteLen); lwd_i++)
                        {
                            MsgPtr.data[lwd_i] = *((BufPtr->TxHandle.DataPtr)++);
                        }
                        for(lwd_i = (BufPtr->TxHandle.ValidLen - BufPtr->TxCtrl.HadExecuteLen +1) ; lwd_i < 8; lwd_i++)
                        {
                            MsgPtr.data[lwd_i] = 0;
                        }
                        if(ERR_OK == CanIf_RequestSend(BufPtr->Chn, &MsgPtr, rslt))
                        {
                            // data send complete
                            BufPtr->TxCtrl.BufSt = CANTP_SENDCPT;
                            BufPtr->TxCtrl.HadExecuteLen = 0;
                        }

                    }
                }

                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

#ifdef CANTP_OVER_TIME
    CanTp_OverTimeProcess();
#endif
}

GeneralReturnType CanTp_CopyDataToBuf(eCanIf_ChannelType chn, sCanTp_MsgType * CanTpMsgPtr)
{
    uWord lwd_i;
    uByte lby_i;
    GeneralReturnType rslt;
    sCanTp_BufHandleType *RAMPTR BufHandlePtr;
    sCanTp_BufType *RAMPTR BufPtr;
    sCanTp_CtrlType *RAMPTR CtrlPtr;

    rslt = ERR_CANIF_HANDLE_ERR;

    for(lby_i = 0; lby_i < MAX_NUM_BUF; lby_i++)
    {
        BufPtr =  &(CanTp_Buf[lby_i]);
        BufHandlePtr = &(CanTp_Buf[lby_i].TxHandle);
        CtrlPtr = &(CanTp_Buf[lby_i].TxCtrl);

        // can channel err
        if(INT_CAN_CHANNEL != BufPtr->Chn)
        {
            continue;
        }

        // comfire direction
        if(TX_BUF != BufHandlePtr->TxOrRx)
        {
            continue;
        }

        // confire len
        if(CanTpMsgPtr->DataLen > MAX_CANTP_BUFFER)
        {
            continue;
        }

        // comfir TxBuffer status ,when buffer is sending data ,can not write data to buffer
        if(CANTP_SENDING == CtrlPtr->BufSt)
        {
            if(BufHandlePtr->Id == CanTpMsgPtr->Id)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            // load data
            BufHandlePtr->Id = CanTpMsgPtr->Id;
            BufHandlePtr->ValidLen = CanTpMsgPtr->DataLen;

            // reset DataPtr
            BufHandlePtr->DataPtr = (uByte *RAMPTR)(&CanTp_IntCanTxBuffer[lby_i][0]);

            for(lwd_i = 0; lwd_i < CanTpMsgPtr->DataLen; lwd_i++)
            {
                *(BufHandlePtr->DataPtr++) = CanTpMsgPtr->Data[lwd_i];
            }

            // set tp send control parameter
            CtrlPtr->BSCnt= 0;

            CtrlPtr->HadExecuteLen = 0;

            // reset DataPtr
            BufHandlePtr->DataPtr = (uByte *RAMPTR)(&CanTp_IntCanTxBuffer[lby_i][0]);

            CtrlPtr->ExecuteStep = CANTP_TX_STEP_DEFAULT;
            CtrlPtr->BufSt = CANTP_SENDING;  // start  send data

//            CanTp_MainTx();

            rslt = ERR_OK;
            return rslt;
        }
    }
    if(MAX_NUM_BUF == lby_i)
    {
        rslt = ERR_CANTP_OVER_BUF;
    }
    return rslt;
}


GeneralReturnType CanTp_EventRx(eCanIf_ChannelType chn, sCanIf_MsgType* MsgPtr, sCanTp_MsgType * CanTpMsgPtr)
{
    uByte lby_i;
    uByte lby_buf;
    GeneralReturnType rslt;
    uLWord llwd;
    uWord wtmp;
    uByte btmp;
    sCanTp_BufHandleType *RAMPTR BufHandlePtr;
    sCanTp_BufType *RAMPTR BufPtr;
    sCanTp_CtrlType *RAMPTR CtrlPtr;
    sCanTp_MsgType  FollowFrame;

    rslt = ERR_CANIF_HANDLE_ERR;

    // receive follow control frame
    if(3 == ((MsgPtr->data[0] & 0xF0) >> 4))
    {
        for(lby_buf = 0; lby_buf < MAX_NUM_BUF; lby_buf++)
        {
            BufPtr =  &(CanTp_Buf[lby_buf]);
            BufHandlePtr = &(CanTp_Buf[lby_buf].TxHandle);
            CtrlPtr = &(CanTp_Buf[lby_buf].TxCtrl);

            // continue parameter
            BufHandlePtr->ContinueFrameTime = MsgPtr->data[2] / 5;

            wtmp = (uWord)(MsgPtr->id& 0x0000FFFF);
            btmp = (uByte)(wtmp >> 8);
            wtmp <<= 8;
            wtmp &= 0xFF00;
            wtmp |= btmp;
            // comfirm id

            llwd = (uLWord)wtmp;
            llwd += MsgPtr->id & 0xFFFF0000;
            if(llwd == BufHandlePtr->Id)
            {
                CtrlPtr->FollowFrameStatus = CANTP_FOLLOW_RECV;
                rslt = ERR_CANTP_RX_FOLLOW_CTRL;
            }
        }
    }
    else
    {
        for(lby_buf = 0; lby_buf < MAX_NUM_BUF; lby_buf++)
        {
            BufPtr =  &(CanTp_Buf[lby_buf]);
            BufHandlePtr = &(CanTp_Buf[lby_buf].RxHandle);
            CtrlPtr = &(CanTp_Buf[lby_buf].RxCtrl);

            // confirm can channel
            if(INT_CAN_CHANNEL != BufPtr->Chn)
            {
                continue;
            }

            // comfirm direction
            if(RX_BUF != BufHandlePtr->TxOrRx)
            {
                continue;
            }
            if(lby_buf == 1)
            {
                llwd = 0;
            }

            if(CtrlPtr->BufSt == CANTP_RECVING)
            {
                // comfirm continue frame
                if(2 == ((MsgPtr->data[0] & 0xF0) >> 4))
                {
                    if(MsgPtr->id != BufHandlePtr->Id)
                    {
                        continue;
                    }

                    CtrlPtr->ExecuteStep = CANTP_RX_CONTIUNE_FRAME;
                    CtrlPtr->CRCnt = 0;
                }
                // comfirm first frame
                else if(1 == ((MsgPtr->data[0] & 0xF0) >> 4))
                {
                    // check weather same id frame
                    if(MsgPtr->id == BufHandlePtr->Id)
                    {
                        // send follow control frame again
                        if(CANTP_FOLLOW_FRAME_DFAUL == CtrlPtr->FollowFrameStatus)
                        {

                            wtmp = (uWord)(MsgPtr->id& 0x0000FFFF);
                            btmp = (uByte)(wtmp >> 8);
                            wtmp <<= 8;
                            wtmp &= 0xFF00;
                            wtmp |= btmp;


                            llwd = (uLWord)wtmp;

                            llwd += MsgPtr->id & 0xFFFF0000;

                            FollowFrame.Id = llwd;
                            FollowFrame.DataLen = 8;
                            FollowFrame.Data[0] = 0x30;  // follow control format
                            FollowFrame.Data[1] = 0;
                            FollowFrame.Data[2] = 0x14;
                            CtrlPtr->FollowFrameStatus = CANTP_FOLLOW_SEND;

                            if(CANTP_ERR_OK == CanTp_CopyDataToBuf(chn, &FollowFrame))
                            {
                                CtrlPtr->FollowFrameStatus = CANTP_FOLLOW_FRAME_DFAUL;
                            }
                        }

                    }
                    break;
                }
                else
                {
                    // check next buff
                    continue;
                }
            }
            else
            {
                // bakeup id
                BufHandlePtr->Id = MsgPtr->id;

                // reset HadExecuteLen
                CtrlPtr->HadExecuteLen = 0;

                // reset DataPtr
                BufHandlePtr->DataPtr = &(CanTp_IntCanRxBuffer[lby_buf][0]);

                // comfirm sigle frame
                if(0 == ((MsgPtr->data[0] & 0xF0) >> 4))
                {
                    CtrlPtr->ExecuteStep = CANTP_RX_SNGL_FRAME;
                }

                //comfirm fist frame
                else if(1 == ((MsgPtr->data[0] & 0xF0) >> 4))
                {
                    CtrlPtr->ExecuteStep = CANTP_RX_FIRST_FRAME;
                }

                // comfirm followe control frame
                else if(3 == ((MsgPtr->data[0] & 0xF0) >> 4))
                {
                    CtrlPtr->ExecuteStep = CANTP_RX_FOLLOW_FRAME;
                }

            }

            switch(CtrlPtr->ExecuteStep)
            {
            case CANTP_RX_SNGL_FRAME:
            {
                for(lby_i = 0; lby_i < MsgPtr->length - 1; lby_i++)
                {
                    CanTpMsgPtr->Data[lby_i] = MsgPtr->data[lby_i+1];
                }
                CanTpMsgPtr->DataLen = MsgPtr->length - 1;
                CanTpMsgPtr->Id = MsgPtr->id;

                // reset HadExecuteLen
                CtrlPtr->HadExecuteLen = 0;

                // reset DataPtr
                BufHandlePtr->DataPtr = &(CanTp_IntCanRxBuffer[lby_buf][0]);
                CtrlPtr->BufSt = CANTP_RECVCPT;

                rslt = ERR_OK;
                return rslt;

                break;
            }
            case CANTP_RX_FIRST_FRAME:
            {
                // valid data len
                BufHandlePtr->ValidLen = MsgPtr->data[1];
                BufHandlePtr->ValidLen += ((MsgPtr->data[0] & 0x0F) << 8);

                // over max len
                if(BufHandlePtr->ValidLen >= 0xEFF)
                {
                    rslt = ERR_CANTP_OVER_BUF;
                    CtrlPtr->CRCntEn = FLAG_DISABLE;
                    CtrlPtr->CRCnt = 0;
                    return rslt;
                }

                // comfirm ValiLen
                if(BufHandlePtr->ValidLen > 7)
                {
                    // max continue frame
                    BufHandlePtr->MaxContinueFrameCount = (BufHandlePtr->ValidLen - 6) / 7;
                    if((BufHandlePtr->ValidLen - 6) % 7)
                    {
                        BufHandlePtr->MaxContinueFrameCount += 1;
                    }

                    // first frame include 6 valid bytes
                    CtrlPtr->HadExecuteLen = 6;
                    for(lby_i = 0; lby_i < 6; lby_i++)
                    {
                        *(BufHandlePtr->DataPtr++) = MsgPtr->data[lby_i+2];
                    }

                    // set continune frame start rx counter
                    CtrlPtr->ContinueFrameCouter = 1;

                    // send follow control frame
                    // receive broadcast do not send follow control frame
                    if((MsgPtr->id & 0x00003F00) != 0x00003F00)
                    {

                        if(CANTP_FOLLOW_FRAME_DFAUL == CtrlPtr->FollowFrameStatus)
                        {

                            wtmp = (uWord)(MsgPtr->id& 0x0000FFFF);
                            btmp = (uByte)(wtmp >> 8);
                            wtmp <<= 8;
                            wtmp &= 0xFF00;
                            wtmp |= btmp;
                            // comfirm id

                            llwd = (uLWord)wtmp;

                            llwd += MsgPtr->id & 0xFFFF0000;

                            FollowFrame.Id = llwd;
                            FollowFrame.DataLen = 8;
                            FollowFrame.Data[0] = 0x30;  // follow control format
                            FollowFrame.Data[1] = 0;
                            FollowFrame.Data[2] = 0x14;
                            CtrlPtr->FollowFrameStatus = CANTP_FOLLOW_SEND;

                            if(CANTP_ERR_OK == CanTp_CopyDataToBuf(chn, &FollowFrame))
                            {
                                CtrlPtr->FollowFrameStatus = CANTP_FOLLOW_FRAME_DFAUL;
                            }
                        }
                    }
                    CtrlPtr->BufSt = CANTP_RECVING;
                }

                rslt = ERR_CANTP_RX_FIRST_FRAME;
                return rslt;

                break;
            }
            case CANTP_RX_CONTIUNE_FRAME:
            {
                // comfirm rx contiune frame counter
                if((MsgPtr->data[0] & 0x0F) == CtrlPtr->ContinueFrameCouter )
                {
                    CtrlPtr->ContinueFrameCouter++;
                    if(CtrlPtr->ContinueFrameCouter > 15)
                    {
                        CtrlPtr->ContinueFrameCouter = 0;
                    }
                    // copy data to buff
                    if((CtrlPtr->HadExecuteLen + (MsgPtr->length - 1)) < BufHandlePtr->ValidLen)
                    {
                        CtrlPtr->HadExecuteLen += (MsgPtr->length - 1);
                        for(lby_i = 0; lby_i < MsgPtr->length - 1; lby_i++)
                        {
                            *(BufHandlePtr->DataPtr++) = MsgPtr->data[lby_i+1];
                        }

                        CtrlPtr->BufSt = CANTP_RECVING;

                        rslt = ERR_CANTP_RX_CONTINUE_FRAME;
                        return rslt;
                    }

                    // had receive all data
                    else
                    {
                        for(lby_i = 0; lby_i < (BufHandlePtr->ValidLen - CtrlPtr->HadExecuteLen); lby_i++)
                        {
                            *(BufHandlePtr->DataPtr++) = MsgPtr->data[lby_i+1];
                        }

                        // reset DataPtr
                        BufHandlePtr->DataPtr = &(CanTp_IntCanRxBuffer[lby_buf][0]);

                        // read all data
                        for(lby_i = 0; lby_i < BufHandlePtr->ValidLen; lby_i++)
                        {
                            CanTpMsgPtr->Data[lby_i] = *(BufHandlePtr->DataPtr++);
                        }
                        CanTpMsgPtr->DataLen = BufHandlePtr->ValidLen;
                        CanTpMsgPtr->Id = MsgPtr->id;

                        // reset BufHandlePtr->DataPtr
                        BufHandlePtr->DataPtr = &(CanTp_IntCanRxBuffer[lby_buf][0]);
                        CtrlPtr->BufSt = CANTP_RECVCPT;

                        // had receive all data stop CRCnt timer;
                        CtrlPtr->CRCntEn = FLAG_DISABLE;
                        CtrlPtr->CRCnt = 0;

                        rslt = ERR_OK;
                        return rslt;
                    }
                }
                break;
            }
            default:
                break;
            } // end switch
        }
    }

    if(MAX_NUM_BUF == lby_buf)
    {
        rslt = ERR_CANTP_OVER_BUF;
        return rslt;
    }
    return rslt ;
}

static void CanTp_OverTimeProcess(void)
{
    sCanTp_BufType *RAMPTR  BufPtr;
    uByte i;

    for(i = 0; i < MAX_NUM_BUF; i++)
    {
        BufPtr = &CanTp_Buf[i];

        // check BS time over
        if(FLAG_ENABLE == BufPtr->TxCtrl.BSCntEn)
        {
            BufPtr->TxCtrl.BSCnt++;
            if(BufPtr->TxCtrl.BSCnt >= BS_OVER_TIME_VAL)
            {
                BufPtr->TxCtrl.BSCnt = 0;
                BufPtr->TxCtrl.BSCntEn = FLAG_DISABLE;

                // release tx buff
                BufPtr->TxCtrl.BufSt = CANTP_DEFAULT;
            }
        }
        else
        {
            BufPtr->TxCtrl.BSCnt = 0;
        }

        //check CR time over
        if(FLAG_ENABLE == BufPtr->RxCtrl.CRCntEn)
        {
            BufPtr->RxCtrl.CRCnt++;
            if(BufPtr->RxCtrl.CRCnt >= CR_OVER_TIME_VAL)
            {
                BufPtr->RxCtrl.CRCnt = 0;
                BufPtr->RxCtrl.CRCntEn = FLAG_DISABLE;

                // release rx buff
                BufPtr->RxCtrl.BufSt = CANTP_DEFAULT;
            }
        }
        else
        {
            BufPtr->RxCtrl.CRCnt = 0;
        }
    }
}

#pragma DATA_SEG DEFAULT