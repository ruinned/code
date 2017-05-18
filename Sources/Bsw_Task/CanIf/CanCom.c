// Can_Com.c
#include "GeneralTypeDef.h"
#include "CanCom_Type.h"
#include "CanIf.h"
#include "Ecu.h"
#include "SchIf.h"
#include "wdgm.h"
#include "DataLayer.h" 
#ifdef CAR_CAN_EN
#include "CarCan.h"
#endif
#ifdef CHG_CAN_EN
#include "ChgCan.h"
#endif



void CCom_Init(void)
{
    uWord lwd_i;
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;
    uLWord sa;

    /* initial internal CAN */
    CanIf_Init(INT_CAN_CHANNEL);

#ifdef CAR_CAN_EN
    /* initial car CAN */
    CanIf_Init(CAR_CAN_CHANNEL);
#endif

#ifdef CHG_CAN_EN
    /* initial charge CAN */
    CanIf_Init(CHG_CAN_CHANNEL);
#endif

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
       sa = (uLWord)Ecu_GetEcuSA();  
#if (ECU_CATEGORY == M_BMU_R)
    /* Trig Event and cycle Info 1,  */  
    (void)CCom_TxEnablePdu(7);
#endif            
#endif

    /* initial Msg in Pdu, and tx and rx control parameter */
    for(lwd_i = 0; !((CCom_Config.CCom_PduPtr + lwd_i)->FlgEndOfList); lwd_i++)
    {
        PduPtr =  GET_PDU(lwd_i);
        PduTxCtrlPtr = GET_PDUTXCTRL(lwd_i);

        // confirmation id by config
        if(PduPtr->PduHandleId == lwd_i)
        {
            /* initial Msg in Pdu */
            /* If ECU is Bmu or Bmu_R, need modify SA by ECU Config */
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))             
            if(sa > 0x1L) // default SA = 1
            {
               PduPtr->PduMsgPtr->id &= 0xFFFFFF00;
               PduPtr->PduMsgPtr->id += sa;
            }
#endif
            /* initial tx and rx control parameter */
            if(PduTxCtrlPtr != NULL)
            {
                if(PduPtr->PduDirection == SEND)
                {

                    if(PduTxCtrlPtr->TxStartedFlag == STARTED) /* Had started,can copy date to  */
                    {
                        // set control paremeter
                        if(PduPtr->PduTxPara.sTxMode.eTxModeMode == TX_MIXED)
                        {
                            if(PduTxCtrlPtr->TxNumberOfRepetitionsCnt == 0)
                            {
                                // initial == 0, and can update when complete send
                                PduTxCtrlPtr->TxNumberOfRepetitionsCnt = PduPtr->PduTxPara.sTxMode.TxModeNumberOfRepetitionsFactor;
                            }
                            PduTxCtrlPtr->TxRepetitionPeriodCnt = PduPtr->PduTxPara.sTxMode.TxModeRepetitionPeriodFactor / MIN_TIMER_MS;
                        }
                        else if(PduPtr->PduTxPara.sTxMode.eTxModeMode == TX_PERIODIC)
                        {
                            PduTxCtrlPtr->TxTimePeriodCnt = PduPtr->PduTxPara.sTxMode.TxModeTimePeriodFactor / MIN_TIMER_MS; // first set to default

                            if(PduTxCtrlPtr->TxChangeTimePeriodFactor > 0)
                            {
                                // cycle had modify, use new cycle
                                PduTxCtrlPtr->TxTimePeriodCnt = PduTxCtrlPtr->TxChangeTimePeriodFactor / MIN_TIMER_MS;
                            }

                            if(1 == PduTxCtrlPtr->TxIsFirst.TxFirstFlag)
                            {
                                // set fast send cycle for first send
                                PduTxCtrlPtr->TxIsFirst.TxFirstFlag = 0; // clear first flag
                                PduTxCtrlPtr->TxTimePeriodCnt = PduTxCtrlPtr->TxIsFirst.TxFirstTimePeriodFactor / MIN_TIMER_MS;
                            }
                        }
                        else
                        {
                            PduTxCtrlPtr->TxNumberOfRepetitionsCnt = 0;
                            PduTxCtrlPtr->TxRepetitionPeriodCnt = 0;
                            PduTxCtrlPtr->TxTimePeriodCnt = 0;
                        }
                    }
                }
            }
        }
    }
}

uByte CCom_CopyTxDataToPdu(CCom_PduIdType Pdu_Id, sCCOM_PduInfoType * PduInfoPtr)
{
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;
    uByte rslt;
    uByte i;
    rslt = ERR_OK;
    PduPtr =  GET_PDU(Pdu_Id);
    PduTxCtrlPtr = GET_PDUTXCTRL(Pdu_Id);
    // check Pdu_Id
    if((PduPtr->PduHandleId != Pdu_Id) || (PduPtr->PduDirection != SEND))
    {
        return ERR_FAILED;
    }

    SaveStatusReg();
    if((PduInfoPtr != NULL)
            && (PduPtr->PduMsgPtr != NULL)
            &&(PduTxCtrlPtr != NULL) )
    {
        if(PduTxCtrlPtr->TxStartedFlag == STARTED) /* Had started,can copy date to  */
        {
            if(PduInfoPtr->SduLength <= PduPtr->PduSize)
            {
                PduPtr->PduMsgPtr->length = PduPtr->PduSize;
                for(i = 0; i < PduInfoPtr->SduLength; i++)
                {
                    PduPtr->PduMsgPtr->data[i] = PduInfoPtr->SduDataPtr[i];
                }
            }
            else
            {
                rslt = ERR_RANGE;
            }
        }
        else
        {
            rslt = ERR_TXFULL;
        }
    }
    else
    {
        rslt =  ERR_FAILED;
    }
    RestoreStatusReg();

    return rslt;

}


uByte CCom_UpdateTxCycle(CCom_PduIdType Pdu_Id,uLWord Cycle)
{
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;
    uByte rslt;

    rslt = ERR_FAILED;
    PduPtr =  GET_PDU(Pdu_Id);
    PduTxCtrlPtr = GET_PDUTXCTRL(Pdu_Id);
    // check Pdu_Id
    if((PduPtr->PduHandleId != Pdu_Id) || (PduPtr->PduDirection != SEND))
    {
        return ERR_FAILED;
    }

    SaveStatusReg();
    if(PERIOD_CHANGE_ENABLE == PduPtr->PduTxPara.sTxMode.TxModePeriodChangeEnable)
    {
        PduTxCtrlPtr->TxChangeTimePeriodFactor = Cycle;

        rslt = ERR_OK;
    }
    RestoreStatusReg();

    return rslt;
}

uByte CCom_TxEnablePdu(CCom_PduIdType Pdu_Id)
{
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;
    uByte rslt;

    rslt = ERR_FAILED;
    PduPtr =  GET_PDU(Pdu_Id);
    PduTxCtrlPtr = GET_PDUTXCTRL(Pdu_Id);
    // check Pdu_Id
    if((PduPtr->PduHandleId != Pdu_Id) || (PduPtr->PduDirection != SEND))
    {
        return ERR_FAILED;
    }

    SaveStatusReg();
    if(PduTxCtrlPtr->TxStartedFlag == STOPED)
    {
        PduTxCtrlPtr->TxStartedFlag = STARTED;
        PduTxCtrlPtr->TxResult = CANIF_NULL;
        rslt = ERR_OK;
    }
    RestoreStatusReg();

    return rslt;
}

uByte CCom_TxDisablePdu(CCom_PduIdType Pdu_Id)
{
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;
    uByte rslt;

    rslt = ERR_FAILED;
    PduPtr =  GET_PDU(Pdu_Id);
    PduTxCtrlPtr = GET_PDUTXCTRL(Pdu_Id);
    // check Pdu_Id
    if((PduPtr->PduHandleId != Pdu_Id) || (PduPtr->PduDirection != SEND))
    {
        return ERR_FAILED;
    }

    SaveStatusReg();
    PduTxCtrlPtr->TxStartedFlag = STOPED;
    PduTxCtrlPtr->TxResult = CANIF_NULL;
    rslt = ERR_OK;

    RestoreStatusReg();

    return rslt;
}

// call by 5ms task
void CCom_MainTx(void)
{
    uWord lwd_i;
    const sCCom_PduType *PduPtr;
    sCCom_PduTxCtrlType *PduTxCtrlPtr;

    for(lwd_i = 0; !((CCom_Config.CCom_PduPtr + lwd_i)->FlgEndOfList); lwd_i++)
    {
        PduPtr =  GET_PDU(lwd_i);
        PduTxCtrlPtr = GET_PDUTXCTRL(lwd_i);

        // confirmation id by config
        if(PduPtr->PduHandleId == lwd_i)
        {
            // confirmation pdu Direction by config
            if(PduPtr->PduDirection == SEND)
            {

                switch(PduPtr->PduTxPara.sTxMode.eTxModeMode)
                {
                case TX_PERIODIC:
                {
                    if(CANIF_SEND_OK == PduTxCtrlPtr->TxResult)
                    {
                        // clear result for next check

                        // do nothing
                    }

                    if(STARTED == PduTxCtrlPtr->TxStartedFlag)
                    {

                        if(PduTxCtrlPtr->TxTimePeriodCnt > 0)
                        {
                            PduTxCtrlPtr->TxTimePeriodCnt--;
                        }
                        if(0 == PduTxCtrlPtr->TxTimePeriodCnt)
                        {
                            // timer reached, request send
                            if(PduPtr->PduMsgPtr != NULL)  // prevent msg ptr config error
                            {
                                if(!CanIf_RequestSend(PduPtr->PduCanIfChn,PduPtr->PduMsgPtr,(uWord)(&PduTxCtrlPtr->TxResult)))
                                {
                                    SaveStatusReg();

                                    PduTxCtrlPtr->TxTimePeriodCnt = PduPtr->PduTxPara.sTxMode.TxModeTimePeriodFactor / MIN_TIMER_MS; // first set to default

                                    if(PduTxCtrlPtr->TxChangeTimePeriodFactor > 0)
                                    {
                                        // cycle had modify, use new cycle
                                        PduTxCtrlPtr->TxTimePeriodCnt = PduTxCtrlPtr->TxChangeTimePeriodFactor / MIN_TIMER_MS;
                                    }

                                    if(1 == PduTxCtrlPtr->TxIsFirst.TxFirstFlag)
                                    {
                                        // set fast send cycle for first send
                                        PduTxCtrlPtr->TxIsFirst.TxFirstFlag = 0; // clear first flag
                                        PduTxCtrlPtr->TxTimePeriodCnt = PduTxCtrlPtr->TxIsFirst.TxFirstTimePeriodFactor / MIN_TIMER_MS;
                                    }

                                    RestoreStatusReg();
                                }
                            }
                        }
                    }




                    break;
                }

                /*
                 * Deal DIRECT mode pdu, When trig send, immediate send one times
                 * No use
                 */
                case TX_DIRECT:
                {
                    // check whether send ok
                    if(CANIF_SEND_OK == PduTxCtrlPtr->TxResult)
                    {
                        // clear result for next check
                        PduTxCtrlPtr->TxResult = CANIF_NULL;

                        SaveStatusReg();

                        PduTxCtrlPtr->TxStartedFlag = STOPED;
                        RestoreStatusReg();
                    }

                    if(STARTED == PduTxCtrlPtr->TxStartedFlag)
                    {
                        if(PduPtr->PduMsgPtr != NULL)  // prevent msg ptr config error
                        {
                            if(!CanIf_RequestSend(PduPtr->PduCanIfChn,PduPtr->PduMsgPtr,(uWord)(&PduTxCtrlPtr->TxResult)))
                            {
                                // do nothing

                                // need set number here
                            }
                        }
                    }

                    break;
                }

                /*
                 * Deal MIXD mode pdu, When trig send, send N times by this repetition period
                 */
                case TX_MIXED:
                {
                    // check whether send ok
                    if(CANIF_SEND_OK == PduTxCtrlPtr->TxResult)
                    {
                        // clear result for next check
                        PduTxCtrlPtr->TxResult = CANIF_NULL;

                        SaveStatusReg();

                        if(PduTxCtrlPtr->TxNumberOfRepetitionsCnt > 0)
                        {
                            PduTxCtrlPtr->TxNumberOfRepetitionsCnt--;
                        }

                        if(PduTxCtrlPtr->TxNumberOfRepetitionsCnt > 0)
                        {
                            // reset period
                            PduTxCtrlPtr->TxRepetitionPeriodCnt = PduPtr->PduTxPara.sTxMode.TxModeRepetitionPeriodFactor / MIN_TIMER_MS;
                        }
                        else
                        {
                            // send complete, and stop tx
                            //PduTxCtrlPtr->TxStartedFlag = STOPED;
                            
                            //reset repetition count
                            PduTxCtrlPtr->TxNumberOfRepetitionsCnt = PduPtr->PduTxPara.sTxMode.TxModeNumberOfRepetitionsFactor; 
                        }
                        RestoreStatusReg();
                    }

                    if(STARTED == PduTxCtrlPtr->TxStartedFlag)
                    {

                        if(PduTxCtrlPtr->TxNumberOfRepetitionsCnt > 0)
                        {
                            if(PduTxCtrlPtr->TxRepetitionPeriodCnt > 0)
                            {
                                PduTxCtrlPtr->TxRepetitionPeriodCnt--;
                            }
                            if(0 == PduTxCtrlPtr->TxRepetitionPeriodCnt)
                            {
                                if(PduPtr->PduMsgPtr != NULL)  // prevent msg ptr config error
                                {
                                    if(!CanIf_RequestSend(PduPtr->PduCanIfChn,PduPtr->PduMsgPtr,(uWord)(&PduTxCtrlPtr->TxResult)))
                                    {
                                        SaveStatusReg();
                                        // reset period
                                        PduTxCtrlPtr->TxRepetitionPeriodCnt = PduPtr->PduTxPara.sTxMode.TxModeRepetitionPeriodFactor / MIN_TIMER_MS;
                                        RestoreStatusReg();
                                    }

                                }
                            }
                        }

                    }




                    break;
                }
                default :
                    break; // do nothing
                }



            } // end of if(PduPtr->PduDirection == SEND)
        }
    }

#ifdef CAR_CAN_EN
    Data_SendCarMsg();
#endif

#ifdef CHG_CAN_EN
    Data_SendChgMsg();
#endif
}



// call by main task
void CCom_MainRx(void)
{
    uByte chn;
    uByte buf_length,i;
    sCanIf_MsgType msg;

    for(chn = 0; chn < CAN_CHANNEL_NUM; chn++)
    {
        /* have data ? */
        buf_length = CanIf_GetRxBufferLength(chn);
        if(buf_length)
        {
            /* read all msg */
            for(i = 0; i < buf_length; i++)
            {
                if(!CanIf_RxIndication(chn,&msg))
                {
                    Data_ProcMsg_Cbk(chn,msg);
                }
            }
        }
    }
}