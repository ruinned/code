/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <EcuM.h>
 *  @brief      <EcuM>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Sep 17, 2014 - 9:43:57 AM>
 */
/*============================================================================*/

#ifndef ECUM_H_ 
#define ECUM_H_

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Sep 17, 2014    wbn    		Initial version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define ECUM_H_MODULE_ID            0U
#define ECUM_H_VENDOR_ID          0U
#define ECUM_H_AR_MAJOR_VERSION   1U
#define ECUM_H_AR_MINOR_VERSION   3U
#define ECUM_H_AR_PATCH_VERSION   0U
#define ECUM_H_SW_MAJOR_VERSION   1U
#define ECUM_H_SW_MINOR_VERSION   0U
#define ECUM_H_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
typedef uint8 EcuM_StateType;

#define ECUM_STATE_RESET 0x50

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_Init>
 * @ServiceId           <0x00>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <main>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_Init(void);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_MainFunction>
 * @ServiceId           <0x01>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <main>
 */
/******************************************************************************/
extern FUNC(void, ECUM_CODE) EcuM_MainFunction(void);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_ComM_HasRequestedRUN>
 * @ServiceId           <0x06>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Channel>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <boolean>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <ComM_Xxx>
 */
/******************************************************************************/
extern FUNC(boolean, ECUM_CODE) EcuM_ComM_HasRequestedRUN(NetworkHandleType Channel);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_ComM_RequestRUN>
 * @ServiceId           <0x07>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Channel>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Std_ReturnType>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <ComM_Xxx>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_RequestRUN(NetworkHandleType Channel);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/******************************************************************************/
/*
 * @Brief               <EcuM_ComM_ReleaseRUN>
 * @ServiceId           <0x08>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Channel>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Std_ReturnType>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <ComM_Xxx>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_ReleaseRUN(NetworkHandleType Channel);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"


/******************************************************************************/
/*
 * @Brief               <EcuM_SelectShutdownTarget>
 * @ServiceId           <0x09>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <x,y>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <ComM_Xxx>
 */
/******************************************************************************/
#define EcuM_SelectShutdownTarget(x,y) E_OK

/******************************************************************************/
/*
 * @Brief               <EcuM_KillAllRUNRequests>
 * @ServiceId           <0x0A>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Channel>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <EcuM_Init>
 * @CallByAPI           <ComM_Xxx>
 */
/******************************************************************************/
#define EcuM_KillAllRUNRequests()

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* ECUM_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
