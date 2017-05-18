// IntCan.c
#include "CanIf_Type.h"
#include "CanIf.h"
#include "CanTp.h"
#include "CanTp_Type.h"
#include "CanCom.h"
#include "Ecu.h"
#include "BatterySt.h"
#include "BatterySysSt.h"
#include "ChargeSt.h"
#include "BmsSysSt.h"
#include "IntCan.h"
#include "SysConf.h"
#include "AppRelay.h"
#include "ErrorAlarm.h"
#include "EE.h"
#include "version.h"
#include "SchIf.h"
#include "EcuDebug.h"
#include "AppDo.h"
#include "DoIf.h"
#include "SensorDi.h"
#include "BalM.h"
#include "SensorAi.h"
#include "DataStore.h"
#include "ExctCmd.h"
#include "ATB.h"
#include "LvM.h"
#include "RunModem.h"
#include "IntCan_StaticSet.h"

#include "CurDet.h"
#include "CurrentIf.h"
#include "SumvDet.h"
#include "SumvIf.h"
#include "ClockIf.h"
#include "IntCan_TestFrame.h"
#include "Project.h"

#include "Xgate_RealTime.h"

#include "wdi.h"
#include "WdgIf.h"
#include "Elock.h"

#if(ECU_RAM_USE_MODE)
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM
#endif
#else
#pragma DATA_SEG DEFAULT
#endif

uWord gwd_sys_cellv[MAX_PACK_CELL_NUM];
uByte  gby_sys_Temper[MAX_PACK_TEMPER_NUM];
#pragma DATA_SEG DEFAULT

//#define __DEBUG_DIAG

#define GET_HIGH_BYTE(VAL) ((uByte)(VAL >> 8))
#define GET_LOW_BYTE(VAL) ((uByte)(VAL))
typedef union
{
    uByte by;
    struct
    {
        uByte bit0: 2;
        uByte bit1: 2;
        uByte bit2: 2;
        uByte bit3: 2;
    } bit;
} uCommonStruct;


#define INT_CAN_MAIN_INFO_BUFF 20
#define INT_CAN_EVENT_INFO_BUF 8


#define INTCAN_ERR      (ERR_PROTOCOL_INTCAN_BASE)
//#define INTCAN_DISABLE   (ERR_PROTOCOL_INTCAN_BASE + 1)
//#define FLAG_ENABLE   (ERR_PROTOCOL_INTCAN_BASE + 2)

#define FLAG_ENABLE 1
#define FLAG_DISABLE 0

// define CAN Error time
#define CAN_ERR_TIME TM_MS(5000)

typedef enum
{
    VOLT_MSG,
    RES_MSG,
    TEMPER_MSG,
    BALSOC_MSG,
    EVENT_MSG,
    MAIN_MSG
} eIntCanMsgType;

// define for Main Informater number
typedef enum
{
    MAIN_MSG0,
    MAIN_MSG1,
    MAIN_MSG2,
    MAIN_MSG3,
    MAIN_MSG4,
    MAIN_MSG5,
    MAIN_MSG6,
    MAIN_MSG7,
    MAIN_MSG8,
    MAIN_MSG9,
    MAIN_MSG10,
    MAIN_MSG11,
    MAIN_MSG12,
    MAIN_MSG13,
    MAIN_MSG14,
    MAIN_MSG15,
    MAIN_MSG16,
    MAIN_MSG17,
    MAIN_MSG18,
    MAX_MAIN_MSG
} eMainMsgNumType;

/* define for event informater number */
typedef enum
{
    EVENT_MSG0,
    EVENT_MSG1,
    EVENT_MSG2,
    EVENT_MSG3,
    EVENT_MSG4,
    EVENT_MSG5,
    EVENT_MSG6,
    EVENT_MSG7,
    MAX_EVET_MSG
} eEventMsgNumType;

typedef struct
{
    uByte TimeCnt;
    uByte FirSendFlag;
    uByte FirSendTime;
    uByte SendEn;
    uByte SendCnt;
} sIntCan_CtrlType;

// supper class
typedef struct
{
    sCanIf_MsgType Msg;
    uWord PeriodCnt;
    sIntCan_CtrlType SendCtrl;
} sIntCan_MsgType;

// local ECU information
typedef struct
{

    uByte ProcessCnt;
    uByte Life;
    uByte BmuNum;
    uWord AllCellNum;
    uWord AllTemperNum;
    uWord BmuStartCellNo[MAX_BMU_NUM];
    uWord BmuStartTemperNo[MAX_BMU_NUM];
    uByte MsgLoadPage;
    uByte SendMsgCnt;
    uByte ChipNum;

} sIntCan_LocalEcuInfoType;



// volt temper res soc pf control
typedef struct
{
    uByte StartPf;
    uByte EndPf;
    uByte PfCounter;
} sIntCan_MsgPfType;

// define for receive broadcast msg data
typedef struct
{
    uLWord ProtocolVer;
    uByte RunMode;
    uByte BmuNum;
    uByte Life;

} sIntCan_RecvBroadcastInfoType;

// define for pause or remain send msg
typedef struct
{
    uByte StopFlag;
    uWord Cnt;

} sIntCan_PauseSendMsgCtrlType;

// define for test mode control
typedef struct
{
    uByte VoltCurFrameCnt;
    uByte AHFrameCnt;
    uByte CellVFrameCnt;
    uByte TemperFrameCnt;
    uByte ModeFlag;
    uWord ExitTestModeCnt1;
    uWord ExitTestModeCnt2;
    uWord ExitTestModeCnt3;
    uWord ExitTestModeCnt4;
} sIntCan_ModeCtrlType;

// define for calibrate cur and sumv
#define CS5460_CUR_OFFSET  1
#define CS5460_CUR_GAIN    2
#define CS5460_VOLT_OFFSET 3
#define CS5460_VOLT_GAIN   4




// define for ID
#define SOURCE_ID 0

// MAIN_MSG
#define MAIN_MSG17_ID  (0x4009100 + SOURCE_ID)
#define MAIN_MSG0_ID  (0x4019100 + SOURCE_ID)
#define MAIN_MSG1_ID  (0x4029100 + SOURCE_ID)
#define MAIN_MSG2_ID  (0x4039100 + SOURCE_ID)
#define MAIN_MSG3_ID  (0x4049100 + SOURCE_ID)
#define MAIN_MSG4_ID  (0x4059100 + SOURCE_ID)
#define MAIN_MSG5_ID  (0x4069100 + SOURCE_ID)
#define MAIN_MSG6_ID  (0x4079100 + SOURCE_ID)
#define MAIN_MSG7_ID  (0x4089100 + SOURCE_ID)
#define MAIN_MSG8_ID  (0x4099100 + SOURCE_ID)
#define MAIN_MSG9_ID  (0x40A9100 + SOURCE_ID)
#define MAIN_MSG10_ID (0x40B9100 + SOURCE_ID)
#define MAIN_MSG11_ID (0x40C9100 + SOURCE_ID)
#define MAIN_MSG12_ID (0x40D9100 + SOURCE_ID)
#define MAIN_MSG13_ID (0x40E9100 + SOURCE_ID)
#define MAIN_MSG14_ID (0x40F9100 + SOURCE_ID)
#define MAIN_MSG15_ID (0x4109100 + SOURCE_ID)
#define MAIN_MSG16_ID (0x4119100 + SOURCE_ID)
#define MAIN_MSG18_ID (0x4129100 + SOURCE_ID)
#define MAIN_MSG80_ID (0x4500000 + SOURCE_ID)    // only use in bmu

// Event_MSG id
#define EVENT_MSG0_ID  (0x4800000 + SOURCE_ID)    // use for bcu control bmu relay
#define EVENT_MSG1_ID  (0x4819100 + SOURCE_ID)
#define EVENT_MSG2_ID  (0x4829100 + SOURCE_ID)
#define EVENT_MSG3_ID  (0x4839100 + SOURCE_ID)
#define EVENT_MSG4_ID  (0x4849100 + SOURCE_ID)
#define EVENT_MSG5_ID  0// 0x4858000 reserve to balance
#define EVENT_MSG6_ID  (0x4869100 + SOURCE_ID)
#define EVENT_MSG7_ID  (0x4879100 + SOURCE_ID)

// define Volt frame id
#define VOLT_MSG_ID      (0x8009100 + SOURCE_ID)
#define TEMPER_MSG_ID    (0x10009100 + SOURCE_ID)
#define INTRES_MSG_ID    (0xC009100 + SOURCE_ID)
#define BALSOC_MSG_ID    (0x14009100 + SOURCE_ID)

#define BROADCAST_MSG_ID (0x4FF3F00 + SOURCE_ID)

#define BCU_BALANCE_MSG_ID  (0x4853F00)
#define BMU_BALANCE_MSG_ID  (0x4850000 + SOURCE_ID)
#define BROADCAST_PS     0x3F
#define PC_PS            0x91 //0x80
#define BCU_PS           0x00
#define BCU_SA           0x00
#define BMU_PS           IntCan_LocalEcuInfo.BmuNo
#define BCU_CTRL_BMU_RLY_ID 0x4800000

/* mask for can-id, indicates the message type */
#define TYPE_MASK  0xFF000000

/* mask for can-id, indicates the slave-address */
#define ADDR_MASK  0x000000FF

/* mask for can-id, indicates the message count */
#define CNT_MASK   0x00FF0000

/* mask for can-ps, indicates the message ps */
#define PS_MASK   0x0000FF00

/* define for int_can result */
#define FLAG_OK  1
#define FLAG_FAIL 0

/* define PRIORITY of can4-msg */
#define INT_CAN_MAIN_INFO     0x04
#define INT_CAN_VOLT          0x08
#define INT_CAN_TEMPER        0x10
#define INT_CAN_RESISTANCE    0x0C
#define INT_CAN_SOC           0x14
#define INT_CAN_EXCT_CMD      0x18

/* define for extern commande */
#define DIAG_OFFSET 0x20

#define RD_PRJ_INFO                0x80     // write project message        
#define WR_PRJ_INFO                (RD_PRJ_INFO + DIAG_OFFSET)
#define RD_M_ERR_TRHD_INFO         0x81    // ERR THRD MESSAGE
#define WR_M_ERR_TRHD_INFO         (RD_M_ERR_TRHD_INFO + DIAG_OFFSET)
#define RD_CTRL_INFO               0x82   // control information
#define WR_CTRL_INFO               (RD_CTRL_INFO + DIAG_OFFSET)
#define RD_HIS_INFO                0x83   // history information
#define WR_HIS_INFO                (RD_HIS_INFO + DIAG_OFFSET)
#define RD_BAT_INFO                0x84
#define WR_BAT_INFO                (RD_BAT_INFO + DIAG_OFFSET)
#define RD_M_HD_ERR_INFO           0x85  // BCU detail hardware err code
#define RD_RAM_VAL                 0x86  // read ram value
#define ERASE_RECORD               0x87
#define RD_RECORD                  0x88
#define RD_SOFWARE_VER             0x89
//#define RD_HARDWARE_VER            0x8A // impelent 0x89 in
#define RD_BATSYS_INFO             0x8B

#define RD_QR_CODE_INFO            0x8C
#define WR_QR_CODE_INFO            (RD_QR_CODE_INFO + DIAG_OFFSET)    //生产二维码

#define CCP_AUTH                   0xF0
#define BOOT                       0xFF
#define CLB_VOLT_CUR_CS5460        0x62
#define SET_OFFSET_GAIN            0x60
#define RD_CUR_VOLT                0x41
#define SET_CUR_ENERGE             0x63



// HMU
#define RD_CUR_SUMV_OFFSET_GAIN    0x40
#define WR_CUR_SUMV_OFFSET_GAIN    (RD_CUR_SUMV_OFFSET_GAIN + DIAG_OFFSET)
#define RD_CUR_SUMV                0x41
#define CALIB_CS5460               0x62
#define SET_PARAMETER              0x63
#define RD_H_HD_ERR_INFO           0x44

// BMU
#define RD_S_CFG                   0x00
#define WR_S_CFG                   (RD_S_CFG + DIAG_OFFSET)
#define RD_S_ERR_TRHD_INFO         0x02
#define WR_S_ERR_TRHD_INFO         (RD_S_ERR_TRHD_INFO + DIAG_OFFSET)
#define RD_QUICK_CHNG_INFO         0x03
#define WR_QUICK_CHNG_INFO         (RD_QUICK_CHNG_INFO + DIAG_OFFSET)
#define RD_S_HD_ERR_INFO           0x04
#define WR_UNIQUE_NO               (RD_UNIQUE_NO + DIAG_OFFSET)
#define RD_UNIQUE_NO               0x05
#define WR_BAL_CTRL_PARA           (RD_BAL_CTRL_PARA + DIAG_OFFSET)
#define RD_BAL_CTRL_PARA           0x06

// test
#define TEST_CMD                   0xC0
#define ELOCK_TEST                 0x0F

// define cur sensor for energe thrhd
#define FLA_50A_ENERGE_THRHD  2248
#define FLA_100A_ENERGE_THRHD 1124
#define FLA_200A_ENERGE_THRHD 562
#define FLA_300A_ENERGE_THRHD 375
#define FLA_400A_ENERGE_THRHD 281
#define FLA_500A_ENERGE_THRHD 225
#define FLA_600A_ENERGE_THRHD 188

// define for CanErr
#define INT_CAN_ERR_CNT_VAL (5000 / 10)

/* define for multiple packe transe */
sCanTp_MsgType CanTpMsgTx;
sCanTp_MsgType CanTpMsgRx;

// Main_Msg
sIntCan_MsgType IntCan_MainMsg[INT_CAN_MAIN_INFO_BUFF];

// Event_Msg
sIntCan_MsgType IntCan_EventMsg[INT_CAN_EVENT_INFO_BUF];

// broadcast Msg
sIntCan_MsgType IntCan_BroadCastMsg;

// CellV_Msg
sIntCan_MsgType IntCan_VoltMsg;
// Temper_Msg
sIntCan_MsgType IntCan_TemperMsg;

sIntCan_LocalEcuInfoType IntCan_LocalEcuInfo;   // local board informatio
sIntCan_PauseSendMsgCtrlType  IntCan_PauseSendMsgCtrl;

uIntCan_SlaveRlyStType IntCan_SendSlaveRlySt[MAX_BMU_NUM]; // slave relay control status

// define for Can err
uWord gwd_IntCanErrCnt[MAX_BMU_NUM];
uByte gby_IntCanErrUpdateDataOnce[MAX_BMU_NUM];
sIntCan_ModeCtrlType IntCan_ModeCtrl;

// define for self config receive msg
sCanIf_MsgType IntCan_SelfConfMsg[3];


// Cell_Msg, Res_Msg, Temper_Msg, BalSoc_Msg

sIntCan_RecvBroadcastInfoType IntCan_RecvBroadCastInfo[MAX_BMU_NUM];



/* declare function */
static void IntCan_DiagRslt(sCanIf_MsgType msg_ptr);
static void IntCan_CanTpDiagRslt(sCanTp_MsgType msg);
static void IntCan_LoadSysConf(void);

uWord IntCan_GetBmuMaxOutLineCnt(void);
static void IntCan_CanErrUpdateData(void);

static void IntCan_ModifyPeriod(void);

uWord IntCan_ModifyMainMsgPeriod(eMainMsgNumType index, uWord period); //
void IntCan_ModifyBroacastMsgPeriod(uWord Period);

static void IntCan_MsgSendDisable(void);
static void IntCan_MsgSendEnable(void);


#ifdef __DEBUG_INTCAN
static void IntCan_TestData(void);
void IntCan_InitTest(void);
#endif

uWord    gwd_result;

uByte IntCan_Msg_Vol_Start_Sa = 0;
uByte IntCan_Msg_Temper_Start_Sa = 0;
uByte IntCan_VoltMsg_Send_En = 1;
uByte IntCan_TempMsg_Send_En = 1;

#pragma DATA_SEG BOOT_RAM
 volatile uWord app_request_boot;
#pragma DATA_SEG DEFAULT

void IntCan_Init(void)
{
    uWord i;
    uByte *ptr;
    uLWord llwd;
    sCanIf_AcceptanceCodeType accept;
    sCanIf_AcceptanceMaskType mask;

    // clear CanTpMsgTx
    ptr = (uByte*)(&CanTpMsgTx);
    for(i = 0; i < sizeof(CanTpMsgTx); i++)
    {
        *(ptr++) = 0;
    }

    // clear CanTpMsgRx
    ptr = (uByte*)(&CanTpMsgRx);
    for(i = 0; i < sizeof(CanTpMsgRx); i++)
    {
        *(ptr++) = 0;
    }

    // clear IntCan_MainMsg
    ptr = (uByte*)(IntCan_MainMsg);
    for(i = 0; i < sizeof(IntCan_MainMsg); i++)
    {
        *(ptr++) = 0;
    }

    IntCan_LoadSysConf();
    // initiate main Msg id

    // Main_Msg 0
    IntCan_MainMsg[MAIN_MSG0].Msg.id = MAIN_MSG0_ID;
    IntCan_MainMsg[MAIN_MSG0].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG0].PeriodCnt = TM_MS(10);



    // Main_Msg 1
    IntCan_MainMsg[MAIN_MSG1].Msg.id = MAIN_MSG1_ID;
    IntCan_MainMsg[MAIN_MSG1].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG1].PeriodCnt = TM_MS(10);


    // Main_Msg 2
    IntCan_MainMsg[MAIN_MSG2].Msg.id = MAIN_MSG2_ID;
    IntCan_MainMsg[MAIN_MSG2].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG2].PeriodCnt = TM_MS(50);

    // Main_Msg 3
    IntCan_MainMsg[MAIN_MSG3].Msg.id = MAIN_MSG3_ID;
    IntCan_MainMsg[MAIN_MSG3].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG3].PeriodCnt = TM_MS(1000);

    // Main_Msg 4
    IntCan_MainMsg[MAIN_MSG4].Msg.id = MAIN_MSG4_ID;
    IntCan_MainMsg[MAIN_MSG4].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG4].PeriodCnt = TM_MS(500);

    // Main_Msg 5
    IntCan_MainMsg[MAIN_MSG5].Msg.id = MAIN_MSG5_ID;
    IntCan_MainMsg[MAIN_MSG5].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG5].PeriodCnt = TM_MS(1000);

    // Main_Msg 6
    IntCan_MainMsg[MAIN_MSG6].Msg.id = MAIN_MSG6_ID;
    IntCan_MainMsg[MAIN_MSG6].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG6].PeriodCnt = TM_MS(50);


    // Main_Msg 7
    IntCan_MainMsg[MAIN_MSG7].Msg.id = MAIN_MSG7_ID;
    IntCan_MainMsg[MAIN_MSG7].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG7].PeriodCnt = TM_MS(1000);
    IntCan_MainMsg[MAIN_MSG7].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 8
    IntCan_MainMsg[MAIN_MSG8].Msg.id = MAIN_MSG8_ID;
    IntCan_MainMsg[MAIN_MSG8].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG8].PeriodCnt = TM_MS(1000);
    IntCan_MainMsg[MAIN_MSG8].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 9
    IntCan_MainMsg[MAIN_MSG9].Msg.id = MAIN_MSG9_ID;
    IntCan_MainMsg[MAIN_MSG9].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG9].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG9].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 10
    IntCan_MainMsg[MAIN_MSG10].Msg.id = MAIN_MSG10_ID;
    IntCan_MainMsg[MAIN_MSG10].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG10].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG10].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 11
    IntCan_MainMsg[MAIN_MSG11].Msg.id = MAIN_MSG11_ID;
    IntCan_MainMsg[MAIN_MSG11].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG11].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG11].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 12
    IntCan_MainMsg[MAIN_MSG12].Msg.id = MAIN_MSG12_ID;
    IntCan_MainMsg[MAIN_MSG12].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG12].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG12].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 13  reserve
    IntCan_MainMsg[MAIN_MSG13].Msg.id = MAIN_MSG13_ID;
    IntCan_MainMsg[MAIN_MSG13].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG13].PeriodCnt = TM_MS(100);

    // Main_Msg 14
    IntCan_MainMsg[MAIN_MSG14].Msg.id = MAIN_MSG14_ID;
    IntCan_MainMsg[MAIN_MSG14].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG14].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG14].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 15
    IntCan_MainMsg[MAIN_MSG15].Msg.id = MAIN_MSG15_ID;
    IntCan_MainMsg[MAIN_MSG15].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG15].PeriodCnt = TM_MS(1000);
    IntCan_MainMsg[MAIN_MSG15].SendCtrl.FirSendTime = TM_MS(5);

    // Main_Msg 16
    IntCan_MainMsg[MAIN_MSG16].Msg.id = MAIN_MSG16_ID;
    IntCan_MainMsg[MAIN_MSG16].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG16].PeriodCnt = TM_MS(1000);
    IntCan_MainMsg[MAIN_MSG16].SendCtrl.FirSendTime = TM_MS(5);
    
    // Main_Msg 17
    IntCan_MainMsg[MAIN_MSG17].Msg.id = MAIN_MSG17_ID;
    IntCan_MainMsg[MAIN_MSG17].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG17].PeriodCnt = TM_MS(500);
    IntCan_MainMsg[MAIN_MSG17].SendCtrl.FirSendTime = TM_MS(5);
    
    IntCan_MainMsg[MAIN_MSG18].Msg.id = MAIN_MSG18_ID;
    IntCan_MainMsg[MAIN_MSG18].Msg.length = 8;
    IntCan_MainMsg[MAIN_MSG18].PeriodCnt = TM_MS(100);
    IntCan_MainMsg[MAIN_MSG18].SendCtrl.FirSendTime = TM_MS(5);

    //Event_Msg 0
    IntCan_EventMsg[EVENT_MSG0].Msg.id = EVENT_MSG0_ID;
    IntCan_EventMsg[EVENT_MSG0].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG0].PeriodCnt = TM_MS(1000);

    //Event_Msg 1
    IntCan_EventMsg[EVENT_MSG1].Msg.id = EVENT_MSG1_ID;
    IntCan_EventMsg[EVENT_MSG1].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG1].PeriodCnt = TM_MS(500);

    //Event_Msg 2
    IntCan_EventMsg[EVENT_MSG2].Msg.id = EVENT_MSG2_ID;
    IntCan_EventMsg[EVENT_MSG2].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG2].PeriodCnt = TM_MS(1000);

    //Event_Msg 3
    IntCan_EventMsg[EVENT_MSG3].Msg.id = EVENT_MSG3_ID;
    IntCan_EventMsg[EVENT_MSG3].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG3].PeriodCnt = TM_MS(1000);

    //Event_Msg 4
    IntCan_EventMsg[EVENT_MSG4].Msg.id = EVENT_MSG4_ID;
    IntCan_EventMsg[EVENT_MSG4].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG4].PeriodCnt = TM_MS(1000);

    //Event_Msg 5  quick change batsystem
    IntCan_EventMsg[EVENT_MSG5].Msg.id = EVENT_MSG5_ID;
    IntCan_EventMsg[EVENT_MSG5].Msg.length = 8;
    IntCan_EventMsg[EVENT_MSG5].PeriodCnt = TM_MS(1000);

    // Event Msg6 test frame
    IntCan_EventMsg[EVENT_MSG6].Msg.id = EVENT_MSG6_ID;
    IntCan_EventMsg[EVENT_MSG6].Msg.length = 8;
    if(IntCan_StaticSet.TestFrame1Cycle >= 10)
    {
        IntCan_EventMsg[EVENT_MSG6].PeriodCnt = TM_MS(IntCan_StaticSet.TestFrame1Cycle);
    }
    else
    {
        IntCan_EventMsg[EVENT_MSG6].PeriodCnt = TM_MS(10);
    }

    // Event Msg7 test frame
    IntCan_EventMsg[EVENT_MSG7].Msg.id = EVENT_MSG7_ID;
    IntCan_EventMsg[EVENT_MSG7].Msg.length = 8;
    if(IntCan_StaticSet.TestFrame2Cycle >= 10)
    {
        IntCan_EventMsg[EVENT_MSG7].PeriodCnt = TM_MS(IntCan_StaticSet.TestFrame2Cycle);
    }
    else
    {
        IntCan_EventMsg[EVENT_MSG7].PeriodCnt = TM_MS(10);
    } 
    
      
    IntCan_VoltMsg.Msg.id = VOLT_MSG_ID;
    IntCan_VoltMsg.Msg.length = 8;
    IntCan_VoltMsg.PeriodCnt = TM_MS(20); 

    IntCan_TemperMsg.Msg.id = TEMPER_MSG_ID;
    IntCan_TemperMsg.Msg.length = 8;
    IntCan_TemperMsg.PeriodCnt = TM_MS(100);
    
#ifdef __DEBUG_DIAG

#else
    // enable all main msg send enable
    for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
    {
        IntCan_MainMsg[i].SendCtrl.SendEn = FLAG_ENABLE;
    }
    
    IntCan_VoltMsg.SendCtrl.SendEn = FLAG_ENABLE;
    IntCan_TemperMsg.SendCtrl.SendEn = FLAG_ENABLE;
#endif

    // broadcast
    IntCan_BroadCastMsg.Msg.id = BROADCAST_MSG_ID;
    IntCan_BroadCastMsg.Msg.length = 8;
    IntCan_BroadCastMsg.PeriodCnt = TM_MS(50);

#ifdef __DEBUG_DIAG

#else
    IntCan_BroadCastMsg.SendCtrl.SendEn = FLAG_ENABLE;
#endif


// modify for BCU_C period
    if(IntCan_LocalEcuInfo.BmuNum <= 4)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,10);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,10);

        IntCan_ModifyBroacastMsgPeriod(50);

    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 6)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,20);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,20);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 8)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,30);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,30);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 11)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,40);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,40);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 12)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,50);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,50);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 19)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,100);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,100);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG6,100);

        IntCan_ModifyBroacastMsgPeriod(100);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 30)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,200);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,200);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG6,200);

        IntCan_ModifyBroacastMsgPeriod(200);
    }

    CanIf_Init(INT_CAN_CHANNEL , IntCan_StaticSet.CanBaudrate);

    accept.AccCode1 = 0x00000000;
    mask.AccMask1 =   0xFFFFFFFF;

    accept.AccCode2 = 0x00000000;
    mask.AccMask2   = 0xFFFFFFFF;
    CanIf_SetAcceptanceMaskCode(INT_CAN_CHANNEL, &accept, &mask);

    // for first load immediately
    IntCan_LocalEcuInfo.ProcessCnt = TM_MS(10);
}

static void IntCan_LoadSysConf(void)
{
    uByte data[30];
    uByte len = 0;
    volatile uByte i;
    //uWord *ptr;
    //uWord *ptr2;
    volatile Byte btmp;
    // load BmuNum
    //(void)EE_LoadVar(SYSCONF_BMU_NUMBER);
    (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, data, &len);
    IntCan_LocalEcuInfo.BmuNum = data[0];

    // prevent BmuNum equal 0
    if(!IntCan_LocalEcuInfo.BmuNum)
    {
        IntCan_LocalEcuInfo.BmuNum = 1;
    }

    // prevent BmuNum Over MaxNum
    if(IntCan_LocalEcuInfo.BmuNum > MAX_BMU_NUM)
    {
        IntCan_LocalEcuInfo.BmuNum = MAX_BMU_NUM;
    }
    Bat_UpdateBmuNum(IntCan_LocalEcuInfo.BmuNum);
    
    (void)SysConf_GetParaValueFromRam(SYSCONF_CHIP_NUMBER, data, &len);
    IntCan_LocalEcuInfo.ChipNum = data[0];
    IntCan_LocalEcuInfo.ChipNum = IntCan_LocalEcuInfo.BmuNum;
    Bat_UpdateChipNum(IntCan_LocalEcuInfo.ChipNum);

    IntCan_LocalEcuInfo.AllCellNum = 0;
    IntCan_LocalEcuInfo.AllTemperNum = 0;
    
    
    (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, data, &len);
   // ptr = (uWord*)(data & 0x0f);
   // ptr2 = (uWord*)((data&0xf0)>>4);
    for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
    {
        btmp = data[i]&0x0f;
        (void)Bat_UpdatePackSerialNum((i + 1), btmp);
        IntCan_LocalEcuInfo.AllCellNum += btmp;
        
        btmp = (data[i] & 0xf0)>>4;
        (void)Bat_UpdatePackTemperNum((i + 1), btmp);
        IntCan_LocalEcuInfo.AllTemperNum += btmp;
    }
    
   /* (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, data, &len);
    ptr = (uWord*)(data);
    for(i = 0; i < IntCan_LocalEcuInfo.ChipNum; i++)
    {
        (void)Bat_UpdatePackTemperNum((i + 1), (uByte)(*ptr));
        IntCan_LocalEcuInfo.AllTemperNum += *(ptr++);
    } */

    if (IntCan_LocalEcuInfo.BmuNum <= 15)
    {
        // load all cell and temper number
        //(void)EE_LoadVar(SYSCONF_BMU_CELLNUM_G1);
    /*    (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G1, data, &len);
        ptr = (uWord*)(data);
        for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {
            (void)Bat_UpdatePackSerialNum((i + 1), *ptr);
            IntCan_LocalEcuInfo.AllCellNum += *(ptr++);
        }
      */
        // load bmu temper number
        //(void)EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G1);
       /* (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G1, data, &len);
        ptr = (uWord*)(data);
        for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {
            (void)Bat_UpdatePackTemperNum((i + 1), (uByte)(*ptr));
            IntCan_LocalEcuInfo.AllTemperNum += *(ptr++);
        } */

        // get 1-15 bmu start cell no
        //(void)EE_LoadVar(SYSCONF_BMU_START_CELLNO_G1);
         
         //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G1, data, &len);
          //ptr = (uWord*)data;
          for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
          {
              IntCan_LocalEcuInfo.BmuStartCellNo[i] =1;  //*(ptr++);
          }

        // get 0-15 bmu start temper no
        //(void)EE_LoadVar(SYSCONF_BMU_START_TEMPERNO_G1);
       
        //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_TEMPERNO_G1, data, &len);
        //ptr = (uWord*)data;
        for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {

            IntCan_LocalEcuInfo.BmuStartTemperNo[i] = 1; // *(ptr++);
        } 
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= MAX_BMU_NUM)
    {
        // load all cell and temper number
        //(void)EE_LoadVar(SYSCONF_BMU_CELLNUM_G1);
      /*  (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G1, data, &len);
        ptr = (uWord*)(data);
        for(i = 0; i < 15; i++)
        {
            (void)Bat_UpdatePackSerialNum((i + 1), *ptr);
            IntCan_LocalEcuInfo.AllCellNum += *(ptr++);
        } */

        //(void)EE_LoadVar(SYSCONF_BMU_CELLNUM_G2);
     /*   (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G2, data, &len);
        ptr = (uWord*)(data);
        for(i = 15; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {
            (void)Bat_UpdatePackSerialNum((i + 1), *ptr);
            IntCan_LocalEcuInfo.AllCellNum += *(ptr++);
        }
      */
        // load bmu temper number
        //(void)EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G1);
      /*  (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G1, data, &len);
        ptr = (uWord*)(data);
        for(i = 0; i < 15; i++)
        {
            (void)Bat_UpdatePackTemperNum((i + 1), (uByte)(*ptr));
            IntCan_LocalEcuInfo.AllTemperNum += *(ptr++);
        } */

        // load bmu temper number
        //(void)EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G2);
      /*  (void)SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G2, data, &len);
        ptr = (uWord*)(data);
        for(i = 15; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {
            (void)Bat_UpdatePackTemperNum((i + 1), (uByte)*ptr);
            IntCan_LocalEcuInfo.AllTemperNum += *(ptr++);
        } */

        // get 1-15 bmu start cell no
        //(void)EE_LoadVar(SYSCONF_BMU_START_CELLNO_G1);
        //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G1, data, &len);
        //ptr = (uWord*)data;
        for(i = 0; i < 15; i++)
        {
            IntCan_LocalEcuInfo.BmuStartCellNo[i] = 1; //*(ptr++);
        }

        // get 15-20 bmu start cell no
        //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G2, data, &len);
        //ptr = (uWord*)data;
        for(i = 15; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {

            IntCan_LocalEcuInfo.BmuStartCellNo[i] =1; // *(ptr++);
        }

        // get 0-15 bmu start temper no
        //(void)EE_LoadVar(SYSCONF_BMU_START_TEMPERNO_G1);
        //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_TEMPERNO_G1, data, &len);
        //ptr = (uWord*)data;
        for(i = 0; i < 15; i++)
        {

            IntCan_LocalEcuInfo.BmuStartTemperNo[i] = 1; // *(ptr++);
        }

        // get 15-30 bmu start temper no
        //(void)SysConf_GetParaValueFromRam(SYSCONF_BMU_START_TEMPERNO_G2, data, &len);
        //ptr = (uWord*)data;
        for(i = 15; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {

            IntCan_LocalEcuInfo.BmuStartTemperNo[i] = 1; //*(ptr++);
        }
    }

    Bat_UpdateTemperNum(IntCan_LocalEcuInfo.AllTemperNum);
    Bat_UpdateBatNum(IntCan_LocalEcuInfo.AllCellNum);

}

static void IntCan_UpdateCanPara(void)
{
    uLWord llwd;
    uByte i;
// use when ExctCmd
    IntCan_LoadSysConf();
    // initiate main Msg id

    // Main_Msg 0
    IntCan_MainMsg[MAIN_MSG0].Msg.id = MAIN_MSG0_ID;

    // Main_Msg 1
    IntCan_MainMsg[MAIN_MSG1].Msg.id = MAIN_MSG1_ID;

    // Main_Msg 2
    IntCan_MainMsg[MAIN_MSG2].Msg.id = MAIN_MSG2_ID;

    // Main_Msg 3
    IntCan_MainMsg[MAIN_MSG3].Msg.id = MAIN_MSG3_ID;

    // Main_Msg 4
    IntCan_MainMsg[MAIN_MSG4].Msg.id = MAIN_MSG4_ID;

    // Main_Msg 5
    IntCan_MainMsg[MAIN_MSG5].Msg.id = MAIN_MSG5_ID;

    // Main_Msg 6
    IntCan_MainMsg[MAIN_MSG6].Msg.id = MAIN_MSG6_ID;

    // Main_Msg 7
    IntCan_MainMsg[MAIN_MSG7].Msg.id = MAIN_MSG7_ID;

    // Main_Msg 8
    IntCan_MainMsg[MAIN_MSG8].Msg.id = MAIN_MSG8_ID;

    // Main_Msg 9
    IntCan_MainMsg[MAIN_MSG9].Msg.id = MAIN_MSG9_ID;

    // Main_Msg 10
    IntCan_MainMsg[MAIN_MSG10].Msg.id = MAIN_MSG10_ID;

    // Main_Msg 11
    IntCan_MainMsg[MAIN_MSG11].Msg.id = MAIN_MSG11_ID;

    // Main_Msg 12
    IntCan_MainMsg[MAIN_MSG12].Msg.id = MAIN_MSG12_ID;

    // Main_Msg 13  reserve
    IntCan_MainMsg[MAIN_MSG13].Msg.id = MAIN_MSG13_ID;

    // Main_Msg 14
    IntCan_MainMsg[MAIN_MSG14].Msg.id = MAIN_MSG14_ID;

    // Main_Msg 15
    IntCan_MainMsg[MAIN_MSG15].Msg.id = MAIN_MSG15_ID;

    // Main_Msg 16
    IntCan_MainMsg[MAIN_MSG16].Msg.id = MAIN_MSG16_ID;

    //Event_Msg 0
    IntCan_EventMsg[EVENT_MSG0].Msg.id = EVENT_MSG0_ID;

    //Event_Msg 1
    IntCan_EventMsg[EVENT_MSG1].Msg.id = EVENT_MSG1_ID;

    //Event_Msg 2
    IntCan_EventMsg[EVENT_MSG2].Msg.id = EVENT_MSG2_ID;

    //Event_Msg 3
    IntCan_EventMsg[EVENT_MSG3].Msg.id = EVENT_MSG3_ID;

    //Event_Msg 4
    IntCan_EventMsg[EVENT_MSG4].Msg.id = EVENT_MSG4_ID;

    //Event_Msg 5  quick change batsystem
    IntCan_EventMsg[EVENT_MSG5].Msg.id = EVENT_MSG5_ID;

    //Event_Msg 6  quick change batsystem
    IntCan_EventMsg[EVENT_MSG6].Msg.id = EVENT_MSG6_ID;

    //Event_Msg 7  quick change batsystem
    IntCan_EventMsg[EVENT_MSG7].Msg.id = EVENT_MSG7_ID;   

    // broadcast
    IntCan_BroadCastMsg.Msg.id = BROADCAST_MSG_ID;


// modify for BCU_C period
    if(IntCan_LocalEcuInfo.BmuNum <= 4)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,10);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,10);

        IntCan_ModifyBroacastMsgPeriod(50);

    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 6)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,20);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,20);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 8)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,30);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,30);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 11)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,40);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,40);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 12)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,50);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,50);

        IntCan_ModifyBroacastMsgPeriod(50);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 19)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,100);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,100);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG6,100);

        IntCan_ModifyBroacastMsgPeriod(100);
    }
    else if(IntCan_LocalEcuInfo.BmuNum <= 30)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG0,200);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG1,200);
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG6,200);

        IntCan_ModifyBroacastMsgPeriod(200);
    }

}

static void IntCan_MsgSendEnable(void)
{
    uByte i;

    IntCan_PauseSendMsgCtrl.StopFlag = FLAG_FAIL;

    for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
    {
        IntCan_MainMsg[i].SendCtrl.SendEn = FLAG_ENABLE;
    }

}

static void IntCan_MsgSendDisable(void)
{
    uByte i;

    IntCan_PauseSendMsgCtrl.StopFlag = FLAG_OK;

    for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
    {
        IntCan_MainMsg[i].SendCtrl.SendEn = FLAG_DISABLE;
    }

    for(i = 0; i < INT_CAN_EVENT_INFO_BUF; i++)
    {
        IntCan_EventMsg[i].SendCtrl.SendEn = FLAG_DISABLE;
    }

}

void IntCan_EnOrDisSendMsg(uByte en)
{
    if(FLAG_ENABLE == en)
    {
        IntCan_MsgSendEnable();
        IntCan_BroadCastMsg.SendCtrl.SendEn = FLAG_ENABLE;
    }
    else if(FLAG_DISABLE == en)
    {
        IntCan_MsgSendDisable();
        IntCan_BroadCastMsg.SendCtrl.SendEn = FLAG_DISABLE;
    }

}

/*
 * Deal CAN Rx Data
 */
//uWord test_flag = 0;
//uWord test_wcnt = 0;
void IntCan_ProcMsg(sCanIf_MsgType msg)
{
    /*
     *  deal internal CAN message here
     */
    /* volatile*/ uByte msg_type;
    /* volatile*/
    uByte device_no;
    /* volatile*/
    uByte msg_cnt,ps, sa;
    /* volatile*/
    uByte i;
    /* volatile*/
    uByte btmp;
    /* volatile*/
    uWord rslt[4];
    /* volatile*/
    uWord wtmp,wcnt;
    /* volatile*/
    uWord local_num;
    /* volatile*/
    uLWord llwtmp;
    sErr_ByteNType err_type;
    /* volatile*/
    uByte data[30],s;
    /* volatile*/
    uByte *ptr_by, *ptr_by_a;
    sVersion_CodeType  version_code;
    sActor_CtrlRelayCmdType ctrl_relay_cmd;
    ModuleID_Type module_id[VERSION_MODULE_NUMBER];
    uWord ModuleNum = 0;
    uWord local_numbak;

    if(FLAG_ENABLE == IntCan_StaticSet.SelfRecvMsgEn)
    {
        if(msg.id == IntCan_StaticSet.SelfRecvMsg0Id)
        {
            IntCan_SelfConfMsg[0].id = msg.id;
            IntCan_SelfConfMsg[0].length = msg.length;
            for(i = 0; i < msg.length; i++)
            {
                IntCan_SelfConfMsg[0].data[i] = msg.data[i];
            }
            return;
        }
        else if(msg.id == IntCan_StaticSet.SelfRecvMsg1Id)
        {
            IntCan_SelfConfMsg[1].id = msg.id;
            IntCan_SelfConfMsg[1].length = msg.length;
            for(i = 0; i < msg.length; i++)
            {
                IntCan_SelfConfMsg[1].data[i] = msg.data[i];
            }
            return;
        }
        else if(msg.id == IntCan_StaticSet.SelfRecvMsg2Id)
        {
            IntCan_SelfConfMsg[2].id = msg.id;
            IntCan_SelfConfMsg[2].length = msg.length;
            for(i = 0; i < msg.length; i++)
            {
                IntCan_SelfConfMsg[2].data[i] = msg.data[i];
            }
            return;
        }
    }
    msg_type =  (uByte)((msg.id & TYPE_MASK) >> 24);
    device_no = (uByte)(msg.id & ADDR_MASK) ;
    msg_cnt =   (uByte)((msg.id & CNT_MASK) >> 16);
    ps = (uByte)((msg.id & PS_MASK) >> 8);
    sa = (uByte)(msg.id);

// filter ps

    if(0x00010091 == msg.id) //收到BCU的boot信息
    {
        if( (0xAA == msg.data[1])
         && (0xAA == msg.data[2])
         && (0xAA == msg.data[3])
         && (0xAA == msg.data[4]) )    //进入 boot
        {
          
          __DI();
    	    app_request_boot = 0x55AA;
    	    asm
          {    
             LDX 65534;		
             LDD 65534;    
             IBEQ D,OUT; 
             LDAA #$FF;   
             STAA $121;    
             JMP 0,X;        
             OUT:;            
          }
        }
    }
   	else if( (0 == msg_type)    //收到BMU的boot命令 此时BCU进入等待模式，CAN停止数据发送
    &&(0x91 == device_no)
    &&(0x00 != ps))
   	{
   	    for(;;)
         {
            __DI();  
            wdi_PutVal(!wdi_GetVal());
         }
   	}

    if((BROADCAST_PS != ps) && (BCU_PS != ps) && (PC_PS != ps) && (BOOT != msg_cnt))
    {
        return;
    }

    // filter bmu command replay ,eg. 0x18898001
    if((PC_PS == ps) && (INT_CAN_EXCT_CMD == msg_type))
    {
        return;
    }

    // process data or command
    switch(msg_type)
    {
      case INT_CAN_VOLT:
      {
          // confirm bmu number
          if((device_no >= 31) && (device_no <= (IntCan_LocalEcuInfo.BmuNum + 30)))
          {
              IntCan_ModeCtrl.CellVFrameCnt++;
              IntCan_ModeCtrl.ExitTestModeCnt3 = 0;
              if((IntCan_ModeCtrl.CellVFrameCnt >= 10) && (IntCan_ModeCtrl.TemperFrameCnt >= 10))
              {
                  IntCan_ModeCtrl.CellVFrameCnt = 10;
                  IntCan_ModeCtrl.ModeFlag = FLAG_OK;
              }
              for(i = 0; i< 4; i++)
              {
                  // filter 0xff data
                  if((0xFF == msg.data[i]) && (0xFF == msg.data[i * 2 + 1]))
                  {
                      continue;
                  }
                  else
                  {
                      wtmp = (msg.data[2 * i] & 0x7f);
                      wtmp <<= 8;
                      wtmp += msg.data[2 * i + 1];

                      local_numbak = msg_cnt * 4 + i + 1;
                      if(local_numbak >= 1)
                      {
                          local_num = local_numbak;// - IntCan_LocalEcuInfo.BmuStartCellNo[device_no - 1];
                          // call BatterySt interface to update the cell volt
                          if(local_num <= Bat_GetPackSerialNum(device_no - 30))
                          {
                              
                              (void)Bat_UpdateCellV(device_no - 30, local_num, wtmp);
                          }
                      }
                  }
              }
          }

          break;
      } // end case VOLT

      case INT_CAN_TEMPER:
      {
          // confirm bmu number
          if((device_no >= 31) && (device_no <= (IntCan_LocalEcuInfo.BmuNum + 30)))
          {
              IntCan_ModeCtrl.TemperFrameCnt++;
              IntCan_ModeCtrl.ExitTestModeCnt4 = 0;
              if((IntCan_ModeCtrl.CellVFrameCnt >= 10) && (IntCan_ModeCtrl.TemperFrameCnt >= 10))
              {
                  IntCan_ModeCtrl.TemperFrameCnt = 10;
                  IntCan_ModeCtrl.ModeFlag = FLAG_OK;
              }
              for(i = 0; i < 8; i++)
              {
                  if(0xFF == msg.data[i])
                  {
                      continue;
                  }
                  else
                  {
                      local_numbak = msg_cnt * 8 + i + 1;
                      if(local_numbak >= 1)
                      {
                          local_num = local_numbak;// - IntCan_LocalEcuInfo.BmuStartTemperNo[device_no - 1];
                          if(local_num <= Bat_GetPackTemperNum(device_no - 30))
                          {
                              // call BatterySt interface to update the temper
                              (void)Bat_UpdateTemper(device_no - 30, local_num, msg.data[i]) ;
                          }
                      }
                  }
              }
          }
          break;
      } // end case TEMPTER

      case INT_CAN_RESISTANCE:
      {
          // confirm bmu number
          if((device_no >= 1) &&(device_no <= MAX_BMU_NUM))
          {
              for(i = 0; i < 8; i++)
              {
                  if(0xFF == msg.data[i])
                  {
                      continue;
                  }
                  else
                  {
                      wtmp = msg.data[i];
                      local_numbak = msg_cnt * 8 + i + 1;
                      if(local_numbak >= IntCan_LocalEcuInfo.BmuStartCellNo[device_no - 1])
                      {
                          local_num = local_numbak - IntCan_LocalEcuInfo.BmuStartCellNo[device_no - 1];
                      }
                  }
              }
          }
          break;
      } // end case RES

      case INT_CAN_SOC:
      {
          // confirm bmu number
          if((device_no >= 1) &&(device_no <= MAX_BMU_NUM))
          {
              for(i = 0; i< 4; i++)
              {
                  // filter 0xff data
                  if((0x03 == (0x03 &msg.data[i])) && (0xFF == msg.data[i * 2 + 1]))
                  {
                      continue;
                  }
                  else
                  {
                      wtmp = msg.data[2 * i];
                      wtmp <<= 8;
                      wtmp += msg.data[2 * i + 1];

                      local_numbak = msg_cnt * 4 + i + 1;
                      if(local_numbak >= IntCan_LocalEcuInfo.BmuStartCellNo[device_no - 1])
                      {
                          local_num = local_numbak - IntCan_LocalEcuInfo.BmuStartCellNo[device_no - 1];

                          // balance status
                          btmp = (uByte)(wtmp >> 10);
                          if(local_num < MAX_PACK_CELL_NUM)
                          {
                              (void)Bat_UpdateCellBalanceSt(device_no, local_num+1, btmp); 
                          }

                          // soc
                          wtmp &= 0x3FF;
                      }
                  }
              }
          }
          break;
      } // end case BALANCE

      case INT_CAN_MAIN_INFO:
      {
          if((device_no > 0) && (device_no <= IntCan_LocalEcuInfo.BmuNum))
          {
              switch(msg_cnt)
              {
                  // sumv  cur
                case 1:
                {
                    // call BatterySt interface to update in sumv
                    wtmp = msg.data[0];
                    wtmp <<= 8;
                    wtmp += msg.data[1];
                    (void)Bat_UpdatePackSumV(device_no, ACC_SUMV, wtmp) ;

                    // call BatterySt interface to update current
                    wtmp = msg.data[2];
                    wtmp <<= 8;
                    wtmp += msg.data[3];

                    // call BatterySt interface to update out sumv
                    wtmp = msg.data[4];
                    wtmp <<= 8;
                    wtmp += msg.data[5];

                    // call BatterySt interface to update avg current
                    wtmp = msg.data[6];
                    wtmp <<= 8;
                    wtmp += msg.data[7];

                    break;
                }
                case 2:
                {
                    Prj_UpdateRecvFirFrame( device_no);

                    // call BatterySt interface to update maxv
                    wtmp = msg.data[0];
                    wtmp <<= 8;
                    wtmp += msg.data[1];
                    (void)Bat_UpdatePackMaxV(device_no, wtmp);

                    // call BatterySt interface to update minv
                    wtmp = msg.data[2];
                    wtmp <<= 8;
                    wtmp += msg.data[3];
                    (void)Bat_UpdatePackMinV(device_no, wtmp);

                    // call BatterySt interface to update maxv&minv no
                    wtmp = msg.data[4];
                    wtmp <<= 4;
                    wtmp += ((msg.data[5] >> 4) & 0x0F); ;
                    (void)Bat_UpdatePackMaxVNo(device_no, wtmp);

                    wtmp = msg.data[5] & 0x0F;
                    wtmp <<= 8;
                    wtmp += msg.data[6];
                    (void)Bat_UpdatePackMinVNo(device_no, wtmp);

                    // clear slave can delay err cnt
                    if(device_no > 0)
                    {
                        gwd_IntCanErrCnt[device_no - 1] = 0;
                    }
                    break;
                }
                case 3:
                {
                    (void)Bat_UpdatePackMaxT(device_no, msg.data[0]);

                    (void)Bat_UpdatePackMinT(device_no, msg.data[1]);

                    // max_t_no
                    wtmp = msg.data[2];
                    wtmp <<= 4;
                    wtmp += ((msg.data[3] >> 4) & 0x0F);
                    (void)Bat_UpdatePackMaxTNo(device_no, wtmp);

                    wtmp = msg.data[3] & 0x0F;
                    wtmp <<= 8;
                    wtmp += msg.data[4];
                    (void)Bat_UpdatePackMinTNo(device_no, wtmp);

                    // soh
                    wtmp = msg.data[5];
                    wtmp <<= 4;
                    wtmp += ((msg.data[6] >> 4) & 0x0F);

                    // soc
                    wtmp = msg.data[6] & 0x0F;
                    wtmp <<= 8;
                    wtmp += msg.data[7];

                    break;
                }
                case 4:
                {
                    // max_r
                    (void)Bat_UpdatePackMaxR(device_no, msg.data[0]);

                    // min_r
                    (void)Bat_UpdatePackMaxR(device_no, msg.data[1]);

                    // max_r_no
                    wtmp = msg.data[2];
                    wtmp <<= 4;
                    wtmp += ((msg.data[3] >> 4) & 0x0F);
                    (void)Bat_UpdatePackMaxRNo(device_no, wtmp);

                    // min_r_no
                    wtmp = msg.data[3] & 0x0F;
                    wtmp <<= 8;
                    wtmp += msg.data[4];
                    (void)Bat_UpdatePackMinRNo(device_no, wtmp);

                    // total r
                    wtmp = msg.data[5];
                    wtmp += msg.data[6];

                    break;
                }
                case 13:
                {
                    for(i = 0; i < 8; i++)
                    {
                        err_type.By[i] = msg.data[i];
                    }
                    (void)Err_UpdateHwErrLevelFromCan(device_no, &err_type);
                    break;
                }
                case 15:
                {
                    // reserve
                    btmp = msg.data[3];
                    btmp >>= 6;
                    btmp &= 0x03;
                    Bat_UpdatePackBalSt(device_no, btmp);

                    break;
                }
                case 80:  // slave relay status
                {
                    if((device_no >= 1) && (device_no <= MAX_BMU_NUM))
                    {
                        BatSys_UpdateAllPackFanSt(device_no, (msg.data[0] >> 6) & 0x03);// Fan relay st
                        BatSys_UpdateAllPackHeaterSt(device_no, (msg.data[0] >> 4) & 0x03);// heat relay st

                        BmsSys_UpdateBmuDi1St(device_no,  msg.data[2] & 0x03);

                        BmsSys_UpdateBmuDo1St(device_no,  msg.data[3] & 0x03);

                        wtmp = msg.data[4];
                        wtmp *= 2;
                        BmsSys_UpdateBmuAi1St(device_no,  wtmp);

                    }
                    break;
                }
                case 0x85: // balance
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        if((device_no >= 1) && (device_no <= MAX_BMU_NUM))
                        {
                            BalM_UpdateDataFromCan(device_no, CanTpMsgRx.Data[0], &CanTpMsgRx.Data[2],2);
                        }
                    }
                    break;
                }
                case 255: // broadcast msg
                {

                    break;
                }


              default: break;
                 
              }
          }
          else if(0x3e == device_no)  // hmu info ,for test mode
          {
              switch(msg_cnt)
              {
              case 0:
              {
                  IntCan_ModeCtrl.VoltCurFrameCnt++;
                  IntCan_ModeCtrl.ExitTestModeCnt1 = 0;
                  if((IntCan_ModeCtrl.VoltCurFrameCnt++ >= 10) && (IntCan_ModeCtrl.AHFrameCnt >= 10))
                  {
                      IntCan_ModeCtrl.VoltCurFrameCnt = 10;
                      IntCan_ModeCtrl.ModeFlag = FLAG_OK;

                      wtmp = msg.data[0];
                      wtmp <<= 8;
                      wtmp += msg.data[1];
                      Bat_UpdateSumV(INT_SUMV, wtmp);

                      wtmp = msg.data[2];
                      wtmp <<= 8;
                      wtmp += msg.data[3];
                      Bat_UpdateCurrent(INSCUR, wtmp);

                      wtmp = msg.data[4];
                      wtmp <<= 8;
                      wtmp += msg.data[5];
                      Bat_UpdateSumV(OUT_SUMV, wtmp);

                      wtmp = msg.data[6];
                      wtmp <<= 8;
                      wtmp += msg.data[7];
                      Bat_UpdateCurrent(AVGCUR, wtmp);
                  }
                  break;
              }
              case 5:
              {
                  IntCan_ModeCtrl.AHFrameCnt++;
                  IntCan_ModeCtrl.ExitTestModeCnt2 = 0;
                  if((IntCan_ModeCtrl.AHFrameCnt >= 10) && (IntCan_ModeCtrl.VoltCurFrameCnt >= 10))
                  {
                      IntCan_ModeCtrl.AHFrameCnt = 10;
                      IntCan_ModeCtrl.ModeFlag = FLAG_OK;
                      llwtmp = msg.data[0];
                      llwtmp <<= 8;
                      llwtmp += msg.data[1];
                      llwtmp <<= 8;
                      llwtmp += msg.data[2];
                      llwtmp <<= 8;
                      llwtmp += msg.data[3];
                      Bat_UpdateAccumulateChgAh(llwtmp);

                      llwtmp = msg.data[4];
                      llwtmp <<= 8;
                      llwtmp += msg.data[5];
                      llwtmp <<= 8;
                      llwtmp += msg.data[6];
                      llwtmp <<= 8;
                      llwtmp += msg.data[7];

                      Bat_UpdateAccumulateDchgAh(llwtmp);
                  }
                  break;
              }
              default:
                  break;
              }
          }


          break;
      } // end case MAIN_INFO

        // extern cmd
      case INT_CAN_EXCT_CMD:
      {
          // deal CMD typed
          switch(msg_cnt)
          {
              if(sa == 0x91)   // 参数标定需要判断源地址 
              {
                case WR_PRJ_INFO:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:
                        {
                            (void) WR_BCUSysconfProductNo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 2:
                        {
                            (void) WR_BCUCellvNum();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 3:
                        {
                            (void) WR_BCUCellNumPerChip();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 4:
                        {
                            (void) WR_BCUCellvStartNo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 5:
                        {
                            (void) WR_BCUTemperNum();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 6:
                        {
                            (void) WR_BCUTemperStartNo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 7:
                        {
                            (void) WR_BCUSoc();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 8:
                        {
                            (void) WR_BCURatesCapActualCap();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 9:
                        {
                            (void) WR_BCUCfgVer();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 10:
                        {
                            (void) WR_BCUCfgUpdate();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 11:
                        {
                            (void) WR_BCUVehiclePlate();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 12:
                        {
                            (void) WR_BCUSoh();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 13:
                        {
                            (void) WR_BCUFusePos();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        case 14:   // write  BMS system clock
                        {
                            // indicate Xgate stop read real time
                            Xgate_CpuSetRealTimeRdEn(1);
                            // wait xgate stop
                            ECU_DELAYMS(1);

                            (void) WR_BCUSystemClock();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);

                            // restart xgate read time
                            Xgate_CpuSetRealTimeRdEn(0);
                            break;

                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end WR_PRJ_INFO
                case RD_PRJ_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:  // call SysConf interface to load information
                        {
                            (void) RD_BCUSysconfProductNo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 2:
                        {
                            (void) RD_BCUCellvNum();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 3:   //BMU0 Cell number per Chip
                        {
                            (void) RD_BCUCellNumPerChip();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }
                        case 4:  // BMU0-BMU30 start cell no
                        {
                            (void) RD_BCUCellvStartNo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 5:   //BMU0-BMU30 temper number
                        {
                            (void) RD_BCUTemperNum();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 6:   //BMU0-BMU30 temper start n0
                        {
                            (void) RD_BCUTemperStartNo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 7:   //SOC
                        {
                            (void) RD_BCUSoc();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 8:   //Rated Capacity ActualCapatity
                        {
                            (void) RD_BCURatesCapActualCap();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }
                        case 9:   //CFG_Version
                        {
                            (void) RD_BCUCfgVer();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }
                        case 10:  //Config update information
                        {
                            (void) RD_BCUCfgUpdate();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }
                        case 11:   //Vehicle plate
                        {
                            (void) RD_BCUVehiclePlate();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 12:   //SOH
                        {
                            (void) RD_BCUSoh();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }

                        case 13:   //fuse position
                        {
                            (void) RD_BCUFusePos();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        case 14:   // Read  BMS system clock
                        {
                            (void) RD_BCUSystemClock();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_PRJ_INFO
                case WR_M_ERR_TRHD_INFO:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:    //fuse position
                        {
                            (void) WR_BCUThrhdLvl1();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:    //Thrhd lvl2
                        {
                            (void) WR_BCUThrhdLvl2();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 3:   //Thrhd lvl3
                        {
                            (void) WR_BCUThrhdLvl3();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        default:
                            break;
                        }
                    }
                    break;
                } //end WR_M_ERR_TRHD_INFO

                case RD_M_ERR_TRHD_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //Thrhd lvl1
                        {
                            (void) RD_BCUThrhdLvl1();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:  //Thrhd lvl2
                        {
                            (void) RD_BCUThrhdLvl2();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 3:   //Thrhd lvl3
                        {
                            (void) RD_BCUThrhdLvl3();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_M_ERR_TRHD_INFO

                case WR_CTRL_INFO:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:    //Heat control(fan/warm/blance close&open)
                        {
                            (void) WR_BCUHeatControl();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:    //charge control
                        {
                            (void) WR_BCUChargeControl();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        default:
                            break;
                        }
                    }
                    break;
                } //end WR_CTRL_INFO

                case RD_CTRL_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //Heat control(fan/warm/blance close&open)
                        {
                            (void) RD_BCUHeatControl();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;


                        }
                        case 2:  //charge control parameter
                        {
                            (void) RD_BCUChargeControl();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_CTRL_INFO

                case WR_HIS_INFO:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:    //History Cellv Infomation    14byte
                        {
                            (void) WR_BCUHistoryCellvInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:   //History Temper Infomation   5byte
                        {
                            (void) WR_BCUHistoryTemperInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 3:    // History low Temper area[0]-[13]     28*2byte
                        {
                            (void) WR_BCUHistoryLowTemperArea();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 4:   // History high Temper area[0]-[13]     28*2byte
                        {
                            (void) WR_BCUHistoryHighTemperArea();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 5:   // History avrg Temper area[0]-[13]     28*2byte
                        {
                            (void) WR_BCUHistoryAvrgTemperArea();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 6:  // History det Temper area[0]-[13]     28*2byte
                        {
                            (void) WR_BCUHistoryDetTemperArea();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 7:   //History Power Infomation
                        {
                            (void) WR_BCUHistoryPowerInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 8:   //History cur area[0]-[13]     28*2byte
                        {
                            (void) WR_BCUHistoryCurrentArea();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 9:  //History SOC area[0]-[9]     28+12byte
                        {
                            (void) WR_BCUHistorySocInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 10:  //total chg&dch AH   8byte  &  Remian Capatiyt 4byte
                        {
                            (void) WR_BCUHistoryCapacityInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        default:
                            break;
                        }
                    }
                    break;
                } //end WR_HIS_INFO

                case RD_HIS_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //History Cellv Infomation    14byte
                        {
                            (void) RD_BCUHistoryCellvInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:   //History Temper Infomation   5byte
                        {
                            (void) RD_BCUHistoryTemperInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 3:  //History low Temper area[0]-[13]     28*2byte
                        {
                            (void) RD_BCUHistoryLowTemperArea();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 4:   //History high Temper area[0]-[13]     28*2byte
                        {
                            (void) RD_BCUHistoryHighTemperArea();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 5:   //History avrg Temper area[0]-[13]     28*2byte
                        {
                            (void) RD_BCUHistoryAvrgTemperArea();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 6:   //History det Temper area[0]-[13]     28*2byte
                        {
                            (void) RD_BCUHistoryDetTemperArea();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 7:    //History power Infomation  8byte
                        {
                            (void) RD_BCUHistoryPowerInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 8:    //History current area[0]-[13]     28*2byte
                        {
                            (void) RD_BCUHistoryCurrentArea();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 9:   //History SOC area[0]-[9]     28+8byte
                        {
                            (void) RD_BCUHistorySocInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 10:   //total chg&dch AH   8byte  &  Remian Capatiyt1 4byte
                        {
                            (void) RD_BCUHistoryCapacityInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_HIS_INFO

                case WR_BAT_INFO:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //bat pack information1  30byte
                        {
                            (void) WR_BCUBatPackInfo1();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:   //bat pack no   4byte
                        {
                            (void) WR_BCUBatPackInfo2();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 3:   //bat pack information3   8byte
                        {
                            (void) WR_BCUBatPackInfo3();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 4:   //bat pack information4    6byte
                        {
                            (void) WR_BCUBatPackInfo4();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        default:
                            break;
                        }
                    }
                    break;
                } //end WR_BAT_INFO

                case RD_BAT_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //bat pack information1
                        {
                            (void) RD_BCUBatPackInfo1();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:    //bat pack information2
                        {
                            (void) RD_BCUBatPackInfo2();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 3:    //bat pack information3
                        {
                            (void) RD_BCUBatPackInfo3();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 4:    //bat pack information4
                        {
                            (void) RD_BCUBatPackInfo4();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_BAT_INFO

                case RD_M_HD_ERR_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        Err_DealHwErrDetail();
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:   //detail hard err
                        {
                            (void) RD_HDErrInfo1();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 2:
                        {
                            (void) RD_HDErrInfo2();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 3:
                        {
                            (void) RD_HDErrInfo3();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }

                        case 4:
                        {
                            (void) RD_HDErrInfo4();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 5:
                        {
                            (void) RD_HDErrInfo5();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 6:
                        {
                            (void) RD_HDErrInfo6();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 7:
                        {
                            (void) RD_HDErrInfo7();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_M_HD_ERR_INFO
                case RD_BATSYS_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:
                        {
                            //current/temper sensr type
                            (void) RD_BCUBatsysInfo1();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }
                        case 2:   //Positive/Negative/Precharge/Charge  contactor life
                        {
                            (void) RD_BCUBatsysInfo2();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 3:   //Vehicle VIN
                        {
                            (void) RD_BCUBatsysInfo3();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_BATSYS_INFO

                case CLB_VOLT_CUR_CS5460:
                {
                    IntCan_MainMsg[MAIN_MSG0].SendCtrl.SendEn = FLAG_ENABLE;
                    switch(msg.data[0])
                    {
                    case CS5460_CUR_OFFSET:
                    {
                        (void)CurDet_ClbCurCurChnOffset(&llwtmp);
                        CurDet_InitCurDet();
                        msg.data[1] = (uByte)(llwtmp >> 24);
                        msg.data[2] = (uByte)(llwtmp >> 16);
                        msg.data[3] = (uByte)(llwtmp >> 8);
                        msg.data[4] = (uByte)(llwtmp >> 0);
                        msg.length = 5;
                        break;
                    }
                    case CS5460_CUR_GAIN:
                    {
                        (void)CurDet_ClbCurCurChnGain(&llwtmp);
                        CurDet_InitCurDet();
                        msg.data[1] = (uByte)(llwtmp >> 24);
                        msg.data[2] = (uByte)(llwtmp >> 16);
                        msg.data[3] = (uByte)(llwtmp >> 8);
                        msg.data[4] = (uByte)(llwtmp >> 0);
                        msg.length = 5;
                        break;
                    }
                    case CS5460_VOLT_OFFSET:
                    {
                        (void)CurDet_ClbCurVolChnOffset(&llwtmp);
                        msg.data[1] = (uByte)(llwtmp >> 24);
                        msg.data[2] = (uByte)(llwtmp >> 16);
                        msg.data[3] = (uByte)(llwtmp >> 8);
                        msg.data[4] = (uByte)(llwtmp >> 0);
                        msg.length = 5;
                        break;
                    }
                    case CS5460_VOLT_GAIN:
                    {
                        (void)CurDet_ClbCurVolChnGain(&llwtmp);
                        msg.data[1] = (uByte)(llwtmp >> 24);
                        msg.data[2] = (uByte)(llwtmp >> 16);
                        msg.data[3] = (uByte)(llwtmp >> 8);
                        msg.data[4] = (uByte)(llwtmp >> 0);
                        msg.length = 5;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }

                    IntCan_DiagRslt(msg);

                    break;
                }  // end CLB_VOLT_CUR_CS5460

                case RD_CUR_VOLT:
                {
                    
                    IntCan_MainMsg[MAIN_MSG0].SendCtrl.SendEn = FLAG_ENABLE;
                    wtmp = 0;
                    switch(msg.data[0])
                    {
                      case 1:   // read avg cur
                      {          
                          CurDet_InitCurDet();
                          wdi_NegVal();
                          while(CurrentIf_RdCur(CURRENTIF_CS5460A_1, RDENERGY, &wtmp, &btmp))
                          {
                              //WdgIf_Trig();
                              //test_wcnt++;
                              wdi_NegVal();
                          }
                          
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }
                      case 2:  // read int sumv AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_INNER_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_INNER_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          
                          SumvDet_UpdateRunStep(SUMV_INNER_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }
                      case 3:  // read out sumv AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_EXTER_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_EXTER_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          SumvDet_UpdateRunStep(SUMV_EXTER_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      } 
                      case 4:  // read int sumv1 AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_BACKUP_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_BACKUP_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          
                          SumvDet_UpdateRunStep(SUMV_BACKUP_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }
                      case 5:  // read out sumv AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_EXT_BACKUP_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_EXT_BACKUP_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          SumvDet_UpdateRunStep(SUMV_EXT_BACKUP_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }
                      case 6:  // read int sumv AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_BACKUP1_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_BACKUP1_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          
                          SumvDet_UpdateRunStep(SUMV_BACKUP1_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }
                      case 7:  // read out sumv AD
                      {
                          for(s = 0;s < 3;s++) 
                          {
                                (void) SumvIf_StartConvert(SUMV_EXT_BACKUP1_ID);
                                ECU_DELAYMS(15);
                                WdgIf_Trig();
                                (void)SumvIf_RdAd(SUMV_EXT_BACKUP1_ID, &wtmp);
                                if(0 != wtmp) 
                                {
                                    break;
                                }
                          }
                          SumvDet_UpdateRunStep(SUMV_EXT_BACKUP1_ID);
                          msg.data[1] = (uByte)(wtmp >> 8);
                          msg.data[2] = (uByte)(wtmp);
                          break;
                      }

                      default: break;
                          
                    }

                    gwd_result = wtmp;

                    msg.length = 3;
                    IntCan_DiagRslt(msg);

                    break;

                }

                case SET_OFFSET_GAIN:
                {
                    IntCan_MainMsg[MAIN_MSG0].SendCtrl.SendEn = FLAG_ENABLE;
                    switch(msg.data[0])
                    {
                    case 1:      // set cur gain
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];
                        if(ERR_OK == CurDet_UpdateGainAndOffset(wtmp, 0))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }
                    case 2:     // set in sumv gain and offset
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];
                        if(ERR_OK == SumvDet_UpdateGainAndOffset(SUMV_INNER_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }
                    case 3:       // extern sumv
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];  // offset
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];

                        if(ERR_OK == SumvDet_UpdateGainAndOffset(SUMV_EXTER_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }

                        break;
                    } 
                    case 4:     // set in sumv gain and offset
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];
                        if(ERR_OK ==SumvDet_UpdateGainAndOffset(SUMV_BACKUP_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }
                    case 5:       // extern sumv
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];  // offset
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];

                        if(ERR_OK == SumvDet_UpdateGainAndOffset(SUMV_EXT_BACKUP_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }

                        break;
                    }
                    case 6:     // set in sumv gain and offset
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];
                        if(ERR_OK ==SumvDet_UpdateGainAndOffset(SUMV_BACKUP1_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }
                    case 7:       // extern sumv
                    {
                        wtmp = msg.data[1];
                        wtmp <<= 8;
                        wtmp += msg.data[2];   // gain

                        llwtmp = msg.data[3];  // offset
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        llwtmp <<= 8;
                        llwtmp += msg.data[5];
                        llwtmp <<= 8;
                        llwtmp += msg.data[6];

                        if(ERR_OK == SumvDet_UpdateGainAndOffset(SUMV_EXT_BACKUP1_ID, wtmp, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }

                        break;
                    }

                    default:
                        break;
                    }

                    msg.length = 2;
                    IntCan_DiagRslt(msg);

                    break;

                }  // end case  SET_OFFSET_GAIN
                case SET_CUR_ENERGE:
                {
                    IntCan_MainMsg[MAIN_MSG0].SendCtrl.SendEn = FLAG_ENABLE;
                    switch(msg.data[0])
                    {
                    case 1:      // default
                    {
                        if(FLAG_ENABLE == IntCan_StaticSet.AhThrhdSelfDefEn)
                        {
                            llwtmp = IntCan_StaticSet.AhThrhd;
                            if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                              &&(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                            {
                                msg.data[1] = 1;
                            }
                            else
                            {
                                msg.data[1] = 0;
                            }
                        }
                        else
                        {
                            if(1 == msg.data[1])      // FLA 100A
                            {
                                llwtmp = FLA_100A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                  &&(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(2 == msg.data[1])  // FLA 200A
                            {
                                llwtmp = FLA_200A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                  &&(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(3 == msg.data[1])  // FLA 300A
                            {
                                llwtmp = FLA_300A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                  &&(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(4 == msg.data[1])  // FLA 400A
                            {
                                llwtmp = FLA_400A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                        && (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(5 == msg.data[1])  // FLA 500A
                            {
                                llwtmp = FLA_500A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                        && (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(6 == msg.data[1])  // FLA 600A
                            {
                                llwtmp = FLA_600A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                        && (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else if(11 == msg.data[1])  // FLA 50A
                            {
                                llwtmp = FLA_50A_ENERGE_THRHD;
                                if( (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                                        && (ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp)))
                                {
                                    msg.data[1] = 1;
                                }
                                else
                                {
                                    msg.data[1] = 0;
                                }
                            }
                            else
                            {
                                msg.data[1] = 0;
                            }
                        }

                        break;
                    }
                    case 2:     // default
                    {
                        break;
                    }
                    case 3:    // default
                    {
                        break;
                    }
                    case 4:
                    {
                        llwtmp = msg.data[1];
                        llwtmp <<= 8;
                        llwtmp += msg.data[2];
                        llwtmp <<= 8;
                        llwtmp += msg.data[3];
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];
                        if(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_CHG, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }
                    case 5:
                    {
                        llwtmp = msg.data[1];
                        llwtmp <<= 8;
                        llwtmp += msg.data[2];
                        llwtmp <<= 8;
                        llwtmp += msg.data[3];
                        llwtmp <<= 8;
                        llwtmp += msg.data[4];

                        if(ERR_OK == CurDet_UpdateAhCalcThrhd(CURRENTIF_FLAG_DCH, llwtmp))
                        {
                            msg.data[1] = 1;
                        }
                        else
                        {
                            msg.data[1] = 0;
                        }
                        break;
                    }

                    default:
                        break;
                    }

                    msg.length = 2;
                    IntCan_DiagRslt(msg);

                    break;
                } // end case SET_CUR_ENERGE

                case RD_RAM_VAL:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:
                        {

                            break;
                        }
                        case 2:
                        {

                            break;
                        }

                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_RAM_VAL

                case RD_SOFWARE_VER:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        switch(CanTpMsgRx.Data[0])
                        {
                        case 1:
                        {
                            (void) RD_GetProjectVer();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        case 2:
                        {
                            (void) RD_GetPlatVer();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }

                        case 3:
                        {
                            (void) RD_GetModuleVer();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;
                        }

                        case 4:
                        {
                            (void) RD_GetHardwareVer();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            break;

                        }
                        default:
                            break;
                        }
                    }
                    break;
                } //end RD_SOFWARE_VER

                case CCP_AUTH:
                {
                    if(1 == msg.data[0])
                    {
        //                IntCan_MsgSendDisable();
                        /*
                                        for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
                                        {
                                            IntCan_MainMsg[i].SendCtrl.SendEn = FLAG_DISABLE;
                                        }
                        */
                    }
                    else if(0x07 == msg.data[0])
                    {
        //                IntCan_MsgSendEnable();
                        /*
                                        for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
                                        {
                                            IntCan_MainMsg[i].SendCtrl.SendEn = FLAG_ENABLE;
                                        }
                        */
                    }
                    break;
                } //end CCP_AUTH

                /* Test Cmd Service */
                case TEST_CMD:
                {
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        CanTpMsgTx.DataLen = 0;
                        CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
                        CanTpMsgTx.DataLen += 1;
                        CanTpMsgTx.Id = msg.id;
                        // deal cmd_num
                        switch(CanTpMsgRx.Data[0])
                        {
                          case 1: // test relay
                          {
                              switch(CanTpMsgRx.Data[1])
                              {
                                case 1: // pos relay
                                {
                                    CanTpMsgTx.Data[1] = 1; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(POS_RELAY_ID,ctrl_relay_cmd);

                                        //BatSys_UpdatePosRlySt((uByte)Actor_GetRelaySt(POS_RELAY_ID));
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 2: // neg relay
                                {
                                    CanTpMsgTx.Data[1] = 2; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(NEG_RELAY_ID,ctrl_relay_cmd);
                                        //BatSys_UpdateNegRlySt((uByte)Actor_GetRelaySt(NEG_RELAY_ID));
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 3: // pre relay
                                {
                                    CanTpMsgTx.Data[1] = 3; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(PRE_RELAY_ID,ctrl_relay_cmd);
                                        //BatSys_UpdatePreRlySt((uByte)Actor_GetRelaySt(PRE_RELAY_ID));
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 4: // heat relay
                                {
                                    CanTpMsgTx.Data[1] = 4; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_HEAT_ID,ctrl_relay_cmd);
                                        //BatSys_UpdateAuxASt((uByte)Actor_GetRelaySt(CPU_HEAT_ID)); // had place to Actor_CtrlRelay
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 5: //  chg relay
                                {
                                    CanTpMsgTx.Data[1] = 5; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CHG_RELAY_ID,ctrl_relay_cmd);
                                        //BatSys_UpdateChgRlySt((uByte)Actor_GetRelaySt(CHG_RELAY_ID));
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 6: //  fan relay
                                {
                                    CanTpMsgTx.Data[1] = 6; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = (Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_FAN_ID,ctrl_relay_cmd);
                                        //BatSys_UpdateAuxBSt((uByte)Actor_GetRelaySt(CPU_FAN_ID));
                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 0xAA : // all relay closed
                                {
                                    CanTpMsgTx.Data[1] = 0xAA; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = RELAY_CLOSED;//(Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(POS_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(NEG_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(PRE_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CHG_RELAY_ID,ctrl_relay_cmd);

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_FAN_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_HEAT_ID,ctrl_relay_cmd);

                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 0x55: // all relay opened
                                {
                                    CanTpMsgTx.Data[1] = 0x55; // relay NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        ctrl_relay_cmd.CmdType = TEST_RELAY_CMD;
                                        ctrl_relay_cmd.Bcmd = RELAY_OPENED;//(Bool)(CanTpMsgRx.Data[2]);
                                        ctrl_relay_cmd.DuratonTime =  *(uLWord *)(&(CanTpMsgRx.Data[3]));

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(POS_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(NEG_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(PRE_RELAY_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CHG_RELAY_ID,ctrl_relay_cmd);

                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_FAN_ID,ctrl_relay_cmd);
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlRelay(CPU_HEAT_ID,ctrl_relay_cmd);

                                        CanTpMsgTx.DataLen += 2;

                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }
                                default: break;
                                  
                              }

                              break;
                          }
                        // end of test relay

                          case 2: // DO relay
                          {
                              switch(CanTpMsgRx.Data[1])
                              {
                                case 1: // DO1
                                {
                                    CanTpMsgTx.Data[1] = 1; // DO NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlDo(CPU_DO1_ID, (Bool)(CanTpMsgRx.Data[2]));
                                        CanTpMsgTx.DataLen += 2;
                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                case 2: // DO2
                                {
                                    CanTpMsgTx.Data[1] = 2; // DO NO. in CAN
                                    CanTpMsgTx.DataLen += 1;
                                    if(0 == ps) // is bcu?
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = Actor_CtrlDo(CPU_DO2_ID, (Bool)(CanTpMsgRx.Data[2]));
                                        CanTpMsgTx.DataLen += 2;
                                    }
                                    else
                                    {
                                        *(uWord *)&(CanTpMsgTx.Data[2]) = ERR_INTCAN_ERROR_CMD; // default return error code
                                        CanTpMsgTx.DataLen += 2;
                                    }

                                    break;
                                }

                                default:break;    
                              }

                              break;
                          }

                          default: break;
                            
                        }

                        IntCan_CanTpDiagRslt(CanTpMsgTx);
                    }
                    break;
                } // end case Test
                
                
                case WR_QR_CODE_INFO:    //写生产二维码信息
                {
                    
                    // call CanTp recv multi pack
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        // deal cmd_num
                        if(CanTpMsgRx.Data[0])
                        {
                            (void) WR_QrCodeInfo();
                            CanTpMsgTx.DataLen = 3;
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                            
                        }
                    }
                    break;
                }
                case RD_QR_CODE_INFO:
                {
                    if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
                    {
                        if(CanTpMsgRx.Data[0])
                        {
    
                            (void) RD_QrCodeInfo();
                            CanTpMsgTx.Id = msg.id;
                            IntCan_CanTpDiagRslt(CanTpMsgTx);
                        }
     
                     }
                     break;
                }
                case ELOCK_TEST:
                {
                     if(ps == 0x00)
                     {
                       /*  Elock_UpdateMainSartFlag(2);
                         if(0 == msg.data[0])
                         {
                             Elock_TurnRight();
                         }
                         else if(1 == msg.data[0])
                         {
                             Elock_TurnLeft();
                         }
                         else if(2 == msg.data[0])
                         {
                             Elock_TurnOff();
                         }
                         else
                         {
                             Elock_UpdateMainSartFlag(0);
                         }  */
                     }
                     break;
                }
                
          
          }// end if(sa == 0x91)
          
          
          case BOOT:
          {
              IntCan_MsgSendDisable();
              (void)RunModem_SwitchModeTo(RUNMODE_SYS_MTCE);
              IntCan_BroadCastMsg.Msg.data[4] = BatSys_GetRunMode();
              if(0 == ps)
              {

                  (void)POWER_OFF(); // prevent can not power off

                  ATB_GoIntoBoot();
              }

              break;
          } //end BOOT

          case ERASE_RECORD:
          {
              /* if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
               {
                   switch(CanTpMsgRx.Data[0])
                   {
                   case 1:
                   {

                       break;
                   }
                   default:
                       break;
                   }
               } */
              DataStore_EraseFlash(msg);
              break;
          } //end ERASE_RECORD
         
                 
          case RD_RECORD:
          {
              /* if(ERR_OK == CanTp_EventRx(INT_CAN_CHANNEL, &msg, &CanTpMsgRx))
               {
                   switch(CanTpMsgRx.Data[0])
                   {
                   case 1:
                   {

                       break;
                   }
                   default:
                       break;
                   }
               }*/
              IntCan_MsgSendDisable();
              (void)RunModem_SwitchModeTo(RUNMODE_DIAG);
              IntCan_BroadCastMsg.Msg.data[4] = BatSys_GetRunMode();

              DataStore_ReadAndTxData(msg);
              break;
          } //end RD_RECORD

          default: break;
              
          } // end switch msg_cnt

          IntCan_UpdateCanPara();

          break;

      } // end case EXT_CMD:

      default:
      {
          break;
      }

    }  // end switch(msg_type)
}

void IntCan_DiagRslt(sCanIf_MsgType msg_ptr)
{
    uWord  wtmp;
    uByte  btmp;
    uWord rslt = 0;

    // exchange id
    wtmp = (uWord)(msg_ptr.id & 0x0000FFFF);
    btmp = (uByte)(wtmp >> 8);
    wtmp <<= 8;
    wtmp &= 0xFF00;
    wtmp |= btmp;
    msg_ptr.id &= 0xFFFF0000;
    msg_ptr.id += wtmp;
    msg_ptr.length = 8;

    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &msg_ptr, 0);
}

static void IntCan_CanTpDiagRslt(sCanTp_MsgType msg)
{
    uWord  wtmp;
    uByte  btmp;
    uWord rslt = 0;

    // exchange id
    wtmp = (uWord)(msg.Id & 0x0000FFFF);
    btmp = (uByte)(wtmp >> 8);
    wtmp <<= 8;
    wtmp &= 0xFF00;
    wtmp |= btmp;
    msg.Id &= 0xFFFF0000;
    msg.Id += wtmp;

    SaveStatusReg();
    (void)CanTp_CopyDataToBuf(INT_CAN_CHANNEL, &msg);
    RestoreStatusReg();
}


// call by 5ms main task
void IntCan_MainSendMsg(void)
{
    uByte i;
    uWord rslt = 0;

    // send BroadcastMsg
    if(0 != IntCan_BroadCastMsg.Msg.id)
    {
        // comfirm enalbe send
        if(FLAG_ENABLE == IntCan_BroadCastMsg.SendCtrl.SendEn)
        {
            IntCan_BroadCastMsg.SendCtrl.TimeCnt++;
            if(1 == IntCan_BroadCastMsg.SendCtrl.FirSendFlag)
            {
                if(IntCan_BroadCastMsg.SendCtrl.TimeCnt >= IntCan_BroadCastMsg.PeriodCnt)
                {
                    IntCan_BroadCastMsg.SendCtrl.TimeCnt = 0;
                    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_BroadCastMsg.Msg, 0);
                    IntCan_LocalEcuInfo.Life++;
                }
            }
            else
            {
                // waite send first
                if(IntCan_BroadCastMsg.SendCtrl.TimeCnt >= IntCan_BroadCastMsg.SendCtrl.FirSendTime)
                {
                    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_BroadCastMsg.Msg, 0);
                    IntCan_BroadCastMsg.SendCtrl.TimeCnt = 0;
                    IntCan_BroadCastMsg.SendCtrl.FirSendFlag = 1;
                    IntCan_LocalEcuInfo.Life++;
                }
            }
        }
    }

    // send main msg
    for(i = 0; i < INT_CAN_MAIN_INFO_BUFF; i++)
    {
        // comfirm id
        if(0 != IntCan_MainMsg[i].Msg.id)
        {
            // comfirm enalbe send
            if(FLAG_ENABLE == IntCan_MainMsg[i].SendCtrl.SendEn)
            {

                IntCan_MainMsg[i].SendCtrl.TimeCnt++;
                if(1 == IntCan_MainMsg[i].SendCtrl.FirSendFlag)
                {
                    if(IntCan_MainMsg[i].SendCtrl.TimeCnt >= IntCan_MainMsg[i].PeriodCnt)
                    {
                        IntCan_MainMsg[i].SendCtrl.TimeCnt = 0;
                        (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_MainMsg[i].Msg, 0);
                        IntCan_MainMsg[i].SendCtrl.SendCnt++;
                    }
                }
                else
                {
                    // waite send first
                    if(IntCan_MainMsg[i].SendCtrl.TimeCnt >= IntCan_MainMsg[i].SendCtrl.FirSendTime)
                    {
                        (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_MainMsg[i].Msg, 0);
                        IntCan_MainMsg[i].SendCtrl.TimeCnt = 0;
                        IntCan_MainMsg[i].SendCtrl.FirSendFlag = 1;
                    }
                }
            }
        }
    }

    // send event message
    for(i = 0; i < INT_CAN_EVENT_INFO_BUF; i++)
    {
        // comfirm id
        if(0 != IntCan_EventMsg[i].Msg.id)
        {
            // comfirm enalbe send
            if(FLAG_ENABLE == IntCan_EventMsg[i].SendCtrl.SendEn)
            {
                // send first frame instant
                if(1 == IntCan_EventMsg[i].SendCtrl.FirSendFlag)
                {
                    IntCan_EventMsg[i].SendCtrl.TimeCnt++;
                    if(IntCan_EventMsg[i].SendCtrl.TimeCnt >= IntCan_EventMsg[i].PeriodCnt)
                    {
                        IntCan_EventMsg[i].SendCtrl.TimeCnt = 0;
                        (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_EventMsg[i].Msg, 0);
                    }
                }
                else
                {
                    (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_EventMsg[i].Msg, 0);
                    IntCan_EventMsg[i].SendCtrl.FirSendFlag = 1;
                }
            }
        }
    }
    
    if(1 == IntCan_VoltMsg_Send_En)
    {
      
        IntCan_VoltMsg.SendCtrl.TimeCnt++;
                

        if(IntCan_VoltMsg.SendCtrl.TimeCnt >= IntCan_VoltMsg.PeriodCnt )
        {
            IntCan_VoltMsg.SendCtrl.TimeCnt = 0;

           (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_VoltMsg.Msg, rslt);
           
            IntCan_VoltMsg.SendCtrl.FirSendFlag = 1;
            IntCan_VoltMsg_Send_En = 0;
        }
    } 
    
    if(1 == IntCan_TempMsg_Send_En)
    {
      
        IntCan_TemperMsg.SendCtrl.TimeCnt++;
                

        if(IntCan_TemperMsg.SendCtrl.TimeCnt >= IntCan_TemperMsg.PeriodCnt )
        {
            IntCan_TemperMsg.SendCtrl.TimeCnt = 0;

           (void)CanIf_RequestSend(INT_CAN_CHANNEL, &IntCan_TemperMsg.Msg, rslt);
           
            IntCan_TemperMsg.SendCtrl.FirSendFlag = 1;
            IntCan_TempMsg_Send_En = 0;
        }
    }

    CanTp_MainTx();
}


//call by main task
void IntCan_MainReceiveMsg(void)
{
    GeneralReturnType  rslt;
    uByte buf_length;
    sCanIf_MsgType msg;

    /* have data ? */
    buf_length = CanIf_GetRxBufferLength(INT_CAN_CHANNEL);

    while(buf_length)
    {
        rslt = CanIf_RxIndication(INT_CAN_CHANNEL,&msg);
        if(!rslt)
        {
            IntCan_ProcMsg(msg);
        }
        buf_length--;
    }

}


// call by 10ms by main task
void IntCan_LoadMsgData(void)
{

    uByte i,j;
    uByte btmp;
    uWord wtmp = 0;
    uLWord llwd;
    uByte* ptr;
    uByte num = 0;
    uByte soc;
    uByte data[6];
    sErr_ByteNType err_byte;
    eSensor_DiStatusType DiSt;

    uByte total_bmu_num=0;
    uByte cellv_cnt=0;
    uByte temper_cnt=0;
    uByte total_bat_num=0;
    uByte total_temper_num=0;

    SaveStatusReg();

#ifdef __DEBUG_INTCAN
    IntCan_TestData();

#endif

    // for load bmu,bcu_m_c, volt res temper soc
    
     // 0x08009100 ~ 0X08xx9100 //
    for(i = 0; i < 120; i++)
    {
        gwd_sys_cellv[i] = 0;    
    }
    total_bmu_num = Bat_GetBmuNum();
    cellv_cnt = 0;
    total_bat_num = 0; 
    for(i = 0; i < total_bmu_num; i++)
    {
        btmp = (uByte)(Bat_GetPackSerialNum(i+1));
        total_bat_num += btmp;
        for(j = 0; j < btmp; j++)
        {
            if(Bat_GetCellV(i+1, j+1) < 0xFFF0)
            {
               gwd_sys_cellv[cellv_cnt++] = Bat_GetCellV(i+1, j+1);
            }

        }
    }
    
    if(1 == IntCan_VoltMsg.SendCtrl.FirSendFlag)
    {
       // btmp = sCarCan_Msg_Vol_Start_Sa;
        llwd = (uLWord)(IntCan_Msg_Vol_Start_Sa);
        llwd = llwd << 16;
        IntCan_VoltMsg.Msg.id = (llwd) + 0x08009100;
        
        btmp = total_bat_num/4;
        if((total_bat_num % 4) > 0)
        {
            btmp += 1;
        }
        
        IntCan_VoltMsg.Msg.data[0] = GET_HIGH_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4]); 
        IntCan_VoltMsg.Msg.data[1] = GET_LOW_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4]);
        IntCan_VoltMsg.Msg.data[2] = GET_HIGH_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 1]); 
        IntCan_VoltMsg.Msg.data[3] = GET_LOW_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 1]);
        IntCan_VoltMsg.Msg.data[4] = GET_HIGH_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 2]);
        IntCan_VoltMsg.Msg.data[5] = GET_LOW_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 2]);
        IntCan_VoltMsg.Msg.data[6] = GET_HIGH_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 3]); 
        IntCan_VoltMsg.Msg.data[7] = GET_LOW_BYTE(gwd_sys_cellv[IntCan_Msg_Vol_Start_Sa*4 + 3]);
       
        IntCan_Msg_Vol_Start_Sa++;
        if(IntCan_Msg_Vol_Start_Sa >= btmp)
        {
            IntCan_Msg_Vol_Start_Sa = 0;
        }
        IntCan_VoltMsg.SendCtrl.FirSendFlag = 0;
        IntCan_VoltMsg_Send_En = 1;
    }
    
    for(i = 0; i < 39; i++)
    {
        gby_sys_Temper[i] = 0;    
    }
    total_bmu_num = Bat_GetBmuNum();
    temper_cnt = 0;
    total_temper_num = 0; 
    for(i = 0; i < total_bmu_num; i++)
    {
        btmp = (uByte)(Bat_GetPackTemperNum(i+1));
        total_temper_num += btmp;
        for(j = 0; j < btmp; j++)
        {
            if(Bat_GetTemper(i+1, j+1) < 0xF0)
            {
               gby_sys_Temper[temper_cnt++] = Bat_GetTemper(i+1, j+1);
            }
        }
    }
    
    if(1 == IntCan_TemperMsg.SendCtrl.FirSendFlag)
    {
       // btmp = sCarCan_Msg_Vol_Start_Sa;
        llwd = (uLWord)(IntCan_Msg_Temper_Start_Sa);
        llwd = llwd << 16;
        IntCan_TemperMsg.Msg.id = (llwd) + 0x10009100;
        
        btmp = total_temper_num/8;
        if((total_temper_num % 8) > 0)
        {
            btmp += 1;
        }
        for(i = 0;i < 8;i++)
        {
          
        IntCan_TemperMsg.Msg.data[i] = gby_sys_Temper[IntCan_Msg_Temper_Start_Sa*8 + i]; 
        }
       
        IntCan_Msg_Temper_Start_Sa++;
        if(IntCan_Msg_Temper_Start_Sa >= btmp)
        {
            IntCan_Msg_Temper_Start_Sa = 0;
        }
        IntCan_TemperMsg.SendCtrl.FirSendFlag = 0;
        IntCan_TempMsg_Send_En = 1;
    }

    // main_msg0

    wtmp = Bat_GetSumV(IntCan_StaticSet.SumVType);
    IntCan_MainMsg[MAIN_MSG0].Msg.data[0] = GET_HIGH_BYTE(wtmp); // INT sumv
    IntCan_MainMsg[MAIN_MSG0].Msg.data[1] = GET_LOW_BYTE(wtmp);

    wtmp = Bat_GetCurrent(INSCUR);
    IntCan_MainMsg[MAIN_MSG0].Msg.data[2] = GET_HIGH_BYTE(wtmp); // instant cur
    IntCan_MainMsg[MAIN_MSG0].Msg.data[3] = GET_LOW_BYTE(wtmp);

    wtmp = Bat_GetSumV(OUT_SUMV);
    IntCan_MainMsg[MAIN_MSG0].Msg.data[4] = GET_HIGH_BYTE(wtmp); // out sumv
    IntCan_MainMsg[MAIN_MSG0].Msg.data[5] = GET_LOW_BYTE(wtmp);

    wtmp = Bat_GetCurrent(AVGCUR);
    IntCan_MainMsg[MAIN_MSG0].Msg.data[6] = GET_HIGH_BYTE(wtmp);  // average cur
    IntCan_MainMsg[MAIN_MSG0].Msg.data[7] = GET_LOW_BYTE(wtmp);

    // main_msg1
    wtmp = Bat_GetMaxV();
    IntCan_MainMsg[MAIN_MSG1].Msg.data[0] = GET_HIGH_BYTE(wtmp); // max_v
    IntCan_MainMsg[MAIN_MSG1].Msg.data[1] = GET_LOW_BYTE(wtmp);

    wtmp = Bat_GetMinV();
    IntCan_MainMsg[MAIN_MSG1].Msg.data[2] = GET_HIGH_BYTE(wtmp); // min_v
    IntCan_MainMsg[MAIN_MSG1].Msg.data[3] = GET_LOW_BYTE(wtmp);

    wtmp = Bat_GetMaxVTotalNo();
    wtmp &= 0x0FFF;
    IntCan_MainMsg[MAIN_MSG1].Msg.data[4] = (uByte)(wtmp >> 4); // max_v_no
    IntCan_MainMsg[MAIN_MSG1].Msg.data[5] = (uByte)((wtmp & 0x000F) << 4);

    wtmp = Bat_GetMinVTotalNo();
    wtmp &= 0x0FFF;
    IntCan_MainMsg[MAIN_MSG1].Msg.data[5] |= (uByte)(wtmp >> 8); // min_v_no
    IntCan_MainMsg[MAIN_MSG1].Msg.data[6] = (uByte)(wtmp);
    wtmp = Bat_GetBatNum();
    IntCan_MainMsg[MAIN_MSG1].Msg.data[7] = (uByte)(wtmp);

    if(0 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage++;
        // main_msg2
        IntCan_MainMsg[MAIN_MSG2].Msg.data[0] = Bat_GetMaxT();  // max_t
        IntCan_MainMsg[MAIN_MSG2].Msg.data[1] = Bat_GetMinT();  // min_t

        wtmp =  Bat_GetMaxTTotalNo();
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG2].Msg.data[2] = (uByte)(wtmp >> 4); // max_t_no
        IntCan_MainMsg[MAIN_MSG2].Msg.data[3] = (uByte)((wtmp & 0x000F) << 4);

        wtmp =  Bat_GetMinTTotalNo();
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG2].Msg.data[3] |= (uByte)(wtmp >> 8); // min_v_no
        IntCan_MainMsg[MAIN_MSG2].Msg.data[4] = (uByte)(wtmp);

        wtmp =  Bat_GetSOC();        // soc
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG2].Msg.data[5] = (uByte)(wtmp >> 4);
        IntCan_MainMsg[MAIN_MSG2].Msg.data[6] = (uByte)((wtmp & 0x000F) << 4);

        wtmp = Bat_GetSoh();   // soh
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG2].Msg.data[6] |= (uByte)(wtmp >> 8);
        IntCan_MainMsg[MAIN_MSG2].Msg.data[7] = (uByte)(wtmp);

        // main_msg3
        IntCan_MainMsg[MAIN_MSG3].Msg.data[0] = (uByte)Bat_GetMaxR();  // max_r
        IntCan_MainMsg[MAIN_MSG3].Msg.data[1] = (uByte)Bat_GetMinR();  // min_r

        wtmp = Bat_GetMaxRTotalNo(); //max_r_no
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG3].Msg.data[2] = (uByte)(wtmp >> 4);
        IntCan_MainMsg[MAIN_MSG3].Msg.data[3] = (uByte)((wtmp & 0x0F) << 4);

        wtmp = Bat_GetMinRTotalNo(); //min_r_no
        wtmp &= 0x0FFF;
        IntCan_MainMsg[MAIN_MSG3].Msg.data[3] |= (uByte)(wtmp >> 8);
        IntCan_MainMsg[MAIN_MSG3].Msg.data[4] = (uByte)(wtmp);

        wtmp = Bat_GetTotalIntRes();   // total res
        IntCan_MainMsg[MAIN_MSG3].Msg.data[5] = GET_HIGH_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG3].Msg.data[6] = GET_LOW_BYTE(wtmp);
        
        wtmp = Bat_GetTemperNum();
        IntCan_MainMsg[MAIN_MSG3].Msg.data[7] = (uByte)(wtmp);

        // main_msg4
        wtmp = Bat_GetMaxFeedBackCur();  // max_chg_cur
        IntCan_MainMsg[MAIN_MSG4].Msg.data[0] = GET_HIGH_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG4].Msg.data[1] = GET_LOW_BYTE(wtmp);

        wtmp = Bat_GetMaxPermitDchCur();  // max_dch_cur
        IntCan_MainMsg[MAIN_MSG4].Msg.data[2] = GET_HIGH_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG4].Msg.data[3] = GET_LOW_BYTE(wtmp);

        llwd = Bat_GetActualCapacityCalc(); // act_cap
        IntCan_MainMsg[MAIN_MSG4].Msg.data[4] = (uByte)(llwd >> 24);//GET_HIGH_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG4].Msg.data[5] = (uByte)(llwd >> 16);//GET_LOW_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG4].Msg.data[6] = (uByte)(llwd >> 8);
        IntCan_MainMsg[MAIN_MSG4].Msg.data[7] = (uByte)(llwd);

        // main_msg5
        llwd = Bat_GetAccumulateChgAh() ;  // total in ah unit 1mAH
        IntCan_MainMsg[MAIN_MSG5].Msg.data[0] = (uByte)(llwd >> 24);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[1] = (uByte)(llwd >> 16);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[2] = (uByte)(llwd >> 8);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[3] = (uByte)(llwd);

        llwd = Bat_GetAccumulateDchgAh() ;  // total out ah unit 1mAH
        IntCan_MainMsg[MAIN_MSG5].Msg.data[4] = (uByte)(llwd >> 24);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[5] = (uByte)(llwd >> 16);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[6] = (uByte)(llwd >> 8);
        IntCan_MainMsg[MAIN_MSG5].Msg.data[7] = (uByte)(llwd);
    }

    else if(1 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage++;
        //main_msg6
        IntCan_MainMsg[MAIN_MSG6].Msg.data[0] = 0;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[0] |= (BatSys_GetPosRlySt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[0] |= (BatSys_GetNegRlySt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[0] |= (BatSys_GetPreRlySt() & 0x03) << 2;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[0] |= (BatSys_GetChgRlySt() & 0x03);

        IntCan_MainMsg[MAIN_MSG6].Msg.data[1] = 0;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[1] |= (BatSys_GetAuxASt() &0x03) << 6;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[1] |= (BatSys_GetAuxBSt() &0x03) << 4;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[1] |= ((uByte)Actor_GetDoSt((uByte)CPU_DO1_ID) &0x03) << 2;

        IntCan_MainMsg[MAIN_MSG6].Msg.data[1] |= ((uByte)Actor_GetDoSt((uByte)CPU_DO2_ID) &0x03);

        IntCan_MainMsg[MAIN_MSG6].Msg.data[2] = Chg_GetCpDutyCycle(); //cp

        IntCan_MainMsg[MAIN_MSG6].Msg.data[3] = 0;
        IntCan_MainMsg[MAIN_MSG6].Msg.data[3] |= (((uByte)Sensor_GetDiSt((uByte)CPU_DI1_ID) &0x03) << 6);  // DI1
        IntCan_MainMsg[MAIN_MSG6].Msg.data[3] |= (((uByte)Sensor_GetDiSt((uByte)CPU_DI2_ID) &0x03) << 4);  // DI2
        IntCan_MainMsg[MAIN_MSG6].Msg.data[3] |= (((uByte)Sensor_GetDiSt((uByte)CPU_DI3_ID) &0x03) << 2);  // DI3
        IntCan_MainMsg[MAIN_MSG6].Msg.data[3] |= (((uByte)Sensor_GetDiSt((uByte)CPU_DI4_ID) &0x03) << 0);  // Di4

        IntCan_MainMsg[MAIN_MSG6].Msg.data[4] = (uByte)(Chg_GetCcVolt() / 2); //cc


        wtmp =  Sensor_GetAiSt((uByte)CPU_AI1_ID);   // 1mv
        wtmp /= 200; // 0.2v

        IntCan_MainMsg[MAIN_MSG6].Msg.data[5] = (uByte)wtmp;   // AI1

        wtmp =  Sensor_GetAiSt((uByte)CPU_AI2_ID);   // 1mv
        wtmp /= 200;  // 0.2v

        IntCan_MainMsg[MAIN_MSG6].Msg.data[6] = (uByte)wtmp;  // AI2

        IntCan_MainMsg[MAIN_MSG6].Msg.data[7] = (uByte)(Chg_GetCc2Volt() / 2); //cc2
    }
    else if(2 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage++;
        // main_msg7
        IntCan_MainMsg[MAIN_MSG7].Msg.data[0] = (BmsSys_GetIntCanBusSt() & 0x03) << 6; // can_state
        IntCan_MainMsg[MAIN_MSG7].Msg.data[0] |= (BmsSys_GetCarCanBusSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG7].Msg.data[0] |= (BmsSys_GetChgCanBusSt() & 0x03) << 2;
        // reserve diag can status

        (void)BmsSys_GetSystemTime(data);
        IntCan_MainMsg[MAIN_MSG7].Msg.data[1] = 0;
        IntCan_MainMsg[MAIN_MSG7].Msg.data[2] = data[0];//BmsSys_GetSystemTime(0);  // year
        IntCan_MainMsg[MAIN_MSG7].Msg.data[3] = data[1];//BmsSys_GetSystemTime(1);  // month
        IntCan_MainMsg[MAIN_MSG7].Msg.data[4] = data[2];//BmsSys_GetSystemTime(2);  // day
        IntCan_MainMsg[MAIN_MSG7].Msg.data[5] = data[3];//BmsSys_GetSystemTime(3);  // houre
        IntCan_MainMsg[MAIN_MSG7].Msg.data[6] = data[4];//BmsSys_GetSystemTime(4);  // minute
        IntCan_MainMsg[MAIN_MSG7].Msg.data[7] = data[5];// BmsSys_GetSystemTime(5);  // Second

        // main_msg8
        wtmp = (uWord)BatSys_GetIsoPosResistor();
        IntCan_MainMsg[MAIN_MSG8].Msg.data[0] = GET_HIGH_BYTE(wtmp); // pos iso resiter
        IntCan_MainMsg[MAIN_MSG8].Msg.data[1] = GET_LOW_BYTE(wtmp);

        wtmp = (uWord)BatSys_GetIsoNegResistor();
        IntCan_MainMsg[MAIN_MSG8].Msg.data[2] = GET_HIGH_BYTE(wtmp); // pos iso resiter
        IntCan_MainMsg[MAIN_MSG8].Msg.data[3] = GET_LOW_BYTE(wtmp);

        // power resver
        // calculate power
        llwd = Bat_GetCurrent(INSCUR);
        wtmp = Bat_GetSumV(INT_SUMV);
        if(llwd >= 30000)
        {
            llwd -= 30000;
            llwd *= wtmp;
            llwd /= 10000;
            llwd += 30000;
        }
        else
        {
            llwd = 30000 - llwd;
            llwd *= wtmp;
            llwd /= 10000;
            llwd = 30000 - llwd;
        }
        wtmp = (uWord)llwd;//Bat_GetPwr();
        Bat_UpdatePwr(wtmp);
        IntCan_MainMsg[MAIN_MSG8].Msg.data[4] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG8].Msg.data[5] = GET_LOW_BYTE(wtmp);
    }

    else if(3 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage++;
        // main_msg9
        err_byte = Err_GetBatAllErrLevel();  // bat error
        for(i = 0; i < 8; i++)
        {
            IntCan_MainMsg[MAIN_MSG9].Msg.data[i] = err_byte.By[i];
        }

        // main_msg10
        err_byte = Err_GetBatSysAllErrLevel(0);   // BatSys error
        for(i = 0; i < 8; i++)
        {
            IntCan_MainMsg[MAIN_MSG10].Msg.data[i] = err_byte.By[i];
        }

        // main_msg11
        err_byte = Err_GetBatSysAllErrLevel(1);    // other err
        for(i = 0; i < 8; i++)
        {
            IntCan_MainMsg[MAIN_MSG11].Msg.data[i] = err_byte.By[i];
        }

        // main_msg12
        err_byte = Err_GetHwAllErrLevel();    // hardware err
        for(i = 0; i < 8; i++)
        {
            IntCan_MainMsg[MAIN_MSG12].Msg.data[i] = err_byte.By[i];
        }
        
        wtmp = Bat_GetSumV(INT_SUMV_BAK1);
        IntCan_MainMsg[MAIN_MSG13].Msg.data[0] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG13].Msg.data[1] = GET_LOW_BYTE(wtmp);
        wtmp = Bat_GetSumV(OUT_SUMV_BAK1);
        IntCan_MainMsg[MAIN_MSG13].Msg.data[2] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG13].Msg.data[3] = GET_LOW_BYTE(wtmp);
        
        IntCan_MainMsg[MAIN_MSG13].Msg.data[4] = 0;//Elock_GetTemp(0); //power
        IntCan_MainMsg[MAIN_MSG13].Msg.data[5] = 0;//Elock_GetTemp(1);
        IntCan_MainMsg[MAIN_MSG13].Msg.data[6] = 0;//Elock_GetTemp(2); //power
        IntCan_MainMsg[MAIN_MSG13].Msg.data[7] = 0;//Elock_GetTemp(3);
    }
    // main_msg13 reserve

    else if(4 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage++;
        // main_msg14
        // hardware module status reserve
        IntCan_MainMsg[MAIN_MSG14].Msg.data[0] = 0;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[0] |= (BatSys_GetLiquidLevelSensorSt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[0] |= (BatSys_GetFanSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[0] |= (BatSys_GetHeaterSt() & 0x03) << 2;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[0] |= (BatSys_GetFaultDryContactSt() & 0x03);

        IntCan_MainMsg[MAIN_MSG14].Msg.data[1] = 0;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[1] |= (BatSys_GetHvilSt() & 0x3) << 6;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[1] |= (BatSys_GetMaintainSwitchSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[1] |= (BatSys_GetSmokeDetectorSt() & 0x03) << 2;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[1] |= (BatSys_GetLatchingMechanismSt() & 0x03);

        IntCan_MainMsg[MAIN_MSG14].Msg.data[2] = 0;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[2] |= (BatSys_GetPressureSensorSt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[2] |= (BatSys_GetHumiditySensorSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[2] |= (BatSys_GetManualMaintenanceSwitchSt() & 0x03) << 2 ;
        IntCan_MainMsg[MAIN_MSG14].Msg.data[2] |= (BatSys_GetCollisionSwitchSt() & 0x03);

        IntCan_MainMsg[MAIN_MSG14].Msg.data[3] = 0;
        wtmp = BatSys_GetPreChgTime();  // pre charge time
        IntCan_MainMsg[MAIN_MSG14].Msg.data[4] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG14].Msg.data[5] = GET_LOW_BYTE(wtmp);

        wtmp = BatSys_GetFanSpeed();   // fan speed
        IntCan_MainMsg[MAIN_MSG14].Msg.data[6] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG14].Msg.data[7] = GET_LOW_BYTE(wtmp);

        // main_msg15
        IntCan_MainMsg[MAIN_MSG15].Msg.data[0] = 0;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[0] |= (BmsSys_GetTemperDetectModuleSt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[0] |= (BmsSys_GetIntSumvDetectModueleSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[0] |= (BmsSys_GetExternSumvDetectModuleSt() & 0x03) << 2;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[0] |= BmsSys_GetVoltageDetectModuleSt() & 0x03;

        IntCan_MainMsg[MAIN_MSG15].Msg.data[1] = 0;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[1] |= (BmsSys_GetIsoDetectModuleSt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[1] |= (BmsSys_GetCurrentDetectModuleSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[1] |= (BmsSys_GetChgBalanceModuleSt() & 0x03) << 2 ;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[1] |= (BmsSys_GetDchgBalanceModuleSt() & 0x03);

        IntCan_MainMsg[MAIN_MSG15].Msg.data[2] = 0;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[2] |= (BmsSys_GetClockSt() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG15].Msg.data[2] |= (BmsSys_GetExtFlashSt() & 0x03) << 4; // flash
        //IntCan_MainMsg[MAIN_MSG15].Msg.data[2] |= 0;   // 45db rsv
        IntCan_MainMsg[MAIN_MSG15].Msg.data[2] |= (BmsSys_GetExtEeSt() & 0x03);

        IntCan_MainMsg[MAIN_MSG15].Msg.data[3] = 0; //
        IntCan_MainMsg[MAIN_MSG15].Msg.data[3] = (Bat_GetMainBalanceSt() & 0x03) << 6;


        wtmp = BmsSys_GetBusLoadRate();   // busload
        IntCan_MainMsg[MAIN_MSG15].Msg.data[4] = GET_HIGH_BYTE(wtmp); //power_supply
        IntCan_MainMsg[MAIN_MSG15].Msg.data[5] = GET_LOW_BYTE(wtmp);

        wtmp = BmsSys_GetK30Volt();   // k30 voltage
        IntCan_MainMsg[MAIN_MSG15].Msg.data[6] = GET_HIGH_BYTE(wtmp); //power_supply
        IntCan_MainMsg[MAIN_MSG15].Msg.data[7] = GET_LOW_BYTE(wtmp);

        // main_msg16
        IntCan_MainMsg[MAIN_MSG16].Msg.data[0] = 0;
        IntCan_MainMsg[MAIN_MSG16].Msg.data[0] |= (BmsSys_GetK15St() & 0x03) << 6;
        IntCan_MainMsg[MAIN_MSG16].Msg.data[0] |= (BmsSys_GetOnBDChgSt() & 0x03) << 4;
        IntCan_MainMsg[MAIN_MSG16].Msg.data[0] |= (BmsSys_GetOffBDChgSt() & 0x03) << 2;
        IntCan_MainMsg[MAIN_MSG16].Msg.data[0] |= BmsSys_GetAlarmSt()& 0x03;// reserve

//        IntCan_MainMsg[MAIN_MSG16].Msg.data[1] |= (BmsSys_GetAwakeSt() & 0x03) << 6;

        BmsSys_GetPowerOffTime(data);
        IntCan_MainMsg[MAIN_MSG16].Msg.data[2] = data[0];//BmsSys_GetPowerOffTime(0);  // year
        IntCan_MainMsg[MAIN_MSG16].Msg.data[3] = data[1];//BmsSys_GetPowerOffTime(1);  // month
        IntCan_MainMsg[MAIN_MSG16].Msg.data[4] = data[2];//BmsSys_GetPowerOffTime(2);  // date
        IntCan_MainMsg[MAIN_MSG16].Msg.data[5] = data[3];//BmsSys_GetPowerOffTime(3);  //
        IntCan_MainMsg[MAIN_MSG16].Msg.data[6] = data[4];//BmsSys_GetPowerOffTime(4);
        IntCan_MainMsg[MAIN_MSG16].Msg.data[7] = data[5];//BmsSys_GetPowerOffTime(5);
    
        IntCan_MainMsg[MAIN_MSG17].Msg.data[0] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[1] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[2] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[3] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[4] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[5] = 0;
        IntCan_MainMsg[MAIN_MSG17].Msg.data[6] = 0;
        
        if(LVM_STATE_ON == BmsSys_GetK15St())
        { 
            wtmp = BmsSys_GetK15Volt();
        }
        else if(LVM_STATE_ON == BmsSys_GetOnBDChgSt())
        {
            wtmp = BmsSys_GetOnBDChgVolt();
        }
        else if(LVM_STATE_ON == BmsSys_GetOffBDChgSt())
        {
            wtmp = BmsSys_GetOffBDChgVolt();
        }
        else if(LVM_STATE_ON == BmsSys_GetAlarmSt())
        {
            wtmp = BmsSys_GetAlarmVolt();
        }
        wtmp /= 200; // 0.2v
        
        IntCan_MainMsg[MAIN_MSG17].Msg.data[7] = (uByte)wtmp;
        
        wtmp = Bat_GetSumV(INT_SUMV_BAK);
        IntCan_MainMsg[MAIN_MSG18].Msg.data[0] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG18].Msg.data[1] = GET_LOW_BYTE(wtmp);
        wtmp = Bat_GetSumV(OUT_SUMV_BAK);
        IntCan_MainMsg[MAIN_MSG18].Msg.data[2] = GET_HIGH_BYTE(wtmp); //power
        IntCan_MainMsg[MAIN_MSG18].Msg.data[3] = GET_LOW_BYTE(wtmp);
        IntCan_MainMsg[MAIN_MSG18].Msg.data[4] = 0;//Elock_GetAiVolt();
       /* btmp = Elock_GetHdErr();
        btmp <<= 4;
        btmp |= (Eclock_GetElockSt() << 2);
        btmp |= Eclock_GetMotoSt();  */
        IntCan_MainMsg[MAIN_MSG18].Msg.data[5] = btmp;
        IntCan_MainMsg[MAIN_MSG18].Msg.data[6] = 0;
    }

    // load event message

    // event_msg0 disable send by period
    else if(5 == IntCan_LocalEcuInfo.MsgLoadPage)
    {
        IntCan_LocalEcuInfo.MsgLoadPage = 0;
        IntCan_EventMsg[EVENT_MSG0].SendCtrl.SendEn = FLAG_DISABLE;

        if((Chg_GetChgSt()) &&(FLAG_FAIL == IntCan_PauseSendMsgCtrl.StopFlag))
        {
            IntCan_EventMsg[EVENT_MSG1].SendCtrl.SendEn = FLAG_ENABLE;
            IntCan_EventMsg[EVENT_MSG2].SendCtrl.SendEn = FLAG_ENABLE;
            IntCan_EventMsg[EVENT_MSG3].SendCtrl.SendEn = FLAG_ENABLE;
            IntCan_EventMsg[EVENT_MSG4].SendCtrl.SendEn = FLAG_ENABLE;

            // event_msg1
            IntCan_EventMsg[EVENT_MSG1].Msg.data[0] = 0;

            if(Chg_GetPermitChgExtSt() | Chg_GetPermitChgOnSt())
            {
                btmp = 1;
            }
            else
            {
                btmp = 0;
            }
            IntCan_EventMsg[EVENT_MSG1].Msg.data[0] |= (btmp & 0x03) << 6; // charge permit status

            IntCan_EventMsg[EVENT_MSG1].Msg.data[0] |= (btmp& 0x03) << 4;; // charge signal status

            wtmp = Chg_GetCableCap(); // cable cap
            if(160 ==wtmp) // 16A
            {
                IntCan_EventMsg[EVENT_MSG1].Msg.data[0] |= 1 << 2;
            }
            else if(320 == wtmp) // 32A
            {
                IntCan_EventMsg[EVENT_MSG1].Msg.data[0] |= 2 << 2;
            }

            if(Chg_GetDcLinkSt() | Chg_GetAcLinkSt())
            {
                btmp = 1;
            }
            else
            {
                btmp = 0;
            }
            IntCan_EventMsg[EVENT_MSG1].Msg.data[0] |= btmp; // charge adjion status

            IntCan_EventMsg[EVENT_MSG1].Msg.data[1] = 0;
            IntCan_EventMsg[EVENT_MSG1].Msg.data[1] |= (Chg_GetChgSt() & 0x03) << 6;
            IntCan_EventMsg[EVENT_MSG1].Msg.data[1] |= (Chg_GetChgStage() & 0x07) << 2;
            IntCan_EventMsg[EVENT_MSG1].Msg.data[1] |=  Chg_GetChargerSt() & 0x03;

            llwd = Chg_GetBmsStopReason();    // BMS stop reason
            llwd &= 0x00FFFFFF;
            IntCan_EventMsg[EVENT_MSG1].Msg.data[2] = (uByte)(llwd >> 16);
            IntCan_EventMsg[EVENT_MSG1].Msg.data[3] = (uByte)(llwd >> 8);
            IntCan_EventMsg[EVENT_MSG1].Msg.data[4] = (uByte)(llwd);

            IntCan_EventMsg[EVENT_MSG1].Msg.data[5] = Chg_GetChargerStopReason(); // charger stop reason

            wtmp = Chg_GetChargerFault();
            IntCan_EventMsg[EVENT_MSG1].Msg.data[6] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG1].Msg.data[7] = GET_LOW_BYTE(wtmp);

            //event_msg2
            wtmp = Chg_GetAccumulatedChgTime();  // total charge time
            IntCan_EventMsg[EVENT_MSG2].Msg.data[0] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG2].Msg.data[1] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetRemainChgTime();  // remain charge time
            IntCan_EventMsg[EVENT_MSG2].Msg.data[2] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG2].Msg.data[3] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetPermitMaxSumV(); // max permit sumv
            IntCan_EventMsg[EVENT_MSG2].Msg.data[4] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG2].Msg.data[5] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetPermitMaxCellV(); // max permit cellv
            IntCan_EventMsg[EVENT_MSG2].Msg.data[6] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG2].Msg.data[7] = GET_LOW_BYTE(wtmp);

            // event_msg3
            wtmp = Chg_GetPermitChgCurOn() | Chg_GetPermitChgCurExt();  //
            IntCan_EventMsg[EVENT_MSG3].Msg.data[0] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG3].Msg.data[1] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetPermitMinCurrent();  //
            IntCan_EventMsg[EVENT_MSG3].Msg.data[2] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG3].Msg.data[3] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetDownCurrentStep();
            IntCan_EventMsg[EVENT_MSG3].Msg.data[4] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG3].Msg.data[5] = GET_LOW_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG3].Msg.data[6] = Chg_GetPermitMaxT();

            // Event Msg 4
            wtmp = Chg_GetOutPutVoltage();
            IntCan_EventMsg[EVENT_MSG4].Msg.data[0] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG4].Msg.data[1] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetOutPutCurrent();
            IntCan_EventMsg[EVENT_MSG4].Msg.data[2] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG4].Msg.data[3] = GET_LOW_BYTE(wtmp);

            wtmp = Chg_GetOutPutEnergy();
            IntCan_EventMsg[EVENT_MSG4].Msg.data[4] = GET_HIGH_BYTE(wtmp);
            IntCan_EventMsg[EVENT_MSG4].Msg.data[5] = GET_LOW_BYTE(wtmp);
        }
        else
        {
            IntCan_EventMsg[EVENT_MSG1].SendCtrl.SendEn = FLAG_DISABLE;
            IntCan_EventMsg[EVENT_MSG2].SendCtrl.SendEn = FLAG_DISABLE;
            IntCan_EventMsg[EVENT_MSG3].SendCtrl.SendEn = FLAG_DISABLE;
            IntCan_EventMsg[EVENT_MSG4].SendCtrl.SendEn = FLAG_DISABLE;
        }
    }
    IntCan_BroadCastMsg.Msg.data[0] = 0;
    IntCan_BroadCastMsg.Msg.data[1] = 0;
    IntCan_BroadCastMsg.Msg.data[2] = 0;
    IntCan_BroadCastMsg.Msg.data[3] = 0;
    IntCan_BroadCastMsg.Msg.data[4] = BatSys_GetRunMode();
    IntCan_BroadCastMsg.Msg.data[5] = LvM_GetBcuShutDownEn();
    IntCan_BroadCastMsg.Msg.data[6] = IntCan_LocalEcuInfo.BmuNum;
    IntCan_BroadCastMsg.Msg.data[7] = IntCan_LocalEcuInfo.Life;

    RestoreStatusReg();

}

uWord IntCan_ModifyMainMsgPeriod(eMainMsgNumType index, uWord period)
{
    uWord rslt;
    rslt = INTCAN_ERR;
    if(index < (eMainMsgNumType)INT_CAN_MAIN_INFO_BUFF)
    {
        IntCan_MainMsg[index].PeriodCnt = TM_MS(period);
        rslt = ERR_OK;
    }
    return rslt;
}

void IntCan_ModifyBroacastMsgPeriod(uWord Period)
{
    IntCan_BroadCastMsg.PeriodCnt = TM_MS(Period);
}

uWord IntCan_EnOrDisEventMsg(eMainMsgNumType index, uWord flag)
{
    uWord rslt;
    rslt = INTCAN_ERR;
    if((uWord)index < (uWord)MAX_EVET_MSG)
    {
        IntCan_EventMsg[index].SendCtrl.SendEn = (uByte)flag;
        rslt = ERR_OK;
    }
    return rslt;
}

void IntCan_SendBalCmd(eIntCan_BalCmdType cmd, uByte* ptr, uByte len)
{

    uByte *ptr_by;
    uByte i;
    uWord wtmp;

    CanTpMsgTx.DataLen = 0;
    ptr_by = CanTpMsgTx.Data;
    *(ptr_by++) = (uByte)cmd;
    CanTpMsgTx.DataLen++;

    for(i = 0; i < len; i++)
    {
        *(ptr_by++) = *(ptr++);
        CanTpMsgTx.DataLen++;
    }

    CanTpMsgTx.Id = BCU_BALANCE_MSG_ID;
    (void)CanTp_CopyDataToBuf (INT_CAN_CHANNEL, &CanTpMsgTx);

}

void IntCan_CtrlSlaveRelay(uByte bmu_no, eIntCan_SlaveRelayNoType rly_num, eIntCan_SlaveRelayCtrlType cmd)
{
    uByte rly_num_bak = (uByte)rly_num;
    sCanIf_MsgType msg;
    uWord wtmp, rslt;

    if((bmu_no >= 1) && (bmu_no <= MAX_BMU_NUM))
    {
        switch(rly_num_bak)
        {
        case SLAVE_RELAY1:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly1 = (uByte)cmd;
            break;
        }
        case SLAVE_RELAY2:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly2 = (uByte)cmd;
            break;
        }
        case SLAVE_RELAY3:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly3 = (uByte)cmd;
            break;
        }
        case SLAVE_RELAY4:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly4 = (uByte)cmd;
            break;
        }
        case SLAVE_RELAY5:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly5 = (uByte)cmd;
            break;
        }
        case SLAVE_RELAY6:
        {
            IntCan_SendSlaveRlySt[bmu_no- 1].bit.rly6 = (uByte)cmd;
            break;
        }
        default:
            break;
        }

        wtmp = bmu_no;
        wtmp <<= 8;
        msg.id = BCU_CTRL_BMU_RLY_ID + wtmp;
        msg.length = 8;
        msg.data[0] = (uByte)(IntCan_SendSlaveRlySt[bmu_no- 1].wd >> 8);
        msg.data[1] = (uByte)(IntCan_SendSlaveRlySt[bmu_no- 1].wd >> 8);

        msg.data[2] = 0;
        msg.data[3] = 0;
        msg.data[4] = 0;
        msg.data[5] = 0;
        msg.data[6] = 0;
        msg.data[7] = 0;
        (void)CanIf_RequestSend(INT_CAN_CHANNEL, &msg, 0);
    }
}

static void IntCan_ModifyPeriod(void)
{
    // modify temper msg period

    if(IntCan_MainMsg[MAIN_MSG2].SendCtrl.SendCnt >= 5)
    {
        (void)IntCan_ModifyMainMsgPeriod(MAIN_MSG2,500);
    }

}


uByte IntCan_GetTestModeSt(void)
{
    return IntCan_ModeCtrl.ModeFlag;
}

void IntCan_TestModeExitCheck(void)
{
    IntCan_ModeCtrl.ExitTestModeCnt1++;
    IntCan_ModeCtrl.ExitTestModeCnt2++;
    IntCan_ModeCtrl.ExitTestModeCnt3++;
    IntCan_ModeCtrl.ExitTestModeCnt4++;
    if((IntCan_ModeCtrl.ExitTestModeCnt1 >= 500) || (IntCan_ModeCtrl.ExitTestModeCnt2 >= 500)
     ||(IntCan_ModeCtrl.ExitTestModeCnt3 >= 500) || (IntCan_ModeCtrl.ExitTestModeCnt4 >= 500))
    {
        IntCan_ModeCtrl.ExitTestModeCnt1 = 500;
        IntCan_ModeCtrl.ExitTestModeCnt2 = 500;
        IntCan_ModeCtrl.ExitTestModeCnt3 = 500;
        IntCan_ModeCtrl.ExitTestModeCnt4 = 500;
        IntCan_ModeCtrl.ModeFlag = FLAG_FAIL;
        IntCan_ModeCtrl.AHFrameCnt = 0;
        IntCan_ModeCtrl.VoltCurFrameCnt = 0;
    }
}


void IntCan_CountCbk(void)
{
    (IntCan_LocalEcuInfo.ProcessCnt < 255) ? (IntCan_LocalEcuInfo.ProcessCnt++) : (IntCan_LocalEcuInfo.ProcessCnt = 255);

    (IntCan_LocalEcuInfo.SendMsgCnt < 255) ? (IntCan_LocalEcuInfo.SendMsgCnt++) : (IntCan_LocalEcuInfo.SendMsgCnt = 255);
}

void IntCan_MainProcess(void)
{
    uByte i;

    if(IntCan_LocalEcuInfo.ProcessCnt >= TM_MS(10))
    {
        IntCan_LocalEcuInfo.ProcessCnt = 0;

        IntCan_LoadMsgData();


        IntCan_ModifyPeriod();

      /*  IntCan_AddTestData();
        IntCan_TestModeExitCheck();

        // for check can status
        for(i = 0; i < IntCan_LocalEcuInfo.BmuNum; i++)
        {
            if(gwd_IntCanErrCnt[i] < 65000)
            {
                gwd_IntCanErrCnt[i] += 1;//TM_MS(10);
            }
            else
            {
                gwd_IntCanErrCnt[i] = 65000;
            }
        } */
        
        //IntCan_CanErrUpdateData(); 
    }

    if(IntCan_LocalEcuInfo.SendMsgCnt >= TM_MS(5))
    {
        IntCan_LocalEcuInfo.SendMsgCnt = 0;
        IntCan_MainSendMsg();
    }
}

uWord IntCan_GetSysCell(uByte CellId)
{
    if(CellId <= MAX_PACK_CELL_NUM)
    {  
        return gwd_sys_cellv[CellId];
    }
    else
    {
        return 0xffff; 
    }
}

uByte IntCan_GetSysTemper(uByte TemperId)
{
    if(TemperId <= MAX_PACK_TEMPER_NUM)
    {  
        return gby_sys_Temper[TemperId];
    }
    else
    {
        return 0xff; 
    }
}

#ifdef __DEBUG_INTCAN
static void IntCan_TestData(void)
{
    uByte data[6];
    uByte i;
    volatile uWord wtmp;

    Bat_UpdateSumV(INT_SUMV, 3000);    //sumv
    Bat_UpdateSumV(OUT_SUMV, 3000);   // out_sumv
    Bat_UpdateCurrent(INSCUR, 31020); // InsCur
    Bat_UpdateCurrent(AVGCUR, 32000); // AvgCur

    Bat_UpdateMaxV(3800);             // max_v                                // min_v
    Bat_UpdateMinV(3500);
    Bat_UpdateMaxVTotalNo(280);      // max_v_no
    Bat_UpdateMinVTotalNo(260);      // min_v_no

    Bat_UpdateMaxT(100);
    Bat_UpdateMinT(60);
    Bat_UpdateMaxTTotalNo(256);
    Bat_UpdateMinTTotalNo(280);
    Bat_UpdateSOC(899);
    Bat_UpdateSoh(999);

    // msg3
    Bat_UpdateMaxR(60);
    Bat_UpdateMinR(52);
    Bat_UpdateMaxRTotalNo(280);
    Bat_UpdateMinRTotalNo(300);
    Bat_UpdateTotalIntRes(1280);

    // msg 4
    Bat_UpdateMaxFeedBackCur(3000);
    Bat_UpdateMaxPermitDchCur(4000);
    Bat_UpdateActualCapacityCalc(2000);

    // msg5
    Bat_UpdateTotalChgAh(200000);
    Bat_UpdateDchgTotalAh(90000);

    // msg6
    BatSys_UpdatePosRlySt(1);
    BatSys_UpdatePreRlySt(2);
    BatSys_UpdateNegRlySt(1);
    BatSys_UpdateChgRlySt(1);
    BatSys_UpdateAuxASt(1);
    BatSys_UpdateAuxBSt(1);
    Ctrl_Do(DOIF_CPUDO1_NUM,1);
    Ctrl_Do(DOIF_CPUDO2_NUM,1);

    // msg7
    BmsSys_UpdateIntCanBusSt(1);
    BmsSys_UpdateCarCanBusSt(2);
    BmsSys_UpdateChgCanBusSt(3);

    data[0] = 13;
    data[1] = 11;
    data[2] = 26;
    data[3] = 16;
    data[4] = 25;
    data[5] = 23;
    BmsSys_UpdateSystemTime(data);

    // msg8
    BatSys_UpdateIsoPosResistor(1000);
    BatSys_UpdateIsoNegResistor(2000);
    Bat_UpdatePwr(2000);

    // msg9
    // msg10
    // msg11
    // msg12
    // msg13  // reserve

    // msg14
    BatSys_UpdateLiquidLevelSensorSt(1) ;
    BatSys_UpdateFanSt(1) ;
    BatSys_UpdateHeaterSt(1) ;
    BatSys_UpdateFaultDryContactSt(2) ;
    BatSys_UpdateHighVoltageLockSt(1) ;
    BatSys_UpdateMaintainSwitchSt(1);
    BatSys_UpdateSmokeDetectorSt(1) ;
    BatSys_UpdateLatchingMechanismSt(1);
    BatSys_UpdatePressureSensorSt(1) ;
    BatSys_UpdateHumiditySensorSt(1);
    BatSys_UpdateManualMaintenanceSwitchSt(2) ;
    BatSys_UpdateCollisionSwitchSt(1);
    BatSys_UpdatePreChgTime(1000);
    BatSys_UpdateFanSpeed(500);

    // main_msg15
    BmsSys_UpdateTemperDetectModuleSt(1);
    BmsSys_UpdateIntSumvDetectModueleSt(1) ;
    BmsSys_UpdateExternSumvDetectModuleSt(1) ;
    BmsSys_UpdateVoltageDetectModuleSt(1) ;

    BmsSys_UpdateIsoDetectModuleSt(1) ;
    BmsSys_UpdateCurrentDetectModuleSt(1)  ;
    BmsSys_UpdateChgBalanceModuleSt(1) ;
    BmsSys_UpdateDchgBalanceModuleSt(1) ;

    BmsSys_UpdateClockSt(1) ;
    BmsSys_UpdateExtFlashSt(1) ; // flash
    BmsSys_UpdateExtEeSt(1) ;

    BmsSys_UpdateBusLoadRate(32);   // busload
    BmsSys_UpdateK30Volt(100);   // k30 voltage

    // main_msg 16
    BmsSys_UpdateK15St(1);
    Chg_UpdateAcLinkSt(1);
    Chg_UpdateDcLinkSt(1);

    data[0] = 13;
    data[1] = 11;
    data[2] = 25;
    data[3] = 17;
    data[4] = 8;
    data[5] = 9;
    BmsSys_UpdatePowerOffTime(data);

    //Event main_msg
    // event_msg1
    Chg_UpdatePermitChgExtSt(1);
    Chg_UpdatePermitChgOnSt(1);
    Chg_UpdateCableCap(1);

    Chg_UpdateChgSt(0);
    Chg_UpdateChgStage(3);
    Chg_UpdateChargerSt(1);
    Chg_UpdateBmsStopReason(0x0FFFFFFF);    // BMS stop reason
    Chg_UpdateChargerStopReason(0xFF); // charger stop reason

    Chg_UpdateChargerFault(0xFFFF);

    //event_msg2
    Chg_UpdateAccumulatedChgTime(20);  // total charge time

    Chg_UpdateRemainChgTime(10);  // remain charge time

    Chg_UpdatePermitMaxSumV(3000); // max permit sumv
    Chg_UpdatePermitMaxCellV(3650); // max permit cellv

    // event_msg3
    Chg_UpdatePermitChgCurOn(100);  //
    Chg_UpdatePermitMinCurrent(40);  //

    Chg_UpdateDownCurrentStep(40);
    Chg_UpdatePermitMaxT(100);

    // Event Msg 4
    Chg_UpdateOutPutVoltage(3000);

//    IntCan_BroadCastMsg.SendCtrl.SendEn = FLAG_DISABLE;
//    IntCan_ResMsg.SendCtrl.SendEn =    FLAG_DISABLE;
//    IntCan_TemperMsg.SendCtrl.SendEn = FLAG_DISABLE;
//    IntCan_BalSocMsg.SendCtrl.SendEn = FLAG_DISABLE;

//    IntCan_VoltMsg.SendCtrl.SendEn =   FLAG_DISABLE;


    for(i = 0; i < 140; i++)
    {
        (void)Bat_UpdateCellV(0, i + 1, 3000 + i * 10);
    }

    for(i = 0; i < 10; i++)
    {
        (void)Bat_UpdateTemper(0, i+ 1, 40 +i);
    }

}

void IntCan_InitTest(void)
{
    uByte btmp;
    volatile  uWord wtmp;
    uByte i;

    wtmp = 120;
    //SysConf_UpdataParaToRam(SYSCONF_BMU0_CELLNUM, &wtmp);
    wtmp = 10;
   // SysConf_UpdataParaToRam( SYSCONF_BMU0_START_CELLNO, &wtmp);

    wtmp = 10;
    //SysConf_UpdataParaToRam(SYSCONF_BMU0_TEMPERNUM, &wtmp);

    wtmp = 10;
   // SysConf_UpdataParaToRam( SYSCONF_BMU0_START_TEMPERNO, &wtmp);
}


#endif /* __DEBUG_INTCAN */

uWord IntCan_GetBmuMaxOutLineCnt(void)
{
    uByte i;
    uWord wtmp = gwd_IntCanErrCnt[0];

    for(i = 1; i < IntCan_LocalEcuInfo.BmuNum; i++)
    {
        if(wtmp < gwd_IntCanErrCnt[i])
        {
            wtmp = gwd_IntCanErrCnt[i];
        }
    }

    return wtmp;
}

uWord IntCan_GetBmuOutLineCnt(uByte bmu)
{
    if((bmu < (IntCan_LocalEcuInfo.BmuNum +1)) && (bmu != 0))
    {
        return(gwd_IntCanErrCnt[bmu -1]);
    }
    else
    {
        return 0;
    }
}

static void IntCan_CanErrUpdateData(void)
{
    uByte i,j;
    uWord wtmp,err_cnt;
    uByte btmp;

    for(i = 1; i <= IntCan_LocalEcuInfo.BmuNum; i++)
    {
        err_cnt = gwd_IntCanErrCnt[i - 1] / 2;  
        if(err_cnt >= IntCan_StaticSet.CanErrCnt)
        {
            if(FLAG_FAIL == gby_IntCanErrUpdateDataOnce[i-1])
            {
                gby_IntCanErrUpdateDataOnce[i-1] = FLAG_OK;
                
                wtmp = Bat_GetPackSerialNum(i);
                for(j = 1; j <= wtmp; j++ )
                {
                    Bat_UpdateCellV(i, j, 0xFFFE);
                }

                wtmp = Bat_GetPackTemperNum(i);
                for(j = 1; j <= wtmp; j++ )
                {
                    Bat_UpdateTemper(i, j, 0xFE);
                }
                
                Bat_UpdatePackMaxV(i,0xFFFE);
                Bat_UpdatePackMinV(i,0xFFFE);
                Bat_UpdatePackMaxT(i,0xFE);
                Bat_UpdatePackMinT(i,0xFE);
            }
        }
        else
        {
            gby_IntCanErrUpdateDataOnce[i-1] = FLAG_FAIL;    
        }
    }
}

sCanIf_MsgType IntCan_GetSelfConfRecvMsg0(void)
{
    return IntCan_SelfConfMsg[0];
}

sCanIf_MsgType IntCan_GetSelfConfRecvMsg1(void)
{
    return IntCan_SelfConfMsg[1];
}

sCanIf_MsgType IntCan_GetSelfConfRecvMsg2(void)
{
    return IntCan_SelfConfMsg[2];
}


void IntCan_WrTestFrameToBuff(uByte index,uByte* ptr, uByte en)
{
    uByte i;

    if(1 == index)
    {
        for(i = 0; i < 8; i++)
        {
            IntCan_EventMsg[EVENT_MSG6].Msg.data[i] = *(ptr++);
        }

        IntCan_EventMsg[EVENT_MSG6].SendCtrl.SendEn = en;

    }
    else if(2 == index)
    {
        for(i = 0; i < 8; i++)
        {
            IntCan_EventMsg[EVENT_MSG7].Msg.data[i] = *(ptr++);
        }
        IntCan_EventMsg[EVENT_MSG7].SendCtrl.SendEn = en;
    }

    //
    if(FLAG_OK == IntCan_PauseSendMsgCtrl.StopFlag)
    {
        IntCan_EventMsg[EVENT_MSG6].SendCtrl.SendEn = 0;
        IntCan_EventMsg[EVENT_MSG7].SendCtrl.SendEn = 0;
    }

}


