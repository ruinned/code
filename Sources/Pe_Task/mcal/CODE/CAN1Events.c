/** ###################################################################
**     Filename  : CAN1Events.c
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
**         CAN1_OnFreeTxBuffer - void CAN1_OnFreeTxBuffer(word BufferMask);
**         CAN1_OnFullRxBuffer - void CAN1_OnFullRxBuffer(void);
**         CAN1_OnBusOff       - void CAN1_OnBusOff(void);
**
** ###################################################################*/
/* MODULE CAN1Events */


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
**     Event       :  CAN1_OnFreeTxBuffer (module CAN1Events)
**
**     Component   :  CAN1 [FreescaleCAN]
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
void CAN1_OnFreeTxBuffer(word BufferMask)
{
  /* Write your code here ... */
  #ifdef PE_CBK_EN
  CHG_CAN_TX_CONFIRM(BufferMask);
  CHG_CAN_SEND_TASK();
  #endif
}

/*
** ===================================================================
**     Event       :  CAN1_OnFullRxBuffer (module CAN1Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full
**         after a successful reception of a message. The event is
**         available only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN1_OnFullRxBuffer(void)
{
  /* Write your code here ... */
   #ifdef PE_CBK_EN
  CHG_CAN_RECEIVE_TASK();
  #endif
}

/*
** ===================================================================
**     Event       :  CAN1_OnBusOff (module CAN1Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN1_OnBusOff(void)
{
  /* Write your code here ... */
}

/* END CAN1Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
