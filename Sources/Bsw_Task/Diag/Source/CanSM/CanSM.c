/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM.c>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: Cygwin C Compiler    MCU:--->
 *
 *  @author     <zheng fang>
 *  @date       <2013-7-26>
 */
/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>         <AUTHOR>                    <REVISION LOG>
 *  V1.0.0      20130726        zheng fang                  Initial version
 *  V1.0.1      20140708        zheng fang                  add transceiver function
 */
/*============================================================================*/

/*=======[M I S R A C  R U L E  V I O L A T I O N]====================================*/

/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 19.7,
 * Msg(4:3453)  A function could probably be used instead of this function-like marco.
 */

/* MISRA-C:2004 Rule 17.4,
 * Msg(4:0491) Array subscripting applied to an object of pointer type.
 */

/* MISRA-C:2004 Rule 17.4,
 * Msg(4:0489) The integer value 1 is being added or subtracted from a pointer.
 */

/* MISRA-C:2004 Rule 14.1,
 * Msg(4:1503) The function is defined but is not used within this project.
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_C_AR_MAJOR_VERSION    1U
#define CANSM_C_AR_MINOR_VERSION    2U
#define CANSM_C_AR_PATCH_VERSION    0U
#define CANSM_C_SW_MAJOR_VERSION    1U
#define CANSM_C_SW_MINOR_VERSION    0U
#define CANSM_C_SW_PATCH_VERSION    1U

/* @req CANSM007 @req CANSM016 */
/*=======[I N C L U D E S]====================================================*/
/* @req CANSM013 */
#include "CanSM.h"
/* @req CANSM174 */
#include "ComM.h"
#if (STD_ON == CANSM_USE_COM)
/* @req CANSM172 */
#include "Com.h"
#endif
/* @req CANSM191 */
#include "ComM_BusSM.h"
#if (STD_ON == CANSM_CONFIGURATION_USE_DEM)
/* @req CANSM014 */
#include "Dem.h"
#endif
/* @req CANSM015 */
#if (STD_ON == CANSM_CONFIGURATION_USE_DET)
#include "Det.h"
#endif
/* @req CANSM017 */
#include "CanIfDiag.h"
#include "CanSM_ComM.h"
#include "CanSM_SchM.h"
#include "CanSM_Cbk.h"

#if (STD_ON == CANSM_USE_OSEKNM)
#include "OsekNm.h"
#include "OsekNm_Cbk.h"
#endif

#if ((CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD) \
     || (CANSM_VARIANT_CLASS == CANSM_VARIANT_LINK_TIME))
#include "CanSM_LCfg.h"
#endif

#if (CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD)
#include "CanSM_PBCfg.h"
#endif
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (CANSM_C_AR_MAJOR_VERSION != CANSM_H_AR_MAJOR_VERSION)
    #error "CanSM.c : Mismatch in Specification Major Version"
#endif
#if (CANSM_C_AR_MINOR_VERSION != CANSM_H_AR_MINOR_VERSION)
    #error "CanSM.c : Mismatch in Specification Major Version"
#endif
#if (CANSM_C_AR_PATCH_VERSION != CANSM_H_AR_PATCH_VERSION)
    #error "CanSM.c : Mismatch in Specification Major Version"
#endif
#if (CANSM_C_SW_MAJOR_VERSION != CANSM_H_SW_MAJOR_VERSION)
    #error "CanSM.c : Mismatch in Specification Major Version"
#endif
#if (CANSM_C_SW_MINOR_VERSION != CANSM_H_SW_MINOR_VERSION)
    #error "CanSM.c : Mismatch in Specification Major Version"
#endif

#if (2U != COMM_H_AR_MAJOR_VERSION)
    #error "ComM.h : Mismatch in Specification Major Version"
#endif
#if (1U != COMM_H_AR_MINOR_VERSION)
    #error "ComM.h : Mismatch in Specification Major Version"
#endif

#if (STD_ON == CANSM_USE_COM)
#if (3U != COM_H_AR_MAJOR_VERSION)
    #error "Com.h : Mismatch in Specification Major Version"
#endif
#if (2U != COM_H_AR_MINOR_VERSION)
    #error "Com.h : Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CANSM_USE_COM */

#if (2U != COMM_BUSSM_H_AR_MAJOR_VERSION)
    #error "ComM_BusSM.h : Mismatch in Specification Major Version"
#endif
#if (1U != COMM_BUSSM_H_AR_MINOR_VERSION)
    #error "ComM_BusSM.h : Mismatch in Specification Major Version"
#endif

#if (STD_ON == CANSM_CONFIGURATION_USE_DEM)
#if (3U != DEM_H_AR_MAJOR_VERSION)
    #error "Dem.h : Mismatch in Specification Major Version"
#endif
#if (1U != DEM_H_AR_MINOR_VERSION)
    #error "Dem.h : Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CANSM_CONFIGURATION_USE_DEM */

#if (STD_ON == CANSM_CONFIGURATION_USE_DET)
#if (2U != DET_H_AR_MAJOR_VERSION)
    #error "Det.h : Mismatch in Specification Major Version"
#endif
#if (2U != DET_H_AR_MINOR_VERSION)
    #error "Det.h : Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

#if (3U != CANIF_H_AR_MAJOR_VERSION)
    #error "CanIf.h : Mismatch in Specification Major Version"
#endif
#if (2U != CANIF_H_AR_MINOR_VERSION)
    #error "CanIf.h : Mismatch in Specification Major Version"
#endif

#if (1U != CANSM_COMM_H_AR_MAJOR_VERSION)
    #error "CanSM_ComM.h : Mismatch in Specification Major Version"
#endif
#if (2U != CANSM_COMM_H_AR_MINOR_VERSION)
    #error "CanSM_ComM.h : Mismatch in Specification Major Version"
#endif

#if (1U != CANSM_SCHM_H_AR_MAJOR_VERSION)
    #error "CanSM_SchM.h : Mismatch in Specification Major Version"
#endif
#if (2U != CANSM_SCHM_H_AR_MINOR_VERSION)
    #error "CanSM_SchM.h : Mismatch in Specification Major Version"
#endif

#if (1U != CANSM_CFG_H_AR_MAJOR_VERSION)
    #error "CanSM_Cfg.h : Mismatch in Specification Major Version"
#endif
#if (2U != CANSM_CFG_H_AR_MINOR_VERSION)
    #error "CanSM_Cfg.h : Mismatch in Specification Major Version"
#endif

#if (1U != CANSM_CBK_H_AR_MAJOR_VERSION)
    #error "CanSM_Cbk.h : Mismatch in Specification Major Version"
#endif
#if (2U != CANSM_CBK_H_AR_MINOR_VERSION)
    #error "CanSM_Cbk.h : Mismatch in Specification Major Version"
#endif

#if (STD_ON == CANSM_USE_OSEKNM)
#if (2U != OSEKNM_H_AR_MAJOR_VERSION)
    #error "OsekNm.h : Mismatch in Specification Major Version"
#endif
#if (5U != OSEKNM_H_AR_MINOR_VERSION)
    #error "OsekNm.h : Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CANSM_USE_OSEKNM */

#if ((CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD) \
     || (CANSM_VARIANT_CLASS == CANSM_VARIANT_LINK_TIME))
#if (CANSM_LCFG_H_AR_MAJOR_VERSION != CANSM_H_AR_MAJOR_VERSION)
    #error "CanSM_Lcfg.h : Mismatch in Specification Major Version"
#endif
#if (CANSM_LCFG_H_AR_MINOR_VERSION != CANSM_H_AR_MINOR_VERSION)
    #error "CanSM_Lcfg.h : Mismatch in Specification Major Version"
#endif
#endif /* (CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD) */

#if (CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD)
#if (CANSM_PBCFG_H_AR_MAJOR_VERSION != CANSM_H_AR_MAJOR_VERSION)
    #error "CanSM_PBCfg.h : Mismatch in Specification Major Version"
#endif
#if (CANSM_PBCFG_H_AR_MINOR_VERSION != CANSM_H_AR_MINOR_VERSION)
    #error "CanSM_PBCfg.h : Mismatch in Specification Major Version"
#endif
#endif /* CANSM_VARIANT_CLASS == CANSM_VARIANT_POST_BUILD */

/*=======[M A C R O S]========================================================*/
#define COUNTER_UP_LIMIT 255u

#if (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS)
/* get controller count of assigned network */
#define CANSM_CONTROLLER_COUNT(NetworkHandle_Index)     CanSM_ConfigPtr->CanSMControllerPtr[NetworkHandle_Index].CanSMControllerCnt

/* get controller header pointer of assigned network */
#define CANSM_CONTROLLER_REF(NetworkHandle_Index)       CanSM_ConfigPtr->CanSMControllerPtr[NetworkHandle_Index].CanSMControllerPtr

/* get network handle of assigned configuration index */
#define CANSM_NETWORK_HANDLE(NetworkHandle_Index)       CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMNetworkHandle

/* get bus-off recovery time tx ensured of assigned configuration index */
#define CANSM_BOR_TIME_TX_ENSURED(NetworkHandle_Index)  CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMBorTimeTxEnsured

/* get bus-off recovery time L1 of assigned configuration index */
#define CANSM_BOR_TIME_L1(NetworkHandle_Index)          CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMBorTimeL1

/* get bus-off recovery time L2 of assigned configuration index*/
#define CANSM_BOR_TIME_L2(NetworkHandle_Index)          CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMBorTimeL2

/* get bus-off recovery counter L1 to L2 of assigned configuration index */
#define CANSM_BOR_COUNTER_L1_TO_L2(NetworkHandle_Index) CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMBorCounterL1ToL2

/* get bus-off recovery counter L2 error of assigned configuration index */
#define CANSM_BOR_COUNTER_L2_ERR(NetworkHandle_Index)   CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMBorCounterL2Err

#if (STD_ON == CANSM_USE_OSEKNM)
/* get notify NM module flag of assigned configuration index */
#define CANSM_NOTIFYNM(NetworkHandle_Index)             CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMNotifyNM
#endif /* STD_ON == CANSM_USE_OSEKNM */

#if (STD_ON == CANSM_USE_TRANSCEIVER)
/* get transceiver Id of assigned configuration index */
#define CANSM_TRANSCEIVER(NetworkHandle_Index)             CanSM_ConfigPtr->CanSMNetworkPtr[NetworkHandle_Index].CanSMTransceiverId
#endif /* STD_ON == CANSM_USE_TRANSCEIVER */

#else /* CANSM_VARIANT_POST_BUILD != CANSM_VARIANT_CLASS */

/* get controller count of assigned network */
#define CANSM_CONTROLLER_COUNT(NetworkHandle_Index)     CanSM_Controller[NetworkHandle_Index].CanSMControllerCnt

/* get controller header pointer of assigned network */
#define CANSM_CONTROLLER_REF(NetworkHandle_Index)       CanSM_Controller[NetworkHandle_Index].CanSMControllerPtr

/* get network handle of assigned configuration index */
#define CANSM_NETWORK_HANDLE(NetworkHandle_Index)       CanSM_Network[NetworkHandle_Index].CanSMNetworkHandle

/* get bus-off recovery time tx ensured of assigned configuration index */
#define CANSM_BOR_TIME_TX_ENSURED(NetworkHandle_Index)  CanSM_Network[NetworkHandle_Index].CanSMBorTimeTxEnsured

/* get bus-off recovery counter L1 to L2 of assigned configuration index */
#define CANSM_BOR_COUNTER_L1_TO_L2(NetworkHandle_Index) CanSM_Network[NetworkHandle_Index].CanSMBorCounterL1ToL2

/* get bus-off recovery counter L2 error of assigned configuration index */
#define CANSM_BOR_COUNTER_L2_ERR(NetworkHandle_Index)   CanSM_Network[NetworkHandle_Index].CanSMBorCounterL2Err

/* get bus-off recovery time L1 of assigned configuration index */
#define CANSM_BOR_TIME_L1(NetworkHandle_Index)          CanSM_Network[NetworkHandle_Index].CanSMBorTimeL1

/* get bus-off recovery time L2 of assigned configuration index */
#define CANSM_BOR_TIME_L2(NetworkHandle_Index)          CanSM_Network[NetworkHandle_Index].CanSMBorTimeL2

#if (STD_ON == CANSM_USE_OSEKNM)
/* get notify NM module flag of assigned configuration index*/
#define CANSM_NOTIFYNM(NetworkHandle_Index)             CanSM_Network[NetworkHandle_Index].CanSMNotifyNM
#endif /* STD_ON == CANSM_USE_OSEKNM */

#if (STD_ON == CANSM_USE_TRANSCEIVER)
/* get transceiver Id of assigned configuration index */
#define CANSM_TRANSCEIVER(NetworkHandle_Index)          CanSM_Network[NetworkHandle_Index].CanSMTransceiverId
#endif /* STD_ON == CANSM_USE_TRANSCEIVER */

#endif /* CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS */

/*=======[I N T E R N A L   D A T A]==========================================*/
/* configuration data pointer */
#if (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS)
#define CANSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "CanSM_MemMap.h"
STATIC P2CONST(CanSM_ConfigType, CANSM_VAR_POWER_ON_INIT, CANSM_CONST_PBCFG)CanSM_ConfigPtr = NULL_PTR;
#define CANSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "CanSM_MemMap.h"
#endif /* CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS */

/* module status */
#if (STD_ON == CANSM_CONFIGURATION_USE_DET)
#define CANSM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "CanSM_MemMap.h"
STATIC VAR(boolean, CANSM_VAR_POWER_ON_INIT)CanSM_Status = FALSE;
#define CANSM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "CanSM_MemMap.h"
#endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

/* module run time structure array */
#define CANSM_START_SEC_VAR_UNSPECIFIED
#include "CanSM_MemMap.h"
STATIC VAR(CanSM_RunTimeType, CANSM_VAR)CanSM_RunTime[CANSM_NETWORK_NUM];
#define CANSM_STOP_SEC_VAR_UNSPECIFIED
#include "CanSM_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_NoComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SilentComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_FullComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR), CANSM_CODE)
CanSM_GetChannelByHandle
(
    NetworkHandleType NetworkHandle
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR), CANSM_CODE)
CanSM_GetChannelByController
(
    uint8 Controller
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#if(CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON)
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckTxConfirmation
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckTimer
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckInitEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorNoBusOffEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorTxOffL1Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL1Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL1BusOff
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorTxOffL2Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL2Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BusError
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_SetPduMode
(
    P2CONST(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr,
    CanIf_ChannelSetModeType PduModeRequest
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetCtrMode
(
    P2CONST(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr,
    CanIf_ControllerModeType Mode
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/******************************************************************************/
/*
 * @brief        :       This service initializes the CanSM module.
 * Service ID    :       <SERVICE_ID_CANSM_INIT>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Non Reentrant>
 * Param-Name[in]:       configPtr:Pointer to the CANSM configuration data.
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       <None>
 * CallByAPI     :       ECUM
 */
/******************************************************************************/
#if (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS)

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(void, CANSM_CODE)
CanSM_Init
(
    P2CONST(CanSM_ConfigType, CANSM_VAR, CANSM_CONST_PBCFG)ConfigPtr
)
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#else /* CANSM_VARIANT_POST_BUILD != CANSM_VARIANT_CLASS */

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(void, COM_CODE)
CanSM_Init(void)
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /* CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS */
{
    uint8 netLoop = 0u;
    Std_ReturnType ret = E_OK;

    #if ((STD_ON == CANSM_CONFIGURATION_USE_DET) \
         && (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS))
    /* check whether configuration pointer is null,if null, report det error */
    /* @req CANSM028 */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(CANSM_MODULE_ID,
                        CANSM_INSTANCE_ID,
                        SERVICE_ID_CANSM_INIT,
                        CANSM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET) */

    /* configuration pointer is not null */
    if (E_OK == ret)
    {
        #if (CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS)
        /* save global configuration parameter pointer */
        CanSM_ConfigPtr = ConfigPtr;
        #endif /* CANSM_VARIANT_POST_BUILD == CANSM_VARIANT_CLASS */
        /* initialize run time structure variable for every network */
        for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop ++)
        {
            /* @req CANSM211 @req CANSM217 */
            CanSM_RunTime[netLoop].curMode = COMM_NO_COMMUNICATION;
            CanSM_RunTime[netLoop].pBusOffFnc = NULL_PTR;
            CanSM_RunTime[netLoop].pBORTimeoutFnc = NULL_PTR;
            CanSM_RunTime[netLoop].pBORMainFnc = NULL_PTR;
            CanSM_RunTime[netLoop].counter = 0u;
            CanSM_RunTime[netLoop].timer = 0u;
            /* @req CANSM045-1 */
            CanSM_RunTime[netLoop].borState = CANSM_BOR_IDLE;
            CanSM_RunTime[netLoop].busOffEvent = FALSE;
            CanSM_RunTime[netLoop].netId = netLoop;

			#if (STD_ON == CANSM_USE_TRANSCEIVER)
			/* set transceiver mode of the network */
			ret = CanIf_SetTransceiverMode(CANSM_TRANSCEIVER(netLoop), CANIF_TRCV_MODE_STANDBY);
			
			if (E_OK == ret)
			{
			#endif /* end of (STD_ON == CANSM_USE_TRANSCEIVER) */
				/* set controller mode of the network */
				/* @req CANSM039 @req CANSM044 */
				(void)CanSM_SetCtrMode(&CanSM_RunTime[netLoop], CANIF_CS_SLEEP);
			#if (STD_ON == CANSM_USE_TRANSCEIVER)	
			}
			#endif /* end of (STD_ON == CANSM_USE_TRANSCEIVER) */
        }
        #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
        /* change module state */
        CanSM_Status = TRUE;
        #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/* @req CANSM240 @req CANSM241 @req CANSM062 @req CANSM181 @req CANSM045-3 @req CANSM045-4
 * @req CANSM045-5 @req CANSM045-6 @req CANSM045-7 @req CANSM045-8 @req CANSM045-9 @req CANSM045-10
 * @req CANSM045-11 @req CANSM045-12 @req CANSM045-13 @req CANSM045-14 @req CANSM045-15 @req CANSM045-16
 * @req CANSM045-17 @req CANSM045-18 @req CANSM045-19 @req CANSM045-20 @req CANSM045-21 @req CANSM045-22
 * @req CANSM045-23 @req CANSM045-24 */
/******************************************************************************/
/*
 * @brief        :       This service shall change the communication mode of a CAN
 *                       network to the requested one.
 * Service ID    :       <SERVICE_ID_CANSM_REQUESTCOMMODE>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       NetworkHandle: Handle of destinated communication network for request
 *                       ComM_Mode: Requested communication mode
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       E_OK: Service accepted
 *                       E_NOT_OK: Service denied
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       ComM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_RequestComMode
(
    NetworkHandleType NetworkHandle,
    ComM_ModeType ComM_Mode
)
{
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;
    Std_ReturnType ret = E_OK;

    /* @req CANSM070 */
    #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
    /* check whether module state is uninit,if uninit, report det error */
    /* @req CANSM184 */
    if (FALSE == CanSM_Status)
    {
        Det_ReportError(CANSM_MODULE_ID,
                        CANSM_INSTANCE_ID,
                        SERVICE_ID_CANSM_REQUESTCOMMODE,
                        CANSM_E_UNINIT);
        ret = E_NOT_OK;
    }

    if (E_OK == ret)
    {
        /* check whether requested communication mode is invalid,if invalid, report det error */
        if ((COMM_NO_COMMUNICATION != ComM_Mode)
            && (COMM_SILENT_COMMUNICATION != ComM_Mode)
            && (COMM_FULL_COMMUNICATION != ComM_Mode))
        {
            Det_ReportError(CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_REQUESTCOMMODE,
                            CANSM_E_INVALID_NETWORK_MODE);
            ret = E_NOT_OK;
        }
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

    if (E_OK == ret)
    {
        /* get run time structure pointer depending on network handle */
        networkPtr = CanSM_GetChannelByHandle(NetworkHandle);

        /* Network Handle is valid */
        if (NULL_PTR != networkPtr)
        {
            /* @req CANSM032 @req CANSM032 @req CANSM182 */
            switch (networkPtr->curMode)
            {
                /* current communication state is NO */
                case COMM_NO_COMMUNICATION:
                    /* request full communication state */
                    /* @req CANSM212 @req CANSM219 */
                    if (COMM_FULL_COMMUNICATION == ComM_Mode)
                    {
                        /* call CanSM_FullComEnter function, and change to full communication state */
                        ret = CanSM_FullComEnter(networkPtr);
                    }
                    else
                    {
                        /* request silent communication state */
                        if (COMM_SILENT_COMMUNICATION == ComM_Mode)
                        {
                            ret = E_NOT_OK;
                        }
                    }
                    break;

                /* current communication state is SILENT */
                case COMM_SILENT_COMMUNICATION:
                    /* request full communication state */
                    /* @req CANSM215 @req CANSM231 */
                    if (COMM_FULL_COMMUNICATION == ComM_Mode)
                    {
                        /* call CanSM_FullComEnter function, and change to full communication state */
                        ret = CanSM_FullComEnter(networkPtr);
                    }
                    else
                    {
                        /* request NO communication state */
                        /* @req CANSM214 @req CANSM218 */
                        if (COMM_NO_COMMUNICATION == ComM_Mode)
                        {
                            /* call CanSM_NoComEnter function, and change to NO communication state */
                            ret = CanSM_NoComEnter(networkPtr);
                        }
                    }
                    break;

                /* current communication state is SILENT */
                case COMM_FULL_COMMUNICATION:
                    /* request SILENT communication state */
                    /* @req CANSM216 @req CANSM232 */
                    if (COMM_SILENT_COMMUNICATION == ComM_Mode)
                    {
                        /* call CanSM_SilentComEnter function, and change to SILENT communication state */
                        ret = CanSM_SilentComEnter(networkPtr);
                    }
                    /* request FULL communication state */
                    else if (COMM_FULL_COMMUNICATION == ComM_Mode)
                    {
                       /* Don't need to do anything */
                    }
                    else /* request other communication state except SILENT and FULL state */
                    {
                        /* call CanSM_BusError function */
                        CanSM_BusError(networkPtr);
                        ret = E_NOT_OK;
                    }
                    break;

                default:
                    break;
            }
        }
        #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
        /* @req CANSM183 */
        else   /* Network Handle is invalid,report det */
        {
            Det_ReportError(CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_REQUESTCOMMODE,
                            CANSM_E_INVALID_NETWORK_HANDLE);
            ret = E_NOT_OK;
        }
        #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */
    }
    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/* @req CANSM063 */
/******************************************************************************/
/*
 * @brief        :       This service shall put out the current communication mode
 *                       of a CAN network.
 * Service ID    :       <SERVICE_ID_CANSM_GETCURRENTCOMMODE>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       NetworkHandle: whose current communication mode shall be put out
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       E_OK: Service accepted
 *                       E_NOT_OK: Service denied
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       ComM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_GetCurrentComMode
(
    NetworkHandleType NetworkHandle,
    P2VAR(ComM_ModeType, AUTOMATIC, CANSM_APPL_DATA) ComM_ModePtr
)
{
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;
    Std_ReturnType ret = E_OK;

    #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
    /* check whether module state is uninit,if uninit, report det error */
    /* @req CANSM188 */
    if (FALSE == CanSM_Status)
    {
        Det_ReportError(CANSM_MODULE_ID,
                        CANSM_INSTANCE_ID,
                        SERVICE_ID_CANSM_GETCURRENTCOMMODE,
                        CANSM_E_UNINIT);
        ret = E_NOT_OK;
    }

    if (E_OK == ret)
    {
        /* check whether ComM_ModePtr parameter is invalid, if invalid, report det error */
        if (NULL_PTR == ComM_ModePtr)
        {
            Det_ReportError(CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_GETCURRENTCOMMODE,
                            CANSM_E_PARAM_POINTER);
            ret = E_NOT_OK;
        }
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

    if (E_OK == ret)
    {
        /* get run time structure pointer depending on network handle */
        /* @req CANSM185 */
        networkPtr = CanSM_GetChannelByHandle(NetworkHandle);

        /* Network Handle is valid */
        if (NULL_PTR != networkPtr)
        {
            /* get current communication mode */
            /* @req CANSM186 */
            *ComM_ModePtr = networkPtr->curMode;
        }
        #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
        /* @req CANSM187 */
        else /* Network Handle is invalid,report det */
        {
            Det_ReportError(CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_GETCURRENTCOMMODE,
                            CANSM_E_INVALID_NETWORK_HANDLE);
            ret = E_NOT_OK;
        }
        #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */
    }
    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/* @req CANSM065 */
/******************************************************************************/
/*
 * @brief        :       Scheduled function of the CanSM.
 * Service ID    :       <SERVICE_ID_CANSM_MAINFUNCTION>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       <None>
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       SchM
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(void, COM_CODE)
CanSM_MainFunction(void)
{
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;
    Std_ReturnType ret = E_OK;
    uint8 loop = 0u;

    /* check whether module state is uninit,if uninit, report det error */
    #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
    if (FALSE == CanSM_Status)
    {
        Det_ReportError(CANSM_MODULE_ID,
                        CANSM_INSTANCE_ID,
                        SERVICE_ID_CANSM_MAINFUNCTION,
                        CANSM_E_UNINIT);
        ret = E_NOT_OK;
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

    if (E_OK == ret)
    {
        for (loop = 0u; loop < CANSM_NETWORK_NUM; loop ++)
        {
            /* get run time structure pointer */
            networkPtr = &CanSM_RunTime[loop];

            switch (networkPtr->borState)
            {
                case CANSM_BOR_TXOFF_L1:
                case CANSM_BOR_TXOFF_L2:
                   if (NULL_PTR != networkPtr->pBORMainFnc)
                   {
                       networkPtr->pBORMainFnc(networkPtr);
                   }

                   break;

                case CANSM_BOR_CHECK_INIT:
                case CANSM_BOR_NO_BUS_OFF:
                case CANSM_BOR_CHECK_L1:
                case CANSM_BOR_CHECK_L2:
                       /* existence bus-off event and bus-off function is not null */
                       if (TRUE == networkPtr->busOffEvent)
                       {
                           if (NULL_PTR != networkPtr->pBusOffFnc)
                           {
                               /* notify NM about bus-off event */
                               #if (STD_ON == CANSM_USE_OSEKNM)
                               if (TRUE == CANSM_NOTIFYNM(networkPtr->netId))
                               {
                                   OsekNm_BusOff(CANSM_NETWORK_HANDLE(networkPtr->netId));
                               }
                               #endif
                               /* execute bus-off function */
                               /* @req CANSM167 */
                               networkPtr->pBusOffFnc(networkPtr);
                               networkPtr->busOffEvent = FALSE;
                           }
                       }
                       else
                       {
                           /* execute bus-off recovery periodic function */
                           /* @req CANSM221 */
                           if (NULL_PTR != networkPtr->pBORMainFnc)
                           {
                               networkPtr->pBORMainFnc(networkPtr);
                           }
                       }
                       break;

                default:
                    break;
            }
        }
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/* @req CANSM064 */
/******************************************************************************/
/*
 * @brief        :       The CanSM is notified about a bus-off event on a certain
 *                       CAN controller with this call-out function. It shall execute
 *                       the bus-off recovery state machine for the corresponding
 *                       network handle.
 * Service ID    :       <SERVICE_ID_CANSM_CONTROLLERBUSOFF>
 * Sync/Async    :       <Synchronous>
 * Reentrancy    :       <Reentrant>
 * Param-Name[in]:       controller: which detected a bus-off event
 * Param-Name[out]:      <None>
 * Param-Name[in/out]:   <None>
 * Return        :       <None>
 * PreCondition  :       CanSM Module has been initialized.
 * CallByAPI     :       CanIf
 */
/******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
FUNC(void, CANSM_CODE)
CanSM_ControllerBusOff
(
    uint8 Controller
)
{
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;
    Std_ReturnType ret = E_OK;

    /* check whether module state is uninit,if uninit, report det error */
    #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
    /* @req CANSM190 */
    if (FALSE == CanSM_Status)
    {
        Det_ReportError(CANSM_MODULE_ID,
                        CANSM_INSTANCE_ID,
                        SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                        CANSM_E_UNINIT);
        ret = E_NOT_OK;
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */

    if (E_OK == ret)
    {
        /* get run time structure pointer depending on Controller */
        /* @req CANSM048 */
        networkPtr = CanSM_GetChannelByController(Controller);

        if (NULL_PTR != networkPtr)
        {
            if (COMM_FULL_COMMUNICATION == networkPtr->curMode)
            {
                /* set bus-off event flag */
                /* @req CANSM235 */
                networkPtr->busOffEvent = TRUE;
            }
        }
        #if (STD_ON == CANSM_CONFIGURATION_USE_DET)
        /* @req CANSM189 */
        else /* Controller is invalid,report det */
        {
            Det_ReportError(CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                            CANSM_E_PARAM_CONTROLLER);
            ret = E_NOT_OK;
        }
        #endif /* STD_ON == CANSM_CONFIGURATION_USE_DET */
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               get network runtime structure pointer depending on input NetworkHandle
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle:  input network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_RequestComMode,CanSM_GetCurrentComMode
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR), CANSM_CODE)
CanSM_GetChannelByHandle
(
    NetworkHandleType NetworkHandle
)
{
    uint8 loop = 0u;
    Std_ReturnType ret = E_NOT_OK;
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;

    /*
     * loop for every network, search corresponding network
     * runtime structure pointer according network handle
     */
    for (loop = 0u; (loop < CANSM_NETWORK_NUM) && (E_NOT_OK == ret); loop ++)
    {
        if (CANSM_NETWORK_HANDLE(loop) == NetworkHandle)
        {
            networkPtr = &CanSM_RunTime[loop];
            ret = E_OK;
        }
    }
    return networkPtr;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               get network runtime structure pointer depending on input controller
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: input network controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_ControllerBusOff
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR), CANSM_CODE)
CanSM_GetChannelByController
(
    uint8 Controller
)
{
    uint8 loop = 0u;
    uint8 networkLoop = 0u;
    P2CONST(uint8, AUTOMATIC, CANSM_CONST_PBCFG)ctr_ptr = NULL_PTR;
    Std_ReturnType ret = E_NOT_OK;
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR)networkPtr = NULL_PTR;

    /*
     * loop for every network, search corresponding network
     * runtime structure pointer according controller
     */
    for (networkLoop = 0u; (networkLoop < CANSM_NETWORK_NUM) && (E_NOT_OK == ret); networkLoop ++)
    {
        ctr_ptr = CANSM_CONTROLLER_REF(networkLoop);

        for (loop = 0u; ((loop < CANSM_CONTROLLER_COUNT(networkLoop)) && (E_NOT_OK == ret)); loop ++)
        {
            if (*ctr_ptr == Controller)
            {
                networkPtr = &CanSM_RunTime[networkLoop];
                ret = E_OK;
            }
            ctr_ptr ++;
        }
    }
    return networkPtr;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               network state machine handling when changing to
 *                     NoCom state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_RequestComMode
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_NoComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    Std_ReturnType ret = E_OK;

    /* stop rx pdu group of the network */
    #if (STD_ON == CANSM_USE_COM)
    Com_IpduGroupStop(CanSM_RxPduGroupId[networkPtr->netId]);
    #endif /* STD_ON == CANSM_USE_COM */
    /* set controller mode of the network as "SLEEP" */
    /* @req CANSM039 @req CANSM044 */
    ret = CanSM_SetCtrMode(networkPtr, CANIF_CS_SLEEP);
	
	#if (STD_ON == CANSM_USE_TRANSCEIVER)
	if (E_OK == ret)
	{
		/* set transceiver mode of the network */
		ret = CanIf_SetTransceiverMode(CANSM_TRANSCEIVER(networkPtr->netId), CANIF_TRCV_MODE_STANDBY);
	}
	else
	{
		/* set transceiver mode of the network */
		(void)CanIf_SetTransceiverMode(CANSM_TRANSCEIVER(networkPtr->netId), CANIF_TRCV_MODE_STANDBY);
	}
    #endif /* end of (STD_ON == CANSM_USE_TRANSCEIVER) */
	
	/* change current state to "NO" */
	networkPtr->curMode = COMM_NO_COMMUNICATION;

	/* clear all function pointer */
	networkPtr->pBusOffFnc = NULL_PTR;
	networkPtr->pBORTimeoutFnc = NULL_PTR;
	networkPtr->pBORMainFnc = NULL_PTR;

	/* mode indication to ComM module */
	/* @req CANSM089 */
	ComM_BusSM_ModeIndication(CANSM_NETWORK_HANDLE(networkPtr->netId),
							  (ComM_ModeType*)&(networkPtr->curMode));

    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               network state machine handling when changing to
 *                     SilentCom state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_RequestComMode
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SilentComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    /* stop rx pdu group of the network */
    #if (STD_ON == CANSM_USE_COM)
    Com_IpduGroupStop(CanSM_TxPduGroupId[networkPtr->netId]);
    #endif /* STD_ON == CANSM_USE_COM */
    /* set controller mode of the network as "TX_OFFLINE" */
    /* @req CANSM083 @req CANSM209 @req CANSM209 */
    CanSM_SetPduMode(networkPtr, CANIF_SET_TX_OFFLINE);

    #if (STD_ON == CANSM_USE_COM)
    #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING)
    /* enable reception deadline monitor of assigned rx pdu group */
    /* @req CANSM208 @req CANSM230 @req CANSM045-2 */
    Com_EnableReceptionDM(CanSM_RxPduGroupId[networkPtr->netId]);
    #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */
    #endif /* STD_ON == CANSM_USE_COM */

    /* change current bus-off state to "CANSM_BOR_IDLE" */
    networkPtr->borState = CANSM_BOR_IDLE;

    /* clear all function pointer */
    networkPtr->pBusOffFnc = NULL_PTR;
    networkPtr->pBORTimeoutFnc = NULL_PTR;
    networkPtr->pBORMainFnc = NULL_PTR;
    /* change current state to "SILENT" */
    networkPtr->curMode = COMM_SILENT_COMMUNICATION;

    /* mode indication to ComM module */
    /* @req CANSM089 */
    ComM_BusSM_ModeIndication(CANSM_NETWORK_HANDLE(networkPtr->netId),
                              (ComM_ModeType*)&(networkPtr->curMode));
    return E_OK;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               network state machine handling when changing to
 *                     FullCom state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_RequestComMode
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_FullComEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    Std_ReturnType ret = E_OK;

    /* @req CANSM045 @req CANSM168 */
    switch (networkPtr->curMode)
    {
        case COMM_SILENT_COMMUNICATION:
            /* set controller mode of the network as "TX_ONLINE" */
            /* @req CANSM083 @req CANSM207 @req CANSM228 */
            CanSM_SetPduMode(networkPtr, CANIF_SET_TX_ONLINE);
            #if (STD_ON == CANSM_USE_COM)
            /* start tx pdu group of the network */
            /* @req CANSM173 */
            Com_IpduGroupStart(CanSM_TxPduGroupId[networkPtr->netId], FALSE);
            #endif /* STD_ON == CANSM_USE_COM */
            /* change current state to "FULL" */
            networkPtr->curMode = COMM_FULL_COMMUNICATION;

            /* mode indication to ComM module */
            /* @req CANSM089 */
            ComM_BusSM_ModeIndication(CANSM_NETWORK_HANDLE(networkPtr->netId),
                                      (ComM_ModeType*)&(networkPtr->curMode));

            /* execute bus-off recovery state,change to "check_init" */
            CanSM_BorCheckInitEnter(networkPtr);
            break;
        case COMM_NO_COMMUNICATION:
		    #if (STD_ON == CANSM_USE_TRANSCEIVER)  
		    ret = CanIf_SetTransceiverMode(CANSM_TRANSCEIVER(networkPtr->netId), CANIF_TRCV_MODE_NORMAL);
			
			if (E_OK == ret)
			{
			#endif /* end of (STD_ON == CANSM_USE_TRANSCEIVER) */
				/* set controller mode of the network as "START" */
				/* @req CANSM039 @req CANSM044 @req CANSM206 @req CANSM225 */
				ret = CanSM_SetCtrMode(networkPtr, CANIF_CS_STARTED);
				
				if (E_OK == ret)
				{
					#if (STD_ON == CANSM_USE_COM)
					/* start tx and rx pdu group of the network */
					/* @req CANSM173 */
					Com_IpduGroupStart(CanSM_RxPduGroupId[networkPtr->netId], CanSM_PCNetwork[networkPtr->netId].CanSMRxPduInit);
					Com_IpduGroupStart(CanSM_TxPduGroupId[networkPtr->netId], CanSM_PCNetwork[networkPtr->netId].CanSMTxPduInit);
					#endif /* STD_ON == CANSM_USE_COM */
					/* change current state to "FULL" */
					networkPtr->curMode = COMM_FULL_COMMUNICATION;
					/* mode indication to ComM module */
					/* @req CANSM089 */
					ComM_BusSM_ModeIndication(CANSM_NETWORK_HANDLE(networkPtr->netId),
											  &(networkPtr->curMode));
					/* execute bus-off recovery state,change to "check_init" */
					CanSM_BorCheckInitEnter(networkPtr);
				}
				else /* set controller mode unsuccessful, execute bus-error function */
				{
					CanSM_BusError(networkPtr);
				}
		    #if (STD_ON == CANSM_USE_TRANSCEIVER)
			}
            #endif /* end of (STD_ON == CANSM_USE_TRANSCEIVER) */
            break;

        default:
            break;
    }
    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#if(CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON)
/*************************************************************************/
/*
 * Brief               bus-off recovery machine Check Tx Confirmation handling
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_MainFunction
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckTxConfirmation
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    Std_ReturnType ret = E_OK;
    uint8 loop = 0u;
    P2CONST(uint8, AUTOMATIC, CANSM_VAR)ctrPtr = NULL_PTR;

    ctrPtr = CANSM_CONTROLLER_REF(networkPtr->netId);

    /* check if all controllers' tx confirmation state is "TX_RX_NOTIFICATION" */
    for (loop = 0u; (loop < CANSM_CONTROLLER_COUNT(networkPtr->netId)) && (E_OK == ret); loop ++)
    {
        if (CANIF_TX_RX_NOTIFICATION != CanIf_GetTxConfirmationState(*ctrPtr))
        {
            ret = E_NOT_OK;
        }
        ctrPtr ++;
    }

    if (E_OK == ret)
    {
       /* call corresponding bus-off recovery time out handle function */
       networkPtr->pBORTimeoutFnc(networkPtr);
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               bus-off recovery machine time handling
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_MainFunction
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckTimer
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    if(networkPtr->timer > 0u)
    {
        networkPtr->timer --;

        /* bus-off recovery time out,execute corresponding handle function */
        if ((networkPtr->timer == 0u) && (NULL_PTR != networkPtr->pBORTimeoutFnc))
        {
            networkPtr->pBORTimeoutFnc(networkPtr);
        }
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorTxOffL1 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_FullComEnter
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckInitEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    /* set bus-off recovery main function */
    /* @req CANSM340 */
    #if (CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON)
    /* @req CANSM341 */
    if (TRUE == CanSM_PCNetwork[networkPtr->netId].CanSMBorTxConfirmationPolling)
    {
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTxConfirmation;
    }
    else
    #endif /* CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON */
    {
        networkPtr->timer = CANSM_BOR_TIME_TX_ENSURED(networkPtr->netId);
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTimer;
    }
    /* set run time structure variable */
    networkPtr->counter = 0u;
    networkPtr->busOffEvent = FALSE;
    networkPtr->pBORTimeoutFnc = (CanSM_CbkFuncType)&CanSM_BorNoBusOffEnter;
    networkPtr->pBusOffFnc = (CanSM_CbkFuncType)&CanSM_BorTxOffL1Enter;
    /* change bus-off recovery state */
    networkPtr->borState = CANSM_BOR_CHECK_INIT;
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorNoBusOff state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_BorCheckTxConfirmation,CanSM_BorCheckTimer
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorNoBusOffEnter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING) && (STD_ON == CANSM_USE_COM)
    if((CANSM_BOR_CHECK_L1 == networkPtr->borState)
       || (CANSM_BOR_CHECK_L2 == networkPtr->borState))
    {
        /* enable reception deadline monitoring for the rx pdu group */
        /* @req CANSM208 @req CANSM228 */
        Com_EnableReceptionDM(CanSM_RxPduGroupId[networkPtr->netId]);
    }
    #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */

    /* report DEM about product error */
    /* @req CANSM072 @req CANSM223 */
    #if (STD_ON == CANSM_CONFIGURATION_USE_DEM)
    /* @req CANSM047 @req CANSM074 @req CANSM222 @req CANSM224 */
 //   Dem_ReportErrorStatus((CANSM_DEM_ERRID_0 + networkPtr->netId), DEM_EVENT_STATUS_PASSED);
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DEM */
    /* set run time structure variable */
    networkPtr->counter = 0u;
    networkPtr->pBORMainFnc = NULL_PTR;
    networkPtr->pBORTimeoutFnc = NULL_PTR;
    networkPtr->pBusOffFnc = (CanSM_CbkFuncType)&CanSM_BorTxOffL1Enter;
    /* change bus-off recovery state */
    networkPtr->borState = CANSM_BOR_NO_BUS_OFF;
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorTxOffL1 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_MainFunction
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorTxOffL1Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    /* @req CANSM225 */
    if (E_NOT_OK == CanSM_SetCtrMode(networkPtr, CANIF_CS_STARTED))
    {
        /* set controller mode unsuccessful, call bus error function */
        CanSM_BusError(networkPtr);
    }
    else
    {
        /* StartTimer */
        networkPtr->timer = CANSM_BOR_TIME_L1(networkPtr->netId);
        /* set pdu mode as "TX_OFFLINE" */
        /* @req CANSM083 @req CANSM209 @req CANSM227 */
        CanSM_SetPduMode(networkPtr, CANIF_SET_TX_OFFLINE);

        #if (STD_ON == CANSM_USE_COM)
        #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING)
        switch(networkPtr->borState)
        {
            case CANSM_BOR_CHECK_INIT:
            case CANSM_BOR_NO_BUS_OFF:
                /* Disable reception deadline monitoring for the rx pdu group */
                /* @req CANSM178 @req CANSM210 @req CANSM229 */
                Com_DisableReceptionDM(CanSM_RxPduGroupId[networkPtr->netId]);
                break;

            default:
                break;
        }
        #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */
        #endif /* STD_ON == CANSM_USE_COM */

        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTimer;
        networkPtr->pBORTimeoutFnc = (CanSM_CbkFuncType)&CanSM_BorCheckL1Enter;
        /* change bus-off recovery state */
        networkPtr->borState = CANSM_BOR_TXOFF_L1;
        networkPtr->pBusOffFnc = NULL_PTR;

        if (0u == CANSM_BOR_COUNTER_L1_TO_L2(networkPtr->netId)) /* fang zheng 2014-1-14 */
        {
            /* change to CANSM_BOR_TXOFF_L2 directly */
            CanSM_BorTxOffL2Enter(networkPtr);
        }
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorCheckL1 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_BorCheckTxConfirmation,CanSM_BorCheckTimer
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL1Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    /* set bus-off recovery main function */
    /* @req CANSM340 */
    #if (CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON)
    /* @req CANSM341 */
    if (TRUE == CanSM_PCNetwork[networkPtr->netId].CanSMBorTxConfirmationPolling)
    {
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTxConfirmation;
    }
    else
    #endif /* CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON */
    {
        networkPtr->timer = CANSM_BOR_TIME_TX_ENSURED(networkPtr->netId);
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTimer;
    }
    /* set bus-off recovery function and time out function */
    networkPtr->pBORTimeoutFnc = (CanSM_CbkFuncType)&CanSM_BorNoBusOffEnter;
    networkPtr->pBusOffFnc = (CanSM_CbkFuncType)&CanSM_BorCheckL1BusOff;

    /* increase counter */
    /* @req CANSM057 */
    if (COUNTER_UP_LIMIT > networkPtr->counter)
    {
        networkPtr->counter ++;
    }
    /* set pdu mode as "TX_ONLINE"*/
    /* @req CANSM083 @req CANSM207 @req CANSM228 */
    CanSM_SetPduMode(networkPtr, CANIF_SET_TX_ONLINE);

    #if (STD_ON == CANSM_USE_OSEKNM)
    if (TRUE == CANSM_NOTIFYNM(networkPtr->netId))
    {
        OsekNm_BusOffRecovery(networkPtr->netId);   /* add by fang zheng 2014-1-16 */
    }
    #endif
    /* change bus-off recovery state */
    networkPtr->borState = CANSM_BOR_CHECK_L1;
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when bus-off event
 *                     occurs at BorCheckL1 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_MainFunction
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL1BusOff
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    if(networkPtr->counter < CANSM_BOR_COUNTER_L1_TO_L2(networkPtr->netId))
    {
        /* change to txoff L1 state */
        CanSM_BorTxOffL1Enter(networkPtr);
    }
    else
    {
        /* change to txoff L2 state */
        CanSM_BorTxOffL2Enter(networkPtr);
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorTxOffL2 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_BorTxOffL2Enter,CanSM_MainFunction
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorTxOffL2Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    boolean Ctr_Setflag = TRUE;

    #if (STD_ON == CANSM_CONFIGURATION_USE_DEM)
    if (CANSM_BOR_COUNTER_L2_ERR(networkPtr->netId) <= networkPtr->counter)
    {
        /* @req CANSM047 @req CANSM074 @req CANSM226 */
 //       Dem_ReportErrorStatus((CANSM_DEM_ERRID_0 + networkPtr->netId), DEM_EVENT_STATUS_FAILED);
    }
    #endif /* STD_ON == CANSM_CONFIGURATION_USE_DEM */

    /* set controller mode as "START", if unsuccessful,call CanSM_BusError */
    /* @req CANSM225 */
    if (CANSM_BOR_TXOFF_L1 != networkPtr->borState)
    {
        if (E_NOT_OK == CanSM_SetCtrMode(networkPtr, CANIF_CS_STARTED))
        {
           CanSM_BusError(networkPtr);
           Ctr_Setflag = FALSE;
        }
        else
        {
            /* set pdu mode as "TX_OFFLINE" */
            /* @req CANSM083 @req CANSM209 @req CANSM2279 */
            CanSM_SetPduMode(networkPtr, CANIF_SET_TX_OFFLINE);
        }
    }

    if (TRUE == Ctr_Setflag)
    {
        networkPtr->timer = CANSM_BOR_TIME_L2(networkPtr->netId);

        /* set bus-off recovery function and time out function */
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTimer;
        networkPtr->pBORTimeoutFnc = (CanSM_CbkFuncType)&CanSM_BorCheckL2Enter;
        /* change bus-off recovery state */
        networkPtr->borState = CANSM_BOR_TXOFF_L2;
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus-off recovery machine handling when changing to
 *                     BorCheckL2 state
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_BorCheckTxConfirmation,CanSM_BorCheckTimer
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BorCheckL2Enter
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    /* set bus-off recovery main function */
    /* @req CANSM340 */
    #if (CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON)
    /* @req CANSM341 */
    if (TRUE == CanSM_PCNetwork[networkPtr->netId].CanSMBorTxConfirmationPolling)
    {
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTxConfirmation;
    }
    else
    #endif /* CANSM_BOR_TX_CONFIRMATION_POLLING == STD_ON */
    {
        networkPtr->timer = CANSM_BOR_TIME_TX_ENSURED(networkPtr->netId);
        networkPtr->pBORMainFnc = (CanSM_CbkFuncType)&CanSM_BorCheckTimer;
    }
    /* increase counter */
    /* @req CANSM057 */
    if (COUNTER_UP_LIMIT > networkPtr->counter)
    {
        networkPtr->counter ++;
    }
    /* set pdu mode as "TX_ONLINE" */
    /* @req CANSM083 @req CANSM207 @req CANSM228 */
    CanSM_SetPduMode(networkPtr, CANIF_SET_TX_ONLINE);

    #if (STD_ON == CANSM_USE_OSEKNM)
    if (TRUE == CANSM_NOTIFYNM(networkPtr->netId))
    {
        OsekNm_BusOffRecovery(networkPtr->netId);   /* add by fang zheng 2014-1-16 */
    }
    #endif
    /* set bus-off recovery function and time out function */
    networkPtr->pBusOffFnc = (CanSM_CbkFuncType)&CanSM_BorTxOffL2Enter;
    networkPtr->pBORTimeoutFnc = (CanSM_CbkFuncType)&CanSM_BorNoBusOffEnter;
    /* change bus-off recovery state */
    networkPtr->borState = CANSM_BOR_CHECK_L2;
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               bus error handling
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_RequestComMode,CanSM_FullComEnter,CanSM_BorTxOffL1Enter,
 *                     CanSM_BorTxOffL2Enter
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_BusError
(
    P2VAR(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr
)
{
    if (COMM_NO_COMMUNICATION == networkPtr->curMode)
    {
        /* set controller mode as "SLEEP" */
        /* @req CANSM039 @req CANSM044 */
        (void)CanSM_SetCtrMode(networkPtr, CANIF_CS_SLEEP);
    }
    else
    {
        if (COMM_FULL_COMMUNICATION == networkPtr->curMode)
        {
            #if (STD_ON == CANSM_USE_COM)
            /* stop Tx pdu group of the network */
            Com_IpduGroupStop(CanSM_TxPduGroupId[networkPtr->netId]);
            #endif
            /* set pdu mode as "TX_OFFLINE" */
            /* @req CANSM083 @req CANSM209 @req CANSM227 */
            CanSM_SetPduMode(networkPtr, CANIF_SET_TX_OFFLINE);

            #if (STD_ON == CANSM_USE_COM)
            #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING)
            /* enable reception deadline monitor of assigned rx pdu group */
            /* @req CANSM208 @req CANSM230 @req CANSM045-2 */
            Com_EnableReceptionDM(CanSM_RxPduGroupId[networkPtr->netId]);
            #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */
            #endif /* STD_ON == CANSM_USE_COM */

            networkPtr->borState = CANSM_BOR_IDLE;
        }

        #if (STD_ON == CANSM_USE_COM)
        /* stop Rx pdu group of the network */
        Com_IpduGroupStop(CanSM_RxPduGroupId[networkPtr->netId]);
        #endif /* STD_ON == CANSM_USE_COM */
        /* set controller mode as "SLEEP" */
        /* @req CANSM039 @req CANSM044 */
        (void)CanSM_SetCtrMode(networkPtr, CANIF_CS_SLEEP);
        /* change bus-off recovery state */
        networkPtr->curMode = COMM_NO_COMMUNICATION;
        /* mode indication to ComM module */
        /* @req CANSM089 */
        ComM_BusSM_ModeIndication(CANSM_NETWORK_HANDLE(networkPtr->netId),
                                     (ComM_ModeType*)&(networkPtr->curMode));
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               set PDU mode of assigned network
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr:point to network run time structure
 *                     PduModeRequest:pdu mode which requested
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_NoComEnter,CanSM_SilentComEnter,CanSM_FullComEnter,
 *                     CanSM_BorTxOffL1Enter,CanSM_BorCheckL1Enter,CanSM_BorTxOffL2Enter,
 *                     CanSM_BorCheckL2Enter,CanSM_BusError
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(void, CANSM_CODE)
CanSM_SetPduMode
(
    P2CONST(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr,
    CanIf_ChannelSetModeType PduModeRequest
)
{
    P2CONST(uint8, AUTOMATIC, CANSM_VAR)ctrPtr = NULL_PTR;
    uint8 ctr_loop = 0u;

    /* set pdu mode "Online" */
    ctrPtr = CANSM_CONTROLLER_REF(networkPtr->netId);

    for (ctr_loop = 0u; ctr_loop < CANSM_CONTROLLER_COUNT(networkPtr->netId); ctr_loop ++)
    {
        (void)CanIf_SetPduMode(*ctrPtr, PduModeRequest);
        ctrPtr ++;
    }
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*************************************************************************/
/*
 * Brief               set controller mode of assigned network
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      networkPtr: point to network run time structure
 *                     Mode: controller mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanSM_Init,CanSM_NoComEnter,CanSM_FullComEnter,
 *                     CanSM_BorTxOffL1Enter,CanSM_BorTxOffL2Enter,
 *                     CanSM_BusError
 */
/*************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
STATIC FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetCtrMode
(
    P2CONST(CanSM_RunTimeType, AUTOMATIC, CANSM_VAR) networkPtr,
    CanIf_ControllerModeType Mode
)
{
    uint8 ctrLoop = 0u;
    Std_ReturnType canifRet = E_OK;
    Std_ReturnType ret = E_OK;

    for (ctrLoop = 0u;
         ctrLoop < CANSM_CONTROLLER_COUNT(networkPtr->netId);
         ctrLoop ++)
    {
         /* set corresponding CAN Controller state as SLEEP */
         canifRet = CanIf_SetControllerMode(CANSM_CONTROLLER_REF(networkPtr->netId)[ctrLoop], Mode);

         if ((E_OK != canifRet) && (E_OK == ret))
         {
              ret = E_NOT_OK;
         }
    }
    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
