/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:59>
 */
/*============================================================================*/


#ifndef DEM_CFG_H_
#define DEM_CFG_H_

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_CFG_H_AR_MAJOR_VERSION   3U
#define DEM_CFG_H_AR_MINOR_VERSION   1U
#define DEM_CFG_H_AR_PATCH_VERSION   0U
#define DEM_CFG_H_SW_MAJOR_VERSION   1U
#define DEM_CFG_H_SW_MINOR_VERSION   0U
#define DEM_CFG_H_SW_PATCH_VERSION   0U


/********[I N C L U D E S]*****************************************************/
#include "Std_Types.h"
#include "Dem_CfgEnum.h"
#include "Dem_IntErrId.h"

#define DEM_BITS_TO_BYTE(x) (((x)+7)>>3)

/*******************************************************************************
*                          General Configuration
*******************************************************************************/


#define DemOperationCycle_ID 0



#define P0A7D00_ID 0xA7D00
#define P16C119_ID 0x16C119
#define P16C11D_ID 0x16C11D
#define P16C219_ID 0x16C219
#define P16C21D_ID 0x16C21D
#define P16FB17_ID 0x16FB17
#define P16FB85_ID 0x16FB85
#define P16FB24_ID 0x16FB24
#define P16FB16_ID 0x16FB16
#define P16FB84_ID 0x16FB84
#define P16FB00_ID 0x16FB00
#define P16FC17_ID 0x16FC17
#define P16FC85_ID 0x16FC85
#define P16FC16_ID 0x16FC16
#define P16FC84_ID 0x16FC84
#define P167E98_ID 0x167E98
#define P167EA4_ID 0x167EA4
#define P167EA5_ID 0x167EA5
#define P167EAF_ID 0x167EAF
#define P16FCAF_ID 0x16FCAF
#define P16F91C_ID 0x16F91C
#define P169F04_ID 0x169F04
#define P16C304_ID 0x16C304
#define P16BA04_ID 0x16BA04
#define P16BB04_ID 0x16BB04
#define U10C000_ID 0xD0C000
#define P166496_ID 0x166496
#define P16A22B_ID 0x16A22B
#define P16A52B_ID 0x16A52B
#define U10C087_ID 0xD0C087
#define P16A692_ID 0x16A692
#define P160B13_ID 0x160B13
#define P166700_ID 0x166700
#define P146E96_ID 0x146E96
#define P147319_ID 0x147319
#define P16A72B_ID 0x16A72B
#define P16A82B_ID 0x16A82B
#define U10C287_ID 0xD0C287
#define P166800_ID 0x166800
#define P166900_ID 0x166900
#define P166A00_ID 0x166A00
#define P166B00_ID 0x166B00
#define P167000_ID 0x167000
#define P167100_ID 0x167100
#define P167200_ID 0x167200
#define P146696_ID 0x146696
#define P166C00_ID 0x166C00
#define P168300_ID 0x168300
#define P168400_ID 0x168400
#define U007388_ID 0xC07388
#define U007588_ID 0xC07588
#define P168500_ID 0x168500
#define P168600_ID 0x168600
#define P168700_ID 0x168700
#define P168900_ID 0x168900

#define DemDidClass_1_ID 0

#define DemFreezeFrameRecordClass_1_ID 1

#define DemExtendedDataRecordClass_1_RecordNum_ID 0x1


#define DEM_CONFIG_VARIANT DEM_CONFIG_PRE_COMPILE

/* DemDevErrorDetect Range: true or false */
#define DEM_DEV_ERROR_DETECT STD_OFF

/* DemGeneralInterfaceSupport Range: true or false */
#define DEM_GENERAL_INTERFACE_SUPPORT STD_OFF

/* DemTriggerDcmReports Range: true or false */
#define DEM_TRIGGER_DCM_REPORTS STD_OFF

/* DemTriggerDltReports Range: true or false */
#define DEM_TRIGGER_DLT_REPORTS STD_OFF

/* DemTriggerFiMReports Range: true or false */
#define DEM_TRIGGER_FIM_REPORTS STD_OFF

/* DemTriggerMonitorInitBeforeClearOk Range: true or false */
#define DEM_TRIGGER_MONITOR_INIT_BEFORE_CLEAR_OK STD_OFF

/* DemVersionInfoApi Range: true or false */
#define DEM_VERSION_INFO_API STD_OFF

/* DemTaskTime */
#define DEM_TASK_TIME 10

/* DemFIMEnable */
#define DEM_FIM_ENABLE STD_OFF

/* DemNVMEnable */
#define DEM_NVM_ENABLE STD_OFF

/*******************************************************************************
*                          Condition Configuration
*******************************************************************************/
/* DemOperationCycleStatusStorage Range: true or false */
#define DEM_OPERATION_CYCLE_STATUS_STORAGE STD_ON
/* DemOperationCycle 1-256 */
#define DEM_OPERATION_CYCLE_NUM 4
#define DEM_OPERATION_CYCLE_NUM_BYTE 1
#define DEM_OPERATION_CYCLE_INVALID DEM_OPERATION_CYCLE_NUM

/* DemEnableCondition 0-255 */
#define DEM_ENABLE_CONDITION_NUM 0
#define DEM_ENABLE_CONDITION_NUM_BYTE 0

/* DemEnableConditionGroup 0-255 */
#define DEM_ENABLE_CONDITION_GROUP_NUM 0
#define DEM_ENABLE_CONDITION_GROUP_INVALID DEM_ENABLE_CONDITION_GROUP_NUM

/* DemEnableCondition 0-255 */
#define DEM_STORAGE_CONDITION_NUM 0
#define DEM_STORAGE_CONDITION_NUM_BYTE 0

/* DemStorageConditionGroup 0-255 */
#define DEM_STORAGE_CONDITION_GROUP_NUM 0
#define DEM_STORAGE_CONDITION_GROUP_INVALID DEM_STORAGE_CONDITION_GROUP_NUM

/*******************************************************************************
*                          FreezeFrame Configuration
*******************************************************************************/
/* DemMaxNumberPrestoredFF Range:0~255 */
#define DEM_MAX_NUMBER_PRESTORED_FF 5

/* DemTypeOfFreezeFrameRecordNumeration type: Enum
#define DEM_FF_RECNUM_CALCULATED 1
#define DEM_FF_RECNUM_CONFIGURED 2 */
#define DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION DEM_FF_RECNUM_CONFIGURED


/******************************************************************************/
/* DemDidClass 0-0xFFFF */
#define DEM_DID_CLASS_NUM 1
#define DEM_DID_INVALID DEM_DID_CLASS_NUM

/* DemFreezeFrameClass 0-65535 */
#define DEM_FREEZE_FRAME_CLASS_NUM 1
#define DEM_FREEZE_FRAME_INVALID DEM_FREEZE_FRAME_CLASS_NUM

/* SUM(COUNT(DemFreezeFrameClass/DemDidClassRef)) */
#define DEM_DID_CLASS_REF_TOTAL_NUM 1

/* MAX(SUM(DemGeneral/DemFreezeFrameClass/DemDidClassRef/DataSize)) */
#define DEM_FREEZE_FRAME_MAX_LEN 20

/* DemFreezeFrameRecordClass 0-255 */
#define DEM_FREEZE_FRAME_RECORD_CLASS_NUM 1
#define DEM_FREEZE_FRAME_RECORD_INVALID DEM_FREEZE_FRAME_RECORD_CLASS_NUM

#define DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM 1
#define DEM_FREEZE_FRAME_REC_NUM_INVALID DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM

/* SUM(COUNT(DemGeneral/DemFreezeFrameRecNumClass/DemFreezeFrameRecordClassRef)) */
#define DEM_FREEZE_FRAME_RECORD_CLASS_REF_TOTAL_NUM 1

#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
/* MAX(COUNT((DemGeneral/DemFreezeFrameRecNumClass/DemFreezeFrameRecordClassRef)))  1-254 */
#define DEM_MAX_NUMBER_FF_RECORDS 1
#endif

/*******************************************************************************
*                          ExtendedData Configuration
*******************************************************************************/
/* DemExtendedDataRecordClass 0-253 */
#define DEM_EXTENDED_DATA_RECORD_CLASS_NUM 1
#define DEM_EXTENDED_DATA_RECORD_INVALID DEM_EXTENDED_DATA_RECORD_CLASS_NUM

/* DemExtendedDataClass 0-* */
#define DEM_EXTENDED_DATA_CLASS_NUM 1
#define DEM_EXTENDED_DATA_INVALID DEM_EXTENDED_DATA_CLASS_NUM

/* SUM(COUNT(DemGeneral/DemExtendedDataClass/DemExtendedDataRecordClassRef)) */
#define DEM_EXTENDED_DATA_RECORD_CLASS_REF_TOTAL_NUM  1

/* MAX(COUNT(DemGeneral/DemExtendedDataClass/DemExtendedDataRecordClassRef)) */
#define DEM_EXTENDED_DATA_MAX_REF_NUM 1
#define DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE 1

/* MAX(SUM(DemGeneral/DemExtendedDataClass/DemExtendedDataRecordClassRef/DataSize)) */
#define DEM_EXTENDED_DATA_MAX_LEN 2

/*******************************************************************************
*                          DTC Configuration
*******************************************************************************/
/* DemClearDTCBehavior type: Enum
#define DEM_CLRRESP_NONVOLATILE_FINISH 1
#define DEM_CLRRESP_NONVOLATILE_TRIGGER 2
#define DEM_CLRRESP_VOLATILE 3 */
#define DEM_CLEAR_DTCBEHAVIOR DEM_CLRRESP_NONVOLATILE_FINISH

/* DemClearDTCLimitation type: Enum
#define DEM_ALL_SUPPORTED_DTCS 1
#define DEM_ONLY_CLEAR_ALL_DTCS 2 */
#define DEM_CLEAR_DTCLIMITATION DEM_ALL_SUPPORTED_DTCS

/* DemSuppressionSupport type: Enum
#define DEM_DTC_SUPPRESSION 1
#define DEM_NO_SUPPRESSION 2 */
#define DEM_SUPPRESSION_SUPPORT DEM_NO_SUPPRESSION

/* DemTypeOfDTCSupported type: Enum
#define DEM_DTC_TRANSLATION_ISO11992_4 1
#define DEM_DTC_TRANSLATION_ISO14229_1 2
#define DEM_DTC_TRANSLATION_ISO15031_6 3
#define DEM_DTC_TRANSLATION_SAEJ1939_73 4
#define DEM_DTC_TRANSLATION_SAE_J2012_DA_DTCFORMAT_04 5 */
#define DEM_TYPE_OF_DTCSUPPORTED DEM_DTC_TRANSLATION_ISO14229_1

/* DemGroupOfDTC 0-255 */
#define DEM_GROUP_OF_DTC_NUM 0
#define DEM_GROUP_OF_DTC_NUM_BYTE 0
#define DEM_GROUP_OF_DTC_INVALID DEM_GROUP_OF_DTC_NUM

/* DemDTC 0-65535 */
#define DEM_DTC_NUM 55
#define DEM_DTC_NUM_BYTE 7
#define DEM_DTC_INVALID DEM_DTC_NUM

/* DemDTCAttributes 0-65535 */
#define DEM_DTC_ATTRIBUTES_NUM 2
#define DEM_DTC_ATTRIBUTES_INVALID DEM_DTC_ATTRIBUTES_NUM

#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED)
/* MAX(DemConfigSet/DemDTCAttributes/DemMaxNumberFreezeFrameRecords) */
#define DEM_MAX_NUMBER_FF_RECORDS 5
#endif

/* COUNT(DemGeneral/DemCallbackDTCStatusChanged) 0-* */
#define DEM_CALLBACK_DTC_STATUS_CHANGED_NUM 0

/*******************************************************************************
*                          Indicator Configuration
*******************************************************************************/
/* DemIndicator 0..255 */
#define DEM_INDICATOR_NUM 0
#define DEM_INDICATOR_INVALID DEM_INDICATOR_NUM

/* SUM(COUNT(DemConfigSet/DemEventParameter/DemIndicatorAttribute)) */
#define DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM 0

/*******************************************************************************
*                          Debounce Configuration
*******************************************************************************/
/* DemDebounceCounterBasedSupport Range: true or false */
#define DEM_DEBOUNCE_COUNTER_BASED_SUPPORT STD_ON

/* DemDebounceCounterBasedClass 0-65535 */
#define DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM 1

/* COUNT(DemConfigSet/DemEventParameter) 
 * WHERE (DemEventParameter/DemDebounceAlgorithmClass == DemDebounceCounterBased) */
#define DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM 61

/* COUNT(DemConfigSet/DemEventParameter) 
 * WHERE ((DemEventParameter/DemDebounceAlgorithmClass == DemDebounceCounterBased) 
 *         && DemDebounceCounterBased/DemDebounceCounterBasedClassRef/DemDebounceCounterStorage == true) */

#define DEM_DEBOUNCE_COUNTER_STORAGE_NUM 0

/* DemDebounceTimeBasedSupport Range: true or false */
#define DEM_DEBOUNCE_TIME_BASED_SUPPORT STD_ON

#define DEM_DEBOUNCE_TIME_BASE_CLASS_NUM 1

/* COUNT(DemConfigSet/DemEventParameter) 
 * WHERE (DemEventParameter/DemDebounceAlgorithmClass == DemDebounceTimeBase) */
#define DEM_DEBOUNCE_TIME_BASED_EVENT_NUM 1
/* COUNT(DemConfigSet/DemEventParameter) 
 * WHERE (DemEventParameter/DemDebounceAlgorithmClass == DemDebounceMonitorInternal) */
#define DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM 0

/*******************************************************************************
*                          Event Configuration
*******************************************************************************/
/* DemBswErrorBufferSize Range:0~255 */
#define DEM_BSW_ERROR_BUFFER_SIZE 5

/* DemDtcStatusAvailabilityMask Range:0~255 */
#define DEM_DTC_STATUS_AVAILABILITY_MASK 0xAB

/* DemAgingRequieresTestedCycle Range: true or false */
#define DEM_AGING_REQUIERES_TESTED_CYCLE STD_OFF

/* DemResetConfirmedBitOnOverflow Range: true or false */
#define DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW STD_ON

/* DemStatusBitStorageTestFailed Range: true or false */
#define DEM_STATUS_BIT_STORAGE_TEST_FAILED STD_OFF

/* DemAgingCycleCounterProcessing type: Enum
#define DEM_PROCESS_AGINGCTR_EXTERN 1
#define DEM_PROCESS_AGINGCTR_INTERN 2 */
#define DEM_AGING_CYCLE_COUNTER_PROCESSING DEM_PROCESS_AGINGCTR_EXTERN

/* DemAvailabilitySupport type: Enum
#define DEM_EVENT_AVAILABILITY 1
#define DEM_NO_AVAILABILITY 2 */
#define DEM_AVAILABILITY_SUPPORT DEM_EVENT_AVAILABILITY

/* DemEnvironmentDataCapture type: Enum
#define DEM_CAPTURE_ASYNCHRONOUS_TO_REPORTING 1
#define DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING 2 */
#define DEM_ENVIRONMENT_DATA_CAPTURE DEM_CAPTURE_ASYNCHRONOUS_TO_REPORTING

/* DemEventCombinationSupport type: Enum
#define DEM_EVCOMB_DISABLED 1
#define DEM_EVCOMB_ONRETRIEVAL 2
#define DEM_EVCOMB_ONSTORAGE 3 */
#define DEM_EVENT_COMBINATION_SUPPORT DEM_EVCOMB_DISABLED

/* DemEventDisplacementStrategy type: Enum
#define DEM_DISPLACEMENT_FULL 1
#define DEM_DISPLACEMENT_NONE 2
#define DEM_DISPLACEMENT_PRIO_OCC 3 */
#define DEM_EVENT_DISPLACEMENT_STRATEGY DEM_DISPLACEMENT_NONE

/* DemOccurrenceCounterProcessing type: Enum
#define DEM_PROCESS_OCCCTR_CDTC 1
#define DEM_PROCESS_OCCCTR_TF 2 */
#define DEM_OCCURRENCE_COUNTER_PROCESSING DEM_PROCESS_OCCCTR_CDTC

/* DemGeneral/DemStatusBitHandlingTestFailedSinceLastClear
#define DEM_STATUS_BIT_AGING_AND_DISPLACEMENT 1
#define DEM_STATUS_BIT_NORMAL 2 */
#define DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR DEM_STATUS_BIT_NORMAL

/* DemEventParameter 1-65535 */
#define DEM_EVENT_PARAMETER_NUM 62
#define DEM_EVENT_PARAMETER_NUM_BYTE 8
#define DEM_EVENT_PARAMETER_INVALID DEM_EVENT_PARAMETER_NUM

/* SUM(COUNT(DemConfigSet/DemEventParameter/DemCallbackClearEventAllowed))) */
#define DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM 0
#define DEM_CALLBACK_CLEAR_EVENT_NOT_ALLOWED DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM

/* SUM(COUNT(DemConfigSet/DemEventParameter/DemCallbackEventStatusChanged))) */
#define DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM 0

/*******************************************************************************
*                          Memory Configuration
*******************************************************************************/
/* DemImmediateNvStorageLimit Range:1~255 */
#define DEM_IMMEDIATE_NV_STORAGE_LIMIT 0x5

/* DemMaxNumberEventEntryEventBuffer Range:1~250 */
#define DEM_MAX_NUMBER_EVENT_ENTRY_EVENT_BUFFER 10

/* DemMaxNumberEventEntryPermanent Range:0~255 */
#define DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT 0

/* DemEventMemoryEntryStorageTrigger type: Enum
#define DEM_TRIGGER_ON_CONFIRMED 1
#define DEM_TRIGGER_ON_FDC_THRESHOLD 2
#define DEM_TRIGGER_ON_PENDING 3
#define DEM_TRIGGER_ON_TEST_FAILED 4 */
#define DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER DEM_TRIGGER_ON_TEST_FAILED

/* DemPrimaryMemory 1-1 */
/* DemMaxNumberEventEntryPrimary Range:1~255 */
#define DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY 55

/* DemMirrorMemory 0-1 */
/* DemMaxNumberEventEntryMirror Range:0~255 */
#define DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR  0

/* COUNT(DemGeneral/Dem<xx>Memory) */
#define DEM_MEM_DEST_TOTAL_NUM 1
#define DEM_MEM_DEST_INVALID DEM_MEM_DEST_TOTAL_NUM
/* MAX(DemConfigSet/DemDTCAttributes/DemMemoryDestinationRef) 0~2 */
#define DEM_MEM_DEST_MAX_NUM_OF_DTC 1 /* DemMemoryDestinationRef 0-2 */

/* MAX(DemGeneral/Dem<XX>Memory/DemMaxNumberEventEntry<XX>) */
#define DEM_MEM_DEST_MAX_ENTRY_NUM 55
#define DEM_MEM_DEST_MAX_ENTRY_NUM_BYTE 7

/* COUNT(DemGeneral/DemNvRamBlockId) */
#define DEM_NVRAM_BLOCKID_NUM 1

#endif /* DEM_CFGNEW_DEM_CFG_H_ */

/********[E N D   O F   F I L E]***********************************************/
