/** ###################################################################
**     Filename  : CAN0Events.c
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
**         CAN0_OnFreeTxBuffer - void CAN0_OnFreeTxBuffer(word BufferMask);
**         CAN0_OnFullRxBuffer - void CAN0_OnFullRxBuffer(void);
**         CAN0_OnBusOff       - void CAN0_OnBusOff(void);
**
** ###################################################################*/
/* MODULE CAN0Events */


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
#include "Can_Cbk.h"
#endif
#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  CAN0_OnFreeTxBuffer (module CAN0Events)
**
**     Component   :  CAN0 [FreescaleCAN]
**     Description :
**         This event is called after a successful transmission of a
**         message. The event is available only if Interrupt
**         service/event is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferMask      - Transmit buffer mask. The
**                           mask can be used to check what message
**                           buffer caused the transmit interrupt.
**     Returns     : Nothing
** ===================================================================
*/
void CAN0_OnFreeTxBuffer(word BufferMask)
{
  /* Write your code here ... */
  #ifdef PE_CBK_EN
  CAR_CAN_TX_CONFIRM(BufferMask);
  CAR_CAN_SEND_TASK();
  #endif
}

/*
** ===================================================================
**     Event       :  CAN0_OnFullRxBuffer (module CAN0Events)
**
**     Component   :  CAN0 [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full
**         after a successful reception of a message. The event is
**         available only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN0_OnFullRxBuffer(void)
{
  /* Write your code here ... */
  #ifdef PE_CBK_EN
  CAR_CAN_RECEIVE_TASK();
  #endif
}

/*
** ===================================================================
**     Event       :  CAN0_OnBusOff (module CAN0Events)
**
**     Component   :  CAN0 [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN0_OnBusOff(void)
{
  /* Write your code here ... */
}

/* END CAN0Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
