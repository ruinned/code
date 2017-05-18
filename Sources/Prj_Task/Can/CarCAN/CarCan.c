// CarCan.c
#include "CanIf_Type.h"
#include "CanIf.h"
#include "Ecu.h"
#include "CarCan.h"
#include "SchIf.h"
#include "BmsSysSt.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "ChargeSt.h"
#include "ErrorAlarm.h"
#include "HvM.h"
#include "SensorAi.h"
#include "SensorDi.h"
#include "AppRelay.h"
#include "IntCan.h"

#include  "CanIfDiag.h"
#include  "App_com.h"

PduInfoType CanIfPdu;
uint8 data[8] = {0x11};


sCarCanMsgType sCarCan_Msg[MAX_CAR_CAN_SEND_BUF];
typedef struct
{
    uByte MsgLoadProCnt;
    uWord CanErrCnt;
}sCarCan_ProCntType;

sHvM_VcuInfoType VcuInfor;

#define CARCAN_TM_MS(cnt) (cnt / 5)

#define GET_HIGH_BYTE(val) ((uByte)(val >> 8))
#define GET_LOW_BYTE(val)   ((uByte)(val))
#define CarCan_FLAG_OK     1
#define CarCan_FLAG_FAIL   0

#define CARCAN_ERR_L1_MS TM_MS(1000)   // mS
#define CARCAN_ERR_L2_MS TM_MS(1000)   // mS

sCarCan_ProCntType CarCan_ProCnt;

sCarCanMsgType sCarCan_TestMsg;  //test can

uByte flag_test_car_can = 0; 


#define  BMS_CARCAN_TEST_ID      0x001
#define  BMS_SEND_MESSAGE_ID1    0x2A6
#define  BMS_SEND_MESSAGE_ID2    0x36F
#define  BMS_SEND_MESSAGE_ID3    0x377
#define  BMS_SEND_MESSAGE_ID4    0x3A9
#define  BMS_SEND_MESSAGE_ID5    0x3BE
#define  BMS_SEND_MESSAGE_ID6    0x38D
#define  BMS_SEND_MESSAGE_ID7    0x382
#define  BMS_SEND_MESSAGE_ID8    0x6C2
#define  BMS_SEND_MESSAGE_ID9    0x37E
#define  BMS_SEND_MESSAGE_ID10   0x37F
#define  BMS_SEND_MESSAGE_ID11   0x3BF

#define  BMS_RECV_VCU_MESSAGE1   0x1CA
#define  BMS_RECV_VCU_MESSAGE2   0x2A1
#define  BMS_RECV_TGW_MESSAGE1   0x29F
#define  BMS_RECV_TEL_MESSAGE1   0x39E
#define  BMS_RECV_GCU_MESSAGE1   0x170
#define  BMS_RECV_DCU_MESSAGE1   0x181
#define  BMS_RECV_DCDC_MESSAGE1  0x379
#define  BMS_RECV_DCDC_MESSAGE2  0x381
#define  BMS_RECV_OBC_MESSAGE1   0x39C

#define  BMS_RECV_ASK_PHY_ADDR   0x704
#define  BMS_RECV_ASK_FUN_ADDR   0x7df
#define  BMS_SEND_ANSWER_ADDR    0x784

#define  BMS_MAX_TEMPER_NUM    8
#define  BMS_MAX_CELLV_NUM     96

uByte gby_Temp[BMS_MAX_TEMPER_NUM];


//receive message type
typedef union
{
    uByte by[2];

    struct
    {
        uByte Vcu_SysFault           : 4;
        uByte Vcu_SysFaultInv        : 4;
        
        uByte Vcu_KeySt              : 2;
        uByte Vcu_Ems_WarmUpCycleSt  : 1;
        uByte Vcu_7_RlCnt            : 4;
        uByte rsv                    : 1;
    } bits;

}uCarCan_VcuMsg1Type;

uCarCan_VcuMsg1Type VcuMsg1;

typedef union
{
    uByte by[8];

    struct
    {
        uByte VCU_BMS_HvilSt           : 3;
        uByte VCU_BMS_SlpCmd           : 1;
        uByte VCU_BMS_DCChgCmd         : 1;
        uByte VCU_BMS_RlyClsReq        : 2;       
        uByte VCU_BMS_HeatrSt          : 1;
        
        uByte VCU_BMS_APUSt            : 2;
        uByte VCU_BMS_ChgPrioCmd       : 2;
        uByte VCU_AcChargerCapability  : 3;
        uByte VCU_BMS_ACChgCmd         : 1;
        
        uByte VCU_BMS_CrashSt          : 8;
       
        uByte VCU_6_RlCnt              : 4; 
        uByte VCU_BMS_CCSt             : 1;
        uByte rsv                      : 3;
        
        uLWord VCU_BMS_TotalOdometer;
    } bits;

}uCarCan_VcuMsg2Type;

uCarCan_VcuMsg2Type VcuMsg2;

typedef struct
{
    uWord TGW_HeatrPwrAct;
}sCarCan_TgwMsg1Type;

sCarCan_TgwMsg1Type TgwMsg1;

typedef union
{
    uByte by[5];

    struct
    {
        uByte TEL_Time_Year            : 8;
        uByte TEL_Time_Month           : 4;
        uByte TEL_Time_Hour            : 5;
        uByte TEL_Time_Day             : 5;       
        uByte TEL_Time_Min             : 6;
        uByte TEL_Time_Sec             : 6;
        uByte rsv                      : 6;
    } bits;

}uCarCan_TelMsg1Type;

uCarCan_TelMsg1Type TelMsg1;

typedef union
{
    uByte by[5];

    struct
    {
        uByte GCU_3_RlCnt              : 4;
        uByte rsv                      : 4;
        uWord GCU_IdcAct;       
        uWord GCU_UdcAct;
    } bits;

}uCarCan_GcuMsg1Type;

uCarCan_GcuMsg1Type  GcuMsg1;

typedef union
{
    uByte by[5];

    struct
    {
        uByte DCU_3_RlCnt              : 4;
        uByte rsv                      : 4;
        uWord DCU_IdcAct;       
        uWord DCU_UdcAct;
    } bits;

}uCarCan_DcuMsg1Type;

uCarCan_DcuMsg1Type DcuMsg1;

typedef union
{
    uByte by[2];

    struct
    {
        uByte DCDC_TempAct              : 8;
        uByte DCDC_IdcHvOvrErr          : 1;
        uByte rsv                       : 7; 
    } bits;

}uCarCan_DcDcMsg1Type;

uCarCan_DcDcMsg1Type DcDcMsg1;

typedef struct
{
    uWord DCDC_UdcHvAct;
    uWord DCDC_IdcHvAct;
}sCarCan_DcDcMsg2Type;

sCarCan_DcDcMsg2Type DcDcMsg2;

typedef union
{
    uByte by[10];

    struct
    {
        uByte OBC_1_RC              : 4;
        uByte OBC_ModeSt            : 4;
        
        uByte OBC_Fault             : 2;
        uByte OBC_CPSt              : 1; 
        uByte rsv                   : 5;
        
        uWord OBC_OutVoltAct;
        uWord OBC_OutCurrAct;
        uWord OBC_InCurrAct;
        uWord OBC_InputUacAct;   
    } bits;
}uCarCan_ObcMsg1Type;

uCarCan_ObcMsg1Type ObcMsg1;


//send message type
typedef struct
{
    uByte Bms_MsgCnt;
    uByte Bms_MsgMaxFrame;
    uByte Bms_TemperNum;   
}sBms_TemperType;

sBms_TemperType Bms_Temper;

typedef struct
{
    uByte Bms_MsgCnt;
    uByte Bms_MsgMaxFrame;
    uByte Bms_TemperNum;   
}sBms_CellvType;

sBms_CellvType Bms_Cellv;

typedef union
{
    uByte by[3];

    struct
    {
        uByte BMS_BattSt              : 4;
        uByte BMS_BattSt_Inv          : 4;

        uByte BMS_1_RlCnt             : 4; 
        uByte rsv                     : 4; 

        uByte BMS_HvilSt              : 2;
        uByte BMS_InslSt              : 3; 
        uByte rsv1                    : 3;  
    } bits;
}uCarCan_BmsMsg1Type;

uCarCan_BmsMsg1Type BmsMsg1;

typedef union
{
    uByte by[2];

    struct
    {
        uByte BMS_ErrLvl              : 3;
        uByte BMS_ErrLvl_Inv          : 3;
        uByte BMS_EmgLineSt           : 2;
         
        uByte BMS_BalanceSt           : 2; 
        uByte BMS_2_RlCnt             : 4; 
        uByte rsv                     : 2; 
    } bits;
}uCarCan_BmsMsg2Type;

uCarCan_BmsMsg2Type BmsMsg2;

typedef union
{
    uByte by;

    struct
    {
        uByte BMS_3_RlCnt              : 4;
        uByte rsv                      : 4; 
    } bits;
}uCarCan_BmsMsg3Type;

uCarCan_BmsMsg3Type BmsMsg3;

typedef union
{
    uByte by[4];

    struct
    {
        uByte BMS_DcChargerSt              : 2;
        uByte BMS_DcChgSt                  : 1;
        uByte BMS_DcChgErrLvl              : 2;
        uByte BMS_DcChgConfigSt            : 1;
        uByte BMS_DcChgWakeUpLineErr       : 1;
        uByte BMS_DcChgCcLineErr           : 1;
        
        uByte BMS_DcChargerComErr          : 1;
        uByte BMS_DcChargerCurrOvrErr      : 1;
        uByte BMS_DcChgRlyErr              : 1;
        uByte BMS_4_RlCnt                  : 4;
        uByte rsv                          : 1;
        
        uWord BMS_BattSoe;
        
    } bits;
}uCarCan_BmsMsg4Type;

uCarCan_BmsMsg4Type BmsMsg4;

typedef union
{
    uByte by[5];

    struct
    {
        uByte BMS_5_RC              : 4;
        uByte BMS_RepairInd         : 2;
        uByte BMS_FaultLampInd      : 1;
        uByte rsv                   : 1;
        
        uWord BMS_ID;
        uWord BMS_SwVersion;
    } bits;
}uCarCan_BmsMsg5Type;

uCarCan_BmsMsg5Type BmsMsg5;

typedef union
{
    uByte by[8];

    struct
    {
        uByte BMS_BattSOCOvrErrLvl      : 2;
        uByte BMS_BattSOCUndrErrLvl     : 2;
        uByte BMS_BattChgCurrOvrErrLvl  : 2;
        uByte BMS_BattDchgCurrOvrErrLvl : 2; 
        
        uByte BMS_CellVoltOvrErrLvl     : 2;
        uByte BMS_CellVoltUndrErrLvl    : 2;
        uByte BMS_PackVoltOvrErrLvl     : 2;
        uByte BMS_PackVoltUndrErrLvl    : 2;  
        
        uByte BMS_CellTempOvrErrLvl     : 2;
        uByte BMS_CellTempUndrErrLvl    : 2;
        uByte BMS_CellTempUnblcErrLvl   : 2;
        uByte BMS_CellVoltUnblcErrLvl   : 2; 
        
        uByte BMS_6_RlCnt               : 4;
        uByte BMS_BattPTCRlyErr         : 1;
        uByte BMS_BattLiquidSnsErrcc    : 1;
        uByte Notused1                  : 2;
        
        uByte BMS_CellVoltDetcErr       : 1; 
        uByte BMS_CellTempDetcErr       : 1;
        uByte BMS_BattCurrSnsErr        : 1;
        uByte BMS_PackIntVoltEr         : 1;
        uByte BMS_PackOutVoltErr        : 1;
        uByte BMS_BattIntComErr         : 1; 
        uByte BMS_KeyCompareErr         : 1;
        uByte BMS_InitErr               : 1;
        
        uByte BMS_BattPosRlyErr         : 1; 
        uByte BMS_BattNegRlyEr          : 1;
        uByte BMS_BattPchgRlyErr        : 1;
        uByte BMS_BattChgRlyErr         : 1;
        uByte BMS_BattMidRlyErr         : 1;
        uByte BMS_VehComErr             : 1; 
        uByte BMS_BattInslErr           : 1;
        uByte BMS_HvilErr               : 1;
        
        uByte BMS_BattHvCircErr         : 1; 
        uByte BMS_VehSysModeErr         : 1;
        uByte BMS_BattPchgErr           : 1;
        uByte BMS_VehCrashErr           : 1;
        uByte BMS_BattEmgLineErr        : 1;
        uByte BMS_ChargerComErr         : 1; 
        uByte BMS_ChargerCurrOvrErr     : 1;
        uByte BMS_BattLvErr             : 1;
        
        uByte BMS_BattLeakErr           : 1; 
        uByte BMS_BattMainFuseErr       : 1;
        uByte BMS_BattChargerFuseErr    : 1;
        uByte BMS_BattInInslErr         : 1;
        uByte BMS_BattOutInslErr        : 1;
        uByte BMS_BalFuncErr            : 1; 
        uByte Notused2                  : 2;
    } bits;
}uCarCan_BmsMsg6Type;

uCarCan_BmsMsg6Type BmsMsg6;

typedef union
{
    uByte by[4];

    struct
    {
        uByte BMS_OBC_AtvCtrl       : 1;
        uByte BMS_OBC_SlpCtrl       : 1;
        uByte BMS_OBC_HeatModeCtrl  : 1;
        uByte Notused1              : 1;
        uByte BMS_ChgPrioInd        : 1;
        uByte BMS_BattChgSt         : 1;
        uByte BMS_BattChgErr        : 1;
        uByte BMS_BattHeatPrsvSt    : 1;
        
        uByte Notused2              : 4;
        uByte BMS_7_RlCnt           : 4;
        
        uByte BMS_ChgComplRemHour   : 5;
        uByte Notused3              : 3;
        
        uByte BMS_ChgComplRemMinute : 6;
        uByte Notused4              : 2;
    } bits;
}uCarCan_BmsMsg7Type;

uCarCan_BmsMsg7Type BmsMsg7;

typedef struct
{
    uByte BMS_LastTimeChgEnergy;
    uLWord BMS_TotalChgEnergy;
}sCarCan_BmsMsg8Type;

sCarCan_BmsMsg8Type BmsMsg8;

typedef union
{
    uByte by[3];

    struct
    {
        uByte BMS_Data3No1          : 8;
        
        uByte BMS_SysErr            : 1;
        uByte BMS_TempErr           : 1;
        uByte BMS_TempOvrErr        : 1;
        uByte BMS_BattVoltOvrErr    : 1;
        uByte BMS_BattVoltUndErr    : 1;
        uByte BMS_SocUndErr         : 1;
        uByte BMS_CellVoltOvrErr    : 1;
        uByte BMS_CellVoltUndErr    : 1;
        
        uByte BMS_SocOvrErr         : 1;
        uByte BMS_SocJumpErr        : 1;
        uByte BMS_BattMatErr        : 1;
        uByte BMS_CellConsErr       : 1;
        uByte BMS_InsErr            : 1;
        uByte BMS_ChgOvrErr         : 1;
        uByte Notused               : 2;
    } bits;
}uCarCan_BmsMsg9Type;

uCarCan_BmsMsg9Type BmsMsg9;



#define  EMERGENCY_ENABLE
#ifdef EMERGENCY_ENABLE

#define  RELAY_CLOSE              0x04
#define  CHARGE_WAITING_MODE      0x0b
#define  CHARGE_MODE              0x0c
                                  
#define  OBC_STANDY               0x02
uByte gby_RelayOpenCnt;
uByte gby_EmeErr;
uByte gby_DelayOpenRelayFlag;
uWord gwd_DelayOpenRelayCnt;
uByte gby_ObcComErr;
uByte gby_VcuCloseReq;
uByte gby_VcuOpenReq;
#define VCU_CLOSE_REQ   2

#define  EMERGENCY_OPEN_RELAY_DETECT  CPU_AI1_ID
#define  RELAY_OPEN_TIME    3
#define  EMERGENCY_OPEN_RELAY_CNT   2
#define  OFF_CUR   20

static void RelayOpenDelay(uByte time)
{
    uByte i;
    for(i = 0;i < time;i++)
    {
        asm nop;
        asm nop;
    }
}

static void CarCan_OpenAllRelay(void)
{
    sActor_CtrlRelayCmdType CtrlRelayCmd;
    
    CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
    CtrlRelayCmd.CmdType = IMMED_RELAY_CMD;
    (void)Actor_CtrlRelay(POS_RELAY_ID, CtrlRelayCmd);
    (void)Actor_CtrlRelay(PRE_RELAY_ID, CtrlRelayCmd);
    (void)Actor_CtrlRelay(CPU_FAN_ID, CtrlRelayCmd);
    (void)Actor_CtrlRelay(CPU_HEAT_ID, CtrlRelayCmd);
    (void)Actor_CtrlRelay(CHG_RELAY_ID, CtrlRelayCmd);
    (void)Actor_CtrlRelay(NEG_RELAY_ID, CtrlRelayCmd);
    BmsMsg1.bits.BMS_BattSt = 9;
}

static void CarCan_EmergencyOpenRly(void)
{
     if(Sensor_GetAiSt(EMERGENCY_OPEN_RELAY_DETECT) < 2000)
     {
         RelayOpenDelay(RELAY_OPEN_TIME);
         if(Sensor_GetAiSt(EMERGENCY_OPEN_RELAY_DETECT) < 2000)
         {
             gby_RelayOpenCnt++;
             if(gby_RelayOpenCnt >= EMERGENCY_OPEN_RELAY_CNT)
             {    
                 gby_RelayOpenCnt = EMERGENCY_OPEN_RELAY_CNT;
                 CarCan_OpenAllRelay();
             }
         }
         else
         {
             gby_RelayOpenCnt = 0;
         }  
     }
     else
     {
         gby_RelayOpenCnt = 0;
     } 
}
#endif

#define HIVL_DETECT_ENABLE

#ifdef HIVL_DETECT_ENABLE
#define HIVL_DETECT_DI  CPU_DI1_ID
#define HIVL_DETECT_MAX_CNT  3 
#define HIVL_CLOSE  1
#define HIVL_OPEN   0

uByte gby_HivlOpenCnt;
uByte gby_HilvCloseCnt;

static void CarCan_HivlDetect(void)
{
    if(1 == Sensor_GetDiSt(HIVL_DETECT_DI))
    {
        gby_HivlOpenCnt = 0;
        gby_HilvCloseCnt++;
        if(gby_HilvCloseCnt >= HIVL_DETECT_MAX_CNT)
        {
            gby_HilvCloseCnt = HIVL_DETECT_MAX_CNT;
            BatSys_UpdateHvilSt(HIVL_CLOSE);
        }
    }
    else
    {
        gby_HilvCloseCnt = 0;
        gby_HivlOpenCnt++;
        if(gby_HivlOpenCnt >= HIVL_DETECT_MAX_CNT)
        { 
            gby_HivlOpenCnt = HIVL_DETECT_MAX_CNT;
            BatSys_UpdateHvilSt(HIVL_OPEN);
        } 
    }
}
#endif


//#ifdef CAR_CAN_EN
void CarCan_Init(void)
{
    uWord i;
    uByte *ptr;

    sCanIf_AcceptanceCodeType accept;
    sCanIf_AcceptanceMaskType mask;
    
    ptr = (uByte*)sCarCan_Msg;
    for(i = 0; i < sizeof(sCarCan_Msg); i++)
    {
        *(ptr++) = 0;
    }

    CanIf_Init(CAR_CAN_CHANNEL, CANIF_BAUDRATE_500K);
    accept.AccCode1 = 0x00000000;
    mask.AccMask1 =   0xffffffff;

    accept.AccCode2 = 0x00000000;
    mask.AccMask2   = 0xffffffff;           
    CanIf_SetAcceptanceMaskCode(CAR_CAN_CHANNEL, &accept, &mask);    

    // project engineer must initial these msg
    // if not use Msg set id 0
    sCarCan_Msg[0].Msg.id = BMS_SEND_MESSAGE_ID1;
    sCarCan_Msg[0].Msg.length = 8;
    sCarCan_Msg[0].Period = TM_MS(20);
    sCarCan_Msg[0].SendCtrl.DelayTime = 0;

    sCarCan_Msg[1].Msg.id = BMS_SEND_MESSAGE_ID2;//
    sCarCan_Msg[1].Msg.length = 8;
    sCarCan_Msg[1].Period = TM_MS(100);
    sCarCan_Msg[1].SendCtrl.DelayTime = 0;

    sCarCan_Msg[2].Msg.id = BMS_SEND_MESSAGE_ID3;
    sCarCan_Msg[2].Msg.length = 8;
    sCarCan_Msg[2].Period = TM_MS(10);
    sCarCan_Msg[2].SendCtrl.DelayTime = 0;

    sCarCan_Msg[3].Msg.id = BMS_SEND_MESSAGE_ID4;
    sCarCan_Msg[3].Msg.length = 8;
    sCarCan_Msg[3].Period = TM_MS(100);
    sCarCan_Msg[3].SendCtrl.DelayTime = 0;

    sCarCan_Msg[4].Msg.id = BMS_SEND_MESSAGE_ID5;
    sCarCan_Msg[4].Msg.length = 8;
    sCarCan_Msg[4].Period = TM_MS(100);
    sCarCan_Msg[4].SendCtrl.DelayTime = 0;

    sCarCan_Msg[5].Msg.id = BMS_SEND_MESSAGE_ID6;
    sCarCan_Msg[5].Msg.length = 8;
    sCarCan_Msg[5].Period = TM_MS(500);
    sCarCan_Msg[5].SendCtrl.DelayTime = 0;

    sCarCan_Msg[6].Msg.id = BMS_SEND_MESSAGE_ID7;
    sCarCan_Msg[6].Msg.length = 8;
    sCarCan_Msg[6].Period = TM_MS(200);
    sCarCan_Msg[6].SendCtrl.DelayTime = 0;

    sCarCan_Msg[7].Msg.id = BMS_SEND_MESSAGE_ID8;;
    sCarCan_Msg[7].Msg.length = 8;
    sCarCan_Msg[7].Period = TM_MS(1000);
    sCarCan_Msg[7].SendCtrl.DelayTime= 0;

    sCarCan_Msg[8].Msg.id = BMS_SEND_MESSAGE_ID9;
    sCarCan_Msg[8].Msg.length = 8;
    sCarCan_Msg[8].Period = TM_MS(200);
    sCarCan_Msg[8].SendCtrl.DelayTime= 0;

    sCarCan_Msg[9].Msg.id = BMS_SEND_MESSAGE_ID10;
    sCarCan_Msg[9].Msg.length = 8;
    sCarCan_Msg[9].Period = TM_MS(25);
    sCarCan_Msg[9].SendCtrl.DelayTime= 0;
    
    sCarCan_Msg[10].Msg.id = BMS_SEND_MESSAGE_ID11;
    sCarCan_Msg[10].Msg.length = 8;
    sCarCan_Msg[10].Period = TM_MS(100);
    sCarCan_Msg[10].SendCtrl.DelayTime= 0;
    
    for(i = 0; i < sizeof(BmsMsg1); i++)
    {
        BmsMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(BmsMsg2); i++)
    {
        BmsMsg2.by[i] = 0;
    }
    BmsMsg3.by = 0;
    for(i = 0; i < sizeof(BmsMsg4); i++)
    {
        BmsMsg4.by[i] = 0;
    }
    for(i = 0; i < sizeof(BmsMsg5); i++)
    {
        BmsMsg5.by[i] = 0;
    }
    for(i = 0; i < sizeof(BmsMsg6); i++)
    {
        BmsMsg6.by[i] = 0;
    }
    for(i = 0; i < sizeof(BmsMsg7); i++)
    {
        BmsMsg7.by[i] = 0;
    }
    BmsMsg8.BMS_LastTimeChgEnergy = 0;
    BmsMsg8.BMS_TotalChgEnergy = 0;
    for(i = 0; i < sizeof(BmsMsg9); i++)
    {
        BmsMsg9.by[i] = 0;
    }
    
    Bms_Temper.Bms_MsgCnt = 1;
    Bms_Temper.Bms_TemperNum = BMS_MAX_TEMPER_NUM;
    Bms_Cellv.Bms_MsgCnt = 1;
    Bms_Cellv.Bms_TemperNum = BMS_MAX_CELLV_NUM;
    
    for(i = 0; i < sizeof(VcuMsg1); i++)
    {
        VcuMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(VcuMsg2); i++)
    {
        VcuMsg2.by[i] = 0;
    }
    TgwMsg1.TGW_HeatrPwrAct = 0;
    for(i = 0; i < sizeof(TelMsg1); i++)
    {
        TelMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(GcuMsg1); i++)
    {
        GcuMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(DcuMsg1); i++)
    {
        DcuMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(ObcMsg1); i++)
    {
        ObcMsg1.by[i] = 0;
    }
    for(i = 0; i < sizeof(DcDcMsg1); i++)
    {
        DcDcMsg1.by[i] = 0;
    }
    DcDcMsg2.DCDC_IdcHvAct = 0;
    DcDcMsg2.DCDC_UdcHvAct = 0;

    // test car can don't move these code!!!!!!
    sCarCan_TestMsg.Msg.id = 0x002;
    sCarCan_TestMsg.Msg.length = 8;
    for(i = 0; i < 8; i++)
    {
        sCarCan_TestMsg.Msg.data[i] = i+1;
    }
    sCarCan_TestMsg.Period = TM_MS(500);
    sCarCan_TestMsg.SendCtrl.DelayTime = 0;
    // initial other variable

    Bms_Temper.Bms_MsgMaxFrame = (BMS_MAX_TEMPER_NUM % 7);
    if(0 == Bms_Temper.Bms_MsgMaxFrame)
    {
        Bms_Temper.Bms_MsgMaxFrame = (BMS_MAX_TEMPER_NUM / 7);
    }
    else
    {
        Bms_Temper.Bms_MsgMaxFrame = (BMS_MAX_TEMPER_NUM / 7);
        Bms_Temper.Bms_MsgMaxFrame++;
    }
    Bms_Temper.Bms_MsgMaxFrame += 1;
    
    Bms_Cellv.Bms_MsgMaxFrame = (BMS_MAX_CELLV_NUM % 3);
    if(0 == Bms_Cellv.Bms_MsgMaxFrame)
    {
        Bms_Cellv.Bms_MsgMaxFrame = (BMS_MAX_CELLV_NUM / 3);
    }
    else
    {
        Bms_Cellv.Bms_MsgMaxFrame = (BMS_MAX_CELLV_NUM / 3);
        Bms_Cellv.Bms_MsgMaxFrame++;
    }
    Bms_Cellv.Bms_MsgMaxFrame += 2;
    
    BmsMsg9.bits.BMS_Data3No1 = 1;
    
    gby_VcuCloseReq = 0;
    gby_VcuOpenReq = 0;
    
    (void)CanIf_SetCanMode(CAR_CAN_CHANNEL,1);
}

static void CarCan_AcChargeControl(void)
{
    uWord wtmp,wtmp_bak;
    sActor_CtrlRelayCmdType CtrlRelayCmd;
    
    wtmp = 0;
    wtmp_bak = 0;
    BmsMsg7.bits.BMS_OBC_AtvCtrl = 0;
    
    BmsMsg7.bits.BMS_OBC_SlpCtrl = VcuMsg2.bits.VCU_BMS_SlpCmd; 
    if((2 == ObcMsg1.bits.OBC_Fault) || (1 == BmsMsg6.bits.BMS_VehComErr))
    {
        BmsMsg7.bits.BMS_OBC_SlpCtrl = 1;
        BmsMsg1.bits.BMS_BattSt = 8;
        BmsMsg7.bits.BMS_BattChgErr = 1;
    }
    
    if(1 == gby_ObcComErr)
    {
        BmsMsg7.bits.BMS_BattChgErr = 1;
        BmsMsg1.bits.BMS_BattSt = 8;
    }
    
    if((BmsMsg1.bits.BMS_BattSt >= RELAY_CLOSE) && (BmsMsg1.bits.BMS_BattSt <= 8) && (BmsMsg1.bits.BMS_BattSt != 7))
    {
        if((1 == VcuMsg2.bits.VCU_BMS_ACChgCmd) && (0 != VcuMsg2.bits.VCU_BMS_RlyClsReq)
          && (1 != BmsMsg7.bits.BMS_OBC_SlpCtrl) && (1 != BmsMsg7.bits.BMS_BattChgErr)
          && (OBC_STANDY == ObcMsg1.bits.OBC_ModeSt) && (0 == ObcMsg1.bits.OBC_Fault)
          && (1 == Chg_GetPermitChgOnSt()))
        {
            BmsMsg1.bits.BMS_BattSt = CHARGE_WAITING_MODE;
        }
    }
    else if(CHARGE_WAITING_MODE == BmsMsg1.bits.BMS_BattSt)
    {
        if((0 == VcuMsg2.bits.VCU_BMS_ACChgCmd) || (0 == VcuMsg2.bits.VCU_BMS_RlyClsReq)
          || (1 == BmsMsg7.bits.BMS_OBC_SlpCtrl) || (1 == BmsMsg7.bits.BMS_BattChgErr) 
          || (0 != ObcMsg1.bits.OBC_Fault) || (1 != Chg_GetPermitChgOnSt()))
        {
            BmsMsg1.bits.BMS_BattSt = 8;
        }
        else 
        {
            wtmp = Bat_GetSumV(INT_SUMV);
            BmsMsg7.bits.BMS_OBC_AtvCtrl = 1;
            if(wtmp >= ObcMsg1.bits.OBC_OutVoltAct)
            {
                wtmp -= ObcMsg1.bits.OBC_OutVoltAct;
            }
            else
            {
                wtmp = 0;
            }
            if(wtmp > 150)
            {
                wtmp = Bat_GetSumV(INT_SUMV);
            }
            else
            {
                BmsMsg1.bits.BMS_BattSt = CHARGE_MODE;
                wtmp = Chg_GetPermitMaxSumV();
                CtrlRelayCmd.CmdType = IMMED_RELAY_CMD;
                CtrlRelayCmd.Bcmd = RELAY_CLOSE_CMD;
                (void)Actor_CtrlRelay(CHG_RELAY_ID, CtrlRelayCmd);
                wtmp_bak = Chg_GetPermitChgCurOn();
            }
        }
    }
    else if(CHARGE_MODE == BmsMsg1.bits.BMS_BattSt)
    {
        if(1 == gby_EmeErr)
        {
            BmsMsg7.bits.BMS_OBC_AtvCtrl = 0;
            BmsMsg7.bits.BMS_BattChgErr = 1;
            CarCan_OpenAllRelay();
            BmsMsg1.bits.BMS_BattSt = RELAY_CLOSE;
            BmsMsg7.bits.BMS_OBC_SlpCtrl = 1;
            BmsMsg1.bits.BMS_BattSt = 9;
        }
        else if(1 == gby_DelayOpenRelayFlag)
        {
            gwd_DelayOpenRelayCnt++;
            if((gwd_DelayOpenRelayCnt >= TM_MS(3000)) || (Bat_GetCurrent(INSCUR) > (30000 - OFF_CUR)))
            {
                CtrlRelayCmd.CmdType = IMMED_RELAY_CMD;
                CtrlRelayCmd.Bcmd = RELAY_OPEN_CMD;
                (void)Actor_CtrlRelay(CHG_RELAY_ID, CtrlRelayCmd);
                BmsMsg1.bits.BMS_BattSt = 8;
                gby_DelayOpenRelayFlag = 0;
            }
        }
        else if((1 == BmsMsg7.bits.BMS_BattChgSt) || (0 == VcuMsg2.bits.VCU_BMS_ACChgCmd)
          || (1 == BmsMsg7.bits.BMS_BattChgErr) || (1 != Chg_GetPermitChgOnSt()) || (0 != ObcMsg1.bits.OBC_Fault))
        {
            BmsMsg7.bits.BMS_OBC_AtvCtrl = 0;
            gby_DelayOpenRelayFlag = 1;
        }
        else
        {
            BmsMsg7.bits.BMS_OBC_AtvCtrl = 1;
            wtmp = Chg_GetPermitMaxSumV();
            wtmp_bak = Chg_GetPermitChgCurOn();
        }
    }
    sCarCan_Msg[6].Msg.data[0] = (uByte)((wtmp & 0xff00) >> 8);
    sCarCan_Msg[6].Msg.data[1] = (uByte)(wtmp & 0x00ff);
    sCarCan_Msg[6].Msg.data[2] = (uByte)((wtmp_bak & 0xff00) >> 8);
    sCarCan_Msg[6].Msg.data[3] = (uByte)(wtmp_bak & 0x00ff);
}

static void CarCan_MainLoadCell(void)
{
    uByte i,j,lby_tmp;
    uWord lwd_tmp,lwd_tmp1;
    uLWord llwd_tmp;
    
    for(j = 0;j < 8;j++)
    {  
        sCarCan_Msg[0].Msg.data[j] = 0;
        sCarCan_Msg[2].Msg.data[j] = 0;
        sCarCan_Msg[9].Msg.data[j] = 0xff;
    }
    
    //0x2a6
    lby_tmp = BmsMsg1.bits.BMS_BattSt;
    sCarCan_Msg[0].Msg.data[1] |= (lby_tmp & 0x0f);
    
    lwd_tmp = Bat_GetCurrent(INSCUR);
    if(lwd_tmp >= 30000)
    {
        lwd_tmp -= 30000; 
        lwd_tmp /= 5;
        lwd_tmp += 1000;
    }
    else
    {
        lwd_tmp = 30000 - lwd_tmp; 
        lwd_tmp /= 5;
        if(lwd_tmp <= 1000)
        {  
            lwd_tmp = 1000 - lwd_tmp;
        }
        else
        {
            lwd_tmp = 1000;
        }
    }
    
    sCarCan_Msg[0].Msg.data[0] = (uByte)((lwd_tmp & 0x0ff0) >> 4);
    sCarCan_Msg[0].Msg.data[1] |= (uByte)((lwd_tmp & 0x000f) << 4);
    
    lby_tmp = BmsMsg1.bits.BMS_HvilSt;
    sCarCan_Msg[0].Msg.data[2] |= ((lby_tmp & 0x03) << 2);
    
    lby_tmp = BmsMsg1.bits.BMS_BattSt;
    lby_tmp = ~lby_tmp;
    sCarCan_Msg[0].Msg.data[2] |= ((lby_tmp & 0x0f) << 4);
    
    lwd_tmp = Bat_GetSumV(INT_SUMV);
    lwd_tmp /= 10;
    sCarCan_Msg[0].Msg.data[2] |= (uByte)((lwd_tmp & 0x0300) >> 8);
    sCarCan_Msg[0].Msg.data[3] = (uByte)(lwd_tmp & 0x00ff);
    
    if(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR) >= 3)
    {
        BmsMsg1.bits.BMS_InslSt = 1;
    }
    else
    {
        BmsMsg1.bits.BMS_InslSt = 0;
    }
    lby_tmp = BmsMsg1.bits.BMS_InslSt;
    sCarCan_Msg[0].Msg.data[5] |= (lby_tmp & 0x07);
    
    lwd_tmp = BmsSys_GetK30Volt();
    lwd_tmp *= 10;
    sCarCan_Msg[0].Msg.data[5] |= (uByte)((lwd_tmp & 0x0007) << 5);
    sCarCan_Msg[0].Msg.data[4] = (uByte)((lwd_tmp & 0x07f8) >> 3);
    
    lby_tmp = BmsMsg1.bits.BMS_1_RlCnt;
    sCarCan_Msg[0].Msg.data[7] |= (lby_tmp & 0x0f);
    
    if(BatSys_GetIsoPosResistor() < BatSys_GetIsoNegResistor())
    {
        lwd_tmp = BatSys_GetIsoPosResistor();
    }
    else
    {
        lwd_tmp = BatSys_GetIsoNegResistor();
    }
    lwd_tmp /= 2;
    sCarCan_Msg[0].Msg.data[7] |= (uByte)((lwd_tmp & 0x03) << 6);
    sCarCan_Msg[0].Msg.data[6] = (uByte)((lwd_tmp & 0x03fc) >> 2);
    
    lwd_tmp = Bat_GetMaxPulseFeedbackPower();
    lwd_tmp /= 10;
    sCarCan_Msg[2].Msg.data[0] = (uByte)lwd_tmp;
    
    lwd_tmp = Bat_GetMaxPermitFeedbackPower();
    lwd_tmp /= 10;
    sCarCan_Msg[2].Msg.data[1] = (uByte)lwd_tmp;
    
    lby_tmp = BmsMsg3.bits.BMS_3_RlCnt;
    sCarCan_Msg[2].Msg.data[5] = (lby_tmp & 0x0f);
    
    lwd_tmp = Bat_GetMaxPulseDchPower();
    lwd_tmp /= 10;
    sCarCan_Msg[2].Msg.data[6] = (uByte)lwd_tmp;
    
    lwd_tmp = Bat_GetMaxPermitDchPower();
    lwd_tmp /= 10;
    sCarCan_Msg[2].Msg.data[7] = (uByte)lwd_tmp;
    
    //0x37F  send cellv
    if(Bms_Cellv.Bms_MsgCnt > Bms_Cellv.Bms_MsgMaxFrame)
    {
        Bms_Cellv.Bms_MsgCnt = 1;
    }
    sCarCan_Msg[9].Msg.data[0] = Bms_Cellv.Bms_MsgCnt;
    if(1 == Bms_Cellv.Bms_MsgCnt)
    {
        sCarCan_Msg[9].Msg.data[1] = (BMS_MAX_CELLV_NUM / 12); 
        sCarCan_Msg[9].Msg.data[1] |= ((BMS_MAX_CELLV_NUM / 12) << 4);
        
        lwd_tmp = Bat_GetSumV(INT_SUMV);
        sCarCan_Msg[9].Msg.data[2] = (uByte)((lwd_tmp & 0xff00) >> 8);
        sCarCan_Msg[9].Msg.data[3] = (uByte)(lwd_tmp & 0x00ff);
        
        lwd_tmp = Bat_GetCurrent(INSCUR);
        if(lwd_tmp >= 30000)
        {
            lwd_tmp -= 30000; 
            lwd_tmp += 10000;
        }
        else
        {
            lwd_tmp = 30000 - lwd_tmp; 
            if(lwd_tmp <= 10000)
            {  
                lwd_tmp = 10000 - lwd_tmp;
            }
            else
            {
                lwd_tmp = 10000;
            }
        }
        
        sCarCan_Msg[9].Msg.data[4] = (uByte)((lwd_tmp & 0xff00) >> 8);
        sCarCan_Msg[9].Msg.data[5] = (uByte)(lwd_tmp & 0x00ff);
        sCarCan_Msg[9].Msg.data[6] = BMS_MAX_CELLV_NUM;
    }
    else if(2 == Bms_Cellv.Bms_MsgCnt)
    {
        sCarCan_Msg[9].Msg.data[1] = Bms_Cellv.Bms_MsgMaxFrame;
        sCarCan_Msg[9].Msg.data[2] = BMS_MAX_CELLV_NUM;
    }
    else
    {    
        lwd_tmp = (Bms_Cellv.Bms_MsgCnt - 3);
        lwd_tmp *= 3;
        for(i = 1;i < 4;i++)
        {
            if(lwd_tmp < BMS_MAX_CELLV_NUM)
            {  
                lwd_tmp1 = IntCan_GetSysCell(lwd_tmp); 
                sCarCan_Msg[9].Msg.data[i*2] = (uByte)(lwd_tmp1 & 0x00ff);
                sCarCan_Msg[9].Msg.data[i*2-1] = (uByte)((lwd_tmp1 & 0xff00) >> 8);
                lwd_tmp++;
            }
            else
            {
                break; 
            }
        }
    }
}

// call by 10 ms task
void CarCan_MainLoadData(void)
{
    uByte i,j,lby_tmp;
    uWord lwd_tmp,lwd_tmp1;
    uLWord llwd_tmp;
    
    for(i = 3;i < 9;i++)
    {
        for(j = 0;j < 8;j++)
        {  
            sCarCan_Msg[i].Msg.data[j] = 0;
        }
    }
    for(j = 0;j < 8;j++)
    {  
        sCarCan_Msg[1].Msg.data[j] = 0;
        sCarCan_Msg[10].Msg.data[j] = 0xff;
    }

    //0x36f
    lby_tmp = BmsMsg2.bits.BMS_ErrLvl;
    sCarCan_Msg[1].Msg.data[1] |= (lby_tmp & 0x07);
    
    if(gby_RelayOpenCnt >= EMERGENCY_OPEN_RELAY_CNT)
    {
        BmsMsg2.bits.BMS_EmgLineSt = 1; 
    }
    else
    {
        BmsMsg2.bits.BMS_EmgLineSt = 0;
    }
    lby_tmp = BmsMsg2.bits.BMS_EmgLineSt;
    sCarCan_Msg[1].Msg.data[1] |= ((lby_tmp & 0x03) << 3);
    
    lwd_tmp = Bat_GetSOC();
    sCarCan_Msg[1].Msg.data[1] |= (uByte)((lwd_tmp & 0x0007) << 5);
    sCarCan_Msg[1].Msg.data[0] =  (uByte)((lwd_tmp & 0x07f8) >> 3);
    
    lby_tmp = BmsMsg2.bits.BMS_ErrLvl;
    lby_tmp = ~lby_tmp;
    sCarCan_Msg[1].Msg.data[3] |= (lby_tmp & 0x07);
    
    BmsMsg2.bits.BMS_BalanceSt = Bat_GetMainBalanceSt();
    lby_tmp = BmsMsg2.bits.BMS_BalanceSt;
    sCarCan_Msg[1].Msg.data[3] |= ((lby_tmp & 0x03) << 3);
    
    lwd_tmp = Bat_GetSoh();
    sCarCan_Msg[1].Msg.data[3] |= (uByte)((lwd_tmp & 0x0007) << 5);
    sCarCan_Msg[1].Msg.data[2] = (uByte)((lwd_tmp & 0x07f8) >> 3);
    
    lby_tmp = Bat_GetAvgT();
    if(lby_tmp >= 40)
    {
        lby_tmp -= 40; 
        lby_tmp *= 2;
        lby_tmp += 80; 
    }
    else
    {
        lby_tmp = 40 - lby_tmp;
        lby_tmp *= 2;
        if(lby_tmp <= 80)
        {  
            lby_tmp = 80 - lby_tmp;
        }
        else
        {
            lby_tmp = 0;
        }
    }
    sCarCan_Msg[1].Msg.data[4] = lby_tmp;
    
    lby_tmp = Bat_GetMaxT();
    if(lby_tmp >= 40)
    {
        lby_tmp -= 40; 
        lby_tmp *= 2;
        lby_tmp += 80; 
    }
    else
    {
        lby_tmp = 40 - lby_tmp;
        lby_tmp *= 2;
        if(lby_tmp <= 80)
        {  
            lby_tmp = 80 - lby_tmp;
        }
        else
        {
            lby_tmp = 0;
        }
    }
    
    sCarCan_Msg[1].Msg.data[5] = lby_tmp;
    
    lby_tmp = Bat_GetMinT();
    if(lby_tmp >= 40)
    {
        lby_tmp -= 40; 
        lby_tmp *= 2;
        lby_tmp += 80; 
    }
    else
    {
        lby_tmp = 40 - lby_tmp;
        lby_tmp *= 2;
        if(lby_tmp <= 80)
        {  
            lby_tmp = 80 - lby_tmp;
        }
        else
        {
            lby_tmp = 0;
        }
    }
    sCarCan_Msg[1].Msg.data[6] = lby_tmp;
    
    lby_tmp = BmsMsg2.bits.BMS_2_RlCnt;
    sCarCan_Msg[1].Msg.data[7] |= (lby_tmp & 0x0f);
 
    //0x3A9
    BmsMsg4.bits.BMS_DcChargerSt = Chg_GetDcLinkSt();
    lby_tmp = BmsMsg4.bits.BMS_DcChargerSt;
    sCarCan_Msg[3].Msg.data[0] |= (lby_tmp & 0x03);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChgSt;
    sCarCan_Msg[3].Msg.data[0] |= ((lby_tmp & 0x01) << 2);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChgErrLvl;
    sCarCan_Msg[3].Msg.data[0] |= ((lby_tmp & 0x03) << 3);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChgConfigSt;
    sCarCan_Msg[3].Msg.data[0] |= ((lby_tmp & 0x01) << 5);
    
    if((1 == Chg_GetDcLinkSt()) && ((BmsSys_GetOffBDChgVolt() < 90) || (BmsSys_GetOffBDChgVolt() > 360)))
    {
       BmsMsg4.bits.BMS_DcChgWakeUpLineErr = 1;
    }
    else
    {
       BmsMsg4.bits.BMS_DcChgWakeUpLineErr = 0;
    }
    lby_tmp = BmsMsg4.bits.BMS_DcChgWakeUpLineErr;
    sCarCan_Msg[3].Msg.data[0] |= ((lby_tmp & 0x01) << 6);
    
    if((Chg_GetCc2Volt() > 10) && (0 == Chg_GetDcLinkSt()))
    {
       BmsMsg4.bits.BMS_DcChgCcLineErr = 1;
    }
    else
    {
       BmsMsg4.bits.BMS_DcChgCcLineErr = 0;
    }
    lby_tmp = BmsMsg4.bits.BMS_DcChgCcLineErr;
    sCarCan_Msg[3].Msg.data[0] |= ((lby_tmp & 0x01) << 7);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChargerComErr;
    sCarCan_Msg[3].Msg.data[1] |= (lby_tmp & 0x01);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChargerCurrOvrErr;
    sCarCan_Msg[3].Msg.data[1] |= ((lby_tmp & 0x01) << 1);
    
    lby_tmp = BmsMsg4.bits.BMS_DcChgRlyErr;
    sCarCan_Msg[3].Msg.data[1] |= ((lby_tmp & 0x01) << 2);
    
    lby_tmp = BmsMsg4.bits.BMS_4_RlCnt;
    sCarCan_Msg[3].Msg.data[1] |= ((lby_tmp & 0x0f) << 3);
    
    lwd_tmp = Bat_GetMaxV();
    lwd_tmp /= 10;
    lwd_tmp += 435;
    sCarCan_Msg[3].Msg.data[3] |= (uByte)(lwd_tmp & 0x00ff);
    sCarCan_Msg[3].Msg.data[2] |= (uByte)((lwd_tmp & 0x0f00) >> 8);
    
    lwd_tmp = Bat_GetMinV();
    lwd_tmp /= 10;
    lwd_tmp += 435;
    sCarCan_Msg[3].Msg.data[5] |= (uByte)(lwd_tmp & 0x00ff);
    sCarCan_Msg[3].Msg.data[4] |= (uByte)((lwd_tmp & 0x0f00) >> 8);
    
    lwd_tmp = BmsMsg4.bits.BMS_BattSoe;
    sCarCan_Msg[3].Msg.data[7] |= (uByte)((lwd_tmp & 0x0003) << 6);
    sCarCan_Msg[3].Msg.data[6] |= (uByte)((lwd_tmp & 0x03fc) >> 2);
    
    //0x3BE
    lby_tmp = BmsMsg5.bits.BMS_5_RC;
    sCarCan_Msg[4].Msg.data[2] |= (lby_tmp & 0x0f);
    
    lwd_tmp = BmsMsg5.bits.BMS_ID;
    sCarCan_Msg[4].Msg.data[4] = (uByte)(lwd_tmp & 0x00ff);
    sCarCan_Msg[4].Msg.data[3] = (uByte)((lwd_tmp & 0xff00) >> 8);
    
    lwd_tmp = BmsMsg5.bits.BMS_SwVersion;
    sCarCan_Msg[4].Msg.data[6] = (uByte)(lwd_tmp & 0x00ff);
    sCarCan_Msg[4].Msg.data[5] = (uByte)((lwd_tmp & 0xff00) >> 8);
    
    lby_tmp = BmsMsg5.bits.BMS_RepairInd;
    sCarCan_Msg[4].Msg.data[7] = (lby_tmp & 0x03);
    
    lby_tmp = BmsMsg5.bits.BMS_FaultLampInd;
    sCarCan_Msg[4].Msg.data[7] = ((lby_tmp & 0x01) << 2);
    
    //0x38D
    for(i = 0;i < 8;i++)
    {
       BmsMsg6.by[i] = 0;
    }
    
    BmsMsg6.bits.BMS_BattSOCOvrErrLvl = 0;
    BmsMsg6.bits.BMS_BattSOCUndrErrLvl = Err_GetBatErrLevel(ERR_BAT_SOC_LOW);
    BmsMsg9.bits.BMS_SocUndErr = BmsMsg6.bits.BMS_BattSOCUndrErrLvl; 
    BmsMsg6.bits.BMS_CellTempOvrErrLvl = Err_GetBatErrLevel(ERR_BAT_T_HIGH);
    BmsMsg9.bits.BMS_TempOvrErr = BmsMsg6.bits.BMS_CellTempOvrErrLvl;
    BmsMsg6.bits.BMS_CellTempUnblcErrLvl = Err_GetBatErrLevel(ERR_BAT_DELTT_ALLPACK);
    BmsMsg6.bits.BMS_CellVoltOvrErrLvl = Err_GetBatErrLevel(ERR_BAT_CELLV_HIGH);
    BmsMsg9.bits.BMS_CellVoltOvrErr = BmsMsg6.bits.BMS_CellVoltOvrErrLvl;
    BmsMsg6.bits.BMS_CellVoltUnblcErrLvl = Err_GetBatErrLevel(ERR_BAT_DELTV_ALLPACK);
    BmsMsg6.bits.BMS_CellVoltUndrErrLvl = Err_GetBatErrLevel(ERR_BAT_CELLV_LOW);
    BmsMsg9.bits.BMS_CellVoltUndErr = BmsMsg6.bits.BMS_CellVoltUndrErrLvl;
    BmsMsg6.bits.BMS_PackVoltOvrErrLvl = Err_GetBatErrLevel(ERR_BAT_SUMV_HIGH);
    BmsMsg9.bits.BMS_BattVoltOvrErr = BmsMsg6.bits.BMS_PackVoltOvrErrLvl;
    BmsMsg6.bits.BMS_PackVoltUndrErrLvl = Err_GetBatErrLevel(ERR_BAT_SUMV_LOW);
    BmsMsg9.bits.BMS_BattVoltUndErr = BmsMsg6.bits.BMS_PackVoltUndrErrLvl;
    BmsMsg6.bits.BMS_BattChgCurrOvrErrLvl = Err_GetBatErrLevel(ERR_BAT_CUR_HIGH_CHG);
    BmsMsg6.bits.BMS_BattDchgCurrOvrErrLvl = Err_GetBatErrLevel(ERR_BAT_CUR_HIGH_DCH);
    if(Err_GetHwErrLevel(ERR_HW_CAN0))
    {
       BmsMsg6.bits.BMS_VehComErr = 1;
    }
    else
    {
       BmsMsg6.bits.BMS_VehComErr = 0;
    }
    
    if(Err_GetHwErrLevel(ERR_HW_DETECT_CELLV))
    {
       BmsMsg6.bits.BMS_CellVoltDetcErr = 1;
    }
    if(Err_GetHwErrLevel(ERR_HW_DETECT_TEMPER))
    {
       BmsMsg6.bits.BMS_CellTempDetcErr = 1;
       BmsMsg9.bits.BMS_TempErr = 1;
    }
    else
    {
       BmsMsg9.bits.BMS_TempErr = 0;
    }
    if(Err_GetHwErrLevel(ERR_HW_DETECT_CUR))
    {
       BmsMsg6.bits.BMS_BattCurrSnsErr = 1;
    }
    if(Err_GetHwErrLevel(ERR_HW_DETECT_SUMV_IN))
    {
       BmsMsg6.bits.BMS_PackIntVoltEr = 1;
    }
    if(Err_GetHwErrLevel(ERR_HW_DETECT_SUMV_OUT))
    {
       BmsMsg6.bits.BMS_PackOutVoltErr = 1;
    }
    if((Err_GetBatSysErrLevel(ERR_BATSYS_POS_ADJOIN)) || (Err_GetBatSysErrLevel(ERR_BATSYS_POS_NOT_CLOSED)))
    {
       BmsMsg6.bits.BMS_BattPosRlyErr = 1;
    }
    if((Err_GetBatSysErrLevel(ERR_BATSYS_NEG_ADJOIN)) || (Err_GetBatSysErrLevel(ERR_BATSYS_NEG_NOT_CLOSED)))
    {
       BmsMsg6.bits.BMS_BattNegRlyEr = 1;
    }
    if((Err_GetBatSysErrLevel(ERR_BATSYS_PRE_ADJOIN)) || (Err_GetBatSysErrLevel(ERR_BATSYS_PRE_NOT_CLOSED)))
    {
       BmsMsg6.bits.BMS_BattPchgRlyErr = 1;
    }
    if((Err_GetBatSysErrLevel(ERR_BATSYS_CHG_ADJOIN)) || (Err_GetBatSysErrLevel(ERR_BATSYS_CHG_NOT_CLOSED)))
    {
       BmsMsg6.bits.BMS_BattChgRlyErr = 1;
    }
    if(BmsSys_GetK15St() != VcuMsg1.bits.Vcu_KeySt)
    {
        BmsMsg6.bits.BMS_KeyCompareErr = 1;
    }
    if(Err_GetBatSysErrLevel(ERR_BATSYS_ISO_ERR) >= 3)
    {
        BmsMsg6.bits.BMS_BattInInslErr = 1;
        BmsMsg9.bits.BMS_InsErr = 1;
    }
    else
    {
        BmsMsg9.bits.BMS_InsErr = 0;
    }
    if(VcuMsg1.bits.Vcu_SysFault == 4)
    {
        BmsMsg6.bits.BMS_VehSysModeErr = 1;
    }
    
    if(Bat_GetSumV(INT_SUMV) >= Bat_GetSumV(ACC_SUMV))
    {
       lwd_tmp = Bat_GetSumV(INT_SUMV) - Bat_GetSumV(ACC_SUMV);
    }
    else
    {
       lwd_tmp = Bat_GetSumV(ACC_SUMV) - Bat_GetSumV(INT_SUMV);
    }
    if(lwd_tmp >= 500)
    {
       BmsMsg6.bits.BMS_BattHvCircErr = 1;
    }
    if((BmsMsg1.bits.BMS_BattSt >= 4) && (BmsMsg1.bits.BMS_BattSt != 7))
    {
        if(Bat_GetSumV(OUT_SUMV) >= Bat_GetSumV(ACC_SUMV))
        {
           lwd_tmp = Bat_GetSumV(OUT_SUMV) - Bat_GetSumV(ACC_SUMV);
        }
        else
        {
           lwd_tmp = Bat_GetSumV(ACC_SUMV) - Bat_GetSumV(OUT_SUMV);
        }
        if(lwd_tmp >= 600)
        {
           BmsMsg6.bits.BMS_BattHvCircErr = 1;
        }
    }
    
    if(7 == BmsMsg1.bits.BMS_BattSt)
    {
        BmsMsg6.bits.BMS_BattPchgErr = 1;
    }
    
    if(BmsSys_GetK30Volt() < 90)
    {
        BmsMsg6.bits.BMS_BattLvErr = 1;
    }
    
    if(Err_GetBatSysErrLevel(ERR_BATSYS_FUSE_BROKEN))
    {
        BmsMsg6.bits.BMS_BattMainFuseErr = 1;
    }
    
    if(gby_RelayOpenCnt >= EMERGENCY_OPEN_RELAY_CNT)
    {
        BmsMsg6.bits.BMS_BattEmgLineErr = 1;
    }
    
    for(i = 0;i < 8;i++)
    {
        sCarCan_Msg[5].Msg.data[i] = BmsMsg6.by[i];
    }
    
    //0x382
    /*lwd_tmp = 0;//Chg_GetPermitMaxSumV();
    sCarCan_Msg[6].Msg.data[0] = (uByte)((lwd_tmp & 0xff00) >> 8);
    sCarCan_Msg[6].Msg.data[1] = (uByte)(lwd_tmp & 0x00ff);

    lwd_tmp = Chg_GetPermitChgCurOn();
    sCarCan_Msg[6].Msg.data[3] = (uByte)((lwd_tmp & 0xff00) >> 8);
    sCarCan_Msg[6].Msg.data[2] = (uByte)(lwd_tmp & 0x00ff); */
    //if(1 == Chg_GetAcLinkSt())
    {  
        CarCan_AcChargeControl();
    }
    lwd_tmp = Chg_GetRemainChgTime();
    BmsMsg7.bits.BMS_ChgComplRemHour = (uByte)(lwd_tmp / 60);
    BmsMsg7.bits.BMS_ChgComplRemMinute = (uByte)(lwd_tmp % 60);
    sCarCan_Msg[6].Msg.data[4] = BmsMsg7.by[0];
    sCarCan_Msg[6].Msg.data[5] = BmsMsg7.by[1];
    sCarCan_Msg[6].Msg.data[6] = BmsMsg7.by[2];
    sCarCan_Msg[6].Msg.data[7] = BmsMsg7.by[3];
    
    //0x6C2
    sCarCan_Msg[7].Msg.data[0] = BmsMsg8.BMS_LastTimeChgEnergy;
    
    llwd_tmp = BmsMsg8.BMS_TotalChgEnergy;
    sCarCan_Msg[7].Msg.data[3] |= (uByte)((llwd_tmp &0x0000000f) << 4);
    sCarCan_Msg[7].Msg.data[2] = (uByte)((llwd_tmp &0x00000ff0) >> 4);
    sCarCan_Msg[7].Msg.data[1] = (uByte)((llwd_tmp &0x000ff000) >> 12);
    
    //0x37E
    if(BmsMsg9.bits.BMS_Data3No1 > 4)
    {
        BmsMsg9.bits.BMS_Data3No1 = 1;
    }
    if(1 == BmsMsg9.bits.BMS_Data3No1)
    {
        for(i = 0;i < 3;i++)
        {
            sCarCan_Msg[8].Msg.data[i] = BmsMsg9.by[i];
        }  
    }
    else if(2 == BmsMsg9.bits.BMS_Data3No1)
    {
        sCarCan_Msg[8].Msg.data[0] = BmsMsg9.bits.BMS_Data3No1;
        lby_tmp = Bat_GetMaxVBmuNo();
        sCarCan_Msg[8].Msg.data[1] = lby_tmp;
        lwd_tmp = Bat_GetMaxVTotalNo();
        sCarCan_Msg[8].Msg.data[2] = (uByte)(lwd_tmp & 0x00ff); 
        lwd_tmp = Bat_GetMaxV();
        sCarCan_Msg[8].Msg.data[4] = (uByte)(lwd_tmp & 0x00ff);
        sCarCan_Msg[8].Msg.data[3] = (uByte)((lwd_tmp & 0xff00) >> 8);
    }
    else if(3 == BmsMsg9.bits.BMS_Data3No1)
    {
        sCarCan_Msg[8].Msg.data[0] = BmsMsg9.bits.BMS_Data3No1;
        lby_tmp = Bat_GetMinVBmuNo();
        sCarCan_Msg[8].Msg.data[1] = lby_tmp;
        lwd_tmp = Bat_GetMinVTotalNo();
        sCarCan_Msg[8].Msg.data[2] = (uByte)(lwd_tmp & 0x00ff); 
        lwd_tmp = Bat_GetMinV();
        sCarCan_Msg[8].Msg.data[4] = (uByte)(lwd_tmp & 0x00ff);
        sCarCan_Msg[8].Msg.data[3] = (uByte)((lwd_tmp & 0xff00) >> 8);
    }
    else if(4 == BmsMsg9.bits.BMS_Data3No1)
    {
        sCarCan_Msg[8].Msg.data[0] = BmsMsg9.bits.BMS_Data3No1;
        lby_tmp = Bat_GetMaxTBmuNo();
        sCarCan_Msg[8].Msg.data[1] = lby_tmp;
        lwd_tmp = Bat_GetMaxTTotalNo();
        sCarCan_Msg[8].Msg.data[2] = (uByte)(lwd_tmp & 0x00ff);
        lby_tmp = Bat_GetMaxT();
        sCarCan_Msg[8].Msg.data[3] = lby_tmp;
        
        lby_tmp = Bat_GetMinTBmuNo();
        sCarCan_Msg[8].Msg.data[4] = lby_tmp;
        lwd_tmp = Bat_GetMinTTotalNo();
        sCarCan_Msg[8].Msg.data[5] = (uByte)(lwd_tmp & 0x00ff);
        lby_tmp = Bat_GetMinT();
        sCarCan_Msg[8].Msg.data[6] = lby_tmp;
    }
    else
    {
      
    }
    
    //send temper
    for(i = 0;i < BMS_MAX_TEMPER_NUM;i++)
    {
        gby_Temp[i] = Bat_GetTemper(i+1,1);
    }
    
    if(Bms_Temper.Bms_MsgCnt > Bms_Temper.Bms_MsgMaxFrame)
    {
        Bms_Temper.Bms_MsgCnt = 1;
    }
    sCarCan_Msg[10].Msg.data[0] = Bms_Temper.Bms_MsgCnt;
    if(1 == Bms_Temper.Bms_MsgCnt)
    {
        sCarCan_Msg[10].Msg.data[1] = BMS_MAX_TEMPER_NUM; 
    }
    else
    {    
        lwd_tmp = (Bms_Temper.Bms_MsgCnt - 2);
        lwd_tmp *= 7;
        for(i = 1;i < 8;i++)
        {
            if(lwd_tmp < BMS_MAX_TEMPER_NUM)
            {   
                sCarCan_Msg[10].Msg.data[i] = gby_Temp[lwd_tmp];
                lwd_tmp++;
            }
            else
            {
                break; 
            }
        }
    }
}


static void CarCan_ProcMsg(sCanIf_MsgType msg)
{
    uWord lwd_tmp;
    uByte i;
    
    switch(msg.id)
    {
        case BMS_CARCAN_TEST_ID:
        {
            flag_test_car_can = 1;
            break;
        }
        case BMS_RECV_VCU_MESSAGE1:
        {
            CarCan_ProCnt.CanErrCnt = 0;
            for(i = 0;i < 2;i++)
            {
                VcuMsg1.by[i] = 0;
            }
            VcuMsg1.bits.Vcu_KeySt = (msg.data[0] & 0x03);
            VcuMsg1.bits.Vcu_SysFault = ((msg.data[0] & 0xf0) >> 4);
            VcuMsg1.bits.Vcu_SysFaultInv = ((msg.data[1] & 0xf0) >> 4);
            VcuMsg1.bits.Vcu_Ems_WarmUpCycleSt = ((msg.data[2] & 0x80) >> 7);
            VcuMsg1.bits.Vcu_7_RlCnt = (msg.data[7] & 0x0f);
            break;
        }
        case BMS_RECV_VCU_MESSAGE2:
        {
            CarCan_ProCnt.CanErrCnt = 0;
            for(i = 0;i < 8;i++)
            {
                VcuMsg2.by[i] = 0;
            }
            VcuMsg2.bits.VCU_BMS_HvilSt = (msg.data[0] & 0x07);
            VcuMsg2.bits.VCU_BMS_SlpCmd = ((msg.data[0] & 0x08) >> 3); 
            VcuMsg2.bits.VCU_BMS_DCChgCmd = ((msg.data[0] & 0x20) >> 5);
            VcuMsg2.bits.VCU_BMS_RlyClsReq = ((msg.data[0] & 0xc0) >> 6);
            VcuMsg2.bits.VCU_BMS_APUSt = ((msg.data[1] & 0x30) >> 4);
            VcuMsg2.bits.VCU_BMS_ChgPrioCmd = ((msg.data[1] & 0xc0) >> 6);
            VcuMsg2.bits.VCU_BMS_TotalOdometer = (msg.data[1] & 0x0f);
            VcuMsg2.bits.VCU_BMS_TotalOdometer <<= 16;
            lwd_tmp = msg.data[2];
            lwd_tmp <<= 8;
            lwd_tmp |= msg.data[3];
            VcuMsg2.bits.VCU_BMS_TotalOdometer |= lwd_tmp; 
            VcuMsg2.bits.VCU_BMS_CrashSt = msg.data[4];
            VcuMsg2.bits.VCU_6_RlCnt = (msg.data[5] & 0x0f);
            VcuMsg2.bits.VCU_BMS_HeatrSt = ((msg.data[5] & 0x10) >> 4);
            VcuMsg2.bits.VCU_BMS_CCSt = ((msg.data[5] & 0x20) >> 5);
            VcuMsg2.bits.VCU_BMS_ACChgCmd = ((msg.data[5] & 0x40) >> 6);
            VcuMsg2.bits.VCU_AcChargerCapability = (msg.data[6] & 0x07);
            
            if(1 == VcuMsg2.bits.VCU_BMS_RlyClsReq)
            {
                gby_VcuCloseReq++;
                gby_VcuOpenReq = 0;
                if(gby_VcuCloseReq >= VCU_CLOSE_REQ)
                {
                    gby_VcuCloseReq = VCU_CLOSE_REQ;
                    VcuInfor.Cmd = VcuMsg2.bits.VCU_BMS_RlyClsReq;
                }
            }
            else
            {
                gby_VcuCloseReq = 0;
                gby_VcuOpenReq++;
                if(gby_VcuOpenReq >= VCU_CLOSE_REQ)
                {
                    gby_VcuOpenReq = VCU_CLOSE_REQ;
                    VcuInfor.Cmd = 0;
                }
            }
            HvM_UpdateVcuInfo(&VcuInfor);
            
            break;
        }
        case BMS_RECV_TGW_MESSAGE1:
        {
            TgwMsg1.TGW_HeatrPwrAct = 0;
            TgwMsg1.TGW_HeatrPwrAct = (msg.data[4] & 0x0f);
            TgwMsg1.TGW_HeatrPwrAct <<= 6;
            TgwMsg1.TGW_HeatrPwrAct |= ((msg.data[5] & 0xfc) >> 2);
            break;
        }
        case BMS_RECV_TEL_MESSAGE1:
        {
            for(i = 0;i < 5;i++)
            {
                TelMsg1.by[i] = 0;
            }
            TelMsg1.bits.TEL_Time_Year = msg.data[0];
            TelMsg1.bits.TEL_Time_Month = ((msg.data[1] & 0xf0) >> 4);
            TelMsg1.bits.TEL_Time_Hour = ((msg.data[2] & 0x7c) >> 2);
            TelMsg1.bits.TEL_Time_Day = ((msg.data[2] & 0x80) >> 7);
            TelMsg1.bits.TEL_Time_Day |= ((msg.data[1] & 0x0f) << 1);
            TelMsg1.bits.TEL_Time_Min = ((msg.data[3] & 0xf0) >> 4);
            TelMsg1.bits.TEL_Time_Min |= ((msg.data[2] & 0x03) << 4);
            TelMsg1.bits.TEL_Time_Sec = ((msg.data[4] & 0xc0) >> 6);
            TelMsg1.bits.TEL_Time_Sec |= ((msg.data[3] & 0x0f) << 2);
            break;
        }
        case BMS_RECV_GCU_MESSAGE1:
        {
            for(i = 0;i < 5;i++)
            {
                GcuMsg1.by[i] = 0;
            }
            GcuMsg1.bits.GCU_3_RlCnt = ((msg.data[4] & 0xf0) >> 4);
            GcuMsg1.bits.GCU_IdcAct = (msg.data[4] & 0x07);
            GcuMsg1.bits.GCU_IdcAct <<= 8;
            GcuMsg1.bits.GCU_IdcAct |= msg.data[5];
            GcuMsg1.bits.GCU_UdcAct = (msg.data[6] & 0x03);
            GcuMsg1.bits.GCU_UdcAct <<= 8;
            GcuMsg1.bits.GCU_UdcAct |= msg.data[7];
            break;
        }
        case BMS_RECV_DCU_MESSAGE1:
        {
            for(i = 0;i < 5;i++)
            {
                DcuMsg1.by[i] = 0;
            }
            DcuMsg1.bits.DCU_3_RlCnt = ((msg.data[4] & 0xf0) >> 4);
            DcuMsg1.bits.DCU_IdcAct = (msg.data[4] & 0x07);
            DcuMsg1.bits.DCU_IdcAct <<= 8;
            DcuMsg1.bits.DCU_IdcAct |= msg.data[5];
            DcuMsg1.bits.DCU_UdcAct = (msg.data[6] & 0x03);
            DcuMsg1.bits.DCU_UdcAct <<= 8;
            DcuMsg1.bits.DCU_UdcAct |= msg.data[7];
            VcuInfor.VcuSumV = DcuMsg1.bits.DCU_UdcAct;
            HvM_UpdateVcuInfo(&VcuInfor);
            break;
        }
        case BMS_RECV_DCDC_MESSAGE2:
        {
            for(i = 0;i < 2;i++)
            {
                DcDcMsg1.by[i] = 0;
            }
            DcDcMsg1.bits.DCDC_IdcHvOvrErr = ((msg.data[1] & 0x40) >> 6);
            DcDcMsg1.bits.DCDC_TempAct = msg.data[3];
            break;
        }
        case BMS_RECV_DCDC_MESSAGE1:
        {
            DcDcMsg2.DCDC_IdcHvAct = 0;
            DcDcMsg2.DCDC_IdcHvAct = 0;
            DcDcMsg2.DCDC_UdcHvAct = msg.data[0]; 
            DcDcMsg2.DCDC_UdcHvAct <<= 2;
            DcDcMsg2.DCDC_UdcHvAct |= ((msg.data[1] & 0xc0) >> 6);
            
            DcDcMsg2.DCDC_IdcHvAct = (msg.data[2] & 0x0f); 
            DcDcMsg2.DCDC_IdcHvAct <<= 8;
            DcDcMsg2.DCDC_IdcHvAct |= msg.data[3];
            break;
        }
        case BMS_RECV_OBC_MESSAGE1:
        {
            for(i = 0;i < 10;i++)
            {
                ObcMsg1.by[i] = 0;
            }
            ObcMsg1.bits.OBC_OutVoltAct = msg.data[0];
            ObcMsg1.bits.OBC_OutVoltAct <<= 5;
            ObcMsg1.bits.OBC_OutVoltAct |= ((msg.data[1] & 0xf8) >> 3);
            ObcMsg1.bits.OBC_OutCurrAct = (msg.data[1] & 0x03);
            ObcMsg1.bits.OBC_OutCurrAct <<= 7;
            ObcMsg1.bits.OBC_OutCurrAct |= ((msg.data[2] & 0xfe) >> 1);
            ObcMsg1.bits.OBC_InCurrAct = (msg.data[2] & 0x01);
            ObcMsg1.bits.OBC_InCurrAct <<= 8;
            ObcMsg1.bits.OBC_InCurrAct |= msg.data[3];
            ObcMsg1.bits.OBC_ModeSt = (msg.data[5] & 0x0f);
            ObcMsg1.bits.OBC_CPSt = ((msg.data[5] & 0x40) >> 6); 
            ObcMsg1.bits.OBC_InputUacAct = msg.data[4];
            ObcMsg1.bits.OBC_InputUacAct <<= 1;
            ObcMsg1.bits.OBC_InputUacAct |= ((msg.data[5] & 0x80) >> 7);
            ObcMsg1.bits.OBC_Fault = (msg.data[6] & 0x03);
            ObcMsg1.bits.OBC_1_RC = ((msg.data[6] & 0xf0) >> 4);
            break;
        }
        case BMS_RECV_ASK_PHY_ADDR:
        {
            
            break;
        }
        case BMS_RECV_ASK_FUN_ADDR:
        {
            
            break;
        }
        default :
        {
            break;
        }
    } 
}


// call cycle 5mS by task
void CarCan_MainSendMsg(void)
{
    uByte i;
    uWord rslt = 0;
 #ifdef EMERGENCY_ENABLE   
   // CarCan_EmergencyOpenRly();
 #endif
    CarCan_MainLoadCell();
    if(1 == flag_test_car_can)
    {
        sCarCan_TestMsg.SendCtrl.TimeCnt++;
        if(sCarCan_TestMsg.SendCtrl.TimeCnt >= sCarCan_TestMsg.Period)    //send test msg
        {
            sCarCan_TestMsg.SendCtrl.TimeCnt = 0;
            //(void)CanIf_RequestSend(CAR_CAN_CHANNEL, &sCarCan_TestMsg.Msg, rslt);
            App_TxCanData(sCarCan_TestMsg.Msg);
        }
    }
    else
    {
        sCarCan_TestMsg.SendCtrl.TimeCnt = 0;    
    }

    for(i = 0; i < MAX_CAR_CAN_SEND_BUF; i++)
    {
        if(0 != sCarCan_Msg[i].Msg.id)
        {
            sCarCan_Msg[i].SendCtrl.TimeCnt++;
            
            if(1 == sCarCan_Msg[i].SendCtrl.FirstSendFlag)
            {
                if(sCarCan_Msg[i].SendCtrl.TimeCnt >= sCarCan_Msg[i].Period )
                {
                   sCarCan_Msg[i].SendCtrl.TimeCnt = 0;

                   //(void)CanIf_RequestSend(CAR_CAN_CHANNEL, &sCarCan_Msg[i].Msg, rslt);
                   App_TxCanData(sCarCan_Msg[i].Msg);
                   switch(i)
                   {
                      case 0:
                      {
                           BmsMsg1.bits.BMS_1_RlCnt++;
                           break;
                      }
                      case 1:
                      {
                           BmsMsg2.bits.BMS_2_RlCnt++;
                           break;
                      }
                      case 2:
                      {
                           BmsMsg3.bits.BMS_3_RlCnt++;
                           break;
                      }
                      case 3:
                      {
                           BmsMsg4.bits.BMS_4_RlCnt++;
                           break;
                      }
                      case 4:
                      {
                           BmsMsg5.bits.BMS_5_RC++;
                           break;
                      }
                      case 5:
                      {
                           BmsMsg6.bits.BMS_6_RlCnt++;
                           break;
                      }
                      case 6:
                      {
                           BmsMsg7.bits.BMS_7_RlCnt++;
                           break;
                      }
                      case 7:
                      {
                           break;
                      }
                      case 8:
                      {
                           BmsMsg9.bits.BMS_Data3No1++;
                           break;
                      }
                      case 9:
                      {
                           Bms_Cellv.Bms_MsgCnt++;
                           break;
                      }
                      case 10:
                      {
                           Bms_Temper.Bms_MsgCnt++;
                           break;
                      }
                      default :
                      {
                           break; 
                      }
                   }
                }
            }
            else
            {
                if(sCarCan_Msg[i].SendCtrl.TimeCnt >= sCarCan_Msg[i].SendCtrl.DelayTime)
                {
                   sCarCan_Msg[i].SendCtrl.FirstSendFlag = 1;
                   sCarCan_Msg[i].SendCtrl.TimeCnt = 0;
                   //(void)CanIf_RequestSend(CAR_CAN_CHANNEL, &sCarCan_Msg[i].Msg, rslt);
                   App_TxCanData(sCarCan_Msg[i].Msg);
                   switch(i)
                   {
                      case 0:
                      {
                           BmsMsg1.bits.BMS_1_RlCnt++;
                           break;
                      }
                      case 1:
                      {
                           BmsMsg2.bits.BMS_2_RlCnt++;
                           break;
                      }
                      case 2:
                      {
                           BmsMsg3.bits.BMS_3_RlCnt++;
                           break;
                      }
                      case 3:
                      {
                           BmsMsg4.bits.BMS_4_RlCnt++;
                           break;
                      }
                      case 4:
                      {
                           BmsMsg5.bits.BMS_5_RC++;
                           break;
                      }
                      case 5:
                      {
                           BmsMsg6.bits.BMS_6_RlCnt++;
                           break;
                      }
                      case 6:
                      {
                           BmsMsg7.bits.BMS_7_RlCnt++;
                           break;
                      }
                      case 7:
                      {
                           break;
                      }
                      case 8:
                      {
                           BmsMsg9.bits.BMS_Data3No1++;
                           break;
                      }
                      case 9:
                      {
                           Bms_Cellv.Bms_MsgCnt++;
                           break;
                      }
                      case 10:
                      {
                           Bms_Temper.Bms_MsgCnt++;
                           break;
                      }
                      default :
                      {
                           break; 
                      }
                   }
                }
                
            }
  
        }
    }

}

//call by main task
void CarCan_MainReceiveMsg(void)
{   
    GeneralReturnType  rslt;      
    uByte buf_length;
    sCanIf_MsgType msg;
    
    /* have data ? */
    buf_length = CanIf_GetRxBufferLength(CAR_CAN_CHANNEL);
    while(buf_length)
    {
         rslt = CanIf_RxIndication(CAR_CAN_CHANNEL,&msg);
         if(!rslt)
         {
             CarCan_ProcMsg(msg);
         } 
         buf_length--;       
    }

}
static void CarCan_CheckStatus(void)
{
    CarCan_ProCnt.CanErrCnt++;
    if(CarCan_ProCnt.CanErrCnt >= CARCAN_ERR_L2_MS)  // 500* 10ms
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_CAN0, ERR_LEVEL_TWO);
        CarCan_ProCnt.CanErrCnt = CARCAN_ERR_L2_MS;
    }
    else if(CarCan_ProCnt.CanErrCnt >= CARCAN_ERR_L1_MS)
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_CAN0, ERR_LEVEL_ONE);
    }
    else
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_CAN0, ERR_LEVEL_NORMAL);
    }
}

void CarCan_CountCbk(void)
{
    (CarCan_ProCnt.MsgLoadProCnt < 255) ? (CarCan_ProCnt.MsgLoadProCnt++) : (CarCan_ProCnt.MsgLoadProCnt = 255);
}

void CarCan_ProMain(void)
{
    if(CarCan_ProCnt.MsgLoadProCnt >= TM_MS(10))
    {

        SaveStatusReg();
        CarCan_MainLoadData();   
 
        RestoreStatusReg();
        CarCan_ProCnt.MsgLoadProCnt = 0;
    #ifdef HIVL_DETECT_ENABLE
        //CarCan_HivlDetect();
    #endif
        //CarCan_CheckStatus();
    }
}

//VcuMsg1
uByte CarCan_GetVcuWarmUpCycleSt(void)
{
    return VcuMsg1.bits.Vcu_Ems_WarmUpCycleSt;
}
uByte CarCan_GetVcuKeySt(void)
{
    return VcuMsg1.bits.Vcu_KeySt;
}
uByte CarCan_GetVcuSysFaultSt(void)
{
    return VcuMsg1.bits.Vcu_SysFault;
}
//VcuMsg2
uByte CarCan_GetVcuApuSt(void)
{
    return VcuMsg2.bits.VCU_BMS_APUSt;
}
uByte CarCan_GetVcuChgPrioCmd(void)
{
    return VcuMsg2.bits.VCU_BMS_ChgPrioCmd;
}
uByte CarCan_GetVcuCrashSt(void)
{
    return VcuMsg2.bits.VCU_BMS_CrashSt;
}
uByte CarCan_GetVcuChargerCapaSt(void)
{
    return VcuMsg2.bits.VCU_AcChargerCapability;
}
uByte CarCan_GetVcuAcChargeCom(void)
{
    return VcuMsg2.bits.VCU_BMS_ACChgCmd;
}
uByte CarCan_GetVcuAcChargeCCSt(void)
{
    return VcuMsg2.bits.VCU_BMS_CCSt;
}
uByte CarCan_GetVcuDcChargeCom(void)
{
    return VcuMsg2.bits.VCU_BMS_DCChgCmd;
}
uByte CarCan_GetVcuHeatrSt(void)
{
    return VcuMsg2.bits.VCU_BMS_HeatrSt;
}
uByte CarCan_GetVcuHvilSt(void)
{
    return VcuMsg2.bits.VCU_BMS_HvilSt;
}
uByte CarCan_GetVcuRlyClsReqCmd(void)
{
    return VcuMsg2.bits.VCU_BMS_RlyClsReq;
}
uByte CarCan_GetVcuSlpCmd(void)
{
    return VcuMsg2.bits.VCU_BMS_SlpCmd;
}
uLWord CarCan_GetVcuTotalOdoMeter(void)
{
    return VcuMsg2.bits.VCU_BMS_TotalOdometer;
}
//ObcMsg1
uByte CarCan_GetObcCpSt(void)
{
    return ObcMsg1.bits.OBC_CPSt;
}
uByte CarCan_GetObcFault(void)
{
    return ObcMsg1.bits.OBC_Fault;
}
uWord CarCan_GetObcInCurr(void)
{
    return ObcMsg1.bits.OBC_InCurrAct;
}
uWord CarCan_GetObcInVolt(void)
{
    return ObcMsg1.bits.OBC_InputUacAct;
}
uWord CarCan_GetObcOutCurr(void)
{
    return ObcMsg1.bits.OBC_OutCurrAct;
}
uWord CarCan_GetObcOutVolt(void)
{
    return ObcMsg1.bits.OBC_OutVoltAct;
}
uByte CarCan_GetObcModeSt(void)
{
    return ObcMsg1.bits.OBC_ModeSt;
}
//GcuMsg1
uWord CarCan_GetGcuIdc(void)
{
    return GcuMsg1.bits.GCU_IdcAct;
}
uWord CarCan_GetGcuUdc(void)
{
    return GcuMsg1.bits.GCU_UdcAct;
}
//DcuMsg1
uWord CarCan_GetDcuIdc(void)
{
    return DcuMsg1.bits.DCU_IdcAct;
}
uWord CarCan_GetDcuUdc(void)
{
    return DcuMsg1.bits.DCU_UdcAct;
}
//TgwMsg1
uWord CarCan_GetTgwPwr(void)
{
    return TgwMsg1.TGW_HeatrPwrAct;
}

//DcDcMsg1
uByte CarCan_GetDcDcIdcHvOvrErr(void)
{
    return DcDcMsg1.bits.DCDC_IdcHvOvrErr;
}
uByte CarCan_GetDcDcTempAct(void)
{
    return DcDcMsg1.bits.DCDC_TempAct;
}
//DcDcMsg2
uWord CarCan_GetDcDcIdc(void)
{
    return DcDcMsg2.DCDC_IdcHvAct;
}
uWord CarCan_GetDcDcUdc(void)
{
    return DcDcMsg2.DCDC_UdcHvAct;
}

//BmsMsg1
void CarCan_UpdateBmsMode(uByte Mode)
{
    BmsMsg1.bits.BMS_BattSt = Mode;
}

uByte CarCan_GetBmsMode(void)
{
    return BmsMsg1.bits.BMS_BattSt;
}

void CarCan_UpdateBmsHvilSt(uByte St)
{
    BmsMsg1.bits.BMS_HvilSt = St;
}

uByte CarCan_GetBmsHvilSt(void)
{
    return BmsMsg1.bits.BMS_HvilSt;
}

void CarCan_UpdateBmsInslSt(uByte St)
{
    BmsMsg1.bits.BMS_InslSt = St;
}

uByte CarCan_GetBmsInslSt(void)
{
    return BmsMsg1.bits.BMS_InslSt;
}

//BmsMsg2
void CarCan_UpdateBmsErrLvl(uByte Err)
{
    BmsMsg2.bits.BMS_ErrLvl = Err;
}

uByte CarCan_GetBmsErrLvl(void)
{
    return BmsMsg2.bits.BMS_ErrLvl;
}

//BmsMsg4
void CarCan_UpdateDcChargerComErr(uByte Err)
{
    BmsMsg4.bits.BMS_DcChargerComErr = Err;
}

uByte CarCan_GetDcChargerComErr(void)
{
    return BmsMsg4.bits.BMS_DcChargerComErr;
}

void CarCan_UpdateDcChargerCurOverErr(uByte Err)
{
    BmsMsg4.bits.BMS_DcChargerCurrOvrErr = Err;
}

uByte CarCan_GetDcChargerCurOverErr(void)
{
    return BmsMsg4.bits.BMS_DcChargerCurrOvrErr;
}

void CarCan_UpdateDcChargerLinkSt(uByte St)
{
    BmsMsg4.bits.BMS_DcChargerSt = St;
}

uByte CarCan_GetDcChargerLinkSt(void)
{
    return BmsMsg4.bits.BMS_DcChargerSt;
}

void CarCan_UpdateDcChargerCcLineErrSt(uByte St)
{
    BmsMsg4.bits.BMS_DcChgCcLineErr = St;
}

uByte CarCan_GetDcChargerCcLineErrSt(void)
{
    return BmsMsg4.bits.BMS_DcChgCcLineErr;
}

void CarCan_UpdateDcChRlyErrSt(uByte St)
{
    BmsMsg4.bits.BMS_DcChgRlyErr = St;
}

uByte CarCan_GetDcChRlyErrSt(void)
{
    return BmsMsg4.bits.BMS_DcChgRlyErr;
}

void CarCan_UpdateDcChErrLvl(uByte St)
{
    BmsMsg4.bits.BMS_DcChgErrLvl = St;
}

uByte CarCan_GetDcChErrLvl(void)
{
    return BmsMsg4.bits.BMS_DcChgErrLvl;
}

void CarCan_UpdateDcChargeFull(uByte flag)
{
    BmsMsg4.bits.BMS_DcChgSt = flag;
}

uByte CarCan_GetDcChargeFull(void)
{
    return BmsMsg4.bits.BMS_DcChgSt;
}

void CarCan_UpdateDcChgWakeUpLineErr(uByte flag)
{
    BmsMsg4.bits.BMS_DcChgWakeUpLineErr = flag;
}

uByte CarCan_GetDcDcChgWakeUpLineErr(void)
{
    return BmsMsg4.bits.BMS_DcChgWakeUpLineErr;
}

void CarCan_UpdateDcChargeConfig(uByte config)
{
   BmsMsg4.bits.BMS_DcChgConfigSt = config;
}

uByte CarCan_GetDcChargeConfig(void)
{
   return BmsMsg4.bits.BMS_DcChgConfigSt;
}


//BmsMsg7
void CarCan_UpdateAcBattChgErr(uByte Err)
{
    BmsMsg7.bits.BMS_BattChgErr = Err;
}

uByte CarCan_GetAcBattChgErr(void)
{
    return BmsMsg7.bits.BMS_BattChgErr;
}

void CarCan_UpdateAcChargeFull(uByte flag)
{
    BmsMsg7.bits.BMS_BattChgSt = flag;
}

uByte CarCan_GetAcChargeFull(void)
{
    return BmsMsg7.bits.BMS_BattChgSt;
}

void CarCan_UpdateAcBattHeatPrsv(uByte flag)
{
    BmsMsg7.bits.BMS_BattHeatPrsvSt = flag;
}

uByte CarCan_GetAcBattHeatPrsv(void)
{
    return BmsMsg7.bits.BMS_BattHeatPrsvSt;
}

void CarCan_UpdateAcChgPrioInd(uByte flag)
{
    BmsMsg7.bits.BMS_ChgPrioInd = flag;
}

uByte CarCan_GetAcChgPrioInd(void)
{
    return BmsMsg7.bits.BMS_ChgPrioInd;
}

void CarCan_UpdateAcChgAtvCtrl(uByte flag)
{
    BmsMsg7.bits.BMS_OBC_AtvCtrl = flag;
}

uByte CarCan_GetAcChgAtvCtrl(void)
{
    return BmsMsg7.bits.BMS_OBC_AtvCtrl;
}

void CarCan_UpdateAcChgHeatModeCtrl(uByte flag)
{
    BmsMsg7.bits.BMS_OBC_HeatModeCtrl = flag;
}

uByte CarCan_GetAcChgHeatModeCtrl(void)
{
    return BmsMsg7.bits.BMS_OBC_HeatModeCtrl;
}

void CarCan_UpdateAcChgSlpCtrl(uByte flag)
{
    BmsMsg7.bits.BMS_OBC_SlpCtrl = flag;
}

uByte CarCan_GetAcChgSlpCtrl(void)
{
    return BmsMsg7.bits.BMS_OBC_SlpCtrl;
}

void CarCan_UpdateAcChgComplRemHour(uByte Hour)
{
    BmsMsg7.bits.BMS_ChgComplRemHour = Hour;
}

uByte CarCan_GetAcChgComplRemHour(void)
{
    return BmsMsg7.bits.BMS_ChgComplRemHour;
}

void CarCan_UpdateAcChgComplRemMinute(uByte Minu)
{
    BmsMsg7.bits.BMS_ChgComplRemMinute = Minu;
}

uByte CarCan_GetAcChgComplRemMinute(void)
{
    return BmsMsg7.bits.BMS_ChgComplRemMinute;
}


//#endif


