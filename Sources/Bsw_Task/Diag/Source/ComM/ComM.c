/*================================================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <ComM.c>
 *  @brief      <Communication Manager Module Source File>
 *  @author     <pinghai.xiong>
 *  @date       <2014-02-24>
 */
/*================================================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]========================================================*/
/*  <VERSION>    <DATE>    <AUTHOR>         <REVISION LOG>
 *  V1.0.0       20140224  pinghai.xiong    Initial version.
 *
 *  V1.1.0       20140326  benneng.wei      1: Added the 'NoPending' sub-state, and process program 
 *                                             depending on the 'NoPending'.
 *                                          2: Modified internal API of the state machine.
 *                                          3: Modified public API, and so on.
 *
 *  V1.1.1       20140402  pinghai.xiong    1: Modified definitions of the 'ComM_UserType', and 
 *                                             added some code.
 *                                          2: Deleted some macro difinitions.
 *                                          3: Increased comments and adjusted some API's contents, 
 *                                             and so on. 
 *
 *  V1.1.2       20140514  pinghai.xiong    1: Added "FULL" and "PASSIVE" mutually exclusive.
 *
 *  V1.1.3       20140702  pinghai.xiong    1: Added critical code of the 'SCHM_ENTER_COMM()' and 
 *                                             'SCHM_EXIT_COMM()' .
 *                                          2: Moved definitions of the 'ComM_MainFunction_xx' in
 *                                             'SchM_ComM.h' file to 'ComM_Cfg.h'.
 */
/*================================================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===================================================*/
#define COMM_C_AR_MAJOR_VERSION  2U
#define COMM_C_AR_MINOR_VERSION  1U
#define COMM_C_AR_PATCH_VERSION  0U
#define COMM_C_SW_MAJOR_VERSION  1U
#define COMM_C_SW_MINOR_VERSION  1U
#define COMM_C_SW_PATCH_VERSION  3U

/*=======[I N C L U D E S]========================================================================*/
#include "ComM.h"
#include "ComM_BusSM.h"
#include "SchM_ComM.h"
#if (STD_ON == COMM_DCM_USED)
#include "ComM_Dcm.h"
#include "Dcm.h"
#include "Dcm_Cbk.h"
#endif /*#if (STD_ON == COMM_DCM_USED)*/
#if (STD_ON == COMM_ECUM_USED)
#include "ComM_EcuM.h"
#include "EcuM.h"
#endif /*#if (STD_ON == COMM_ECUM_USED)*/
#if (STD_ON == COMM_NM_USED)
#include "ComM_Nm.h"
#include "Nm.h"
#endif /*#if (STD_ON == COMM_NM_USED)*/
#if (STD_ON == COMM_NVM_USED)
#include "Nvm.h"
#endif /*#if (STD_ON == COMM_NVM_USED)*/
#if (STD_ON == COMM_DEM_ERROR_DETECT)
#include "Dem.h"
#endif /*#if (STD_ON == COMM_DEM_ERROR_DETECT)*/
#if (STD_ON == COMM_BUS_CAN_USED)
#include "CanSM_ComM.h"
#endif /*#if (STD_ON == COMM_BUS_CAN_USED)*/
#if (STD_ON == COMM_BUS_LIN_USED)
#include "LinSM_ComM.h"
#endif /*#if (STD_ON == COMM_BUS_LIN_USED)*/
#if (STD_ON == COMM_BUS_FLEXRAY_USED)
#include "FrSM_ComM.h"
#endif /*#if (STD_ON == COMM_BUS_FLEXRAY_USED)*/
#if (STD_ON == COMM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===============================================================*/
#if (COMM_C_AR_MAJOR_VERSION != COMM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif /*COMM_C_AR_MAJOR_VERSION != COMM_H_AR_MAJOR_VERSION */
#if (COMM_C_AR_MINOR_VERSION != COMM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif /*COMM_C_AR_MINOR_VERSION != COMM_H_AR_MINOR_VERSION */
#if (COMM_C_AR_PATCH_VERSION != COMM_H_AR_PATCH_VERSION)
    #error "ComM.c:Mismatch in Specification Patch Version"
#endif /*COMM_C_AR_PATCH_VERSION != COMM_H_AR_PATCH_VERSION */
#if (COMM_C_SW_MAJOR_VERSION != COMM_H_SW_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif /*COMM_C_SW_MAJOR_VERSION != COMM_H_SW_MAJOR_VERSION */
#if (COMM_C_SW_MINOR_VERSION != COMM_H_SW_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif /*COMM_C_SW_MINOR_VERSION != COMM_H_SW_MINOR_VERSION */
#if (COMM_C_SW_PATCH_VERSION != COMM_H_SW_PATCH_VERSION)
    #error "ComM.c:Mismatch in Specification Patch Version"
#endif /*COMM_C_AR_PATCH_VERSION != COMM_H_AR_PATCH_VERSION */

#if (2 != COMM_BUSSM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != COMM_BUSSM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif

#if (1 != SCHM_COMM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != SCHM_COMM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif

#if (STD_ON == COMM_DCM_USED)
#if (2 != COMM_DCM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != COMM_DCM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#if (3 != DCM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (3 != DCM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_DCM_USED)*/

#if (STD_ON == COMM_ECUM_USED)
#if (2 != COMM_ECUM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != COMM_ECUM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#if (1 != ECUM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (3 != ECUM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_ECUM_USED)*/

#if (STD_ON == COMM_NM_USED)
#if (2 != COMM_NM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != COMM_NM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#if (1 != NM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != NM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_NM_USED)*/

#if (STD_ON == COMM_NVM_USED)
#if (2 != NVM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (3 != NVM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_NVM_USED)*/

#if (STD_ON == COMM_DEM_ERROR_DETECT)
#if (3 != DEM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != DEM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_DEM_ERROR_DETECT)*/

#if (STD_ON == COMM_BUS_CAN_USED)
#if (1 != CANSM_COMM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (2 != CANSM_COMM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_BUS_CAN_USED)*/

#if (STD_ON == COMM_BUS_LIN_USED)
#if (1 != LINSM_COMM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != LINSM_COMM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_BUS_LIN_USED)*/

#if (STD_ON == COMM_BUS_FLEXRAY_USED)
#if (1 != FRSM_COMM_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (1 != FRSM_COMM_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Minor Version"
#endif
#endif /*#if (STD_ON == COMM_BUS_FLEXRAY_USED)*/

#if (STD_ON == COMM_DEV_ERROR_DETECT)
#if (2 != DET_H_AR_MAJOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#if (2 != DET_H_AR_MINOR_VERSION)
    #error "ComM.c:Mismatch in Specification Major Version"
#endif
#endif /*STD_ON == COMM_DEV_ERROR_DETECT*/

/*=======[M A C R O S]============================================================================*/
/* Defines internal status */
#define COMM_STATUS_NO          0u
#define COMM_STATUS_NOPENDING   1u
#define COMM_STATUS_NETREQ      2u
#define COMM_STATUS_READY       3u
#define COMM_STATUS_SILENT      4u

#define COMM_MAX_REJECT_COUNTER     0xFFFFu
#define COMM_MAX_NVM_TIMEOUT        0xFFFFu

/* Other module notify event */
#define COMM_EVENT_DCM_ACTIVE       0x0001u
#define COMM_EVENT_ECUM_WAKEUP      0x0002u
#define COMM_EVENT_ECUM_RUNMODE     0x0004u
#define COMM_EVENT_NM_START         0x0008u
#define COMM_EVENT_NM_RESTART       0x0010u
#define COMM_EVENT_NM_NETMODE       0x0020u
#define COMM_EVENT_NM_PRESLEEP      0x0040u
#define COMM_EVENT_NM_BUSSLEEP      0x0080u

/* Bus mode indication event */
#define COMM_EVENT_BUS_REQUEST      0x1000u 
#define COMM_EVENT_BUS_OK           0x2000u
#define COMM_EVENT_BUS_FAIL         0x4000u
#define COMM_EVENT_BUS_IND          0x6000u

/* Defines event of all network requested passively in case of the NM is used. */ 
#define COMM_EVENT_PASSIVE_STARTUP \
    (COMM_EVENT_ECUM_WAKEUP|COMM_EVENT_NM_START|COMM_EVENT_NM_RESTART)

/* Defines the NM mode indication */
#define COMM_EVENT_NM_IND \
    (COMM_EVENT_NM_NETMODE|COMM_EVENT_NM_PRESLEEP|COMM_EVENT_NM_BUSSLEEP)

/* Inhibit wakeup and limit to NoCom mask */
#define COMM_MASK_INHIBIT_WAKEUP    0x01u
#define COMM_MASK_LIMIT_TO_NOCOM    0x02u

/* Maximum identification of the user */
#define COMM_MAX_USER_ID        (COMM_NUMBER_OF_USERS - 1u)

/* Maximum identification of the channel */
#define COMM_MAX_CHANNEL_ID     (COMM_NUMBER_OF_CHANNELS - 1u)

/* Set event status flag */
#define COMM_EVENT_SET(_ch,_mask) \
    do{ComM_Channel[_ch].Events |= (_mask);}while(0)

/* Clear event status flag */
#define COMM_EVENT_CLR(_ch,_mask) \
    do{ComM_Channel[_ch].Events &= ((uint16)~(_mask));}while(0)

/* Get event status */
#define COMM_EVENT_IS_SET(_ch,_mask) \
    (0u != (ComM_Channel[_ch].Events & (_mask)))

/* Check whether wakeup inhibition is enabled or not. */
#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
#define COMM_IS_INHIBIT_WAKEUP(_ch) \
  ((ComM_Global.EcuGroupClassification & ComM_Channel[_ch].InhibitStatus)==COMM_MASK_INHIBIT_WAKEUP)
#endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/

/* Check whether limitation is enabled or not. */
#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
#define COMM_IS_LIMIT_TO_NOCOM(_ch) \
    ((COMM_PASSIVE == ComM_ChannelConfig[_ch].ComMNmVariant) || \
    ((ComM_Global.EcuGroupClassification & COMM_MASK_LIMIT_TO_NOCOM) && \
    ((ComM_Global.EcuNoCom) || (ComM_Channel[_ch].InhibitStatus & COMM_MASK_LIMIT_TO_NOCOM))))
#endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

/* Channel map in user */
#define COMM_CHANNEL_MAP(_ch,_user) \
    (ComM_UserConfig[_user].ChannelMap[(uint8)(_ch) >> 3u])
    
/* Whether is a channel in the user or not. */
#define COMM_IS_CHANNEL_IN_USER(_ch,_user) \
    (0u != ((uint8)(1u << ((_ch)&7u)) & COMM_CHANNEL_MAP(_ch,_user)))

/* Read or write non-volatile memory. */
#if (STD_ON == COMM_NVM_USED)
#define COMM_NVM_READ_NO_WAKEUP(_ch) \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_ReadBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)
#define COMM_NVM_READ_GROUP_CLASSIFICATION() \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_ReadBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)
#define COMM_NVM_READ_INHIBIT_COUNTER() \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_ReadBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)

#define COMM_NVM_SAVE_NO_WAKEUP(_ch) \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_WriteBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)
#define COMM_NVM_SAVE_GROUP_CLASSIFICATION() \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_WriteBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)
#define COMM_NVM_SAVE_INHIBIT_COUNTER() \
    SCHM_ENTER_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS); \
    do{uint8 *dst;(void)NvM_WriteBlock(0,dst);}while(0); \
    SCHM_EXIT_COMM(COMM_INSTANCE_ID,COMM_AREA_NVM_ACCESS)
#endif /*#if (STD_ON == COMM_NVM_USED)*/

/* Get handles of mode processing.  */
#define COMM_MODE_HANDLER(_ch) \
    (ComM_ModeHandle[ComM_Channel[_ch].Status])

/* Get handles of requestes communication mode. */
#define COMM_SM_REQ_COMMODE(_ch,_mode) \
    (ComM_SmHandle[ComM_ChannelConfig[_ch].ComMBusType].RequestComMode((_ch),(_mode)))

/* Get handles of read communication mode. */
#define COMM_SM_GET_COMMODE(_ch,_mode) \
    (ComM_SmHandle[ComM_ChannelConfig[_ch].ComMBusType].GetCurrentComMode((_ch),&(_mode)))

/* Set bus mode depending on the channel. */
#define COMM_SET_BUS_MODE(_ch, _mode) \
    do {\
        ComM_Channel[_ch].CurrentMode = (_mode);\
        COMM_EVENT_SET((_ch), COMM_EVENT_BUS_REQUEST);\
        if (E_OK != COMM_SM_REQ_COMMODE((_ch), (_mode))) {\
            COMM_EVENT_CLR((_ch), COMM_EVENT_BUS_REQUEST);\
            COMM_EVENT_SET((_ch), COMM_EVENT_BUS_FAIL);\
        }\
    }while(0)

/*===============[T Y P E   D E F I N I T I O N S]================================================*/
/* Defines handler of switches communication mode. */
typedef P2FUNC(void, COMM_APPL_CODE, ComM_ModeHandleType)(NetworkHandleType Channel);

typedef struct
{
    /* Defines handler of requests communication mode. */
    P2FUNC(Std_ReturnType, COMM_APPL_CODE, RequestComMode)(NetworkHandleType,ComM_ModeType);

    /* Defines handler of gets current communication mode. */
    P2FUNC(Std_ReturnType, COMM_APPL_CODE, GetCurrentComMode)
    (
        NetworkHandleType,
        P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA)
    );
    
}ComM_SmHandleType;

/* Defines run-time structure of a channel. */
typedef struct
{
    /* Previous mode of a channel */
    ComM_ModeType PreviousMode;
    
    /* Current mode of a channel */
    ComM_ModeType CurrentMode;

    /* Status of a channel */
    uint8 Status;

    /* Inhibit status of a channel */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
    ComM_InhibitionStatusType InhibitStatus;
#endif
    /*#if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)*/

    /* Remian times of a channel in the current mode */
    uint16_least Timeout;

    /* Other module flag events */
    uint16 Events;

    /* User counter of requests "full-communication" */
    uint8 UserFullCounter;
    
}ComM_ChannelType;

/* Defines run-time structure of a user. */
typedef struct
{
    /* User request mode currently */
    ComM_ModeType RequestMode;

#if (STD_ON == COMM_RTE_NOTIFY_USED)
    /* Old mode */
    ComM_ModeType OldMode;

    /* Channel counter of the mode(eg. full-com, silent-com, no-com). */
    uint8 ChCounter[3u];
#endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/
    
}ComM_UserType;

/* Defines global run-time structure. */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
typedef struct
{
    /* Inhibit counter of requests "full-communication" */
    uint16 InhibitCounter;

    /* Whether enable "wakeup-inhibition" and "communicate-limitiation" or not */
    ComM_InhibitionStatusType EcuGroupClassification;

    /* Whether enable "communicate-limitation" of the ECU or not */
    boolean EcuNoCom;
    
}ComM_GlobalType;
#endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)*/


/*===============[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]====================*/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/* Request internal communication mode. */
STATIC FUNC(Std_ReturnType, COMM_CODE)
ComM_IntRequestComMode(NetworkHandleType Channel, ComM_ModeType ComMode);

/* Get internal commnication mode currently. */
STATIC FUNC(Std_ReturnType, COMM_CODE) ComM_IntGetCurrentComMode
(
    NetworkHandleType Channel,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComModePtr
);

/* Handle different mode. */
STATIC FUNC(void, COMM_CODE) ComM_NetworkRequestedHandler(NetworkHandleType Channel);
STATIC FUNC(void, COMM_CODE) ComM_ReadySleepHandler(NetworkHandleType Channel);
#if (STD_ON == COMM_NM_USED)
STATIC FUNC(void, COMM_CODE) ComM_SilentComHandler(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_NM_USED)*/
STATIC FUNC(void, COMM_CODE) ComM_NoComHandler(NetworkHandleType Channel);
#if (STD_ON == COMM_ECUM_USED)
STATIC FUNC(void, COMM_CODE) ComM_NoComPendingHandler(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_ECUM_USED)*/

/* Enter to different mode. */
STATIC FUNC(void, COMM_CODE) ComM_EnterNoCom(NetworkHandleType Channel);
#if (STD_ON == COMM_ECUM_USED)
STATIC FUNC(void, COMM_CODE) ComM_EnterNoComPending(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_ECUM_USED)*/
#if (STD_ON == COMM_NM_USED)
STATIC FUNC(void, COMM_CODE) ComM_EnterSilentCom(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_NM_USED)*/
STATIC FUNC(void, COMM_CODE) ComM_EnterNetworkRequested(NetworkHandleType Channel);
STATIC FUNC(void, COMM_CODE) ComM_EnterReadySleep(NetworkHandleType Channel);

/* Handle the DCM state indication. */
STATIC FUNC(void, COMM_CODE)  ComM_BusModeIndication(NetworkHandleType Channel);

/* Handle upper-layer(RTE) state indication. */
#if (STD_ON == COMM_RTE_NOTIFY_USED)
STATIC FUNC(void, COMM_CODE) ComM_UserModeNotify(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/

/* Handle the NM state indication. */
#if (STD_ON == COMM_NM_USED)
STATIC FUNC(void, COMM_CODE) ComM_NmModeProcessing(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_NM_USED)*/

/* Handle "Full Com" request of the user. */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED)) 
STATIC FUNC(void, COMM_CODE) ComM_UserRequestFullCom(NetworkHandleType Channel);
#endif /*#if ((STD_ON==COMM_WAKEUP_INHIBITION_ENABLED)||(STD_ON==COMM_MODE_LIMITATION_ENABLED))*/

/*================================[I N T E R N A L   D A T A]=====================================*/
#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
/* Defines run-time variable of mode handler. */
STATIC CONST(ComM_ModeHandleType, COMM_CONST) ComM_ModeHandle[5u] = 
{
    &ComM_NoComHandler,
    #if (STD_ON == COMM_ECUM_USED)
    &ComM_NoComPendingHandler,
    #else
    NULL_PTR,
    #endif /*#if (STD_ON == COMM_ECUM_USED)*/
    &ComM_NetworkRequestedHandler,
    &ComM_ReadySleepHandler,
    #if (STD_ON == COMM_NM_USED)
    &ComM_SilentComHandler
    #else
    NULL_PTR
    #endif /*#if (STD_ON == COMM_NM_USED)*/
};

/* Defines run-time variable of state manager. */
STATIC CONST(ComM_SmHandleType, COMM_CONST) ComM_SmHandle[4u] = 
{
    /* COMM_BUS_TYPE_CAN = 0 */
    #if (STD_ON == COMM_BUS_CAN_USED)
    {&CanSM_RequestComMode,&CanSM_GetCurrentComMode},
    #else
    {NULL_PTR,NULL_PTR},
    #endif /*#if (STD_ON == COMM_BUS_CAN_USED)*/

    /* COMM_BUS_TYPE_FR = 1 */
    #if (STD_ON == COMM_BUS_FLEXRAY_USED)
    {&FrSM_RequestComMode,&FrSM_GetCurrentComMode},
    #else
    {NULL_PTR,NULL_PTR},
    #endif /*#if (STD_ON == COMM_BUS_FLEXRAY_USED)*/

    /* COMM_BUS_TYPE_INTERNAL = 2 */
    {&ComM_IntRequestComMode,&ComM_IntGetCurrentComMode},

    /* COMM_BUS_TYPE_LIN = 3 */
    #if (STD_ON == COMM_BUS_LIN_USED)
    {&LinSM_RequestComMode,&LinSM_GetCurrentComMode}
    #else
    {NULL_PTR,NULL_PTR}
    #endif /*#if (STD_ON == COMM_BUS_LIN_USED)*/
};
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_UNSPECIFIED
#include "ComM_MemMap.h"
/* Defines global run-time variable */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
STATIC VAR(ComM_GlobalType, COMM_VAR) ComM_Global;
#endif /*#if ((COMM_WAKEUP_INHIBITION_ENABLED==STD_ON) || (COMM_MODE_LIMITATION_ENABLED==STD_ON))*/

/* Defines run-time variable of all users */
STATIC VAR(ComM_UserType, COMM_VAR) ComM_User[COMM_NUMBER_OF_USERS];

/* Defines run-time variable of all channels */
STATIC VAR(ComM_ChannelType, COMM_VAR) ComM_Channel[COMM_NUMBER_OF_CHANNELS];
#define COMM_STOP_SEC_VAR_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
/* Defines status variable of the COMM */
STATIC VAR(ComM_InitStatusType, COMM_VAR_POWER_ON_INIT) ComM_Status = COMM_UNINIT;
#define COMM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "ComM_MemMap.h"

/*====================[F U N C T I O N   I M P L E M E N T A T I O N S]===========================*/
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
/**@req ComM313*/
/**@req ComM146*/
/**@req ComM793*/
FUNC(void, COMM_CODE) ComM_Init(void)
{
    if (COMM_UNINIT == ComM_Status)
    {
        NetworkHandleType index = 0;
        
        #if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)
        ComM_Global.EcuNoCom = COMM_NO_COM;
        ComM_Global.InhibitCounter = 0;
        ComM_Global.EcuGroupClassification = COMM_ECU_GROUP_CLASSIFICATION;
        #if (STD_ON == COMM_NVM_USED)       
        /* Restore "InhibitCounter" and "EcuGroupClassification" from the non-volatile memory */
        COMM_NVM_READ_INHIBIT_COUNTER();
        COMM_NVM_READ_GROUP_CLASSIFICATION();
        #endif /*#if (STD_ON == COMM_NVM_USED) */
        #endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)*/

        for (index = 0u; index < COMM_NUMBER_OF_USERS; index++)
        {
            /**@req ComM485*/
            ComM_User[index].RequestMode = COMM_NO_COMMUNICATION;
            #if (STD_ON == COMM_RTE_NOTIFY_USED)
            ComM_User[index].ChCounter[0u] = ComM_UserConfig[index].NumChannel;
            ComM_User[index].ChCounter[1u] = 0u;
            ComM_User[index].ChCounter[2u] = 0u;
            ComM_User[index].OldMode = COMM_NO_COMMUNICATION;
            #endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/
        }

        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            ComM_Channel[index].PreviousMode = COMM_NO_COMMUNICATION;
            ComM_Channel[index].CurrentMode = COMM_NO_COMMUNICATION;
            ComM_Channel[index].Status = COMM_STATUS_NO;
            ComM_Channel[index].Timeout = 0u;
            ComM_Channel[index].Events = 0u;
            ComM_Channel[index].UserFullCounter = 0u;
            
            #if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)
            ComM_Channel[index].InhibitStatus = 0u;
            #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
            if (ComM_ChannelConfig[index].ComMNoWakeup)
            {
                ComM_Channel[index].InhibitStatus |= COMM_MASK_INHIBIT_WAKEUP;
            }
            #if (STD_ON == COMM_NVM_USED)
            /* Restore ComMNoWakeup from the non-volatile memory */
            COMM_NVM_READ_NO_WAKEUP(index);
            #endif /*#if (STD_ON == COMM_NVM_USED) */
            #endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) */
            #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
            if (ComM_ChannelConfig[index].ComMNoCom)
            {
                ComM_Channel[index].InhibitStatus |= COMM_MASK_LIMIT_TO_NOCOM;
            }
            #endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED) */
            #endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED) || COMM_MODE_LIMITATION_ENABLED) */
        }

        /* Set mode status */
        ComM_Status = COMM_INIT;
    }

	return;
}

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
/**@req ComM147*/
/**@req ComM794*/
FUNC(void, COMM_CODE) ComM_DeInit(void)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        /**@req ComM580*/
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DEINIT, COMM_E_NOT_INITED);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        NetworkHandleType index = 0;
        
        /* Check whether all channels are in No Communication mode */
        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            if (COMM_NO_COMMUNICATION != ComM_Channel[index].CurrentMode)
            {
                /**@req ComM58*/
                #if (STD_ON == COMM_DEM_ERROR_DETECT)
                Dem_ReportErrorStatus(COMM_E_LOWER_LIMIT, DEM_EVENT_STATUS_FAILED);
                #endif /*#if (STD_ON == COMM_DEM_ERROR_DETECT)*/
                
                return;
            }
        }
        
        /* Done, if all channel is in No Communication. */
        ComM_Status = COMM_UNINIT;
    }
}

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
/**@req ComM242*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus
(
    P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
    Std_ReturnType ret = E_NOT_OK;
    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (NULL_PTR == Status)
    {
    	Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETSTATUS, 
    	COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
    {
        *Status = ComM_Status;
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM619*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetInhibitionStatus
(
    NetworkHandleType Channel,
    P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
    {
        #if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)
        *Status = ComM_Channel[Channel].InhibitStatus;
        #else
        *Status = 0u;
        #endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)*/
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM283*//**@req ComM191*//**@req ComM471*//**@req ComM500*//**@req ComM129*/
/**@req ComM110*//**@req ComM795*//**@req ComM441*//**@req ComM439*//**@req ComM441*/
/**@req ComM442*//**@req ComM68*/
FUNC(Std_ReturnType, COMM_CODE) ComM_RequestComMode
(
    ComM_UserHandleType User,
    ComM_ModeType ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE, 
        COMM_E_NOT_INITED);
    }
    /**@req ComM151*/
    else if ((COMM_NO_COMMUNICATION != ComMode) && (COMM_FULL_COMMUNICATION != ComMode))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
    {
        NetworkHandleType index = 0u;
        
        /**@req ComM343*/
        if (ComMode == ComM_User[User].RequestMode)
        {
            return E_OK;
        }

        ComM_User[User].RequestMode = ComMode;

        while (index < COMM_NUMBER_OF_CHANNELS)
        {
        	if (COMM_IS_CHANNEL_IN_USER(index,User))
            {
                if (COMM_FULL_COMMUNICATION == ComMode)
                {
                    ComM_Channel[index].UserFullCounter++;
                    #if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) \
                        || (STD_ON == COMM_MODE_LIMITATION_ENABLED)) 
                    ComM_UserRequestFullCom(index);
                    #endif
                    /*#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
                        || (STD_ON == COMM_MODE_LIMITATION_ENABLED))*/
                }
                else
                {
                    if (ComM_Channel[index].UserFullCounter > 0u)
                    {
                        ComM_Channel[index].UserFullCounter--;
                    }
                }
            }
            index += 1u;
        }
        
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM85*/
/**@req ComM374*/
/**@req ComM796*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetMaxComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
        NetworkHandleType index = 0u;
        
        *ComMode = COMM_FULL_COMMUNICATION;
        while ((index < COMM_NUMBER_OF_CHANNELS) && (*ComMode != COMM_NO_COMMUNICATION))
        {
        	if (COMM_IS_CHANNEL_IN_USER(index,User))
            {
                switch(ComM_Channel[index].CurrentMode)
                {           
                case COMM_NO_COMMUNICATION:
                    #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)   
                    if (COMM_IS_INHIBIT_WAKEUP(index))
                    {
                        *ComMode = COMM_NO_COMMUNICATION;
                    } 
                    #endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) */
                    break;
             
                case COMM_SILENT_COMMUNICATION:
                    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
                    if (COMM_IS_LIMIT_TO_NOCOM(index) && (*ComMode == COMM_FULL_COMMUNICATION))
                    {
                        *ComMode = COMM_SILENT_COMMUNICATION;
                    }
                    #endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/
                    break;
                
                default:
                    break;               
                }                  
            }
            index += 1u;
        }
        #else
        *ComMode = COMM_FULL_COMMUNICATION;
        #endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/
        
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM80*/
/**@req ComM79*/
/**@req ComM797*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetRequestedComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
	Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        *ComMode = ComM_User[User].RequestMode;
        ret = E_OK;
    }

    return ret;
}

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
/**@req ComM84*/
/**@req ComM83*/
/**@req ComM176*/
/**@req ComM798*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
    {
        NetworkHandleType index = 0u;
        ComM_ModeType mode = COMM_FULL_COMMUNICATION;
        
        *ComMode = COMM_FULL_COMMUNICATION;

        while (index < COMM_NUMBER_OF_CHANNELS)
        {
        	if (COMM_IS_CHANNEL_IN_USER(index, User))
            {
                if (E_OK == COMM_SM_GET_COMMODE(index, mode))
                {
                    if (COMM_NO_COMMUNICATION == mode)
                    {
                        *ComMode = mode;
                        break;
                    }
                    else
                    {
                        if (COMM_SILENT_COMMUNICATION == mode)
                        {
                            *ComMode = mode;
                        }
                    }
                }
                #if (STD_ON == COMM_DEV_ERROR_DETECT)
                else
                {
                    Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, 
                    COMM_SERVICEID_GETCURRENTCOMMODE, COMM_E_ERROR_IN_PROV_SERVICE);
                }
                #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
            }
            index += 1u;
        }
        
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM301*/
/**@req ComM488*/
/**@req ComM156*/
/**@req ComM799*/
FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp
(
    NetworkHandleType Channel,
    boolean Status
)
{
	Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_PREVENTWAKEUP, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_PREVENTWAKEUP, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT)*/
    {
        if (Status)
        {
        	ComM_Channel[Channel].InhibitStatus |= COMM_MASK_INHIBIT_WAKEUP;
        }
        else
        {
        	ComM_Channel[Channel].InhibitStatus &= ~COMM_MASK_INHIBIT_WAKEUP;
        }
        ret = E_OK;

        #if (STD_ON == COMM_NVM_USED)
        /* Save "ComMNoWakeup" to the non-volatile memory */
        COMM_NVM_SAVE_NO_WAKEUP(Channel);
        #endif /*#if (STD_ON == COMM_NVM_USED)*/
    }
    
    return ret;
}
#endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/

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
/**@req ComM301*/
/**@req ComM488*/
/**@req ComM163*/
/**@req ComM800*/
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitChannelToNoComMode
(
    NetworkHandleType Channel,
    boolean Status
)
{
	Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITCHANNELTONOCOMMODE, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITCHANNELTONOCOMMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        if (TRUE == Status)
        {
        	ComM_Channel[Channel].InhibitStatus |= COMM_MASK_LIMIT_TO_NOCOM;
        }
        else
        {
        	ComM_Channel[Channel].InhibitStatus &= ~COMM_MASK_LIMIT_TO_NOCOM;
        }
        ret = E_OK;
    }
    
    return ret;
}
#endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

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
/**@req ComM301*/
/**@req ComM124*/
/**@req ComM801*/
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode(boolean Status)
{
	Std_ReturnType ret = E_NOT_OK;

    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITECUTONOCOMMODE, 
        COMM_E_NOT_INITED);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        ComM_Global.EcuNoCom = Status;
        ret = E_OK;
    }
    
    return ret;
}
#endif /*#if ((STD_ON==COMM_RESET_AFTER_FORCING_NO_COM) && (STD_ON==COMM_MODE_LIMITATION_ENABLED))*/

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
/**@req ComM224*/
/**@req ComM802*/
FUNC(Std_ReturnType, COMM_CODE) ComM_ReadInhibitCounter
(
    P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
    Std_ReturnType ret = E_NOT_OK;
    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_READINHIBITCOUNTER, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == CounterValue)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_READINHIBITCOUNTER, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        *CounterValue = ComM_Global.InhibitCounter;
        ret = E_OK;
    }
    
    return ret;
}

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
/**@req ComM108*/
/**@req ComM803*/
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void)
{
    Std_ReturnType ret = E_NOT_OK;
    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_RESETINHIBITCOUNTER, 
        COMM_E_NOT_INITED);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        ComM_Global.InhibitCounter = 0u;
        ret = E_OK;
        
        #if (STD_ON == COMM_NVM_USED)
        /* Save "InhibitCounter" to the non-volatile memory */
        COMM_NVM_SAVE_INHIBIT_COUNTER();
        #endif /*#if (STD_ON == COMM_NVM_USED) */    
    }
    
    return ret;
}
#endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
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
/**@req ComM43*/
/**@req ComM552*/
FUNC(Std_ReturnType, COMM_CODE) ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status)
{
    Std_ReturnType ret = E_NOT_OK;
    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_SETECUGROUPCLASSIFICATION, 
        COMM_E_NOT_INITED);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        ComM_Global.EcuGroupClassification = Status;
        ret = E_OK;

        #if (STD_ON == COMM_NVM_USED)
        /* Save "EcuGroupClassification" to the non-volatile memory */
        COMM_NVM_SAVE_GROUP_CLASSIFICATION();
        #endif /*#if (STD_ON == COMM_NVM_USED) */
    }
    
    return ret;
}
#endif /*#if ((STD_ON==COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON==COMM_MODE_LIMITATION_ENABLED)) */

#if (STD_ON == COMM_NM_USED)
/**************************************************************************************************/
/*
 * Brief: Indication that a NM-message has been received in the Bus Sleep Mode, what indicates 
 * that some nodes in the network have already entered the Network Mode.
 * ServiceId: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: NmIf
 */
/**************************************************************************************************/
/**@req ComM383*/
/**@req ComM804*/
/**@req ComM805*/
FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
                && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_START);
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Network Mode.
 * ServiceId: 0x18
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM390*/
/**@req ComM806*/
/**@req ComM807*/
FUNC(void, COMM_CODE) ComM_Nm_NetworkMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
                && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_NETMODE);
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Prepare Bus-Sleep Mode.
 * ServiceId: 0x19
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM391*/
/**@req ComM808*/
/**@req ComM809*/
FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
                && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_PRESLEEP);
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: Notification that the network management has entered Bus-Sleep Mode.
 * ServiceId: 0x1A
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM392*/
/**@req ComM810*/
/**@req ComM811*/
FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
                && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_BUSSLEEP);
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: If NmIf has started to shut down the coordinated busses, AND not all coordinated busses 
 *        have indicated bus sleep state, AND on at least on one of the coordinated busses NM is 
 *        restarted, THEN the NM Interface shall call the callback function ComM_Nm_RestartIndication
 *        with the nmNetworkHandle of the channels which have already indicated bus sleep state.
 * ServiceId: 0x1B
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM792*/
/**@req ComM812*/
/**@req ComM813*/
FUNC(void, COMM_CODE) ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
                && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_RESTART);       
    }

	return;
}
#endif /*#if (STD_ON == COMM_NM_USED)*/

#if (STD_ON == COMM_DCM_USED)
/**************************************************************************************************/
/*
 * Brief: Indication of active diagnostic by the DCM.
 * ServiceId: 0x1F
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM346*/
/**@req ComM362*/
FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        if (COMM_PASSIVE != ComM_ChannelConfig[Channel].ComMNmVariant)
        {
            COMM_EVENT_SET(Channel, COMM_EVENT_DCM_ACTIVE);
        }
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: Indication of inactive diagnostic by the DCM.
 * ServiceId: 0x20
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM364*/
FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_CLR(Channel, COMM_EVENT_DCM_ACTIVE);
    }

	return;
}
#endif /*#if (STD_ON == COMM_DCM_USED)*/

#if (STD_ON == COMM_ECUM_USED)
/**************************************************************************************************/
/*
 * Brief: Indication that ECU State Manager has entered "Run Mode". 
 * ServiceId: 0x29
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM406*/
FUNC(void, COMM_CODE) ComM_EcuM_RunModeIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_RUNMODEINDICATION, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_RUNMODEINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_ECUM_RUNMODE);
    }

	return;
}

/**************************************************************************************************/
/*
 * Brief: Notification of a wake up on the corresponding channel. 
 * ServiceId: 0x2A
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM694*/
/**@req ComM275*/
/**@req ComM814*/
/**@req ComM815*/
/**@req ComM316*/
FUNC(void, COMM_CODE) ComM_EcuM_WakeUpIndication(NetworkHandleType Channel)
{    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_WAKEUPINDICATION, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_WAKEUPINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        #if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)
        for (Channel = 0u; Channel < COMM_NUMBER_OF_CHANNELS; Channel++)
        #endif /*#if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)*/
        {
            COMM_EVENT_SET(Channel, COMM_EVENT_ECUM_WAKEUP);
        }
    }

	return;
}
#endif /*#if (STD_ON == COMM_ECUM_USED)*/

/**************************************************************************************************/
/*
 * Brief: Indication of the actual bus mode bythe corresponding Bus State Manager. ComM shall 
 *        propagate the indicated state tothe users with means of the RTE. 
 * ServiceId: 0x33
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: None
 */
/**************************************************************************************************/
/**@req ComM675*/
/**@req ComM816*/
/**@req ComM817*/
FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
    NetworkHandleType Channel,
    P2VAR(ComM_ModeType, AUTOMATIC, CANIF_APPL_DATA) ComMode
)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION, 
        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if ((COMM_NO_COMMUNICATION != *ComMode) && (COMM_SILENT_COMMUNICATION != *ComMode) 
        && (COMM_FULL_COMMUNICATION != *ComMode))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
    	if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_REQUEST))
        {
        	COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_REQUEST);
        	if (ComM_Channel[Channel].CurrentMode == *ComMode)
            {
            	COMM_EVENT_SET(Channel, COMM_EVENT_BUS_OK);
            }
        	else
            {
            	COMM_EVENT_SET(Channel, COMM_EVENT_BUS_FAIL);
            }
        }
    }

	return;
}

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
FUNC(void, COMM_CODE) ComM_MainFunction(NetworkHandleType Channel)
{    
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_MAINFUNCATION, 
        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_MAINFUNCATION, 
        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /*#if (STD_ON == COMM_DEV_ERROR_DETECT) */
    {
        ComM_ModeHandleType pHandle = COMM_MODE_HANDLER(Channel);
        
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_IND))
        {
            if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_OK))
            {
                #if (STD_ON == COMM_RTE_NOTIFY_USED)
                ComM_UserModeNotify(Channel);
                #endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/
                ComM_BusModeIndication(Channel);
            }
            else
            {
                COMM_SET_BUS_MODE(Channel, ComM_Channel[Channel].CurrentMode);
            }
            
            COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_IND);
        }

        #if (STD_ON == COMM_NM_USED)
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_IND))
        {
            ComM_NmModeProcessing(Channel);
            COMM_EVENT_CLR(Channel, COMM_EVENT_NM_IND);
        }
        #endif /*#if (STD_ON == COMM_NM_USED)*/
        
        if (NULL_PTR != pHandle)
        {
            pHandle(Channel);
        }
    }

	return;
}

/*=============[I N T E R N A L   F U N C T I O N   I M P L E M E N T A T I O N S]================*/
/**@req ComM51*/ /**@req ComM52*/ /**@req ComM53*/ /**@req ComM786*//**@req ComM787*/
/**@req ComM788*//**@req ComM210*//**@req ComM211*//**@req ComM784*//**@req ComM688*/
/**@req ComM130*//**@req ComM207*//**@req ComM785*//**@req ComM71*/ /**@req ComM72*/
/**@req ComM299*//**@req ComM610*//**@req ComM671*//**@req ComM479*/
/**************************************************************************************************/
/*
 * Brief: Request internal communication mode.
 * Param-Name[in]: Channel,ComM_Mode
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: None
 * CallByAPI: None
 */
/**************************************************************************************************/
STATIC FUNC(Std_ReturnType, COMM_CODE) ComM_IntRequestComMode
(
    NetworkHandleType Channel,
    ComM_ModeType ComMode
)
{
    COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_REQUEST);
    COMM_EVENT_SET(Channel, COMM_EVENT_BUS_OK);
    
    return E_OK;
}

/**************************************************************************************************/
/*
 * Brief: Get internal communication mode currently.
 * Param-Name[in]: Channel
 * Param-Name[out]: ComM_ModePtr
 * Param-Name[in/out]: None
 * Return: Std_ReturnType
 * PreCondition: None
 * CallByAPI: None
 */
/**************************************************************************************************/
STATIC FUNC(Std_ReturnType, COMM_CODE) ComM_IntGetCurrentComMode
(
    NetworkHandleType Channel, 
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComModePtr
)
{    
    *ComModePtr = ComM_Channel[Channel].CurrentMode;
    
    return E_OK;
}

/**************************************************************************************************/
/*
 * Brief: Handle of entering "No" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
#if (STD_ON == COMM_RTE_NOTIFY_USED)
STATIC FUNC(void, COMM_CODE) ComM_UserModeNotify(NetworkHandleType Channel)
{
    uint8 user = COMM_NUMBER_OF_USERS;
    P2VAR(ComM_UserType, AUTOMATIC, COMM_APPL_DATA) pUser;
    ComM_ModeType NewMode;

    while (user > 0u)
    {
        user--;
        if (COMM_IS_CHANNEL_IN_USER(Channel,user))
        {
            pUser = &ComM_User[user];
            pUser->ChCounter[ComM_Channel[Channel].PreviousMode]--;
            pUser->ChCounter[ComM_Channel[Channel].CurrentMode]++;
            if (pUser->ChCounter[COMM_NO_COMMUNICATION] > 0x00u)
            {
                NewMode = COMM_NO_COMMUNICATION;
            }        
            else if (pUser->ChCounter[COMM_SILENT_COMMUNICATION] > 0x00u)
            {
                NewMode = COMM_SILENT_COMMUNICATION;
            }        
            else
            {
                NewMode = COMM_FULL_COMMUNICATION;
            }
            
            if (pUser->OldMode != NewMode)
            {
                pUser->OldMode = NewMode;
                if (NULL_PTR != ComM_UserConfig[user].RteNotify)
                {
                    ComM_UserConfig[user].RteNotify((uint8)NewMode);
                }  
            }
        }
    }
    
    ComM_Channel[Channel].PreviousMode = ComM_Channel[Channel].CurrentMode;
    
	return;
}
#endif

/**************************************************************************************************/
/*
 * Brief: Handle of entering "No" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE)  ComM_BusModeIndication(NetworkHandleType Channel)
{
    switch(ComM_Channel[Channel].CurrentMode)
    {
    case COMM_NO_COMMUNICATION:
        #if (STD_ON == COMM_DCM_USED)
        Dcm_ComM_NoComModeEntered(Channel);
        #endif /*#if (COMM_DCM_USED == STD_ON)*/
        #if (COMM_ECUM_USED == STD_ON)
        (void)EcuM_ComM_ReleaseRUN(Channel);
        #if (COMM_RESET_AFTER_FORCING_NO_COM == STD_ON)
        EcuM_SelectShutdownTarget(ECUM_STATE_RESET, 0u);
        EcuM_KillAllRUNRequests();
        #endif /*#if (COMM_RESET_AFTER_FORCING_NO_COM == STD_ON)*/
        #endif /*#if (COMM_ECUM_USED == STD_ON)*/
        break;

    #if (COMM_DCM_USED == STD_ON)
    case COMM_SILENT_COMMUNICATION:            
        Dcm_ComM_SilentComModeEntered(Channel);
        break;

    case COMM_FULL_COMMUNICATION:          
        Dcm_ComM_FullComModeEntered(Channel);            
        break;
    #endif /*#if (COMM_DCM_USED == STD_ON)*/
    
    default:
        break;
    }
	return;
}

/**************************************************************************************************/
/*
 * Brief: Mode switches with the NM.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
#if (STD_ON == COMM_NM_USED)
STATIC FUNC(void, COMM_CODE) ComM_NmModeProcessing(NetworkHandleType Channel)
{    
    switch(ComM_Channel[Channel].CurrentMode)
    {
    case COMM_SILENT_COMMUNICATION:
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_NETMODE))
        {
            ComM_EnterReadySleep(Channel);
        }
        else
        {
            if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_BUSSLEEP))
            {
                ComM_EnterNoCom(Channel);
            }
        }
        break;
        
    case COMM_FULL_COMMUNICATION:
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_PRESLEEP))
        {
            ComM_EnterSilentCom(Channel);
        }
        else
        {
            if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_BUSSLEEP))
            {
                ComM_EnterNoCom(Channel);
            }
        }
        break;
        
    default:
        break;
    }

	return;
}
#endif

/**************************************************************************************************/
/*
 * Brief: User "Full Com" request depending on the channel.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_RequestComMode()
 */
/**************************************************************************************************/
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)||(STD_ON == COMM_MODE_LIMITATION_ENABLED)) 
STATIC FUNC(void, COMM_CODE) ComM_UserRequestFullCom(NetworkHandleType Channel)
{
    switch(ComM_Channel[Channel].Status)
    {
    #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)    
    case COMM_STATUS_NO:
        if (COMM_IS_INHIBIT_WAKEUP(Channel) 
            && (ComM_Global.InhibitCounter < COMM_MAX_REJECT_COUNTER))
        {
            ComM_Global.InhibitCounter++;
        }
        break;
    #endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/
    
    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)     
    case COMM_STATUS_READY:
    case COMM_STATUS_SILENT:
        if (COMM_IS_LIMIT_TO_NOCOM(Channel)
            && (ComM_Global.InhibitCounter < COMM_MAX_REJECT_COUNTER))
        {
            ComM_Global.InhibitCounter++;
        }
        break;
    #endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED) */
    
    default:
        break;
    }

	return;
}
#endif

/**************************************************************************************************/
/*
 * Brief: Entered into the "No Com" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_EnterNoCom(NetworkHandleType Channel)
{
    ComM_Channel[Channel].Events = 0u;
    ComM_Channel[Channel].Status = COMM_STATUS_NO;
    ComM_Channel[Channel].Timeout = 0u;
    
    if (ComM_Channel[Channel].CurrentMode != COMM_NO_COMMUNICATION)
    {
        ComM_Channel[Channel].UserFullCounter = 0u;
        COMM_SET_BUS_MODE(Channel,COMM_NO_COMMUNICATION);
    }
    #if (STD_ON == COMM_ECUM_USED)
    else
    {
        (void)EcuM_ComM_ReleaseRUN(Channel);
    }
    #endif /*#if (STD_ON == COMM_ECUM_USED)*/

	return;
}

/**************************************************************************************************/
/*
 * Brief: Handle of the "No Com" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_NoComHandler(NetworkHandleType Channel)
{
	if (((ComM_Channel[Channel].UserFullCounter > 0u)
        #if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        &&(!COMM_IS_INHIBIT_WAKEUP(Channel))
        #endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        || (COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*#if (STD_ON == COMM_DCM_USED)*/
        #if ((STD_ON == COMM_NM_USED) || (STD_ON == COMM_ECUM_USED))
        || (COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP))
        #endif /*#if ((STD_ON == COMM_NM_USED) || (STD_ON == COMM_ECUM_USED))*/
        )
    {
        #if (STD_ON == COMM_ECUM_USED)
        ComM_EnterNoComPending(Channel);
        #else
        ComM_EnterNetworkRequested(Channel);
        #endif /*#if (STD_ON == COMM_ECUM_USED)*/
    }

    return;
}

/**************************************************************************************************/
/*
 * Brief: Entered into the "No Pending" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
#if (STD_ON == COMM_ECUM_USED)
STATIC FUNC(void, COMM_CODE) ComM_EnterNoComPending(NetworkHandleType Channel)
{
    
    if (TRUE == EcuM_ComM_HasRequestedRUN(Channel))
    {
        ComM_EnterNetworkRequested(Channel);
    }
    else
    {
        ComM_Channel[Channel].Status = COMM_STATUS_NOPENDING;
        (void)EcuM_ComM_RequestRUN(Channel);
    }

    return;
}

/**************************************************************************************************/
/*
 * Brief: Handle of the "No Pending" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_NoComPendingHandler(NetworkHandleType Channel)
{    
    if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_ECUM_RUNMODE))
    {
        COMM_EVENT_CLR(Channel, COMM_EVENT_ECUM_RUNMODE);
        ComM_EnterNetworkRequested(Channel);
    }
    else if ((!COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP)) 
        && ((ComM_Channel[Channel].UserFullCounter == 0u)
        #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
        ||((ComM_Channel[Channel].UserFullCounter >0u) && (COMM_IS_INHIBIT_WAKEUP(Channel)))
        #endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        && (!COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*#if (STD_ON == COMM_DCM_USED)*/
        )
    {
        ComM_EnterNoCom(Channel);
    }
    else
    {
        /* Do nothing. */
    }

    return;
}
#endif /*#if (STD_ON == COMM_ECUM_USED)*/

/**************************************************************************************************/
/*
 * Brief: Entered into the "NetworkRequested" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_EnterNetworkRequested(NetworkHandleType Channel)
{
    if (ComM_Channel[Channel].CurrentMode != COMM_FULL_COMMUNICATION)
    {
        COMM_SET_BUS_MODE(Channel,COMM_FULL_COMMUNICATION);
    }
    
    switch (ComM_ChannelConfig[Channel].ComMNmVariant)
    {
    #if (STD_ON == COMM_NM_USED)
    #if (STD_OFF == COMM_NM_PASSIVE_USED)
    case COMM_FULL:
        if ((ComM_Channel[Channel].UserFullCounter > 0u)
            #if (STD_ON == COMM_DCM_USED)
            ||(COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
            #endif /*#if (STD_ON == COMM_DCM_USED)*/
            )
        {
            (void)Nm_NetworkRequest(Channel);
        }
        else
        {
            if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP))
            {
                (void)Nm_PassiveStartUp(Channel);
            }
        }
        break;
    #else /*(STD_ON == COMM_NM_PASSIVE_USED)*/
    case COMM_PASSIVE:
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP))
        {
            (void)Nm_PassiveStartUp(Channel);
        }
        break;
    #endif /*#if (STD_OFF == COMM_NM_PASSIVE_USED)*/
    #endif /*#if (STD_ON == COMM_NM_USED)*/

    /*
    case COMM_LIGHT:
    case COMM_NONE:*/
    default:
        ComM_Channel[Channel].Timeout = COMM_T_MIN_FULL_COM_MODE_DURATION;
        break;
    }
    
    COMM_EVENT_CLR(Channel, COMM_EVENT_PASSIVE_STARTUP|COMM_EVENT_ECUM_RUNMODE);
    ComM_Channel[Channel].Status = COMM_STATUS_NETREQ;
    
    return;
}

/**************************************************************************************************/
/*
 * Brief: Handle of the "NetworkRequested" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_NetworkRequestedHandler(NetworkHandleType Channel)
{
    if (ComM_Channel[Channel].Timeout > 0u)
    {
        ComM_Channel[Channel].Timeout--;
    }
    if (((ComM_Channel[Channel].UserFullCounter == 0u)
        #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        ||((ComM_Channel[Channel].UserFullCounter >0u) && (COMM_IS_LIMIT_TO_NOCOM(Channel)))
        #endif /*#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        && (!COMM_EVENT_IS_SET(Channel,COMM_EVENT_DCM_ACTIVE))
        #endif /*#if (STD_ON == COMM_DCM_USED)*/
        && (ComM_Channel[Channel].Timeout == 0u)
        )
    {
        ComM_EnterReadySleep(Channel);
    }

    return;
}

/**************************************************************************************************/
/*
 * Brief: Entered into the "ReadySleep" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_EnterReadySleep(NetworkHandleType Channel)
{
    if (ComM_Channel[Channel].CurrentMode != COMM_FULL_COMMUNICATION)
    {
        COMM_SET_BUS_MODE(Channel,COMM_FULL_COMMUNICATION);
    }
    ComM_Channel[Channel].Status = COMM_STATUS_READY;
    switch (ComM_ChannelConfig[Channel].ComMNmVariant)
    {
    #if ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_USED))
    case COMM_FULL:
        (void)Nm_NetworkRelease(Channel);
        break;
    #endif /*#if ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_USED))*/
    
    case COMM_LIGHT:
        ComM_Channel[Channel].Timeout = ComM_ChannelConfig[Channel].ComMNmLightTimeout;
        break;
        
    case COMM_NONE:
        ComM_EnterNoCom(Channel);
        break;

    /*
    case COMM_PASSIVE:*/
    default:
        break;
    }
    
    return;
}

/**************************************************************************************************/
/*
 * Brief: Handle of the "ReadySleep" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_ReadySleepHandler(NetworkHandleType Channel)
{    
    if (((ComM_Channel[Channel].UserFullCounter > 0u) 
        #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        && (!COMM_IS_LIMIT_TO_NOCOM(Channel))
        #endif /*#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        ||(COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*#if (STD_ON == COMM_DCM_USED)*/
        )
    {
        ComM_EnterNetworkRequested(Channel);
    }
    else
    {
        if (ComM_Channel[Channel].Timeout > 0u)
        {
            ComM_Channel[Channel].Timeout--;
            if (ComM_Channel[Channel].Timeout == 0u)
            {
                ComM_EnterNoCom(Channel);
            }            
        }
    }
    
    return;
}

#if (STD_ON == COMM_NM_USED)
/**************************************************************************************************/
/*
 * Brief: Entered into the "Silent Com" status..
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: void (*ComM_ModeHandleType)(NetworkHandleType)
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_EnterSilentCom(NetworkHandleType Channel)
{
    COMM_SET_BUS_MODE(Channel,COMM_SILENT_COMMUNICATION);
    ComM_Channel[Channel].Status = COMM_STATUS_SILENT;
    
    return;
}

/**************************************************************************************************/
/*
 * Brief: Handle of the "Silent Com" status.
 * Param-Name[in]: Channel
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: ComM shall be initialized.
 * CallByAPI: ComM_MainFunction()
 */
/**************************************************************************************************/
STATIC FUNC(void, COMM_CODE) ComM_SilentComHandler(NetworkHandleType Channel)
{    
    if (((ComM_Channel[Channel].UserFullCounter > 0u)
        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
        && (!COMM_IS_LIMIT_TO_NOCOM(Channel))
        #endif /*#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        || (COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*#if (STD_ON == COMM_DCM_USED)*/
        )
    {
        ComM_EnterNetworkRequested(Channel);
    }

    return;
}
#endif /*#if (STD_ON == COMM_NM_USED)*/

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

