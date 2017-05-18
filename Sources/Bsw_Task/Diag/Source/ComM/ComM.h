/*================================================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <ComM.h>
 *  @brief      <Communication Manager Module Header File> 
 *  @author     <pinghai.xiong>
 *  @date       <2014-02-24>
 */
/*================================================================================================*/
#ifndef COMM_H
#define COMM_H

/*=======[R E V I S I O N   H I S T O R Y]========================================================*/
/*  <VERSION>    <DATE>    <AUTHOR>         <REVISION LOG>
 *  V1.0.0       20140224  pinghai.xiong    Initial version.
 *
 */
/*================================================================================================*/
 
/*=======[V E R S I O N  I N F O R M A T I O N]===================================================*/
#define COMM_VENDOR_ID              62u /* Replace with a symbolic name once available */
#define COMM_MODULE_ID              12u /* from AUTOSAR_BasicSoftwareModules.pdf */
#define COMM_H_AR_MAJOR_VERSION     2u 
#define COMM_H_AR_MINOR_VERSION     1u 
#define COMM_H_AR_PATCH_VERSION     0u 
#define COMM_H_SW_MAJOR_VERSION     1u
#define COMM_H_SW_MINOR_VERSION     1u
#define COMM_H_SW_PATCH_VERSION     3u 

/*=======[I N C L U D E S]========================================================================*/
/**@req ComM466*/
/**@req ComM518*/
/**@req ComM692*/
#include "ComStack_Types.h"
#include "ComM_CfgType.h"
#include "Std_ExtendedTypes.h"

/*=======[M A C R O S]============================================================================*/
#define COMM_INSTANCE_ID    0u
/**@req ComM650*/
/**@req ComM649*/
#define COMM_E_MODE_LIMITATION                      0x2U
#define COMM_E_UNINIT                               0x3U

/*Error Code*/
#define COMM_E_NOT_INITED                           0x01U
#define COMM_E_WRONG_PARAMETERS                     0x02U
#define COMM_E_ERROR_IN_PROV_SERVICE                0x03U

/*Service Id*/
#define COMM_SERVICEID_INIT                         0x01U
#define COMM_SERVICEID_DEINIT                       0x02U
#define COMM_SERVICEID_GETSTATUS                    0x03U
#define COMM_SERVICEID_GETINHIBITIONSTATUS          0x04U
#define COMM_SERVICEID_REQUESTCOMMODE               0x05U
#define COMM_SERVICEID_GETMAXCOMMODE                0x06U
#define COMM_SERVICEID_GETREQUESTEDCOMMODE          0x07U
#define COMM_SERVICEID_GETCURRENTCOMMODE            0x08U
#define COMM_SERVICEID_PREVENTWAKEUP                0x09U
#define COMM_SERVICEID_LIMITCHANNELTONOCOMMODE      0x0bU
#define COMM_SERVICEID_LIMITECUTONOCOMMODE          0x0cU
#define COMM_SERVICEID_READINHIBITCOUNTER           0x0dU
#define COMM_SERVICEID_RESETINHIBITCOUNTER          0x0eU
#define COMM_SERVICEID_SETECUGROUPCLASSIFICATION    0x0fU
#define COMM_SERVICEID_GETVERSIONINFO               0x10U
#define COMM_SERVICEID_NM_NETWORKSTARTINDICATION    0x15U
#define COMM_SERVICEID_NM_NETWORKMODE               0x18U
#define COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE       0x19U
#define COMM_SERVICEID_NM_BUSSLEEPMODE              0x1aU
#define COMM_SERVICEID_NM_RESTARTINDICATION         0x1bU
#define COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC         0x1fU
#define COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC       0x20U
#define COMM_SERVICEID_ECUM_RUNMODEINDICATION       0x29U
#define COMM_SERVICEID_ECUM_WAKEUPINDICATION        0x2aU
#define COMM_SERVICEID_BUSSM_MODEINDICATION         0x33U
#define COMM_SERVICEID_MAINFUNCATION                0x60U

/*=======[T Y P E   D E F I N I T I O N S]========================================================*/
/**@req ComM494*/
typedef enum
{
    COMM_UNINIT = 0u,
    COMM_INIT
}ComM_InitStatusType;

/*
 *ComM_InhibitionStatusType : bit0 -> Wake up inhibition active
 *                            bit1 -> Limit to "No Communication" mode
 */
/**@req ComM496*/
typedef uint8 ComM_InhibitionStatusType;

/**@req ComM190*/
/**@req ComM248*/
typedef enum
{
    COMM_NO_COMMUNICATION = 0u, /*Communication Manager is in "No Communication" mode*/
    COMM_SILENT_COMMUNICATION,  /*Communication Manager is in "Silent Communication" mode*/
    COMM_FULL_COMMUNICATION     /*Communication Manager is in "Full Communication" mode*/
}ComM_ModeType;

/*=======[E X T E R N A L   D A T A]==============================================================*/
#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
extern CONST(ComM_ChannelCfgType, COMM_CONST) ComM_ChannelConfig[COMM_NUMBER_OF_CHANNELS];
extern CONST(ComM_UserCfgType, COMM_CONST) ComM_UserConfig[COMM_NUMBER_OF_USERS];
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]============================*/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**************************************************************************************************/
/*
 * Brief: Initializes the AUTOSAR Communication Manager and restarts the internal state machines. 
 * ServiceId: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: NVRAM manager have to be initialized to have the possibility to "direct" access the 
 * ComM parameters.
 * CallByAPI: This function iscalled by the ECU State Manager.
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Init(void);

/**************************************************************************************************/
/*
 * Brief: De-initializes (terminates) the AUTOSAR Communication Manager. 
 * ServiceId: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: This function is called by the ECU State Manager. 
 */
/**************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_DeInit(void);

/**************************************************************************************************/
/*
 * Brief: Returns the initialization status of the AUTOSAR Communication Manager.
 * ServiceId: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: Status
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: None
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus
(
    P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);

/**************************************************************************************************/
/*
 * Brief: Returns the inhibition status of a ComM channel. 
 * ServiceId: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: Status
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetInhibitionStatus
(
    NetworkHandleType Channel,
    P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);

/**************************************************************************************************/
/*
 * Brief: Requesting of a communication mode by a user.
 * ServiceId: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: User,ComMode
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_RequestComMode
(
    ComM_UserHandleType User,
    ComM_ModeType ComMode
);

/**************************************************************************************************/
/*
 * Brief: Function to query the maximum allowed communication mode of the corresponding user.
 * ServiceId: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: User
 * Param-Name[out]: ComMode
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetMaxComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/**************************************************************************************************/
/*
 * Brief: Function to query the currently requested communication mode of the corresponding user.
 * ServiceId: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: User
 * Param-Name[out]: ComMode
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetRequestedComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/**************************************************************************************************/
/*
 * Brief: Function to query the Current communication mode. ComM shall use the corresponding 
 * interfaces of the Bus State Managers to get the current communication mode.
 * ServiceId: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: User
 * Param-Name[out]: ComMode
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
/**************************************************************************************************/
/*
 * Brief: Changes the inhibition status ComMNoWakeup for the corresponding channel.
 * ServiceId: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel, Status
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp
(
    NetworkHandleType Channel,
    boolean Status
);
#endif

#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
/**************************************************************************************************/
/*
 * Brief: Changes the inhibition status to ComMNoCom for the corresponding channel. 
 * ServiceId: 0x0B
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel, Status
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_LimitChannelToNoComMode
(
    NetworkHandleType Channel,
    boolean Status
);
#endif

#if ((STD_ON == COMM_RESET_AFTER_FORCING_NO_COM) && (STD_ON == COMM_MODE_LIMITATION_ENABLED))
/**************************************************************************************************/
/*
 * Brief: Changes the inhibition status to ComMNoCom. 
 * ServiceId: 0x0C
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Status
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode(boolean Status);
#endif

#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
/**************************************************************************************************/
/*
 * Brief: This function returns the amount of rejected ¡°Full Communication¡± user requests.
 * ServiceId: 0x0D
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: CounterValue
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ReadInhibitCounter
(
    P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
);

/**************************************************************************************************/
/*
 * Brief: This function resets the ¡°Inhibited Full Communication Request Counter¡±.
 * ServiceId: 0x0E
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void);
#endif

/**************************************************************************************************/
/*
 * Brief: Changes the ECU Group Classification status.
 * ServiceId: 0x0F
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Status
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer 
 */
/**************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status);

/**************************************************************************************************/
/*
 * Brief: This function shall perform the processing of the AUTOSAR ComM activities that are not
 *        directly initiated by the calls e.g. from the RTE. There shall be one dedicated Main
          Function for each instance of ComM.
 * ServiceId: 0x60
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM429*/
/**@req ComM818*/
FUNC(void, COMM_CODE) ComM_MainFunction(NetworkHandleType Channel);

#if (STD_ON == COMM_VERSION_INFO_API)
/**************************************************************************************************/
/*
 * Brief: This function returns the published information.
 * ServiceId: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: ComM shall be initialized.
 * CallByAPI: Upper layer
 */
/**************************************************************************************************/
/**@req ComM370*/
#if (STD_ON == COMM_DEV_ERROR_DETECT)
#define ComM_GetVersionInfo(versionInfo) \
    do{ \
        if (NULL_PTR == (versionInfo))\
        { \
            Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETVERSIONINFO, \
            COMM_E_WRONG_PARAMETERS);\
        }\
        else\
        {\
            ((Std_VersionInfoType *)versionInfo)->vendorID   = COMM_VENDOR_ID; \
            ((Std_VersionInfoType *)versionInfo)->moduleID   = COMM_MODULE_ID; \
            ((Std_VersionInfoType *)versionInfo)->instanceID = COMM_INSTANCE_ID;  \
            ((Std_VersionInfoType *)versionInfo)->sw_major_version = COMM_H_SW_MAJOR_VERSION; \
            ((Std_VersionInfoType *)versionInfo)->sw_minor_version = COMM_H_SW_MINOR_VERSION; \
            ((Std_VersionInfoType *)versionInfo)->sw_patch_version = COMM_H_SW_PATCH_VERSION; \
        }\
    }while(0)
#else
#define ComM_GetVersionInfo(versionInfo) \
    do{ \
        ((Std_VersionInfoType *)versionInfo)->vendorID   = COMM_VENDOR_ID; \
        ((Std_VersionInfoType *)versionInfo)->moduleID   = COMM_MODULE_ID; \
        ((Std_VersionInfoType *)versionInfo)->instanceID = COMM_INSTANCE_ID;  \
        ((Std_VersionInfoType *)versionInfo)->sw_major_version = COMM_H_SW_MAJOR_VERSION; \
        ((Std_VersionInfoType *)versionInfo)->sw_minor_version = COMM_H_SW_MINOR_VERSION; \
        ((Std_VersionInfoType *)versionInfo)->sw_patch_version = COMM_H_SW_PATCH_VERSION; \
    }while(0)
#endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
#endif /*#if (STD_ON == COMM_VERSION_INFO_API)*/

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif /* COMM_H */

