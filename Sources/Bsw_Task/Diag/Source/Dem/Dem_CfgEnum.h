/******************************************************************************/
/*  Copyright (C) 2009-2015, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_CfgEnum.h>
 *  @brief      <Dem_CfgEnum>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Jan 12, 2015 - 11:02:20 AM>
 */
/******************************************************************************/

#ifndef DEM_CFGENUM_H_
#define DEM_CFGENUM_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Jan 12, 2015    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_CFGENUM_H_MODULE_ID            54U
#define DEM_CFGENUM_H_VENDOR_ID          0U
#define DEM_CFGENUM_H_AR_MAJOR_VERSION   3U
#define DEM_CFGENUM_H_AR_MINOR_VERSION   1U
#define DEM_CFGENUM_H_AR_PATCH_VERSION   0U
#define DEM_CFGENUM_H_SW_MAJOR_VERSION   1U
#define DEM_CFGENUM_H_SW_MINOR_VERSION   0U
#define DEM_CFGENUM_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/

/********[M A C R O S]*********************************************************/
/* ConfigType */
#define DEM_CONFIG_PRE_COMPILE 0
#define DEM_CONFIG_POST_BUILD 1

/* DemEventMemoryEntryStorageTrigger */
#define DEM_TRIGGER_ON_CONFIRMED 0x00
#define DEM_TRIGGER_ON_FDC_THRESHOLD 0x01
#define DEM_TRIGGER_ON_MIRROR 0x02
#define DEM_TRIGGER_ON_PASSED 0x03
#define DEM_TRIGGER_ON_PENDING 0x04
#define DEM_TRIGGER_ON_TEST_FAILED 0x05

/* DemAgingCycleCounterProcessing */
#define DEM_PROCESS_AGINGCTR_EXTERN 1
#define DEM_PROCESS_AGINGCTR_INTERN 2

/* DemAvailabilitySupport */
#define DEM_EVENT_AVAILABILITY 1
#define DEM_NO_AVAILABILITY 2

/* DemClearDTCBehavior  */
#define DEM_CLRRESP_NONVOLATILE_FINISH 1
#define DEM_CLRRESP_NONVOLATILE_TRIGGER 2
#define DEM_CLRRESP_VOLATILE 3

/* DemClearDTCLimitation */
#define DEM_ALL_SUPPORTED_DTCS 1
#define DEM_ONLY_CLEAR_ALL_DTCS 2

/* DemEnvironmentDataCapture */
#define DEM_CAPTURE_ASYNCHRONOUS_TO_REPORTING 1
#define DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING 2

/* DemEventCombinationSupport */
#define DEM_EVCOMB_DISABLED 1
#define DEM_EVCOMB_ONRETRIEVAL 2
#define DEM_EVCOMB_ONSTORAGE 3

/* DemEventDisplacementStrategy  */
#define DEM_DISPLACEMENT_FULL 1
#define DEM_DISPLACEMENT_NONE 2
#define DEM_DISPLACEMENT_PRIO_OCC 3

/* DemOBDSupport  */
#define DEM_OBD_DEP_SEC_ECU 1
#define DEM_OBD_MASTER_ECU 2
#define DEM_OBD_NO_OBD_SUPPORT 3
#define DEM_OBD_PRIMARY_ECU 4

/* DemOccurrenceCounterProcessing  */
#define DEM_PROCESS_OCCCTR_CDTC 1
#define DEM_PROCESS_OCCCTR_TF 2

/* DemStatusBitHandlingTestFailedSinceLastClear  */
#define DEM_STATUS_BIT_AGING_AND_DISPLACEMENT 1
#define DEM_STATUS_BIT_NORMAL 2

/* DemSuppressionSupport  */
#define DEM_DTC_SUPPRESSION 1
#define DEM_NO_SUPPRESSION 2

/* DemTypeOfDTCSupported  */
/* DEM_DTC_TRANSLATION_ISO11992_4 1
 * DEM_DTC_TRANSLATION_ISO14229_1 2
 * DEM_DTC_TRANSLATION_ISO15031_6 3
 * DEM_DTC_TRANSLATION_SAEJ1939_73 4
 */
#define DEM_DTC_TRANSLATION_SAE_J2012_DA_DTCFORMAT_04 5

/* DemTypeOfFreezeFrameRecordNumeration  */
#define DEM_FF_RECNUM_CALCULATED 1
#define DEM_FF_RECNUM_CONFIGURED 2

/* DemDtrUpdateKind */
#define DEM_DTR_UPDATE_ALWAYS 1
#define DEM_DTR_UPDATE_STEADY 2

#define DEM_UPDATE_RECORD_NO 0
#define DEM_UPDATE_RECORD_YES 1

/* DemOperationCycleType */
#define DEM_OPCYC_IGNITION 0
#define DEM_OPCYC_OBD_DCY 1
#define DEM_OPCYC_OTHER 2
#define DEM_OPCYC_POWER 3
#define DEM_OPCYC_TIME 4
#define DEM_OPCYC_WARMUP 5

/* DemIUMPRDenGroup */
#define DEM_IUMPR_DEN_500MILL 0
#define DEM_IUMPR_DEN_COLDSTART 1
#define DEM_IUMPR_DEN_EVAP 2
#define DEM_IUMPR_DEN_NONE 3
#define DEM_IUMPR_DEN_PHYS_API 4

/* DemIUMPRGroup */
#define DEM_IUMPR_BOOSTPRS 1
#define DEM_IUMPR_CAT1 2
#define DEM_IUMPR_CAT2 3
#define DEM_IUMPR_EGR 4
#define DEM_IUMPR_EGSENSOR 5
#define DEM_IUMPR_EVAP 6
#define DEM_IUMPR_FLSYS 7
#define DEM_IUMPR_NMHCCAT 8
#define DEM_IUMPR_NOXADSORB 9
#define DEM_IUMPR_NOXCAT 10
#define DEM_IUMPR_OXS1 11
#define DEM_IUMPR_OXS2 12
#define DEM_IUMPR_PMFILTER 13
#define DEM_IUMPR_PRIVATE 14
#define DEM_IUMPR_SAIR 15
#define DEM_IUMPR_SECOXS1 16
#define DEM_IUMPR_SECOXS2 17

/* DemRatioKind */
#define DEM_RATIO_API 0
#define DEM_RATIO_OBSERVER 1

/* DemInternalDataElement  */
#define DEM_AGINGCTR_DOWNCNT 0
#define DEM_AGINGCTR_UPCNT 1
#define DEM_CURRENT_FDC 2
#define DEM_CYCLES_SINCE_FIRST_FAILED 3
#define DEM_CYCLES_SINCE_LAST_FAILED 4
#define DEM_FAILED_CYCLES 5
#define DEM_MAX_FDC_DURING_CURRENT_CYCLE 6
#define DEM_MAX_FDC_SINCE_LAST_CLEAR 7
#define DEM_OCCCTR 8
#define DEM_OVFLIND 9
#define DEM_SIGNIFICANCE 10

/* DemWWHOBDDTCClass */
#define DEM_DTC_WWHOBD_CLASS_NOCLASS 0
#define DEM_DTC_WWHOBD_CLASS_A 1
#define DEM_DTC_WWHOBD_CLASS_B1 2
#define DEM_DTC_WWHOBD_CLASS_B2 3
#define DEM_DTC_WWHOBD_CLASS_C 4

/* DemDTCSignificance */
#define DEM_EVENT_SIGNIFICANCE_FAULT 0
#define DEM_EVENT_SIGNIFICANCE_OCCURRENCE 1

/* DemDebounceBehavior */
#define DEM_DEBOUNCE_FREEZE 0
#define DEM_DEBOUNCE_RESET 1

#define DEM_NO_STATUS_BYTE_CHANGE 0
#define DEM_ONLY_THIS_CYCLE_AND_READINESS 1

/* DemEventKind */
#define DEM_EVENT_KIND_BSW 0
#define DEM_EVENT_KIND_SWC 1

/* DemReportBehavior */
#define REPORT_AFTER_INIT 0
#define REPORT_BEFORE_INIT 1

/* DemEventOBDReadinessGroup */
#define DEM_OBD_RDY_AC 0
#define DEM_OBD_RDY_BOOSTPR 1
#define DEM_OBD_RDY_CAT 2
#define DEM_OBD_RDY_CMPRCMPT 3
#define DEM_OBD_RDY_EGSENS 4
#define DEM_OBD_RDY_ERG 5
#define DEM_OBD_RDY_EVAP 6
#define DEM_OBD_RDY_FLSYS 7
#define DEM_OBD_RDY_FLSYS_NONCONT 8
#define DEM_OBD_RDY_HCCAT 9
#define DEM_OBD_RDY_HTCAT 10
#define DEM_OBD_RDY_MISF 11
#define DEM_OBD_RDY_NONE 12
#define DEM_OBD_RDY_NOXCAT 13
#define DEM_OBD_RDY_O2SENS 14
#define DEM_OBD_RDY_O2SENSHT 15
#define DEM_OBD_RDY_PMFLT 16
#define DEM_OBD_RDY_SECAIR 17

/* DemDebounceType */
#define DEM_DEBOUNCE_COUNTER_BASED 0
#define DEM_DEBOUNCE_TIME_BASE 1
#define DEM_DEBOUNCE_MONITOR_INTERNAL 2
/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

#endif /* DEM_CFGENUM_H_ */

/********[E N D   O F   F I L E]***********************************************/
