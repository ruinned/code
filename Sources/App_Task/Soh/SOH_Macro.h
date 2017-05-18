#ifndef SOH_MACRO_H
#define SOH_MACRO_H

#include "GeneralTypeDef.h"

/* select solution */ 
#define USE_SOH_B


/* define parameter for SOH-A */
#define MAX_OCV_CALC_SOH   3286  //35%SOC point
#define MIN_OCV_CALC_SOH   2000  // 2V
#define DELTAV_CALC_SOH    400   // 400mV
/* end of define parameter for SOH-A */


/* define struct for SOH-B */
typedef struct
{
    uWord Soh;         // soh, 0.1% unit
    uLWord TotalDchAh; // AH, 1AH unit
} sSOH_TotalDchAhType;

typedef struct
{
    uWord Soh;      // soh, 0.1% unit
    uByte IntRes;   // intres, 0.01 m¦¸ unit
} sSOH_IntResType;

#define SOH_MAX_AH_POINT 7
#define SOH_MAX_IntRes_POINT 9
/* end of define struct for SOH-B */


/* define struct for macro and config parameter */
typedef struct
{
    uByte Soh_Max_Ah_Point;
    uByte Soh_Max_Intres_Point;
} sSOH_MacroType;

typedef struct
{
    uWord Max_Ocv_Calc_Soh;
    uWord Min_Ocv_Calc_Soh;
    uWord DeltaV_Calc_Soh;    
} sSOH_ConfigParaType;
/* end of define struct for macro and config parameter */

extern const sSOH_TotalDchAhType SOH_TotalDchAh[SOH_MAX_AH_POINT];
extern const sSOH_IntResType SOH_IntRes[SOH_MAX_IntRes_POINT];
extern const sSOH_MacroType SOH_Macro;
extern const sSOH_ConfigParaType SOH_ConfigPara;

#endif // SOH_MACRO_H 