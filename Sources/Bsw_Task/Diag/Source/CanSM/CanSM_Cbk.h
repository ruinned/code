/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM_Cbk.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/
#ifndef CANSM_CBK_H
#define CANSM_CBK_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_CBK_H_AR_MAJOR_VERSION  1U
#define CANSM_CBK_H_AR_MINOR_VERSION  2U
#define CANSM_CBK_H_AR_PATCH_VERSION  0U
#define CANSM_CBK_H_SW_MAJOR_VERSION  1U
#define CANSM_CBK_H_SW_MINOR_VERSION  0U
#define CANSM_CBK_H_SW_PATCH_VERSION  0U
/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"

/* @req CANSM011 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * @brief        :       The CanSM is notified about a bus-off event on a certain
 *                       CAN controller with this call-out function. It shall execute
 *                       the bus-off recovery state machine for the corresponding
 *                       network handle.
 * Service ID    :       <SERVICE_ID_CANSM_CONTROLLERBUSOFF>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       CAN controller: which detected a bus-off event
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       CanIf
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
extern FUNC(void, CANSM_CODE)
CanSM_ControllerBusOff
(
    uint8 Controller
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /* CANSM_CBK_H */

/*=======[E N D   O F   F I L E]==============================================*/
