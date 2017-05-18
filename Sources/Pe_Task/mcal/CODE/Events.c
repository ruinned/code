/** ###################################################################
**     Filename  : Events.c
**     Project   : mcal
**     Processor : MC9S12XEP100MAG
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2013-9-18, 17:20
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         EE_IIC_OnRxChar       - void EE_IIC_OnRxChar(void);
**         EE_IIC_OnTxChar       - void EE_IIC_OnTxChar(void);
**         EE_IIC_OnNACK         - void EE_IIC_OnNACK(void);
**         TIMER_IIC_OnRxChar    - void TIMER_IIC_OnRxChar(void);
**         TIMER_IIC_OnTxChar    - void TIMER_IIC_OnTxChar(void);
**         TIMER_IIC_OnNACK      - void TIMER_IIC_OnNACK(void);
**         SUMV_EXT_IIC_OnRxChar - void SUMV_EXT_IIC_OnRxChar(void);
**         SUMV_EXT_IIC_OnTxChar - void SUMV_EXT_IIC_OnTxChar(void);
**         SUMV_EXT_IIC_OnNACK   - void SUMV_EXT_IIC_OnNACK(void);
**         SUMV_IIC_OnRxChar     - void SUMV_IIC_OnRxChar(void);
**         SUMV_IIC_OnTxChar     - void SUMV_IIC_OnTxChar(void);
**         SUMV_IIC_OnNACK       - void SUMV_IIC_OnNACK(void);
**
** ###################################################################*/
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"
#include "TimerEvents.h"
#include "CAN0Events.h"
#include "CAN1Events.h"
#include "CAN4Events.h"
#include "AC_CP_IOCEvents.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  EE_IIC_OnRxChar (module Events)
**
**     Component   :  EE_IIC [SW_I2C]
**     Description :
**         Called when a correct character is received. In the SLAVE
**         mode, this event is not called if the component receives the
**         first byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void EE_IIC_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  EE_IIC_OnTxChar (module Events)
**
**     Component   :  EE_IIC [SW_I2C]
**     Description :
**         Called when a correct character is sent. In MASTER mode,
**         this event is not called if the component sends the first
**         byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void EE_IIC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  EE_IIC_OnNACK (module Events)
**
**     Component   :  EE_IIC [SW_I2C]
**     Description :
**         In the MASTER mode, this event is called when an invalid
**         slaves acknowledgement occurs during communication transfer.
**         If the acknowledge polling is provided (MASTER mode only), i.
**         e., the <Acknowledge polling trials> property value is
**         higher than one, this event is called only when no trial is
**         successful. In the SLAVE mode, this event is called when a
**         master sends an acknowledgement instead of no
**         acknowledgement at the end of the last byte transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void EE_IIC_OnNACK(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TIMER_IIC_OnRxChar (module Events)
**
**     Component   :  TIMER_IIC [SW_I2C]
**     Description :
**         Called when a correct character is received. In the SLAVE
**         mode, this event is not called if the component receives the
**         first byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TIMER_IIC_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TIMER_IIC_OnTxChar (module Events)
**
**     Component   :  TIMER_IIC [SW_I2C]
**     Description :
**         Called when a correct character is sent. In MASTER mode,
**         this event is not called if the component sends the first
**         byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TIMER_IIC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TIMER_IIC_OnNACK (module Events)
**
**     Component   :  TIMER_IIC [SW_I2C]
**     Description :
**         In the MASTER mode, this event is called when an invalid
**         slaves acknowledgement occurs during communication transfer.
**         If the acknowledge polling is provided (MASTER mode only), i.
**         e., the <Acknowledge polling trials> property value is
**         higher than one, this event is called only when no trial is
**         successful. In the SLAVE mode, this event is called when a
**         master sends an acknowledgement instead of no
**         acknowledgement at the end of the last byte transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TIMER_IIC_OnNACK(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_EXT_IIC_OnRxChar (module Events)
**
**     Component   :  SUMV_EXT_IIC [SW_I2C]
**     Description :
**         Called when a correct character is received. In the SLAVE
**         mode, this event is not called if the component receives the
**         first byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_EXT_IIC_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_EXT_IIC_OnTxChar (module Events)
**
**     Component   :  SUMV_EXT_IIC [SW_I2C]
**     Description :
**         Called when a correct character is sent. In MASTER mode,
**         this event is not called if the component sends the first
**         byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_EXT_IIC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_EXT_IIC_OnNACK (module Events)
**
**     Component   :  SUMV_EXT_IIC [SW_I2C]
**     Description :
**         In the MASTER mode, this event is called when an invalid
**         slaves acknowledgement occurs during communication transfer.
**         If the acknowledge polling is provided (MASTER mode only), i.
**         e., the <Acknowledge polling trials> property value is
**         higher than one, this event is called only when no trial is
**         successful. In the SLAVE mode, this event is called when a
**         master sends an acknowledgement instead of no
**         acknowledgement at the end of the last byte transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_EXT_IIC_OnNACK(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_IIC_OnRxChar (module Events)
**
**     Component   :  SUMV_IIC [SW_I2C]
**     Description :
**         Called when a correct character is received. In the SLAVE
**         mode, this event is not called if the component receives the
**         first byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_IIC_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_IIC_OnTxChar (module Events)
**
**     Component   :  SUMV_IIC [SW_I2C]
**     Description :
**         Called when a correct character is sent. In MASTER mode,
**         this event is not called if the component sends the first
**         byte with slave address and R/W bit.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_IIC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SUMV_IIC_OnNACK (module Events)
**
**     Component   :  SUMV_IIC [SW_I2C]
**     Description :
**         In the MASTER mode, this event is called when an invalid
**         slaves acknowledgement occurs during communication transfer.
**         If the acknowledge polling is provided (MASTER mode only), i.
**         e., the <Acknowledge polling trials> property value is
**         higher than one, this event is called only when no trial is
**         successful. In the SLAVE mode, this event is called when a
**         master sends an acknowledgement instead of no
**         acknowledgement at the end of the last byte transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SUMV_IIC_OnNACK(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  Timer_1ms_OnInterrupt (module Events)
**
**     Component   :  Timer_1ms [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Timer_1ms_OnInterrupt(void)
{
  /* Write your code here ... */
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
