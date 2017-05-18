/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Dcm.h>
 *  @brief      <Dem_Dcm>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Dec 26, 2014 - 10:17:33 AM>
 */
/*============================================================================*/

#ifndef DEM_DCM_H_
#define DEM_DCM_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Dec 26, 2014    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_DCM_H_MODULE_ID            54U
#define DEM_DCM_H_VENDOR_ID          0U
#define DEM_DCM_H_AR_MAJOR_VERSION   3U
#define DEM_DCM_H_AR_MINOR_VERSION   1U
#define DEM_DCM_H_AR_PATCH_VERSION   0U
#define DEM_DCM_H_SW_MAJOR_VERSION   1U
#define DEM_DCM_H_SW_MINOR_VERSION   0U
#define DEM_DCM_H_SW_PATCH_VERSION   0U

/*Switch for implementation version*/
#define DEM_IMPLEMENT_VERSION_3_X    STD_OFF
/********[I N C L U D E S]*****************************************************/
#include "Dem_Types.h"

typedef uint8 Dem_FilterForFDCType;
#define DEM_FILTER_FOR_FDC_NO FALSE
#define DEM_FILTER_FOR_FDC_YES TRUE

typedef uint8 Dem_FilterWithSeverityType;
#define DEM_FILTER_WITH_SEVERITY_NO FALSE
#define DEM_FILTER_WITH_SEVERITY_YES TRUE

typedef uint8 Dem_ReturnSetDTCFilterType;

typedef Dem_ReturnGetNextFilteredElementType Dem_ReturnGetNextFilteredDTCType;
#define DEM_FILTERED_NO_MATCHING_DTC DEM_FILTERED_NO_MATCHING_ELEMENT
#define DEM_FILTERED_WRONG_DTCKIND 0x04

/********[Access DTCs and Status Information]********************************/
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)Dem_GetTranslationType(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_DTCTranslationFormatType, DEM_CODE)
Dem_DcmGetTranslationType(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetTranslationType() Dem_DcmGetTranslationType()
#endif

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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCStatusAvailabilityMask(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTCStatusAvailabilityMask(
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetDTCStatusAvailabilityMask(DTCStatusMask) \
    Dem_DcmGetDTCStatusAvailabilityMask(DTCStatusMask)
#endif
/*************************************************************************/
/*
 * @Brief               <Gets the status of a DTC.>
 * @ServiceId           <0x15>
 * @Sync/Async          <Synchronous/Asynchronous>
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE) Dem_GetStatusOfDTC(uint32 DTC,
    											Dem_DTCKindType DTCKind,
    											Dem_DTCOriginType DTCOrigin,
    											P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE)
Dem_DcmGetStatusOfDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetStatusOfDTC(DTC, DTCKind, DTCOrigin,DTCStatus) \
    Dem_DcmGetStatusOfDTC(DTC,DTCOrigin,DTCStatus)
#endif
/*************************************************************************/
/*
 * @Brief               <Gets the severity of the requested DTC.>
 * @ServiceId           <0x0e>
 * @Sync/Async          <Synchronous/Asynchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC : Diagnostic Trouble Code in UDS format.>
 * @Param-Name[out]     <DTCSeverity:This parameter contains the DTCSeverity according to ISO 14229-1.>
 * @Param-Name[in/out]  <none>
 * @Return              <Status of the operation of type Dem_ReturnGetSeverityOfDTCType.>
 * @PreCondition        <none>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE) Dem_GetSeverityOfDTC(
    uint32 DTC,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE)
Dem_DcmGetSeverityOfDTC(
    uint32 DTC,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetSeverityOfDTC(DTC, DTCSeverity) \
    Dem_DcmGetSeverityOfDTC(DTC, DTCSeverity)
#endif
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
extern FUNC(Dem_ReturnGetFunctionalUnitOfDTCType, DEM_CODE)
Dem_DcmGetFunctionalUnitOfDTC(
    uint32 DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
);
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnSetDTCFilterType, DEM_CODE) Dem_SetDTCFilter(uint8 DTCStatusMask,
        Dem_DTCKindType DTCKind, Dem_DTCOriginType DTCOrigin,
        Dem_FilterWithSeverityType filterWithSeverity,
        Dem_DTCSeverityType DTCSeverityMask,
        Dem_FilterForFDCType filterForFaultDetectionCounter);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_DcmSetDTCFilter(
    Dem_UdsStatusByteType DTCStatusMask,
    Dem_DTCKindType DTCKind,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin,
    boolean FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean FilterForFaultDetectionCounter
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_SetDTCFilter(DTCStatusMask,DTCKind,DTCOrigin,FilterWithSeverity,\
                        DTCSeverityMask,FilterForFaultDetectionCounter) \
    Dem_DcmSetDTCFilter(DTCStatusMask,DTCKind,DEM_DTC_FORMAT_UDS,DTCOrigin,\
        FilterWithSeverity,DTCSeverityMask,FilterForFaultDetectionCounter)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
		Dem_GetNumberOfFilteredDTC(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) numberOfFilteredDTC);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
Dem_DcmGetNumberOfFilteredDTC(
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetNumberOfFilteredDTC(NumberOfFilteredDTC) \
    Dem_DcmGetNumberOfFilteredDTC(NumberOfFilteredDTC)
#endif
/*************************************************************************/
/*
 * @Brief               <Gets the next filtered DTC matching the filter criteria. >
 * @ServiceId           <0x18>
 * @Sync/Async          <Synchronous/Asynchronous>
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredDTCType, DEM_CODE)
		Dem_GetNextFilteredDTC(
				P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
				P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetNextFilteredDTC(DTC, DTCStatus) \
    Dem_DcmGetNextFilteredDTC(DTC,DTCStatus)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)
Dem_GetNextFilteredDTCAndFDC(
		P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
		P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndFDC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetNextFilteredDTCAndFDC(DTC, DTCFaultDetectionCounter) \
    Dem_DcmGetNextFilteredDTCAndFDC(DTC, DTCFaultDetectionCounter)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndSeverity(
		P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndSeverity(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetNextFilteredDTCAndSeverity(DTC,DTCStatus,DTCSeverity,DTCFunctionalUnit) \
    Dem_DcmGetNextFilteredDTCAndSeverity(DTC,DTCStatus,DTCSeverity,DTCFunctionalUnit)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnSetDTCFilterType, DEM_CODE)
		Dem_SetDTCFilterForRecords(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA)  NumberOfFilteredRecords);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_DcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_SetDTCFilterForRecords(NumberOfFilteredRecords) \
    Dem_DcmSetFreezeFrameRecordFilter(DEM_DTC_FORMAT_UDS,NumberOfFilteredRecords)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredDTCType, DEM_CODE)
		Dem_GetNextFilteredRecord(P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
				P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) SnapshotRecord);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredRecord(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetNextFilteredRecord(DTC, SnapshotRecord) \
    Dem_DcmGetNextFilteredRecord(DTC, SnapshotRecord)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)
Dem_GetDTCByOccurrenceTime(
    uint8 DTCRequest,
    Dem_DTCKindType DTCKind,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetDTCByOccurrenceTimeType, DEM_CODE)
Dem_DcmGetDTCByOccurrenceTime(
    Dem_DTCRequestType DTCRequest,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetDTCByOccurrenceTime(DTCRequest, DTCKind, DTC) \
    Dem_DcmGetDTCByOccurrenceTime(DTCRequest,DTC)
#endif
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
extern FUNC(void, DEM_CODE) Dem_DcmControlDTCStatusChangedNotification(
    boolean TriggerNotification
    );
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
extern FUNC(Dem_ReturnDisableDTCRecordUpdateType, DEM_CODE)
Dem_DcmDisableDTCRecordUpdate(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin
    );
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
extern FUNC(Std_ReturnType, DEM_CODE)
Dem_DisableDTCRecordUpdate(void);
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE) Dem_EnableDTCRecordUpdate(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmEnableDTCRecordUpdate(void);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_EnableDTCRecordUpdate() Dem_DcmEnableDTCRecordUpdate()
#endif
/*************************************************************************/
/*
 * @Brief               <Gets a DTC associated with a FreezeFrame.>
 * @ServiceId           <0x1c>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <RecordNumber:This parameter is a unique identifier for a FreezeFrame
 *                          record as defined in ISO15031-5 and ISO14229-1.
 *                          This parameter cannot be 0xFF.
 *                       DTCOrigin:This parameter selects the source memory the DTCs
 *                          shall be read from.
 *                       DTCKind : This parameter defines the requested DTC, either only
 *                          OBD-relevant DTCs or all DTCs
 *                      >
 * @Param-Name[out]     <DTC : Receives the DTC value returned by the function. If the return
 *                          value of the function is other than DEM_GET_DTCOFFF_OK this parameter
 *                          does not contain valid data.>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_ReturnGetDTCOfFreezeFrameRecordType>
 * @PreCondition        <none for V3.1.5>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE) Dem_GetDTCOfFreezeFrameRecord(
		uint8 RecordNumber,
		Dem_DTCOriginType DTCOrigin,
		Dem_DTCKindType DTCKind,
		P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC
		);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
typedef uint8 Dem_ReturnGetDTCOfFreezeFrameRecordType;
#define DEM_GET_DTCOFFF_OK 0x00
#define DEM_GET_DTCOFFF_WRONG_RECORD 0x01
#define DEM_GET_DTCOFFF_NO_DTC_FOR_RECORD 0x02
#define DEM_GET_DTCOFFF_WRONG_DTCKIND 0x03

#define Dem_GetDTCOfFreezeFrameRecord(RecordNumber,DTCOrigin,DTCKind,DTC) DEM_GET_DTCOFFF_NO_DTC_FOR_RECORD
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
    Dem_GetFreezeFrameDataByDTC(
    		uint32  DTC,
    		Dem_DTCKindType  DTCKind,
    		Dem_DTCOriginType  DTCOrigin,
    		uint8  RecordNumber,
    		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
    		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  BufSize );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_GetFreezeFrameDataByDTC(DTC,DTCKind,DTCOrigin,RecordNumber,DestBuffer,BufSize) \
    Dem_DcmGetFreezeFrameDataByDTC(DTC,DTCOrigin, RecordNumber, DestBuffer, (uint16*)BufSize)
#endif
/*************************************************************************/
/*
 * @Brief               <Gets a FreezeFrame Data identifier by DTC>
 * @ServiceId           <0x1c>
 * @Sync/Async          <Synchronous>
 * @Reentrancy          <Non Reentrant>
 * @Param-Name[in]      <DTC:This is the DTC the FreezeFrame is assigned to.
 *                       RecordNumber:This parameter is a unique identifier for a FreezeFrame
 *                          record as defined in ISO15031-5 and ISO14229-1.
 *                          This parameter cannot be 0xFF.
 *                       DTCOrigin:This parameter selects the source memory the DTCs
 *                          shall be read from.
 *                       DTCKind : This parameter defines the requested DTC, either only
 *                          OBD-relevant DTCs or all DTCs
 *                      >
 * @Param-Name[out]     <ArraySize : This parameter specifies the number of data identifiers
 *                          for the selected RecordNumber
 *                       DataId : Pointer to an array with the supported data identifier
 *                          for the selected RecordNumber and DTC>
 * @Param-Name[in/out]  <none>
 * @Return              <Dem_ReturnGetDTCOfFreezeFrameRecordType>
 * @PreCondition        <none for V3.1.5>
 * @CallByAPI           <APIName>
 */
/*************************************************************************/
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE) Dem_GetFreezeFrameDataIdentifierByDTC(
		uint32  DTC,
		Dem_DTCKindType  DTCKind,
		Dem_DTCOriginType  DTCOrigin,
		uint8  RecordNumber,
		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  ArraySize,
		const uint16**  DataId );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
typedef uint8 Dem_ReturnGetFreezeFrameDataIdentifierByDTCType;
#define DEM_GET_ID_OK 0x00
#define DEM_GET_ID_WRONG_DTC 0x01
#define DEM_GET_ID_WRONG_DTCORIGIN 0x02
#define DEM_GET_ID_WRONG_DTCKIND 0x03
#define DEM_GET_ID_WRONG_FF_TYPE 0x04

#define Dem_GetFreezeFrameDataIdentifierByDTC(DTC,DTCKind,DTCOrigin,RecordNumber,ArraySize,DataId)\
    DEM_GET_ID_WRONG_FF_TYPE
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)
Dem_GetSizeOfFreezeFrame(
    uint32 DTC,
    Dem_DTCKindType  DTCKind,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

typedef uint8 Dem_ReturnGetSizeOfFreezeFrameType;
#define DEM_GET_SIZEOFFF_OK DEM_GETSIZEBYDTC_OK
#define DEM_GET_SIZEOFFF_WRONG_DTC DEM_GETSIZEBYDTC_WRONG_DTC
#define DEM_GET_SIZEOFFF_WRONG_DTCOR DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFFF_WRONG_DTCKIND DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFFF_WRONG_RNUM DEM_GETSIZEBYDTC_WRONG_RECNUM
#define DEM_GET_SIZEOFFF_PENDING DEM_GETSIZEBYDTC_PENDING

#define Dem_GetSizeOfFreezeFrame(DTC, DTCKind, DTCOrigin, RecordNumber, SizeOfFreezeFrame) \
    Dem_DcmGetSizeOfFreezeFrameByDTC(DTC, DTCOrigin, RecordNumber, SizeOfFreezeFrame)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE)
Dem_GetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCKindType  DTCKind,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) BufSize
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_DcmGetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define DEM_RECORD_WRONG_DTCKIND 0x06
#define Dem_GetExtendedDataRecordByDTC(DTC,DTCKind,DTCOrigin,ExtendedDataNumber,DestBuffer, SizeOfExtendedDataRecord) \
    Dem_DcmGetExtendedDataRecordByDTC(DTC,DTCOrigin,ExtendedDataNumber,DestBuffer,(uint16*)SizeOfExtendedDataRecord)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_GetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCKindType  DTCKind,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

typedef uint8 Dem_ReturnGetSizeOfExtendedDataRecordByDTCType;
#define DEM_GET_SIZEOFEDRBYDTC_OK DEM_GETSIZEBYDTC_OK
#define DEM_GET_SIZEOFEDRBYDTC_W_DTC DEM_GETSIZEBYDTC_WRONG_DTC
#define DEM_GET_SIZEOFEDRBYDTC_W_DTCOR DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#define DEM_GET_SIZEOFEDRBYDTC_W_DTCKI DEM_GET_SIZEOFEDRBYDTC_W_RNUM
#define DEM_GET_SIZEOFEDRBYDTC_W_RNUM DEM_GETSIZEBYDTC_WRONG_RECNUM
#define DEM_GET_SIZEOFEDRBYDTC_PENDING DEM_GETSIZEBYDTC_PENDING

#define Dem_GetSizeOfExtendedDataRecordByDTC(DTC,DTCKind,DTCOrigin,ExtendedDataNumber,SizeOfExtendedDataRecord) \
    Dem_DcmGetSizeOfExtendedDataRecordByDTC(DTC,DTCOrigin,ExtendedDataNumber,SizeOfExtendedDataRecord)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_ClearDTC(uint32 DTC, Dem_DTCKindType DTCKind, Dem_DTCOriginType DTCOrigin);
#define DEM_CLEAR_WRONG_DTCKIND 0x03

#undef DEM_CLEAR_FAILED
#define DEM_CLEAR_FAILED 0x04

#undef DEM_CLEAR_PENDING
#define DEM_CLEAR_PENDING 0x05
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmClearDTC(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define DEM_CLEAR_WRONG_DTCKIND 0x07

#define Dem_ClearDTC(DTC, DTCKind, DTCOrigin) \
    Dem_DcmClearDTC(DTC,DEM_DTC_FORMAT_UDS,DTCOrigin)
#endif
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
typedef uint32 Dem_DTCGroupType;

typedef uint8 Dem_ReturnControlDTCStorageType;
#define DEM_CONTROL_DTC_STORAGE_OK DEM_CONTROL_DTC_SETTING_OK
#define DEM_CONTROL_DTC_STORAGE_N_OK DEM_CONTROL_DTC_SETTING_N_OK


typedef uint8 Dem_ReturnControlEventUpdateType;
#define DEM_CONTROL_EVENT_UPDATE_OK DEM_CONTROL_DTC_SETTING_OK
#define DEM_CONTROL_EVENT_UPDATE_N_OK DEM_CONTROL_DTC_SETTING_N_OK
#define DEM_CONTROL_EVENT_WRONG_DTCGROUP DEM_CONTROL_DTC_WRONG_DTCGROUP

#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE) Dem_DisableDTCStorage(Dem_DTCGroupType DTCGroup, Dem_DTCKindType DTCKind);

extern FUNC(uint8, DEM_CODE) Dem_DisableEventStatusUpdate(Dem_DTCGroupType DTCGroup, Dem_DTCKindType DTCKind);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmDisableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"



#define Dem_DisableDTCStorage(DTCGroup,DTCKind) \
    Dem_DcmDisableDTCSetting(DTCGroup,DTCKind)

#define Dem_DisableEventStatusUpdate(DTCGroup,DTCKind) \
    Dem_DcmDisableDTCSetting(DTCGroup,DTCKind)
#endif
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
#if (DEM_IMPLEMENT_VERSION_3_X == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(uint8, DEM_CODE) Dem_EnableDTCStorage(Dem_DTCGroupType DTCGroup, Dem_DTCKindType DTCKind);

extern FUNC(uint8, DEM_CODE) Dem_EnableEventStatusUpdate(Dem_DTCGroupType DTCGroup, Dem_DTCKindType DTCKind);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmEnableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind
    );
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#define Dem_EnableDTCStorage(DTCGroup,DTCKind) \
    Dem_DcmEnableDTCSetting(DTCGroup,DTCKind)

#define Dem_EnableEventStatusUpdate(DTCGroup,DTCKind) \
    Dem_DcmEnableDTCSetting(DTCGroup,DTCKind)
#endif


#endif /* DEM_DCM_H_ */

/********[E N D   O F   F I L E]***********************************************/
