/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Types.h>
 *  @brief      <Dem_Types>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Dec 26, 2014 - 10:17:42 AM>
 */
/*============================================================================*/

#ifndef DEM_TYPES_H_
#define DEM_TYPES_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Dec 26, 2014    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_TYPES_H_MODULE_ID            54U
#define DEM_TYPES_H_VENDOR_ID          0U
#define DEM_TYPES_H_AR_MAJOR_VERSION   0U
#define DEM_TYPES_H_AR_MINOR_VERSION   0U
#define DEM_TYPES_H_AR_PATCH_VERSION   0U
#define DEM_TYPES_H_SW_MAJOR_VERSION   1U
#define DEM_TYPES_H_SW_MINOR_VERSION   0U
#define DEM_TYPES_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Cfg.h"

/********[M A C R O S]*********************************************************/
#define DEM_DTC_GROUP_ALL_DTCS 0xFFFFFF

/* @req SWS_Dem_01114 */
typedef uint16 Dem_ComponentIdType;

/* @req SWS_Dem_00925 */
typedef uint16 Dem_EventIdType;

/* @req SWS_Dem_00926 */
typedef uint8 Dem_EventStatusType;
#define DEM_EVENT_STATUS_PASSED 0x00
#define DEM_EVENT_STATUS_FAILED 0x01
#define DEM_EVENT_STATUS_PREPASSED 0x02
#define DEM_EVENT_STATUS_PREFAILED 0x03
#define DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED 0x04

/* @req SWS_Dem_01000 */
typedef uint8 Dem_DebouncingStateType;
#define DEM_TEMPORARILY_DEFECTIVE 0x01
#define DEM_FINALLY_DEFECTIVE 0x02
#define DEM_TEMPORARILY_HEALED 0x04
#define DEM_TEST_COMPLETE 0x08
#define DEM_DTR_UPDATE 0x10

/* @req SWS_Dem_00927 */
typedef uint8 Dem_DebounceResetStatusType;
#define DEM_DEBOUNCE_STATUS_FREEZE 0x00
#define DEM_DEBOUNCE_STATUS_RESET 0x01

/* @req SWS_Dem_00928 */
typedef uint8 Dem_UdsStatusByteType;
#define DEM_UDS_STATUS_TF 0x01
#define DEM_UDS_STATUS_TFTOC 0x02
#define DEM_UDS_STATUS_PDTC 0x04
#define DEM_UDS_STATUS_CDTC 0x08
#define DEM_UDS_STATUS_TNCSLC 0x10
#define DEM_UDS_STATUS_TFSLC 0x20
#define DEM_UDS_STATUS_TNCTOC 0x40
#define DEM_UDS_STATUS_WIR 0x80

/* @req SWS_Dem_00929 */
typedef uint8 Dem_OperationCycleStateType;
#define DEM_CYCLE_STATE_START 0x00
#define DEM_CYCLE_STATE_END 0x01

/* @req SWS_Dem_00930 */
typedef uint8 Dem_IndicatorStatusType;
#define DEM_INDICATOR_OFF 0x00
#define DEM_INDICATOR_CONTINUOUS 0x01
#define DEM_INDICATOR_BLINKING 0x02
#define DEM_INDICATOR_BLINK_CONT 0x03
#define DEM_INDICATOR_SLOW_FLASH 0x04
#define DEM_INDICATOR_FAST_FLASH 0x05
#define DEM_INDICATOR_ON_DEMAND 0x06
#define DEM_INDICATOR_SHORT 0x07

/* @req SWS_Dem_00932 */
typedef uint8 Dem_DTCKindType;
#define DEM_DTC_KIND_ALL_DTCS 0x01
#define DEM_DTC_KIND_EMISSION_REL_DTCS 0x02
#define DEM_DTC_KIND_NUM 2

/* @req SWS_Dem_00933 */
typedef uint8 Dem_DTCFormatType;
#define DEM_DTC_FORMAT_OBD 0x00
#define DEM_DTC_FORMAT_UDS 0x01
#define DEM_DTC_FORMAT_J1939 0x02
#define DEM_DTC_FORMAT_NUM 3

/* @req SWS_Dem_00934 */
typedef uint8 Dem_DTCOriginType;
#define DEM_DTC_ORIGIN_PRIMARY_MEMORY 0x01
#define DEM_DTC_ORIGIN_MIRROR_MEMORY 0x02
#define DEM_DTC_ORIGIN_PERMANENT_MEMORY 0x03
#define DEM_DTC_ORIGIN_SECONDARY_MEMORY 0x04


/*#define DEM_DTC_ORIGIN_USERDEFINED_MEMORY_XX */

/* @req SWS_Dem_00935 */
typedef uint8 Dem_DTCRequestType;
#define DEM_FIRST_FAILED_DTC 0x01
#define DEM_MOST_RECENT_FAILED_DTC 0x02
#define DEM_FIRST_DET_CONFIRMED_DTC 0x03
#define DEM_MOST_REC_DET_CONFIRMED_DTC 0x04

/* @req SWS_Dem_00936 */
typedef uint8 Dem_DTCTranslationFormatType;
#define DEM_DTC_TRANSLATION_ISO15031_6 0x00
#define DEM_DTC_TRANSLATION_ISO14229_1 0x01
#define DEM_DTC_TRANSLATION_SAEJ1939_73 0x02
#define DEM_DTC_TRANSLATION_ISO11992_4 0x03

/* @req SWS_Dem_00937 */
typedef uint8 Dem_DTCSeverityType;
#define DEM_SEVERITY_NO_SEVERITY 0x00
#define DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS 0x01
#define DEM_SEVERITY_WWHOBD_CLASS_A 0x02
#define DEM_SEVERITY_WWHOBD_CLASS_B1 0x04
#define DEM_SEVERITY_WWHOBD_CLASS_B2 0x08
#define DEM_SEVERITY_WWHOBD_CLASS_C 0x10
#define DEM_SEVERITY_MAINTENANCE_ONLY 0x20
#define DEM_SEVERITY_CHECK_AT_NEXT_HALT 0x40
#define DEM_SEVERITY_CHECK_IMMEDIATELY 0x80

/* @req SWS_Dem_00941 */
typedef uint8 Dem_DTRControlType;
#define DEM_DTR_CTL_NORMAL 0x00
#define DEM_DTR_CTL_NO_MAX 0x01
#define DEM_DTR_CTL_NO_MIN 0x02
#define DEM_DTR_CTL_RESET 0x03
#define DEM_DTR_CTL_INVISIBLE 0x04

/* @req SWS_Dem_00942 */
typedef uint8 Dem_InitMonitorReasonType;
#define DEM_INIT_MONITOR_CLEAR 0x01
#define DEM_INIT_MONITOR_RESTART 0x02
#define DEM_INIT_MONITOR_REENABLED 0x03
#define DEM_INIT_MONITOR_STORAGE_REENABLED 0x04

/* @req SWS_Dem_00943 */
typedef uint8 Dem_IumprDenomCondIdType;
#define DEM_IUMPR_GENERAL_DENOMINATOR 0x01
#define DEM_IUMPR_DEN_COND_COLDSTART 0x02
#define DEM_IUMPR_DEN_COND_EVAP 0x03
#define DEM_IUMPR_DEN_COND_500MI 0x04

/* @req SWS_Dem_00944 */
typedef uint8 Dem_IumprDenomCondStatusType;
#define DEM_IUMPR_DEN_STATUS_NOT_REACHED 0x00
#define DEM_IUMPR_DEN_STATUS_REACHED 0x01
#define DEM_IUMPR_DEN_STATUS_INHIBITED 0x02


/********[Dem return types]****************************************************/
/* @req SWS_Dem_00952 */
typedef uint8 Dem_ReturnGetStatusOfDTCType;
#define DEM_STATUS_OK 0x00
#define DEM_STATUS_WRONG_DTC 0x01
#define DEM_STATUS_WRONG_DTCORIGIN 0x02
#define DEM_STATUS_FAILED 0x03
#define DEM_STATUS_PENDING 0x04

/* @req SWS_Dem_00953 */
typedef uint8 Dem_ReturnGetSeverityOfDTCType;
#define DEM_GET_SEVERITYOFDTC_OK 0x00
#define DEM_GET_SEVERITYOFDTC_WRONG_DTC 0x01
#define DEM_GET_SEVERITYOFDTC_NOSEVERITY 0x02
#define DEM_GET_SEVERITYOFDTC_PENDING 0x03

/* @req SWS_Dem_00954 */
typedef uint8 Dem_ReturnGetFunctionalUnitOfDTCType;
#define DEM_GET_FUNCTIONALUNITOFDTC_OK 0x00
#define DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC 0x01

/* @req SWS_Dem_00955 */
typedef uint8 Dem_ReturnSetFilterType;
#define DEM_FILTER_ACCEPTED 0x00
#define DEM_WRONG_FILTER 0x01

/* @req SWS_Dem_00956 */
typedef uint8 Dem_ReturnGetNumberOfFilteredDTCType;
#define DEM_NUMBER_OK 0x00
#define DEM_NUMBER_FAILED 0x01
#define DEM_NUMBER_PENDING 0x02

/* @req SWS_Dem_00957 */
typedef uint8 Dem_ReturnGetNextFilteredElementType;
#define DEM_FILTERED_OK 0x00
#define DEM_FILTERED_NO_MATCHING_ELEMENT 0x01
#define DEM_FILTERED_PENDING 0x02
#define DEM_FILTERED_BUFFER_TOO_SMALL 0x03

/* @req SWS_Dem_00958 */
typedef uint8 Dem_ReturnGetDTCByOccurrenceTimeType;
#define DEM_OCCURR_OK 0x00
#define DEM_OCCURR_NOT_AVAILABLE 0x01

/* @req SWS_Dem_00959 */
typedef uint8 Dem_ReturnDisableDTCRecordUpdateType;
#define DEM_DISABLE_DTCRECUP_OK 0x00
#define DEM_DISABLE_DTCRECUP_WRONG_DTC 0x01
#define DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN 0x02
#define DEM_DISABLE_DTCRECUP_PENDING 0x03

/* @req SWS_Dem_00960 */
typedef uint8 Dem_ReturnGetFreezeFrameDataByDTCType;
#define DEM_GET_FFDATABYDTC_OK 0x00
#define DEM_GET_FFDATABYDTC_WRONG_DTC 0x01
#define DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN 0x02
#define DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER 0x03
#define DEM_GET_FFDATABYDTC_WRONG_BUFFERSIZE 0x04
#define DEM_GET_FFDATABYDTC_PENDING 0x05

/* @req SWS_Dem_00961 */
typedef uint8 Dem_ReturnGetExtendedDataRecordByDTCType;
#define DEM_RECORD_OK 0x00
#define DEM_RECORD_WRONG_DTC 0x01
#define DEM_RECORD_WRONG_DTCORIGIN 0x02
#define DEM_RECORD_WRONG_NUMBER 0x03
#define DEM_RECORD_WRONG_BUFFERSIZE 0x04
#define DEM_RECORD_PENDING 0x05

/* @req SWS_Dem_00962 */
typedef uint8 Dem_ReturnGetSizeOfDataByDTCType;
#define DEM_GETSIZEBYDTC_OK 0x00
#define DEM_GETSIZEBYDTC_WRONG_DTC 0x01
#define DEM_GETSIZEBYDTC_WRONG_DTCORIGIN 0x02
#define DEM_GETSIZEBYDTC_WRONG_RECNUM 0x03
#define DEM_GETSIZEBYDTC_PENDING 0x04

/* @req SWS_Dem_00963 */
typedef uint8 Dem_ReturnClearDTCType;
#define DEM_CLEAR_OK 0x00
#define DEM_CLEAR_WRONG_DTC 0x01
#define DEM_CLEAR_WRONG_DTCORIGIN 0x02
#define DEM_CLEAR_FAILED 0x03
#define DEM_CLEAR_PENDING 0x04
#define DEM_CLEAR_BUSY 0x05
#define DEM_CLEAR_MEMORY_ERROR 0x06

/* @req SWS_Dem_00964 */
typedef uint8 Dem_ReturnControlDTCSettingType;
#define DEM_CONTROL_DTC_SETTING_OK 0x00
#define DEM_CONTROL_DTC_SETTING_N_OK 0x01
#define DEM_CONTROL_DTC_WRONG_DTCGROUP 0x02


/* @req SWS_Dem_00173  Error classification */
#define DEM_E_WRONG_CONFIGURATION 0x10
#define DEM_E_PARAM_POINTER 0x11
#define DEM_E_PARAM_DATA 0x12
#define DEM_E_PARAM_LENGTH 0x13
#define DEM_E_INIT_FAILED 0x14
#define DEM_E_UNINIT 0x20
#define DEM_E_NODATAAVAILABLE 0x30
#define DEM_E_WRONG_RECORDNUMBER 0x31
#define DEM_E_WRONG_DIDNUMBER 0x32
#define DEM_E_WRONG_CONDITION 0x40

#if(DEM_DEV_ERROR_DETECT == STD_ON)
/* Service ID */
#define DEM_SID_PREINIT                         0x01
#define DEM_SID_INIT                            0x02
#define DEM_SID_SHUTDOWN                        0x03
#define DEM_SID_REPORTERRORSTATUS               0x0f
#define DEM_SID_SETEVENTAVAILABLE               0x37
#define DEM_SID_SETEVENTSTATUS                  0x04
#define DEM_SID_RESETEVENTDEBOUNCESTATUS        0x09
#define DEM_SID_RESETEVENTSTATUS                0x05
#define DEM_SID_PRESTOREFREEZEFRAME             0x06
#define DEM_SID_CLEARPRESTOREDFREEZEFRAME       0x07
#define DEM_SID_SETOPERATIONCYCLESTATE          0x08
#define DEM_SID_GETOPERATIONCYCLESTATE          0x9e
#define DEM_SID_SETAGINGCYCLESTATE              0x11
#define DEM_SID_SETAGINGCYCLECOUNTERVALUE       0x12
#define DEM_SID_SETWIRSTATUS                    0x7a
#define DEM_SID_GETCOMPONENTFAILED              0x2a
#define DEM_SID_GETEVENTSTATUS                  0x0a
#define DEM_SID_GETEVENTFAILED                  0x0b
#define DEM_SID_GETEVENTTESTED                  0x0c
#define DEM_SID_GETDEBOUNCINGOFEVENT            0x9f
#define DEM_SID_GETDTCOFEVENT                   0x0d
#define DEM_SID_SETENABLECONDITION              0x39
#define DEM_SID_SETSTORAGECONDITION             0x38
#define DEM_SID_GETFAULTDETECTIONCOUNTER        0x3e
#define DEM_SID_GETINDICATORSTATUS              0x29
#define DEM_SID_SETINDICATORSTATUS              0xa1
#define DEM_SID_GETEVENTFREEZEFRAMEDATA         0x31
#define DEM_SID_GETEVENTEXTENDEDDATARECORD      0x30
#define DEM_SID_GETEVENTMEMORYOVERFLOW          0x32
#define DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES   0x35
#define DEM_SID_SETCOMPONENTAVAILABLE           0x2b
#define DEM_SID_SETDTCSUPPRESSION               0x33
#define DEM_SID_MAINFUNCTION                    0x55

#define DEM_SID_DCMGETTRANSLATIONTYPE                   0x3c
#define DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK         0x16
#define DEM_SID_DCMGETSTATUSOFDTC                       0x15
#define DEM_SID_DCMGETSEVERITYOFDTC                     0x0e
#define DEM_SID_DCMGETFUNCTIONALUNITOFDTC               0x34
#define DEM_SID_DCMSETDTCFILTER                         0x13
#define DEM_SID_DCMGETNUMBEROFFILTEREDDTC               0x17
#define DEM_SID_DCMGETNEXTFILTEREDDTC                   0x18
#define DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC             0x3b
#define DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY        0x3d
#define DEM_SID_DCMSETFREEZEFRAMERECORDFILTER           0x3f
#define DEM_SID_DCMGETNEXTFILTEREDRECORD                0x3a
#define DEM_SID_DCMGETDTCBYOCCURRENCETIME               0x19
#define DEM_SID_DCMCONTROLDTCSTATUSCHANGEDNOTIFICATION  0xb0
#define DEM_SID_DCMDISABLEDTCRECORDUPDATE               0x1a
#define DEM_SID_DCMENABLEDTCRECORDUPDATE                0x1b
#define DEM_SID_DCMGETFREEZEFRAMEDATABYDTC              0x1d
#define DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC            0x1f
#define DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC           0x20
#define DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC     0x21
#define DEM_SID_DCMCLEARDTC                             0x23
#define DEM_SID_DCMDISABLEDTCSETTING                    0x24
#define DEM_SID_DCMENABLEDTCSETTING                     0x25

#endif
/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

#endif /* DEM_TYPES_H_ */

/********[E N D   O F   F I L E]***********************************************/
