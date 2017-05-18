/** ###################################################################
**     Filename  : AC_CP_IOCEvents.c
**     Project   : mcal
**     Processor : MC9S12XEP100MAG
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2013/10/12, 14:10
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         AC_CP_IOC_OnCapture - void AC_CP_IOC_OnCapture(void);
**
** ###################################################################*/
/* MODULE AC_CP_IOCEvents */


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
//#include "Ioc_Cbk.h"
#include "IocIf.h"
#endif

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  AC_CP_IOC_OnCapture (module AC_CP_IOCEvents)
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
  #ifdef PE_CBK_EN
  (void)AC_CP_IOC_TASK();
  #endif
  
}

/* END AC_CP_IOCEvents */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
