// SOF.h
#ifndef SOF_MACRO_H
#define SOF_MACRO_H

#include "GeneralTypeDef.h"
#include "Prj_Define.h"

#define SOF_PULSE_DISCHARGE_CUR_TIME    100   //10S
#define SOF_PULSE_FEEDBACK_CUR_TIME     100   //10S

#define SOF_MINV_SELF_CALC_CUR 3000  // 3V, unit 1mV

/* define struct for macro and config parameter */
typedef struct
{
    uByte Sof_Max_Soc_Point;
    uByte Sof_Max_T_Point;
    uByte Sof_Max_Volt_Point;
    
    uByte Sof_Dch_Cur_Volt_Modify;
    uByte Sof_Dch_Cur_Err_Modify;
    uByte Sof_Fd_Cur_Volt_Modify;
    uByte Sof_Fd_Cur_Err_Modify;
    uByte Sof_Pulse_Cur_Modify;
    uByte Sof_Calc_Max_Power;
    uByte Sof_User_Fun;
} sSOF_MacroType;

typedef struct
{
    uWord Sof_Minv_Self_Calc_Cur;
    
    uWord Sof_Const_DchCur_Smooth_Unit;
    uWord Sof_Const_FeedbackCur_Smooth_Unit;
    uWord Sof_Pulse_DchCur_Smooth_Unit;
    uWord Sof_Pulse_FeedbackCur_Smooth_Unit;
    
    uWord Sof_Const_DchPow_Smooth_Unit;
    uWord Sof_Const_FeedbackPow_Smooth_Unit;
    uWord Sof_Pulse_DchPow_Smooth_Unit;
    uWord Sof_Pulse_FeedbackPow_Smooth_Unit;
    
    uByte Sof_Modify_CellvLow_L1;
    uByte Sof_Modify_CellvLow_L2;
    uByte Sof_Modify_SumvLow_L1;
    uByte Sof_Modify_SumvLow_L2;
    uByte Sof_Modify_DchCur_L1;
    uByte Sof_Modify_DchCur_L2;
    
    uByte Sof_Modify_CellvHigh_L1;
    uByte Sof_Modify_CellvHigh_L2;
    uByte Sof_Modify_SumvHigh_L1;
    uByte Sof_Modify_SumvHigh_L2;
    uByte Sof_Modify_FdCur_L1;
    uByte Sof_Modify_FdCur_L2;
    
    uByte Sof_Modify_DetVolt_L1;
    uByte Sof_Modify_DetVolt_L2;
    uByte Sof_Modify_DetTemp_L1;
    uByte Sof_Modify_DetTemp_L2;
    uByte Sof_Modify_IntCanErr;
    uByte Sof_Pulse_Discharge_Cur_Time;
    uByte Sof_Pulse_FeedBack_Cur_Time; 
          
} sSOF_ConfigParaType;

/* end of define struct for macro and config parameter */

/* define struct for volt modify */
typedef struct
{ 
    uWord volt;
    uByte k;    
}sSOF_VoltLmtType;

extern const sSOF_VoltLmtType VoltLowArray[SOF_MAX_VOLT_POINT];
extern const sSOF_VoltLmtType VoltHighArray[SOF_MAX_VOLT_POINT];
extern const sByte DchTemperArray[SOF_MAX_T_POINT];
extern const sByte FbTemperArray[SOF_MAX_T_POINT];
extern const uWord SocArray[SOF_MAX_SOC_POINT];
extern const uWord CnstDisCurArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord CnstFeedbackCurArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord PulseDisCurArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord PulseFeedbackCurArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord CnstDisPowArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord CnstFeedbackPowArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord PulseDisPowArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const uWord PulseFeedbackPowArray[SOF_MAX_SOC_POINT][SOF_MAX_T_POINT];
extern const sSOF_MacroType SOF_Macro;
extern const sSOF_ConfigParaType SOF_ConfigPara;


#endif /* SOF_MACRO_H */