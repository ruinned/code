/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_FreezeFrame.c>
 *  @brief      <Dem_FreezeFrame>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4 - PM 2:50:07>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4     wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_FREEZEFRAME_C_AR_MAJOR_VERSION   3U
#define DEM_FREEZEFRAME_C_AR_MINOR_VERSION   1U
#define DEM_FREEZEFRAME_C_AR_PATCH_VERSION   0U
#define DEM_FREEZEFRAME_C_SW_MAJOR_VERSION   1U
#define DEM_FREEZEFRAME_C_SW_MINOR_VERSION   0U
#define DEM_FREEZEFRAME_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"
#include "Dem_FreezeFrame.h"
#include "Std_ExtendedTypes.h"
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_FREEZEFRAME_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif

#if (DEM_FREEZEFRAME_C_AR_MAJOR_VERSION != DEM_FREEZEFRAME_H_AR_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_MINOR_VERSION != DEM_FREEZEFRAME_H_AR_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_PATCH_VERSION != DEM_FREEZEFRAME_H_AR_PATCH_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MAJOR_VERSION != DEM_FREEZEFRAME_H_SW_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MINOR_VERSION != DEM_FREEZEFRAME_H_SW_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
typedef struct
{
    Dem_EventIdType ExtId;
    uint8 FFData[DEM_FREEZE_FRAME_MAX_LEN];
} Dem_PreStoreFFInfoType;

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_PreStoreFFInfoType,AUTOMATIC) DemPreStoreFFInfo[DEM_MAX_NUMBER_PRESTORED_FF];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_PreStoreFFGet(Dem_EventIdType ExtId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/********[I N T E R N A L   D A T A]*******************************************/

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFramseSave>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <pFFBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_FreezeFramseSave(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFBuffer,
P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameInit>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <None>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_FreezeFrameInit(void)
{
    uint8 iloop = DEM_MAX_NUMBER_PRESTORED_FF;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = DemPreStoreFFInfo;

    while (iloop > 0)
    {
        iloop--;
        pPreFF->ExtId = 0x00;
        Dem_MemSet(pPreFF->FFData, 0xFF, DEM_FREEZE_FRAME_MAX_LEN);
        pPreFF++;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId>
 * @Param-Name[out]     <pBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_FreezeFrameGet(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer)
{
    uint16 Ref;
    uint16 DidRefNum;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pDidIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;

    /* DtcRef */
    Ref = DemEventParameter[IntId].DemDTCRef;
    /* DTCAttributesRef */
    Ref = DemDTC[Ref].DemDTCAttributesRef;
    /* FreezeFrameRef */
    Ref = DemDTCAttributes[Ref].DemFreezeFrameClassRef;
    if (Ref == DEM_FREEZE_FRAME_INVALID)
    {
        return;
    }
    DidRefNum = DemFreezeFrameClass[Ref].RefNum;
    pDidIndex = &DemDidClassRef[DemFreezeFrameClass[Ref].StartIndex];
    while (DidRefNum > 0)
    {
        DidRefNum--;
        pDid = &DemDidClass[*pDidIndex];
        if (pDid->ReadDataFnc != NULL_PTR)
        {
            if (E_OK != pDid->ReadDataFnc(pBuffer, pDid->DataSize))
            {
                /* SWS_Dem_00463 */
                Dem_MemSet(pBuffer, 0xFF, pDid->DataSize);
            }
        }
        pBuffer += pDid->DataSize;
        pDidIndex++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckFreezeFrameIsStorage>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && Trigger>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <boolean>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(boolean, DEM_CODE) Dem_CheckFreezeFrameIsStorage(Dem_EventIdType IntId,
    uint8 Trigger)
{
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    boolean res = FALSE;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
    uint8 iloop;

    pFFRecordIndex =
        &DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg
            ->DemFreezeFrameRecNumClassRef].StartIndex];
    iloop = DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef]
        .RefNum;
    while ((iloop > 0) && (res == FALSE))
    {
        iloop--;
        if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
        {
            pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
            if (pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
            {
                res = TRUE;
            }
        }
        pFFRecordIndex++;
    }
    return res;
#else
    return FALSE;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameStorage>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer && Trigger>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pEntry>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
uint8 Trigger)
{
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF = NULL_PTR;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
    uint8 iloop;
#endif

    if (pDTCAttrCfg == NULL_PTR)
    {
        return;
    }
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    if (pDTCAttrCfg->DemFreezeFrameRecNumClassRef
    == DEM_FREEZE_FRAME_REC_NUM_INVALID)
    {
        return;
    }
    pFFRecordIndex =
    &DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg
    ->DemFreezeFrameRecNumClassRef].StartIndex];
    iloop = DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef]
    .RefNum;
    while (iloop > 0)
    {
        iloop--;
        if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
        {
            pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
            if (pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
            {
                pFF = &(pEntry->FFList[iloop]);
                if (pFF->RecordNum == 0xFF)
                {
                    pEntry->FFNum++;
                }
                if ((pFF->RecordNum == 0xFF)
                || (pFFRecordCfg->DemFreezeFrameRecordUpdate == TRUE))
                {
                	pEntry->FFNum++;     /*add by fangzheng*/
                	pFF->RecordNum = pFFRecordCfg->DemFreezeFrameRecordNumber;
                    Dem_FreezeFramseSave(IntId, pFF->Data, pEventBuffer);
                    pEntry->LatestFF = iloop;
                }
            }
        }
        pFFRecordIndex++;
    }
#else
    if(pEntry->FFNum < pDTCAttrCfg->DemMaxNumberFreezeFrameRecords)
    {
        pFF = &pEntry->FFList[pEntry->FFNum];
        pEntry->LatestFF = pEntry->FFNum;
        pEntry->FFNum++;
        pFF->RecordNum = pEntry->LatestFF;
        Dem_FreezeFramseSave(IntId,pFF->Data,pEventBuffer);
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGetByRecordNum>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEntry && RecordNum>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_FreezeFrameInfoType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_FreezeFrameGetByRecordNum(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum)
{
    uint8 iloop = 0;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF = pEntry->FFList;
    if (pEntry->FFNum == 0)
    {
        return NULL_PTR;
    }
    if (RecordNum == 0xFF)
    {
        return &(pEntry->FFList[pEntry->LatestFF]);
    }
    while (iloop < DEM_MAX_NUMBER_FF_RECORDS)
    {
        if (pFF->RecordNum == RecordNum)
        {
            return pFF;
        }
        iloop++;
        pFF++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFrameGetFromEntry>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <pEntry && RecordNum && Did>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <pBuffer>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_FreezeFrameGetFromEntry(
P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
uint8 RecordNum,
uint16 Did,
P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer)
{
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF = NULL_PTR;
    uint16 Ref;
    uint16 DidRefNum;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pDidIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pData;

    if (pEntry->FFNum == 0)
    {
        return DEM_E_NODATAAVAILABLE;
    }
    if (RecordNum == 0xFF)
    {
        Dem_MemCopy(pBuffer, pEntry->FFList[pEntry->LatestFF].Data,
        DEM_FREEZE_FRAME_MAX_LEN);
        return E_OK;
    }
    pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNum);
    if (pFF == NULL_PTR)
    {
        return DEM_E_WRONG_RECORDNUMBER;
    }
    pData = pFF->Data;
    /* DtcRef */
    Ref = DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef;
    /* DTCAttributesRef */
    Ref = DemDTC[Ref].DemDTCAttributesRef;
    /* FreezeFrameRef */
    Ref = DemDTCAttributes[Ref].DemFreezeFrameClassRef;
    DidRefNum = DemFreezeFrameClass[Ref].RefNum;
    pDidIndex = &DemDidClassRef[DemFreezeFrameClass[Ref].StartIndex];
    while (DidRefNum > 0)
    {
        DidRefNum--;
        pDid = &DemDidClass[*pDidIndex];
        if (pDid->DemDidIdentifier == Did)
        {
            Dem_MemCopy(pBuffer, pData, pDid->DataSize);
            return E_OK;
        }
        else
        {
            pData += pDid->DataSize;
        }
        pDidIndex++;
    }
    return DEM_E_WRONG_DIDNUMBER;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_PreStoreFF>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_PreStoreFF(Dem_EventIdType ExtId)
{
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = Dem_PreStoreFFGet(ExtId);

    if (pPreFF == NULL_PTR)
    {
        pPreFF = Dem_PreStoreFFGet(0x00);
        if (pPreFF == NULL_PTR)
        {
            return E_NOT_OK;
        }
        pPreFF->ExtId = ExtId;
    }

    Dem_FreezeFrameGet(Dem_GetEventInternalId(ExtId), pPreFF->FFData);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ClearPreStoreFF
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Std_ReturnType>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPreStoreFF(Dem_EventIdType ExtId)
{
    Std_ReturnType res = E_NOT_OK;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = Dem_PreStoreFFGet(ExtId);

    if (pPreFF != NULL_PTR)
    {
        pPreFF->ExtId = 0x00;
        Dem_MemSet(pPreFF->FFData, 0xFF, DEM_FREEZE_FRAME_MAX_LEN);
        res = E_OK;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * @Brief               <Dem_PreStoreFFGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtId>
 * @Param-Name[out]     <None>
 * @Param-Name[in/out]  <None>
 * @Return              <Dem_PreStoreFFInfoType*>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_PreStoreFFGet(Dem_EventIdType ExtId)
{
    uint8 iloop = DEM_MAX_NUMBER_PRESTORED_FF;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = DemPreStoreFFInfo;

    while (iloop > 0)
    {
        iloop--;
        if (pPreFF->ExtId == ExtId)
        {
            return pPreFF;
        }
        pPreFF++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * @Brief               <Dem_FreezeFramseSave>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId && pEventBuffer>
 * @Param-Name[out]     <pFFBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_FreezeFramseSave(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFBuffer,
    P2CONST(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
    {
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = NULL_PTR;
    if (DemEventParameter[IntId].DemFFPrestorageSupported == TRUE)
    {
        pPreFF = Dem_PreStoreFFGet(Dem_GetEventExternalId(IntId));
    }
    if (pPreFF != NULL_PTR)
    {
        Dem_MemCopy(pFFBuffer, pPreFF->FFData, DEM_FREEZE_FRAME_MAX_LEN);
        pPreFF->ExtId = 0x00;
        Dem_MemSet(pPreFF->FFData, 0xFF, DEM_FREEZE_FRAME_MAX_LEN);
        return;
    }
#endif

#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
    if (pEventBuffer != NULL_PTR)
    {
        Dem_MemCopy(pFFBuffer, pEventBuffer->FFData, DEM_FREEZE_FRAME_MAX_LEN);
        return;
    }
#endif
    Dem_FreezeFrameGet(IntId, pFFBuffer);
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

    /********[E N D   O F   F I L E]***********************************************/
#endif
