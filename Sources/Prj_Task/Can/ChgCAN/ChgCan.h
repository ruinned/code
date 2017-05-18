// ChgCan.h
#ifndef CHGCAN_H
#define CHGCAN_H

#include "CanIf_Type.h"
#include "GeneralTypeDef.h"

  
#define MAX_CHG_CAN_SEND_BUF 10

typedef struct
{
    uWord TimeCnt;
    uByte FirstSendFlag;
    uByte DelayTime; 
    uByte Flg;
}sChgCan_CtrlType;



typedef struct
{
    sCanIf_MsgType Msg;
    uWord Period;
    sChgCan_CtrlType SendCtrl; 
    
}sChgCan_MsgType;


#ifdef CHG_CAN_EN
extern void ChgCan_Init(void);
extern void ChgCan_MainReceiveMsg(void);
extern void ChgCan_MainSendMsg(void);
extern void ChgCan_CountCbk(void);
extern void ChgCan_MainLoadData(void);

extern uByte ChgCan_GetLife(void);
extern void ChgCan_UpdateLife(uByte life);

#endif



#endif /* CARCAN_H *//