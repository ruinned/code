/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_ExtendedData.c>
 *  @brief      <Dem_ExtendedData>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.4  - PM 3:00:26>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.4  wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_EXTENDEDDATA_C_AR_MAJOR_VERSION   3U
#define DEM_EXTENDEDDATA_C_AR_MINOR_VERSION   1U
#define DEM_EXTENDEDDATA_C_AR_PATCH_VERSION   0U
#define DEM_EXTENDEDDATA_C_SW_MAJOR_VERSION   1U
#define DEM_EXTENDEDDATA_C_SW_MINOR_VERSION   0U
#define DEM_EXTENDEDDATA_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Common.h"
#include "Dem_ExtendedData.h"
/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_EXTENDEDDATA_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif

#if (DEM_EXTENDEDDATA_C_AR_MAJOR_VERSION != DEM_EXTENDEDDATA_H_AR_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_MINOR_VERSION != DEM_EXTENDEDDATA_H_AR_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_PATCH_VERSION != DEM_EXTENDEDDATA_H_AR_PATCH_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MAJOR_VERSION != DEM_EXTENDEDDATA_H_SW_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MINOR_VERSION != DEM_EXTENDEDDATA_H_SW_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)

/********[I N T E R N A L   D A T A]*******************************************/

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataGet>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <IntId >
 * @Param-Name[out]     <pBuffer>
 * @Param-Name[in/out]  <None>
 * @Return              <None>
 * @PreCondition        <None>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ExtendedDataGet(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop;
    Std_ReturnType res = E_NOT_OK;

    if ((pDTCAttrCfg == NULL_PTR )
        || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return;
    }
    iloop = DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum;
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop > 0)
    {
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            res = E_NOT_OK;
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if (pExtRecord->ReadDataFnc != NULL_PTR)
            {
                res = pExtRecord->ReadDataFnc(pBuffer,
                    pExtRecord->DataSize);
            }
            if (res != E_OK)
            {
                Dem_MemSet(pBuffer, 0xFF, pExtRecord->DataSize);
            }
            pBuffer += pExtRecord->DataSize;
        }
        pExtIndex++;
        iloop--;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataGetFromEntry>
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
FUNC(Std_ReturnType, DEM_CODE) Dem_ExtendedDataGetFromEntry(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(
        Dem_GetEventInternalId(pEntry->EventId));
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop;
   P2VAR(uint8, AUTOMATIC, DEM_VAR) pExt = pEntry->ExtData;

    iloop = DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum;
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop > 0)
    {
        iloop--;
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if (pExtRecord->DemExtendedDataRecordNumber == RecordNum)
            {
                if (0x00 == DEM_BITS_ISSET(pEntry->ExtStatus, iloop))
                {
                    return DEM_E_NODATAAVAILABLE;
                }
                else
                {
                    Dem_MemCopy(pBuffer, pExt, pExtRecord->DataSize);
                    return E_OK;
                }
            }
            else
            {
                pExt += pExtRecord->DataSize;
            }
        }
        pExtIndex++;
    }
    return DEM_E_WRONG_RECORDNUMBER;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_CheckExtendedDataIsStorage>
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
FUNC(boolean, DEM_CODE) Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId,uint8 Trigger)
{
    boolean res = FALSE;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop;

    if ((pDTCAttrCfg == NULL_PTR )
        || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return res;
    }
    iloop = DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum;
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while ((iloop > 0)&&(res == FALSE))
    {
        iloop--;
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if (pExtRecord->DemExtendedDataRecordTrigger == Trigger)
            {
                res = TRUE;
            }
        }
        pExtIndex++;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Dem_ExtendedDataStorage>
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
FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    P2VAR(Dem_EventBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    uint8 Trigger)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop;
    Std_ReturnType res = E_NOT_OK;
    uint8 Offset = 0;

    if ((pDTCAttrCfg == NULL_PTR )
        || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return;
    }
    iloop = DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum;
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop > 0)
    {
        iloop--;
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            res = E_NOT_OK;
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if ((pExtRecord->DemExtendedDataRecordTrigger == Trigger)
                && (pExtRecord->ReadDataFnc != NULL_PTR )
                && ((pExtRecord->DemExtendedDataRecordUpdate == TRUE)
                    || (0x00 == DEM_BITS_ISSET(pEntry->ExtStatus, iloop))))
            {
                DEM_BITS_SET(pEntry->ExtStatus, iloop);
            }
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
            if (pEventBuffer != NULL_PTR)
            {
                Dem_MemCopy(&(pEntry->ExtData[Offset]),
                    &(pEventBuffer->ExtData[Offset]), pExtRecord->DataSize);
            }
            else
#endif
            {
                if (pExtRecord->ReadDataFnc != NULL_PTR)
                {
                    res = pExtRecord->ReadDataFnc(
                        &(pEntry->ExtData[Offset]),
                        pExtRecord->DataSize);
                }
                if (res != E_OK)
                {
                    Dem_MemSet(&(pEntry->ExtData[Offset]), 0xFF,
                        pExtRecord->DataSize);
                }
            }

            Offset += pExtRecord->DataSize;
        }
        pExtIndex++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/

/********[E N D   O F   F I L E]***********************************************/
#endif
