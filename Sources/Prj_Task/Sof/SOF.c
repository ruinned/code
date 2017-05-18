// SOF.c
#include "SOF.h"

#include "SOF-A.h"


/* define struct for macro and config parameter */
const sSOF_MacroType SOF_Macro =
{
    SOF_MAX_SOC_POINT,
    SOF_MAX_T_POINT,
    SOF_MAX_VOLT_POINT,
    SOF_DCH_CUR_VOLT_MODIFY,
    SOF_DCH_CUR_ERR_MODIFY,
    SOF_FD_CUR_VOLT_MODIFY,
    SOF_FD_CUR_ERR_MODIFY,
    SOF_PLUSE_CUR_MODIFY,      
    SOF_CALC_MAX_POWER,
    SOF_USER_FUN,
};

const sSOF_ConfigParaType SOF_ConfigPara =
{
    SOF_MINV_SELF_CALC_CUR,
    
    /* use for smooth permit current or power */
    SOF_CONST_DCHCUR_SMOOTH_UNIT,
    SOF_CONST_FEEDBACKCUR_SMOOTH_UNIT,
    SOF_PULSE_DCHCUR_SMOOTH_UNIT,
    SOF_PULSE_FEEDBACKCUR_SMOOTH_UNIT,
    
    SOF_CONST_DCHPOW_SMOOTH_UNIT,
    SOF_CONST_FEEDBACKPOW_SMOOTH_UNIT,
    SOF_PULSE_DCHPOW_SMOOTH_UNIT,
    SOF_PULSE_FEEDBACKPOW_SMOOTH_UNIT,
    
    /* for discharge cur modify */
    SOF_MODIFY_CELLV_LOW_L1,
    SOF_MODIFY_CELLV_LOW_L2,  
    SOF_MODIFY_SUMV_LOW_L1,
    SOF_MODIFY_SUMV_LOW_L2,    
    SOF_MODIFY_DCHCUR_HIGH_L1,    
    SOF_MODIFY_DCHCUR_HIGH_L2, 
    
    /* for feedback cur modify */   
    SOF_MODIFY_CELLV_HIGH_L1,     
    SOF_MODIFY_CELLV_HIGH_L2,     
    SOF_MODIFY_SUMV_HIGH_L1,
    SOF_MODIFY_SUMV_HIGH_L2,
    SOF_MODIFY_FDCUR_HIGH_L1,
    SOF_MODIFY_FDCUR_HIGH_L2,
    
    /* for both discharge and feedback cur modify */
    SOF_MODIFY_DETVOLT_L1,     
    SOF_MODIFY_DETVOLT_L2,     
    SOF_MODIFY_DETTEMP_L1,  
    SOF_MODIFY_DETTEMP_L2,  
    SOF_MODIFY_INTCAN_ERR,
    SOF_PULSE_DISCHARGE_CUR_TIME,
    SOF_PULSE_FEEDBACK_CUR_TIME,        
};
/* end define struct for macro and config parameter */


#define SOF_CONST_DCHCUR       0
#define SOF_CONST_FEEDBACKCUR  1
#define SOF_PULSE_DCHCUR       2
#define SOF_PULSE_FEEDBACKCUR  3

#define SOF_CONST_DCHPOW       4
#define SOF_CONST_FEEDBACKPOW  5
#define SOF_PULSE_DCHPOW       6
#define SOF_PULSE_FEEDBACKPOW  7


/* module initial */
void SOF_Init(void)
{
    SOF_ModuleInit();
    /*** !!! Here you can place your own code !!! ***/
     
    /*** !!! end of your own code !!! ***/
}

/* module count call back */
void SOF_MainCountCbk(void)
{
    SOF_ModuleCountCbk();
    /*** !!! Here you can place your own code !!! ***/
     
    /*** !!! end of your own code !!! ***/
}

/* main call back */
void SOF_MainCbk(void)
{
    /*** !!! Here you can place your own code !!! ***/
    /* for example */
    /*
        SOF_SetTargetUser(SOF_CONST_DCHCUR, 200); // const_dch_cur = 20A
        SOF_SetTargetUser(SOF_CONST_FEEDBACKCUR, 100); // const_feedback_cur = 10A
        SOF_ModifyUser(SOF_CONST_DCHCUR, 80); // const_dch_cur = 80% * const_dch_cur
        SOF_ModifyUser(SOF_CONST_FEEDBACKCUR, 50); // const_feedback_cur = 50% * const_feedback_cur
    */
    /* end example */
    
    
     
    /*** !!! end of your own code !!! ***/
    
    SOF_ModuleCbk();
}
