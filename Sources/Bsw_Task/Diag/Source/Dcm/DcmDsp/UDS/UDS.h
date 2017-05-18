/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file        <UDS.h>
 *  @brief      <declarations of UDS services> 
 *  
 *  <Compiler: CodeWarrior    MCU:XXX>
 *  
 *  @author     <Guan Shengyong>
 *  @date       <27-03-2013>
 */
/*============================================================================*/

#ifndef UDS_H
#define UDS_H

/****************************** references *********************************/
#include "Dcm_Types.h"
#include "Compiler.h"
#include "Compiler_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/******************************Dcm_Internal.h****************************************/


/**************************************************************************
 **********************Service request packet length defined***************
 **************************************************************************/
#define  DCM_UDS0X10_REQ_DATA_LENGTH    			(2U) /*0x10 Service request packet length defined*/
#define  DCM_UDS0X11_REQ_DATA_LENGTH    			(2U) /*0x11 Service request packet length defined*/
#define  DCM_UDS0X14_REQ_DATA_LENGTH    			(4U) /*0x14 Service request packet length defined*/
#define  DCM_UDS0X19_REQ_DATA_MINLENGTH 			(2U) /*0x19 Service request packet length defined*/
#define  DCM_UDS0X19_SUBFUNC0X01_REQ_DATA_LENGTH 	(3U) /*sub-function 0x01 request message length*/
#define  DCM_UDS0X19_SUBFUNC0X02_REQ_DATA_LENGTH 	(3U) /*sub-function 0x02 request message length*/
#define  DCM_UDS0X19_SUBFUNC0X03_REQ_DATA_LENGTH 	(2U) /*sub-function 0x03 request message length*/
#define  DCM_UDS0X19_SUBFUNC0X04_REQ_DATA_LENGTH 	(6U) /*sub-function 0x04 request message length*/
#define  DCM_UDS0X19_SUBFUNC0X06_REQ_DATA_LENGTH 	(6U) /*sub-function 0x06 request message length*/
#define  DCM_UDS0X19_SUBFUNC0X0A_REQ_DATA_LENGTH 	(2U) /*sub-function 0x0A request message length*/
#define  DCM_UDS0X27_REQ_DATA_MINLENGTH             (2U)
#define  DCM_UDS0X22_REQ_DATA_MINLENGTH 			(3U) /*0x22 Service request packet length defined*/
#define  DCM_UDS0X2E_REQ_DATA_MINLENGTH 			(4U) /*0x2E Service request packet length defined*/
#define  DCM_UDS0X2F_REQ_DATA_MINLENGTH 			(4U) /*0x2F Service request packet length defined*/
#define  DCM_UDS0X31_REQ_DATA_MINLENGTH 			(4U) /*0x31 Service request packet length defined*/
#define  DCM_UDS0X3E_REQ_DATA_LENGTH    			(2U) /*0x3E Service request packet length defined*/
#define  DCM_UDS0X85_REQ_DATA_MINLENGTH    			(2U) /*0x85 request message minimum length*/
#define  DCM_UDS0X85_REQ_DATA_MAXLENGTH    			(5U) /*0x85 request message maximum length*/ 
#define  DCM_UDS0X28_REQ_DATA_MAXLENGTH    			(3U) /*0x28 request message length*/
#define  DCM_UDS0X87_REQ_DATA_MINLENGTH    			(2U) /*0x87 request message length*/
#define  DCM_UDS0X87_SUBFUNC0X01_REQ_DATA_LENGTH    (3U) /*sub-function 0x01 request message length*/
#define  DCM_UDS0X87_SUBFUNC0X02_REQ_DATA_LENGTH    (5U) /*sub-function 0x02 request message length*/
#define  DCM_UDS0X87_SUBFUNC0X03_REQ_DATA_LENGTH    (2U) /*sub-function 0x03 request message length*/
/**********************************************************************
 *             UDS 0x19 service sub-function
 **********************************************************************/
/*UDS 0x19 service,sub-function defined*/
#define  DCM_REPORTNUMBEROFDTCBYSTATUSMASK                      (0x01)/*reportNumberOfDTCByStatusMask */
#define  DCM_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD              (0x07)/*reportNumberOfDTCBySeverityMaskRecord*/
#define  DCM_REPORTNUMBEROFMIRRORMEMORYDTCBYSTATUSMASK          (0x11)/*reportNumberOfMirrorMemoryDTCByStatusMask*/
#define  DCM_REPORTNUMBEROFEMISSOONREALTEOBDDTCBYSTATUSMASK     (0x12)/*reportNumberOfEmissionsRelatedOBDDTCByStatusMask*/
#define  DCM_REPORTDTCBYSTATUSMASK                              (0x02)/*reportDTCByStatusMask*/
#define  DCM_REPORTSUPPORTEDDTC                                 (0x0A)/*reportSupportedDTC*/
#define  DCM_REPORTMIRRORMEMORYDTCBYSTATUSMASK                  (0x0F)/*reportMirrorMemoryDTCByStatusMask*/
#define  DCM_REPORTEMISSIONRELATEDOBDDTCBYSTATUSMASK            (0x13)/*reportEmissionsRelatedOBDDTCByStatusMask*/
#define  DCM_REPORTDTCWITHPERMANENTSTATUS                       (0x15)/*reportDTCWithPermanentStatus*/
#define  DCM_REPORTDTCBYSEVERITYMASKRECORD                      (0x08)/*reportDTCBySeverityMaskRecord*/
#define  DCM_REPORTSEVERITYINFORMATIONOFDTC                     (0x09)/*reportSeverityInformationOfDTC*/
#define  DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER             (0x06)/*reportDTCExtendedDataRecordByDTCNumber */
#define  DCM_REPORTMIRRORMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER (0x10)/*reportMirrorMemoryDTCExtendedDataRecordByDTCNumber*/
#define  DCM_REPORTREPORTDTCSNAPSHOTIDENTIFICATION              (0x03)/*reportDTCSnapshotIdentification*/
#define  DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER                 (0x04)/*reportDTCSnapshotRecordByDTCNumber*/
#define  DCM_REPORTDTCSNAPSHOTRECORDBYRECORDNUMBER              (0x05)/*reportDTCSnapshotRecordByRecordNumber*/
#define  DCM_REPORTREPORTFIRSTTESTFAILEDDTC                     (0x0B)/*reportFirstTestFailedDTC*/
#define  DCM_REPORTREPORTFIRSTCONFIRMEDDTC                      (0x0C)/*reportFirstConfirmedDTC*/
#define  DCM_REPORTMOSTRECENTTESTFAILEDDTC                      (0x0D)/*reportMostRecentTestFailedDTC*/
#define  DCM_REPORTMOSTRECENTCONFIRMEDDTC                       (0x0E)/*reportMostRecentConfirmedDTC*/
#define  DCM_REPORTREPORTDTCFAULTDETECTIONCOUNTER               (0x14)/*reportDTCFaultDetectionCounter*/

#define  ALL_SUPPORTED_DTC				  ((uint8)0x00)
/***********************************************************************
              UDS 0x3F service InputOutputControlParameter define
 ***********************************************************************/
#define  DCM_UDS0X2F_RETURNCONTROLTOECU   (0u) 		/*ReturnControlToEcu*/
#define  DCM_UDS0X2F_RESETTODEFAULT       (1u) 		/*ResetToDefault*/
#define  DCM_UDS0X2F_FREEZECURRENTSTATE   (2u) 		/*FreezeCurrentState*/
#define  DCM_UDS0X2F_SHORTTERMADJUSTMENT  (3u) 		/*ShortTermAdjustment*/


/**********************************************************************
 *            UDS 0x31 service sub-function define
 **********************************************************************/
#define  DCM_UDS0X31_STARTROUTINE          (1u)		/*startRoutine*/
#define  DCM_UDS0X31_STOPROUTINE           (2u)		/*stopRoutine*/
#define  DCM_UDS0X31_REQUESTROUTINERESULTS (3u)		/*RequestRoutineResult*/

/**********************************************************************
 *            UDS 0x3E service sub-function define
 **********************************************************************/
#define  DCM_UDS0X3E_ZERO_SUBFUNCTION      (0u)		/*zero sub-function*/

/**********************************************************************
 *            UDS 0x85 service sub-function define
 **********************************************************************/
#define  DCM_UDS0X85_ON     (1u)			/*turn on the setting of DTC*/
#define  DCM_UDS0X85_OFF    (2u)			/*turn off the setting of DTC*/

/**********************************************************************
 *            UDS 0x28 service sub-function define
 **********************************************************************/
#define  DCM_UDS0X28_ENABLE_RX_AND_TX             (0u)/*enable Rx and Tx*/
#define  DCM_UDS0X28_ENABLE_RX_AND_DISABLE_TX     (1u)/*enable Rx and disable Tx*/
#define  DCM_UDS0X28_DISABLE_RX_AND_ENABLE_TX     (2u)/*disable Rx and enable Tx*/
#define  DCM_UDS0X28_DISABLE_RX_AND_TX            (3u)/*disable Rx and Tx*/

#define COMTYPE_NORM							  (1u)/*normal communication message*/
#define COMTYPE_NM							  	  (2u)/*network management communication message*/
#define COMTYPE_NORM_AND_NM						  (3u)/*NORM and NM communication message*/

/**********************************************************************
 *            UDS 0x87 service sub-function define
 **********************************************************************/
#define  DCM_UDS0X87_VERIFY_BAUDRATE_TRANSITION_WITH_FIXED_BAUDRATE      (1u)/*VBTWFBR*/
#define  DCM_UDS0X87_VERIFY_BAUDRATE_TRANSITION_WITH_SPECIFIC_BAUDRATE   (2u)/*VBTWSBR*/
#define  DCM_UDS0X87_TRANSITION_BAUDRATE (3u)		/*transition baud rate*/


/******************************END of Dcm_Internal.h*********************************/
/******************************dependence****************************************/
typedef uint8 Dcm_CommunicationModeType;
#define DCM_ENABLE_RX_TX_NORM							0x00
#define DCM_ENABLE_RX_DISABLE_TX_NORM   				0x01
#define DCM_DISABLE_RX_ENABLE_TX_NORM  				    0x02
#define DCM_DISABLE_RX_TX_NORMAL  						0x03
#define DCM_ENABLE_RX_TX_NM   							0x04
#define DCM_ENABLE_RX_DISABLE_TX_NM   					0x05
#define DCM_DISABLE_RX_ENABLE_TX_NM   					0x06
#define DCM_DISABLE_RX_TX_NM   							0x07
#define DCM_ENABLE_RX_TX_NORM_NM  						0x08
#define DCM_ENABLE_RX_DISABLE_TX_NORM_NM   				0x09
#define DCM_DISABLE_RX_ENABLE_TX_NORM_NM   				0x0A
#define DCM_DISABLE_RX_TX_NORM_NM   					0x0B


/******************************END of dependence****************************************/
#if(STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
/*definition of processing status of link control*/
typedef enum Dcm_LinkControlStatus_t
{
	LINK_CONTROL_IDLE = 0,
	LINK_CONTROL_FBR_VERIFICATION = 1,
	LINK_CONTROL_FBR_TRANSITION = 2,
	LINK_CONTROL_SBR_VERIFICATION = 3,
	LINK_CONTROL_SBR_TRANSITION = 4	
}Dcm_LinkControlStatusType;

/*definition of 'link control' control block*/
typedef struct Dcm_LinkControlCtrlType_t
{
	VAR(Dcm_LinkControlStatusType, TYPEDEF) linkCtrlStatus;
	VAR(uint8, TYPEDEF) fixedBaudrate;
	VAR(uint32, TYPEDEF) specialBaudrate;
}Dcm_LinkControlCtrlType;
#endif

/*SID Table*/
#define SID_DIAGNOSTIC_SESSION_CONTROL				0x10
#define SID_ECU_RESET								0x11
#define SID_CLEAR_DIAGNOSTIC_INFORMATION			0x14
#define SID_READ_DTC_INFORMATION					0x19
#define SID_READ_DATA_BY_IDENTIFIER					0x22
#define SID_SECURITY_ACCESS							0x27
#define SID_COMMUNICATION_CONTROL 					0x28
#define SID_WRITE_DATA_BY_IDENTIFIER				0x2E
#define SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER		0x2F
#define SID_ROUTINE_CONTROL							0x31
#define SID_TESTER_PRESENT							0x3E
#define SID_CONTROL_DTC_SETTING						0x85
#define SID_LINK_CONTROL 							0x87
#define SID_TEST                                    0xB0
/*END OF SID Table*/

#if(STD_ON == DCM_UDS_FUNC_ENABLED)
/****************************** declarations *********************************/
#if(STD_ON == DCM_UDS_SERVICE0X10_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x10(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X11_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x11(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X27_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x27(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x28(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X3E_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x3E(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X85_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x85(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x87(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X14_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x14(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X19_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x19(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X22_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x22(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X2E_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x2E(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X2F_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x2F(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif

#if(STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
#define DCM_START_SEC_CODE
#include "MemMap.h"
extern  FUNC(Std_ReturnType, DCM_CODE)  DspInternal_UDS0x31(uint8  ProtocolCtrlId);
#define DCM_STOP_SEC_CODE
#include "MemMap.h"
#endif


#endif/*end of 'STD_ON == DCM_UDS_FUNC_ENABLED'*/


/****************************** definitions *********************************/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* UDS_H_ */
