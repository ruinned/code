

#include "GbChgCan.h"
#include "SchIf.h"
#include "BatterySt.h"
#include "ChargeSt.h"
//#include "CanIf.h" 
#include "ChgCan.h"
#include "CarCan.h"
#include "AiIf.h"
#include "GbChg_IfDet.h"
#include "ErrorAlarm.h"
#include "GbChgM-B.h"
#include "ChgM-B.h"
#include "BatterySysSt.h"
#include "ThM-AB.h"
#include "CarCan.h"
#include "ChgM-B.h"

#define GB_FLAG_ENABLE 1U 
#define GB_FLAG_DISABLE 0U
#define GB_FLAG_FAIL 0U
#define GB_FLAG_OK   1U

#define MSG_BRO 0
#define MSG_BCL 1
#define MSG_BSM 2
#define MSG_BST 3
#define MSG_BSD 4
#define MSG_BEM 5
#define MSG_BHM 6
#define GB_CHG_CAN_TX_BUF 7
#define OFFSET_CUR_GB  4000

typedef struct
{
    sCanIf_MsgType Msg;
    uWord Period;
    uWord TimeCnt;
    uByte FirstSendFlag;
    uByte DelayTime; 
    uByte Flg;
    
}sGbChgCan_MsgType;

uWord gwd_Volt[7];
uByte gby_Temper[7];
uByte gby_MsgNumBmv,gby_MsgNumBmt;
sGbChgCan_MsgType GbChgCan_Msg[GB_CHG_CAN_TX_BUF];
sGbChgCan_TmCntType GbChgCan_TmCnt;
sGbChgCan_CtrlType  GbChgCan_Ctrl;
sGbChgCan_RxType  GbChgCan_RxData;


uByte BYTE_NUM_BRM;
uByte MSG_NUM_BRM;


void GbChgCan_LoadData(void);
void  GbChgCan_TpcmProc(void);


void GbChgCan_UpdateCntBst(uByte cnt)
{
    GbChgCan_TmCnt.Bst = cnt;
}

uByte GbChgCan_GetCrmRxBms(void)
{
    return GbChgCan_RxData.Crm.RxBms;
}
uByte GbChgCan_GetCroChgReady(void)
{
    return GbChgCan_RxData.Cro.Ready ;
}

uWord GbChgCan_GetRxFlg(void)
{
   return GbChgCan_RxData.RxFlg; 
}
void GbChgCan_UpdateRxFlg(uWord flg)
{
    GbChgCan_RxData.RxFlg = flg;
}

uByte GbChgCan_GetChargerRxBcsFinish(void)  
{
  uByte rslt;
  rslt = GbChgCan_Ctrl.Flg.RxBcsFinish;
  return rslt;
}
void GbChgCan_UpdateChargerRxBcsFinish(uByte Flg)  
{
  GbChgCan_Ctrl.Flg.RxBcsFinish = Flg; 
}

uByte GbChgCan_GetCcsChgEn(void)
{
    return GbChgCan_RxData.Ccs.ChgEn;
}

void GbChgCan_Init(void)
{
    uByte i, *p_byte;
    
    p_byte = (uByte* )(&GbChgCan_TmCnt);
    for(i = 0; i < sizeof(GbChgCan_TmCnt); i++)
    {
        *(p_byte++) = 0;
    }
    
    p_byte = (uByte*)(&GbChgCan_Ctrl);
    for(i = 0; i < sizeof(GbChgCan_Ctrl); i++)
    {
        *(p_byte++) = 0;
    } 
    
    
    p_byte = (uByte*)(GbChgCan_Msg);
    for(i = 0; i < sizeof(GbChgCan_Msg); i++)
    {
        *(p_byte++)  = 0;
    }
    
    GbChgCan_Msg[MSG_BHM].Msg.id = ID_BHM;
    GbChgCan_Msg[MSG_BHM].Msg.length = 2;
    GbChgCan_Msg[MSG_BHM].Period = TM_MS(250);
    GbChgCan_Msg[MSG_BHM].DelayTime= 0;

    GbChgCan_Msg[MSG_BRO].Msg.id = ID_BRO;
    GbChgCan_Msg[MSG_BRO].Msg.length = 1;
    GbChgCan_Msg[MSG_BRO].Period = TM_MS(250);
    GbChgCan_Msg[MSG_BRO].DelayTime= 0;

    GbChgCan_Msg[MSG_BCL].Msg.id = ID_BCL;
    GbChgCan_Msg[MSG_BCL].Msg.length = 5;
    GbChgCan_Msg[MSG_BCL].Period = TM_MS(50);
    GbChgCan_Msg[MSG_BCL].DelayTime= 0;

    GbChgCan_Msg[MSG_BSM].Msg.id = ID_BSM;
    GbChgCan_Msg[MSG_BSM].Msg.length = 7;
    GbChgCan_Msg[MSG_BSM].Period = TM_MS(250);
    GbChgCan_Msg[MSG_BSM].DelayTime= 0;

    GbChgCan_Msg[MSG_BST].Msg.id = ID_BST;
    GbChgCan_Msg[MSG_BST].Msg.length = 4;
    GbChgCan_Msg[MSG_BST].Period = TM_MS(10);
    GbChgCan_Msg[MSG_BST].DelayTime= 0;

    GbChgCan_Msg[MSG_BSD].Msg.id = ID_BSD;
    GbChgCan_Msg[MSG_BSD].Msg.length = 7;
    GbChgCan_Msg[MSG_BSD].Period = TM_MS(250);
    GbChgCan_Msg[MSG_BSD].DelayTime= 0;

    GbChgCan_Msg[MSG_BEM].Msg.id = ID_BEM;
    GbChgCan_Msg[MSG_BEM].Msg.length = 4;
    GbChgCan_Msg[MSG_BEM].Period = TM_MS(250);
    GbChgCan_Msg[MSG_BEM].DelayTime= 0;
    
}

void GbChg_CountCbk(void)
{
    (GbChgCan_TmCnt.Brm < 255) ? (GbChgCan_TmCnt.Brm++) : (GbChgCan_TmCnt.Brm = 255); 
    (GbChgCan_TmCnt.Bcp < 255) ? (GbChgCan_TmCnt.Bcp++) : (GbChgCan_TmCnt.Bcp = 255);
    (GbChgCan_TmCnt.Bcs < 255) ? (GbChgCan_TmCnt.Bcs++) : (GbChgCan_TmCnt.Bcs = 255);
    (GbChgCan_TmCnt.Bmv < 255) ? (GbChgCan_TmCnt.Bmv++) : (GbChgCan_TmCnt.Bmv = 255);
    (GbChgCan_TmCnt.Bmt < 255) ? (GbChgCan_TmCnt.Bmt++) : (GbChgCan_TmCnt.Bmt = 255);
    (GbChgCan_TmCnt.Bsp < 255) ? (GbChgCan_TmCnt.Bsp++) : (GbChgCan_TmCnt.Bsp = 255);
    
    (GbChgCan_TmCnt.Tpcm < 255) ? (GbChgCan_TmCnt.Tpcm++) : (GbChgCan_TmCnt.Tpcm = 255);
    (GbChgCan_TmCnt.WaitAckBrm < 255) ? (GbChgCan_TmCnt.WaitAckBrm++) : (GbChgCan_TmCnt.WaitAckBrm = 255);
    (GbChgCan_TmCnt.WaitAckBcp < 255) ? (GbChgCan_TmCnt.WaitAckBcp++) : (GbChgCan_TmCnt.WaitAckBcp = 255);
    (GbChgCan_TmCnt.WaitAckBcs < 255) ? (GbChgCan_TmCnt.WaitAckBcs++) : (GbChgCan_TmCnt.WaitAckBcs = 255);
    (GbChgCan_TmCnt.WaitAckBmv < 255) ? (GbChgCan_TmCnt.WaitAckBmv++) : (GbChgCan_TmCnt.WaitAckBmv = 255);
    (GbChgCan_TmCnt.WaitAckBmt < 255) ? (GbChgCan_TmCnt.WaitAckBmt++) : (GbChgCan_TmCnt.WaitAckBmt = 255);
    (GbChgCan_TmCnt.WaitAckBsp < 255) ? (GbChgCan_TmCnt.WaitAckBsp++) : (GbChgCan_TmCnt.WaitAckBsp = 255);
    
    
    (GbChgCan_TmCnt.WaitCtsBrm < 255) ? (GbChgCan_TmCnt.WaitCtsBrm++) : (GbChgCan_TmCnt.WaitCtsBrm = 255);
    (GbChgCan_TmCnt.WaitCtsBcp < 255) ? (GbChgCan_TmCnt.WaitCtsBcp++) : (GbChgCan_TmCnt.WaitCtsBcp = 255);
    (GbChgCan_TmCnt.WaitCtsBcs < 255) ? (GbChgCan_TmCnt.WaitCtsBcs++) : (GbChgCan_TmCnt.WaitCtsBcs = 255); 
    (GbChgCan_TmCnt.WaitCtsBmv < 255) ? (GbChgCan_TmCnt.WaitCtsBmv++) : (GbChgCan_TmCnt.WaitCtsBmv = 255);
    (GbChgCan_TmCnt.WaitCtsBmt < 255) ? (GbChgCan_TmCnt.WaitCtsBmt++) : (GbChgCan_TmCnt.WaitCtsBmt = 255);
    (GbChgCan_TmCnt.WaitCtsBsp < 255) ? (GbChgCan_TmCnt.WaitCtsBsp++) : (GbChgCan_TmCnt.WaitCtsBsp = 255);    
}

static void GbChg_TpcmRts(uByte cmd, uWord byte_num, uByte msg_num, uByte pf)
{
    sCanIf_MsgType  msg;
    uWord rslt_ref;

     rslt_ref = 0;
    
    msg.id = ID_TPCM;
    msg.length = 8;

    msg.data[0] = cmd;
    msg.data[1] = (uByte)(byte_num & 0x00ff);
    msg.data[2] = (uByte)(byte_num >> 8);
    msg.data[3] = msg_num;
    msg.data[4] = 0xff;
    msg.data[5] = 0;
    msg.data[6] = pf;
    msg.data[7] = 0;

    (void)CanIf_RequestSend(CHG_CAN_CHANNEL,  &msg, rslt_ref);
}

static void GbChg_TpcmAbort(uByte cmd, uByte pf)
{
    sCanIf_MsgType  msg;
    uWord rslt_ref = 0;

    msg.id = ID_TPCM;
    msg.length = 8;

    msg.data[0] = cmd;
    msg.data[1] = 0xff;
    msg.data[2] = 0xff;
    msg.data[3] = 0xff;
    msg.data[4] = 0xff;
    msg.data[5] = 0;
    msg.data[6] = pf;
    msg.data[7] = 0;

    (void)CanIf_RequestSend(CHG_CAN_CHANNEL,  &msg, rslt_ref);
  
    
    GbChgCan_Ctrl.MsgNumBrm = 0;
    GbChgCan_Ctrl.MsgNumBcp = 0;
    GbChgCan_Ctrl.MsgNumBcs = 0;
    GbChgCan_Ctrl.MsgNumBmv = 0;   
    GbChgCan_Ctrl.MsgNumBmt = 0; 
    GbChgCan_Ctrl.MsgNumBsp = 0;   
    
    GbChgCan_Ctrl.Flg.RxCtsBrm = GB_FLAG_FAIL;
    GbChgCan_Ctrl.Flg.RxCtsBcp = GB_FLAG_FAIL;
    GbChgCan_Ctrl.Flg.RxCtsBcs = GB_FLAG_FAIL;
    GbChgCan_Ctrl.Flg.RxCtsBmv = GB_FLAG_FAIL;
    GbChgCan_Ctrl.Flg.RxCtsBmt = GB_FLAG_FAIL;
    GbChgCan_Ctrl.Flg.RxCtsBsp = GB_FLAG_FAIL;
}



static void GbChg_TpcmSendData(uByte pf, uByte msg_no)
{

    sCanIf_MsgType  msg;
    uWord wtmp;
    uLWord llwtmp;
    uWord rslt_ref = 0;
    uWord k;
    uByte i,j;
    uWord SerialNumBak;
    uWord SerialNum;
    uWord TemperNum;
    uWord TemperNumBak;
    uByte Data[17];

    msg.id = ID_TPCM_DATA;
    msg.length = 8;

    msg.data[0] = msg_no;
    switch (pf)
    {
    case BRM_PF:
    {
        if(1 == msg_no)
        {
            /* version V1.1  */
            msg.data[1] = 0x01;
            msg.data[2] = 0x01;
            msg.data[3] = 0;

            /* battery type  */
            
            if(LFP == Bat_GetMaterialType())
            {
                wtmp = 3;			 /*  LiFePO4  */
            }
            else if(LMO == Bat_GetMaterialType())
            {
                wtmp = 4;			 /*  LiMn2O4  */
            }
            else if(NCM == Bat_GetMaterialType())
            {
                wtmp = 6;    
            }
            else if(LTO == Bat_GetMaterialType())
            {
                wtmp = 8;
            }
            else
            {
                wtmp = 3;			 /*  LiFePO4  */
            }

            msg.data[4] = (uByte)wtmp;

            /* rated capacity   */
           /* 0.1AH unit  */
            wtmp = (uWord)(Bat_GetRatedCapacityCalc() / 100); 
            msg.data[5] = (uByte)(wtmp & 0x00ff);
            msg.data[6] = (uByte)((wtmp & 0xff00) >> 8);

            //rated sum volt  low byte
          
            wtmp =  Bat_GetSerialNum();
           
            if(LFP == Bat_GetMaterialType())
            {
                wtmp *= 32;		 /* 3.2V one cell for LiFePO4 */
            }
            else if(LMO == Bat_GetMaterialType())
            {
                wtmp *= 37;		 /* 3.7V one cell for LiMn2O4 */
            }
            else if(NCM == Bat_GetMaterialType())
            {
                wtmp *= 37;       
            }
            else if(LTO == Bat_GetMaterialType())
            {
                wtmp *= 24;       
            }
            else
            {
                 wtmp *= 32;		 /* 3.2V one cell for LiFePO4 */
            }

            msg.data[7] = (uByte)(wtmp & 0x00ff);
        }
        else if(2 == msg_no)
        {
            //rated sum volt high byte
         
            wtmp =  Bat_GetSerialNum();
            if(LFP == Bat_GetMaterialType())
            {
                wtmp *= 32;		 /* 3.2V one cell for LiFePO4 */
            }
            else if(LMO == Bat_GetMaterialType())
            {
                wtmp *= 37;		 /* 3.7V one cell for LiMn2O4 */
            }
            else if(NCM == Bat_GetMaterialType())
            {
                wtmp *= 37;		 
            }
            else if(LTO == Bat_GetMaterialType())
            {
                wtmp *= 24;	
            }
            else
            {
                 wtmp *= 32 ;	
            }
            msg.data[1] = (uByte)((wtmp & 0xff00) >> 8);

           /* battery company */
            msg.data[2] = 0xff;
            msg.data[3] = 0xff;
            msg.data[4] = 0xff;
            msg.data[5] = 0xff;

           /*  battery pack num  */
            llwtmp = Bat_GetAllPackNo();
            msg.data[6] = (uByte)(llwtmp & 0x000000FF);
            msg.data[7] = (uByte)((llwtmp & 0x0000FF00) >> 8);
        }
        else if(3 == msg_no)
        {
            /* battery pack num  */
            llwtmp = Bat_GetAllPackNo();
            msg.data[1] = (uByte)((llwtmp & 0x00FF0000) >> 16);
            msg.data[2] = (uByte)((llwtmp & 0xFF000000) >> 24);

            /* battery make date */
            Bat_GetBatDate(Data);
            msg.data[3] = Data[0];		 
            msg.data[4] = Data[1];		 
            msg.data[5] = Data[2];		 

             /* battery charge times */
            wtmp = Bat_GetHisChgTimes(); 
            msg.data[6] = (uByte)(wtmp & 0x00FF);
            msg.data[7] = (uByte)((wtmp & 0xFF00) >> 8);
        }
        else if(4 == msg_no)
        {
            /* battery charge times */
            msg.data[1] =  0;

             /*battery property  */
            msg.data[2] = Bat_GetPropertyId();

             /* Rsv  */
            msg.data[3] = 0xff;

            /* vehicle VIN  */
            BatSys_GetCarVinNum(Data);
            msg.data[4] = Data[16];
            msg.data[5] = Data[15];
            msg.data[6] = Data[14];
            msg.data[7] = Data[13];
        }
        else if(5 == msg_no)
        {
            /* vehicle VIN  */
            BatSys_GetCarVinNum(Data);
            msg.data[1] = Data[12];
            msg.data[2] = Data[11];
            msg.data[3] = Data[10];
            msg.data[4] = Data[9];
            msg.data[5] = Data[8];
            msg.data[6] = Data[7];
            msg.data[7] = Data[6];
        }
        else if(6 == msg_no)
        {
             /* vehicle VIN   */
            BatSys_GetCarVinNum(Data);
            msg.data[1] = Data[5];
            msg.data[2] = Data[4];
            msg.data[3] = Data[3];
            msg.data[4] = Data[2];
            msg.data[5] = Data[1];
            msg.data[6] = Data[0];
            msg.data[7] = 0x01;    //first time
        }
        else if(7 == msg_no)    //GB27930-2015
        {
             /* vehicle VIN   */
            msg.data[1] = 0x15;   //DATA--21ÈÕ
            msg.data[2] = 0x04;   //Month--4ÔÂ
            msg.data[3] = 0x07;   //0X07E0--2016Äê
            msg.data[4] = 0xe0;
            msg.data[5] = 0xff;
            msg.data[6] = 0xff;
            msg.data[7] = 0xff;
        }
        break;
    }
    case BCP_PF:
    {
        if(1 == msg_no)
        {
             /* max charge voltage   */
            wtmp = Chg_GetPermitMaxCellV(); 
            wtmp /= 10;          /* 0.01 unit */
            msg.data[1] = (uByte)(wtmp & 0x00ff);
            msg.data[2] = (uByte)((wtmp & 0xff00) >> 8);

            /* max charge current  */
            if(Chg_GetPermitMaxCurrent() < OFFSET_CUR_GB)
            {
               wtmp = OFFSET_CUR_GB - Chg_GetPermitMaxCurrent();
            }
            else
            {
               wtmp = 0;
            }
            msg.data[3] = (uByte)(wtmp & 0x00ff);
            msg.data[4] = (uByte)((wtmp & 0xff00) >> 8);

            /* battery pack WH  */
            wtmp = Bat_GetRatedEnergy();
            msg.data[5] = (uByte)(wtmp & 0x00ff); /* 0.1kWh unit */
            msg.data[6] = (uByte)((wtmp & 0xff00) >> 8);

            /* max charge sumvolt low byte */
            wtmp = Chg_GetPermitMaxSumV();  /* 0.1V unit */
            msg.data[7] = (uByte)(wtmp & 0x00ff);
        }
        else if(2 == msg_no)
        {
            /* max charge sumvolt high byte */

            wtmp = Chg_GetPermitMaxSumV();  /* 0.1V unit */
            msg.data[1] = (uByte)((wtmp & 0xff00) >> 8);

            /* max charge temper */
            msg.data[2] = Chg_GetPermitMaxT() + 10; /* -50 degree */

            /* SOC */
             /* 0.1% unit */
            wtmp = Bat_GetSOC();
            msg.data[3] = (uByte)(wtmp & 0x00ff);
            msg.data[4] = (uByte)((wtmp & 0xff00) >> 8);

            /* sumvolt  */
            /* 0.1V unit */
            wtmp = Bat_GetSumV(INT_SUMV);
            msg.data[5] = (uByte)(wtmp & 0x00ff);
            msg.data[6] = (uByte)((wtmp & 0xff00) >> 8);

            /* Rsv */
            msg.data[7] = 0xff;
        }
        break;
    }
    case BCS_PF:
    {
        if(1 == msg_no)
        {
           
           /* sumvolt  0.1V unit */
            wtmp = Bat_GetSumV(INT_SUMV);
            msg.data[1] = (uByte)(wtmp & 0x00ff);
            msg.data[2] = (uByte)((wtmp & 0xff00) >> 8);

             
            /* current 0.1A unit */
            wtmp = Bat_GetCurrent(AVGCUR);
            if(wtmp < OFFSET_CUR)
            {
               wtmp = OFFSET_CUR - Bat_GetCurrent(AVGCUR);
            }
            else
            {
                wtmp = 0;
            }
            if(wtmp < OFFSET_CUR_GB)
            {
                wtmp = OFFSET_CUR_GB - wtmp; /* 0.1A unit and -400A offset*/
            }
            else
            {
                wtmp = 0;
            }
            msg.data[3] = (uByte)(wtmp & 0x00ff);
            msg.data[4] = (uByte)((wtmp & 0xff00) >> 8);

            /*maxv bmu No and maxv  */
            wtmp = Bat_GetMaxVBmuNo();
            if(wtmp > 0)
            {
                wtmp = wtmp - 1;
            }
            
            wtmp <<= 12;
           /* 0.01V unit */
            wtmp |= (Bat_GetMaxV() / 10);
            msg.data[5] = (uByte)(wtmp & 0x00ff);
            msg.data[6] = (uByte)((wtmp & 0xff00) >> 8);

             /* SOC  */
            wtmp = Bat_GetSOC();
            wtmp = wtmp  / 10;     /* 1% unit */
            msg.data[7] = (uByte)wtmp;
        }
        else if(2 == msg_no)
        {
           /* 0.1AH unit */
          
            llwtmp = Chg_GetRemainChgTime();
            /* charge remain time */
            msg.data[1] = (uByte)(llwtmp & 0x00ff);
            msg.data[2] = (uByte)((llwtmp & 0xff00) >> 8);
                                                    
            /* Rsv  */
            msg.data[3] = 0xff;
            msg.data[4] = 0xff;
            msg.data[5] = 0xff;
            msg.data[6] = 0xff;
            msg.data[7] = 0xff;
        }
        break;
    }
    case BMV_PF:
    {
        k = ((msg_no >> 1) * 7 ) + 1;
        if(msg_no % 2 == 1)
        {
           for(i = 0; i < 7; i++) 
           {
                SerialNumBak  = 0;
                SerialNum = 0; 

                for(j = 1 ; j <= Bat_GetBmuNum(); j++)
                {
                    SerialNum += Bat_GetPackSerialNum(j);
                    SerialNumBak = SerialNum - Bat_GetPackSerialNum(j);
                    
                    if(k <= SerialNum)
                    {
                        wtmp = j - 1;
                        gwd_Volt[i] = ((Bat_GetCellV(j,  (k - SerialNumBak)) / 10) + (wtmp << 12)) ; 
                        
                        break;
                    }
                    
                }
                k++;
           }
           
           msg.data[1] = (uByte)(gwd_Volt[0] & 0x00ff);
           msg.data[2] = (uByte)((gwd_Volt[0] & 0xff00) >> 8);
           msg.data[3] = (uByte)(gwd_Volt[1] & 0x00ff);
           msg.data[4] = (uByte)((gwd_Volt[1] & 0xff00) >> 8); 
           msg.data[5] = (uByte)(gwd_Volt[2] & 0x00ff);
           msg.data[6] = (uByte)((gwd_Volt[2]  & 0xff00) >> 8);
           msg.data[7] = (uByte)(gwd_Volt[3] & 0x00ff);
             
        }
        else
        {  
            msg.data[1] = (uByte)((gwd_Volt[3] & 0xff00) >> 8);  
            msg.data[2] = (uByte)(gwd_Volt[4] & 0x00ff); 
            msg.data[3] = (uByte)((gwd_Volt[4] & 0xff00) >> 8); 
            msg.data[4] = (uByte)(gwd_Volt[5] & 0x00ff);
            msg.data[5] = (uByte)((gwd_Volt[5]  & 0xff00) >> 8);
            msg.data[6] = (uByte)(gwd_Volt[6] & 0x00ff);
            msg.data[7] = (uByte)((gwd_Volt[6] & 0xff00) >> 8);
        
        }

       break;
    }
    case BMT_PF:
    {
        
           k = ((msg_no >> 1) * 7 ) + 1;       
           for(i = 0; i < 7; i++) 
           {
                TemperNum  = 0;
                TemperNumBak = 0;

                for(j = 1 ; j <= Bat_GetBmuNum(); j++)
                {
                    TemperNum += Bat_GetPackTemperNum(j);
                    TemperNumBak = TemperNum - Bat_GetPackTemperNum(j);
                    
                    if(k <= TemperNum)
                    {
                       
                        gby_Temper[i] = (Bat_GetTemper(j,  (k - TemperNumBak))) ; 
                        
                        break;
                    }
                    
                }
                k++; 
           }
           
           msg.data[1] = (uByte)(gby_Temper[0] + 10);
           msg.data[2] = (uByte)(gby_Temper[1] + 10);
           msg.data[3] = (uByte)(gby_Temper[2] + 10);
           msg.data[4] = (uByte)(gby_Temper[3] + 10);
           msg.data[5] = (uByte)(gby_Temper[4] + 10);
           msg.data[6] = (uByte)(gby_Temper[5] + 10);
           msg.data[7] = (uByte)(gby_Temper[6] + 10);
             
       break;
    }
    case BSP_PF:
    {
       
        wtmp = Bat_GetSOC();
        wtmp = (uByte)(wtmp  / 10); // 1% unit

        if(1 == msg_no)
        {
            msg.data[1] = (uByte)wtmp;
            msg.data[2] = (uByte)wtmp;
            msg.data[3] = (uByte)wtmp;
            msg.data[4] = (uByte)wtmp;
            msg.data[5] = (uByte)wtmp;
            msg.data[6] = (uByte)wtmp;
            msg.data[7] = (uByte)wtmp;
        }
        else if(2 == msg_no)
        {
            msg.data[1] = (uByte)wtmp;
            msg.data[2] = (uByte)wtmp;
            msg.data[3] = 0xff;
            msg.data[4] = 0xff;
            msg.data[5] = 0xff;
            msg.data[6] = 0xff;
            msg.data[7] = 0xff;
        }
        break;
    }
    default:
    {
        break;
    }
    }

   (void)CanIf_RequestSend(CHG_CAN_CHANNEL,  &msg, rslt_ref); 
}


void GbChgCan_TpcmProc(void)
{
    uWord wtmp;
    uByte i;
    sGbChgCan_CtrlType * pGbChgCanCtrl; 
    sGbChgCan_TmCntType *pGbChgCnt;
    
    pGbChgCnt = &GbChgCan_TmCnt;
    pGbChgCanCtrl = &GbChgCan_Ctrl;

     /* if abort or cts , Tpcm and RxCtsBrm will clear */
    /* request send multi byte msg */
    if( (pGbChgCnt->Brm >= TM_MS(250))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Brm)  /* send message Brm enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)          /* use Tpcm enable  */ 
            && (pGbChgCanCtrl->Flg.RxCtsBrm == GB_FLAG_FAIL) )	 /*has Not receive cts of Brm */
    {
        pGbChgCnt->Brm = 0;
        pGbChgCnt->WaitAckBrm = 0;
        pGbChgCnt->WaitCtsBrm = 0;
        pGbChgCanCtrl->Tpcm = TPCM_BRM;
        
        if(GbChg_Mode == MODE_2015)
        {
            BYTE_NUM_BRM = 49;
            MSG_NUM_BRM = 7;
        }
        else
        {
            BYTE_NUM_BRM = 41;
            MSG_NUM_BRM = 6; 
        }

        GbChg_TpcmRts(RTS, BYTE_NUM_BRM, MSG_NUM_BRM, BRM_PF);		   /* request send Brm  */
    }

    if( (pGbChgCnt->Bcp>= TM_MS(500))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Bcp)  /* send message Bcp enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)     /* use Tpcm enable */
            && (pGbChgCanCtrl->Flg.RxCtsBcp == GB_FLAG_FAIL) )	  /*  has Not receive cts of Bcp */
    {
        pGbChgCnt->Bcp= 0;
        pGbChgCnt->WaitAckBcp = 0;
        pGbChgCnt->WaitCtsBcp = 0;
        pGbChgCanCtrl->Tpcm = TPCM_BCP;

        GbChg_TpcmRts(RTS, BYTE_NUM_BCP, MSG_NUM_BCP, BCP_PF);		  /* request send Bcp */
    }
    
    if( (pGbChgCnt->Bcs >= TM_MS(250))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Bcs)  /* send message Bcs enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)    /*  use Tpcm enable */   
            && (pGbChgCanCtrl->Flg.RxCtsBcs == GB_FLAG_FAIL) )	 /*  has Not receive cts of Bcs */
    {
        pGbChgCnt->Bcs = 0;
        pGbChgCnt->WaitAckBcs = 0;
        pGbChgCnt->WaitCtsBcs = 0;
        pGbChgCanCtrl->Tpcm = TPCM_BCS;
        GbChg_TpcmRts(RTS, BYTE_NUM_BCS, MSG_NUM_BCS, BCS_PF);		 /* request send Bcs */
    }

    if( (pGbChgCnt->Bmv >= TM_MS(1000))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Bmv) /* send message Bmv enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)    /* use Tpcm enable  */
            && (pGbChgCanCtrl->Flg.RxCtsBmv == GB_FLAG_FAIL) )	 /* has Not receive cts of Bmv */
    {
        pGbChgCnt->Bmv = 0;
        pGbChgCnt->WaitAckBmv= 0;
        pGbChgCnt->WaitCtsBmv = 0;
        pGbChgCanCtrl->Tpcm = TPCM_BMV;
        
        wtmp = 0;

        for(i = 1; i <= Bat_GetBmuNum(); i++)
        {
            wtmp +=  Bat_GetPackSerialNum(i);
        }
        if(wtmp <= 7)
        {
            if( (2 * wtmp) % 7 != 0)
            {
                gby_MsgNumBmv = (uByte)((2 * wtmp) / 7 + 1);    
            }
            else
            {
                gby_MsgNumBmv = (uByte)((2 * wtmp) / 7);
            }
            
            GbChg_TpcmRts(RTS, (2 * wtmp), gby_MsgNumBmv, BMV_PF);		 /* request send Bmv  */
        }   
    }

    if( (pGbChgCnt->Bmt>= TM_MS(1000))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Bmt) /* send message Bmt enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)    /* use Tpcm enable  */
            && (pGbChgCanCtrl->Flg.RxCtsBmt == GB_FLAG_FAIL) )	 /* has Not receive cts of Bmt */
    {
        pGbChgCnt->Bmt= 0;
        pGbChgCnt->WaitAckBmt = 0;
        pGbChgCnt->WaitCtsBmt = 0;
        pGbChgCanCtrl->Tpcm = TPCM_BMT;

        wtmp = 0;

        for(i = 1; i <= Bat_GetBmuNum(); i++)
        {
            wtmp +=  Bat_GetPackTemperNum(i);
        }
        if(wtmp % 7 != 0)
        {
            gby_MsgNumBmt = (uByte)(wtmp / 7 + 1);    
        }
        else
        {
            gby_MsgNumBmt = (uByte)(wtmp / 7);
        }
        GbChg_TpcmRts(RTS, wtmp, gby_MsgNumBmt, BMT_PF);		 /* request send Bmt */
    }

    if( (pGbChgCnt->Bsp >= TM_MS(1000))
            && (GB_FLAG_ENABLE == GbChgM_Ctrl.Flg.Bsp) /* send message Bsp enable */
            && (pGbChgCanCtrl->Tpcm == TPCM_NULL)    /* use Tpcm enable */
            && (pGbChgCanCtrl->Flg.RxCtsBsp == GB_FLAG_FAIL) )	 /* has Not receive cts of Bsp  */
    {
        pGbChgCnt->Bsp = 0;
        pGbChgCnt->WaitAckBsp = 0;
        pGbChgCnt->WaitCtsBsp = 0;
        pGbChgCanCtrl->Tpcm  = TPCM_BSP;

        GbChg_TpcmRts(RTS, BYTE_NUM_BSP, MSG_NUM_BSP, BSP_PF);		 /* request send Bsp */
    }
    /*
     *  end of request send multi byte msg
     */

    /*
     *  deal wait Tpcm ack time out
     */
    if(pGbChgCanCtrl->Tpcm == TPCM_BRM)
    {
        if(pGbChgCnt->WaitAckBrm >= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBrm = 0;
            GbChg_TpcmAbort(ABORT, BRM_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if(pGbChgCanCtrl->Tpcm == TPCM_BCP)
    {
        if(pGbChgCnt->WaitAckBcp >= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBcp = 0;
            GbChg_TpcmAbort(ABORT, BCP_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }
    
    if(pGbChgCanCtrl->Tpcm == TPCM_BCS)
    {
        if(pGbChgCnt->WaitAckBcs >= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBcs = 0;
            GbChg_TpcmAbort(ABORT, BCS_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if(pGbChgCanCtrl->Tpcm == TPCM_BMV)
    {
        if(pGbChgCnt->WaitAckBmv>= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBmv= 0;
            GbChg_TpcmAbort(ABORT, BMV_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if(pGbChgCanCtrl->Tpcm == TPCM_BMT)
    {
        if(pGbChgCnt->WaitAckBmt >= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBmt = 0;
            GbChg_TpcmAbort(ABORT, BMT_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if(pGbChgCanCtrl->Tpcm == TPCM_BSP)
    {
        if(pGbChgCnt->WaitAckBsp >= TM_MS(1250))
        {
            pGbChgCnt->WaitAckBsp = 0;
            GbChg_TpcmAbort(ABORT, BSP_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }
    /*
     *  end of deal wait Tpcm ack time out
     */

    /*
     deal wait Tpcm cts time out
     */
    if( (pGbChgCanCtrl->Tpcm == TPCM_BRM)
            && (pGbChgCanCtrl->MsgNumBrm == 0) )
    {
        if(pGbChgCnt->WaitCtsBrm >= TM_MS(1050))
        {
            pGbChgCnt->WaitCtsBrm = 0;
            pGbChgCnt->WaitAckBrm = 0;
            GbChg_TpcmAbort(ABORT, BRM_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if( (pGbChgCanCtrl->Tpcm == TPCM_BCP)
            && (pGbChgCanCtrl->MsgNumBcp == 0) )
    {
        if(pGbChgCnt->WaitCtsBcp >= TM_MS(1050))
        {
            pGbChgCnt->WaitCtsBcp = 0;
            pGbChgCnt->WaitAckBcp = 0;
            GbChg_TpcmAbort(ABORT, BCP_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }
    
    if( (pGbChgCanCtrl->Tpcm == TPCM_BCS)
            && (pGbChgCanCtrl->MsgNumBcs == 0) )
    {
        if(pGbChgCnt->WaitCtsBcs >= TM_MS(1050))
        {
            pGbChgCnt->WaitCtsBcs = 0;
            pGbChgCnt->WaitAckBcs = 0;
            GbChg_TpcmAbort(ABORT, BCS_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if( (pGbChgCanCtrl->Tpcm == TPCM_BMV)
            && (pGbChgCanCtrl->MsgNumBmv == 0) )
    {
        if(pGbChgCnt->WaitCtsBmv >= TM_MS(1050))
        {
            pGbChgCnt->WaitCtsBmv = 0;
            pGbChgCnt->WaitAckBmv= 0;
            GbChg_TpcmAbort(ABORT, BMV_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if( (pGbChgCanCtrl->Tpcm == TPCM_BMT)
            && (pGbChgCanCtrl->MsgNumBmt == 0) )
    {
        if(pGbChgCnt->WaitCtsBmt >= TM_MS(1050))
        {
            pGbChgCnt->WaitCtsBmt = 0;
            pGbChgCnt->WaitAckBmt = 0;
            GbChg_TpcmAbort(ABORT, BMT_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }

    if((pGbChgCanCtrl->Tpcm == TPCM_BSP) 
       && (pGbChgCanCtrl->MsgNumBsp == 0))
    {
        if(pGbChgCnt->WaitCtsBsp >= TM_MS(1050))
        {

            pGbChgCnt->WaitCtsBsp = 0;
            pGbChgCnt->WaitAckBsp = 0;
            GbChg_TpcmAbort(ABORT, BSP_PF);
            pGbChgCanCtrl->Tpcm = TPCM_NULL;
        }
    }
    /*
     * end of deal wait Tpcm cts time out 
     */

    /*
    * deal Tpcm send data
    */

    if((pGbChgCanCtrl->MsgNumBrm > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BRM))
    {
        GbChg_TpcmSendData(BRM_PF, pGbChgCanCtrl->MsgNoBrm);		//Brm
        pGbChgCanCtrl->MsgNumBrm -= 1;
        
        if(GbChg_Mode == MODE_2015)
        {
            MSG_NUM_BRM = 7;
        }
        else
        {
            MSG_NUM_BRM = 6; 
        }

        if(pGbChgCanCtrl->MsgNoBrm < MSG_NUM_BRM)
        {
            pGbChgCanCtrl->MsgNoBrm++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBrm = 0;
            pGbChgCanCtrl->MsgNoBrm = 1;
        }
    }

    if((pGbChgCanCtrl->MsgNumBcp > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BCP))
    {
        GbChg_TpcmSendData(BCP_PF, pGbChgCanCtrl->MsgNoBcp);
        pGbChgCanCtrl->MsgNumBcp -= 1;

        if(pGbChgCanCtrl->MsgNoBcp < MSG_NUM_BCP)
        {
            pGbChgCanCtrl->MsgNoBcp++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBcp = 0;
            pGbChgCanCtrl->MsgNoBcp = 1;
        }
    }
    
    if((pGbChgCanCtrl->MsgNumBcs > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BCS))
    {
        GbChg_TpcmSendData(BCS_PF, pGbChgCanCtrl->MsgNoBcs);
        pGbChgCanCtrl->MsgNumBcs -= 1;

        if(pGbChgCanCtrl->MsgNoBcs < MSG_NUM_BCS)
        {
            pGbChgCanCtrl->MsgNoBcs++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBcs = 0;
            pGbChgCanCtrl->MsgNoBcs = 1;
        }
    }

    if((pGbChgCanCtrl->MsgNumBmv > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BMV))
    {
        GbChg_TpcmSendData(BMV_PF, pGbChgCanCtrl->MsgNoBmv);
        pGbChgCanCtrl->MsgNumBmv -= 1;

        if(pGbChgCanCtrl->MsgNoBmv < gby_MsgNumBmv)
        {
            pGbChgCanCtrl->MsgNoBmv++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBmv = 0;
            pGbChgCanCtrl->MsgNoBmv = 1;
        }
    }

    if((pGbChgCanCtrl->MsgNumBmt > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BMT))
    {
        GbChg_TpcmSendData(BMT_PF, pGbChgCanCtrl->MsgNoBmt);
        pGbChgCanCtrl->MsgNumBmt -= 1;

        if(pGbChgCanCtrl->MsgNoBmt < gby_MsgNumBmt)
        {
            pGbChgCanCtrl->MsgNoBmt++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBmt = 0;
            pGbChgCanCtrl->MsgNoBmt = 1;
        }
    }

    if((pGbChgCanCtrl->MsgNumBsp > 0) && (pGbChgCanCtrl->Tpcm == TPCM_BSP))
    {
        GbChg_TpcmSendData(BSP_PF, pGbChgCanCtrl->MsgNoBsp);
        pGbChgCanCtrl->MsgNumBsp -= 1;

        if(pGbChgCanCtrl->MsgNoBsp < MSG_NUM_BSP)
        {
            pGbChgCanCtrl->MsgNoBsp++;
        }
        else				 /* all messages have send */
        {
            pGbChgCanCtrl->MsgNumBsp = 0;
            pGbChgCanCtrl->MsgNoBsp = 1;
        }
    } 
  
}


void GbChgCan_LoadData(void)
{
    uWord wtmp,DC_ChgMaxOut,DC_ChgMinOut;
    uByte btmp;
    uByte lby_chg_enable_ext;
    
    lby_chg_enable_ext = Chg_GetPermitChgExtSt();


    /* can1, for GB charge protocol */
    
    /*BHM*/
    wtmp = Chg_GetPermitMaxSumV();
    GbChgCan_Msg[6].Msg.data[0] = (uByte)(wtmp & 0x00ff);
    GbChgCan_Msg[6].Msg.data[1] = (uByte)((wtmp & 0xff00) >> 8);

    /* Brm */

    /* use Tpcm */

    /* Bcp */

    /* use Tpcm */

    /* Bro */
  #ifdef __DEBUG_GB_CHG_TEST
        lby_chg_enable_ext = 1;
  #endif
   DC_ChgMaxOut= Chg_GetMaxOutPutVoltage();
   wtmp= Chg_GetPermitMaxSumV();
   DC_ChgMinOut = Chg_GetMinOutPutVoltage();
  
    if((GB_FLAG_ENABLE == lby_chg_enable_ext) && (1 == ChgM_GetRelayReady()))// && (wtmp < DC_ChgMaxOut) && (wtmp >= DC_ChgMinOut))       /* need to check  */
    {
        GbChgCan_Msg[0].Msg.data[0] = BMS_READY;		/* BMS Ready to charge */ 
    }
    else
    {
        GbChgCan_Msg[0].Msg.data[0] = BMS_NOT_READY;		/* BMS has Not Ready, charge disable */

    }

    /* Bcl */

    /* max pmt charge sumvolt */
    wtmp = Chg_GetPermitMaxSumV();
    /* 0.1V unit */
    GbChgCan_Msg[1].Msg.data[0] = (uByte)(wtmp & 0x00ff);
    GbChgCan_Msg[1].Msg.data[1] = (uByte)((wtmp & 0xff00) >> 8);

    /* max pmt charge current */
    if(GbChg_Mode == MODE_2015) //GB27930-2015
    {
        btmp = GbChgCan_GetCcsChgEn();   
        if((btmp & 0x01) == 0x01)    //Chg_En
        {
            wtmp = Chg_GetPermitChgCurExt();
            if(wtmp < OFFSET_CUR_GB)
            {
              wtmp = OFFSET_CUR_GB - wtmp; /* 0.1A unit */
            }
            else
            {
              wtmp = 4000;
            }
        }
        else
        {
            wtmp = 4000;       //ÔÝÍ£
        }
    }
    else   //GB27930-2011
    {
        wtmp = Chg_GetPermitChgCurExt();
        if(wtmp < OFFSET_CUR_GB)
        {
          wtmp = OFFSET_CUR_GB - wtmp; /* 0.1A unit */
        }
        else
        {
          wtmp = 4000;
        }    
    }
    GbChgCan_Msg[1].Msg.data[2] = (uByte)(wtmp & 0x00ff);
    GbChgCan_Msg[1].Msg.data[3] = (uByte)((wtmp & 0xff00) >> 8);

    //charge mode
    if(Chg_GetChgStage() == 4)
    {
        btmp = CHG_MODE_CV;
    }
    else 
    {
        btmp = CHG_MODE_CC;
    }
    GbChgCan_Msg[1].Msg.data[4] = btmp; //constant current charge mode

    /* Bcs */

   /* use Tpcm */

    /* Bsm */

    /* max_v_position  */
    wtmp = Bat_GetMaxVTotalNo();
    if(wtmp > 0)
    {       
       GbChgCan_Msg[2].Msg.data[0] = (uByte)wtmp - 1;
    }
    else
    {
        GbChgCan_Msg[2].Msg.data[0] = (uByte)wtmp;
    }
    /* max_t */
    wtmp = Bat_GetMaxT();
    GbChgCan_Msg[2].Msg.data[1] = wtmp + 10;

    /* max_t position */
    wtmp = Bat_GetMaxTTotalNo();
    if(wtmp > 0)
    {
        GbChgCan_Msg[2].Msg.data[2] = (uByte)wtmp - 1;
    }
    else
    {
        GbChgCan_Msg[2].Msg.data[2] = (uByte)wtmp;
    }
    /* Min_t */
    wtmp = Bat_GetMinT();
    GbChgCan_Msg[2].Msg.data[3] = wtmp + 10;

    /* Min_t position */
    wtmp = Bat_GetMinTTotalNo();
    if(wtmp > 0)
    {
        GbChgCan_Msg[2].Msg.data[4] = (uByte)wtmp - 1;
    }
    else
    {
        GbChgCan_Msg[2].Msg.data[4] =(uByte)wtmp ;
    }

    /* battery status */
    GbChgCan_Msg[2].Msg.data[5] = ChgM_Ctrl.Data.BatSt;

    /* iso and connector status and charge enable control */  
    if(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR))
    {
        GbChgCan_Msg[2].Msg.data[6] |= 0x01;   /* iso error */
    }
    else
    {
        GbChgCan_Msg[2].Msg.data[6] &= 0xFE;
    }
    lby_chg_enable_ext = Chg_GetPermitChgExtSt();
    
#ifdef __DEBUG_GB_CHG_TEST
     lby_chg_enable_ext = 1;
#endif
    if(GB_FLAG_ENABLE == lby_chg_enable_ext)
    {
        GbChgCan_Msg[2].Msg.data[6] |= 0x10;   /* charge enable */
    }
    else
    {
        GbChgCan_Msg[2].Msg.data[6] &= 0xCF;
    }
   

    /* Bmv */

    /* use Tpcm */

    /* Bmt */

    /* use Tpcm */

    /* Bsp */

    /* use Tpcm */

    /* Bst */
    GbChgCan_Msg[3].Msg.data[0] = GbChgM_Ctrl.ChgEndReason1;

    GbChgCan_Msg[3].Msg.data[1] = GbChgM_Ctrl.ChgEndReason2;

    GbChgCan_Msg[3].Msg.data[2] = GbChgM_Ctrl.ChgEndReason3;

    GbChgCan_Msg[3].Msg.data[3] = GbChgM_Ctrl.ChgEndReason4; 

    /* Bsd */
    wtmp = Bat_GetSOC();
    wtmp = (uByte)(wtmp / 10);
    GbChgCan_Msg[4].Msg.data[0] = (uByte)wtmp; // SOC, 1% unit

    /* Minv */
    /* 0.01V unit */
    wtmp = Bat_GetMinV();
    wtmp /= 10;
    GbChgCan_Msg[4].Msg.data[1] = (uByte)(wtmp & 0x00ff);
    GbChgCan_Msg[4].Msg.data[2] = (uByte)((wtmp & 0xff00) >> 8);

    /* maxv */
    /* 0.01V unit */
    wtmp = Bat_GetMaxV();
    wtmp /= 10;
    GbChgCan_Msg[4].Msg.data[3] = (uByte)(wtmp & 0x00ff);
    GbChgCan_Msg[4].Msg.data[4] = (uByte)((wtmp & 0xff00) >> 8);

    /* Mint and maxt*/
    
    GbChgCan_Msg[4].Msg.data[5] = Bat_GetMinT() + 10;
    GbChgCan_Msg[4].Msg.data[6] = Bat_GetMaxT() + 10;

    /* Bem */

    GbChgCan_Msg[5].Msg.data[0] = GbChgM_Ctrl.HandShakeErr;
    GbChgCan_Msg[5].Msg.data[1] = GbChgM_Ctrl.ConfigErr;
    GbChgCan_Msg[5].Msg.data[2] = GbChgM_Ctrl.ChargingErr;
    GbChgCan_Msg[5].Msg.data[3] = GbChgM_Ctrl.ChargeEndErr;
}

void GbChgCan_TxData(void)
{
     uByte i;
     uWord rslt = 0;

     GbChgCan_Msg[MSG_BHM].Flg = GbChgM_Ctrl.Flg.Bhm;  
     
     GbChgCan_Msg[MSG_BRO].Flg = GbChgM_Ctrl.Flg.Bro; 
     GbChgCan_Msg[MSG_BCL].Flg = GbChgM_Ctrl.Flg.Bcl;
     GbChgCan_Msg[MSG_BSM].Flg = GbChgM_Ctrl.Flg.Bsm;
     GbChgCan_Msg[MSG_BST].Flg = GbChgM_Ctrl.Flg.Bst;
     GbChgCan_Msg[MSG_BSD].Flg = GbChgM_Ctrl.Flg.Bsd;
     GbChgCan_Msg[MSG_BEM].Flg = GbChgM_Ctrl.Flg.Bem;
     
    for(i = 0; i < GB_CHG_CAN_TX_BUF; i++)                                
    {
        if(0 != GbChgCan_Msg[i].Msg.id)
        {
            GbChgCan_Msg[i].TimeCnt++;
                 
            if(1 == GbChgCan_Msg[i].FirstSendFlag)
            {    
                if((GbChgCan_Msg[i].TimeCnt >= GbChgCan_Msg[i].Period) && (1 == GbChgCan_Msg[i].Flg))
                {
                    GbChgCan_Msg[i].TimeCnt = 0;
                    (void)CanIf_RequestSend(CHG_CAN_CHANNEL, &GbChgCan_Msg[i].Msg, rslt);        
                }    
            }
            else
            {
                if((GbChgCan_Msg[i].TimeCnt >= GbChgCan_Msg[i].DelayTime) && (1 == GbChgCan_Msg[i].Flg))
                {
                    GbChgCan_Msg[i].FirstSendFlag = 1;
                    GbChgCan_Msg[i].TimeCnt = 0;
                    (void)CanIf_RequestSend(CHG_CAN_CHANNEL, &GbChgCan_Msg[i].Msg, rslt);                        
                }
            }
           

        }
    }
}


void GbChg_RxDataFromCharger(sCanIf_MsgType msg)
{                                          
   uWord wtmp;
  
   sGbChgCan_RxType *pGbRxData;
   sGbChgCan_CtrlType *pGbChgCtrl; 
   sGbChgCan_TmCntType *pGbTmCnt; 
   
   pGbRxData = &GbChgCan_RxData;
   pGbTmCnt = &GbChgCan_TmCnt;
   pGbChgCtrl = &GbChgCan_Ctrl;
                   
#ifdef ECU_M_ENABLE_GBCHGIF

    switch (msg.id)
    {
    case ID_CHM:        //GB27930-2015
    {
        GbChg_Mode = MODE_2015;
        pGbRxData->Chm.ChgVer[0] = msg.data[0];
        pGbRxData->Chm.ChgVer[1] = msg.data[1];
        pGbRxData->Chm.ChgVer[2] = msg.data[2];
        pGbRxData->RxFlg |= CHM_RX;
        break;
    }
    
    case ID_CRM:        /* charger recognize message */
    {
        pGbRxData->Crm.RxBms = msg.data[0];
        pGbRxData->Crm.No = msg.data[1];
        pGbRxData->Crm.Region[0] = msg.data[2];
        pGbRxData->Crm.Region[1] = msg.data[3];
        pGbRxData->Crm.Region[2] = msg.data[4];
        pGbRxData->Crm.Region[3] = msg.data[5];
        pGbRxData->Crm.Region[4] = msg.data[6];
        pGbRxData->Crm.Region[5] = msg.data[7];

        pGbRxData->RxFlg |= CRM_RX;
        break;
    }
    case ID_CTS:    /* time synchronization */
    {
       pGbRxData->Cts.Sec = msg.data[0];
       pGbRxData->Cts.Min = msg.data[1];
       pGbRxData->Cts.Hour = msg.data[2];
       pGbRxData->Cts.Day = msg.data[3];
       pGbRxData->Cts.Month = msg.data[4];
      
       wtmp = (((((msg.data[6] >> 4) & 0x0F) * 10) + (msg.data[6] & 0x0F)) * 100 )
                       +  ((((msg.data[5] >> 4) & 0x0F) * 10) + (msg.data[5] & 0x0F));

      
       pGbRxData->Cts.Year = wtmp;

       pGbRxData->RxFlg |= CTS_RX;
       break;
    }
    case ID_CML:    /* maximum output of charger */
    {
        wtmp = msg.data[1];
        wtmp = (wtmp << 8) + msg.data[0];
        pGbRxData->Cml.MaxOutSumv = wtmp;       
        Chg_UpdateMaxOutPutVoltage(wtmp);

        wtmp = msg.data[3];
        wtmp = (wtmp << 8) + msg.data[2];
        pGbRxData->Cml.MinOutSumv = wtmp;
        Chg_UpdateMinOutPutVoltage(wtmp);

        wtmp = msg.data[5];
        wtmp = (wtmp << 8) + msg.data[4];
        if(wtmp < OFFSET_CUR_GB)
        {
           wtmp = OFFSET_CUR - (OFFSET_CUR_GB - wtmp);
        }
        else
        {
           wtmp = 0;
        }
        pGbRxData->Cml.MaxOutCur = wtmp;       
        Chg_UpdateOutPutMaxCurrent(wtmp);
        if((Chg_GetPermitMaxSumV() >= pGbRxData->Cml.MaxOutSumv) || (Chg_GetPermitMaxSumV() <= pGbRxData->Cml.MinOutSumv))
        {
            CarCan_UpdateDcChargeConfig(0);
            GbChgM_Ctrl.ChgStep = ChgStep9;
            GbChgM_Ctrl.Flg.Bem = 1;
        }
        else
        {
            CarCan_UpdateDcChargeConfig(1);
        }
        pGbRxData->RxFlg |= CML_RX;
        break;
    }
    case ID_CRO:    /* charger ready */
    {
        pGbRxData->Cro.Ready = msg.data[0];

        pGbRxData->RxFlg |= CRO_RX;
        break;
    }
    case ID_CCS:    /* charger status */
    {
        wtmp = msg.data[1];
        wtmp = (wtmp << 8) + msg.data[0];
        pGbRxData->Ccs.OutputSumv = wtmp; 
       
        Chg_UpdateOutPutVoltage(wtmp);

        wtmp = msg.data[3];
        wtmp = (wtmp << 8) + msg.data[2];
        if(wtmp < OFFSET_CUR_GB)
        {
           wtmp = OFFSET_CUR - (OFFSET_CUR_GB - wtmp);
        }
        else
        {
           wtmp = 0;
        }
        pGbRxData->Ccs.OutputCur = wtmp;
       
        Chg_UpdateOutPutCurrent(wtmp);

        wtmp = msg.data[5];
        wtmp = (wtmp << 8) + msg.data[4];
        pGbRxData->Ccs.ChgTime = wtmp;
        
        if(GbChg_Mode == MODE_2015)  //GB27930-2011
        {
            pGbRxData->Ccs.ChgEn = msg.data[6];
        }

        pGbRxData->RxFlg |= CCS_RX;
        break;
    }
    case ID_CST:    /* charger request stop charge */
    {
        pGbRxData->Cct.StopReason = msg.data[0];
        pGbRxData->Cct.FaultReason = msg.data[1];
        pGbRxData->Cct.FaultReason &= (msg.data[2] << 8);        
        pGbRxData->Cct.ErrorReason = msg.data[3];
        
        wtmp = pGbRxData->Cct.ErrorReason;
        pGbRxData->Cct.FaultReason = (pGbRxData->Cct.FaultReason & 0x0FFF) + (wtmp << 12);
        Chg_UpdateChargerStopReason(pGbRxData->Cct.StopReason);
        Chg_UpdateChargerFault(pGbRxData->Cct.FaultReason);
        
        
        pGbRxData->RxFlg |= CST_RX;
        break;
    }
    case ID_CSD:    /* Charger Statistics */
    {
        wtmp = msg.data[1];
        wtmp = (wtmp << 8) + msg.data[0];
        pGbRxData->Csd.ChgTime = wtmp;

        wtmp = msg.data[3];
        wtmp = (wtmp << 8) + msg.data[2];
        pGbRxData->Csd.ChgEng = wtmp;

        pGbRxData->Csd.ChgNo = msg.data[4];

        pGbRxData->RxFlg |= CSD_RX;
        break;
    }
    case ID_CEM:    /* Charger error message */
    {
       pGbRxData->RxFlg |= CEM_RX;
        break;
    }
    case ID_TPCM_CHG:	 /* TPCM */
    {
        if(CTS == msg.data[0])
        {
            if(BRM_PF == msg.data[6])
            {
                pGbTmCnt->WaitAckBrm = 0;
                pGbTmCnt->WaitCtsBrm = 0;

                pGbChgCtrl->Flg.RxCtsBrm = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBrm = msg.data[1];
                pGbChgCtrl->MsgNoBrm = msg.data[2];
            }
            else if(BCP_PF == msg.data[6])
            {
                pGbTmCnt->WaitAckBcp = 0;
                pGbTmCnt->WaitCtsBcp = 0;

                pGbChgCtrl->Flg.RxCtsBcp = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBcp = msg.data[1];
                pGbChgCtrl->MsgNoBcp = msg.data[2];
            }
            else if(BCS_PF == msg.data[6])
            {
               
                pGbTmCnt->WaitAckBcs = 0;
                pGbTmCnt->WaitCtsBcs = 0;

               
                pGbChgCtrl->Flg.RxCtsBcs = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBcs = msg.data[1];
                pGbChgCtrl->MsgNoBcs = msg.data[2];
            }
            else if(BMV_PF == msg.data[6])
            {
                pGbTmCnt->WaitAckBmv = 0;
                pGbTmCnt->WaitCtsBmv = 0;

                pGbChgCtrl->Flg.RxCtsBmv = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBmv = msg.data[1];
                pGbChgCtrl->MsgNoBmv = msg.data[2];
            }
            else if(BMT_PF == msg.data[6])
            {
                pGbTmCnt->WaitAckBmt = 0;
                pGbTmCnt->WaitCtsBmt = 0;

                pGbChgCtrl->Flg.RxCtsBmt = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBmt = msg.data[1];
                pGbChgCtrl->MsgNoBmt = msg.data[2];
            }
            else if(BSP_PF == msg.data[6])
            {
                pGbTmCnt->WaitAckBsp = 0;
                pGbTmCnt->WaitCtsBsp = 0;

                pGbChgCtrl->Flg.RxCtsBsp = GB_FLAG_OK;
                pGbChgCtrl->MsgNumBsp = msg.data[1];
                pGbChgCtrl->MsgNoBsp = msg.data[2];
            }
        }
        else if( (END_MSG_ACK == msg.data[0]) || (ABORT == msg.data[0]) )
        {
            pGbTmCnt->WaitAckBrm = 0;
            pGbTmCnt->WaitAckBcp = 0;
            pGbTmCnt->WaitAckBcs = 0;
            pGbTmCnt->WaitAckBmv = 0;
            pGbTmCnt->WaitAckBmt = 0;
            pGbTmCnt->WaitAckBsp = 0;

            pGbTmCnt->WaitCtsBrm = 0;
            pGbTmCnt->WaitCtsBcp = 0;
            pGbTmCnt->WaitCtsBcs = 0;
            pGbTmCnt->WaitCtsBmv = 0;
            pGbTmCnt->WaitCtsBmt = 0;
            pGbTmCnt->WaitCtsBsp = 0;

            pGbChgCtrl->Flg.RxCtsBrm = GB_FLAG_FAIL;
            pGbChgCtrl->Flg.RxCtsBcp = GB_FLAG_FAIL;
            pGbChgCtrl->Flg.RxCtsBcs = GB_FLAG_FAIL;
            pGbChgCtrl->Flg.RxCtsBmv = GB_FLAG_FAIL;
            pGbChgCtrl->Flg.RxCtsBmt = GB_FLAG_FAIL;
            pGbChgCtrl->Flg.RxCtsBsp = GB_FLAG_FAIL;

            pGbChgCtrl->Tpcm = TPCM_NULL;

            pGbChgCtrl->MsgNumBrm = 0;
            pGbChgCtrl->MsgNumBcp = 0;
            pGbChgCtrl->MsgNumBcs = 0;
            pGbChgCtrl->MsgNumBmv = 0;
            pGbChgCtrl->MsgNumBmt = 0;
            pGbChgCtrl->MsgNumBsp = 0;
            
            if((END_MSG_ACK == msg.data[0]) && (BCS_PF == msg.data[6]))
            {
                 pGbChgCtrl->Flg.RxBcsFinish = GB_FLAG_OK;
            }
        }
        else
        {
        }
        break;
    }

    default:
    {
        break;
    }
    }

#endif      

}


