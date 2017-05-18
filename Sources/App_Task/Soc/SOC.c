// SOC.c
#include "SOC.h"

#include "SOC-A.h"

extern const sSOC_SocToVoltType SocToOcvArrayStepUp[SOC_MAX_T_POINT][6];
extern const sSOC_SocToVoltType SocToOcvArrayStepDown[SOC_MAX_T_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayChgPre_HT[SOC_MAX_C_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayChgPre_LT[SOC_MAX_C_POINT][4];
extern const sSOC_SocToVoltType SocToVoltArrayDchDynamic[SOC_MAX_T_POINT][SOC_MAX_DCH_POINT];
extern const sSOC_TemperToAhType TemperToAhArray[SOC_MAX_T_AH_POINT];


const sSOC_SocToVoltType SocToOcvArrayStepUp[SOC_MAX_T_POINT][6] =
// [temperature][point] // not less than feature point 
{
    {   //T = 15~60'C
        {950, 3338}, {700, 3332}, {650, 3310}, {600, 3299},
        {350, 3293}, {300, 3286}
    },
    {   //T = 0~15'C
        {950, 3338}, {700, 3332}, {650, 3310}, {600, 3299},
        {350, 3293}, {300, 3286}
    },
    {   //T = -10~0'C
        {950, 3338}, {700, 3332}, {650, 3310}, {600, 3299},
        {350, 3293}, {300, 3286}
    },
    {   //T < -10'C
        {950, 3338}, {700, 3332}, {650, 3310}, {600, 3299},
        {350, 3293}, {300, 3286}
    }
};

const sSOC_SocToVoltType SocToOcvArrayStepDown[SOC_MAX_T_POINT][4] =
// [temperature][point] // not more than feature point
{
    {   //T = 15~60'C
        {350, 3270}, {600, 3282}, {650, 3296}, {700, 3314}
    },
    {   //T = 0~15'C
        {350, 3270}, {600, 3282}, {650, 3296}, {700, 3314}
    },
    {   //T = -10~0'C
        {350, 3270}, {600, 3282}, {650, 3296}, {700, 3314}
    },
    {   //T < -10'C
        {350, 3270}, {600, 3282}, {650, 3296}, {700, 3314}
    }
};

const sSOC_SocToVoltType SocToVoltArrayChgPre_HT[SOC_MAX_C_POINT][4] =
// [chg_rate][point] // pre estimate point when charging @ T >= 15'C
// if has data at other temperatures, should use three-dimensional array
{
    {
        {980, 3650}, {900, 3417}, {200, 3320}, {100, 3273} // 0.03C-0.24C
    },

    {
        {970, 3650}, {900, 3449}, {200, 3337}, {100, 3293} // 0.24C-0.38C
    },

    {
        {960, 3650}, {900, 3500}, {200, 3383}, {100, 3344} // 0.38C-0.70C
    },

    {
        {950, 3650}, {900, 3566}, {200, 3394}, {100, 3354} // 0.70C-1.20C
    }
};

const sSOC_SocToVoltType SocToVoltArrayChgPre_LT[SOC_MAX_C_POINT][4] =
// [chg_rate][point] // pre estimate point when charging @ T < 15'C
// if has data at other temperatures, should use three-dimensional array
{
    {
        {980, 3650}, {900, 3417}, {200, 3320}, {100, 3273} // 0.03C-0.24C
    },

    {
        {970, 3650}, {900, 3449}, {200, 3337}, {100, 3293} // 0.24C-0.38C
    },

    {
        {960, 3650}, {900, 3500}, {200, 3383}, {100, 3344} // 0.38C-0.70C
    },

    {
        {950, 3650}, {900, 3566}, {200, 3394}, {100, 3354} // 0.70C-1.20C
    }
};

const sSOC_SocToVoltType SocToVoltArrayDchDynamic[SOC_MAX_T_POINT][SOC_MAX_DCH_POINT] =
// [temperature][point] // dynamic estimate point when discharging 
{
    {   //T = 15~60'C
        {  0, 2779}, { 50, 3093}, {100, 3169}, {150, 3189},
        {200, 3211}, {250, 3234}, {300, 3252}, {350, 3269},
        {400, 3311}
    },
    
    {   //T = 0~15'C
        {  0, 2779}, { 50, 3093}, {100, 3169}, {150, 3189},
        {200, 3211}, {250, 3234}, {300, 3252}, {350, 3269},
        {400, 3311}
    },
    
    {   //T = -10~0'C
        {  0, 2779}, { 50, 3093}, {100, 3169}, {150, 3189},
        {200, 3211}, {250, 3234}, {300, 3252}, {350, 3269},
        {400, 3311}
    },
    
    {   //T < -10'C
        {  0, 2779}, { 50, 3093}, {100, 3169}, {150, 3189},
        {200, 3211}, {250, 3234}, {300, 3252}, {350, 3269},
        {400, 3311}
    }
};

const sSOC_TemperToAhType TemperToAhArray[SOC_MAX_T_AH_POINT] =
{
    //-20'C,   -15'C,    -10'C,     -5'C,     0'C,
    {20, 36}, {25, 58}, {30, 68}, {35, 77}, {40, 84}, 
    //  5'C,    10'C,     15'C,     20'C,     25'C,
    {45, 91}, {50, 96}, {55, 100}, {60, 100}, {65, 100},
};
/**************** end of battery data for SOC estimation *********************/

/* define struct for macro and config parameter */
const sSOC_MacroSwitchType SOC_MacroSwitch =
{
    OCV_INIT_ESTMAT,
    OCV_STEP_ESTMAT,
    CHG_PRE_ESTMAT,
    CHG_END_ESTMAT,
    DYN_LOW_ESTMAT,
    STATIC_LOW_ESTMAT,
    LMT_LOW_ESTMAT,
    LMT_HIGH_ESTMAT,
    SOC_SMOOTH,
    
    ACTUAL_AH_SOH_ESTMAT,
    ACTUAL_AH_TEMPER_ESTMAT,
    ACTUAL_AH_SMOOTH,
};

const sSOC_MacroType SOC_Macro =
{
    SOC_MAX_SOC_POINT,
    SOC_MAX_T_POINT,
    SOC_MAX_C_POINT,
    SOC_MAX_T_AH_POINT,
    SOC_MAX_DCH_POINT,
};

const sSOC_ConfigParaType SOC_ConfigPara =
{
    DELTAV_SOC_LESS_985,
    DELTAV_SOC_LESS_995,
    DELTA_MIN_V,
    OCV_STANDBY_TIME,
    OCV_LINEAR_RANGE_L1,
    OCV_LINEAR_RANGE_H1,
    OCV_LINEAR_RANGE_L2,
    OCV_LINEAR_RANGE_H2,
    OCV_STEP_RANGE_L,
    OCV_STEP_RANGE_H,
    AVGV_CHG_END,
    MAX_SOC_LOW_ESTMAT,
    MIN_CELLV_LOW,
    AVG_CELLV_LOW,
    MAX_CELLV_LMT,
    MIN_CELLV_LMT,
};
/* end define struct for macro and config parameter */


/* define time interval for soc smooth */
#define WCNT_100MS 1
#define WCNT_500MS 5
#define WCNT_1S    10
#define WCNT_2S    (2 * WCNT_1S)
#define WCNT_5S    (5 * WCNT_1S)
#define WCNT_10S   (10 * WCNT_1S)
/* end of define time interval for soc smooth */


/* module initial */
void SOC_Init(void)
{
    /* soc module initial */
    SOC_ModuleInit();
    
    /*** !!! Here you can place your own code !!! ***/
    
    /*** !!! end of your own code !!! ***/
}

/* module count call back */
void SOC_MainCountCbk(void)
{
    /* count every 5ms */
    SOC_ModuleCountCbk();

    /*** !!! Here you can place your own code !!! ***/
    
    /*** !!! end of your own code !!! ***/
}

/* main call back */
void SOC_MainCbk(void)
{
    /* soc estimate process */
    SOC_ModuleCbk();
    
    /*** !!! Here you can place your own code !!! ***/
    
    /* for example */
    /* 
       soc_target = 10; // 1%
       time_interval = WCNT_2S; // change 0.1%/2S
       SOC_EstmatUser(soc_target, time_interval);
    */
    /* end example */
    
    
    /*** !!! end of your own code !!! ***/
}