// Sch_Cbk.h
#ifndef SCH_CBK_H
#define SCH_CBK_H

#include "Ecu.h"


#define MIN_TIMER_MS 5 // 5ms

#define TM_5MS   (5/MIN_TIMER_MS)
#define TM_10MS  (10/MIN_TIMER_MS)
#define TM_100MS (100/MIN_TIMER_MS)
#define TM_500MS (500/MIN_TIMER_MS)
#define TM_MS(ms)  ((ms)/MIN_TIMER_MS) // ms must can be divided by 5


void Sch_CounterCallback(void);
void Sch_TimerCallback(void); 
void Sch_MainCallback(void);

void Sch_MainInit(void);

#ifdef __DEBUG_TASK
void Debug_MainTask(void);
void Debug_TimerTask(void);
#endif
  
#endif /* SCH_CBK_H */  