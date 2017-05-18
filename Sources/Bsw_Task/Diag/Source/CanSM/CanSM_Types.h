/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *
 *  @file       <CanSM_Types.h>
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
 *  V1.0.1      20140708        zheng fang                  add transceiver configuration
 */
/*============================================================================*/

#ifndef CANSM_TYPES_H
#define CANSM_TYPES_H

/* @req CANSM155 @req CANSM156 @req CANSM122 @req CANSM163 */
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANSM_TYPES_H_AR_MAJOR_VERSION   1U
#define CANSM_TYPES_H_AR_MINOR_VERSION   2U
#define CANSM_TYPES_H_AR_PATCH_VERSION   0U
#define CANSM_TYPES_H_SW_MAJOR_VERSION   1U
#define CANSM_TYPES_H_SW_MINOR_VERSION   0U
#define CANSM_TYPES_H_SW_PATCH_VERSION   1U

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "CanSM_Cfg.h"
#include "ComM.h"
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* @req CANSM127 */
typedef struct
{
    /* controller  pointer */
    P2CONST(uint8, CANSM_VAR, CANSM_CONST) CanSMControllerPtr;

    /* controller  count */
    uint8 CanSMControllerCnt;
}CanSM_ControllerType;

/* @req CANSM126 */
typedef struct
{
    /*
     * bus-off-counter value the bus-off recovery state machine
     * switches from level 1 to level 2,PC,PB,LT
     */
    /* @req CANSM131 */
    uint8 CanSMBorCounterL1ToL2;

    /*
     * bus-off-counter value the bus-off recovery state machine
     * shall report a failed production error state to the DEM.
     */
    /* @req CANSM132 */
    uint8 CanSMBorCounterL2Err;

    /* Bus-off  recovery  level 1 time,PC,PB,LT */
    /* @req CANSM128 */
    uint16 CanSMBorTimeL1;

    /* Bus-off  recovery  level 2 time,PC,PB,LT */
    uint16 CanSMBorTimeL2;

    /* @req CANSM129 */
    /* Bus-off  event  check duration  time,PC,PB,LT */
    /* @req CANSM130 */
    uint16 CanSMBorTimeTxEnsured;

    /* Can  network  handle */
    /* @req CANSM161 */
    NetworkHandleType CanSMNetworkHandle;

    #if (STD_ON == CANSM_USE_OSEKNM)
    /* NM notification */
    boolean CanSMNotifyNM;
    #endif /* STD_ON == CANSM_USE_OSEKNM */

	#if (STD_ON == CANSM_USE_TRANSCEIVER)
    /* Can  Transceiver Id */
    uint16 CanSMTransceiverId;
	#endif
}CanSM_NetworkType;

/* @req CANSM126 */
typedef struct
{
    #if (STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING)
	/*
	 * whether disables the deadline monitoring of RX PDU
	 * Groups during bus-off recovery to avoid timeouts,PC
	 */
    /* @req CANSM175 */
	boolean CanSMBorDisableRxDlMonitoring;
    #endif /* STD_ON == CANSM_BOR_DISABLE_RX_DL_MONITORING */

    #if (STD_ON == CANSM_BOR_TX_CONFIRMATION_POLLING)
	/*
	 * whether the CanSM polls the CanIf_GetTxConfigurationState
	 * API to decide the bus-off state to be recovered instead of
	 * using the CanSMBorTimeTxEnsured parameter for this decision. PC
	 */
	/* @req CANSM339 */
	boolean CanSMBorTxConfirmationPolling;
    #endif /* STD_ON == CANSM_BOR_TX_CONFIRMATION_POLLING */

	 /* the initialize parameter for the configured RX PDU group */
	 /* @req CANSM242 */
	 boolean CanSMRxPduInit;

	 /* the initialize parameter for the configured TX PDU group */
	 /* @req CANSM243 */
	 boolean CanSMTxPduInit;
}CanSM_PCNetworkType;

/* @req CANSM061 @req CANSM123 */
typedef struct
{
	/* the CAN network specific parameters of each CANnetwork */
	P2CONST(CanSM_NetworkType, CANSM_VAR, CANSM_CONST) CanSMNetworkPtr;

	/* the controller IDs assigned to a CAN network */
	P2CONST(CanSM_ControllerType, CANSM_VAR, CANSM_CONST) CanSMControllerPtr;
}CanSM_ConfigType;

/* @req CANSM169 */
typedef enum
{
    /* CanSM bus-off recovery state: idle */
    CANSM_BOR_IDLE,

    /* CanSM bus-off recovery state: check init */
    CANSM_BOR_CHECK_INIT,

    /* CanSM bus-off recovery state: no bus off */
    CANSM_BOR_NO_BUS_OFF,

    /* CanSM bus-off recovery state: tx off L1 */
    CANSM_BOR_TXOFF_L1,

    /* CanSM bus-off recovery state: check L1 */
    CANSM_BOR_CHECK_L1,

    /* CanSM bus-off recovery state: tx off L2 */
    CANSM_BOR_TXOFF_L2,

    /* CanSM bus-off recovery state: check L2 */
    CANSM_BOR_CHECK_L2
}CanSM_BusOffRecoveryStateType;

struct CanSM_ChannelTypeTag;
/* MISRA RULE 16.4£º1330 VIOLATION: because structure member function needs structure
 *                                  itself as input parameter, so the rule is violated*/
typedef void (*UDS_FAR CanSM_CbkFuncType)(struct CanSM_ChannelTypeTag *UDS_FAR pCh);

//typedef void P2VAR(CanSM_CbkFuncType, AUTOMATIC, AUTOMATIC)(P2VAR(struct CanSM_ChannelTypeTag, AUTOMATIC, AUTOMATIC) pCh);

typedef struct CanSM_ChannelTypeTag
{
	/* Timer or Polling main function */
    CanSM_CbkFuncType pBORMainFnc;

    /* bus-off recovery time out function */
    CanSM_CbkFuncType pBORTimeoutFnc;

    /* bus-off recovery function */
    CanSM_CbkFuncType pBusOffFnc;

    /* timer for bus-off recovery state machine */
    /* @req CANSM203 */
    uint32 timer;
    
    /* net work index */
    uint8 netId;

    /* counter for bus-off recovery state machine */
    /* @req CANSM057 */
    uint8 counter;

    /* flag for bus-off event */
    boolean busOffEvent;

    /* current communication mode of net work */
    ComM_ModeType curMode;

    /* bus-off recovery currnet state of net work */
    CanSM_BusOffRecoveryStateType borState;
}CanSM_RunTimeType;

#endif /* CANSM_TYPES_H */
/*=======[E N D   O F   F I L E]==============================================*/
