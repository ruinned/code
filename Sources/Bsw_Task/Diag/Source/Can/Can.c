/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <Can.c>
 *  @brief      <Can driver Module source file>
 *
 * <Compiler: CodeWarrior V5.1    MCU:MC9S12>
 *
 *  @author     <bo.zeng>
 *  @date       <24-09-2013>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130924  bo.zeng      Initial version
 *                                      these features not support in this version:
 *                                      1. multiple Can Drivers.
 *                                      2. Can transceiver.
 *                                      3. mixed type HRH only according to CANID 
 *                                      configured now.
 *  V1.0.1      20140522   bo.zeng      move cleaing BOHOLD bit after Can_StopMode,
 *                                      because may cause can't recovery from busoff 
 *                                      forever.
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CAN_C_AR_MAJOR_VERSION      2U
#define CAN_C_AR_MINOR_VERSION      4U
#define CAN_C_AR_PATCH_VERSION      0U

#define CAN_C_SW_MAJOR_VERSION      1U
#define CAN_C_SW_MINOR_VERSION      0U
#define CAN_C_SW_PATCH_VERSION      1U

/*=======[I N C L U D E S]====================================================*/
#include "Can.h"
#if (STD_ON == CAN_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == CAN_DEV_ERROR_DETECT */
#if (STD_ON == CAN_DEM_ERROR_DETECT)
#include "Dem.h"
#endif /* STD_ON == CAN_DEM_ERROR_DETECT */
#include "CanIfDiag_Cbk.h"
#include "SchM_Can.h"
#include "Can_Regs.h"
#include "Can_Irq.h"
#include "Std_ExtendedTypes.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (CAN_C_AR_MAJOR_VERSION != CAN_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif /* CAN_C_AR_MAJOR_VERSION != CAN_H_AR_MAJOR_VERSION */
#if (CAN_C_AR_MINOR_VERSION != CAN_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Minor Version"
#endif /* CAN_C_AR_MINOR_VERSION != CAN_H_AR_MINOR_VERSION */
#if (CAN_C_AR_PATCH_VERSION != CAN_H_AR_PATCH_VERSION)
    #error "Can.c:Mismatch in Specification Patch Version"
#endif /* CAN_C_AR_PATCH_VERSION != CAN_H_AR_PATCH_VERSION */
#if (CAN_C_SW_MAJOR_VERSION != CAN_H_SW_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif /* CAN_C_SW_MAJOR_VERSION != CAN_H_SW_MAJOR_VERSION */
#if (CAN_C_SW_MINOR_VERSION != CAN_H_SW_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Minor Version"
#endif /* CAN_C_SW_MINOR_VERSION != CAN_H_SW_MINOR_VERSION */

#if (STD_ON == CAN_DEV_ERROR_DETECT)
#if (2 != DET_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (2 != DET_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CAN_DEV_ERROR_DETECT */

#if (STD_ON == CAN_DEM_ERROR_DETECT)
#if (3 != DEM_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (1 != DEM_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#endif /* STD_ON == CAN_DEM_ERROR_DETECT */

#if (3 != CANIF_CBK_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (2 != CANIF_CBK_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif

#if (1 != SCHM_CAN_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (1 != SCHM_CAN_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif

#if (2 != CAN_REGS_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (4 != CAN_REGS_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif

#if (2 != CAN_IRQ_H_AR_MAJOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif
#if (4 != CAN_IRQ_H_AR_MINOR_VERSION)
    #error "Can.c:Mismatch in Specification Major Version"
#endif

/*=======[M A C R O S]========================================================*/
/* Can Hardware Number of Mailboxs */
#define CAN_HW_MAX_MAILBOXES         3U

/* DEM Report function empty */
#if (STD_OFF == CAN_DEM_ERROR_DETECT)
#define Dem_ReportErrorStatus(eventId, eventStatus)   do{}while(0)
#endif

/* MISRA RULE 19.7:3453 VIOLATION: the marco like a function */
#if(CAN_VARIANT_PB == CAN_VARIANT_CFG)
/* MISRA RULE 17.4:491 VIOLATION: Array subscripting applied to an object pointer */
#define CAN_CNTRL_CFG(controller)         (Can_GlobalConfigPtr->CanController[controller])
#define CAN_HWOBJ_CFG(hwObjId)            (Can_GlobalConfigPtr->CanHardwareObject[hwObjId])
#define CAN_HWOBJ_ID(hoh)                 (Can_GlobalConfigPtr->CanHoh[hoh])
#define CAN_HWOBJ_NUM                     (Can_GlobalConfigPtr->CanHardwareObjectNum)
#define CAN_HOH_NUM                       (Can_GlobalConfigPtr->CanHohNum)
#else /* CAN_VARIANT_PB == CAN_VARIANT_CFG */
#define CAN_CNTRL_CFG(controller)         (Can_ControllerConfigData[controller])
#define CAN_HWOBJ_CFG(hwObjId)            (Can_HardwareObjectConfigData[hwObjId])
#define CAN_HWOBJ_ID(hoh)                 (Can_HohConfigData[hoh])
#define CAN_HWOBJ_NUM                     CAN_MAX_HARDWAREOBJECTS
#define CAN_HOH_NUM                       CAN_MAX_HOHS
#endif /* CAN_VARIANT_PB == CAN_VARIANT_CFG */

/* MISRA RULE 11.3:303 VIOLATION: Hardware register address operation */
/* Contorller Register */
#define CAN_CNTRL_REG(controller)  \
    ((volatile P2VAR(Can_CntrlRegType, AUTOMATIC, AUTOMATIC))Can_ControllerPCConfigData[controller].CanControllerBaseAddr)

/* controller PC config */
#define CAN_CNTRL_PCCFG(controller)  (Can_ControllerPCConfigData[controller])

/* hardware object ID to Receive mailbox ID */
#define CAN_RXMB_ID(hwObjId)  \
    ((uint16)((hwObjId) - CAN_CNTRL_CFG(CAN_HWOBJ_CFG(hwObjId).CanControllerRef).CanRxHwObjFirst))

/* hardware object ID to Transmit mailbox ID */
#define CAN_TXMB_ID(hwObjId)  \
    ((uint16)((hwObjId) - CAN_CNTRL_CFG(CAN_HWOBJ_CFG(hwObjId).CanControllerRef).CanTxHwObjFirst))

/* Receive mailbox ID to hardware object ID */
#define CAN_RXMB_TO_HWOBJ(Controller, mbId) \
    ((uint16)((mbId) + CAN_CNTRL_CFG(Controller).CanRxHwObjFirst))

/* Transmit mailbox ID to hardware object ID */
#define CAN_TXMB_TO_HWOBJ(Controller, mbId) \
    ((uint16)((mbId) + CAN_CNTRL_CFG(Controller).CanTxHwObjFirst))

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
#if(STD_ON == CAN_DEV_ERROR_DETECT)
/* Can Driver State Machine */
typedef enum
{
    CAN_UNINIT = 0,
    CAN_READY
} Can_DriverStatusType;
#endif /* STD_ON == CAN_DEV_ERROR_DETECT */

typedef enum
{
    CAN_CS_UNINT = 0U,
    CAN_CS_STOPPED,
    CAN_CS_STARTED,
    CAN_CS_SLEEP
} Can_ControllerModeType;

/* Controller Runtime Structure */
typedef struct
{
    Can_ControllerModeType  CntrlMode;      /* controller mode */

    /* Interrupt masks software backup */
    uint8                   SwIntRier;
    uint8                   SwIntTier;

    uint8                   IntLockCount;  

    uint8                   TxPendMask;

    boolean                 IsWakeup;

    /* Transmit PDU handles for TxConfirmation callbacks to CANIF */
    PduIdType               TxPduHandles[CAN_HW_MAX_MAILBOXES];  

    /* Backup Transmit CanId */
    Can_IdType              TxCanId[CAN_HW_MAX_MAILBOXES];  
} Can_ControllerStatusType;

/*=======[I N T E R N A L   D A T A]==========================================*/
#if(STD_ON == CAN_DEV_ERROR_DETECT)
/* @req <CAN103> */
/* can module status:(CAN_UNINIT, CAN_READY) */
#define CAN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Can_MemMap.h"
STATIC VAR(Can_DriverStatusType, CAN_VAR_POWER_ON_INIT) Can_DriverStatus = CAN_UNINIT;
#define CAN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Can_MemMap.h"
#endif /* STD_ON == CAN_DEV_ERROR_DETECT */

/* Global Config Pointer */
#define CAN_START_SEC_CONST_UNSPECIFIED
#include "Can_MemMap.h"
STATIC P2CONST(Can_ConfigType, CAN_CONST, CAN_CONST_PBCFG) Can_GlobalConfigPtr;
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include "Can_MemMap.h"

/* Controller Runtime structure */
#define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Can_MemMap.h"
STATIC VAR(Can_ControllerStatusType, CAN_VAR) Can_Cntrl[CAN_MAX_CONTROLLERS];
#define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Can_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CAN_START_SEC_CODE
#include "Can_MemMap.h"

/* Mode Control */
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_StartMode(uint8 Controller);
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_StopMode(uint8 Controller);
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_SleepMode(uint8 Controller);
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_WakeupMode(uint8 Controller);

STATIC FUNC(void, CAN_CODE) Can_InitHwCntrl
(
    uint8 Controller, 
    P2CONST(Can_ControllerConfigType, AUTOMATIC, CAN_CONST_PBCFG) Config
);
STATIC FUNC(void,    CAN_CODE)   Can_InitMB(uint8 Controller);
#if(STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
STATIC FUNC(void,    CAN_CODE)   Can_TxCancel(uint8 Controller, uint32 mbId);
#endif /* STD_ON == CAN_HW_TRANSMIT_CANCELLATION */
STATIC FUNC(void,    CAN_CODE)   Can_BusOff_Handler(uint8 Controller);
STATIC FUNC(void,    CAN_CODE)   Can_Error_Handler(uint8 Controller);
#if(STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
STATIC FUNC(uint16,  CAN_CODE)   Can_FindLowPriorityMb(uint8 Hth);
#endif /* STD_ON == CAN_MULTIPLEXED_TRANSMISSION */
STATIC FUNC(boolean, CAN_CODE)   Can_IsTxMbFree(uint16 HwObjId);
STATIC FUNC(void,    CAN_CODE)   Can_WriteMb
(
    uint16 HwObjId, 
    P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_CONST) PduInfo
);
STATIC FUNC(void,    CAN_CODE)   Can_GetMBInfo
(
    uint16 HwObjId,
    P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) pdu 
);
#if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
STATIC FUNC(void, CAN_CODE)      Can_CheckAbortMb(uint8 Controller, uint8 Hth);
#endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */

#if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)||(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
STATIC FUNC(boolean, CAN_CODE)   Can_PriorityHigher(Can_IdType destId, Can_IdType srcId);
#endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)||(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */
STATIC FUNC(void, CAN_CODE)      Can_HwSetFilter(uint8 Controller, uint16 HwObjId);
STATIC FUNC(void, CAN_CODE)      Can_HwClearAllMB(uint8 Controller);
#if (STD_ON == CAN_TX_POLLING)
STATIC FUNC(void, CAN_CODE)      Can_TxAllProcess(uint8 Controller);
#endif /* STD_ON == CAN_TX_POLLING */

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/******************************************************************************/
/*
 * Brief               <This function initializes the CAN driver>
 * ServiceId           <0x00>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <Config:Pointer to driver configuration>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_Init
(
    P2CONST(Can_ConfigType, AUTOMATIC, CAN_CONST_PBCFG) Config
)
{
    uint8 controller = 0;
 
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_UNINIT != Can_DriverStatus) /* @req <CAN174> @req <CAN247> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INIT_ID, CAN_E_TRANSITION);
    }
    #if(CAN_VARIANT_PB == CAN_VARIANT_CFG)   
    else if(NULL_PTR == Config)        /* @req <CAN175> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INIT_ID, CAN_E_PARAM_POINTER);
    }
    #endif /* CAN_VARIANT_PB == CAN_VARIANT_CFG */
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    {
        /* backup config pointer */
        #if(CAN_VARIANT_PB == CAN_VARIANT_CFG)   
        Can_GlobalConfigPtr = Config;
        #endif /* #if(CAN_VARIANT_PB == CAN_VARIANT_CFG) */

        /*@req <CAN245> init each controller */
        for(controller = 0; controller < CAN_MAX_CONTROLLERS; controller++)
        {
            Can_Cntrl[controller].SwIntRier    = 0U;
            Can_Cntrl[controller].SwIntTier    = 0U;
            Can_Cntrl[controller].IntLockCount = 0U;
            Can_Cntrl[controller].TxPendMask   = 0U;
            Can_Cntrl[controller].IsWakeup     = FALSE;
            Can_Cntrl[controller].CntrlMode    = CAN_CS_STOPPED; /* @req <CAN259> */
        }

        /* @req <CAN246> */
        #if (STD_ON == CAN_DEV_ERROR_DETECT)
        Can_DriverStatus = CAN_READY;  
        #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    }
    
    return;
}

/******************************************************************************/
/*
 * Brief               <This function initializing only Can controller specific settings>
 * ServiceId           <0x02>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <Controller-CAN controller to be initialized>
 * Param-Name[out]     <Config-pointer to controller configuration>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_InitController
(
    uint8 Controller,
    P2CONST(Can_ControllerConfigType, AUTOMATIC, CAN_CONST_PBCFG) Config
)
{
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)  /* @req <CAN187> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INITCONTROLLER_ID, CAN_E_UNINIT);
    }
    else if(NULL_PTR == Config)        /* @req <CAN188> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INITCONTROLLER_ID, CAN_E_PARAM_POINTER);
    }
    else if(Controller >= CAN_MAX_CONTROLLERS) /* @req <CAN189> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INITCONTROLLER_ID, 
                CAN_E_PARAM_CONTROLLER);
    }
    else if(CAN_CS_STOPPED != Can_Cntrl[Controller].CntrlMode) /* @req <CAN190> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_INITCONTROLLER_ID, CAN_E_TRANSITION);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */   
    {
        Can_InitHwCntrl(Controller, Config);

        Can_Cntrl[Controller].CntrlMode = CAN_CS_STOPPED;  /* @req <CAN256> */
    }
    
    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs software triggered state transitions 
 *                         of the CAN controller State machine.>
 * ServiceId           <0x03>
 * Sync/Async          <Asynchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <Controller-CAN controller for which the status shall be changed>
 * Param-Name[in]      <Transition-Possible transitions>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Can_ReturnType-CAN_OK or CAN_NOT_OK>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/*@req <CAN230> @req <CAN017> */
FUNC(Can_ReturnType, CAN_CODE) Can_SetControllerMode
(
    uint8 Controller,
    Can_StateTransitionType Transition
)
{
    Can_ReturnType ret = CAN_NOT_OK;

    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)          /* @req <CAN0198> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_UNINIT);
        ret = CAN_NOT_OK;
    }
    else if(Controller >= CAN_MAX_CONTROLLERS) /* @req <CAN0199> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_SETCONTROLLERMODE_ID, CAN_E_PARAM_CONTROLLER);
        ret = CAN_NOT_OK;
    }    
    /* @req <CAN0200> */
    else if(Transition > CAN_T_CNT)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_TRANSITION);
        ret = CAN_NOT_OK;
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    {
        switch (Transition)
        {
            case CAN_T_START:
                ret = Can_StartMode(Controller);
                break;
            case CAN_T_STOP:
                ret = Can_StopMode(Controller);
                break;
            case CAN_T_SLEEP:
                ret = Can_SleepMode(Controller);
                break;
            case CAN_T_WAKEUP:
                ret = Can_WakeupMode(Controller);
                break;
            default:
                ret = CAN_NOT_OK;
                break;
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function disable all interrupt for this controller. >
 * ServiceId           <0x04>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Controller- CAN controller for which interrupts shall be disabled>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/*@req <CAN0231>*/
FUNC(void, CAN_CODE) Can_DisableControllerInterrupts(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    /*@req <CAN0205>*/
    /*@req <CAN0206>*/
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_DISABLECONTROLLERINTERRUPTS_ID, CAN_E_UNINIT);
    }
    else if(Controller >= CAN_MAX_CONTROLLERS)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_DISABLECONTROLLERINTERRUPTS_ID, CAN_E_PARAM_CONTROLLER);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    {
        SchM_Enter_Can(CAN_INSTANCE, INTERRUPT_PROTECTION_AREA);

        if(0U == Can_Cntrl[Controller].IntLockCount)
        {
            canRegs = CAN_CNTRL_REG(Controller);

            /* Turn off Tx/Rx/Busoff/Err/Tx warning/Rx warning/Wakeup interrupts */
            canRegs->Tier = 0U;
            canRegs->Rier = 0U;
        }

        if (Can_Cntrl[Controller].IntLockCount < 255U)
        {
            Can_Cntrl[Controller].IntLockCount++;
        }

        SchM_Exit_Can(CAN_INSTANCE, INTERRUPT_PROTECTION_AREA);             
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function enable all allowed interrupts. >
 * ServiceId           <0x05>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Controller- CAN controller for which interrupts shall be disabled>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/* @req <CAN0232> @req <CAN050> */
FUNC(void, CAN_CODE)  Can_EnableControllerInterrupts(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    /*@req <CAN0209>*/
    /*@req <CAN0210>*/
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_ENABLECONTROLLERINTERRUPTS_ID, CAN_E_UNINIT);
    }
    else if(Controller >= CAN_MAX_CONTROLLERS)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_ENABLECONTROLLERINTERRUPTS_ID, CAN_E_PARAM_CONTROLLER);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */   
    {

        SchM_Enter_Can(CAN_INSTANCE, INTERRUPT_PROTECTION_AREA);

        /* @req <CAN0209> enable interrupt call before disable interrupt no action */
        if(Can_Cntrl[Controller].IntLockCount > 0U)
        {
            Can_Cntrl[Controller].IntLockCount--;

            /* apply software int flag to hardware */
            if(0U == Can_Cntrl[Controller].IntLockCount)
            {
                canRegs = CAN_CNTRL_REG(Controller);

                canRegs->Tier = Can_Cntrl[Controller].SwIntTier;
                canRegs->Rier = Can_Cntrl[Controller].SwIntRier;
            }
        }

        SchM_Exit_Can(CAN_INSTANCE, INTERRUPT_PROTECTION_AREA);             
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function perform HW-Transmit handle transmit. >
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Hth-information which HW-transmit handle shall be used for transmit.>
 * Param-Name[in]      <PduInfo-Pointer to SDU user memory,DLC and Identifier>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Can_ReturnType-Returns CAN_OK,CAN_NOT_OK or CAN_BUSY>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/* @req <CAN276> @req <CAN0233> @req <CAN0212> @req <CAN0275> */
FUNC(Can_ReturnType, CAN_CODE) Can_Write
(
    uint8 Hth,
    P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_DATA) PduInfo
)
{
    Can_ReturnType ret = CAN_NOT_OK;
    uint8   controller;
    uint16  hwObjId = 0;
    
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    boolean errFlag = FALSE;

    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, CAN_E_UNINIT);
        errFlag = TRUE;
        ret = CAN_NOT_OK;
    }
    /* check hth is vaild */
    else if((Hth >= CAN_HOH_NUM) 
            || (CAN_OBJECT_TYPE_TRANSMIT != CAN_HWOBJ_CFG(CAN_HWOBJ_ID(Hth)).CanObjectType)) 
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, CAN_E_PARAM_HANDLE);
        errFlag = TRUE;
        ret = CAN_NOT_OK;
    }
    else if((NULL_PTR == PduInfo) || (NULL_PTR == PduInfo->sdu))
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, CAN_E_PARAM_POINTER);
        errFlag = TRUE;
        ret = CAN_NOT_OK;
    }
    else if(CAN_DATA_LENGTH < PduInfo->length)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, CAN_E_PARAM_DLC);
        errFlag = TRUE;
        ret = CAN_NOT_OK;
    }
    else
    {
        /* do nothing */
    }

    if (FALSE == errFlag)
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */                    
    {
        hwObjId = CAN_HWOBJ_ID(Hth);
        controller = CAN_HWOBJ_CFG(hwObjId).CanControllerRef;

        Can_DisableControllerInterrupts(controller);

        if (CAN_CS_STARTED == Can_Cntrl[controller].CntrlMode)
        {
            #if(STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
            hwObjId = Can_FindLowPriorityMb(Hth);
            #endif /* STD_ON == CAN_MULTIPLEXED_TRANSMISSION */

            if (TRUE == Can_IsTxMbFree(hwObjId))
            {
                Can_WriteMb(hwObjId, PduInfo);
                ret = CAN_OK;
            }
            else 
            {
                #if(STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
                uint16  mbId = 0;
                mbId = CAN_TXMB_ID(hwObjId);
                if (TRUE == Can_PriorityHigher(PduInfo->id, 
                            Can_Cntrl[controller].TxCanId[mbId]))
                {
                    Can_TxCancel(controller, mbId);
                }
                #endif /* STD_ON == CAN_HW_TRANSMIT_CANCELLATION */

                ret = CAN_BUSY;
            }
        }

        Can_EnableControllerInterrupts(controller);
    }

    return ret;    
}

/******************************************************************************/
/*
 * Brief               <This function checks if a wakeup has occurred for the given controller. >
 * ServiceId           <0x0b>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <Controller- Controller to be checked for a wakeup>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType- Returns-E_OK or E_NOT_OK>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(Std_ReturnType, CAN_CODE) Can_Cbk_CheckWakeup(uint8 Controller)
{
    #if(STD_ON == CAN_WAKEUP_SUPPORT)
    Std_ReturnType ret = E_NOT_OK;
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

     /*@req <CAN362>*/
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_CBK_CHECKWAKEUP_ID, CAN_E_UNINIT);
        ret = E_NOT_OK;
    }
    /*@req <CAN363>*/
    else if(Controller >= CAN_MAX_CONTROLLERS)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, 
                CAN_CBK_CHECKWAKEUP_ID, CAN_E_PARAM_CONTROLLER);
        ret = E_NOT_OK;
    } 
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    {
        canRegs = CAN_CNTRL_REG(Controller);

        /* Check Controller Wakeup flag */
        if(TRUE == Can_Cntrl[Controller].IsWakeup)
        {
            Can_Cntrl[Controller].IsWakeup = FALSE;
            ret = E_OK;
        }
    }
    
    return ret;
    #else
    return E_NOT_OK;
    #endif /* STD_ON == CAN_WAKEUP_SUPPORT */
}

/******************************************************************************/
/*
 * Brief               <This function performs the polling of TX confirmation and TX cancellation 
 *                          confirmation when.CAN_TX_PROCESSING is set to POLLING. >
 * ServiceId           <0x01>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/

/* @req <CAN0225> @req <CAN031> */
FUNC(void, CAN_CODE) Can_MainFunction_Write(void)
{
    #if (STD_ON == CAN_TX_POLLING)
    uint8 controller;

    /*@req <CAN179>*/
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    /*@req <CAN187>*/
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_MAINFUCTION_WRITE_ID, CAN_E_UNINIT);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */
    {
        /* loop each Controller TX comfirmation and TX cancel comfirmation */
        for (controller = 0; controller < CAN_MAX_CONTROLLERS; controller++)
        {
            /*@req <CAN178>*/
            if((CAN_PROCESS_TYPE_POLLING == CAN_CNTRL_PCCFG(controller).CanTxProcessing)
                    && (CAN_CS_STARTED == Can_Cntrl[controller].CntrlMode))
            {
                Can_TxAllProcess(controller);
            }
        }
    }
    #endif /* STD_ON == CAN_TX_POLLING */

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs the polling of RX indications when 
 *                          CAN_RX_PROCESSING is set to POLLING.> 
 * ServiceId           <0x08>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
 /*@req <CAN012>
[heguarantee that neither the ISRs 
 nor the function Can_MainFunction_Read can be interrupted by itself. ]
*/
/* @req <CAN226> @req <CAN108> @req <CAN180> */
FUNC(void, CAN_CODE) Can_MainFunction_Read(void)
{
    #if (STD_ON == CAN_RX_POLLING)
    uint8 controller;

    /*@req <CAN181>*/
    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    /*@req <CAN187>*/
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_MAINFUNCTION_READ_ID, CAN_E_UNINIT);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */    
    {
        /* scan each Controller */
        for (controller = 0; controller < CAN_MAX_CONTROLLERS; controller++)
        {
            if((CAN_PROCESS_TYPE_POLLING == CAN_CNTRL_PCCFG(controller).CanRxProcessing)
                    && (CAN_CS_STARTED == Can_Cntrl[controller].CntrlMode))
            {
                P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
                canRegs = CAN_CNTRL_REG(controller);
                while (CAN_RFLG_RXF == (canRegs->Rflg & CAN_RFLG_RXF))
                {
                    Can_RxProcess(controller);
                }
            }
        }        
    }
    #endif /* STD_ON == CAN_RX_POLLING */

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs the polling of bus-off events that are configured
 *                          statically as "to be polled".> 
 * ServiceId           <0x09>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_MainFunction_BusOff(void)
{
    #if (STD_ON == CAN_BUSOFF_POLLING)
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint8 controller;

    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_READY != Can_DriverStatus)  /* @req <CAN184> */
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_MAINFUNCTION_BUSOFF_ID, CAN_E_UNINIT);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */  
    {
        for (controller = 0; controller < CAN_MAX_CONTROLLERS; controller++)
        {
            if((CAN_PROCESS_TYPE_POLLING == CAN_CNTRL_PCCFG(controller).CanBusOffProcessing)
                    && (CAN_CS_STARTED == Can_Cntrl[controller].CntrlMode))
            {
                Can_ErrTwRwBusOff_Handler(controller);
            }
        }       
    }
    #endif /* STD_ON == CAN_BUSOFF_POLLING */

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs the polling of wake-up events that are configured
 *                          statically as "to be polled".> 
 * ServiceId           <0x0a>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_MainFunction_Wakeup(void)
{
    #if((STD_ON == CAN_WAKEUP_POLLING) && (STD_ON == CAN_WAKEUP_SUPPORT))
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint8 controller;

    #if(STD_ON == CAN_DEV_ERROR_DETECT)
    /*@req <CAN184>*/
    if(CAN_READY != Can_DriverStatus)
    {
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_MAINFUNCTION_WAKEUP_ID, CAN_E_UNINIT);
    }
    else
    #endif /* STD_ON == CAN_DEV_ERROR_DETECT */    
    {
        for (controller = 0; controller < CAN_MAX_CONTROLLERS; controller++)
        {
            if((CAN_PROCESS_TYPE_POLLING == CAN_CNTRL_PCCFG(controller).CanWakeupProcessing)
                    && (CAN_CS_SLEEP == Can_Cntrl[controller].CntrlMode))
            {
                canRegs = CAN_CNTRL_REG(controller);
                if(CAN_RFLG_WUPIF == (CAN_RFLG_WUPIF & canRegs->Rflg))
                {
                    Can_Wakeup_Handler(controller);
                }
            }
        }        
    }
    #endif /* (STD_ON == CAN_WAKEUP_POLLING) && (STD_ON == CAN_WAKEUP_SUPPORT) */

    return;
}

/*=======[I N T E R N A L  F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/*
 * Brief               <This function performs bus-off  process>
 * Param-Name[in]      <Controller- CAN controller to be Tw/Rw/bus-off process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_ErrTwRwBusOff_Handler(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    canRegs = CAN_CNTRL_REG(Controller);

    if (CAN_RFLG_CSCIF == (canRegs->Rflg & CAN_RFLG_CSCIF))
    {
        /* BusOff */
        if ((CAN_RFLG_TXBUSOFF == (canRegs->Rflg & CAN_RFLG_TSTAT))
                || (CAN_RFLG_RXBUSOFF == (canRegs->Rflg & CAN_RFLG_RSTAT)))
        {
            Can_BusOff_Handler(Controller);
        }
        /* Error */
        else if ((CAN_RFLG_TXERR == (canRegs->Rflg & CAN_RFLG_TSTAT))
                || (CAN_RFLG_RXERR == (canRegs->Rflg & CAN_RFLG_RSTAT)))
        {
            Can_Error_Handler(Controller);
        }
        else
        {
            /* clear status change interrupt flag */
            canRegs->Rflg = CAN_RFLG_CSCIF;
        }
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs wake up  process>
 * Param-Name[in]      <Controller- CAN controller to be wake up process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_Wakeup_Handler(uint8 Controller)
{
    #if(STD_ON == CAN_WAKEUP_SUPPORT)
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    /* Transition the controller to freeze mode */
    if(CAN_OK == Can_WakeupMode(Controller))
    {
        canRegs = CAN_CNTRL_REG(Controller);
        canRegs->Rflg = CAN_RFLG_RXF;      /* clear IFALG */
        canRegs->Rflg = CAN_RFLG_WUPIF;    /* Clear the Wakeup flag */

        Can_Cntrl[Controller].IsWakeup = TRUE;

        /* @req CAN271 */
        EcuM_CheckWakeup(CAN_CNTRL_PCCFG(Controller).CanWakeupSourceRef); 

    }
    #endif /* #if(STD_ON == CAN_WAKEUP_SUPPORT) */

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs error  process>
 * Param-Name[in]      <Controller- CAN controller to be error process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_Error_Handler(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    canRegs = CAN_CNTRL_REG(Controller);

    /* clear status change interrupt flag */
    canRegs->Rflg = CAN_RFLG_CSCIF;

    /* user add ... */

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs bus-off  process>
 * Param-Name[in]      <Controller- CAN controller to be Tw/Rw/bus-off process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_BusOff_Handler(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    canRegs = CAN_CNTRL_REG(Controller);

    /* clear status change interrupt flag */
    canRegs->Rflg = CAN_RFLG_CSCIF;

    /* @req <CAN272> */
    if(CAN_OK == Can_StopMode(Controller))
    {
        /* request recovery from bus-off */
        canRegs->Misc = 0x1U;

        CanIf_ControllerBusOff(Controller);
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs Tx confirmation and Tx cancellation process>
 * Param-Name[in]      <Controller- CAN controller to be Tx process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_TxProcess(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint16 hwObjId    = 0;
    uint16 endHwObjId = 0;
    uint8  iFlag = 0;
    uint16 mbId = 0;

    canRegs = CAN_CNTRL_REG(Controller);
    iFlag = canRegs->Tflg & Can_Cntrl[Controller].TxPendMask;
 
    endHwObjId = (uint16)(CAN_CNTRL_CFG(Controller).CanTxHwObjFirst
                          + CAN_CNTRL_CFG(Controller).CanTxHwObjCount);
    for (hwObjId = CAN_CNTRL_CFG(Controller).CanTxHwObjFirst; hwObjId < endHwObjId; hwObjId++)
    {
        mbId = CAN_TXMB_ID(hwObjId);
        if(0 != (iFlag & (uint8)(1U << mbId)))
        {
            /* clear Tx pend mask */
            Can_Cntrl[Controller].TxPendMask &= (uint8)(~(uint8)(1U << mbId));
            /* Diable sepcified mbid tx interrupt */
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanTxProcessing)
            {
                /* Turn on the interrupt mailboxes */
                Can_Cntrl[Controller].SwIntTier &= (uint8)(~(uint8)(1U << mbId));
                if (0 == Can_Cntrl[Controller].IntLockCount)
                {
                    canRegs->Tier = Can_Cntrl[Controller].SwIntTier;
                }
            }

            #if(STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
            /* Abort Acknowledge */
            if(0 != (canRegs->Taak & (uint8)(1U << mbId)))
            {
                uint8  txData[8];
                Can_PduType pdu;      /* */

                pdu.sdu = txData;
                Can_GetMBInfo(hwObjId, &pdu);
                pdu.swPduHandle = Can_Cntrl[Controller].TxPduHandles[mbId];

                CanIf_CancelTxConfirmation(&pdu);
            }
            else
            #endif /* STD_ON == CAN_HW_TRANSMIT_CANCELLATION */
            {
                CanIf_TxConfirmation(Can_Cntrl[Controller].TxPduHandles[mbId]);

                #if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
                Can_CheckAbortMb(Controller, CAN_HWOBJ_CFG(hwObjId).CanObjectId);
                #endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */
            }
            
            break;
        }
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function performs Rx indications  process>
 * Param-Name[in]      <Controller- CAN controller to be Rx process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
FUNC(void, CAN_CODE) Can_RxProcess(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    Can_PduType pdu;
    uint8   rxData[8];
    uint16  hwObjId;

    canRegs = CAN_CNTRL_REG(Controller);

    /* get hit filter, filter and hardware object is one-to-one */
    hwObjId = CAN_RXMB_TO_HWOBJ(Controller, canRegs->Idac & CAN_IDAC_IDHIT);

    /* Get pdu */
    pdu.sdu = rxData;
    Can_GetMBInfo(hwObjId, &pdu);

    canRegs->Rflg = CAN_RFLG_RXF;  /* clear IFALG */
    CanIfDiag_RxIndication(CAN_HWOBJ_CFG(hwObjId).CanObjectId, pdu.id, pdu.length, pdu.sdu);

    return;
}

#if(STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
/******************************************************************************/
/*
 * Brief               <This function performs Tx cancellation process>
 * Param-Name[in]      <Controller- CAN controller to be Tx process>
 * Param-Name[in]      <mbId- CAN controller message buffer index>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_TxCancel(uint8 Controller, uint32 mbId)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    canRegs = CAN_CNTRL_REG(Controller);

    if (0U == (canRegs->Tflg & (uint8)(1U << mbId)))
    {
        canRegs->Tarq = (uint8)(1U << mbId);
    }

    return;
}
#endif /* STD_ON == CAN_HW_TRANSMIT_CANCELLATION */

#if (STD_ON == CAN_TX_POLLING)
/******************************************************************************/
/*
 * Brief               <This function performs Tx process for all mailboxes>
 * Param-Name[in]      <Controller- CAN controller to be Tx process>
 * Param-Name[in]      <mbId- CAN controller message buffer index>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_TxAllProcess(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint16 hwObjId    = 0;
    uint16 endHwObjId = 0;
    uint8  iFlag = 0;
    uint16 mbId = 0;

    canRegs = CAN_CNTRL_REG(Controller);
    iFlag = canRegs->Tflg & Can_Cntrl[Controller].TxPendMask;
 
    endHwObjId = (uint16)(CAN_CNTRL_CFG(Controller).CanTxHwObjFirst
                          + CAN_CNTRL_CFG(Controller).CanTxHwObjCount);
    for (hwObjId = CAN_CNTRL_CFG(Controller).CanTxHwObjFirst; hwObjId < endHwObjId; hwObjId++)
    {
        mbId = CAN_TXMB_ID(hwObjId);
        if(0 != (iFlag & (uint8)(1U << mbId)))
        {
            /* clear Tx pend mask */
            Can_Cntrl[Controller].TxPendMask &= (uint8)(~(uint8)(1U << mbId));
            /* Diable sepcified mbid tx interrupt */
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanTxProcessing)
            {
                /* Turn on the interrupt mailboxes */
                Can_Cntrl[Controller].SwIntTier &= (uint8)(~(uint8)(1U << mbId));
                if (0 == Can_Cntrl[Controller].IntLockCount)
                {
                    canRegs->Tier = Can_Cntrl[Controller].SwIntTier;
                }
            }

            #if(STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
            /* Abort Acknowledge */
            if(0 != (canRegs->Taak & (uint8)(1U << mbId)))
            {
                uint8  txData[8];
                Can_PduType pdu;

                pdu.sdu = txData;
                Can_GetMBInfo(hwObjId, &pdu);
                pdu.swPduHandle = Can_Cntrl[Controller].TxPduHandles[mbId];

                CanIf_CancelTxConfirmation(&pdu);
            }
            else
            #endif /* STD_ON == CAN_HW_TRANSMIT_CANCELLATION */
            {
                CanIf_TxConfirmation(Can_Cntrl[Controller].TxPduHandles[mbId]);

                #if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
                Can_CheckAbortMb(Controller, CAN_HWOBJ_CFG(hwObjId).CanObjectId);
                #endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */
            }
        }
    }

    return;
}
#endif /* STD_ON == CAN_TX_POLLING */

/******************************************************************************/
/*
 * Brief               <This function performs controller start Mode  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/* @req <CAN261> */
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_StartMode(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    Can_ReturnType ret = CAN_NOT_OK;
    uint16 timeout  = 0U;

    if (CAN_CS_STARTED == Can_Cntrl[Controller].CntrlMode)
    {
        ret = CAN_OK;
    }
    else if(CAN_CS_STOPPED == Can_Cntrl[Controller].CntrlMode)  
    {
        canRegs = CAN_CNTRL_REG(Controller);

        /* 
         * goto normal mode 
         */
        canRegs->Ctl0 &= ~CAN_CTL0_INITRQ;
        timeout = CAN_TIMEOUT_DURATION;
        while((timeout > 0U) && (CAN_CTL1_INITAK == (canRegs->Ctl1 & CAN_CTL1_INITAK)))
        {
            timeout--;
        }

        if(0U == timeout)
        {
            Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
            ret = CAN_NOT_OK;
        }                
        else
        {
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanRxProcessing)
            {
                /* Turn on the interrupt mailboxes */
                Can_Cntrl[Controller].SwIntRier |= CAN_RIER_RXFIE;
            }
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanBusOffProcessing)
            {
                Can_Cntrl[Controller].SwIntRier |= CAN_RIER_CSCIE;
            }
            /* RSTATE: 3, TSTATE: 3 */
            Can_Cntrl[Controller].SwIntRier |= 0x3CU;

            /* apply software int flag to hardware */
            if (0U == Can_Cntrl[Controller].IntLockCount)
            {
                canRegs->Rier = Can_Cntrl[Controller].SwIntRier;
            }
            
            Can_Cntrl[Controller].CntrlMode = CAN_CS_STARTED;
            ret = CAN_OK;
        }
    }
    else
    {
        #if(STD_ON == CAN_DEV_ERROR_DETECT)
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_TRANSITION);
        #endif
        ret = CAN_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function performs controller stop Mode  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/* @req <CAN283> */
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_StopMode(uint8 Controller)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    Can_ReturnType ret = CAN_NOT_OK;
    uint16 timeout  = 0U;
    uint16 i = 0U;

    if (CAN_CS_STOPPED == Can_Cntrl[Controller].CntrlMode)
    {
        ret = CAN_OK;
    }
    else if(CAN_CS_STARTED == Can_Cntrl[Controller].CntrlMode)  
    {
        canRegs = CAN_CNTRL_REG(Controller);

        /* turn off Rx/Tx/Busoff/Err/Tx Warning/Rx Warning/wakeup interrupt */
        Can_Cntrl[Controller].SwIntTier = 0U;
        Can_Cntrl[Controller].SwIntRier = 0U;
        /* apply software flag to hardware */
        if (0U == Can_Cntrl[Controller].IntLockCount)
        {
            canRegs->Tier = Can_Cntrl[Controller].SwIntTier;
            canRegs->Rier = Can_Cntrl[Controller].SwIntRier;
        }

        /* @req <CAN282> */
        /* clear all mailboxs */
        Can_HwClearAllMB(Controller);

        /* goto init mode */
        canRegs->Ctl0 |= CAN_CTL0_INITRQ;
        timeout = CAN_TIMEOUT_DURATION;
        while((timeout > 0U) && (CAN_CTL1_INITAK != (canRegs->Ctl1 & CAN_CTL1_INITAK)))
        {
            timeout--;
        }
        if(0U == timeout)
        {
            /* report product error  */
            Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
            ret = CAN_NOT_OK;
        } 
        else
        {
            Can_Cntrl[Controller].CntrlMode = CAN_CS_STOPPED;
            ret = CAN_OK;
        }
    }
    else
    {
        #if(STD_ON == CAN_DEV_ERROR_DETECT)
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_TRANSITION);
        #endif
        ret = CAN_NOT_OK;
    } 

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function performs controller sleep Mode  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/* @req <CAN265> */
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_SleepMode(uint8 Controller)
{
    Can_ReturnType ret = CAN_NOT_OK;

    if (CAN_CS_SLEEP == Can_Cntrl[Controller].CntrlMode)
    {
        ret = CAN_OK;
    }
    else if(CAN_CS_STOPPED == Can_Cntrl[Controller].CntrlMode)  
    {
        #if(STD_ON == CAN_WAKEUP_SUPPORT)
        P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
        uint16 timeout  = 0U;

        canRegs = CAN_CNTRL_REG(Controller);

        /*  goto sleep Mode */
        timeout = CAN_TIMEOUT_DURATION;
        canRegs->Ctl0 &= ~(CAN_CTL0_INITRQ);
        canRegs->Ctl0 |= CAN_CTL0_SLPRQ;
        while((timeout > 0U) 
                && ((CAN_CTL1_INITAK == (canRegs->Ctl1 & CAN_CTL1_INITAK)) 
                    || (CAN_CTL1_SLPAK != (canRegs->Ctl1 & CAN_CTL1_SLPAK))))
        {
            timeout--;
        }
        if(0U == timeout)
        {
            Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
            ret = CAN_NOT_OK;
        }
        else
        {
            /* Enable wake up interrupt */
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanWakeupProcessing)
            {
                /* Enable wake up interrupt */
                Can_Cntrl[Controller].SwIntRier |= CAN_RIER_WUPIE;

                /* apply software int flag to hardware */
                if (0U == Can_Cntrl[Controller].IntLockCount)
                {
                    canRegs->Rier |= CAN_RIER_WUPIE;
                }
            }

            Can_Cntrl[Controller].CntrlMode = CAN_CS_SLEEP;
            ret = CAN_OK;
        }
        #else
        /* @req CAN290 */
        Can_Cntrl[Controller].CntrlMode = CAN_CS_SLEEP;
        ret = CAN_OK;
        #endif /* #if(STD_ON == CAN_WAKEUP_SUPPORT) */
    }
    else
    {
        #if(STD_ON == CAN_DEV_ERROR_DETECT)
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_TRANSITION);
        #endif
        ret = CAN_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function performs controller wakeup Mode  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(Can_ReturnType, CAN_CODE) Can_WakeupMode(uint8 Controller)
{
    Can_ReturnType ret = CAN_NOT_OK;
    
    if (CAN_CS_STOPPED == Can_Cntrl[Controller].CntrlMode)
    {
        ret = CAN_OK;
    }
    else if(CAN_CS_SLEEP == Can_Cntrl[Controller].CntrlMode)  
    {
        #if (STD_ON == CAN_WAKEUP_SUPPORT)
        P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
        uint16 timeout  = 0U;

        canRegs = CAN_CNTRL_REG(Controller);

        /* Exit Sleep Mode */
        canRegs->Ctl0 &= ~CAN_CTL0_SLPRQ;
        timeout = CAN_TIMEOUT_DURATION;
        while((timeout > 0U) && (CAN_CTL1_SLPAK == (canRegs->Ctl1 & CAN_CTL1_SLPAK)))
        {
            timeout--;
        }
        if(0U == timeout)
        {
            Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
            ret = CAN_NOT_OK;
        }
        else
        {
            /* Disable wake up interrupt */
            #if(STD_ON == CAN_WAKEUP_SUPPORT)
            if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(Controller).CanWakeupProcessing)
            {
                /* Enable wake up interrupt */
                Can_Cntrl[Controller].SwIntRier &= ~CAN_RIER_WUPIE;

                /* apply software int flag to hardware */
                if (0U == Can_Cntrl[Controller].IntLockCount)
                {
                    canRegs->Rier &= ~CAN_RIER_WUPIE;
                }
            }
            #endif /* #if(STD_ON == CAN_WAKEUP_SUPPORT) */

            canRegs->Ctl0 |= CAN_CTL0_INITRQ;
            timeout = CAN_TIMEOUT_DURATION;
            while((timeout > 0U) && (CAN_CTL1_INITAK != (canRegs->Ctl1 & CAN_CTL1_INITAK)))
            {
                timeout--;
            }
            if(0U == timeout)
            {
                Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
                ret = CAN_NOT_OK;
            }
            else
            {
                Can_Cntrl[Controller].CntrlMode = CAN_CS_STOPPED;
                ret = CAN_OK;
            }
        }
        #else
        Can_Cntrl[Controller].CntrlMode = CAN_CS_STOPPED;
        ret = CAN_OK;
        #endif /* STD_ON == CAN_WAKEUP_SUPPORT */
    }
    else
    {
        #if(STD_ON == CAN_DEV_ERROR_DETECT)
        Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE, CAN_SETCONTROLLERMODE_ID, CAN_E_TRANSITION);
        #endif
        ret = CAN_NOT_OK;                
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function performs controller wakeup Mode  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_InitHwCntrl
(
    uint8 Controller, 
    P2CONST(Can_ControllerConfigType, AUTOMATIC, CAN_CONST_PBCFG) Config
)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint16  timeout = 0U;

    canRegs = CAN_CNTRL_REG(Controller);

    /* Eanble Module */
    canRegs->Ctl1 |= CAN_CTL1_CANE;

    /* Goto initialization Mode */
    canRegs->Ctl0 |= CAN_CTL0_INITRQ;
    timeout = CAN_TIMEOUT_DURATION;
    while ((timeout > 0U) && (CAN_CTL1_INITAK != (canRegs->Ctl1 & CAN_CTL1_INITAK)))
    {
        timeout--;
    }
    if (0U == timeout)
    {
       Dem_ReportErrorStatus(CAN_E_TIMEOUT, DEM_EVENT_STATUS_FAILED);
    }

    /* Select Clock Source */
    if (CAN_CLOCKSRC_TYPE_OSC == CAN_CNTRL_PCCFG(Controller).CanClockSource)
    {
        canRegs->Ctl1 &= (~CAN_CTL1_CLKSRC);  
    }
    else   /* bus clock */
    {
        canRegs->Ctl1 |= CAN_CTL1_CLKSRC;  
    }

    /* Disable List-Only Mode */
    canRegs->Ctl1 &= ~CAN_CTL1_LISTEN;

    /* @req <CAN274> */
    /* Diable automatic bus-off reovery */
    canRegs->Ctl1 |= CAN_CTL1_BORM;

    /* Wake-up Enable */
    #if (STD_ON == CAN_WAKEUP_SUPPORT)
    canRegs->Ctl0 |= CAN_CTL0_WUPE;
    #endif /* STD_ON == CAN_WAKEUP_SUPPORT */

    /* Wake-up filter on */
    canRegs->Ctl1 |= CAN_CTL1_WUPM;

    /* set bit time */
    canRegs->Btr0 = CAN_CNTRL_CFG(Controller).Btr0;
    canRegs->Btr1 = CAN_CNTRL_CFG(Controller).Btr1;

    /* set filter type */
    canRegs->Idac = CAN_CNTRL_CFG(Controller).CanFilterType;  

    /* init all mailboxs */
    Can_InitMB(Controller);
}

/******************************************************************************/
/*
 * Brief               <This function performs controller init message buffer  process>
 * Param-Name[in]      <Controller- CAN controller to be Mode process>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_InitMB(uint8 Controller)
{
    uint16 hwObjId   = 0U;
    uint16 endHwObj  = 0U;
    
    /* 
     * Init Receive Hardware Object 
     */
    endHwObj = (uint16)(CAN_CNTRL_CFG(Controller).CanRxHwObjFirst 
                        + CAN_CNTRL_CFG(Controller).CanRxHwObjCount);
    for (hwObjId = CAN_CNTRL_CFG(Controller).CanRxHwObjFirst; hwObjId < endHwObj; hwObjId++)
    {
        Can_HwSetFilter(Controller, hwObjId);
    }

    return;
}

#if(STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
/******************************************************************************/
/*
 * Brief               <find lowest priority mailbox for same hth>
 * Param-Name[in]      <Hth - HW-transmit handle>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(uint16, CAN_CODE) Can_FindLowPriorityMb(uint8 Hth)
{
    uint16 hwObjId  = 0U;
    uint16 retHwObj = CAN_HWOBJ_ID(Hth);
    uint32 canId    = 0U; 
    uint8  controller = CAN_HWOBJ_CFG(hwObjId).CanControllerRef;

    if (FALSE == Can_IsTxMbFree(retHwObj))
    {
        canId = Can_Cntrl[controller].TxCanId[CAN_TXMB_ID(retHwObj)];
        hwObjId = (uint16)(retHwObj + 1U);
        while ((hwObjId < CAN_HWOBJ_NUM) && (Hth == CAN_HWOBJ_CFG(hwObjId).CanObjectId))
        {
            if (TRUE == Can_IsTxMbFree(hwObjId))    
            {
                retHwObj = hwObjId;
                break;
            }
            else if (TRUE == Can_PriorityHigher(canId, 
                        Can_Cntrl[controller].TxCanId[CAN_TXMB_ID(hwObjId)]))
            {
                retHwObj = hwObjId;
                canId = Can_Cntrl[controller].TxCanId[CAN_TXMB_ID(hwObjId)];
            }
            else
            {
                /* do nothing */
            }

            hwObjId++;
        }
    }

    return retHwObj;
}
#endif /* STD_ON == CAN_MULTIPLEXED_TRANSMISSION */

/******************************************************************************/
/*
 * Brief               <check a mailbox if free, can transmit pdu>
 * Param-Name[in]      <HwObjId - hardware object index>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(boolean, CAN_CODE) Can_IsTxMbFree(uint16 HwObjId)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint8  controller;
    uint16 mbId = CAN_TXMB_ID(HwObjId);
    boolean ret;

    controller = CAN_HWOBJ_CFG(HwObjId).CanControllerRef;
    canRegs = CAN_CNTRL_REG(controller);

    if((0U != (canRegs->Tflg & (uint8)(1U << mbId))) 
            && (0U == (Can_Cntrl[controller].TxPendMask & (uint8)(1U << mbId)))) 
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <Write PDU into mailbox to Transmit request>
 * Param-Name[in]      <mbId    - mailbox index>
 * Param-Name[in]      <PduInfo - pdu information>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_WriteMb(uint16 HwObjId, 
        P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_DATA) PduInfo)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint8  controller = 0U;
    uint16 mbId = 0U;
    uint8  i = 0U;

    controller = CAN_HWOBJ_CFG(HwObjId).CanControllerRef;
    canRegs = CAN_CNTRL_REG(controller);

    mbId = CAN_TXMB_ID(HwObjId);

    /* select Transmit Buffer index */
    canRegs->Tbsel = (uint8)(1U << mbId);

    /* Id */
    if(CAN_ID_TYPE_EXTENDED == CAN_HWOBJ_CFG(HwObjId).CanIdType)
    {
        canRegs->TxMb.Id = ((PduInfo->id & 0x1FFC0000UL) << 3U);
        canRegs->TxMb.Id |= ((PduInfo->id & 0x0003FFFFUL) << 1U);
        canRegs->TxMb.Id |= CAN_ID_IDE;
    }
    else if (CAN_ID_TYPE_STANDARD == CAN_HWOBJ_CFG(HwObjId).CanIdType)
    {
        canRegs->TxMb.Id = ((PduInfo->id & 0x000007FFUL) << 21U);
        canRegs->TxMb.Id &= (~CAN_ID_IDE);
    }
    else /* mixed type */
    {
        if (0x80000000U == (0x80000000U & PduInfo->id)) /* extended ID */
        {
            canRegs->TxMb.Id = ((PduInfo->id & 0x1FFC0000UL) << 3U);
            canRegs->TxMb.Id |= ((PduInfo->id & 0x0003FFFFUL) << 1U);
            canRegs->TxMb.Id |= CAN_ID_IDE;
        }
        else
        {
            canRegs->TxMb.Id = ((PduInfo->id & 0x000007FFUL) << 21U);
            canRegs->TxMb.Id &= (~CAN_ID_IDE);
        }
    }

    /* DLC */
    canRegs->TxMb.Dlc = PduInfo->length;

    /* SDU */
    for (i = 0U; i < PduInfo->length; i++)
    {
        /* MISRA RULE 17.4:491 VIOLATION: Array subscripting applied to an object pointer */
        canRegs->TxMb.Data[i] = PduInfo->sdu[i];
    }

    /* save pdu handle & CanId */
    Can_Cntrl[controller].TxPduHandles[mbId] = PduInfo->swPduHandle;
    Can_Cntrl[controller].TxCanId[mbId] = PduInfo->id;

    /* send request */
    Can_Cntrl[controller].TxPendMask |= (uint8)(1U << mbId);
    canRegs->Tflg = (uint8)(1U << mbId);

    /* Enable Tx interrupt */
    if (CAN_PROCESS_TYPE_INTERRUPT == CAN_CNTRL_PCCFG(controller).CanTxProcessing)
    {
        /* Turn on the interrupt mailboxes */
        Can_Cntrl[controller].SwIntTier |= (uint8)(1U << mbId);
        if (0U == Can_Cntrl[controller].IntLockCount)
        {
            canRegs->Tier = Can_Cntrl[controller].SwIntTier;
        }
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <get PDU from specific mailbox hardware>
 * Param-Name[in]      <mbId    - mailbox index>
 * Param-Name[in/out]  <pdu     - pdu>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_GetMBInfo(uint16 HwObjId, 
                                P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) pdu)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    P2VAR(volatile Can_MBRegType, AUTOMATIC, AUTOMATIC) mbRegs;
    uint8 i = 0U;
    uint32 canId = 0U;
    uint16 mbId  = 0U;

    canRegs = CAN_CNTRL_REG(CAN_HWOBJ_CFG(HwObjId).CanControllerRef);

    if (CAN_OBJECT_TYPE_TRANSMIT == CAN_HWOBJ_CFG(HwObjId).CanObjectType)
    {
        mbId = CAN_TXMB_ID(HwObjId);
        /* select Transmit Buffer index */
        canRegs->Tbsel = (uint8)(1U << mbId);
        mbRegs = &canRegs->TxMb;
    }  
    else
    {
        mbRegs = &canRegs->RxMb;
    }

    /* ID */
    canId = mbRegs->Id;
    if(CAN_ID_IDE == (canId & CAN_ID_IDE))   /* extended frame */
    {
        pdu->id = ((canId & 0xFFE00000UL) >> 3U) | ((canId & 0x0007FFFEUL) >> 1U);
        pdu->id |= 0x80000000U;   /* CanIf need extended Canid set 31 bit */
    }
    else /* standard frame */
    {
        pdu->id = (canId & 0xFFE00000UL) >> 21U; 
    }

    /* DLC */
    pdu->length = mbRegs->Dlc & 0xFU;

    /* SDU */
    for (i = 0U; i < pdu->length; i++) /* @req <CAN299> */
    {
        /* MISRA RULE 17.4:491 VIOLATION: Array subscripting applied to an object pointer */
        pdu->sdu[i] = mbRegs->Data[i];
    }

    return;
}

#if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
/******************************************************************************/
/*
 * Brief               <abort check >
 * Param-Name[in]      <Controller - controller id>
 * Param-Name[in/out]  <Config - Pointer to Controller Config>
 * Return              <mailbox index>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(void, CAN_CODE) Can_CheckAbortMb(uint8 Controller, uint8 Hth)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;
    uint16 hwObjId  = CAN_HWOBJ_ID(Hth);
    uint16 mbId = 0U;

    canRegs = CAN_CNTRL_REG(Controller);

    while ((hwObjId < CAN_HWOBJ_NUM) && (Hth == CAN_HWOBJ_CFG(hwObjId).CanObjectId))
    {
        mbId = CAN_TXMB_ID(hwObjId);

        if((1 << mbId) == (canRegs->Taak & (uint8)(1U << mbId)))
        {
            uint8  txData[8];
            Can_PduType pdu;

            pdu.sdu = txData;
            Can_GetMBInfo(hwObjId, &pdu);
            pdu.swPduHandle = Can_Cntrl[Controller].TxPduHandles[mbId];

            CanIf_CancelTxConfirmation(&pdu);
        }

        hwObjId++;
    }

    return;
}
#endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)&&(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */

#if ((STD_ON==CAN_HW_TRANSMIT_CANCELLATION)||(STD_ON==CAN_MULTIPLEXED_TRANSMISSION))
/******************************************************************************/
/*
 * Brief               <compare two canid priority>
 * Param-Name[in]      <destId - destination Can ID>
 * Param-Name[in]      <srcId  - source Can ID>
 * Return              <TRUE: destId higher than srcId, 
 *                      FALSE: destId not higher than srcId>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
STATIC FUNC(boolean, CAN_CODE) Can_PriorityHigher(Can_IdType destId, Can_IdType srcId)
{
    boolean ret = FALSE;

    /* clear bit 29, 30 */
    destId &= 0x9FFFFFFFU;
    srcId  &= 0x9FFFFFFFU;

    /* low 11 bit same, then compare high 21 bit */
    if ((destId & 0x7FFU) == (srcId & 0x7FFU)) 
    {
        /* compare high 21 bit */
        if ((destId & 0xFFFFF800U) < (srcId & 0xFFFFF800U))
        {
            ret = TRUE;    
        }
        else
        {
            ret = FALSE;
        }
    }
    else if ((destId & 0x7FFU) < (srcId & 0x7FFU))
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}
#endif /* (STD_ON==CAN_HW_TRANSMIT_CANCELLATION)||(STD_ON==CAN_MULTIPLEXED_TRANSMISSION) */

STATIC FUNC(void, CAN_CODE) Can_HwSetFilter(uint8 Controller, uint16 HwObjId)
{
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs = CAN_CNTRL_REG(Controller);
    uint16 mbId = CAN_RXMB_ID(HwObjId);
    uint32 rxId = 0;
    uint32 mask = 0;
    uint8  group  = 0;
    uint8  groupOffset = 0;

    /* Get Can ID & Filter Mask */
    if(CAN_ID_TYPE_EXTENDED == CAN_HWOBJ_CFG(HwObjId).CanIdType)
    {
        mask  = ((CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x1FFC0000UL) << 3U);
        mask |= ((CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x0003FFFFUL) << 1U);
        rxId  = ((CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x1FFC0000UL) << 3U);
        rxId |= ((CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x0003FFFFUL) << 1U);
        rxId |= CAN_ID_IDE | CAN_ID_SRR;
    }
    else if(CAN_ID_TYPE_STANDARD == CAN_HWOBJ_CFG(HwObjId).CanIdType)
    {
        mask = (CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x7FFU) << 21U;
        rxId = (CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x7FFU) << 21U;
    }
    else /* mixed type */
    {
        if (0x80000000U == (0x80000000U & CAN_HWOBJ_CFG(HwObjId).CanIdValue)) /* extended ID */
        {
            mask  = ((CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x1FFC0000UL) << 3U);
            mask |= ((CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x0003FFFFUL) << 1U);
            rxId  = ((CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x1FFC0000UL) << 3U);
            rxId |= ((CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x0003FFFFUL) << 1U);
            rxId |= CAN_ID_IDE | CAN_ID_SRR;
        }
        else /* standard ID */
        {
            mask = (CAN_HWOBJ_CFG(HwObjId).CanFilterMask & 0x7FFU) << 21U;
            rxId = (CAN_HWOBJ_CFG(HwObjId).CanIdValue & 0x7FFU) << 21U;
        }
    }
    
    /* 0: Match bit, 1: ignore bit */
    mask = ~mask;

    /* set to IDAR & IDMR */
    if (CAN_IDAC_FILTER_32BITS == CAN_CNTRL_CFG(Controller).CanFilterType)
    {
        group = (uint8)mbId;

        canRegs->FilterGrp[group].Idar[0U] = (uint8)(rxId >> 24U);
        canRegs->FilterGrp[group].Idar[1U] = (uint8)(rxId >> 16U);
        canRegs->FilterGrp[group].Idar[2U] = (uint8)(rxId >> 8U);
        canRegs->FilterGrp[group].Idar[3U] = (uint8)rxId;
        canRegs->FilterGrp[group].Idmr[0U] = (uint8)(mask >> 24U);
        canRegs->FilterGrp[group].Idmr[1U] = (uint8)(mask >> 16U);
        canRegs->FilterGrp[group].Idmr[2U] = (uint8)(mask >> 8U);
        canRegs->FilterGrp[group].Idmr[3U] = (uint8)mask;
    }
    else if (CAN_IDAC_FILTER_16BITS == CAN_CNTRL_CFG(Controller).CanFilterType)
    {
        group = (uint8)(mbId / 2U);
        groupOffset = (uint8)((mbId % 2U) * 2U);

        mask >>= 16U;
        rxId >>= 16U;
        canRegs->FilterGrp[group].Idar[groupOffset]   = (uint8)(rxId >> 8U);
        canRegs->FilterGrp[group].Idar[groupOffset + 1] = (uint8)rxId;
        canRegs->FilterGrp[group].Idmr[groupOffset]   = (uint8)(mask >> 8U);
        canRegs->FilterGrp[group].Idmr[groupOffset + 1] = (uint8)mask;
    }
    else if (CAN_IDAC_FILTER_08BITS == CAN_CNTRL_CFG(Controller).CanFilterType)
    {
        group = (uint8)(mbId / 4U);
        groupOffset = (uint8)(mbId % 4U);

        mask >>= 24U;
        rxId >>= 24U;
        canRegs->FilterGrp[group].Idar[groupOffset] = (uint8)rxId;
        canRegs->FilterGrp[group].Idmr[groupOffset] = (uint8)mask;
    }
    else
    {
        /* do nothing */
    }

    return;
}

STATIC FUNC(void, CAN_CODE) Can_HwClearAllMB(uint8 Controller)
{
    uint16 i = 0;
    P2VAR(volatile Can_CntrlRegType, AUTOMATIC, AUTOMATIC) canRegs;

    canRegs = CAN_CNTRL_REG(Controller);

    /* clear all tx mailbox */
    for (i = 0U; i < CAN_CNTRL_CFG(Controller).CanTxHwObjCount; i++)
    {
        Can_Cntrl[Controller].TxPendMask = 0U;
        if (0U != (canRegs->Tflg & (uint8)(1U << i)))
        {
            canRegs->Tarq = (uint8)(1U << i);
        }
    }

    /* clear rx fifo */
    while (CAN_RFLG_RXF == (canRegs->Rflg & CAN_RFLG_RXF))
    {
        canRegs->Rflg = CAN_RFLG_RXF;  /* clear IFALG */
    }

    return;
}
#define CAN_STOP_SEC_CODE
#include "Can_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/

