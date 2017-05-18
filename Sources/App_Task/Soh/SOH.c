#include "SOH.h"

#ifdef USE_SOH_A
#include "SOH-A.h"
#endif

#ifdef USE_SOH_B
#include "SOH-B.h"
#endif

/* define for SOH-B */
const sSOH_TotalDchAhType SOH_TotalDchAh[SOH_MAX_AH_POINT] =
{
    {1000, 0}, {950, 33000}, {900, 66000}, {850, 99000}, 
    {800, 132000}, {750, 165000}, {700, 198000}
};

const sSOH_IntResType SOH_IntRes[SOH_MAX_IntRes_POINT] =
{
    {1000,100}, {950,120}, {900,140}, {850,160}, {800,170}, {750,180}, 
    {700,190}, {650,200}, {600,210}
};
/* end define for SOH-B */

/* define struct for macro and config parameter */
const sSOH_MacroType SOH_Macro =
{
    SOH_MAX_AH_POINT,
    SOH_MAX_IntRes_POINT,
};

const sSOH_ConfigParaType SOH_ConfigPara =
{
    MAX_OCV_CALC_SOH,
    MIN_OCV_CALC_SOH,
    DELTAV_CALC_SOH,
};
/* end define struct for macro and config parameter */


/* module initial */
void SOH_Init(void)
{
    /* soh module initial */
    SOH_ModuleInit();
    
    /*** !!! Here you can place your own code !!! ***/
    
    /*** !!! end of your own code !!! ***/
}

/* module count call back */
void SOH_MainCountCbk(void)
{
    /* count every 5ms */
    SOH_ModuleCountCbk();

    /*** !!! Here you can place your own code !!! ***/
    
    /*** !!! end of your own code !!! ***/
}

/* main call back */
void SOH_MainCbk(void)
{
    /* soh estimate process */
    SOH_ModuleCbk();
    
    /*** !!! Here you can place your own code !!! ***/
    
    /*** !!! end of your own code !!! ***/
}