// Can_Cbk.h
#ifndef CAN_CBK_H
#define CAN_CBK_H

#include "CanIf_Type.h"

extern void Can_SendTask(eCanIf_ChannelType chn);
extern void Can_ReceiveTask(eCanIf_ChannelType chn);
extern void Can_TxConfirm(eCanIf_ChannelType chn, uWord BufferMask);
extern void CanIf_AppRx(uLWord CanId, uByte length, uByte *pdata);


#define INT_CAN_SEND_TASK() Can_SendTask(INT_CAN_CHANNEL)
#define CAR_CAN_SEND_TASK() Can_SendTask(CAR_CAN_CHANNEL)
#define CHG_CAN_SEND_TASK() Can_SendTask(CHG_CAN_CHANNEL)

#define INT_CAN_RECEIVE_TASK() Can_ReceiveTask(INT_CAN_CHANNEL)
#define CAR_CAN_RECEIVE_TASK() Can_ReceiveTask(CAR_CAN_CHANNEL)
#define CHG_CAN_RECEIVE_TASK() Can_ReceiveTask(CHG_CAN_CHANNEL)

#define INT_CAN_TX_CONFIRM(BufferMask) Can_TxConfirm(INT_CAN_CHANNEL,BufferMask)
#define CAR_CAN_TX_CONFIRM(BufferMask) Can_TxConfirm(CAR_CAN_CHANNEL,BufferMask)
#define CHG_CAN_TX_CONFIRM(BufferMask) Can_TxConfirm(CHG_CAN_CHANNEL,BufferMask)

#endif /* CAN_CBK_H */