/*================================================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <ComM_BusSM.h>
 *  @brief      <Communication Manager Module Header File> 
 *  @author     <pinghai.xiong>
 *  @date       <2014-02-24>
 */
/*================================================================================================*/
#ifndef COMM_BUSSM_H
#define COMM_BUSSM_H

/*=======[R E V I S I O N   H I S T O R Y]========================================================*/
/*  <VERSION>    <DATE>    <AUTHOR>         <REVISION LOG>
 *  V1.0.0       20140224  pinghai.xiong    Initial version
 * 
 */
/*================================================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===================================================*/
#define COMM_BUSSM_H_AR_MAJOR_VERSION  2U
#define COMM_BUSSM_H_AR_MINOR_VERSION  1U
#define COMM_BUSSM_H_AR_PATCH_VERSION  0U
#define COMM_BUSSM_H_SW_MAJOR_VERSION  1U
#define COMM_BUSSM_H_SW_MINOR_VERSION  0U
#define COMM_BUSSM_H_SW_PATCH_VERSION  0U

/*=======[I N C L U D E S]========================================================================*/
#include "ComM.h"
#include "Std_ExtendedTypes.h"

/**************************************************************************************************/
/*
 * Brief: Indication of the actual bus mode bythe corresponding Bus State Manager. ComM shall 
 *        propagate the indicated state tothe users with means of the RTE. 
 * ServiceId: 0x33
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
    NetworkHandleType Channel,
    P2VAR(ComM_ModeType, AUTOMATIC, CANIF_APPL_DATA) ComMode
);

#endif /*#ifndef COMM_BUSSM_H*/

