/** ###################################################################
**     Filename  : IOCEvents.c
**     Project   : mcal
**     Processor : MC9S12XEQ384MAG
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2013/10/12, 14:07
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         AC_CP_IOC_OnCapture - void AC_CP_IOC_OnCapture(void);
**
** ###################################################################*/
/* MODULE IOCEvents */


#include "Cpu.h"
#include "CpuEvents.h"
#include "TimerEvents.h"
#include "CAN0Events.h"
#include "CAN1Events.h"
#include "CAN4Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  AC_CP_IOC_OnCapture (module IOCEvents)
**
**     Component   :  AC_CP_IOC [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AC_CP_IOC_OnCapture(void)
{
  /* Write your code here ... */
}

/* END IOCEvents */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
