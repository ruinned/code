// CarCan.h
#ifndef CARCAN_H
#define CARCAN_H

#include "CanIf_Type.h"
#include "CanIf.h"
#include "Ecu.h"

#include "Project.h"


#define MAX_CAR_CAN_SEND_BUF 11

typedef struct
{
    uWord TimeCnt;
    uByte FirstSendFlag;
    uByte DelayTime; 
}sCarCanCtrlType;

typedef struct
{
    sCanIf_MsgType Msg;
    uWord Period;
    sCarCanCtrlType SendCtrl; 
}sCarCanMsgType;


#ifdef CAR_CAN_EN
extern void CarCan_Init(void);
extern void CarCan_MainReceiveMsg(void);
extern void CarCan_MainSendMsg(void);
extern void CarCan_CountCbk(void);
extern void CarCan_ProMain(void);
extern void CarCan_UpdateDcChargeConfig(uByte config);
extern uByte CarCan_GetDcChargeConfig(void);
extern uByte CarCan_GetVcuDcChargeCom(void);
extern void CarCan_UpdateDcChErrLvl(uByte Err);
extern uByte CarCan_GetVcuAcChargeCom(void);
extern uByte CarCan_GetVcuAcChargeCCSt(void);
extern uByte CarCan_GetVcuChargerCapaSt(void);
extern void CarCan_UpdateDcChargeFull(uByte flag);
extern void CarCan_UpdateAcChargeFull(uByte flag);
extern uByte CarCan_GetBmsErrLvl(void);
extern void CarCan_UpdateBmsMode(uByte Mode);
extern uByte CarCan_GetBmsMode(void);
extern void CarCan_UpdateDcChargerComErr(uByte Err);
extern void CarCan_UpdateAcChgComplRemHour(uByte Hour);
extern uByte CarCan_GetAcChgComplRemHour(void);
extern void CarCan_UpdateAcChgComplRemMinute(uByte Minu);
extern uByte CarCan_GetAcChgComplRemMinute(void);
#endif


#endif /* CARCAN_H */