// BalM.c
#include "BalM-F.h"
#include "SchIf.h"
#include "Ecu.h"
#include "BatterySt.h"
#include "BmsSysSt.h"
#include "BatterySysSt.h"
#include "ChargeSt.h"
#include "ErrorAlarm.h"
#include "EE.h"
#include "IntCan.h"
#include "WdgIf.h"
#include "LvM.h"

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
#include "CellvIf.h"
#include "CellvDet.h"
#include "BalanceIf.h"
#endif

#if ((ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BCU_C))
#include "Hvm.h"
#endif

#ifdef USE_BALM_F

const sVersion_CodeType BalM_VerDef =
{
    MODULE_BALM_NAME,
    BALM_SW_MAJOR_VERSION,
    BALM_SW_MINOR_VERSION,
    BALM_SW_REVISION_VERSION,
    0,
    0,
    BALM_DEVELOP_STATUS
};

/* Version Get Interface of Version Module  */
ModuleID_Type BalM_F_GetVesionModuleVer(sVersion_CodeType * VerPtr)
{
    *VerPtr = BalM_VerDef;
    return MODULE_BALM_ID;
}

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

typedef struct
{
    uByte cnt;
    uByte ReceiveCmdCnt;

} sBalM_TmCntType;

sBalM_TmCntType BalM_TmCnt;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))
typedef struct
{
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))

    uByte  StopBalRslt[MAX_PACK_NUM];
    uByte  CalcBalRslt[MAX_PACK_NUM];
    uByte  StartBalRslt[MAX_PACK_NUM];
    uByte  ForceBalRslt[MAX_PACK_NUM];

#endif

    uByte  StopBalRsltCnt;
    uByte  CalcBalRsltCnt;
    uByte  StartBalRsltCnt;
    uByte  ForceBalRsltCnt;

} sBalM_RxSlaType;

sBalM_RxSlaType BalM_RxSla;
#endif

typedef struct
{
    uByte BatSt;
    uByte BalSt;
    uByte CalcBal;
    uByte StartBal;
    uByte BalCtrl;
    uByte TxBalCmd;
    uByte RxBalCmd;
    uByte UpdateFlag;

    uWord AvgV;
    uWord MinV;
    uWord MaxV;
    uWord DetV;

    uLWord BatCap;
    uWord BalMode;
    uByte BalCmd;
    uByte ForceBal;
    uByte ForceBalOne;
    uByte ForceBalFlag;
    
    uWord ForceBalNo;
    uWord ForceBalTime;
    uLWord SaveFlag1;
    uLWord SaveFlag2;
    uLWord SaveFlag3;

} sBalM_CtrlType;

sBalM_CtrlType BalM_Ctrl;

typedef struct
{
    uWord TimeCnt10s;
    uWord TimeCnt5Min;

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)

    uWord TotalDchBalCnt[MAX_PACK_NUM][MAX_PACK_CELL_NUM];
    uWord TotalChgBalCnt[MAX_PACK_NUM][MAX_PACK_CELL_NUM];

#endif

    uWord AfterChgCnt;
    uWord VoltHighCnt;
    uWord StartBalCnt;
    uWord CalcBalCnt;

    uWord UpdateBalStCnt;
    uWord VoltBalCnt;
    uWord StopBalCnt;

    uWord DetCurStbCnt;
    uWord ChgLast2MinCnt;
    uWord ChgEndLast2HourCnt;
    uWord StopBal10SCnt;
    uWord StartBal2MinCnt;
    uWord BalTimePeriod;
} sBalM_BalCntType;

sBalM_BalCntType BalM_BalCnt;
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
typedef struct
{    
    uWord Volt_High_Bal_L2;  
    uWord Volt_High_Bal_L1;
    uWord Volt_Low_Bal_L1;
    uWord Volt_Low_Bal_L2;
    uWord Bal_Min_Volt_L1;
    uWord Bal_Min_Volt_L2;
    uWord Bal_Delta_Volt;
    uWord Rsv1;
    uWord Rsv2;
    uWord Rsv3;
    uWord Rsv4;
    uWord Rsv5;
} sBalM_VoltParaType;
sBalM_VoltParaType BalM_VoltPara;

typedef struct
{    
    uWord Volt_End_Bal[11];  
} sBalM_VoltEndType;
sBalM_VoltEndType BalM_VoltEnd;
typedef struct
{    
    uByte Calc_Bal_Mode;  
} sBalM_CalcModelType;
sBalM_CalcModelType BalM_CalcModel;
#endif                                                        
#pragma DATA_SEG DEFAULT

// balance control time, 200ms per count
#define BALM_TIME_2S      10
#define BALM_TIME_5S      25
#define BALM_TIME_10S     50
#define BALM_TIME_20S     100
#define BALM_TIME_25S     125
#define BALM_TIME_1MIN    300
#define BALM_TIME_2MIN    600
#define BALM_TIME_3MIN    900
#define BALM_TIME_5MIN    1500
#define BALM_TIME_10MIN   3000
#define BALM_TIME_120MIN  36000


#define BALTIMECNT_2S     1

// balance time count, 10s per count
#define BALTIMECNT_5MIN    30
#define BALTIMECNT_3HOUR   1080
#define BALTIMECNT_5HOUR   1800
#define BALTIMECNT_7HOUR   2520
#define BALTIMECNT_10HOUR  3600
#define BALTIMECNT_12HOUR  4320
#define BALTIMECNT_90HOUR  32400
#define BALTIMECNT_120HOUR 43200


// force balance mask
#define FORCE_BAL_MASK  0x0fff

// balance control flag
#define MASK_CAL_BAL 		  0x01
#define MASK_START_BAL	  0x02
#define MASK_STOP_BAL		  0x04
#define MASK_FORCE_BAL    0x08
#define MASK_CLEAR_BAL    0x10
#define MASK_FORCE_ONE_BAL 0x20

#define ALL_BAL_CLEAR  0xFE
#define ALL_BAL_OPEN   0xFA
#define ONE_BAL_OPEN   0xFC

#define BALM_FLAG_DISABLE 0
#define BALM_FLAG_ENABLE  1
#define BALM_FLAG_FAIL    0
#define BALM_FLAG_OK      1

// balance judge result
#define NEED_BAL      0x01
#define ALL_BAT_LOW   0x02
#define ALL_BAT_HIGH  0x04
#define DONT_NEED_BAL 0x08

// balance on off status
#define ST_BAL_OFF    0x00
#define ST_BAL_ON     0x01

// chg and dch balance status
#define DCH_BAL_OFF   0x00
#define DCH_BAL_ON    0x20
#define DCH_BAL_ERR   0x08
#define CHG_BAL_OFF   0x00
#define CHG_BAL_ON    0x10
#define CHG_BAL_ERR   0x04

/* define balance control commad for LTC6803 */
#define CMD_DCH_BAL_OFF   0x10
#define CMD_DCH_BAL_ON    0x11

#define CMD_CHG_BAL_OFF   0x20
#define CMD_CHG_BAL_ON    0x21

// battery status
#define BALM_BAT_NOT_CHG      0x00
#define BALM_BAT_CHG_NOW      0x01
#define BALM_BAT_CHG_END      0x02
#define BALM_BAT_AFTER_CHG    0x03
#define BALM_BAT_DCH_NOW      0x04
#define BALM_BAT_STANDBY      0x05

#define DET_CUR_5A  50
#define CHG_CUR_5A  (OFFSET_CUR - 50)
#define CHG_CUR_2A  (OFFSET_CUR - 20)

#define DCH_CUR_5A  (OFFSET_CUR + 50) 
#define DCH_CUR_10A (OFFSET_CUR + 100) 

static void BalM_ProcBalTime(void);
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))
static void BalM_SendBalCmd(void);
static void BalM_SendCmd_CalcBal(void);
static void BalM_SendCmd_StartBal(void);
static void BalM_SendCmd_StopBal(void);
static void BalM_SendCmd_ForceBal(void);
#endif
static void BalM_ReceiveBalCmd(void);
static void BalM_ProcBal(void);
static uByte BalM_CalcBal(void);
static void BalM_StartBal(void);
static void BalM_StopBal(void);
static void BalM_ForceBal(void);
static void BalM_ClearBal(void);
static uWord BalM_GetCellSoc(uWord volt);
static uWord BalM_SaveBalTime(void);

void BalM_ModuleInit(void)
{
    volatile uWord i;
    GeneralReturnType rslt;
    uByte * RAMPTR p_data;
    uByte bmu_no;
    uByte dlen;
    uByte data[4] ={0,0,0,0 };
    uWord bat_num;
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))  
    uByte cell_num_per_chip[MAX_CELLV_IC_MODULE_NUM], DeviceNum;
#endif
    BalM_TmCnt.cnt = 0;
    BalM_TmCnt.ReceiveCmdCnt = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))        
  
     // get device num from config
    (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);

    DeviceNum = 0;
    for(i = 0; i < MAX_CELLV_IC_MODULE_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            DeviceNum++;
        }
    }
    rslt = 0;
   
    rslt = BalanceIf_InitSpChgDchBalance(DeviceNum);
 
    
#endif    

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))

    p_data = (uByte* RAMPTR)(&BalM_RxSla);

    for(i = 0; i < sizeof(BalM_RxSla); i++)
    {
        *(p_data++) = 0;
    }

#endif

    p_data = (uByte* RAMPTR)(&BalM_Ctrl);

    for(i = 0; i < sizeof(BalM_Ctrl); i++)
    {
        *(p_data++) = 0;
    }

    p_data = (uByte* RAMPTR)(&BalM_BalCnt);

    for(i = 0; i < sizeof(BalM_BalCnt); i++)
    {
        *(p_data++) = 0;
    }
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))    
    p_data = (uByte* RAMPTR)(&BalM_VoltPara);
    for(i = 0; i < sizeof(BalM_VoltPara); i++)
    {
        *(p_data++) = 0;
    }
    p_data = (uByte* RAMPTR)(&BalM_VoltEnd);
    for(i = 0; i < sizeof(BalM_VoltEnd); i++)
    {
        *(p_data++) = 0;
    }
#endif

    /* init remian balance time */
#if (ECU_CATEGORY == M_BCU_C)
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_1_15);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_16_30);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_31_45);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_46_60);
    WdgIf_Trig();
    #if(MAX_CELL_NUM > 60)
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_61_75);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_76_90);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_91_105);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_106_120);
    WdgIf_Trig();
    #endif

#if(MAX_CELL_NUM > 120)
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_121_135);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_136_150);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_151_165);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_166_180);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 180)
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_181_195);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_196_210);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_211_225);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_226_240);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 240)
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_241_255);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_256_270);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_271_285);
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_286_300);
    WdgIf_Trig();
#endif
#endif

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_1_15);
    if(ERR_OK != rslt)
    {
        rslt = EE_LoadVar(SYSTEMST_BAL_TIME_BAK_1_15);
    }
    WdgIf_Trig();
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_16_30);
    if(ERR_OK != rslt)
    {
        rslt = EE_LoadVar(SYSTEMST_BAL_TIME_BAK_16_30);
    }
    WdgIf_Trig();
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_31_45);
    if(ERR_OK != rslt)
    {
        rslt = EE_LoadVar(SYSTEMST_BAL_TIME_BAK_31_45);
    }
    WdgIf_Trig();  
    rslt = EE_LoadVar(SYSTEMST_BAL_TIME_46_60);
    if(ERR_OK != rslt)
    {
        rslt = EE_LoadVar(SYSTEMST_BAL_TIME_BAK_46_60);
    }
    WdgIf_Trig();
#endif
    /* end of init remian balance time */

    /* init balance flag */
#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    bmu_no = 0;
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
#endif

#if (ECU_CATEGORY == M_BCU_C)
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, &data[0], &dlen);
#endif

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_VOLT_PARA, &data[0], &dlen);
#endif
    bat_num = data[1] + (data[0] << 8);

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
#if(ECU_CATEGORY == M_BCU_C)        
     BalM_VoltPara.Volt_High_Bal_L2 = BalM_ConfigPara.Volt_High_Bal_L2; 
     BalM_VoltPara.Volt_High_Bal_L1 = BalM_ConfigPara.Volt_High_Bal_L1;
     BalM_VoltPara.Volt_Low_Bal_L1 = BalM_ConfigPara.Volt_Low_Bal_L1;
     BalM_VoltPara.Volt_Low_Bal_L2 = BalM_ConfigPara.Volt_Low_Bal_L2;
     BalM_VoltPara.Bal_Min_Volt_L1 = BalM_ConfigPara.Bal_Min_Volt_L1;
     BalM_VoltPara.Bal_Min_Volt_L2 = BalM_ConfigPara.Bal_Min_Volt_L2;
     BalM_VoltPara.Bal_Delta_Volt = BalM_ConfigPara.Bal_Delta_Volt;
     for(i = 0; i < 11; i++)
     {          
        BalM_VoltEnd.Volt_End_Bal[i] = Soc_ChgEndVolt[i].Volt;
     }
#else      
    rslt = SysConf_InitParaTable(BALM_MANAGE_PARA1, (uLWord)(&BalM_VoltPara), sizeof(BalM_VoltPara));
    rslt = SysConf_InitParaTable(BALM_MANAGE_PARA2, (uLWord)(&BalM_VoltEnd), sizeof(BalM_VoltEnd));
    rslt = ERR_OK;
    rslt |= EE_LoadVar(BALM_MANAGE_PARA1);
    rslt |= EE_LoadVar(BALM_MANAGE_PARA2);
    if(ERR_OK != rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
#endif
    rslt = SysConf_InitParaTable(BALM_CALC_BAL_MODE, (uLWord)(&BalM_CalcModel), sizeof(BalM_CalcModel)); //calc_bal_mode
    //rslt = EE_LoadVar(BALM_CALC_BAL_MODE);
    
    for(i = 0; i < bat_num; i++)
    {
        if((Bat_GetCellBalanceTimeRemain(i + 1) & 0x7FFF) > BALTIMECNT_90HOUR)       //<10h
        {
            (void)Bat_UpdateCellBalanceTimeRemain(i + 1, 0);
        }

        //if(Bat_GetCellBalanceTimeRemain(i + 1) != 0)
        //{
        //    (void)Bat_UpdateCellBalanceSt(bmu_no, i + 1, DCH_BAL_ON);
        //}
    }
#endif
    /* end of init balance flag */

    /* init total balance time */
#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_1_20);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_21_40);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_41_60);
    WdgIf_Trig();
    #if(MAX_CELL_NUM > 60)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_61_80);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_81_100);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_101_120);
    WdgIf_Trig();
    #endif

    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_1_20);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_21_40);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_41_60);
    WdgIf_Trig();
    #if(MAX_CELL_NUM > 60)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_61_80);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_81_100);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_101_120);
    WdgIf_Trig();
    #endif

#if(MAX_CELL_NUM > 120)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_121_140);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_141_160);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_161_180);
    WdgIf_Trig();
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_121_140);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_141_160);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_161_180);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 180)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_181_200);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_201_220);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_221_240);
    WdgIf_Trig();
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_181_200);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_201_220);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_221_240);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 240)
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_241_260);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_261_280);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_CHG_BAL_TIME_281_300);
    WdgIf_Trig();
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_241_260);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_261_280);
    rslt = EE_LoadVar(SYSTEMST_TOTAL_DCH_BAL_TIME_281_300);
    WdgIf_Trig();
#endif
#endif
    /* end of init total balance time */

    BalM_Ctrl.BalSt = ST_BAL_OFF;
   // BalM_Ctrl.StartBal = BALM_FLAG_ENABLE;
    BalM_Ctrl.BalCtrl = BALM_FLAG_ENABLE;
    (void) rslt;
}

void BalM_ModuleCountCbk(void)
{
    (BalM_TmCnt.cnt < 255) ? (BalM_TmCnt.cnt++) : (BalM_TmCnt.cnt = 255);
    (BalM_TmCnt.ReceiveCmdCnt < 255) ? (BalM_TmCnt.ReceiveCmdCnt++) : (BalM_TmCnt.ReceiveCmdCnt = 255);
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
    BalanceIf_DelayCountCbk();
#endif
}

void BalM_ModuleCbk(void)
{
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
    BalanceIf_BalanceCbk();
#endif    
    if(BalM_TmCnt.cnt >= TM_MS(200))
    {
        BalM_TmCnt.cnt = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))
        BalM_SendBalCmd();
#endif


#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
        BalM_ProcBal();
#endif

        BalM_ProcBalTime();        
    }
    
#if ( (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))    
     if(BalM_TmCnt.ReceiveCmdCnt >= TM_MS(100))
     {
        BalM_TmCnt.ReceiveCmdCnt =  0;
        BalM_ReceiveBalCmd();
     }
 #endif    
    
}

static void BalM_ProcBalTime(void)
{
    GeneralReturnType rslt;
    uByte i;
    uByte data[2], dlen;
    uByte bmu_num;
    uByte bmu_no;
    uWord bat_num;
    uWord j;
    uWord k;
    uLWord lwtmp;
    uWord wtmp;

    BalM_BalCnt.TimeCnt10s++;
    BalM_BalCnt.TimeCnt5Min++;

#if ((ECU_CATEGORY == M_BCU_C)|| (ECU_CATEGORY == M_BCU_D))
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);

    // need modify bat_num
    bat_num = Bat_GetBatNum();
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_VOLT_PARA, &data[0], &dlen);
    bat_num = data[1] + (data[0] << 8);
    bmu_num = 1;
#endif
    if((BalM_Ctrl.ForceBal == ST_BAL_ON) && (BalM_Ctrl.ForceBalOne == ST_BAL_OFF))
    {
        BalM_BalCnt.BalTimePeriod = BALM_TIME_2S;
    }
    else
    {
        BalM_BalCnt.BalTimePeriod = BALM_TIME_10S; 
    }

    if(BalM_BalCnt.TimeCnt10s >=  BalM_BalCnt.BalTimePeriod)
    {
        BalM_BalCnt.TimeCnt10s = 0;

        /* calc single balance time and remain balance time below */
        k = 0;
        lwtmp = 1;

#if (ECU_CATEGORY == M_BCU_C)
        for(i = 0; i <= bmu_num; i++)
#else
        for(i = 1; i <= bmu_num; i++)
#endif
        {
            if(ST_BAL_ON == Bat_GetPackBalSt(i))
            {
                for(j = 0; j < bat_num; j++)
                {
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
#ifdef BMS_ENABLE_SINGLE_BAL_TIME
                    if(DCH_BAL_ON == Bat_GetCellBalanceSt(i, j + 1)) // cell is discharge balance now
                    {
                        (void)Bat_UpdateCellDchBalanceTimeSingle(k + 1, (Bat_GetCellDchBalanceTimeSingle(k + 1) + 1));
                    }
                    else if(CHG_BAL_ON == Bat_GetCellBalanceSt(i, j + 1))  // cell is charge balance now
                    {
                        (void)Bat_UpdateCellChgBalanceTimeSingle(k + 1, (Bat_GetCellChgBalanceTimeSingle(k + 1) + 1));
                    }
#endif
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
                    wtmp = Bat_GetCellBalanceTimeRemain(k + 1); 
                    if(0 == (wtmp & 0x8000))
                    {
                        if((wtmp > 0) && (DCH_BAL_ON == Bat_GetCellBalanceSt(i, j + 1)))
                        {
                            (void)Bat_UpdateCellBalanceTimeRemain(k + 1, (wtmp - 1));
                            if((BalM_Ctrl.ForceBal == ST_BAL_OFF) ||(BalM_Ctrl.ForceBalOne == ST_BAL_ON))
                            {
                              BalM_Ctrl.SaveFlag1 |= lwtmp << (k / 15);
                            }

                            if(0 == (Bat_GetCellBalanceTimeRemain(k + 1) & 0x7fff))
                            {
                                BalM_Ctrl.UpdateFlag = BALM_FLAG_ENABLE;
                                
                            }
                        }
                    }
                    else
                    {
                        wtmp &= 0x7fff;
                        // need add charge balance time conrol
                        if((wtmp > 0) && (CHG_BAL_ON == Bat_GetCellBalanceSt(i, j + 1))) 
                        {
                            (void)Bat_UpdateCellBalanceTimeRemain(k + 1, (Bat_GetCellBalanceTimeRemain(k + 1) - 1));
                            if((BalM_Ctrl.ForceBal == ST_BAL_OFF) ||(BalM_Ctrl.ForceBalOne == ST_BAL_ON))
                            {
                                BalM_Ctrl.SaveFlag1 |= lwtmp << (k / 15);
                            }

                            if(0 == (Bat_GetCellBalanceTimeRemain(k + 1) & 0x7fff))
                            {
                                BalM_Ctrl.UpdateFlag = BALM_FLAG_ENABLE;
                                (void)Bat_UpdateCellBalanceSt(i, (j + 1), CHG_BAL_OFF);                                                           
                                BalM_StopBal();
                            }
                        }
                    }
#endif
                    k++;
                }
            }
        }
        /* end of calc single balance time and remain balance time */
    }

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    /* calc total balance time below */
    k = 0;
    lwtmp = 1;

#if (ECU_CATEGORY == M_BCU_C)
    for(i = 0; i <= bmu_num; i++)
#else
    for(i = 1; i <= bmu_num; i++)
#endif
    {
        if(ST_BAL_ON == Bat_GetPackBalSt(i))
        {
            for(j = 0; j < bat_num; j++)
            {
                if(DCH_BAL_ON == Bat_GetCellBalanceSt(i , j + 1)) // cell is discharge balance now
                {
                    BalM_BalCnt.TotalDchBalCnt[i][j]++;

                    if(BalM_BalCnt.TotalDchBalCnt[i][j] > BALM_TIME_10MIN)
                    {
                        BalM_BalCnt.TotalDchBalCnt[i][j] = 0;
                        (void)Bat_UpdateCellDchBalanceTimeTotal(k + 1, (Bat_GetCellDchBalanceTimeTotal(k + 1) + 1));
                        BalM_Ctrl.SaveFlag2 |= lwtmp << (uByte)(k / 20);
                    }
                }
                else if(CHG_BAL_ON == Bat_GetCellBalanceSt(i, j + 1))  // cell ECU_CATEGORY == M_BCU_Cs charge balance now
                {
                    BalM_BalCnt.TotalChgBalCnt[i][j]++;

                    if(BalM_BalCnt.TotalChgBalCnt[i][j] > BALM_TIME_10MIN)
                    {
                        BalM_BalCnt.TotalChgBalCnt[i][j] = 0;
                        (void)Bat_UpdateCellChgBalanceTimeTotal(k + 1, (Bat_GetCellChgBalanceTimeTotal(k + 1) + 1));
                        BalM_Ctrl.SaveFlag3 |= lwtmp << (uByte)(k / 20);
                    }
                }
                k++;
            }
        }
    }
    /* end of calc total balance time */
#endif

    /* save balance time to EE every 5min */
    if( (BalM_BalCnt.TimeCnt5Min >= BALM_TIME_5MIN)
            || (LVM_STATE_OFF == BmsSys_GetAwakeSt()) )
    {
        BalM_BalCnt.TimeCnt5Min = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
        /* save total balance time to EE */
        lwtmp =1;
        for(i = 0; i < 15; i++)
        {
            if(0 != ((BalM_Ctrl.SaveFlag2 >> i) & 0x01))
            {
                wtmp = i + (uWord)SYSTEMST_TOTAL_DCH_BAL_TIME_1_20;
                rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
                BalM_Ctrl.SaveFlag2 &= (~(lwtmp << i));
            }

            if(0 != ((BalM_Ctrl.SaveFlag3 >> i) & 0x01))
            {
                wtmp = i + (uWord)SYSTEMST_TOTAL_CHG_BAL_TIME_1_20;
                rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
                BalM_Ctrl.SaveFlag3 &= (~(lwtmp << i));
            }
        }
        /* end of save total balance time to EE */
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
        /* save remain balance time to EE */
        lwtmp =1;
        for(i = 0; i < 20; i++)
        {
            if(0 != ((BalM_Ctrl.SaveFlag1 >> i) & 0x01))
            {
                wtmp = i + (uWord)SYSTEMST_BAL_TIME_1_15;
                rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
                wtmp = i + (uWord)SYSTEMST_BAL_TIME_BAK_1_15;
                rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
#endif
                BalM_Ctrl.SaveFlag1 &= (~(lwtmp << i));
                WdgIf_Trig();
            }
        }
        /* end of save remain balance time to EE */
#endif
    }
    /* end of save balance time to EE */
    (void) rslt;
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))
// BCU-C or BCU-D or BMU-R control balance below
static void BalM_SendBalCmd(void)
{
    uByte i;
    uByte btmp;
    uWord wtmp;
    uByte dlen;
    uByte bmu_no;
    uByte bmu_num;
    uByte chg_st;
    uWord current;
    static uWord current_bak = OFFSET_CUR;
    uWord det_cur;
    uWord bms_hd_err;
    GeneralReturnType rslt;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);
    bmu_no = 0;
#else
    bmu_num = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
#endif

    // get bms hardware error code
    bms_hd_err = (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_CUR);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_CELLV);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_CAN4);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_EE_CPU);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_EE_EXT);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_BAL_CHG);

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) )
    BalM_Ctrl.MaxV = Bat_GetMaxV();
    BalM_Ctrl.MinV = Bat_GetMinV();
    BalM_Ctrl.AvgV = Bat_GetAvgV();
    BalM_Ctrl.DetV = Bat_GetDeltaV();
    BalM_Ctrl.BatCap = Bat_GetRatedCapacityCalc();
    current = Bat_GetCurrent(AVGCUR);
#endif
#if ( (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BMU) )
    BalM_Ctrl.MaxV = Bat_GetPackMaxV(bmu_no);
    BalM_Ctrl.MinV = Bat_GetPackMinV(bmu_no);
    BalM_Ctrl.AvgV = Bat_GetPackAvgV(bmu_no);
    BalM_Ctrl.DetV = Bat_GetPackDeltaV(bmu_no);
    BalM_Ctrl.BatCap = Bat_GetPackRatedCapacity(bmu_no); 
    current = Bat_GetPackCurrent(bmu_no , INSCUR);     
#endif
    chg_st = Chg_GetChgSt();

    // judge battery status of "delta chg cur per 10s below 5A"
    if( (current_bak <= OFFSET_CUR) && (current <= OFFSET_CUR) )
    {
        if(current >= current_bak)
        {
            det_cur = current - current_bak;
        }
        else
        {
            det_cur = current_bak - current;
        }

        if(det_cur < DET_CUR_5A)
        {
            if(BalM_BalCnt.DetCurStbCnt < BALM_TIME_10S)
            {
                BalM_BalCnt.DetCurStbCnt++ ;
            }
        }
        else
        {
            BalM_BalCnt.DetCurStbCnt = 0;
            current_bak = current;          //reback up current
        }
    }

    // judge battery status of "charge now last at least 2min"
    if(BALM_BAT_CHG_NOW == BalM_Ctrl.BatSt)
    {
        if(BalM_BalCnt.ChgLast2MinCnt < BALM_TIME_2MIN)
        {
            BalM_BalCnt.ChgLast2MinCnt++;
        }
    }
    else
    {
        BalM_BalCnt.ChgLast2MinCnt=0;
    }

    // calc balance off last time
    if(ST_BAL_OFF == BalM_Ctrl.BalSt)
    {
        if(BalM_BalCnt.StopBal10SCnt < BALM_TIME_10S)
        {
            BalM_BalCnt.StopBal10SCnt++;
        }
    }

    // calc balance start last time
    if(ST_BAL_ON == BalM_Ctrl.BalSt)
    {
        if(BalM_BalCnt.StartBal2MinCnt < BALM_TIME_1MIN)
        {
            BalM_BalCnt.StartBal2MinCnt++;
        }
    }
    
    // calc after charge last time
    if(BALM_BAT_AFTER_CHG == BalM_Ctrl.BatSt) 
    {
        if(BalM_BalCnt.ChgEndLast2HourCnt < BALM_TIME_120MIN)
        {
            BalM_BalCnt.ChgEndLast2HourCnt++;
        }
    }

    // judge battery status of "charge now"
    if( (CHG_ST_ON == chg_st) && (current <= CHG_CUR_2A)
     && (BALM_BAT_CHG_END != BalM_Ctrl.BatSt) 
     && (BALM_BAT_AFTER_CHG != BalM_Ctrl.BatSt) )
    {
        BalM_Ctrl.BatSt = BALM_BAT_CHG_NOW;
        BalM_BalCnt.AfterChgCnt = 0;
    }

    // judge battery status of "after charge"
    if((BALM_BAT_CHG_NOW == BalM_Ctrl.BatSt) || (BALM_BAT_CHG_END == BalM_Ctrl.BatSt))
    {
        if((current > CHG_CUR_2A) && (current <= DCH_CUR_10A))
        {
            if(BalM_BalCnt.AfterChgCnt > BALM_TIME_1MIN)
            {
                BalM_Ctrl.BatSt = BALM_BAT_AFTER_CHG;
            }
            else
            {
                BalM_BalCnt.AfterChgCnt++;
            }
        }
    }

    // judge battery status of "no charge"
    if(current > DCH_CUR_10A)
    {
        BalM_Ctrl.BatSt = BALM_BAT_NOT_CHG;
        BalM_BalCnt.AfterChgCnt = 0;
    }

    // check max volt
    /*if( (BalM_Ctrl.MaxV > BalM_ConfigPara.Max_v_Calc_Bal_Thd)
            && (current <= (OFFSET_CUR + (BalM_Ctrl.BatCap * 35 / 1000)))  // current <= 0.35C
            && (current >= (OFFSET_CUR - (BalM_Ctrl.BatCap * 35 / 1000))) )
    {
        if(BalM_BalCnt.VoltHighCnt < BALM_TIME_1MIN)
        {
            BalM_BalCnt.VoltHighCnt++;
        }
    }
    else
    {
        BalM_BalCnt.VoltHighCnt = 0;
    }

    // calc balance every 3min
    if(BalM_BalCnt.CalcBalCnt < BALM_TIME_3MIN)
    {
        BalM_BalCnt.CalcBalCnt++;
    } */

    // calc balance when charge now and after charge
    if( ((BalM_BalCnt.DetCurStbCnt >= BALM_TIME_10S)
          && (BalM_BalCnt.ChgLast2MinCnt >= BALM_TIME_2MIN)
          && (BalM_BalCnt.StopBal10SCnt >= BALM_TIME_10S) && (CHG_STAGE_CC_MODE == Chg_GetChgStage())))
    /* || ((BALM_BAT_AFTER_CHG == BalM_Ctrl.BatSt)
          && (BalM_BalCnt.DetCurStbCnt >= BALM_TIME_10S)
          && (BalM_Ctrl.DetV >= BalM_ConfigPara.Bal_Delta_Volt)
          && (BalM_BalCnt.StopBal10SCnt >= BALM_TIME_10S))) */
    {
        BalM_BalCnt.StopBal10SCnt = 0;
        
        BalM_Ctrl.CalcBal = BALM_FLAG_ENABLE;
        BalM_Ctrl.StartBal = BALM_FLAG_ENABLE;

        // clear flag
        BalM_RxSla.StopBalRsltCnt = 0;
        BalM_RxSla.CalcBalRsltCnt = 0;
        BalM_RxSla.StartBalRsltCnt = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
        for(i = 0; i < bmu_num; i++)
        {
            BalM_RxSla.StopBalRslt[i] = 0;
            BalM_RxSla.CalcBalRslt[i] = 0;
            BalM_RxSla.StartBalRslt[i] = 0;
        }
#endif
    }

    // calc balance when charge end
    //if( (CHG_ST_END == chg_st) && (BALM_FLAG_ENABLE == BalM_Ctrl.BalCtrl))
    if((CHG_STAGE_READY_TO_END == Chg_GetChgStage()) && (CHG_ST_ON == Chg_GetChgSt())
            && (BALM_FLAG_ENABLE == BalM_Ctrl.BalCtrl))
    {
        // judge battery status of "charge end"
        BalM_Ctrl.BatSt = BALM_BAT_CHG_END;
        
        BalM_BalCnt.AfterChgCnt = 0;
        BalM_BalCnt.StopBal10SCnt = 0;
        
        BalM_Ctrl.CalcBal = BALM_FLAG_ENABLE;
        BalM_Ctrl.BalCtrl = BALM_FLAG_DISABLE;

        // clear flag
        BalM_RxSla.StopBalRsltCnt = 0;
        BalM_RxSla.CalcBalRsltCnt = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
        for(i = 0; i < bmu_num; i++)
        {
            BalM_RxSla.StopBalRslt[i] = 0;
            BalM_RxSla.CalcBalRslt[i] = 0;
        }
#endif
    }

    // calc balance
    if((BALM_FLAG_ENABLE == BalM_Ctrl.CalcBal) && (0 == bms_hd_err) && (BalM_Ctrl.MinV >= BalM_ConfigPara.Min_v_Calc_Bal_Thd))
    {
        BalM_Ctrl.CalcBal = BALM_FLAG_DISABLE;
        BalM_Ctrl.TxBalCmd |= MASK_CAL_BAL;
        BalM_Ctrl.RxBalCmd &= (~MASK_CAL_BAL);
        BalM_Ctrl.BalSt = ST_BAL_OFF;
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
        Bat_UpdateMainBalanceSt(ST_BAL_OFF);
#endif
    }

    // start balance
    if( (BALM_FLAG_ENABLE == BalM_Ctrl.StartBal)
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_CAL_BAL))
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_FORCE_BAL))
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_STOP_BAL))
            && (BalM_Ctrl.MinV >= BalM_ConfigPara.Min_v_Start_Bal_Thd)
            && (BalM_BalCnt.DetCurStbCnt >= BALM_TIME_10S)
            && (0 == bms_hd_err) )
    {
        if(BalM_BalCnt.StartBalCnt >= BALM_TIME_5S)
        {
            BalM_BalCnt.StartBalCnt = 0;
            BalM_Ctrl.StartBal = BALM_FLAG_DISABLE;

            BalM_Ctrl.TxBalCmd |= MASK_START_BAL;
            BalM_Ctrl.RxBalCmd &= (~MASK_START_BAL);
#if (ECU_CATEGORY == M_BCU_D)  
            BalM_Ctrl.BalSt = ST_BAL_ON;
#endif
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))          
            Bat_UpdateMainBalanceSt(ST_BAL_ON);
#endif
        }
        else
        {
            BalM_BalCnt.StartBalCnt++;
        }
    }
    else
    {
        BalM_BalCnt.StartBalCnt = 0;
    }

    // stop balance
    if( (0 != bms_hd_err) 
     || (BalM_Ctrl.MaxV > BalM_ConfigPara.Min_v_Stop_Bal_Thd)
     || (BALM_BAT_NOT_CHG == BalM_Ctrl.BatSt)
     || (BalM_BalCnt.StartBal2MinCnt >= BALM_TIME_1MIN) 
     || (BalM_BalCnt.ChgEndLast2HourCnt >= BALM_TIME_120MIN) 
     || (BALM_BAT_AFTER_CHG == BalM_Ctrl.BatSt))
    {
        BalM_Ctrl.TxBalCmd &= (~MASK_START_BAL);
        BalM_Ctrl.TxBalCmd &= (~MASK_CAL_BAL);
        BalM_Ctrl.TxBalCmd &= (~MASK_FORCE_BAL);
        
        BalM_BalCnt.StartBal2MinCnt = 0;

        if(ST_BAL_ON == BalM_Ctrl.BalSt)
        {
            BalM_Ctrl.TxBalCmd |= MASK_STOP_BAL;
            BalM_Ctrl.RxBalCmd &= (~MASK_STOP_BAL);

            BalM_Ctrl.BalSt = ST_BAL_OFF;
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))            
            Bat_UpdateMainBalanceSt(ST_BAL_OFF);
#endif
        }
    }

    // re enable send start balance cmd
    /*if( (BALM_FLAG_ENABLE == BalM_Ctrl.BalCtrl)
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_CAL_BAL))
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_STOP_BAL))
            && (0 == (BalM_Ctrl.TxBalCmd & MASK_FORCE_BAL))
            && (0 == bms_hd_err)
            && (ST_BAL_OFF == BalM_Ctrl.BalSt)
     && (BalM_Ctrl.MinV >= BalM_ConfigPara.Min_v_Start_Bal_Thd) )
    {
        BalM_Ctrl.StartBal = BALM_FLAG_ENABLE;
    } */

    // receive bmu ack of bal cmd
    BalM_RxSla.CalcBalRsltCnt = 0;
    BalM_RxSla.StartBalRsltCnt = 0;
    BalM_RxSla.StopBalRsltCnt = 0;
    BalM_RxSla.ForceBalRsltCnt = 0;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    for(i = 0; i < bmu_num; i++)
    {
        if(0 != BalM_RxSla.CalcBalRslt[i])
        {
            BalM_RxSla.CalcBalRsltCnt++;
        }

        if(0 != BalM_RxSla.StartBalRslt[i])
        {
            BalM_RxSla.StartBalRsltCnt++;
        }

        if(0 != BalM_RxSla.StopBalRslt[i])
        {
            BalM_RxSla.StopBalRsltCnt++;
        }

        if(0 != BalM_RxSla.ForceBalRslt[i])
        {
            BalM_RxSla.ForceBalRsltCnt++;
        }
    }
#endif 

    if((BalM_Ctrl.RxBalCmd & MASK_FORCE_BAL) != MASK_FORCE_BAL)
    {
        btmp = BALM_FLAG_OK;

        // all bmu has ack
        if(BalM_RxSla.ForceBalRsltCnt >= bmu_num)
        {
            BalM_RxSla.ForceBalRsltCnt = 0;

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
            for(i = 0; i < bmu_num; i++)
            {
                btmp &= BalM_RxSla.ForceBalRslt[i];
            }

            // stop send cmd of 'force bal'
            if((BALM_FLAG_OK == btmp)  && (bmu_num != 0))
            {
                BalM_Ctrl.RxBalCmd |= MASK_FORCE_BAL;
                BalM_Ctrl.TxBalCmd &= (~MASK_FORCE_BAL);

                for(i = 0; i < bmu_num; i++)
                {
                    BalM_RxSla.ForceBalRslt[i] = 0;
                }
            }
#else
            BalM_Ctrl.RxBalCmd |= MASK_FORCE_BAL;
            //BalM_Ctrl.TxBalCmd &= (~MASK_FORCE_BAL);
#endif
        }
    }

    if((BalM_Ctrl.RxBalCmd & MASK_CAL_BAL) != MASK_CAL_BAL)
    {
        btmp = BALM_FLAG_OK;

        // all bmu has ack
        if(BalM_RxSla.CalcBalRsltCnt >= bmu_num)
        {
            BalM_RxSla.CalcBalRsltCnt = 0;

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
            for(i = 0; i < bmu_num; i++)
            {
                if(0 == BalM_RxSla.CalcBalRslt[i])
                {
                    btmp = 0;
                }
            }

            // stop send cmd of 'calc bal'
            if((btmp != 0) && (bmu_num != 0))
            {
                BalM_Ctrl.RxBalCmd |= MASK_CAL_BAL;
                BalM_Ctrl.TxBalCmd &= (~MASK_CAL_BAL);

                for(i = 0; i < bmu_num; i++)
                {
                    BalM_RxSla.CalcBalRslt[i] = 0;
                }
            }
#else
            BalM_Ctrl.RxBalCmd |= MASK_CAL_BAL;
            //BalM_Ctrl.TxBalCmd &= (~MASK_CAL_BAL);
#endif
        }
    }

    if((BalM_Ctrl.RxBalCmd & MASK_START_BAL) != MASK_START_BAL)
    {
        btmp = BALM_FLAG_OK;

        // all bmu has ack
        if(BalM_RxSla.StartBalRsltCnt >= bmu_num)
        {
            BalM_RxSla.StartBalRsltCnt = 0;

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
            for(i = 0; i < bmu_num; i++)
            {
                btmp &= BalM_RxSla.StartBalRslt[i];
            }

            // stop send cmd of 'start bal'
            if((BALM_FLAG_OK == btmp)  && (bmu_num != 0))
            {
                BalM_Ctrl.RxBalCmd |= MASK_START_BAL;
                BalM_Ctrl.TxBalCmd &= (~MASK_START_BAL);

                for(i = 0; i < bmu_num; i++)
                {
                    BalM_RxSla.StartBalRslt[i] = 0;
                }
            }
#else
            BalM_Ctrl.RxBalCmd |= MASK_START_BAL;
            //BalM_Ctrl.TxBalCmd &= (~MASK_START_BAL);
#endif
        }
    }

    if((BalM_Ctrl.RxBalCmd & MASK_STOP_BAL) != MASK_STOP_BAL)
    {
        btmp = BALM_FLAG_OK;

        // all bmu has ack
        if(BalM_RxSla.StopBalRsltCnt >= bmu_num)
        {
            BalM_RxSla.StopBalRsltCnt = 0;

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
            for(i = 0; i < bmu_num; i++)
            {
                btmp &= BalM_RxSla.StopBalRslt[i];
            }

            if((BALM_FLAG_OK == btmp)  && (bmu_num != 0))
            {
                // stop send cmd of 'stop bal'
                BalM_Ctrl.RxBalCmd |= MASK_STOP_BAL;
                BalM_Ctrl.TxBalCmd &= (~MASK_STOP_BAL);

                for(i = 0; i < bmu_num; i++)
                {
                    BalM_RxSla.StopBalRslt[i] = 0;
                }
            }
#else
            BalM_Ctrl.RxBalCmd |= MASK_STOP_BAL;
            //BalM_Ctrl.TxBalCmd &= (~MASK_STOP_BAL);
#endif
        }
    }
    // end of receive bmu ack of bal cmd


    // send balance command, need modify
    if(BalM_Ctrl.TxBalCmd & MASK_FORCE_BAL)
    {
        BalM_SendCmd_ForceBal();
#if ((ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
        if(bmu_num == 0)
        {
           BalM_Ctrl.TxBalCmd &= (~MASK_FORCE_BAL);
        }
#endif   
    }
    else if(BalM_Ctrl.TxBalCmd & MASK_CAL_BAL)
    {
        BalM_SendCmd_CalcBal();
#if ((ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
       if(bmu_num == 0)
       {
           BalM_Ctrl.TxBalCmd &= (~MASK_CAL_BAL);
       }
#endif        
    }
    else if(BalM_Ctrl.TxBalCmd & MASK_START_BAL)
    {
        BalM_SendCmd_StartBal();
#if ((ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
        if(bmu_num == 0)
        {
           BalM_Ctrl.TxBalCmd &= (~MASK_START_BAL);
        }
#endif        
    }
    else if(BalM_Ctrl.TxBalCmd & MASK_STOP_BAL)
    {
        BalM_SendCmd_StopBal();
#if ((ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C)) 
        if(bmu_num == 0)
        {
           BalM_Ctrl.TxBalCmd &= (~MASK_STOP_BAL);
        }
#endif        
    }
    else
    {
        // nop
    }
    // end of send balance command
}
// end of BCU-C or BCU-D or BMU-R control balance
#endif // #if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))  


#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))

static void BalM_SendCmd_CalcBal(void)
{
    uWord rslt;
    uWord wtmp;
    uByte msg_data[10];
    uByte bmu_num;
    uByte dlen;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU_R))
    // direct control
    BalM_Ctrl.BalMode = BalM_ConfigPara.BalMode;
    (void)BalM_CalcBal();
#endif

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    if(0 != bmu_num)
    {
        // use can send msg
        msg_data[0] = BalM_Ctrl.BatSt;
        // battery average volt
        wtmp = BalM_Ctrl.AvgV; // 0.001V unit
        msg_data[1] = ((wtmp & 0xFF00) >> 8);
        msg_data[2] = (wtmp & 0x00FF);
        // battery min volt
        wtmp = BalM_Ctrl.MinV; // 0.001V unit
        msg_data[3] = ((wtmp & 0xFF00) >> 8);
        msg_data[4] = (wtmp & 0x00FF);
        // battery max volt
        wtmp = BalM_Ctrl.MaxV; // 0.001V unit
        msg_data[5] = ((wtmp & 0xFF00) >> 8);
        msg_data[6] = (wtmp & 0x00FF);
        // battery capacity
        wtmp = (uWord)(BalM_Ctrl.BatCap / 100); // 0.1AH unit
        msg_data[7] = ((wtmp & 0xFF00) >> 8);
        msg_data[8] = (wtmp & 0x00FF);
        msg_data[9] = BalM_ConfigPara.BalMode;//BalM_Ctrl.BalMode;

        IntCan_SendBalCmd(CAL_BAL, &msg_data[0], 10);
    }
#endif
}

static void BalM_SendCmd_StartBal(void)
{
    uWord rslt;
    uByte msg_data[2];
    uByte bmu_num;
    uByte dlen;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU_R))
    // direct control
    if(ST_BAL_OFF == BalM_Ctrl.BalSt)
    {
       BalM_StartBal();
    }
#endif

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    if(0 != bmu_num)
    {
        // use can send msg
        msg_data[0] = 1;
        IntCan_SendBalCmd(START_BAL, &msg_data[0], 1);
    }
#endif
}

static void BalM_SendCmd_StopBal(void)
{
    uWord rslt;
    uByte msg_data[2];
    uByte bmu_num;
    uByte dlen;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU_R))
    // direct control
    if(ST_BAL_ON != BalM_Ctrl.ForceBal) 
    {
        BalM_StopBal();
    }
#endif

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    if(0 != bmu_num)
    {
        // use can send msg
        msg_data[0] = 1;
        IntCan_SendBalCmd(STOP_BAL, &msg_data[0], 1);
    }
#endif
}

static void BalM_SendCmd_ForceBal(void)
{
    uWord rslt;
    uByte msg_data[2];
    uByte bmu_num;
    uByte dlen;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, &bmu_num, &dlen);
#endif

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU_R))
    // direct control
  
    BalM_ClearBal();
#endif

#if (ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D)
    if(0 != bmu_num)
    {
        // use can send msg
        msg_data[0] = 0xFE;
        IntCan_SendBalCmd(FORCE_BAL, &msg_data[0], 1);
    }
#endif
}

#endif // #if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BMU_R))  


//************************************************************/
// Function:
// Trans para:
//************************************************************/
void BalM_UpdateDataFromCan(uByte bmu_no, uByte cmd, uByte *p_data, uByte len)
{
    uWord wtmp;

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))

    if((bmu_no > 0) && (bmu_no <= MAX_PACK_NUM))
    {
        if((uByte)CAL_BAL == cmd)
        {
            BalM_RxSla.CalcBalRslt[bmu_no- 1] = *p_data;
        }
        else if((uByte)START_BAL == cmd)
        {
            BalM_RxSla.StartBalRslt[bmu_no- 1] = *p_data;
        }
        else if((uByte)STOP_BAL == cmd)
        {
            BalM_RxSla.StopBalRslt[bmu_no- 1] = *p_data;
        }
        else if((uByte)FORCE_BAL == cmd)
        {
            BalM_RxSla.ForceBalRslt[bmu_no- 1] = *p_data;
        }
        else
        {
        }
    }
#endif

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
    if((uByte)CAL_BAL == cmd)
    {
        BalM_Ctrl.BatSt = *p_data++;

        wtmp = *p_data++;
        wtmp = (wtmp << 8) + (*p_data++);
        BalM_Ctrl.AvgV =  wtmp; //0.001V unit

        wtmp = *p_data++;
        wtmp = (wtmp << 8) + (*p_data++);
        BalM_Ctrl.MinV =wtmp;   //0.001V unit

        wtmp = *p_data++;
        wtmp = (wtmp << 8) + (*p_data++);
        BalM_Ctrl.MaxV = wtmp;  //0.001V unit

        wtmp = *p_data++;
        wtmp = (wtmp << 8) + (*p_data++);
        BalM_Ctrl.BatCap = wtmp;  //0.1AH unit
        BalM_Ctrl.BatCap *= 100;  //0.001AH unit

        BalM_Ctrl.BalMode = *p_data++;
        BalM_Ctrl.BalCmd |= MASK_CAL_BAL;
    }
    else if((uByte)START_BAL == cmd)
    {
        BalM_Ctrl.BalCmd |= MASK_START_BAL;
    }
    else if((uByte)STOP_BAL == cmd)
    {
        BalM_Ctrl.BalCmd |= MASK_STOP_BAL;
    }
    else if((uByte)FORCE_BAL == cmd)
    {
        if(ALL_BAL_CLEAR == (*p_data))
        {
           BalM_Ctrl.BalCmd |= MASK_CLEAR_BAL;
        }
        else if(ALL_BAL_OPEN == (*p_data))
        {          
        BalM_Ctrl.BalCmd |= MASK_FORCE_BAL;
        }
        else if(ONE_BAL_OPEN == (*p_data))
        {
           BalM_Ctrl.BalCmd |= MASK_FORCE_ONE_BAL;
           wtmp = *p_data++;
           wtmp = *p_data++;
           wtmp = (wtmp << 8) + (*p_data++);         
           BalM_Ctrl.ForceBalNo = wtmp;
           wtmp = *p_data++;
           wtmp = (wtmp << 8) + (*p_data++);         
           BalM_Ctrl.ForceBalTime = wtmp;   
        }
        else
        {
            BalM_Ctrl.BalCmd = 0;
        }
    }
    else
    {
        BalM_Ctrl.BalCmd = 0;
    }
#endif
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static void BalM_ReceiveBalCmd(void)
{
    uByte bmu_no, dlen;
    uWord i,j;    
    uByte msg_data[2];
    GeneralReturnType rslt;
    uByte channel;
    uWord cell_no, all_cell_num,cell_no_acc;
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C)) 
    uByte cell_num_per_chip[MAX_VOLT_PACK_NUM];
    uWord wtmp;
#endif    
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C)) 
  
    #if(ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    #else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    #endif
    
    if(ST_BAL_ON == BalM_Ctrl.ForceBalFlag)
    {  
        BalM_Ctrl.ForceBalFlag = ST_BAL_OFF;    
        BalM_StartBal();  
    } 

    if(BalM_Ctrl.BalCmd & MASK_CAL_BAL)
    {
        msg_data[1] = BALM_FLAG_OK;
        msg_data[0] = (uByte)CAL_BAL;
        BalM_Ctrl.BalCmd &= (~MASK_CAL_BAL);
        IntCan_SendBalCmd(CAL_BAL, &msg_data[0], 2);
        if(ST_BAL_ON != BalM_Ctrl.ForceBal) 
        {
          msg_data[1] = BalM_CalcBal();
        }
    }
    else if(BalM_Ctrl.BalCmd & MASK_START_BAL)
    {
       if((ST_BAL_ON != BalM_Ctrl.ForceBal) && (BalM_Ctrl.BalSt == ST_BAL_OFF))
       {
          BalM_StartBal();
       }
        BalM_Ctrl.BalCmd &= (~MASK_START_BAL);
        msg_data[0] = (uByte)START_BAL;
        msg_data[1] = BALM_FLAG_OK;
        IntCan_SendBalCmd(START_BAL, &msg_data[0], 2);
    }
    else if(BalM_Ctrl.BalCmd & MASK_STOP_BAL)
    {
        if(ST_BAL_ON != BalM_Ctrl.ForceBal) 
        {
            BalM_StopBal();
        }
        BalM_Ctrl.BalCmd &= (~MASK_STOP_BAL);
        msg_data[0] = (uByte)STOP_BAL;
        msg_data[1] = BALM_FLAG_OK;
        IntCan_SendBalCmd(STOP_BAL, &msg_data[0], 2);
    }
    else if(BalM_Ctrl.BalCmd & MASK_FORCE_BAL)
    {
        BalM_StopBal();
        BalM_ForceBal();
        // BalM_StartBal();
        BalM_Ctrl.ForceBalFlag = ST_BAL_ON; 
        BalM_Ctrl.ForceBalOne  = ST_BAL_OFF;
        BalM_Ctrl.BalCmd &= (~MASK_FORCE_BAL);
        msg_data[0] = (uByte)FORCE_BAL;
        msg_data[1] = BALM_FLAG_OK;
        IntCan_SendBalCmd(FORCE_BAL, &msg_data[0], 2);
    }
    else if(BalM_Ctrl.BalCmd & MASK_CLEAR_BAL)
    {
        BalM_ClearBal();
        BalM_StopBal();
        BalM_Ctrl.BalCmd &= (~MASK_CLEAR_BAL);
        msg_data[0] = (uByte)FORCE_BAL;
        msg_data[1] = BALM_FLAG_OK;
        IntCan_SendBalCmd(FORCE_BAL, &msg_data[0], 2);
    }
    else if(BalM_Ctrl.BalCmd & MASK_FORCE_ONE_BAL)
    { 
   #if(ECU_CATEGORY == M_BCU_C)
       bmu_no = 0;
    #else
       rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    #endif
        rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);

        j = 0;
        cell_no =0;
        channel = 0;
        all_cell_num = 0;
        cell_no_acc = 0;
        
        for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
        {
            all_cell_num += cell_num_per_chip[i];
        }

        for(i = 0; i < all_cell_num; i++)
        { 
            if(cell_no >= cell_num_per_chip[channel])
            {               
                cell_no_acc += cell_num_per_chip[channel];
                channel++;  
                cell_no = 0; 
            } 
             
            if(BalM_Ctrl.ForceBalNo <= (cell_no_acc + cell_num_per_chip[channel]))
            {
               for(j = cell_no_acc; j < (cell_no_acc + cell_num_per_chip[channel]); j++)
               {
                   (void)Bat_UpdateCellBalanceSt(bmu_no, j + 1, CHG_BAL_OFF);
               }
               break;
            } 
         
            cell_no++;   
        }
        
        BalM_StopBal();
        BalM_Ctrl.BalCmd &= (~MASK_FORCE_ONE_BAL);
        msg_data[0] = (uByte)FORCE_BAL;
        msg_data[1] = BALM_FLAG_OK;
        IntCan_SendBalCmd(FORCE_BAL, &msg_data[0], 2);
        
        if(0 != (BalM_Ctrl.ForceBalTime & 0x7fff))
        {
            (void)Bat_UpdateCellBalanceTimeRemain(BalM_Ctrl.ForceBalNo, BalM_Ctrl.ForceBalTime);
            
            if(0 == (BalM_Ctrl.ForceBalTime & 0x8000))
            {
                (void)Bat_UpdateCellBalanceSt(bmu_no, BalM_Ctrl.ForceBalNo, DCH_BAL_ON);
            }
            else
            {
                (void)Bat_UpdateCellBalanceSt(bmu_no, BalM_Ctrl.ForceBalNo, CHG_BAL_ON);
            }
            
            i = BalM_Ctrl.ForceBalNo / 15;
            wtmp = i + (uWord)SYSTEMST_BAL_TIME_1_15;
            rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
            wtmp = i + (uWord)SYSTEMST_BAL_TIME_BAK_1_15;
            rslt = EE_SaveVar(BOTHEE, (eSysConf_ParameterNOType)wtmp);
#endif
            BalM_Ctrl.ForceBal = ST_BAL_ON; 
            BalM_Ctrl.ForceBalOne = ST_BAL_ON;
            BalM_Ctrl.ForceBalFlag = ST_BAL_ON;          
            //BalM_StartBal();
        }
    }        
    else
    {
        BalM_Ctrl.BalCmd = 0;
    }
#endif
    (void) rslt;
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R))
// BCU-C or BMU or BMU-R control balance bolow
static void BalM_ProcBal(void)
{
    uWord i, j;
    uByte bmu_no;
    uByte dlen;
    uByte data[4];
    uWord bat_num;
    uWord maxv;
    uWord bms_hd_err;
    GeneralReturnType rslt;
    uByte flg;
    uByte chip_num;
    uByte cell_num_per_chip[20];
    uByte bal_loc_mark_chip = 1;
    uByte local_cell_no;
    uWord wtmp;

    // get bms hardware error code
    bms_hd_err = (uWord)Err_GetHwErrLevel(ERR_HW_DETECT_CELLV);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_CAN4);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_EE_CPU);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_EE_EXT);
    bms_hd_err = (bms_hd_err << 2) + (uWord)Err_GetHwErrLevel(ERR_HW_BAL_CHG);

#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, &data[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_VOLT_PARA, &data[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#endif
    bat_num = data[1] + (data[0] << 8);
    
    //find the chip num
    chip_num = 0;
    for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            chip_num++;
        }
    }

    maxv = Bat_GetPackMaxV(bmu_no);
    
    if(BALM_FLAG_ENABLE == BalM_Ctrl.UpdateFlag)
    {
        BalM_Ctrl.UpdateFlag = BALM_FLAG_DISABLE;
        
        if(ST_BAL_ON == BalM_Ctrl.ForceBal) 
        {
            // find the balance flag every chip
            flg = 0;
            local_cell_no = 0;
            for (i = 0; i < chip_num; i++)
            {
                wtmp = 0;
                // find the max remain balance time every chip
                for(j = 0; j < cell_num_per_chip[i]; j++)
                {
                    local_cell_no++;
                    if(Bat_GetCellBalanceTimeRemain(local_cell_no) > wtmp)
                    {
                        wtmp = Bat_GetCellBalanceTimeRemain(local_cell_no);
                        bal_loc_mark_chip = local_cell_no;
                    }
                }
                
                if((wtmp & 0x7fff) > 0)
                {
                    (void)Bat_UpdateCellBalanceSt(bmu_no, bal_loc_mark_chip, CHG_BAL_ON);
                    flg = 1;
                }
            } 
            
            if(1 == flg)
            {
                BalM_StartBal();
            }
            else
            {
                BalM_Ctrl.ForceBal = ST_BAL_OFF;
                BalM_Ctrl.ForceBalOne = ST_BAL_OFF;
            }
        }
    }
    
    if(ST_BAL_ON == Bat_GetPackBalSt(bmu_no))
    {
        // stop balance when voltage low and hardware error
        if ((maxv > BalM_VoltPara.Volt_High_Bal_L2) || (0 != bms_hd_err)) 
        {
            if(BalM_BalCnt.StopBalCnt >= BALM_TIME_25S)
            {
                BalM_BalCnt.StopBalCnt = BALM_TIME_25S;

                BalM_StopBal();
            }
            else
            {
                BalM_BalCnt.StopBalCnt++;
            }
        }
        else
        {
            BalM_BalCnt.StopBalCnt = 0;
          
        }
    }
    (void) rslt;
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static uByte BalM_CalcBal(void)
{
    uWord i, j;
    GeneralReturnType rslt;
    uByte bmu_no;
    uByte dlen;
    uByte bat_high_num;
    uByte calc_bal_rslt;
    uByte data[4];
    uWord bat_num;
    uWord wtmp;
    uLWord lwtmp;
    uWord soc, max_soc;
    uByte chip_num;
    uByte cell_num_per_chip[20];
    uByte bal_loc_mark_chip = 1;
    uByte local_cell_no;

    bat_high_num = 0;
    calc_bal_rslt = 0;

#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, &data[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_VOLT_PARA, &data[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#endif
    bat_num = data[1] + (data[0] << 8);

    //find the chip num
    chip_num = 0;
    for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            chip_num++;
        }
    }

    // clear balance flag
    for (i = 0; i < MAX_PACK_CELL_NUM; i++)
    {
        (void)Bat_UpdateCellBalanceSt(bmu_no, i + 1, CHG_BAL_OFF);
    }
    
    if(ST_BAL_ON != BalM_Ctrl.ForceBal) 
    {
        BalM_StopBal();
    }

    if(BALM_BAT_CHG_END == BalM_Ctrl.BatSt)
    {
        calc_bal_rslt = DONT_NEED_BAL;
        
        // calc each cell's SOC and charge balance time below, unit 0.1%
        max_soc = BalM_GetCellSoc(BalM_Ctrl.MaxV);
        
        for (i = 0; i < bat_num; i++)
        {
            wtmp = Bat_GetCellV(bmu_no , i + 1);
            soc = BalM_GetCellSoc(wtmp);
            
            if( (wtmp >= BalM_VoltPara.Bal_Min_Volt_L1) && (max_soc >= (soc + 15)) )
            {
                // calc charge balance time for each cell
                lwtmp = max_soc - soc - 15;
                lwtmp *= BalM_Ctrl.BatCap;
                lwtmp = ((uLWord)lwtmp * 36) / ((uLWord)BAL_CUR * 100); // unit 10 sec
                if(lwtmp > BALTIMECNT_90HOUR)
                {
                    lwtmp = BALTIMECNT_90HOUR;
                }
                lwtmp |= 0x8000; // high bit is 1 when charge balance time
                (void)Bat_UpdateCellBalanceTimeRemain(i + 1, (uWord)lwtmp);
                calc_bal_rslt = NEED_BAL;
            }
            else
            {
                // don't need balance
                lwtmp = 0; // default value, need modify
                (void)Bat_UpdateCellBalanceTimeRemain(i + 1, (uWord)lwtmp);
            }
        } 
        
        // find the balance flag every chip
        local_cell_no = 0;
        for (i = 0; i < chip_num; i++)
        {
            wtmp = 0;
            
            // find the max remain balance time every chip
            for(j = 0; j < cell_num_per_chip[i]; j++)
            {
                local_cell_no++;
                if(Bat_GetCellBalanceTimeRemain(local_cell_no) > wtmp)
                {
                    wtmp = Bat_GetCellBalanceTimeRemain(local_cell_no);
                    bal_loc_mark_chip = local_cell_no;
                }
            }
            
            if((wtmp & 0x7fff) > 0)
            {
                (void)Bat_UpdateCellBalanceSt(bmu_no, bal_loc_mark_chip, CHG_BAL_ON);
            }
        } 
        
        (void)BalM_SaveBalTime();    
    }
    else if( (BALM_BAT_CHG_NOW == BalM_Ctrl.BatSt) || (BALM_BAT_AFTER_CHG == BalM_Ctrl.BatSt) )
    {
        calc_bal_rslt = DONT_NEED_BAL;
        
        if(BalM_Ctrl.MaxV >= BalM_VoltPara.Volt_High_Bal_L1) // max_v >= 3.4V
        {
            // find the balance flag every chip by cell volt
            local_cell_no = 0;
            for (i = 0; i < chip_num; i++)
            {
                wtmp = 0xffff;
                
                // find the min volt battery every chip
                for(j = 0; j < cell_num_per_chip[i]; j++)
                {
                    local_cell_no++;
                    if(Bat_GetCellV(bmu_no , local_cell_no) < wtmp)
                    {
                        wtmp = Bat_GetCellV(bmu_no , local_cell_no);
                        bal_loc_mark_chip = local_cell_no;
                    }
                }
                
                if(BalM_Ctrl.MaxV > (Bat_GetCellV(bmu_no , bal_loc_mark_chip) + BalM_VoltPara.Bal_Delta_Volt))
                {
                    (void)Bat_UpdateCellBalanceSt(bmu_no, bal_loc_mark_chip, CHG_BAL_ON);
                    calc_bal_rslt = NEED_BAL;
                }
            }
        }
        else
        {
            // find the balance flag every chip by remain balance time
            local_cell_no = 0;
            for (i = 0; i < chip_num; i++)
            {
                wtmp = 0;
                
                // find the max remain balance time every chip
                for(j = 0; j < cell_num_per_chip[i]; j++)
                {
                    local_cell_no++;
                    if(Bat_GetCellBalanceTimeRemain(local_cell_no) > wtmp)
                    {
                        wtmp = Bat_GetCellBalanceTimeRemain(local_cell_no);
                        bal_loc_mark_chip = local_cell_no;
                    }
                }
                
                if((wtmp & 0x7fff) > 0)
                {
                    (void)Bat_UpdateCellBalanceSt(bmu_no, bal_loc_mark_chip, CHG_BAL_ON);
                    calc_bal_rslt = NEED_BAL;
                }
            } 
        }
    }
    else
    {
        calc_bal_rslt = DONT_NEED_BAL;
    }
    
    (void) rslt;
    return calc_bal_rslt;
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static void BalM_StartBal(void)
{
    uByte i, j;
    uByte bmu_no;
    uByte channel;
    uByte dlen;
    uByte cell_no;
    uByte cell_num_per_chip[20];
    uByte jumperwire_pos[MAX_JUMPER_WIRE_NUM];
    uByte all_cell_num;
    uByte offset;
    GeneralReturnType rslt;

#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU0_FUSE_POS, &jumperwire_pos[0], &dlen);
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_FUSE_POS, &jumperwire_pos[0], &dlen);
#endif

    all_cell_num = 0;
    for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
    {
        all_cell_num += cell_num_per_chip[i];
    }

    for(i = 0; i < MAX_JUMPER_WIRE_NUM; i++)
    {
        channel = jumperwire_pos[i] >> 4;
        if((0 < channel) && (channel <= MAX_VOLT_PACK_NUM))
        {
            cell_num_per_chip[channel - 1]++;

            if(cell_num_per_chip[channel - 1] > MAX_VOLTNUMPERPACK)
            {
                return; // config error
            }
        }
    }
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);

    cell_no = 0; //
    offset = 0;
    channel = 0;
    j = 0;
    for(i = 0; i < all_cell_num; i++)
    {
        if(cell_no < cell_num_per_chip[channel])
        {
            /* check jumper wire */
            if(j < MAX_JUMPER_WIRE_NUM)
            {
                if( (channel + 1) == (jumperwire_pos[j] >> 4)
                        && ((cell_no + 1 + offset) == (jumperwire_pos[j] & 0x0f)) )
                {
                    offset++;
                    j++;
                }
            }

            if(CHG_BAL_ON == Bat_GetCellBalanceSt(bmu_no, i + 1))
            {
                //CellvDet_BackupVolt((eCellvIf_DeviceIdType)channel, (cell_no + offset));
                if(ERR_OK == BalanceIf_CtrlChgDchBalance((eBalanceIdType)channel, (cell_no + 1 + offset), BAL_MODE_CHARGE))
                {
                   CellvDet_BackupVolt((eCellvIf_DeviceIdType)channel, (cell_no + offset));
                }
            }
            else
            {
                //(void)BalanceIf_CtrlChgDchBalance((eBalanceIdType)channel, (cell_no + 1 + offset), BAL_MODE_NO_ACTIVE);
            }
        }
        else
        {
            channel++;
            cell_no = 0;
            offset = 0;

            /* check jumper wire */
            if(j < MAX_JUMPER_WIRE_NUM)
            {
                if( (channel + 1) == (jumperwire_pos[j] >> 4)
                        && ((cell_no + 1) == (jumperwire_pos[j] & 0x0f)) )
                {
                    offset++;
                    j++;
                }
            }

            if(CHG_BAL_ON == Bat_GetCellBalanceSt(bmu_no, i + 1))
            {
                //CellvDet_BackupVolt((eCellvIf_DeviceIdType)channel, (cell_no + offset));
                if(ERR_OK == BalanceIf_CtrlChgDchBalance((eBalanceIdType)channel, (cell_no + 1 + offset), BAL_MODE_CHARGE))
                {
                    CellvDet_BackupVolt((eCellvIf_DeviceIdType)channel, (cell_no + offset));
                }
            }
            else
            {
                //(void)BalanceIf_CtrlChgDchBalance((eBalanceIdType)channel, (cell_no + 1 + offset), BAL_MODE_NO_ACTIVE);
            }
        }
        cell_no++;
    }

    BalM_Ctrl.BalSt = ST_BAL_ON;
    (void)Bat_UpdatePackBalSt(bmu_no, ST_BAL_ON);
    (void) rslt;
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static void BalM_StopBal(void)
{
    uByte i, j;
    uByte dlen;
    uByte bmu_no;
    uByte cell_num_per_chip[20];
    uByte chip_num;
    GeneralReturnType rslt;

#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#else
    bmu_no = 1;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#endif

    chip_num = 0;
    for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            chip_num++;
        }
    }
    for (i = 0; i < chip_num; i++)
    {
       // (void)BalanceIf_CtrlChgDchBalance((eBalanceIdType)i, 0, BAL_MODE_NO_ACTIVE);
       (void)BalanceIf_CloseBalance((eBalanceIdType)i);
    }
    
    CellvDet_ClearBackupVolt();
    
    BalM_Ctrl.BalSt = ST_BAL_OFF;
    (void)Bat_UpdatePackBalSt(bmu_no, ST_BAL_OFF);

    (void) rslt;    
}

//************************************************************/
// Function:
// Trans para:
//************************************************************/
static void BalM_ClearBal(void)
{
    uByte i;
    GeneralReturnType rslt;
    uByte bmu_no;
    uByte dlen;
#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
#endif    
    for (i = 0; i < MAX_PACK_CELL_NUM; i++)
    {
        (void)Bat_UpdateCellBalanceTimeRemain(i + 1, 0);
        (void)Bat_UpdateCellBalanceSt(bmu_no, i + 1, CHG_BAL_OFF);
    }
    rslt = BalM_SaveBalTime();
    BalM_Ctrl.ForceBal = ST_BAL_OFF;
    BalM_Ctrl.ForceBalOne = ST_BAL_OFF;
    (void) rslt;
}

static void BalM_ForceBal(void)
{
    uByte i, j, k;
    uByte dlen;
    uByte bmu_no;
    uByte cell_num_per_chip[20];
    GeneralReturnType rslt;
    uByte chip_num;
    uWord bal_time;

#if (ECU_CATEGORY == M_BCU_C)
    bmu_no = 0;
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#else
    rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);
#endif

    //find the chip num
    chip_num = 0;
    for(i = 0; i < MAX_VOLT_PACK_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            chip_num++;
        }
    }
    
    for (i = 0; i < MAX_PACK_CELL_NUM; i++)
    {
        (void)Bat_UpdateCellBalanceSt(bmu_no, i + 1, CHG_BAL_OFF);
    }

    bal_time = BALTIMECNT_2S + 0x8000;
    k = 1;
    for (i = 0; i < chip_num; i++)
    {
        (void)Bat_UpdateCellBalanceSt(bmu_no, k, CHG_BAL_ON); // the first battery of chip balance on
        
        
        for(j = 0; j < cell_num_per_chip[i]; j++)
        {
            (void)Bat_UpdateCellBalanceTimeRemain(k, bal_time);  
            k++;
        }
    }
    
    //BalM_Ctrl.BalSt = ST_BAL_ON;
    //(void)Bat_UpdatePackBalSt(bmu_no, ST_BAL_ON);
    BalM_Ctrl.ForceBal = ST_BAL_ON;
  //  rslt = BalM_SaveBalTime();
	  (void) rslt;
}
//************************************************************/
// Function:
// Trans para:
//************************************************************/
static uWord BalM_GetCellSoc(uWord volt)
{
    uByte i;
    uLWord soc;
    uLWord lwtmp;
    uLWord lwtmp2;

    soc = 0;
    if(volt >= BalM_VoltEnd.Volt_End_Bal[10])
    {
        soc = 1000; // cell is 100% SOC
    }
    else if(volt <= BalM_VoltEnd.Volt_End_Bal[0])
    {
        soc = 900; // cell is less than 90% SOC
    }
    else // cell's soc between 95%-100%
    {
        for (i = 1; i < 11; i++)
        {
            if(volt == BalM_VoltEnd.Volt_End_Bal[i])
            {
                soc = 900 + 10 * i;
                break;
            }
            else
            {
                if(volt < BalM_VoltEnd.Volt_End_Bal[i])
                {
                    lwtmp = (BalM_VoltEnd.Volt_End_Bal[i] - BalM_VoltEnd.Volt_End_Bal[i - 1]);
                    lwtmp *= 100;
                    lwtmp /= 10;//(Soc_ChgEndVolt[i].Soc - Soc_ChgEndVolt[i - 1].Soc);

                    lwtmp2 = volt - BalM_VoltEnd.Volt_End_Bal[i - 1];

                    soc = (uWord)(lwtmp2 * 100 / lwtmp);
                    soc += (900 + (10 * (i - 1)));//Soc_ChgEndVolt[i - 1].Soc;

                    break ;
                }
            }
        }
    }
    return (uWord)soc;
}
// end of BCU-C or BMU or BMU-R control balance
#endif // #if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R)) 

static uWord BalM_SaveBalTime(void)
{
    uWord rslt = ERR_OK;

#if (ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R)
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_1_15);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_16_30);
    WdgIf_Trig();
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_31_45);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_46_60);
    WdgIf_Trig();
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_BAK_1_15);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_BAK_16_30);
    WdgIf_Trig();
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_BAK_31_45);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_BAK_46_60); 
    WdgIf_Trig(); 
#endif

#if (ECU_CATEGORY == M_BCU_C)
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_1_15);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_16_30);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_31_45);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_46_60);
    WdgIf_Trig();
    #if(MAX_CELL_NUM > 60)  
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_61_75);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_76_90);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_91_105);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_106_120);
    WdgIf_Trig();
    #endif

#if(MAX_CELL_NUM > 120)
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_121_135);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_136_150);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_151_165);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_166_180);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 180)
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_181_195);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_196_210);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_211_225);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_226_240);
    WdgIf_Trig();
#endif

#if(MAX_CELL_NUM > 240)
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_241_255);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_256_270);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_271_285);
    rslt = EE_SaveVar(BOTHEE, SYSTEMST_BAL_TIME_286_300);
    WdgIf_Trig();
#endif
#endif
    return rslt;
}


#endif // #ifdef USE_BALM_F



