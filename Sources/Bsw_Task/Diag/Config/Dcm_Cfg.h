/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dcm_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:59>
 */
/*============================================================================*/


#ifndef DCMCFG_H
#define DCMCFG_H
/****************************** references *********************************/
#include "ComStack_Types.h"
/****************************************************************************************
*********************************DcmGeneral container***********************************
*****************************************************************************************/
#define  DCM_DEV_ERROR_DETECT               (STD_OFF)
#define  DCM_REQUEST_INDICATION_ENABLED     (STD_ON)
#define  DCM_RESPOND_ALL_REQUEST            (STD_ON)
#define  DCM_VERSION_INFO_API               (STD_OFF)

/****************************************************************************************
********************************DcmPageBufferCfg container******************************
*****************************************************************************************/
#define  DCM_PAGEDBUFFER_ENABLED            (STD_OFF)

/****************************************************************************************
*********************************DcmDsl container***************************************
*****************************************************************************************/
#define  DCM_CHANNEL_NUM                    (2u)     /*number of configuration Channel */
#define  DCM_DSLPROTOCOLROW_NUM_MAX         (1u)     /*number of configuration protocol*/
#define  DCM_CHANNEL_LENGTH                 (510u)   /*the total length of channel*/

#define  DCM_DSLDIAGRESP_FORCERESPENDEN     (STD_ON)  /*Enable/Disable application directly trigger response Pending*/
#define  DCM_PROTOCOLTIMELIMIT_ENABLED      (STD_ON) /*Enable/Disable protocol time limit*/

/****************************************************************************************
*********************************DcmDsd container****************************************
*****************************************************************************************/
#define  DCM_SERVICE_TAB_NUM                (1u)     /*number of service table*/
/*******************************************/
/*UDS protocol*/
#define  DCM_UDS_FUNC_ENABLED               (STD_ON) /*Enable/Disable UDS Protocol*/
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
	#define  DCM_UDS_SERVICE_NUM            (12u)
    #define  DCM_UDS_SERVICE0X10_ENABLED    (STD_ON) /*DiagnosticSessionControl*/
    #define  DCM_UDS_SERVICE0X11_ENABLED    (STD_ON) /*ECUReset*/
    #define  DCM_UDS_SERVICE0X27_ENABLED    (STD_ON) /*SecurityAccess*/
    #define  DCM_UDS_SERVICE0X28_ENABLED    (STD_ON) /*CommunicationControl*/
    #define  DCM_UDS_SERVICE0X3E_ENABLED    (STD_ON) /*TesterPresent*/
    #define  DCM_UDS_SERVICE0X83_ENABLED    (STD_OFF) /*AccessTimingParameter*/
    #define  DCM_UDS_SERVICE0X84_ENABLED    (STD_OFF) /*SecuredDataTransmission*/
    #define  DCM_UDS_SERVICE0X85_ENABLED    (STD_ON) /*ControlDTCSetting*/
    #define  DCM_UDS_SERVICE0X86_ENABLED    (STD_OFF) /*ResponseOnEvent*/
    #define  DCM_UDS_SERVICE0X87_ENABLED    (STD_OFF) /*LinkControl*/
    #define  DCM_UDS_SERVICE0X22_ENABLED    (STD_ON) /*ReadDataByIdentifier*/
    #define  DCM_UDS_SERVICE0X23_ENABLED    (STD_OFF) /*ReadMemoryByAddress*/
    #define  DCM_UDS_SERVICE0X24_ENABLED    (STD_OFF) /*ReadScalingDataByIdentifier*/
    #define  DCM_UDS_SERVICE0X2A_ENABLED    (STD_OFF) /*ReadDataByPeriodicIdentifier*/
    #define  DCM_UDS_SERVICE0X2C_ENABLED    (STD_OFF) /*DynamicallyDefineDataIdentifier*/
    #define  DCM_UDS_SERVICE0X2E_ENABLED    (STD_ON) /*WriteDataByIdentifier*/
    #define  DCM_UDS_SERVICE0X3D_ENABLED    (STD_OFF) /*WriteMemoryByAddress*/
    #define  DCM_UDS_SERVICE0X14_ENABLED    (STD_ON) /*ClearDiagnosticInformation*/
    #define  DCM_UDS_SERVICE0X19_ENABLED    (STD_ON) /*ReadDTCInformation*/
    #define  DCM_UDS_SERVICE0X2F_ENABLED    (STD_ON) /*InputOutputControlByIdentifier*/
    #define  DCM_UDS_SERVICE0X31_ENABLED    (STD_ON) /*RoutineControl*/
#endif
/**********************************************/
#define  DCM_PERIODIC_FUNC_ENABLED          (STD_OFF) 		/*Enable/Disable Periodic Protocol*/

/**********************************************/
#define  DCM_ROE_FUNC_ENABLED               (STD_OFF) 		/*Enable/Disable ROE Protocol*/

/******************************************************************************
 ********************************DcmDsp container*****************************
 ******************************************************************************/
#define  DCM_DSP_DID_FUNC_ENABLED			(STD_ON)		/*Enable/disable DID function*/

/**=======================================================**/
#define  DCM_DSP_ECU_RESET_FUNC_ENABLED		(STD_ON)       	/*Enable/disable EcuReset function,corresponding UDS protocol 0x11 Service*/

/**=======================================================**/
#define  DCM_DSP_READ_DTC_FUNC_ENABLED		(STD_ON)       	/*Enable/disable ReadDTC function,corresponding UDS protocol 0x19 Service*/
#define  DCM_UDS_SERVICE0X19_01_ENABLED		(STD_ON)
#define  DCM_UDS_SERVICE0X19_02_ENABLED		(STD_ON)
#define  DCM_UDS_SERVICE0X19_03_ENABLED		(STD_OFF)
#define  DCM_UDS_SERVICE0X19_04_ENABLED		(STD_ON)
#define  DCM_UDS_SERVICE0X19_06_ENABLED		(STD_ON)
#define  DCM_UDS_SERVICE0X19_0A_ENABLED		(STD_OFF)
/**=======================================================**/
#define  DCM_DSP_ROUTINE_FUNC_ENABLED		(STD_ON)       	/*Enable/disable RoutinControl function,corresponding UDS protocol 0x31 Service.*/
#define  DCM_DSP_ROUTINE_MAX_NUM            (2u)       	/*Number of RoutineID*/

/**=======================================================**/
#define  DCM_SESSION_FUNC_ENABLED			(STD_ON)   		/*Enable/disable SessionControl Function*/

/**=======================================================**/
#define  DCM_SECURITY_FUNC_ENABLED			(STD_ON)   		/*Enable/disable SecurityAccess Function*/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	#define  DCM_SECURITY_ADR_MAXSIZE       	  (0u)      /*In all security levels,the maximum length of ADR*/
#endif

/**=======================================================**/
#define  DCM_DSP_PID_FUNC_ENABLED           		(STD_OFF)		/*corresponding OBD protocol 0x01 Service*/

/**=======================================================**/
#define  DCM_DSP_REQUESTCONTROL_FUNC_ENABLED      	(STD_OFF)   	/*corresponding OBD protocol 0x08 Service*/

/**=======================================================**/
#define  DCM_DSP_TEST_RESULT_BYOBDMID_FUNC_ENABLED	(STD_OFF)   	/*corresponding OBD protocol 0x06 Service*/

/**=======================================================**/
#define  DCM_DSP_VEHINFO_FUNC_ENABLED				(STD_OFF)   	/*corresponding OBD protocol 0x09 Service*/

/****************************************************************************************
********************************* OS ****************************************************
*****************************************************************************************/
#define  DCM_REFERENCE_OS_COUNTER_ID                 0x00	  		/*DCM reference OS Count ID*/

#define  DCM_FACTORY_MODE_ENABLED    STD_ON

#endif /* DCMCFG_H_ */

