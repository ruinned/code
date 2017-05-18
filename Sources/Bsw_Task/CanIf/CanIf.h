// CanIf.h
#ifndef CANIF_H
#define CANIF_H

#include "GeneralTypeDef.h"
#include "CanIf_Type.h"

typedef enum
{
  CYCLE_SEND,
  EVENT_SEND,
  EVENT_CYCLE_SEND,  
}eCanIf_SendTypeType;

typedef enum
{
    LISTEN_MODE,
    NORMAL_MODE,
    SLEEP_MODE,
}eCanIf_CanMOde;

void CanIf_Init(eCanIf_ChannelType chn,uByte Baudrate);
void CanIf_StartSend(eCanIf_ChannelType chn);
GeneralReturnType CanIf_RequestSend(eCanIf_ChannelType chn, const sCanIf_MsgType * msg_ptr, uWord rslt_ref);
GeneralReturnType CanIf_RxIndication(eCanIf_ChannelType chn, sCanIf_MsgType * msg_ptr);
uByte CanIf_GetRxBufferLength(eCanIf_ChannelType chn);

GeneralReturnType CanIf_SetCanMode(eCanIf_ChannelType chn, eCanIf_CanMOde mode);
//void Can_TxConfirm(eCanIf_ChannelType chn, uWord BufferMask);
void CanIf_SetAcceptanceMaskCode(eCanIf_ChannelType chn,sCanIf_AcceptanceCodeType *AcceptCodePtr,sCanIf_AcceptanceMaskType *AcceptMaskPtr);
#endif /* CANIF_H */