/** ###################################################################
**     Filename  : mcal.c
**     Project   : mcal
**     Processor : MC9S12XEP100MAG
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2013/9/12, 14:56
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE mcal */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TimerEvents.h"
#include "CAN0Events.h"
#include "CAN1Events.h"
#include "CAN4Events.h"
#include "AC_CP_IOCEvents.h"
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
//#include "CPU_TLE_CS.h"
//#include "CPU_TLE_SCK.h"
//#include "CPU_TLE_SDI.h"
//#include "CPU_TLE_SDO.h"
//#include "CPU_TLE_CO1.h"
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
//#include "OV_Power.h"
//#include "UV_Power.h"
#include "CPU_SUMV_SDA.h"
#include "CPU_SUMV_SCL.h"
#include "CPU_SUMV_RDY.h"
#include "SUMV_POW.h"
//#include "CPU_SUMV_EXT_SDA.h"
//#include "CPU_SUMV_EXT_SCL.h"
//#include "CPU_SUMV_EXT_RDY.h"
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
#include "AC_CP_IOC.h"
#include "RELAY_CHG.h"
#include "RELAY_PRE.h"
#include "RELAY_NEG.h"
#include "RELAY_POS.h"
#include "CPUDO2.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "PE_cfg.h"
#ifdef PE_CBK_EN
#include "Sch_Cbk.h"
//#include "Sch_Main.h"
#endif
#include "Mcu.h"

void main(void)
{
  /* Write your local variable definition here */
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  
  /* Write your code here */
  
  #ifdef PE_CBK_EN
  Sch_MainInit(); 
  #endif 
  
  for(;;)
  {
    #ifdef PE_CBK_EN
      Sch_MainCallback();

    #endif
      Dem_UpdateEntry();
      Dem_ClearAllInfo(); 
  }
  
  

  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END mcal */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.04 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
