// DataLayer.h
#ifndef DATALAYER_H
#define DATALAYER_H
#include "Ecu.h"
#include "CanIf_Type.h"
#include "AppRelay.h"

extern void Data_ProcMsg_Cbk(eCanIf_ChannelType chn, sCanIf_MsgType msg);


#ifdef CAR_CAN_EN
extern void Data_SendCarMsg(void); 
#endif

#ifdef CHG_CAN_EN
extern void Data_SendChgMsg(void);
#endif

extern uByte Data_CtrlBmuRelay(uWord relay_id, sActor_CtrlRelayCmdType relay_cmd);

#endif /* DATALAYER_H */