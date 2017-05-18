/** ###################################################################
**     Filename  : TimerEvents.c
**     Project   : mcal
**     Processor : MC9S12XEP100MAG
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2013/9/12, 15:21
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         Timer_OnInterrupt - void Timer_OnInterrupt(void);
**
** ###################################################################*/
/* MODULE TimerEvents */


#include "Cpu.h"
#include "Events.h"
#include "TimerEvents.h"
#include "CAN0Events.h"
#include "CAN1Events.h"
#include "CAN4Events.h"
#include "AC_CP_IOCEvents.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "PE_cfg.h"
#ifdef PE_CBK_EN
//#include "Sch_Timer.h"
#include "Sch_Cbk.h"
#endif

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  Timer_OnInterrupt (module TimerEvents)
**
**     Component   :  Timer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Timer_OnInterrupt(void)
{
  /* Write your code here ... */
  #ifdef PE_CBK_EN
  //Timer_Task();
  Sch_CounterCallback();
  Sch_TimerCallback();
  
  #endif
}

/* END TimerEvents */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
