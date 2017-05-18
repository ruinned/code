/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Common.c>
 *  @brief      <Dem_Common>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 2:38:35>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4  wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_COMMON_C_AR_MAJOR_VERSION   3U
#define DEM_COMMON_C_AR_MINOR_VERSION   1U
#define DEM_COMMON_C_AR_PATCH_VERSION   0U
#define DEM_COMMON_C_SW_MAJOR_VERSION   1U
#define DEM_COMMON_C_SW_MINOR_VERSION   0U
#define DEM_COMMON_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"

/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_COMMON_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_Common.c : Mismatch in Specification Major Version"
#endif
#if (DEM_COMMON_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_Common.c : Mismatch in Specification Major Version"
#endif
#if (DEM_COMMON_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_Common.c : Mismatch in Specification Major Version"
#endif
#if (DEM_COMMON_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_Common.c : Mismatch in Specification Major Version"
#endif
#if (DEM_COMMON_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_Common.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/

/********[I N T E R N A L   D A T A]*******************************************/

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <MemSet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Val && Size>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <Dest>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MemSet(
    P2VAR(uint8, AUTOMATIC, DEM_VAR) Dest,
    uint8 Val,
    uint32 Size)
{
    while (Size > 0)
    {
        *(Dest++) = Val;
        Size--;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <MemCopy>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <Src && Size>
 * @Param-Name[out]     <Dest>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MemCopy(
    P2VAR(uint8, AUTOMATIC, DEM_VAR) Dest,
    P2CONST(uint8, AUTOMATIC, DEM_VAR) Src,
    uint32 Size)
{
    while (Size > 0)
    {
        *(Dest++) = *(Src++);
        Size--;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <CheckCondictionFulfilled>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <cond && group && len>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <boolean>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(boolean, DEM_CODE) Dem_CheckCondictionFulfilled(
    P2CONST(uint8, AUTOMATIC, DEM_VAR) cond,
    P2CONST(uint8, AUTOMATIC, DEM_VAR) group,
    uint8 len)
{
    boolean res = TRUE;

    while ((len > 0) && (res != FALSE))
    {
        len--;
        if (((*cond) & (*group)) != *group)
        {
            res = FALSE;
        }
        else
        {
            cond++;
            group++;
        }
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DTC_ATTRIBUTES_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <Get EventDTCAttributesCfg>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <const Dem_DTCAttributesType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_EventDTCAttributesCfg(Dem_EventIdType IntId)
{
    uint16 Ref;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pRes = NULL_PTR;

    /* DtcRef */
    Ref = DemEventParameter[IntId].DemDTCRef;
    if (Ref != DEM_DTC_INVALID)
    {
        /* DTCAttributesRef */
        Ref = DemDTC[Ref].DemDTCAttributesRef;
        if (Ref != DEM_DTC_ATTRIBUTES_INVALID)
        {
            pRes = &DemDTCAttributes[Ref];
        }
    }

    return pRes;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * @Brief               <GetInternalMemDest>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCOrigin>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <uint8>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint8, DEM_CODE) Dem_GetInternalMemDest(Dem_DTCOriginType DTCOrigin)
{
    uint8 iloop = DEM_MEM_DEST_TOTAL_NUM;

    while (iloop > 0)
    {
        iloop--;
        if (DemMemDestCfg[iloop].ExtId == DTCOrigin)
        {
            return iloop;
        }
    }
    return DEM_MEM_DEST_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_GetEventIdByDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTC>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_EventIdType>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_EventIdType, DEM_CODE) Dem_GetEventIdByDTC(uint32 DTC)
{
    Dem_EventIdType id = 0;

    while (id < DEM_EVENT_PARAMETER_NUM)
    {
        if ((DemEventParameter[id].DemDTCRef != DEM_DTC_INVALID)
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
            && (DemPbCfgPtr->DemDtcValue[DemEventParameter[id].DemDTCRef] == DTC)
#else
            && (DemDTC[DemEventParameter[id].DemDTCRef].DemDtcValue == DTC)
#endif
            )
        {
            return id;
        }
        id++;
    }
    return DEM_EVENT_PARAMETER_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant/Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_TriggerOnEventStatus(Dem_EventIdType IntId,
    Dem_UdsStatusByteType OldStatus,
    Dem_UdsStatusByteType NewStatus)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemEventParameter[IntId];
    uint8 iloop;
#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    P2CONST(Dem_TriggerOnEventStatusType, AUTOMATIC, DEM_CONST) pCbk =
        &DemCallbackEventStatusChanged[pEventCfg->StatusChangedCbkStartIndex];
#endif
    uint32 DTC = 0x00;

    if (DemDTCStatusChangedInd == TRUE)
    {
        Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_UDS, &DTC);
        if (DTC != 0x00)
        {
            /* TODO: Dcm_DemTriggerOnDTCStatus(DTC,OldStatus,NewStatus);*/
#if(DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0)
            iloop = DEM_CALLBACK_DTC_STATUS_CHANGED_NUM;
            while(iloop > 0)
            {
                iloop--;
                if(DemCallbackDTCStatusChanged[iloop] != NULL_PTR)
                {
                    (DemCallbackDTCStatusChanged[iloop])(DTC,OldStatus,NewStatus);
                }
            }
#endif
        }
    }
#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    IntId = Dem_GetEventExternalId(IntId);
    iloop = pEventCfg->StatusChangedCbkNum;
    while (iloop > 0)
    {
        if (*pCbk != NULL_PTR)
        {
            (*pCbk)(IntId, OldStatus, NewStatus);
        }
        pCbk++;
        iloop--;
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/

/********[E N D   O F   F I L E]***********************************************/
