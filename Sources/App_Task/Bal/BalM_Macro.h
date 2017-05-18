// BalM.h
#ifndef BALM_MACRO_H
#define BALM_MACRO_H

#include "GeneralTypeDef.h"

// select solution
#define USE_BALM_E

// select balance mode
#define BALM_MODE_A 1
#define BALM_MODE_B 2
#define BALM_MODE_C 3
#define BALM_MODE_D 4
#define BALM_MODE_E 5 
#define BALM_MODE_F 6 

#define BALM_MODE  BALM_MODE_A                       

// balance control volt for LiFePO4
#define MAX_V_CALC_BAL_THD   3400
#define MIN_V_CALC_BAL_THD   3000
#define OCV_CALC_BAL_THD     3300
#define MIN_V_START_BAL_THD  3000
#define MIN_V_STOP_BAL_THD   2800

// calc balance voltage for LiFePO4
#define VOLT_HIGH_BAL_L2  3550
#define VOLT_HIGH_BAL_L1  3500
#define VOLT_LOW_BAL_L1   3500
#define VOLT_LOW_BAL_L2   3450
#define BAL_MIN_VOLT_L1   3200
#define BAL_MIN_VOLT_L2   2800
#define BAL_DELTA_VOLT    20

// ocv linear range
#define BALM_OCV_LINEAR_RANGE_H1   3293
#define BALM_OCV_LINEAR_RANGE_L1   3213
#define BALM_OCV_LINEAR_RANGE_H2   3341
#define BALM_OCV_LINEAR_RANGE_L2   3316

// ocv-soc standby time
#define BAL_OCV_STANDBY_TIME  3600 // 1hour

#ifndef USE_BALM_F
#define BAL_CUR  100  // unit mA      
#else
#define BAL_CUR  300  // 300mA charge balance current  
#endif    


#define BALM_MAX_T_POINT 1
#define BALM_MAX_SOCOCV_POINT 21
#define BALM_MAX_SOC_POINT 11

// define for macro and config 
typedef struct
{
    uByte Balm_Max_T_Point;
    uByte Balm_Max_SocOcv_Point;
    uByte Balm_Max_Soc_Point;
} sBalM_MacroType;

typedef struct
{
    uWord Max_v_Calc_Bal_Thd;
    uWord Min_v_Calc_Bal_Thd;
    uWord Ocv_Calc_Bal_Thd;
    uWord Min_v_Start_Bal_Thd;
    uWord Min_v_Stop_Bal_Thd;
    
    uWord Volt_High_Bal_L2;  
    uWord Volt_High_Bal_L1;
    uWord Volt_Low_Bal_L1;
    uWord Volt_Low_Bal_L2;
    uWord Bal_Min_Volt_L1;
    uWord Bal_Min_Volt_L2;
    uWord Bal_Delta_Volt;
    
    uWord Balm_Ocv_Linear_Range_H1;
    uWord Balm_Ocv_Linear_Range_L1;
    uWord Balm_Ocv_Linear_Range_H2;
    uWord Balm_Ocv_Linear_Range_L2;
    
    uWord Bal_Ocv_Standby_Time;
    uWord bal_cur;
    uByte BalMode;
} sBalM_ConfigParaType;

// define v-soc table for balance judge when charge end
typedef struct
{
    uWord Soc;    // state of charge, 0.1% unit
    uWord Volt;   // volt, 0.001V unit
} sBalM_Soc2VoltType;

extern const sBalM_Soc2VoltType Soc_ChgEndVolt[BALM_MAX_SOC_POINT];
extern const sBalM_Soc2VoltType BalM_SocToOcv[BALM_MAX_T_POINT][BALM_MAX_SOCOCV_POINT];
extern const sBalM_MacroType BalM_Macro;
extern const sBalM_ConfigParaType BalM_ConfigPara;

#endif /* BALM_H */