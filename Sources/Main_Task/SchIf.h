// SchIf.h
#ifndef SCHIF_H
#define SCHIF_H


#define MIN_TIMER_MS 5 // 5ms

#define TM_5MS   (5/MIN_TIMER_MS)
#define TM_10MS  (10/MIN_TIMER_MS)
#define TM_100MS (100/MIN_TIMER_MS)
#define TM_500MS (500/MIN_TIMER_MS)
#define TM_MS(ms)  ((ms)/MIN_TIMER_MS) // ms must can be divided by 5

typedef enum
{
   BYTE_TM,
   WORD_TM,
   LWORD_TM,   
 }eTimerClasType;


#endif                 