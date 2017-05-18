/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Can.h>
 *  @brief      <Can Registers define>
 *  
 *  <Compiler: CodeWarrior V5.1    MCU:MC9S12>
 *  
 *  @author     <bo.zeng>
 *  @date       <15-07-2013>
 */
/*============================================================================*/
#ifndef  CAN_REGS_H
#define  CAN_REGS_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0      20130924    bo.zeng    Initial version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CAN_REGS_H_AR_MAJOR_VERSION  2
#define CAN_REGS_H_AR_MINOR_VERSION  4
#define CAN_REGS_H_AR_PATCH_VERSION  0
#define CAN_REGS_H_SW_MAJOR_VERSION  1
#define CAN_REGS_H_SW_MINOR_VERSION  0
#define CAN_REGS_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"

/*=======[M A C R O S]========================================================*/
#define CAN_DATA_LENGTH     8

/*
 *  CTL0 Register
 */
#define CAN_CTL0_RXFRM      0x80U             /* Receive Frame Flag */
#define CAN_CTL0_RXACT      0x40U             /* Receive Active Status */
#define CAN_CTL0_CSWAI      0x20U             /* CAN Stops in Wait Mode */
#define CAN_CTL0_SYNCH      0x10U             /* Synchronized Status */
#define CAN_CTL0_TIME       0x08U             /* Timer Enable */
#define CAN_CTL0_WUPE       0x04U             /* Wake-Up Enable */
#define CAN_CTL0_SLPRQ      0x02U             /* Sleep Mode Request */
#define CAN_CTL0_INITRQ     0x01U             /* initialization Mode Request */

/*
 *  CTL1 Register
 */
#define CAN_CTL1_CANE       0x80U             /* MSCAN Enable */
#define CAN_CTL1_CLKSRC     0x40U             /* Clock Source */
#define CAN_CTL1_LOOPB      0x20U             /* Loopback Self Test Mode */
#define CAN_CTL1_LISTEN     0x10U             /* Listen Only Mode */
#define CAN_CTL1_BORM       0x08U             /* Bus-Off Recovery Mode*/
#define CAN_CTL1_WUPM       0x04U             /* Wake-Up Mode */
#define CAN_CTL1_SLPAK      0x02U             /* Sleep Mode Acknowledge */
#define CAN_CTL1_INITAK     0x01U             /* Initialization Mode Acknowledge */

/*
 *  RFLG Register
 */
#define CAN_RFLG_WUPIF      0x80U             /* Wake-up Interrupt Flag */
#define CAN_RFLG_CSCIF      0x40U             /* Can Status Change Interrupt Flag */
#define CAN_RFLG_RSTAT      0x30U             /* Receiver Status Bits */
#define CAN_RFLG_TSTAT      0x0CU             /* Transmitter Status Bits */
#define CAN_RFLG_OVRIF      0x02U             /* Overrun Interrupt Flag */
#define CAN_RFLG_RXF        0x01U             /* Receive Buffer Full Flag */

#define CAN_RFLG_RXWRN      0x10U
#define CAN_RFLG_RXERR      0x20U
#define CAN_RFLG_RXBUSOFF   0x30U

#define CAN_RFLG_TXWRN      0x04U
#define CAN_RFLG_TXERR      0x08U
#define CAN_RFLG_TXBUSOFF   0x0CU

/*
 *  RIER Register
 */
#define CAN_RIER_WUPIE      0x80U             /* Wake-up Interrupt Enable */
#define CAN_RIER_CSCIE      0x40U             /* Can Status Change Interrupt Enble */
#define CAN_RIER_RSTATE     0x30U             /* Receiver Status Change Enable */
#define CAN_RIER_TSTATE     0x0CU             /* Transmitter Status Change Enable */
#define CAN_RIER_OVRIE      0x02U             /* Overrun Interrupt Enable */
#define CAN_RIER_RXFIE      0x01U             /* Receive Buffer Full Enable */

/*
 *  TFLG Register
 */
#define CAN_TFLG_TXE        0x07U             /* Transmitter Buffer Empty */

/*
 *  TIER Register
 */
#define CAN_TIER_TXEIE      0x07U             /* Transmitter Empty Interrupt Enable */

/*
 *  TARQ Register
 */
#define CAN_TARQ_ARQ        0x07U             /* Abort Request */

/*
 *  TARQ Register
 */
#define CAN_TAAK_ABTAK      0x07U             /* Abort Acknowledge */

/*
 *  BSEL Register
 */
#define CAN_BSEL_TX         0x07U             /* Transmit Buffer Select */

/*
 *  IDAC Register
 */
#define CAN_IDAC_IDAM       0x30U             /* Identifier Acceptance Mode */
#define CAN_IDAC_IDHIT      0x07U             /* Identifier Acceptance Hit Identifier */

#define CAN_IDAC_FILTER_32BITS  0x0U
#define CAN_IDAC_FILTER_16BITS  0x10U
#define CAN_IDAC_FILTER_08BITS  0x20U
#define CAN_IDAC_FILTER_CLOSE   0x30U

/*
 *  MISC Register
 */
#define CAN_MISC_BOHOLD     0x01U             /* Bus-off State Hold Until User Request */

/*
 * Message Buffer
 */
#define CAN_ID_SRR          0x00100000U       /* Substitute Remote Request */
#define CAN_ID_IDE          0x00080000U       /* Substitute Remote Request */
#define CAN_ID_RTR          0x00000001U       /* Remote Transmission Request */

typedef struct 
{
    uint32  Id;
    uint8   Data[CAN_DATA_LENGTH];
    uint8   Dlc;
    uint8   Tbpr;
    uint8   Tsrh;
    uint8   Tsrl;
} Can_MBRegType;

typedef struct
{
    uint8 Idar[4U];
    uint8 Idmr[4U];
} Can_FilterGrpRegType;

typedef struct 
{
    uint8 Ctl0;
    uint8 Ctl1;
    uint8 Btr0;
    uint8 Btr1;
    uint8 Rflg;
    uint8 Rier;
    uint8 Tflg;
    uint8 Tier;
    uint8 Tarq;
    uint8 Taak;
    uint8 Tbsel;
    uint8 Idac;
    uint8 Reserved[1U];
    uint8 Misc;
    uint8 Rxerr;
    uint8 Txerr;
    Can_FilterGrpRegType FilterGrp[2U];
    Can_MBRegType RxMb;
    Can_MBRegType TxMb;
} Can_CntrlRegType;

#endif  /* #ifndef  CAN_REGS_H */

/*=======[E N D   O F   F I L E]==============================================*/



