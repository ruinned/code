/*================================================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <ComM_Nm.h>
 *  @brief      <Communication Manager Module Header File> 
 *  @author     <pinghai.xiong>
 *  @date       <2014-02-24>
 */
/*================================================================================================*/
#ifndef COMM_NM_H
#define COMM_NM_H

/*=======[R E V I S I O N   H I S T O R Y]========================================================*/
/*  <VERSION>    <DATE>    <AUTHOR>         <REVISION LOG>
 *  V1.0.0       20140224  pinghai.xiong    Initial version
 * 
 */
/*================================================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===================================================*/
#define COMM_NM_H_AR_MAJOR_VERSION  2U
#define COMM_NM_H_AR_MINOR_VERSION  1U
#define COMM_NM_H_AR_PATCH_VERSION  0U
#define COMM_NM_H_SW_MAJOR_VERSION  1U
#define COMM_NM_H_SW_MINOR_VERSION  0U
#define COMM_NM_H_SW_PATCH_VERSION  0U

/*=======[I N C L U D E S]========================================================================*/
#include "ComStack_Types.h"
#include "ComM_Cfg.h"

/**************************************************************************************************/
/*
 * Brief: Indication that a NM-message has been received in the Bus Sleep Mode, what indicates 
 * that some nodes in the network have already entered the Network Mode.
 * ServiceId: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: NmIf
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication(NetworkHandleType Channel);

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Network Mode.
 * ServiceId: 0x18
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Nm_NetworkMode(NetworkHandleType Channel);

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Prepare Bus-Sleep Mode.
 * ServiceId: 0x19
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel);

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Bus-Sleep Mode.
 * ServiceId: 0x1A
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode(NetworkHandleType Channel);

/**************************************************************************************************/
/*
 * Brief: If NmIf has started to shut down the coordinated busses, AND not all coordinated busses 
 *        have indicated bus sleep state, AND on at least on one of the coordinated busses NM is 
 *        restarted, THEN the NM Interface shall call the callback function ComM_Nm_RestartIndication
 *        with the nmNetworkHandle of the channels which have already indicated bus sleep state.
 * ServiceId: 0x1B
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Nm_RestartIndication(NetworkHandleType Channel);

#endif /*#ifndef COMM_NM_H*/

