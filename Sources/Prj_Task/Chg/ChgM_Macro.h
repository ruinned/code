// ChgM_Macro.h
#ifndef CHGM_MACRO_H
#define CHGM_MACRO_H

#include "GeneralTypeDef.h"
#include "SchIf.h"
#include "BmsSysSt.h"
#include "Prj_Define.h"

// select solution
#define USE_CHGM_B

/* config time count */
#define CHGM_CYCLE  TM_MS(45)

//#define CHG_DET_RLY      /* need detect relay status when judge charge enable  */
//#define CHG_DET_CHG_CAN    /* need detect charge CAN connect when judge charge enable */
//#define CHG_DET_CHARGER_OUT_CUR    /* need detect charger's max output current when calc pmt current */

#define CHG_CALC_CUR_MODE1
//#define CHG_CALC_CUR_MODE2

#define TIME_1S     1000
#define TIME_2S     2000
#define TIME_2S5    2500
#define TIME_3S     3000
#define TIME_5S     5000
#define TIME_10S    10000
#define TIME_1MIN   60000
#define TIME_3MIN   180000
#define TIME_5MIN   300000

#define CHGM_CNT_WAIT_READY    (TIME_2S5 / (CHGM_CYCLE * 5))
#define CHGM_CNT_CAN_ERR       (TIME_10S / (CHGM_CYCLE * 5))
#define CHGM_CNT_CHG_ERR       (TIME_2S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_STEP_CUR      (TIME_2S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_DOWN_CUR      (TIME_2S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_RLY_OFF       (TIME_1S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_RLY_ON        (TIME_1S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_CALC_CUR      (TIME_3S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_WAIT_STOP     (TIME_5S /  (CHGM_CYCLE * 5))
#define CHGM_CNT_CHG_1MIN      (TIME_1MIN /  (CHGM_CYCLE * 5))
#define CHGM_CNT_CHG_3MIN      (TIME_3MIN /  (CHGM_CYCLE * 5))
#define CHGM_CNT_START_TIME    (TIME_5MIN /  (CHGM_CYCLE * 5))
/* end of config time count */



/*========================================================================
 Define for GB Charge
======================================================================== */
/* define the charge mode */
#define NO_CHG_CON        0				  //No charge connector
#define AC_CHG_MODE1_B    1					//ac charge mode1 connection type B
#define AC_CHG_MODE2_B    2					//ac charge mode2 connection type B
#define AC_CHG_MODE3_A    3					//ac charge mode3 connection type A
#define AC_CHG_MODE3_B    4					//ac charge mode3 connection type B
#define AC_CHG_MODE3_C    5					//ac charge mode3 connection type C
#define AC_CHG_MODE       6					//ac charge mode
#define DC_CHG_MODE       7					//dc charge mode
#define AC_CHG_CON_ERR    8         //ac charge connet error
#define AC_CHG_LOCK_ERR   9
#define CHG_MODE_CONFLICT 10


/* config CC and CC2 volt range  */
#define CC_V_L_10AMODE (((BmsSys_GetK30Volt() - 5) * 3 / 5) - 10)   
#define CC_V_H_10AMODE (((BmsSys_GetK30Volt() - 5) * 3 / 5) + 10)

#define CC_V_L_16AMODE (((BmsSys_GetK30Volt() - 5) * 17 / 42) - 8)
#define CC_V_H_16AMODE (((BmsSys_GetK30Volt() - 5) * 17 / 42) + 8)

#define CC_V_L_32AMODE (((BmsSys_GetK30Volt() - 5) * 11 / 61) - 6)
#define CC_V_H_32AMODE (((BmsSys_GetK30Volt() - 5) * 11 / 61) + 6)

#define CC_V_L_63AMODE (((BmsSys_GetK30Volt() - 5) * 10 / 110) - 6)
#define CC_V_H_63AMODE (((BmsSys_GetK30Volt() - 5) * 10 / 110) + 6)

#define CC2_V_L        (((BmsSys_GetK30Volt() - 5) / 2) - 10)
#define CC2_V_H        (((BmsSys_GetK30Volt() - 5) / 2) + 10)

#define CON_CONFLICT_V (((BmsSys_GetK30Volt() - 5) * 3) / 4)

/* end of config CC and CC2 volt range */

typedef struct
{
    uByte    t;		   /* control variable, lby_max_t or lby_min_t, unit -40C  */
    uWord    Cur;		 /* pmt charge current which calc by temper, unit: 0.01C for Dc chg, 0.1 for ac chg*/
} sChg_CurTableType;

#define T_SEC  8

extern const sChg_CurTableType DcChg_TemperCurTable[T_SEC];
extern const sChg_CurTableType AcChg_TemperCurTable[T_SEC];

#endif /* CHGM_MACRO_H */
