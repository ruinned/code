/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm_Types.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCM_TYPES_H
#define DCM_TYPES_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_TYPES_H_VENDOR_ID  0
#define DCM_TYPES_H_MODULE_ID  0
#define DCM_TYPES_H_AR_MAJOR_VERSION  3
#define DCM_TYPES_H_AR_MINOR_VERSION  3
#define DCM_TYPES_H_AR_PATCH_VERSION  0
#define DCM_TYPES_H_SW_MAJOR_VERSION  1
#define DCM_TYPES_H_SW_MINOR_VERSION  0
#define DCM_TYPES_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include "Dcm_Cfg.h"

/****************************** definitions *********************************/
/****************************************************************************************
 ********Security level type definition (0x02 ~ 0x7F: dependent on configuration)********
 ****************************************************************************************/
typedef  uint8   Dcm_SecLevelType;
#define  DCM_SEC_LEV_LOCKED      ((Dcm_SecLevelType)0x00)
#define  DCM_SEC_LEV_L1          ((Dcm_SecLevelType)0x01)
#define  DCM_SEC_LEV_L2          ((Dcm_SecLevelType)0x02)
#define  DCM_SEC_LEV_L3          ((Dcm_SecLevelType)0x03)
#define  DCM_SEC_LEV_L4          ((Dcm_SecLevelType)0x04)
#define  DCM_SEC_LEV_ALL         ((Dcm_SecLevelType)0xFF)

/****************************************************************************************
 *******Session Control Type Definition (0x05 to 0x7F: dependent on configuration)*******
 ****************************************************************************************/
typedef  uint8   Dcm_SesType;
#define  DCM_DEFAULT_SESSION                  ((Dcm_SesType)0x01)
#define  DCM_PROGRAMMING_SESSION              ((Dcm_SesType)0x02)
#define  DCM_EXTENDED_DIAGNOSTIC_SESSION      ((Dcm_SesType)0x03)
#define  DCM_SAFETY_SYSTEN_DIAGNOSTIC_SESSION ((Dcm_SesType)0x04)
#define  DCM_ALL_SESSION_LEVEL                ((Dcm_SesType)0xFF)

/****************************************************************************************
 ***************************Protocol Type Definition**********************************
 ****************************************************************************************/
typedef  uint8   Dcm_ProtocolType;
#define  DCM_OBD_ON_CAN                       ((Dcm_ProtocolType)0x00)
#define  DCM_UDS_ON_CAN                       ((Dcm_ProtocolType)0x01)
#define  DCM_UDS_ON_FLEXRAY                   ((Dcm_ProtocolType)0x02)
#define  DCM_ROE_ON_CAN                       ((Dcm_ProtocolType)0x03)
#define  DCM_ROE_ON_FLEXRAY                   ((Dcm_ProtocolType)0x04)
#define  DCM_PERIODIC_ON_CAN                  ((Dcm_ProtocolType)0x05)
#define  DCM_PERIODIC_ON_FLEXRAY              ((Dcm_ProtocolType)0x06)
#define  DCM_KWP2K_ON_CAN                     ((Dcm_ProtocolType)0x07)
#define  DCM_NO_PROTOCOL                      ((Dcm_ProtocolType)0xFF)
/****************************************************************************************
 *************************Negative response code types defined**************************
 ****************************************************************************************/
/****@req DCM-FUNR-083[DCM228]****/
typedef  uint8   Dcm_NegativeResponseCodeType;
#define  DCM_E_GENERALREJECT                             ((Dcm_NegativeResponseCodeType)0x10)	/*generalReject*/
#define  DCM_E_SERVICENOTSUPPORTED                       ((Dcm_NegativeResponseCodeType)0x11)	/*serviceNotSupported */
#define  DCM_E_SUBFUNCTIONNOTSUPPORTED                   ((Dcm_NegativeResponseCodeType)0x12)	/*subFunctionNotSupported*/
#define  DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT     ((Dcm_NegativeResponseCodeType)0x13) 	/*incorrectMessageLengthOrInvalidFormat */
#define  DCM_E_BUSYREPEATREQUEST                         ((Dcm_NegativeResponseCodeType)0x21)   /*busyRepeatRequest*/
#define  DCM_E_CONDITIONSNOTCORRECT                      ((Dcm_NegativeResponseCodeType)0x22)	/*conditionsNotCorrect*/
#define  DCM_E_REQUESTSEQUENCEERROR                      ((Dcm_NegativeResponseCodeType)0x24)   /*requestSequenceError */
#define  DCM_E_REQUESTOUTOFRANGE                         ((Dcm_NegativeResponseCodeType)0x31)   /*requestOutOfRange */
#define  DCM_E_SECURITYACCESSDENIED                      ((Dcm_NegativeResponseCodeType)0x33)   /*securityAccessDenied*/
#define  DCM_E_INVALIDKEY                                ((Dcm_NegativeResponseCodeType)0x35) 	/*invalidKey*/
#define  DCM_E_EXCEEDEDNUMBEROFATTEMPTS                  ((Dcm_NegativeResponseCodeType)0x36) 	/*exceedNumberOfAttempts*/
#define  DCM_E_REQUIREDTIMEDELAYNOTEXPIRED               ((Dcm_NegativeResponseCodeType)0x37) 	/*requiredTimeDelayNotExpired*/
#define  DCM_E_GENERALPROGRAMMINGFAILURE                 ((Dcm_NegativeResponseCodeType)0x72)   /*generalProgrammingFailure*/
#define  DCM_E_RESPONSE_PENDING                          ((Dcm_NegativeResponseCodeType)0x78)   /*requestCorrectlyReceived-ResponsePending */
#define  DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION    ((Dcm_NegativeResponseCodeType)0x7E)   /*subFunctionNotSupportedInActiveSession*/
#define  DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION        ((Dcm_NegativeResponseCodeType)0x7F)   /*serviceNotSupportedInActiveSession*/
#define  DCM_E_RPMTOOHIGH                                ((Dcm_NegativeResponseCodeType)0x81)   /*rpmTooHigh*/
#define  DCM_E_RPMTOOLOW                                 ((Dcm_NegativeResponseCodeType)0x82)   /*rpmTooLow*/
#define  DCM_E_ENGINEISRUNNING                           ((Dcm_NegativeResponseCodeType)0x83)   /*engineIsRunning*/
#define  DCM_E_ENGINEISNOTRUNNING                        ((Dcm_NegativeResponseCodeType)0x84)   /*engineIsNotRunning*/
#define  DCM_E_ENGINERUNTIMETOOLOW                       ((Dcm_NegativeResponseCodeType)0x85)   /*engineRunTimeTooLow */
#define  DCM_E_TEMPERATURETOOHIGH                        ((Dcm_NegativeResponseCodeType)0x86)   /*temperatureTooHigh */
#define  DCM_E_TEMPERATURETOOLOW                         ((Dcm_NegativeResponseCodeType)0x87)   /*temperatureTooLow*/
#define  DCM_E_VEHICLESPEEDTOOHIGH                       ((Dcm_NegativeResponseCodeType)0x88)   /*vehicleSpeedTooHigh*/
#define  DCM_E_VEHICLESPEEDTOOLOW                        ((Dcm_NegativeResponseCodeType)0x89)   /*vehicleSpeedTooLow*/
#define  DCM_E_THROTTLE_PEDALTOOHIGH                     ((Dcm_NegativeResponseCodeType)0x8A)   /*throttle/PedalTooHigh*/
#define  DCM_E_THROTTLE_PEDALTOOLOW                      ((Dcm_NegativeResponseCodeType)0x8B)   /*throttle/PedalTooLow*/
#define  DCM_E_TRANSMISSIONRANGENOTINNEUTRAL             ((Dcm_NegativeResponseCodeType)0x8C)
#define  DCM_E_TRANSMISSIONRANGENOTINGEAR                ((Dcm_NegativeResponseCodeType)0x8D)
#define  DCM_E_BRAKESWITCH_NOTCLOSED                     ((Dcm_NegativeResponseCodeType)0x8F)
#define  DCM_E_SHIFTERLEVERNOTINPARK                     ((Dcm_NegativeResponseCodeType)0x90)
#define  DCM_E_TORQUECONVERTERCLUTCHLOCKED               ((Dcm_NegativeResponseCodeType)0x91)
#define  DCM_E_VOLTAGETOOHIGH                            ((Dcm_NegativeResponseCodeType)0x92)
#define  DCM_E_VOLTAGETOOLOW                             ((Dcm_NegativeResponseCodeType)0x93)

/******************************************************************************
 ******************************************************************************
 *****************************************************************************/
#define  E_SESSION_NOT_ALLOWED   ((Std_ReturnType)4) /*Application does not allow the session change*/
#define  E_PROTOCOL_NOT_ALLOWED  ((Std_ReturnType)5) /*Application does not allow further processing of the protocol*/
#define  E_REQUEST_NOT_ACCEPTED  ((Std_ReturnType)8) /**/
#define  E_REQUEST_ENV_NOK       ((Std_ReturnType)9) /**/
#define  E_PENDING               ((Std_ReturnType)10)/**/
#define  E_COMPARE_KEY_FAILED    ((Std_ReturnType)11)/*Compare key failure*/

#if(STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
#define  E_FORCE_RCRRP           ((Std_ReturnType)12)/*Application requests sent immediately NRC = 0x78*/
#endif

#define DCM_INVALID_PDUID         0xFFu

#endif /* DCM_TYPES_H_ */
