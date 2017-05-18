/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <EcuM_Cbk.h>
 *  @brief      <EcuM_Cbk>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Sep 17, 2014 - 9:43:49 AM>
 */
/*============================================================================*/

#ifndef ECUM_CBK_H_ 
#define ECUM_CBK_H_

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Sep 17, 2014    wbn    		Initial version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define ECUM_CBK_H_MODULE_ID            0U
#define ECUM_CBK_H_VENDOR_ID          0U
#define ECUM_CBK_H_AR_MAJOR_VERSION   0U
#define ECUM_CBK_H_AR_MINOR_VERSION   0U
#define ECUM_CBK_H_AR_PATCH_VERSION   0U
#define ECUM_CBK_H_SW_MAJOR_VERSION   1U
#define ECUM_CBK_H_SW_MINOR_VERSION   0U
#define ECUM_CBK_H_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "EcuM_Types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_CheckWakeup>
 * @ServiceId           <0x03>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <wks>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <...>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_CheckWakeup(EcuM_WakeupSourceType wks);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_SetWakeupEvent>
 * @ServiceId           <0x02>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <wks>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <...>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_SetWakeupEvent(EcuM_WakeupSourceType wks);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_ValidateWakeupEvent>
 * @ServiceId           <0x04>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <wks>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <....>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType wks);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_CheckValidation>
 * @ServiceId           <0x05>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <wks>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <...>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_CheckValidation(EcuM_WakeupSourceType wks);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* ECUM_CBK_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
