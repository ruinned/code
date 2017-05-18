// CanTp.h

#ifndef CANTP_H
#define CANTP_H

#include "GeneralTypeDef.h"
#include "CanIf_Type.h"
#include "CanTp_Type.h"


void Init_CanTp(void); 
void CanTp_MainTx(void);            
uByte CanTp_EventRx(eCanIf_ChannelType chn, sCanIf_MsgType* MsgPtr, sCanTp_MsgType * CanTpMsgPtr);          
uByte CanTp_CopyDataToBuf(eCanIf_ChannelType chn, sCanTp_MsgType * CanTpMsgPtr);



#endif