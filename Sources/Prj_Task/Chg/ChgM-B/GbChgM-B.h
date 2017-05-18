#ifndef GBCHGM_B_H
#define GBCHGM_B_H
#include "GeneralTypeDef.h"
#include "CanIf_Type.h"

#define CHG_CAN_ERR    0x00000040UL
#define CHG_BAT_NOT_MATCH 0x00000800UL
#define CHG_HD_ERR     0x00100000UL
#define CHG_RLY_ERR    0x00000200UL
#define CHG_BAT_FULL   0x00000400UL
#define CHG_ISO_ERR    0x00080000UL
#define CHG_CELLV_HIGH 0x00040000UL
#define CHG_CELLV_LOW  0x00020000UL
#define CHG_SUMV_HIGH  0x00010000UL
#define CHG_SUMV_LOW   0x00008000UL
#define CHG_TEMP_HIGH  0x00004000UL
#define CHG_TEMP_LOW   0x00002000UL
#define CHG_CUR_HIGH   0x00001000UL

#define LINK_FAIL_300MS     30U    /* count for detect ac link fail, 300ms */


/* other */
#define CHG_RX_BMS      0xAAU
#define CHG_NOT_RX_BMS  0x00U

#define CHG_READY      0xAAU
#define CHG_NOT_READY  0x00U
//#define BMS_READY      0xAAU
//#define BMS_NOT_READY  0x00U

#define CHG_MODE_CV    0x01U
#define CHG_MODE_CC    0x02U

/* GB charge end reason */
#define  REACH_SOC     0x01U
#define  REACH_SUMV    0x04U
#define  REACH_CELLV   0x10U
#define  RX_CST        0x40U

#define  ISO_ERR       0x01U
#define  CON_OVER_T    0x04U
#define  BMS_OVER_T    0x10U
#define  CHG_CON_ERR   0x40U
#define  BAT_OVER_T    0x01U
#define  OTHER_ERR     0x04U
#define  OVER_CUR      0x01U
#define  OVER_VOLT     0x04U
#define  CC2VOLT_ERR   0x10U


/* Charge step */
#define ChgStep0    0U
#define ChgStep1    1U
#define ChgStep2    2U
#define ChgStep3    3U
#define ChgStep4    4U
#define ChgStep5    5U
#define ChgStep6    6U
#define ChgStep7    7U
#define ChgStep8    8U
#define ChgStep9    9U
#define ChgStep10   10U


#define CNT_DET_LINK    20U   //200ms, for detect

#define CHG_S_ON  1U
#define CHG_S_OFF 0U


#define CHG_P_ON  0U
#define CHG_P_OFF 1U


#define MODE_2015  1U
#define MODE_2011  2U

typedef struct
{

    uByte Brm : 1; 					 //bms recognise
    uByte Bcp : 1; 					 //battery charge data
    uByte Bro : 1;				   //battery Ready to charge
    uByte Bcl : 1; 					 //battery charge requirement
    uByte Bcs : 1; 					 //battery charge status
    uByte Bsm : 1;					 //bms send battery status
    uByte Bmv : 1; 					 //battery voltage
    uByte Bmt : 1;					 //battery temperature
    uByte Bsp : 1;					 //battery reserved
    uByte Bst : 1; 					 //bms stop charge
    uByte Bsd : 1;				   //bms statistics data message
    uByte Bem : 1; 					 //bms error message
    uByte Bhm : 1;
    uByte rsv : 3;

} sGbChgM_FLG;

typedef struct
{
    uByte ChgStep;
    uByte ChgStepBak;
    uByte HandShakeErr;
    uByte ConfigErr;
    uByte ChargingErr;
    uByte ChargeEndErr;
    uByte ChgEndReason1;
    uByte ChgEndReason2;
    uByte ChgEndReason3;
    uByte ChgEndReason4;
    sGbChgM_FLG Flg;
} sGbChgM_CtrlType;

extern sGbChgM_CtrlType GbChgM_Ctrl;

extern uByte GbChg_Mode;

extern void GbChgM_UpdateAcPmtChgCur(uWord cur);
extern uWord GbChgM_GetAcPmtChgCur(void);

extern void GbChgM_LinkModeJudge(void);
extern void GbChgM_EndReasonDeal(void);
extern void GbChgM_StopReasonJudge(void);
extern uWord GbChgM_ACPmtCurCalc(void);

extern void GbChgM_ModuleInit(void);
extern void GbChgM_ModuleCountCbk(void);
extern void GbChgM_ModuleCbk(void); 

#endif /* GBCHGM_B_H  */