#ifndef INTRES_AB_H
#define INTRES_AB_H

#include "GeneralTypeDef.h"
#include "IntRes_Macro.h"  


/////////20ms once//////////////////
#define INTRES_TIMECNT_1S  50
#define INTRES_TIMECNT_2S  100
#define INTRES_TIMECNT_4S  200
#define INTRES_TIMECNT_5S  250
#define INTRES_TIMECNT_10S 500
#define INTRES_TIMECNT_11S 550

#define STEP_WAIT_LOW_CUR      0
#define STEP_WAIT_HIGH_CUR     1
#define STEP_WAIT_CALC_INTRES  2

#define INTRES_T_10C  50         //10C

#define INTRES_DCHCUR_8A  30080
#define INTRES_DCHCUR_60A 30600
#define INTRES_CHGCUR_1A  29990     // -1A

extern void IntRes_ModuleInit(void);
extern void IntRes_ModuleCountCbk(void);
extern void IntRes_ModuleCbk(void);

#endif 