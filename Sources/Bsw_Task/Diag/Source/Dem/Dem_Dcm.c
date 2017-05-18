/*============================================================================*/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Dcm.c>
 *  @brief      <Dem_Dcm>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <2015.3.16  - PM 4:19:57>
 */
/*============================================================================*/

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       2015.3.16     wbn        Initial version
 *
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_DCM_C_AR_MAJOR_VERSION   3U
#define DEM_DCM_C_AR_MINOR_VERSION   1U
#define DEM_DCM_C_AR_PATCH_VERSION   0U
#define DEM_DCM_C_SW_MAJOR_VERSION   1U
#define DEM_DCM_C_SW_MINOR_VERSION   0U
#define DEM_DCM_C_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Dcm.h"
#include "Dem_Common.h"
#include "Dem_EventMemory.h"
#include "Dem_EventDebounce.h"
#include "Dem_FreezeFrame.h"
#include "Dem_ExtendedData.h"
#include "Std_ExtendedTypes.h"
/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_DCM_C_AR_MAJOR_VERSION != DEM_COMMON_H_AR_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_MINOR_VERSION != DEM_COMMON_H_AR_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_PATCH_VERSION != DEM_COMMON_H_AR_PATCH_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MAJOR_VERSION != DEM_COMMON_H_SW_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MINOR_VERSION != DEM_COMMON_H_SW_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif

#if (DEM_DCM_C_AR_MAJOR_VERSION != DEM_DCM_H_AR_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_MINOR_VERSION != DEM_DCM_H_AR_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_PATCH_VERSION != DEM_DCM_H_AR_PATCH_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MAJOR_VERSION != DEM_DCM_H_SW_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MINOR_VERSION != DEM_DCM_H_SW_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
/********[M A C R O S]*********************************************************/
/* DTCSettingStatus */

typedef struct
{
    /* All Group */
    boolean AllGroupIsEnabled;
#if(DEM_GROUP_OF_DTC_NUM > 0)
    uint8 DTCGroupStatus[DEM_GROUP_OF_DTC_NUM_BYTE];
#endif
} Dem_DTCSettingInfoType;

/* DTC Filter */
typedef struct
{
    /* Filter */
    Dem_UdsStatusByteType DTCStatusMask;
    Dem_DTCKindType DTCKind;
    Dem_DTCFormatType DTCFormat;
    Dem_DTCOriginType DTCOrigin;
    boolean FilterWithSeverity;
    Dem_DTCSeverityType DTCSeverityMask;
    boolean FilterForFaultDetectionCounter;

    /* IsSet */
    boolean IsSet;
    /* Result */
    uint8 NumberOfFilteredDTC;
    uint8 GetNum;
    uint8 CurrentIndex;
    uint8 SetBits[DEM_MEM_DEST_MAX_ENTRY_NUM_BYTE];
} Dem_DTCFilterInfoType;

/* DTCByOccurrenceTime */
typedef struct
{
    Dem_EventIdType FirstFailed;
    Dem_EventIdType MostRecentFailed;
    Dem_EventIdType FirstDetConfirmed;
    Dem_EventIdType MostRecDetConfirmed;
} Dem_DTCByOccurrenceTimeType;

/* FreezeFrameRecordFilter */
typedef struct
{
    /* Filter */
    Dem_DTCFormatType DTCFormat;

    /* Result */
    uint16 NumberOfFilteredRecords;
    uint16 GetNum;
    uint8 MemDestIndex;
    uint8 EntryIndex;
    uint8 FFIndex;
} Dem_FreezeFrameRecordFilterInfoType;

typedef struct
{
    uint16 DTCIndex;
    boolean ClearAllGroup;
    uint8 DTCGroupIndex;
    uint8 MemDest;
} Dem_ClearDTCInfoType;

/********[I N T E R N A L   D A T A]*******************************************/
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_DTCSettingInfoType,AUTOMATIC) DemDTCSettingInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_DTCFilterInfoType,AUTOMATIC) DemDTCFilterInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_DTCByOccurrenceTimeType,AUTOMATIC) DemDTCByOccurrenceTimeInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_FreezeFrameRecordFilterInfoType,AUTOMATIC) DemFreezeFrameRecordFilterInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemDTCStatusChangedInd = FALSE;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemClearDTCSet = FALSE;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
STATIC VAR(Dem_ClearDTCInfoType,AUTOMATIC) DemClearDTCInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

/********[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/
#if(DEM_GROUP_OF_DTC_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint8, DEM_CODE) Dem_GetDTCGroupIndex(uint32 Id);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint16, DEM_CODE) Dem_GetDTCIndex(uint32 DTC,
    Dem_DTCFormatType DTCFormat);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_GetExtendedDataRecordCfg(uint8 ExtendedDataNumber);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 MemDest);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
#if(DEM_GROUP_OF_DTC_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 MemDest,
    uint8 GroupIndex);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 MemDest,
    uint16 DTCIndex);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/********[F U N C T I O N   I M P L E M E N T A T I O N S]*********************/
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
FUNC(void, DEM_CODE) Dem_DcmInit(void)
{
    DemDTCStatusChangedInd = TRUE;
    DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
#if(DEM_GROUP_OF_DTC_NUM > 0)
    Dem_MemSet((uint8*UDS_FAR)&DemDTCSettingInfo.DTCGroupStatus, 0xFF,
        sizeof(Dem_DTCSettingInfoType));
#endif
    Dem_MemSet((uint8*UDS_FAR)&DemDTCFilterInfo, 0x00, sizeof(Dem_DTCFilterInfoType));
    Dem_MemSet((uint8*UDS_FAR)&DemFreezeFrameRecordFilterInfo, 0x00,
        sizeof(Dem_FreezeFrameRecordFilterInfoType));

    DemDTCByOccurrenceTimeInfo.FirstDetConfirmed = DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.FirstFailed = DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.MostRecDetConfirmed =
    DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.MostRecentFailed = DEM_EVENT_PARAMETER_INVALID;

    DemClearDTCInfo.ClearAllGroup = FALSE;
    DemClearDTCInfo.DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
    DemClearDTCInfo.DTCIndex = DEM_DTC_INVALID;
    DemClearDTCInfo.MemDest = DEM_MEM_DEST_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the supported DTC formats of the ECU.>
 * @ServiceId           <0x3c>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Returns the configured DTC translation format. Acombination of
 *                              different DTC formats is not possible.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_DTCTranslationFormatType, DEM_CODE)
Dem_DcmGetTranslationType(void)
{
    return DEM_TYPE_OF_DTCSUPPORTED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the DTC Status availability mask.>
 * @ServiceId           <0x16>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <DTCStatusMask:The value DTCStatusMask indicates the supported DTC status
 *                              bits from the Dem.>
 * @Param-Name[in/out]  <none>
 * @Return              <E_OK: get of DTC status mask was successful
 *                       E_NOT_OK: get of DTC status mask failed>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTCStatusAvailabilityMask(
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask
)
{
    *DTCStatusMask = DEM_DTC_STATUS_AVAILABILITY_MASK;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the status of a DTC.>
 * @ServiceId           <0x15>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin: If the Dem supports more than one event memory this
 *                              parameter is used to select the source memory
 *                              the DTCs shall be read from.>
 * @Param-Name[out]     <DTCStatus: This parameter receives the status information of the
 *                          requested DTC. If the return value of the function call is
 *                          other than DEM_STATUS_OK this parameter does not contain valid
 *                          data. 0x00...0xFF match DTCStatusMask as defined in ISO14229-1>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnGetStatusOfDTCType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE) Dem_DcmGetStatusOfDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus
)
{
    uint8 MemDest;
    Dem_EventIdType EventId;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETSTATUSOFDTC,DEM_E_UNINIT,DEM_STATUS_FAILED);
    DEM_DEV_CHECK_RETURN((DTCStatus == NULL_PTR),DEM_SID_DCMGETSTATUSOFDTC,DEM_E_PARAM_POINTER,DEM_STATUS_FAILED);

    MemDest = Dem_GetInternalMemDest(DTCOrigin);
    if (MemDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_STATUS_WRONG_DTCORIGIN;
    }

    EventId = Dem_GetEventIdByDTC(DTC);
    if (EventId == DEM_EVENT_PARAMETER_INVALID)
    {
        return DEM_STATUS_WRONG_DTC;
    }
#if 0
    /**认为该处处理在“DemPrimaryMemory”这种存储方式时没有判断DTCStatus，不删除的原因是不清楚其他的存储方式是如何处理。**/
    pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(EventId), MemDest);
    if (pEntry == NULL_PTR)
    {
        return DEM_STATUS_FAILED;
    }
#endif
    pEvent = Dem_GetEventInfo(EventId);
    *DTCStatus = pEvent->UdsStatus;
    return DEM_STATUS_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the severity of the requested DTC.>
 * @ServiceId           <0x0e>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC : Diagnostic Trouble Code in UDS format.>
 * @Param-Name[out]     <DTCSeverity:This parameter contains the DTCSeverity according to ISO 14229-1.>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnGetSeverityOfDTCType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE) Dem_DcmGetSeverityOfDTC(
    uint32 DTC,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity
)
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETSEVERITYOFDTC,DEM_E_UNINIT,DEM_GET_SEVERITYOFDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((DTCSeverity == NULL_PTR),DEM_SID_DCMGETSEVERITYOFDTC,DEM_E_PARAM_POINTER,DEM_GET_SEVERITYOFDTC_WRONG_DTC);
    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    }
    *DTCSeverity = DemDTC[DTC].DemDTCSeverity;
    return DEM_GET_SEVERITYOFDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the functional unit of the requested DTC.>
 * @ServiceId           <0x34>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Diagnostic Trouble Code in UDS format.>
 * @Param-Name[out]     <DTCFunctionalUnit:Functional unit value of this DTC>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnGetFunctionalUnitOfDTCType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetFunctionalUnitOfDTCType, DEM_CODE) Dem_DcmGetFunctionalUnitOfDTC(
    uint32 DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
)
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETFUNCTIONALUNITOFDTC,DEM_E_UNINIT,DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((DTCFunctionalUnit == NULL_PTR),DEM_SID_DCMGETFUNCTIONALUNITOFDTC,DEM_E_PARAM_POINTER,DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC);
    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    }
    *DTCFunctionalUnit = DemDTC[DTC].DemDTCFunctionalUnit;
    return DEM_GET_FUNCTIONALUNITOFDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets the DTC Filter.>
 * @ServiceId           <0x13>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTCStatusMask:Status-byte mask for DTC status-byte filtering Values
 *                       DTCKind: Defines the functional group of DTCs to be reported
 *                       DTCFormat: Defines the output-format of the requested DTC values for the
 *                              sub-sequent API calls.
 *                       DTCOrigin: If the Dem supports more than one event memory this parameter
 *                              is used to select the source memory the DTCs shall be read from.
 *                       FilterWithSeverity: This flag defines whether severity information (ref.
 *                              to parameter below) shall be used for filtering
 *                       DTCSeverityMask: This parameter contains the DTCSeverityMask according
 *                              to ISO14229-1
 *                       FilterForFaultDetectionCounter: This flag defines whether the fault
 *                              detection counter information shall be used for filtering. >
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to (re-)set a DTC filter.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_DcmSetDTCFilter(
    Dem_UdsStatusByteType DTCStatusMask,
    Dem_DTCKindType DTCKind,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin,
    boolean FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean FilterForFaultDetectionCounter
    )
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT), DEM_SID_DCMSETDTCFILTER, DEM_E_UNINIT,
        DEM_WRONG_FILTER);
    DEM_DEV_CHECK_RETURN((DEM_DTC_FORMAT_UDS != DTCFormat), DEM_SID_DCMSETDTCFILTER,
        DEM_E_WRONG_CONFIGURATION, DEM_WRONG_FILTER);
    DEM_DEV_CHECK_RETURN((DEM_DTC_KIND_ALL_DTCS != DTCKind), DEM_SID_DCMSETDTCFILTER,
        DEM_E_WRONG_CONFIGURATION, DEM_WRONG_FILTER);

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    DEM_DEV_CHECK_RETURN((DTCOrigin == DEM_MEM_DEST_INVALID), DEM_SID_DCMSETDTCFILTER,
        DEM_E_WRONG_CONFIGURATION, DEM_WRONG_FILTER);

    if(DTCStatusMask == 0x00)
    {
        DTCStatusMask = 0xFF;
    }
    DemDTCFilterInfo.DTCStatusMask = DTCStatusMask;
    DemDTCFilterInfo.DTCKind = DTCKind;
    DemDTCFilterInfo.DTCFormat = DTCFormat;
    DemDTCFilterInfo.DTCOrigin = DTCOrigin; /* MemDestIndex */
    DemDTCFilterInfo.FilterWithSeverity = FilterWithSeverity;
    DemDTCFilterInfo.DTCSeverityMask = DTCSeverityMask;
    DemDTCFilterInfo.FilterForFaultDetectionCounter =
        FilterForFaultDetectionCounter;

    DemDTCFilterInfo.CurrentIndex = 0;
    DemDTCFilterInfo.NumberOfFilteredDTC = 0;
    DemDTCFilterInfo.GetNum = 0;
    Dem_MemSet((uint8*UDS_FAR)DemDTCFilterInfo.SetBits, 0x00,
    DEM_MEM_DEST_MAX_ENTRY_NUM_BYTE);
    DemDTCFilterInfo.IsSet = TRUE;
    return DEM_FILTER_ACCEPTED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the number of a filtered DTC.>
 * @ServiceId           <0x17>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <NumberOfFilteredDTC:The number of DTCs matching the defined status mask.>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve a number of DTC from the Dem>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
Dem_DcmGetNumberOfFilteredDTC(
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC
)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 iloop = 0;
    uint8 EntryNum;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETNUMBEROFFILTEREDDTC,DEM_E_UNINIT,DEM_NUMBER_FAILED);
    DEM_DEV_CHECK_RETURN((NumberOfFilteredDTC == NULL_PTR),DEM_SID_DCMGETNUMBEROFFILTEREDDTC,DEM_E_PARAM_POINTER,DEM_NUMBER_FAILED);

    if(DemDTCFilterInfo.IsSet == FALSE)
    {
        return DEM_NUMBER_FAILED;
    }

    EntryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while (iloop < EntryNum)
    {
        if (pEntry->EventId != 0x00)
        {
            pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
            if ((0x00 != (pEvent->UdsStatus & pFilter->DTCStatusMask))
            && ((pFilter->FilterWithSeverity == FALSE)
                || ((pFilter->FilterWithSeverity == TRUE)
                    && (0x00
                        != (pFilter->DTCSeverityMask
                            & DemDTC[DemEventParameter[pEvent->IntId]
                            .DemDTCRef].DemDTCSeverity))))
            && ((pFilter->FilterForFaultDetectionCounter == FALSE)
                || ((pFilter->FilterForFaultDetectionCounter == TRUE)
                    && (0 < Dem_GetInteralFDC(pEvent->IntId))))
            )
            {
                pFilter->NumberOfFilteredDTC++;
                DEM_BITS_SET(pFilter->SetBits, iloop);
            }
        }
        iloop++;
        pEntry++;
    }
    DemDTCFilterInfo.IsSet = FALSE;
    *NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
    return DEM_NUMBER_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the next filtered DTC matching the filter criteria. >
 * @ServiceId           <0x18>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <DTC : Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCStatus: This parameter receives the status information of
 *                          the requested DTC. >
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve a DTC from the Dem. >
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 EntryNum;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETNEXTFILTEREDDTC,DEM_E_UNINIT,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTC == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTC,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTCStatus == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTC,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);

    EntryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < EntryNum)
    && (pFilter->GetNum < pFilter->NumberOfFilteredDTC))
    {
        if (0x00 != DEM_BITS_ISSET(pFilter->SetBits, pFilter->CurrentIndex))
        {
            pEvent = Dem_GetEventInfo(
            Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId));
            Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
            *DTCStatus = pEvent->UdsStatus;
            res = DEM_FILTERED_OK;
            pFilter->GetNum++;
        }
        pFilter->CurrentIndex++;
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the next filtered DTC and its associated Fault Detection Counter
 *                      (FDC) matching the filter criteria. >
 * @ServiceId           <0x3b>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <DTC: Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCFaultDetectionCounter:This parameter receives the Fault Detection
 *                          Counter information of the requested DTC. >
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve a DTC from the Dem.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndFDC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 EntryNum;
    Dem_EventIdType IntId;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC,DEM_E_UNINIT,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTCFaultDetectionCounter == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);

    EntryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < EntryNum)
    && (pFilter->GetNum < pFilter->NumberOfFilteredDTC))
    {
        if (0x00 != DEM_BITS_ISSET(pFilter->SetBits, pFilter->CurrentIndex))
        {
            IntId = Dem_GetEventInternalId(
            pEntry[pFilter->CurrentIndex].EventId);
            Dem_GetEventDTC(IntId, pFilter->DTCFormat, DTC);
            *DTCFaultDetectionCounter = Dem_GetInteralFDC(IntId);
            res = DEM_FILTERED_OK;
            pFilter->GetNum++;
        }
        pFilter->CurrentIndex++;
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the next filtered DTC and its associated Severity matching the
 *                          filter criteria.>
 * @ServiceId           <0x3d>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <DTC:Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCStatus:This parameter receives the status information of
 *                          the requested DTC.
 *                       DTCSeverity: Receives the severity value returned by the function.
 *                       DTCFunctionalUnit: Receives the functional unit value returned by
 *                          the function.>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve a DTC from the Dem.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndSeverity(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 EntryNum;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY,DEM_E_UNINIT,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTC == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTCStatus == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTCSeverity == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTCFunctionalUnit == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);

    EntryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < EntryNum)
    && (pFilter->GetNum < pFilter->NumberOfFilteredDTC))
    {
        if (0x00 != DEM_BITS_ISSET(pFilter->SetBits, pFilter->CurrentIndex))
        {
            pEvent = Dem_GetEventInfo(
            Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId));
            Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
            *DTCStatus = pEvent->UdsStatus;
            *DTCSeverity = DemDTC[DemEventParameter[pEvent->IntId].DemDTCRef]
            .DemDTCSeverity;
            *DTCFunctionalUnit = DemDTC[DemEventParameter[pEvent->IntId]
            .DemDTCRef].DemDTCFunctionalUnit;
            res = DEM_FILTERED_OK;
            pFilter->GetNum++;
        }
        pFilter->CurrentIndex++;
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Sets a freeze frame record filter.>
 * @ServiceId           <0x3f>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTCFormat: Defines the output-format of the requested DTC values for the
 *                           sub-sequent API calls.>
 * @Param-Name[out]     <NumberOfFilteredRecords:Number of freeze frame records currently stored
 *                           in the event memory.>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to (re-)set a freeze frame record filter.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_DcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 MemDestIndex = 0;
    uint8 EntryNum = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemFreezeFrameRecordFilterInfo;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMSETFREEZEFRAMERECORDFILTER,DEM_E_UNINIT,DEM_WRONG_FILTER);
    DEM_DEV_CHECK_RETURN((DEM_DTC_FORMAT_UDS != DTCFormat),DEM_SID_DCMSETFREEZEFRAMERECORDFILTER,DEM_E_WRONG_CONFIGURATION,DEM_WRONG_FILTER);
    DEM_DEV_CHECK_RETURN((NumberOfFilteredRecords == NULL_PTR),DEM_SID_DCMSETFREEZEFRAMERECORDFILTER,DEM_E_PARAM_POINTER,DEM_WRONG_FILTER);

    pFilter->DTCFormat = DTCFormat;
    *NumberOfFilteredRecords = 0;
    while (MemDestIndex < DEM_MEM_DEST_TOTAL_NUM)
    {
        pEntry = DemMemDestCfg[MemDestIndex].EntryList;
        EntryNum = DemMemDestCfg[MemDestIndex].EntryNum;
        while (EntryNum > 0)
        {
            if (pEntry->EventId != 0x00)
            {
                *NumberOfFilteredRecords += pEntry->FFNum;
            }
            pEntry++;
            EntryNum--;
        }
        MemDestIndex++;
    }
    pFilter->GetNum = 0;
    pFilter->EntryIndex = 0;
    pFilter->FFIndex = 0;
    pFilter->MemDestIndex = 0;
    pFilter->NumberOfFilteredRecords = *NumberOfFilteredRecords;
    return DEM_FILTER_ACCEPTED;
#else
    return DEM_WRONG_FILTER;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the next freeze frame record number and its associated DTC
 *                          stored in the event memory. >
 * @ServiceId           <0x3a>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <DTC:Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       RecordNumber:Freeze frame record number of the reported DTC
 *                          (relative addressing). >
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve a DTC and its associated
 *                          snapshot record number from the Dem.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredRecord(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemFreezeFrameRecordFilterInfo;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETNEXTFILTEREDRECORD,DEM_E_UNINIT,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((DTC == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDRECORD,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    DEM_DEV_CHECK_RETURN((RecordNumber == NULL_PTR),DEM_SID_DCMGETNEXTFILTEREDRECORD,DEM_E_PARAM_POINTER,DEM_FILTERED_NO_MATCHING_ELEMENT);
    while ((res != DEM_FILTERED_OK)
    && (pFilter->GetNum < pFilter->NumberOfFilteredRecords)
    && (pFilter->MemDestIndex < DEM_MEM_DEST_TOTAL_NUM))
    {
        if (pFilter->EntryIndex
        >= DemMemDestCfg[pFilter->MemDestIndex].EntryNum)
        {
            pFilter->EntryIndex = 0;
            pFilter->FFIndex = 0;
            pFilter->MemDestIndex++;
        }
        else
        {
            pEntry = &DemMemDestCfg[pFilter->MemDestIndex].EntryList[pFilter
            ->EntryIndex];
            if (pFilter->FFIndex >= DEM_MAX_NUMBER_FF_RECORDS)
            {
                pFilter->FFIndex = 0;
                pFilter->EntryIndex++;
            }
            else
            {
                if (pEntry->FFList[pFilter->FFIndex].RecordNum != 0xFF)
                {
                    Dem_GetEventDTC(Dem_GetEventInternalId(pEntry->EventId),
                    pFilter->DTCFormat, DTC);
                    *RecordNumber = pEntry->FFList[pFilter->FFIndex].RecordNum;
                    res = DEM_FILTERED_OK;
                    pFilter->GetNum++;
                }
                pFilter->FFIndex++;
            }
        }
    }
    return res;
#else
    return DEM_FILTERED_NO_MATCHING_ELEMENT;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the DTC by occurrence time. >
 * @ServiceId           <0x19>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTCRequest:This parameter defines the request type of the DTC.>
 * @Param-Name[out]     <DTC: Receives the DTC value in UDS format returned by the function. >
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnGetDTCByOccurrenceTimeType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetDTCByOccurrenceTimeType, DEM_CODE)
Dem_DcmGetDTCByOccurrenceTime(
    Dem_DTCRequestType DTCRequest,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC
)
{
    Dem_EventIdType IntId = DEM_EVENT_PARAMETER_INVALID;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETDTCBYOCCURRENCETIME,DEM_E_UNINIT,DEM_OCCURR_NOT_AVAILABLE);
    DEM_DEV_CHECK_RETURN((DTC == NULL_PTR),DEM_SID_DCMGETDTCBYOCCURRENCETIME,DEM_E_PARAM_POINTER,DEM_OCCURR_NOT_AVAILABLE);
    switch (DTCRequest)
    {
        case DEM_FIRST_FAILED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.FirstFailed;
        break;
        case DEM_MOST_RECENT_FAILED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.MostRecentFailed;
        break;
        case DEM_FIRST_DET_CONFIRMED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.FirstDetConfirmed;
        break;
        case DEM_MOST_REC_DET_CONFIRMED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.MostRecDetConfirmed;
        break;
        default:
        break;
    }
    if ((IntId != DEM_EVENT_PARAMETER_INVALID)
    && (DemEventParameter[IntId].DemDTCRef != DEM_DTC_INVALID))
    {
        Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_UDS, DTC);
        return DEM_OCCURR_OK;
    }
    return DEM_OCCURR_NOT_AVAILABLE;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Controls the triggering of Dcm_DemTriggerOnDTCStatus.>
 * @ServiceId           <0xb0>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <TriggerNotification:This parameter specifies whether the triggering of
 *                          the notification shall be enabled (TRUE) or disabled (FALSE).>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DcmControlDTCStatusChangedNotification(
    boolean TriggerNotification
    )
{
    DemDTCStatusChangedInd = TriggerNotification;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[Access extended data records and FreezeFrame data]*************/
/*************************************************************************/
/*
 * @Brief               <Disables the event memory update of a specific DTC
 *                          (only one at one time).>
 * @ServiceId           <0x1a>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Selects the DTC in UDS format, for which DTC record update
 *                          shall be disabled.
 *                       DTCOrigin: If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          for which DTC record update shall be disabled.>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to disable the event memory
 *                          update of a specific DTC.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnDisableDTCRecordUpdateType, DEM_CODE)
Dem_DcmDisableDTCRecordUpdate(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin
    )
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),
        DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_UNINIT,
        DEM_DISABLE_DTCRECUP_WRONG_DTC);

    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_DISABLE_DTCRECUP_WRONG_DTC;
    }

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN;
    }

    DEM_BITS_SET(DemMemDestInfo[DTCOrigin].DTCRecordUpdateStatus, DTC);
    return DEM_DISABLE_DTCRECUP_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Disables the event memory update of a specific DTC
 *                          (only one at one time).>
 * @ServiceId           <0x1a>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Selects the DTC in UDS format, for which DTC record update
 *                          shall be disabled.
 *                       DTCOrigin: If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          for which DTC record update shall be disabled.>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to disable the event memory
 *                          update of a specific DTC.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName for V3.1.5>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE)
Dem_DisableDTCRecordUpdate(void)
{
    uint8 iloop = DEM_MEM_DEST_TOTAL_NUM;
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),
        DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_UNINIT, E_OK);
    while (iloop > 0)
    {
        iloop--;
        Dem_MemSet(DemMemDestInfo[iloop].DTCRecordUpdateStatus, 0xFF,
        DEM_DTC_NUM_BYTE);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Enables the event memory update of the DTC disabled by
 *                          Dem_DcmDisableDTCRecordUpdate() before.>
 * @ServiceId           <0x1b>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <none>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Always E_OK is returned.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmEnableDTCRecordUpdate(void)
{
    uint8 iloop = DEM_MEM_DEST_TOTAL_NUM;
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),
        DEM_SID_DCMENABLEDTCRECORDUPDATE, DEM_E_UNINIT, E_OK);
    while (iloop > 0)
    {
        iloop--;
        Dem_MemSet(DemMemDestInfo[iloop].DTCRecordUpdateStatus, 0x00,
        DEM_DTC_NUM_BYTE);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets freeze frame data by DTC. The function stores the data
 *                          in the provided DestBuffer.>
 * @ServiceId           <0x1d>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC : Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber:This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.>
 * @Param-Name[out]     <DestBuffer:This parameter contains a byte pointer that points
 *                          to the buffer, to which the freeze frame data record
 *                          shall be written to.>
 * @Param-Name[in/out]  <BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.>
 * @Return              <Status of the operation to retrieve freeze frame data by DTC.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr;
    uint16 DataSize;
    uint8 SRNid = 1;
    uint8*UDS_FAR src = NULL_PTR;
    uint16 DidRefNum;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pDidIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;
    
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETFREEZEFRAMEDATABYDTC,DEM_E_UNINIT,DEM_GET_FFDATABYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((DestBuffer == NULL_PTR),DEM_SID_DCMGETFREEZEFRAMEDATABYDTC,DEM_E_PARAM_POINTER,DEM_GET_FFDATABYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((BufSize == NULL_PTR),DEM_SID_DCMGETFREEZEFRAMEDATABYDTC,DEM_E_PARAM_POINTER,DEM_GET_FFDATABYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((RecordNumber == 0xFF),DEM_SID_DCMGETFREEZEFRAMEDATABYDTC,DEM_E_PARAM_DATA,DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER);
    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_GET_FFDATABYDTC_WRONG_DTC;
    }

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN;
    }
    pEntry = Dem_MemEntryGetByDTC(DTC, DTCOrigin);
    if (pEntry == NULL_PTR)
    {
        *BufSize = 0;
        return DEM_GET_FFDATABYDTC_OK;
    }
    pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNumber);
    if (pFF == NULL_PTR)
    {
        *BufSize = 0;
        return DEM_GET_FFDATABYDTC_OK;
    }
    pDTCAttr = Dem_EventDTCAttributesCfg(Dem_GetEventInternalId(pEntry->EventId));
    
    #if 0
    DataSize = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].DataSize;
    if (*BufSize < DataSize)
    {
        return DEM_GET_FFDATABYDTC_WRONG_BUFFERSIZE;
    }
    Dem_MemCopy(DestBuffer, pFF->Data, DataSize);
    *BufSize = DataSize;
    
    #else
    /* add by zheng.fang 2016.11.28 */
    *BufSize = 0;
    if (pDTCAttr->DemFreezeFrameClassRef == DEM_FREEZE_FRAME_INVALID)
    {
    	 return DEM_GET_FFDATABYDTC_WRONG_DTC;
    }
    DidRefNum = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].RefNum;
    pDidIndex = &DemDidClassRef[DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].StartIndex];
    src = pFF->Data;
    
    while (DidRefNum > 0)
    {
        DidRefNum--;
        pDid = &DemDidClass[*pDidIndex];
        if (pDid->ReadDataFnc != NULL_PTR)
        {
          *(DestBuffer++) = SRNid;
          *(DestBuffer++) = (uint8)((pDid->DemDidIdentifier) >> 8);
          *(DestBuffer++) = (uint8)(pDid->DemDidIdentifier);
          Dem_MemCopy(DestBuffer, src, pDid->DataSize);
          DestBuffer += pDid->DataSize;
          src += pDid->DataSize;
          SRNid++;
          *(uint8*UDS_FAR)BufSize += (3 + pDid->DataSize);
        }
        pDidIndex++;
    } 
    #endif
    return DEM_GET_FFDATABYDTC_OK;
#else
    return DEM_GET_FFDATABYDTC_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <0x1f>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC : Diagnostic Trouble Code in UDS format
 *                       DTCOrigin : If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1. >
 * @Param-Name[out]     <SizeOfFreezeFrame:>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC,DEM_E_UNINIT,DEM_GETSIZEBYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((SizeOfFreezeFrame == NULL_PTR),DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC,DEM_E_PARAM_POINTER,DEM_GETSIZEBYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((RecordNumber == 0xFF),DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC,DEM_E_PARAM_DATA,DEM_GETSIZEBYDTC_WRONG_DTC);
    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }

    *SizeOfFreezeFrame = 0;
    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
    }
    pEntry = Dem_MemEntryGetByDTC(DTC, DTCOrigin);
    if (pEntry == NULL_PTR)
    {
        return DEM_GETSIZEBYDTC_OK;
    }
    pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNumber);
    if (pFF == NULL_PTR)
    {
        return DEM_GETSIZEBYDTC_OK;
    }
    pDTCAttr = Dem_EventDTCAttributesCfg(pEntry->EventId);
    *SizeOfFreezeFrame = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef]
    .DataSize;
    return DEM_GETSIZEBYDTC_OK;
#else
    return DEM_GETSIZEBYDTC_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets extended data by DTC. The function stores the data in
 *                          the provided DestBuffer.>
 * @ServiceId           <0x20>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       ExtendedDataNumber:dentification/Number of requested extended data
 *                       record. The values 0xFE and 0xFF are not allowed.>
 * @Param-Name[out]     <DestBuffer:This parameter contains a byte pointer that points to
 *                          the buffer, to which the extended data record shall
 *                          be written to. >
 * @Param-Name[in/out]  <BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer. >
 * @Return              <Status of the operation to retrieve extended data by DTC.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_DcmGetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC,DEM_E_UNINIT,DEM_RECORD_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((DestBuffer == NULL_PTR),DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC,DEM_E_PARAM_POINTER,DEM_RECORD_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((BufSize == NULL_PTR),DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC,DEM_E_PARAM_POINTER,DEM_RECORD_WRONG_BUFFERSIZE);
    DEM_DEV_CHECK_RETURN((ExtendedDataNumber >= 0xFE),DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC,DEM_E_PARAM_DATA,DEM_RECORD_WRONG_NUMBER);
    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_RECORD_WRONG_DTC;
    }

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_RECORD_WRONG_DTCORIGIN;
    }
    pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
    if (pExtDataCfg == NULL_PTR)
    {
        return DEM_RECORD_WRONG_NUMBER;
    }

    if (*BufSize < pExtDataCfg->DataSize)
    {
        return DEM_RECORD_WRONG_BUFFERSIZE;
    }
    pEntry = Dem_MemEntryGetByDTC(DTC, DTCOrigin);
    if (pEntry == NULL_PTR)
    {
        *BufSize = 0;
        return DEM_RECORD_OK;
    }
    if (E_OK
    != Dem_ExtendedDataGetFromEntry(pEntry, ExtendedDataNumber, DestBuffer))
    {
        *BufSize = 0;
        return DEM_RECORD_OK;
    }
    *(uint8*UDS_FAR)BufSize = pExtDataCfg->DataSize;
    return DEM_RECORD_OK;
#else
    return DEM_RECORD_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Gets the size of extended data by DTC.>
 * @ServiceId           <0x21>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:Diagnostic Trouble Code in UDS format.
 *                      DTCOrigin:If the Dem supports more than one event memory ,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                      ExtendedDataNumber:Identification/Number of requested extended
 *                          data record. >
 * @Param-Name[out]     <SizeOfExtendedDataRecord:Size of the requested extended data record(s)
 *                          including record number size>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation to retrieve the size of extended data.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
)
{
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg;

    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC,DEM_E_UNINIT,DEM_GETSIZEBYDTC_WRONG_DTC);
    DEM_DEV_CHECK_RETURN((SizeOfExtendedDataRecord == NULL_PTR),DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC,DEM_E_PARAM_POINTER,DEM_GETSIZEBYDTC_WRONG_DTC);

    DTC = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (DTC == DEM_DTC_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    *SizeOfExtendedDataRecord = 0;
    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
    }
    pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
    if (pExtDataCfg == NULL_PTR)
    {
        return DEM_GETSIZEBYDTC_OK;
    }
    pEntry = Dem_MemEntryGetByDTC(DTC, DTCOrigin);
    if (pEntry == NULL_PTR)
    {
        return DEM_GETSIZEBYDTC_OK;
    }
    *SizeOfExtendedDataRecord = pExtDataCfg->DataSize;
    return DEM_GETSIZEBYDTC_OK;
#else
    return DEM_GETSIZEBYDTC_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/********[DTC storage]*********/
/*************************************************************************/
/*
 * @Brief               <Clears single DTCs, as well as groups of DTCs.>
 * @ServiceId           <0x23>
 * @Sync/Async          <Asynchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnClearDTCType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmClearDTC(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin
    )
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr;

    pClr = &DemClearDTCInfo;
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),DEM_SID_DCMCLEARDTC,DEM_E_UNINIT,DEM_CLEAR_FAILED);
    if (DemClearDTCSet == TRUE)
    {
        return DEM_CLEAR_BUSY;
    }
    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_CLEAR_WRONG_DTCORIGIN;
    }
    if ((DTC & 0xFFFFFF) == 0xFFFFFF)
    {
        pClr->ClearAllGroup = TRUE;
    }
#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
    else
    {
        pClr->ClearAllGroup = FALSE;
#if(DEM_GROUP_OF_DTC_NUM > 0)
        pClr->DTCGroupIndex = Dem_GetDTCGroupIndex(DTC);
        if (pClr->DTCGroupIndex == DEM_GROUP_OF_DTC_INVALID)
#endif
        {
            pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
            if (pClr->DTCIndex == DEM_DTC_INVALID)
            {
                return DEM_CLEAR_WRONG_DTC;
            }
        }
    }
#endif
    pClr->MemDest = DTCOrigin;
    DemClearDTCSet = TRUE;
    return DEM_CLEAR_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * @Brief               <Disables the DTC setting for a DTC group.>
 * @ServiceId           <0x24>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTCGroup:Defines the group of DTC that shall be disabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind, either
 *                          only OBD-relevant DTCs or all DTCs>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Returns status of the operation>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
/*
 * The following DTC groups are provided:
 * 'all DTCs' DTC group (mandatory, fixed value = 0xFFFFFF)
 *  Emission related DTCs for WWH-OBD (0xFFFF33) */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
Dem_ReturnControlDTCSettingType Dem_DcmDisableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind /* not used */
    )
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),
        DEM_SID_DCMDISABLEDTCSETTING, DEM_E_UNINIT,
        DEM_CONTROL_DTC_SETTING_N_OK);
#if(DEM_GROUP_OF_DTC_NUM > 0)

    if ((DTCGroup & 0xFFFFFF) == 0xFFFFFF)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0x00,
        DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        DTCGroup = Dem_GetDTCGroupIndex(DTCGroup);
        if (DTCGroup == DEM_GROUP_OF_DTC_INVALID)
        {
            return DEM_CONTROL_DTC_WRONG_DTCGROUP;
        }
        DEM_BITS_CLR(DemDTCSettingInfo.DTCGroupStatus, DTCGroup);
    }
#else
    DEM_DEV_CHECK_RETURN(((DTCGroup&0xFFFFFF)!= 0xFFFFFF),0x24,DEM_E_WRONG_CONFIGURATION,DEM_CONTROL_DTC_SETTING_N_OK);
    DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
#endif
    Dem_SetDTCSettingProcess();
    return DEM_CONTROL_DTC_SETTING_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <Enables the DTC setting for a DTC group.>
 * @ServiceId           <0x25>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTCGroup:Defines the group of DTC that shall be enabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind,
 *                          either only OBD-relevant DTCs or all DTCs>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <Returns the status of the operation>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmEnableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind
    )
{
    DEM_DEV_CHECK_RETURN((DemInitState != DEM_STATE_INIT),
        DEM_SID_DCMENABLEDTCSETTING, DEM_E_UNINIT,
        DEM_CONTROL_DTC_SETTING_N_OK);
#if(DEM_GROUP_OF_DTC_NUM > 0)
    if ((DTCGroup & 0xFFFFFF) == 0xFFFFFF)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = TRUE;    /* TRUE 20170322 Dem测试*/
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0xFF,
        DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        DTCGroup = Dem_GetDTCGroupIndex(DTCGroup);
        if (DTCGroup == DEM_GROUP_OF_DTC_INVALID)
        {
            return DEM_CONTROL_DTC_WRONG_DTCGROUP;
        }
        DEM_BITS_SET(DemDTCSettingInfo.DTCGroupStatus, DTCGroup);
    }
#else
    DEM_DEV_CHECK_RETURN(((DTCGroup&0xFFFFFF)!= 0xFFFFFF),0x25,DEM_E_WRONG_CONFIGURATION,DEM_CONTROL_DTC_SETTING_N_OK);
    DemDTCSettingInfo.AllGroupIsEnabled = TRUE;  /* TRUE 20170322 Dem测试*/
#endif
    Dem_SetDTCSettingProcess();
    return DEM_CONTROL_DTC_SETTING_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/
/*************************************************************************/
/*
 * @Brief               <Dem_SetDTCSettingProcess>
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
STATIC FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void)
{
    uint16 iloop = 0;
    P2VAR(Dem_EventInfoType, AUTOMATIC, DEM_VAR) pEvent;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;

    pEventCfg = DemEventParameter;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        pEvent = Dem_GetEventInfo(iloop);
#if(DEM_GROUP_OF_DTC_NUM > 0)
        if ((pEventCfg->DemDTCRef != DEM_DTC_INVALID)
            && (DemDTC[pEventCfg->DemDTCRef].GroupRef
                != DEM_GROUP_OF_DTC_INVALID))
        {
            if (0x00
                == DEM_BITS_ISSET(DemDTCSettingInfo.DTCGroupStatus,
                    DemDTC[pEventCfg->DemDTCRef].GroupRef))
            {
                DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                Dem_DebounceFreeze(iloop);
            }
            else
            {
                if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                {
                    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                    /* @req SWS_Dem_00682 Monitor re-initialization  */
                    Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_REENABLED);
                }
            }
        }
        else
        #endif
        {
            if (DemDTCSettingInfo.AllGroupIsEnabled == FALSE)
            {
                DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                Dem_DebounceFreeze(iloop);
            }
            else
            {
                if(0x00 == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                {
                    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
                    /* @req SWS_Dem_00682 Monitor re-initialization  */
                    Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_REENABLED);
                }
            }
        }
        pEventCfg++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_GROUP_OF_DTC_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <GetDTCGroupIndex>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant/Non Reentrant>
 * @Param-Name[in]      <Id>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <DTCGroupIndex>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint8, DEM_CODE) Dem_GetDTCGroupIndex(uint32 Id)
{
    uint8 iloop = DEM_GROUP_OF_DTC_NUM;

    while (iloop > 0)
    {
        iloop--;
        if (DemGroupOfDTC[iloop] == Id)
        {
            return iloop;
        }
    }
    return DEM_GROUP_OF_DTC_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <DTC && DTCFormat>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <DTC Index>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(uint16, DEM_CODE) Dem_GetDTCIndex(uint32 DTC,
    Dem_DTCFormatType DTCFormat)
{
    uint16 index = DEM_DTC_NUM;

    if (DTCFormat != DEM_DTC_FORMAT_UDS)
    {
        return DEM_DTC_INVALID;
    }
    while (index > 0)
    {
        index--;
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
        if (DemPbCfgPtr->DemDtcValue[index] == DTC)
        #else
        if (DemDTC[index].DemDtcValue == DTC)
#endif
        {
            return index;
        }
    }
    return DEM_DTC_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant/Non Reentrant>
 * @Param-Name[in]      <IntId && DTCFormat>
 * @Param-Name[out]     <DTC>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_GetEventDTC(Dem_EventIdType IntId,
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint32, AUTOMATIC, DEM_VAR) DTC)
{
    switch (DTCFormat)
    {
        case DEM_DTC_FORMAT_UDS:
#if(DEM_CONFIG_VARIANT == DEM_CONFIG_POST_BUILD)
        *DTC = DemPbCfgPtr->DemDtcValue[DemEventParameter[IntId].DemDTCRef];
#else
        *DTC = DemDTC[DemEventParameter[IntId].DemDTCRef].DemDtcValue;
#endif
        break;
        default:
        /* TODO: */
        *DTC = 0x00;
        break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant/Non Reentrant>
 * @Param-Name[in]      <IntId && Status>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_SetOccurrenceEvent(Dem_EventIdType IntId,
    uint8 Status)
{
    switch (Status)
    {
        case DEM_UDS_STATUS_TF:
            if (DemDTCByOccurrenceTimeInfo.FirstFailed
                == DEM_EVENT_PARAMETER_INVALID)
            {
                DemDTCByOccurrenceTimeInfo.FirstFailed = IntId;
            }
            DemDTCByOccurrenceTimeInfo.MostRecentFailed = IntId;
            break;
        case DEM_UDS_STATUS_CDTC:
            if (DemDTCByOccurrenceTimeInfo.FirstDetConfirmed
                == DEM_EVENT_PARAMETER_INVALID)
            {
                DemDTCByOccurrenceTimeInfo.FirstDetConfirmed = IntId;
            }
            DemDTCByOccurrenceTimeInfo.MostRecDetConfirmed = IntId;
            break;
        default:
            break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <BRIEF DESCRIPTION>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant>
 * @Param-Name[in]      <ExtendedDataNumber>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <const Dem_ExtendedDataRecordClassType*>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_GetExtendedDataRecordCfg(uint8 ExtendedDataNumber)
{
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg;
    uint8 iloop = DEM_EXTENDED_DATA_RECORD_CLASS_NUM;

    pExtDataCfg = DemExtendedDataRecordClass;

    while (iloop > 0)
    {
        iloop--;
        if (pExtDataCfg->DemExtendedDataRecordNumber == ExtendedDataNumber)
        {
            return pExtDataCfg;
        }
        pExtDataCfg++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * @Brief               <ClearAllDTC>
 * @ServiceId           <-->
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Reentrant/Non Reentrant>
 * @Param-Name[in]      <MemDest>
 * @Param-Name[out]     <none>
 * @Param-Name[in/out]  <none>
 * @Return              <none>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
STATIC FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 MemDest)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    uint8 iloop = DemMemDestCfg[MemDest].EntryNum;
    uint16 tempEventId = 0;
    pEntry = DemMemDestCfg[MemDest].EntryList;

    while (iloop > 0)
    {
        iloop--;
        /* @req SWS_Dem_00680 Monitor re-initialization  */
        Dem_InitMonitorForEvent(iloop,DEM_INIT_MONITOR_CLEAR);
        tempEventId = pEntry->EventId;
        Dem_MemEntryDelete(pEntry);        
        Dem_ResetUdsStatus(tempEventId);
        DemUpdateNvmFlag[iloop] = TRUE;
        DemSetClearDtcFlag = TRUE;
        pEntry++;
    }
    DemMemDestInfo[MemDest].OverFlow = FALSE;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
#if(DEM_GROUP_OF_DTC_NUM > 0)
/*************************************************************************/
/*
 * @Brief               <ClearGroupDTC>
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
STATIC FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 MemDest,
    uint8 GroupIndex)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[MemDest].EntryList;
    uint8 iloop = DemMemDestCfg[MemDest].EntryNum;
    uint16 Ref;
    boolean ClearSet = FALSE;
    uint16 tempEventId = 0;
    while (iloop > 0)
    {
        iloop--;
        if (pEntry->EventId != 0x00)
        {
            Ref = DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)]
            .DemDTCRef;
            if (DemDTC[Ref].GroupRef == GroupIndex)
            {
                /* @req SWS_Dem_00680 Monitor re-initialization  */
                Dem_InitMonitorForEvent(Dem_GetEventInternalId(pEntry->EventId),DEM_INIT_MONITOR_CLEAR);
                tempEventId = pEntry->EventId;
                Dem_MemEntryDelete(pEntry);
                Dem_ResetUdsStatus(tempEventId);
                DemUpdateNvmFlag[iloop] = TRUE;
                ClearSet = TRUE;
            }
        }
        pEntry++;
    }
    if (ClearSet == TRUE)
    {
        DemMemDestInfo[MemDest].OverFlow = FALSE;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * @Brief               <ClearOneDTC>
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
STATIC FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 MemDest,
    uint16 DTCIndex)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    uint16 tempEventId = 0;
    pEntry = Dem_MemEntryGetByDTC(DTCIndex, MemDest);
    if (pEntry != NULL_PTR)
    {
        /* @req SWS_Dem_00680 Monitor re-initialization  */
        Dem_InitMonitorForEvent(Dem_GetEventInternalId(pEntry->EventId),DEM_INIT_MONITOR_CLEAR);
        tempEventId = pEntry->EventId;
        Dem_MemEntryDelete(pEntry);
        Dem_ResetUdsStatus(tempEventId);
        DemUpdateNvmFlag[Dem_GetEventInternalId(tempEventId)] = TRUE;
        DemMemDestInfo[MemDest].OverFlow = FALSE;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * @Brief               <ClearDTCProcess>
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
FUNC(void, DEM_CODE) Dem_ClearDTCProcess(void)
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr = &DemClearDTCInfo;

    if (pClr->ClearAllGroup == TRUE)
    {
        Dem_ClearAllDTC(pClr->MemDest);
        pClr->ClearAllGroup = FALSE;
    }
#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
    else
    {
        if (pClr->DTCGroupIndex != DEM_GROUP_OF_DTC_INVALID)
        {
#if(DEM_GROUP_OF_DTC_NUM > 0)
            Dem_ClearGroupDTC(pClr->MemDest, pClr->DTCGroupIndex);
#endif
            pClr->DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
        }
        else
        {
            if (pClr->DTCIndex != DEM_DTC_INVALID)
            {
                Dem_ClearOneDTC(pClr->MemDest, pClr->DTCIndex);
                pClr->DTCIndex = DEM_DTC_INVALID;
            }
        }
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/********[E N D   O F   F I L E]***********************************************/
