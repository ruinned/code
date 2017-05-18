/*============================================================================*/
/** Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Os.c>
 *  @brief      <>
 *  
 *  <>
 *  
 *  @author     <ISOFT>
 *  @date       <2013-04-09>
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"
#include "Std_ExtendedTypes.h"

#pragma push
#pragma DATA_SEG SHARED_DATA
volatile TickType SysTick;
#pragma pop

/*************************************************************************/
/*
 * Brief               <return the current tick value of the counter>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <CounterID: The Counter which tick value should be read;
 *                      Value: Contains the current tick value of the counter;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <StatusType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
StatusType GetCounterValue( CounterType  CounterID,
		                        TickRefType  Value  )
{
    (*Value) = SysTick;
     return(E_OK);
}

/*************************************************************************/
/*
 * Brief               <return the current tick value of the counter>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <CounterID: The Counter to be read;
 *                      Value: The previously read tick value of the counter;>
 *
 * Param-Name[out]     <Value: Contains the current tick value of the counter;
 *                      ElapsedValue: The difference to the previous read value;>
 * Param-Name[in/out]  <None>
 * Return              <StatusType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
StatusType GetElapsedCounterValue(	CounterType   CounterID,
									                  TickRefType   Value,
									                  TickRefType   ElapsedValue  )
{
	(*ElapsedValue) =(TickType) (SysTick-(*Value));
	(*Value) =  SysTick;   /*chenms 2013-9-27 11:34*/
	return(E_OK);
}
