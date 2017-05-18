// DataLayer.c 
#include "CanIf_Type.h"
#include "CanIf.h"
#include "Ecu.h"
#include "AppRelay.h"
#include "IntCan.h"
#ifdef CAR_CAN_EN
#include "CarCan.h"
#ifdef CHG_CAN_EN
#include "ChgCan.h"
#endif
#endif
/*
 * Deal CAN Rx Data 
 */
//void Data_IntCanProcMsg(sCanIf_MsgType msg)
//{
   /*
    *  deal internal CAN message here
    */ 
 /*  sCanIf_MsgType request_msg;
   request_msg.id = 0x18018000;
   request_msg.length = 0;  */
   
   
   
   /*  */
//   (void)CanIf_RequestSend(INT_CAN_CHANNEL,&request_msg,0);
//}
// move to IntCan.c

/*#ifdef CAR_CAN_EN
 void Data_CarCanProcMsg(sCanIf_MsgType msg);

#ifdef CHG_CAN_EN  
void Data_ChgCanProcMsg(sCanIf_MsgType msg);   
#endif
#endif */

typedef void (* ProcMsgFuncPtrType)(sCanIf_MsgType msg);

const ProcMsgFuncPtrType ProcMsgFuncPtr[] =
{
   Data_IntCanProcMsg,
   #ifdef CAR_CAN_EN
   Data_CarCanProcMsg,
   #ifdef CHG_CAN_EN
   ChgCan_ProcMsg
   #endif
   #endif
   
};
void Data_ProcMsg_Cbk(eCanIf_ChannelType chn, sCanIf_MsgType msg) 
{
    (* ProcMsgFuncPtr[chn])(msg);
}

#ifdef CAR_CAN_EN
void Data_SendCarMsg(void)
{ // call by 5ms task
  
}
#endif

#ifdef CHG_CAN_EN
void Data_SendChgMsg(void)
{ // call by 5ms task
  
}
#endif


#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
uByte Data_CtrlBmuRelay(uWord relay_id, sActor_CtrlRelayCmdType relay_cmd)
{
    uByte rslt;
    sCanIf_MsgType msg;
    uLWord ps;  
    uByte i;  
    
    rslt = ERR_FAILED;     
    
    /*
     * Calc ps, example relay_id = 43
     * 43 - BMU_RELAY_ID_BASE = 33
     * 33 / BMU_MAX_RELAY_NUM = 1
     * 1 + 1 = 2   , indication BMU2
     * 2 << 8 = 0x0200
     */
    ps =  (uLWord)((relay_id - BMU_RELAY_ID_BASE) / BMU_MAX_RELAY_NUM + 1U);
    ps <<= 8;
    
    if(ps == 0)
    {
      return ERR_FAILED;
    }
    for(i = 0; i < 8; i++)
    {
      msg.data[i] = 0xFFU;
    }
    
    msg.id = 0x4800000L + ps;
    msg.length = 8;
    /*
     * Calc Byte number,example relay_id = 43
     * 43 - BMU_RELAY_ID_BASE = 33
     * 33 % BMU_MAX_RELAY_NUM =  1
     * 1 / 2 = 0
     * data[0]
     * (33 % BMU_MAX_RELAY_NUM =  1) % 2 = 1 ,low 4bits
     */
    if(0 == (((relay_id - BMU_RELAY_ID_BASE) % BMU_MAX_RELAY_NUM) % 2))
    { // high 4 bits      
      if(relay_cmd.Bcmd)
      { // ctrl relay close
        msg.data[(((relay_id - BMU_RELAY_ID_BASE) % BMU_MAX_RELAY_NUM) / 2)] = 0x1F;
      }
      else
      {  // ctrl relay open
        msg.data[(((relay_id - BMU_RELAY_ID_BASE) % BMU_MAX_RELAY_NUM) / 2)] = 0x0F;
      }
    }
    else
    { // low 4 bits
      if(relay_cmd.Bcmd)
      { // ctrl relay close
        msg.data[(((relay_id - BMU_RELAY_ID_BASE) % BMU_MAX_RELAY_NUM) / 2)] = 0xF1;
      }
      else
      {  // ctrl relay open
        msg.data[(((relay_id - BMU_RELAY_ID_BASE) % BMU_MAX_RELAY_NUM) / 2)] = 0xF0;
      }
    }
    rslt = CanIf_RequestSend(INT_CAN_CHANNEL,&msg,0);
    
    return rslt;
}
#endif