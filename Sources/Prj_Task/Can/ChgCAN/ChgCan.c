/* ChgCan.c */
#include "CanIf_Type.h"
#include "CanIf.h"
#include "ChgCan.h"
#include "SchIf.h"
#include "BmsSysSt.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "GbChgCan.h"

#include "ChargeSt.h"
#include "ChgM-B.h"

uByte gby_cnt;

#ifdef CHG_CAN_EN

typedef struct
{
    uByte TaskPeriod;
    uByte Life;
}sChgCan_ComType;

sChgCan_ComType  ChgCan_Com;

sChgCan_MsgType ChgCan_Msg[MAX_CHG_CAN_SEND_BUF]; 
sChgCan_MsgType ChgCan_TestMsg;

uByte flag_test_chg_can = 0; 


uByte ChgCan_GetLife(void)
{
    return ChgCan_Com.Life;
}
void ChgCan_UpdateLife(uByte life)
{
    ChgCan_Com.Life = life;
}

void ChgCan_Init(void)
{
    uWord i;
    uByte *ptr;
    sCanIf_AcceptanceCodeType accept;
    sCanIf_AcceptanceMaskType mask;
        
   /* sChgCan_CtrlType *CtrPtr; */
 
    ptr = (uByte*)(ChgCan_Msg);
    for(i = 0; i < sizeof(ChgCan_Msg); i++)
    {
        *(ptr++) = 0;
    }
    
    ptr = (uByte *)(&ChgCan_Com);
    for(i = 0; i < sizeof(ChgCan_Com); i++)
    {
        *(ptr++) = 0;
    }
    
    CanIf_Init(CHG_CAN_CHANNEL, CANIF_BAUDRATE_250K);

    accept.AccCode1 = 0x00000000;
    mask.AccMask1 =   0xFFFFFFFF;

    accept.AccCode2 = 0x00000000;
    mask.AccMask2   = 0xFFFFFFFF;           
    CanIf_SetAcceptanceMaskCode(CHG_CAN_CHANNEL, &accept, &mask);
 
 
#ifdef ECU_M_ENABLE_GBCHGIF    
     GbChgCan_Init();
#endif     

    ChgCan_TestMsg.Msg.id = 0x04ff56f4;   //for test chg can id
    ChgCan_TestMsg.Msg.length = 8;
    for(i = 0; i < 8; i++)
    {
        ChgCan_TestMsg.Msg.data[i] = i+1;    
    }
    ChgCan_TestMsg.Period = TM_500MS;
    ChgCan_TestMsg.SendCtrl.DelayTime= 0;  
    
    gby_cnt = 0;                                     
    /* initial other variable */  
          
}

/* call by 10 ms task */
void ChgCan_MainLoadData(void)
{
    uWord wtmp;
    
    if(ChgCan_Com.TaskPeriod >= TM_MS(10))
    {
        ChgCan_Com.TaskPeriod = 0;
        SaveStatusReg();
        
        /* call BatterySt interface reach the battery data  */
       
#ifdef ECU_M_ENABLE_GBCHGIF      
        GbChgCan_LoadData();
#endif              

        RestoreStatusReg(); 
                            
    }
}

 
void ChgCan_ProcMsg(sCanIf_MsgType msg)
{
   uByte life; 
      
   life = ChgCan_GetLife(); 
   life++;
   
   if(0x04fff456 == msg.id)  // for test chg can msg  don't move these code !!!!!!!!!!!!
   {
      flag_test_chg_can = 1;
   }
   else
   {
      flag_test_chg_can = 0;
   }
   
    
#ifdef ECU_M_ENABLE_GBCHGIF    
     GbChg_RxDataFromCharger(msg);    
#endif 
     ChgCan_UpdateLife(life);
     
        
     
}

/* call by 5MS main task  */
void ChgCan_MainSendMsg(void) 
{
 
    uWord rslt = 0;
    uByte i;


#ifdef ECU_M_ENABLE_GBCHGIF   
    GbChgCan_TxData();
    gby_cnt++;
    if(gby_cnt >= 2) 
    {
        gby_cnt = 0;  
        GbChgCan_TpcmProc();
    }
#endif 

    if(1 == flag_test_chg_can)
    {
        ChgCan_TestMsg.SendCtrl.TimeCnt++;
        if(ChgCan_TestMsg.SendCtrl.TimeCnt >= ChgCan_TestMsg.Period) 
        {
            ChgCan_TestMsg.SendCtrl.TimeCnt = 0;
            (void)CanIf_RequestSend(CHG_CAN_CHANNEL, &ChgCan_TestMsg.Msg, rslt);        
        }
    }
    else
    {
        ChgCan_TestMsg.SendCtrl.TimeCnt = 0;
    }
 
}


/* call by main task */
void ChgCan_MainReceiveMsg(void)
{   
    GeneralReturnType  rslt;      
    uByte buf_length;
    sCanIf_MsgType msg;
    
    /* have data ? */
    buf_length = CanIf_GetRxBufferLength(CHG_CAN_CHANNEL);
    if(buf_length)
    {
        rslt = CanIf_RxIndication(CHG_CAN_CHANNEL,&msg);
        if(!rslt)
        {
            ChgCan_ProcMsg(msg);
        }
    }
}

void ChgCan_CountCbk(void)
{
    GbChg_CountCbk();
    (ChgCan_Com.TaskPeriod < 255) ? (ChgCan_Com.TaskPeriod++) : (ChgCan_Com.TaskPeriod = 255);
}


#endif


