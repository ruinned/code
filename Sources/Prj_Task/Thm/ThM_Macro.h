// ThM.h
#ifndef THM_MACRO_H
#define THM_MACRO_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

// select solution 
//#define USE_THM_A
#define USE_THM_AB

/* sub function enable macro */
#define THM_FUN_ENABLE      1
#define THM_FUN_DISABLE     0

// select function

#define THM_BCU_CTRL_BCU    THM_FUN_DISABLE
#define THM_BCU_CTRL_BMU    THM_FUN_DISABLE


#define HEAT_BCU_CTRL_BCU    THM_FUN_DISABLE
#define HEAT_BCU_CTRL_BMU    THM_FUN_DISABLE

#define FAN_RELAY_ID  CPU_FAN_ID
#define HEAT_RELAY_ID  CPU_HEAT_ID



#define FAN_ON_MIN_T  55
#define FAN_OFF_MIN_T 50

#define FAN_HIGH_SPEED  1000
#define FAN_LOW_SPEED   500

#define THM_CNT_5S      10


#endif /* THM_H */