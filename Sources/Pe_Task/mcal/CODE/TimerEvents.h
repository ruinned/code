/** ###################################################################
**     Filename  : TimerEvents.h
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

#ifndef __TimerEvents_H
#define __TimerEvents_H
/* MODULE TimerEvents */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "wdi.h"
#include "Timer.h"
#include "CAN0.h"
#include "CAN1.h"
#include "CAN4.h"
#include "CPU_CHG_S.h"
#include "CPU_CHG_P.h"
#include "ISO_CTRL_POS.h"
#include "ISO_CTRL_NEG.h"
#include "ISO_POW.h"
#include "CPUDI1.h"
#include "ExternFlash_Cs.h"
#include "ExternFlash_Sck.h"
#include "ExternFlash_Si.h"
#include "ExternFlash_So.h"
#include "RELAY_HEAT.h"
#include "RELAY_DIG_ST5.h"
#include "RELAY_FAN.h"
#include "RELAY_DIG_ST6.h"
#include "CPU_TLE_CO1.h"
#include "ExternTimer_Int.h"
#include "ExternTimer_Scl.h"
#include "ExternTimer_Sda.h"
#include "PORTA0.h"
#include "PORTA1.h"
#include "PORTA2.h"
#include "CON1.h"
#include "Slaver_Pow_Ctrl.h"
#include "CPUDI2.h"
#include "CPUDI3.h"
#include "CPUDI4.h"
#include "CPU_SUMV_SDA.h"
#include "CPU_SUMV_SCL.h"
#include "CPU_SUMV_RDY.h"
#include "SUMV_POW.h"
#include "CPU_SUMV_EXT_SDA.h"
#include "CPU_SUMV_EXT_SCL.h"
#include "CPU_ISO_SDA.h"
#include "CPU_ISO_SCL.h"
#include "ExternFlash_Rst.h"
#include "CPU_RLY_POS.h"
#include "CPU_RLY_NEG.h"
#include "CPU_RLY_PRE.h"
#include "CPU_RLY_CHG.h"
#include "CPU_RLY_CTRL.h"
#include "RELAY_DIG_ST34.h"
#include "RELAY_DIG_ST12.h"
#include "AI1.h"
#include "AI2.h"
#include "CHG_CAN_POW.h"
#include "CPUCAN_CAR_EN.h"
#include "CPUCAN_INT_EN.h"
#include "CURPOWER.h"
#include "CPUCINT.h"
#include "CPUCSO.h"
#include "CPUCSI.h"
#include "CPUCSCK.h"
#include "CPUCCS.h"
#include "ExtEE_Sck.h"
#include "ExtEE_Sda.h"
#include "CPU_CAR_CAN_STB.h"
#include "CPU_CAR_CAN_ERR.h"
#include "ExtEE_Wp.h"
#include "CPUDO1.h"
#include "CPU_RLY_HEAT.h"
#include "CPU_RLY_FAN.h"
#include "CPUEE.h"
#include "Timer_1ms.h"
#include "Lock_UnLock.h"
#include "Lock_Lock.h"
#include "LTC_SPI.h"
#include "LTC_CS.h"
#include "AC_CP_IOC.h"
#include "RELAY_CHG.h"
#include "RELAY_PRE.h"
#include "RELAY_NEG.h"
#include "RELAY_POS.h"
#include "CPUDO2.h"

#pragma CODE_SEG DEFAULT

void Timer_OnInterrupt(void);
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


/* END TimerEvents */
#endif /* __TimerEvents_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
