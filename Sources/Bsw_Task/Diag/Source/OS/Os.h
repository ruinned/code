/*============================================================================*/
/** Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Os.h>
 *  @brief      <>
 *  
 *  <>
 *  
 *  @author     <ISOFT>
 *  @date       <2012-09-14>
 */
/*============================================================================*/
#ifndef OS_H
#define OS_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Types.h"
#include "Std_ExtendedTypes.h"

#define TICK_DURATION 	  1000      /* the duration of ticks of the system in us */
//#define MS_PER_TICK       (1)       /* the duration of ticks of the system in ms */

extern  volatile TickType SysTick;
extern  StatusType GetCounterValue( CounterType  CounterID,
		                                TickRefType  Value);

StatusType GetElapsedCounterValue(	CounterType   CounterID,
		                                TickRefType   Value,
		                                TickRefType   ElapsedValue);

#endif
