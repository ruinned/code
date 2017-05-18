/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <EcuM.c>
 *  @brief      <EcuM>
 *  
 *  <Compiler: All> 
 *  
 *  @author     <wbn>
 *  @date       <Sep 17, 2014 - 9:44:09 AM>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Sep 17, 2014    wbn        Initial version
 *
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define ECUM_C_AR_MAJOR_VERSION   0U
#define ECUM_C_AR_MAJOR_VERSION   0U
#define ECUM_C_AR_MINOR_VERSION   0U
#define ECUM_C_AR_PATCH_VERSION   0U
#define ECUM_C_SW_MAJOR_VERSION   1U
#define ECUM_C_SW_MINOR_VERSION   0U
#define ECUM_C_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "EcuM.h"
#include "EcuM_Types.h"
#include "ComM_EcuM.h"
#include "ComM_Cfg.h"
#include "ComM.h"
#include "CanIfDiag.h"
#include "Can_Cfg.h"

/*=======[V E R S I O N  C H E C K]===========================================*/


/*=======[M A C R O S]========================================================*/
typedef struct EcuM_InfoTypeTag
{
    uint32 AlreadyRun;
    uint32 RequestRun;
}EcuM_InfoType;

/*=======[I N T E R N A L   D A T A]==========================================*/
#define ECUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "EcuM_MemMap.h"
STATIC VAR(EcuM_InfoType, ECUM_VAR_POWER_ON_INIT) EcuMInfo;
#define ECUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"
extern CONST(EcuM_WksCfgInfoType, ECUM_CONST)  EcuMWksCfgInfo[ECUM_WKS_NUM];
#define ECUM_START_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
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
FUNC(void, ECUM_CODE) EcuM_Init(void)
{
    EcuMInfo.AlreadyRun = 0x00u;
    EcuMInfo.RequestRun = 0x00u;
}
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
FUNC(void, ECUM_CODE) EcuM_MainFunction(void)
{
    uint8 chid = 0u;
    while(EcuMInfo.RequestRun !=0u)
    {
        if(EcuMInfo.RequestRun & (1 << chid))
        {
            #if (STD_ON == COMM_ECUM_USED)
            ComM_EcuM_RunModeIndication(chid);
            #endif
            ComM_RequestComMode(chid,COMM_FULL_COMMUNICATION);
            EcuMInfo.RequestRun &= ~(1 << chid);
            EcuMInfo.AlreadyRun |= (1 << chid);
        }
        chid++;
    }
}
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
FUNC(void, ECUM_CODE) EcuM_SetWakeupEvent(EcuM_WakeupSourceType wks)
{
#if (STD_ON == COMM_ECUM_USED)
    uint8 iloop = 0u;

    while(iloop < ECUM_WKS_NUM)
    {
        if(wks&EcuMWksCfgInfo[iloop].Id)
        {
            ComM_EcuM_WakeUpIndication(EcuMWksCfgInfo[iloop].ComMChannelIdRef);
        }
        iloop++;
    }
#endif
}
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
FUNC(void, ECUM_CODE) EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType wks)
{

}
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
FUNC(void, ECUM_CODE) EcuM_CheckValidation(EcuM_WakeupSourceType wks)
{
    
}
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
FUNC(boolean, ECUM_CODE) EcuM_ComM_HasRequestedRUN(NetworkHandleType Channel)
{
    boolean res = FALSE;

    if(EcuMInfo.AlreadyRun & (1 << Channel))
    {
        res = TRUE;
    }
    return res;
}
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
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_RequestRUN(NetworkHandleType Channel)
{
    if(!(EcuMInfo.AlreadyRun & (1 << Channel)))
    {
        EcuMInfo.RequestRun |= (uint32)(1 << Channel);
    }
    return E_OK;
}
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
FUNC(Std_ReturnType, ECUM_CODE)  EcuM_ComM_ReleaseRUN(NetworkHandleType Channel)
{
    EcuMInfo.RequestRun &= (uint32)(~((1 << Channel)));
    EcuMInfo.AlreadyRun &= (uint32)(~((1 << Channel)));

    return E_OK;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

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
FUNC(void, ECUM_CODE) EcuM_CheckWakeup(EcuM_WakeupSourceType wks)
{
  //  CanIf_CheckWakeup(wks);
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/*=======[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]====*/


/*=======[E N D   O F   F I L E]==============================================*/
