// CanIf.c
#include "GeneralTypeDef.h"
#include "CanIf_Type.h"
#include "Can_Lcfg.h"
#include "Can_Cbk.h"
#include "CanIf.h"
#include "Ecu.h"

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

sCanIf_BufferType CanIf_RxBuffer[CAN_CHANNEL_NUM][MAX_BUFFER_LEN];
sCanIf_BufferType CanIf_TxBuffer[CAN_CHANNEL_NUM][MAX_BUFFER_LEN];



/* define buffer operation */
sCanIf_ChannelBufType CanIf_ChannelBuf[] =
{

    // channel for INT_CAN
    {
        INT_CAN_CHANNEL,
        (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[INT_CAN_CHANNEL])), // rxbuf
        (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[INT_CAN_CHANNEL])), // txbuf
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_RX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[INT_CAN_CHANNEL])), //rd_ptr point to rxbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[INT_CAN_CHANNEL])) //wr_ptr point to rxbuf
        },
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_TX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[INT_CAN_CHANNEL])), //rd_ptr point to txbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[INT_CAN_CHANNEL])) //wr_ptr point to txbuf
        },
    },
#ifdef CAR_CAN_EN
    // channel for CAR_CAN
    {
        CAR_CAN_CHANNEL,
        (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CAR_CAN_CHANNEL])), // rxbuf
        (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CAR_CAN_CHANNEL])), // txbuf
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_RX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CAR_CAN_CHANNEL])), //rd_ptr point to rxbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CAR_CAN_CHANNEL])) //wr_ptr point to rxbuf
        },
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_TX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CAR_CAN_CHANNEL])), //rd_ptr point to txbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CAR_CAN_CHANNEL])) //wr_ptr point to txbuf
        },
    },
    
#ifdef CHG_CAN_EN
    // channel for CHG_CAN
    {
        CHG_CAN_CHANNEL,
        (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CHG_CAN_CHANNEL])), // rxbuf
        (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CHG_CAN_CHANNEL])), // txbuf
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_RX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CHG_CAN_CHANNEL])), //rd_ptr point to rxbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_RxBuffer[CHG_CAN_CHANNEL])) //wr_ptr point to rxbuf
        },
        {
            // rxbufer handle
            0, // length which had been used
            CANIF_TX_BUF, // buffer used for
            CANIF_BUF_EMPTY, // buffer had no date
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CHG_CAN_CHANNEL])), //rd_ptr point to txbuf
            (sCanIf_BufferType *RAMPTR)(&(CanIf_TxBuffer[CHG_CAN_CHANNEL])) //wr_ptr point to txbuf
        },
    },
#endif // CHG_CAN_EN
#endif // CAR_CAN_EN 
   


};

sCanIf_ChannelHandResultType CanIf_ChannelHandResult[] =
{
    
    
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
       
};

static void Buf_Init(eCanIf_ChannelType chn)
{
    sCanIf_ChannelBufType *RAMPTR ChnBufPtr;
    uByte i,j;

    ChnBufPtr = &(CanIf_ChannelBuf[chn]);

    // initialate rx buf
    for(i = 0; i < MAX_BUFFER_LEN; i++)
    {
        /* inialate status of frame in buffer */
        (ChnBufPtr->rxbuf_ptr + i)->frame_status = CANIF_BUF_DEFAULT;

        /* inialate frame in buf */
        //(ChnBufPtr->rxbuf_ptr + i)->frame.no = 0;
        (ChnBufPtr->rxbuf_ptr + i)->frame.msg.id = 0;
        (ChnBufPtr->rxbuf_ptr + i)->frame.msg.length = 0;
        for(j = 0; j < 8; j++)
        {
            (ChnBufPtr->rxbuf_ptr + i)->frame.msg.data[j] = 0;
        }

        (ChnBufPtr->rxbuf_ptr + i)->frame_handle_result_ref = 0;

        /* inialate next pointer */
        if(i < MAX_BUFFER_LEN - 1)
        {
            (ChnBufPtr->rxbuf_ptr + i)->ptr_next = (ChnBufPtr->rxbuf_ptr + i + 1);
        }
        else
        {
            // point to first node for cycle list
            (ChnBufPtr->rxbuf_ptr + i)->ptr_next = ChnBufPtr->rxbuf_ptr;
        }
    }

    // initialate tx buf
    for(i = 0; i < MAX_BUFFER_LEN; i++)
    {
        /* inialate status of frame in buffer */
        (ChnBufPtr->rxbuf_ptr + i)->frame_status = CANIF_BUF_DEFAULT;

        /* inialate frame in buf */
        // (ChnBufPtr->txbuf_ptr + i)->frame.no = 0;
        (ChnBufPtr->txbuf_ptr + i)->frame.msg.id = 0;
        (ChnBufPtr->txbuf_ptr + i)->frame.msg.length = 0;
        for(j = 0; j < 8; j++)
        {
            (ChnBufPtr->txbuf_ptr + i)->frame.msg.data[j] = 0;
        }

        (ChnBufPtr->txbuf_ptr + i)->frame_handle_result_ref = 0;

        /* inialate next pointer */
        if(i < MAX_BUFFER_LEN - 1)
        {
            (ChnBufPtr->txbuf_ptr + i)->ptr_next = (ChnBufPtr->txbuf_ptr + i + 1);
        }
        else
        {
            // point to first node for cycle list
            (ChnBufPtr->txbuf_ptr + i)->ptr_next = ChnBufPtr->txbuf_ptr;
        }
    }

    /* for(i = 0; i < HARD_BUF_NUM; i++)
     {
         ChnBufPtr->tx_handle_rcd[i].frame_no_backup = 0;
         ChnBufPtr->tx_handle_rcd[i].hard_tx_buffer_num = HARD_BUF1;
         ChnBufPtr->tx_handle_rcd[i].result = CANIF_SEND_FAIL;
     }*/

}

static GeneralReturnType Wr_FrameToBuf(eCanIf_ChannelType chn,
                           eCanIf_BufferUseType rxOrtx,
                           const sCanIf_FrameType *RAMPTR CanIf_FramePtr,
                           uWord rslt_ref)
{
    // read frame in soft buffer for "TX or RX"
    sCanIf_ChannelBufType *RAMPTR ChnBufPtr;
    sCanIf_BufferHandleType *RAMPTR BufHandlePtr;
    eCanIf_FrmameStatusType  frame_status_bak;
    uByte i;
    GeneralReturnType rslt = ERR_CANIF_HANDLE_ERR;

    ChnBufPtr = &(CanIf_ChannelBuf[chn]);
    if(rxOrtx == CANIF_RX_BUF)
    {
        //get handle pointer
        BufHandlePtr = &(ChnBufPtr->rxhandle);
    }
    else if(rxOrtx == CANIF_TX_BUF)
    {
        BufHandlePtr = &(ChnBufPtr->txhandle);
    }
    else
    {
        return ERR_CANIF_HANDLE_ERR;
    }

    SaveStatusReg();
    // confirm use, if right , go on
    if(rxOrtx == BufHandlePtr->use)
    {
        if(BufHandlePtr->st != CANIF_BUF_FULL)
        {
            // if buffer is not full
            // write data to buffer

            if(CANIF_BUF_LOCK != BufHandlePtr->wr_ptr->frame_status)
            {
                frame_status_bak = BufHandlePtr->wr_ptr->frame_status;
                BufHandlePtr->wr_ptr->frame_status = CANIF_BUF_LOCK; // lock for write

                if(CANIF_BUF_LOCK != frame_status_bak)
                {
                    if(  (CANIF_BUF_DEFAULT == frame_status_bak)
                            || (CANIF_BUF_RD_COMPLT == frame_status_bak))
                        //|| (CANIF_BUF_WR_COMPLT == frame_status_bak))/* old msg can be write overlay by new msg */
                    {
                        BufHandlePtr->wr_ptr->frame_status = CANIF_BUF_WRING;

                        /* pack frame */
                        BufHandlePtr->wr_ptr->frame.type = CanIf_FramePtr->type;
                        BufHandlePtr->wr_ptr->frame.format = CanIf_FramePtr->format;
                        BufHandlePtr->wr_ptr->frame.msg.id = CanIf_FramePtr->msg.id;
                        BufHandlePtr->wr_ptr->frame.msg.length = CanIf_FramePtr->msg.length;
                        for(i = 0; i < 8; i++)
                        {
                            BufHandlePtr->wr_ptr->frame.msg.data[i] = CanIf_FramePtr->msg.data[i];
                        }

                        /* get result pointer */
                        BufHandlePtr->wr_ptr->frame_handle_result_ref = rslt_ref;
                        if((uByte *)rslt_ref != NULL)
                        {
                            *(uByte *)rslt_ref = CANIF_WR_SWBUF_OK;
                        }

                        if(BufHandlePtr->length < MAX_BUFFER_LEN)
                        {
                            BufHandlePtr->length++;
                        }

                        if(BufHandlePtr->length < MAX_BUFFER_LEN)
                        {
                            BufHandlePtr->st = CANIF_BUF_HAVE_DATA;
                        }
                        else
                        {
                            BufHandlePtr->st = CANIF_BUF_FULL;
                        }

                        /* mask ok and release buffer to other handler */
                        BufHandlePtr->wr_ptr->frame_status = CANIF_BUF_WR_COMPLT;

                        /* update write pointer to next buffer */
                        BufHandlePtr->wr_ptr = (sCanIf_BufferType *RAMPTR)(BufHandlePtr->wr_ptr->ptr_next);


                        rslt =  ERR_OK;
                    }
                    else
                    {
                        // recovery last status
                        BufHandlePtr->wr_ptr->frame_status = frame_status_bak;

                        /* get the pointer to the next buffer  */
                        // BufHandlePtr->wr_ptr = (sCanIf_BufferType *)(BufHandlePtr->wr_ptr->ptr_next);


                        rslt = ERR_BUF_LOCKED;
                    }
                }
                else
                {
                    // recovery last status
                    asm NOP;
                    BufHandlePtr->wr_ptr->frame_status = frame_status_bak;

                    /* get the pointer to the next buffer  */
                    //     BufHandlePtr->wr_ptr = (sCanIf_BufferType *)(BufHandlePtr->wr_ptr->ptr_next);

                    rslt = ERR_BUF_LOCKED;
                }
            }//if(CANIF_BUF_LOCK != BufHandlePtr->wr_ptr->frame_status)
            else
            {
                /* get the pointer to the next buffer  */
                // BufHandlePtr->wr_ptr = (sCanIf_BufferType *)(BufHandlePtr->wr_ptr->ptr_next);


                rslt = ERR_BUF_LOCKED;
            }
        } // if(BufHandlePtr->st != CANIF_BUF_FULL)
        else
        {

            rslt = ERR_BUF_FULL;
        }

        /* get the pointer to the next buffer  */
        //BufHandlePtr->wr_ptr = (sCanIf_BufferType *)(BufHandlePtr->wr_ptr->ptr_next);

    }  //if(rxOrtx == BufHandlePtr->use)
    else
    {
        rslt = ERR_BUF_INCONSISTENCY;
    }

    RestoreStatusReg();
    return rslt;
}

static GeneralReturnType Rd_BufToFrame(eCanIf_ChannelType chn,
                           eCanIf_BufferUseType rxOrtx,
                           sCanIf_FrameType * CanIf_FramePtr,
                           uWord * rslt_ref_ptr)
{
    // read frame to frame from buffer for "TX or RX"
    sCanIf_ChannelBufType *RAMPTR ChnBufPtr;
    sCanIf_BufferHandleType *RAMPTR BufHandlePtr;
    eCanIf_FrmameStatusType  frame_status_bak;
    uByte i;
    GeneralReturnType rslt = ERR_CANIF_HANDLE_ERR;


    ChnBufPtr = &(CanIf_ChannelBuf[chn]);
    if(rxOrtx == CANIF_RX_BUF)
    {
        //get handle pointer
        BufHandlePtr = &(ChnBufPtr->rxhandle);
    }
    else if(rxOrtx == CANIF_TX_BUF)
    {
        BufHandlePtr = &(ChnBufPtr->txhandle);
    }
    else
    {
        return ERR_CANIF_HANDLE_ERR;
    }

    SaveStatusReg();
    // confirm use, if right , go on
    if(rxOrtx == BufHandlePtr->use)
    {
        if(BufHandlePtr->st != CANIF_BUF_EMPTY)
        {
            // if buffer is not empty,read one frame
            // read data to frame from buffer
            if(CANIF_BUF_LOCK != BufHandlePtr->rd_ptr->frame_status)
            {
                frame_status_bak = BufHandlePtr->rd_ptr->frame_status;
                BufHandlePtr->rd_ptr->frame_status = CANIF_BUF_LOCK; // lock for read

                if(CANIF_BUF_LOCK != frame_status_bak)
                {
                    if( (CANIF_BUF_WR_COMPLT == frame_status_bak)
                            || (CANIF_BUF_RDING == frame_status_bak) ) // if not read complete,can read again
                        // ||  (CANIF_BUF_RD_COMPLT == frame_status_bak)) // msg can be send again if send fail // can read
                    {
                        BufHandlePtr->rd_ptr->frame_status = CANIF_BUF_RDING;

                        /* get frame from buffer */
                        CanIf_FramePtr->type = BufHandlePtr->rd_ptr->frame.type;
                        CanIf_FramePtr->format = BufHandlePtr->rd_ptr->frame.format;
                        CanIf_FramePtr->msg.id = BufHandlePtr->rd_ptr->frame.msg.id;
                        CanIf_FramePtr->msg.length = BufHandlePtr->rd_ptr->frame.msg.length;
                        for(i = 0; i < 8; i++)
                        {
                            CanIf_FramePtr->msg.data[i] = BufHandlePtr->rd_ptr->frame.msg.data[i];
                        }

                        /* get result pointer */
                        if(rslt_ref_ptr != NULL)
                        {
                            *rslt_ref_ptr = BufHandlePtr->rd_ptr->frame_handle_result_ref;
                        }

                        /* if(BufHandlePtr->length > 0)
                         {
                             BufHandlePtr->length--;
                         }

                         if(BufHandlePtr->length == 0)
                         {
                             BufHandlePtr->st = CANIF_BUF_EMPTY;
                         }
                         else
                         {
                             BufHandlePtr->st = CANIF_BUF_HAVE_DATA;
                         }  */

                        /* mask ok and release buffer to other handler */
                        // BufHandlePtr->rd_ptr->frame_status = CANIF_BUF_RD_COMPLT;

                        /* update read pointer to next buffer */
                        // BufHandlePtr->rd_ptr = (sCanIf_BufferType *)(BufHandlePtr->rd_ptr->ptr_next);

                        rslt =  ERR_OK;
                    }
                    else
                    {
                        // recovery last status
                        BufHandlePtr->rd_ptr->frame_status = frame_status_bak;

                        /* get the pointer to the next buffer  */
                        //  BufHandlePtr->rd_ptr = (sCanIf_BufferType *)(BufHandlePtr->rd_ptr->ptr_next);

                        rslt = ERR_BUF_LOCKED;
                    }
                }
                else
                {
                    // recovery last status
                    asm NOP;
                    BufHandlePtr->rd_ptr->frame_status = frame_status_bak;

                    /* get the pointer to the next buffer  */
                    //     BufHandlePtr->rd_ptr = (sCanIf_BufferType *)(BufHandlePtr->rd_ptr->ptr_next);

                    rslt = ERR_BUF_LOCKED;
                }
            }//if(CANIF_BUF_LOCK != BufHandlePtr->rd_ptr->frame_status)
            else
            {
                /* get the pointer to the next buffer  */
                //  BufHandlePtr->rd_ptr = (sCanIf_BufferType *)(BufHandlePtr->rd_ptr->ptr_next);


                rslt = ERR_BUF_LOCKED;
            }
        } //if(BufHandlePtr->st != CANIF_BUF_EMPTY)
        else
        {

            rslt = ERR_BUF_EMPTY;
        }

        /* get the pointer to the next buffer  */
        //BufHandlePtr->rd_ptr = (sCanIf_BufferType *)(BufHandlePtr->rd_ptr->ptr_next);

    }  //if(rxOrtx == BufHandlePtr->use)
    else
    {
        rslt = ERR_BUF_INCONSISTENCY;
    }

    RestoreStatusReg();
    return rslt;
}

void Rd_BufToFrameCbk(eCanIf_ChannelType chn, eCanIf_BufferUseType rxOrtx)
{
    // if buffer data was used OK ,call this switch to next buffer
    sCanIf_ChannelBufType *RAMPTR ChnBufPtr;
    sCanIf_BufferHandleType *RAMPTR BufHandlePtr;

    ChnBufPtr = &(CanIf_ChannelBuf[chn]);
    if(rxOrtx == CANIF_RX_BUF)
    {
        //get handle pointer
        BufHandlePtr = &(ChnBufPtr->rxhandle);
    }
    else if(rxOrtx == CANIF_TX_BUF)
    {
        BufHandlePtr = &(ChnBufPtr->txhandle);
    }
    else
    {
        return ;
    }

    SaveStatusReg();
    // confirm use, if right , go on
    if(rxOrtx == BufHandlePtr->use)
    {
        if(CANIF_BUF_RDING == BufHandlePtr->rd_ptr->frame_status)
        {
            if(BufHandlePtr->length > 0)
            {
                BufHandlePtr->length--;
            }

            if(BufHandlePtr->length == 0)
            {
                BufHandlePtr->st = CANIF_BUF_EMPTY;
            }
            else
            {
                BufHandlePtr->st = CANIF_BUF_HAVE_DATA;
            }

            /* mask ok and release buffer to other handler */
            BufHandlePtr->rd_ptr->frame_status = CANIF_BUF_RD_COMPLT;

            /* update read pointer to next buffer */
            BufHandlePtr->rd_ptr = (sCanIf_BufferType *RAMPTR)(BufHandlePtr->rd_ptr->ptr_next);

        }

    }
    RestoreStatusReg();
}

#define UPDATA_RSLTPTR(chn,buf_num,rslt_ref) \
  CanIf_ChannelHandResult[chn].tx_rslt_ref[buf_num] = rslt_ref;

/* end of define buffer operation */

void CanIf_Init(eCanIf_ChannelType chn,uByte Baudrate)
{
    const sCan_HandleType * Can_handlePtr;

    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);

    (*Can_handlePtr->Can_CtrlHandleTypePtr->SetCanBaudrate)(Baudrate);
    Buf_Init(chn);

    CanIf_StartSend(chn);
}

void CanIf_StartSend(eCanIf_ChannelType chn)
{
    const sCan_HandleType * Can_handlePtr;

    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);

    (*Can_handlePtr->Can_CtrlHandleTypePtr->CanEnableEvent)();
    (void)(*Can_handlePtr->Can_CtrlHandleTypePtr->CanEnable)();

}

/* extern interface: define msg operations below */
GeneralReturnType CanIf_RequestSend(eCanIf_ChannelType chn, const sCanIf_MsgType * msg_ptr, uWord rslt_ref)
{
    // write msg to soft buffer
    sCanIf_FrameType frame;
    uByte i;
    GeneralReturnType val;

    frame.msg.length = msg_ptr->length;
    frame.msg.id = msg_ptr->id;
    IS_DATA_FRAME(frame.msg.length,frame.type);
    IS_EXTENDED_FORMAT(frame.msg.id,frame.format);

    for(i = 0; i < 8; i++)
    {
        frame.msg.data[i] = msg_ptr->data[i];
    }
    //*rslt_ptr = 1;
    val = Wr_FrameToBuf(chn,CANIF_TX_BUF,&frame,rslt_ref);

    Can_SendTask(chn);
    return val;
}

GeneralReturnType CanIf_RxIndication(eCanIf_ChannelType chn, sCanIf_MsgType * msg_ptr)
{
    // read soft buffer to msg
    sCanIf_FrameType frame;
    uByte i;
    GeneralReturnType val;
    val = Rd_BufToFrame(chn,CANIF_RX_BUF, &frame,NULL);
    if(!val)
    {

        Rd_BufToFrameCbk(chn,CANIF_RX_BUF);
        msg_ptr->id = frame.msg.id;
        msg_ptr->length = frame.msg.length;
        if(frame.msg.length > 8)
        {
            return ERR_MSG_LENGTH_OVER;
        }
        for(i = 0; i < frame.msg.length; i++)
        {
            msg_ptr->data[i] = frame.msg.data[i];
        }

        return ERR_OK;
    }
    return val;
}

uByte CanIf_GetRxBufferLength(eCanIf_ChannelType chn)
{       
    sCanIf_ChannelBufType *RAMPTR ChnBufPtr;
    sCanIf_BufferHandleType *RAMPTR BufHandlePtr;      
    uByte length;
    ChnBufPtr = &(CanIf_ChannelBuf[chn]);     
    BufHandlePtr = &(ChnBufPtr->rxhandle);
    
    SaveStatusReg();
    length = BufHandlePtr->length;
    RestoreStatusReg();
    
    return length;
}

void Can_TxConfirm(eCanIf_ChannelType chn, uWord BufferMask)
{
    uWord BufferMaskBak;
    uByte i;
    BufferMaskBak = BufferMask;

    if(BufferMaskBak & 0x7U)//for s12
    {
        for(i = 0; i < HARD_BUF_NUM; i++)
        {
            if((BufferMaskBak >> i) & 0x1U)
            {
                if( (uByte *)(CanIf_ChannelHandResult[chn].tx_rslt_ref[i]) != NULL)
                {
                    *(uByte *)(CanIf_ChannelHandResult[chn].tx_rslt_ref[i]) = CANIF_SEND_OK;
                }
            }
        }
    }
}

void Can_SendTask(eCanIf_ChannelType chn)
{
    // read soft trismit buffer to hard trismit buffer
    sCanIf_FrameType frame;
    uWord rslt_ref = 0;
    eCanIf_HardTxBufNum hard_buf_num = HARD_BUF1;
    const sCan_HandleType * Can_handlePtr;
    GeneralReturnType val;

    val = ERR_CANIF_HANDLE_ERR;
    Can_handlePtr = &(Can_Handle[chn]);
    val = Rd_BufToFrame(chn,CANIF_TX_BUF,&frame,&rslt_ref);

    if(ERR_OK == val)
    {
        if(CANIF_EXTENDED_FORMAT == frame.format)
        {
            frame.msg.id |= 0x80000000UL;//CAN_EXTENDED_FRAME_ID;
        }

        for(hard_buf_num = HARD_BUF1; hard_buf_num < HARD_BUF_NUM; hard_buf_num++)
        {
            if(!( (*Can_handlePtr->Can_RxTxHandlePtr->SendFramePtr)\
                    ((uByte)hard_buf_num,\
                     frame.msg.id,\
                     (uByte)frame.type,\
                     frame.msg.length,\
                     frame.msg.data) ) )
            {
                // store result pointer for confirmation
                if((uByte *)rslt_ref != NULL)
                {
                    *(uByte *)rslt_ref = CANIF_WR_HWBUF_OK;
                }
                UPDATA_RSLTPTR(chn,hard_buf_num,rslt_ref);

                Rd_BufToFrameCbk(chn,CANIF_TX_BUF);
                break;
            }
        }

    }
}

void Can_ReceiveTask(eCanIf_ChannelType chn)
{
    // called in rx interrupt, had disable interrupt
    // read hard receive buffer to soft receive buffer
    sCanIf_FrameType frame;
    uByte data[16],i;
    uByte type,format;
    GeneralReturnType val = ERR_CANIF_HANDLE_ERR;
    const sCan_HandleType * Can_handlePtr;
    
    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);
    val = (* Can_handlePtr->Can_RxTxHandlePtr->ReadFramePtr)\
          (&(frame.msg.id),\
           &type, \
           &format, \
           &(frame.msg.length),\
           data);
           
    if(frame.msg.length >= 8)    
    {
        for(i = 0 ; i < 8; i++)
        {
            frame.msg.data[i] = data[i];    
        }
    }
    else
    {
        for(i = 0 ; i < frame.msg.length; i++)
        {
            frame.msg.data[i] = data[i];    
        }        
    }
           
    if(ERR_OK == val)
    {
        frame.type = (eCanIf_FrameTypeType)type;
        frame.format = (eCanIf_FrameFormatType)format;
        (void)Wr_FrameToBuf(chn,CANIF_RX_BUF,&frame,0);
    }


}

GeneralReturnType CanIf_SetCanMode(eCanIf_ChannelType chn, eCanIf_CanMOde mode)
{
    const sCan_HandleType * Can_handlePtr;
    GeneralReturnType rslt;
    rslt = ERR_CANIF_HANDLE_ERR;
    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);
    if(Can_handlePtr != NULL)
    {
      (*Can_handlePtr->Can_NmCtrlPtr->WakeAndSleepCtrl)((uByte)mode);  
    }     
    
    return rslt;  
}

static void CanIf_SetAcceptanceCode(eCanIf_ChannelType chn,sCanIf_AcceptanceCodeType *AcceptCodePtr)
{
    const sCan_HandleType * Can_handlePtr;

    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);

    (*Can_handlePtr->Can_CtrlHandleTypePtr->SetAcceptanceCode)(AcceptCodePtr->AccCode1,AcceptCodePtr->AccCode2);
}

static void CanIf_SetAcceptanceMask(eCanIf_ChannelType chn,sCanIf_AcceptanceMaskType *AcceptMaskPtr)
{
    const sCan_HandleType * Can_handlePtr;

    // get hardware handler
    Can_handlePtr = &(Can_Handle[chn]);

    (*Can_handlePtr->Can_CtrlHandleTypePtr->SetAcceptanceMask)(AcceptMaskPtr->AccMask1,AcceptMaskPtr->AccMask2);
}

void CanIf_SetAcceptanceMaskCode(eCanIf_ChannelType chn,sCanIf_AcceptanceCodeType *AcceptCodePtr,sCanIf_AcceptanceMaskType *AcceptMaskPtr)
{
   CanIf_SetAcceptanceCode(chn,AcceptCodePtr);
   CanIf_SetAcceptanceMask(chn,AcceptMaskPtr);
}

void CanIf_AppRx(uLWord CanId, uByte length, uByte *pdata) 
{
    uByte i;
    sCanIf_FrameType frame;
    eCanIf_ChannelType chn;
    
    chn = CAR_CAN_CHANNEL;
    frame.msg.id = CanId;
    frame.msg.length = length; 
    
    if(frame.msg.length >= 8)    
    {
        for(i = 0 ; i < 8; i++)
        {
            frame.msg.data[i] = *(pdata+i);    
        }
    }
    else
    {
        for(i = 0 ; i < frame.msg.length; i++)
        {
            frame.msg.data[i] = *(pdata+i);    
        }        
    }    
    (void)Wr_FrameToBuf(chn,CANIF_RX_BUF,&frame,0);
}


#pragma DATA_SEG DEFAULT






