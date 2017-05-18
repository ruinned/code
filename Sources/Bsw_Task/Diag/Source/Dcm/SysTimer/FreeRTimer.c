/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <FreeRTimer.c>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms    Initial version                         */
/*============================================================================*/

/******************************* references ************************************/
#include "Dcm_Include.h"

/****************************** implementations ********************************/
TickType Frt_ReadOutTicks(void)
{
	TickType  OSCurTicks;

	(void)GetCounterValue(DCM_REFERENCE_OS_COUNTER_ID,
			              &OSCurTicks);
    return(OSCurTicks);
}

TickType Frt_CalculateElapsedTicks(TickType OldCurTickValue)
{
	TickType  ElapsedTicks;

	(void)GetElapsedCounterValue(DCM_REFERENCE_OS_COUNTER_ID,
								 &OldCurTickValue,
								 &ElapsedTicks);
    return(ElapsedTicks);
}

/*******************internal functions****************************/
