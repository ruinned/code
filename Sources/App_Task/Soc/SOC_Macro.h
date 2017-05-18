// SOC.h
#ifndef SOC_MACRO_H
#define SOC_MACRO_H

#include "GeneralTypeDef.h"
#include "Prj_Define.h"

#define DYN_LOW_ESTMAT    SOC_FUN_DISABLE
#define STATIC_LOW_ESTMAT SOC_FUN_DISABLE
#define OCV_STEP_ESTMAT   SOC_FUN_DISABLE
#define CHG_PRE_ESTMAT    SOC_FUN_DISABLE
#define ACTUAL_AH_SOH_ESTMAT    SOC_FUN_DISABLE
#define ACTUAL_AH_TEMPER_ESTMAT SOC_FUN_DISABLE
#define ACTUAL_AH_SMOOTH        SOC_FUN_DISABLE

#define OCV_STEP_RANGE_L     0     // unit 1mV
#define OCV_STEP_RANGE_H     0     // unit 1mV    // unit 1mV

#define MAX_SOC_LOW_ESTMAT   330U  // unit 0.1%
#define MIN_CELLV_LOW        3292U // unit 1mV
#define AVG_CELLV_LOW        3311U // unit 1mV

#define SOC_MAX_C_POINT    4U
#define SOC_MAX_T_AH_POINT 10U
#define SOC_MAX_DCH_POINT  9U

/* define struct for OCV-SOC */
typedef struct
{
    uWord Soc; // unit:0.1%
    uWord Volt;// unit:1mV
} sSOC_SocToVoltType;

/* define struct for temper-AH */
typedef struct
{
    uByte Temper; // unit:1'C, -40 offset
    uByte k_Ah;   // unit:1%
} sSOC_TemperToAhType;

/* define struct for macro and config parameter */
typedef struct
{
    uByte Ocv_Init_Estmat;
    uByte Ocv_Step_Estmat;
    uByte Chg_Pre_Estmat;
    uByte Chg_End_Estmat;
    uByte Dyn_Low_Estmat;
    uByte Static_Low_Estmat;
    uByte Lmt_Low_Estmat;
    uByte Lmt_High_Estmat;
    uByte Soc_Smooth;

    uByte ActualAh_Soh_Estmat; 
    uByte ActualAh_Temper_Estmat;
    uByte ActualAh_Smooth;
} sSOC_MacroSwitchType;

typedef struct
{
    uByte Soc_Max_Soc_Point;
    uByte Soc_Max_T_Point;
    uByte Soc_Max_C_Point;
    uByte Soc_Max_T_Ah_Point;
    uByte Soc_Max_Dch_Point;
} sSOC_MacroType;

typedef struct
{
    // use for control soc no more than 100% when volt not high enough
    uWord DeltaV_Soc_Less_985;
    uWord DeltaV_Soc_Less_995;
    
    // use for select min volt or avg volt to estimate soc 
    uWord Delta_Min_V;
    
    // use for ocv estimate
    uWord Ocv_Standby_Time;
    uWord Ocv_Linear_Range_L1;
    uWord Ocv_Linear_Range_H1;
    uWord Ocv_Linear_Range_L2;
    uWord Ocv_Linear_Range_H2;
    uWord Ocv_Step_Range_L;
    uWord Ocv_Step_Range_H;
    
    // use for charge end estimate
    uWord AvgV_Chg_End;
    
    // use for SOC low estimate
    uWord Max_Soc_Low_Estmat;
    uWord Min_CellV_Low;
    uWord Avg_CellV_Low;
    
    // use for limit estimate
    uWord Max_CellV_Lmt;
    uWord Min_CellV_Lmt;
} sSOC_ConfigParaType;
/* end of define struct for macro and config parameter */


extern const sSOC_SocToVoltType SocToOcvArray[SOC_MAX_T_POINT][SOC_MAX_SOC_POINT];
extern const sSOC_SocToVoltType SocToOcvArrayStepUp[SOC_MAX_T_POINT][6];
extern const sSOC_SocToVoltType SocToOcvArrayStepDown[SOC_MAX_T_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayChgPre_HT[SOC_MAX_C_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayChgPre_LT[SOC_MAX_C_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayDchDynamic[SOC_MAX_T_POINT][SOC_MAX_DCH_POINT];
extern const sSOC_TemperToAhType TemperToAhArray[SOC_MAX_T_AH_POINT];
extern const sSOC_MacroSwitchType SOC_MacroSwitch;
extern const sSOC_MacroType SOC_Macro;
extern const sSOC_ConfigParaType SOC_ConfigPara;

#endif /* SOC_MACRO_H */